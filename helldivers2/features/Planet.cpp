#define _CRT_SECURE_NO_WARNINGS

#include "../features.h"
#include <algorithm>

using namespace std;


void InitPlanetNameList()
{
    Features.PlanetNameList.insert({ 0, "Super Earth" });
    Features.PlanetNameList.insert({ 1, "Klen Dahth II" });
    Features.PlanetNameList.insert({ 2, "Pathfinder V" });
    Features.PlanetNameList.insert({ 3, "Widow's Harbor" });
    Features.PlanetNameList.insert({ 4, "New Haven" });
    Features.PlanetNameList.insert({ 5, "Pilen V" });
    Features.PlanetNameList.insert({ 7, "Hydrofall Prime" });
    Features.PlanetNameList.insert({ 8, "Zea Rugosia" });
    Features.PlanetNameList.insert({ 9, "Darrowsport" });
    Features.PlanetNameList.insert({ 10, "Fornskogur II" });
    Features.PlanetNameList.insert({ 11, "Midasburg" });
    Features.PlanetNameList.insert({ 12, "Cerberus Iiic" });
    Features.PlanetNameList.insert({ 13, "Prosperity Falls" });
    Features.PlanetNameList.insert({ 14, "Okul VI" });
    Features.PlanetNameList.insert({ 15, "Martyr's Bay" });
    Features.PlanetNameList.insert({ 16, "Freedom Peak" });
    Features.PlanetNameList.insert({ 17, "Fort Union" });
    Features.PlanetNameList.insert({ 18, "Kelvinor" });
    Features.PlanetNameList.insert({ 19, "Wraith" });
    Features.PlanetNameList.insert({ 20, "Igla" });
    Features.PlanetNameList.insert({ 21, "New Kiruna" });
    Features.PlanetNameList.insert({ 22, "Fort Justice" });
    Features.PlanetNameList.insert({ 24, "Zegema Paradise" });
    Features.PlanetNameList.insert({ 25, "Providence" });
    Features.PlanetNameList.insert({ 26, "Primordia" });
    Features.PlanetNameList.insert({ 27, "Sulfura" });
    Features.PlanetNameList.insert({ 28, "Nublaria I" });
    Features.PlanetNameList.insert({ 29, "Krakatwo" });
    Features.PlanetNameList.insert({ 30, "Volterra" });
    Features.PlanetNameList.insert({ 31, "Crucible" });
    Features.PlanetNameList.insert({ 32, "Veil" });
    Features.PlanetNameList.insert({ 33, "Marre IV" });
    Features.PlanetNameList.insert({ 34, "Fort Sanctuary" });
    Features.PlanetNameList.insert({ 35, "Seyshel Beach" });
    Features.PlanetNameList.insert({ 36, "Hellmire" });
    Features.PlanetNameList.insert({ 37, "Effluvia" });
    Features.PlanetNameList.insert({ 38, "Solghast" });
    Features.PlanetNameList.insert({ 39, "Diluvia" });
    Features.PlanetNameList.insert({ 40, "Viridia Prime" });
    Features.PlanetNameList.insert({ 41, "Obari" });
    Features.PlanetNameList.insert({ 42, "Myradesh" });
    Features.PlanetNameList.insert({ 43, "Atrama" });
    Features.PlanetNameList.insert({ 44, "Emeria" });
    Features.PlanetNameList.insert({ 45, "Barabos" });
    Features.PlanetNameList.insert({ 46, "Fenmire" });
    Features.PlanetNameList.insert({ 47, "Mastia" });
    Features.PlanetNameList.insert({ 48, "Shallus" });
    Features.PlanetNameList.insert({ 49, "Krakabos" });
    Features.PlanetNameList.insert({ 50, "Iridica" });
    Features.PlanetNameList.insert({ 51, "Azterra" });
    Features.PlanetNameList.insert({ 52, "Azur Secundus" });
    Features.PlanetNameList.insert({ 53, "Ivis" });
    Features.PlanetNameList.insert({ 54, "Slif" });
    Features.PlanetNameList.insert({ 55, "Caramoor" });
    Features.PlanetNameList.insert({ 57, "Kharst" });
    Features.PlanetNameList.insert({ 58, "Eukoria" });
    Features.PlanetNameList.insert({ 59, "Myrium" });
    Features.PlanetNameList.insert({ 60, "Kerth Secundus" });
    Features.PlanetNameList.insert({ 61, "Parsh" });
    Features.PlanetNameList.insert({ 62, "Reaf" });
    Features.PlanetNameList.insert({ 63, "Irulta" });
    Features.PlanetNameList.insert({ 64, "Emorath" });
    Features.PlanetNameList.insert({ 65, "Ilduna Prime" });
    Features.PlanetNameList.insert({ 66, "Maw" });
    Features.PlanetNameList.insert({ 67, "Meridia" });
    Features.PlanetNameList.insert({ 68, "Borea" });
    Features.PlanetNameList.insert({ 69, "Curia" });
    Features.PlanetNameList.insert({ 70, "Tarsh" });
    Features.PlanetNameList.insert({ 71, "Shelt" });
    Features.PlanetNameList.insert({ 72, "Imber" });
    Features.PlanetNameList.insert({ 73, "Blistica" });
    Features.PlanetNameList.insert({ 74, "Ratch" });
    Features.PlanetNameList.insert({ 75, "Julheim" });
    Features.PlanetNameList.insert({ 76, "Valgaard" });
    Features.PlanetNameList.insert({ 77, "Arkturus" });
    Features.PlanetNameList.insert({ 78, "Esker" });
    Features.PlanetNameList.insert({ 79, "Terrek" });
    Features.PlanetNameList.insert({ 80, "Cirrus" });
    Features.PlanetNameList.insert({ 81, "Crimsica" });
    Features.PlanetNameList.insert({ 82, "Heeth" });
    Features.PlanetNameList.insert({ 83, "Veld" });
    Features.PlanetNameList.insert({ 84, "Alta V" });
    Features.PlanetNameList.insert({ 85, "Ursica XI" });
    Features.PlanetNameList.insert({ 86, "Inari" });
    Features.PlanetNameList.insert({ 87, "Skaash" });
    Features.PlanetNameList.insert({ 88, "Moradesh" });
    Features.PlanetNameList.insert({ 89, "Rasp" });
    Features.PlanetNameList.insert({ 90, "Bashyr" });
    Features.PlanetNameList.insert({ 91, "Regnus" });
    Features.PlanetNameList.insert({ 92, "Mog" });
    Features.PlanetNameList.insert({ 93, "Valmox" });
    Features.PlanetNameList.insert({ 94, "Iro" });
    Features.PlanetNameList.insert({ 95, "Grafmere" });
    Features.PlanetNameList.insert({ 96, "New Stockholm" });
    Features.PlanetNameList.insert({ 97, "Oasis" });
    Features.PlanetNameList.insert({ 98, "Genesis Prime" });
    Features.PlanetNameList.insert({ 99, "Outpost 32" });
    Features.PlanetNameList.insert({ 100, "Calypso" });
    Features.PlanetNameList.insert({ 101, "Elysian Meadows" });
    Features.PlanetNameList.insert({ 102, "Alderidge Cove" });
    Features.PlanetNameList.insert({ 103, "Trandor" });
    Features.PlanetNameList.insert({ 104, "East Iridium Trading Bay" });
    Features.PlanetNameList.insert({ 105, "Liberty Ridge" });
    Features.PlanetNameList.insert({ 106, "Baldrick Prime" });
    Features.PlanetNameList.insert({ 107, "The Weir" });
    Features.PlanetNameList.insert({ 108, "Kuper" });
    Features.PlanetNameList.insert({ 109, "Oslo Station" });
    Features.PlanetNameList.insert({ 110, "Pöpli IX" });
    Features.PlanetNameList.insert({ 111, "Gunvald" });
    Features.PlanetNameList.insert({ 112, "Dolph" });
    Features.PlanetNameList.insert({ 113, "Bekvam III" });
    Features.PlanetNameList.insert({ 114, "Duma Tyr" });
    Features.PlanetNameList.insert({ 115, "Vernen Wells" });
    Features.PlanetNameList.insert({ 116, "Aesir Pass" });
    Features.PlanetNameList.insert({ 117, "Aurora Bay" });
    Features.PlanetNameList.insert({ 118, "Penta" });
    Features.PlanetNameList.insert({ 119, "Gaellivare" });
    Features.PlanetNameList.insert({ 120, "Vog-sojoth" });
    Features.PlanetNameList.insert({ 121, "Kirrik" });
    Features.PlanetNameList.insert({ 122, "Mortax Prime" });
    Features.PlanetNameList.insert({ 123, "Wilford Station" });
    Features.PlanetNameList.insert({ 124, "Pioneer II" });
    Features.PlanetNameList.insert({ 125, "Erson Sands" });
    Features.PlanetNameList.insert({ 126, "Socorro III" });
    Features.PlanetNameList.insert({ 127, "Bore Rock" });
    Features.PlanetNameList.insert({ 128, "Fenrir III" });
    Features.PlanetNameList.insert({ 129, "Turing" });
    Features.PlanetNameList.insert({ 130, "Angel's Venture" });
    Features.PlanetNameList.insert({ 131, "Darius II" });
    Features.PlanetNameList.insert({ 132, "Acamar IV" });
    Features.PlanetNameList.insert({ 133, "Achernar Secundus" });
    Features.PlanetNameList.insert({ 134, "Achird III" });
    Features.PlanetNameList.insert({ 135, "Acrab XI" });
    Features.PlanetNameList.insert({ 136, "Acrux IX" });
    Features.PlanetNameList.insert({ 137, "Acubens Prime" });
    Features.PlanetNameList.insert({ 151, "Osupsam" });
    Features.PlanetNameList.insert({ 152, "Brink-2" });
    Features.PlanetNameList.insert({ 153, "Bunda Secundus" });
    Features.PlanetNameList.insert({ 154, "Canopus" });
    Features.PlanetNameList.insert({ 155, "Caph" });
    Features.PlanetNameList.insert({ 156, "Castor" });
    Features.PlanetNameList.insert({ 157, "Durgen" });
    Features.PlanetNameList.insert({ 158, "Draupnir" });
    Features.PlanetNameList.insert({ 159, "Mort" });
    Features.PlanetNameList.insert({ 160, "Ingmar" });
    Features.PlanetNameList.insert({ 162, "Charbal-VII" });
    Features.PlanetNameList.insert({ 163, "Charon Prime" });
    Features.PlanetNameList.insert({ 164, "Choepessa IV" });
    Features.PlanetNameList.insert({ 165, "Choohe" });
    Features.PlanetNameList.insert({ 166, "Chort Bay" });
    Features.PlanetNameList.insert({ 167, "Claorell" });
    Features.PlanetNameList.insert({ 168, "Clasa" });
    Features.PlanetNameList.insert({ 170, "Demiurg" });
    Features.PlanetNameList.insert({ 171, "Deneb Secundus" });
    Features.PlanetNameList.insert({ 172, "Electra Bay" });
    Features.PlanetNameList.insert({ 173, "Enuliale" });
    Features.PlanetNameList.insert({ 174, "Epsilon Phoencis VI" });
    Features.PlanetNameList.insert({ 175, "Erata Prime" });
    Features.PlanetNameList.insert({ 177, "Estanu" });
    Features.PlanetNameList.insert({ 178, "Fori Prime" });
    Features.PlanetNameList.insert({ 180, "Gacrux" });
    Features.PlanetNameList.insert({ 181, "Gar Haren" });
    Features.PlanetNameList.insert({ 182, "Gatria" });
    Features.PlanetNameList.insert({ 183, "Gemma" });
    Features.PlanetNameList.insert({ 184, "Grand Errant" });
    Features.PlanetNameList.insert({ 185, "Hadar" });
    Features.PlanetNameList.insert({ 186, "Adhara" });
    Features.PlanetNameList.insert({ 187, "Haldus" });
    Features.PlanetNameList.insert({ 188, "Halies Port" });
    Features.PlanetNameList.insert({ 190, "Hesoe Prime" });
    Features.PlanetNameList.insert({ 191, "Heze Bay" });
    Features.PlanetNameList.insert({ 192, "Hort" });
    Features.PlanetNameList.insert({ 193, "Hydrobius" });
    Features.PlanetNameList.insert({ 194, "Karlia" });
    Features.PlanetNameList.insert({ 196, "Keid" });
    Features.PlanetNameList.insert({ 197, "Khandark" });
    Features.PlanetNameList.insert({ 199, "Klaka 5" });
    Features.PlanetNameList.insert({ 200, "Kneth Port" });
    Features.PlanetNameList.insert({ 201, "Kraz" });
    Features.PlanetNameList.insert({ 202, "Kuma" });
    Features.PlanetNameList.insert({ 203, "Lastofe" });
    Features.PlanetNameList.insert({ 204, "Leng Secundus" });
    Features.PlanetNameList.insert({ 205, "Lesath" });
    Features.PlanetNameList.insert({ 206, "Maia" });
    Features.PlanetNameList.insert({ 207, "Malevelon Creek" });
    Features.PlanetNameList.insert({ 208, "Mantes" });
    Features.PlanetNameList.insert({ 209, "Marfark" });
    Features.PlanetNameList.insert({ 210, "Martale" });
    Features.PlanetNameList.insert({ 211, "Matar Bay" });
    Features.PlanetNameList.insert({ 212, "Meissa" });
    Features.PlanetNameList.insert({ 213, "Mekbuda" });
    Features.PlanetNameList.insert({ 214, "Menkent" });
    Features.PlanetNameList.insert({ 215, "Merak" });
    Features.PlanetNameList.insert({ 216, "Merga IV" });
    Features.PlanetNameList.insert({ 217, "Minchir" });
    Features.PlanetNameList.insert({ 218, "Mintoria" });
    Features.PlanetNameList.insert({ 219, "Mordia 9" });
    Features.PlanetNameList.insert({ 221, "Nabatea Secundus" });
    Features.PlanetNameList.insert({ 222, "Navi VII" });
    Features.PlanetNameList.insert({ 223, "Nivel 43" });
    Features.PlanetNameList.insert({ 225, "Oshaune" });
    Features.PlanetNameList.insert({ 226, "Overgoe Prime" });
    Features.PlanetNameList.insert({ 227, "Pandion-XXIV" });
    Features.PlanetNameList.insert({ 228, "Partion" });
    Features.PlanetNameList.insert({ 229, "Peacock" });
    Features.PlanetNameList.insert({ 230, "Phact Bay" });
    Features.PlanetNameList.insert({ 231, "Pherkad Secundus" });
    Features.PlanetNameList.insert({ 232, "Polaris Prime" });
    Features.PlanetNameList.insert({ 233, "Pollux 31" });
    Features.PlanetNameList.insert({ 234, "Prasa" });
    Features.PlanetNameList.insert({ 235, "Propus" });
    Features.PlanetNameList.insert({ 236, "Ras Algethi" });
    Features.PlanetNameList.insert({ 237, "Rd-4" });
    Features.PlanetNameList.insert({ 238, "Rogue 5" });
    Features.PlanetNameList.insert({ 239, "Rirga Bay" });
    Features.PlanetNameList.insert({ 240, "Seasse" });
    Features.PlanetNameList.insert({ 241, "Senge 23" });
    Features.PlanetNameList.insert({ 242, "Setia" });
    Features.PlanetNameList.insert({ 243, "Shete" });
    Features.PlanetNameList.insert({ 244, "Siemnot" });
    Features.PlanetNameList.insert({ 245, "Sirius" });
    Features.PlanetNameList.insert({ 246, "Skat Bay" });
    Features.PlanetNameList.insert({ 247, "Spherion" });
    Features.PlanetNameList.insert({ 248, "Stor Tha Prime" });
    Features.PlanetNameList.insert({ 249, "Stout" });
    Features.PlanetNameList.insert({ 250, "Termadon" });
    Features.PlanetNameList.insert({ 251, "Tibit" });
    Features.PlanetNameList.insert({ 252, "Tien Kwan" });
    Features.PlanetNameList.insert({ 253, "Troost" });
    Features.PlanetNameList.insert({ 254, "Ubanea" });
    Features.PlanetNameList.insert({ 256, "Vandalon IV" });
    Features.PlanetNameList.insert({ 257, "Varylia 5" });
    Features.PlanetNameList.insert({ 258, "Wasat" });
    Features.PlanetNameList.insert({ 259, "Vega Bay" });
    Features.PlanetNameList.insert({ 260, "Wezen" });
    Features.PlanetNameList.insert({ 261, "Vindemitarix Prime" });
    Features.PlanetNameList.insert({ 262, "X-45" });
    Features.PlanetNameList.insert({ 263, "Yed Prior" });
    Features.PlanetNameList.insert({ 264, "Zefia" });
    Features.PlanetNameList.insert({ 265, "Zosma" });
    Features.PlanetNameList.insert({ 266, "Zzaniah Prime" });
    Features.PlanetNameList.insert({ 267, "Skitter" });
    Features.PlanetNameList.insert({ 268, "Euphoria III" });
    Features.PlanetNameList.insert({ 269, "Diaspora X" });
    Features.PlanetNameList.insert({ 270, "Gemstone Bluffs" });
    Features.PlanetNameList.insert({ 271, "Zagon Prime" });
    Features.PlanetNameList.insert({ 272, "Omicron" });
    Features.PlanetNameList.insert({ 273, "Cyberstan" });
}

