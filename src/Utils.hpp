#ifndef NEGATIVEHITBOXES_UTILS_HPP
#define NEGATIVEHITBOXES_UTILS_HPP

#include <Geode/Geode.hpp>
using namespace geode::prelude;

inline const GameObjectType solids[] {
    GameObjectType::Solid,
    GameObjectType::Slope,
};

inline const GameObjectType portals[] {
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

inline const GameObjectType pads[] {
    GameObjectType::GravityPad,
    GameObjectType::YellowJumpPad,
    GameObjectType::PinkJumpPad,
    GameObjectType::RedJumpPad,
    GameObjectType::SpiderPad,
};

// GD calls them rings
inline const GameObjectType rings[] {
    GameObjectType::RedJumpRing,
    GameObjectType::YellowJumpRing,
    GameObjectType::PinkJumpRing,
    GameObjectType::GravityRing,
    GameObjectType::DashRing,
    GameObjectType::GreenRing,
    GameObjectType::DropRing,
    GameObjectType::CustomRing,
};

inline const GameObjectType hazards[] {
    GameObjectType::Hazard,
    GameObjectType::AnimatedHazard
};

inline bool shouldIgnoreObject(GameObject* obj) {
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
    bool safeMode = mod->getSettingValue<bool>("safe-mode");
    bool disableMod = mod->getSettingValue<bool>("disable-mod");

    return safeMode && !disableMod;
}

inline void showSafeModePopup(CCObject* sender, std::function<void(CCObject*)> onYes) {
    if (isSafeModeEnabled()) {
        createQuickPopup(
            "Heads up!",
            "<cy>Safe Mode</c> is <cr>enabled.</c>\n\n"
            "This means that your stats will <cr>not</c> save.\nThis is done to prevent <cr>cheating.</c>\n\n"
            "Would you still like to play this level? (With <cy>Safe Mode</c> <cg>on</c>, obviously)",
            "No", "Yes",
            [onYes, sender](auto, bool button2) {
                if (button2)
                    onYes(sender);
            }
        );
    }
    else
        onYes(sender);
}
#endif