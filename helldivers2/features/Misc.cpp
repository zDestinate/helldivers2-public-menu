#include "../features.h"
#include "../include/FileManager.h"

using namespace std;


void features::SetNoJetpackCooldown()
{
    string key = "NoJetpackCooldown";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bNoJetpackCooldown = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        BYTE JneBytes[8] = { 0x0 };
        uintptr_t JneAddress = (*(uint8_t*)(TempAddress + 13)) + (TempAddress + 12) + 1;
        memcpy(JneBytes, &JneAddress, sizeof(uintptr_t));

        BYTE NoJetpackCooldownBytes[] =
        {
            0x48, 0x8B, 0x4E, 0x48,	                        // mov rcx,[rsi+48]
            0x8B, 0x00,	                                    // mov eax,[rax]
            0x89, 0x04, 0xB9,	                            // mov [rcx+rdi*4],eax
            0x83, 0xFD, 0x01,	                            // cmp ebp,01 { 1 }
            0x75, 0x02,	                                    // jne 06860010
            0xEB, 0x0E,	                                    // jmp 0686001E
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,             // jmp game.dll+5FEC13
            JneBytes[0], JneBytes[1], JneBytes[2], JneBytes[3], JneBytes[4], JneBytes[5], JneBytes[6], JneBytes[7]
        };

        Memory::Patch64(TempData.AllocMemoryAddress, NoJetpackCooldownBytes);
        Memory::CreateJmpFar(TempAddress, (uintptr_t)(TempData.AllocMemoryAddress));

        TempData.bInitialized = true;
    }

    if (Config.bNoJetpackCooldown != TempData.State)
    {
        TempData.State = Config.bNoJetpackCooldown;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bNoJetpackCooldown)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress + 6), 3, true);
                TempData.OriginalBytesSize = 3;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress + 6), 3);
            return;
        }
        else
        {
            if (TempData.OriginalBytes != nullptr)
            {
                Memory::Patch64((LPVOID)(AllocAddress + 6), TempData.OriginalBytes, TempData.OriginalBytesSize);
                return;
            }
        }
    }
}

