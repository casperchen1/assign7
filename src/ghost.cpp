#include "iconFactory.h"
#include "ghost.h"
#include "map.h"
#include <queue>
#include <set>
#include <random>

Icon* IGhost::ghost_icon_scared = IconFactory::create(1, 1, BLUE, "><");
const Icon* RedGhost::ghost_icon = IconFactory::create(1, 1, RED, "''");
const Icon* PinkGhost::ghost_icon = IconFactory::create(1, 1, PINK, "''");
const Icon* CyanGhost::ghost_icon = IconFactory::create(1, 1, CYAN, "''");


IGhost::IGhost(Icon icon, Position pos, const Pacman* const player) : GameObject(icon, pos), _player(player) {}

void IGhost::update() {
    _speed_tick++;
    if(_speed_tick > 1000) _speed_tick = 0;
    if(_speed_tick%4 == 0) {
        decideDirection();
        move();
    }
}

void IGhost::scared(bool state) {
    _scared = state;
    if(_scared) {
        updateObject(*ghost_icon_scared, _pos);
    }
}

bool IGhost::isScared() {
    return _scared;
}

void IGhost::move() {
    if(_dir == UP) _pos.e2--;
    else if(_dir == RIGHT) _pos.e1++;
    else if(_dir == DOWN) _pos.e2++;
    else if(_dir == LEFT) _pos.e1--;
}

RedGhost::RedGhost(const Pacman* const player) : IGhost(*RedGhost::ghost_icon, {MAP_WIDTH/2, MAP_HEIGHT/2-2}, player) {}
PinkGhost::PinkGhost(const Pacman* const player) : IGhost(*PinkGhost::ghost_icon, {MAP_WIDTH/2+2, MAP_HEIGHT/2}, player) {}
CyanGhost::CyanGhost(const Pacman* const player, const RedGhost* const rg) : IGhost(*CyanGhost::ghost_icon, {MAP_WIDTH/2-2, MAP_HEIGHT/2}, player), _rg(rg) {}

struct Node {
    int x;
    int y;
    bool operator<(const Node& other) const {
        if(x == other.x) {
            return y < other.y;
        } else {
            return x < other.x;
        }
    }
};

void RedGhost::decideDirection() {
    /*
    Position player_pos = _player->getPosition();
    int px = player_pos.e1;
    int py = player_pos.e2;
    short dst = 0; //0000
    if(px-_pos.e1 > 0) dst+=(1<<1);
    else if(px-_pos.e1 < 0) dst+=(1<<3);
    if(py-_pos.e2 < 0) dst+=1;
    else if(py-_pos.e2 > 0) dst+=(1<<2);
    short options = 0; //0000
    if(MAP[_pos.e2-1][_pos.e1] == 0) options+=1;
    if(MAP[_pos.e2][_pos.e1+1] == 0) options+=(1<<1);
    if(MAP[_pos.e2+1][_pos.e1] == 0) options+=(1<<2);
    if(MAP[_pos.e2][_pos.e1-1] == 0) options+=(1<<3);
    dst = dst & options;
    if(dst == 0) {
        for(int i=0; i<4; i++) {
            if((options >> i) & 1 == 1) {
                _dir = (Direction)i;
                return;
            }
        }
    } else {
        for(int i=0; i<4; i++) {
            if((dst >> i) & 1 == 1) {
                _dir = (Direction)i;
                return;
            }
        }
    }
    */
    
    Position player_pos = _player->getPosition();
    std::queue<Node> q;
    std::set<Node> visited;
    std::vector<std::vector<Node>> parent(MAP_HEIGHT, std::vector<Node>(MAP_WIDTH, {-1, -1}));
    q.push({_pos.e1, _pos.e2});
    visited.insert({_pos.e1, _pos.e2});
    while(!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if(x == player_pos.e1 && y == player_pos.e2) {
            Node curr = {x, y};
            while(abs(curr.x-_pos.e1)+abs(curr.y-_pos.e2) > 1) {
                curr = parent[curr.y][curr.x];
            }
            if(curr.x > _pos.e1) _dir = RIGHT;
            else if(curr.x < _pos.e1) _dir = LEFT;
            else if(curr.y > _pos.e2) _dir = DOWN;
            else if(curr.y < _pos.e2) _dir = UP;
            return;
        }

        if(!visited.count({x+1, y}) && x+1 < MAP_WIDTH &&  MAP[y][x+1] == 0) {
            q.push({x+1, y});
            parent[y][x+1] = {x, y};
            visited.insert({x+1, y});
        }
        if(!visited.count({x, y+1}) && y+1 < MAP_HEIGHT && MAP[y+1][x] == 0) {
            q.push({x, y+1});
            parent[y+1][x] = {x, y};
            visited.insert({x, y+1});
        }
        if(!visited.count({x-1, y}) && x-1 >= 0 && MAP[y][x-1] == 0) {
            q.push({x-1, y});
            parent[y][x-1] = {x, y};
            visited.insert({x-1, y});
        }
        if(!visited.count({x, y-1}) && y-1 >= 0 && MAP[y-1][x] == 0) {
            q.push({x, y-1});
            parent[y-1][x] = {x, y};
            visited.insert({x, y-1});
        }
    }
    
}

