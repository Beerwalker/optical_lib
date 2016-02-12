#ifndef __HULL_H__
#define __HULL_H__

#include <algorithm>
#include <vector>
using namespace std;

typedef double coord_t;         // coordinate type
typedef double coord2_t;  // must be big enough to hold 2*max(|coordinate|)^2

struct TPoint {
	coord_t x, y;
	TPoint(){}
	TPoint(coord_t x, coord_t y):x(x),y(y){}

	bool operator <(const TPoint &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
};

coord2_t cross(const TPoint &O, const TPoint &A, const TPoint &B);
vector<TPoint> convex_hull(vector<TPoint> P);


#endif