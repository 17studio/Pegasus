
use "time"

class HistoryHandler is HTTPHandler


class HistoryHandlerFactory is HandlerFactory


class _CrawlIntervalNotify is TimerNotify


actor StockHistoryCrawler
  """
  实际负责实现历史数据抓取的类

  StockHistoryCrawler创建
  """
  new create(parent: MasterActor) =>
    None

