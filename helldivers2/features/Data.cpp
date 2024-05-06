#define _CRT_SECURE_NO_WARNINGS

#include "../features.h"
#include "../include/FileManager.h"
#include "../funchook.h"

using namespace std;


struct Damage_Data
{
    int ID;
    int Damage;
    int StructureDamage;
    int PenetrationPower1;
    int PenetrationPower2;
    int PenetrationPower3;
};

struct Explosive_Data
{
    int ID;
    int DamageID;
    int Unknown;
    float RadiusInner;
    float RadiusFalloff;
};


void features::LoadWeaponStats()
{
    //Find total damage ID
    if (!Config.nTotalDamageID)
    {
        for (int i = 1;; i++)
        {
            uintptr_t TempAddress = (uintptr_t)GameHooking::FuncHook.oGetDamageStats(i);
            if (!TempAddress)
            {
                Config.nTotalDamageID = i - 1;
                break;
            }
        }
    }

    if (Config.nWeaponDamageID < 1 || Config.nWeaponDamageID > Config.nTotalDamageID)
    {
        Config.nWeaponDamageID = 1;
    }

    if (!Config.nDamageNameListLoaded && Config.nWeaponDamageID > WeaponDamageIDNameList.size() - 2)
    {
        strcpy(Config.szWeaponDamageIDName, "Unknown");
    }
    else if (!Config.nDamageNameListLoaded && (Config.nWeaponDamageID > 0) && (Config.nWeaponDamageID <= WeaponDamageIDNameList.size() - 2))
    {
        strcpy(Config.szWeaponDamageIDName, WeaponDamageIDNameList[Config.nWeaponDamageID].c_str());
    }

    uintptr_t* TempAddress = (uintptr_t*)GameHooking::FuncHook.oGetDamageStats(Config.nWeaponDamageID);

    Damage_Data* DamageStats = (Damage_Data*)(TempAddress);
    Config.nWeaponDamage = DamageStats->Damage;
    Config.nWeaponStructureDamage = DamageStats->StructureDamage;
    Config.nWeaponPenetrationPower1 = DamageStats->PenetrationPower1;
    Config.nWeaponPenetrationPower2 = DamageStats->PenetrationPower2;
    Config.nWeaponPenetrationPower3 = DamageStats->PenetrationPower3;
}

void features::SetWeaponStats()
{
    //Find total damage ID
    if (!Config.nTotalDamageID)
    {
        for (int i = 1;; i++)
        {
            uintptr_t TempAddress = (uintptr_t)GameHooking::FuncHook.oGetDamageStats(i);
            if (!TempAddress)
            {
                Config.nTotalDamageID = i - 1;
                break;
            }
        }
    }

    if (Config.nWeaponDamageID < 1 || Config.nWeaponDamageID > Config.nTotalDamageID)
    {
        Config.nWeaponDamageID = 1;
    }

    uintptr_t* TempAddress = (uintptr_t*)GameHooking::FuncHook.oGetDamageStats(Config.nWeaponDamageID);

    Damage_Data* DamageStats = (Damage_Data*)(TempAddress);
    DamageStats->Damage = Config.nWeaponDamage;
    DamageStats->StructureDamage = Config.nWeaponStructureDamage;
    DamageStats->PenetrationPower1 = Config.nWeaponPenetrationPower1;
    DamageStats->PenetrationPower2 = Config.nWeaponPenetrationPower2;
    DamageStats->PenetrationPower3 = Config.nWeaponPenetrationPower3;
}

struct LoadWeaponThreadParam
{
    int* LoadStatus;
    char* szLoadStatus;
    vector<string>* WeaponDamageIDNameList;
};

void LoadWeaponThread(LPVOID Param)
{
    LoadWeaponThreadParam* ThreadParam = (LoadWeaponThreadParam*)Param;
    vector<string>* WeaponDamageIDNameList = ThreadParam->WeaponDamageIDNameList;

    //Extract Damage ID names
    uintptr_t WeaponDamageIDNameAddress = Memory::FindString("DamageInfoType_None");
    if (WeaponDamageIDNameAddress)
    {
        size_t WeaponDamageIDNameOffset = 0;
        string strWeaponDamageIDName = (char*)(WeaponDamageIDNameAddress);
        do
        {
            WeaponDamageIDNameList->push_back(strWeaponDamageIDName.substr(15));
            WeaponDamageIDNameOffset += strWeaponDamageIDName.length() + 1;
            strWeaponDamageIDName = (char*)(WeaponDamageIDNameAddress + WeaponDamageIDNameOffset);
        } while (strWeaponDamageIDName.find("DamageInfoType_") != string::npos);

        strcpy(ThreadParam->szLoadStatus, "Data loaded");
        *(ThreadParam->LoadStatus) = 0;
    }
    else
    {
        strcpy(ThreadParam->szLoadStatus, "Data not found");
    }

    delete ThreadParam;
}

