#pragma once
#include "Instrument.h"

class Instrumentv2 : public sVisionInstrument
{
protected:
	mat distances;
	mat real_distances;
	virtual void init(const mat & instrumentReferencePoints) override;
public:
	friend CombinationEnumeratorv2;
	Instrumentv2(const std::vector<Point3d> &instrumentReferencePoints, VIID id);
	Instrumentv2(const mat &instrumentReferencePoints, VIID id);
	Instrumentv2(const Instrumentv2& instr);
	~Instrumentv2();

	Instrumentv2& operator =(const Instrumentv2 &);

	double compareInstruments(mat distances);
};

