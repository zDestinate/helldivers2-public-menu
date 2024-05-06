#define _CRT_SECURE_NO_WARNINGS

#include "../features.h"
#include <algorithm>

using namespace std;


void features::LoadStratagemList()
{
#ifdef VMPROTECT
    VMProtectBeginUltra("features::LoadStratagemList");
    if (!VMProtectIsProtected())
    {
        return;
    }
#endif

    string key = "StratagemList";
    CheckFeatureExist(key);

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    uintptr_t TempAddress = GetAddress(key, "48 8D 15 ? ? ? ? 0F 1F 40 ? 4C 8B 02 41 39 48 ? 74 ? FF C0 48 83 C2 ? 83 F8 ? 72 ? 33 C0");
    if (!TempAddress)
    {
        return;
    }

    unsigned int nRIPOffset;
    memcpy(&nRIPOffset, (LPVOID)(TempAddress + 3), 4);
    TempAddress += nRIPOffset + 7;

    for (int i = 0;; i++)
    {
        uintptr_t StratagemAddress = TempAddress + (i * 8);
        uintptr_t CurrentAddress = *(uintptr_t*)(StratagemAddress);

        if (CurrentAddress == NULL)
        {
            break;
        }

        Stratagem_Data StratagemData;
        StratagemData.HexValue = ((int)*(uintptr_t*)(CurrentAddress + 4));

        string StratagemName = (char*)(*(uintptr_t*)(CurrentAddress + 16));
        StratagemData.Name = StratagemName;

        StratagemList.push_back(StratagemData);
    }

    Config.bStratagemListLoaded = true;

#ifdef VMPROTECT
    VMProtectEnd();
#endif
}

void features::FilterStratagemList()
{
    if (StratagemList.empty())
    {
        return;
    }

    FilteredStratagemList.clear();

    string strFilterName = Config.szStratagemFilter;
    if (strFilterName.empty())
    {
        return;
    }

    int nIndexSearch = -1;
    try
    {
        nIndexSearch = stoi(strFilterName);
    }
    catch (...)
    {
    }

    transform(strFilterName.begin(), strFilterName.end(), strFilterName.begin(), ::toupper);
    for (int i = 0; i < StratagemList.size(); i++)
    {
        if (nIndexSearch >= 1 && nIndexSearch == i + 1)
        {
            FilteredStratagemList.push_back({ i + 1, StratagemList[i].Name });
            break;
        }

        string strName = StratagemList[i].Name;
        transform(strName.begin(), strName.end(), strName.begin(), ::toupper);
        if (strName.find(strFilterName) != string::npos)
        {
            FilteredStratagemList.push_back({ i + 1, StratagemList[i].Name });
        }
    }
}

void features::ReplaceStratagem()
{
    if (!Config.pStratagemSlotAddress.size())
    {
        return;
    }

    if (Config.nStratagemPlayerIndex < 0)
    {
        return;
    }

    try
    {
        for (int i = 0; i < StratagemPlayerSlotData.size(); i++)
        {
            //if (StratagemPlayerSlotData[i].NewID < 1 ||StratagemPlayerSlotData[i].NewID > StratagemList.size())
            if (StratagemPlayerSlotData[i].NewID < 1)
                continue;

            *(Config.pStratagemSlotAddress[Config.nStratagemPlayerIndex] + (i * 15)) = StratagemPlayerSlotData[i].NewID;
            *((Config.pStratagemSlotAddress[Config.nStratagemPlayerIndex] + (i * 15)) + 3) = StratagemPlayerSlotData[i].Amount;
        }
    }
    catch (...)
    {
        Config.pStratagemSlotAddress.erase(Config.pStratagemSlotAddress.begin() + (Config.nStratagemPlayerIndex - 1));
        Config.nStratagemPlayerIndex = -1;
        std::strncpy(Config.szStratagemPlayerID, "Not Selected", sizeof(Config.szStratagemPlayerID));
        sort(Config.pStratagemSlotAddress.begin(), Config.pStratagemSlotAddress.end());
    }
}

void features::LoadPlayerStratagem()
{
    if (!Config.pStratagemSlotAddress.size())
    {
        return;
    }

    if (Config.nStratagemPlayerIndex < 0)
    {
        return;
    }

    try
    {
        for (int i = 0; i < StratagemPlayerSlotData.size(); i++)
        {
            StratagemPlayerSlotData[i].OriginalID = *(Config.pStratagemSlotAddress[Config.nStratagemPlayerIndex] + (i * 15));
            StratagemPlayerSlotData[i].NewID = *(Config.pStratagemSlotAddress[Config.nStratagemPlayerIndex] + (i * 15));
            StratagemPlayerSlotData[i].Amount = *((Config.pStratagemSlotAddress[Config.nStratagemPlayerIndex] + (i * 15)) + 3);
        }
    }
    catch (...)
    {
        Config.pStratagemSlotAddress.erase(Config.pStratagemSlotAddress.begin() + Config.nStratagemPlayerIndex);
        Config.nStratagemPlayerIndex = -1;
        std::strncpy(Config.szStratagemPlayerID, "Not Selected", sizeof(Config.szStratagemPlayerID));
        sort(Config.pStratagemSlotAddress.begin(), Config.pStratagemSlotAddress.end());
    }
}