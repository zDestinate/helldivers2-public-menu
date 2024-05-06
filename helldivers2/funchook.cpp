#define _CRT_SECURE_NO_WARNINGS

#include "config.h"
#include "memory.h"
#include "funchook.h"
#include "features.h"
#include <algorithm>
#include <ctime>


#ifdef VMPROTECT
#include "libs/VMProtect/VMProtectSDK.h"
#endif

using namespace std;

namespace GameHooking
{
	funchook FuncHook;

	DWORD64 funchook::GetAddress(const char* pattern, const char* szModule)
	{
		uintptr_t TempAddress = 0;
		if (strlen(szModule))
		{
			TempAddress = Memory::FindPattern64(szModule, pattern);
		}
		else
		{
			TempAddress = Memory::FindPattern64("game.dll", pattern);
		}
		
		return (DWORD64)TempAddress;
	}

	void funchook::Hook(bool bAttach, string strName, PVOID pTarget, PVOID pDetour, PVOID* pOriginal)
	{
		if (pTarget)
		{
			if (bAttach)
			{
				MH_STATUS hookstatus = MH_CreateHook(pTarget, pDetour, pOriginal);
				MH_STATUS enablehookstatus = MH_EnableHook(pTarget);
				if (hookstatus == MH_OK || enablehookstatus == MH_OK)
				{
#ifdef DEBUG_CONSOLE
					printf("[FuncHook][Hook] %s Success\n", strName.c_str());
#endif
					return;
				}
#ifdef DEBUG_CONSOLE
				printf("[FuncHook][Hook] %s Failed (%s %s)\n", strName.c_str(), MH_StatusToString(hookstatus), MH_StatusToString(enablehookstatus));
#endif
				return;
			}
			else
			{
				MH_DisableHook(pTarget);
				MH_RemoveHook(pTarget);
			}

			return;
		}

#ifdef DEBUG_CONSOLE
		printf("[FuncHook][Hook] %s Failed (Function not found)\n", strName.c_str());
#endif
	}

	namespace features
	{
		//Player
		static void StaminaReduction(__int64 a1, int a2, float a3)
		{
			if (Config.bInfStamina)
				return;

			FuncHook.oStaminaReduction(a1, a2, a3);
		}

		static __int64 StratagemCooldown(__int64 a1, unsigned int a2, char a3, __int64 a4)
		{
			if (Config.bInfStratagem)
				return 1;

			return FuncHook.oStratagemCooldown(a1, a2, a3, a4);
		}

		static __int64 RagdollCheck(__int64 a1, int a2, int a3, char a4)
		{
			if (Config.bNoRagdoll)
			{
				return 1;
			}

			return FuncHook.oRagdollCheck(a1, a2, a3, a4);
		}

		static __int64 PlayerLocation(__int64 a1, __int64 a2, float a3, __m128* a4)
		{
			__int64 ret = FuncHook.oPlayerLocation(a1, a2, a3, a4);
			if(Config.pPlayerLocationAddress != ret)
			{
				Config.pPlayerLocationAddress = ret;
			}

			return ret;
		}

		static __int64 PlayerCameraRotation(__int64 a1, __int64 a2, __m128* a3, unsigned int* a4)
		{
			uintptr_t pCamAddress = *(uintptr_t*)(a1 + 0x108);
			if (Config.pPlayerCameraRotationAddress != pCamAddress)
			{
				Config.pPlayerCameraRotationAddress = pCamAddress;
			}

			return FuncHook.oPlayerCameraRotation(a1, a2, a3, a4);
		}



		//Weapon
		static unsigned __int64 OnRecoil(__int64* a1, float* a2, float* a3, float a4, char a5)
		{
			if (Config.bNoRecoil)
			{
				return 0;
			}

			return FuncHook.oOnRecoil(a1, a2, a3, a4, a5);
		}

		static __int64 OnAmmoDecrease(__int64 a1, __int64 a2)
		{
			if (Config.bInfSpecialWeapon)
			{
				return 0;
			}

			return FuncHook.oOnAmmoDecrease(a1, a2);
		}



