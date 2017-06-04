
primitive StockSh
primitive StockSz

type StockType is (StockSh | StockSz)

primitive CodeFund
primitive CodeStock

type CodeType is (CodeFund | CodeStock)


primitive StockUtil
  fun get_code_type(code: String): CodeType =>
    """
    判断代码是属于那种类型，目前仅支持 ['fund', 'stock']
    :return str 返回code类型, fund 基金 stock 股票
    """
    var t: CodeType = CodeStock
    t

  fun get_stock_type(code: String): StockType =>
    """
    判断股票ID对应的证券市场
    匹配规则
    ['50', '51', '60', '90', '110'] 为 sh
    ['00', '13', '18', '15', '16', '18', '20', '30', '39', '115'] 为 sz
    ['5', '6', '9'] 开头的为 sh， 其余为 sz
    :param stock_code:股票ID, 若以 'sz', 'sh' 开头直接返回对应类型，否则使用内置规则判断
    :return 'sh' or 'sz'
    """
    var t = StockSz
    t

  fun default_stock_file_path(): String =>
    """
    返回默认的股票信息文件的保存路径
    """
    var path = ""
    path