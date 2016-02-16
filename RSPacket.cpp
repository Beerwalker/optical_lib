/*
 * RC_Packet.cpp
 *
 *  Created on: Feb 5, 2016
 *      Author: root
 */

#include "RSPacket.h"
#include <algorithm>

using namespace RSPacket;


void RSPacket::setPacketTime(struct PacketStruct &p, struct timespec time)
{
	p.header.time.sec = time.tv_sec;
	p.header.time.nanosec = time.tv_nsec;
}

// Function for printing the point
void RSPacket::printPoint(struct PointStruct & p)
{
	printf("[%d, %d] ", p.X, p.Y);
}

// Function for printing the point array
void RSPacket::printPointArr(struct PointArrayStruct & pointArray)
{
	printf("PointArray size[%u]", pointArray.size);
	printf("\t");
	for (int i = 0; i < pointArray.size; ++i)
	{
		RSPacket::printPoint(pointArray.array[i]);
	}
	printf("\n");
}

void RSPacket::printTime(struct TimeStruct & time)
{
	printf("Time: sec [%x], nanosec [%x]\n", time.sec, time.nanosec);
}

// Function for printing the contain of the packet
void RSPacket::printPacket(struct PacketStruct & p)
{
	printf("Packet start\n\t");
	RSPacket::printTime(p.header.time);
	printf("\tCmd [%x] Error Code: [%x]\n", p.header.cmd, p.header.errorcode);
	//%u for unsigned integer
	//	printPointArr(&p->);
	printf("\n");
	printf("Packet end\n");

}

void RSPacket::printBallsData(struct BallsDataStruct & bd)
{
	printf("Image size [%dx%d]\n", bd.imageSize[0], bd.imageSize[1]);
	printPointArr(bd.pointsArrays[0]);
	printPointArr(bd.pointsArrays[1]);
}

void RSPacket::swapWords(uint8_t * buffer, uint8_t sizeInWords)
{
	for (int i = 0; i < sizeInWords; i++)
	{
		std::swap(buffer[i * 2], buffer[i * 2 + 1]);
	}
}
