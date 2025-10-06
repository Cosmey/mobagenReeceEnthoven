#include "Agent.h"
#include <unordered_set>
#include <queue>
#include "World.h"
using namespace std;
std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  priority_queue<PriorityPoint,std::vector<PriorityPoint>,greater<PriorityPoint>> priorityFrontier;
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  priorityFrontier.push(PriorityPoint(catPos,0,0));
  frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points
  bool foundEdge = false;
  Point2D current;
  int distanceTravelled;

  while (!priorityFrontier.empty() && !foundEdge) {
    current = priorityFrontier.top().getPoint();
    distanceTravelled = priorityFrontier.top().getDistanceTravelled();
    priorityFrontier.pop();
    frontierSet.erase(current);
    visited[current] = true;
    std::vector<Point2D> neighbors = getVisitableNeighbors(w, current,&visited,&frontierSet);
    for (int i = 0;i < neighbors.size();i++) {
      if (!w->isValidPosition(neighbors[i])) {
        foundEdge = true;
        break;
      }
      cameFrom[neighbors[i]] = current;
      int neighborTotalDistance = distanceTravelled +1;
      priorityFrontier.push(PriorityPoint(neighbors[i],calculateHeuristic(w,neighbors[i],neighborTotalDistance),neighborTotalDistance));
      frontierSet.emplace(neighbors[i]);
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
std::vector<Point2D> Agent::getVisitableNeighbors(World* w, Point2D current, unordered_map<Point2D, bool>* visited,std::unordered_set<Point2D>* frontierSet) {
  std::vector<Point2D> neighbors;
  if (!w->isValidPosition(World::W(current)) || (!w->getContent(World::W(current)) && !visited->contains(World::W(current)) && !frontierSet->contains(World::W(current)))) neighbors.push_back(World::W(current));
  if (!w->isValidPosition(World::E(current)) || (!w->getContent(World::E(current)) && !visited->contains(World::E(current))&& !frontierSet->contains(World::E(current)))) neighbors.push_back(World::E(current));
  if (!w->isValidPosition(World::NW(current)) || (!w->getContent(World::NW(current)) && !visited->contains(World::NW(current))&& !frontierSet->contains(World::NW(current)))) neighbors.push_back(World::NW(current));
  if (!w->isValidPosition(World::NE(current)) || (!w->getContent(World::NE(current)) && !visited->contains(World::NE(current))&& !frontierSet->contains(World::NE(current)))) neighbors.push_back(World::NE(current));
  if (!w->isValidPosition(World::SE(current)) || (!w->getContent(World::SE(current)) && !visited->contains(World::SE(current))&& !frontierSet->contains(World::SE(current)))) neighbors.push_back(World::SE(current));
  if (!w->isValidPosition(World::SW(current)) || (!w->getContent(World::SW(current)) && !visited->contains(World::SW(current))&& !frontierSet->contains(World::SW(current)))) neighbors.push_back(World::SW(current));

  return neighbors;
}
int Agent::calculateHeuristic(World* w, Point2D current,int distanceTravelled) {
  return -std::max(std::abs(current.x),std::abs(current.y)) + distanceTravelled;
}







PriorityPoint::PriorityPoint(Point2D point, int priority,int distanceTravelled) {
 this->priority = priority;
  this->point = point;
  this->distanceTravelled = distanceTravelled;
}

bool PriorityPoint::operator<(const PriorityPoint& other) const {
  return priority < other.priority;
}

bool PriorityPoint::operator>(const PriorityPoint& other) const {
  return priority > other.priority;
}
