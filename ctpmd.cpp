#include "ctpmd.h"

void MdApi::ReqUserLogin()
{
	if(!beConnected) {
		cout << "MdApi::beConnected false!" << endl;
		return;
	}

	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _brokerid.c_str());
	strcpy(req.UserID, _investorid.c_str());
	strcpy(req.Password, _password.c_str());
	int iResult = api->ReqUserLogin(&req, ++iRequestID);
	cout << "--Do MdApi::ReqUserLogin: investorid[" << _investorid <<"] " << ((iResult == 0) ? "ok" : "failed") << endl;
}

void MdApi::AddInstrument(string instrumentid)
{
	_instrumentid = instrumentid;
	cout << "MdApi::AddInstrument: instrumentid["  << _instrumentid << "]" << endl;
}

void MdApi::SubscribeMarketData()
{
	char* instruments[1] = {};
	instruments[0] = (char *)_instrumentid.c_str();  
	int iResult = api->SubscribeMarketData(instruments, 1);
	cout << "--Do MdApi::SubscribeMarketData: instrumentid[" << _instrumentid << "] " << ((iResult == 0) ? "ok" : "failed") << endl;
}

/*---------------------------------------------*/

void MdApi::OnFrontConnected()
{
	cout << "MdApi::OnFrontConnected" << endl;
	beConnected = true;

	ReqUserLogin();
}

void MdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "MdApi::OnRspUserLogin" << endl;
	SubscribeMarketData();
}

void MdApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)  
{  
	if (pDepthMarketData == NULL) {
		cout << "MdApi::OnRtnDepthMarketData error." << endl;
		return;
	}

    CThostFtdcDepthMarketDataField marketData;  
    memcpy(&marketData, pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField)); 
    /*
    cout << "MdApi::OnRtnDepthMarketData:["  
             << marketData.TradingDay << " "  
             << marketData.UpdateTime << " "  
             << marketData.UpdateMillisec << "] "  
             << marketData.InstrumentID << " "  
             << marketData.LastPrice << endl;
	*/
    strategyEngine_->OnTick(marketData);
}
