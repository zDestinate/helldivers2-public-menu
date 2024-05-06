// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// STANDARD LIBRARIES
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

// DIRECTX
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

/*
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include <dxgi1_4.h>
#pragma comment(lib, "dxgi.lib")
*/


// MINHOOK
#include "libs/MinHook/MinHook.h"

#ifdef VMPROTECT
	#include "libs/VMProtect/VMProtectSDK.h"
#endif

#endif //PCH_H