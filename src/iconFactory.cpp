// TODO implementation
#include "iconFactory.h"

IconFactory* IconFactory::_instance = nullptr;

IconFactory::IconFactory() {}

IconFactory* IconFactory::getInstance() {
    if(_instance == nullptr) {
        _instance = new IconFactory();
    }
    return _instance;
}

Icon* IconFactory::create(int width, int height, Color color, std::string ascii) {
    return new Icon(height, std::vector<Cell>(width, {color, ascii}));
}