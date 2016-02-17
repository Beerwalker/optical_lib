#ifndef __ENUMERATOR_H__
#define __ENUMERATOR_H__

#include <armadillo>
#include <map>
class Instrumentv2;

using namespace arma;

class CombinationEnumerator{
protected:
	mat* points;
	mat combination;
	int combination_size;
	int combination_row;
	Instrumentv2* ref_instrument;
	std::map<double,mat> candidates;
	double epsilon;

	mat found_points;

	mat min_combination;

	virtual bool enumerateCombinations(int offset, int k);
public:
	CombinationEnumerator();
	CombinationEnumerator(mat& allPoints, int k):points(&allPoints), combination_size(k){};
	void setCombinationSize(int size);

	void setPoints(mat& allPoints);
	virtual bool findInstrument(Instrumentv2* ref_instrument, double epsilon, mat& instrument_points);
};

#endif
