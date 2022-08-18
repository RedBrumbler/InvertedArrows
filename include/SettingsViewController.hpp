#pragma once
#include "HMUI/ViewController.hpp"

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"


DECLARE_CLASS_CODEGEN(InvertedArrows, SettingsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(ListWrapper<StringW>, _directions);
    DECLARE_INSTANCE_METHOD(ListWrapper<StringW>, get_directions);
    DECLARE_INSTANCE_METHOD(int, StringWToListIndex, StringW value);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

    DECLARE_BSML_PROPERTY(bool, enabled);
    DECLARE_BSML_PROPERTY(bool, alsoFlipBombY);
    DECLARE_BSML_PROPERTY(bool, useCustomDirections);
    
    DECLARE_BSML_PROPERTY(StringW, upDirection);
    DECLARE_BSML_PROPERTY(StringW, downDirection);
    DECLARE_BSML_PROPERTY(StringW, leftDirection);
    DECLARE_BSML_PROPERTY(StringW, rightDirection);

    DECLARE_BSML_PROPERTY(StringW, upLeftDirection);
    DECLARE_BSML_PROPERTY(StringW, upRightDirection);
    DECLARE_BSML_PROPERTY(StringW, downLeftDirection);
    DECLARE_BSML_PROPERTY(StringW, downRightDirection);
)