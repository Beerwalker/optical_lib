#ifndef __ENUMERATOR_H__
#define __ENUMERATOR_H__

#include <armadillo>
class sVisionInstrument;

using namespace arma;

class CombinationEnumerator{
private:
	mat* points;
	mat combination;
	int combination_size;
	int combination_row;
	sVisionInstrument* ref_instrument;
	double epsilon;

	mat found_points;

	bool enumerateCombinations(int offset, int k);
public:
	CombinationEnumerator();
	CombinationEnumerator(mat& allPoints, int k):points(&allPoints), combination_size(k){};
	void setCombinationSize(int size);

	void setPoints(mat& allPoints);
	bool findInstrument(sVisionInstrument* ref_instrument, double epsilon, mat& instrument_points);
};

#endif