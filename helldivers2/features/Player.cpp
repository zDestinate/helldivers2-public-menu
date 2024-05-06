#include "../features.h"

using namespace std;


void features::SetInfAmmo()
{
    string key_main = "InfAmmo_Main";
    string key_sickle = "InfAmmo_Sickle";

    auto it = FeatureList.find(key_main);
    FeatureData& TempData_main = it->second;
    auto it_sickle = FeatureList.find(key_sickle);
    FeatureData& TempData_sickle = it_sickle->second;

    uintptr_t TempAddress_main = TempData_main.Address;
    uintptr_t TempAddress_sickle = TempData_sickle.Address;

    if (!TempAddress_main || !TempAddress_sickle)
    {
        Config.bInfAmmo = TempData_main.State;
        return;
    }

    if (!TempData_main.bInitialized)
    {
        BYTE AmmoBytes[] =
        {
            0x42, 0x83, 0x3C, 0xC1, 0x02,	// cmp dword ptr [rcx+r8*8],02 { 2 }
            0x7E, 0x05,	                    // jle 05
            0x42, 0x83, 0x2C, 0xC1, 0x01,	// sub dword ptr [rcx+r8*8],01 { 1 }
            0x48, 0x8B, 0x45, 0x38,	        // mov rax,[rbp+38]
            0x0F, 0x94, 0xC3,	            // sete bl
            0x48, 0x8B, 0x0C, 0xD0	        // mov rcx,[rax+rdx*8]
        };

        uintptr_t TempAllocAddress_main = (uintptr_t)TempData_main.AllocMemoryAddress + 2;
        TempData_main.JmpBytes = Memory::ArrayLength(AmmoBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress_main, AmmoBytes);



        BYTE Sickle_Memory[] =
        {
            0x42, 0x83, 0x3C, 0x01, 0x01,	// cmp dword ptr [rcx+r8],01 { 1 }
            0x7E, 0x05,	                    // jle 05
            0x42, 0x83, 0x2C, 0x01, 0x01,	// sub dword ptr [rcx+r8],01 { 1 }
            0x49, 0x8B, 0x46, 0x40,	        // mov rax,[r14+40]
            0x0F, 0x94, 0xC3,	            // sete bl
            0x4D, 0x03, 0x46, 0x58	        // add r8,[r14+58]
        };

        uintptr_t TempAllocAddress_sickle = (uintptr_t)TempData_sickle.AllocMemoryAddress + 2;
        TempData_sickle.JmpBytes = Memory::ArrayLength(Sickle_Memory) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress_sickle, Sickle_Memory);

        TempData_main.bInitialized = true;
    }

    if (Config.bInfAmmo != TempData_main.State)
    {
        TempData_main.State = Config.bInfAmmo;

        uintptr_t AllocAddress_main = (uintptr_t)TempData_main.AllocMemoryAddress;
        uintptr_t AllocAddress_sickle = (uintptr_t)TempData_sickle.AllocMemoryAddress;

        if (Config.bInfAmmo)
        {
            BYTE AmmoPatch[] = { 0x00 };

            if (TempData_main.OriginalBytes == nullptr)
            {
                TempData_main.OriginalBytes = Memory::Patch64((LPVOID)(AllocAddress_main + 2 + TempData_main.JmpBytes + 4), AmmoPatch, true);
                TempData_main.OriginalBytesSize = 1;

                TempData_sickle.OriginalBytes = Memory::Patch64((LPVOID)(AllocAddress_sickle + 2 + TempData_sickle.JmpBytes + 4), AmmoPatch, true);
                TempData_sickle.OriginalBytesSize = 1;
                return;
            }

            Memory::Patch64((LPVOID)(AllocAddress_main + 2 + TempData_main.JmpBytes + 4), AmmoPatch);
            Memory::Patch64((LPVOID)(AllocAddress_sickle + 2 + TempData_sickle.JmpBytes + 4), AmmoPatch);
            return;
        }
        else
        {
            BYTE AmmoPatch[] = { 0x01 };

            if (TempData_main.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress_main + 2 + TempData_main.JmpBytes + 4), TempData_main.OriginalBytes, TempData_main.OriginalBytesSize);
                Memory::Patch64((LPVOID)(AllocAddress_sickle + 2 + TempData_sickle.JmpBytes + 4), TempData_sickle.OriginalBytes, TempData_sickle.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetInfAmmo_Legit()
{
    string key_main = "InfAmmo_Main";
    string key_sickle = "InfAmmo_Sickle";

    auto it = FeatureList.find(key_main);
    FeatureData& TempData_main = it->second;
    auto it_sickle = FeatureList.find(key_sickle);
    FeatureData& TempData_sickle = it_sickle->second;

    uintptr_t TempAddress_main = TempData_main.Address;
    uintptr_t TempAddress_sickle = TempData_sickle.Address;

    if (!TempAddress_main)
    {
        Config.bInfAmmo_Legit = TempData_main.State;
        return;
    }

    if (Config.bInfAmmo_Legit != TempData_main.State)
    {
        TempData_main.State = Config.bInfAmmo_Legit;

        uintptr_t AllocAddress_main = (uintptr_t)TempData_main.AllocMemoryAddress;
        uintptr_t AllocAddress_sickle = (uintptr_t)TempData_sickle.AllocMemoryAddress;

        if (Config.bInfAmmo_Legit)
        {
            BYTE AmmoPatch[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress_main), AmmoPatch);
            Memory::Patch64((LPVOID)(AllocAddress_sickle), AmmoPatch);
            return;
        }
        else
        {
            BYTE AmmoPatch[] = { 0xEB, TempData_main.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress_main), AmmoPatch);

            BYTE AmmoPatch_sickle[] = { 0xEB, TempData_sickle.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress_sickle), AmmoPatch_sickle);
            return;
        }
    }
}