void features::LoadWeaponDamageNameData()
{
    if (Config.nDamageNameListLoaded == 2)
    {
        Config.nDamageNameListLoaded = 1;

        strcpy(Config.szWeaponDamageIDName, "Loading data...");

        //Create Param for thread
        LoadWeaponThreadParam* ThreadParam = new LoadWeaponThreadParam;
        ThreadParam->LoadStatus = &(Config.nDamageNameListLoaded);
        ThreadParam->szLoadStatus = Config.szWeaponDamageIDName;
        ThreadParam->WeaponDamageIDNameList = &WeaponDamageIDNameList;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&LoadWeaponThread, (LPVOID)ThreadParam, 0, NULL);
    }
}

void features::WeaponDamageSaveID()
{
    if (!g_FileManager.Config["Data"].contains("Damage"))
    {
        g_FileManager.Config["Data"]["Damage"] = json::array();
    }

    if (!g_FileManager.Config["Data"]["Damage"].is_array())
    {
        g_FileManager.Config["Data"]["Damage"] = json::array();
    }

    json& DamageArray = g_FileManager.Config["Data"]["Damage"];
    bool bFound = false;
    for (auto& item : DamageArray)
    {
        int ID = item["ID"].is_number_integer() ? item["ID"].get<int>() : 0;
        if (ID > 0 && ID == Config.nWeaponDamageID)
        {
            item["Damage"] = Config.nWeaponDamage;
            item["StructureDamage"] = Config.nWeaponStructureDamage;
            item["Pen1"] = Config.nWeaponPenetrationPower1;
            item["Pen2"] = Config.nWeaponPenetrationPower2;
            item["Pen3"] = Config.nWeaponPenetrationPower3;
            bFound = true;
            break;
        }
    }

    //Not found then we will create new damage data to the config
    if (!bFound)
    {
        json Temp;
        Temp["ID"] = Config.nWeaponDamageID;
        Temp["Damage"] = Config.nWeaponDamage;
        Temp["StructureDamage"] = Config.nWeaponStructureDamage;
        Temp["Pen1"] = Config.nWeaponPenetrationPower1;
        Temp["Pen2"] = Config.nWeaponPenetrationPower2;
        Temp["Pen3"] = Config.nWeaponPenetrationPower3;

        DamageArray.push_back(Temp);
    }

    g_FileManager.SaveFile(L"config.json", g_FileManager.Config);
}

void features::WeaponDamageLoadConfig()
{
    if (!g_FileManager.Config["Data"].contains("Damage"))
    {
        return;
    }

    if (!g_FileManager.Config["Data"]["Damage"].is_array())
    {
        return;
    }

    //Find total damage ID
    if (!Config.nTotalDamageID)
    {
        for (int i = 1;; i++)
        {
            uintptr_t TempAddress = (uintptr_t)GameHooking::FuncHook.oGetDamageStats(i);
            if (!TempAddress)
            {
                Config.nTotalDamageID = i - 1;
                break;
            }
        }
    }

    json DamageArray = g_FileManager.Config["Data"]["Damage"];
    for (auto item : DamageArray)
    {
        //Check ID
        int ID = item["ID"].is_number_integer() ? item["ID"].get<int>() : 0;
        if (ID < 1 || ID > Config.nTotalDamageID)
        {
            continue;
        }

        //Check values
        int Damage = item["Damage"].is_number_integer() ? item["Damage"].get<int>() : -1;
        int StructureDamage = item["StructureDamage"].is_number_integer() ? item["StructureDamage"].get<int>() : -1;
        int Pen1 = item["Pen1"].is_number_integer() ? item["Pen1"].get<int>() : -1;
        int Pen2 = item["Pen2"].is_number_integer() ? item["Pen2"].get<int>() : -1;
        int Pen3 = item["Pen3"].is_number_integer() ? item["Pen3"].get<int>() : -1;

        if (Damage < 0 || StructureDamage < 0 || Pen1 < 0 || Pen2 < 0 || Pen3 < 0)
        {
            continue;
        }

        uintptr_t* TempAddress = (uintptr_t*)GameHooking::FuncHook.oGetDamageStats(ID);

        Damage_Data* DamageStats = (Damage_Data*)(TempAddress);
        DamageStats->Damage = Damage;
        DamageStats->StructureDamage = StructureDamage;
        DamageStats->PenetrationPower1 = Pen1;
        DamageStats->PenetrationPower2 = Pen2;
        DamageStats->PenetrationPower3 = Pen3;
    }
}

