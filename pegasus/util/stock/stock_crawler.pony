"""
定义用于抓取的抽象类，用于实时抓取数据

需自定义行为
  fun max_crawl_number()
  fun stock_api()
  fun format_response_data()
"""
trait StockCrawler
  fun real()

  fun market_snapshot()

  fun get_stock_data()
    """
    获取设定的股票信息，由多次get_stock_by_range()调用的返回结果组成
    """

  fun get_stock_by_range()
    """
    在设定的范围内获取股票的数据，获取的条数可设定
    """


  fun gen_crawl_list()
    """
    生成抓取的stock list，格式[[], [], []...]
    """

  fun format_response_data()

  fun max_crawl_number()

  fun stock_api()
