#pragma once
#include <Windows.h>
#include <vector>

//#define DEBUG_CONSOLE
//#define GAME_LOGGING

class config
{
public:
	//offsets
	DWORD64 ClientBase = 0;

	//Player
	bool bInfStamina = false;
	bool bInfAmmo = false;
	bool bInfAmmo_Legit = false;
	bool bInfGrenade = false;
	bool bInfGrenade_Legit = false;
	bool bInfStim = false;
	bool bInfStim_Legit = false;
	bool bInfBackpack = false;
	bool bInfStratagem = false;
	bool bNoRagdoll = false;
	bool bInstantStratagemDrop = false;
	bool bCombinationAlwaysCorrect = false;
	bool bInstantStandupFromFall = false;
	bool bTakeNoDamage = false;
	int nPlayerMinHealth = 0;
	float fPlayerSpeed = 1.0f;
	bool bEnablePlayerCoords = false;
	bool bNoClip = false;
	uintptr_t pPlayerCameraRotationAddress = 0;
	uintptr_t pPlayerLocationAddress = 0;
	float fPlayerLocationX = 0;
	float fPlayerLocationY = 0;
	float fPlayerLocationZ = 0;

	//Weapon
	bool bNoReload = false;
	bool bNoLaserCannonOverheat = false;
	bool bInstantCharge = false;
	bool bNoRecoil = false;
	bool bNoSway = false;
	bool bInfSpecialWeapon = false;
	bool bWeaponDataLoaded = false;
	int nTotalWeaponID = 0;
	int nTotalWeaponGrenadeID = 0;
	int nWeaponLoadoutPrimaryID = 1;
	int nWeaponLoadoutSecondaryID = 1;
	int nWeaponLoadoutGrenadeID = 1;

	//Stratagems
	std::vector<uintptr_t*>pStratagemSlotAddress;
	bool bStratagemListLoaded = false;
	int nStratagemPlayerIndex = -1;
	char szStratagemPlayerID[255] = "Not Selected";
	char szStratagemFilter[255] = "";

	//Mission
	uintptr_t pMissionInfo = 0;
	bool bNoVaultCheck = false;
	bool bReduceEnemyAggro = false;
	bool bShowAllMapIcons = false;
	bool bNoBoundary = false;
	bool bInstantCompleteOutpost = false;
	bool bShowHiddenOutpost = false;
	bool bInstantCompleteMission = false;
	bool bPickupCustomSample = false;
	int nNormalSample = 0;
	int nRareSample = 0;
	int nSuperSample = 0;
	bool bPickupSample = false;
	int nPickupSample = 0;
	float fCustomMissionTime = 2400.0f;
	float fMaxExtractionTime = 1.0f;

	//Planet
	uintptr_t pPlanetDataAddress = 0;
	uintptr_t pSelectedPlanetAddress = 0;
	bool bPlanetListLoaded = false;
	char szCurrentPlanetName[255] = "Unknown";
	int nPlanetCurrentID = -1;
	int nPlanetReplaceID = 0;
	char szPlanetFilter[255] = "";

	//Data
	char szWeaponDamageIDName[255] = "Data is not loaded";
	int nDamageNameListLoaded = 2;
	int nTotalDamageID = 0;
	int nWeaponDamageID = 1;
	int nWeaponDamage = 0;
	int nWeaponStructureDamage = 0;
	int nWeaponPenetrationPower1 = 0;
	int nWeaponPenetrationPower2 = 0;
	int nWeaponPenetrationPower3 = 0;
	char szWeaponExplosiveIDName[255] = "Data is not loaded";
	int nExplosiveNameListLoaded = 2;
	int nTotalExplosiveID = 0;
	int nWeaponExplosiveID = 1;
	int nWeaponExplosiveDamageID = 0;
	float fWeaponExplosiveRadiusInner = 0;
	float fWeaponExplosiveRadiusFalloff = 0;

	//Misc
	bool bUnlockAllStratagems = false;
	bool bUnlockAllEquipment = false;
	bool bUnlockAllArmor = false;
	bool bNoJetpackCooldown = false;
	bool bNoStationaryTurretOverheat = false;
	bool bBackpackShieldSetting = false;
	float fBackpackShieldCooldownTime = 60.0f;
	float fBackpackShieldMaxEnergy = 150.0f;
	char szSpawnEntityReinforcementName[255] = "Default";
	uint32_t nSpawnEntityReinforcementData = 0;
	int nArmorRating = 100;
	int nArmorSpeed = 500;
	int nArmorStaminaRegen = 100;

	//static function
	static void Init();

};
extern config Config;