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
            return NegativeHitboxes::Utils::isModEnabled();
        });

        auto tab = eclipse::MenuTab::find("Negative Hitboxes");

        tab.addToggle("nh-enabled", "Enabled", [](bool value) {
            auto mod = Mod::get();
            mod->setSettingValue("enable-mod", value);
        }).setDescription("Inverts the level's hitboxes. It is recommended to re-enter the level, but not required.");

        tab.addToggle("nh-safe-mode", "Safe Mode", [](bool value) {
            auto mod = Mod::get();
            mod->setSettingValue("safe-mode", value);
        }).setDescription("Prevents level progression.");

        tab.addToggle("nh-ignore-solids", "Ignore Solids", [](bool value) {
            auto mod = Mod::get();
            mod->setSettingValue("ignore-solids", value);
        }).setDescription("Ignores solid objects. (blocks, slabs, etc.)");

        tab.addToggle("nh-ignore-hazards", "Ignore Hazards", [](bool value) {
            auto mod = Mod::get();
            mod->setSettingValue("ignore-hazards", value);
        }).setDescription("Ignores hazard objects. (spikes, saws, etc.)");

        tab.addToggle("nh-ignore-pads", "Ignore Pads", [](bool value) {
            auto mod = Mod::get();
            mod->setSettingValue("ignore-pads", value);
        }).setDescription("Ignores pads.");

        tab.addToggle("nh-ignore-rings", "Ignore Orbs", [](bool value) {
            auto mod = Mod::get();
            mod->setSettingValue("ignore-rings", value);
        }).setDescription("Ignores orbs.");

        tab.addToggle("nh-ignore-portals", "Ignore Portals", [](bool value) {
            auto mod = Mod::get();
            mod->setSettingValue("ignore-portals", value);
        }).setDescription("Ignores portals.");

        auto mod = Mod::get();
        eclipse::config::set("nh-safe-mode", mod->getSettingValue<bool>("safe-mode"));
        eclipse::config::set("nh-enabled", NegativeHitboxes::Utils::isModEnabled());
        eclipse::config::set("nh-ignore-solids", mod->getSettingValue<bool>("ignore-solids"));
        eclipse::config::set("nh-ignore-hazards", mod->getSettingValue<bool>("ignore-hazards"));
        eclipse::config::set("nh-ignore-pads", mod->getSettingValue<bool>("ignore-pads"));
        eclipse::config::set("nh-ignore-rings", mod->getSettingValue<bool>("ignore-rings"));
        eclipse::config::set("nh-ignore-portals", mod->getSettingValue<bool>("ignore-portals"));

        geode::listenForSettingChanges<bool>("ignore-solids", [](bool value) {
            eclipse::config::set("nh-ignore-solids", value);
        });

        geode::listenForSettingChanges<bool>("ignore-hazards", [](bool value) {
            eclipse::config::set("nh-ignore-hazards", value);
        });

        geode::listenForSettingChanges<bool>("ignore-pads", [](bool value) {
            eclipse::config::set("nh-ignore-pads", value);
        });

        geode::listenForSettingChanges<bool>("ignore-rings", [](bool value) {
            eclipse::config::set("nh-ignore-rings", value);
        });

        geode::listenForSettingChanges<bool>("ignore-portals", [](bool value) {
            eclipse::config::set("nh-ignore-portals", value);
        });

        geode::listenForSettingChanges<bool>("safe-mode", [](bool value) {
            eclipse::config::set("nh-safe-mode", value);
            updateSafeModeLabel();
        });

        geode::listenForSettingChanges<bool>("enable-mod", [](bool value) {
            eclipse::config::set("nh-enabled", value);
            updateSafeModeLabel();
        });

        updateSafeModeLabel();
    });
}