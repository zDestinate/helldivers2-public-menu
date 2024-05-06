#include "../features.h"
#include "../libs/xorstr.hpp"

using namespace std;


void features::SetNoReload()
{
    string key = "NoReload";
    //string key_2 = "NoReload_2";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;
    //auto it2 = FeatureList.find(key);
    //FeatureData& TempData2 = it2->second;

    uintptr_t TempAddress = TempData.Address;
    //uintptr_t TempAddress2 = TempData2.Address;

    if (!TempAddress)
    {
        Config.bNoReload = TempData.State;
        return;
    }

    if (Config.bNoReload != TempData.State)
    {
        TempData.State = Config.bNoReload;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;
        //uintptr_t AllocAddress2 = (uintptr_t)TempData2.AllocMemoryAddress;

        if (Config.bNoReload)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress), 5, true);
                TempData.OriginalBytesSize = 5;

                //TempData2.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress2), 4, true);
                //TempData2.OriginalBytesSize = 4;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress), 5);
            //Memory::Nop((LPVOID)(AllocAddress2), 4);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress), TempData.OriginalBytes, TempData.OriginalBytesSize);
                //Memory::Patch64((LPVOID)(AllocAddress2), TempData2.OriginalBytes, TempData2.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetNoLaserOverheat()
{
    string key = "NoLaserOverheat";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bNoLaserCannonOverheat = TempData.State;
        return;
    }

    if (Config.bNoLaserCannonOverheat != TempData.State)
    {
        TempData.State = Config.bNoLaserCannonOverheat;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bNoLaserCannonOverheat)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress + 7), 5, true);
                TempData.OriginalBytesSize = 5;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress + 7), 5);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress + 7), TempData.OriginalBytes, TempData.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetInstantCharge()
{
    string key_railgun = "InstantChargeRailgun";
    string key_quasar = "InstantChargeQuasar";

    auto it_railgun = FeatureList.find(key_railgun);
    FeatureData& TempData_railgun = it_railgun->second;
    auto it_quasar = FeatureList.find(key_quasar);
    FeatureData& TempData_quasar = it_quasar->second;

    uintptr_t TempAddress_railgun = TempData_railgun.Address;
    uintptr_t TempAddress_quasar = TempData_quasar.Address;

    if (!TempAddress_railgun || !TempAddress_quasar)
    {
        Config.bInstantCharge = TempData_railgun.State;
        return;
    }

    if (!TempData_railgun.bInitialized)
    {
        //Railgun
        uintptr_t JeAddress = TempAddress_railgun + 0x3E;
        BYTE JeBytes[8] = { 0x00 };
        memcpy(JeBytes, &JeAddress, sizeof(uintptr_t));

        BYTE InstantChargeRailgunBytes[] =
        {
            0xEB, 55,
            0x43, 0xC7, 0x44, 0x3E, 0x04, 0xFC, 0xFF, 0x3F, 0x40,	// mov [r14+r15+04],403FFFFC { 3.00 }
            0xF3, 0x43, 0x0F, 0x10, 0x44, 0x3E, 0x04,	            // movss xmm0,[r14+r15+04]
            0x45, 0x84, 0xE4,	                                    // test r12b,r12b
            0x74, 0x02,	                                            // je C3260017
            0xEB, 0x0E,	                                            // jmp C3260025
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                     // jmp game.dll+58FDF7
            JeBytes[0], JeBytes[1], JeBytes[2], JeBytes[3], JeBytes[4], JeBytes[5], JeBytes[6], JeBytes[7],
            0x45, 0x0F, 0x2F, 0xD1,	                                // comiss xmm10,xmm9
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
            0xF3, 0x43, 0x0F, 0x11, 0x44, 0x3E, 0x04,	            // movss [r14+r15+04],xmm0
            0x45, 0x84, 0xE4,	                                    // test r12b,r12b
            0x74, 0x02,	                                            // je C3260017
            0xEB, 0x0E,	                                            // jmp C3260025
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                     // jmp game.dll+58FDF7
            JeBytes[0], JeBytes[1], JeBytes[2], JeBytes[3], JeBytes[4], JeBytes[5], JeBytes[6], JeBytes[7],
            0x45, 0x0F, 0x2F, 0xD1	                                // comiss xmm10,xmm9
        };

        intptr_t RetFromAllocAddress = (uintptr_t)TempData_railgun.AllocMemoryAddress + Memory::ArrayLength(InstantChargeRailgunBytes) + 6;
        intptr_t RetBytes[8] = { 0x00 };
        memcpy(RetBytes, (LPVOID)RetFromAllocAddress, sizeof(uintptr_t));
        memcpy(&InstantChargeRailgunBytes[49], RetBytes, sizeof(uintptr_t));
        TempData_railgun.JmpBytes = 55;

        Memory::Patch64(TempData_railgun.AllocMemoryAddress, InstantChargeRailgunBytes);
        Memory::CreateJmpFar(TempAddress_railgun, (uintptr_t)(TempData_railgun.AllocMemoryAddress));


        //Quasar
        BYTE InstantChargeQuasarBytes[] =
        {
            0x41, 0xC7, 0x45, 0x04, 0x8F, 0xC2, 0xC7, 0x42,	// mov [r13+04],42C7C28F { 99.88 }
            0xF3, 0x41, 0x0F, 0x10, 0x4D, 0x04,	            // movss xmm1,[r13+04]
            0xF3, 0x41, 0x0F, 0x10, 0x57, 0x38,	            // movss xmm2,[r15+38]
            0x0F, 0x2F, 0xD1	                            // comiss xmm2,xmm1
        };

        uintptr_t TempAllocAddress_quasar = (uintptr_t)TempData_quasar.AllocMemoryAddress + 2;
        TempData_quasar.JmpBytes = Memory::ArrayLength(InstantChargeQuasarBytes) + 14;
        Memory::Patch64((LPVOID)TempAllocAddress_quasar, InstantChargeQuasarBytes);

        TempData_railgun.bInitialized = true;
        return;
    }

    if (Config.bInstantCharge != TempData_railgun.State)
    {
        TempData_railgun.State = Config.bInstantCharge;

        uintptr_t AllocAddress_railgun = (uintptr_t)TempData_railgun.AllocMemoryAddress;
        uintptr_t AllocAddress_quasar = (uintptr_t)TempData_quasar.AllocMemoryAddress;

        if (Config.bInstantCharge)
        {
            BYTE JmpPatch[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress_railgun), JmpPatch);
            Memory::Patch64((LPVOID)(AllocAddress_quasar), JmpPatch);
            return;
        }
        else
        {
            BYTE JmpPatch_railgun[] = { 0xEB, TempData_railgun.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress_railgun), JmpPatch_railgun);

            BYTE JmpPatch_quasar[] = { 0xEB, TempData_quasar.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress_quasar), JmpPatch_quasar);
            return;
        }
    }
}

