#include "collider.h"

void ICollider::initCollider(Position pos, Vec2 size) {
    _collider_pos = pos;
    _size = size;
}

Position ICollider::getColliderPos() {
    return _collider_pos;
}