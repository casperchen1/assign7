// TODO implementation
#include "gameObjectFactory.h"

GameObjectFactory* GameObjectFactory::_instance = nullptr;

GameObjectFactory::GameObjectFactory() {}

GameObjectFactory& GameObjectFactory::getInstance() {
    if(_instance == nullptr) {
        _instance = new GameObjectFactory();
    }
    return *_instance;
}

GameObject* GameObjectFactory::create(Icon icon, Position pos) {
    return new GameObject(icon, pos);
}