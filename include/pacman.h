#pragma once
#include "gameObject.h"
#include "collider.h"
#include "iconFactory.h"

class Pacman : public GameObject, public ICollider {
public:
    Pacman();
    void update() override;
    bool setDirection(Direction);
    Direction getDirection() const;
    bool intersect(ICollider *) override;
    void onCollision(ICollider *) override;
private:
    Direction _dir;
    static const Icon* pacman_icon; 
    int _speed_tick = 0;
    int speed = 2;
};