#include "Entity.h"
#include "Components/Transform.h"
#include "StaticColliderComponent.h"

StaticColliderComponent::StaticColliderComponent(Entity *parent, glm::vec2 size, bool isPassable, TileState* tileState)
: Component(parent), size(size), isPassable(isPassable), tileState(tileState) {
}

const glm::vec2 &StaticColliderComponent::getSize() const {
    return size;
}

bool StaticColliderComponent::getIsPassable() const {
    return isPassable;
}

glm::vec2 StaticColliderComponent::getCenter() {
    glm::vec3 center = parent->transform->getLocalPosition();
    return {center.x, center.z};
}

TileState *StaticColliderComponent::getTileState() {
    return tileState;
}

