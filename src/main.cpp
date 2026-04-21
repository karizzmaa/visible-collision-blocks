#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

static constexpr int COLLISION_BLOCK_ID = 1816;
static constexpr GLubyte COLLISION_VISIBLE_OPACITY = 80;

static bool isModeEnabled() {
    return Mod::get()->getSettingValue<bool>("enabled");
}

static bool isCollisionBlock(GameObject* obj) {
    return obj && obj->m_objectID == COLLISION_BLOCK_ID;
}

class $modify(VisibleGameObject, GameObject) {
    void setOpacity(unsigned char opacity) {
        if (isModeEnabled() && isCollisionBlock(this)) {
            if (opacity == 0) {
                opacity = COLLISION_VISIBLE_OPACITY;
            }
            GameObject::setOpacity(opacity);
            return;
        }
        GameObject::setOpacity(opacity);
    }

    void customSetup() {
        GameObject::customSetup();
        if (isModeEnabled() && isCollisionBlock(this)) {
            this->setCascadeOpacityEnabled(true);
            if (this->getOpacity() == 0) {
                GameObject::setOpacity(COLLISION_VISIBLE_OPACITY);
            }
        }
    }
};

class $modify(VisibleBaseGameLayer, GJBaseGameLayer) {
    void toggleGroupVisibility(int groupID, bool visible, gd::unordered_map<int, int>& p2) {
        GJBaseGameLayer::toggleGroupVisibility(groupID, visible, p2);

        if (!isModeEnabled()) return;

        auto& objects = this->m_objects;
        for (int i = 0; i < objects->count(); i++) {
            auto obj = static_cast<GameObject*>(objects->objectAtIndex(i));
            if (!isCollisionBlock(obj)) continue;
            if (obj->getOpacity() == 0) {
                obj->GameObject::setOpacity(COLLISION_VISIBLE_OPACITY);
            }
        }
    }

    void processCommands(float dt) {
        GJBaseGameLayer::processCommands(dt);

        if (!isModeEnabled()) return;

        auto& objects = this->m_objects;
        for (int i = 0; i < objects->count(); i++) {
            auto obj = static_cast<GameObject*>(objects->objectAtIndex(i));
            if (!isCollisionBlock(obj)) continue;
            if (obj->getOpacity() == 0) {
                obj->GameObject::setOpacity(COLLISION_VISIBLE_OPACITY);
            }
        }
    }
};
