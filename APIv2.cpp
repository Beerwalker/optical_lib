#include "APIv2.h"
#include <chrono>

void APIv2::beginSearch()
{
	if(!isSearchStarted.load()){
		worker = std::thread([this] { this->worker_thread(); });
	}
}

void APIv2::endSearch()
{
	if (isSearchStarted.load()){
		isSearchStarted.store(false);
		worker.join();
	}
}

//1 - modbus error
int APIv2::checkStatus()
{
	return this->errCode.load();
}

void APIv2::worker_thread()
{
	isSearchStarted.store(true);
	int ret_code = modbus->Start();
	if (ret_code < 0){
		errCode.store(ret_code);
		isSearchStarted.store(false);
		return;
	}
	RSPacket::BallsDataStruct balls;
	while(isSearchStarted.load()){
		ret_code = modbus->SetNumberOfBalls(10/*balls_count.load()*/);
		if (ret_code < 0){
			//do shit
			errCode.store(ret_code);
			isSearchStarted.store(false);
			continue;
		}
		ret_code = modbus->GetBallsData(balls);
		if (ret_code < 0){
			//do shit
			errCode.store(ret_code);
			isSearchStarted.store(false);
			continue;
		}
		//convert to mat
		mat left = zeros<mat>(balls.pointsArrays[0].size,3);
		for(int i =0; i < left.n_rows; i++){
			left(i,0) = balls.pointsArrays[0].array[i].X;
			left(i,1) = balls.pointsArrays[0].array[i].Y;
		}
		mat right = zeros<mat>(balls.pointsArrays[1].size,3);
		for(int i =0; i < right.n_rows; i++){
			right(i,0) = balls.pointsArrays[1].array[i].X;
			right(i,1) = balls.pointsArrays[1].array[i].Y;
		}
		findAllInstruments(left, right);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}

	//exit
	ret_code = modbus->Stop();
	if(ret_code < 0){
		errCode.store(ret_code);
	}
	//isSearchStarted.store(false);
}

void APIv2::Release()
{
	//TBD
}

int APIv2::instrumentCount()
{
	return this->m_instruments.size();
}

TIMEVAL APIv2::getNominalTime()
{
	//TBD
	return 25;
}