void features::LoadPlanetList()
{
    string key = "PlanetList";
    CheckFeatureExist(key);

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    if (!Config.pPlanetDataAddress)
    {
        return;
    }

    if (!TempData.Address)
    {
        TempData.Address = Config.pPlanetDataAddress;
    }

    InitPlanetNameList();

    for (int i = 0; i < 404; i++)
    {
        uintptr_t PlanetNameAddress = *(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(TempData.Address) + (i * 8)));
        string PlanetName;

        if (PlanetNameList.find(i) != PlanetNameList.end())
        {
            PlanetName = PlanetNameList[i];
        }
        else if (*(char*)(PlanetNameAddress) != 0)
        {
            PlanetName = (char*)(PlanetNameAddress);
            int SlashIndex = PlanetName.find_last_of('/');
            PlanetName = PlanetName.substr(SlashIndex + 1);
        }
        else
        {
            PlanetNameAddress++;
            PlanetName = (char*)(PlanetNameAddress);
        }


        uintptr_t PlanetHexAddress = (*(uintptr_t*)(TempData.Address + 8)) + (i * 4);
        int HexValue = *(uintptr_t*)(PlanetHexAddress);

        Planet_Data TempPlanetData;
        TempPlanetData.HexValue = HexValue;
        TempPlanetData.Name = PlanetName;

        PlanetList.push_back(TempPlanetData);
    }

    Config.bPlanetListLoaded = true;
}