void features::WeaponDamageClearAll()
{
    if (!g_FileManager.Config["Data"].contains("Damage"))
        return;

    g_FileManager.Config["Data"].erase("Damage");
    g_FileManager.SaveFile(L"config.json", g_FileManager.Config);
}

void features::LoadExplosiveStats()
{
    if (!Config.nTotalExplosiveID)
    {
        for (int i = 1;; i++)
        {
            uintptr_t TempAddress = (uintptr_t)GameHooking::FuncHook.oGetExplosiveStats(i);
            if (!TempAddress)
            {
                Config.nTotalExplosiveID = i - 1;
                break;
            }
        }
    }

    if (Config.nWeaponExplosiveID < 1 || Config.nWeaponExplosiveID > Config.nTotalExplosiveID)
    {
        Config.nWeaponExplosiveID = 1;
    }

    if (!Config.nExplosiveNameListLoaded && Config.nWeaponExplosiveID > WeaponExplosiveIDNameList.size() - 2)
    {
        strcpy(Config.szWeaponExplosiveIDName, "Unknown");
    }
    else if (!Config.nExplosiveNameListLoaded && (Config.nWeaponExplosiveID > 0) && (Config.nWeaponExplosiveID <= WeaponExplosiveIDNameList.size() - 2))
    {
        strcpy(Config.szWeaponExplosiveIDName, WeaponExplosiveIDNameList[Config.nWeaponExplosiveID].c_str());
    }

    uintptr_t* TempAddress = (uintptr_t*)GameHooking::FuncHook.oGetExplosiveStats(Config.nWeaponExplosiveID);

    Explosive_Data* ExplosiveStats = (Explosive_Data*)(TempAddress);
    Config.nWeaponExplosiveDamageID = ExplosiveStats->DamageID;
    Config.fWeaponExplosiveRadiusInner = ExplosiveStats->RadiusInner;
    Config.fWeaponExplosiveRadiusFalloff = ExplosiveStats->RadiusFalloff;
}

void features::SetExplosiveStats()
{
    if (!Config.nTotalExplosiveID)
    {
        for (int i = 1;; i++)
        {
            uintptr_t TempAddress = (uintptr_t)GameHooking::FuncHook.oGetExplosiveStats(i);
            if (!TempAddress)
            {
                Config.nTotalExplosiveID = i - 1;
                break;
            }
        }
    }

    if (Config.nWeaponExplosiveID < 1 || Config.nWeaponExplosiveID > Config.nTotalExplosiveID)
    {
        Config.nWeaponExplosiveID = 1;
    }

    uintptr_t* TempAddress = (uintptr_t*)GameHooking::FuncHook.oGetExplosiveStats(Config.nWeaponExplosiveID);

    Explosive_Data* ExplosiveStats = (Explosive_Data*)(TempAddress);
    ExplosiveStats->DamageID = Config.nWeaponExplosiveDamageID;
    ExplosiveStats->RadiusInner = Config.fWeaponExplosiveRadiusInner;
    ExplosiveStats->RadiusFalloff = Config.fWeaponExplosiveRadiusFalloff;
}

struct LoadExplosiveThreadParam
{
    int* LoadStatus;
    char* szLoadStatus;
    vector<string>* WeaponExplosiveIDNameList;
};

void LoadExplosiveThread(LPVOID Param)
{
    LoadExplosiveThreadParam* ThreadParam = (LoadExplosiveThreadParam*)Param;
    vector<string>* WeaponExplosiveIDNameList = ThreadParam->WeaponExplosiveIDNameList;

    //Extract Damage ID names
    uintptr_t WeaponExplosiveIDNameAddress = Memory::FindString("ExplosionType_None");
    if (WeaponExplosiveIDNameAddress)
    {
        size_t WeaponExplosiveIDNameOffset = 0;
        string strWeaponExplosiveIDName = (char*)(WeaponExplosiveIDNameAddress);
        do
        {
            WeaponExplosiveIDNameList->push_back(strWeaponExplosiveIDName.substr(14));
            WeaponExplosiveIDNameOffset += strWeaponExplosiveIDName.length() + 1;
            strWeaponExplosiveIDName = (char*)(WeaponExplosiveIDNameAddress + WeaponExplosiveIDNameOffset);
        } while (strWeaponExplosiveIDName.find("ExplosionType_") != string::npos);

        strcpy(ThreadParam->szLoadStatus, "Data loaded");
        *(ThreadParam->LoadStatus) = 0;
    }
    else
    {
        strcpy(ThreadParam->szLoadStatus, "Data not found");
    }

    delete ThreadParam;
}

