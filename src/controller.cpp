#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <string>

#include "environment.h"
#include "controller.h"
#include "gameObjectFactory.h"
#include "iconFactory.h"
#include "pacman.h"
#include "ghost.h"


void Controller::addObj(GameObject* obj) {
    if((_ghost_index == -1) && dynamic_cast<IGhost*>(obj)) _ghost_index = _objs.size();
    _objs.push_back(obj);
}

Controller::Controller(View& view) : _view(view){
}

void Controller::run() {
    // initial setup
    std::cin.tie(0);
    std::ios::sync_with_stdio(0);
    configure_terminal();

    // init state
    int input = -1;
    clock_t start, end;
    
    // Main loop
    while (true) {
        start = clock();
        // game loop goes here
        input = read_input();

        // ESC to exit program
        if(input==27)break;

        this->handleInput(input);

        _view.resetLatest();
        
        renderMap();
        for(GameObject* obj : _objs) 
        {
            if(obj == nullptr) continue;
            obj->update();

            _view.updateGameObject(obj);
        }
        checkCoins();
        checkGhosts();
        if(_score == 189) {
            _game_state = WIN;
            break;
        }
        if(_game_state == LOSE) break;
        _view.render();


        end = clock();

        // frame rate normalization
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_taken > SPF) continue;
        int frameDelay = int((SPF - time_taken) * 1000); // 0.1 seconds
        if(frameDelay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay)); // frame delay
    }

}

State Controller::getGameState() {
    return _game_state;
}

void Controller::assignPlayer(GameObject* player) {
    _objs.push_back(player);
    _player_index = _objs.size()-1;
}

void Controller::checkCoins() {
    Position player_pos = _objs[_player_index]->getPosition();
    for(int i=0; i<_ghost_index; i++) {
        if(i != _player_index && _objs[i] != nullptr) {
            Position coin_pos = _objs[i]->getPosition();
            if(player_pos.e1 == coin_pos.e1 && player_pos.e2 == coin_pos.e2) {
                delete _objs[i];
                _objs[i] = nullptr;
                _score++;
            }
        }
    }
}

void Controller::checkGhosts() {
    Position player_pos = _objs[_player_index]->getPosition();
    for(int i=_ghost_index; i<_objs.size(); i++) {
        Position ghost_pos = _objs[i]->getPosition();
        if(static_cast<IGhost*>(_objs[i])->isScared()) {
            if(!(player_pos.e1 == ghost_pos.e1 && player_pos.e2 == ghost_pos.e2)) return;
            int index = i-_ghost_index;
            if(index == 0) _objs[i]->updateObject(*RedGhost::ghost_icon, {MAP_WIDTH/2, MAP_HEIGHT/2-1});
            else if(index == 1) _objs[i]->updateObject(*PinkGhost::ghost_icon, {MAP_WIDTH/2, MAP_HEIGHT/2-1});
            else if(index == 2) _objs[i]->updateObject(*CyanGhost::ghost_icon, {MAP_WIDTH/2, MAP_HEIGHT/2-1});
            static_cast<IGhost*>(_objs[i])->scared(false);
        } else {
            if(player_pos.e1 == ghost_pos.e1 && player_pos.e2 == ghost_pos.e2) {
                _game_state = LOSE;
            }
        }
    }
}

const Icon* wall_icon = IconFactory::create(1, 1, BLUE, " ");
const Icon* coin_icon = IconFactory::create(1, 1, NOCHANGE, "。");
const Icon* line_icon = IconFactory::create(1, 1, NOCHANGE, "|");

const Icon* power_icon = IconFactory::create(1, 1, BLUE, "1");
const Icon* power_icon_down = IconFactory::create(1, 1, BLACK, "1");
const Icon* flash_icon = IconFactory::create(1, 1, YELLOW, "2");
const Icon* flash_icon_down = IconFactory::create(1, 1, BLACK, "2");

