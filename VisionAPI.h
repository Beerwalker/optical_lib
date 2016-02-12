#ifndef __VISIONAPI_H__
#define __VISIONAPI_H__

#include <vector>
#include <list>
#include "compile_support.h"

typedef unsigned int TIMEVAL; //temporary - для обозначения времени. точность не меньше милисекунды.
typedef int VIID;//vision instrument id
typedef struct {double x; double y; double z;} Point3d;//temporary - use your vector3d here

class cVisionAPI{
public:
	virtual VIID createInstrument(const std::vector<Point3d> &instrumentReferencePoints) = 0; 

	virtual void removeInstrument(VIID instrId) = 0;
	virtual int instrumentCount() = 0;
	virtual void removeInstruments() = 0;

	virtual int apiVersion() = 0;

	virtual void beginSearch()=0;
	virtual void endSearch()=0;
	virtual int checkStatus() = 0;//что то для получения текущего состояния системы (+получения кода ошибки и т.п.)

	virtual void getInstrumentData(VIID id, std::vector<Point3d>& current_points, bool& visible, TIMEVAL& update_time)=0;
	virtual TIMEVAL getNominalTime()=0;

	/*-----------------------------------------------*/
	/*DEPRECARED*/
	virtual bool isInstrumentVisible(VIID id)=0;
	virtual TIMEVAL getInstrumentUpdateTime(VIID id) = 0;
	virtual std::vector<Point3d> getLastCoords(VIID id)=0;
	/*-----------------------------------------------*/
	
	virtual void Release()=0;
};

extern "C" API_PUBLIC cVisionAPI*  GetVisionAPI();
#endif