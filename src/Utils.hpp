#ifndef NEGATIVEHITBOXES_UTILS_HPP
#define NEGATIVEHITBOXES_UTILS_HPP

#include <Geode/Geode.hpp>
using namespace geode::prelude;

namespace NegativeHitboxes::Utils {
    inline const std::set solids {
        GameObjectType::Solid,
        GameObjectType::Slope,
    };

    inline const std::set portals {
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

    inline const std::set pads {
        GameObjectType::GravityPad,
        GameObjectType::YellowJumpPad,
        GameObjectType::PinkJumpPad,
        GameObjectType::RedJumpPad,
        GameObjectType::SpiderPad,
    };

    // GD calls them rings
    inline const std::set rings {
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

    inline const std::set hazards {
        GameObjectType::Hazard,
        GameObjectType::AnimatedHazard
    };


    // Credit for IDs: https://github.com/Tyegurr/AccurateHitboxesGD/blob/master/src/ids.h
    inline const std::set sawIds = {
        1705, 1706, 1707, 1708, 1709,
        1710, 1734, 1735, 1736, 186,
        187, 188, 740, 741, 742, 678,
        679, 680, 1619, 1620, 1701,
        1702, 1703, 88, 89, 98
    };

    inline bool shouldIgnoreObject(GameObject* obj) {
        auto mod = Mod::get();
        auto type = obj->m_objectType;

        bool ignoreHazards = mod->getSettingValue<bool>("ignore-hazards");

        // saws didnt get affected for some reason
        bool isHazard = hazards.contains(type) || sawIds.contains(obj->m_objectID);

        if (ignoreHazards && isHazard)
            return true;

        bool ignoreSolids = mod->getSettingValue<bool>("ignore-solids");
        bool isSolid = solids.contains(type);

        if (ignoreSolids && isSolid)
            return true;

        bool ignorePortals = mod->getSettingValue<bool>("ignore-portals");
        bool isPortal = portals.contains(type) || obj->isSpeedObject();

        if (ignorePortals && isPortal)
            return true;

        bool ignoreRings = mod->getSettingValue<bool>("ignore-rings");
        bool isRing = rings.contains(type);

        if (ignoreRings && isRing)
            return true;

        bool ignorePads = mod->getSettingValue<bool>("ignore-pads");
        bool isPad =  pads.contains(type);

        if (ignorePads && isPad)
            return true;

        return false;
    }

    inline bool isModEnabled() {
        auto mod = Mod::get();
        return mod->getSettingValue<bool>("enable-mod");
    }

    inline bool isSafeModeEnabled() {
        auto mod = Mod::get();
        auto safeMode = mod->getSettingValue<bool>("safe-mode");

        return safeMode && isModEnabled();
    }

    inline int getRandomNumber(int min = 0, int max = 1) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    inline std::string getRandomVoiceline() {
        auto mod = Mod::get();
        auto resourcesPath = mod->getResourcesDir();
        auto fileName = "geo" + numToString(getRandomNumber(1, 5)) + ".ogg";
        auto path = resourcesPath / fileName;

        log::debug("Resources dir: {}", resourcesPath);
        log::debug("Path: {}", path);
        return string::pathToString(path); // this uses geode::utils::string::pathToString()
    }

    inline void playRandomVoiceline() {
        auto mod = Mod::get();
        auto voicelinesEnabled = mod->getSettingValue<bool>("voicelines") && isModEnabled();

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
                "Would you like to play this level anyway?",
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
}
#endif