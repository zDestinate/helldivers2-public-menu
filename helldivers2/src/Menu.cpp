#define _CRT_SECURE_NO_WARNINGS

#include "../pch.h"
#include "../include/Menu.hpp"
#include "../config.h"
#include "../include/FileManager.h"
#include "../include/util.h"
#include "../libs/xorstr.hpp"

std::string GetLabel(json data)
{
    if (data.is_null())
    {
        return "";
    }

    std::wstring wstrTemp = utf_to_wstring(data);
    std::string str = wstring_to_utf8(wstrTemp);
    return str;
}

namespace DX11_Base
{
    // helper variables
    char inputBuffer_getFnAddr[100];
    char inputBuffer_getClass[100];
    char inputBuffer_setWaypoint[32];

    namespace Styles
    {
        float ButtonHeight = 25.0f;

        void InitStyle()
        {
            ImGuiStyle& style = ImGui::GetStyle();
            ImVec4* colors = ImGui::GetStyle().Colors;

            //	STYLE PROPERTIES
            style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
            style.WindowPadding = ImVec2(15, 15);
            style.WindowRounding = 5.0f;
            style.FramePadding = ImVec2(5, 5);
            style.FrameRounding = 4.0f;
            style.ItemSpacing = ImVec2(12, 8);
            style.ItemInnerSpacing = ImVec2(8, 6);
            style.IndentSpacing = 25.0f;
            style.ScrollbarSize = 15.0f;
            style.ScrollbarRounding = 9.0f;
            style.GrabMinSize = 5.0f;
            style.GrabRounding = 3.0f;

            ImGui::StyleColorsClassic();

            if (g_Menu->dbg_RAINBOW_THEME)
            {
                //  RGB MODE STLYE PROPERTIES
                colors[ImGuiCol_Separator] = ImVec4(g_Menu->dbg_RAINBOW);
                colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 1.0f);
            }
            else
            {
                /// YOUR DEFAULT STYLE PROPERTIES HERE
                colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
                colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
                //colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
                colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
                colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
                colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
                colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
                colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
                colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
                colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
                colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
                colors[ImGuiCol_Tab] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_TabHovered] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_Separator] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
                colors[ImGuiCol_TableHeaderBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_TableBorderLight] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_TableBorderStrong] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            }

            Styles::ButtonHeight = ImGui::GetTextLineHeightWithSpacing() + (style.FramePadding.y * 2) - 7.0f;
        }

#if PRIVATE_VERSION == 1
        float InputWidth = 525.0f;
#else
        float InputWidth = 505.0f;