void PinkGhost::decideDirection() {
    Position player_pos = _player->getPosition();
    const Pacman* const player = static_cast<const Pacman*>(_player);
    Direction facing = player->getDirection();
    if(facing == RIGHT) {
        player_pos.e1 = std::min(MAP_WIDTH-2, player_pos.e1+4);
    } else if(facing == LEFT) {
        player_pos.e1 = std::max(1, player_pos.e1-4);
    } else if(facing == UP) {
        player_pos.e2 = std::max(1, player_pos.e2-4);
    } else if(facing == DOWN) {
        player_pos.e2 = std::min(MAP_HEIGHT-2, player_pos.e2+4);
    }
    if(MAP[player_pos.e2][player_pos.e1] == 1) {
        player_pos = _player->getPosition();
    }
    std::queue<Node> q;
    std::set<Node> visited;
    std::vector<std::vector<Node>> parent(MAP_HEIGHT, std::vector<Node>(MAP_WIDTH, {-1, -1}));
    q.push({_pos.e1, _pos.e2});
    visited.insert({_pos.e1, _pos.e2});
    while(!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if(x == player_pos.e1 && y == player_pos.e2) {
            Node curr = {x, y};
            while(abs(curr.x-_pos.e1)+abs(curr.y-_pos.e2) > 1) {
                curr = parent[curr.y][curr.x];
            }
            if(curr.x > _pos.e1) _dir = RIGHT;
            else if(curr.x < _pos.e1) _dir = LEFT;
            else if(curr.y > _pos.e2) _dir = DOWN;
            else if(curr.y < _pos.e2) _dir = UP;
            return;
        }
        if(!visited.count({x, y+1}) && y+1 < MAP_HEIGHT && MAP[y+1][x] == 0) {
            q.push({x, y+1});
            parent[y+1][x] = {x, y};
            visited.insert({x, y+1});
        }
        if(!visited.count({x-1, y}) && x-1 >= 0 && MAP[y][x-1] == 0) {
            q.push({x-1, y});
            parent[y][x-1] = {x, y};
            visited.insert({x-1, y});
        }
        if(!visited.count({x, y-1}) && y-1 >= 0 && MAP[y-1][x] == 0) {
            q.push({x, y-1});
            parent[y-1][x] = {x, y};
            visited.insert({x, y-1});
        }
        if(!visited.count({x+1, y}) && x+1 < MAP_WIDTH &&  MAP[y][x+1] == 0) {
            q.push({x+1, y});
            parent[y][x+1] = {x, y};
            visited.insert({x+1, y});
        }
    }
}

void CyanGhost::decideDirection() {
    Position player_pos_ini = _player->getPosition();
    Position player_pos = player_pos_ini;
    Direction facing = _player->getDirection();
    if(facing == RIGHT) {
        player_pos.e1 = std::min(MAP_WIDTH-2, player_pos.e1+2);
    } else if(facing == LEFT) {
        player_pos.e1 = std::max(1, player_pos.e1-2);
    } else if(facing == UP) {
        player_pos.e2 = std::max(1, player_pos.e2-2);
    } else if(facing == DOWN) {
        player_pos.e2 = std::min(MAP_HEIGHT-2, player_pos.e2+2);
    }
    Position red_pos = _rg->getPosition();
    Position dst = player_pos;
    short dstx = (player_pos.e1+red_pos.e1)/2;
    short dsty = (player_pos.e2+red_pos.e2)/2;
    if(MAP[dsty][dstx] == 0) {
        dst = {dstx, dsty};
    } else {
        dst = {player_pos_ini.e1, player_pos_ini.e2};
    }

    std::queue<Node> q;
    std::set<Node> visited;
    std::vector<std::vector<Node>> parent(MAP_HEIGHT, std::vector<Node>(MAP_WIDTH, {-1, -1}));
    q.push({_pos.e1, _pos.e2});
    visited.insert({_pos.e1, _pos.e2});
    while(!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if(x == dst.e1 && y == dst.e2) {
            Node curr = {x, y};
            while(abs(curr.x-_pos.e1)+abs(curr.y-_pos.e2) > 1) {
                curr = parent[curr.y][curr.x];
            }
            if(curr.x > _pos.e1) _dir = RIGHT;
            else if(curr.x < _pos.e1) _dir = LEFT;
            else if(curr.y > _pos.e2) _dir = DOWN;
            else if(curr.y < _pos.e2) _dir = UP;
            return;
        }

        if(!visited.count({x+1, y}) && x+1 < MAP_WIDTH &&  MAP[y][x+1] == 0) {
            q.push({x+1, y});
            parent[y][x+1] = {x, y};
            visited.insert({x+1, y});
        }
        if(!visited.count({x, y+1}) && y+1 < MAP_HEIGHT && MAP[y+1][x] == 0) {
            q.push({x, y+1});
            parent[y+1][x] = {x, y};
            visited.insert({x, y+1});
        }
        if(!visited.count({x-1, y}) && x-1 >= 0 && MAP[y][x-1] == 0) {
            q.push({x-1, y});
            parent[y][x-1] = {x, y};
            visited.insert({x-1, y});
        }
        if(!visited.count({x, y-1}) && y-1 >= 0 && MAP[y-1][x] == 0) {
            q.push({x, y-1});
            parent[y-1][x] = {x, y};
            visited.insert({x, y-1});
        }
    }
}