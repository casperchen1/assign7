#ifndef COLLIDER_H
#define COLLIDER_H

#include "unit.h"

class ICollider {

public:
  virtual ~ICollider() = default;
  virtual bool intersect(ICollider *) = 0;
  virtual void onCollision(ICollider *) = 0;
  void initCollider(Position pos, Vec2 size);
  Position getColliderPos();
protected:
  Position _collider_pos;
  Vec2 _size;
};
#endif
