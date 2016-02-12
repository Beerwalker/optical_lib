#include "CombinationEnumerator.h"
#include "DetectionHelper.h"
#include "Instrument.h"
#include "VisionAPI.h"
#include <mutex>

CombinationEnumerator::CombinationEnumerator(){
	points = 0;
};
bool CombinationEnumerator::enumerateCombinations(int offset, int k) {
	//full combination when k == 0
	if (k == 0) {
		mat whiten_comb = DetectionHelper::whitenData(combination);
		mat shape_contour = DetectionHelper::getBoundary(whiten_comb);
		double ratio = DetectionHelper::calculatePolyRatio(shape_contour);


		mat curr_center;
		std::vector<int> shape_to_reference(whiten_comb.n_rows);
		bool has_curr_center = DetectionHelper::matchShapeToRef(whiten_comb, shape_contour, shape_to_reference, curr_center);
		if (abs(ratio - ref_instrument->poly_ratio) < epsilon){
			if (ref_instrument->has_center){
				if (!has_curr_center) return false;
				//compare distance from shape centroid to center
				//calculate reference to current scaling factor
				double perimeter_current = DetectionHelper::calculatePerimeter(shape_contour);
				double perimeter_ref = DetectionHelper::calculatePerimeter(ref_instrument->shape);
				double scale_factor = perimeter_current / perimeter_ref;

				//compare distances to centroid
				vec ref_centroid = DetectionHelper::compute2DPolygonCentroid(ref_instrument->shape);
				vec centroid = DetectionHelper::compute2DPolygonCentroid(shape_contour);
				mat ref_center = ref_instrument->center;
				double ref_distance = sqrt(pow(ref_center(0, 0) - ref_centroid(0), 2) + pow(ref_center(0, 1) - ref_centroid(1), 2));
				double distance = sqrt(pow(curr_center(0, 0) - centroid(0), 2) + pow(curr_center(0, 1) - centroid(1), 2));
				if (abs(ref_distance*scale_factor - distance)>0.05) return false;
				
				//get point closest to centroid
				int closest_to_centroid = DetectionHelper::getClosestToCentroid(shape_contour,centroid);
				
				//obtaining resulting vector
				int ref_closest = ref_instrument->closest_index;

				for (int i = 0; i < shape_contour.n_rows-1; i++) {
					found_points.row(ref_instrument->shape_to_reference[(ref_closest + i) % (shape_contour.n_rows-1)]) = combination.row(shape_to_reference[(closest_to_centroid+i) % (shape_contour.n_rows - 1)]);
				}
				
				//записать центр
				found_points.row(ref_instrument->shape_to_reference[ref_instrument->m_referencePoints.n_rows - 1]) = combination.row(shape_to_reference[whiten_comb.n_rows - 1]);
				cout << found_points << endl;
				return true;
			}
			else{
				vec centroid = DetectionHelper::compute2DPolygonCentroid(shape_contour);
				//get point closest to centroid
				int closest_to_centroid = DetectionHelper::getClosestToCentroid(shape_contour,centroid);

				//obtaining resulting vector
				int ref_closest = ref_instrument->closest_index;

				for (int i = 0; i < shape_contour.n_rows; i++) {
					found_points.row(ref_instrument->shape_to_reference[(ref_closest + i) % (shape_contour.n_rows-1)]) = combination.row(shape_to_reference[(closest_to_centroid+i) % (shape_contour.n_rows - 1)]);
				}

				return true;
			}
		}
		else return false;
		//return abs(ratio - etalon_ratio) < epsilon ? true : false;
	}
	for (int i = offset; i <= (*points).n_rows - k; ++i) {
		combination.row(combination_row++) = (*points).row(i);
		cout << combination << endl;
		if (enumerateCombinations(i+1, k-1)) return true;
		combination_row--;
	}
	return false;
}

void CombinationEnumerator::setCombinationSize(int size){
	combination_size = size;
}

void CombinationEnumerator::setPoints(mat& allPoints){
	points = &allPoints;
}
bool CombinationEnumerator::findInstrument(sVisionInstrument* ref_instrument, double epsilon, mat& instrument_points){
	ref_instrument->access_mutex.lock();
	sVisionInstrument instrument(*ref_instrument);
	ref_instrument->access_mutex.unlock();
	this->ref_instrument = &instrument;
	this->epsilon = epsilon;
	found_points = zeros<mat>(combination_size, 3);
	combination = zeros<mat>(combination_size,3);
	combination_row = 0;
	bool result = enumerateCombinations(0,combination_size);
	if (result) instrument_points = found_points;
	return result;
}