void features::FilterPlanetList()
{
    if (PlanetList.empty())
    {
        return;
    }

    FilteredPlanetList.clear();

    string strFilterName = Config.szPlanetFilter;
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
    for (int i = 0; i < PlanetList.size(); i++)
    {
        if (nIndexSearch >= 0 && nIndexSearch == i)
        {
            FilteredPlanetList.push_back({ i, PlanetList[i].Name });
            break;
        }

        string strName = PlanetList[i].Name;
        transform(strName.begin(), strName.end(), strName.begin(), ::toupper);
        if (strName.find(strFilterName) != string::npos)
        {
            FilteredPlanetList.push_back({ i, PlanetList[i].Name });
        }
    }
}

void features::ReplacePlanet()
{
    string key = "PlanetList";
    CheckFeatureExist(key);

    auto it = FeatureList.find(key);
    FeatureData& TempData = it->second;

    if (!Config.pSelectedPlanetAddress)
    {
        return;
    }

    if (Config.nPlanetReplaceID < 0 || Config.nPlanetReplaceID >(PlanetList.size() - 1))
    {
        return;
    }

    memcpy((uintptr_t*)(Config.pSelectedPlanetAddress), &(PlanetList[Config.nPlanetReplaceID].HexValue), 4);
    Config.nPlanetCurrentID = Config.nPlanetReplaceID;
    strcpy(Config.szCurrentPlanetName, PlanetList[Config.nPlanetReplaceID].Name.c_str());
}