void features::SetNoStationaryTurretOverheat()
{
    string key = "NoStationaryTurretOverheat";

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = TempData.Address;

    if (!TempAddress)
    {
        Config.bNoStationaryTurretOverheat = TempData.State;
        return;
    }

    if (Config.bNoStationaryTurretOverheat != TempData.State)
    {
        TempData.State = Config.bNoStationaryTurretOverheat;

        uintptr_t AllocAddress = (uintptr_t)TempData.AllocMemoryAddress;

        if (Config.bNoStationaryTurretOverheat)
        {
            if (TempData.OriginalBytes == nullptr)
            {
                TempData.OriginalBytes = Memory::Nop((LPVOID)(AllocAddress), 5, true);
                TempData.OriginalBytesSize = 5;
                return;
            }

            Memory::Nop((LPVOID)(AllocAddress), 5);
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

void features::SetBackpackShieldSetting()
{
#ifdef VMPROTECT
    VMProtectBeginUltra("features::SetBackpackShieldSetting");
    if (!VMProtectIsProtected())
    {
        return;
    }
#endif

    string key_cd = "BackpackShieldCooldownTime";
    string key_energy = "BackpackShieldEnergy";
    CheckFeatureExist(key_cd);
    CheckFeatureExist(key_energy);

    auto it_cd = FeatureList.find(key_cd);
    FeatureData& TempData = it_cd->second;
    auto it_energy = FeatureList.find(key_energy);
    FeatureData& TempData_Energy = it_energy->second;

    uintptr_t TempAddress_cd = TempData.Address;
    uintptr_t TempAddress_energy = TempData_Energy.Address;

    if (!TempAddress_cd || !TempAddress_energy)
    {
        Config.bBackpackShieldSetting = TempData.State;
        return;
    }

    if (!TempData.bInitialized)
    {
        //Cooldown
        TempData.fValue = Config.fBackpackShieldCooldownTime;
        BYTE CDTimeBytes[4] = { 0x0 };
        memcpy(CDTimeBytes, &TempData.fValue, sizeof(float));

        BYTE BackpackShieldCooldownBytes[] =
        {
            0x51,                                                                       // push rcx
            0x52,                                                                       // push rdx
            0x66, 0x48, 0x0F, 0x7E, 0xC9,                                               // movq rcx,xmm1
            0xBA, CDTimeBytes[0], CDTimeBytes[1], CDTimeBytes[2], CDTimeBytes[3],       // mov edx, (float)1.0
            0x39, 0xD1,                                                                 // cmp ecx,edx
            0x7E, 0x07,                                                                 // jle 0x07
            0x8B, 0xCA,                                                                 // mov ecx,edx
            0x66, 0x48, 0x0F, 0x6E, 0xC9,                                               // movq xmm1,rcx
            0x59,                                                                       // pop rcx
            0x5A,                                                                       // pop rdx
            0xF3, 0x0F, 0x11, 0x4C, 0xF5, 0x08,	                                        // movss [rbp+rsi*8+08],xmm1
            0x0F, 0x28, 0xD1,	                                                        // movaps xmm2,xmm1
            0x43, 0x80, 0x7C, 0x34, 0x01, 0x00	                                        // cmp byte ptr [r12+r14+01],00 { 0 }
        };

        uintptr_t TempAllocAddress = (uintptr_t)TempData.AllocMemoryAddress + 2;
        TempData.JmpBytes = Memory::ArrayLength(BackpackShieldCooldownBytes) + 14;
        Memory::Patch64((LPVOID)TempAllocAddress, BackpackShieldCooldownBytes);


        //Energy
        TempData_Energy.fValue = Config.fBackpackShieldMaxEnergy;
        BYTE FloatBytes_Energy[4] = { 0x0 };
        memcpy(FloatBytes_Energy, &TempData_Energy.fValue, sizeof(float));

        BYTE BackpackShieldEnergyBytes[] =
        {
            0x53,                                                                                           // push rbx
            0xBB, FloatBytes_Energy[0], FloatBytes_Energy[1], FloatBytes_Energy[2], FloatBytes_Energy[3],   // mov ebx,43160000 { 150.00 }
            0x42, 0x89, 0x5C, 0x30, 0x04,	                                                                // mov [rax+r14+04],ebx
            0x5B,	                                                                                        // pop rbx
            0x49, 0x8B, 0x45, 0x40,	                                                                        // mov rax,[r13+40]
            0x4D, 0x8B, 0x45, 0x50	                                                                        // mov r8,[r13+50]
        };

        uintptr_t TempAllocAddress_Energy = (uintptr_t)TempData_Energy.AllocMemoryAddress + 2;
        TempData_Energy.JmpBytes = Memory::ArrayLength(BackpackShieldEnergyBytes) + 14;
        Memory::Patch64((LPVOID)TempAllocAddress_Energy, BackpackShieldEnergyBytes);

        TempData.bInitialized = true;
        return;
    }

    if (Config.bBackpackShieldSetting != TempData.State)
    {
        TempData.State = Config.bBackpackShieldSetting;

        uintptr_t AllocAddress_cd = (uintptr_t)TempData.AllocMemoryAddress;
        uintptr_t AllocAddress_energy = (uintptr_t)TempData_Energy.AllocMemoryAddress;

        if (Config.bBackpackShieldSetting)
        {
            BYTE PatchBytes[] = { 0x90, 0x90 };

            if (Config.fBackpackShieldCooldownTime < 0.0f)
            {
                Config.fBackpackShieldCooldownTime = 0.0f;
            }

            *(float*)(AllocAddress_cd + 10) = Config.fBackpackShieldCooldownTime;
            Memory::Patch64((LPVOID)(AllocAddress_cd), PatchBytes);


            if (Config.fBackpackShieldMaxEnergy < 0.0f)
            {
                Config.fBackpackShieldMaxEnergy = 0.0f;
            }

            *(float*)(AllocAddress_energy + 4) = Config.fBackpackShieldMaxEnergy;
            Memory::Patch64((LPVOID)(AllocAddress_energy), PatchBytes);

            return;
        }
        else
        {
            BYTE PatchBytes[] = { 0xEB, TempData.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress_cd), PatchBytes);

            BYTE PatchBytes_Energy[] = { 0xEB, TempData_Energy.JmpBytes };
            Memory::Patch64((LPVOID)(AllocAddress_energy), PatchBytes_Energy);
            return;
        }
    }

#ifdef VMPROTECT
    VMProtectEnd();
#endif
}

void features::InitEnemyEntityList()
{
    Features.EnemyEntityList.push_back({ 0, "Default" });

    Features.EnemyEntityList.push_back({ 0xCF576442, "Terminid - Scavenger" });
    Features.EnemyEntityList.push_back({ 0x93B54329, "Terminid - Warrior" });
    Features.EnemyEntityList.push_back({ 0x60052616, "Terminid - Warrior 2" });
    Features.EnemyEntityList.push_back({ 0x8F7704F8, "Terminid - Warrior 3" });
    Features.EnemyEntityList.push_back({ 0xD6CEAF11, "Terminid - Hive Guard" });
    Features.EnemyEntityList.push_back({ 0xD1A37E30, "Terminid - Hunter" });
    Features.EnemyEntityList.push_back({ 0x390B51AD, "Terminid - Brood Commander" });
    Features.EnemyEntityList.push_back({ 0xA2679E02, "Terminid - Charger" });
    Features.EnemyEntityList.push_back({ 0x4CE0A543, "Terminid - Charger Behemoth" });
    Features.EnemyEntityList.push_back({ 0x6D826AC1, "Terminid - Nursing Spewer" });
    Features.EnemyEntityList.push_back({ 0x47AE3D0E, "Terminid - Nursing Spewer 2" });
    Features.EnemyEntityList.push_back({ 0x8672C541, "Terminid - Bile Spewer" });
    Features.EnemyEntityList.push_back({ 0x5DB0B082, "Terminid - Stalker" });
    Features.EnemyEntityList.push_back({ 0x962A5257, "Terminid - Shrieker" });
    Features.EnemyEntityList.push_back({ 0x32037646, "Terminid - Bile Titan" });

    Features.EnemyEntityList.push_back({ 0xBE43C08E, "Automaton - Marauder" });
    Features.EnemyEntityList.push_back({ 0x60DF0436, "Automaton - Brawler" });
    Features.EnemyEntityList.push_back({ 0x964DE5E5, "Automaton - Commissar" });
    Features.EnemyEntityList.push_back({ 0x341BEE06, "Automaton - Scout Raider" });
    Features.EnemyEntityList.push_back({ 0xF6D6F75B, "Automaton - MG Raider" });
    Features.EnemyEntityList.push_back({ 0x6F4E6468, "Automaton - Rocket Raider" });
    Features.EnemyEntityList.push_back({ 0x65D9EB91, "Automaton - Scout Strider" });
    Features.EnemyEntityList.push_back({ 0x4B634472, "Automaton - Berserker" });
    Features.EnemyEntityList.push_back({ 0x1DBAEAE7, "Automaton - Heavy Devastator" });
    Features.EnemyEntityList.push_back({ 0x6C44D48B, "Automaton - Heavy Devastator 2" });
    Features.EnemyEntityList.push_back({ 0x82D0EB09, "Automaton - Rocket Devastator" });
    Features.EnemyEntityList.push_back({ 0x639A1550, "Automaton - Rocket Devastator 2" });
    Features.EnemyEntityList.push_back({ 0x81BF7968, "Automaton - Rocket Devastator 3" });
    Features.EnemyEntityList.push_back({ 0x30D5739C, "Automaton - Hulk Scorcher" });
    Features.EnemyEntityList.push_back({ 0xE5E739C3, "Automaton - Hulk Bruiser" });
    Features.EnemyEntityList.push_back({ 0x32E94061, "Automaton - Hulk Obliterator" });
}

void features::SaveAllConfig()
{
    //Player
    g_FileManager.Config["Player"]["bInfStamina"] = Config.bInfStamina;
    g_FileManager.Config["Player"]["bInfAmmo"] = Config.bInfAmmo;
    g_FileManager.Config["Player"]["bInfAmmo_Legit"] = Config.bInfAmmo_Legit;
    g_FileManager.Config["Player"]["bInfGrenade"] = Config.bInfGrenade;
    g_FileManager.Config["Player"]["bInfGrenade_Legit"] = Config.bInfGrenade_Legit;
    g_FileManager.Config["Player"]["bInfStim"] = Config.bInfStim;
    g_FileManager.Config["Player"]["bInfStim_Legit"] = Config.bInfStim_Legit;
    g_FileManager.Config["Player"]["bInfBackpack"] = Config.bInfBackpack;
    g_FileManager.Config["Player"]["bInfStratagem"] = Config.bInfStratagem;
    g_FileManager.Config["Player"]["bNoRagdoll"] = Config.bNoRagdoll;
    g_FileManager.Config["Player"]["bInstantStratagemDrop"] = Config.bInstantStratagemDrop;
    g_FileManager.Config["Player"]["bCombinationAlwaysCorrect"] = Config.bCombinationAlwaysCorrect;
    g_FileManager.Config["Player"]["bTakeNoDamage"] = Config.bTakeNoDamage;
    g_FileManager.Config["Player"]["fPlayerSpeed"] = Config.fPlayerSpeed;
    g_FileManager.Config["Player"]["bNoClip"] = Config.bNoClip;

    //Weapon
    g_FileManager.Config["Weapon"]["bNoReload"] = Config.bNoReload;
    g_FileManager.Config["Weapon"]["bNoLaserCannonOverheat"] = Config.bNoLaserCannonOverheat;
    g_FileManager.Config["Weapon"]["bInstantCharge"] = Config.bInstantCharge;
    g_FileManager.Config["Weapon"]["bNoRecoil"] = Config.bNoRecoil;
    g_FileManager.Config["Weapon"]["bNoSway"] = Config.bNoSway;
    g_FileManager.Config["Weapon"]["bInfSpecialWeapon"] = Config.bInfSpecialWeapon;

    //Mission
    g_FileManager.Config["Mission"]["bNoVaultCheck"] = Config.bNoVaultCheck;
    g_FileManager.Config["Mission"]["bReduceEnemyAggro"] = Config.bReduceEnemyAggro;
    g_FileManager.Config["Mission"]["bShowAllMapIcons"] = Config.bShowAllMapIcons;
    g_FileManager.Config["Mission"]["bNoBoundary"] = Config.bNoBoundary;
    g_FileManager.Config["Mission"]["bInstantCompleteOutpost"] = Config.bInstantCompleteOutpost;
    g_FileManager.Config["Mission"]["bShowHiddenOutpost"] = Config.bShowHiddenOutpost;
    g_FileManager.Config["Mission"]["bInstantCompleteMission"] = Config.bInstantCompleteMission;

    //Misc
    g_FileManager.Config["Misc"]["bUnlockAllStratagems"] = Config.bUnlockAllStratagems;
    g_FileManager.Config["Misc"]["bUnlockAllEquipment"] = Config.bUnlockAllEquipment;
    g_FileManager.Config["Misc"]["bUnlockAllArmor"] = Config.bUnlockAllArmor;
    g_FileManager.Config["Misc"]["bNoJetpackCooldown"] = Config.bNoJetpackCooldown;
    g_FileManager.Config["Misc"]["bNoStationaryTurretOverheat"] = Config.bNoStationaryTurretOverheat;
    g_FileManager.Config["Misc"]["bBackpackShieldSetting"] = Config.bBackpackShieldSetting;
    g_FileManager.Config["Misc"]["fBackpackShieldCooldownTime"] = Config.fBackpackShieldCooldownTime;
    g_FileManager.Config["Misc"]["fBackpackShieldMaxEnergy"] = Config.fBackpackShieldMaxEnergy;

    g_FileManager.SaveFile(L"config.json", g_FileManager.Config);
}

void features::LoadAllConfig()
{
    //Player
    Config.bInfStamina = g_FileManager.Config["Player"]["bInfStamina"].is_boolean() ? g_FileManager.Config["Player"]["bInfStamina"].get<bool>() : false;
    Config.bInfAmmo = g_FileManager.Config["Player"]["bInfAmmo"].is_boolean() ? g_FileManager.Config["Player"]["bInfAmmo"].get<bool>() : false;
    Config.bInfAmmo_Legit = g_FileManager.Config["Player"]["bInfAmmo_Legit"].is_boolean() ? g_FileManager.Config["Player"]["bInfAmmo_Legit"].get<bool>() : false;
    Config.bInfGrenade = g_FileManager.Config["Player"]["bInfGrenade"].is_boolean() ? g_FileManager.Config["Player"]["bInfGrenade"].get<bool>() : false;
    Config.bInfGrenade_Legit = g_FileManager.Config["Player"]["bInfGrenade_Legit"].is_boolean() ? g_FileManager.Config["Player"]["bInfGrenade_Legit"].get<bool>() : false;
    Config.bInfStim = g_FileManager.Config["Player"]["bInfStim"].is_boolean() ? g_FileManager.Config["Player"]["bInfStim"].get<bool>() : false;
    Config.bInfStim_Legit = g_FileManager.Config["Player"]["bInfStim_Legit"].is_boolean() ? g_FileManager.Config["Player"]["bInfStim_Legit"].get<bool>() : false;
    Config.bInfBackpack = g_FileManager.Config["Player"]["bInfBackpack"].is_boolean() ? g_FileManager.Config["Player"]["bInfBackpack"].get<bool>() : false;
    Config.bInfStratagem = g_FileManager.Config["Player"]["bInfStratagem"].is_boolean() ? g_FileManager.Config["Player"]["bInfStratagem"].get<bool>() : false;
    Config.bNoRagdoll = g_FileManager.Config["Player"]["bNoRagdoll"].is_boolean() ? g_FileManager.Config["Player"]["bNoRagdoll"].get<bool>() : false;
    Config.bInstantStratagemDrop = g_FileManager.Config["Player"]["bInstantStratagemDrop"].is_boolean() ? g_FileManager.Config["Player"]["bInstantStratagemDrop"].get<bool>() : false;
    Config.bCombinationAlwaysCorrect = g_FileManager.Config["Player"]["bCombinationAlwaysCorrect"].is_boolean() ? g_FileManager.Config["Player"]["bCombinationAlwaysCorrect"].get<bool>() : false;
    Config.bTakeNoDamage = g_FileManager.Config["Player"]["bTakeNoDamage"].is_boolean() ? g_FileManager.Config["Player"]["bTakeNoDamage"].get<bool>() : false;
    Config.fPlayerSpeed = g_FileManager.Config["Player"]["fPlayerSpeed"].is_number_float() ? g_FileManager.Config["Player"]["fPlayerSpeed"].get<float>() : 1.0f;
    Config.bNoClip = g_FileManager.Config["Player"]["bNoClip"].is_boolean() ? g_FileManager.Config["Player"]["bNoClip"].get<bool>() : false;

    SetInfAmmo();
    SetInfAmmo_Legit();
    SetInfGrenade();
    SetInfGrenade_Legit();
    SetInfStim();
    SetInfStim_Legit();
    SetInfBackpack();
    SetInstantStratagemDrop();
    SetCombinationAlwaysCorrect();
    SetTakeNoDamage();
    SetPlayerSpeed();

    //Weapon
    Config.bNoReload = g_FileManager.Config["Weapon"]["bNoReload"].is_boolean() ? g_FileManager.Config["Weapon"]["bNoReload"].get<bool>() : false;
    Config.bNoLaserCannonOverheat = g_FileManager.Config["Weapon"]["bNoLaserCannonOverheat"].is_boolean() ? g_FileManager.Config["Weapon"]["bNoLaserCannonOverheat"].get<bool>() : false;
    Config.bInstantCharge = g_FileManager.Config["Weapon"]["bInstantCharge"].is_boolean() ? g_FileManager.Config["Weapon"]["bInstantCharge"].get<bool>() : false;
    Config.bNoRecoil = g_FileManager.Config["Weapon"]["bNoRecoil"].is_boolean() ? g_FileManager.Config["Weapon"]["bNoRecoil"].get<bool>() : false;
    Config.bNoSway = g_FileManager.Config["Weapon"]["bNoSway"].is_boolean() ? g_FileManager.Config["Weapon"]["bNoSway"].get<bool>() : false;
    Config.bInfSpecialWeapon = g_FileManager.Config["Weapon"]["bInfSpecialWeapon"].is_boolean() ? g_FileManager.Config["Weapon"]["bInfSpecialWeapon"].get<bool>() : false;

    SetNoReload();
    SetNoLaserOverheat();
    SetInstantCharge();
    SetNoSway();

    //Mission
    Config.bNoVaultCheck = g_FileManager.Config["Mission"]["bNoVaultCheck"].is_boolean() ? g_FileManager.Config["Mission"]["bNoVaultCheck"].get<bool>() : false;
    Config.bReduceEnemyAggro = g_FileManager.Config["Mission"]["bReduceEnemyAggro"].is_boolean() ? g_FileManager.Config["Mission"]["bReduceEnemyAggro"].get<bool>() : false;
    Config.bShowAllMapIcons = g_FileManager.Config["Mission"]["bShowAllMapIcons"].is_boolean() ? g_FileManager.Config["Mission"]["bShowAllMapIcons"].get<bool>() : false;
    Config.bNoBoundary = g_FileManager.Config["Mission"]["bNoBoundary"].is_boolean() ? g_FileManager.Config["Mission"]["bNoBoundary"].get<bool>() : false;
    Config.bInstantCompleteOutpost = g_FileManager.Config["Mission"]["bInstantCompleteOutpost"].is_boolean() ? g_FileManager.Config["Mission"]["bInstantCompleteOutpost"].get<bool>() : false;
    Config.bShowHiddenOutpost = g_FileManager.Config["Mission"]["bShowHiddenOutpost"].is_boolean() ? g_FileManager.Config["Mission"]["bShowHiddenOutpost"].get<bool>() : false;
    Config.bInstantCompleteMission = g_FileManager.Config["Mission"]["bInstantCompleteMission"].is_boolean() ? g_FileManager.Config["Mission"]["bInstantCompleteMission"].get<bool>() : false;

    SetNoVaultCheck();
    SetShowMapAllIcons();
    SetNoBoundary();
    InstantCompleteOutpost();
    ShowHiddenOutpost();
    InstantCompleteMission();

    //Misc
    Config.bUnlockAllStratagems = g_FileManager.Config["Misc"]["bUnlockAllStratagems"].is_boolean() ? g_FileManager.Config["Misc"]["bUnlockAllStratagems"].get<bool>() : false;
    Config.bUnlockAllEquipment = g_FileManager.Config["Misc"]["bUnlockAllEquipment"].is_boolean() ? g_FileManager.Config["Misc"]["bUnlockAllEquipment"].get<bool>() : false;
    Config.bUnlockAllArmor = g_FileManager.Config["Misc"]["bUnlockAllArmor"].is_boolean() ? g_FileManager.Config["Misc"]["bUnlockAllArmor"].get<bool>() : false;
    Config.bNoJetpackCooldown = g_FileManager.Config["Misc"]["bNoJetpackCooldown"].is_boolean() ? g_FileManager.Config["Misc"]["bNoJetpackCooldown"].get<bool>() : false;
    Config.bNoStationaryTurretOverheat = g_FileManager.Config["Misc"]["bNoStationaryTurretOverheat"].is_boolean() ? g_FileManager.Config["Misc"]["bNoStationaryTurretOverheat"].get<bool>() : false;
    Config.bBackpackShieldSetting = g_FileManager.Config["Misc"]["bBackpackShieldSetting"].is_boolean() ? g_FileManager.Config["Misc"]["bBackpackShieldSetting"].get<bool>() : false;
    Config.fBackpackShieldCooldownTime = g_FileManager.Config["Misc"]["fBackpackShieldCooldownTime"].is_number_float() ? g_FileManager.Config["Misc"]["fBackpackShieldCooldownTime"].get<float>() : 60.0f;
    Config.fBackpackShieldMaxEnergy = g_FileManager.Config["Misc"]["fBackpackShieldMaxEnergy"].is_number_float() ? g_FileManager.Config["Misc"]["fBackpackShieldMaxEnergy"].get<float>() : 150.0f;

    SetNoJetpackCooldown();
    SetNoStationaryTurretOverheat();
    SetBackpackShieldSetting();
}