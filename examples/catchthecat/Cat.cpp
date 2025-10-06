#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) {
  auto rand = Random::Range(0, 5);
  auto pos = world->getCat();
  std::vector<Point2D> path = generatePath(world);
  Point2D nextPoint;
  return path.back();
}
