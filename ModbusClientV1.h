#ifndef MODBUSCLIENTV1_H_
#define MODBUSCLIENTV1_H_

//#include "ModbusClientInterface.h"
#include "ModbusHandler.h"
//#include <modbus/modbus.h>

#define verbose 0


class ModbusClientV1: public ModbusHandler
{
public:

	//write commands
	int Start(){return 0;};
	int Stop(){return 0;};
	int SetNumberOfBalls(int){return 0;};

	//read commands
	int GetBallsData(RSPacket::BallsDataStruct &){return 0;};
	char* GetVersion() {return "0.1";}

//	int sendTimePacket();

	ModbusClientV1(char*){};
	~ModbusClientV1(){};

};

#endif /* MODBUSCLIENTV1_H_ */
