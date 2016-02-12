#include "APIstub.h"
#include <chrono>

using namespace std::chrono;

void APIstub::worker_thread()
{
	isSearchStarted.store(true);
	while(isSearchStarted.load()){
		for(int i =0; i < m_instruments.size(); i++){
			mat average = sum(m_instruments[i].getReferencePoints(),0);
			average = average / m_instruments[i].getReferencePoints().n_rows;

			mat transform = eye(4,4);
			//add translate
			transform(0,3) = double(rand()%10)*average(0,0)/100;
			transform(1,3) = double(rand()%10)*average(0,1)/100;
			transform(2,3) = double(rand()%10)*average(0,2)/100;
			//add scale
			double scale = 1.0f + double(rand()%10)/100;
			transform(0,0) = scale;
			transform(1,1) = scale;

			//apply transform
			mat result = m_instruments[i].getReferencePoints();

			result = result.t();
			result = resize(result, result.n_rows+1, result.n_cols);
			result.row(3).fill(1);

			for (int k = 0; k < result.n_cols ; k++){
				result.col(k) = transform * result.col(k);
			}
			result = resize(result,result.n_rows-1, result.n_cols);
			result = result.t();
			m_instruments[i].update(result,true, system_clock::now().time_since_epoch().count());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void APIstub::beginSearch()
{
	if(!isSearchStarted.load()){
		worker = std::thread([this] { this->worker_thread(); });
	}
}

void APIstub::endSearch()
{
	if (isSearchStarted.load()){
		isSearchStarted.store(false);
		worker.join();
	}
}

//1 - modbus error
int APIstub::checkStatus()
{
	return this->errCode.load();
}

void APIstub::Release()
{
	this->endSearch();
	this->~APIstub();
}

int APIstub::instrumentCount()
{
	return this->m_instruments.size();
}

TIMEVAL APIstub::getNominalTime()
{
	//TBD
	return 25;
}

