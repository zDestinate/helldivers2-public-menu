#include "../pch.h"
#include "../include/Hooking.hpp"
#include "../config.h"

namespace DX11_Base {
	Hooking::Hooking()
	{
		MH_Initialize();

		return;
	}

	Hooking::~Hooking()
	{
		MH_RemoveHook(MH_ALL_HOOKS);
	}

	void Hooking::Hook()
	{
		g_GameVariables->Init();
		g_D3D11Window->Hook();
		Config.Init();
		MH_EnableHook(MH_ALL_HOOKS);
		return;
	}

	void Hooking::Unhook()
	{
		g_D3D11Window->Unhook();
		g_Running = FALSE;
		return;
	}
}