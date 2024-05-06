#include "../features.h"

using namespace std;


void features::SetNoVaultCheck()
{
    string key = "NoVaultCheck";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bNoVaultCheck = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        uintptr_t Jmp1 = *(int*)(TempAddress + 2) + TempAddress + 6;
        BYTE Jmp1Bytes[8] = { 0x00 };
        memcpy(Jmp1Bytes, &Jmp1, sizeof(uintptr_t));

        uintptr_t Jmp2 = *(uint8_t*)(TempAddress + 12) + TempAddress + 2;
        BYTE Jmp2Bytes[8] = { 0x00 };
        memcpy(Jmp2Bytes, &Jmp2, sizeof(uintptr_t));

        BYTE PatchBytes[] =
        {
            0x74, 0x02,	                                    // je 008D0004
            0xEB, 0x0E,	                                    // jmp 008D0012
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,             // jmp game.dll+2B67BC
            Jmp1Bytes[0], Jmp1Bytes[1], Jmp1Bytes[2], Jmp1Bytes[3], Jmp1Bytes[4], Jmp1Bytes[5], Jmp1Bytes[6], Jmp1Bytes[7],
            0x81, 0xFA, 0xE0, 0xB8, 0x96, 0x68,	            // cmp edx,6896B8E0 { 1754708192 }
            0x74, 0x02,	                                    // je 008D001C
            0xEB, 0x0E,	                                    // jmp 008D002A
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,             // jmp game.dll+2B6773
            Jmp2Bytes[0], Jmp2Bytes[1], Jmp2Bytes[2], Jmp2Bytes[3], Jmp2Bytes[4], Jmp2Bytes[5], Jmp2Bytes[6], Jmp2Bytes[7],
        };

        Memory::Patch64(TempData.AllocMemoryAddress, PatchBytes);
        Memory::CreateJmpFar(TempAddress, (uintptr_t)(TempData.AllocMemoryAddress));

        TempData.bInitialized = true;
    }

    if (Config.bNoVaultCheck != TempData.State)
    {
        TempData.State = Config.bNoVaultCheck;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bNoVaultCheck)
        {
            BYTE PatchBytes[] = { 0xEB };

            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Patch64(TempData.AllocMemoryAddress, PatchBytes, true);
                TempData.OriginalBytesSize = Memory::ArrayLength(PatchBytes);
                return;
            }

            Memory::Patch64(TempData.AllocMemoryAddress, PatchBytes);
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

