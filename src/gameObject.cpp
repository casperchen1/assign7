// TODO implementation
#include "gameObject.h"

GameObject::GameObject(Icon icon, Position pos) : _icon(icon), _pos(pos) {}

Position GameObject::getPosition() const {
    return _pos;
}

Icon GameObject::getIcon() const {
    return _icon;
}

void GameObject::update() {
    
}

void GameObject::updateObject(Icon icon, Position pos) {
    _icon = icon;
    _pos = pos;
}