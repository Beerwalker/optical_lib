#include "DetectionHelper.h"
#include "CombinationEnumerator.h"
#include "CombinationEnumeratorv2.h"
#include "Instrument.h"
#include "Instrumentv2.h"
#include "APIstub.h"
#include "APIv2.h"
#include "CircleDetector.h"
#include <chrono>
#include "opencv2/opencv.hpp"


// Armadillo documentation is available at:
// http://arma.sourceforge.net/docs.html

int main(int argc, char** argv)
{
	mat needle = "977 522 0; 1182 486 0; 1139 568 0; 1104 426 0; 1079 504 0";
	mat needle_mm = "0 0 0; 70 0 0; 145 0 0; 95 43 0; 105 -45 0";
	mat refFrame = "794 256 0;  802 468 0; 1049 461 0; 1024 274 0; 923 331 0";
	mat frame_mm = "50 60 0; -50 55 0; -50 -65 0; 50 -5 0; 0 0 0";

		//mat fromVid = "794 256 0; 802 468 0; 1024 274 0; 1049 461 0; 923 331 0";
		mat fromVid = "794 256 0;  802 468 0; 1049 461 0; 1024 274 0; 923 331 0";
		mat fromVid2 = "990 282 0; 817 306 0; 979 490 0; 1130 441 0; 1009 376 0";
		mat fromVid3 = "937 281 0; 797 366 0; 1124 422 0; 1020 525 0; 994 384 0";
		mat instrData = "1 45 0; 2 234 0; 3 234 0; 4 234 0";
		mat pointData2 = "1 0 0; 2 0 0; 3 0 0; 4 0 0; 5 0 0; 6 0 0; 7 0 0";
		mat last_try = "1938 455 0; 633 353 0; 732 398 0; 590 401 0; 703 446 0; 674 392 0; 1105 354 0";
		mat kirill = "50 60 0; -50 55 0; -50 -65 0; 50 -65 0; 0 17 0";
	
		mat mixture = "129 55 0; 990 282 0; 800 400 0; 399 252 0; 817 306 0; 979 490 0; 1130 441 0;950 799 0; 152 800 0; 352 20 0; 614 555 0; 1009 376 0";
	
		//newest shit
		mat right_needle_big = "850 397 0; 985 452 0; 828 506 0; 931 574 0; 681 559 0";


		//Instrumentv2 instrument(needle_mm,1);
		//CombinationEnumeratorv2 finder(right_needle_big, 5);
		//mat result;
		//bool isFound = finder.findInstrument(&instrument, 0.01f, result);
	
		//draw result
		//cv::Mat drawing(1080, 1920, CV_8UC1, cv::Scalar(0));
		//cv::Point* result_points = new cv::Point(result.n_rows);
		//for(int i =0 ; i < result.n_rows; i++){
		//result_points[i].x = result(i,0);
		//result_points[i].y = result(i,1);
		//}
		//cv::fillConvexPoly(drawing, result_points,result.n_rows, cv::Scalar(255));
		//cv::namedWindow("edges"); cv::imshow("edges", drawing);
		//cv::waitKey(-1);
	/*HOME TEST*/
	CircleDetector detector(3.0f,24.0f);
	cv::VideoCapture cap("rightcam_bothInstuments.avi"); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;
	cv::Mat gray;
	cv::Mat color;
	int u = 0;
	std::vector<cv::Point2f> circles;
	Instrumentv2 instrument(frame_mm,1);
 	while(u < 500){
		u++;
		cap >> color; // get a new frame from camera
		if (color.rows == 0) continue;
		//cv::resize(color,color,Size(),0.5,0.5,INTER_AREA);
		cv::cvtColor(color, gray, CV_RGB2GRAY);
		detector.getCircles(gray,circles,12);
		arma::mat circles_result = zeros<mat>(circles.size(),3);
		for(int k = 0; k < circles.size(); k++){
			circles_result(k,0) = circles[k].x;
			circles_result(k,1) = circles[k].y;
		}

		
		CombinationEnumeratorv2 finder(circles_result, 5);
		mat instrument_points;
		bool isFound = finder.findInstrument(&instrument, 0.015f, instrument_points);

		cv::Mat drawing(1080, 1920, CV_8UC1, cv::Scalar(0));
		cv::Point* result_points = new cv::Point[instrument_points.n_rows];
		for(int i =0 ; i <instrument_points.n_rows; i++){
			result_points[i].x = instrument_points(i,0);
			result_points[i].y = instrument_points(i,1);
			cv::circle(drawing,result_points[i],5, cv::Scalar(255),3);
		}
		//cv::fillConvexPoly(drawing, result_points,z.size(), cv::Scalar(255));
		cv::namedWindow("result",cv::WINDOW_NORMAL ); cv::imshow("result", drawing);
		cv::waitKey(1);
	}
	/*-----------------------------*/


//	mat refFrame = "794 256 0;  802 468 0; 1049 461 0; 1024 274 0; 923 331 0";
//	//mat needle = "977 522 0; 1182 486 0; 1139 568 0; 1104 426 0; 1079 504 0";
//	//mat refFrame = "100 125 0; 0 120 0; 0 0 0; 100 0 0";
//	vector<Point3d> points;
//	Point3d point;
//	point.x = 794;
//	point.y = 256;
//	point.z = 0;
//	points.push_back(point);
//	point.x = 802;
//	point.y = 468;
//	point.z = 0;
//	points.push_back(point);
//	point.x = 1049;
//	point.y = 461;
//	point.z = 0;
//	points.push_back(point);
//	point.x = 1024;
//	point.y = 274;
//	point.z = 0;
//	points.push_back(point);
//	point.x = 923;
//	point.y = 331;
//	point.z = 0;
//	points.push_back(point);
//
//	vector<Point3d> needle_points;
//	point.x = 977;
//	point.y = 522;
//	point.z = 0;
//	needle_points.push_back(point);
//	point.x = 1182;
//	point.y = 486;
//	point.z = 0;
//	needle_points.push_back(point);
//	point.x = 1139;
//	point.y = 568;
//	point.z = 0;
//	needle_points.push_back(point);
//	point.x = 1104;
//	point.y = 426;
//	point.z = 0;
//	needle_points.push_back(point);
//	point.x = 1079;
//	point.y = 504;
//	point.z = 0;
//	needle_points.push_back(point);
//
//	APIv2 test2;
//	VIID id = test2.createInstrument(points);
//	VIID id_needle = test2.createInstrument(needle_points);
//	test2.beginSearch();
//
////	cv::Mat image(1500, 1000, CV_8UC1);
////	cv::Point pts[4];
////	cv::namedWindow("shit", CV_WINDOW_NORMAL);
//	while (1)
//	{
//		vector<Point3d> k = test2.getLastCoords(id);
//		int t = test2.getInstrumentUpdateTime(id);
//		bool tt = test2.isInstrumentVisible(id);
//		if (tt)
//		{
//			std::cout << "frame" << std::endl;
//			std::cout << k[0].x << " " << k[0].y << " " << k[0].z << std::endl;
//			std::cout << k[1].x << " " << k[1].y << " " << k[1].z << std::endl;
//			std::cout << k[2].x << " " << k[2].y << " " << k[2].z << std::endl;
//			std::cout << k[3].x << " " << k[3].y << " " << k[3].z << std::endl;
//			std::cout << k[4].x << " " << k[4].y << " " << k[4].z << std::endl << std::endl;
//			//draw result
//			//draw result
//			//cv::Mat drawing(1080, 1920, CV_8UC1, cv::Scalar(0));
//			//cv::Point* result_points = new cv::Point[k.size()];
//			//for(int i =0 ; i < k.size(); i++){
//			//	result_points[i].x = k[i].x;
//			//	result_points[i].y = k[i].y;
//			//	cv::circle(drawing,result_points[i],5, cv::Scalar(255));
//			//}
//			////cv::fillConvexPoly(drawing, result_points,z.size(), cv::Scalar(255));
//			//cv::namedWindow("edges"); cv::imshow("edges", drawing);
//			//cv::waitKey(-1);
//		}
//		else
//		{
//			std::cout << "Not visible" << std::endl;
//		}
//		vector<Point3d> z = test2.getLastCoords(id_needle);
//		t = test2.getInstrumentUpdateTime(id_needle);
//		tt = test2.isInstrumentVisible(id_needle);
//		if (tt)
//		{
//			std::cout << "needle" << std::endl;
//			std::cout << z[0].x << " " << z[0].y << " " << z[0].z << std::endl;
//			std::cout << z[1].x << " " << z[1].y << " " << z[1].z << std::endl;
//			std::cout << z[2].x << " " << z[2].y << " " << z[2].z << std::endl;
//			std::cout << z[3].x << " " << z[3].y << " " << z[3].z << std::endl;
//			std::cout << z[4].x << " " << z[4].y << " " << z[4].z << std::endl << std::endl;
//
//		}
//		else
//		{
//			std::cout << "Not visible" << std::endl;
//		}
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//	}
//	test2.endSearch();

//	float epsilon = 0.0001;
//	//mat pointData  = {{360, 354, 0}, {535, 360, 0}, {443, 449, 0}, {365, 491, 0}, {530, 508, 0}};
//	mat pointData2 = "360 354 0; 100 25 0; 456 89 0; 535 360 0; 443 449 0; 365 491 0; 530 508 0";
//	//mat pointData2 = "360 354 0; 535 360 0; 443 449 0; 365 491 0; 530 508 0";
//	mat pointData = "577 348 0; 407 484 0; 399 347 0; 485 440 0; 576 499 0";
//	//%b2=[577 348 0; 407 484 0; 399 347 0; 485 440 0; 576 499 0];
//	//mat pointData = "-1 -1 0; 1 -1 0; 1 1 0; -1 1 0; 0 0 0 ";
//	sVisionInstrument instrument(pointData,1);
//	CombinationEnumerator finder(pointData2, 5);
//	mat result;
//	bool isFound = finder.findInstrument(&instrument, 2, result);
//
//
//	vector<Point3d> points;
//	for(int i = 0; i < 5; i++){
//		Point3d point;
//		point.x = rand() % 100;
//		point.y = rand() % 100;
//		point.z = rand() % 100;
//		points.push_back(point);
//	}
//
//	//APIstub api_test;
//	//VIID id = api_test.createInstrument(points);
//	//api_test.beginSearch();
//	//api_test.endSearch();
//
//	APIv2 test2;
//	VIID id = test2.createInstrument(points);
//	test2.beginSearch();
//	vector<Point3d> k = test2.getLastCoords(id);
//	int t = test2.getInstrumentUpdateTime(id);
//	bool tt = test2.isInstrumentVisible(id);
//	test2.endSearch();

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

