#pragma once
#include "HMUI/ViewController.hpp"

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(InvertedArrows, SettingsHost, Il2CppObject,
    DECLARE_INSTANCE_FIELD_DEFAULT(ListWrapper<Il2CppObject*>, _directions, nullptr);
    DECLARE_INSTANCE_METHOD(ListWrapper<Il2CppObject*>, get_directions);
    DECLARE_INSTANCE_METHOD(int, StringWToListIndex, StringW value);

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
    DECLARE_CTOR(ctor);
    public:
        static SettingsHost* get_instance();
    private:
        static SafePtr<SettingsHost> instance;
)