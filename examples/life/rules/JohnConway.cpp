#include "JohnConway.h"

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  for (int i = 0;i < world.SideSize();i++) {
    for (int j = 0;j < world.SideSize();j++) {
      Point2D point = Point2D(i,j);
      world.SetNext(point,evaluateCell(world,point));
    }
  }
}
bool JohnConway::evaluateCell(World& world,Point2D point) {
  bool live = world.Get(point);
  int neighborCount = CountNeighbors(world,point);
  if (live) {
    if (neighborCount < 2) return false;
    if (neighborCount < 4) return true;
    return false;
  }
  else {
    if (neighborCount == 3) {
      return true;
    }
  }

  return false;
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  int count = 0;
  for (int i = -1;i <= 1;i++) {
    for (int j = -1;j <= 1;j++){
      int newX = point.x+i;
      int newY = point.y+j;
      if (newX < 0) newX = world.SideSize()-1;
      if (newX > world.SideSize()-1) newX = 0;
      if (newY < 0) newY = world.SideSize()-1;
      if (newY > world.SideSize()-1) newY = 0;
      Point2D newPoint = Point2D(newX,newY);
      if(world.Get(newPoint) == true && (i != 0 || j != 0)) {
        count++;
      }
    }
  }
  return count;
}
