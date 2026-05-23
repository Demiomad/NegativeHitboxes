#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include "../Utils.hpp"

using namespace geode::prelude;

class $modify(SMLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        bool m_shownPopup = false;
    };

    void onPlay(CCObject* sender) {
        if (!m_fields->m_shownPopup) {
            m_fields->m_shownPopup = true;
            showSafeModePopup(sender, [this](auto sender) {
                LevelInfoLayer::onPlay(sender);
            });
        }
        else
            LevelInfoLayer::onPlay(sender);
    }
};

class $modify(SMLevelPage, LevelPage) {
    struct Fields {
        bool m_shownPopup = false;
    };

    void onPlay(CCObject* sender) {
        if (!m_fields->m_shownPopup) {
            m_fields->m_shownPopup = true;
            showSafeModePopup(sender, [this](auto sender) {
                LevelPage::onPlay(sender);
            });
        }
        else
            LevelPage::onPlay(sender);
    }
};

class $modify(SMEditLevelLayer, EditLevelLayer) {
    struct Fields {
        bool m_shownPopup = false;
    };

    void onPlay(CCObject* sender) {
        if (!m_fields->m_shownPopup) {
            m_fields->m_shownPopup = true;
            showSafeModePopup(sender, [this](auto sender) {
                EditLevelLayer::onPlay(sender);
            });
        }
        else
            EditLevelLayer::onPlay(sender);
    }
};

class $modify(SMPlayerObject, PlayerObject) {
    void incrementJumps() {
        if (!isSafeModeEnabled())
            PlayerObject::incrementJumps();
    }
};

class $modify(SMPlayLayer, PlayLayer) {
    struct Fields {
        int m_oldAttempts = 0;
        int m_oldJumps = 0;
    };

    void resetLevel() {
        if (isSafeModeEnabled())
            m_level->m_attempts = m_level->m_attempts - 1;

        PlayLayer::resetLevel();
    }

    void destroyPlayer(PlayerObject *player, GameObject *object) {
        bool original = m_isTestMode;

        if (isSafeModeEnabled()) {
            m_isTestMode = true;

            auto statsMan = GameStatsManager::get();
            statsMan->setStat("1", m_fields->m_oldJumps);
            statsMan->setStat("2", m_fields->m_oldAttempts);
        }

        PlayLayer::destroyPlayer(player, object);
        m_isTestMode = original;
    }

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (isSafeModeEnabled()) {
            auto statsMan = GameStatsManager::get();
            m_fields->m_oldAttempts = statsMan->getStatFromKey(StatKey::Attempts);
            m_fields->m_oldJumps = statsMan->getStatFromKey(StatKey::Jumps);
        }

        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        return true;
    }

    void levelComplete() {
        bool original = m_isTestMode;

        if (isSafeModeEnabled())
            m_isTestMode = true;

        PlayLayer::levelComplete();

        m_isTestMode = original;
    }
};