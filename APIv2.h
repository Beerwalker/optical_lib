#pragma once


#include <atomic>
#include <thread>
#include "APInull.h"
#include "ModbusClientV1.h"


class APIv2 : public cNullVisionApi
{
private:
	ModbusClientV1* modbus;
	std::atomic<bool> isSearchStarted;
	std::atomic<int> errCode;
	std::thread worker;

	void worker_thread();
public:	
	APIv2() : cNullVisionApi(1) {
		modbus = new ModbusClientV1("/dev/ttyM0");
		isSearchStarted.store(false);
	};

	void beginSearch() override;
	void endSearch() override;
	int checkStatus() override;
	TIMEVAL getNominalTime() override;

	void Release() override;
	int instrumentCount() override;
};

