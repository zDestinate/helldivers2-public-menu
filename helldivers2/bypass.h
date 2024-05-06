#pragma once
#include <Windows.h>

class bypass
{
private:
	void PatchBytes();

public:
	bypass();

	bool bBypassed = false;
	void Patch();
};
extern bypass g_Bypass;