
interface CrawlerHandler
  fun stock_http_api(): String
    """ 返回远端的访问地址 """

  fun max_crawl_concurrency(): U32
    """ 设置最大可以并发抓取的HTTP请求，在需要间隔请求时，允许在一个间隔中发出多个请求 """

  fun format_response_data(content: String): JsonDoc
    """ 实现远端抓取返回数据的格式化 """

  fun hdf5_file_name(): String
    """ 返回保存HDF5目标文件名，使用Hdf5file对数据进行保存 """