void features::SetNoSway()
{
    string key = "NoSway";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bNoSway = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        uintptr_t JaAddress = TempAddress + 0x11;
        BYTE JaBytes[8] = { 0x0 };
        memcpy(JaBytes, &JaAddress, sizeof(uintptr_t));

        BYTE NoSwayBytes[] =
        {
            0x0F, 0x57, 0xC0,	                // xorps xmm0,xmm0
            0x0F, 0x2E, 0xC2,	                // ucomiss xmm0,xmm2
            0x77, 0x02,	                        // ja 0672000A
            0xEB, 0x0E,	                        // jmp 06720018
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp game.dll+40F1C0
            JaBytes[0], JaBytes[1], JaBytes[2], JaBytes[3], JaBytes[4], JaBytes[5], JaBytes[6], JaBytes[7],
            0x0F, 0x57, 0xC0,	                // xorps xmm0,xmm0
            0xF3, 0x0F, 0x51, 0xC2,	            // sqrtss xmm0,xmm2
        };

        Memory::Patch64(TempData.AllocMemoryAddress, NoSwayBytes);
        Memory::CreateJmpFar(TempAddress, (uintptr_t)(TempData.AllocMemoryAddress));

        TempData.bInitialized = true;
        return;
    }

    if (Config.bNoSway != TempData.State)
    {
        TempData.State = Config.bNoSway;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 27;

        if (Config.bNoSway)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress), 4, true);
                TempData.OriginalBytesSize = 4;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress), 4);
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

