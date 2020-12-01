#include "config.hpp"

extern ModInfo modInfo;

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

void SaveConfig(bool value)
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("Enable Inverted Arrows", value, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    bool enabled = (getConfig().config.HasMember("Enable Inverted Arrows") && getConfig().config["Enable Inverted Arrows"].GetBool());
    if (enabled) setenv("InvertedArrowsEnabled", "yes", 1);
    return enabled;
}