		//Stratagem
		static double UnknownStratagemExtractData(__int64 a1, float a2, unsigned int a3, __int64 a4)
		{
			//Extract players slot addresses
			uintptr_t DataAddress = a4 + 0x198;
			if (!count(Config.pStratagemSlotAddress.begin(), Config.pStratagemSlotAddress.end(), (uintptr_t*)DataAddress))
			{
				if (Config.pStratagemSlotAddress.size() > 4)
				{
					Config.pStratagemSlotAddress.clear();
				}

				Config.pStratagemSlotAddress.push_back((uintptr_t*)DataAddress);
				sort(Config.pStratagemSlotAddress.begin(), Config.pStratagemSlotAddress.end());

				Config.nStratagemPlayerIndex = -1;
				std::strncpy(Config.szStratagemPlayerID, "Not Selected", sizeof(Config.szStratagemPlayerID));
			}


			return FuncHook.oUnknownStratagemExtractData(a1, a2, a3, a4);
		}



		//Mission
		static void UpdateHUDOnMissionHover(__int64 a1)
		{
			int v2 = (unsigned int)*(uintptr_t*)(a1 + 0x23ECC);
			if (v2 != -1)
			{
				BYTE check = *(unsigned __int8*)(*(uintptr_t*)(a1 + 8) + 0x109);
				if (check != 0xFF)
				{
					uintptr_t address = (v2 * 0x23EB8) + a1 + 0x2644;
					
					if (Config.pMissionInfo != address)
					{
						Config.pMissionInfo = address;
						FuncHook.Hook(false, "UpdateHUDOnMissionHover", (LPVOID)FuncHook.Target_UpdateHUDOnMissionHover, features::UpdateHUDOnMissionHover, &(LPVOID&)(FuncHook.oUpdateHUDOnMissionHover));
						FuncHook.Target_UpdateHUDOnMissionHover(a1);
						return;
					}
				}
				
			}	

			FuncHook.oUpdateHUDOnMissionHover(a1);
		}

		static char EnemyAggro(__int64 a1, int a2, __int64 a3)
		{
			if (Config.bReduceEnemyAggro)
				return 0;

			return FuncHook.oEnemyAggro(a1, a2, a3);
		}

		static uintptr_t pTotalSampleAddress = 0;
		static void SamplePickup(__int64 a1, int a2, int SampleValue, int a4)
		{
			if (Config.bPickupCustomSample)
			{
				if (!pTotalSampleAddress)
				{
					pTotalSampleAddress = *(int*)((uintptr_t)FuncHook.Target_SamplePickup + 30) + (uintptr_t)FuncHook.Target_SamplePickup + 34;
					pTotalSampleAddress = *(uintptr_t*)(*(uintptr_t*)pTotalSampleAddress + 0x40) + 4;
				}

				*(int*)(pTotalSampleAddress) = Config.nNormalSample;
				*(int*)(pTotalSampleAddress + 4) = Config.nRareSample;
				*(int*)(pTotalSampleAddress + 8) = Config.nSuperSample;

				FuncHook.oSamplePickup(a1, a2, 0, a4);
				return;
			}
			else if (Config.bPickupSample)
			{
				FuncHook.oSamplePickup(a1, a2, Config.nPickupSample, a4);
				return;
			}
			else
			{
				FuncHook.oSamplePickup(a1, a2, SampleValue, a4);
			}
		}



		//Planet
		static __int64 LoadPlanetData(__int64 a1, __int64 a2, __int64 a3, DWORD* a4)
		{
			if (!Config.pPlanetDataAddress)
			{
				uintptr_t TempAddress = a2 + 624;
				Config.pPlanetDataAddress = (*(uintptr_t*)(TempAddress)) + 8;

				FuncHook.Hook(false, "Load Planet Data", (LPVOID)FuncHook.Target_LoadPlanetData, features::LoadPlanetData, &(LPVOID&)(FuncHook.oLoadPlanetData));
				return FuncHook.Target_LoadPlanetData(a1, a2, a3, a4);
			}

			return FuncHook.oLoadPlanetData(a1, a2, a3, a4);
		}