void features::SetWeaponLoadout(int Type)
{
    int nTempID = -1;

    switch (Type)
    {
    case 1:
        nTempID = Config.nWeaponLoadoutPrimaryID - 1;
        if (nTempID < 0 || nTempID >= Config.nTotalWeaponID)
        {
            Config.nWeaponLoadoutPrimaryID = 1;
            return;
        }

        PlayerLoadout->WeaponPrimary = WeaponDataList[nTempID].Data;
        break;
    case 2:
        nTempID = Config.nWeaponLoadoutSecondaryID - 1;
        if (nTempID < 0 || nTempID >= Config.nTotalWeaponID)
        {
            Config.nWeaponLoadoutSecondaryID = 1;
            return;
        }

        PlayerLoadout->WeaponSecondary = WeaponDataList[nTempID].Data;
        break;
    case 3:
        nTempID = Config.nWeaponLoadoutGrenadeID - 1;
        if (nTempID < 0 || nTempID >= Config.nTotalWeaponGrenadeID)
        {
            Config.nWeaponLoadoutGrenadeID = 1;
            return;
        }

        PlayerLoadout->Grenade = WeaponGrenadeDataList[nTempID].Data;
        break;
    }
}

void features::LoadWeaponDataList()
{
	return;
	
	/*
    uintptr_t pTempLoadoutAddress = Memory::FindPattern64("game.dll", _XOR_("XXXXXXXXXXXXXXXXXXXX"));
    if (!pTempLoadoutAddress)
    {
        return;
    }

    uintptr_t PlayerLoadoutAddress = *(uintptr_t*)(*(int*)(pTempLoadoutAddress + 3) + pTempLoadoutAddress + 7);
    PlayerLoadout = (PlayerLoadout_Data*)(PlayerLoadoutAddress + 0x3C);

    DWORD oldprotect;
    VirtualProtect((LPVOID)(PlayerLoadoutAddress + 0x3C), sizeof(PlayerLoadout_Data), PAGE_EXECUTE_READWRITE, &oldprotect);

    uintptr_t pTempWeaponListAddress = Memory::FindPattern64("game.dll", _XOR_("XXXXXXXXXXXXXXXXXXXX"));
    if (pTempWeaponListAddress)
    {
        uintptr_t WeaponListAddress = *(uintptr_t*)(*(int*)(pTempWeaponListAddress + 3) + pTempWeaponListAddress + 7);
        if (!WeaponListAddress)
        {
            return;
        }

        for (;; WeaponListAddress += 4 * 8)
        {
            Weapon_Data TempWeapon;
            TempWeapon.Data = *(int*)WeaponListAddress;
            TempWeapon.Asset = *(uint64_t*)(WeaponListAddress + 8);
            TempWeapon.LoadoutItemType = *(int*)(WeaponListAddress + 16);

            if (!TempWeapon.LoadoutItemType || TempWeapon.LoadoutItemType >= 5)
            {
                break;
            }

            if (!TempWeapon.Data)
            {
                continue;
            }

            if (TempWeapon.LoadoutItemType == 4)
            {
                WeaponGrenadeDataList.push_back(TempWeapon);
            }
            else
            {
                WeaponDataList.push_back(TempWeapon);
            }
        }

        Config.nTotalWeaponID = WeaponDataList.size();
        Config.nTotalWeaponGrenadeID = WeaponGrenadeDataList.size();
        if (!Config.nTotalWeaponID)
        {
            return;
        }

        Config.bWeaponDataLoaded = true;
    }
	*/
}