#ifndef __VISIONAPIvSTUB_H__
#define __VISIONAPIvSTUB_H__

#include <atomic>
#include <thread>
#include "APInull.h"

//API v1
class APIstub : public cNullVisionApi
{
private:
	std::atomic<bool> isSearchStarted;
	std::atomic<int> errCode;
	std::thread worker;

	void worker_thread();
public:	
	APIstub() : cNullVisionApi(2) {
		isSearchStarted.store(false);
	};

	void beginSearch();
	void endSearch();
	int checkStatus();
	TIMEVAL getNominalTime();

	void Release();
	int instrumentCount();
};


#endif