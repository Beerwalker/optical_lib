// Implementation of Andrew's monotone chain 2D convex hull algorithm.
// Asymptotic complexity: O(n log n).
// Practical performance: 0.5-1.0 seconds for n=1000000 on a 1GHz machine.
#include "ConvexHull.h"

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
coord2_t cross(const TPoint &O, const TPoint &A, const TPoint &B)
{
	double res = (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
	return res;
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
vector<TPoint> convex_hull(vector<TPoint> P)
{
	int n = P.size(), k = 0;
	vector<TPoint> H(2*n);

	// Sort points lexicographically
	sort(P.begin(), P.end());

	// Build lower hull
	for (int i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}

	//vector<Point> rev;
	//for(int i = P.size()-1; i >=0; i--){
	//	rev.push_back(P[i]);
	//}
	//vector<Point> U(2*n);
	//k=0;
	//for (int i = 0; i < n; ++i) {
	//	while (k >= 2 && cross(U[k-2], U[k-1], rev[i]) <= 0) k--;
	//	U[k++] = rev[i];
	//}

	// Build upper hull
	for (int i = n-2, t = k+1; i >= 0; i--) {
		while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0.0001) k--;
		H[k++] = P[i];
	}

	H.resize(k);
	return H;
}