#include "../extern/beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include "modloader/shared/modloader.hpp"

struct config_t {
    bool enabled = false;
    bool useCustomDirections = false;
    bool alsoFlipBombY = false;
    int* directions = nullptr;
    int upDirection = 1;
    int downDirection = 0;
    int leftDirection = 3;
    int rightDirection = 2;
    int upLeftDirection = 7;
    int upRightDirection = 6;
    int downLeftDirection = 5;
    int downRightDirection = 4;
};

Configuration& getConfig();
void SaveConfig(); // literally 1 bool lol ;p
bool LoadConfig();