#endif
    }


    namespace Tabs
    {
        void TABPlayer()
        {
            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][0]).c_str(), &Config.bInfStamina);

            ImGui::Columns(2);
            if (Config.bInfAmmo_Legit)
                ImGui::BeginDisabled();

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][1]).c_str(), &Config.bInfAmmo))
            {
                Features.SetInfAmmo();
            }

            if (Config.bInfAmmo_Legit)
                ImGui::EndDisabled();


            if (Config.bInfGrenade_Legit)
                ImGui::BeginDisabled();

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][2]).c_str(), &Config.bInfGrenade))
            {
                Features.SetInfGrenade();
            }

            if (Config.bInfGrenade_Legit)
                ImGui::EndDisabled();



            if (Config.bInfStim_Legit)
                ImGui::BeginDisabled();

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][3]).c_str(), &Config.bInfStim))
            {
                Features.SetInfStim();
            }

            if (Config.bInfStim_Legit)
                ImGui::EndDisabled();

            ImGui::NextColumn();

            if (Config.bInfAmmo)
                ImGui::BeginDisabled();

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][4]).c_str(), &Config.bInfAmmo_Legit))
            {
                Features.SetInfAmmo_Legit();
            }

            if (Config.bInfAmmo)
                ImGui::EndDisabled();


            if (Config.bInfGrenade)
                ImGui::BeginDisabled();

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][5]).c_str(), &Config.bInfGrenade_Legit))
            {
                Features.SetInfGrenade_Legit();
            }

            if (Config.bInfGrenade)
                ImGui::EndDisabled();


            if (Config.bInfStim)
                ImGui::BeginDisabled();

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][6]).c_str(), &Config.bInfStim_Legit))
            {
                Features.SetInfStim_Legit();
            }

            if (Config.bInfStim)
                ImGui::EndDisabled();

            ImGui::Columns();
            ImGui::Columns(2);
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][7]).c_str(), &Config.bInfBackpack))
            {
                Features.SetInfBackpack();
            }
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][8]).c_str(), &Config.bInfStratagem);
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][9]).c_str(), &Config.bCombinationAlwaysCorrect))
            {
                Features.SetCombinationAlwaysCorrect();
            }

            ImGui::NextColumn();
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][10]).c_str(), &Config.bNoRagdoll);
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][11]).c_str(), &Config.bInstantStratagemDrop))
            {
                Features.SetInstantStratagemDrop();
            }


            ImGui::Columns();
            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][12]).c_str(), &Config.bTakeNoDamage))
            {
                Features.SetTakeNoDamage();
            }

            ImGui::SetNextItemWidth(Styles::InputWidth);
            ImGui::InputInt("##IntPlayerMinHealth", &Config.nPlayerMinHealth);
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Player"][13]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.SetFreezePlayerHealth();
            }

            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Player"][14]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.SetUnfreezePlayerHealth();
            }


            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::SetNextItemWidth(Styles::InputWidth);
            ImGui::SliderFloat("##SliderPlayerSpeed", &Config.fPlayerSpeed, 1, 14, "%.3f");
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Player"][15]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.SetPlayerSpeed();
            }


            ImGui::Dummy(ImVec2(0.0f, 15.0f));

            if (!Config.bEnablePlayerCoords)
                ImGui::BeginDisabled();

            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Player"][16]).c_str(), &Config.bNoClip);
            ImGui::Columns(3);
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputFloat("##PlayerLocationX", &Config.fPlayerLocationX, 1.0f, 10.0f, "%.5f");
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputFloat("##PlayerLocationY", &Config.fPlayerLocationY, 1.0f, 10.0f, "%.5f");
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputFloat("##PlayerLocationZ", &Config.fPlayerLocationZ, 1.0f, 10.0f, "%.5f");
            ImGui::Columns();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Player"][17]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 3 - 5, Styles::ButtonHeight)))
            {
                Features.PlayerTeleport();
            }
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Player"][18]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.PlayerCopyLocation();
            }
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Player"][19]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.PlayerTeleportWaypoint();
            }

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            char szPlayerLocation[255];
            if (Config.pPlayerLocationAddress)
            {
                Vector3D_Data* LocationData = (Vector3D_Data*)Config.pPlayerLocationAddress;
                sprintf(szPlayerLocation, "X: %.5f  Y: %.5f  Z: %.5f", LocationData->X, LocationData->Y, LocationData->Z);
            }
            else
            {
                sprintf(szPlayerLocation, "X: Unknown  Y: Unknown  Z: Unknown");
            }
            ImGui::Text(szPlayerLocation);

            if (!Config.bEnablePlayerCoords)
                ImGui::EndDisabled();
        }

        void TABWeapon()
        {
            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));

            ImGui::Columns(2);
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Weapon"][0]).c_str(), &Config.bNoReload))
            {
                Features.SetNoReload();
            }

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Weapon"][1]).c_str(), &Config.bNoLaserCannonOverheat))
            {
                Features.SetNoLaserOverheat();
            }

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Weapon"][2]).c_str(), &Config.bInstantCharge))
            {
                Features.SetInstantCharge();
            }

            ImGui::NextColumn();
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Weapon"][3]).c_str(), &Config.bNoRecoil);
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Weapon"][4]).c_str(), &Config.bNoSway))
            {
                Features.SetNoSway();
            }
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Weapon"][5]).c_str(), &Config.bInfSpecialWeapon);


            ImGui::Columns();
            ImGui::Dummy(ImVec2(0.0f, 15.0f));

            if(!Config.bWeaponDataLoaded)
                ImGui::BeginDisabled();

            ImGui::Text("%s (1 - %s)", GetLabel(g_FileManager.Language["Localization"]["Weapon"][6]).c_str(),
                Config.nTotalWeaponID ? std::to_string(Config.nTotalWeaponID).c_str() : "Unknown");
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponLoadoutPrimaryID", &Config.nWeaponLoadoutPrimaryID);
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Weapon"][7]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.SetWeaponLoadout(1);
            }

            ImGui::Text("%s (1 - %s)", GetLabel(g_FileManager.Language["Localization"]["Weapon"][8]).c_str(),
                Config.nTotalWeaponID ? std::to_string(Config.nTotalWeaponID).c_str() : "Unknown");
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponLoadoutSecondaryID", &Config.nWeaponLoadoutSecondaryID);
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Weapon"][9]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.SetWeaponLoadout(2);
            }

            ImGui::Text("%s (1 - %s)", GetLabel(g_FileManager.Language["Localization"]["Weapon"][10]).c_str(),
                Config.nTotalWeaponGrenadeID ? std::to_string(Config.nTotalWeaponGrenadeID).c_str() : "Unknown");
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponLoadoutGrenadeID", &Config.nWeaponLoadoutGrenadeID);
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Weapon"][11]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.SetWeaponLoadout(3);
            }

            if (!Config.bWeaponDataLoaded)
                ImGui::EndDisabled();

            if (Config.bWeaponDataLoaded)
                ImGui::BeginDisabled();

            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Weapon"][12]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.LoadWeaponDataList();
            }

            if (Config.bWeaponDataLoaded)
                ImGui::EndDisabled();
        }

        void TABStratagem()
        {
            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));
            if (!Config.bStratagemListLoaded)
                ImGui::BeginDisabled();

            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][0]).c_str());
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            if (ImGui::BeginCombo("##ComboStratagemPlayerSelect", Config.szStratagemPlayerID))
            {
                for (int i = 0; i < Config.pStratagemSlotAddress.size(); i++)
                {
                    char szSelection[255];
                    sprintf(szSelection, "%s %u", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][1]).c_str(),  i + 1);
                    bool is_selected = (Config.szStratagemPlayerID == szSelection);
                    if (ImGui::Selectable(szSelection, is_selected))
                    {
                        strcpy(Config.szStratagemPlayerID, szSelection);
                        Config.nStratagemPlayerIndex = i;
                    }
                }
                ImGui::EndCombo();
            }


            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Columns(3);
            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][2]).c_str());
            ImGui::Dummy(ImVec2(0.0f, 6.0f));
            ImGui::Text("%s 1: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[0].OriginalID);
            ImGui::Dummy(ImVec2(0.0f, 1.0f));
            ImGui::Text("%s 2: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[1].OriginalID);
            ImGui::Dummy(ImVec2(0.0f, 2.0f));
            ImGui::Text("%s 3: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[2].OriginalID);
            ImGui::Dummy(ImVec2(0.0f, 2.0f));
            ImGui::Text("%s 4: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[3].OriginalID);
            ImGui::Dummy(ImVec2(0.0f, 2.0f));
            ImGui::Text("%s 5*: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[4].OriginalID);
            ImGui::Dummy(ImVec2(0.0f, 2.0f));
            ImGui::Text("%s 6*: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[5].OriginalID);
            ImGui::Dummy(ImVec2(0.0f, 2.0f));
            ImGui::Text("%s 7*: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[6].OriginalID);
            ImGui::Dummy(ImVec2(0.0f, 2.0f));
            ImGui::Text("%s 8*: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[7].OriginalID);
            ImGui::Dummy(ImVec2(0.0f, 2.0f));
            ImGui::Text("%s 9: %d", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][3]).c_str(), Features.StratagemPlayerSlotData[8].OriginalID);

            ImGui::NextColumn();
            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][4]).c_str());
            ImGui::Dummy(ImVec2(0.0f, 0.0f));
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID1", &Features.StratagemPlayerSlotData[0].NewID);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID2", &Features.StratagemPlayerSlotData[1].NewID);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID3", &Features.StratagemPlayerSlotData[2].NewID);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID4", &Features.StratagemPlayerSlotData[3].NewID);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID5", &Features.StratagemPlayerSlotData[4].NewID);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID6", &Features.StratagemPlayerSlotData[5].NewID);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID7", &Features.StratagemPlayerSlotData[6].NewID);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID8", &Features.StratagemPlayerSlotData[7].NewID);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewID9", &Features.StratagemPlayerSlotData[8].NewID);

            ImGui::NextColumn();
            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][5]).c_str());
            ImGui::Dummy(ImVec2(0.0f, 0.0f));
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount1", &Features.StratagemPlayerSlotData[0].Amount);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount2", &Features.StratagemPlayerSlotData[1].Amount);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount3", &Features.StratagemPlayerSlotData[2].Amount);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount4", &Features.StratagemPlayerSlotData[3].Amount);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount5", &Features.StratagemPlayerSlotData[4].Amount);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount6", &Features.StratagemPlayerSlotData[5].Amount);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount7", &Features.StratagemPlayerSlotData[6].Amount);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount8", &Features.StratagemPlayerSlotData[7].Amount);
            ImGui::SetNextItemWidth(Styles::InputWidth / 3 - 10);
            ImGui::InputInt("##IntPlayerSlotDataNewAmount9", &Features.StratagemPlayerSlotData[8].Amount);

            ImGui::Columns();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Stratagems"][6]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 3 - 5, Styles::ButtonHeight)))
            {
                Features.ReplaceStratagem();
            }
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Stratagems"][7]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2, Styles::ButtonHeight)))
            {
                Features.LoadPlayerStratagem();
            }
            if (!Config.bStratagemListLoaded)
                ImGui::EndDisabled();

            if (Config.bStratagemListLoaded)
                ImGui::BeginDisabled();
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Stratagems"][8]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.LoadStratagemList();
            }
            if (Config.bStratagemListLoaded)
                ImGui::EndDisabled();

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Stratagems"][9]).c_str());
            ImGui::SetNextItemWidth(Styles::InputWidth);
            if (ImGui::InputText("##StrStratagemFilter", Config.szStratagemFilter, 255))
            {
                Features.FilterStratagemList();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5, 5));
            if (ImGui::BeginTable("##TableStratagemList", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter, ImVec2(ImGui::GetContentRegionAvail().x, 200)))
            {
                ImGui::TableSetupColumn(GetLabel(g_FileManager.Language["Localization"]["Stratagems"][10]).c_str(), ImGuiTableColumnFlags_WidthFixed, 25.0f);
                ImGui::TableSetupColumn(GetLabel(g_FileManager.Language["Localization"]["Stratagems"][11]).c_str(), ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableHeadersRow();

                if (strlen(Config.szStratagemFilter))
                {
                    if (!Features.FilteredStratagemList.empty())
                    {
                        for (int i = 0; i < Features.FilteredStratagemList.size(); i++)
                        {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("%d", Features.FilteredStratagemList[i].ID);
                            ImGui::TableNextColumn();
                            ImGui::Text("%s", Features.FilteredStratagemList[i].Name.c_str());
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < Features.StratagemList.size(); i++)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("%d", i + 1);
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", Features.StratagemList[i].Name.c_str());
                    }
                }

                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }

        void TABMission()
        {
            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));
            ImGui::Text("%s\n%s: %s\n%s: %s",
                GetLabel(g_FileManager.Language["Localization"]["Mission"][0]).c_str(),
                GetLabel(g_FileManager.Language["Localization"]["Mission"][1]).c_str(),
                Config.pMissionInfo ? std::to_string((int)*(uintptr_t*)(Config.pMissionInfo + 4)).c_str() : "Unknown",
                GetLabel(g_FileManager.Language["Localization"]["Mission"][2]).c_str(),
                Config.pMissionInfo ? std::to_string((int)*(uintptr_t*)(Config.pMissionInfo)).c_str() : "Unknown");

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Columns(2);
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][3]).c_str(), &Config.bReduceEnemyAggro);
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][4]).c_str(), &Config.bNoVaultCheck))
            {
                Features.SetNoVaultCheck();
            }

            ImGui::NextColumn();
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][5]).c_str(), &Config.bShowAllMapIcons))
            {
                Features.SetShowMapAllIcons();
            }

            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][6]).c_str(), &Config.bNoBoundary))
            {
                Features.SetNoBoundary();
            }

            ImGui::Columns();
            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Columns(2);
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][7]).c_str(), &Config.bInstantCompleteOutpost))
            {
                Features.InstantCompleteOutpost();
            }
            ImGui::NextColumn();
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][8]).c_str(), &Config.bShowHiddenOutpost))
            {
                Features.ShowHiddenOutpost();
            }
            ImGui::Columns();
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][9]).c_str(), &Config.bInstantCompleteMission))
            {
                Features.InstantCompleteMission();
            }


            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            if (Config.bPickupSample)
                ImGui::BeginDisabled();

            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][10]).c_str(), &Config.bPickupCustomSample);

            if (Config.bPickupSample)
                ImGui::EndDisabled();

            ImGui::Columns(3);
            ImGui::Text("%s:", GetLabel(g_FileManager.Language["Localization"]["Mission"][11]).c_str());
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputInt("##IntSetNormalSample", &Config.nNormalSample);

            ImGui::NextColumn();
            ImGui::Text("%s:", GetLabel(g_FileManager.Language["Localization"]["Mission"][12]).c_str());
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputInt("##IntSetRareSample", &Config.nRareSample);

            ImGui::NextColumn();
            ImGui::Text("%s:", GetLabel(g_FileManager.Language["Localization"]["Mission"][13]).c_str());
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputInt("##IntSetSuperSample", &Config.nSuperSample);


            if (Config.bPickupCustomSample)
                ImGui::BeginDisabled();
            ImGui::Columns();
            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Mission"][14]).c_str(), &Config.bPickupSample);
            if (Config.bPickupCustomSample)
                ImGui::EndDisabled();

            ImGui::SetNextItemWidth(Styles::InputWidth);
            ImGui::InputInt("##IntPickupSample", &Config.nPickupSample);


            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Text("%s: %02d:%02d:%02d", GetLabel(g_FileManager.Language["Localization"]["Mission"][15]).c_str(),
                (int)(Config.fCustomMissionTime / 60), (int)(Config.fCustomMissionTime) % 60,
                (int)(Config.fCustomMissionTime * 100) % 100);
            ImGui::SetNextItemWidth(Styles::InputWidth);
            ImGui::SliderFloat("##FloatCustomMissionTime", &Config.fCustomMissionTime, 1, 2490, "%.2f");
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Mission"][16]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.SetCustomMissionTime();
            }

            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Mission"][17]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.UnfreezeCustomMissionTime();
            }


            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Text("%s: %02d:%02d:%02d", GetLabel(g_FileManager.Language["Localization"]["Mission"][18]).c_str(),
                (int)(Config.fMaxExtractionTime / 60), (int)(Config.fMaxExtractionTime) % 60,
                (int)(Config.fMaxExtractionTime * 100) % 100);
            ImGui::SetNextItemWidth(Styles::InputWidth);
            ImGui::SliderFloat("##FloatMaxExtractionTime", &Config.fMaxExtractionTime, 0, 600, "%.2f");
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Mission"][19]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.SetMaxExtractionTime();
            }

            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Mission"][20]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.UnfreezeExtractionTime();
            }
        }

        void TABPlanet()
        {
            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));
            if (!Config.bPlanetListLoaded)
                ImGui::BeginDisabled();
            ImGui::Columns(2);
            ImGui::Text("%s\n%s: %d\n%s: %s", GetLabel(g_FileManager.Language["Localization"]["Planet"][0]).c_str(),
                GetLabel(g_FileManager.Language["Localization"]["Planet"][1]).c_str(), Config.nPlanetCurrentID,
                GetLabel(g_FileManager.Language["Localization"]["Planet"][2]).c_str(), Config.szCurrentPlanetName);
            ImGui::NextColumn();
            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Planet"][3]).c_str());
            ImGui::SetNextItemWidth(Styles::InputWidth / 2 - 10);
            ImGui::InputInt("##IntPlanetSourceID", &Config.nPlanetReplaceID);

            ImGui::Columns();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Planet"][4]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 10, Styles::ButtonHeight)))
            {
                Features.ReplacePlanet();
            }
            if (!Config.bPlanetListLoaded)
                ImGui::EndDisabled();

            if (Config.bPlanetListLoaded)
                ImGui::BeginDisabled();
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Planet"][5]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.LoadPlanetList();
            }
            if (Config.bPlanetListLoaded)
                ImGui::EndDisabled();

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Planet"][6]).c_str());
            ImGui::SetNextItemWidth(Styles::InputWidth);
            if (ImGui::InputText("##StrPlanetFilter", Config.szPlanetFilter, 255))
            {
                Features.FilterPlanetList();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5, 5));
            if (ImGui::BeginTable("##TablePlanetList", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter, ImVec2(ImGui::GetContentRegionAvail().x, 200)))
            {
                ImGui::TableSetupColumn(GetLabel(g_FileManager.Language["Localization"]["Planet"][1]).c_str(), ImGuiTableColumnFlags_WidthFixed, 25.0f);
                ImGui::TableSetupColumn(GetLabel(g_FileManager.Language["Localization"]["Planet"][7]).c_str(), ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableHeadersRow();

                if (strlen(Config.szPlanetFilter))
                {
                    if (!Features.FilteredPlanetList.empty())
                    {
                        for (int i = 0; i < Features.FilteredPlanetList.size(); i++)
                        {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("%d", Features.FilteredPlanetList[i].ID);
                            ImGui::TableNextColumn();
                            ImGui::Text("%s", Features.FilteredPlanetList[i].Name.c_str());
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < Features.PlanetList.size(); i++)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("%d", i);
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", Features.PlanetList[i].Name.c_str());
                    }
                }


                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }

        void TABSpawner()
        {
            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));
			
			/*
			XXXXXXXXXXXX
			*/
        }

        void TABData()
        {
            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));

            ImGui::Text("%s: %s", GetLabel(g_FileManager.Language["Localization"]["Data"][0]).c_str(), Config.szWeaponDamageIDName);
            ImGui::Text("%s (1 - %s)", GetLabel(g_FileManager.Language["Localization"]["Data"][1]).c_str(),
                Config.nTotalDamageID ? std::to_string(Config.nTotalDamageID).c_str() : "Unknown");
            ImGui::Columns(2);
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponDamageID", &Config.nWeaponDamageID);

            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Data"][2]).c_str());
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponDamage", &Config.nWeaponDamage);

            ImGui::Text("%s 1", GetLabel(g_FileManager.Language["Localization"]["Data"][3]).c_str());
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponPenetration1", &Config.nWeaponPenetrationPower1);

            ImGui::Text("%s 3", GetLabel(g_FileManager.Language["Localization"]["Data"][3]).c_str());
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponPenetration3", &Config.nWeaponPenetrationPower3);

            ImGui::NextColumn();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][4]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.LoadWeaponStats();
            }

            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Data"][5]).c_str());
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponStructureDamage", &Config.nWeaponStructureDamage);

            ImGui::Text("%s 2", GetLabel(g_FileManager.Language["Localization"]["Data"][3]).c_str());
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponPenetration2", &Config.nWeaponPenetrationPower2);

            ImGui::Columns();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][6]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.SetWeaponStats();
            }

            ImGui::SameLine();
            if (Config.nDamageNameListLoaded != 2)
                ImGui::BeginDisabled();

            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][7]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.LoadWeaponDamageNameData();
            }

            if (Config.nDamageNameListLoaded != 2)
                ImGui::EndDisabled();

            ImGui::Columns();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][8]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 3 - 5, Styles::ButtonHeight)))
            {
                Features.WeaponDamageSaveID();
            }
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][9]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.WeaponDamageLoadConfig();
            }
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][10]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.WeaponDamageClearAll();
            }


            //Explosive
            ImGui::Columns();
            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Text("%s: %s", GetLabel(g_FileManager.Language["Localization"]["Data"][11]).c_str(), Config.szWeaponExplosiveIDName);
            ImGui::Text("%s (1 - %s)", GetLabel(g_FileManager.Language["Localization"]["Data"][12]).c_str(),
                Config.nTotalExplosiveID ? std::to_string(Config.nTotalExplosiveID).c_str() : "Unknown");
            ImGui::Columns(2);
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponExplosiveID", &Config.nWeaponExplosiveID);

            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Data"][13]).c_str());
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputInt("##IntWeaponExplosiveDamageID", &Config.nWeaponExplosiveDamageID);

            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Data"][14]).c_str());
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputFloat("##FloatWeaponExplosiveRadiusFalloff", &Config.fWeaponExplosiveRadiusFalloff, 0.1f, 1.0f, "%.1f");

            ImGui::NextColumn();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][15]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.LoadExplosiveStats();
            }

            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Data"][16]).c_str());
            ImGui::SetNextItemWidth((Styles::InputWidth / 2) - 10);
            ImGui::InputFloat("##FloatWeaponExplosiveRadiusInner", &Config.fWeaponExplosiveRadiusInner, 0.1f, 1.0f, "%.2f");

            ImGui::Columns();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][17]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.SetExplosiveStats();
            }

            ImGui::SameLine();
            if (Config.nExplosiveNameListLoaded != 2)
                ImGui::BeginDisabled();

            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][18]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.LoadWeaponExplosiveNameData();
            }

            if (Config.nExplosiveNameListLoaded != 2)
                ImGui::EndDisabled();

            ImGui::Columns();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][19]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 3 - 5, Styles::ButtonHeight)))
            {
                Features.WeaponExplosiveSaveID();
            }
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][20]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.WeaponExplosiveLoadConfig();
            }
            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Data"][21]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.WeaponExplosiveClearAll();
            }
        }

        void TABMisc()
        {
            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));

            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Misc"][0]).c_str(), &Config.bUnlockAllStratagems);
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Misc"][1]).c_str(), &Config.bUnlockAllEquipment);
            ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Misc"][2]).c_str(), &Config.bUnlockAllArmor);

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Columns(2);
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Misc"][3]).c_str(), &Config.bNoJetpackCooldown))
            {
                Features.SetNoJetpackCooldown();
            }

            ImGui::NextColumn();
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Misc"][4]).c_str(), &Config.bNoStationaryTurretOverheat))
            {
                Features.SetNoStationaryTurretOverheat();
            }

			ImGui::Columns();
            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            if (ImGui::Checkbox(GetLabel(g_FileManager.Language["Localization"]["Misc"][5]).c_str(), &Config.bBackpackShieldSetting))
            {
                Features.SetBackpackShieldSetting();
            }

            if (Config.bBackpackShieldSetting)
                ImGui::BeginDisabled();

            ImGui::Columns(2);
            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Misc"][6]).c_str());
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::SliderFloat("##SliderBackpackShieldCooldownTime", &Config.fBackpackShieldCooldownTime, 0.0f, 60.0f, "%.2f");

            ImGui::NextColumn();
            ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Misc"][7]).c_str());
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::SliderFloat("##SliderBackpackShieldMaxEnergy", &Config.fBackpackShieldMaxEnergy, 1.0f, 500.0f, "%.0f");

            if (Config.bBackpackShieldSetting)
                ImGui::EndDisabled();


            ImGui::Columns();
            ImGui::Dummy(ImVec2(0.0f, 15.0f));

            ImGui::Columns(2);
            ImGui::AlignTextToFramePadding();
			ImGui::Text("%s", GetLabel(g_FileManager.Language["Localization"]["Misc"][8]).c_str());
            ImGui::NextColumn();
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputText("##strSpawnEntityReinforcementName", Config.szSpawnEntityReinforcementName, 255);
            const bool bOpen_SpawnEntityReinforcement = ImGui::IsItemActive();
            const bool bActivated_SpawnEntityReinforcement = ImGui::IsItemActivated();
            if (bActivated_SpawnEntityReinforcement)
            {
                ImGui::OpenPopup("##popupSpawnEntityReinforcementList");
            }

            {
                ImGuiWindowFlags popupFlags = ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_ChildWindow;

                ImGui::SetNextWindowPos({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y });
                ImGui::SetNextWindowSizeConstraints({ ImGui::GetItemRectSize().x , 0.f}, { ImGui::GetItemRectSize().x, 250.f });

                if (ImGui::BeginPopup("##popupSpawnEntityReinforcementList", popupFlags))
                {
                    for (auto const& EnemyEntity : Features.EnemyEntityList)
                    {
                        std::string strCurrentEntityName = Config.szSpawnEntityReinforcementName;
                        transform(strCurrentEntityName.begin(), strCurrentEntityName.end(), strCurrentEntityName.begin(), ::tolower);
                        std::string strCurrentNameFromList = EnemyEntity.Name;
                        transform(strCurrentNameFromList.begin(), strCurrentNameFromList.end(), strCurrentNameFromList.begin(), ::tolower);

                        if (strCurrentNameFromList.find(strCurrentEntityName) == std::string::npos)
                            continue;

                        if (ImGui::Selectable(EnemyEntity.Name.c_str()))
                        {
                            ImGui::ClearActiveID();
                            strcpy(Config.szSpawnEntityReinforcementName, EnemyEntity.Name.c_str());
                            Config.nSpawnEntityReinforcementData = EnemyEntity.Data;
                        }
                    }

                    if (!bOpen_SpawnEntityReinforcement && !ImGui::IsWindowFocused())
                    {
                        if (!strlen(Config.szSpawnEntityReinforcementName))
                        {
                            strcpy(Config.szSpawnEntityReinforcementName, "Default");
                            Config.nSpawnEntityReinforcementData = 0;
                        }

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
            }


#if PRIVATE_VERSION == 1
            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::SetNextItemWidth(Styles::InputWidth);
            ImGui::InputInt("##IntArmorRating", &Config.nArmorRating);
            if (ImGui::Button("Set Armor Rating", ImVec2(ImGui::GetContentRegionAvail().x, 25)))
            {

            }

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::SetNextItemWidth(Styles::InputWidth);
            ImGui::InputInt("##IntArmorSpeed", &Config.nArmorSpeed);
            if (ImGui::Button("Set Armor Speed", ImVec2(ImGui::GetContentRegionAvail().x, 25)))
            {

            }

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::SetNextItemWidth(Styles::InputWidth);
            ImGui::InputInt("##IntArmorRating", &Config.nArmorStaminaRegen);
            if (ImGui::Button("Set Armor Stamina Regen", ImVec2(ImGui::GetContentRegionAvail().x, 25)))
            {

            }
#endif
            ImGui::Columns();
            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Misc"][9]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, Styles::ButtonHeight)))
            {
                Features.SaveAllConfig();
            }

            ImGui::SameLine();
            if (ImGui::Button(GetLabel(g_FileManager.Language["Localization"]["Misc"][10]).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, Styles::ButtonHeight)))
            {
                Features.LoadAllConfig();
            }
        }

        void TABAbout()
        {
#ifdef VMPROTECT
            VMProtectBeginUltra("TABAbout");
            if (!VMProtectIsProtected())
            {
                return;
            }
#endif

            ImGui::Dummy(ImVec2(Styles::InputWidth, 0.0f));

            ImGui::Columns(2);
#if PRIVATE_VERSION == 1
            ImGui::Text(_XOR_("Title: Helldivers 2 Menu\nVersion: 1.2.1b\nType: Private\n\nCreated by Destinate"));
#else
            ImGui::Text(_XOR_("Title: Helldivers 2 Menu\nVersion: 1.2.1b\nType: Public\n\nCreated by Destinate"));

            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::Text(_XOR_("Public version is free.\nIf you paid for it,\nthen you got scammed."));
#endif
            ImGui::NextColumn();
            ImGui::Text(_XOR_("Credits:\n\nemoisback\ngir489\ncfemen\nvoid*\nZoDDeL\nExitium\nSleepyCatto\nchris11\nkillekrok555\nimpushingp\nKanna\nErik9631\nNuLLxD"));

            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::Text(_XOR_("UC Community\nFR Community"));

#ifdef VMPROTECT
            VMProtectEnd();
#endif
        }
    }

    void Menu::Draw()
    {
        if (g_GameVariables->m_ShowMenu)
        {
            MainMenu();
        }

        /*
        if (g_GameVariables->m_ShowHud)
        {
            HUD(&g_GameVariables->m_ShowHud);
        }

        if (g_GameVariables->m_ShowDemo)
            ImGui::ShowDemoWindow();
        */
    }

    void Menu::MainMenu()
    {
        if (!g_GameVariables->m_ShowDemo)
            Styles::InitStyle();

        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(g_Menu->dbg_RAINBOW));
        }

        if (!ImGui::Begin(_XOR_("Helldivers 2 Menu (1.2.1b) (Public)"), &g_GameVariables->m_ShowMenu, 96 | ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::End();
            return;
        }
        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }

        ImGuiContext* pImGui = GImGui;

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem(GetLabel(g_FileManager.Language["Localization"]["Tab"][0]).c_str()))
            {
                Tabs::TABPlayer();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(GetLabel(g_FileManager.Language["Localization"]["Tab"][1]).c_str()))
            {
                Tabs::TABWeapon();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(GetLabel(g_FileManager.Language["Localization"]["Tab"][2]).c_str()))
            {
                Tabs::TABStratagem();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(GetLabel(g_FileManager.Language["Localization"]["Tab"][3]).c_str()))
            {
                Tabs::TABMission();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(GetLabel(g_FileManager.Language["Localization"]["Tab"][4]).c_str()))
            {
                Tabs::TABPlanet();
                ImGui::EndTabItem();
            }
