#ifndef MODBUSCLIENTINTERFACE_H_
#define MODBUSCLIENTINTERFACE_H_

#include <RSPacket.h>

class ModbusClientInterface
{
public:
	virtual int Start() = 0;
	virtual int Stop() = 0;
	virtual int SetNumberOfBalls(int) = 0;
	virtual int GetBallsData(RSPacket::BallsDataStruct& ) = 0;
	virtual char* GetVersion() = 0;
};

#endif /* MODBUSCLIENTINTERFACE_H_ */
