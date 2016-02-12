#include "DetectionHelper.h"
#include "CombinationEnumerator.h"
#include "Instrument.h"
#include "APIstub.h"
#include "APIv2.h"


// Armadillo documentation is available at:
// http://arma.sourceforge.net/docs.html






int main(int argc, char** argv)
{
	float epsilon = 0.0001;
	//mat pointData  = {{360, 354, 0}, {535, 360, 0}, {443, 449, 0}, {365, 491, 0}, {530, 508, 0}};
	mat pointData2 = "360 354 0; 100 25 0; 456 89 0; 535 360 0; 443 449 0; 365 491 0; 530 508 0";
	//mat pointData2 = "360 354 0; 535 360 0; 443 449 0; 365 491 0; 530 508 0";
	mat pointData = "577 348 0; 407 484 0; 399 347 0; 485 440 0; 576 499 0";
	//%b2=[577 348 0; 407 484 0; 399 347 0; 485 440 0; 576 499 0];
	//mat pointData = "-1 -1 0; 1 -1 0; 1 1 0; -1 1 0; 0 0 0 ";
	sVisionInstrument instrument(pointData,1);
	CombinationEnumerator finder(pointData2, 5);
	mat result;
	bool isFound = finder.findInstrument(&instrument, 2, result);


	vector<Point3d> points;
	for(int i = 0; i < 5; i++){
		Point3d point;
		point.x = rand() % 100;
		point.y = rand() % 100;
		point.z = rand() % 100;
		points.push_back(point);
	}

	//APIstub api_test;
	//VIID id = api_test.createInstrument(points);
	//api_test.beginSearch();
	//api_test.endSearch();
	
	APIv2 test2;
	VIID id = test2.createInstrument(points);
	test2.beginSearch();
	vector<Point3d> k = test2.getLastCoords(id);
	int t = test2.getInstrumentUpdateTime(id);
	bool tt = test2.isInstrumentVisible(id);
	test2.endSearch();

	//mat result = DetectionHelper::whitenData(pointData);
	//mat shape_contour = DetectionHelper::getBoundary(pointData);
	//double ratio = DetectionHelper::calculatePolyRatio(shape_contour);


	//get mean column-wise and substract it
	//mat mu = mean(pointData);
	//for(int i=0;i<pointData.n_cols;i++){
	//	pointData.cols(0,i).rows(0,size(pointData).n_rows-1) -= mu(0,i);
	//}
	//

	//mat A = pointData.t()*pointData;

	//mat V,unused;
	//vec D;
	//svd(V,D,unused,A);
	//mat diagD = diagmat(D);

	//mat whMat = sqrt(pointData.n_rows-1)*V*sqrt(inv(diagD+eye(size(diagD))*epsilon))*V.t();
	//mat whiten = pointData*whMat;
	//pointData = whiten;
	//std::cout << pointData << std::endl;

	//mat shape_contour = getBoundary(pointData);



	//std::cout << pointData << std::endl;
	//std::cout << boundary << std::endl;
	//mat distances(size(pointData).n_rows,size(pointData).n_rows);

	////calculate pairwise distances
	//for(int i = 0; i < distances.n_rows; i++){
	//	for(int j = 0; j < distances.n_rows; j++){
	//		distances(i,j) = pow(pointData(i,0)-pointData(j,0),2) + pow(pointData(i,1) - pointData(j,1),2);
	//	}
	//}
	//distances = sum(distances,1);

	//uword center_position;
	//distances.min(center_position);

	////start to make shape contour and get starting point!
	//mat shape_contour = zeros<mat>(pointData.n_rows,3);
	//shape_contour.row(0) = pointData.row((center_position)%4+1);
	//vec u = (shape_contour.row(0) - pointData.row(center_position)).t();

	////get angles
	//vec angles = zeros<vec>(pointData.n_rows);
	//for(int i = 0; i < pointData.n_rows; i++){
	//	vec v = (pointData.row(i) - pointData.row(center_position)).t();
	//	std::cout << v << std::endl;
	//	if(i == center_position){
	//		angles(i) = 0;
	//		continue;
	//	}
	//	//double angle = atan2(det(u,v),dot(u,v));
	//	double angle2 = atan2(u(1),u(0)) - atan2(v(1),v(0));
	//	double angle = (atan2(u(1),u(0)) - atan2(v(1),v(0)))* 180 / 3.1428;
	//	if (angle < 0){
	//		angle = angle + 360;
	//	}
	//	angles(i) = angle;
	//}

	////build shape according to angles
	//for(int i =0; i < pointData.n_rows-2; i++){
	//	uword current_max_pos = NULL;
	//	angles.max(current_max_pos);
	//	angles(current_max_pos) = 0;
	//	shape_contour.row(i+1) = pointData.row(current_max_pos);
	//}
	//shape_contour.row(pointData.n_rows-1) = shape_contour.row(0);

	//calculate area
	//double first=0,second=0;
	//for (int i =0; i < shape_contour.n_rows -1; i++){
	//	first += shape_contour(i,0)*shape_contour(i+1,1);
	//	second += shape_contour(i,1)*shape_contour(i+1,0);
	//}
	//double area = (first - second)/2;

	////find perimeter
	//double perimeter = 0;
	//for (int i = 0; i < shape_contour.n_rows-1; i++){
	//	perimeter += sqrt(pow(shape_contour(i,0)-shape_contour(i+1,0),2) + pow(shape_contour(i,1)-shape_contour(i+1,1),2));
	//}

	//	return 0;
}

