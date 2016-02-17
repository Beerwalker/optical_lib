#ifndef __DETECTION_H__
#define __DETECTION_H__

#include <iostream>
#include <armadillo>
#include "ConvexHull.h"

using namespace std;
using namespace arma;

class DetectionHelper{
public:
	static bool arePolygonsEqual(mat polygon, double ratio, double epsilon);
	static mat whitenData(mat source);
	static double calculatePolyRatio(mat shape_contour);
	static double calculatePerimeter(mat& shape_contour);
	static mat getBoundary(mat points);
	static mat compute2DPolygonCentroid(const mat& shape);
	static bool deductCenter(const mat & figure, const mat & shape, mat& center);
	static bool matchShapeToRef(const mat& reference, const mat& shape, std::vector<int>& shape_to_reference, mat& center);
	static int getClosestToCentroid(const mat& shape, const vec& centroid);
	static mat calculate3Dcoords(const mat& left, const mat& right, const mat& Q);
};

#endif
