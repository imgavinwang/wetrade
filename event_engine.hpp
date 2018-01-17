#pragma once

#include <iostream>
#include <functional>
#include <boost/any.hpp>

#include "tsqueue.hpp"
#include "tstimer.hpp"

using namespace std;

class EventEngine
{

public:
	EventEngine()
	{
		t = new TsTimer();
	};
	~EventEngine()
	{

	};

	void Start();
	void Stop();
	void Register();
	void UnRegister();
	void Put();

	void StartTimer(int interval, std::function<void()> task) 
	{
		cout << "EventEngine::StartTimer" << endl;
		auto f = std::bind(&TsTimer::StartTimer, t, placeholders::_1, placeholders::_2);
		std::thread(f, interval, task).detach();
	};

	void StopTimer()
	{
		cout << "EventEngine::StopTimer" << endl;
		t->Expire();
	}

private:
	bool _switch;
	TsTimer* t;
	//TsQueue<any> _queue;
};
