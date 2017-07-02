
use "collections"
use "json"
use "files"
use "net/http"
use "debug"

class CodeRequestHandler is HTTPHandler
  /*
   * 因为对网络的操作是在另外的ACTOR中执行，主调ACTOR无法干预执行的步骤，
   * 所以，主调ACTOR应该采取等待完成，并把执行方法，交由网络调度ACTOR执行（回调模式）
   */
  let _fn: {(FilePath)} val
  let _file_path: FilePath val
  let _session: HTTPSession

  new create(fn :{(FilePath)} val, path: FilePath, session: HTTPSession) =>
    _fn = fn
    _file_path = path
    _session = session

  fun ref chunk(data': ByteSeq val) =>
    _fn(_file_path)

  fun finished() =>
    _session.dispose()

class CodeRequestHandlerFactory is HandlerFactory
  let _fn: {(FilePath)} val
  let _file_path: FilePath val

  new val create(fn: {(FilePath)} val, path: FilePath) =>
    _fn = fn
    _file_path = path

  fun apply(session: HTTPSession): HTTPHandler =>
    var h = CodeRequestHandler(_fn, _file_path, session)
    h

class StockConfigure
  """
  完成对股票的配置管理，包括
  - 编码的获取、更新
  - 编码的保存、读取
  """
  /*
  从文件里读取相关数据，在pony里面，顺序执行的代码，是有可能会存在耗时操作的（定义>1ms的操作为耗时操作？）
  如文件读写发生在机械硬盘且需要物理读写时，操作较慢，因为文件操作是耗时操作，所以考虑使用系统库提供的带二级缓存的文件实现

  pony提供了基于二级缓存文件实现之上的json库，包含：JsonDoc, JsonArray, JsonObject...
  */
  let _auth: AmbientAuth
  let _code_url: String = "http://www.shdjt.com/js/lib/astock.js"
  let _config_name: String // 设置默认的股票信息文件的保存路径
  let _config_path: FilePath
  var _config_doc: JsonDoc = JsonDoc.create() // 从文件中读取出来的Json文档
  var _handler_factory: (CodeRequestHandlerFactory val | None) = None // 用于保存处理远程请求的句柄工厂，方便查询运行情况

  new create(auth: AmbientAuth, name: String) ? =>
    _auth = auth
    _config_name = name
    _config_path = FilePath(auth, _config_name)
    let config_file = File(_config_path)
    let size = config_file.size()
    if size > 0 then
      try
        let content: String val = config_file.read_string(size) // should be optimized
        _config_doc.parse(content)
      else
        Debug.out("failed to jsondoc parse:" + name)
      end
    end

  fun ref crawl_codes(): Payload val ? =>
    """从网络获取股票的编码，并返回处理句柄"""
    _handler_factory = CodeRequestHandlerFactory({(path: FilePath) =>
      // 格式化抓取结果
      // let config_file = File(_config_path)
      // if not (config_file.errno() is FileOK) then
      //   error
      // end
      None
    }, _config_path)
    let client = HTTPClient(_auth)
    let request = client(Payload.request("GET", URL.build(_code_url)), _handler_factory as CodeRequestHandlerFactory val)
    request

  fun update_codes_to_file() =>
    """从网络获取股票的编码并写到配置文件中"""
    false

  fun load_codes() =>
    """从文件中读取所有的编码"""
    None

  fun _save_to_file() =>
    None