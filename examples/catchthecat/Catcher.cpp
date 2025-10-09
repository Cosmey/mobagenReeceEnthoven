#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {
  std::vector<Point2D> path = generatePath(world);
  std::vector<Point2D> excludedPath;
  if (path.size() > 1) {
    std::vector<Point2D> exclusions;
    exclusions.push_back(path[0]);
    excludedPath = generatePath(world, exclusions);
    if (excludedPath[0] == Point2D::INFINITE) return path[0];
    else return excludedPath[0];
  }
  else if (path[0] == Point2D(INT32_MAX, INT32_MAX)) {
    std::vector<Point2D> neighbors = getVisitableNeighbors(world,world->getCat());
    auto rand = Random::Range(0, neighbors.size()-1);
    return neighbors[rand];
  }
  else if (path[0] != world->getCat()) return path[0];

  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
    auto cat = world->getCat();
    if (cat.x != p.x && cat.y != p.y && !world->getContent(p)) return p;
  }
}

Point2D Catcher::randPointNearCat(World* world) {
  Point2D catPoint = world->getCat();
  std::vector<Point2D> neighbors = world->neighbors(catPoint);
  auto rand = Random::Range(0, neighbors.size()-1);
  int indexer = rand;
  while (indexer-rand < neighbors.size()) {
    if (!world->getContent(neighbors[indexer]) && world->isValidPosition(neighbors[indexer])) return neighbors[indexer];
    indexer++;
    if (indexer > neighbors.size()-1) indexer = 0;
  }

  return Point2D(0,0);
}
