#pragma once
#include "libs/MinHook/MinHook.h"
#include <xmmintrin.h>


namespace GameHooking
{
	class funchook
	{
	public:
		DWORD64 GetAddress(const char* pattern, const char* szModule = "");
		void Hook(bool bAttach, std::string strName, PVOID pTarget, PVOID pDetour, PVOID* pOriginal);

		//Player
		typedef void(__fastcall* StaminaReduction_Function)(__int64 a1, int a2, float a3);
		StaminaReduction_Function Target_StaminaReduction = nullptr;
		StaminaReduction_Function oStaminaReduction = nullptr;

		typedef __int64(__fastcall* StratagemCooldown_Function)(__int64 a1, unsigned int a2, char a3, __int64 a4);
		StratagemCooldown_Function Target_StratagemCooldown = nullptr;
		StratagemCooldown_Function oStratagemCooldown = nullptr;

		typedef __int64(__fastcall* RagdollCheck_Function)(__int64 a1, int a2, int a3, char a4);
		RagdollCheck_Function Target_RagdollCheck = nullptr;
		RagdollCheck_Function oRagdollCheck = nullptr;

		typedef __int64(__fastcall* PlayerLocation_Function)(__int64 a1, __int64 a2, float a3, __m128* a4);
		PlayerLocation_Function Target_PlayerLocation = nullptr;
		PlayerLocation_Function oPlayerLocation = nullptr;

		typedef __int64(__fastcall* PlayerCameraRotation_Function)(__int64 a1, __int64 a2, __m128* a3, unsigned int* a4);
		PlayerCameraRotation_Function Target_PlayerCameraRotation = nullptr;
		PlayerCameraRotation_Function oPlayerCameraRotation = nullptr;



		//Weapon
		typedef unsigned __int64(__fastcall* OnRecoil_Function)(__int64 *a1, float *a2, float *a3, float a4, char a5);
		OnRecoil_Function Target_OnRecoil = nullptr;
		OnRecoil_Function oOnRecoil = nullptr;

		typedef __int64(__fastcall* OnAmmoDecrease_Function)(__int64 a1, __int64 a2);
		OnAmmoDecrease_Function Target_OnAmmoDecrease = nullptr;
		OnAmmoDecrease_Function oOnAmmoDecrease = nullptr;



		//Stratagem
		typedef double(__fastcall* UnknownStratagemExtractData_Function)(__int64 a1, float a2, unsigned int a3, __int64 a4);
		UnknownStratagemExtractData_Function Target_UnknownStratagemExtractData = nullptr;
		UnknownStratagemExtractData_Function oUnknownStratagemExtractData = nullptr;


		
		//Mission
		typedef void(__fastcall* UpdateHUDOnMissionHover_Function)(__int64 a1);
		UpdateHUDOnMissionHover_Function Target_UpdateHUDOnMissionHover = nullptr;
		UpdateHUDOnMissionHover_Function oUpdateHUDOnMissionHover = nullptr;

		typedef char(__fastcall* EnemyAggro_Function)(__int64 a1, int a2, __int64 a3);
		EnemyAggro_Function Target_EnemyAggro = nullptr;
		EnemyAggro_Function oEnemyAggro = nullptr;

		typedef void(__fastcall* SamplePickup_Function)(__int64 a1, int a2, int a3, int a4);
		SamplePickup_Function Target_SamplePickup = nullptr;
		SamplePickup_Function oSamplePickup = nullptr;



		//Planet
		typedef __int64(__fastcall* LoadPlanetData_Function)(__int64 a1, __int64 a2, __int64 a3, DWORD* a4);
		LoadPlanetData_Function Target_LoadPlanetData = nullptr;
		LoadPlanetData_Function oLoadPlanetData = nullptr;

		typedef void(__fastcall* CurrentSelectedPlanet_Function)(__int64 a1, int a2);
		CurrentSelectedPlanet_Function Target_CurrentSelectedPlanet = nullptr;
		CurrentSelectedPlanet_Function oCurrentSelectedPlanet = nullptr;



		//Data
		typedef void* (__fastcall* GetDamageStats_Function)(int a1);
		GetDamageStats_Function Target_GetDamageStats = nullptr;
		GetDamageStats_Function oGetDamageStats = nullptr;

		typedef void* (__fastcall* GetExplosiveStats_Function)(int a1);
		GetExplosiveStats_Function Target_GetExplosiveStats = nullptr;
		GetExplosiveStats_Function oGetExplosiveStats = nullptr;


		//Misc
		typedef bool(__fastcall* StratagemUnlockCheck_Function)(__int64 a1, int a2);
		StratagemUnlockCheck_Function Target_StratagemUnlockCheck = nullptr;
		StratagemUnlockCheck_Function oStratagemUnlockCheck = nullptr;

		typedef bool(__fastcall* EquipmentUnlockCheck_Function)(uint32_t* a1, int a2);
		EquipmentUnlockCheck_Function Target_EquipmentUnlockCheck = nullptr;
		EquipmentUnlockCheck_Function oEquipmentUnlockCheck = nullptr;

		typedef bool(__fastcall* ArmorUnlockCheck_Function)(int* a1);
		ArmorUnlockCheck_Function Target_ArmorUnlockCheck = nullptr;
		ArmorUnlockCheck_Function oArmorUnlockCheck = nullptr;

		typedef __int64(__fastcall* OnEnemyReinforcementSpawn_Function)(__int64 a1, int a2, __int64 a3, char a4);
		OnEnemyReinforcementSpawn_Function Target_OnEnemyReinforcementSpawn = nullptr;
		OnEnemyReinforcementSpawn_Function oOnEnemyReinforcementSpawn = nullptr;



		//GameEngine
#ifdef GAME_LOGGING
		typedef __int64(*GameEngineLog_Function)(const char* a1, ...);
		GameEngineLog_Function Target_GameEngineLog = nullptr;
		GameEngineLog_Function oGameEngineLog = nullptr;
#endif


		typedef __int64(__fastcall* Test_Function)(__int64 a1, __int64 a2, float a3, __m128* a4);
		Test_Function Target_Test = nullptr;
		Test_Function oTest = nullptr;


		void Init();
		void Detach();
	};

	extern funchook FuncHook;
};