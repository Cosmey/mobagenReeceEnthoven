#include "Catcher.h"
#include "World.h"
#include "../../cmake-build-debug/_deps/sdl2-src/src/haptic/windows/SDL_dinputhaptic_c.h"

Point2D Catcher::Move(World* world) {
  std::vector<Point2D> path = generatePath(world);
  if (path.size() > 1) {
    for (int i = 0; i < path.size(); i++) {
      if (path[0] != world->getCat()) {
        return createTrap(world, path[i]);
      }
    }
  }
  else if (path[0] == Point2D(INT32_MAX, INT32_MAX)) return randPointNearCat(world);
  else if (path[0] != world->getCat()) return path[0];

  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
    auto cat = world->getCat();
    if (cat.x != p.x && cat.y != p.y && !world->getContent(p)) return p;
  }
}

//places blocks all around a single open exit
Point2D Catcher::createTrap(World* world, Point2D edgePoint) {
  // true if on left or right edge
  if (!world->isValidPosition(world->W(edgePoint)) || !world->isValidPosition(world->E(edgePoint))) {
    Point2D northPoint = edgePoint;
    Point2D southPoint = edgePoint;
    while (world->isValidPosition(northPoint) || world->isValidPosition(southPoint)) {
      if (world->isValidPosition(world->NW(northPoint)))
        northPoint = world->NW(northPoint);
      else if (world->isValidPosition(world->NE(northPoint)))
        northPoint = world->NE(northPoint);

      if (world->isValidPosition(world->SW(southPoint)))
        southPoint = world->SW(southPoint);
      else if (world->isValidPosition(world->SE(southPoint)))
        southPoint = world->SE(southPoint);

      if (world->isValidPosition(northPoint)
          && (world->isValidPosition(world->NW(northPoint)) || world->isValidPosition(world->NE(northPoint))
              || world->isValidPosition(world->W(northPoint)) || world->isValidPosition(world->E(northPoint)))
          && !world->getContent(northPoint))
        return northPoint;
      if (world->isValidPosition(southPoint)
          && (world->isValidPosition(world->SW(southPoint)) || world->isValidPosition(world->SE(southPoint))
              || world->isValidPosition(world->W(northPoint)) || world->isValidPosition(world->E(northPoint)))
          && !world->getContent(southPoint))
        return northPoint;
    }
  }

  // true if on top or bottom edge
  if ((!world->isValidPosition(world->NW(edgePoint)) && !world->isValidPosition(world->NE(edgePoint)))
      || (!world->isValidPosition(world->SW(edgePoint)) && !world->isValidPosition(world->SE(edgePoint)))) {
    Point2D westPoint = edgePoint;
    Point2D eastPoint = edgePoint;
    while (world->isValidPosition(eastPoint) || world->isValidPosition(westPoint)) {
      westPoint = world->W(westPoint);
      eastPoint = world->E(eastPoint);
      if (world->isValidPosition(eastPoint) && world->isValidPosition(world->E(eastPoint)) && !world->getContent(eastPoint)) return eastPoint;
      if (world->isValidPosition(westPoint) && world->isValidPosition(world->W(westPoint)) && !world->getContent(westPoint)) return westPoint;
    }
  }

  return edgePoint;
}
Point2D Catcher::randPointNearCat(World* world) {
  Point2D catPoint = world->getCat();
  if (!world->getContent(world->W(catPoint)) && !world->isValidPosition(world->W(catPoint))) return world->W(catPoint);
  if (!world->getContent(world->E(catPoint)) && !world->isValidPosition(world->E(catPoint))) return world->E(catPoint);
  if (!world->getContent(world->NW(catPoint)) && !world->isValidPosition(world->NW(catPoint))) return world->NW(catPoint);
  if (!world->getContent(world->SW(catPoint)) && !world->isValidPosition(world->SW(catPoint))) return world->SW(catPoint);
  if (!world->getContent(world->NE(catPoint)) && !world->isValidPosition(world->NE(catPoint))) return world->NE(catPoint);
  if (!world->getContent(world->SE(catPoint)) && !world->isValidPosition(world->SE(catPoint))) return world->SE(catPoint);
}
