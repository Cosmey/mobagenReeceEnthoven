#include "Agent.h"
#include <unordered_set>
#include <queue>
#include "World.h"
using namespace std;
std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  priority_queue<std::pair<int,Point2D>> priorityFrontier;
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  priorityFrontier.push(std::make_pair(0,catPos));
  frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points
  bool foundEdge = false;
  Point2D current;

  while (!priorityFrontier.empty() && !foundEdge) {
    current = priorityFrontier.top().second;
    priorityFrontier.pop();
    frontierSet.erase(current);
    visited[current] = true;
    std::vector<Point2D> neighbors = getVisitableNeighbors(w, current,&visited);
    for (int i = 0;i < neighbors.size();i++) {
      if (!w->isValidPosition(neighbors[i])) {
        foundEdge = true;
        break;
      }
      cameFrom[neighbors[i]] = current;
      priorityFrontier.push(std::make_pair(calculateHeuristic(w,neighbors[i]), neighbors[i]));
    }


  }

  std::vector<Point2D> path;
  Point2D currentPathPoint = current;
  while (catPos != currentPathPoint) {
    path.push_back(currentPathPoint);
    currentPathPoint = cameFrom[currentPathPoint];
  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  // if there isnt a reachable border, just return empty vector
  // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move
  return path;
}
std::vector<Point2D> Agent::getVisitableNeighbors(World* w, Point2D current, unordered_map<Point2D, bool>* visited) {
  std::vector<Point2D> neighbors;
  if (!w->getContent(World::W(current)) && !visited->contains(World::W(current))) neighbors.push_back(World::W(current));
  if (!w->getContent(World::E(current)) && !visited->contains(World::E(current))) neighbors.push_back(World::E(current));
  if (!w->getContent(World::NW(current)) && !visited->contains(World::NW(current))) neighbors.push_back(World::NW(current));
  if (!w->getContent(World::NE(current)) && !visited->contains(World::NE(current))) neighbors.push_back(World::NE(current));
  if (!w->getContent(World::SE(current)) && !visited->contains(World::SE(current))) neighbors.push_back(World::SE(current));
  if (!w->getContent(World::SW(current)) && !visited->contains(World::SW(current))) neighbors.push_back(World::SW(current));

  return neighbors;
}
int Agent::calculateHeuristic(World* w, Point2D current) {
  return std::max(std::abs(current.x),std::abs(current.y));
}
