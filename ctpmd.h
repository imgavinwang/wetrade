#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <string.h>

#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"

#include "event_engine.hpp"

using namespace std;

class MdApi : public CThostFtdcMdSpi
{
public:
	MdApi(EventEngine* eventEngine)
	{
		eventEngine_ = eventEngine;
		api = CThostFtdcMdApi::CreateFtdcMdApi("./mddata_", true, true); //创建行情实例  
	};

	~MdApi()
	{
		Release();
	};

	//连接初始化
	void SetConnect(string frontaddr, string brokerid, string investorid, string password)
	{
		cout << "MdApi::SetConnect" << endl;
		beConnected = false;
		beLogined = false;
		iRequestID = 0;

		_frontaddr = frontaddr;
		_brokerid = brokerid;
		_investorid = investorid;
		_password = password;	

		api->RegisterSpi(this);                                          //注册行情回调实例  		
		api->RegisterFront((char*)_frontaddr.c_str());                   //设置行情前置地址  		
	};
	void Init(){cout << "MdApi::Init" << endl;api->Init();};//启动线程连接运行
	void Join(){cout << "MdApi::Join" << endl;api->Join();};
	void Release(){api->Release();};

	void ReqUserLogin();
	void AddInstrument(string instrumentid);
	void SubscribeMarketData();

	virtual void OnFrontConnected();
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
	CThostFtdcMdApi* api;
	EventEngine* eventEngine_;

	bool beConnected;
	bool beLogined;
	int iRequestID;

	string _frontaddr;
	string _brokerid;
	string _investorid;
	string _password;	
	string _instrumentid;
};
