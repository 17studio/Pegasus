
#include "capi.h"

#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"

//==================================================================================
//
// 类实现: MD_API
//
//==================================================================================

struct CtpMdSpiCallback {
  ctp_md_front_connected_cb connected = nullptr;
  ctp_md_front_disconnected_cb disconnected = nullptr;
  ctp_md_heart_beat_warning_cb heat_beat_warning = nullptr;
  ctp_md_user_login_rsp_cb login = nullptr;
  ctp_md_user_logout_rsp_cb logout = nullptr;
  ctp_md_error_rsp_cb error = nullptr;
  ctp_md_sub_market_data_rsp_cb sub_market = nullptr;
  ctp_md_unsub_market_data_rsp_cb unsub_market = nullptr;
  ctp_md_sub_quote_rsp_cb sub_quote = nullptr;
  ctp_md_unsub_quote_rsp_cb unsub_quote = nullptr;
  ctp_md_depth_market_data_cb market_data = nullptr;
  ctp_md_quote_cb quote = nullptr;
};

class CThostFtdcMdSpiImpl : public CThostFtdcMdSpi {
 public:
  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  virtual void OnFrontConnected() override { callback_.connected(); };

  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  ///@param nReason 错误原因
  ///        0x1001 网络读失败
  ///        0x1002 网络写失败
  ///        0x2001 接收心跳超时
  ///        0x2002 发送心跳失败
  ///        0x2003 收到错误报文
  virtual void OnFrontDisconnected(int nReason) override { callback_.disconnected(nReason); };

  ///心跳超时警告。当长时间未收到报文时，该方法被调用。
  ///@param nTimeLapse 距离上次接收报文的时间
  virtual void OnHeartBeatWarning(int nTimeLapse) override { callback_.heat_beat_warning(nTimeLapse); };

  ///登录请求响应
  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.login(pRspUserLogin, pRspInfo, nRequestID, bIsLast);
  };

  ///登出请求响应
  virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.logout(pUserLogout, pRspInfo, nRequestID, bIsLast);
  };

  ///错误应答
  virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.error(pRspInfo, nRequestID, bIsLast);
  };

  ///订阅行情应答
  virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument,
                                  CThostFtdcRspInfoField* pRspInfo,
                                  int nRequestID,
                                  bool bIsLast) override {
    callback_.sub_market(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
  };

  ///取消订阅行情应答
  virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument,
                                    CThostFtdcRspInfoField* pRspInfo,
                                    int nRequestID,
                                    bool bIsLast) override {
    callback_.unsub_market(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
  };

  ///订阅询价应答
  virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument,
                                   CThostFtdcRspInfoField* pRspInfo,
                                   int nRequestID,
                                   bool bIsLast) override {
    callback_.sub_quote(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
  };

  ///取消订阅询价应答
  virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument,
                                     CThostFtdcRspInfoField* pRspInfo,
                                     int nRequestID,
                                     bool bIsLast) override {
    callback_.unsub_quote(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
  };

  ///深度行情通知
  virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) override { callback_.market_data(pDepthMarketData); };

  ///询价通知
  virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) override { callback_.quote(pForQuoteRsp); };

  ///完成回调的设置
  void SetOnFrontConnected(ctp_md_front_connected_cb cb) { callback_.connected = cb; }
  void SetOnFrontDisconnected(ctp_md_front_disconnected_cb cb) { callback_.disconnected = cb; }
  void SetOnHeartBeatWarning(ctp_md_heart_beat_warning_cb cb) { callback_.heat_beat_warning = cb; }
  void SetOnRspUserLogin(ctp_md_user_login_rsp_cb cb) { callback_.login = cb; }
  void SetOnRspUserLogout(ctp_md_user_logout_rsp_cb cb) { callback_.logout = cb; }
  void SetOnRspError(ctp_md_error_rsp_cb cb) { callback_.error = cb; }
  void SetOnRspSubMarketData(ctp_md_sub_market_data_rsp_cb cb) { callback_.sub_market = cb; }
  void SetOnRspUnSubMarketData(ctp_md_unsub_market_data_rsp_cb cb) { callback_.unsub_market = cb; }
  void SetOnRspSubForQuoteRsp(ctp_md_sub_quote_rsp_cb cb) { callback_.sub_quote = cb; }
  void SetOnRspUnSubForQuoteRsp(ctp_md_unsub_quote_rsp_cb cb) { callback_.unsub_quote = cb; }
  void SetOnRtnDepthMarketData(ctp_md_depth_market_data_cb cb) { callback_.market_data = cb; }
  void SetOnRtnForQuoteRsp(ctp_md_quote_cb cb) { callback_.quote = cb; }

 private:
  CtpMdSpiCallback callback_;
};

//==================================================================================
//
// 定义被外部使用的C struct
//
//==================================================================================

struct ctp_md_spi_t {
  CThostFtdcMdSpiImpl impl;
};

struct ctp_md_api_t {
  CThostFtdcMdApi* mdapi = nullptr;
};

//==================================================================================
//
// C代码，外部函数的实现
//
//==================================================================================

extern "C" {

ctp_md_spi_t* ctp_mdspi_create() {
  return new ctp_md_spi_t;
}
void ctp_mdspi_free(ctp_md_spi_t* spi) {
  delete spi;
}
void ctp_mdspi_set_front_connected_cb(ctp_md_spi_t* spi, ctp_md_front_connected_cb cb) {
  spi->impl.SetOnFrontConnected(cb);
}
void ctp_mdspi_set_front_disconnected_cb(ctp_md_spi_t* spi, ctp_md_front_disconnected_cb cb) {
  spi->impl.SetOnFrontDisconnected(cb);
}
void ctp_mdspi_set_heartbeat_warning_cb(ctp_md_spi_t* spi, ctp_md_heart_beat_warning_cb cb) {
  spi->impl.SetOnHeartBeatWarning(cb);
}
void ctp_mdspi_set_user_login_cb(ctp_md_spi_t* spi, ctp_md_user_login_rsp_cb cb) {
  spi->impl.SetOnRspUserLogin(cb);
}
void ctp_mdspi_set_user_logout_cb(ctp_md_spi_t* spi, ctp_md_user_logout_rsp_cb cb) {
  spi->impl.SetOnRspUserLogout(cb);
}
void ctp_mdspi_set_error_cb(ctp_md_spi_t* spi, ctp_md_error_rsp_cb cb) {
  spi->impl.SetOnRspError(cb);
}
void ctp_mdspi_set_sub_market_data_rsp_cb(ctp_md_spi_t* spi, ctp_md_sub_market_data_rsp_cb cb) {
  spi->impl.SetOnRspSubMarketData(cb);
}
void ctp_mdspi_set_unsub_market_data_rsp_cb(ctp_md_spi_t* spi, ctp_md_unsub_market_data_rsp_cb cb) {
  spi->impl.SetOnRspUnSubMarketData(cb);
}
void ctp_mdspi_set_sub_quote_rsp_cb(ctp_md_spi_t* spi, ctp_md_unsub_quote_rsp_cb cb) {
  spi->impl.SetOnRspSubForQuoteRsp(cb);
}
void ctp_mdspi_set_unsub_quote_rsp_cb(ctp_md_spi_t* spi, ctp_md_unsub_quote_rsp_cb cb) {
  spi->impl.SetOnRspUnSubForQuoteRsp(cb);
}
void ctp_mdspi_set_depth_market_data_cb(ctp_md_spi_t* spi, ctp_md_depth_market_data_cb cb) {
  spi->impl.SetOnRtnDepthMarketData(cb);
}
void ctp_mdspi_set_quote_cb(ctp_md_spi_t* spi, ctp_md_quote_cb cb) {
  spi->impl.SetOnRtnForQuoteRsp(cb);
}

ctp_md_api_t* ctp_mdapi_create(const char* flowpath, bool useudp, bool multicast) {
  auto api = new ctp_md_api_t;
  api->mdapi = CThostFtdcMdApi::CreateFtdcMdApi(flowpath, useudp, multicast);
  return api;
}
void ctp_mdapi_free(ctp_md_api_t* api) {
  api->mdapi->Release();
  delete api;
}
const char* ctp_mdapi_get_api_version(ctp_md_api_t* api) {
  return api->mdapi->GetApiVersion();
}
void ctp_mdapi_init(ctp_md_api_t* api) {
  api->mdapi->Init();
}
void ctp_mdapi_join(ctp_md_api_t* api) {
  api->mdapi->Join();
}
const char* ctp_mdapi_get_trading_day(ctp_md_api_t* api) {
  return api->mdapi->GetTradingDay();
}
void ctp_mdapi_register_front(ctp_md_api_t* api, char* front_address) {
  api->mdapi->RegisterFront(front_address);
}
void ctp_mdapi_register_nameserver(ctp_md_api_t* api, char* nameserver_address) {
  api->mdapi->RegisterNameServer(nameserver_address);
}
void ctp_mdapi_register_fens_userinfo(ctp_md_api_t* api, CThostFtdcFensUserInfoField* userinfo) {
  api->mdapi->RegisterFensUserInfo(userinfo);
}
void ctp_mdapi_register_spi(ctp_md_api_t* api, ctp_md_spi_t* spi) {
  api->mdapi->RegisterSpi(&(spi->impl));
}
void ctp_mdapi_sub_market_data(ctp_md_api_t* api, char* instruments[], int count) {
  api->mdapi->SubscribeMarketData(instruments, count);
}
void ctp_mdapi_unsub_market_data(ctp_md_api_t* api, char* instruments[], int count) {
  api->mdapi->UnSubscribeMarketData(instruments, count);
}
void ctp_mdapi_sub_quote(ctp_md_api_t* api, char* instruments[], int count) {
  api->mdapi->SubscribeForQuoteRsp(instruments, count);
}
void ctp_mdapi_unsub_quote(ctp_md_api_t* api, char* instruments[], int count) {
  api->mdapi->UnSubscribeForQuoteRsp(instruments, count);
}
void ctp_mdapi_user_login(ctp_md_api_t* api, CThostFtdcReqUserLoginField* userlogin, int request_id) {
  api->mdapi->ReqUserLogin(userlogin, request_id);
}
void ctp_mdapi_user_logout(ctp_md_api_t* api, CThostFtdcUserLogoutField* userlogout, int request_id) {
  api->mdapi->ReqUserLogout(userlogout, request_id);
}

}  // end extern "C"

//==================================================================================
//
// 类实现: TRADER_API
//
//==================================================================================