void features::LoadWeaponExplosiveNameData()
{
    if (Config.nExplosiveNameListLoaded == 2)
    {
        Config.nExplosiveNameListLoaded = 1;

        strcpy(Config.szWeaponExplosiveIDName, "Loading data...");

        //Create Param for thread
        LoadExplosiveThreadParam* ThreadParam = new LoadExplosiveThreadParam;
        ThreadParam->LoadStatus = &(Config.nExplosiveNameListLoaded);
        ThreadParam->szLoadStatus = Config.szWeaponExplosiveIDName;
        ThreadParam->WeaponExplosiveIDNameList = &WeaponExplosiveIDNameList;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&LoadExplosiveThread, (LPVOID)ThreadParam, 0, NULL);
    }
}

void features::WeaponExplosiveSaveID()
{
    if (!g_FileManager.Config["Data"].contains("Explosive"))
    {
        g_FileManager.Config["Data"]["Explosive"] = json::array();
    }

    if (!g_FileManager.Config["Data"]["Explosive"].is_array())
    {
        g_FileManager.Config["Data"]["Explosive"] = json::array();
    }

    json& ExplosiveArray = g_FileManager.Config["Data"]["Explosive"];
    bool bFound = false;
    for (auto& item : ExplosiveArray)
    {
        int ID = item["ID"].is_number_integer() ? item["ID"].get<int>() : 0;
        if (ID > 0 && ID == Config.nWeaponExplosiveID)
        {
            item["DamageID"] = Config.nWeaponExplosiveDamageID;
            item["RadiusInner"] = Config.fWeaponExplosiveRadiusInner;
            item["RadiusFalloff"] = Config.fWeaponExplosiveRadiusFalloff;
            bFound = true;
            break;
        }
    }

    //Not found then we will create new damage data to the config
    if (!bFound)
    {
        json Temp;
        Temp["ID"] = Config.nWeaponExplosiveID;
        Temp["DamageID"] = Config.nWeaponExplosiveDamageID;
        Temp["RadiusInner"] = Config.fWeaponExplosiveRadiusInner;
        Temp["RadiusFalloff"] = Config.fWeaponExplosiveRadiusFalloff;

        ExplosiveArray.push_back(Temp);
    }

    g_FileManager.SaveFile(L"config.json", g_FileManager.Config);
}

void features::WeaponExplosiveLoadConfig()
{
    if (!g_FileManager.Config["Data"].contains("Explosive"))
    {
        return;
    }

    if (!g_FileManager.Config["Data"]["Explosive"].is_array())
    {
        return;
    }

    if (!Config.nTotalExplosiveID)
    {
        for (int i = 1;; i++)
        {
            uintptr_t TempAddress = (uintptr_t)GameHooking::FuncHook.oGetExplosiveStats(i);
            if (!TempAddress)
            {
                Config.nTotalExplosiveID = i - 1;
                break;
            }
        }
    }

    json ExplosiveArray = g_FileManager.Config["Data"]["Explosive"];
    for (auto item : ExplosiveArray)
    {
        //Check ID
        int ID = item["ID"].is_number_integer() ? item["ID"].get<int>() : 0;
        if (ID < 1 || ID > Config.nTotalExplosiveID)
        {
            continue;
        }

        //Check values
        int DamageID = item["DamageID"].is_number_integer() ? item["DamageID"].get<int>() : -1;
        float RadiusInner = item["RadiusInner"].is_number_float() ? item["RadiusInner"].get<float>() : -1.0f;
        float RadiusFalloff = item["RadiusFalloff"].is_number_float() ? item["RadiusFalloff"].get<float>() : -1.0f;

        if (DamageID < 0 || RadiusInner < 0.0f || RadiusFalloff < 0.0f)
        {
            continue;
        }

        uintptr_t* TempAddress = (uintptr_t*)GameHooking::FuncHook.oGetExplosiveStats(ID);
        Explosive_Data* ExplosiveStats = (Explosive_Data*)(TempAddress);
        ExplosiveStats->DamageID = DamageID;
        ExplosiveStats->RadiusInner = RadiusInner;
        ExplosiveStats->RadiusFalloff = RadiusFalloff;
    }
}

void features::WeaponExplosiveClearAll()
{
    if (!g_FileManager.Config["Data"].contains("Explosive"))
        return;

    g_FileManager.Config["Data"].erase("Explosive");
    g_FileManager.SaveFile(L"config.json", g_FileManager.Config);
}