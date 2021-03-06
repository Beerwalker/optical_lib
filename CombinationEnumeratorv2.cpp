﻿#include "CombinationEnumeratorv2.h"
#include "DetectionHelper.h"
#include "Instrumentv2.h"
#include "VisionAPI.h"
#include <mutex>

CombinationEnumeratorv2::CombinationEnumeratorv2(void)
{
}


CombinationEnumeratorv2::~CombinationEnumeratorv2(void)
{

}

bool CombinationEnumeratorv2::enumerateCombinations(int offset, int k)
{
	//full combination when k == 0
	if (k == 0) {
		//cout << combination << endl;
		mat whiten_comb = DetectionHelper::whitenData(combination);
		mat shape_contour = DetectionHelper::getBoundary(whiten_comb);
		double ratio = DetectionHelper::calculatePolyRatio(shape_contour);


		mat curr_center;
		std::vector<int> shape_to_reference(whiten_comb.n_rows);
		bool has_curr_center = DetectionHelper::matchShapeToRef(whiten_comb, shape_contour, shape_to_reference, curr_center);
		if (abs(ratio - ref_instrumentv2->poly_ratio)/ref_instrumentv2->poly_ratio < epsilon && shape_contour.n_rows == ref_instrumentv2->shape.n_rows){
			//cout << abs(ratio - ref_instrumentv2->poly_ratio)/ref_instrumentv2->poly_ratio << endl;
			//cout << combination << endl;
			if (ref_instrumentv2->has_center){
				if (!has_curr_center) return false;
				//get point closest to centroid
				vec centroid = DetectionHelper::compute2DPolygonCentroid(shape_contour);
				int closest_to_centroid = DetectionHelper::getClosestToCentroid(shape_contour,centroid);
				//-------------------------------------------
				//check distance from center
				mat current_distances = zeros<mat>(shape_contour.n_rows-1,1);
				//calculate distance to center from each point
				for(int i = 0; i < shape_contour.n_rows-1; i++){
					current_distances(i,0) = sqrt(pow(curr_center(0,0) - shape_contour(i,0),2) + pow(curr_center(0,1) - shape_contour(i,1),2));
				}

				for(int k = 0; k < shape_contour.n_rows-1; k++){
					double difference = abs(ref_instrumentv2->distances((ref_instrumentv2->closest_index + k) % (shape_contour.n_rows-1),0) - current_distances((closest_to_centroid+k) % (shape_contour.n_rows - 1),0));
					if (difference > 0.1){
						return false;
					}
				}

				//-------------------------------------------
				//cout << combination << endl;
				//compare distance from shape centroid to center
				//calculate reference to current scaling factor
				double perimeter_current = DetectionHelper::calculatePerimeter(shape_contour);
				double perimeter_ref = DetectionHelper::calculatePerimeter(ref_instrumentv2->shape);
				double scale_factor = perimeter_current / perimeter_ref;

				//compare distances to centroid
				vec ref_centroid = DetectionHelper::compute2DPolygonCentroid(ref_instrumentv2->shape);

				mat ref_center = ref_instrumentv2->center;
				double ref_distance = sqrt(pow(ref_center(0, 0) - ref_centroid(0), 2) + pow(ref_center(0, 1) - ref_centroid(1), 2));
				double distance = sqrt(pow(curr_center(0, 0) - centroid(0), 2) + pow(curr_center(0, 1) - centroid(1), 2));
				if (abs(ref_distance*scale_factor - distance)>0.1) return false;

				//obtaining resulting vector
				int ref_closest = ref_instrumentv2->closest_index;

				for (int i = 0; i < shape_contour.n_rows-1; i++) {
					found_points.row(ref_instrumentv2->shape_to_reference[(ref_closest + i) % (shape_contour.n_rows-1)]) = combination.row(shape_to_reference[(closest_to_centroid+i) % (shape_contour.n_rows - 1)]);
				}

				//�������� �����
				found_points.row(ref_instrumentv2->shape_to_reference[ref_instrumentv2->m_referencePoints.n_rows - 1]) = combination.row(shape_to_reference[whiten_comb.n_rows - 1]);
				candidates.insert(std::pair<double, mat>(abs(ref_distance*scale_factor - distance),found_points));
				//cout << found_points << endl;
				return false;
			}
			else{
				vec centroid = DetectionHelper::compute2DPolygonCentroid(shape_contour);
				//get point closest to centroid
				int closest_to_centroid = DetectionHelper::getClosestToCentroid(shape_contour,centroid);

				//obtaining resulting vector
				int ref_closest = ref_instrumentv2->closest_index;

				for (int i = 0; i < shape_contour.n_rows; i++) {
					found_points.row(ref_instrumentv2->shape_to_reference[(ref_closest + i) % (shape_contour.n_rows-1)]) = combination.row(shape_to_reference[(closest_to_centroid+i) % (shape_contour.n_rows - 1)]);
				}
				candidates.insert(std::pair<double, mat>(ratio,found_points));
				return false;
			}
		}
		else return false;
		//return abs(ratio - etalon_ratio) < epsilon ? true : false;
	}
	for (int i = offset; i <= (*points).n_rows - k; ++i) {
		combination.row(combination_row++) = (*points).row(i);
		//cout << combination << endl;
		if (enumerateCombinations(i+1, k-1)) return true;
		combination_row--;
	}
	return false;
}

bool CombinationEnumeratorv2::findInstrument(Instrumentv2* ref_instrumentv2, double epsilon, map<double,mat>& instrument_points)
{
	if (points->n_rows < combination_size) return false;
	candidates.clear();
	ref_instrumentv2->access_mutex.lock();
	Instrumentv2 instrument(*ref_instrumentv2);
	ref_instrumentv2->access_mutex.unlock();
	this->ref_instrumentv2 = &instrument;
	this->epsilon = epsilon;
	found_points = zeros<mat>(combination_size, 3);
	combination = zeros<mat>(combination_size,3);
	combination_row = 0;
	enumerateCombinations(0,combination_size);
	bool isFound = false;
	//filter found points
	if (candidates.size() > 0){
		isFound = true;
		instrument_points = candidates;
		//auto best_iter = candidates.begin();
		//instrument_points = best_iter->second;
		
		//check distances

	}

	//if (result) instrument_points = found_points;
	return isFound;
}
