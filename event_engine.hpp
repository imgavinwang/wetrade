#pragma once

#include <iostream>
#include <functional>
#include <boost/any.hpp>

#include "tsqueue.hpp"
#include "tstimer.hpp"

using namespace std;

class EventEngine
{
/*
public:
	void Start();
	void Stop();
	void Register();
	void UnRegister();
	void Put();

private:
	void _Run();
	void _Process();
	void _OnTimer();
	void _RunTimer();
*/
public:
	EventEngine()
	{
		t = new TsTimer();
	};
	~EventEngine()
	{

	};

	void StartTimer(int interval, std::function<void()> task) 
	{
		cout << "EventEngine::StartTimer" << endl;
		auto f = std::bind(&TsTimer::StartTimer, t, placeholders::_1, placeholders::_2);
		std::thread(f, interval, task).detach();
	};

	void Stop()
	{
		cout << "EventEngine::Stop" << endl;
		t->Expire();
	}

private:
	bool _switch;
	TsTimer* t;
	//TsQueue<any> _queue;
};