struct CtpTraderSpiCallback {
  ctp_trader_front_connected_cb ctp_trader_front_connected_ = nullptr;
  ctp_trader_front_disconnected_cb ctp_trader_front_disconnected_ = nullptr;
  ctp_trader_heart_beat_warning_cb ctp_trader_heart_beat_warning_ = nullptr;
  ctp_trader_authenticate_rsp_cb ctp_trader_authenticate_rsp_ = nullptr;
  ctp_trader_user_login_rsp_cb ctp_trader_user_login_rsp_ = nullptr;
  ctp_trader_user_logout_rsp_cb ctp_trader_user_logout_rsp_ = nullptr;
  ctp_trader_user_passwd_update_rsp_cb ctp_trader_user_passwd_update_rsp_ = nullptr;
  ctp_trader_trading_account_passwd_update_cb ctp_trader_trading_account_passwd_update_ = nullptr;
  ctp_trader_order_insert_rsp_cb ctp_trader_order_insert_rsp_ = nullptr;
  ctp_trader_parked_order_insert_rsp_cb ctp_trader_parked_order_insert_rsp_ = nullptr;
  ctp_trader_packed_order_action_rsp_cb ctp_trader_packed_order_action_rsp_ = nullptr;
  ctp_trader_order_action_rsp_cb ctp_trader_order_action_rsp_ = nullptr;
  ctp_trader_query_max_order_volume_rsp_cb ctp_trader_query_max_order_volume_rsp_ = nullptr;
  ctp_trader_settlement_info_confirm_rsp_cb ctp_trader_settlement_info_confirm_rsp_ = nullptr;
  ctp_trader_remove_packed_order_rsp_cb ctp_trader_remove_packed_order_rsp_ = nullptr;
  ctp_trader_remove_packed_order_action_rsp_cb ctp_trader_remove_packed_order_action_rsp_ = nullptr;
  ctp_trader_exec_order_insert_rsp_cb ctp_trader_exec_order_insert_rsp_ = nullptr;
  ctp_trader_exec_order_action_rsp_cb ctp_trader_exec_order_action_rsp_ = nullptr;
  ctp_trader_for_quote_insert_rsp_cb ctp_trader_for_quote_insert_rsp_ = nullptr;
  ctp_trader_quote_insert_rsp_cb ctp_trader_quote_insert_rsp_ = nullptr;
  ctp_trader_quote_action_rsp_cb ctp_trader_quote_action_rsp_ = nullptr;
  ctp_trader_batch_order_action_rsp_cb ctp_trader_batch_order_action_rsp_ = nullptr;
  ctp_trader_comb_action_insert_rsp_cb ctp_trader_comb_action_insert_rsp_ = nullptr;
  ctp_trader_query_order_rsp_cb ctp_trader_query_order_rsp_ = nullptr;
  ctp_trader_query_trade_rsp_cb ctp_trader_query_trade_rsp_ = nullptr;
  ctp_trader_query_investor_position_rsp_cb ctp_trader_query_investor_position_rsp_ = nullptr;
  ctp_trader_query_trading_account_rsp_cb ctp_trader_query_trading_account_rsp_ = nullptr;
  ctp_trader_query_investor_rsp_cb ctp_trader_query_investor_rsp_ = nullptr;
  ctp_trader_query_trading_code_rsp_cb ctp_trader_query_trading_code_rsp_ = nullptr;
  ctp_trader_query_instrument_margin_rate_rsp_cb ctp_trader_query_instrument_margin_rate_rsp_ = nullptr;
  ctp_trader_query_instrument_commission_rate_rsp_cb ctp_trader_query_instrument_commission_rate_rsp_ = nullptr;
  ctp_trader_query_exchange_rsp_cb ctp_trader_query_exchange_rsp_ = nullptr;
  ctp_trader_query_product_rsp_cb ctp_trader_query_product_rsp_ = nullptr;
  ctp_trader_query_instrument_rsp_cb ctp_trader_query_instrument_rsp_ = nullptr;
  ctp_trader_query_depth_market_data_rsp_cb ctp_trader_query_depth_market_data_rsp_ = nullptr;
  ctp_trader_query_settlement_info_rsp_cb ctp_trader_query_settlement_info_rsp_ = nullptr;
  ctp_trader_query_transfer_bank_rsp_cb ctp_trader_query_transfer_bank_rsp_ = nullptr;
  ctp_trader_query_investor_position_detail_rsp_cb ctp_trader_query_investor_position_detail_rsp_ = nullptr;
  ctp_trader_query_notice_rsp_cb ctp_trader_query_notice_rsp_ = nullptr;
  ctp_trader_query_settlement_info_confirm_rsp_cb ctp_trader_query_settlement_info_confirm_rsp_ = nullptr;
  ctp_trader_query_investor_position_combine_detail_rsp_cb ctp_trader_query_investor_position_combine_detail_rsp_ = nullptr;
  ctp_trader_query_cfmmc_trading_account_key_rsp_cb ctp_trader_query_cfmmc_trading_account_key_rsp_ = nullptr;
  ctp_trader_query_ewarrant_offset_rsp_cb ctp_trader_query_ewarrant_offset_rsp_ = nullptr;
  ctp_trader_query_investor_product_group_margin_rsp_cb ctp_trader_query_investor_product_group_margin_rsp_ = nullptr;
  ctp_trader_query_exchange_margin_rate_rsp_cb ctp_trader_query_exchange_margin_rate_rsp_ = nullptr;
  ctp_trader_query_exchange_margin_rate_adjust_rsp_cb ctp_trader_query_exchange_margin_rate_adjust_rsp_ = nullptr;
  ctp_trader_query_exchange_rate_rsp_cb ctp_trader_query_exchange_rate_rsp_ = nullptr;
  ctp_trader_query_secagent_acidmap_rsp_cb ctp_trader_query_secagent_acidmap_rsp_ = nullptr;
  ctp_trader_query_product_exchange_rate_cb ctp_trader_query_product_exchange_rate_ = nullptr;
  ctp_trader_query_product_group_rsp_cb ctp_trader_query_product_group_rsp_ = nullptr;
  ctp_trader_query_mmintrument_commission_rate_rsp_cb ctp_trader_query_mmintrument_commission_rate_rsp_ = nullptr;
  ctp_trader_query_mmoption_instrument_commission_rate_rsp_cb ctp_trader_query_mmoption_instrument_commission_rate_rsp_ = nullptr;
  ctp_trader_query_instrument_order_commission_rate_rsp_cb ctp_trader_query_instrument_order_commission_rate_rsp_ = nullptr;
  ctp_trader_query_option_instrument_trade_cost_rsp_cb ctp_trader_query_option_instrument_trade_cost_rsp_ = nullptr;
  ctp_trader_query_option_instrument_commission_rate_rsp_cb ctp_trader_query_option_instrument_commission_rate_rsp_ = nullptr;
  ctp_trader_query_exec_order_rsp_cb ctp_trader_query_exec_order_rsp_ = nullptr;
  ctp_trader_query_forquote_rsp_cb ctp_trader_query_forquote_rsp_ = nullptr;
  ctp_trader_query_quote_rsp_cb ctp_trader_query_quote_rsp_ = nullptr;
  ctp_trader_query_comb_instrument_guard_rsp_cb ctp_trader_query_comb_instrument_guard_rsp_ = nullptr;
  ctp_trader_query_comb_action_rsp_cb ctp_trader_query_comb_action_rsp_ = nullptr;
  ctp_trader_query_transfer_serial_rsp_cb ctp_trader_query_transfer_serial_rsp_ = nullptr;
  ctp_trader_query_account_register_rsp_cb ctp_trader_query_account_register_rsp_ = nullptr;
  ctp_trader_error_rsp_cb ctp_trader_error_rsp_ = nullptr;
  ctp_trader_return_order_rsp_cb ctp_trader_return_order_rsp_ = nullptr;
  ctp_trader_return_trade_rsp_cb ctp_trader_return_trade_rsp_ = nullptr;
  ctp_trader_return_order_insert_error_cb ctp_trader_return_order_insert_error_ = nullptr;
  ctp_trader_return_order_action_error_cb ctp_trader_return_order_action_error_ = nullptr;
  ctp_trader_return_instrument_status_cb ctp_trader_return_instrument_status_ = nullptr;
  ctp_trader_return_bulletin_cb ctp_trader_return_bulletin_ = nullptr;
  ctp_trader_return_trading_notice_cb ctp_trader_return_trading_notice_ = nullptr;
  ctp_trader_return_error_conditional_order_cb ctp_trader_return_error_conditional_order_ = nullptr;
  ctp_trader_return_exec_order_cb ctp_trader_return_exec_order_ = nullptr;
  ctp_trader_return_exec_order_insert_error_cb ctp_trader_return_exec_order_insert_error_ = nullptr;
  ctp_trader_return_excc_order_action_error_cb ctp_trader_return_excc_order_action_error_ = nullptr;
  ctp_trader_return_forquote_insert_error_cb ctp_trader_return_forquote_insert_error_ = nullptr;
  ctp_trader_return_quote_cb ctp_trader_return_quote_ = nullptr;
  ctp_trader_return_quote_insert_error_cb ctp_trader_return_quote_insert_error_ = nullptr;
  ctp_trader_return_quote_action_error_cb ctp_trader_return_quote_action_error_ = nullptr;
  ctp_trader_return_forquote_rsp_cb ctp_trader_return_forquote_rsp_ = nullptr;
  ctp_trader_return_fmmct_trading_account_token_cb ctp_trader_return_fmmct_trading_account_token_ = nullptr;
  ctp_trader_return_batch_order_action_error_cb ctp_trader_return_batch_order_action_error_ = nullptr;
  ctp_trader_return_comb_action_cb ctp_trader_return_comb_action_ = nullptr;
  ctp_trader_return_comb_action_insert_error_cb ctp_trader_return_comb_action_insert_error_ = nullptr;
  ctp_trader_query_contract_bank_rsp_cb ctp_trader_query_contract_bank_rsp_ = nullptr;
  ctp_trader_query_packed_order_rsp_cb ctp_trader_query_packed_order_rsp_ = nullptr;
  ctp_trader_query_packed_order_action_rsp_cb ctp_trader_query_packed_order_action_rsp_ = nullptr;
  ctp_trader_query_trading_notice_rsp_cb ctp_trader_query_trading_notice_rsp_ = nullptr;
  ctp_trader_query_broker_trading_params_rsp_cb ctp_trader_query_broker_trading_params_rsp_ = nullptr;
  ctp_trader_query_broker_trading_algos_rsp_cb ctp_trader_query_broker_trading_algos_rsp_ = nullptr;
  ctp_trader_query_cfmmc_trading_account_token_rsp_cb ctp_trader_query_cfmmc_trading_account_token_rsp_ = nullptr;
  ctp_trader_return_from_bank_to_future_by_bank_cb ctp_trader_return_from_bank_to_future_by_bank_ = nullptr;
  ctp_trader_return_from_future_to_bank_by_bank_cb ctp_trader_return_from_future_to_bank_by_bank_ = nullptr;
  ctp_trader_return_repeal_from_bank_to_future_by_bank_cb ctp_trader_return_repeal_from_bank_to_future_by_bank_ = nullptr;
  ctp_trader_return_repeal_from_future_to_bank_by_bank_cb ctp_trader_return_repeal_from_future_to_bank_by_bank_ = nullptr;
  ctp_trader_return_from_bank_to_future_by_future_cb ctp_trader_return_from_bank_to_future_by_future_ = nullptr;
  ctp_trader_return_from_future_to_bank_by_future_cb ctp_trader_return_from_future_to_bank_by_future_ = nullptr;
  ctp_trader_return_repeal_from_bank_to_future_by_futuremanual_cb ctp_trader_return_repeal_from_bank_to_future_by_futuremanual_ = nullptr;
  ctp_trader_return_repeal_from_future_to_bank_by_futuremanual_cb ctp_trader_return_repeal_from_future_to_bank_by_futuremanual_ = nullptr;
  ctp_trader_return_query_bank_balance_by_future_cb ctp_trader_return_query_bank_balance_by_future_ = nullptr;
  ctp_trader_return_bank_to_future_by_future_error_cb ctp_trader_return_bank_to_future_by_future_error_ = nullptr;
  ctp_trader_return_future_to_bank_by_future_error_cb ctp_trader_return_future_to_bank_by_future_error_ = nullptr;
  ctp_trader_return_repeal_bank_to_future_by_futuremanual_error_cb ctp_trader_return_repeal_bank_to_future_by_futuremanual_error_ = nullptr;
  ctp_trader_return_repeal_future_to_bank_by_futuremanual_error_cb ctp_trader_return_repeal_future_to_bank_by_futuremanual_error_ = nullptr;
  ctp_trader_return_query_bank_balance_by_future_error_cb ctp_trader_return_query_bank_balance_by_future_error_ = nullptr;
  ctp_trader_return_repeal_from_bank_to_future_by_future_cb ctp_trader_return_repeal_from_bank_to_future_by_future_ = nullptr;
  ctp_trader_return_repeal_from_future_to_bank_by_future_cb ctp_trader_return_repeal_from_future_to_bank_by_future_ = nullptr;
  ctp_trader_from_bank_to_future_by_future_rsp_cb ctp_trader_from_bank_to_future_by_future_rsp_ = nullptr;
  ctp_trader_from_future_to_bank_by_future_rsp_cb ctp_trader_from_future_to_bank_by_future_rsp_ = nullptr;
  ctp_trader_query_bank_account_monoy_by_future_rsp_cb ctp_trader_query_bank_account_monoy_by_future_rsp_ = nullptr;
  ctp_trader_return_open_account_by_bank_cb ctp_trader_return_open_account_by_bank_ = nullptr;
  ctp_trader_return_cancel_account_by_bank_cb ctp_trader_return_cancel_account_by_bank_ = nullptr;
  ctp_trader_return_change_account_by_bank_cb ctp_trader_return_change_account_by_bank_ = nullptr;
};

class CThostFtdcTraderSpiImpl : public CThostFtdcTraderSpi {
 public:
  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  virtual void OnFrontConnected() override { callback_.ctp_trader_front_connected_(); };

  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  ///@param nReason 错误原因
  ///        0x1001 网络读失败
  ///        0x1002 网络写失败
  ///        0x2001 接收心跳超时
  ///        0x2002 发送心跳失败
  ///        0x2003 收到错误报文
  virtual void OnFrontDisconnected(int nReason) override { callback_.ctp_trader_front_disconnected_(nReason); };

  ///心跳超时警告。当长时间未收到报文时，该方法被调用。
  ///@param nTimeLapse 距离上次接收报文的时间
  virtual void OnHeartBeatWarning(int nTimeLapse) override { callback_.ctp_trader_heart_beat_warning_(nTimeLapse); };

