// TODO 
// Finish the declaration of IconFactory that
// 1. offers some methods to create an Icon.
// 2. encapsulates the constructor from the caller. 
#pragma once
#include "icon.h"

class IconFactory {
public:
    static IconFactory* getInstance();
    static Icon* create(int width, int height, Color color, std::string ascii);
private:
    IconFactory();
    static IconFactory* _instance;
};

