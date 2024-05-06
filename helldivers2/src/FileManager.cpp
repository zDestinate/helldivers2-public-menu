#include "../pch.h"
#include "../include/FileManager.h"
#include "../config.h"
#include "../include/util.h"
#include <fstream>

FileManager g_FileManager;

using namespace std;

void FileManager::Init()
{
    TCHAR szProcPath[MAX_PATH];
    GetModuleFileNameW(NULL, szProcPath, MAX_PATH);

	//Check for process name
	/*
	wstring ProcessName = szProcPath;
	if (ProcessName.find(L"\\helldivers2.exe") == wstring::npos)
	{
		exit(1);
	}
	*/

    GetCurrentDirectoryW(MAX_PATH, szProcPath);
    ProcessPath = szProcPath;
	//In case its in the bin folder
	string::size_type pos = ProcessPath.find(L"\\bin");
	if (pos != string::npos)
	{
		ProcessPath = ProcessPath.substr(0, pos);
	}

#ifdef DEBUG_CONSOLE
	printf("[FileManager][Init] Initialized\n");
#endif

	LoadConfig(L"config.json");
	LoadLanguage(L"localization.json");
}

void FileManager::LoadConfig(wstring strConfigFileName)
{
	if (ProcessPath.empty())
	{
		return;
	}

	ConfigPath = ProcessPath + L"\\" + strConfigFileName;

	Config.clear();

	//Check if the file exists
	if (!ifstream(ConfigPath))
	{
		Config["DLLDelay"] = 0;
		Config["MenuKeybind"] = VK_DELETE;
		Config["Preload"]["bEnablePlayerCoords"] = true;
		SaveFile(strConfigFileName, Config);
	}

	//Read the file
	ifstream file(ConfigPath);
	if (!file.is_open())
	{
#ifdef DEBUG_CONSOLE
		printf("[FileManager][LoadConfig] Unable to open config file\n");
#endif
		exit(1);
	}

	try
	{
		this->Config = json::parse(file);
		file.close();
	}
	catch (const exception& ex)
	{
		file.close();

#ifdef DEBUG_CONSOLE
		printf("[FileManager][LoadConfig] Failed to parse config file. Exception: %s\n", ex.what());
#endif
		//Create new config
		Config.clear();
		Config["DLLDelay"] = 0;
		Config["MenuKeybind"] = VK_DELETE;
		Config["Preload"]["bEnablePlayerCoords"] = true;
		SaveFile(strConfigFileName, Config);
	}

	::Config.bEnablePlayerCoords = Config["Preload"]["bEnablePlayerCoords"].is_boolean() ? Config["Preload"]["bEnablePlayerCoords"].get<bool>() : true;

#ifdef DEBUG_CONSOLE
	printf("[FileManager][LoadConfig] Loaded\n");
#endif
}

void FileManager::SaveFile(wstring strFileName, json Data)
{
	if (ProcessPath.empty())
	{
		return;
	}

	wstring strFilePath = ProcessPath + L"\\" + strFileName;

	ofstream file(strFilePath);
	if (!file)
	{
#ifdef DEBUG_CONSOLE
		printf("[FileManager][SaveFile] Failed to save file %ls\n", strFileName.c_str());
#endif
		exit(1);
		return;
	}

	file << Data.dump(4) << endl;
	file.close();

#ifdef DEBUG_CONSOLE
	printf("[FileManager][SaveFile] Save file %ls successful\n", strFileName.c_str());
#endif
}