  ///客户端认证响应
  virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField,
                                 CThostFtdcRspInfoField* pRspInfo,
                                 int nRequestID,
                                 bool bIsLast) override {
    callback_.ctp_trader_authenticate_rsp_(pRspAuthenticateField, pRspInfo, nRequestID, bIsLast);
  };

  ///登录请求响应
  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_user_login_rsp_(pRspUserLogin, pRspInfo, nRequestID, bIsLast);
  };

  ///登出请求响应
  virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_user_logout_rsp_(pUserLogout, pRspInfo, nRequestID, bIsLast);
  };

  ///用户口令更新请求响应
  virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate,
                                       CThostFtdcRspInfoField* pRspInfo,
                                       int nRequestID,
                                       bool bIsLast) override {
    callback_.ctp_trader_user_passwd_update_rsp_(pUserPasswordUpdate, pRspInfo, nRequestID, bIsLast);
  };

  ///资金账户口令更新请求响应
  virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField* pTradingAccountPasswordUpdate,
                                                 CThostFtdcRspInfoField* pRspInfo,
                                                 int nRequestID,
                                                 bool bIsLast) override {
    callback_.ctp_trader_trading_account_passwd_update_(pTradingAccountPasswordUpdate, pRspInfo, nRequestID, bIsLast);
  };

  ///报单录入请求响应
  virtual void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_order_insert_rsp_(pInputOrder, pRspInfo, nRequestID, bIsLast);
  };

  ///预埋单录入请求响应
  virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField* pParkedOrder,
                                      CThostFtdcRspInfoField* pRspInfo,
                                      int nRequestID,
                                      bool bIsLast) override {
    callback_.ctp_trader_parked_order_insert_rsp_(pParkedOrder, pRspInfo, nRequestID, bIsLast);
  };

  ///预埋撤单录入请求响应
  virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField* pParkedOrderAction,
                                      CThostFtdcRspInfoField* pRspInfo,
                                      int nRequestID,
                                      bool bIsLast) override {
    callback_.ctp_trader_packed_order_action_rsp_(pParkedOrderAction, pRspInfo, nRequestID, bIsLast);
  };

  ///报单操作请求响应
  virtual void OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction,
                                CThostFtdcRspInfoField* pRspInfo,
                                int nRequestID,
                                bool bIsLast) override {
    callback_.ctp_trader_order_action_rsp_(pInputOrderAction, pRspInfo, nRequestID, bIsLast);
  };

  ///查询最大报单数量响应
  virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField* pQueryMaxOrderVolume,
                                        CThostFtdcRspInfoField* pRspInfo,
                                        int nRequestID,
                                        bool bIsLast) override {
    callback_.ctp_trader_query_max_order_volume_rsp_(pQueryMaxOrderVolume, pRspInfo, nRequestID, bIsLast);
  };

  ///投资者结算结果确认响应
  virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
                                          CThostFtdcRspInfoField* pRspInfo,
                                          int nRequestID,
                                          bool bIsLast) override {
    callback_.ctp_trader_settlement_info_confirm_rsp_(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
  };

  ///删除预埋单响应
  virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField* pRemoveParkedOrder,
                                      CThostFtdcRspInfoField* pRspInfo,
                                      int nRequestID,
                                      bool bIsLast) override {
    callback_.ctp_trader_remove_packed_order_rsp_(pRemoveParkedOrder, pRspInfo, nRequestID, bIsLast);
  };

  ///删除预埋撤单响应
  virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField* pRemoveParkedOrderAction,
                                            CThostFtdcRspInfoField* pRspInfo,
                                            int nRequestID,
                                            bool bIsLast) override {
    callback_.ctp_trader_remove_packed_order_action_rsp_(pRemoveParkedOrderAction, pRspInfo, nRequestID, bIsLast);
  };

  ///执行宣告录入请求响应
  virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField* pInputExecOrder,
                                    CThostFtdcRspInfoField* pRspInfo,
                                    int nRequestID,
                                    bool bIsLast) override {
    callback_.ctp_trader_exec_order_insert_rsp_(pInputExecOrder, pRspInfo, nRequestID, bIsLast);
  };

  ///执行宣告操作请求响应
  virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField* pInputExecOrderAction,
                                    CThostFtdcRspInfoField* pRspInfo,
                                    int nRequestID,
                                    bool bIsLast) override {
    callback_.ctp_trader_exec_order_action_rsp_(pInputExecOrderAction, pRspInfo, nRequestID, bIsLast);
  };

  ///询价录入请求响应
  virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField* pInputForQuote,
                                   CThostFtdcRspInfoField* pRspInfo,
                                   int nRequestID,
                                   bool bIsLast) override {
    callback_.ctp_trader_for_quote_insert_rsp_(pInputForQuote, pRspInfo, nRequestID, bIsLast);
  };

  ///报价录入请求响应
  virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField* pInputQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_quote_insert_rsp_(pInputQuote, pRspInfo, nRequestID, bIsLast);
  };

  ///报价操作请求响应
  virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField* pInputQuoteAction,
                                CThostFtdcRspInfoField* pRspInfo,
                                int nRequestID,
                                bool bIsLast) override {
    callback_.ctp_trader_quote_action_rsp_(pInputQuoteAction, pRspInfo, nRequestID, bIsLast);
  };

  ///批量报单操作请求响应
  virtual void OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField* pInputBatchOrderAction,
                                     CThostFtdcRspInfoField* pRspInfo,
                                     int nRequestID,
                                     bool bIsLast) override {
    callback_.ctp_trader_batch_order_action_rsp_(pInputBatchOrderAction, pRspInfo, nRequestID, bIsLast);
  };

  ///申请组合录入请求响应
  virtual void OnRspCombActionInsert(CThostFtdcInputCombActionField* pInputCombAction,
                                     CThostFtdcRspInfoField* pRspInfo,
                                     int nRequestID,
                                     bool bIsLast) override {
    callback_.ctp_trader_comb_action_insert_rsp_(pInputCombAction, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询报单响应
  virtual void OnRspQryOrder(CThostFtdcOrderField* pOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_order_rsp_(pOrder, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询成交响应
  virtual void OnRspQryTrade(CThostFtdcTradeField* pTrade, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_trade_rsp_(pTrade, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询投资者持仓响应
  virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition,
                                        CThostFtdcRspInfoField* pRspInfo,
                                        int nRequestID,
                                        bool bIsLast) override {
    callback_.ctp_trader_query_investor_position_rsp_(pInvestorPosition, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询资金账户响应
  virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount,
                                      CThostFtdcRspInfoField* pRspInfo,
                                      int nRequestID,
                                      bool bIsLast) override {
    callback_.ctp_trader_query_trading_account_rsp_(pTradingAccount, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询投资者响应
  virtual void OnRspQryInvestor(CThostFtdcInvestorField* pInvestor, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_investor_rsp_(pInvestor, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询交易编码响应
  virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField* pTradingCode,
                                   CThostFtdcRspInfoField* pRspInfo,
                                   int nRequestID,
                                   bool bIsLast) override {
    callback_.ctp_trader_query_trading_code_rsp_(pTradingCode, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询合约保证金率响应
  virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField* pInstrumentMarginRate,
                                            CThostFtdcRspInfoField* pRspInfo,
                                            int nRequestID,
                                            bool bIsLast) override {
    callback_.ctp_trader_query_instrument_margin_rate_rsp_(pInstrumentMarginRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询合约手续费率响应
  virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField* pInstrumentCommissionRate,
                                                CThostFtdcRspInfoField* pRspInfo,
                                                int nRequestID,
                                                bool bIsLast) override {
    callback_.ctp_trader_query_instrument_commission_rate_rsp_(pInstrumentCommissionRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询交易所响应
  virtual void OnRspQryExchange(CThostFtdcExchangeField* pExchange, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_exchange_rsp_(pExchange, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询产品响应
  virtual void OnRspQryProduct(CThostFtdcProductField* pProduct, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_product_rsp_(pProduct, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询合约响应
  virtual void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_instrument_rsp_(pInstrument, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询行情响应
  virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData,
                                       CThostFtdcRspInfoField* pRspInfo,
                                       int nRequestID,
                                       bool bIsLast) override {
    callback_.ctp_trader_query_depth_market_data_rsp_(pDepthMarketData, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询投资者结算结果响应
  virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo,
                                      CThostFtdcRspInfoField* pRspInfo,
                                      int nRequestID,
                                      bool bIsLast) override {
    callback_.ctp_trader_query_settlement_info_rsp_(pSettlementInfo, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询转帐银行响应
  virtual void OnRspQryTransferBank(CThostFtdcTransferBankField* pTransferBank,
                                    CThostFtdcRspInfoField* pRspInfo,
                                    int nRequestID,
                                    bool bIsLast) override {
    callback_.ctp_trader_query_transfer_bank_rsp_(pTransferBank, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询投资者持仓明细响应
  virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField* pInvestorPositionDetail,
                                              CThostFtdcRspInfoField* pRspInfo,
                                              int nRequestID,
                                              bool bIsLast) override {
    callback_.ctp_trader_query_investor_position_detail_rsp_(pInvestorPositionDetail, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询客户通知响应
  virtual void OnRspQryNotice(CThostFtdcNoticeField* pNotice, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_notice_rsp_(pNotice, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询结算信息确认响应
  virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
                                             CThostFtdcRspInfoField* pRspInfo,
                                             int nRequestID,
                                             bool bIsLast) override {
    callback_.ctp_trader_query_settlement_info_confirm_rsp_(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询投资者持仓明细响应
  virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField* pInvestorPositionCombineDetail,
                                                     CThostFtdcRspInfoField* pRspInfo,
                                                     int nRequestID,
                                                     bool bIsLast) override {
    callback_.ctp_trader_query_investor_position_combine_detail_rsp_(pInvestorPositionCombineDetail, pRspInfo, nRequestID, bIsLast);
  };

  ///查询保证金监管系统经纪公司资金账户密钥响应
  virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField* pCFMMCTradingAccountKey,
                                              CThostFtdcRspInfoField* pRspInfo,
                                              int nRequestID,
                                              bool bIsLast) override {
    callback_.ctp_trader_query_cfmmc_trading_account_key_rsp_(pCFMMCTradingAccountKey, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询仓单折抵信息响应
  virtual void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField* pEWarrantOffset,
                                      CThostFtdcRspInfoField* pRspInfo,
                                      int nRequestID,
                                      bool bIsLast) override {
    callback_.ctp_trader_query_ewarrant_offset_rsp_(pEWarrantOffset, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询投资者品种/跨品种保证金响应
  virtual void OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField* pInvestorProductGroupMargin,
                                                  CThostFtdcRspInfoField* pRspInfo,
                                                  int nRequestID,
                                                  bool bIsLast) override {
    callback_.ctp_trader_query_investor_product_group_margin_rsp_(pInvestorProductGroupMargin, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询交易所保证金率响应
  virtual void OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField* pExchangeMarginRate,
                                          CThostFtdcRspInfoField* pRspInfo,
                                          int nRequestID,
                                          bool bIsLast) override {
    callback_.ctp_trader_query_exchange_margin_rate_rsp_(pExchangeMarginRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询交易所调整保证金率响应
  virtual void OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField* pExchangeMarginRateAdjust,
                                                CThostFtdcRspInfoField* pRspInfo,
                                                int nRequestID,
                                                bool bIsLast) override {
    callback_.ctp_trader_query_exchange_margin_rate_adjust_rsp_(pExchangeMarginRateAdjust, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询汇率响应
  virtual void OnRspQryExchangeRate(CThostFtdcExchangeRateField* pExchangeRate,
                                    CThostFtdcRspInfoField* pRspInfo,
                                    int nRequestID,
                                    bool bIsLast) override {
    callback_.ctp_trader_query_exchange_rate_rsp_(pExchangeRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询二级代理操作员银期权限响应
  virtual void OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField* pSecAgentACIDMap,
                                       CThostFtdcRspInfoField* pRspInfo,
                                       int nRequestID,
                                       bool bIsLast) override {
    callback_.ctp_trader_query_secagent_acidmap_rsp_(pSecAgentACIDMap, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询产品报价汇率
  virtual void OnRspQryProductExchRate(CThostFtdcProductExchRateField* pProductExchRate,
                                       CThostFtdcRspInfoField* pRspInfo,
                                       int nRequestID,
                                       bool bIsLast) override {
    callback_.ctp_trader_query_product_exchange_rate_(pProductExchRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询产品组
  virtual void OnRspQryProductGroup(CThostFtdcProductGroupField* pProductGroup,
                                    CThostFtdcRspInfoField* pRspInfo,
                                    int nRequestID,
                                    bool bIsLast) override {
    callback_.ctp_trader_query_product_group_rsp_(pProductGroup, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询做市商合约手续费率响应
  virtual void OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField* pMMInstrumentCommissionRate,
                                                  CThostFtdcRspInfoField* pRspInfo,
                                                  int nRequestID,
                                                  bool bIsLast) override {
    callback_.ctp_trader_query_mmintrument_commission_rate_rsp_(pMMInstrumentCommissionRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询做市商期权合约手续费响应
  virtual void OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField* pMMOptionInstrCommRate,
                                             CThostFtdcRspInfoField* pRspInfo,
                                             int nRequestID,
                                             bool bIsLast) override {
    callback_.ctp_trader_query_mmoption_instrument_commission_rate_rsp_(pMMOptionInstrCommRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询报单手续费响应
  virtual void OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField* pInstrumentOrderCommRate,
                                               CThostFtdcRspInfoField* pRspInfo,
                                               int nRequestID,
                                               bool bIsLast) override {
    callback_.ctp_trader_query_instrument_order_commission_rate_rsp_(pInstrumentOrderCommRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询期权交易成本响应
  virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField* pOptionInstrTradeCost,
                                            CThostFtdcRspInfoField* pRspInfo,
                                            int nRequestID,
                                            bool bIsLast) override {
    callback_.ctp_trader_query_option_instrument_trade_cost_rsp_(pOptionInstrTradeCost, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询期权合约手续费响应
  virtual void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField* pOptionInstrCommRate,
                                           CThostFtdcRspInfoField* pRspInfo,
                                           int nRequestID,
                                           bool bIsLast) override {
    callback_.ctp_trader_query_option_instrument_commission_rate_rsp_(pOptionInstrCommRate, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询执行宣告响应
  virtual void OnRspQryExecOrder(CThostFtdcExecOrderField* pExecOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_exec_order_rsp_(pExecOrder, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询询价响应
  virtual void OnRspQryForQuote(CThostFtdcForQuoteField* pForQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_forquote_rsp_(pForQuote, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询报价响应
  virtual void OnRspQryQuote(CThostFtdcQuoteField* pQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_quote_rsp_(pQuote, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询组合合约安全系数响应
  virtual void OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField* pCombInstrumentGuard,
                                           CThostFtdcRspInfoField* pRspInfo,
                                           int nRequestID,
                                           bool bIsLast) override {
    callback_.ctp_trader_query_comb_instrument_guard_rsp_(pCombInstrumentGuard, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询申请组合响应
  virtual void OnRspQryCombAction(CThostFtdcCombActionField* pCombAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_query_comb_action_rsp_(pCombAction, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询转帐流水响应
  virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField* pTransferSerial,
                                      CThostFtdcRspInfoField* pRspInfo,
                                      int nRequestID,
                                      bool bIsLast) override {
    callback_.ctp_trader_query_transfer_serial_rsp_(pTransferSerial, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询银期签约关系响应
  virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField* pAccountregister,
                                       CThostFtdcRspInfoField* pRspInfo,
                                       int nRequestID,
                                       bool bIsLast) override {
    callback_.ctp_trader_query_account_register_rsp_(pAccountregister, pRspInfo, nRequestID, bIsLast);
  };

  ///错误应答
  virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override {
    callback_.ctp_trader_error_rsp_(pRspInfo, nRequestID, bIsLast);
  };

  ///报单通知
  virtual void OnRtnOrder(CThostFtdcOrderField* pOrder) override { callback_.ctp_trader_return_order_rsp_(pOrder); };

  ///成交通知
  virtual void OnRtnTrade(CThostFtdcTradeField* pTrade) override { callback_.ctp_trader_return_trade_rsp_(pTrade); };

  ///报单录入错误回报
  virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_order_insert_error_(pInputOrder, pRspInfo);
  };

  ///报单操作错误回报
  virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField* pOrderAction, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_order_action_error_(pOrderAction, pRspInfo);
  };

  ///合约交易状态通知
  virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField* pInstrumentStatus) override {
    callback_.ctp_trader_return_instrument_status_(pInstrumentStatus);
  };

  ///交易所公告通知
  virtual void OnRtnBulletin(CThostFtdcBulletinField* pBulletin) override { callback_.ctp_trader_return_bulletin_(pBulletin); };

  ///交易通知
  virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField* pTradingNoticeInfo) override {
    callback_.ctp_trader_return_trading_notice_(pTradingNoticeInfo);
  };

  ///提示条件单校验错误
  virtual void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField* pErrorConditionalOrder) override {
    callback_.ctp_trader_return_error_conditional_order_(pErrorConditionalOrder);
  };

  ///执行宣告通知
  virtual void OnRtnExecOrder(CThostFtdcExecOrderField* pExecOrder) override { callback_.ctp_trader_return_exec_order_(pExecOrder); };

  ///执行宣告录入错误回报
  virtual void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField* pInputExecOrder, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_exec_order_insert_error_(pInputExecOrder, pRspInfo);
  };

  ///执行宣告操作错误回报
  virtual void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField* pExecOrderAction, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_excc_order_action_error_(pExecOrderAction, pRspInfo);
  };

  ///询价录入错误回报
  virtual void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField* pInputForQuote, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_forquote_insert_error_(pInputForQuote, pRspInfo);
  };

  ///报价通知
  virtual void OnRtnQuote(CThostFtdcQuoteField* pQuote) override { callback_.ctp_trader_return_quote_(pQuote); };

  ///报价录入错误回报
  virtual void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField* pInputQuote, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_quote_insert_error_(pInputQuote, pRspInfo);
  };

  ///报价操作错误回报
  virtual void OnErrRtnQuoteAction(CThostFtdcQuoteActionField* pQuoteAction, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_quote_action_error_(pQuoteAction, pRspInfo);
  };

  ///询价通知
  virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) override { callback_.ctp_trader_return_forquote_rsp_(pForQuoteRsp); };

  ///保证金监控中心用户令牌
  virtual void OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField* pCFMMCTradingAccountToken) override {
    callback_.ctp_trader_return_fmmct_trading_account_token_(pCFMMCTradingAccountToken);
  };

  ///批量报单操作错误回报
  virtual void OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField* pBatchOrderAction, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_batch_order_action_error_(pBatchOrderAction, pRspInfo);
  };

  ///申请组合通知
  virtual void OnRtnCombAction(CThostFtdcCombActionField* pCombAction) override { callback_.ctp_trader_return_comb_action_(pCombAction); };

  ///申请组合录入错误回报
  virtual void OnErrRtnCombActionInsert(CThostFtdcInputCombActionField* pInputCombAction, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_comb_action_insert_error_(pInputCombAction, pRspInfo);
  };

  ///请求查询签约银行响应
  virtual void OnRspQryContractBank(CThostFtdcContractBankField* pContractBank,
                                    CThostFtdcRspInfoField* pRspInfo,
                                    int nRequestID,
                                    bool bIsLast) override {
    callback_.ctp_trader_query_contract_bank_rsp_(pContractBank, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询预埋单响应
  virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField* pParkedOrder,
                                   CThostFtdcRspInfoField* pRspInfo,
                                   int nRequestID,
                                   bool bIsLast) override {
    callback_.ctp_trader_query_packed_order_rsp_(pParkedOrder, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询预埋撤单响应
  virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField* pParkedOrderAction,
                                         CThostFtdcRspInfoField* pRspInfo,
                                         int nRequestID,
                                         bool bIsLast) override {
    callback_.ctp_trader_query_packed_order_action_rsp_(pParkedOrderAction, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询交易通知响应
  virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField* pTradingNotice,
                                     CThostFtdcRspInfoField* pRspInfo,
                                     int nRequestID,
                                     bool bIsLast) override {
    callback_.ctp_trader_query_trading_notice_rsp_(pTradingNotice, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询经纪公司交易参数响应
  virtual void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField* pBrokerTradingParams,
                                           CThostFtdcRspInfoField* pRspInfo,
                                           int nRequestID,
                                           bool bIsLast) override {
    callback_.ctp_trader_query_broker_trading_params_rsp_(pBrokerTradingParams, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询经纪公司交易算法响应
  virtual void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField* pBrokerTradingAlgos,
                                          CThostFtdcRspInfoField* pRspInfo,
                                          int nRequestID,
                                          bool bIsLast) override {
    callback_.ctp_trader_query_broker_trading_algos_rsp_(pBrokerTradingAlgos, pRspInfo, nRequestID, bIsLast);
  };

  ///请求查询监控中心用户令牌
  virtual void OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField* pQueryCFMMCTradingAccountToken,
                                                  CThostFtdcRspInfoField* pRspInfo,
                                                  int nRequestID,
                                                  bool bIsLast) override {
    callback_.ctp_trader_query_cfmmc_trading_account_token_rsp_(pQueryCFMMCTradingAccountToken, pRspInfo, nRequestID, bIsLast);
  };

  ///银行发起银行资金转期货通知
  virtual void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField* pRspTransfer) override {
    callback_.ctp_trader_return_from_bank_to_future_by_bank_(pRspTransfer);
  };

  ///银行发起期货资金转银行通知
  virtual void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField* pRspTransfer) override {
    callback_.ctp_trader_return_from_future_to_bank_by_bank_(pRspTransfer);
  };

  ///银行发起冲正银行转期货通知
  virtual void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField* pRspRepeal) override {
    callback_.ctp_trader_return_repeal_from_bank_to_future_by_bank_(pRspRepeal);
  };

  ///银行发起冲正期货转银行通知
  virtual void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField* pRspRepeal) override {
    callback_.ctp_trader_return_repeal_from_future_to_bank_by_bank_(pRspRepeal);
  };

  ///期货发起银行资金转期货通知
  virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField* pRspTransfer) override {
    callback_.ctp_trader_return_from_bank_to_future_by_future_(pRspTransfer);
  };

  ///期货发起期货资金转银行通知
  virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField* pRspTransfer) override {
    callback_.ctp_trader_return_from_future_to_bank_by_future_(pRspTransfer);
  };

  ///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
  virtual void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField* pRspRepeal) override {
    callback_.ctp_trader_return_repeal_from_bank_to_future_by_futuremanual_(pRspRepeal);
  };

  ///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
  virtual void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField* pRspRepeal) override {
    callback_.ctp_trader_return_repeal_from_future_to_bank_by_futuremanual_(pRspRepeal);
  };

  ///期货发起查询银行余额通知
  virtual void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField* pNotifyQueryAccount) override {
    callback_.ctp_trader_return_query_bank_balance_by_future_(pNotifyQueryAccount);
  };

  ///期货发起银行资金转期货错误回报
  virtual void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField* pReqTransfer, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_bank_to_future_by_future_error_(pReqTransfer, pRspInfo);
  };

  ///期货发起期货资金转银行错误回报
  virtual void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField* pReqTransfer, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_future_to_bank_by_future_error_(pReqTransfer, pRspInfo);
  };

  ///系统运行时期货端手工发起冲正银行转期货错误回报
  virtual void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField* pReqRepeal, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_repeal_bank_to_future_by_futuremanual_error_(pReqRepeal, pRspInfo);
  };

  ///系统运行时期货端手工发起冲正期货转银行错误回报
  virtual void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField* pReqRepeal, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_repeal_future_to_bank_by_futuremanual_error_(pReqRepeal, pRspInfo);
  };

  ///期货发起查询银行余额错误回报
  virtual void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField* pReqQueryAccount, CThostFtdcRspInfoField* pRspInfo) override {
    callback_.ctp_trader_return_query_bank_balance_by_future_error_(pReqQueryAccount, pRspInfo);
  };

  ///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
  virtual void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField* pRspRepeal) override {
    callback_.ctp_trader_return_repeal_from_bank_to_future_by_future_(pRspRepeal);
  };

  ///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
  virtual void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField* pRspRepeal) override {
    callback_.ctp_trader_return_repeal_from_future_to_bank_by_future_(pRspRepeal);
  };

  ///期货发起银行资金转期货应答
  virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField* pReqTransfer,
                                             CThostFtdcRspInfoField* pRspInfo,
                                             int nRequestID,
                                             bool bIsLast) override {
    callback_.ctp_trader_from_bank_to_future_by_future_rsp_(pReqTransfer, pRspInfo, nRequestID, bIsLast);
  };

  ///期货发起期货资金转银行应答
  virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField* pReqTransfer,
                                             CThostFtdcRspInfoField* pRspInfo,
                                             int nRequestID,
                                             bool bIsLast) override {
    callback_.ctp_trader_from_future_to_bank_by_future_rsp_(pReqTransfer, pRspInfo, nRequestID, bIsLast);
  };

  ///期货发起查询银行余额应答
  virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField* pReqQueryAccount,
                                                  CThostFtdcRspInfoField* pRspInfo,
                                                  int nRequestID,
                                                  bool bIsLast) override {
    callback_.ctp_trader_query_bank_account_monoy_by_future_rsp_(pReqQueryAccount, pRspInfo, nRequestID, bIsLast);
  };

  ///银行发起银期开户通知
  virtual void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField* pOpenAccount) override {
    callback_.ctp_trader_return_open_account_by_bank_(pOpenAccount);
  };

  ///银行发起银期销户通知
  virtual void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField* pCancelAccount) override {
    callback_.ctp_trader_return_cancel_account_by_bank_(pCancelAccount);
  };

  ///银行发起变更银行账号通知
  virtual void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField* pChangeAccount) override {
    callback_.ctp_trader_return_change_account_by_bank_(pChangeAccount);
  };

  ///完成回调的设置
  void SetOnFrontConnected(ctp_trader_front_connected_cb cb) { callback_.ctp_trader_front_connected_ = cb; }
  void SetOnFrontDisconnected(ctp_trader_front_disconnected_cb cb) { callback_.ctp_trader_front_disconnected_ = cb; }
  void SetOnHeartBeatWarning(ctp_trader_heart_beat_warning_cb cb) { callback_.ctp_trader_heart_beat_warning_ = cb; }
  void SetOnRspAuthenticate(ctp_trader_authenticate_rsp_cb cb) { callback_.ctp_trader_authenticate_rsp_ = cb; }
  void SetOnRspUserLogin(ctp_trader_user_login_rsp_cb cb) { callback_.ctp_trader_user_login_rsp_ = cb; }
  void SetOnRspUserLogout(ctp_trader_user_logout_rsp_cb cb) { callback_.ctp_trader_user_logout_rsp_ = cb; }
  void SetOnRspUserPasswordUpdate(ctp_trader_user_passwd_update_rsp_cb cb) { callback_.ctp_trader_user_passwd_update_rsp_ = cb; }
  void SetOnRspTradingAccountPasswordUpdate(ctp_trader_trading_account_passwd_update_cb cb) {
    callback_.ctp_trader_trading_account_passwd_update_ = cb;
  }
  void SetOnRspOrderInsert(ctp_trader_order_insert_rsp_cb cb) { callback_.ctp_trader_order_insert_rsp_ = cb; }
  void SetOnRspParkedOrderInsert(ctp_trader_parked_order_insert_rsp_cb cb) { callback_.ctp_trader_parked_order_insert_rsp_ = cb; }
  void SetOnRspParkedOrderAction(ctp_trader_packed_order_action_rsp_cb cb) { callback_.ctp_trader_packed_order_action_rsp_ = cb; }
  void SetOnRspOrderAction(ctp_trader_order_action_rsp_cb cb) { callback_.ctp_trader_order_action_rsp_ = cb; }
  void SetOnRspQueryMaxOrderVolume(ctp_trader_query_max_order_volume_rsp_cb cb) { callback_.ctp_trader_query_max_order_volume_rsp_ = cb; }
  void SetOnRspSettlementInfoConfirm(ctp_trader_settlement_info_confirm_rsp_cb cb) { callback_.ctp_trader_settlement_info_confirm_rsp_ = cb; }
  void SetOnRspRemoveParkedOrder(ctp_trader_remove_packed_order_rsp_cb cb) { callback_.ctp_trader_remove_packed_order_rsp_ = cb; }
  void SetOnRspRemoveParkedOrderAction(ctp_trader_remove_packed_order_action_rsp_cb cb) { callback_.ctp_trader_remove_packed_order_action_rsp_ = cb; }
  void SetOnRspExecOrderInsert(ctp_trader_exec_order_insert_rsp_cb cb) { callback_.ctp_trader_exec_order_insert_rsp_ = cb; }
  void SetOnRspExecOrderAction(ctp_trader_exec_order_action_rsp_cb cb) { callback_.ctp_trader_exec_order_action_rsp_ = cb; }
  void SetOnRspForQuoteInsert(ctp_trader_for_quote_insert_rsp_cb cb) { callback_.ctp_trader_for_quote_insert_rsp_ = cb; }
  void SetOnRspQuoteInsert(ctp_trader_quote_insert_rsp_cb cb) { callback_.ctp_trader_quote_insert_rsp_ = cb; }
  void SetOnRspQuoteAction(ctp_trader_quote_action_rsp_cb cb) { callback_.ctp_trader_quote_action_rsp_ = cb; }
  void SetOnRspBatchOrderAction(ctp_trader_batch_order_action_rsp_cb cb) { callback_.ctp_trader_batch_order_action_rsp_ = cb; }
  void SetOnRspCombActionInsert(ctp_trader_comb_action_insert_rsp_cb cb) { callback_.ctp_trader_comb_action_insert_rsp_ = cb; }
  void SetOnRspQryOrder(ctp_trader_query_order_rsp_cb cb) { callback_.ctp_trader_query_order_rsp_ = cb; }
  void SetOnRspQryTrade(ctp_trader_query_trade_rsp_cb cb) { callback_.ctp_trader_query_trade_rsp_ = cb; }
  void SetOnRspQryInvestorPosition(ctp_trader_query_investor_position_rsp_cb cb) { callback_.ctp_trader_query_investor_position_rsp_ = cb; }
  void SetOnRspQryTradingAccount(ctp_trader_query_trading_account_rsp_cb cb) { callback_.ctp_trader_query_trading_account_rsp_ = cb; }
  void SetOnRspQryInvestor(ctp_trader_query_investor_rsp_cb cb) { callback_.ctp_trader_query_investor_rsp_ = cb; }
  void SetOnRspQryTradingCode(ctp_trader_query_trading_code_rsp_cb cb) { callback_.ctp_trader_query_trading_code_rsp_ = cb; }
  void SetOnRspQryInstrumentMarginRate(ctp_trader_query_instrument_margin_rate_rsp_cb cb) {
    callback_.ctp_trader_query_instrument_margin_rate_rsp_ = cb;
  }
  void SetOnRspQryInstrumentCommissionRate(ctp_trader_query_instrument_commission_rate_rsp_cb cb) {
    callback_.ctp_trader_query_instrument_commission_rate_rsp_ = cb;
  }
  void SetOnRspQryExchange(ctp_trader_query_exchange_rsp_cb cb) { callback_.ctp_trader_query_exchange_rsp_ = cb; }
  void SetOnRspQryProduct(ctp_trader_query_product_rsp_cb cb) { callback_.ctp_trader_query_product_rsp_ = cb; }
  void SetOnRspQryInstrument(ctp_trader_query_instrument_rsp_cb cb) { callback_.ctp_trader_query_instrument_rsp_ = cb; }
  void SetOnRspQryDepthMarketData(ctp_trader_query_depth_market_data_rsp_cb cb) { callback_.ctp_trader_query_depth_market_data_rsp_ = cb; }
  void SetOnRspQrySettlementInfo(ctp_trader_query_settlement_info_rsp_cb cb) { callback_.ctp_trader_query_settlement_info_rsp_ = cb; }
  void SetOnRspQryTransferBank(ctp_trader_query_transfer_bank_rsp_cb cb) { callback_.ctp_trader_query_transfer_bank_rsp_ = cb; }
  void SetOnRspQryInvestorPositionDetail(ctp_trader_query_investor_position_detail_rsp_cb cb) {
    callback_.ctp_trader_query_investor_position_detail_rsp_ = cb;
  }
  void SetOnRspQryNotice(ctp_trader_query_notice_rsp_cb cb) { callback_.ctp_trader_query_notice_rsp_ = cb; }
  void SetOnRspQrySettlementInfoConfirm(ctp_trader_query_settlement_info_confirm_rsp_cb cb) {
    callback_.ctp_trader_query_settlement_info_confirm_rsp_ = cb;
  }
  void SetOnRspQryInvestorPositionCombineDetail(ctp_trader_query_investor_position_combine_detail_rsp_cb cb) {
    callback_.ctp_trader_query_investor_position_combine_detail_rsp_ = cb;
  }
  void SetOnRspQryCFMMCTradingAccountKey(ctp_trader_query_cfmmc_trading_account_key_rsp_cb cb) {
    callback_.ctp_trader_query_cfmmc_trading_account_key_rsp_ = cb;
  }
  void SetOnRspQryEWarrantOffset(ctp_trader_query_ewarrant_offset_rsp_cb cb) { callback_.ctp_trader_query_ewarrant_offset_rsp_ = cb; }
  void SetOnRspQryInvestorProductGroupMargin(ctp_trader_query_investor_product_group_margin_rsp_cb cb) {
    callback_.ctp_trader_query_investor_product_group_margin_rsp_ = cb;
  }
  void SetOnRspQryExchangeMarginRate(ctp_trader_query_exchange_margin_rate_rsp_cb cb) { callback_.ctp_trader_query_exchange_margin_rate_rsp_ = cb; }
  void SetOnRspQryExchangeMarginRateAdjust(ctp_trader_query_exchange_margin_rate_adjust_rsp_cb cb) {
    callback_.ctp_trader_query_exchange_margin_rate_adjust_rsp_ = cb;
  }
  void SetOnRspQryExchangeRate(ctp_trader_query_exchange_rate_rsp_cb cb) { callback_.ctp_trader_query_exchange_rate_rsp_ = cb; }
  void SetOnRspQrySecAgentACIDMap(ctp_trader_query_secagent_acidmap_rsp_cb cb) { callback_.ctp_trader_query_secagent_acidmap_rsp_ = cb; }
  void SetOnRspQryProductExchRate(ctp_trader_query_product_exchange_rate_cb cb) { callback_.ctp_trader_query_product_exchange_rate_ = cb; }
  void SetOnRspQryProductGroup(ctp_trader_query_product_group_rsp_cb cb) { callback_.ctp_trader_query_product_group_rsp_ = cb; }
  void SetOnRspQryMMInstrumentCommissionRate(ctp_trader_query_mmintrument_commission_rate_rsp_cb cb) {
    callback_.ctp_trader_query_mmintrument_commission_rate_rsp_ = cb;
  }
  void SetOnRspQryMMOptionInstrCommRate(ctp_trader_query_mmoption_instrument_commission_rate_rsp_cb cb) {
    callback_.ctp_trader_query_mmoption_instrument_commission_rate_rsp_ = cb;
  }
  void SetOnRspQryInstrumentOrderCommRate(ctp_trader_query_instrument_order_commission_rate_rsp_cb cb) {
    callback_.ctp_trader_query_instrument_order_commission_rate_rsp_ = cb;
  }
  void SetOnRspQryOptionInstrTradeCost(ctp_trader_query_option_instrument_trade_cost_rsp_cb cb) {
    callback_.ctp_trader_query_option_instrument_trade_cost_rsp_ = cb;
  }
  void SetOnRspQryOptionInstrCommRate(ctp_trader_query_option_instrument_commission_rate_rsp_cb cb) {
    callback_.ctp_trader_query_option_instrument_commission_rate_rsp_ = cb;
  }
  void SetOnRspQryExecOrder(ctp_trader_query_exec_order_rsp_cb cb) { callback_.ctp_trader_query_exec_order_rsp_ = cb; }
  void SetOnRspQryForQuote(ctp_trader_query_forquote_rsp_cb cb) { callback_.ctp_trader_query_forquote_rsp_ = cb; }
  void SetOnRspQryQuote(ctp_trader_query_quote_rsp_cb cb) { callback_.ctp_trader_query_quote_rsp_ = cb; }
  void SetOnRspQryCombInstrumentGuard(ctp_trader_query_comb_instrument_guard_rsp_cb cb) {
    callback_.ctp_trader_query_comb_instrument_guard_rsp_ = cb;
  }
  void SetOnRspQryCombAction(ctp_trader_query_comb_action_rsp_cb cb) { callback_.ctp_trader_query_comb_action_rsp_ = cb; }
  void SetOnRspQryTransferSerial(ctp_trader_query_transfer_serial_rsp_cb cb) { callback_.ctp_trader_query_transfer_serial_rsp_ = cb; }
  void SetOnRspQryAccountregister(ctp_trader_query_account_register_rsp_cb cb) { callback_.ctp_trader_query_account_register_rsp_ = cb; }
  void SetOnRspError(ctp_trader_error_rsp_cb cb) { callback_.ctp_trader_error_rsp_ = cb; }
  void SetOnRtnOrder(ctp_trader_return_order_rsp_cb cb) { callback_.ctp_trader_return_order_rsp_ = cb; }
  void SetOnRtnTrade(ctp_trader_return_trade_rsp_cb cb) { callback_.ctp_trader_return_trade_rsp_ = cb; }
  void SetOnErrRtnOrderInsert(ctp_trader_return_order_insert_error_cb cb) { callback_.ctp_trader_return_order_insert_error_ = cb; }
  void SetOnErrRtnOrderAction(ctp_trader_return_order_action_error_cb cb) { callback_.ctp_trader_return_order_action_error_ = cb; }
  void SetOnRtnInstrumentStatus(ctp_trader_return_instrument_status_cb cb) { callback_.ctp_trader_return_instrument_status_ = cb; }
  void SetOnRtnBulletin(ctp_trader_return_bulletin_cb cb) { callback_.ctp_trader_return_bulletin_ = cb; }
  void SetOnRtnTradingNotice(ctp_trader_return_trading_notice_cb cb) { callback_.ctp_trader_return_trading_notice_ = cb; }
  void SetOnRtnErrorConditionalOrder(ctp_trader_return_error_conditional_order_cb cb) { callback_.ctp_trader_return_error_conditional_order_ = cb; }
  void SetOnRtnExecOrder(ctp_trader_return_exec_order_cb cb) { callback_.ctp_trader_return_exec_order_ = cb; }
  void SetOnErrRtnExecOrderInsert(ctp_trader_return_exec_order_insert_error_cb cb) { callback_.ctp_trader_return_exec_order_insert_error_ = cb; }
  void SetOnErrRtnExecOrderAction(ctp_trader_return_excc_order_action_error_cb cb) { callback_.ctp_trader_return_excc_order_action_error_ = cb; }
  void SetOnErrRtnForQuoteInsert(ctp_trader_return_forquote_insert_error_cb cb) { callback_.ctp_trader_return_forquote_insert_error_ = cb; }
  void SetOnRtnQuote(ctp_trader_return_quote_cb cb) { callback_.ctp_trader_return_quote_ = cb; }
  void SetOnErrRtnQuoteInsert(ctp_trader_return_quote_insert_error_cb cb) { callback_.ctp_trader_return_quote_insert_error_ = cb; }
  void SetOnErrRtnQuoteAction(ctp_trader_return_quote_action_error_cb cb) { callback_.ctp_trader_return_quote_action_error_ = cb; }
  void SetOnRtnForQuoteRsp(ctp_trader_return_forquote_rsp_cb cb) { callback_.ctp_trader_return_forquote_rsp_ = cb; }
  void SetOnRtnCFMMCTradingAccountToken(ctp_trader_return_fmmct_trading_account_token_cb cb) {
    callback_.ctp_trader_return_fmmct_trading_account_token_ = cb;
  }
  void SetOnErrRtnBatchOrderAction(ctp_trader_return_batch_order_action_error_cb cb) { callback_.ctp_trader_return_batch_order_action_error_ = cb; }
  void SetOnRtnCombAction(ctp_trader_return_comb_action_cb cb) { callback_.ctp_trader_return_comb_action_ = cb; }
  void SetOnErrRtnCombActionInsert(ctp_trader_return_comb_action_insert_error_cb cb) { callback_.ctp_trader_return_comb_action_insert_error_ = cb; }
  void SetOnRspQryContractBank(ctp_trader_query_contract_bank_rsp_cb cb) { callback_.ctp_trader_query_contract_bank_rsp_ = cb; }
  void SetOnRspQryParkedOrder(ctp_trader_query_packed_order_rsp_cb cb) { callback_.ctp_trader_query_packed_order_rsp_ = cb; }
  void SetOnRspQryParkedOrderAction(ctp_trader_query_packed_order_action_rsp_cb cb) { callback_.ctp_trader_query_packed_order_action_rsp_ = cb; }
  void SetOnRspQryTradingNotice(ctp_trader_query_trading_notice_rsp_cb cb) { callback_.ctp_trader_query_trading_notice_rsp_ = cb; }
  void SetOnRspQryBrokerTradingParams(ctp_trader_query_broker_trading_params_rsp_cb cb) {
    callback_.ctp_trader_query_broker_trading_params_rsp_ = cb;
  }
  void SetOnRspQryBrokerTradingAlgos(ctp_trader_query_broker_trading_algos_rsp_cb cb) { callback_.ctp_trader_query_broker_trading_algos_rsp_ = cb; }
  void SetOnRspQueryCFMMCTradingAccountToken(ctp_trader_query_cfmmc_trading_account_token_rsp_cb cb) {
    callback_.ctp_trader_query_cfmmc_trading_account_token_rsp_ = cb;
  }
  void SetOnRtnFromBankToFutureByBank(ctp_trader_return_from_bank_to_future_by_bank_cb cb) {
    callback_.ctp_trader_return_from_bank_to_future_by_bank_ = cb;
  }
  void SetOnRtnFromFutureToBankByBank(ctp_trader_return_from_future_to_bank_by_bank_cb cb) {
    callback_.ctp_trader_return_from_future_to_bank_by_bank_ = cb;
  }
  void SetOnRtnRepealFromBankToFutureByBank(ctp_trader_return_repeal_from_bank_to_future_by_bank_cb cb) {
    callback_.ctp_trader_return_repeal_from_bank_to_future_by_bank_ = cb;
  }
  void SetOnRtnRepealFromFutureToBankByBank(ctp_trader_return_repeal_from_future_to_bank_by_bank_cb cb) {
    callback_.ctp_trader_return_repeal_from_future_to_bank_by_bank_ = cb;
  }
  void SetOnRtnFromBankToFutureByFuture(ctp_trader_return_from_bank_to_future_by_future_cb cb) {
    callback_.ctp_trader_return_from_bank_to_future_by_future_ = cb;
  }
  void SetOnRtnFromFutureToBankByFuture(ctp_trader_return_from_future_to_bank_by_future_cb cb) {
    callback_.ctp_trader_return_from_future_to_bank_by_future_ = cb;
  }
  void SetOnRtnRepealFromBankToFutureByFutureManual(ctp_trader_return_repeal_from_bank_to_future_by_futuremanual_cb cb) {
    callback_.ctp_trader_return_repeal_from_bank_to_future_by_futuremanual_ = cb;
  }
  void SetOnRtnRepealFromFutureToBankByFutureManual(ctp_trader_return_repeal_from_future_to_bank_by_futuremanual_cb cb) {
    callback_.ctp_trader_return_repeal_from_future_to_bank_by_futuremanual_ = cb;
  }
  void SetOnRtnQueryBankBalanceByFuture(ctp_trader_return_query_bank_balance_by_future_cb cb) {
    callback_.ctp_trader_return_query_bank_balance_by_future_ = cb;
  }
  void SetOnErrRtnBankToFutureByFuture(ctp_trader_return_bank_to_future_by_future_error_cb cb) {
    callback_.ctp_trader_return_bank_to_future_by_future_error_ = cb;
  }
  void SetOnErrRtnFutureToBankByFuture(ctp_trader_return_future_to_bank_by_future_error_cb cb) {
    callback_.ctp_trader_return_future_to_bank_by_future_error_ = cb;
  }
  void SetOnErrRtnRepealBankToFutureByFutureManual(ctp_trader_return_repeal_bank_to_future_by_futuremanual_error_cb cb) {
    callback_.ctp_trader_return_repeal_bank_to_future_by_futuremanual_error_ = cb;
  }
  void SetOnErrRtnRepealFutureToBankByFutureManual(ctp_trader_return_repeal_future_to_bank_by_futuremanual_error_cb cb) {
    callback_.ctp_trader_return_repeal_future_to_bank_by_futuremanual_error_ = cb;
  }
  void SetOnErrRtnQueryBankBalanceByFuture(ctp_trader_return_query_bank_balance_by_future_error_cb cb) {
    callback_.ctp_trader_return_query_bank_balance_by_future_error_ = cb;
  }
  void SetOnRtnRepealFromBankToFutureByFuture(ctp_trader_return_repeal_from_bank_to_future_by_future_cb cb) {
    callback_.ctp_trader_return_repeal_from_bank_to_future_by_future_ = cb;
  }
  void SetOnRtnRepealFromFutureToBankByFuture(ctp_trader_return_repeal_from_future_to_bank_by_future_cb cb) {
    callback_.ctp_trader_return_repeal_from_future_to_bank_by_future_ = cb;
  }
  void SetOnRspFromBankToFutureByFuture(ctp_trader_from_bank_to_future_by_future_rsp_cb cb) {
    callback_.ctp_trader_from_bank_to_future_by_future_rsp_ = cb;
  }
  void SetOnRspFromFutureToBankByFuture(ctp_trader_from_future_to_bank_by_future_rsp_cb cb) {
    callback_.ctp_trader_from_future_to_bank_by_future_rsp_ = cb;
  }
  void SetOnRspQueryBankAccountMoneyByFuture(ctp_trader_query_bank_account_monoy_by_future_rsp_cb cb) {
    callback_.ctp_trader_query_bank_account_monoy_by_future_rsp_ = cb;
  }
  void SetOnRtnOpenAccountByBank(ctp_trader_return_open_account_by_bank_cb cb) { callback_.ctp_trader_return_open_account_by_bank_ = cb; }
  void SetOnRtnCancelAccountByBank(ctp_trader_return_cancel_account_by_bank_cb cb) { callback_.ctp_trader_return_cancel_account_by_bank_ = cb; }
  void SetOnRtnChangeAccountByBank(ctp_trader_return_change_account_by_bank_cb cb) { callback_.ctp_trader_return_change_account_by_bank_ = cb; }

 private:
  CtpTraderSpiCallback callback_;
};

//==================================================================================
//
// 定义被外部使用的C struct
//
//==================================================================================

struct ctp_trader_spi_t {
  CThostFtdcTraderSpiImpl impl;
};

struct ctp_trader_api_t {
  CThostFtdcTraderApi* traderapi = nullptr;
};

//==================================================================================
//
// C代码，外部函数的实现
//
//==================================================================================

extern "C" {

ctp_trader_spi_t* ctp_traderspi_create() {
  return new ctp_trader_spi_t;
}
void ctp_traderspi_free(ctp_trader_spi_t* spi) {
  delete spi;
}

void ctp_traderspi_set_front_connected_cb(ctp_trader_spi_t* spi, ctp_trader_front_connected_cb cb) {
  spi->impl.SetOnFrontConnected(cb);
}
void ctp_traderspi_set_front_disconnected_cb(ctp_trader_spi_t* spi, ctp_trader_front_disconnected_cb cb) {
  spi->impl.SetOnFrontDisconnected(cb);
}
void ctp_traderspi_set_heart_beat_warning_cb(ctp_trader_spi_t* spi, ctp_trader_heart_beat_warning_cb cb) {
  spi->impl.SetOnHeartBeatWarning(cb);
}
void ctp_traderspi_set_authenticate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_authenticate_rsp_cb cb) {
  spi->impl.SetOnRspAuthenticate(cb);
}
void ctp_traderspi_set_user_login_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_user_login_rsp_cb cb) {
  spi->impl.SetOnRspUserLogin(cb);
}
void ctp_traderspi_set_user_logout_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_user_logout_rsp_cb cb) {
  spi->impl.SetOnRspUserLogout(cb);
}
void ctp_traderspi_set_user_passwd_update_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_user_passwd_update_rsp_cb cb) {
  spi->impl.SetOnRspUserPasswordUpdate(cb);
}
void ctp_traderspi_set_trading_account_passwd_update_cb(ctp_trader_spi_t* spi, ctp_trader_trading_account_passwd_update_cb cb) {
  spi->impl.SetOnRspTradingAccountPasswordUpdate(cb);
}
void ctp_traderspi_set_order_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_order_insert_rsp_cb cb) {
  spi->impl.SetOnRspOrderInsert(cb);
}
void ctp_traderspi_set_parked_order_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_parked_order_insert_rsp_cb cb) {
  spi->impl.SetOnRspParkedOrderInsert(cb);
}
void ctp_traderspi_set_packed_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_packed_order_action_rsp_cb cb) {
  spi->impl.SetOnRspParkedOrderAction(cb);
}
void ctp_traderspi_set_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_order_action_rsp_cb cb) {
  spi->impl.SetOnRspOrderAction(cb);
}
void ctp_traderspi_set_query_max_order_volume_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_max_order_volume_rsp_cb cb) {
  spi->impl.SetOnRspQueryMaxOrderVolume(cb);
}
void ctp_traderspi_set_settlement_info_confirm_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_settlement_info_confirm_rsp_cb cb) {
  spi->impl.SetOnRspSettlementInfoConfirm(cb);
}
void ctp_traderspi_set_remove_packed_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_remove_packed_order_rsp_cb cb) {
  spi->impl.SetOnRspRemoveParkedOrder(cb);
}
void ctp_traderspi_set_remove_packed_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_remove_packed_order_action_rsp_cb cb) {
  spi->impl.SetOnRspRemoveParkedOrderAction(cb);
}
void ctp_traderspi_set_exec_order_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_exec_order_insert_rsp_cb cb) {
  spi->impl.SetOnRspExecOrderInsert(cb);
}
void ctp_traderspi_set_exec_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_exec_order_action_rsp_cb cb) {
  spi->impl.SetOnRspExecOrderAction(cb);
}
void ctp_traderspi_set_for_quote_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_for_quote_insert_rsp_cb cb) {
  spi->impl.SetOnRspForQuoteInsert(cb);
}
void ctp_traderspi_set_quote_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_quote_insert_rsp_cb cb) {
  spi->impl.SetOnRspQuoteInsert(cb);
}
void ctp_traderspi_set_quote_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_quote_action_rsp_cb cb) {
  spi->impl.SetOnRspQuoteAction(cb);
}
void ctp_traderspi_set_batch_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_batch_order_action_rsp_cb cb) {
  spi->impl.SetOnRspBatchOrderAction(cb);
}
void ctp_traderspi_set_comb_action_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_comb_action_insert_rsp_cb cb) {
  spi->impl.SetOnRspCombActionInsert(cb);
}
void ctp_traderspi_set_query_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_order_rsp_cb cb) {
  spi->impl.SetOnRspQryOrder(cb);
}
void ctp_traderspi_set_query_trade_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_trade_rsp_cb cb) {
  spi->impl.SetOnRspQryTrade(cb);
}
void ctp_traderspi_set_query_investor_position_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_investor_position_rsp_cb cb) {
  spi->impl.SetOnRspQryInvestorPosition(cb);
}
void ctp_traderspi_set_query_trading_account_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_trading_account_rsp_cb cb) {
  spi->impl.SetOnRspQryTradingAccount(cb);
}
void ctp_traderspi_set_query_investor_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_investor_rsp_cb cb) {
  spi->impl.SetOnRspQryInvestor(cb);
}
void ctp_traderspi_set_query_trading_code_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_trading_code_rsp_cb cb) {
  spi->impl.SetOnRspQryTradingCode(cb);
}
void ctp_traderspi_set_query_instrument_margin_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_instrument_margin_rate_rsp_cb cb) {
  spi->impl.SetOnRspQryInstrumentMarginRate(cb);
}
void ctp_traderspi_set_query_instrument_commission_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_instrument_commission_rate_rsp_cb cb) {
  spi->impl.SetOnRspQryInstrumentCommissionRate(cb);
}
void ctp_traderspi_set_query_exchange_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exchange_rsp_cb cb) {
  spi->impl.SetOnRspQryExchange(cb);
}
void ctp_traderspi_set_query_product_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_product_rsp_cb cb) {
  spi->impl.SetOnRspQryProduct(cb);
}
void ctp_traderspi_set_query_instrument_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_instrument_rsp_cb cb) {
  spi->impl.SetOnRspQryInstrument(cb);
}
void ctp_traderspi_set_query_depth_market_data_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_depth_market_data_rsp_cb cb) {
  spi->impl.SetOnRspQryDepthMarketData(cb);
}
void ctp_traderspi_set_query_settlement_info_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_settlement_info_rsp_cb cb) {
  spi->impl.SetOnRspQrySettlementInfo(cb);
}
void ctp_traderspi_set_query_transfer_bank_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_transfer_bank_rsp_cb cb) {
  spi->impl.SetOnRspQryTransferBank(cb);
}
void ctp_traderspi_set_query_investor_position_detail_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_investor_position_detail_rsp_cb cb) {
  spi->impl.SetOnRspQryInvestorPositionDetail(cb);
}
void ctp_traderspi_set_query_notice_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_notice_rsp_cb cb) {
  spi->impl.SetOnRspQryNotice(cb);
}
void ctp_traderspi_set_query_settlement_info_confirm_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_settlement_info_confirm_rsp_cb cb) {
  spi->impl.SetOnRspQrySettlementInfoConfirm(cb);
}
void ctp_traderspi_set_query_investor_position_combine_detail_rsp_cb(ctp_trader_spi_t* spi,
                                                                     ctp_trader_query_investor_position_combine_detail_rsp_cb cb) {
  spi->impl.SetOnRspQryInvestorPositionCombineDetail(cb);
}
void ctp_traderspi_set_query_cfmmc_trading_account_key_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_cfmmc_trading_account_key_rsp_cb cb) {
  spi->impl.SetOnRspQryCFMMCTradingAccountKey(cb);
}
void ctp_traderspi_set_query_ewarrant_offset_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_ewarrant_offset_rsp_cb cb) {
  spi->impl.SetOnRspQryEWarrantOffset(cb);
}
void ctp_traderspi_set_query_investor_product_group_margin_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_investor_product_group_margin_rsp_cb cb) {
  spi->impl.SetOnRspQryInvestorProductGroupMargin(cb);
}
void ctp_traderspi_set_query_exchange_margin_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exchange_margin_rate_rsp_cb cb) {
  spi->impl.SetOnRspQryExchangeMarginRate(cb);
}
void ctp_traderspi_set_query_exchange_margin_rate_adjust_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exchange_margin_rate_adjust_rsp_cb cb) {
  spi->impl.SetOnRspQryExchangeMarginRateAdjust(cb);
}
void ctp_traderspi_set_query_exchange_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exchange_rate_rsp_cb cb) {
  spi->impl.SetOnRspQryExchangeRate(cb);
}
void ctp_traderspi_set_query_secagent_acidmap_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_secagent_acidmap_rsp_cb cb) {
  spi->impl.SetOnRspQrySecAgentACIDMap(cb);
}
void ctp_traderspi_set_query_product_exchange_rate_cb(ctp_trader_spi_t* spi, ctp_trader_query_product_exchange_rate_cb cb) {
  spi->impl.SetOnRspQryProductExchRate(cb);
}
void ctp_traderspi_set_query_product_group_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_product_group_rsp_cb cb) {
  spi->impl.SetOnRspQryProductGroup(cb);
}
void ctp_traderspi_set_query_mmintrument_commission_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_mmintrument_commission_rate_rsp_cb cb) {
  spi->impl.SetOnRspQryMMInstrumentCommissionRate(cb);
}
void ctp_traderspi_set_query_mmoption_instrument_commission_rate_rsp_cb(ctp_trader_spi_t* spi,
                                                                        ctp_trader_query_mmoption_instrument_commission_rate_rsp_cb cb) {
  spi->impl.SetOnRspQryMMOptionInstrCommRate(cb);
}
void ctp_traderspi_set_query_instrument_order_commission_rate_rsp_cb(ctp_trader_spi_t* spi,
                                                                     ctp_trader_query_instrument_order_commission_rate_rsp_cb cb) {
  spi->impl.SetOnRspQryInstrumentOrderCommRate(cb);
}
void ctp_traderspi_set_query_option_instrument_trade_cost_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_option_instrument_trade_cost_rsp_cb cb) {
  spi->impl.SetOnRspQryOptionInstrTradeCost(cb);
}
void ctp_traderspi_set_query_option_instrument_commission_rate_rsp_cb(ctp_trader_spi_t* spi,
                                                                      ctp_trader_query_option_instrument_commission_rate_rsp_cb cb) {
  spi->impl.SetOnRspQryOptionInstrCommRate(cb);
}
void ctp_traderspi_set_query_exec_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exec_order_rsp_cb cb) {
  spi->impl.SetOnRspQryExecOrder(cb);
}
void ctp_traderspi_set_query_forquote_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_forquote_rsp_cb cb) {
  spi->impl.SetOnRspQryForQuote(cb);
}
void ctp_traderspi_set_query_quote_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_quote_rsp_cb cb) {
  spi->impl.SetOnRspQryQuote(cb);
}
void ctp_traderspi_set_query_comb_instrument_guard_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_comb_instrument_guard_rsp_cb cb) {
  spi->impl.SetOnRspQryCombInstrumentGuard(cb);
}
void ctp_traderspi_set_query_comb_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_comb_action_rsp_cb cb) {
  spi->impl.SetOnRspQryCombAction(cb);
}
void ctp_traderspi_set_query_transfer_serial_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_transfer_serial_rsp_cb cb) {
  spi->impl.SetOnRspQryTransferSerial(cb);
}
void ctp_traderspi_set_query_account_register_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_account_register_rsp_cb cb) {
  spi->impl.SetOnRspQryAccountregister(cb);
}
void ctp_traderspi_set_error_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_error_rsp_cb cb) {
  spi->impl.SetOnRspError(cb);
}
void ctp_traderspi_set_return_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_return_order_rsp_cb cb) {
  spi->impl.SetOnRtnOrder(cb);
}
void ctp_traderspi_set_return_trade_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_return_trade_rsp_cb cb) {
  spi->impl.SetOnRtnTrade(cb);
}
void ctp_traderspi_set_return_order_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_order_insert_error_cb cb) {
  spi->impl.SetOnErrRtnOrderInsert(cb);
}
void ctp_traderspi_set_return_order_action_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_order_action_error_cb cb) {
  spi->impl.SetOnErrRtnOrderAction(cb);
}
void ctp_traderspi_set_return_instrument_status_cb(ctp_trader_spi_t* spi, ctp_trader_return_instrument_status_cb cb) {
  spi->impl.SetOnRtnInstrumentStatus(cb);
}
void ctp_traderspi_set_return_bulletin_cb(ctp_trader_spi_t* spi, ctp_trader_return_bulletin_cb cb) {
  spi->impl.SetOnRtnBulletin(cb);
}
void ctp_traderspi_set_return_trading_notice_cb(ctp_trader_spi_t* spi, ctp_trader_return_trading_notice_cb cb) {
  spi->impl.SetOnRtnTradingNotice(cb);
}
void ctp_traderspi_set_return_error_conditional_order_cb(ctp_trader_spi_t* spi, ctp_trader_return_error_conditional_order_cb cb) {
  spi->impl.SetOnRtnErrorConditionalOrder(cb);
}
void ctp_traderspi_set_return_exec_order_cb(ctp_trader_spi_t* spi, ctp_trader_return_exec_order_cb cb) {
  spi->impl.SetOnRtnExecOrder(cb);
}
void ctp_traderspi_set_return_exec_order_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_exec_order_insert_error_cb cb) {
  spi->impl.SetOnErrRtnExecOrderInsert(cb);
}
void ctp_traderspi_set_return_excc_order_action_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_excc_order_action_error_cb cb) {
  spi->impl.SetOnErrRtnExecOrderAction(cb);
}
void ctp_traderspi_set_return_forquote_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_forquote_insert_error_cb cb) {
  spi->impl.SetOnErrRtnForQuoteInsert(cb);
}
void ctp_traderspi_set_return_quote_cb(ctp_trader_spi_t* spi, ctp_trader_return_quote_cb cb) {
  spi->impl.SetOnRtnQuote(cb);
}
void ctp_traderspi_set_return_quote_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_quote_insert_error_cb cb) {
  spi->impl.SetOnErrRtnQuoteInsert(cb);
}
void ctp_traderspi_set_return_quote_action_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_quote_action_error_cb cb) {
  spi->impl.SetOnErrRtnQuoteAction(cb);
}
void ctp_traderspi_set_return_forquote_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_return_forquote_rsp_cb cb) {
  spi->impl.SetOnRtnForQuoteRsp(cb);
}
void ctp_traderspi_set_return_fmmct_trading_account_token_cb(ctp_trader_spi_t* spi, ctp_trader_return_fmmct_trading_account_token_cb cb) {
  spi->impl.SetOnRtnCFMMCTradingAccountToken(cb);
}
void ctp_traderspi_set_return_batch_order_action_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_batch_order_action_error_cb cb) {
  spi->impl.SetOnErrRtnBatchOrderAction(cb);
}
void ctp_traderspi_set_return_comb_action_cb(ctp_trader_spi_t* spi, ctp_trader_return_comb_action_cb cb) {
  spi->impl.SetOnRtnCombAction(cb);
}
void ctp_traderspi_set_return_comb_action_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_comb_action_insert_error_cb cb) {
  spi->impl.SetOnErrRtnCombActionInsert(cb);
}
void ctp_traderspi_set_query_contract_bank_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_contract_bank_rsp_cb cb) {
  spi->impl.SetOnRspQryContractBank(cb);
}
void ctp_traderspi_set_query_packed_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_packed_order_rsp_cb cb) {
  spi->impl.SetOnRspQryParkedOrder(cb);
}
void ctp_traderspi_set_query_packed_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_packed_order_action_rsp_cb cb) {
  spi->impl.SetOnRspQryParkedOrderAction(cb);
}
void ctp_traderspi_set_query_trading_notice_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_trading_notice_rsp_cb cb) {
  spi->impl.SetOnRspQryTradingNotice(cb);
}
void ctp_traderspi_set_query_broker_trading_params_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_broker_trading_params_rsp_cb cb) {
  spi->impl.SetOnRspQryBrokerTradingParams(cb);
}
void ctp_traderspi_set_query_broker_trading_algos_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_broker_trading_algos_rsp_cb cb) {
  spi->impl.SetOnRspQryBrokerTradingAlgos(cb);
}
void ctp_traderspi_set_query_cfmmc_trading_account_token_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_cfmmc_trading_account_token_rsp_cb cb) {
  spi->impl.SetOnRspQueryCFMMCTradingAccountToken(cb);
}
void ctp_traderspi_set_return_from_bank_to_future_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_from_bank_to_future_by_bank_cb cb) {
  spi->impl.SetOnRtnFromBankToFutureByBank(cb);
}
void ctp_traderspi_set_return_from_future_to_bank_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_from_future_to_bank_by_bank_cb cb) {
  spi->impl.SetOnRtnFromFutureToBankByBank(cb);
}
void ctp_traderspi_set_return_repeal_from_bank_to_future_by_bank_cb(ctp_trader_spi_t* spi,
                                                                    ctp_trader_return_repeal_from_bank_to_future_by_bank_cb cb) {
  spi->impl.SetOnRtnRepealFromBankToFutureByBank(cb);
}
void ctp_traderspi_set_return_repeal_from_future_to_bank_by_bank_cb(ctp_trader_spi_t* spi,
                                                                    ctp_trader_return_repeal_from_future_to_bank_by_bank_cb cb) {
  spi->impl.SetOnRtnRepealFromFutureToBankByBank(cb);
}
void ctp_traderspi_set_return_from_bank_to_future_by_future_cb(ctp_trader_spi_t* spi, ctp_trader_return_from_bank_to_future_by_future_cb cb) {
  spi->impl.SetOnRtnFromBankToFutureByFuture(cb);
}
void ctp_traderspi_set_return_from_future_to_bank_by_future_cb(ctp_trader_spi_t* spi, ctp_trader_return_from_future_to_bank_by_future_cb cb) {
  spi->impl.SetOnRtnFromFutureToBankByFuture(cb);
}
void ctp_traderspi_set_return_repeal_from_bank_to_future_by_futuremanual_cb(ctp_trader_spi_t* spi,
                                                                            ctp_trader_return_repeal_from_bank_to_future_by_futuremanual_cb cb) {
  spi->impl.SetOnRtnRepealFromBankToFutureByFutureManual(cb);
}
void ctp_traderspi_set_return_repeal_from_future_to_bank_by_futuremanual_cb(ctp_trader_spi_t* spi,
                                                                            ctp_trader_return_repeal_from_future_to_bank_by_futuremanual_cb cb) {
  spi->impl.SetOnRtnRepealFromFutureToBankByFutureManual(cb);
}
void ctp_traderspi_set_return_query_bank_balance_by_future_cb(ctp_trader_spi_t* spi, ctp_trader_return_query_bank_balance_by_future_cb cb) {
  spi->impl.SetOnRtnQueryBankBalanceByFuture(cb);
}
void ctp_traderspi_set_return_bank_to_future_by_future_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_bank_to_future_by_future_error_cb cb) {
  spi->impl.SetOnErrRtnBankToFutureByFuture(cb);
}
void ctp_traderspi_set_return_future_to_bank_by_future_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_future_to_bank_by_future_error_cb cb) {
  spi->impl.SetOnErrRtnFutureToBankByFuture(cb);
}
void ctp_traderspi_set_return_repeal_bank_to_future_by_futuremanual_error_cb(ctp_trader_spi_t* spi,
                                                                             ctp_trader_return_repeal_bank_to_future_by_futuremanual_error_cb cb) {
  spi->impl.SetOnErrRtnRepealBankToFutureByFutureManual(cb);
}
void ctp_traderspi_set_return_repeal_future_to_bank_by_futuremanual_error_cb(ctp_trader_spi_t* spi,
                                                                             ctp_trader_return_repeal_future_to_bank_by_futuremanual_error_cb cb) {
  spi->impl.SetOnErrRtnRepealFutureToBankByFutureManual(cb);
}
void ctp_traderspi_set_return_query_bank_balance_by_future_error_cb(ctp_trader_spi_t* spi,
                                                                    ctp_trader_return_query_bank_balance_by_future_error_cb cb) {
  spi->impl.SetOnErrRtnQueryBankBalanceByFuture(cb);
}
void ctp_traderspi_set_return_repeal_from_bank_to_future_by_future_cb(ctp_trader_spi_t* spi,
                                                                      ctp_trader_return_repeal_from_bank_to_future_by_future_cb cb) {
  spi->impl.SetOnRtnRepealFromBankToFutureByFuture(cb);
}
void ctp_traderspi_set_return_repeal_from_future_to_bank_by_future_cb(ctp_trader_spi_t* spi,
                                                                      ctp_trader_return_repeal_from_future_to_bank_by_future_cb cb) {
  spi->impl.SetOnRtnRepealFromFutureToBankByFuture(cb);
}
void ctp_traderspi_set_from_bank_to_future_by_future_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_from_bank_to_future_by_future_rsp_cb cb) {
  spi->impl.SetOnRspFromBankToFutureByFuture(cb);
}
void ctp_traderspi_set_from_future_to_bank_by_future_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_from_future_to_bank_by_future_rsp_cb cb) {
  spi->impl.SetOnRspFromFutureToBankByFuture(cb);
}
void ctp_traderspi_set_query_bank_account_monoy_by_future_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_bank_account_monoy_by_future_rsp_cb cb) {
  spi->impl.SetOnRspQueryBankAccountMoneyByFuture(cb);
}
void ctp_traderspi_set_return_open_account_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_open_account_by_bank_cb cb) {
  spi->impl.SetOnRtnOpenAccountByBank(cb);
}
void ctp_traderspi_set_return_cancel_account_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_cancel_account_by_bank_cb cb) {
  spi->impl.SetOnRtnCancelAccountByBank(cb);
}
void ctp_traderspi_set_return_change_account_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_change_account_by_bank_cb cb) {
  spi->impl.SetOnRtnChangeAccountByBank(cb);
}