void features::SetShowMapAllIcons()
{
    string main_key = "ShowMapAllIcons_Main";
    string key_hives = "ShowMapAllIcons_Hives";
    string key_discovered = "ShowMapAllIcons_Discovered";
    string key_poi = "ShowMapAllIcons_PoI";
    string key_blip = "ShowMapAllIcons_Blip";

    auto it = FeatureList.find(main_key);
    FeatureData& TempData = it->second;

    auto it_hives = FeatureList.find(key_hives);
    FeatureData& TempData_hives = it_hives->second;
    auto it_discovered = FeatureList.find(key_discovered);
    FeatureData& TempData_discovered = it_discovered->second;
    auto it_poi = FeatureList.find(key_poi);
    FeatureData& TempData_poi = it_poi->second;
    auto it_blip = FeatureList.find(key_blip);
    FeatureData& TempData_blip = it_blip->second;

    uintptr_t TempAddress_main = TempData.Address;
    uintptr_t TempAddress_hives = TempData_hives.Address;
    uintptr_t TempAddress_discovered = TempData_discovered.Address;
    uintptr_t TempAddress_poi = TempData_poi.Address;
    uintptr_t TempAddress_blip = TempData_blip.Address;

    if (!TempAddress_main || !TempAddress_hives || !TempAddress_discovered || !TempAddress_poi || !TempAddress_blip)
    {
        Config.bShowAllMapIcons = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        //Hives
        uintptr_t HiveJne = *(int*)(TempAddress_hives + 10) + TempAddress_hives + 14;
        BYTE HiveJneBytes[8] = { 0x00 };
        memcpy(HiveJneBytes, &HiveJne, sizeof(uintptr_t));

        BYTE HiveBytes[] =
        {
            0x41, 0x80, 0xBE, 0x3C, 0xBA, 0x07, 0x00, 0x00,	// cmp byte ptr [r14+0007BA3C],00 { 0 }
            0x75, 0x02,	                                    // jne 0095000C
            0xEB, 0x0E,	                                    // jmp 0095001A
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,	            // jmp game.dll+EE77AE
            HiveJneBytes[0], HiveJneBytes[1], HiveJneBytes[2], HiveJneBytes[3], HiveJneBytes[4], HiveJneBytes[5], HiveJneBytes[6], HiveJneBytes[7]
        };

        Memory::Patch64(TempData_hives.AllocMemoryAddress, HiveBytes);
        Memory::CreateJmpFar(TempAddress_hives, (uintptr_t)(TempData_hives.AllocMemoryAddress));


        //Discovered
        uintptr_t DiscoveredJne = *(int*)(TempAddress_discovered + 2) + TempAddress_discovered + 6;
        BYTE DiscoveredJneBytes[8] = { 0x00 };
        memcpy(DiscoveredJneBytes, &DiscoveredJne, sizeof(uintptr_t));

        BYTE DiscoveredBytes[] =
        {
            0x75, 0x02,	                        // jne 066E0009
            0xEB, 0x0E,	                        // jmp 066E0017
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp game.dll+F807B0
            DiscoveredJneBytes[0], DiscoveredJneBytes[1], DiscoveredJneBytes[2], DiscoveredJneBytes[3], DiscoveredJneBytes[4], DiscoveredJneBytes[5], DiscoveredJneBytes[6], DiscoveredJneBytes[7],
            0x48, 0x8B, 0x45, 0x80,	            // mov rax,[rbp-80]
            0x80, 0x78, 0x29, 0x00	            // cmp byte ptr [rax+29],00 { 0 }
        };

        Memory::Patch64(TempData_discovered.AllocMemoryAddress, DiscoveredBytes);
        Memory::CreateJmpFar(TempAddress_discovered, (uintptr_t)(TempData_discovered.AllocMemoryAddress));


        //PoI
        uintptr_t POIJe = *(int*)(TempAddress_poi + 2) + TempAddress_poi + 6;
        BYTE POIJeBytes[8] = { 0x00 };
        memcpy(POIJeBytes, &POIJe, sizeof(uintptr_t));

        BYTE POIBytes[] =
        {
            0x74, 0x02,	                                // je 04980004
            0xEB, 0x0E,	                                // jmp 04980012
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,         // jmp game.dll+EE5FA3
            POIJeBytes[0], POIJeBytes[1], POIJeBytes[2], POIJeBytes[3], POIJeBytes[4], POIJeBytes[5], POIJeBytes[6], POIJeBytes[7],
            0x4C, 0x8B, 0x45, 0x80,	                    // mov r8,[rbp-80]
            0xF3, 0x43, 0x0F, 0x10, 0x4C, 0xBC, 0x10	// movss xmm1,[r12+r15*4+10]
        };

        Memory::Patch64(TempData_poi.AllocMemoryAddress, POIBytes);
        Memory::CreateJmpFar(TempAddress_poi, (uintptr_t)(TempData_poi.AllocMemoryAddress));


        //Blip
        uintptr_t BlipJne = *(int*)(TempAddress_blip + 2) + TempAddress_blip + 6;
        BYTE BlipJneBytes[8] = { 0x00 };
        memcpy(BlipJneBytes, &BlipJne, sizeof(uintptr_t));

        BYTE BlipBytes[] =
        {
            0x75, 0x02,	                                // jne 024D0004
            0xEB, 0x0E,	                                // jmp 024D0012
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,         // jmp game.dll+EE81B9
            BlipJneBytes[0], BlipJneBytes[1], BlipJneBytes[2], BlipJneBytes[3], BlipJneBytes[4], BlipJneBytes[5], BlipJneBytes[6], BlipJneBytes[7],
            0x49, 0x8D, 0xB8, 0xB8, 0x5E, 0x01, 0x00,	// lea rdi,[r8+00015EB8]
            0x41, 0x0F, 0x28, 0xCD,                     // movaps xmm1,xmm13
        };

        Memory::Patch64(TempData_blip.AllocMemoryAddress, BlipBytes);
        Memory::CreateJmpFar(TempAddress_blip, (uintptr_t)(TempData_blip.AllocMemoryAddress));

        TempData.bInitialized = true;
    }

    if (Config.bShowAllMapIcons != TempData.State)
    {
        TempData.State = Config.bShowAllMapIcons;

        uintptr_t AllocAddress_main = (uintptr_t)TempData.AllocMemoryAddress;
        uintptr_t AllocAddress_hives = (uintptr_t)TempData_hives.AllocMemoryAddress;
        uintptr_t AllocAddress_discovered = (uintptr_t)TempData_discovered.AllocMemoryAddress;
        uintptr_t AllocAddress_poi = (uintptr_t)TempData_poi.AllocMemoryAddress;
        uintptr_t AllocAddress_blip = (uintptr_t)TempData_blip.AllocMemoryAddress;

        if (Config.bShowAllMapIcons)
        {
            BYTE ShowAllMapIcons_main[] =
            {
                0xb8, 0x01, 0x00, 0x00, 0x00, 0x90
            };

            BYTE ShowAllMapIcons_hives[] =
            {
                0xF8, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
            };

            BYTE ShowAllMapIcons_discovered_blip[] =
            {
                0xEB
            };

            BYTE ShowAllMapIcons_poi[] =
            {
                0x90, 0x90
            };

            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Patch64((LPVOID)(AllocAddress_main), ShowAllMapIcons_main, true);
                TempData.OriginalBytesSize = Memory::ArrayLength(ShowAllMapIcons_main);

                TempData_hives.OriginalBytes = Memory::Patch64((LPVOID)(AllocAddress_hives), ShowAllMapIcons_hives, true);
                TempData_hives.OriginalBytesSize = Memory::ArrayLength(ShowAllMapIcons_hives);

                TempData_discovered.OriginalBytes = Memory::Patch64((LPVOID)(AllocAddress_discovered), ShowAllMapIcons_discovered_blip, true);
                TempData_discovered.OriginalBytesSize = Memory::ArrayLength(ShowAllMapIcons_discovered_blip);

                TempData_poi.OriginalBytes = Memory::Patch64((LPVOID)(AllocAddress_poi), ShowAllMapIcons_poi, true);
                TempData_poi.OriginalBytesSize = Memory::ArrayLength(ShowAllMapIcons_poi);

                TempData_blip.OriginalBytes = Memory::Patch64((LPVOID)(AllocAddress_blip), ShowAllMapIcons_discovered_blip, true);
                TempData_blip.OriginalBytesSize = Memory::ArrayLength(ShowAllMapIcons_discovered_blip);

                return;
            }

            Memory::Patch64((LPVOID)(AllocAddress_main), ShowAllMapIcons_main);
            Memory::Patch64((LPVOID)(AllocAddress_hives), ShowAllMapIcons_hives);
            Memory::Patch64((LPVOID)(AllocAddress_discovered), ShowAllMapIcons_discovered_blip);
            Memory::Patch64((LPVOID)(AllocAddress_poi), ShowAllMapIcons_poi);
            Memory::Patch64((LPVOID)(AllocAddress_blip), ShowAllMapIcons_discovered_blip);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress_main), TempData.OriginalBytes, TempData.OriginalBytesSize);
                Memory::Patch64((LPVOID)(AllocAddress_hives), TempData_hives.OriginalBytes, TempData_hives.OriginalBytesSize);
                Memory::Patch64((LPVOID)(AllocAddress_discovered), TempData_discovered.OriginalBytes, TempData_discovered.OriginalBytesSize);
                Memory::Patch64((LPVOID)(AllocAddress_poi), TempData_poi.OriginalBytes, TempData_poi.OriginalBytesSize);
                Memory::Patch64((LPVOID)(AllocAddress_blip), TempData_blip.OriginalBytes, TempData_blip.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetNoBoundary()
{
    string key = "NoBoundary";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bNoBoundary = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        uintptr_t JneAddress1 = (TempAddress + 2) + *(int*)(TempAddress + 2) + 4;
        BYTE JneBytes1[8] = { 0x00 };
        memcpy(JneBytes1, &JneAddress1, sizeof(uintptr_t));

        uintptr_t JneAddress2 = (TempAddress + 12) + *(uint8_t*)(TempAddress + 12) + 4;
        BYTE JneBytes2[8] = { 0x00 };
        memcpy(JneBytes2, &JneAddress2, sizeof(uintptr_t));

        BYTE NoBoundaryBytes[] =
        {
            0x75, 0x02,	                        // jne 02
            0xEB, 0x0E,	                        // jmp 0E
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp game.dll+B4E6B5
            JneBytes1[0], JneBytes1[1], JneBytes1[2], JneBytes1[3], JneBytes1[4], JneBytes1[5], JneBytes1[6], JneBytes1[7],
            0x40, 0x38, 0x77, 0x02,	            // cmp [rdi+02],sil
            0x75, 0x02,	                        // jne 02
            0xEB, 0x0E,	                        // jmp 0E
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp game.dll+B4E672
            JneBytes2[0], JneBytes2[1], JneBytes2[2], JneBytes2[3], JneBytes2[4], JneBytes2[5], JneBytes2[6], JneBytes2[7]
        };

        Memory::Patch64(TempData.AllocMemoryAddress, NoBoundaryBytes);
        Memory::CreateJmpFar(TempAddress, (uintptr_t)(TempData.AllocMemoryAddress));

        TempData.bInitialized = true;
        return;
    }

    if (Config.bNoBoundary != TempData.State)
    {
        TempData.State = Config.bNoBoundary;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bNoBoundary)
        {
            BYTE PatchBytes[] = { 0xEB };

            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Patch64((LPVOID)AllocAddress, PatchBytes, true);
                TempData.OriginalBytesSize = 1;
                return;
            }

            Memory::Patch64((LPVOID)AllocAddress, PatchBytes);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)AllocAddress, TempData.OriginalBytes, TempData.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::InstantCompleteOutpost()
{
    string key = "InstantCompleteOutpost";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bInstantCompleteOutpost = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        BYTE InstantCompleteBytes[] =
        {
            0x48, 0x89, 0x74, 0x24, 0x48,	            // mov [rsp+48],rsi
            0xC6, 0x44, 0x88, 0x21, 0x01,	            // mov byte ptr [rax+rcx*4+21],01 { 1 }
            0x0F, 0xB6, 0x54, 0x88, 0x21,	            // movzx edx,byte ptr [rax+rcx*4+21]
            0x38, 0x94, 0x3E, 0x7D, 0x02, 0x00, 0x00	// cmp [rsi+rdi+0000027D],dl
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(InstantCompleteBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress, InstantCompleteBytes);

        TempData.bInitialized = true;
    }

    if (Config.bInstantCompleteOutpost != TempData.State)
    {
        TempData.State = Config.bInstantCompleteOutpost;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bInstantCompleteOutpost)
        {
            BYTE PatchBytes[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
            return;
        }
        else
        {
            BYTE PatchBytes[] = { 0xEB, TempData.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
            return;
        }
    }
}

void features::ShowHiddenOutpost()
{
#ifdef VMPROTECT
    VMProtectBeginUltra("features::ShowHiddenOutpost");
    if (!VMProtectIsProtected())
    {
        return;
    }
#endif

	return;
	
	/*
    string key = "ShowHiddenOutpost";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bShowHiddenOutpost = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        uintptr_t Jmp1 = TempAddress + 0xBD;
        BYTE Jmp1Bytes[8] = { 0x0 };
        memcpy(Jmp1Bytes, &Jmp1, sizeof(uintptr_t));

        BYTE ShowHiddenOutpostBytes[] =
        {
            XXXXXXXXXXXXXXXXXXXX
        };

        intptr_t RetFromAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + Memory::ArrayLength(ShowHiddenOutpostBytes) + 6;
        intptr_t RetBytes[8] = { 0x00 };
        memcpy(RetBytes, (LPVOID)RetFromAllocAddress, sizeof(uintptr_t));
        memcpy(&ShowHiddenOutpostBytes[72], RetBytes, sizeof(uintptr_t));
        TempData.JmpBytes = 45;

        Memory::Patch64(TempData.AllocMemoryAddress, ShowHiddenOutpostBytes);
        Memory::CreateJmpFar(TempAddress, (uintptr_t)(TempData.AllocMemoryAddress));

        TempData.bInitialized = true;
    }

    if (Config.bShowHiddenOutpost != TempData.State)
    {
        TempData.State = Config.bShowHiddenOutpost;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bShowHiddenOutpost)
        {
            BYTE PatchBytes[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
            return;
        }
        else
        {
            BYTE PatchBytes[] = { 0xEB, TempData.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
            return;
        }
    }
	*/

#ifdef VMPROTECT
    VMProtectEnd();
#endif
}

void features::InstantCompleteMission()
{
    string key = "InstantCompleteMission";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bInstantCompleteMission = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        BYTE InstantCompleteBytes[] =
        {
            0x41, 0xC7, 0x47, 0x38, 0x02, 0x00, 0x00, 0x00,	// mov [r15+38],00000002 { 2 }
            0x41, 0x8B, 0x47, 0x38,	                        // mov eax,[r15+38]
            0x83, 0xE8, 0x02,	                            // sub eax,02 { 2 }
            0x83, 0xF8, 0x01,	                            // cmp eax,01 { 1 }
            0x8B, 0x83, 0x14, 0x08, 0x00, 0x00	            // mov eax,[rbx+00000814]
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(InstantCompleteBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress, InstantCompleteBytes);

        TempData.bInitialized = true;
    }

    if (Config.bInstantCompleteMission != TempData.State)
    {
        TempData.State = Config.bInstantCompleteMission;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bInstantCompleteMission)
        {
            BYTE PatchBytes[] = { 0x90, 0x90 };
            Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
            return;
        }
        else
        {
            BYTE PatchBytes[] = { 0xEB, TempData.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
            return;
        }
    }
}

void features::SetCustomMissionTime()
{
    string key = "CustomMissionTime";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.fCustomMissionTime = TempData.fValue;
        return;
    }

    if (!TempData.bInitialized)
    {
        TempData.fValue = Config.fCustomMissionTime;

        BYTE FloatBytes[4] = { 0x0 };
        memcpy(FloatBytes, &TempData.fValue, sizeof(float));

        BYTE MovssBytes[4];
        memcpy(&MovssBytes, (LPVOID)(TempAddress + 15), 4);

        BYTE CustomMissionTimeBytes[] =
        {
            0xEB, 40,
            0x50,                                                               // push rax
            0xB8, FloatBytes[0], FloatBytes[1], FloatBytes[2], FloatBytes[3],   // mov eax, float { Value }
            0x66, 0x0F, 0x6E, 0xC0,                                             // movd xmm0,eax
            0x58,                                                               // pop rax
            0xF3, 0x41, 0x0F, 0x5F, 0xC7,                                       // maxss xmm0,xmm15
            0xF3, 0x43, 0x0F, 0x11, 0x84, 0xF4,                                 // movss [r12+r14*8+000385C8],xmm0
            MovssBytes[0], MovssBytes[1], MovssBytes[2], MovssBytes[3],
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
            0xF3, 0x0F, 0x5C, 0xC7,                                             // subss xmm0,xmm7
            0xF3, 0x41, 0x0F, 0x5F, 0xC7,                                       // maxss xmm0,xmm15
            0xF3, 0x43, 0x0F, 0x11, 0x84, 0xF4,                                 // movss [r12+r14*8+000385C8],xmm0
            MovssBytes[0], MovssBytes[1], MovssBytes[2], MovssBytes[3],
        };

        intptr_t RetFromAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + Memory::ArrayLength(CustomMissionTimeBytes) + 6;
        intptr_t RetBytes[8] = { 0x00 };
        memcpy(RetBytes, (LPVOID)RetFromAllocAddress, sizeof(uintptr_t));
        memcpy(&CustomMissionTimeBytes[34], RetBytes, sizeof(uintptr_t));
        TempData.JmpBytes = 40;

        Memory::Patch64(TempData.AllocMemoryAddress, CustomMissionTimeBytes);
        Memory::CreateJmpFar(TempAddress, (uintptr_t)(TempData.AllocMemoryAddress));

        TempData.bInitialized = true;
        return;
    }

    if (Config.fCustomMissionTime != TempData.fValue)
    {
        TempData.fValue = Config.fCustomMissionTime;
        TempData.State = true;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        *(float*)(AllocAddress + 4) = TempData.fValue;

        BYTE PatchBytes[] = { 0x90, 0x90 };
        Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
        return;
    }
}

