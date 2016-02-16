#include <windows.h>
#include "CircleDetector.h"

inline void CircleDetector::getCircle(cv::Point2f& p1,cv::Point2f& p2,cv::Point2f& p3, cv::Point2f& center, float& radius){
	float x1 = p1.x;
	float x2 = p2.x;
	float x3 = p3.x;

	float y1 = p1.y;
	float y2 = p2.y;
	float y3 = p3.y;

	// PLEASE CHECK FOR TYPOS IN THE FORMULA :)
	center.x = (x1*x1+y1*y1)*(y2-y3) + (x2*x2+y2*y2)*(y3-y1) + (x3*x3+y3*y3)*(y1-y2);
	center.x /= ( 2*(x1*(y2-y3) - y1*(x2-x3) + x2*y3 - x3*y2) );

	center.y = (x1*x1 + y1*y1)*(x3-x2) + (x2*x2+y2*y2)*(x1-x3) + (x3*x3 + y3*y3)*(x2-x1);
	center.y /= ( 2*(x1*(y2-y3) - y1*(x2-x3) + x2*y3 - x3*y2) );

	radius = sqrt((center.x-x1)*(center.x-x1) + (center.y-y1)*(center.y-y1));
}

std::vector<cv::Point2f> CircleDetector::getPointPositions(cv::Mat binaryImage){
	std::vector<cv::Point2f> pointPositions;

	for(unsigned int y=0; y<binaryImage.rows; ++y)
	{
		//unsigned char* rowPtr = binaryImage.ptr<unsigned char>(y);
		for(unsigned int x=0; x<binaryImage.cols; ++x)
		{
			//if(rowPtr[x] > 0) pointPositions.push_back(cv::Point2i(x,y));
			if(binaryImage.at<unsigned char>(y,x) > 0) pointPositions.push_back(cv::Point2f(x,y));
		}
	}

	return pointPositions;
}

float CircleDetector::evaluateCircle(cv::Mat dt, cv::Point2f center, float radius){
	float completeDistance = 0.0f;
	int counter = 0;

	float maxDist = 1.0f;   //TODO: this might depend on the size of the circle!

	float minStep = 0.001f;
	// choose samples along the circle and count inlier percentage

	//HERE IS THE TRICK that no minimum/maximum circle is used, the number of generated points along the circle depends on the radius.
	// if this is too slow for you (e.g. too many points created for each circle), increase the step parameter, but only by factor so that it still depends on the radius

	// the parameter step depends on the circle size, otherwise small circles will create more inlier on the circle
	float step = 2*3.14159265359f / (6.0f * radius);
	if(step < minStep) step = minStep; // TODO: find a good value here.

	for(float t =0; t<2*3.14159265359f; t+= step)
	{
		float cX = radius*cos(t) + center.x;
		float cY = radius*sin(t) + center.y;

		if(cX < dt.cols)
			if(cX >= 0)
				if(cY < dt.rows)
					if(cY >= 0)
						completeDistance += dt.at<float>(cY,cX);


	}
	//return inverse average distance
	return -completeDistance/(2*3.14159265359f/step);
}

void CircleDetector::getCircles(cv::Mat image, std::vector<cv::Point2f>& points, int circlesToDetect){
	cv::Mat write(image.clone());
	write.setTo(0);
	circles.clear();
	points.clear();
	edgePositions.clear();

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency); 
	QueryPerformanceCounter(&StartingTime);


	cv::Canny(image, mask, canny_lower,canny_upper);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	std::cout << ElapsedMicroseconds.QuadPart<<std::endl;
	edgePositions = getPointPositions(mask);

	// create distance transform to efficiently evaluate distance to nearest edge
	cv::distanceTransform(255-mask, dt,CV_DIST_L1, 3);

    //#pragma omp parallel for
	for(int i=0; i<nIterations; ++i)
	{

		unsigned int idx1 = rand()%edgePositions.size();
		unsigned int idx2 = abs(int(idx1 + rand()%edgePositions.size()/4 - edgePositions.size()/8))%edgePositions.size();
		unsigned int idx3 = abs(int(idx1 + rand()%edgePositions.size()/4 - edgePositions.size()/8))%edgePositions.size();

		// we need 3 different samples:
		if(idx1 == idx2) continue;
		if(idx1 == idx3) continue;
		if(idx3 == idx2) continue;

		// create circle from 3 points:
		cv::Point2f center; float radius;
		getCircle(edgePositions[idx1],edgePositions[idx2],edgePositions[idx3],center,radius);

		if(radius < minCircleRadius || radius > maxCircleRadius || radius != radius)continue;

		//verify or falsify the circle by inlier counting:
		float cVal = evaluateCircle(dt,center,radius);

		if (cVal > -1.0f){
			circles.insert(std::pair<float, CircleData>(cVal,CircleData(radius,center)));
		}

	}

	//filter repeating circles
	auto best_iter = circles.rbegin();
	int limit = circlesToDetect < circles.size() ? circlesToDetect : circles.size();
	for (int i = 0; i < limit;i++){
		if (best_iter == circles.rend()) continue;
		auto best = best_iter->second;
		for (auto j = ++best_iter; j != circles.rend(); j++){
			CircleData current = j->second;
			auto dist = cv::norm(best.center-current.center);
			if (cv::norm(best.center-current.center) < best.radius*2){
				circles.erase(std::next(j).base());
				j--;
			}

		}
	}

	limit = circlesToDetect < circles.size() ? circlesToDetect : circles.size();
	best_iter = circles.rbegin();
	for (int i = 0; i <limit; i++ ){
		points.push_back(best_iter->second.center);
		best_iter++;
	}

	auto sz = circles.size();
	auto iter = circles.rbegin();
	for (int i = 0; i <limit; i++ ){
		auto min = iter->second;
		iter++;
		cv::circle(write,min.center,min.radius,cv::Scalar(255),4);
	}
	//cv::namedWindow("edges"); cv::imshow("edges", mask);
	cv::namedWindow("color",cv::WINDOW_NORMAL); cv::imshow("color", write);
	cv::waitKey(1);
}