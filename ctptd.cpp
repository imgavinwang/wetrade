#include "ctptd.h"

void TdApi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _brokerid.c_str());
	strcpy(req.UserID, _investorid.c_str());
	strcpy(req.Password, _password.c_str());
	int iResult = api->ReqUserLogin(&req, ++iRequestID);
	cout << "--Do TdApi::ReqUserLogin: UserID[" << req.UserID <<"] " << ((iResult == 0) ? "ok" : "failed") << endl;
}

void TdApi::AddInstrument(string instrumentid)
{
	_instrumentid = instrumentid;
	cout << "TdApi::AddInstrument: instrumentid["  << _instrumentid << "]" << endl;
}

void TdApi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _brokerid.c_str());
	strcpy(req.InvestorID, _investorid.c_str());
	int iResult = api->ReqSettlementInfoConfirm(&req, ++iRequestID);
	cout << "--Do TdApi::ReqSettlementInfoConfirm: " << ((iResult == 0) ? "ok" : "failed") << endl;
}

void TdApi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, _instrumentid.c_str());
	int iResult = api->ReqQryInstrument(&req, ++iRequestID);
	cout << "--Do TdApi::ReqQryInstrument: " << ((iResult == 0) ? "ok" : "failed") << endl;
}

void TdApi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _brokerid.c_str());
	strcpy(req.InvestorID, _investorid.c_str());
	int iResult = api->ReqQryTradingAccount(&req, ++iRequestID);
	cout << "--Do TdApi::ReqQryTradingAccount: " << ((iResult == 0) ? "ok" : "failed") << endl;
}

void TdApi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, _brokerid.c_str());
	strcpy(req.InvestorID, _investorid.c_str());
	strcpy(req.InstrumentID, _instrumentid.c_str());

	int iResult = api->ReqQryInvestorPosition(&req, ++iRequestID);
	cout << "--Do TdApi::ReqQryInvestorPosition: InstrumentID[" << req.InstrumentID << "] " << ((iResult == 0) ? "ok" : "failed") << endl;
}

void TdApi::ReqOrderInsert(TThostFtdcInstrumentIDType instrumentID, TThostFtdcPriceType price, 
		TThostFtdcVolumeType volume, TThostFtdcDirectionType direction)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, _brokerid.c_str());
	///投资者代码
	strcpy(req.InvestorID, _investorid.c_str());
	///合约代码
	strcpy(req.InstrumentID, instrumentID);
	///报单引用
	strcpy(req.OrderRef, order_ref);
	///用户代码
//	TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///买卖方向: 
	req.Direction = direction;
	///组合开平标志: 开仓
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	req.LimitPrice = price;
	///数量: 1
	req.VolumeTotalOriginal = volume;
	///有效期类型: 当日有效
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 1;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;
	///业务单元
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	req.UserForceClose = 0;

	int iOrderRef = atoi(order_ref);
	iOrderRef++;
	sprintf(order_ref, "%d", iOrderRef);

	int iResult = api->ReqOrderInsert(&req, ++iRequestID);
	cout << "--Do TdApi::ReqOrderInsert: instrumentID[" << instrumentID << "] price[" << price << "] volume[" << volume << "] direction[" << direction << "] order_ref[" << order_ref << "] " << ((iResult == 0) ? "ok" : "failed") << endl;
}

void TdApi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	order_action_send = false;		//是否发送了报单
	if (order_action_send)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, pOrder->BrokerID);
	///投资者代码
	strcpy(req.InvestorID, pOrder->InvestorID);
	///报单操作引用
//	TThostFtdcOrderActionRefType	OrderActionRef;
	///报单引用
	strcpy(req.OrderRef, pOrder->OrderRef);
	///请求编号
//	TThostFtdcRequestIDType	RequestID;
	///前置编号
	req.FrontID = front_id;
	///会话编号
	req.SessionID = session_id;
	///交易所代码
//	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
//	TThostFtdcOrderSysIDType	OrderSysID;
	///操作标志
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///价格
//	TThostFtdcPriceType	LimitPrice;
	///数量变化
//	TThostFtdcVolumeType	VolumeChange;
	///用户代码
