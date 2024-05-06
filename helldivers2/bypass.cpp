#include "pch.h"
#include "bypass.h"
#include "config.h"
#include "include/Process.h"
#include "include/FileManager.h"
#include "memory.h"
#include <string>
#include <fstream>
#include "libs/xorstr.hpp"

using namespace std;

bypass g_Bypass;

bypass::bypass()
{
	Process::g_hHeap = HeapCreate(0, 0, 0);
}

void bypass::Patch()
{
	if (g_FileManager.ConfigPath.empty())
	{
		exit(1);
		return;
	}

	//Check if exe loaded completely
	uintptr_t WinMainAddress = 0;
	int nCounter = 0;
	do
	{
		if (nCounter > 50)
		{
			exit(1);
		}

		WinMainAddress = Memory::FindPattern64("helldivers2.exe", "40 53 48 83 EC ? E8 ? ? ? ? 8B D8 3D");
		Sleep(200);
		nCounter++;
	} while (!WinMainAddress);


	//Check if gameguard file exist
	wstring GameGuardPath = g_FileManager.ConfigPath + L"\\bin\\GameGuard.des";
	if (ifstream(GameGuardPath))
	{
#ifdef DEBUG_CONSOLE
		printf("[Bypass] GameGuard file found. Skipping bypass\n");
#endif
		return;
	}

	PatchBytes();
}

/*
	This is from Erik9631
	The bypass here is basically prevent GG from booting it up and return a success code.
	However, GG has some checks that need to be run for smooth gameplay.
	If you are using this bypass, you will experience lag related to checks that aren't running.
	For instance, when you pick up medal or super credit, you will freeze for about 3 seconds.
	
	If you want a better bypass, use cfemen bypass instead.
*/
void bypass::PatchBytes()
{
#ifdef VMPROTECT
	VMProtectBeginUltra("bypass::PatchBytes");
	if (!VMProtectIsProtected())
	{
		return;
	}
#endif

	return;

	/*
	FROZEN_THREADS threads;
	Process::Suspend(&threads);

	uintptr_t OnLoadAddress1 = Memory::FindPattern64("helldivers2.exe", _XOR_("XXXXXXXXXXX"));
	uintptr_t OnLoadAddress2 = Memory::FindPattern64("helldivers2.exe", _XOR_("XXXXXXXXXXX"));

	if (!OnLoadAddress1 || !OnLoadAddress2)
	{
#ifdef DEBUG_CONSOLE
		printf("[Bypass] Address not found\n");
#endif
		Process::Resume(&threads);
		return;
	}

    BYTE BypassBytes[] =
    {
		0xB8, 0x55, 0x07, 0x00, 0x00,	// mov eax, 0x755 { 1877 }
		0xC3							// ret
    };

	Memory::Patch64((LPVOID)OnLoadAddress1, BypassBytes);
	Memory::Patch64((LPVOID)OnLoadAddress2, BypassBytes);
	bBypassed = true;

#ifdef DEBUG_CONSOLE
	printf("[Bypass] Successfully patched\n");
#endif

	Process::Resume(&threads);
	*/

#ifdef VMPROTECT
	VMProtectEnd();
#endif
}