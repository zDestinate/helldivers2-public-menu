#pragma once

#include "pch.h"
#include "config.h"
#include "memory.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <map>


struct FeatureData
{
	uintptr_t Address = 0;
	LPVOID AllocMemoryAddress = 0;
	bool State = false;
	int Type = -1;
	bool bInitialized = false;
	uint8_t JmpBytes = 0;
	BYTE* OriginalBytes = nullptr;
	unsigned int OriginalBytesSize = 0;
	float fValue = 1.0f;
	int nValue = 1;
};

struct Vector3D_Data
{
	float X;
	float Z;
	float Y;
};

struct PlayerLoadout_Data
{
	int WeaponPrimary;
	int WeaponSecondary;
	int Grenade;
};

struct PlayerArmorLoadoutStats_Data
{
	float Armor;
	float Speed;
	float StaminaRegen;
	int PassiveSkillData;
};

struct Weapon_Data
{
	int Data;
	uint64_t Asset;
	int LoadoutItemType;
};

struct Stratagem_Data
{
	int HexValue;
	std::string Name;
};

struct Stratagem_Filter_Data
{
	int ID;
	std::string Name;
};

struct Stratagem_PlayerSlot_Data
{
	int OriginalID = -1;
	int NewID = -1;
	int Amount = -1;
};

struct Planet_Data
{
	int HexValue;
	std::string Name;
};

struct Planet_Filter_Data
{
	int ID;
	std::string Name;
};

struct EnemyEntity_Data
{
	uint32_t Data;
	std::string Name;
};

class features
{
private:
	std::map<std::string, FeatureData> FeatureList;
	void CheckFeatureExist(const std::string key);
	uintptr_t GetAddress(const std::string key, const char* pattern, unsigned int offset = 0);

	void AllocMemory(std::string key, const char* pattern, MemoryType type, unsigned int offset = 0, unsigned int return_offset = 0, int size = 0);

public:
	features();
	~features();
	void Alloc();

	//Player
	void SetInfAmmo();
	void SetInfAmmo_Legit();
	void SetInfGrenade();
	void SetInfGrenade_Legit();
	void SetInfStim();
	void SetInfStim_Legit();
	void SetInfBackpack();
	void SetInstantStratagemDrop();
	void SetCombinationAlwaysCorrect();
	void SetInstantStandupFromFall();
	void SetTakeNoDamage();
	void SetFreezePlayerHealth();
	void SetUnfreezePlayerHealth();
	void SetPlayerSpeed();
	void PlayerTeleport();
	void PlayerCopyLocation();
	void PlayerTeleportWaypoint();

	//Weapon
	void SetNoReload();
	void SetNoLaserOverheat();
	void SetInstantCharge();
	void SetNoSway();
	PlayerLoadout_Data* PlayerLoadout = nullptr;
	std::vector<Weapon_Data> WeaponDataList;
	std::vector<Weapon_Data> WeaponGrenadeDataList;
	void LoadWeaponDataList();
	void SetWeaponLoadout(int Type);

	//Stratagem
	std::vector<Stratagem_Data> StratagemList;
	std::vector<Stratagem_Filter_Data> FilteredStratagemList;
	std::vector<Stratagem_PlayerSlot_Data> StratagemPlayerSlotData;
	void LoadStratagemList();
	void FilterStratagemList();
	void ReplaceStratagem();
	void LoadPlayerStratagem();

	//Mission
	void SetNoVaultCheck();
	void SetShowMapAllIcons();
	void SetNoBoundary();
	void InstantCompleteOutpost();
	void ShowHiddenOutpost();
	void InstantCompleteMission();
	void SetCustomMissionTime();
	void UnfreezeCustomMissionTime();
	void SetMaxExtractionTime();
	void UnfreezeExtractionTime();

	//Planet
	std::map<int, std::string> PlanetNameList;
	std::vector<Planet_Data> PlanetList;
	std::vector<Planet_Filter_Data> FilteredPlanetList;
	void LoadPlanetList();
	void FilterPlanetList();
	void ReplacePlanet();

	//Data
	std::vector<std::string> WeaponDamageIDNameList;
	void LoadWeaponStats();
	void SetWeaponStats();
	void LoadWeaponDamageNameData();
	void WeaponDamageSaveID();
	void WeaponDamageLoadConfig();
	void WeaponDamageClearAll();
	std::vector<std::string> WeaponExplosiveIDNameList;
	void LoadExplosiveStats();
	void SetExplosiveStats();
	void LoadWeaponExplosiveNameData();
	void WeaponExplosiveSaveID();
	void WeaponExplosiveLoadConfig();
	void WeaponExplosiveClearAll();

	//Misc
	void SetNoJetpackCooldown();
	void SetNoStationaryTurretOverheat();
	void SetBackpackShieldSetting();
	std::vector<EnemyEntity_Data> EnemyEntityList;
	void InitEnemyEntityList();
	void SaveAllConfig();
	void LoadAllConfig();
};
extern features Features;