void features::SetInfGrenade()
{
    string key = "InfGrenade";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bInfGrenade = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        BYTE GrenadeBytes[] =
        {
            0x42, 0x83, 0x3C, 0x30, 0x01,	// cmp dword ptr [rax+r14],01 { 1 }
            0x74, 0x04,	                    // je 04
            0x42, 0xFF, 0x0C, 0x30,	        // dec [rax+r14]
            0x48, 0x8B, 0x45, 0x40,	        // mov rax,[rbp+40]
            0x4C, 0x8B, 0x45, 0x58,	        // mov r8,[rbp+58]
            0x4D, 0x01, 0xF0,	            // add r8,r14
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(GrenadeBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress, GrenadeBytes);

        TempData.bInitialized = true;
        return;
    }

    if (Config.bInfGrenade != TempData.State)
    {
        TempData.State = Config.bInfGrenade;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bInfGrenade)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress + 2 + TempData.JmpBytes), 4, true);
                TempData.OriginalBytesSize = 4;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress + 2 + TempData.JmpBytes), 4);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress + 2 + TempData.JmpBytes), TempData.OriginalBytes, TempData.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetInfGrenade_Legit()
{
    string key = "InfGrenade";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bInfGrenade_Legit = TempData.State;
        return;
    }

    if (Config.bInfGrenade_Legit != TempData.State)
    {
        TempData.State = Config.bInfGrenade_Legit;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bInfGrenade_Legit)
        {
            BYTE AmmoPatch[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress), AmmoPatch);
            return;
        }
        else
        {
            BYTE AmmoPatch[] = { 0xEB, TempData.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress), AmmoPatch);
            return;
        }
    }
}

void features::SetInfStim()
{
    string key = "InfStim";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bInfStim = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        BYTE StimBytes[] =
        {
            0x4B, 0x8D, 0x0C, 0x40,	        // lea rcx,[r8+r8*2]
            0x48, 0x01, 0xC9,	            // add rcx,rcx
            0x45, 0x8B, 0x74, 0xCF, 0x18,	// mov r14d,[r15+rcx*8+18]
            0x41, 0x83, 0xFE, 0x01,	        // cmp r14d,01 { 1 }
            0x74, 0x03,	                    // je 03
            0x41, 0xFF, 0xCE	            // dec r14d
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(StimBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress, StimBytes);

        TempData.bInitialized = true;
    }

    if (Config.bInfStim != TempData.State)
    {
        TempData.State = Config.bInfStim;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bInfStim)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress + 2 + TempData.JmpBytes + 12), 3, true);
                TempData.OriginalBytesSize = 3;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress + 2 + TempData.JmpBytes + 12), 3);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress + 2 + TempData.JmpBytes + 12), TempData.OriginalBytes, TempData.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetInfStim_Legit()
{
    string key = "InfStim";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bInfStim_Legit = TempData.State;
        return;
    }

    if (Config.bInfStim_Legit != TempData.State)
    {
        TempData.State = Config.bInfStim_Legit;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bInfStim_Legit)
        {
            BYTE AmmoPatch[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress), AmmoPatch);
            return;
        }
        else
        {
            BYTE AmmoPatch[] = { 0xEB, TempData.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress), AmmoPatch);
            return;
        }
    }
}

