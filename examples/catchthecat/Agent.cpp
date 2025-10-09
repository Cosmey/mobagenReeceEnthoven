#include "Agent.h"
#include <unordered_set>
#include <queue>
#include "World.h"
using namespace std;


std::vector<Point2D> Agent::generatePath(World* w,std::vector<Point2D> exclusions) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  priority_queue<PriorityPoint,std::vector<PriorityPoint>,greater<PriorityPoint>> priorityFrontier;
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  for (int i = 0;i < exclusions.size();i++) {
    visited[exclusions[i]] = true;
  }

  // bootstrap state
  auto catPos = w->getCat();
  priorityFrontier.push(PriorityPoint(catPos,0,0));
  frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points
  bool foundEdge = false;
  Point2D current;
  float distanceTravelled;
  int numExplored = 0;

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
      float neighborTotalDistance = distanceTravelled +1;
      priorityFrontier.push(PriorityPoint(neighbors[i],calculateHeuristic(w,neighbors[i],neighborTotalDistance),neighborTotalDistance));
      frontierSet.emplace(neighbors[i]);
    }

    numExplored++;
  }

  std::cout << numExplored << std::endl;

  std::vector<Point2D> path;
  Point2D currentPathPoint = current;
  while (catPos != currentPathPoint) {
    path.push_back(currentPathPoint);
    currentPathPoint = cameFrom[currentPathPoint];
  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  // if there isnt a reachable border, just return empty vector
  // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move

  std::vector<Point2D> noExitPath;
  noExitPath.push_back(borderExit);
  if (!foundEdge) return noExitPath;
  else return path;
}
std::vector<Point2D> Agent::getVisitableNeighbors(World* w, Point2D current, unordered_map<Point2D, bool>* visited,
                                                  std::unordered_set<Point2D>* frontierSet) {
  std::vector<Point2D> neighbors = w->neighbors(current);
  std::vector<Point2D> returnedNeighbors;
  // checks for each direction that the point is either out of bounds, or has no wall and has not been visited/added to the frontier

  for (int i = 0; i < neighbors.size(); i++) {
    if (!w->isValidPosition(neighbors[i])
        || (!w->getContent(neighbors[i]) && !visited->contains(neighbors[i]) && !frontierSet->contains(neighbors[i])))
      returnedNeighbors.push_back(neighbors[i]);
  }

  return returnedNeighbors;
}
std::vector<Point2D> Agent::getVisitableNeighbors(World* w, Point2D current) {
  std::vector<Point2D> neighbors = w->neighbors(current);
  std::vector<Point2D> returnedNeighbors;
  // checks for each direction that the point is either out of bounds, or has no wall and has not been visited/added to the frontier

  for (int i = 0; i < neighbors.size(); i++) {
    if (!w->isValidPosition(neighbors[i]) || (!w->getContent(neighbors[i])))
      returnedNeighbors.push_back(neighbors[i]);
  }

  return returnedNeighbors;

}

float Agent::calculateHeuristic(World* w, Point2D current,int distanceTravelled) {
  float newDistance = distanceTravelled*0.99f;
  if (distanceTravelled - newDistance >= 1) newDistance = distanceTravelled;
  float thing = -(float)std::max(std::abs(current.x),std::abs(current.y)) + newDistance;
  return thing;
}







PriorityPoint::PriorityPoint(Point2D point, float priority,int distanceTravelled) {
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
