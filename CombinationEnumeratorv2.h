#pragma once
#include "CombinationEnumerator.h"
#include <map>

class Instrumentv2;

class CombinationEnumeratorv2 : public CombinationEnumerator
{
protected:
	Instrumentv2* ref_instrumentv2;
	virtual bool enumerateCombinations(int offset, int k) override;
	
public:
	CombinationEnumeratorv2(void);
	CombinationEnumeratorv2(mat& allPoints, int k):CombinationEnumerator(allPoints,k){};
	~CombinationEnumeratorv2(void);
	bool findInstrument(Instrumentv2* ref_instrument, double epsilon, std::map<double,mat>& instrument_points);
};

