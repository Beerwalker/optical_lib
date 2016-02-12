#include "APInull.h"
#include <chrono>
#include "CombinationEnumerator.h"

using namespace std::chrono;


cNullVisionApi::cNullVisionApi(int version)
{
	Q = zeros<mat>(4,4);
	Q.ones();
	m_version = version;
	next_id = 0;
}

void cNullVisionApi::findAllInstruments(mat& left_cam, mat& right_cam) {
	//copy instr vector 
	instrument_mutex.lock();
	std::vector<sVisionInstrument> temp_instruments(m_instruments);
	instrument_mutex.unlock();
	int n_instrumnets = temp_instruments.size();
	CombinationEnumerator left_enumerator(left_cam, 5);
	CombinationEnumerator right_enumerator(right_cam, 5);
	mat result_left, result_right;
	TIMEVAL current_time = system_clock::now().time_since_epoch().count();
	for (int i = 0; i < n_instrumnets; i++) {
		left_enumerator.setCombinationSize(temp_instruments[i].getReferencePointCount());
		bool isLeftFound = left_enumerator.findInstrument(&temp_instruments[i], 2, result_left);

		right_enumerator.setCombinationSize(temp_instruments[i].getReferencePointCount());
		bool isRightFound = right_enumerator.findInstrument(&temp_instruments[i], 2, result_right);

		mat last_coords;
		if (isLeftFound && isRightFound) {
			for(int j =0; j < result_right.n_rows; j++){
				double d = result_right(j,0) - result_left(j,0);
				last_coords(j,0) = result_left(j,0) * Q(0,0) + Q(0,3);
				last_coords(j,1) = result_left(j,1) * Q(1,1) + Q(1,3);
				last_coords(j,2) = Q(2,3);
				double W = d*Q(3,2) + Q(3,3);
				last_coords(j,0) = last_coords(j,0) / W;
				last_coords(j,1) = last_coords(j,1) / W;
				last_coords(j,2) = last_coords(j,2) / W;

			}
			//write updated instrument data
			temp_instruments[i].update(last_coords,true, current_time);
		}
		else {
			//set not visible
			temp_instruments[i].update(temp_instruments[i].getLastCoords(),false, temp_instruments[i].getLastUpdateTime());
		}
	}

	//store updated data 
	instrument_mutex.lock();
	m_instruments = temp_instruments;
	instrument_mutex.unlock();
}

VIID cNullVisionApi::genID()
{
	return next_id++;
}


VIID cNullVisionApi::createInstrument(const std::vector<Point3d>& instrumentReferencePoints)
{
	sVisionInstrument instrument(instrumentReferencePoints, genID());
	m_instruments.push_back(instrument);
	balls_count.store(balls_count.load() + instrumentReferencePoints.size());
	return instrument.getID();
}

void cNullVisionApi::removeInstrument(VIID instrId) {
	for (std::vector<sVisionInstrument>::size_type i = 0; i != m_instruments.size(); i++) {
		if (m_instruments[i].getID() == instrId) {
			m_instruments.erase(m_instruments.begin() + i);
			balls_count.store(balls_count.load() - m_instruments[i].getReferencePointCount());
			return;
		}
	}
}
int cNullVisionApi::apiVersion()
{
	return m_version;
}
bool cNullVisionApi::isInstrumentVisible(VIID id)
{
	for (std::vector<sVisionInstrument>::size_type i = 0; i != m_instruments.size(); i++) {
		if (m_instruments[i].getID() == id) {
			return m_instruments[i].isVisible();
		}
	}
	return false;
}
TIMEVAL cNullVisionApi::getInstrumentUpdateTime(VIID id)
{
	for (std::vector<sVisionInstrument>::size_type i = 0; i != m_instruments.size(); i++) {
		if (m_instruments[i].getID() == id) {
			return m_instruments[i].getLastUpdateTime();
		}
	}
	return NULL;
}
std::vector<Point3d> cNullVisionApi::getLastCoords(VIID id)
{
	for (std::vector<sVisionInstrument>::size_type i = 0; i != m_instruments.size(); i++) {
		if (m_instruments[i].getID() == id) {
			mat coords = m_instruments[i].getLastCoords();
			std::vector<Point3d> result(coords.n_rows);
			for (int j = 0; j != coords.n_rows; j++) {
				result[j].x = coords(i, 0);
				result[j].y = coords(i, 1);
				result[j].z = coords(i, 2);
			}
			return result;
		}
	}
	std::vector<Point3d> empty;
	return empty;
}

void cNullVisionApi::getInstrumentData(VIID id, std::vector<Point3d>& current_points, bool& visible, TIMEVAL& update_time)
{
	for (std::vector<sVisionInstrument>::size_type i = 0; i != m_instruments.size(); i++) {
		if (m_instruments[i].getID() == id) {
			mat coords;
			m_instruments[i].getAll(coords, visible, update_time);
			current_points.resize(coords.n_rows);
			for (int j = 0; j != coords.n_rows; j++) {
				current_points[j].x = coords(i, 0);
				current_points[j].y = coords(i, 1);
				current_points[j].z = coords(i, 2);
			}
		}
	}

	current_points.resize(0);
	visible = false;
	update_time = NULL;
}

;