void features::SetInfBackpack()
{
    string key = "InfBackpack";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bInfBackpack = TempData.State;
        return;
    }

    if (Config.bInfBackpack != TempData.State)
    {
        TempData.State = Config.bInfBackpack;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bInfBackpack)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress), 2, true);
                TempData.OriginalBytesSize = 2;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress), 2);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress), TempData.OriginalBytes, TempData.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetInstantStratagemDrop()
{
    string key = "InstantStratagemDrop";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bInstantStratagemDrop = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        BYTE InstantStratagemDropBytes[] =
        {
            0x52,	                            // push rdx
            0x66, 0x0F, 0x7E, 0xC2,	            // movd edx,xmm0
            0x81, 0xFA, 0x00, 0x00, 0x00, 0x3F,	// cmp edx,3F000000 { 0.50 }
            0x7E, 0x09,	                        // jle 00830016
            0xBA, 0x00, 0x00, 0x00, 0x3F,	    // mov edx,3F000000 { 0.50 }
            0x66, 0x0F, 0x6E, 0xC2,	            // movd xmm0,edx
            0x5A,	                            // pop rdx
            0xF3, 0x42, 0x0F, 0x11, 0x04, 0xF0,	// movss [rax+r14*8],xmm0
            0x49, 0x8B, 0x47, 0x78,	            // mov rax,[r15+78]
            0x4E, 0x8D, 0x04, 0xF0	            // lea r8,[rax+r14*8]
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(InstantStratagemDropBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress, InstantStratagemDropBytes);

        TempData.bInitialized = true;
        return;
    }

    if (Config.bInstantStratagemDrop != TempData.State)
    {
        TempData.State = Config.bInstantStratagemDrop;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bInstantStratagemDrop)
        {
            BYTE InstantStratagemDropBytes[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress), InstantStratagemDropBytes);
            return;
        }
        else
        {
            BYTE InstantStratagemDropBytes[] = { 0xEB, TempData.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress), InstantStratagemDropBytes);
            return;
        }
    }
}

