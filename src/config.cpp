#include "config.hpp"

extern Logger& getLogger();
extern ModInfo modInfo;
config_t config;

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

void SaveConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("enabled", config.enabled, allocator);
    getConfig().config.AddMember("alsoFlipBombY", config.alsoFlipBombY, allocator);
    getConfig().config.AddMember("useCustomDirections", config.useCustomDirections, allocator);
    getConfig().config.AddMember("upDirection", config.upDirection, allocator);
    getConfig().config.AddMember("downDirection", config.downDirection, allocator);
    getConfig().config.AddMember("leftDirection", config.leftDirection, allocator);
    getConfig().config.AddMember("rightDirection", config.rightDirection, allocator);
    getConfig().config.AddMember("upLeftDirection", config.upLeftDirection, allocator);
    getConfig().config.AddMember("upRightDirection", config.upRightDirection, allocator);
    getConfig().config.AddMember("downLeftDirection", config.downLeftDirection, allocator);
    getConfig().config.AddMember("downRightDirection", config.downRightDirection, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    LoggerContextObject logger = getLogger().WithContext("Config Load");
    logger.info("Loading Configuration...");
    getConfig().Load();
    bool foundEverything = true;

    if(getConfig().config.HasMember("enabled") && getConfig().config["enabled"].IsBool()){
        config.enabled = getConfig().config["enabled"].GetBool();    
        if (config.enabled && !getenv("InvertedArrowsEnabled")) setenv("InvertedArrowsEnabled", "yes", 1);
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("alsoFlipBombY") && getConfig().config["alsoFlipBombY"].IsBool()){
        config.alsoFlipBombY = getConfig().config["alsoFlipBombY"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("useCustomDirections") && getConfig().config["useCustomDirections"].IsBool()){
        config.useCustomDirections = getConfig().config["useCustomDirections"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("upDirection") && getConfig().config["upDirection"].IsInt()){
        config.upDirection = getConfig().config["upDirection"].GetInt();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("downDirection") && getConfig().config["downDirection"].IsInt()){
        config.downDirection = getConfig().config["downDirection"].GetInt();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("leftDirection") && getConfig().config["leftDirection"].IsInt()){
        config.leftDirection = getConfig().config["leftDirection"].GetInt();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("rightDirection") && getConfig().config["rightDirection"].IsInt()){
        config.rightDirection = getConfig().config["rightDirection"].GetInt();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("upLeftDirection") && getConfig().config["upLeftDirection"].IsInt()){
        config.upLeftDirection = getConfig().config["upLeftDirection"].GetInt();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("upRightDirection") && getConfig().config["upRightDirection"].IsInt()){
        config.upRightDirection = getConfig().config["upRightDirection"].GetInt();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("downLeftDirection") && getConfig().config["downLeftDirection"].IsInt()){
        config.downLeftDirection = getConfig().config["downLeftDirection"].GetInt();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("downRightDirection") && getConfig().config["downRightDirection"].IsInt()){
        config.downRightDirection = getConfig().config["downRightDirection"].GetInt();    
    }else{
        foundEverything = false;
    }

    config.directions = &config.upDirection;

    if (foundEverything) logger.info("Configuration Loaded Succesfully!");
    return foundEverything;
}