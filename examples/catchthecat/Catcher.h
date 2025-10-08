#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"
#include "math/Point2D.h"

class Catcher : public Agent {
public:
  explicit Catcher() : Agent(){};
  Point2D Move(World*) override;
  Point2D createTrap(World* world,Point2D edgePoint);
  Point2D randPointNearCat(World* world);
};

#endif  // CATCHER_H
