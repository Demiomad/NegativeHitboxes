#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>

#include "../Utils.hpp"

using namespace geode::prelude;

class $modify(NHGameObject, GameObject) {
    void activateObject() override {
        GameObject::activateObject();

        bool modDisabled = Mod::get()->getSettingValue<bool>("disable-mod");
        if (m_editorEnabled || modDisabled) return;

        bool isPositive = m_scaleX > 0 && m_scaleY > 0;

        if (isPositive && m_objectType != GameObjectType::Decoration) {
            if (shouldIgnoreObject(this))
                return;

            log::debug("Scale: {}, {}", m_scaleX, m_scaleY);
            log::debug("Negating scale of object with ID {}", m_objectID);

            m_scaleX = -m_scaleX;
            m_scaleY = -m_scaleY;
        }
    }
};