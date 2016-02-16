#include "DetectionHelper.h"
#include "float.h"

bool DetectionHelper::arePolygonsEqual(mat polygon, double ratio, double epsilon){
	return false;
}

mat DetectionHelper::whitenData(mat source){
	const float epsilon = 0.0001;

	//get mean column-wise and substract it
	mat mu = mean(source);
	for(int i=0;i<source.n_cols;i++){
		for(int j=0;j<source.n_rows;j++){
			source(j,i) -= mu(0,i);
		}
		//source.cols(0,i).rows(0,arma::size(source).n_rows-1) -= mu(0,i);
	}

	mat A = source.t()*source;

	mat V,unused;
	vec D;
	svd(V,D,unused,A);
	mat diagD = diagmat(D);

	mat whMat = sqrt(source.n_rows-1)*V*sqrt(inv(diagD+eye(arma::size(diagD))*epsilon))*V.t();
	source = source*whMat;
	return source;
}

//shape must have last element == first element
double DetectionHelper::calculatePolyRatio(mat shape_contour){
	double first=0,second=0;
	for (int i =0; i < shape_contour.n_rows -1; i++){
		first += shape_contour(i,0)*shape_contour(i+1,1);
		second += shape_contour(i,1)*shape_contour(i+1,0);
	}
	double area = (first - second)/2;

	//find perimeter
	double perimeter = DetectionHelper::calculatePerimeter(shape_contour);

	return perimeter*perimeter/area;
}

double DetectionHelper::calculatePerimeter(mat & shape_contour)
{
	//find perimeter
	double perimeter = 0;
	for (int i = 0; i < shape_contour.n_rows - 1; i++) {
		perimeter += sqrt(pow(shape_contour(i, 0) - shape_contour(i + 1, 0), 2) + pow(shape_contour(i, 1) - shape_contour(i + 1, 1), 2));
	}

	return perimeter;
}

mat DetectionHelper::getBoundary(mat points){
	vector<TPoint> points_vec;
	for(int i =0 ;i < points.n_rows; i++){
		points_vec.push_back(TPoint(points(i,0),points(i,1)));
	}
	vector<TPoint> result = convex_hull(points_vec);
	points = zeros<mat>(result.size(),3);
	for(int i =0 ;i < points.n_rows; i++){
		points(i,0) = result[i].x;
		points(i,1) = result[i].y;
	}
	return points;
}

//shape must have last element == first element
mat DetectionHelper::compute2DPolygonCentroid(const mat& shape)
{
	vec centroid = zeros<vec>(2);
	int vertexCount = shape.n_rows;

	double signedArea = 0.0;
	double x0 = 0.0; // Current vertex X
	double y0 = 0.0; // Current vertex Y
	double x1 = 0.0; // Next vertex X
	double y1 = 0.0; // Next vertex Y
	double a = 0.0;  // Partial signed area

	// For all vertices
	int i=0;
	for (i=0; i<vertexCount-1; ++i)
	{
		x0 = shape(i,0);
		y0 = shape(i,1);
		x1 = shape((i+1) % vertexCount,0);
		y1 = shape((i+1) % vertexCount,1);
		a = x0*y1 - x1*y0;
		signedArea += a;
		centroid(0) += (x0 + x1)*a;
		centroid(1) += (y0 + y1)*a;
	}

	signedArea *= 0.5;
	centroid(0) /= (6.0*signedArea);
	centroid(1) /= (6.0*signedArea);

	return centroid;
}

bool DetectionHelper::deductCenter(const mat & figure, const mat & shape, mat& center)
{
	bool found = false;
	center = zeros<mat>(1,figure.n_cols);
	for (int i = 0; i < figure.n_rows; i++) {
		for (int j = 0; j < shape.n_rows; j++) {
			if (arma::norm(figure.row(i) - shape.row(j), "inf") < 0.001) {
				found = true;
				break;
			}
		}
		//couldn't find point on figure -> then center
		if (!found){
			center = figure.row(i);
			return true;
		}
	}

	return false;
}

bool DetectionHelper::matchShapeToRef(const mat& reference, const mat& shape, std::vector<int>& shape_to_reference, mat& center){

	bool has_center = false;
	//����������� ����� �� shape � reference
	shape_to_reference.resize(reference.n_rows);
	for (int i = 0; i != reference.n_rows; i++) {
		for (int j = 0; j != shape.n_rows - 1; j++) {
			if (arma::norm(reference.row(i) - shape.row(j), "inf") < 0.001) {
				shape_to_reference[j] = i;
				break;
			}
			//reference point not found -> then it's center!!!!!!!!
			else if (j == shape.n_rows - 2) {
				shape_to_reference[reference.n_rows - 1] = i;
				center = zeros<mat>(1,3);
				center = reference.row(i);
				has_center = true;
			}
		}
	}

	return has_center;
}

int DetectionHelper::getClosestToCentroid(const mat& shape, const vec& centroid){
	double min_distance = DBL_MAX;
	int closest_index = 0;
	for (int i = 0; i < shape.n_rows - 1; i++) {
		double cur_distance = pow(centroid(0) - shape(i, 0), 2) + pow(centroid(1) - shape(i, 1), 2);
		if (cur_distance < min_distance) {
			min_distance = cur_distance;
			closest_index = i;
		}
	}

	return closest_index;
}
