#ifndef NEGATIVEHITBOXES_UTILS_HPP
#define NEGATIVEHITBOXES_UTILS_HPP

#include <Geode/Geode.hpp>
using namespace geode::prelude;

inline bool shouldIgnoreObject(GameObject* obj) {
    const GameObjectType solids[] {
        GameObjectType::Solid,
        GameObjectType::Slope,
    };

    const GameObjectType portals[] {
        GameObjectType::CubePortal,
        GameObjectType::ShipPortal,
        GameObjectType::BallPortal,
        GameObjectType::UfoPortal,
        GameObjectType::WavePortal,
        GameObjectType::SpiderPortal,
        GameObjectType::SwingPortal,
        GameObjectType::MiniSizePortal,
        GameObjectType::DualPortal,
        GameObjectType::GravityTogglePortal,
        GameObjectType::InverseGravityPortal,
        GameObjectType::NormalGravityPortal,
        GameObjectType::TeleportPortal,
        GameObjectType::NormalMirrorPortal,
        GameObjectType::SoloPortal
    };

    const GameObjectType pads[] {
        GameObjectType::GravityPad,
        GameObjectType::YellowJumpPad,
        GameObjectType::PinkJumpPad,
        GameObjectType::RedJumpPad,
        GameObjectType::SpiderPad,
    };

    // GD calls them rings
    const GameObjectType rings[] {
        GameObjectType::RedJumpRing,
        GameObjectType::YellowJumpRing,
        GameObjectType::PinkJumpRing,
        GameObjectType::GravityRing,
        GameObjectType::DashRing,
        GameObjectType::GreenRing,
        GameObjectType::DropRing,
        GameObjectType::CustomRing,
        GameObjectType::SpiderOrb,
        GameObjectType::TeleportOrb,
    };

    const GameObjectType hazards[] {
        GameObjectType::Hazard,
        GameObjectType::AnimatedHazard
    };

    auto mod = Mod::get();
    auto type = obj->m_objectType;

    bool ignoreHazards = mod->getSettingValue<bool>("ignore-hazards");
    bool isHazard = std::ranges::any_of(hazards, [&](auto t) { return type == t; });

    if (ignoreHazards && isHazard) {
        return true;
    }

    bool ignoreSolids = mod->getSettingValue<bool>("ignore-solids");
    bool isSolid = std::ranges::any_of(solids, [&](auto t) { return type == t; });

    if (ignoreSolids && isSolid) {
        return true;
    }

    bool ignorePortals = mod->getSettingValue<bool>("ignore-portals");
    bool isPortal =  std::ranges::any_of(portals, [&](auto t) { return type == t; }) || obj->isSpeedObject();

    if (ignorePortals && isPortal) {
        return true;
    }

    bool ignoreRings = mod->getSettingValue<bool>("ignore-rings");
    bool isRing =  std::ranges::any_of(rings, [&](auto t) { return type == t; });

    if (ignoreRings && isRing) {
        return true;
    }

    bool ignorePads = mod->getSettingValue<bool>("ignore-pads");
    bool isPad =  std::ranges::any_of(pads, [&](auto t) { return type == t; });

    if (ignorePads && isPad) {
        return true;
    }

    return false;
}

inline bool isSafeModeEnabled() {
    auto mod = Mod::get();
    auto safeMode = mod->getSettingValue<bool>("safe-mode");
    auto disableMod = mod->getSettingValue<bool>("disable-mod");

    return safeMode && !disableMod;
}

inline int getRandomNumber(int min = 0, int max = 1) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

inline std::string getRandomVoiceline() {
    auto mod = Mod::get();
    auto resourcesPath = mod->getResourcesDir();
    auto fileName = "geo" + numToString(getRandomNumber(1, 3)) + ".ogg";
    auto path = resourcesPath / fileName;

    log::debug("Resources dir: {}", resourcesPath);
    log::debug("Path: {}", path);
    return string::pathToString(path); // this uses geode::utils::string::pathToString()
}

inline void playRandomVoiceline() {
    auto mod = Mod::get();
    auto voicelinesEnabled = mod->getSettingValue<bool>("voicelines");

    if (voicelinesEnabled) {
        auto engine = FMODAudioEngine::sharedEngine();
        engine->playEffect(getRandomVoiceline());
    }
}

inline void showSafeModePopup(CCObject* sender, std::function<void(CCObject*)> onYes) {
    if (isSafeModeEnabled()) {
        createQuickPopup(
            "Heads up!",
            "<cy>Safe Mode</c> is <cr>enabled.</c>\n\n"
            "This means that your stats will <cr>not</c> be saved.\n"
            "This is done to prevent <cr>cheating.</c>\n\n"
            "Would you still like to play this level?",
            "No", "Yes",
            [onYes, sender](auto, bool button2) {
                if (button2) {
                    onYes(sender);
                    playRandomVoiceline();
                }
            }
        );
    }
    else {
        onYes(sender);
        playRandomVoiceline();
    }
}
#endif