void features::SetCombinationAlwaysCorrect()
{
    string key = "CombinationAlwaysCorrect";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bCombinationAlwaysCorrect = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        uintptr_t JneAddress = (TempAddress + 6) + *(uint8_t*)(TempAddress + 6) + 1;
        BYTE JneBytes[8] = { 0x00 };
        memcpy(JneBytes, &JneAddress, sizeof(uintptr_t));

        uintptr_t JmpAddress = (TempAddress + 14) + *(uint8_t*)(TempAddress + 14) + 1;
        BYTE JmpBytes[8] = { 0x00 };
        memcpy(JmpBytes, &JmpAddress, sizeof(uintptr_t));

        BYTE CombinationBytes[] =
        {
            0x42, 0x3B, 0x4C, 0xAE, 0x04,	    // cmp ecx,[rsi+r13*4+04]
            0x75, 0x02,                         // jne 02
            0xEB, 0x0E,	                        // jmp 0E
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp game.dll+5D8FB8
            JneBytes[0], JneBytes[1], JneBytes[2], JneBytes[3], JneBytes[4], JneBytes[5], JneBytes[6], JneBytes[7],
            0x41, 0x8D, 0x45, 0x01,	            // lea eax,[r13+01]
            0x89, 0x06,	                        // mov [rsi],eax
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp game.dll+5D8FC4
            JmpBytes[0], JmpBytes[1], JmpBytes[2], JmpBytes[3], JmpBytes[4], JmpBytes[5], JmpBytes[6], JmpBytes[7]
        };

        Memory::Patch64(TempData.AllocMemoryAddress, CombinationBytes);
        Memory::CreateJmpFar(TempAddress, (uintptr_t)(TempData.AllocMemoryAddress));

        TempData.bInitialized = true;
        return;
    }

    if (Config.bCombinationAlwaysCorrect != TempData.State)
    {
        TempData.State = Config.bCombinationAlwaysCorrect;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bCombinationAlwaysCorrect)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress + 5), 2, true);
                TempData.OriginalBytesSize = 2;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress + 5), 2);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress + 5), TempData.OriginalBytes, TempData.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetInstantStandupFromFall()
{
    string key = "InstantStandupFromFall";
    CheckFeatureExist(key);

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    if (Config.bInstantStandupFromFall != TempData.State)
    {
        uintptr_t TempAddress = GetAddress(key, "43 3A ? ? ? ? ? ? 75 ? 84");
        if (!TempAddress)
        {
            Config.bInstantStandupFromFall = TempData.State;
            return;
        }

        TempData.State = Config.bInstantStandupFromFall;

        if (Config.bInstantStandupFromFall)
        {
            uintptr_t Jmp1 = TempAddress + 0x19;
            BYTE Jmp1Bytes[8] = { 0x0 };
            memcpy(Jmp1Bytes, &Jmp1, sizeof(uintptr_t));

            uintptr_t Jmp2 = TempAddress + 0x31;
            BYTE Jmp2Bytes[8] = { 0x0 };
            memcpy(Jmp2Bytes, &Jmp1, sizeof(uintptr_t));


            BYTE InstantStandupBytes[] =
            {
                0x43, 0xC7, 0x84, 0x3C, 0x8C, 0xD6, 0x44, 0x00,                                     // mov [r12+r15+0044D68C],00000000 { 0 }
                0x00, 0x00, 0x00, 0x00,
                0x43, 0x3A, 0x8C, 0x3C, 0x8C, 0xD6, 0x44, 0x00,                                     // cmp cl,[r12+r15+0044D68C]
                0x75, 0x02,                                                                         // jne 02
                0xEB, 0x0E,                                                                         // jmp 0E
                0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                                                 // jmp far game.dll+64E52F
                Jmp1Bytes[0], Jmp1Bytes[1], Jmp1Bytes[2], Jmp1Bytes[3], Jmp1Bytes[4], Jmp1Bytes[5], Jmp1Bytes[6], Jmp1Bytes[7],
                0x84, 0xC0,                                                                         // test al,al
                0x74, 0x02,                                                                         // je 02
                0xEB, 0x0E,                                                                         // jmp 0E
                0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                                                 // jmp far game.dll+64E548
                Jmp2Bytes[0], Jmp2Bytes[1], Jmp2Bytes[2], Jmp2Bytes[3], Jmp2Bytes[4], Jmp2Bytes[5], Jmp2Bytes[6], Jmp2Bytes[7]
            };

            if (TempData.OriginalBytes == nullptr)
            {
                LPVOID memory = Memory::AllocateMemory64(TempAddress, sizeof(InstantStandupBytes));
                TempData.AllocMemoryAddress = memory;

                TempData.OriginalBytes = Memory::PatchJmp64(TempAddress, memory, InstantStandupBytes, true);
                TempData.OriginalBytesSize = 14;
                return;
            }

            LPVOID memory = TempData.AllocMemoryAddress;
            Memory::PatchJmp64(TempAddress, memory, InstantStandupBytes, false);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(TempAddress), TempData.OriginalBytes, TempData.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetTakeNoDamage()
{
    string key = "PlayerDamage";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bTakeNoDamage = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        BYTE TakeNoDamageBytes[] =
        {
            0x4D, 0x85, 0xC0,	    // test r8,r8
            0x75, 0x04,	            // jne 04
            0x42, 0x89, 0x2C, 0x80,	// mov [rax+r8*4],ebp
            0x48, 0x8B, 0x46, 0x50,	// mov rax,[rsi+50]
            0x4E, 0x8D, 0x04, 0x80,	// lea r8,[rax+r8*4]
            0x48, 0x8B, 0x46, 0x38,	// mov rax,[rsi+38]
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(TakeNoDamageBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress, TakeNoDamageBytes);

        TempData.bInitialized = true;
    }

    if (Config.bTakeNoDamage != TempData.State)
    {
        TempData.State = Config.bTakeNoDamage;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bTakeNoDamage)
        {
            BYTE TakeNoDamageBytes[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress), TakeNoDamageBytes);
            return;
        }
        else
        {
            BYTE TakeNoDamageBytes[] = { 0xEB, TempData.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress), TakeNoDamageBytes);
            return;
        }
    }
}

void features::SetFreezePlayerHealth()
{
    string key = "FreezePlayerHealth";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.nPlayerMinHealth = TempData.nValue;
        return;
    }

    if (!TempData.bInitialized)
    {
        TempData.nValue = Config.nPlayerMinHealth;

        BYTE IntBytes[4] = { 0x0 };
        memcpy(IntBytes, &TempData.nValue, sizeof(int));

        BYTE PatchBytes[] =
        {
            0x50,	                                                        // push rax
            0x48, 0x8B, 0x43, 0x50,	                                        // mov rax,[rbx+50]
            0xC7, 0x00, IntBytes[0], IntBytes[1], IntBytes[2], IntBytes[3],	// mov [rax],0000270F { 9999 }
            0x58,	                                                        // pop rax
            0x44, 0x8B, 0x53, 0x28,	                                        // mov r10d,[rbx+28]
            0x8B, 0x7B, 0x30,	                                            // mov edi,[rbx+30]
            0x0F, 0xAF, 0xFA,	                                            // imul edi,edx
            0x45, 0x8D, 0x7A, 0xFF	                                        // lea r15d,[r10-01]
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(PatchBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress, PatchBytes);

        TempData.bInitialized = true;
        return;
    }

    if (Config.nPlayerMinHealth != TempData.nValue || !TempData.State)
    {
        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.nPlayerMinHealth > 0)
        {
            TempData.nValue = Config.nPlayerMinHealth;
            TempData.State = true;

            *(int*)(AllocAddress + 2 + 7) = TempData.nValue;

            BYTE PatchBytes[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
            return;
        }
    }
}

