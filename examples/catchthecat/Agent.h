#ifndef AGENT_H
#define AGENT_H
#include "math/Point2D.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class World;





class PriorityPoint {
public:
  PriorityPoint(Point2D point,int priority,int distanceTravelled);

  bool operator<(const PriorityPoint &other) const;
  bool operator>(const PriorityPoint &other) const;
  Point2D getPoint() const {return point;};
  int getDistanceTravelled() const {return distanceTravelled;};
private:
  Point2D point;
  int priority;
  int distanceTravelled;
};



class Agent {
public:
  explicit Agent() = default;

  virtual Point2D Move(World*) = 0;

  std::vector<Point2D> generatePath(World* w,std::vector<Point2D> exclusions = std::vector<Point2D>());
  std::vector<Point2D> getVisitableNeighbors(World* w,Point2D current,std::unordered_map<Point2D, bool>* visited,std::unordered_set<Point2D>* frontierSet);
  std::vector<Point2D> getVisitableNeighbors(World* w,Point2D current);
  int calculateHeuristic(World* w,Point2D current,int distanceTravelled);
};




#endif  // AGENT_H
