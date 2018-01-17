#pragma once

#include "ctpmd.h"
#include "ctptd.h"

#include "event_engine.hpp"
#include "strategy_engine.h"

class GatewayEngine
{
public:
	GatewayEngine()
	{
		eventEngine = new EventEngine();
		strategyEngine = new StrategyEngine(eventEngine);
		mdapi = new MdApi(strategyEngine);
		tdapi = new TdApi();
		strategyEngine->RegisterTdApi(tdapi);
	};
	~GatewayEngine()
	{
		Release();
	};
	
	void SetConnect(string tdaddr, string mdaddr, string brokerid, string investorid, string password)
	{
		tdapi->SetConnect(tdaddr, brokerid, investorid, password);
		mdapi->SetConnect(mdaddr, brokerid, investorid, password);
	};
	void Init()
	{
		tdapi->Init();
		mdapi->Init();

		//定时器10秒查询资金和持仓
		eventEngine->StartTimer(3000, std::bind(&GatewayEngine::QueryTimer, this));
		//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		//eventEngine->StopTimer();
	};
	void Join()
	{	
		tdapi->Join();
		mdapi->Join();
	};
	void Release()
	{
		mdapi->Release();
		mdapi = nullptr;

		tdapi->Release();
		tdapi = nullptr;

		delete strategyEngine;
		strategyEngine = nullptr;

		delete eventEngine;
		eventEngine = nullptr;
	}

	void QueryTimer()
	{
		tdapi->ReqQryTradingAccount();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		tdapi->ReqQryInvestorPosition();
	}

	void AddInstrument(string instrumentid)
	{
		tdapi->AddInstrument(instrumentid);
		mdapi->AddInstrument(instrumentid);
	};

	void sendOrder();

private:
	TdApi* tdapi;
	MdApi* mdapi;
	EventEngine* eventEngine;
	StrategyEngine *strategyEngine;
	//DataEngine* dataEngine;
};
