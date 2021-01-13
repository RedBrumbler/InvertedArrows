#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/GameNoteController.hpp"
#include "GlobalNamespace/GameNoteController_GameNoteType.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/BeatmapSaveData_NoteData.hpp"
#include "GlobalNamespace/NoteCutDirection.hpp"
#include "GlobalNamespace/NoteLineLayer.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/BombNoteController.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData_NoteSpawnData.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"

#include "bs-utils/shared/utils.hpp"
#include <string>
#include "config.hpp"

#include "SettingsViewController.hpp"

#include "custom-types/shared/register.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

ModInfo modInfo;
extern config_t config;
bool debug = true;
std::string sceneLoadedName = "";

std::string gameCore = "GameCore";

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output);

Logger& getLogger() {
  static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
  return *logger;
}


#define INFO_LOG(value...)  getLogger().WithContext("INFO").info(value)
#define ERROR_LOG(value...) getLogger().WithContext("ERROR").error(value)

#if MODDEBUG
#define DEBUG_LOG(value...) if (debug) getLogger().WithContext("DEBUG").debug(value)
#else
#define DEBUG_LOG(value...)
#endif

unsigned char directionLookup[8] = {1, 0, 3, 2, 7, 6, 5, 4};

MAKE_HOOK_OFFSETLESS(GameNoteController_Init, void, GlobalNamespace::GameNoteController* self, GlobalNamespace::NoteData* noteData, float worldRotation, Vector3 moveStartPos, Vector3 moveEndPos, Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, GlobalNamespace::GameNoteController_GameNoteType gameNoteType, float cutDirectionAngleOffset)
{
    DEBUG_LOG("gamenotecontroller init called! direction is %d", noteData->cutDirection.value);
    int value = noteData->cutDirection.value;
    if (!(value & 0b1000) && config.enabled) 
    {
        DEBUG_LOG("value was below 8, inverting direction");
        int newValue = config.useCustomDirections ? config.directions[value] : directionLookup[value];
        noteData->ChangeNoteCutDirection(GlobalNamespace::NoteCutDirection(newValue));
    }
    else if (!config.enabled)  
    { 
        DEBUG_LOG("Note was not an arrow");   
    }
    else if (config.enabled)   
    { 
        DEBUG_LOG("Mod was not enabled");     
    }

    DEBUG_LOG("Value is now %d", noteData->cutDirection.value);
    
    GameNoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity, gameNoteType, cutDirectionAngleOffset);
}

MAKE_HOOK_OFFSETLESS(SceneManager_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene previousActiveScene, UnityEngine::SceneManagement::Scene nextActiveScene)
{
    getSceneName(nextActiveScene, sceneLoadedName);
    DEBUG_LOG("Found scene %s", sceneLoadedName.c_str());

    if (sceneLoadedName == gameCore)
    {
        if (config.enabled)
        {
            INFO_LOG("Disabling score submission because inverted arrows is enabled");
            bs_utils::Submission::disable(modInfo);
        } 
        else 
        {
            INFO_LOG("Enabling score submission because inverted arrows is disabled");
            bs_utils::Submission::enable(modInfo);
        }
    }

    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
}

int layerLookup[3] = {2, 1, 0};

MAKE_HOOK_OFFSETLESS(BeatmapObjectSpawnMovementData_GetJumpingNoteSpawnData, GlobalNamespace::BeatmapObjectSpawnMovementData::NoteSpawnData, GlobalNamespace::BeatmapObjectSpawnMovementData* self, GlobalNamespace::NoteData* noteData)
{
    if (config.alsoFlipBombY && noteData->get_colorType() == -1)
        noteData->set_noteLineLayer(layerLookup[noteData->get_noteLineLayer().value]);
    return BeatmapObjectSpawnMovementData_GetJumpingNoteSpawnData(self, noteData);
}
extern "C" void setup(ModInfo& info) 
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
}

extern "C" void load()
{
    if (!LoadConfig()) 
            SaveConfig();

    il2cpp_functions::Init();
    QuestUI::Init();

    INFO_LOG("Installing hooks");
    LoggerContextObject logger = getLogger().WithContext("Load");
    INSTALL_HOOK_OFFSETLESS(logger, GameNoteController_Init, il2cpp_utils::FindMethodUnsafe("", "GameNoteController", "Init", 10));
    INSTALL_HOOK_OFFSETLESS(logger, SceneManager_ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
    INSTALL_HOOK_OFFSETLESS(logger, BeatmapObjectSpawnMovementData_GetJumpingNoteSpawnData, il2cpp_utils::FindMethodUnsafe("", "BeatmapObjectSpawnMovementData", "GetJumpingNoteSpawnData", 1));
    INFO_LOG("Finished installing hooks");

    INFO_LOG("Registering Custom types");
    custom_types::Register::RegisterType<InvertedArrows::SettingsViewController>();
    INFO_LOG("Custom types registered");

    INFO_LOG("Registering UI view controllers");
    QuestUI::Register::RegisterModSettingsViewController<InvertedArrows::SettingsViewController*>((ModInfo){"Inverted Arrows", VERSION}, "Inverted Arrows");
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