#if PRIVATE_VERSION == 1
            if (ImGui::BeginTabItem("Spawner"))
            {
                Tabs::TABSpawner();
                ImGui::EndTabItem();
            }
#endif
            if (ImGui::BeginTabItem(GetLabel(g_FileManager.Language["Localization"]["Tab"][5]).c_str()))
            {
                Tabs::TABData();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(GetLabel(g_FileManager.Language["Localization"]["Tab"][6]).c_str()))
            {
                Tabs::TABMisc();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(GetLabel(g_FileManager.Language["Localization"]["Tab"][7]).c_str()))
            {
                Tabs::TABAbout();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    void Menu::HUD(bool* p_open)
    {
        ImGui::SetNextWindowPos(g_D3D11Window->pViewport->WorkPos);
        ImGui::SetNextWindowSize(g_D3D11Window->pViewport->WorkSize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, NULL);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
        if (!ImGui::Begin("##HUDWINDOW", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoBackground))
        {
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();
            return;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        auto ImDraw = ImGui::GetWindowDrawList();
        auto draw_size = g_D3D11Window->pViewport->WorkSize;
        auto center = ImVec2({ draw_size.x * .5f, draw_size.y * .5f });
        auto top_center = ImVec2({ draw_size.x * .5f, draw_size.y * 0.0f });


        // Watermark
        //ImDraw->AddText(top_center, g_Menu->dbg_RAINBOW, "AAA");

        ImGui::End();
    }
}