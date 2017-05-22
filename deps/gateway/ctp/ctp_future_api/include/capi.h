/*
 * 对C++实现进行了一层封装，暴露C API提供给第三方语言调用
 *
 * 调用次序：
 * 1. 创建ctp_md_spi_t & ctp_md_api_t
 * 2. 设置ctp_md_spi_t callback functions
 * 3. 参考CTP文档使用说明
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
typedef void (*ctp_md_user_login_rsp_cb)(CThostFtdcRspUserLoginField* user_login_field, CThostFtdcRspInfoField* info, int request_id, bool is_last);
typedef void (*ctp_md_user_logout_rsp_cb)(CThostFtdcUserLogoutField* user_logout_field, CThostFtdcRspInfoField* info, int request_id, bool is_last);
typedef void (*ctp_md_error_rsp_cb)(CThostFtdcRspInfoField* info, int request_id, bool is_last);
typedef void (*ctp_md_sub_market_data_rsp_cb)(CThostFtdcSpecificInstrumentField* specific_instrument,
                                              CThostFtdcRspInfoField* info,
                                              int request_id,
                                              bool is_last);
typedef void (*ctp_md_unsub_market_data_rsp_cb)(CThostFtdcSpecificInstrumentField* specific_instrument,
                                                CThostFtdcRspInfoField* info,
                                                int request_id,
                                                bool is_last);
typedef void (*ctp_md_sub_quote_rsp_cb)(CThostFtdcSpecificInstrumentField* specific_instrument,
                                        CThostFtdcRspInfoField* info,
                                        int request_id,
                                        bool is_last);
typedef void (*ctp_md_unsub_quote_rsp_cb)(CThostFtdcSpecificInstrumentField* specific_instrument,
                                          CThostFtdcRspInfoField* info,
                                          int request_id,
                                          bool is_last);
typedef void (*ctp_md_depth_market_data_cb)(CThostFtdcDepthMarketDataField* depth_market_data);
typedef void (*ctp_md_quote_cb)(CThostFtdcForQuoteRspField* quote);

typedef struct ctp_trader_spi_t ctp_trader_spi_t;
typedef struct ctp_trader_api_t ctp_trader_api_t;

typedef void (*ctp_trader_front_connected_cb)();
typedef void (*ctp_trader_front_disconnected_cb)(int reason);
typedef void (*ctp_trader_heart_beat_warning_cb)(int timelaps);
typedef void (*ctp_trader_authenticate_rsp_cb)(CThostFtdcRspAuthenticateField* authenticate_field,
                                               CThostFtdcRspInfoField* info,
                                               int request_id,
                                               bool is_last);
typedef void (*ctp_trader_user_login_rsp_cb)(CThostFtdcRspUserLoginField* pRspUserLogin,
                                             CThostFtdcRspInfoField* pRspInfo,
                                             int nRequestID,
                                             bool bIsLast);
typedef void (*ctp_trader_user_logout_rsp_cb)(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_user_passwd_update_rsp_cb)(CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate,
                                                     CThostFtdcRspInfoField* pRspInfo,
                                                     int nRequestID,
                                                     bool bIsLast);
typedef void (*ctp_trader_trading_account_passwd_update_cb)(CThostFtdcTradingAccountPasswordUpdateField* pTradingAccountPasswordUpdate,
                                                            CThostFtdcRspInfoField* pRspInfo,
                                                            int nRequestID,
                                                            bool bIsLast);
typedef void (*ctp_trader_order_insert_rsp_cb)(CThostFtdcInputOrderField* pInputOrder,
                                               CThostFtdcRspInfoField* pRspInfo,
                                               int nRequestID,
                                               bool bIsLast);
typedef void (*ctp_trader_parked_order_insert_rsp_cb)(CThostFtdcParkedOrderField* pParkedOrder,
                                                      CThostFtdcRspInfoField* pRspInfo,
                                                      int nRequestID,
                                                      bool bIsLast);
typedef void (*ctp_trader_packed_order_action_rsp_cb)(CThostFtdcParkedOrderActionField* pParkedOrderAction,
                                                      CThostFtdcRspInfoField* pRspInfo,
                                                      int nRequestID,
                                                      bool bIsLast);
typedef void (*ctp_trader_order_action_rsp_cb)(CThostFtdcInputOrderActionField* pInputOrderAction,
                                               CThostFtdcRspInfoField* pRspInfo,
                                               int nRequestID,
                                               bool bIsLast);
typedef void (*ctp_trader_query_max_order_volume_rsp_cb)(CThostFtdcQueryMaxOrderVolumeField* pQueryMaxOrderVolume,
                                                         CThostFtdcRspInfoField* pRspInfo,
                                                         int nRequestID,
                                                         bool bIsLast);
typedef void (*ctp_trader_settlement_info_confirm_rsp_cb)(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
                                                          CThostFtdcRspInfoField* pRspInfo,
                                                          int nRequestID,
                                                          bool bIsLast);
typedef void (*ctp_trader_remove_packed_order_rsp_cb)(CThostFtdcRemoveParkedOrderField* pRemoveParkedOrder,
                                                      CThostFtdcRspInfoField* pRspInfo,
                                                      int nRequestID,
                                                      bool bIsLast);
typedef void (*ctp_trader_remove_packed_order_action_rsp_cb)(CThostFtdcRemoveParkedOrderActionField* pRemoveParkedOrderAction,
                                                             CThostFtdcRspInfoField* pRspInfo,
                                                             int nRequestID,
                                                             bool bIsLast);
typedef void (*ctp_trader_exec_order_insert_rsp_cb)(CThostFtdcInputExecOrderField* pInputExecOrder,
                                                    CThostFtdcRspInfoField* pRspInfo,
                                                    int nRequestID,
                                                    bool bIsLast);
typedef void (*ctp_trader_exec_order_action_rsp_cb)(CThostFtdcInputExecOrderActionField* pInputExecOrderAction,
                                                    CThostFtdcRspInfoField* pRspInfo,
                                                    int nRequestID,
                                                    bool bIsLast);
typedef void (*ctp_trader_for_quote_insert_rsp_cb)(CThostFtdcInputForQuoteField* pInputForQuote,
                                                   CThostFtdcRspInfoField* pRspInfo,
                                                   int nRequestID,
                                                   bool bIsLast);
typedef void (*ctp_trader_quote_insert_rsp_cb)(CThostFtdcInputQuoteField* pInputQuote,
                                               CThostFtdcRspInfoField* pRspInfo,
                                               int nRequestID,
                                               bool bIsLast);
typedef void (*ctp_trader_quote_action_rsp_cb)(CThostFtdcInputQuoteActionField* pInputQuoteAction,
                                               CThostFtdcRspInfoField* pRspInfo,
                                               int nRequestID,
                                               bool bIsLast);
typedef void (*ctp_trader_batch_order_action_rsp_cb)(CThostFtdcInputBatchOrderActionField* pInputBatchOrderAction,
                                                     CThostFtdcRspInfoField* pRspInfo,
                                                     int nRequestID,
                                                     bool bIsLast);
typedef void (*ctp_trader_comb_action_insert_rsp_cb)(CThostFtdcInputCombActionField* pInputCombAction,
                                                     CThostFtdcRspInfoField* pRspInfo,
                                                     int nRequestID,
                                                     bool bIsLast);
typedef void (*ctp_trader_query_order_rsp_cb)(CThostFtdcOrderField* pOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_query_trade_rsp_cb)(CThostFtdcTradeField* pTrade, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_query_investor_position_rsp_cb)(CThostFtdcInvestorPositionField* pInvestorPosition,
                                                          CThostFtdcRspInfoField* pRspInfo,
                                                          int nRequestID,
                                                          bool bIsLast);
typedef void (*ctp_trader_query_trading_account_rsp_cb)(CThostFtdcTradingAccountField* pTradingAccount,
                                                        CThostFtdcRspInfoField* pRspInfo,
                                                        int nRequestID,
                                                        bool bIsLast);
typedef void (*ctp_trader_query_investor_rsp_cb)(CThostFtdcInvestorField* pInvestor, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_query_trading_code_rsp_cb)(CThostFtdcTradingCodeField* pTradingCode,
                                                     CThostFtdcRspInfoField* pRspInfo,
                                                     int nRequestID,
                                                     bool bIsLast);
typedef void (*ctp_trader_query_instrument_margin_rate_rsp_cb)(CThostFtdcInstrumentMarginRateField* pInstrumentMarginRate,
                                                               CThostFtdcRspInfoField* pRspInfo,
                                                               int nRequestID,
                                                               bool bIsLast);
typedef void (*ctp_trader_query_instrument_commission_rate_rsp_cb)(CThostFtdcInstrumentCommissionRateField* pInstrumentCommissionRate,
                                                                   CThostFtdcRspInfoField* pRspInfo,
                                                                   int nRequestID,
                                                                   bool bIsLast);
typedef void (*ctp_trader_query_exchange_rsp_cb)(CThostFtdcExchangeField* pExchange, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_query_product_rsp_cb)(CThostFtdcProductField* pProduct, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_query_instrument_rsp_cb)(CThostFtdcInstrumentField* pInstrument,
                                                   CThostFtdcRspInfoField* pRspInfo,
                                                   int nRequestID,
                                                   bool bIsLast);
typedef void (*ctp_trader_query_depth_market_data_rsp_cb)(CThostFtdcDepthMarketDataField* pDepthMarketData,
                                                          CThostFtdcRspInfoField* pRspInfo,
                                                          int nRequestID,
                                                          bool bIsLast);
typedef void (*ctp_trader_query_settlement_info_rsp_cb)(CThostFtdcSettlementInfoField* pSettlementInfo,
                                                        CThostFtdcRspInfoField* pRspInfo,
                                                        int nRequestID,
                                                        bool bIsLast);
typedef void (*ctp_trader_query_transfer_bank_rsp_cb)(CThostFtdcTransferBankField* pTransferBank,
                                                      CThostFtdcRspInfoField* pRspInfo,
                                                      int nRequestID,
                                                      bool bIsLast);
typedef void (*ctp_trader_query_investor_position_detail_rsp_cb)(CThostFtdcInvestorPositionDetailField* pInvestorPositionDetail,
                                                                 CThostFtdcRspInfoField* pRspInfo,
                                                                 int nRequestID,
                                                                 bool bIsLast);
typedef void (*ctp_trader_query_notice_rsp_cb)(CThostFtdcNoticeField* pNotice, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_query_settlement_info_confirm_rsp_cb)(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
                                                                CThostFtdcRspInfoField* pRspInfo,
                                                                int nRequestID,
                                                                bool bIsLast);
typedef void (*ctp_trader_query_investor_position_combine_detail_rsp_cb)(CThostFtdcInvestorPositionCombineDetailField* pInvestorPositionCombineDetail,
                                                                         CThostFtdcRspInfoField* pRspInfo,
                                                                         int nRequestID,
                                                                         bool bIsLast);
typedef void (*ctp_trader_query_cfmmc_trading_account_key_rsp_cb)(CThostFtdcCFMMCTradingAccountKeyField* pCFMMCTradingAccountKey,
                                                                  CThostFtdcRspInfoField* pRspInfo,
                                                                  int nRequestID,
                                                                  bool bIsLast);
typedef void (*ctp_trader_query_ewarrant_offset_rsp_cb)(CThostFtdcEWarrantOffsetField* pEWarrantOffset,
                                                        CThostFtdcRspInfoField* pRspInfo,
                                                        int nRequestID,
                                                        bool bIsLast);
typedef void (*ctp_trader_query_investor_product_group_margin_rsp_cb)(CThostFtdcInvestorProductGroupMarginField* pInvestorProductGroupMargin,
                                                                      CThostFtdcRspInfoField* pRspInfo,
                                                                      int nRequestID,
                                                                      bool bIsLast);
typedef void (*ctp_trader_query_exchange_margin_rate_rsp_cb)(CThostFtdcExchangeMarginRateField* pExchangeMarginRate,
                                                             CThostFtdcRspInfoField* pRspInfo,
                                                             int nRequestID,
                                                             bool bIsLast);
typedef void (*ctp_trader_query_exchange_margin_rate_adjust_rsp_cb)(CThostFtdcExchangeMarginRateAdjustField* pExchangeMarginRateAdjust,
                                                                    CThostFtdcRspInfoField* pRspInfo,
                                                                    int nRequestID,
                                                                    bool bIsLast);
typedef void (*ctp_trader_query_exchange_rate_rsp_cb)(CThostFtdcExchangeRateField* pExchangeRate,
                                                      CThostFtdcRspInfoField* pRspInfo,
                                                      int nRequestID,
                                                      bool bIsLast);
typedef void (*ctp_trader_query_secagent_acidmap_rsp_cb)(CThostFtdcSecAgentACIDMapField* pSecAgentACIDMap,
                                                         CThostFtdcRspInfoField* pRspInfo,
                                                         int nRequestID,
                                                         bool bIsLast);
typedef void (*ctp_trader_query_product_exchange_rate_cb)(CThostFtdcProductExchRateField* pProductExchRate,
                                                          CThostFtdcRspInfoField* pRspInfo,
                                                          int nRequestID,
                                                          bool bIsLast);
typedef void (*ctp_trader_query_product_group_rsp_cb)(CThostFtdcProductGroupField* pProductGroup,
                                                      CThostFtdcRspInfoField* pRspInfo,
                                                      int nRequestID,
                                                      bool bIsLast);
typedef void (*ctp_trader_query_mmintrument_commission_rate_rsp_cb)(CThostFtdcMMInstrumentCommissionRateField* pMMInstrumentCommissionRate,
                                                                    CThostFtdcRspInfoField* pRspInfo,
                                                                    int nRequestID,
                                                                    bool bIsLast);
typedef void (*ctp_trader_query_mmoption_instrument_commission_rate_rsp_cb)(CThostFtdcMMOptionInstrCommRateField* pMMOptionInstrCommRate,
                                                                            CThostFtdcRspInfoField* pRspInfo,
                                                                            int nRequestID,
                                                                            bool bIsLast);
typedef void (*ctp_trader_query_instrument_order_commission_rate_rsp_cb)(CThostFtdcInstrumentOrderCommRateField* pInstrumentOrderCommRate,
                                                                         CThostFtdcRspInfoField* pRspInfo,
                                                                         int nRequestID,
                                                                         bool bIsLast);
typedef void (*ctp_trader_query_option_instrument_trade_cost_rsp_cb)(CThostFtdcOptionInstrTradeCostField* pOptionInstrTradeCost,
                                                                     CThostFtdcRspInfoField* pRspInfo,
                                                                     int nRequestID,
                                                                     bool bIsLast);
typedef void (*ctp_trader_query_option_instrument_commission_rate_rsp_cb)(CThostFtdcOptionInstrCommRateField* pOptionInstrCommRate,
                                                                          CThostFtdcRspInfoField* pRspInfo,
                                                                          int nRequestID,
                                                                          bool bIsLast);
typedef void (*ctp_trader_query_exec_order_rsp_cb)(CThostFtdcExecOrderField* pExecOrder,
                                                   CThostFtdcRspInfoField* pRspInfo,
                                                   int nRequestID,
                                                   bool bIsLast);
typedef void (*ctp_trader_query_forquote_rsp_cb)(CThostFtdcForQuoteField* pForQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_query_quote_rsp_cb)(CThostFtdcQuoteField* pQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_query_comb_instrument_guard_rsp_cb)(CThostFtdcCombInstrumentGuardField* pCombInstrumentGuard,
                                                              CThostFtdcRspInfoField* pRspInfo,
                                                              int nRequestID,
                                                              bool bIsLast);
typedef void (*ctp_trader_query_comb_action_rsp_cb)(CThostFtdcCombActionField* pCombAction,
                                                    CThostFtdcRspInfoField* pRspInfo,
                                                    int nRequestID,
                                                    bool bIsLast);
typedef void (*ctp_trader_query_transfer_serial_rsp_cb)(CThostFtdcTransferSerialField* pTransferSerial,
                                                        CThostFtdcRspInfoField* pRspInfo,
                                                        int nRequestID,
                                                        bool bIsLast);
typedef void (*ctp_trader_query_account_register_rsp_cb)(CThostFtdcAccountregisterField* pAccountregister,
                                                         CThostFtdcRspInfoField* pRspInfo,
                                                         int nRequestID,
                                                         bool bIsLast);
typedef void (*ctp_trader_error_rsp_cb)(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
typedef void (*ctp_trader_return_order_rsp_cb)(CThostFtdcOrderField* pOrder);
typedef void (*ctp_trader_return_trade_rsp_cb)(CThostFtdcTradeField* pTrade);
typedef void (*ctp_trader_return_order_insert_error_cb)(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_order_action_error_cb)(CThostFtdcOrderActionField* pOrderAction, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_instrument_status_cb)(CThostFtdcInstrumentStatusField* pInstrumentStatus);
typedef void (*ctp_trader_return_bulletin_cb)(CThostFtdcBulletinField* pBulletin);
typedef void (*ctp_trader_return_trading_notice_cb)(CThostFtdcTradingNoticeInfoField* pTradingNoticeInfo);
typedef void (*ctp_trader_return_error_conditional_order_cb)(CThostFtdcErrorConditionalOrderField* pErrorConditionalOrder);
typedef void (*ctp_trader_return_exec_order_cb)(CThostFtdcExecOrderField* pExecOrder);
typedef void (*ctp_trader_return_exec_order_insert_error_cb)(CThostFtdcInputExecOrderField* pInputExecOrder, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_excc_order_action_error_cb)(CThostFtdcExecOrderActionField* pExecOrderAction, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_forquote_insert_error_cb)(CThostFtdcInputForQuoteField* pInputForQuote, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_quote_cb)(CThostFtdcQuoteField* pQuote);
typedef void (*ctp_trader_return_quote_insert_error_cb)(CThostFtdcInputQuoteField* pInputQuote, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_quote_action_error_cb)(CThostFtdcQuoteActionField* pQuoteAction, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_forquote_rsp_cb)(CThostFtdcForQuoteRspField* pForQuoteRsp);
typedef void (*ctp_trader_return_fmmct_trading_account_token_cb)(CThostFtdcCFMMCTradingAccountTokenField* pCFMMCTradingAccountToken);
typedef void (*ctp_trader_return_batch_order_action_error_cb)(CThostFtdcBatchOrderActionField* pBatchOrderAction, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_comb_action_cb)(CThostFtdcCombActionField* pCombAction);
typedef void (*ctp_trader_return_comb_action_insert_error_cb)(CThostFtdcInputCombActionField* pInputCombAction, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_query_contract_bank_rsp_cb)(CThostFtdcContractBankField* pContractBank,
                                                      CThostFtdcRspInfoField* pRspInfo,
                                                      int nRequestID,
                                                      bool bIsLast);
typedef void (*ctp_trader_query_packed_order_rsp_cb)(CThostFtdcParkedOrderField* pParkedOrder,
                                                     CThostFtdcRspInfoField* pRspInfo,
                                                     int nRequestID,
                                                     bool bIsLast);
typedef void (*ctp_trader_query_packed_order_action_rsp_cb)(CThostFtdcParkedOrderActionField* pParkedOrderAction,
                                                            CThostFtdcRspInfoField* pRspInfo,
                                                            int nRequestID,
                                                            bool bIsLast);
typedef void (*ctp_trader_query_trading_notice_rsp_cb)(CThostFtdcTradingNoticeField* pTradingNotice,
                                                       CThostFtdcRspInfoField* pRspInfo,
                                                       int nRequestID,
                                                       bool bIsLast);
typedef void (*ctp_trader_query_broker_trading_params_rsp_cb)(CThostFtdcBrokerTradingParamsField* pBrokerTradingParams,
                                                              CThostFtdcRspInfoField* pRspInfo,
                                                              int nRequestID,
                                                              bool bIsLast);
typedef void (*ctp_trader_query_broker_trading_algos_rsp_cb)(CThostFtdcBrokerTradingAlgosField* pBrokerTradingAlgos,
                                                             CThostFtdcRspInfoField* pRspInfo,
                                                             int nRequestID,
                                                             bool bIsLast);
typedef void (*ctp_trader_query_cfmmc_trading_account_token_rsp_cb)(CThostFtdcQueryCFMMCTradingAccountTokenField* pQueryCFMMCTradingAccountToken,
                                                                    CThostFtdcRspInfoField* pRspInfo,
                                                                    int nRequestID,
                                                                    bool bIsLast);
typedef void (*ctp_trader_return_from_bank_to_future_by_bank_cb)(CThostFtdcRspTransferField* pRspTransfer);
typedef void (*ctp_trader_return_from_future_to_bank_by_bank_cb)(CThostFtdcRspTransferField* pRspTransfer);
typedef void (*ctp_trader_return_repeal_from_bank_to_future_by_bank_cb)(CThostFtdcRspRepealField* pRspRepeal);
typedef void (*ctp_trader_return_repeal_from_future_to_bank_by_bank_cb)(CThostFtdcRspRepealField* pRspRepeal);
typedef void (*ctp_trader_return_from_bank_to_future_by_future_cb)(CThostFtdcRspTransferField* pRspTransfer);
typedef void (*ctp_trader_return_from_future_to_bank_by_future_cb)(CThostFtdcRspTransferField* pRspTransfer);
typedef void (*ctp_trader_return_repeal_from_bank_to_future_by_futuremanual_cb)(CThostFtdcRspRepealField* pRspRepeal);
typedef void (*ctp_trader_return_repeal_from_future_to_bank_by_futuremanual_cb)(CThostFtdcRspRepealField* pRspRepeal);
typedef void (*ctp_trader_return_query_bank_balance_by_future_cb)(CThostFtdcNotifyQueryAccountField* pNotifyQueryAccount);
typedef void (*ctp_trader_return_bank_to_future_by_future_error_cb)(CThostFtdcReqTransferField* pReqTransfer, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_future_to_bank_by_future_error_cb)(CThostFtdcReqTransferField* pReqTransfer, CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_repeal_bank_to_future_by_futuremanual_error_cb)(CThostFtdcReqRepealField* pReqRepeal,
                                                                                 CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_repeal_future_to_bank_by_futuremanual_error_cb)(CThostFtdcReqRepealField* pReqRepeal,
                                                                                 CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_query_bank_balance_by_future_error_cb)(CThostFtdcReqQueryAccountField* pReqQueryAccount,
                                                                        CThostFtdcRspInfoField* pRspInfo);
typedef void (*ctp_trader_return_repeal_from_bank_to_future_by_future_cb)(CThostFtdcRspRepealField* pRspRepeal);
typedef void (*ctp_trader_return_repeal_from_future_to_bank_by_future_cb)(CThostFtdcRspRepealField* pRspRepeal);
typedef void (*ctp_trader_from_bank_to_future_by_future_rsp_cb)(CThostFtdcReqTransferField* pReqTransfer,
                                                                CThostFtdcRspInfoField* pRspInfo,
                                                                int nRequestID,
                                                                bool bIsLast);
typedef void (*ctp_trader_from_future_to_bank_by_future_rsp_cb)(CThostFtdcReqTransferField* pReqTransfer,
                                                                CThostFtdcRspInfoField* pRspInfo,
                                                                int nRequestID,
                                                                bool bIsLast);
typedef void (*ctp_trader_query_bank_account_monoy_by_future_rsp_cb)(CThostFtdcReqQueryAccountField* pReqQueryAccount,
                                                                     CThostFtdcRspInfoField* pRspInfo,
                                                                     int nRequestID,
                                                                     bool bIsLast);
typedef void (*ctp_trader_return_open_account_by_bank_cb)(CThostFtdcOpenAccountField* pOpenAccount);
typedef void (*ctp_trader_return_cancel_account_by_bank_cb)(CThostFtdcCancelAccountField* pCancelAccount);
typedef void (*ctp_trader_return_change_account_by_bank_cb)(CThostFtdcChangeAccountField* pChangeAccount);

// functions

ctp_md_spi_t* ctp_mdspi_create();
void ctp_mdspi_free(ctp_md_spi_t* spi);
void ctp_mdspi_set_front_connected_cb(ctp_md_spi_t* spi, ctp_md_front_connected_cb cb);
void ctp_mdspi_set_front_disconnected_cb(ctp_md_spi_t* spi, ctp_md_front_disconnected_cb cb);
void ctp_mdspi_set_heartbeat_warning_cb(ctp_md_spi_t* spi, ctp_md_heart_beat_warning_cb cb);
void ctp_mdspi_set_user_login_cb(ctp_md_spi_t* spi, ctp_md_user_login_rsp_cb cb);
void ctp_mdspi_set_user_logout_cb(ctp_md_spi_t* spi, ctp_md_user_logout_rsp_cb cb);
void ctp_mdspi_set_error_cb(ctp_md_spi_t* spi, ctp_md_error_rsp_cb cb);
void ctp_mdspi_set_sub_market_data_rsp_cb(ctp_md_spi_t* spi, ctp_md_sub_market_data_rsp_cb cb);
void ctp_mdspi_set_unsub_market_data_rsp_cb(ctp_md_spi_t* spi, ctp_md_unsub_market_data_rsp_cb cb);
void ctp_mdspi_set_sub_quote_rsp_cb(ctp_md_spi_t* spi, ctp_md_unsub_quote_rsp_cb cb);
void ctp_mdspi_set_unsub_quote_rsp_cb(ctp_md_spi_t* spi, ctp_md_unsub_quote_rsp_cb cb);
void ctp_mdspi_set_depth_market_data_cb(ctp_md_spi_t* spi, ctp_md_depth_market_data_cb cb);
void ctp_mdspi_set_quote_cb(ctp_md_spi_t* spi, ctp_md_quote_cb cb);

ctp_md_api_t* ctp_mdapi_create(const char* flowpath, bool useudp, bool multicast);
void ctp_mdapi_free(ctp_md_api_t* api);
const char* ctp_mdapi_get_api_version(ctp_md_api_t* api);
void ctp_mdapi_init(ctp_md_api_t* api);
void ctp_mdapi_join(ctp_md_api_t* api);
const char* ctp_mdapi_get_trading_day(ctp_md_api_t* api);
void ctp_mdapi_register_front(ctp_md_api_t* api, char* front_address);
void ctp_mdapi_register_nameserver(ctp_md_api_t* api, char* nameserver_address);
void ctp_mdapi_register_fens_userinfo(ctp_md_api_t* api, CThostFtdcFensUserInfoField* userinfo);
void ctp_mdapi_register_spi(ctp_md_api_t* api, ctp_md_spi_t* spi);
void ctp_mdapi_sub_market_data(ctp_md_api_t* api, char* instruments[], int count);
void ctp_mdapi_unsub_market_data(ctp_md_api_t* api, char* instruments[], int count);
void ctp_mdapi_sub_quote(ctp_md_api_t* api, char* instruments[], int count);
void ctp_mdapi_unsub_quote(ctp_md_api_t* api, char* instruments[], int count);
void ctp_mdapi_user_login(ctp_md_api_t* api, CThostFtdcReqUserLoginField* userlogin, int request_id);
void ctp_mdapi_user_logout(ctp_md_api_t* api, CThostFtdcUserLogoutField* userlogout, int request_id);

ctp_trader_spi_t* ctp_traderspi_create();
void ctp_traderspi_free(ctp_trader_spi_t* spi);
void ctp_traderspi_set_front_connected_cb(ctp_trader_spi_t* spi, ctp_trader_front_connected_cb cb);
void ctp_traderspi_set_front_disconnected_cb(ctp_trader_spi_t* spi, ctp_trader_front_disconnected_cb cb);
void ctp_traderspi_set_heart_beat_warning_cb(ctp_trader_spi_t* spi, ctp_trader_heart_beat_warning_cb cb);
void ctp_traderspi_set_authenticate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_authenticate_rsp_cb cb);
void ctp_traderspi_set_user_login_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_user_login_rsp_cb cb);
void ctp_traderspi_set_user_logout_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_user_logout_rsp_cb cb);
void ctp_traderspi_set_user_passwd_update_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_user_passwd_update_rsp_cb cb);
void ctp_traderspi_set_trading_account_passwd_update_cb(ctp_trader_spi_t* spi, ctp_trader_trading_account_passwd_update_cb cb);
void ctp_traderspi_set_order_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_order_insert_rsp_cb cb);
void ctp_traderspi_set_parked_order_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_parked_order_insert_rsp_cb cb);
void ctp_traderspi_set_packed_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_packed_order_action_rsp_cb cb);
void ctp_traderspi_set_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_order_action_rsp_cb cb);
void ctp_traderspi_set_query_max_order_volume_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_max_order_volume_rsp_cb cb);
void ctp_traderspi_set_settlement_info_confirm_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_settlement_info_confirm_rsp_cb cb);
void ctp_traderspi_set_remove_packed_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_remove_packed_order_rsp_cb cb);
void ctp_traderspi_set_remove_packed_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_remove_packed_order_action_rsp_cb cb);
void ctp_traderspi_set_exec_order_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_exec_order_insert_rsp_cb cb);
void ctp_traderspi_set_exec_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_exec_order_action_rsp_cb cb);
void ctp_traderspi_set_for_quote_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_for_quote_insert_rsp_cb cb);
void ctp_traderspi_set_quote_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_quote_insert_rsp_cb cb);
void ctp_traderspi_set_quote_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_quote_action_rsp_cb cb);
void ctp_traderspi_set_batch_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_batch_order_action_rsp_cb cb);
void ctp_traderspi_set_comb_action_insert_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_comb_action_insert_rsp_cb cb);
void ctp_traderspi_set_query_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_order_rsp_cb cb);
void ctp_traderspi_set_query_trade_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_trade_rsp_cb cb);
void ctp_traderspi_set_query_investor_position_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_investor_position_rsp_cb cb);
void ctp_traderspi_set_query_trading_account_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_trading_account_rsp_cb cb);
void ctp_traderspi_set_query_investor_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_investor_rsp_cb cb);
void ctp_traderspi_set_query_trading_code_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_trading_code_rsp_cb cb);
void ctp_traderspi_set_query_instrument_margin_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_instrument_margin_rate_rsp_cb cb);
void ctp_traderspi_set_query_instrument_commission_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_instrument_commission_rate_rsp_cb cb);
void ctp_traderspi_set_query_exchange_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exchange_rsp_cb cb);
void ctp_traderspi_set_query_product_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_product_rsp_cb cb);
void ctp_traderspi_set_query_instrument_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_instrument_rsp_cb cb);
void ctp_traderspi_set_query_depth_market_data_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_depth_market_data_rsp_cb cb);
void ctp_traderspi_set_query_settlement_info_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_settlement_info_rsp_cb cb);
void ctp_traderspi_set_query_transfer_bank_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_transfer_bank_rsp_cb cb);
void ctp_traderspi_set_query_investor_position_detail_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_investor_position_detail_rsp_cb cb);
void ctp_traderspi_set_query_notice_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_notice_rsp_cb cb);
void ctp_traderspi_set_query_settlement_info_confirm_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_settlement_info_confirm_rsp_cb cb);
void ctp_traderspi_set_query_investor_position_combine_detail_rsp_cb(ctp_trader_spi_t* spi,
                                                                     ctp_trader_query_investor_position_combine_detail_rsp_cb cb);
void ctp_traderspi_set_query_cfmmc_trading_account_key_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_cfmmc_trading_account_key_rsp_cb cb);
void ctp_traderspi_set_query_ewarrant_offset_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_ewarrant_offset_rsp_cb cb);
void ctp_traderspi_set_query_investor_product_group_margin_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_investor_product_group_margin_rsp_cb cb);
void ctp_traderspi_set_query_exchange_margin_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exchange_margin_rate_rsp_cb cb);
void ctp_traderspi_set_query_exchange_margin_rate_adjust_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exchange_margin_rate_adjust_rsp_cb cb);
void ctp_traderspi_set_query_exchange_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exchange_rate_rsp_cb cb);
void ctp_traderspi_set_query_secagent_acidmap_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_secagent_acidmap_rsp_cb cb);
void ctp_traderspi_set_query_product_exchange_rate_cb(ctp_trader_spi_t* spi, ctp_trader_query_product_exchange_rate_cb cb);
void ctp_traderspi_set_query_product_group_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_product_group_rsp_cb cb);
void ctp_traderspi_set_query_mmintrument_commission_rate_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_mmintrument_commission_rate_rsp_cb cb);
void ctp_traderspi_set_query_mmoption_instrument_commission_rate_rsp_cb(ctp_trader_spi_t* spi,
                                                                        ctp_trader_query_mmoption_instrument_commission_rate_rsp_cb cb);
void ctp_traderspi_set_query_instrument_order_commission_rate_rsp_cb(ctp_trader_spi_t* spi,
                                                                     ctp_trader_query_instrument_order_commission_rate_rsp_cb cb);
void ctp_traderspi_set_query_option_instrument_trade_cost_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_option_instrument_trade_cost_rsp_cb cb);
void ctp_traderspi_set_query_option_instrument_commission_rate_rsp_cb(ctp_trader_spi_t* spi,
                                                                      ctp_trader_query_option_instrument_commission_rate_rsp_cb cb);
void ctp_traderspi_set_query_exec_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_exec_order_rsp_cb cb);
void ctp_traderspi_set_query_forquote_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_forquote_rsp_cb cb);
void ctp_traderspi_set_query_quote_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_quote_rsp_cb cb);
void ctp_traderspi_set_query_comb_instrument_guard_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_comb_instrument_guard_rsp_cb cb);
void ctp_traderspi_set_query_comb_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_comb_action_rsp_cb cb);
void ctp_traderspi_set_query_transfer_serial_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_transfer_serial_rsp_cb cb);
void ctp_traderspi_set_query_account_register_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_account_register_rsp_cb cb);
void ctp_traderspi_set_error_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_error_rsp_cb cb);
void ctp_traderspi_set_return_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_return_order_rsp_cb cb);
void ctp_traderspi_set_return_trade_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_return_trade_rsp_cb cb);
void ctp_traderspi_set_return_order_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_order_insert_error_cb cb);
void ctp_traderspi_set_return_order_action_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_order_action_error_cb cb);
void ctp_traderspi_set_return_instrument_status_cb(ctp_trader_spi_t* spi, ctp_trader_return_instrument_status_cb cb);
void ctp_traderspi_set_return_bulletin_cb(ctp_trader_spi_t* spi, ctp_trader_return_bulletin_cb cb);
void ctp_traderspi_set_return_trading_notice_cb(ctp_trader_spi_t* spi, ctp_trader_return_trading_notice_cb cb);
void ctp_traderspi_set_return_error_conditional_order_cb(ctp_trader_spi_t* spi, ctp_trader_return_error_conditional_order_cb cb);
void ctp_traderspi_set_return_exec_order_cb(ctp_trader_spi_t* spi, ctp_trader_return_exec_order_cb cb);
void ctp_traderspi_set_return_exec_order_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_exec_order_insert_error_cb cb);
void ctp_traderspi_set_return_excc_order_action_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_excc_order_action_error_cb cb);
void ctp_traderspi_set_return_forquote_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_forquote_insert_error_cb cb);
void ctp_traderspi_set_return_quote_cb(ctp_trader_spi_t* spi, ctp_trader_return_quote_cb cb);
void ctp_traderspi_set_return_quote_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_quote_insert_error_cb cb);
void ctp_traderspi_set_return_quote_action_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_quote_action_error_cb cb);
void ctp_traderspi_set_return_forquote_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_return_forquote_rsp_cb cb);
void ctp_traderspi_set_return_fmmct_trading_account_token_cb(ctp_trader_spi_t* spi, ctp_trader_return_fmmct_trading_account_token_cb cb);
void ctp_traderspi_set_return_batch_order_action_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_batch_order_action_error_cb cb);
void ctp_traderspi_set_return_comb_action_cb(ctp_trader_spi_t* spi, ctp_trader_return_comb_action_cb cb);
void ctp_traderspi_set_return_comb_action_insert_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_comb_action_insert_error_cb cb);
void ctp_traderspi_set_query_contract_bank_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_contract_bank_rsp_cb cb);
void ctp_traderspi_set_query_packed_order_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_packed_order_rsp_cb cb);
void ctp_traderspi_set_query_packed_order_action_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_packed_order_action_rsp_cb cb);
void ctp_traderspi_set_query_trading_notice_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_trading_notice_rsp_cb cb);
void ctp_traderspi_set_query_broker_trading_params_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_broker_trading_params_rsp_cb cb);
void ctp_traderspi_set_query_broker_trading_algos_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_broker_trading_algos_rsp_cb cb);
void ctp_traderspi_set_query_cfmmc_trading_account_token_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_cfmmc_trading_account_token_rsp_cb cb);
void ctp_traderspi_set_return_from_bank_to_future_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_from_bank_to_future_by_bank_cb cb);
void ctp_traderspi_set_return_from_future_to_bank_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_from_future_to_bank_by_bank_cb cb);
void ctp_traderspi_set_return_repeal_from_bank_to_future_by_bank_cb(ctp_trader_spi_t* spi,
                                                                    ctp_trader_return_repeal_from_bank_to_future_by_bank_cb cb);
void ctp_traderspi_set_return_repeal_from_future_to_bank_by_bank_cb(ctp_trader_spi_t* spi,
                                                                    ctp_trader_return_repeal_from_future_to_bank_by_bank_cb cb);
void ctp_traderspi_set_return_from_bank_to_future_by_future_cb(ctp_trader_spi_t* spi, ctp_trader_return_from_bank_to_future_by_future_cb cb);
void ctp_traderspi_set_return_from_future_to_bank_by_future_cb(ctp_trader_spi_t* spi, ctp_trader_return_from_future_to_bank_by_future_cb cb);
void ctp_traderspi_set_return_repeal_from_bank_to_future_by_futuremanual_cb(ctp_trader_spi_t* spi,
                                                                            ctp_trader_return_repeal_from_bank_to_future_by_futuremanual_cb cb);
void ctp_traderspi_set_return_repeal_from_future_to_bank_by_futuremanual_cb(ctp_trader_spi_t* spi,
                                                                            ctp_trader_return_repeal_from_future_to_bank_by_futuremanual_cb cb);
void ctp_traderspi_set_return_query_bank_balance_by_future_cb(ctp_trader_spi_t* spi, ctp_trader_return_query_bank_balance_by_future_cb cb);
void ctp_traderspi_set_return_bank_to_future_by_future_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_bank_to_future_by_future_error_cb cb);
void ctp_traderspi_set_return_future_to_bank_by_future_error_cb(ctp_trader_spi_t* spi, ctp_trader_return_future_to_bank_by_future_error_cb cb);
void ctp_traderspi_set_return_repeal_bank_to_future_by_futuremanual_error_cb(ctp_trader_spi_t* spi,
                                                                             ctp_trader_return_repeal_bank_to_future_by_futuremanual_error_cb cb);
void ctp_traderspi_set_return_repeal_future_to_bank_by_futuremanual_error_cb(ctp_trader_spi_t* spi,
                                                                             ctp_trader_return_repeal_future_to_bank_by_futuremanual_error_cb cb);
void ctp_traderspi_set_return_query_bank_balance_by_future_error_cb(ctp_trader_spi_t* spi,
                                                                    ctp_trader_return_query_bank_balance_by_future_error_cb cb);
void ctp_traderspi_set_return_repeal_from_bank_to_future_by_future_cb(ctp_trader_spi_t* spi,
                                                                      ctp_trader_return_repeal_from_bank_to_future_by_future_cb cb);
void ctp_traderspi_set_return_repeal_from_future_to_bank_by_future_cb(ctp_trader_spi_t* spi,
                                                                      ctp_trader_return_repeal_from_future_to_bank_by_future_cb cb);
void ctp_traderspi_set_from_bank_to_future_by_future_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_from_bank_to_future_by_future_rsp_cb cb);
void ctp_traderspi_set_from_future_to_bank_by_future_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_from_future_to_bank_by_future_rsp_cb cb);
void ctp_traderspi_set_query_bank_account_monoy_by_future_rsp_cb(ctp_trader_spi_t* spi, ctp_trader_query_bank_account_monoy_by_future_rsp_cb cb);
void ctp_traderspi_set_return_open_account_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_open_account_by_bank_cb cb);
void ctp_traderspi_set_return_cancel_account_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_cancel_account_by_bank_cb cb);
void ctp_traderspi_set_return_change_account_by_bank_cb(ctp_trader_spi_t* spi, ctp_trader_return_change_account_by_bank_cb cb);

ctp_trader_api_t* ctp_traderapi_create(const char* flowpath);
void ctp_traderapi_free(ctp_trader_api_t* api);
const char* ctp_traderapi_get_api_version(ctp_trader_api_t* api);
void ctp_traderapi_init(ctp_trader_api_t* api);
int ctp_traderapi_join(ctp_trader_api_t* api);
const char* ctp_traderapi_get_trading_day(ctp_trader_api_t* api);
void ctp_traderapi_register_front(ctp_trader_api_t* api, char* pszFrontAddress);
void ctp_traderapi_register_nameserver(ctp_trader_api_t* api, char* pszNsAddress);
void ctp_traderapi_register_fens_userinfo(ctp_trader_api_t* api, CThostFtdcFensUserInfoField* pFensUserInfo);
void ctp_traderapi_register_spi(ctp_trader_api_t* api, ctp_trader_spi_t* spi);
void ctp_traderapi_sub_private_topic(ctp_trader_api_t* api, THOST_TE_RESUME_TYPE nResumeType);
void ctp_traderapi_sub_public_topic(ctp_trader_api_t* api, THOST_TE_RESUME_TYPE nResumeType);
int ctp_traderapi_req_authenticate(ctp_trader_api_t* api, CThostFtdcReqAuthenticateField* pReqAuthenticateField, int nRequestID);
int ctp_traderapi_req_user_login(ctp_trader_api_t* api, CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID);
int ctp_traderapi_req_user_logout(ctp_trader_api_t* api, CThostFtdcUserLogoutField* pUserLogout, int nRequestID);
int ctp_traderapi_req_user_passwd_update(ctp_trader_api_t* api, CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate, int nRequestID);
int ctp_traderapi_req_trading_account_passwd_update(ctp_trader_api_t* api,
                                                    CThostFtdcTradingAccountPasswordUpdateField* pTradingAccountPasswordUpdate,
                                                    int nRequestID);
int ctp_traderapi_req_order_insert(ctp_trader_api_t* api, CThostFtdcInputOrderField* pInputOrder, int nRequestID);
int ctp_traderapi_req_parked_order_insert(ctp_trader_api_t* api, CThostFtdcParkedOrderField* pParkedOrder, int nRequestID);
int ctp_traderapi_req_packed_order_action(ctp_trader_api_t* api, CThostFtdcParkedOrderActionField* pParkedOrderAction, int nRequestID);
int ctp_traderapi_req_order_action(ctp_trader_api_t* api, CThostFtdcInputOrderActionField* pInputOrderAction, int nRequestID);
int ctp_traderapi_req_query_max_order_volume(ctp_trader_api_t* api, CThostFtdcQueryMaxOrderVolumeField* pQueryMaxOrderVolume, int nRequestID);
int ctp_traderapi_req_settlement_info_confirm(ctp_trader_api_t* api, CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, int nRequestID);
int ctp_traderapi_req_remove_parked_order(ctp_trader_api_t* api, CThostFtdcRemoveParkedOrderField* pRemoveParkedOrder, int nRequestID);
int ctp_traderapi_req_remove_parked_order_action(ctp_trader_api_t* api,
                                                 CThostFtdcRemoveParkedOrderActionField* pRemoveParkedOrderAction,
                                                 int nRequestID);
int ctp_traderapi_req_exec_order_insert(ctp_trader_api_t* api, CThostFtdcInputExecOrderField* pInputExecOrder, int nRequestID);
int ctp_traderapi_req_exec_order_action(ctp_trader_api_t* api, CThostFtdcInputExecOrderActionField* pInputExecOrderAction, int nRequestID);
int ctp_traderapi_req_forquote_insert(ctp_trader_api_t* api, CThostFtdcInputForQuoteField* pInputForQuote, int nRequestID);
int ctp_traderapi_req_quote_insert(ctp_trader_api_t* api, CThostFtdcInputQuoteField* pInputQuote, int nRequestID);
int ctp_traderapi_req_quote_action(ctp_trader_api_t* api, CThostFtdcInputQuoteActionField* pInputQuoteAction, int nRequestID);
int ctp_traderapi_req_batch_order_action(ctp_trader_api_t* api, CThostFtdcInputBatchOrderActionField* pInputBatchOrderAction, int nRequestID);
int ctp_traderapi_req_comb_action_insert(ctp_trader_api_t* api, CThostFtdcInputCombActionField* pInputCombAction, int nRequestID);
int ctp_traderapi_req_query_order(ctp_trader_api_t* api, CThostFtdcQryOrderField* pQryOrder, int nRequestID);
int ctp_traderapi_req_query_trade(ctp_trader_api_t* api, CThostFtdcQryTradeField* pQryTrade, int nRequestID);
int ctp_traderapi_req_query_investor_position(ctp_trader_api_t* api, CThostFtdcQryInvestorPositionField* pQryInvestorPosition, int nRequestID);
int ctp_traderapi_req_query_trading_account(ctp_trader_api_t* api, CThostFtdcQryTradingAccountField* pQryTradingAccount, int nRequestID);
int ctp_traderapi_req_query_investor(ctp_trader_api_t* api, CThostFtdcQryInvestorField* pQryInvestor, int nRequestID);
int ctp_traderapi_req_query_trading_code(ctp_trader_api_t* api, CThostFtdcQryTradingCodeField* pQryTradingCode, int nRequestID);
int ctp_traderapi_req_query_instrument_margin_rate(ctp_trader_api_t* api,
                                                   CThostFtdcQryInstrumentMarginRateField* pQryInstrumentMarginRate,
                                                   int nRequestID);
int ctp_traderapi_req_query_instrument_commission_rate(ctp_trader_api_t* api,
                                                       CThostFtdcQryInstrumentCommissionRateField* pQryInstrumentCommissionRate,
                                                       int nRequestID);
int ctp_traderapi_req_query_exchage(ctp_trader_api_t* api, CThostFtdcQryExchangeField* pQryExchange, int nRequestID);
int ctp_traderapi_req_query_product(ctp_trader_api_t* api, CThostFtdcQryProductField* pQryProduct, int nRequestID);
int ctp_traderapi_req_query_instrument(ctp_trader_api_t* api, CThostFtdcQryInstrumentField* pQryInstrument, int nRequestID);
int ctp_traderapi_req_query_depth_market_dat(ctp_trader_api_t* api, CThostFtdcQryDepthMarketDataField* pQryDepthMarketData, int nRequestID);
int ctp_traderapi_req_query_settlement_info(ctp_trader_api_t* api, CThostFtdcQrySettlementInfoField* pQrySettlementInfo, int nRequestID);
int ctp_traderapi_req_query_transfer_bank(ctp_trader_api_t* api, CThostFtdcQryTransferBankField* pQryTransferBank, int nRequestID);
int ctp_traderapi_req_query_investor_position_detail(ctp_trader_api_t* api,
                                                     CThostFtdcQryInvestorPositionDetailField* pQryInvestorPositionDetail,
                                                     int nRequestID);
int ctp_traderapi_req_query_notice(ctp_trader_api_t* api, CThostFtdcQryNoticeField* pQryNotice, int nRequestID);
int ctp_traderapi_req_query_settlement_info_confirm(ctp_trader_api_t* api,
                                                    CThostFtdcQrySettlementInfoConfirmField* pQrySettlementInfoConfirm,
                                                    int nRequestID);
int ctp_traderapi_req_query_investor_position_combine_detail(ctp_trader_api_t* api,
                                                             CThostFtdcQryInvestorPositionCombineDetailField* pQryInvestorPositionCombineDetail,
                                                             int nRequestID);
int ctp_traderapi_req_query_cfmmc_trading_account_key(ctp_trader_api_t* api,
                                                      CThostFtdcQryCFMMCTradingAccountKeyField* pQryCFMMCTradingAccountKey,
                                                      int nRequestID);
int ctp_traderapi_req_query_ewarrent_offset(ctp_trader_api_t* api, CThostFtdcQryEWarrantOffsetField* pQryEWarrantOffset, int nRequestID);
int ctp_traderapi_req_query_investor_product_group_margin(ctp_trader_api_t* api,
                                                          CThostFtdcQryInvestorProductGroupMarginField* pQryInvestorProductGroupMargin,
                                                          int nRequestID);
int ctp_traderapi_req_query_exchange_margin_rate(ctp_trader_api_t* api, CThostFtdcQryExchangeMarginRateField* pQryExchangeMarginRate, int nRequestID);
int ctp_traderapi_req_query_exchange_margin_rate_adjust(ctp_trader_api_t* api,
                                                        CThostFtdcQryExchangeMarginRateAdjustField* pQryExchangeMarginRateAdjust,
                                                        int nRequestID);
int ctp_traderapi_req_query_exchage_rate(ctp_trader_api_t* api, CThostFtdcQryExchangeRateField* pQryExchangeRate, int nRequestID);
int ctp_traderapi_req_query_sec_agent_acidmap(ctp_trader_api_t* api, CThostFtdcQrySecAgentACIDMapField* pQrySecAgentACIDMap, int nRequestID);
int ctp_traderapi_req_query_product_exchange_rate(ctp_trader_api_t* api, CThostFtdcQryProductExchRateField* pQryProductExchRate, int nRequestID);
int ctp_traderapi_req_query_product_group(ctp_trader_api_t* api, CThostFtdcQryProductGroupField* pQryProductGroup, int nRequestID);
int ctp_traderapi_req_query_mminstrument_commission_rate(ctp_trader_api_t* api,
                                                         CThostFtdcQryMMInstrumentCommissionRateField* pQryMMInstrumentCommissionRate,
                                                         int nRequestID);
int ctp_traderapi_req_query_mmoption_instrument_commission_rate(ctp_trader_api_t* api,
                                                                CThostFtdcQryMMOptionInstrCommRateField* pQryMMOptionInstrCommRate,
                                                                int nRequestID);
int ctp_traderapi_req_query_instrument_order_commission_rate(ctp_trader_api_t* api,
                                                             CThostFtdcQryInstrumentOrderCommRateField* pQryInstrumentOrderCommRate,
                                                             int nRequestID);
int ctp_traderapi_req_query_option_instrument_trade_cost(ctp_trader_api_t* api,
                                                         CThostFtdcQryOptionInstrTradeCostField* pQryOptionInstrTradeCost,
                                                         int nRequestID);
int ctp_traderapi_req_query_option_commission_rate(ctp_trader_api_t* api,
                                                   CThostFtdcQryOptionInstrCommRateField* pQryOptionInstrCommRate,
                                                   int nRequestID);
int ctp_traderapi_req_query_exex_order(ctp_trader_api_t* api, CThostFtdcQryExecOrderField* pQryExecOrder, int nRequestID);
int ctp_traderapi_req_query_forquote(ctp_trader_api_t* api, CThostFtdcQryForQuoteField* pQryForQuote, int nRequestID);
int ctp_traderapi_req_query_quote(ctp_trader_api_t* api, CThostFtdcQryQuoteField* pQryQuote, int nRequestID);
int ctp_traderapi_req_query_comb_instrument_guard(ctp_trader_api_t* api,
                                                  CThostFtdcQryCombInstrumentGuardField* pQryCombInstrumentGuard,
                                                  int nRequestID);
int ctp_traderapi_req_query_comb_action(ctp_trader_api_t* api, CThostFtdcQryCombActionField* pQryCombAction, int nRequestID);
int ctp_traderapi_req_query_transfer_serial(ctp_trader_api_t* api, CThostFtdcQryTransferSerialField* pQryTransferSerial, int nRequestID);
int ctp_traderapi_req_query_account_register(ctp_trader_api_t* api, CThostFtdcQryAccountregisterField* pQryAccountregister, int nRequestID);
int ctp_traderapi_req_query_contract_bank(ctp_trader_api_t* api, CThostFtdcQryContractBankField* pQryContractBank, int nRequestID);
int ctp_traderapi_req_query_parked_order(ctp_trader_api_t* api, CThostFtdcQryParkedOrderField* pQryParkedOrder, int nRequestID);
int ctp_traderapi_req_query_parked_order_action(ctp_trader_api_t* api, CThostFtdcQryParkedOrderActionField* pQryParkedOrderAction, int nRequestID);
int ctp_traderapi_req_query_trading_notice(ctp_trader_api_t* api, CThostFtdcQryTradingNoticeField* pQryTradingNotice, int nRequestID);
int ctp_traderapi_req_query_broker_trading_params(ctp_trader_api_t* api,
                                                  CThostFtdcQryBrokerTradingParamsField* pQryBrokerTradingParams,
                                                  int nRequestID);
int ctp_traderapi_req_query_broker_trading_algos(ctp_trader_api_t* api, CThostFtdcQryBrokerTradingAlgosField* pQryBrokerTradingAlgos, int nRequestID);
int ctp_traderapi_req_query_cfmmc_trading_account_token(ctp_trader_api_t* api,
                                                        CThostFtdcQueryCFMMCTradingAccountTokenField* pQueryCFMMCTradingAccountToken,
                                                        int nRequestID);
int ctp_traderapi_req_from_bank_to_future_by_future(ctp_trader_api_t* api, CThostFtdcReqTransferField* pReqTransfer, int nRequestID);
int ctp_traderapi_req_from_future_to_bank_by_future(ctp_trader_api_t* api, CThostFtdcReqTransferField* pReqTransfer, int nRequestID);
int ctp_traderapi_req_query_bank_account_money_by_future(ctp_trader_api_t* api, CThostFtdcReqQueryAccountField* pReqQueryAccount, int nRequestID);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif  // CTP_CAPI_H_