ctp_trader_api_t* ctp_traderapi_create(const char* flowpath) {
  auto api = new ctp_trader_api_t;
  api->traderapi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath);
  return api;
}
void ctp_traderapi_free(ctp_trader_api_t* api) {
  api->traderapi->Release();
  delete api;
}
const char* ctp_traderapi_get_api_version(ctp_trader_api_t* api) {
  return api->traderapi->GetApiVersion();
}
void ctp_traderapi_init(ctp_trader_api_t* api) {
  return api->traderapi->Init();
}
int ctp_traderapi_join(ctp_trader_api_t* api) {
  return api->traderapi->Join();
}
const char* ctp_traderapi_get_trading_day(ctp_trader_api_t* api) {
  return api->traderapi->GetTradingDay();
}
void ctp_traderapi_register_front(ctp_trader_api_t* api, char* pszFrontAddress) {
  return api->traderapi->RegisterFront(pszFrontAddress);
}
void ctp_traderapi_register_nameserver(ctp_trader_api_t* api, char* pszNsAddress) {
  return api->traderapi->RegisterNameServer(pszNsAddress);
}
void ctp_traderapi_register_fens_userinfo(ctp_trader_api_t* api, CThostFtdcFensUserInfoField* pFensUserInfo) {
  return api->traderapi->RegisterFensUserInfo(pFensUserInfo);
}
void ctp_traderapi_register_spi(ctp_trader_api_t* api, ctp_trader_spi_t* spi) {
  return api->traderapi->RegisterSpi(&(spi->impl));
}
void ctp_traderapi_sub_private_topic(ctp_trader_api_t* api, THOST_TE_RESUME_TYPE nResumeType) {
  return api->traderapi->SubscribePrivateTopic(nResumeType);
}
void ctp_traderapi_sub_public_topic(ctp_trader_api_t* api, THOST_TE_RESUME_TYPE nResumeType) {
  return api->traderapi->SubscribePublicTopic(nResumeType);
}
int ctp_traderapi_req_authenticate(ctp_trader_api_t* api, CThostFtdcReqAuthenticateField* pReqAuthenticateField, int nRequestID) {
  return api->traderapi->ReqAuthenticate(pReqAuthenticateField, nRequestID);
}
int ctp_traderapi_req_user_login(ctp_trader_api_t* api, CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID) {
  return api->traderapi->ReqUserLogin(pReqUserLoginField, nRequestID);
}
int ctp_traderapi_req_user_logout(ctp_trader_api_t* api, CThostFtdcUserLogoutField* pUserLogout, int nRequestID) {
  return api->traderapi->ReqUserLogout(pUserLogout, nRequestID);
}
int ctp_traderapi_req_user_passwd_update(ctp_trader_api_t* api, CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate, int nRequestID) {
  return api->traderapi->ReqUserPasswordUpdate(pUserPasswordUpdate, nRequestID);
}
int ctp_traderapi_req_trading_account_passwd_update(ctp_trader_api_t* api,
                                                    CThostFtdcTradingAccountPasswordUpdateField* pTradingAccountPasswordUpdate,
                                                    int nRequestID) {
  return api->traderapi->ReqTradingAccountPasswordUpdate(pTradingAccountPasswordUpdate, nRequestID);
}
int ctp_traderapi_req_order_insert(ctp_trader_api_t* api, CThostFtdcInputOrderField* pInputOrder, int nRequestID) {
  return api->traderapi->ReqOrderInsert(pInputOrder, nRequestID);
}
int ctp_traderapi_req_parked_order_insert(ctp_trader_api_t* api, CThostFtdcParkedOrderField* pParkedOrder, int nRequestID) {
  return api->traderapi->ReqParkedOrderInsert(pParkedOrder, nRequestID);
}
int ctp_traderapi_req_packed_order_action(ctp_trader_api_t* api, CThostFtdcParkedOrderActionField* pParkedOrderAction, int nRequestID) {
  return api->traderapi->ReqParkedOrderAction(pParkedOrderAction, nRequestID);
}
int ctp_traderapi_req_order_action(ctp_trader_api_t* api, CThostFtdcInputOrderActionField* pInputOrderAction, int nRequestID) {
  return api->traderapi->ReqOrderAction(pInputOrderAction, nRequestID);
}
int ctp_traderapi_req_query_max_order_volume(ctp_trader_api_t* api, CThostFtdcQueryMaxOrderVolumeField* pQueryMaxOrderVolume, int nRequestID) {
  return api->traderapi->ReqQueryMaxOrderVolume(pQueryMaxOrderVolume, nRequestID);
}
int ctp_traderapi_req_settlement_info_confirm(ctp_trader_api_t* api, CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, int nRequestID) {
  return api->traderapi->ReqSettlementInfoConfirm(pSettlementInfoConfirm, nRequestID);
}
int ctp_traderapi_req_remove_parked_order(ctp_trader_api_t* api, CThostFtdcRemoveParkedOrderField* pRemoveParkedOrder, int nRequestID) {
  return api->traderapi->ReqRemoveParkedOrder(pRemoveParkedOrder, nRequestID);
}
int ctp_traderapi_req_remove_parked_order_action(ctp_trader_api_t* api,
                                                 CThostFtdcRemoveParkedOrderActionField* pRemoveParkedOrderAction,
                                                 int nRequestID) {
  return api->traderapi->ReqRemoveParkedOrderAction(pRemoveParkedOrderAction, nRequestID);
}
int ctp_traderapi_req_exec_order_insert(ctp_trader_api_t* api, CThostFtdcInputExecOrderField* pInputExecOrder, int nRequestID) {
  return api->traderapi->ReqExecOrderInsert(pInputExecOrder, nRequestID);
}
int ctp_traderapi_req_exec_order_action(ctp_trader_api_t* api, CThostFtdcInputExecOrderActionField* pInputExecOrderAction, int nRequestID) {
  return api->traderapi->ReqExecOrderAction(pInputExecOrderAction, nRequestID);
}
int ctp_traderapi_req_forquote_insert(ctp_trader_api_t* api, CThostFtdcInputForQuoteField* pInputForQuote, int nRequestID) {
  return api->traderapi->ReqForQuoteInsert(pInputForQuote, nRequestID);
}
int ctp_traderapi_req_quote_insert(ctp_trader_api_t* api, CThostFtdcInputQuoteField* pInputQuote, int nRequestID) {
  return api->traderapi->ReqQuoteInsert(pInputQuote, nRequestID);
}
int ctp_traderapi_req_quote_action(ctp_trader_api_t* api, CThostFtdcInputQuoteActionField* pInputQuoteAction, int nRequestID) {
  return api->traderapi->ReqQuoteAction(pInputQuoteAction, nRequestID);
}
int ctp_traderapi_req_batch_order_action(ctp_trader_api_t* api, CThostFtdcInputBatchOrderActionField* pInputBatchOrderAction, int nRequestID) {
  return api->traderapi->ReqBatchOrderAction(pInputBatchOrderAction, nRequestID);
}
int ctp_traderapi_req_comb_action_insert(ctp_trader_api_t* api, CThostFtdcInputCombActionField* pInputCombAction, int nRequestID) {
  return api->traderapi->ReqCombActionInsert(pInputCombAction, nRequestID);
}
int ctp_traderapi_req_query_order(ctp_trader_api_t* api, CThostFtdcQryOrderField* pQryOrder, int nRequestID) {
  return api->traderapi->ReqQryOrder(pQryOrder, nRequestID);
}
int ctp_traderapi_req_query_trade(ctp_trader_api_t* api, CThostFtdcQryTradeField* pQryTrade, int nRequestID) {
  return api->traderapi->ReqQryTrade(pQryTrade, nRequestID);
}
int ctp_traderapi_req_query_investor_position(ctp_trader_api_t* api, CThostFtdcQryInvestorPositionField* pQryInvestorPosition, int nRequestID) {
  return api->traderapi->ReqQryInvestorPosition(pQryInvestorPosition, nRequestID);
}
int ctp_traderapi_req_query_trading_account(ctp_trader_api_t* api, CThostFtdcQryTradingAccountField* pQryTradingAccount, int nRequestID) {
  return api->traderapi->ReqQryTradingAccount(pQryTradingAccount, nRequestID);
}
int ctp_traderapi_req_query_investor(ctp_trader_api_t* api, CThostFtdcQryInvestorField* pQryInvestor, int nRequestID) {
  return api->traderapi->ReqQryInvestor(pQryInvestor, nRequestID);
}
int ctp_traderapi_req_query_trading_code(ctp_trader_api_t* api, CThostFtdcQryTradingCodeField* pQryTradingCode, int nRequestID) {
  return api->traderapi->ReqQryTradingCode(pQryTradingCode, nRequestID);
}
int ctp_traderapi_req_query_instrument_margin_rate(ctp_trader_api_t* api,
                                                   CThostFtdcQryInstrumentMarginRateField* pQryInstrumentMarginRate,
                                                   int nRequestID) {
  return api->traderapi->ReqQryInstrumentMarginRate(pQryInstrumentMarginRate, nRequestID);
}
int ctp_traderapi_req_query_instrument_commission_rate(ctp_trader_api_t* api,
                                                       CThostFtdcQryInstrumentCommissionRateField* pQryInstrumentCommissionRate,
                                                       int nRequestID) {
  return api->traderapi->ReqQryInstrumentCommissionRate(pQryInstrumentCommissionRate, nRequestID);
}
int ctp_traderapi_req_query_exchage(ctp_trader_api_t* api, CThostFtdcQryExchangeField* pQryExchange, int nRequestID) {
  return api->traderapi->ReqQryExchange(pQryExchange, nRequestID);
}
int ctp_traderapi_req_query_product(ctp_trader_api_t* api, CThostFtdcQryProductField* pQryProduct, int nRequestID) {
  return api->traderapi->ReqQryProduct(pQryProduct, nRequestID);
}
int ctp_traderapi_req_query_instrument(ctp_trader_api_t* api, CThostFtdcQryInstrumentField* pQryInstrument, int nRequestID) {
  return api->traderapi->ReqQryInstrument(pQryInstrument, nRequestID);
}
int ctp_traderapi_req_query_depth_market_dat(ctp_trader_api_t* api, CThostFtdcQryDepthMarketDataField* pQryDepthMarketData, int nRequestID) {
  return api->traderapi->ReqQryDepthMarketData(pQryDepthMarketData, nRequestID);
}
int ctp_traderapi_req_query_settlement_info(ctp_trader_api_t* api, CThostFtdcQrySettlementInfoField* pQrySettlementInfo, int nRequestID) {
  return api->traderapi->ReqQrySettlementInfo(pQrySettlementInfo, nRequestID);
}
int ctp_traderapi_req_query_transfer_bank(ctp_trader_api_t* api, CThostFtdcQryTransferBankField* pQryTransferBank, int nRequestID) {
  return api->traderapi->ReqQryTransferBank(pQryTransferBank, nRequestID);
}
int ctp_traderapi_req_query_investor_position_detail(ctp_trader_api_t* api,
                                                     CThostFtdcQryInvestorPositionDetailField* pQryInvestorPositionDetail,
                                                     int nRequestID) {
  return api->traderapi->ReqQryInvestorPositionDetail(pQryInvestorPositionDetail, nRequestID);
}
int ctp_traderapi_req_query_notice(ctp_trader_api_t* api, CThostFtdcQryNoticeField* pQryNotice, int nRequestID) {
  return api->traderapi->ReqQryNotice(pQryNotice, nRequestID);
}
int ctp_traderapi_req_query_settlement_info_confirm(ctp_trader_api_t* api,
                                                    CThostFtdcQrySettlementInfoConfirmField* pQrySettlementInfoConfirm,
                                                    int nRequestID) {
  return api->traderapi->ReqQrySettlementInfoConfirm(pQrySettlementInfoConfirm, nRequestID);
}
int ctp_traderapi_req_query_investor_position_combine_detail(ctp_trader_api_t* api,
                                                             CThostFtdcQryInvestorPositionCombineDetailField* pQryInvestorPositionCombineDetail,
                                                             int nRequestID) {
  return api->traderapi->ReqQryInvestorPositionCombineDetail(pQryInvestorPositionCombineDetail, nRequestID);
}
int ctp_traderapi_req_query_cfmmc_trading_account_key(ctp_trader_api_t* api,
                                                      CThostFtdcQryCFMMCTradingAccountKeyField* pQryCFMMCTradingAccountKey,
                                                      int nRequestID) {
  return api->traderapi->ReqQryCFMMCTradingAccountKey(pQryCFMMCTradingAccountKey, nRequestID);
}
int ctp_traderapi_req_query_ewarrent_offset(ctp_trader_api_t* api, CThostFtdcQryEWarrantOffsetField* pQryEWarrantOffset, int nRequestID) {
  return api->traderapi->ReqQryEWarrantOffset(pQryEWarrantOffset, nRequestID);
}
int ctp_traderapi_req_query_investor_product_group_margin(ctp_trader_api_t* api,
                                                          CThostFtdcQryInvestorProductGroupMarginField* pQryInvestorProductGroupMargin,
                                                          int nRequestID) {
  return api->traderapi->ReqQryInvestorProductGroupMargin(pQryInvestorProductGroupMargin, nRequestID);
}
int ctp_traderapi_req_query_exchange_margin_rate(ctp_trader_api_t* api,
                                                 CThostFtdcQryExchangeMarginRateField* pQryExchangeMarginRate,
                                                 int nRequestID) {
  return api->traderapi->ReqQryExchangeMarginRate(pQryExchangeMarginRate, nRequestID);
}
int ctp_traderapi_req_query_exchange_margin_rate_adjust(ctp_trader_api_t* api,
                                                        CThostFtdcQryExchangeMarginRateAdjustField* pQryExchangeMarginRateAdjust,
                                                        int nRequestID) {
  return api->traderapi->ReqQryExchangeMarginRateAdjust(pQryExchangeMarginRateAdjust, nRequestID);
}
int ctp_traderapi_req_query_exchage_rate(ctp_trader_api_t* api, CThostFtdcQryExchangeRateField* pQryExchangeRate, int nRequestID) {
  return api->traderapi->ReqQryExchangeRate(pQryExchangeRate, nRequestID);
}
int ctp_traderapi_req_query_sec_agent_acidmap(ctp_trader_api_t* api, CThostFtdcQrySecAgentACIDMapField* pQrySecAgentACIDMap, int nRequestID) {
  return api->traderapi->ReqQrySecAgentACIDMap(pQrySecAgentACIDMap, nRequestID);
}
int ctp_traderapi_req_query_product_exchange_rate(ctp_trader_api_t* api, CThostFtdcQryProductExchRateField* pQryProductExchRate, int nRequestID) {
  return api->traderapi->ReqQryProductExchRate(pQryProductExchRate, nRequestID);
}
int ctp_traderapi_req_query_product_group(ctp_trader_api_t* api, CThostFtdcQryProductGroupField* pQryProductGroup, int nRequestID) {
  return api->traderapi->ReqQryProductGroup(pQryProductGroup, nRequestID);
}
int ctp_traderapi_req_query_mminstrument_commission_rate(ctp_trader_api_t* api,
                                                         CThostFtdcQryMMInstrumentCommissionRateField* pQryMMInstrumentCommissionRate,
                                                         int nRequestID) {
  return api->traderapi->ReqQryMMInstrumentCommissionRate(pQryMMInstrumentCommissionRate, nRequestID);
}
int ctp_traderapi_req_query_mmoption_instrument_commission_rate(ctp_trader_api_t* api,
                                                                CThostFtdcQryMMOptionInstrCommRateField* pQryMMOptionInstrCommRate,
                                                                int nRequestID) {
  return api->traderapi->ReqQryMMOptionInstrCommRate(pQryMMOptionInstrCommRate, nRequestID);
}
int ctp_traderapi_req_query_instrument_order_commission_rate(ctp_trader_api_t* api,
                                                             CThostFtdcQryInstrumentOrderCommRateField* pQryInstrumentOrderCommRate,
                                                             int nRequestID) {
  return api->traderapi->ReqQryInstrumentOrderCommRate(pQryInstrumentOrderCommRate, nRequestID);
}
int ctp_traderapi_req_query_option_instrument_trade_cost(ctp_trader_api_t* api,
                                                         CThostFtdcQryOptionInstrTradeCostField* pQryOptionInstrTradeCost,
                                                         int nRequestID) {
  return api->traderapi->ReqQryOptionInstrTradeCost(pQryOptionInstrTradeCost, nRequestID);
}
int ctp_traderapi_req_query_option_commission_rate(ctp_trader_api_t* api,
                                                   CThostFtdcQryOptionInstrCommRateField* pQryOptionInstrCommRate,
                                                   int nRequestID) {
  return api->traderapi->ReqQryOptionInstrCommRate(pQryOptionInstrCommRate, nRequestID);
}
int ctp_traderapi_req_query_exex_order(ctp_trader_api_t* api, CThostFtdcQryExecOrderField* pQryExecOrder, int nRequestID) {
  return api->traderapi->ReqQryExecOrder(pQryExecOrder, nRequestID);
}
int ctp_traderapi_req_query_forquote(ctp_trader_api_t* api, CThostFtdcQryForQuoteField* pQryForQuote, int nRequestID) {
  return api->traderapi->ReqQryForQuote(pQryForQuote, nRequestID);
}
int ctp_traderapi_req_query_quote(ctp_trader_api_t* api, CThostFtdcQryQuoteField* pQryQuote, int nRequestID) {
  return api->traderapi->ReqQryQuote(pQryQuote, nRequestID);
}
int ctp_traderapi_req_query_comb_instrument_guard(ctp_trader_api_t* api,
                                                  CThostFtdcQryCombInstrumentGuardField* pQryCombInstrumentGuard,
                                                  int nRequestID) {
  return api->traderapi->ReqQryCombInstrumentGuard(pQryCombInstrumentGuard, nRequestID);
}
int ctp_traderapi_req_query_comb_action(ctp_trader_api_t* api, CThostFtdcQryCombActionField* pQryCombAction, int nRequestID) {
  return api->traderapi->ReqQryCombAction(pQryCombAction, nRequestID);
}
int ctp_traderapi_req_query_transfer_serial(ctp_trader_api_t* api, CThostFtdcQryTransferSerialField* pQryTransferSerial, int nRequestID) {
  return api->traderapi->ReqQryTransferSerial(pQryTransferSerial, nRequestID);
}
int ctp_traderapi_req_query_account_register(ctp_trader_api_t* api, CThostFtdcQryAccountregisterField* pQryAccountregister, int nRequestID) {
  return api->traderapi->ReqQryAccountregister(pQryAccountregister, nRequestID);
}
int ctp_traderapi_req_query_contract_bank(ctp_trader_api_t* api, CThostFtdcQryContractBankField* pQryContractBank, int nRequestID) {
  return api->traderapi->ReqQryContractBank(pQryContractBank, nRequestID);
}
int ctp_traderapi_req_query_parked_order(ctp_trader_api_t* api, CThostFtdcQryParkedOrderField* pQryParkedOrder, int nRequestID) {
  return api->traderapi->ReqQryParkedOrder(pQryParkedOrder, nRequestID);
}
int ctp_traderapi_req_query_parked_order_action(ctp_trader_api_t* api, CThostFtdcQryParkedOrderActionField* pQryParkedOrderAction, int nRequestID) {
  return api->traderapi->ReqQryParkedOrderAction(pQryParkedOrderAction, nRequestID);
}
int ctp_traderapi_req_query_trading_notice(ctp_trader_api_t* api, CThostFtdcQryTradingNoticeField* pQryTradingNotice, int nRequestID) {
  return api->traderapi->ReqQryTradingNotice(pQryTradingNotice, nRequestID);
}
int ctp_traderapi_req_query_broker_trading_params(ctp_trader_api_t* api,
                                                  CThostFtdcQryBrokerTradingParamsField* pQryBrokerTradingParams,
                                                  int nRequestID) {
  return api->traderapi->ReqQryBrokerTradingParams(pQryBrokerTradingParams, nRequestID);
}
int ctp_traderapi_req_query_broker_trading_algos(ctp_trader_api_t* api,
                                                 CThostFtdcQryBrokerTradingAlgosField* pQryBrokerTradingAlgos,
                                                 int nRequestID) {
  return api->traderapi->ReqQryBrokerTradingAlgos(pQryBrokerTradingAlgos, nRequestID);
}
int ctp_traderapi_req_query_cfmmc_trading_account_token(ctp_trader_api_t* api,
                                                        CThostFtdcQueryCFMMCTradingAccountTokenField* pQueryCFMMCTradingAccountToken,
                                                        int nRequestID) {
  return api->traderapi->ReqQueryCFMMCTradingAccountToken(pQueryCFMMCTradingAccountToken, nRequestID);
}
int ctp_traderapi_req_from_bank_to_future_by_future(ctp_trader_api_t* api, CThostFtdcReqTransferField* pReqTransfer, int nRequestID) {
  return api->traderapi->ReqFromBankToFutureByFuture(pReqTransfer, nRequestID);
}
int ctp_traderapi_req_from_future_to_bank_by_future(ctp_trader_api_t* api, CThostFtdcReqTransferField* pReqTransfer, int nRequestID) {
  return api->traderapi->ReqFromFutureToBankByFuture(pReqTransfer, nRequestID);
}
int ctp_traderapi_req_query_bank_account_money_by_future(ctp_trader_api_t* api, CThostFtdcReqQueryAccountField* pReqQueryAccount, int nRequestID) {
  return api->traderapi->ReqQueryBankAccountMoneyByFuture(pReqQueryAccount, nRequestID);
}

}  // end extern "C"
