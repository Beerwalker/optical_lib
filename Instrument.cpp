#include "Instrument.h"
#include "CombinationEnumerator.h"
#include "DetectionHelper.h"

sVisionInstrument::sVisionInstrument(const std::vector<Point3d> &instrumentReferencePoints, VIID id){
	m_id = id;
	init(convertRefencePoints(instrumentReferencePoints));

}

sVisionInstrument::sVisionInstrument(const mat & instrumentReferencePoints, VIID id)
{
	m_id = id;
	init(instrumentReferencePoints);
}

sVisionInstrument::sVisionInstrument(const sVisionInstrument & instr)
{
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
}

mat& sVisionInstrument::convertRefencePoints(const std::vector<Point3d> &instrumentReferencePoints){
	//to mat
	m_referencePoints = zeros<mat>(instrumentReferencePoints.size(),3);
	for(std::vector<Point3d>::size_type i = 0; i != instrumentReferencePoints.size(); i++) {
		m_referencePoints(i,0) = instrumentReferencePoints[i].x;
		m_referencePoints(i,1) = instrumentReferencePoints[i].y;
		m_referencePoints(i,2) = instrumentReferencePoints[i].z;
	}

	return m_referencePoints;
}

void sVisionInstrument::init(const mat & instrumentReferencePoints){
	m_lastPointsCoord = zeros<mat>(instrumentReferencePoints.n_rows, 3);
	m_lastCoordUpdateTime = 0;
	m_bVisible = false;
	m_referencePoints = DetectionHelper::whitenData(instrumentReferencePoints);
	shape = DetectionHelper::getBoundary(m_referencePoints);
	poly_ratio = DetectionHelper::calculatePolyRatio(shape);

	//����������� ����� �� shape � reference
	has_center = DetectionHelper::matchShapeToRef(m_referencePoints, shape, shape_to_reference, center);
	//cout << m_referencePoints << endl;
	//find closest point to centroid on shape
	vec centroid = DetectionHelper::compute2DPolygonCentroid(shape);
	closest_index = DetectionHelper::getClosestToCentroid(shape,centroid);
}

int sVisionInstrument::getReferencePointCount(){
	return m_referencePoints.n_rows;
}
mat sVisionInstrument::getLastCoords()
{
	access_mutex.lock();
	mat result = m_lastPointsCoord;
	access_mutex.unlock();
	return result;
}
TIMEVAL sVisionInstrument::getLastUpdateTime()
{
	access_mutex.lock();
	TIMEVAL result = m_lastCoordUpdateTime;
	access_mutex.unlock();
	return result;
}
bool sVisionInstrument::isVisible()
{
	access_mutex.lock();
	bool result = m_bVisible;
	access_mutex.unlock();
	return result;
}
//void sVisionInstrument::setLastCoords(const mat & coords)
//{
//	read_mutex.lock();
//	write_mutex.lock();
//	m_lastPointsCoord = coords;
//	write_mutex.unlock();
//	read_mutex.unlock();
//}
//void sVisionInstrument::setLastUpdateTime(TIMEVAL time)
//{
//	read_mutex.lock();
//	write_mutex.lock();
//	m_lastCoordUpdateTime = time;
//	write_mutex.unlock();
//	read_mutex.unlock();
//}
//
//void sVisionInstrument::setVisible(bool isVisible)
//{
//	read_mutex.lock();
//	write_mutex.lock();
//	m_bVisible = isVisible;
//	write_mutex.unlock();
//	read_mutex.unlock();
//}

VIID sVisionInstrument::getID(){
	return this->m_id;
}

VIID sVisionInstrument::getID() const{
	return this->m_id;
}

sVisionInstrument& sVisionInstrument::operator=(const sVisionInstrument &instr)
{
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

void sVisionInstrument::update(const mat& coords, bool isVisible,TIMEVAL time)
{
	access_mutex.lock();
	m_lastPointsCoord = coords;
	m_lastCoordUpdateTime = time;
	m_bVisible = isVisible;
	access_mutex.unlock();
}

void sVisionInstrument::getAll(mat& coords, bool& isVisible, TIMEVAL& time)
{
	access_mutex.lock();
	coords = m_lastPointsCoord;
	isVisible = m_bVisible;
	time = m_lastCoordUpdateTime;
	access_mutex.unlock();
}

