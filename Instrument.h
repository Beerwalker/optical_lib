#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include <armadillo>
#include <vector>
#include <list>
#include <mutex>
#include "VisionAPI.h"

class CombinationEnumerator;


using namespace arma;

//Внутренняя реализация инструмента. Может быть любой.
class sVisionInstrument
{
private:
	VIID m_id;
	mat m_referencePoints;
	mat m_lastPointsCoord;
	TIMEVAL m_lastCoordUpdateTime;
	bool m_bVisible;

	std::mutex access_mutex;

	mat shape;
	mat center;
	bool has_center;
	std::vector<int> shape_to_reference;
	int closest_index;
	double poly_ratio;

	mat& convertRefencePoints(const std::vector<Point3d> &instrumentReferencePoints);
	void init(const mat & instrumentReferencePoints);
public:
	friend CombinationEnumerator;
	sVisionInstrument(const std::vector<Point3d> &instrumentReferencePoints, VIID id);
	sVisionInstrument(const mat &instrumentReferencePoints, VIID id);
	sVisionInstrument(const sVisionInstrument& instr);
	sVisionInstrument& sVisionInstrument::operator =(const sVisionInstrument &);

	VIID getID();
	VIID getID() const;
	int sVisionInstrument::getReferencePointCount();

	//tmp 
	const mat& getReferencePoints(){
		return m_referencePoints;
	};

	mat getLastCoords();
	TIMEVAL getLastUpdateTime();
	bool isVisible();
	void update(const mat& coords, bool isVisible,TIMEVAL time);
	void getAll(mat& coords, bool& isVisible, TIMEVAL& time);
	//void setLastCoords(const mat& coords);
	//void setLastUpdateTime(TIMEVAL time);
	//void setVisible(bool isVisible);
	
};

#endif