const Icon* up_icon = IconFactory::create(1, 1, BLACK, "FACING: ↑");
const Icon* right_icon = IconFactory::create(1, 1, BLACK, "FACING: →");
const Icon* left_icon = IconFactory::create(1, 1, BLACK, "FACING: ←");
const Icon* down_icon = IconFactory::create(1, 1, BLACK, "FACING: ↓");
void Controller::initMap() {
    for(int i=0; i<MAP_HEIGHT; i++) {
        for(int j=0; j<MAP_WIDTH; j++) {
            if(MAP[i][j]) {
                GameObject* wall = new GameObject(*wall_icon, {j, i});
                _map_obj.push_back(wall);
            } else if(!(i == MAP_HEIGHT/2 && j == MAP_WIDTH/2)) {
                GameObject* coin = new GameObject(*coin_icon, {j, i});
                _objs.push_back(coin);
            }
        }
    }
    for(int i=0; i<GAME_WINDOW_HEIGHT; i++) {
        GameObject* line = new GameObject(*line_icon, {MAP_WIDTH, i});
        _map_obj.push_back(line);
    }
    
    _arrow_index = _map_obj.size();
    GameObject* arrow = new GameObject(*right_icon, {MAP_WIDTH+1, 8});
    _map_obj.push_back(arrow);
    
    Icon* score_board_icon = IconFactory::create(1, 1, NOCHANGE, "SCORE: " + std::to_string(_score));
    GameObject* score_board = new GameObject(*score_board_icon, {MAP_WIDTH+1, 0});
    _map_obj.push_back(score_board);
    delete score_board_icon;

    GameObject* power = new GameObject(*power_icon, {MAP_WIDTH+1, 4});
    _abilities.push_back(power);
    GameObject* flash = new GameObject(*flash_icon, {MAP_WIDTH+3, 4});
    _abilities.push_back(flash);

    for(int i=0; i<_abilities.size(); i++) {
        _abilities_used.push_back(false);
    }
}

const Icon* blink_icon = IconFactory::create(1, 1, WHITE, " ");
void Controller::renderMap() {
    Icon* score_board_icon = IconFactory::create(1, 1, NOCHANGE, "SCORE: " + std::to_string(_score));
    GameObject* score_board = _map_obj.back();
    _map_obj.back()->updateObject(*score_board_icon, score_board->getPosition());
    delete score_board_icon;

    Direction facing = static_cast<Pacman*>(_objs[_player_index])->getDirection();
    if(facing == RIGHT) _map_obj[_arrow_index]->updateObject(*right_icon, {MAP_WIDTH+1, 8});
    else if(facing == LEFT) _map_obj[_arrow_index]->updateObject(*left_icon, {MAP_WIDTH+1, 8});
    else if(facing == UP) _map_obj[_arrow_index]->updateObject(*up_icon, {MAP_WIDTH+1, 8});
    else if(facing == DOWN) _map_obj[_arrow_index]->updateObject(*down_icon, {MAP_WIDTH+1, 8});

    for(const auto& wall : _map_obj) {
        _view.updateGameObject(wall);
    }
    _time_tick++;
    if(_score % 50 == 0) {
        for(int i=0; i<_abilities.size(); i++) {
            _abilities_used[i] = false;
        }
    }
    if(_time_tick > 3) {
        for(int i=0; i<_abilities.size(); i++) {
            if(_abilities_used[i] == false) {
                if(_abilities[i]->getIcon()[0][0].color != WHITE) {
                    _abilities[i]->updateObject(*blink_icon, _abilities[i]->getPosition());
                } else {
                    if(i == 0) {
                        _abilities[i]->updateObject(*power_icon, _abilities[i]->getPosition());
                    } else if(i == 1) {
                        _abilities[i]->updateObject(*flash_icon, _abilities[i]->getPosition());
                    }
                }
            }
        }
        _time_tick = 0;
    }
    for(const auto& abi : _abilities) {
        _view.updateGameObject(abi);
    }
}

