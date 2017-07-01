trait StockHistoryCrawler
  """
  定义用于抓取的抽象类，用于抓取历史数据

  需自定义行为
    fun max_crawl_interval() 每条记录抓取时间间隔
    fun stock_api() 抓取的远端api
    fun format_response_data() 格式化抓取的数据

  具体实现参考easyquotation: https://github.com/shidenggui/easyquotation
  """
  fun history(code: String) =>
    """抓取单只股票的数据"""
    None

  fun market_snapshot()
    """抓取当前市场的所有数据"""

  fun get_stock_by_range(seq: Seq[String])
    """获取设定的股票信息，由多次_crawl_stock_data()调用的返回结果组成"""

  fun _crawl_stock_data(code: String)
    """在设定的范围内获取股票的数据，获取的条数可设定"""

  fun _generate_crawl_list()
    """生成抓取的stock list，格式[[], [], []...]"""

  fun format_response_data()

  fun max_crawl_interval()
    """设置最大可以并发抓取的HTTP请求"""

  fun stock_api()
