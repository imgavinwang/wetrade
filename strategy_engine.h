#pragma once

#include <iostream>

class StrategyEngine
{
public:
	StrategyEngine(EventEngine* eventEngine)
	{
		eventEngine_ = eventEngine;
	};
	~StrategyEngine()
	{

	};
	
private:
	EventEngine* eventEngine_;
};
