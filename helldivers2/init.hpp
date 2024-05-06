#pragma once

#include <Windows.h>
#include <fstream>
#include <string>
#include "config.h"
#include "include/FileManager.h"
#include "include/helper.h"
#include "include/util.h"
#include "include/Game.hpp"
#include "include/D3D11Window.hpp"
#include "include/Hooking.hpp"
#include "funchook.h"
#include "bypass.h"

using namespace DX11_Base;
using namespace GameHooking;


void OnHook(LPVOID lpParam)
{
#ifdef VMPROTECT
    VMProtectBeginUltra("OnHook");
    if (!VMProtectIsProtected())
    {
        FreeLibraryAndExitThread(g_hModule, EXIT_SUCCESS);
        return;
    }
#endif

#if defined(DEBUG_CONSOLE) || defined(GAME_LOGGING)
    AllocConsole();
    FILE* pFile = nullptr;
    freopen_s(&pFile, "CONOUT$", "w", stdout);
#endif

    //Init file manager
    g_FileManager.Init();

    //Bypass
    g_Bypass.Patch();

    int nDelay = g_FileManager.Config["DLLDelay"].is_number_integer() ? g_FileManager.Config["DLLDelay"].get<int>() : 0;
    if (nDelay < 0)
    {
        nDelay = 0;
    }

#ifdef DEBUG_CONSOLE
    printf("[DLLDelay] Delay: %d\n", nDelay);
#endif

    Sleep(nDelay);

    //Mutex lock
    const wchar_t szUniqueNamedMutex[] = L"Helldivers2_MutexLock";
    HANDLE hHandleMutex = CreateMutex(NULL, TRUE, szUniqueNamedMutex);
    if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        exit(1);
        return;
    }

    //Manually inject game.dll
    if (!GetModuleHandleA("game.dll"))
    {
        //mrleepark
        std::string strGameDllPath = wstring_to_utf8(g_FileManager.ProcessPath) + "\\data\\game\\game.dll";

        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
        LPVOID pDllPath = VirtualAllocEx(hProcess, nullptr, strlen(strGameDllPath.c_str()) + 1, MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(hProcess, pDllPath, (LPVOID)strGameDllPath.c_str(), strlen(strGameDllPath.c_str()) + 1, nullptr);
        HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), pDllPath, 0, 0);
        WaitForSingleObject(hLoadThread, INFINITE);
        VirtualFreeEx(hProcess, pDllPath, strlen(strGameDllPath.c_str()) + 1, MEM_RELEASE);

        CloseHandle(hLoadThread);
        CloseHandle(hProcess);
        Sleep(500);
    }

    //HOOK GAME FUNCTIONS
    FuncHook.Init();

#ifdef DEBUG_CONSOLE
    printf("[FuncHook] Initialized\n");
#endif


    //Create Trampoline
    Features.Alloc();


    //ESTABLISH GAME DATA   
    g_hGameModule = GetModuleHandleA("game.dll");
    g_GameData = std::make_unique<GameData>();
    g_GameVariables = std::make_unique<GameVariables>();

#ifdef DEBUG_CONSOLE
    printf("[GameData] Initialized\n");
#endif


    //CREATE WINDOW AND ESTABLISH HOOKS
    g_D3D11Window = std::make_unique<D3D11Window>();
    g_Hooking = std::make_unique<Hooking>();
    g_Menu = std::make_unique<Menu>();
    g_Hooking->Hook();

#ifdef DEBUG_CONSOLE
    printf("[DirectX] Initialized\n");
#endif

    ///RENDER LOOP
    int nMenuKeybind = g_FileManager.Config["MenuKeybind"].is_number_integer() ? g_FileManager.Config["MenuKeybind"].get<int>() : VK_DELETE;
    g_Running = TRUE;
    while (g_Running)
    {
        if (GetAsyncKeyState(nMenuKeybind) & 1)
        {
            g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;
            g_GameVariables->m_ShowHud = !g_GameVariables->m_ShowMenu;
#ifdef DEBUG_CONSOLE
            printf("[Keypress] %s\n", g_GameVariables->m_ShowMenu ? "Show Menu" : "Hide Menu");
#endif
        }
    }

	CloseHandle(hHandleMutex);

#ifdef VMPROTECT
    VMProtectEnd();
#endif

    //EXIT
    FreeLibraryAndExitThread(g_hModule, EXIT_SUCCESS);
}