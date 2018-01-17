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
		mdapi = new MdApi(eventEngine);
		tdapi = new TdApi(eventEngine);
		strategyEngine = new StrategyEngine(eventEngine);
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
		eventEngine->StartTimer(10000, std::bind(&GatewayEngine::QueryTimer, this));
		//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		//eventEngine->Stop();
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

int main()
{
	GatewayEngine* gateway = new GatewayEngine();
	gateway->SetConnect("tcp://180.168.146.187:10000", "tcp://180.168.212.228:41213", 
		"9999", "051344", "test");
	gateway->AddInstrument("IF1801");

	//启动交易、行情线程
	gateway->Init();
	gateway->Join();

	delete gateway;
	gateway = nullptr;

	return 0;
}
