#include <Geode/Geode.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

static constexpr int COLLISION_BLOCK_ID = 1816;

static bool isEnabled() {
    return Mod::get()->getSettingValue<bool>("enabled");
}

static bool isCollisionBlock(GameObject* obj) {
    return obj && obj->m_objectID == COLLISION_BLOCK_ID;
}

static void forceVisible(CCNode* node) {
    node->setVisible(true);
    node->setOpacity(255);
}

class $modify(VisibleCollisionEffectObj, EffectGameObject) {
    void triggerObject(GJBaseGameLayer* layer, int p1, gd::vector<int> const* p2) {
        EffectGameObject::triggerObject(layer, p1, p2);
        if (isEnabled() && isCollisionBlock(this)) {
            forceVisible(this);
        }
    }
};

class $modify(VisibleCollisionGameObj, GameObject) {
    void setVisible(bool visible) {
        if (isEnabled() && isCollisionBlock(this) && !visible) {
            GameObject::setVisible(true);
            return;
        }
        GameObject::setVisible(visible);
    }

    void setOpacity(unsigned char opacity) {
        if (isEnabled() && isCollisionBlock(this) && opacity < 255) {
            GameObject::setOpacity(255);
            return;
        }
        GameObject::setOpacity(opacity);
    }
};

class $modify(VisibleCollisionPlayLayer, PlayLayer) {
    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);
        if (!isEnabled()) return;

        auto objects = m_objects;
        if (!objects) return;

        for (int i = 0; i < objects->count(); i++) {
            auto obj = static_cast<GameObject*>(objects->objectAtIndex(i));
            if (isCollisionBlock(obj)) {
                forceVisible(obj);
            }
        }
    }
};

class $modify(VisibleCollisionEditorLayer, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelEditorLayer::init(level, p1)) return false;
        return true;
    }

    void postUpdate(float dt) {
        LevelEditorLayer::postUpdate(dt);
        if (!isEnabled()) return;

        auto objects = m_objects;
        if (!objects) return;

        for (int i = 0; i < objects->count(); i++) {
            auto obj = static_cast<GameObject*>(objects->objectAtIndex(i));
            if (isCollisionBlock(obj)) {
                forceVisible(obj);
            }
        }
    }
};