		static uintptr_t pCurrenSelectedPlanet_Data1 = 0;
		static void CurrentSelectedPlanet(__int64 a1, int a2)
		{
			if (!pCurrenSelectedPlanet_Data1)
			{
				uintptr_t MovAddress = (uintptr_t)FuncHook.Target_CurrentSelectedPlanet + 90;

				unsigned int nMovOffset;
				memcpy(&nMovOffset, (LPVOID)(MovAddress + 3), 4);
				pCurrenSelectedPlanet_Data1 = MovAddress + nMovOffset + 7;

			}

			uintptr_t TempAddress = (284 * a2) + *(uintptr_t*)(pCurrenSelectedPlanet_Data1) + 5028568 + 16;
			if (Config.pSelectedPlanetAddress != TempAddress)
			{
				Config.pSelectedPlanetAddress = TempAddress;
				if (Config.bPlanetListLoaded)
				{
					int PlanetHex = *(uintptr_t*)(TempAddress);
					for (int i = 0; i < Features.PlanetList.size(); i++)
					{
						if (Features.PlanetList[i].HexValue == PlanetHex)
						{
							Config.nPlanetCurrentID = i;
							strcpy(Config.szCurrentPlanetName, Features.PlanetList[i].Name.c_str());
							break;
						}
					}
				}
			}

			FuncHook.oCurrentSelectedPlanet(a1, a2);
		}



		//Data
		static void* GetDamageStats(int a1)
		{
			return FuncHook.oGetDamageStats(a1);
		}

		static void* GetExplosiveStats(int a1)
		{
			return FuncHook.oGetExplosiveStats(a1);
		}



		//Misc
		static bool StratagemUnlock(__int64 a1, int a2)
		{
			if(Config.bUnlockAllStratagems)
				return true;

			return FuncHook.oStratagemUnlockCheck(a1, a2);
		}

		static bool EquipmentUnlock(uint32_t* a1, int a2)
		{
			if (Config.bUnlockAllEquipment)
				return true;

			return FuncHook.oEquipmentUnlockCheck(a1, a2);
		}

		static bool ArmorUnlock(int* a1)
		{
			if (Config.bUnlockAllArmor)
				return true;

			return FuncHook.oArmorUnlockCheck(a1);
		}

		static __int64 OnEnemyReinforcementSpawn(__int64 a1, int EnemyTypeData, __int64 a3, char a4)
		{
			if (Config.nSpawnEntityReinforcementData)
			{
				EnemyTypeData = Config.nSpawnEntityReinforcementData;
			}

			return FuncHook.oOnEnemyReinforcementSpawn(a1, EnemyTypeData, a3, a4);
		}


#ifdef GAME_LOGGING
		static __int64 GameEngineLog(const char* a1, ...)
		{
			char buffer[256];
			va_list args;
			va_start(args, a1);
			vsnprintf(buffer, 255, a1, args);
			va_end(args);

			time_t t = std::time(0);
			char cstr[128];
			strftime(cstr, sizeof(cstr), "%m-%d-%Y %H:%M:%S", std::localtime(&t));

			printf("[%s] %s\n", cstr, buffer);
			return FuncHook.oGameEngineLog(buffer);
		}
#endif


		static __int64 Test(__int64 a1, __int64 a2, float a3, __m128* a4)
		{
			printf("[FuncHook][Test] 0x%llx 0x%llx %f\n", a1, a2, a3);

			__int64 ret = FuncHook.oTest(a1, a2, a3, a4);

			printf("[FuncHook][Test] Ret 0x%llx %lld\n", ret, ret);

			return ret;
		}
	}

