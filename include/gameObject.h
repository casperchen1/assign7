#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include "unit.h"
#include "icon.h"
#include <iostream>
class GameObject{

public:
    GameObject(Icon icon, Position pos);
    Position getPosition() const;
    Icon getIcon() const;
    void updateObject(Icon, Position);
    virtual void update();
    virtual ~GameObject() = default;
protected:
    
    Position _pos;
    Icon _icon;
};

#endif