void features::UnfreezeCustomMissionTime()
{
    string key = "CustomMissionTime";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.fCustomMissionTime = TempData.fValue;
        return;
    }

    if (!TempData.bInitialized)
    {
        Config.fCustomMissionTime = TempData.fValue;
        return;
    }

    if (TempData.State)
    {
        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;
        TempData.State = false;

        BYTE PatchBytes[] = { 0xEB, TempData.JmpBytes };
        Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
        return;
    }
}

void features::SetMaxExtractionTime()
{
    string key = "MaxExtractionTime";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.fMaxExtractionTime = TempData.fValue;
        return;
    }

    if (!TempData.bInitialized)
    {
        TempData.fValue = Config.fMaxExtractionTime;

        BYTE FloatBytes[4] = { 0x0 };
        memcpy(FloatBytes, &TempData.fValue, sizeof(float));

        BYTE ExtractionTimeBytes[] =
        {
            0x53,	                                                            // push rbx
            0xBB, FloatBytes[0], FloatBytes[1], FloatBytes[2], FloatBytes[3],	// mov ebx,3F800000 { 1.00 }
            0x66, 0x0F, 0x6E, 0xC3,	                                            // movd xmm0,ebx
            0x5B,	                                                            // pop rbx
            0xF3, 0x0F, 0x11, 0x04, 0xC8,	                                    // movss [rax+rcx*8],xmm0
            0x49, 0x8B, 0x47, 0x78,	                                            // mov rax,[r15+78]
            0x4C, 0x8D, 0x04, 0xC8,	                                            // lea r8,[rax+rcx*8]
            0x49, 0x8B, 0x47, 0x60	                                            // mov rax,[r15+60]
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(ExtractionTimeBytes) + 14;

        Memory::Patch64((LPVOID)TempAllocAddress, ExtractionTimeBytes);

        TempData.bInitialized = true;
        return;
    }

    if (Config.fMaxExtractionTime != TempData.fValue)
    {
        TempData.fValue = Config.fMaxExtractionTime;
        TempData.State = true;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        *(float*)(AllocAddress + 4) = TempData.fValue;

        BYTE PatchBytes[] = { 0x90, 0x90 };
        Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
        return;
    }
}

void features::UnfreezeExtractionTime()
{
    string key = "MaxExtractionTime";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.fMaxExtractionTime = TempData.fValue;
        return;
    }

    if (!TempData.bInitialized)
    {
        Config.fMaxExtractionTime = TempData.fValue;
        return;
    }

    if (TempData.State)
    {
        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;
        TempData.State = false;

        BYTE PatchBytes[] = { 0xEB, TempData.JmpBytes };
        Memory::Patch64((LPVOID)(AllocAddress), PatchBytes);
        return;
    }
}