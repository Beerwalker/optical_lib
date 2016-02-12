#ifndef __APINULL_H__
#define __APINULL_H__

#include "VisionAPI.h"
#include <armadillo>
#include "Instrument.h"
#include <atomic>

using namespace arma;
	//Обобщение базовых функций. Чтобы не пришлось реализовывать в каждом потомке. 
class cNullVisionApi : public cVisionAPI
{
protected:
	cNullVisionApi(int version);
	cNullVisionApi(const cNullVisionApi & api){};
protected:
	std::vector<sVisionInstrument> m_instruments;
	std::atomic<int> balls_count;
	std::mutex  instrument_mutex;
	int m_version;
	VIID next_id;
	mat Q;

	void findAllInstruments(mat& left_cam, mat& right_cam);
	VIID genID();
public:
	int getiInstrumentCount() {
		return m_instruments.size();
	};
	void removeInstruments() {
		m_instruments.clear();
	};

	VIID createInstrument(const std::vector<Point3d> &instrumentReferencePoints);
	void removeInstrument(VIID instrId);
	int apiVersion();

	void getInstrumentData(VIID id, std::vector<Point3d>& current_points, bool& visible, TIMEVAL& update_time);
	bool isInstrumentVisible(VIID id);
	TIMEVAL getInstrumentUpdateTime(VIID id);
	std::vector<Point3d> getLastCoords(VIID id);
};




#endif