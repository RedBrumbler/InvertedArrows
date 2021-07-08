#include "SettingsViewController.hpp"
#include "config.hpp"


#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include <cstdlib>
#include <vector>

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

extern config_t config;

std::vector<std::string> noteNames = {
    "Up",
    "Down",
    "Left",
    "Right",
    "Up Left",
    "Up Right",
    "Down Left",
    "Down Right"
};

DEFINE_TYPE(InvertedArrows, SettingsViewController);

#define CREATE_NOTESELECTOR(toSelect, name) \
    dropDown = BeatSaberUI::CreateDropdown(container->get_transform(), name, noteNames[toSelect], noteNames, [](std::string value) { \
        int i = 0; \
        for (i = 0; i < 8; i++) \
        { \
           if (noteNames[i] == value) break; \
        } \
        toSelect = i; \
        SaveConfig(); \
    });


void InvertedArrows::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (firstActivation)
    {
        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        Toggle* enableToggle = BeatSaberUI::CreateToggle(container->get_transform(), "Enable Inverted Arrows", config.enabled, [](bool value) { 
                config.enabled = value;
                SaveConfig();
                if (config.enabled)
                {
                    setenv("InvertedArrowsEnabled", "yes", 1);
                }
                else
                {
                    unsetenv("InvertedArrowsEnabled");
                }
            });
        BeatSaberUI::AddHoverHint(enableToggle->get_gameObject(), "Wether the mod will change arrow directions at all, enabling this will disable scores from being submitted to scoresaber (if installed)");
        Toggle* bombFlipToggle = BeatSaberUI::CreateToggle(container->get_transform(), "Also Flip Bomb Y", config.alsoFlipBombY, [](bool value) { 
                config.alsoFlipBombY = value;
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(bombFlipToggle->get_gameObject(), "Wether to also invert the bomb layer when flipping the directions");
        
        Toggle* customDirToggle = BeatSaberUI::CreateToggle(container->get_transform(), "Use Custom Directions", config.useCustomDirections, [](bool value) { 
                config.useCustomDirections = value;
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(customDirToggle->get_gameObject(), "Wether to just invert the directions, or to use the custom directions defined below");

        SimpleTextDropdown* dropDown = nullptr;
        CREATE_NOTESELECTOR(config.upDirection, "Up Replacement Direction");
        CREATE_NOTESELECTOR(config.downDirection, "Down Replacement Direction");
        CREATE_NOTESELECTOR(config.leftDirection, "Left Replacement Direction");
        CREATE_NOTESELECTOR(config.rightDirection, "Right Replacement Direction");
        CREATE_NOTESELECTOR(config.upLeftDirection, "Up Left Replacement Direction");
        CREATE_NOTESELECTOR(config.upRightDirection, "Up Right Replacement Direction");
        CREATE_NOTESELECTOR(config.downLeftDirection, "Down Left Replacement Direction");
        CREATE_NOTESELECTOR(config.downRightDirection, "Down Right Replacement Direction");
    }
}