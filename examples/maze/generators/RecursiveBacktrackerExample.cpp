#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>
#include <stack>
bool RecursiveBacktrackerExample::Step(World* w) {
  if (stack.size() == 0) {
    Point2D randomPoint = randomStartPoint(w);
    if (randomPoint.x != INT_MAX && randomPoint.y != INT_MAX) {
      stack.push_back(randomPoint);
      visited[randomPoint.x][randomPoint.y] = true;
      w->SetNodeColor(randomPoint,Color32(0,0,255,255));
      return true;
    }
    else return false;
  }

  Point2D current = stack.back();
  visited[current.x][current.y] = true;
  std::vector<Point2D> neighbors = getVisitables(w, current);
  Point2D newPoint;
  if (neighbors.size() > 0) {
    if (neighbors.size() == 1) newPoint = neighbors[0];
    else  newPoint = neighbors[rand() % neighbors.size()];
    w->SetNodeColor(newPoint,Color32(0,0,255,255));
    Point2D direction = newPoint - current;
    if (direction == Point2D(0, 1)) w->SetNorth(newPoint, false);
    else if (direction == Point2D(0, -1)) w->SetSouth(newPoint, false);
    else if (direction == Point2D(-1, 0)) w->SetEast(newPoint, false);
    else if (direction == Point2D(1, 0)) w->SetWest(newPoint, false);
    stack.emplace_back(newPoint);
  }
  else {
    w->SetNodeColor(current,Color32(0,0,0,0));
    stack.pop_back();
  }



  return true;
}

void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;
  if ((p.x - 1 >= -sideOver2) && !visited[p.x - 1][p.y]) visitables.push_back(Point2D(p.x - 1, p.y));
  if ((p.x + 1 <= sideOver2) && !visited[p.x + 1][p.y]) visitables.push_back(Point2D(p.x + 1, p.y));
  if ((p.y + 1 <= sideOver2) && !visited[p.x][p.y + 1]) visitables.push_back(Point2D(p.x, p.y + 1));
  if ((p.y - 1 >= -sideOver2) && !visited[p.x][p.y - 1]) visitables.push_back(Point2D(p.x, p.y - 1));

  return visitables;
}

