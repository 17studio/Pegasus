
use "time"

class HistoryHandler is HTTPHandler


class HistoryHandlerFactory is HandlerFactory


class _CrawlIntervalNotify is TimerNotify


actor StockHistoryCrawler
  """
  实际负责实现历史数据抓取的actor

  StockHistoryCrawler从配置文件中读入需要抓取的列表（如果缺少配置文件，则生成配置文件），
  然后创建远程抓取的任务，持续抓取并写到文件系统中，完成后，通知父类，工作完成，然后关闭相关资源
  """
  let _timers: Timers

  new create(parent: MasterActor) =>
    None

