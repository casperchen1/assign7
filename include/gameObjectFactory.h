// TODO 
// Finish the declaration of GameObjectFactory that
// 1. offers some methods to create a GameObject.
// 2. encapsulates the constructor from the caller. 
#pragma once

#include "gameObject.h"

class GameObjectFactory {
public:
    static GameObjectFactory& getInstance();
    static GameObject* create(Icon icon, Position pos);
private:
    GameObjectFactory();
    static GameObjectFactory* _instance;
};
