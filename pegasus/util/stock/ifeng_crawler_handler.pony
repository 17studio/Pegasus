/*
 * 需要注意特殊编码的情况，简写列表，为了找出这些简写，可以提供特殊的函数：
 *   INDEX_LIST = {'sh': 'sh000001', 'sz': 'sz399001', 'hs300': 'sz399300',
 *              'sz50': 'sh000016', 'zxb': 'sz399005', 'cyb': 'sz399006', 'zx300': 'sz399008', 'zh500':'sh000905'}
 *
 * 正常情况，参照以下方式的编码格式：
 *
 * K线：日 周 月
 *   DAY_PRICE_URL = '%sapi.finance.%s/%s/?code=%s&type=last'
 *   http://api.finance.ifeng.com/akdaily/?code=sh000001&type=last
 *
 * K线：5分钟、15分钟、30分钟和60分钟
 *   DAY_PRICE_MIN_URL = '%sapi.finance.%s/akmin?scode=%s&type=%s'
 *   http://api.finance.ifeng.com/akmin?scode=sh000001&type=5
 *
 * 以上URL来源：tushare -> get_hist_data
 */

class IFengCrawlerHandler is CrawlerHandler
  fun stock_http_api(): String =>
    None

  fun max_crawl_concurrency(): U32 =>
    None

  fun format_response_data(content: String): JsonDoc =>
    None

  fun hdf5_file_name(): String =>
    None
