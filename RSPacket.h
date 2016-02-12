#ifndef SRC_RC_PACKET_H_
#define SRC_RC_PACKET_H_

#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define RS_MAX_POINTS 			2
#define RS_MAX_PACKET_SIZE 		100 // In uint16_t (words)
#define RS_PACKET_HEADER_SIZE	5	// Size of packet header

#define RS_CMD_NTD 				0	// Nothing to do (probably unused)
#define RS_CMD_SETTIME 			1	// Sync time on the optical station
//#define RS_CMD_WRITEDATA 		2	// Write some data to optical station
//#define RS_CMD_READDATA 		3	// Read data from optical station
#define RS_CMD_SET_NUM_OF_BALLS 4	// Setting up bassert(size % 2 == 0);alls number
#define RS_CMD_PING				5	// Ping server
#define RS_CMD_START			6	//
#define RS_CMD_STOP				7	//
#define RS_CMD_GET_BALLS_DATA	8	//

#define MODBUS_WR 				0x10	// Modbus protocol oficial command FORCE MULTIPLE REGISTERS
#define MODBUS_RD 				0x3	// Modbus protocol oficial command READ HOLDING REGISTERS

//specs
//////https://www.honeywellprocess.com/library/support/Public/Documents/51-52-25-66.pdf

#define DEBUG

//class RS_Packet
//{
//public:

namespace RSPacket
{

// Structure, represents coordinates of each ball
struct PointStruct // size = 2 words
{
	uint16_t X;
	uint16_t Y;
};

struct PointArrayStruct // size = 1 + 2 * MAX_POINTS = 41 words
{
	uint16_t size;
	struct PointStruct array[RS_MAX_POINTS];
};

struct TimeStruct // size = 4 words
{
	uint32_t sec;
//	uint8_t sec[4];
	uint32_t nanosec;
//	uint8_t nanosec[4];
};

struct BallsDataStruct // size = 2 + PointArrayStruct * 2 = 2 + 82 = 84 words
{
	uint16_t imageSize[2];
	struct PointArrayStruct pointsArrays[2];
};

struct PacketHeader // size = 5 words
{
	struct TimeStruct time;		//4 words
	uint8_t cmd;				//1 byte
	uint8_t errorcode;			//1 byte
};

//Packet struct for all cases
struct PacketStruct
{
	struct PacketHeader header;			 // size = 5 word
	uint16_t data[RS_MAX_PACKET_SIZE - RS_PACKET_HEADER_SIZE]; // size of data =  MAX_PACKET_LENGTH - 5 = 95 words
};

void setPacketTime(struct PacketStruct &p, struct timespec time);

void swapWords(uint8_t *, uint8_t);

// Function for printing the point
void printPoint(struct PointStruct &);

// Function for printing the point array
void printPointArr(struct PointArrayStruct &);

void printBallsData(struct BallsDataStruct &);

void printTime(struct TimeStruct &);

// Function for printing the contain of the packet
void printPacket(struct PacketStruct &);

}
//};

#endif /* SRC_RC_PACKET_H_ */
