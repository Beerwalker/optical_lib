#include "Instrumentv2.h"
#include "DetectionHelper.h"


Instrumentv2::~Instrumentv2()
{
}

void Instrumentv2::init(const mat & instrumentReferencePoints)
{
	//calculate distance to center from each point
	for(int i = 0; i < shape.n_rows-1; i++){
		distances(i,0) = sqrt(pow(center(0,0) - shape(i,0),2) + pow(center(0,1) - shape(i,1),2));
	}
}

Instrumentv2& Instrumentv2::operator=(const Instrumentv2 &instr)
{
	distances = instr.distances;
	m_id = instr.m_id;
	m_referencePoints = instr.m_referencePoints;
	m_lastPointsCoord = instr.m_lastPointsCoord;
	m_lastCoordUpdateTime = instr.m_lastCoordUpdateTime;
	m_bVisible = instr.m_bVisible;

	shape = instr.shape;
	center = instr.center;
	shape_to_reference = instr.shape_to_reference;
	closest_index = instr.closest_index;
	poly_ratio = instr.poly_ratio;
	has_center = instr.has_center;
	return *this;
}

Instrumentv2::Instrumentv2(const std::vector<Point3d> &instrumentReferencePoints, VIID id) : sVisionInstrument(instrumentReferencePoints,id)
{
	distances = zeros<mat>(shape.n_rows-1,1);
	//calculate distance to center from each point
	for(int i = 0; i < shape.n_rows-1; i++){
		distances(i,0) = sqrt(pow(center(0,0) - shape(i,0),2) + pow(center(0,1) - shape(i,1),2));
	}
}

Instrumentv2::Instrumentv2(const mat &instrumentReferencePoints, VIID id) : sVisionInstrument(instrumentReferencePoints, id)
{
	distances = zeros<mat>(shape.n_rows-1,1);
	//calculate distance to center from each point
	for(int i = 0; i < shape.n_rows-1; i++){
		distances(i,0) = sqrt(pow(center(0,0) - shape(i,0),2) + pow(center(0,1) - shape(i,1),2));
	}
}

Instrumentv2::Instrumentv2(const Instrumentv2& instr) : sVisionInstrument(instr)
{
	distances = instr.distances;
}
