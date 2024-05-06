#include "features.h"
#include "libs/xorstr.hpp"

using namespace std;

features Features;

features::features()
{
    for (int i = 0; i < 9; i++)
    {
        StratagemPlayerSlotData.push_back({-1, -1, -1});
    }

    InitEnemyEntityList();
}

features::~features()
{
    
}

void features::AllocMemory(const string key, const char* pattern, MemoryType type, unsigned int offset, unsigned int return_offset, int size)
{
    CheckFeatureExist(key);

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    TempData.Address = GetAddress(key, pattern, offset);
    if (!TempData.Address)
    {
#ifdef DEBUG_CONSOLE
        printf("[Features][AllocMemory] %s Failed (Address not found)\n", key.c_str());
#endif
        return;
    }
    TempData.AllocMemoryAddress = Memory::CreateTrampoline(TempData.Address, type, return_offset, size);

#ifdef DEBUG_CONSOLE
    printf("[Features][AllocMemory] %s Success\n", key.c_str());
#endif
}

void features::Alloc()
{
#ifdef VMPROTECT
    VMProtectBeginUltra("features::Alloc");
    if (!VMProtectIsProtected())
    {
        return;
    }
#endif

    //Player
    AllocMemory("InfAmmo_Main", "42 83 ? ? ? 48 8B ? ? 0F 94 ? 48 8B", MemoryType::MEMORY_ALLOCATE, 0, 2, 23);
    AllocMemory("InfAmmo_Sickle", "41 83 ? ? ? 49 8B ? ? 0F 94 ? 4D", MemoryType::MEMORY_ALLOCATE, 0, 2, 23);
	//AllocMemory("InfAmmo_SingleReloadRound", "89 1C 08 48 8B 46");
    SetInfAmmo();
    AllocMemory("InfGrenade", "41 FF ? ? 48 8B ? ? 4C ? ? ? 4D", MemoryType::MEMORY_ALLOCATE, 0, 1, 22);
    SetInfGrenade();
    AllocMemory("InfStim", "4B 8D 0C 40 48 03 C9 45 8B 74 CF", MemoryType::MEMORY_ALLOCATE, 0, 1, 21);
    SetInfStim();
    AllocMemory("InfBackpack", "2B ? BA ? ? ? ? 42 89", MemoryType::MEMORY_COPY, 0, 1);
    AllocMemory("InstantStratagemDrop", _XOR_("F3 42 0F 11 04 F0 49 8B 47"), MemoryType::MEMORY_ALLOCATE, 0, 0, 37);
    SetInstantStratagemDrop();
    AllocMemory("CombinationAlwaysCorrect", _XOR_("42 3B ? ? ? 75 ? 41 8D"), MemoryType::MEMORY_MANUAL, 0, 1, 43);
    SetCombinationAlwaysCorrect();
    AllocMemory("PlayerDamage", _XOR_("42 89 ? ? 48 8B ? ? 4E ? ? ? 48 8B ? ? 48 8B ? ? 8B ? ? E8 ? ? ? ? 83"), MemoryType::MEMORY_ALLOCATE, 0, 2, 21);
    SetTakeNoDamage();
    AllocMemory("FreezePlayerHealth", _XOR_("44 8B 53 ? 8B 7B ? 0F AF FA 45 8D 7A"), MemoryType::MEMORY_ALLOCATE, 0, 0, 26);
    SetFreezePlayerHealth();
    AllocMemory("PlayerSpeed", "F3 41 0F 59 56 0C F3", MemoryType::MEMORY_MANUAL, 0, 1, 40);
    SetPlayerSpeed();

    //Weapon
    AllocMemory("NoReload", "44 89 74 01 18 4D 8B 42 50", MemoryType::MEMORY_COPY, 0, 3);
    //AllocMemory("NoReload_SingleReloadRound", "FF ? ? ? 83 ? ? 74 ? 49 8B ? F6");
    AllocMemory("NoLaserOverheat", "4D 03 C7 48 8B 0C D8 F3 41 0F 11 08", MemoryType::MEMORY_COPY, 0, 1);
    AllocMemory("InstantChargeRailgun", "F3 43 0F 11 44 3E ? 45 84 E4", MemoryType::MEMORY_MANUAL, 0, 2, 89);
    AllocMemory("InstantChargeQuasar", "F3 41 0F 10 4D ? F3 41 0F 10 57", MemoryType::MEMORY_ALLOCATE, 0, 1, 23);
    SetInstantCharge();
    AllocMemory("NoSway", "0F 57 C0 0F 2E C2 77 ? 0F 57 C0 F3 0F 51 C2 EB ? 0F 28 C2 E8 ? ? ? ? F3 0F 10 7D", MemoryType::MEMORY_MANUAL, 0, 1, 31);
    SetNoSway();

    //Mission
    AllocMemory("NoVaultCheck", "0F 84 BD 00 00 00 81 FA E0", MemoryType::MEMORY_MANUAL, 0, 0, 42);
    SetNoVaultCheck();
    AllocMemory("ShowMapAllIcons_Main", _XOR_("43 0F B6 44 8A ? F2 44 0F 11 65"), MemoryType::MEMORY_COPY, 0, 4);
    AllocMemory("ShowMapAllIcons_Hives", _XOR_("41 80 BE 3C BA 07 00 00"), MemoryType::MEMORY_MANUAL, 0, 0, 26);
    AllocMemory("ShowMapAllIcons_Discovered", _XOR_("0F 85 ? ? ? ? 48 8B 45 ? 80 78"), MemoryType::MEMORY_MANUAL, 0, 0, 26);
    AllocMemory("ShowMapAllIcons_PoI", _XOR_("0F 84 ? ? ? ? 4C 8B 45 ? F3 43 0F 10 4C BC"), MemoryType::MEMORY_MANUAL, 0, 3, 29);
    AllocMemory("ShowMapAllIcons_Blip", _XOR_("0F 85 ? ? ? ? 49 8D B8"), MemoryType::MEMORY_MANUAL, 0, 3, 29);
    SetShowMapAllIcons();
    AllocMemory("NoBoundary", "0F 85 ? ? ? ? 40 38 77", MemoryType::MEMORY_MANUAL, 0, 2, 40);
    SetNoBoundary();
    AllocMemory("InstantCompleteOutpost", "48 89 74 24 ? 0F B6 54 88", MemoryType::MEMORY_ALLOCATE, 0, 3, 22);
    InstantCompleteOutpost();
    //AllocMemory("ShowHiddenOutpost", _XOR_("XXXXXXXXXXXXXXXXXXXX"), MemoryType::MEMORY_MANUAL, 0, 1, 107);
    //ShowHiddenOutpost();
    AllocMemory("InstantCompleteMission", "41 8B 47 ? 83 E8 ? 83 F8 ? 8B 83", MemoryType::MEMORY_ALLOCATE, 0, 2, 24);
    InstantCompleteMission();
    AllocMemory("CustomMissionTime", "F3 0F 5C C7 F3 41 0F 5F C7", MemoryType::MEMORY_MANUAL, 0, 5, 61);
    SetCustomMissionTime();
    AllocMemory("MaxExtractionTime", "F3 0F 11 04 C8 49 8B 47", MemoryType::MEMORY_ALLOCATE, 0, 3, 28);
    SetMaxExtractionTime();

    //Misc
    AllocMemory("NoJetpackCooldown", "48 8B ? ? 8B ? 89 ? ? 83 ? ? 75 ? 8B", MemoryType::MEMORY_MANUAL, 0, 0, 30);
    SetNoJetpackCooldown();
    AllocMemory("NoStationaryTurretOverheat", "F3 ? ? ? ? 41 8B ? ? 41 89 ? ? 8B ? E8", MemoryType::MEMORY_COPY, 0, 1);
    AllocMemory("BackpackShieldCooldownTime", "F3 ? ? ? ? ? 0F 28 ? 43 80", MemoryType::MEMORY_ALLOCATE, 0, 1, 40);
    AllocMemory("BackpackShieldEnergy", "F3 ? ? ? ? ? ? 49 8B ? ? 4D ? ? ? 49 83 ? ? 4D ? ? 48 8B ? ? BA", MemoryType::MEMORY_ALLOCATE, 0, 1, 20);
    SetBackpackShieldSetting();

#ifdef VMPROTECT
    VMProtectEnd();
#endif
}


void features::CheckFeatureExist(const string key)
{
    if (FeatureList.count(key) == 0)
    {
        FeatureData TempData;
        FeatureList.insert({ key, TempData });
    }
}

uintptr_t features::GetAddress(const std::string key, const char* pattern, unsigned int offset)
{
    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    if (TempData.Address > 0)
    {
		return TempData.Address;
    }

	uintptr_t TempAddress = Memory::FindPattern64("game.dll", pattern);
    if (!TempAddress)
    {
        return TempAddress;
    }

    TempData.Address = TempAddress + offset;
    return TempAddress + offset;
}

