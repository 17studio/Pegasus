
#include "capi.h"

#include "ThostFtdcMdApi.h"

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
  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
                              CThostFtdcRspInfoField* pRspInfo,
                              int nRequestID,
                              bool bIsLast) override {
    callback_.login(pRspUserLogin, pRspInfo, nRequestID, bIsLast);
  };

  ///登出请求响应
  virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout,
                               CThostFtdcRspInfoField* pRspInfo,
                               int nRequestID,
                               bool bIsLast) override {
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
  virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) override {
    callback_.market_data(pDepthMarketData);
  };

  ///询价通知
  virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) override { callback_.quote(pForQuoteRsp); };

  ///完成各项回掉的设置
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
