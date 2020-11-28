#include "../extern/beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include "modloader/shared/modloader.hpp"

Configuration& getConfig();
void SaveConfig(bool value); // literally 1 bool lol ;p
bool LoadConfig();
