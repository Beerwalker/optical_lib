/*
 * ModbusClientV1.cpp
 *
 *  Created on: Feb 5, 2016
 *      Author: root
 */

#include "ModbusClientV1.h"
#include <modbus/modbus.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <string.h>

//using namespace RSPacket;
#include "RSPacket.h"

ModbusClientV1::ModbusClientV1(char* device)
{
	ctx = modbus_new_rtu(device, 115200, 'E', 8, 1); // Port_name, Speed, Parity, Data_bits, Stop_bits
	if (ctx == NULL)
	{
		fprintf(stderr, "Unable to create the libmodbus context\n");
		return;
	}

	if (verbose > 4)
	{
		modbus_set_debug(ctx, 1);
	}

	int rc = modbus_set_slave(ctx, 1);

	// Connecting to existing modbus socket
	rc = modbus_connect(ctx);
	if (rc == -1)
	{
		fprintf(stderr, "Connection failed [%s]\n", modbus_strerror(errno));
		modbus_free(ctx);
		return;
	}

	// Change bytes and response timeouts for connections
	change_byte_timeout(0, 10000); /* 10 ms */
	change_response_timeout(0, 100000); /* 100 ms */

	isInitialized = true;
}

ModbusClientV1::~ModbusClientV1()
{
	modbus_close(ctx);
	modbus_free(ctx);
}

int ModbusClientV1::Start()
{
	RSPacket::PacketStruct p =
	{ 0 };
	fillTheCommonPacketHeader(p, RS_CMD_START);
	if (writePacket(&p, RS_PACKET_HEADER_SIZE) == RS_PACKET_HEADER_SIZE)
		return 0;
	return -1;
}

int ModbusClientV1::Stop()
{
	RSPacket::PacketStruct p =
	{ 0 };
	fillTheCommonPacketHeader(p, RS_CMD_STOP);
	if (writePacket(&p, RS_PACKET_HEADER_SIZE) == RS_PACKET_HEADER_SIZE)
		return 0;
	return -1;
}

int ModbusClientV1::SetNumberOfBalls(int numOfBalls)
{
	RSPacket::PacketStruct p =
	{ 0 };
	fillTheCommonPacketHeader(p, RS_CMD_SET_NUM_OF_BALLS);
	p.data[0] = numOfBalls;
	if (writePacket(&p, RS_PACKET_HEADER_SIZE + 2)
			== (RS_PACKET_HEADER_SIZE + 2))
		return 0;
	return -1;
}

int ModbusClientV1::GetBallsData(RSPacket::BallsDataStruct & ballsData)
{

	RSPacket::PacketStruct p =
	{ 0 };

	fillTheCommonPacketHeader(p, RS_CMD_GET_BALLS_DATA);
	if (readPacket(&p) != -1)
	{
//		RSPacket::swapWords()
		memcpy(&ballsData, &p.data, sizeof(RSPacket::BallsDataStruct));
		return 0;
	}

	return -1;

}

int ModbusClientV1::writePacket(RSPacket::PacketStruct * p, int size)
{
	if (verbose > 2)
	{
		printf("SENDING PACKET\n");
	}

	if (verbose > 3)
		printPacket(*p);

	if (!isInitialized)
	{
		fprintf(stderr, "writePacket failed! Modbus is not initialized!\n");
		return -1;
	}

	int rc = modbus_write_registers(ctx, 0, size, (uint16_t*) p);
	if (rc == -1)
	{
		fprintf(stderr, "Failed to write BallsCount data [%s]\n",
				modbus_strerror(errno));
	}
	return rc;
}

int ModbusClientV1::readPacket(RSPacket::PacketStruct * p)
{

	if (verbose > 2)
	{
		printf("READING PACKET\n");
	}

	uint16_t buffer[RS_MAX_PACKET_SIZE] =
	{ 0 };

	if (!isInitialized)
	{
		fprintf(stderr, "readPacket failed! Modbus is not initialized!\n");
		return -1;
	}

	int rc = modbus_read_registers(ctx, 0, RS_MAX_PACKET_SIZE, /*buffer*/
	(uint16_t*) p); // Read all registers
	if (rc == -1)
	{
		fprintf(stderr, "Failed to read data [%s]\n", modbus_strerror(errno));
	}
	else
	{
//		uint8_t * b = (uint8_t*)b;
//		uint8_t * data = &b[3];
//		uint8_t size = b[2];
//		assert(size % 2 == 0);
//		RSPacket::swapWords(data, size/2);
		if (verbose > 3)
			printPacket(*p);
	}
	return rc;
}
// Function for changing Modbus bytes timeout
void ModbusClientV1::change_byte_timeout(int sec, int usec)
{
	struct timeval t;

	modbus_get_byte_timeout(ctx, &t);
	printTimeTV("Initial byte timeout", &t);
	t.tv_sec = sec;
	t.tv_usec = usec;
	modbus_set_byte_timeout(ctx, &t);
	modbus_get_byte_timeout(ctx, &t);
	printTimeTV("New byte timeout", &t);
}
// Function for changing Modbus response timeout
void ModbusClientV1::change_response_timeout(int sec, int usec)
{
	struct timeval t;

	modbus_get_response_timeout(ctx, &t);
	printTimeTV("Initial response timeout", &t);
	t.tv_sec = sec;
	t.tv_usec = usec;
	modbus_set_response_timeout(ctx, &t);
	modbus_get_response_timeout(ctx, &t);
	printTimeTV("New response timeout", &t);
}

void readSystemTime(struct timespec * p)
{
	clock_gettime(CLOCK_REALTIME, p);
}

//void writeSystemTime(struct timespec * p)
//{
//	assert(!clock_settime(CLOCK_REALTIME, p));
//}
// Function for filling the packet structure
void ModbusClientV1::fillTheCommonPacketHeader(RSPacket::PacketStruct & p,
		int cmd)
{
	struct timespec time;
	time.tv_sec = 0x55;
	time.tv_nsec = 0x66;

	p.header.errorcode = 0;
	p.header.cmd = cmd;
	readSystemTime(&time);
	setPacketTime(p, time);
}

//int ModbusClientV1::sendTimePacket()
//{
//	RSPacket::PacketStruct p =
//	{ 0 };
//	fillTheCommonPacketHeader(p, RS_CMD_SETTIME);
//	int rc = writePacket(&p, RS_PACKET_HEADER_SIZE);
//	return rc;
//}
// Function for printing time of the system
void ModbusClientV1::printTimeTV(char * msg, struct timeval * p)
{
	printf("%s: %u s, %lu us\n", msg, p->tv_sec, p->tv_usec);
}