//	TThostFtdcUserIDType	UserID;
	///合约代码
	strcpy(req.InstrumentID, pOrder->InstrumentID);
	int iResult = api->ReqOrderAction(&req, ++iRequestID);
	cout << "--Do TdApi::ReqOrderAction: " << ((iResult == 0) ? "ok" : "failed") << endl;
	order_action_send = true;
}

/*---------------------------------------------*/

void TdApi::OnFrontConnected()
{
	cout << "TdApi::OnFrontConnected" << endl;
	
	///用户登录请求
	ReqUserLogin();
}

void TdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "TdApi::OnRspUserLogin: ";
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		front_id = pRspUserLogin->FrontID;
		session_id = pRspUserLogin->SessionID;
		sprintf(order_ref, "%s", pRspUserLogin->MaxOrderRef);
		///获取当前交易日
		sprintf(trading_day, "%s", pRspUserLogin->TradingDay);
		cout << "trading_day[" << trading_day << "] front_id[" << front_id << "] session_id[" << session_id << "] order_ref[" << order_ref << "] " << endl;
		
		///投资者结算结果确认
		ReqSettlementInfoConfirm();
	}
}

void TdApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "TdApi::OnRspSettlementInfoConfirm" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		cout << "ConfirmDate:" << pSettlementInfoConfirm->ConfirmDate << " " << pSettlementInfoConfirm->ConfirmTime << endl;
		
		///查询资金账户
		//ReqQryTradingAccount();
	}
}

void TdApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "TdApi::OnRspQryTradingAccount: " << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		memcpy(&tradingAccount, pTradingAccount, sizeof(CThostFtdcTradingAccountField));
		cout << "Available[" << tradingAccount.Available << "] CurrMargin[" << tradingAccount.CurrMargin << "] PositionProfit[" << tradingAccount.PositionProfit << "]" << endl;

		//查询头寸
		//ReqQryInvestorPosition();
	}
}

void TdApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "TdApi::OnRspQryInvestorPosition: " << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		memcpy(&investorPosition, pInvestorPosition, sizeof(CThostFtdcInvestorPositionField));
		cout << "InstrumentID[" << investorPosition.InstrumentID << "] PosiDirection[" << investorPosition.PosiDirection << "] Position[" << investorPosition.Position << "] PositionCost[" << investorPosition.PositionCost << "] " << endl;

		//下单
		//ReqOrderInsert((char*)_instrumentid.c_str(), 4234.4, 1, THOST_FTDC_D_Sell);
	}
}
void TdApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "TdApi::OnRspQryInstrument" << endl;

	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		cout << "InstrumentID:" << pInstrument->InstrumentID << endl;
	}
}

void TdApi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "TdApi::OnRspOrderInsert" << endl;
}

void TdApi::OnRtnOrder(CThostFtdcOrderField *pOrder)  
{  
    char str[10];  
    sprintf(str, "%d", pOrder->OrderSubmitStatus);  
    int orderState = atoi(str) - 48;    //报单状态0=已经提交，3=已经接受  
  
    cout << "TdApi::OnRtnOrder: ";
  
    if (IsMyOrder(pOrder))  
    {  
        if (IsTradingOrder(pOrder))  
        {  
            cout << "waiting order deal... OrderRef[" << pOrder->OrderRef << "] " << endl;  
            //reqOrderAction(pOrder); // 这里可以撤单  
            //reqUserLogout(); // 登出测试  
        }  
        else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)  
            cout << "order canceled OrderRef[" << pOrder->OrderRef << "] " << endl;  
    }  
}  
  
void TdApi::OnRtnTrade(CThostFtdcTradeField *pTrade)  
{  
	cout << "TdApi::OnRtnTrade deal: ";
    cout << "TradeTime[" << pTrade->TradeTime << "] ";  
    cout << "InstrumentID[" << pTrade->InstrumentID << "] ";  
    cout << "Price[" << pTrade->Price << "] ";  
    cout << "Volume[" << pTrade->Volume << "] ";  
    cout << "Direction[" << pTrade->Direction << "] ";
    cout << "OrderRef[" << pTrade->OrderRef << "] " << endl;  
}  

void TdApi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "TdApi::OnRspOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool TdApi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == front_id) &&
			(pOrder->SessionID == session_id) &&
			(strcmp(pOrder->OrderRef, order_ref) == 0));
}

bool TdApi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

bool TdApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cout << "ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}
