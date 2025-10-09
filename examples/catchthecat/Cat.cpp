#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) {
  std::vector<Point2D> path = generatePath(world);
  if (path[0] == Point2D::INFINITE) {
    std::vector<Point2D> neighbors = getVisitableNeighbors(world,world->getCat());
    auto rand = Random::Range(0, neighbors.size()-1);
    return neighbors[rand];
  }
  else  return path.back();
}
