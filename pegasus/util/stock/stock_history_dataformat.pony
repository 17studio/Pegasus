
class StockHistoyDataFormat
  """
  date：日期
  open：开盘价
  high：最高价
  close：收盘价
  low：最低价
  volume：成交量
  [amount]: 成交金额 成交量x成交价 考虑到在一天的时间内，成交价可能不同于收盘价，在此时成交金额有意义
  [turnover]:换手率[注：指数无此项]，换手率=某一段时期内的成交量/发行总股数×100%（在中国：成交量/流通股本×100%）

  考虑到实际的API，提供的数据各不相同，所以抽取公共部分作为收入，同时也规避因为数据源变化，
  造成策略无法使用的情况

  某些特殊的策略，可能会依赖某些特定的数据源，对此情况，目前暂不做考虑
  """