#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
using namespace geode::prelude;

class $modify(NHGameObject, GameObject) {
    void activateObject() {
        GameObject::activateObject();

        bool modDisabled = Mod::get()->getSettingValue<bool>("disable-mod");
        if (m_editorEnabled || modDisabled) return;

        bool isPositive = m_scaleX > 0 && m_scaleY > 0;

        if (isPositive && m_objectType != GameObjectType::Decoration) {
            bool ignoreSolids = Mod::get()->getSettingValue<bool>("ignore-solids");
            if (ignoreSolids && m_objectType == GameObjectType::Solid)
                return;

            bool ignoreHazards = Mod::get()->getSettingValue<bool>("ignore-hazards");
            if (ignoreHazards && m_objectType == GameObjectType::Hazard)
                return;

            log::debug("Scale: {}, {}", m_scaleX, m_scaleY);
            log::debug("Negating scale of object with ID {}", m_objectID);

            m_scaleX = -m_scaleX;
            m_scaleY = -m_scaleY;
        }
    }
};