	void funchook::Init()
	{
#ifdef VMPROTECT
		VMProtectBeginUltra("funchook_Init");
		if (!VMProtectIsProtected())
		{
			return;
		}
#endif

		//Player
		DWORD64 StaminaReductionAddress = GetAddress("4C 8B DC 55 48 81 EC ? ? ? ? 41 0F 29 7B");
		Target_StaminaReduction = (StaminaReduction_Function)StaminaReductionAddress;

		DWORD64 StratagemCooldownAddress = GetAddress("48 8B C4 44 88 40 ? 53 55 41 54");
		Target_StratagemCooldown = (StratagemCooldown_Function)StratagemCooldownAddress;

		DWORD64 RagdollCheckAddress = GetAddress("40 57 48 83 EC ? 3B 15 ? ? ? ? 41 8B F8 4C 89 7C 24");
		Target_RagdollCheck = (RagdollCheck_Function)RagdollCheckAddress;

		DWORD64 PlayerLocationAddress = GetAddress("48 8B C4 48 89 58 ? 48 89 70 ? F3 0F 11 50 ? 57", "helldivers2.exe");
		Target_PlayerLocation = (PlayerLocation_Function)PlayerLocationAddress;

		DWORD64 PlayerCameraRotationAddress = GetAddress("48 89 5C 24 ? 4C 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8", "helldivers2.exe");
		Target_PlayerCameraRotation = (PlayerCameraRotation_Function)PlayerCameraRotationAddress;


		//Weapon
		DWORD64 OnRecoilAddress = GetAddress("48 83 EC ? 48 8B 01 4C 8B C9");
		Target_OnRecoil = (OnRecoil_Function)OnRecoilAddress;

		DWORD64 OnAmmoDecreaseAddress = GetAddress("48 89 4C 24 ? 53 55 56 57 41 55 41 56 41 57 48 83 EC ? 4C 8B 2D");
		Target_OnAmmoDecrease = (OnAmmoDecrease_Function)OnAmmoDecreaseAddress;


		//Stratagem
		DWORD64 UnknownStratagemExtractDataAddress = GetAddress("4D 85 C9 0F 84 ? ? ? ? 48 8B C4 55");
		Target_UnknownStratagemExtractData = (UnknownStratagemExtractData_Function)UnknownStratagemExtractDataAddress;


		//Mission
		DWORD64 UpdateHUDOnMissionHoverAddress = GetAddress("40 56 41 57 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 83 79");
		Target_UpdateHUDOnMissionHover = (UpdateHUDOnMissionHover_Function)UpdateHUDOnMissionHoverAddress;

		DWORD64 EnemyAggroAddress = GetAddress("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 3B 15 ? ? ? ? 4D 8B F0");
		Target_EnemyAggro = (EnemyAggro_Function)EnemyAggroAddress;

		DWORD64 SamplePickupAddress = GetAddress("45 85 C9 0F 84 ? ? ? ? 53 41 56");
		Target_SamplePickup = (SamplePickup_Function)SamplePickupAddress;


		//Planet
		DWORD64 LoadPlanetDataAddress = GetAddress("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC ? 49 8B F1");
		Target_LoadPlanetData = (LoadPlanetData_Function)LoadPlanetDataAddress;

		DWORD64 CurrentSelectedPlanetAddress = GetAddress("4C 8B DC 55 56 57 49 8D 6B ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24");
		Target_CurrentSelectedPlanet = (CurrentSelectedPlanet_Function)CurrentSelectedPlanetAddress;


		//Data
		DWORD64 GetDamageStatsAddress = GetAddress("85 ? 75 ? 48 8D ? ? ? ? ? C3 8B ? 48 8D ? ? ? ? ? 48 8B ? ? C3 ? ? ? ? ? ? 8B ? 48 8D ? ? 48 8D");
		Target_GetDamageStats = (GetDamageStats_Function)GetDamageStatsAddress;

		DWORD64 GetExplosiveStatsAddress = GetAddress("85 ? 75 ? 48 8D ? ? ? ? ? C3 8B ? 48 8D ? ? ? ? ? 48 8B ? ? C3 ? ? ? ? ? ? 8D");
		Target_GetExplosiveStats = (GetExplosiveStats_Function)GetExplosiveStatsAddress;


		//Misc
		DWORD64 StratagemUnlockCheckAddress = GetAddress("48 89 5C 24 ? 48 8B D9 85 D2");
		Target_StratagemUnlockCheck = (StratagemUnlockCheck_Function)StratagemUnlockCheckAddress;

		DWORD64 EquipmentUnlockCheckAddress = GetAddress("83 B9 ? ? ? ? ? 76 ? 85 D2 74 ? 44 8B 89 ? ? ? ? 45 33 C0 45 85 C9 74 ? 48 8D 81 ? ? ? ? 39 50 ? 74 ? 41 FF C0 48 83 C0 ? 45 3B C1 72 ? 32 C0 C3 8B 00 48 69 C8");
		Target_EquipmentUnlockCheck = (EquipmentUnlockCheck_Function)EquipmentUnlockCheckAddress;
		
		//Load armor function AoB
		//48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4C 8D B1
		//Including armor passive skills
		DWORD64 ArmorUnlockCheckAddress = GetAddress("48 83 EC ? 44 8B 49 ? 45 33 C0 8B 01");
		Target_ArmorUnlockCheck = (ArmorUnlockCheck_Function)ArmorUnlockCheckAddress;

		DWORD64 OnEnemyReinforcementSpawnAddress = GetAddress("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 45 0F B6 E9");
		Target_OnEnemyReinforcementSpawn = (OnEnemyReinforcementSpawn_Function)OnEnemyReinforcementSpawnAddress;



		//GameEngine
#ifdef GAME_LOGGING
		DWORD64 GameEngineLogAddress = GetAddress("48 89 4C 24 ? 48 89 54 24 ? 4C 89 44 24 ? 4C 89 4C 24 ? 53 56 57 48 83 EC ? 65 48 8B 04 25", "helldivers2.exe");
		Target_GameEngineLog = (GameEngineLog_Function)GameEngineLogAddress;
#endif


		//DWORD64 TestAddress = GetAddress("48 8B C4 44 88 40 ? 53 55 41 54");
		//oTest = (Test_Function)TestAddress;


		MH_Initialize();

		//Player
		Hook(true, "Stamina Reduction", (LPVOID)Target_StaminaReduction, features::StaminaReduction, &(LPVOID&)(oStaminaReduction));
		Hook(true, "Stratagem Cooldown", (LPVOID)Target_StratagemCooldown, features::StratagemCooldown, &(LPVOID&)(oStratagemCooldown));
		Hook(true, "RagdollCheck", (LPVOID)Target_RagdollCheck, features::RagdollCheck, &(LPVOID&)(oRagdollCheck));
		if (Config.bEnablePlayerCoords)
		{
			Hook(true, "Player Location", (LPVOID)Target_PlayerLocation, features::PlayerLocation, &(LPVOID&)(oPlayerLocation));
			Hook(true, "Player Camera Rotation", (LPVOID)Target_PlayerCameraRotation, features::PlayerCameraRotation, &(LPVOID&)(oPlayerCameraRotation));
		}

		//Weapon
		Hook(true, "OnRecoil", (LPVOID)Target_OnRecoil, features::OnRecoil, &(LPVOID&)(oOnRecoil));
		Hook(true, "OnAmmoDecrease", (LPVOID)Target_OnAmmoDecrease, features::OnAmmoDecrease, &(LPVOID&)(oOnAmmoDecrease));

		//Stratagem
		Hook(true, "Stratagem Data Extraction", (LPVOID)Target_UnknownStratagemExtractData, features::UnknownStratagemExtractData, &(LPVOID&)(oUnknownStratagemExtractData));

		//Mission
		Hook(true, "UpdateHUDOnMissionHover", (LPVOID)Target_UpdateHUDOnMissionHover, features::UpdateHUDOnMissionHover, &(LPVOID&)(oUpdateHUDOnMissionHover));
		Hook(true, "Enemy Aggro", (LPVOID)Target_EnemyAggro, features::EnemyAggro, &(LPVOID&)(oEnemyAggro));
		Hook(true, "Sample Pickup", (LPVOID)Target_SamplePickup, features::SamplePickup, &(LPVOID&)(oSamplePickup));

		//Planet
		Hook(true, "Load Planet Data", (LPVOID)Target_LoadPlanetData, features::LoadPlanetData, &(LPVOID&)(oLoadPlanetData));
		Hook(true, "Current Selected Planet", (LPVOID)Target_CurrentSelectedPlanet, features::CurrentSelectedPlanet, &(LPVOID&)(oCurrentSelectedPlanet));

		//Data
		Hook(true, "GetDamageStats", (LPVOID)Target_GetDamageStats, features::GetDamageStats, &(LPVOID&)(oGetDamageStats));
		Hook(true, "GetExplosiveStats", (LPVOID)Target_GetExplosiveStats, features::GetExplosiveStats, &(LPVOID&)(oGetExplosiveStats));

		//Misc
		Hook(true, "Stratagem Unlock", (LPVOID)Target_StratagemUnlockCheck, features::StratagemUnlock, &(LPVOID&)(oStratagemUnlockCheck));
		Hook(true, "Equipment Unlock", (LPVOID)Target_EquipmentUnlockCheck, features::EquipmentUnlock, &(LPVOID&)(oEquipmentUnlockCheck));
		Hook(true, "Armor Unlock", (LPVOID)Target_ArmorUnlockCheck, features::ArmorUnlock, &(LPVOID&)(oArmorUnlockCheck));
		Hook(true, "OnEnemyReinforcementSpawn", (LPVOID)Target_OnEnemyReinforcementSpawn, features::OnEnemyReinforcementSpawn, &(LPVOID&)(oOnEnemyReinforcementSpawn));

		//GameEngine
#ifdef GAME_LOGGING
		Hook(true, "Game Engine Logging", (LPVOID)Target_GameEngineLog, features::GameEngineLog, &(LPVOID&)(oGameEngineLog));
#endif

		//Hook(true, "Test", (LPVOID)Target_Test, features::Test, &(LPVOID&)(oTest));

#ifdef VMPROTECT
		VMProtectEnd();
#endif
	}

