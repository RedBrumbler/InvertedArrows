#include "SettingsViewController.hpp"
#include "config.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"
#include "bs-utils/shared/utils.hpp"

extern ModInfo modInfo;
extern config_t config;
DEFINE_TYPE(InvertedArrows, SettingsViewController);

namespace InvertedArrows {
    void SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::Settings_bsml, get_transform(), this);
    }

    ListWrapper<StringW> SettingsViewController::get_directions() {
        if (!_directions) {
            _directions = List<StringW>::New_ctor();
            _directions->EnsureCapacity(8);
            _directions->Add("Up");
            _directions->Add("Down");
            _directions->Add("Left");
            _directions->Add("Right");
            _directions->Add("Up Left");
            _directions->Add("Up Right");
            _directions->Add("Down Left");
            _directions->Add("Down Right");
        }
        return _directions;
    }

    int SettingsViewController::StringWToListIndex(StringW value) {
        auto dirs = get_directions();
        int v = dirs->IndexOf(value);
        return std::max(0, std::min(7, v));
    }

    bool SettingsViewController::get_enabled() { return config.enabled; }
    void SettingsViewController::set_enabled(bool value) { 
        config.enabled = value; 
        SaveConfig(); 
        if (value) {
            bs_utils::Submission::disable(modInfo);
        } else {
            bs_utils::Submission::enable(modInfo);
        }
    }
    bool SettingsViewController::get_alsoFlipBombY() { return config.alsoFlipBombY; }
    void SettingsViewController::set_alsoFlipBombY(bool value) { config.alsoFlipBombY = value; SaveConfig(); }
    bool SettingsViewController::get_useCustomDirections() { return config.useCustomDirections; }
    void SettingsViewController::set_useCustomDirections(bool value) { config.useCustomDirections = value; SaveConfig(); }
    
    StringW SettingsViewController::get_upDirection() { return get_directions()[config.upDirection]; }
    void SettingsViewController::set_upDirection(StringW value) { config.upDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsViewController::get_downDirection() { return get_directions()[config.downDirection]; }
    void SettingsViewController::set_downDirection(StringW value) { config.downDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsViewController::get_leftDirection() { return get_directions()[config.leftDirection]; }
    void SettingsViewController::set_leftDirection(StringW value) { config.leftDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsViewController::get_rightDirection() { return get_directions()[config.rightDirection]; }
    void SettingsViewController::set_rightDirection(StringW value) { config.rightDirection = StringWToListIndex(value); SaveConfig(); }
    
    StringW SettingsViewController::get_upLeftDirection() { return get_directions()[config.upLeftDirection]; }
    void SettingsViewController::set_upLeftDirection(StringW value) { config.upLeftDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsViewController::get_upRightDirection() { return get_directions()[config.upRightDirection]; }
    void SettingsViewController::set_upRightDirection(StringW value) { config.upRightDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsViewController::get_downLeftDirection() { return get_directions()[config.downLeftDirection]; }
    void SettingsViewController::set_downLeftDirection(StringW value) { config.downLeftDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsViewController::get_downRightDirection() { return get_directions()[config.downRightDirection]; }
    void SettingsViewController::set_downRightDirection(StringW value) { config.downRightDirection = StringWToListIndex(value); SaveConfig(); }
}