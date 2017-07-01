
use "time"

primitive CodeFund
primitive CodeStock

type CodeType is (CodeFund | CodeStock)

primitive ExchangeSh
primitive ExchangeSz

type ExchangeType is (ExchangeSh | ExchangeSz)

primitive StockUtil
  """
  为了表达复杂的金融信息，定义以下概念：

  stock           股票，在股票交易所上有唯一代码可以进行交易的交易物，包含个股和基金
  code            股票代码，即6位数字代码
  exchange        交易所
  codex           交易所编码，考虑到很多查询，都需要用exchange+code的方式查询，如sh000001，定义codex用于和code区分
  spcode          special code简写，为了直观称呼，定义了指数代码（sh=上证指数 sz=深圳成指 hs300=沪深300指数 sz50=上证50 zxb=中小板 cyb=创业板）
  code type       股票的类型，分别为基金和个股
  exchange type   交易所的类型，分别指上交所和深交所，用{sh, sz}指代

  StockUtil实现股票的各种支持功能
  """
  fun get_codex(code: String): String =>
    """ 返回股票的交易所编码 """
    code

  fun get_code_type(code: String): CodeType =>
    """
    判断代码是属于那种类型，目前仅支持 ['fund', 'share']
    :return str 返回code类型, fund 基金 share 个股
    """
    var t: CodeType = CodeStock
    t

  fun get_stock_exchange(code: String): ExchangeType =>
    """
    判断股票ID对应的证券市场
    匹配规则
    ['50', '51', '60', '90', '110'] 为 sh
    ['00', '13', '18', '15', '16', '18', '20', '30', '39', '115'] 为 sz
    ['5', '6', '9'] 开头的为 sh， 其余为 sz
    :param stock_code:股票ID, 若以 'sz', 'sh' 开头直接返回对应类型，否则使用内置规则判断
    :return 'sh' or 'sz'
    """
    var t = ExchangeSz
    t

  fun is_tradetime(date: Date): Bool =>
    """ 输入的时间是不是可以交易？ """
    false

  fun seconds_to_next_trade_time(): I32 =>
    """ 离下一次交易时间还有多少秒？ """
    0