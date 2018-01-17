#include "gateway_engine.h"

int main()
{
	GatewayEngine* gateway = new GatewayEngine();
	gateway->SetConnect("tcp://180.168.146.187:10000", "tcp://180.168.212.228:41213", 
		"9999", "051344", "thankgod");
	gateway->AddInstrument("IF1801");

	//启动交易、行情线程
	gateway->Init();
	gateway->Join();

	delete gateway;
	gateway = nullptr;

	return 0;
}