#pragma once
#include "pch.h"
#include "init.hpp"
#include "libs/UPD/UniversalProxyDLL.h"

using namespace DX11_Base;


BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
    g_hModule = hModule;

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);

        UPD::CreateProxy(hModule);

        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&OnHook, hModule, 0, nullptr);
        break;

    case DLL_PROCESS_DETACH:
        FuncHook.Detach();
        g_KillSwitch = TRUE;
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}