	void funchook::Detach()
	{
#ifdef VMPROTECT
		VMProtectBeginUltra("funchook_Detach");
		if (!VMProtectIsProtected())
		{
			return;
		}
#endif

		//Player
		Hook(false, "Stamina Reduction", (LPVOID)Target_StaminaReduction, features::StaminaReduction, &(LPVOID&)(oStaminaReduction));
		Hook(false, "Stratagem Cooldown", (LPVOID)Target_StratagemCooldown, features::StratagemCooldown, &(LPVOID&)(oStratagemCooldown));
		Hook(false, "RagdollCheck", (LPVOID)Target_RagdollCheck, features::RagdollCheck, &(LPVOID&)(oRagdollCheck));
		if (Config.bEnablePlayerCoords)
		{
			Hook(false, "Player Location", (LPVOID)Target_PlayerLocation, features::PlayerLocation, &(LPVOID&)(oPlayerLocation));
			Hook(false, "Player Camera Rotation", (LPVOID)Target_PlayerCameraRotation, features::PlayerCameraRotation, &(LPVOID&)(oPlayerCameraRotation));
		}
		

		//Weapon
		Hook(false, "OnRecoil", (LPVOID)Target_OnRecoil, features::OnRecoil, &(LPVOID&)(oOnRecoil));
		Hook(false, "OnAmmoDecrease", (LPVOID)Target_OnAmmoDecrease, features::OnAmmoDecrease, &(LPVOID&)(oOnAmmoDecrease));

		//Stratagem
		Hook(false, "Stratagem Data Extraction", (LPVOID)Target_UnknownStratagemExtractData, features::UnknownStratagemExtractData, &(LPVOID&)(oUnknownStratagemExtractData));

		//Mission
		Hook(false, "UpdateHUDOnMissionHover", (LPVOID)Target_UpdateHUDOnMissionHover, features::UpdateHUDOnMissionHover, &(LPVOID&)(oUpdateHUDOnMissionHover));
		Hook(false, "Enemy Aggro", (LPVOID)Target_EnemyAggro, features::EnemyAggro, &(LPVOID&)(oEnemyAggro));
		Hook(false, "Sample Pickup", (LPVOID)Target_SamplePickup, features::SamplePickup, &(LPVOID&)(oSamplePickup));

		//Planet
		Hook(false, "Load Planet Data", (LPVOID)Target_LoadPlanetData, features::LoadPlanetData, &(LPVOID&)(oLoadPlanetData));
		Hook(false, "Current Selected Planet", (LPVOID)Target_CurrentSelectedPlanet, features::CurrentSelectedPlanet, &(LPVOID&)(oCurrentSelectedPlanet));

		//Data
		Hook(false, "GetDamageStats", (LPVOID)Target_GetDamageStats, features::GetDamageStats, &(LPVOID&)(oGetDamageStats));
		Hook(false, "GetExplosiveStats", (LPVOID)Target_GetExplosiveStats, features::GetExplosiveStats, &(LPVOID&)(oGetExplosiveStats));

		//Misc
		Hook(false, "Stratagem Unlock", (LPVOID)Target_StratagemUnlockCheck, features::StratagemUnlock, &(LPVOID&)(oStratagemUnlockCheck));
		Hook(false, "Equipment Unlock", (LPVOID)Target_EquipmentUnlockCheck, features::EquipmentUnlock, &(LPVOID&)(oEquipmentUnlockCheck));
		Hook(false, "Armor Unlock", (LPVOID)Target_ArmorUnlockCheck, features::ArmorUnlock, &(LPVOID&)(oArmorUnlockCheck));
		Hook(false, "OnEnemyReinforcementSpawn", (LPVOID)Target_OnEnemyReinforcementSpawn, features::OnEnemyReinforcementSpawn, &(LPVOID&)(oOnEnemyReinforcementSpawn));

		//GameEngine
#ifdef GAME_LOGGING
		Hook(false, "Game Engine Logging", (LPVOID)Target_GameEngineLog, features::GameEngineLog, &(LPVOID&)(oGameEngineLog));
#endif

#ifdef VMPROTECT
		VMProtectEnd();
#endif
	}
}



