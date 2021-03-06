#include <vector>
#include <algorithm>
#define ll long long

struct point
{
  ll x, y;
  point(){}
  point(int a, int b): x(a), y(b){}
};

/*
a->b->c
1 = points couter-clockwise
-1 = points clockwise
0 = points on a line
*/
int ccw(const point&a, const point&b, const point&c)
{
  ll delta = a.x * b.y + b.x * c.y + c.x * a.y - a.x * c.y - b.x * a.y - c.x * b.y;
  if(delta == 0) return 0;
  return delta < 0 ? -1 : 1;
}

class SortPoints
{
  public:
  static point origin;

  static bool cmpMinX(const point&a, const point&b)
  {
    if(a.x == origin.x && a.y == origin.y) return true;
    if(b.x == origin.x && b.y == origin.y) return false;
    return ccw(origin, a, b) > 0;
  }

  //sort points taking the point with minimum x as the center (4quad < 1quad)
  void sortAroundMinX(std::vector<point>&points)
  {
    origin = points[0];
    for(int i=1; i<points.size(); i++)
      if(points[i].x < origin.x || (points[i].x == origin.x && points[i].y < origin.y))
        origin = points[i];

    std::sort(points.begin(), points.end(), cmpMinX);
  }
};

//computes convex-hull of a set of points in O(NlogN) (first points must not be repeated at the end)
point SortPoints::origin = point(0, 0);
void grahamScan(std::vector<point>&poly, std::vector<point>&CH)
{
  SortPoints sp;
  sp.sortAroundMinX(poly);
  poly.push_back(poly[0]);
  for(int i=0; i<3; i++)
    CH.push_back(poly[i]);

  for(int i=3; i<poly.size(); i++)
  {
    while(CH.size() >= 3 && ccw(CH[CH.size()-2], CH[CH.size()-1], poly[i]) <= 0)
      CH.pop_back();
    CH.push_back(poly[i]);
  }

  poly.pop_back();
  CH.pop_back(); //remove first point repetition
}
