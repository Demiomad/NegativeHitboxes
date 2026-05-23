#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "../Utils.hpp"

using namespace geode::prelude;

class $modify(NHPlayLayer, PlayLayer) {
    void setupHasCompleted() {
        log::info("Doing other stuff so negative hitboxes work properly");

        log::debug("Disabling \"Fix Negative Scale\" for \"{}\"", m_level->m_levelName);

        if (m_levelSettings) {
            log::debug("Current value: {}", m_levelSettings->m_fixNegativeScale);

            m_levelSettings->m_fixNegativeScale = false;

            log::debug("Expected value: false");
            log::debug("Actual value: {}", m_levelSettings->m_fixNegativeScale);
        }
        else {
            log::debug("m_levelSettings was null");
        }

        PlayLayer::setupHasCompleted();
    }
};