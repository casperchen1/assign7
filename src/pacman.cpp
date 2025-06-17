#include "pacman.h"
#include "map.h"

const Icon* Pacman::pacman_icon = IconFactory::create(1, 1, YELLOW, " ");

Pacman::Pacman() : GameObject(*pacman_icon, {1, 1}), _dir(RIGHT) {
    initCollider({1, 1}, {1, 1});
}

void Pacman::update() {
    _speed_tick++;
    if(_speed_tick <= speed) return;
    else _speed_tick = 0;
    int x = _pos.e1;
    int y = _pos.e2;
    if(_dir == RIGHT && MAP[y][x+1] != 1) _pos.e1++;
    else if(_dir == UP && MAP[y-1][x] != 1) _pos.e2--;
    else if(_dir == LEFT && MAP[y][x-1] != 1) _pos.e1--;
    else if(_dir == DOWN && MAP[y+1][x] != 1) _pos.e2++;

    int Nx = _pos.e1;
    int Ny = _pos.e2;
    if(Nx > MAP_WIDTH-1) _pos.e1 = 0;
    if(Nx < 0) _pos.e1 = MAP_WIDTH-1;
    //if(Ny > MAP_HEIGHT-1) _pos.e2 = 0;
    //if(Ny < 0) _pos.e2 = MAP_HEIGHT-1;
}

bool Pacman::setDirection(Direction dir) {
    int x = _pos.e1;
    int y = _pos.e2;
    if((dir == RIGHT && MAP[y][x+1] != 1) 
    || (dir == UP && MAP[y-1][x] != 1) 
    || (dir == LEFT && MAP[y][x-1] != 1) 
    || (dir == DOWN && MAP[y+1][x] != 1)) {
        _dir = dir;
        return true;
    }
    return false;
}

Direction Pacman::getDirection() const {
    return _dir;
}

bool Pacman::intersect(ICollider*) {
    return true;
}

void Pacman::onCollision(ICollider*) {
    
}