void FileManager::LoadLanguage(std::wstring strLanguageFileName)
{
#ifdef VMPROTECT
	VMProtectBeginMutation("LoadLanguage");
	if (!VMProtectIsProtected())
	{
		return;
	}
#endif

	if (ProcessPath.empty())
	{
		return;
	}

	LanguagePath = ProcessPath + L"\\localization\\";
	wstring LanguageConfigPath = LanguagePath + strLanguageFileName;

	Language.clear();

	//Check if the language config file exists
	ifstream file(LanguageConfigPath);
	if (file.is_open())
	{
		try
		{
			Language = json::parse(file);
			file.close();

#ifdef DEBUG_CONSOLE
			printf("[FileManager][Language] Loaded successfully\n");
#endif
		}
		catch (const exception& ex)
		{
			file.close();
			Language.clear();
#ifdef DEBUG_CONSOLE
			printf("[FileManager][Language] Failed to parse config file. Exception: %s\n", ex.what());
#endif
		}
	}

	//Font file and size
	Language["Font"] = Language["Font"].is_string() ? Language["Font"].get<string>() : "";
	Language["FontSize"] = Language["FontSize"].is_number_float() ? Language["FontSize"].get<float>() : 15.0f;

	//Language
	json& Localization = Language["Localization"];

	//Tabs
	Localization["Tab"][0] = Localization["Tab"][0].is_string() ? Localization["Tab"][0] : "Player";
	Localization["Tab"][1] = Localization["Tab"][1].is_string() ? Localization["Tab"][1] : "Weapon";
	Localization["Tab"][2] = Localization["Tab"][2].is_string() ? Localization["Tab"][2] : "Stratagems";
	Localization["Tab"][3] = Localization["Tab"][3].is_string() ? Localization["Tab"][3] : "Mission";
	Localization["Tab"][4] = Localization["Tab"][4].is_string() ? Localization["Tab"][4] : "Planets";
	Localization["Tab"][5] = Localization["Tab"][5].is_string() ? Localization["Tab"][5] : "Data";
	Localization["Tab"][6] = Localization["Tab"][6].is_string() ? Localization["Tab"][6] : "Misc";
	Localization["Tab"][7] = Localization["Tab"][7].is_string() ? Localization["Tab"][7] : "About";

	//Player
	Localization["Player"][0] = Localization["Player"][0].is_string() ? Localization["Player"][0] : "Infinite Stamina";
	Localization["Player"][1] = Localization["Player"][1].is_string() ? Localization["Player"][1] : "Infinite Ammo";
	Localization["Player"][2] = Localization["Player"][2].is_string() ? Localization["Player"][2] : "Infinite Grenade";
	Localization["Player"][3] = Localization["Player"][3].is_string() ? Localization["Player"][3] : "Infinite Stim";
	Localization["Player"][4] = Localization["Player"][4].is_string() ? Localization["Player"][4] : "Infinite Ammo (Legit)";
	Localization["Player"][5] = Localization["Player"][5].is_string() ? Localization["Player"][5] : "Infinite Grenade (Legit)";
	Localization["Player"][6] = Localization["Player"][6].is_string() ? Localization["Player"][6] : "Infinite Stim (Legit)";
	Localization["Player"][7] = Localization["Player"][7].is_string() ? Localization["Player"][7] : "Infinite Backpack";
	Localization["Player"][8] = Localization["Player"][8].is_string() ? Localization["Player"][8] : "Infinite Stratagems";
	Localization["Player"][9] = Localization["Player"][9].is_string() ? Localization["Player"][9] : "Combination Always Correct";
	Localization["Player"][10] = Localization["Player"][10].is_string() ? Localization["Player"][10] : "No Ragdoll";
	Localization["Player"][11] = Localization["Player"][11].is_string() ? Localization["Player"][11] : "Instant Stratagem Drop";
	Localization["Player"][12] = Localization["Player"][12].is_string() ? Localization["Player"][12] : "Take No Damage";
	Localization["Player"][13] = Localization["Player"][13].is_string() ? Localization["Player"][13] : "Set & Freeze Health Value";
	Localization["Player"][14] = Localization["Player"][14].is_string() ? Localization["Player"][14] : "Unfreeze Health Value";
	Localization["Player"][15] = Localization["Player"][15].is_string() ? Localization["Player"][15] : "Set Speed";
	Localization["Player"][16] = Localization["Player"][16].is_string() ? Localization["Player"][16] : "No Clip";
	Localization["Player"][17] = Localization["Player"][17].is_string() ? Localization["Player"][17] : "TP Location";
	Localization["Player"][18] = Localization["Player"][18].is_string() ? Localization["Player"][18] : "Copy Location";
	Localization["Player"][19] = Localization["Player"][19].is_string() ? Localization["Player"][19] : "TP Waypoint";

	//Weapon
	Localization["Weapon"][0] = Localization["Weapon"][0].is_string() ? Localization["Weapon"][0] : "No Reload";
	Localization["Weapon"][1] = Localization["Weapon"][1].is_string() ? Localization["Weapon"][1] : "No Laser Cannon Overheat";
	Localization["Weapon"][2] = Localization["Weapon"][2].is_string() ? Localization["Weapon"][2] : "Instant Charge";
	Localization["Weapon"][3] = Localization["Weapon"][3].is_string() ? Localization["Weapon"][3] : "No Recoil";
	Localization["Weapon"][4] = Localization["Weapon"][4].is_string() ? Localization["Weapon"][4] : "No Sway";
	Localization["Weapon"][5] = Localization["Weapon"][5].is_string() ? Localization["Weapon"][5] : "Infinite Special Weapon";
	Localization["Weapon"][6] = Localization["Weapon"][6].is_string() ? Localization["Weapon"][6] : "Primary (Weapon ID)";
	Localization["Weapon"][7] = Localization["Weapon"][7].is_string() ? Localization["Weapon"][7] : "Change Primary Weapon";
	Localization["Weapon"][8] = Localization["Weapon"][8].is_string() ? Localization["Weapon"][8] : "Secondary (Weapon ID)";
	Localization["Weapon"][9] = Localization["Weapon"][9].is_string() ? Localization["Weapon"][9] : "Change Secondary Weapon";
	Localization["Weapon"][10] = Localization["Weapon"][10].is_string() ? Localization["Weapon"][10] : "Grenade (Grenade ID)";
	Localization["Weapon"][11] = Localization["Weapon"][11].is_string() ? Localization["Weapon"][11] : "Change Grenade";
	Localization["Weapon"][12] = Localization["Weapon"][12].is_string() ? Localization["Weapon"][12] : "Load Weapon Data List";

	//Stratagems
	Localization["Stratagems"][0] = Localization["Stratagems"][0].is_string() ? Localization["Stratagems"][0] : "Select Player";
	Localization["Stratagems"][1] = Localization["Stratagems"][1].is_string() ? Localization["Stratagems"][1] : "Player";
	Localization["Stratagems"][2] = Localization["Stratagems"][2].is_string() ? Localization["Stratagems"][2] : "Original ID";
	Localization["Stratagems"][3] = Localization["Stratagems"][3].is_string() ? Localization["Stratagems"][3] : "Slot";
	Localization["Stratagems"][4] = Localization["Stratagems"][4].is_string() ? Localization["Stratagems"][4] : "New ID";
	Localization["Stratagems"][5] = Localization["Stratagems"][5].is_string() ? Localization["Stratagems"][5] : "Amount";
	Localization["Stratagems"][6] = Localization["Stratagems"][6].is_string() ? Localization["Stratagems"][6] : "Update Slots";
	Localization["Stratagems"][7] = Localization["Stratagems"][7].is_string() ? Localization["Stratagems"][7] : "Load Slots";
	Localization["Stratagems"][8] = Localization["Stratagems"][8].is_string() ? Localization["Stratagems"][8] : "Load Data List";
	Localization["Stratagems"][9] = Localization["Stratagems"][9].is_string() ? Localization["Stratagems"][9] : "Filter";
	Localization["Stratagems"][10] = Localization["Stratagems"][10].is_string() ? Localization["Stratagems"][10] : "ID";
	Localization["Stratagems"][11] = Localization["Stratagems"][11].is_string() ? Localization["Stratagems"][11] : "Stratagem Name";

	//Mission
	Localization["Mission"][0] = Localization["Mission"][0].is_string() ? Localization["Mission"][0] : "Selected Mission";
	Localization["Mission"][1] = Localization["Mission"][1].is_string() ? Localization["Mission"][1] : "Active Objectives";
	Localization["Mission"][2] = Localization["Mission"][2].is_string() ? Localization["Mission"][2] : "Objectives and Outpost (Include Hidden)";
	Localization["Mission"][3] = Localization["Mission"][3].is_string() ? Localization["Mission"][3] : "Reduce Enemy Aggro";
	Localization["Mission"][4] = Localization["Mission"][4].is_string() ? Localization["Mission"][4] : "Vault/Bunker Left Door Only";
	Localization["Mission"][5] = Localization["Mission"][5].is_string() ? Localization["Mission"][5] : "Show All Map Icons";
	Localization["Mission"][6] = Localization["Mission"][6].is_string() ? Localization["Mission"][6] : "No Map Boundary";
	Localization["Mission"][7] = Localization["Mission"][7].is_string() ? Localization["Mission"][7] : "Instant Complete Outpost";
	Localization["Mission"][8] = Localization["Mission"][8].is_string() ? Localization["Mission"][8] : "Show Hidden Outpost";
	Localization["Mission"][9] = Localization["Mission"][9].is_string() ? Localization["Mission"][9] : "Instant Complete Mission";
	Localization["Mission"][10] = Localization["Mission"][10].is_string() ? Localization["Mission"][10] : "Set Custom Sample For Next Sample Pick Up";
	Localization["Mission"][11] = Localization["Mission"][11].is_string() ? Localization["Mission"][11] : "Normal Sample";
	Localization["Mission"][12] = Localization["Mission"][12].is_string() ? Localization["Mission"][12] : "Rare Sample";
	Localization["Mission"][13] = Localization["Mission"][13].is_string() ? Localization["Mission"][13] : "Super Sample";
	Localization["Mission"][14] = Localization["Mission"][14].is_string() ? Localization["Mission"][14] : "Set Next Sample Pick Up Value";
	Localization["Mission"][15] = Localization["Mission"][15].is_string() ? Localization["Mission"][15] : "Mission Time";
	Localization["Mission"][16] = Localization["Mission"][16].is_string() ? Localization["Mission"][16] : "Set & Freeze Mission Time";
	Localization["Mission"][17] = Localization["Mission"][17].is_string() ? Localization["Mission"][17] : "Unfreeze Mission Time";
	Localization["Mission"][18] = Localization["Mission"][18].is_string() ? Localization["Mission"][18] : "Extraction Time";
	Localization["Mission"][19] = Localization["Mission"][19].is_string() ? Localization["Mission"][19] : "Set & Freeze Extraction Time";
	Localization["Mission"][20] = Localization["Mission"][20].is_string() ? Localization["Mission"][20] : "Unfreeze Extraction Time";

	//Planet
	Localization["Planet"][0] = Localization["Planet"][0].is_string() ? Localization["Planet"][0] : "Selected Planet";
	Localization["Planet"][1] = Localization["Planet"][1].is_string() ? Localization["Planet"][1] : "ID";
	Localization["Planet"][2] = Localization["Planet"][2].is_string() ? Localization["Planet"][2] : "Name";
	Localization["Planet"][3] = Localization["Planet"][3].is_string() ? Localization["Planet"][3] : "New ID";
	Localization["Planet"][4] = Localization["Planet"][4].is_string() ? Localization["Planet"][4] : "Replace";
	Localization["Planet"][5] = Localization["Planet"][5].is_string() ? Localization["Planet"][5] : "Load Data List";
	Localization["Planet"][6] = Localization["Planet"][6].is_string() ? Localization["Planet"][6] : "Filter";
	Localization["Planet"][7] = Localization["Planet"][7].is_string() ? Localization["Planet"][7] : "Planet Name";

	//Data
	Localization["Data"][0] = Localization["Data"][0].is_string() ? Localization["Data"][0] : "Damage Name";
	Localization["Data"][1] = Localization["Data"][1].is_string() ? Localization["Data"][1] : "Damage ID";
	Localization["Data"][2] = Localization["Data"][2].is_string() ? Localization["Data"][2] : "Damage";
	Localization["Data"][3] = Localization["Data"][3].is_string() ? Localization["Data"][3] : "Penetration Power";
	Localization["Data"][4] = Localization["Data"][4].is_string() ? Localization["Data"][4] : "Load Damage Stats";
	Localization["Data"][5] = Localization["Data"][5].is_string() ? Localization["Data"][5] : "Structure/Armor Damage";
	Localization["Data"][6] = Localization["Data"][6].is_string() ? Localization["Data"][6] : "Change Damage Stats";
	Localization["Data"][7] = Localization["Data"][7].is_string() ? Localization["Data"][7] : "Load Damage Name";
	Localization["Data"][8] = Localization["Data"][8].is_string() ? Localization["Data"][8] : "Save Damage ID";
	Localization["Data"][9] = Localization["Data"][9].is_string() ? Localization["Data"][9] : "Load Damage Cfg";
	Localization["Data"][10] = Localization["Data"][10].is_string() ? Localization["Data"][10] : "Clear Damage Cfg";
	Localization["Data"][11] = Localization["Data"][11].is_string() ? Localization["Data"][11] : "Explosive Name";
	Localization["Data"][12] = Localization["Data"][12].is_string() ? Localization["Data"][12] : "Explosive ID";
	Localization["Data"][13] = Localization["Data"][13].is_string() ? Localization["Data"][13] : "Explosive Damage (Damage ID)";
	Localization["Data"][14] = Localization["Data"][14].is_string() ? Localization["Data"][14] : "Radius (Falloff)";
	Localization["Data"][15] = Localization["Data"][15].is_string() ? Localization["Data"][15] : "Load Explosive Stats";
	Localization["Data"][16] = Localization["Data"][16].is_string() ? Localization["Data"][16] : "Radius (Inner)";
	Localization["Data"][17] = Localization["Data"][17].is_string() ? Localization["Data"][17] : "Change Explosive Stats";
	Localization["Data"][18] = Localization["Data"][18].is_string() ? Localization["Data"][18] : "Load Explosive Name";
	Localization["Data"][19] = Localization["Data"][19].is_string() ? Localization["Data"][19] : "Save Explosive ID";
	Localization["Data"][20] = Localization["Data"][20].is_string() ? Localization["Data"][20] : "Load Explosive Cfg";
	Localization["Data"][21] = Localization["Data"][21].is_string() ? Localization["Data"][21] : "Clear Explosive Cfg";

	//Misc
	Localization["Misc"][0] = Localization["Misc"][0].is_string() ? Localization["Misc"][0] : "Unlock All Stratagems";
	Localization["Misc"][1] = Localization["Misc"][1].is_string() ? Localization["Misc"][1] : "Unlock All Equipment";
	Localization["Misc"][2] = Localization["Misc"][2].is_string() ? Localization["Misc"][2] : "Unlock All Armor";
	Localization["Misc"][3] = Localization["Misc"][3].is_string() ? Localization["Misc"][3] : "No Jetpack Cooldown";
	Localization["Misc"][4] = Localization["Misc"][4].is_string() ? Localization["Misc"][4] : "No Stationary Turret Overheat";
	Localization["Misc"][5] = Localization["Misc"][5].is_string() ? Localization["Misc"][5] : "Set Custom Backpack Shield Setting";
	Localization["Misc"][6] = Localization["Misc"][6].is_string() ? Localization["Misc"][6] : "Cooldown Time";
	Localization["Misc"][7] = Localization["Misc"][7].is_string() ? Localization["Misc"][7] : "Max Energy";
	Localization["Misc"][8] = Localization["Misc"][8].is_string() ? Localization["Misc"][8] : "Enemy Reinforcement Type";
	Localization["Misc"][9] = Localization["Misc"][9].is_string() ? Localization["Misc"][9] : "Save All Config";
	Localization["Misc"][10] = Localization["Misc"][10].is_string() ? Localization["Misc"][10] : "Load All Config";


	//Uncomment this to create a new language config file
	//SaveFile(strLanguageFileName, Language);

#ifdef DEBUG_CONSOLE
	printf("[FileManager][Language] Initialized\n");
#endif

#ifdef VMPROTECT
	VMProtectEnd();
#endif
}

string FileManager::GetFontFile()
{
	if (LanguagePath.empty())
	{
		return "";
	}

	if (!Language.size())
	{
		return "";
	}

	string FontFileName = Language["Font"].get<string>();
	if (FontFileName.empty())
	{
		return "";
	}

	string FontFullPath = wstring_to_utf8(LanguagePath) + FontFileName;

	if (!ifstream(FontFullPath))
	{
#ifdef DEBUG_CONSOLE
		printf("[FileManager][GetFontFile] Unable to open font file\n");
#endif
		return "";
	}

	return FontFullPath;
}