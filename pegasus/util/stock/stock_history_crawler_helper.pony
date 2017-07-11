
primitive HistoryFromSina
primitive HistoryFromTencent
primitive HistoryFromIFeng
primitive HistoryFromNetease
primitive HistoryFromEasyMoney
primitive HistoryFromIFeng

type HistoryDataSource is
    ( HistoryFromSina
    | HistoryFromTencent
    | HistoryFromIFeng
    | HistoryFromNetease
    )

primitive HistoryDay
primitive HistoryWeek
primitive HistoryMonth
primitive HistoryMinute5
primitive HistoryMinute15
primitive HistoryMinute30
primitive HistoryMinute60
primitive HistoryTick

type HistoryType is
    ( HistoryDay
    | HistoryWeek
    | HistoryMonth
    | HistoryMinute5
    | HistoryMinute15
    | HistoryMinute30
    | HistoryMinute60
    | HistoryTick
    )

class StockHistoryCrawlerHelper
  """
  定义用于抓取的抽象类，用于抓取历史数据，方便建立数据仓库
  具体实现参考tushare：https://github.com/waditu/tushare

  根据http://tushare.org/trading.html，tushare现在提供了三种按时间间隔数据获取接口，分别为：
  - get_hist_data 返回数据内容更多，但有日期的局限
  - get_h_data 可以获取自上市日开始的所有数据，但是开始日期需要通过远端获取
  - get_k_data 缺少成交额数据

  考虑到某些策略基于tick数据建立，有实时性要求，还需要实现对tick历史数据的获取，tushare提供：
  - get_tick_data 获取历史分笔数据
  - get_today_ticks 获取今日分笔数据

  历史数据重建的步骤为：
  1. 从接口获取历史数据，需要支持定时抓取
  2. 格式化历史数据
  3. 保存到数据文件

  考虑到实际运行中，远端数据源的处理是不稳定的，抓取的实现方式是提供默认数据源，如果需要调整，则通过创建参数设定。

  实际操作中，需要注意的情况是，股票可能出现的退市情况，使用日期遍历数据的做法是需要修正的
  """
  let _normal_crawler: CrawlerHandler
  let _kick_crawler: CrawlerHandler

  new create(normal: HistoryDataSource = HistoryFromIFeng,
      kick: HistoryDataSource = HistoryFromSina, retry: Bool = false) =>
    None

  fun get_history(code: String, type: HistoryType, start: Date, stop: Date): JsonDoc =>
    """ 抓取单只股票的数据 """
    None

  fun get_history_by_range(seq: Seq[String], type: HistoryType, start: Date, stop: Date,
      interval_milsecs: U32 = 100, max_crawl_number: U32 = 0): Array[JsonDoc] =>
    """ 获取设定的批量股票信息，由多次_crawl_stock_data()调用的返回结果组成 """
    None

  fun save_to_hdf5(code: String, type: HistoryType, data: JsonDoc) =>
    None

  fun _crawl_stock_data(code: String) =>
    """ 在设定的范围内获取股票的数据，获取的条数可设定 """
    None

  fun _generate_crawl_list() =>
    """ 生成抓取的stock list，格式[[], [], []...] """
    None