void Controller::handleInput(int keyInput){
    // TODO 
    // handle key events.
    if(_input_buffer != -1) {
        static_cast<Pacman*>(_objs[_player_index])->setDirection((Direction)_input_buffer);
        _input_buffer = -1;
        
    }
    if(keyInput == 119) {
        bool success = static_cast<Pacman*>(_objs[_player_index])->setDirection(UP);
        if(!success) {
            _input_buffer = (int)UP;
        }
    }
    if(keyInput == 100) {
        bool success = static_cast<Pacman*>(_objs[_player_index])->setDirection(RIGHT);
        if(!success) {
            _input_buffer = (int)RIGHT;
        }
    }
    if(keyInput == 115) {
        bool success = static_cast<Pacman*>(_objs[_player_index])->setDirection(DOWN);
        if(!success) {
            _input_buffer = (int)DOWN;
        }
    }
    if(keyInput == 97) {
        bool success = static_cast<Pacman*>(_objs[_player_index])->setDirection(LEFT);
        if(!success) {
            _input_buffer = (int)LEFT;
        }
    }
    if(keyInput == 49 && _abilities_used[0] == false) {
        _abilities_used[0] = true;
        _abilities[0]->updateObject(*power_icon_down, _abilities[0]->getPosition());  
        for(int i=_ghost_index; i<_objs.size(); i++) {
            static_cast<IGhost*>(_objs[i])->scared(true);
        }
    }
    if(keyInput == 50 && _abilities_used[1] == false) {
        Direction facing = static_cast<Pacman*>(_objs[_player_index])->getDirection();
        Position player_pos = _objs[_player_index]->getPosition();
        /*
        if(facing == RIGHT) dst.e1 = std::min(MAP_WIDTH-2, player_pos.e1+4);
        else if(facing == LEFT) dst.e1 = std::max(1, player_pos.e1-4);
        else if(facing == UP) dst.e2 = std::max(1, player_pos.e2-4);
        else if(facing == DOWN) dst.e2 = std::min(MAP_HEIGHT-2, player_pos.e2+4);
        */
        int steps = 4;
        bool encounter_wall = false;
        bool found = false;
        while(steps-- && (player_pos.e1 > 0 && player_pos.e1 < MAP_WIDTH-1) && (player_pos.e2 > 0 && player_pos.e2 < MAP_HEIGHT-1)) {
            if(facing == RIGHT) player_pos.e1++;
            else if(facing == LEFT) player_pos.e1--;
            else if(facing == UP) player_pos.e2--;
            else if(facing == DOWN) player_pos.e2++;
            if(MAP[player_pos.e2][player_pos.e1] == 1) { 
                encounter_wall = true;
                continue;
            }
            if(encounter_wall && MAP[player_pos.e2][player_pos.e1] == 0) {
                found = true;
                break;
            }
        }

        if(encounter_wall && !found) return;

        _objs[_player_index]->updateObject(_objs[_player_index]->getIcon(), player_pos); 
        _abilities_used[1] = true;
        _abilities[1]->updateObject(*flash_icon_down, _abilities[1]->getPosition());  
    } 
}

void Controller::update(){

}
void reset_terminal() {
    printf("\e[m"); // reset color changes
    printf("\e[?25h"); // show cursor
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// terminal initial configuration setup
void configure_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
	new_termios = old_termios; // save it to be able to reset on exit
    
    new_termios.c_lflag &= ~(ICANON | ECHO); // turn off echo + non-canonical mode
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    printf("\e[?25l"); // hide cursor
    std::atexit(reset_terminal);
}


int read_input() {
    fflush(stdout);
   	char buf[4096]; // maximum input buffer
	int n = read(STDIN_FILENO, buf, sizeof(buf));
    return n > 0 ? buf[n - 1] : -1;
}

