#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/GameNoteController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/NoteCutDirection.hpp"
#include "GlobalNamespace/NoteLineLayer.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/BombNoteController.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData_NoteSpawnData.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"

#include "bs-utils/shared/utils.hpp"
#include <string>
#include "config.hpp"
#include "assets.hpp"

#include "SettingsHost.hpp"

#include "custom-types/shared/register.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSMLDataCache.hpp"

ModInfo modInfo;
extern config_t config;
std::string sceneLoadedName = "";

using namespace UnityEngine;

std::string gameCore = "GameCore";

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output);

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

#define INFO_LOG(value...)  getLogger().WithContext("INFO").info(value)
#define ERROR_LOG(value...) getLogger().WithContext("ERROR").error(value)
#define DEBUG_LOG(value...) getLogger().WithContext("DEBUG").debug(value)

unsigned char directionLookup[8] = {1, 0, 3, 2, 7, 6, 5, 4};

MAKE_HOOK_MATCH(GameNoteController_Init, &GlobalNamespace::GameNoteController::Init, void, GlobalNamespace::GameNoteController* self,::GlobalNamespace::NoteData* noteData, float worldRotation, ::UnityEngine::Vector3 moveStartPos, ::UnityEngine::Vector3 moveEndPos, ::UnityEngine::Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, ::GlobalNamespace::NoteVisualModifierType noteVisualModifierType, float cutAngleTolerance, float uniformScale)
{
    DEBUG_LOG("gamenotecontroller init called! direction is %d", noteData->cutDirection.value);
    int value = noteData->cutDirection.value;
    if (!(value & 0b1000) && config.enabled) 
    {
        DEBUG_LOG("value was below 8, inverting direction");
        int newValue = config.useCustomDirections ? config.directions[value] : directionLookup[value];
        noteData->ChangeNoteCutDirection(GlobalNamespace::NoteCutDirection(newValue));
    }
    else if (config.enabled)  
    { 
        DEBUG_LOG("Note was not an arrow");   
    }
    else if (!config.enabled)   
    { 
        DEBUG_LOG("Mod was not enabled");     
    }

    DEBUG_LOG("Value is now %d", noteData->cutDirection.value);
    
    GameNoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity, noteVisualModifierType, cutAngleTolerance, uniformScale);
}

/*
MAKE_HOOK_MATCH(GameNoteController_Init, void, GlobalNamespace::GameNoteController* self, GlobalNamespace::NoteData* noteData, float worldRotation, Vector3 moveStartPos, Vector3 moveEndPos, Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, GlobalNamespace::GameNoteController_GameNoteType gameNoteType, float cutDirectionAngleOffset)
{
    DEBUG_LOG("gamenotecontroller init called! direction is %d", noteData->cutDirection.value);
    int value = noteData->cutDirection.value;
    if (!(value & 0b1000) && config.enabled) 
    {
        DEBUG_LOG("value was below 8, inverting direction");
        int newValue = config.useCustomDirections ? config.directions[value] : directionLookup[value];
        noteData->ChangeNoteCutDirection(GlobalNamespace::NoteCutDirection(newValue));
    }
    else if (config.enabled)  
    { 
        DEBUG_LOG("Note was not an arrow");   
    }
    else if (!config.enabled)   
    { 
        DEBUG_LOG("Mod was not enabled");     
    }

    DEBUG_LOG("Value is now %d", noteData->cutDirection.value);
    
    GameNoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity, gameNoteType, cutDirectionAngleOffset);
}
*/

int layerLookup[3] = {2, 1, 0};

MAKE_HOOK_MATCH(BeatmapObjectSpawnMovementData_GetJumpingNoteSpawnData, &GlobalNamespace::BeatmapObjectSpawnMovementData::GetJumpingNoteSpawnData, GlobalNamespace::BeatmapObjectSpawnMovementData::NoteSpawnData, GlobalNamespace::BeatmapObjectSpawnMovementData* self, GlobalNamespace::NoteData* noteData)
{
    if (config.enabled && config.alsoFlipBombY && noteData->get_colorType() == -1)
        noteData->set_noteLineLayer(layerLookup[noteData->get_noteLineLayer().value]);
    return BeatmapObjectSpawnMovementData_GetJumpingNoteSpawnData(self, noteData);
}

extern "C" void setup(ModInfo& info) 
{
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;
}

extern "C" void load()
{
    if (!LoadConfig()) 
            SaveConfig();

    il2cpp_functions::Init();

    INFO_LOG("Installing hooks");
    LoggerContextObject logger = getLogger().WithContext("Load");
    INSTALL_HOOK(logger, GameNoteController_Init);
    INSTALL_HOOK(logger, BeatmapObjectSpawnMovementData_GetJumpingNoteSpawnData);
    INFO_LOG("Finished installing hooks");

    INFO_LOG("Registering Custom types");
    custom_types::Register::AutoRegister();
    INFO_LOG("Custom types registered");

    INFO_LOG("Registering UI view controllers");
    BSML::Register::RegisterSettingsMenu("Inverted Arrows", MOD_ID "_settings", InvertedArrows::SettingsHost::get_instance(), false);
    INFO_LOG("UI view controllers registered");
}

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output)
{
    LoggerContextObject logger = getLogger().WithContext("getSceneName");
    Il2CppString* csString = UnityEngine::SceneManagement::Scene::GetNameInternal(scene.m_Handle); 
    RET_0_UNLESS(logger, csString);
    output = to_utf8(csstrtostr(csString));
    return true; 
}

BSML_DATACACHE(settings) {
    return IncludedAssets::Settings_bsml;
}