void features::SetUnfreezePlayerHealth()
{
    string key = "FreezePlayerHealth";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.nPlayerMinHealth = TempData.nValue;
        return;
    }

    if (!TempData.bInitialized)
    {
        Config.nPlayerMinHealth = TempData.nValue;
        return;
    }

    if (TempData.State == true)
    {
        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;
        TempData.State = false;

        BYTE PatchBytes[] = { 0xEB, TempData.JmpBytes };
        Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
        return;
    }
}

void features::SetPlayerSpeed()
{
    string key = "PlayerSpeed";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.fPlayerSpeed = TempData.fValue;
        return;
    }

    if (!TempData.bInitialized)
    {
        TempData.fValue = Config.fPlayerSpeed;
        BYTE FloatBytes[4] = { 0x0 };
        memcpy(FloatBytes, &TempData.fValue, sizeof(float));

        BYTE PlayerSpeedBytes[] =
        {
            0x53,                                                                                   // push rbx
            0x48, 0xBB, 0xAB, 0x26, 0xF9, 0xF2, 0xAB, 0x26, 0xF9, 0xF2,                             // mov rbx,F2F926ABF2F926AB { -218552661 }
            0x48, 0x39, 0x18,                                                                       // cmp [rax],rbx
            0x75, 0x08,                                                                             // jne short 0x8
            0x41, 0xC7, 0x46, 0x0C, FloatBytes[0], FloatBytes[1], FloatBytes[2], FloatBytes[3],     // mov [r14+0C], float { Value }
            0x5B,                                                                                   // pop rbx
            0xF3, 0x41, 0x0F, 0x59, 0x56, 0x0C,                                                     // mulss xmm2,[r14+0C]
            0xF3, 0x41, 0x0F, 0x59, 0x56, 0x10,                                                     // mulss xmm2,[r14+10]
            0x0F, 0x28, 0xE2                                                                        // movaps xmm4,xmm2
        };

        Memory::Patch64(TempData.AllocMemoryAddress, PlayerSpeedBytes);
        Memory::CreateJmpFar(TempAddress, (uintptr_t)(TempData.AllocMemoryAddress));

        TempData.bInitialized = true;
    }

    if (Config.fPlayerSpeed != TempData.fValue)
    {
        TempData.fValue = Config.fPlayerSpeed;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;
        *(float*)(AllocAddress + 20) = TempData.fValue;

        return;
    }
}

void features::PlayerTeleport()
{
    if (!Config.pPlayerLocationAddress)
    {
        return;
    }

    Vector3D_Data* LocationData = (Vector3D_Data*)Config.pPlayerLocationAddress;
    LocationData->X = Config.fPlayerLocationX;
    LocationData->Y = Config.fPlayerLocationY;
    LocationData->Z = Config.fPlayerLocationZ;
}

void features::PlayerCopyLocation()
{
    if (!Config.pPlayerLocationAddress)
    {
        return;
    }

    Vector3D_Data* LocationData = (Vector3D_Data*)Config.pPlayerLocationAddress;
    Config.fPlayerLocationX = LocationData->X;
    Config.fPlayerLocationY = LocationData->Y;
    Config.fPlayerLocationZ = LocationData->Z;
}

void features::PlayerTeleportWaypoint()
{
    string key = "TeleportWaypoint";
    CheckFeatureExist(key);

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = GetAddress(key, "4C 8B 3D ? ? ? ? 45 33 F6 49 8B F1");
    if (!TempAddress)
    {
        return;
    }

    if (!Config.pPlayerLocationAddress)
    {
        return;
    }

    unsigned int nRIPOffset;
    memcpy(&nRIPOffset, (LPVOID)(TempAddress + 3), 4);
    TempAddress += nRIPOffset + 7;

    //F3 42 0F 10 44 39
    uintptr_t WaypointAddress = *(uintptr_t*)(TempAddress) + (96 * 0x0C500) + 56;
    Vector3D_Data* WaypointLocation = (Vector3D_Data*)WaypointAddress;

    Vector3D_Data* PlayerLocation = (Vector3D_Data*)Config.pPlayerLocationAddress;
    PlayerLocation->X = WaypointLocation->X;
    PlayerLocation->Z = WaypointLocation->Z;
}