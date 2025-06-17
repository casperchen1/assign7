#include <iostream>
#include <string>
#include "view.h"
#include "controller.h"
#include "AnsiPrint.h"
#include "pacman.h"
#include "ghost.h"
#include "gameObjectFactory.h"
#include "iconFactory.h"

/**
 * Print my id
 * */
void
PrintMyID(std::string studId) {

    std::string str = "ID: " + studId;
    std::cout << AnsiPrint(str.c_str(),YELLOW,RED, true, true) << std::endl << std::endl;
}

int main(){
    // TODO 
    // Integrate the Simple MVC Pattern to run the program.

    View view;
    Controller controller(view);
    Pacman* player = new Pacman();
    RedGhost* rg = new RedGhost(player);
    PinkGhost* pg = new PinkGhost(player);
    CyanGhost* cg = new CyanGhost(player, rg);
    controller.initMap();
    controller.assignPlayer(player);
    controller.addObj(rg);
    controller.addObj(pg);
    controller.addObj(cg);

    controller.run();

    State game_state = controller.getGameState();
    if(game_state == WIN) std::cout << "you won!\n";
    else if(game_state == LOSE) std::cout << "you lost!\n";

    // TODO
    // change to your student ID.
    PrintMyID("113703033");
}
