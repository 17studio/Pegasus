
actor _Init
  """
  初始化启动，检查运行环境，如果需要，则初始化环境

  1. 读入配置文件（需要远端股票列表）
  2. 根据配置文件，获取相应数据，并保存到文件中

  文件格式参考hdf5的保存格式说明，写入数据有非覆盖和覆盖两种模式，
  根据配置文件决定最终采用的保存模式
  """
