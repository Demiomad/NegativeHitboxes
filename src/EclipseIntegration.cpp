#include <Geode/Geode.hpp>
#include <eclipse.eclipse-menu/include/eclipse.hpp>
#include <eclipse.eclipse-menu/include/labels.hpp>
#include <eclipse.eclipse-menu/include/modules.hpp>

#include "Utils.hpp"

using namespace geode::prelude;

inline void updateSafeModeLabel() {
    eclipse::label::setVariable("nhSafeMode", NegativeHitboxes::Utils::isSafeModeEnabled());
}

$on_mod(Loaded) {
    auto loader = Loader::get();

    if (!loader->isModLoaded("eclipse.eclipse-menu"))
        return;

    loader->queueInMainThread([]() {
        eclipse::modules::registerCheat("Negative Hitboxes", []() {
            return NegativeHitboxes::Utils::isSafeModeEnabled();
        });

        auto tab = eclipse::MenuTab::find("Negative Hitboxes");

        tab.addToggle("nh-safe-mode", "Safe Mode", [](bool value) {
            auto mod = Mod::get();
            mod->setSettingValue("safe-mode", value);
        }).setDescription("Prevents level progression.");

        auto mod = Mod::get();
        eclipse::config::set("nh-safe-mode", mod->getSettingValue<bool>("safe-mode"));

        geode::listenForSettingChanges<bool>("safe-mode", [](bool value) {
            eclipse::config::set("nh-safe-mode", value);
            updateSafeModeLabel();
        });

        updateSafeModeLabel();
    });
}