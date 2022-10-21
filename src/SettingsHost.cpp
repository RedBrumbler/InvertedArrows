#include "SettingsHost.hpp"
#include "config.hpp"

#include "bsml/shared/BSML.hpp"
#include "bs-utils/shared/utils.hpp"

extern ModInfo modInfo;
extern config_t config;
DEFINE_TYPE(InvertedArrows, SettingsHost);

namespace InvertedArrows {
    SafePtr<SettingsHost> SettingsHost::instance;
    SettingsHost* SettingsHost::get_instance() {
        if (!instance) {
            instance = SettingsHost::New_ctor();
        }
        return instance.ptr();
    }

    void SettingsHost::ctor() {
        if (config.enabled) {
            bs_utils::Submission::disable(modInfo);
        } else {
            bs_utils::Submission::enable(modInfo);
        }
    }

    ListWrapper<Il2CppObject*> SettingsHost::get_directions() {
        if (!_directions) {
            _directions = List<Il2CppObject*>::New_ctor();
            _directions->EnsureCapacity(8);
            _directions->Add(StringW("Up"));
            _directions->Add(StringW("Down"));
            _directions->Add(StringW("Left"));
            _directions->Add(StringW("Right"));
            _directions->Add(StringW("Up Left"));
            _directions->Add(StringW("Up Right"));
            _directions->Add(StringW("Down Left"));
            _directions->Add(StringW("Down Right"));
        }
        return _directions;
    }

    int SettingsHost::StringWToListIndex(StringW value) {
        auto dirs = get_directions();
        int v = dirs->IndexOf(value);
        return std::max(0, std::min(7, v));
    }

    bool SettingsHost::get_enabled() { return config.enabled; }
    void SettingsHost::set_enabled(bool value) { 
        config.enabled = value; 
        SaveConfig(); 
        if (value) {
            bs_utils::Submission::disable(modInfo);
        } else {
            bs_utils::Submission::enable(modInfo);
        }
    }
    bool SettingsHost::get_alsoFlipBombY() { return config.alsoFlipBombY; }
    void SettingsHost::set_alsoFlipBombY(bool value) { config.alsoFlipBombY = value; SaveConfig(); }
    bool SettingsHost::get_useCustomDirections() { return config.useCustomDirections; }
    void SettingsHost::set_useCustomDirections(bool value) { config.useCustomDirections = value; SaveConfig(); }
    
    StringW SettingsHost::get_upDirection() { return get_directions()[config.upDirection]; }
    void SettingsHost::set_upDirection(StringW value) { config.upDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsHost::get_downDirection() { return get_directions()[config.downDirection]; }
    void SettingsHost::set_downDirection(StringW value) { config.downDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsHost::get_leftDirection() { return get_directions()[config.leftDirection]; }
    void SettingsHost::set_leftDirection(StringW value) { config.leftDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsHost::get_rightDirection() { return get_directions()[config.rightDirection]; }
    void SettingsHost::set_rightDirection(StringW value) { config.rightDirection = StringWToListIndex(value); SaveConfig(); }
    
    StringW SettingsHost::get_upLeftDirection() { return get_directions()[config.upLeftDirection]; }
    void SettingsHost::set_upLeftDirection(StringW value) { config.upLeftDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsHost::get_upRightDirection() { return get_directions()[config.upRightDirection]; }
    void SettingsHost::set_upRightDirection(StringW value) { config.upRightDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsHost::get_downLeftDirection() { return get_directions()[config.downLeftDirection]; }
    void SettingsHost::set_downLeftDirection(StringW value) { config.downLeftDirection = StringWToListIndex(value); SaveConfig(); }
    StringW SettingsHost::get_downRightDirection() { return get_directions()[config.downRightDirection]; }
    void SettingsHost::set_downRightDirection(StringW value) { config.downRightDirection = StringWToListIndex(value); SaveConfig(); }
}