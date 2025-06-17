#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <termios.h>
#include <vector>
#include "view.h"
#include "map.h"
#include "gameObject.h"

class Controller{

public:
    void addObj(GameObject*);
    Controller(View&);
    void run();
    void initMap();
    void assignPlayer(GameObject*);
    State getGameState();
private:
    void checkCoins();
    void checkGhosts();
    void renderMap();
    void handleInput(int);
    void update();

    // Model
    std::vector<GameObject*> _objs;
    std::vector<GameObject*> _map_obj;
    std::vector<GameObject*> _abilities;
    std::vector<bool> _abilities_used;

    // View
    View& _view; 

    int _player_index;
    int _ghost_index = -1;
    int _arrow_index = -1;
    int _input_buffer = -1;
    int _frames_buffer = 0;
    short _time_tick = 0;

    int _score = 0;
    State _game_state = PLAYING;
};

static struct termios old_termios, new_termios;
void reset_terminal();
void configure_terminal();
int read_input();

#endif
