#pragma once
#include "gameObject.h"
#include "pacman.h"

class IGhost : public GameObject {
public:
    IGhost(Icon, Position, const Pacman* const player);
    void update() override;
    void scared(bool state);
    Icon getGhostIcon();
    bool isScared();
    virtual void decideDirection() = 0;
protected:
    int _starting_tick = -1;
    static Icon* ghost_icon_scared;
    void move();
    Direction _dir;
    const Pacman* const _player;
    int _speed_tick = 0;
    bool _scared = false;
};

class RedGhost : public IGhost {
public:
    RedGhost(const Pacman* const player);
    void decideDirection() override;
    const static Icon* ghost_icon; 
};

class PinkGhost : public IGhost {
public:
    PinkGhost(const Pacman* const player);
    void decideDirection() override;
    const static Icon* ghost_icon; 
};

class CyanGhost : public IGhost {
public:
    CyanGhost(const Pacman* const player, const RedGhost* const rg);
    void decideDirection() override;
    const static Icon* ghost_icon; 
private:
    const RedGhost* const _rg;
};