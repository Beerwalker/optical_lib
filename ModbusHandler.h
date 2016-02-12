#ifndef __RSPACKET_H__
#define __RSPACKET_H__

#include <armadillo>
#include "RSPacket.h"

class ModbusHandler{

public:
	//ModbusHandler();
	int Start(){return 0;};
	int Stop(){return 0;};
	int SetNumberOfBalls(int){return 0;};
	int GetBallsData(RSPacket::BallsDataStruct& balls){return 0;};
	char* GetVersion(){return 0;};
};

#endif