/*
 * 对C++实现进行了一层封装，暴露C API提供给第三方语言调用
 *
 * 调用次序：
 * 1. 创建ctp_md_spi_t
 * 2. 设置ctp_md_spi_t回调
 * 3. 参考CTP文档使用
 */

#ifndef CTP_CAPI_H_
#define CTP_CAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

// includes

#include "ThostFtdcUserApiStruct.h"

// types

typedef struct ctp_md_spi_t ctp_md_spi_t;
typedef struct ctp_md_api_t ctp_md_api_t;

typedef void (*ctp_md_front_connected_cb)();
typedef void (*ctp_md_front_disconnected_cb)(int reason);
typedef void (*ctp_md_heart_beat_warning_cb)(int timelapse);
typedef void (*ctp_md_user_login_rsp_cb)(
    CThostFtdcRspUserLoginField* user_login_field,
    CThostFtdcRspInfoField* info,
    int request_id,
    bool is_last);
typedef void (*ctp_md_user_logout_rsp_cb)(
    CThostFtdcUserLogoutField* user_logout_field,
    CThostFtdcRspInfoField* info,
    int request_id,
    bool is_last);
typedef void (*ctp_md_error_rsp_cb)(CThostFtdcRspInfoField* info,
                                    int request_id,
                                    bool is_last);
typedef void (*ctp_md_sub_market_data_rsp_cb)(
    CThostFtdcSpecificInstrumentField* specific_instrument,
    CThostFtdcRspInfoField* info,
    int request_id,
    bool is_last);
typedef void (*ctp_md_unsub_market_data_rsp_cb)(
    CThostFtdcSpecificInstrumentField* specific_instrument,
    CThostFtdcRspInfoField* info,
    int request_id,
    bool is_last);
typedef void (*ctp_md_sub_quote_rsp_cb)(
    CThostFtdcSpecificInstrumentField* specific_instrument,
    CThostFtdcRspInfoField* info,
    int request_id,
    bool is_last);
typedef void (*ctp_md_unsub_quote_rsp_cb)(
    CThostFtdcSpecificInstrumentField* specific_instrument,
    CThostFtdcRspInfoField* info,
    int request_id,
    bool is_last);
typedef void (*ctp_md_depth_market_data_cb)(
    CThostFtdcDepthMarketDataField* depth_market_data);
typedef void (*ctp_md_quote_cb)(CThostFtdcForQuoteRspField* quote);

// functions

ctp_md_spi_t* ctp_mdspi_create();
void ctp_mdspi_free(ctp_md_spi_t* spi);
void ctp_mdspi_set_front_connected_cb(ctp_md_spi_t* spi,
                                      ctp_md_front_connected_cb cb);
void ctp_mdspi_set_front_disconnected_cb(ctp_md_spi_t* spi,
                                         ctp_md_front_disconnected_cb cb);
void ctp_mdspi_set_heartbeat_warning_cb(ctp_md_spi_t* spi,
                                        ctp_md_heart_beat_warning_cb cb);
void ctp_mdspi_set_user_login_cb(ctp_md_spi_t* spi,
                                 ctp_md_user_login_rsp_cb cb);
void ctp_mdspi_set_user_logout_cb(ctp_md_spi_t* spi,
                                  ctp_md_user_logout_rsp_cb cb);
void ctp_mdspi_set_error_cb(ctp_md_spi_t* spi, ctp_md_error_rsp_cb cb);
void ctp_mdspi_set_sub_market_data_rsp_cb(ctp_md_spi_t* spi,
                                          ctp_md_sub_market_data_rsp_cb cb);
void ctp_mdspi_set_unsub_market_data_rsp_cb(ctp_md_spi_t* spi,
                                            ctp_md_unsub_market_data_rsp_cb cb);
void ctp_mdspi_set_sub_quote_rsp_cb(ctp_md_spi_t* spi,
                                    ctp_md_unsub_quote_rsp_cb cb);
void ctp_mdspi_set_unsub_quote_rsp_cb(ctp_md_spi_t* spi,
                                      ctp_md_unsub_quote_rsp_cb cb);
void ctp_mdspi_set_depth_market_data_cb(ctp_md_spi_t* spi,
                                        ctp_md_depth_market_data_cb cb);
void ctp_mdspi_set_quote_cb(ctp_md_spi_t* spi, ctp_md_quote_cb cb);

ctp_md_api_t* ctp_mdapi_create(const char* flowpath,
                               bool useudp,
                               bool multicast);
void ctp_mdapi_free(ctp_md_api_t* api);
const char* ctp_mdapi_get_api_version(ctp_md_api_t* api);
void ctp_mdapi_init(ctp_md_api_t* api);
void ctp_mdapi_join(ctp_md_api_t* api);
const char* ctp_mdapi_get_trading_day(ctp_md_api_t* api);
void ctp_mdapi_register_front(ctp_md_api_t* api, char* front_address);
void ctp_mdapi_register_nameserver(ctp_md_api_t* api,
                                   char* nameserver_address);
void ctp_mdapi_register_fens_userinfo(ctp_md_api_t* api,
                                      CThostFtdcFensUserInfoField* userinfo);
void ctp_mdapi_register_spi(ctp_md_api_t* api, ctp_md_spi_t* spi);
void ctp_mdapi_sub_market_data(ctp_md_api_t* api,
                               char* instruments[],
                               int count);
void ctp_mdapi_unsub_market_data(ctp_md_api_t* api,
                                 char* instruments[],
                                 int count);
void ctp_mdapi_sub_quote(ctp_md_api_t* api,
                         char* instruments[],
                         int count);
void ctp_mdapi_unsub_quote(ctp_md_api_t* api,
                           char* instruments[],
                           int count);
void ctp_mdapi_user_login(ctp_md_api_t* api,
                          CThostFtdcReqUserLoginField* userlogin,
                          int request_id);
void ctp_mdapi_user_logout(ctp_md_api_t* api,
                           CThostFtdcUserLogoutField* userlogout,
                           int request_id);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif  // CTP_CAPI_H_