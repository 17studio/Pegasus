interface StockRealtimeCrawler
  """
  定义用于抓取的抽象类，用于抓取实时数据，参考
    https://github.com/shidenggui/easytrader
    https://github.com/shidenggui/easyquotation

  实时抓取应用的场景为定时获得市场数据，定时获得市场数据有助于实现以下功能：
    1. 保存数据到数据仓库，以便将来重用
    2. 提供实时数据功能，方便实现交易

  基于现在的系统设计，定时获取的数据会广播到相关者处，所以把该功能归纳到接口环节

  需自定义行为
    fun max_crawl_number() 最大抓取条数
    fun stock_api() 抓取的远端api
    fun format_response_data() 格式化抓取的数据
  """
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
