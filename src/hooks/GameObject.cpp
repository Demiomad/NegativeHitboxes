#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>

#include "../Utils.hpp"

using namespace geode::prelude;

class $modify(NHGameObject, GameObject) {
    void activateObject() override {
        GameObject::activateObject();

        if (m_editorEnabled || !NegativeHitboxes::Utils::isModEnabled())
            return;

        bool isPositive = m_scaleX > 0 && m_scaleY > 0;

        if (isPositive && m_objectType != GameObjectType::Decoration) {
            if (NegativeHitboxes::Utils::shouldIgnoreObject(this))
                return;

            log::debug("Scale: {}, {}", m_scaleX, m_scaleY);
            log::debug("Negating scale of object with ID {}", m_objectID);

            m_scaleX = -m_scaleX;
            m_scaleY = -m_scaleY;
        }
    }
};