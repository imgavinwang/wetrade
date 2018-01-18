#pragma once

#include <iostream>

#include "event_engine.hpp"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ctptd.h"

class StrategyEngine
{
public:
	StrategyEngine(EventEngine* eventEngine)
	{
		eventEngine_ = eventEngine;
		count = 0;
	};
	~StrategyEngine()
	{

	};
	
	void RegisterTdApi(TdApi* tdapi) 
	{
		tdapi_ = tdapi;
	};

	void OnTick(CThostFtdcDepthMarketDataField tick)
	{
		cout << "StrategyEngine::OnTick:["  
             << tick.TradingDay << " "  
             << tick.UpdateTime << " "  
             << tick.UpdateMillisec << "] "  
             << tick.InstrumentID << " "  
             << tick.LastPrice << endl;

        count++;
        if(count > 10 && tick.LastPrice > 0) {
        	tdapi_->ReqOrderInsert((char*)tick.InstrumentID, tick.LastPrice, 1, THOST_FTDC_D_Sell);
        	count = 0;
        }
	};
	
private:
	EventEngine* eventEngine_;
	TdApi* tdapi_;
	int count;
};
