#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

class $modify(NHPlayLayer, PlayLayer) {
    void setupHasCompleted() {
        log::info("Doing other stuff so negative hitboxes work properly");

        log::debug("Disabling \"Fix Negative Scale\" for \"{}\"", m_level->m_levelName);

        log::debug("Current value: {}", m_levelSettings->m_fixNegativeScale);
        m_levelSettings->m_fixNegativeScale = false;

        log::debug("Expected value: false");
        log::debug("Actual value: {}", m_levelSettings->m_fixNegativeScale);

        PlayLayer::setupHasCompleted();
    }
};