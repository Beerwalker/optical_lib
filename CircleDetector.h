#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>



struct CircleData{
	float radius;
	cv::Point2f center;

	CircleData(float radius, cv::Point2f center) :radius(radius),center(center){};
};

class CircleDetector{
private:
	cv::Mat mask;
	cv::Mat dt;
	std::map<float,CircleData> circles;
	std::vector<cv::Point2f> edgePositions;

	static const unsigned int canny_lower = 240;
	static const unsigned int canny_upper = 255;
	static const unsigned int nIterations = 10000;
	float minCircleRadius; //5.0f
	float maxCircleRadius; //16.0f

	inline void getCircle(cv::Point2f& p1,cv::Point2f& p2,cv::Point2f& p3, cv::Point2f& center, float& radius);
	std::vector<cv::Point2f> getPointPositions(cv::Mat binaryImage);
	float evaluateCircle(cv::Mat dt, cv::Point2f center, float radius);
public:

	CircleDetector(float minRad, float maxRad):minCircleRadius(minRad),maxCircleRadius(maxRad){};
	void getCircles(cv::Mat image, std::vector<cv::Point2f>& points, int circlesToDetect);
};