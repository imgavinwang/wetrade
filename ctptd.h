﻿#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <string.h>

#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"

#include "event_engine.hpp"

using namespace std;

class TdApi : public CThostFtdcTraderSpi
{
public:
	TdApi()
	{
		api = CThostFtdcTraderApi::CreateFtdcTraderApi("./tddata_"); //创建交易实例  
	};

	~TdApi()
	{
		Release();
	};

	//连接初始化
	void SetConnect(string frontaddr, string brokerid, string investorid, string password)
	{
		cout << "TdApi::SetConnect" << endl;
		beConnected = false;
		beLogined = false;
		iRequestID = 0;

		_frontaddr = frontaddr;
		_brokerid = brokerid;
		_investorid = investorid;
		_password = password;

    	api->RegisterSpi(this);                                      //注册交易回调实例  
    	api->SubscribePublicTopic(THOST_TERT_RESTART);               //订阅公共流  
    	api->SubscribePrivateTopic(THOST_TERT_RESTART);              //订阅私有流  
    	api->RegisterFront((char*)_frontaddr.c_str());               //设置交易前置地址  
	};
	void Init(){cout << "TdApi::Init" << endl;api->Init();};//启动线程连接运行
	void Join(){cout << "TdApi::Join" << endl;api->Join();};
	void Release(){api->Release();};

	void ReqUserLogin();
	void AddInstrument(string instrumentid);
	void ReqSettlementInfoConfirm();
	void ReqQryInstrument();
	void ReqQryTradingAccount();
	void ReqQryInvestorPosition();
	void ReqOrderInsert(TThostFtdcInstrumentIDType instrumentID, TThostFtdcPriceType price, 
		TThostFtdcVolumeType volume, TThostFtdcDirectionType direction);
	void ReqOrderAction(CThostFtdcOrderField *pOrder);

	void OnFrontConnected();
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRtnOrder(CThostFtdcOrderField *pOrder);
	void OnRtnTrade(CThostFtdcTradeField *pTrade);
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

private:
	CThostFtdcTraderApi* api;
	//StrategyEngine *strategyEngine_;

	bool beConnected;
	bool beLogined;
	int iRequestID;
	bool order_action_send;

	string _frontaddr;
	string _brokerid;
	string _investorid;
	string _password;	
	string _instrumentid;

	TThostFtdcFrontIDType front_id;
	TThostFtdcSessionIDType session_id;
	TThostFtdcOrderRefType order_ref;
	TThostFtdcDateType trading_day;

	CThostFtdcTradingAccountField tradingAccount;
	CThostFtdcInvestorPositionField investorPosition;
};
