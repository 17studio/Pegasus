
use "collections"
use "json"
use "files"
use "net/http"
use "debug"
use "regex"

class CodeRequestHandler is HTTPHandler
  /*
   * 因为对网络的操作是在另外的ACTOR中执行，主调ACTOR无法干预执行的步骤，
   * 所以，主调ACTOR应该采取等待完成，并把执行方法，交由网络调度ACTOR执行（回调模式）
   */
  let _file_path: FilePath val // 保存该参数，方便完成文件操作
  let _session: HTTPSession
  var _body_buffer: String ref = String // 保存读取的信息

  new create(path: FilePath, session: HTTPSession) =>
    _file_path = path
    _session = session

  fun ref chunk(data': ByteSeq val) =>
    _body_buffer.append(data') // 复制，性能可以优化吗？

  fun finished() =>
    _session.dispose()
    // 格式化抓取结果，并写到文件中
    let config_file = File(_file_path)
    if not (config_file.errno() is FileOK) then
      return
    end
    config_file.write(_body_buffer)


class CodeRequestHandlerFactory is HandlerFactory
  let _file_path: FilePath val

  new val create(path: FilePath) =>
    _file_path = path

  fun apply(session: HTTPSession): HTTPHandler =>
    var h = CodeRequestHandler(_file_path, session)
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

  new create(auth: AmbientAuth, name: String) ? =>
    _auth = auth
    _config_name = name
    _config_path = FilePath(auth, _config_name)

  fun ref crawl_codes_to_file(name: String): Payload val ? =>
    """从网络获取股票的编码，写到配置文件中，并返回处理句柄"""
    var config_path = FilePath(_auth, name)
    var handler_factory: (CodeRequestHandlerFactory val | None) = None // 用于保存处理远程请求的句柄工厂，方便查询运行情况
    handler_factory = CodeRequestHandlerFactory(config_path)
    let client = HTTPClient(_auth)
    let request = client(Payload.request("GET", URL.build(_code_url)), handler_factory as CodeRequestHandlerFactory val)
    request

  fun load_codes(): JsonDoc ? =>
    """从文件中读取所有的编码"""
    let config_file = File(_config_path)
    if not (config_file.errno() is FileOK) then
      error
    end
    let size = config_file.size()
    if size > 0 then
      var json_array: JsonArray = JsonArray
      try
        let content: String val = config_file.read_string(size) // 文件以UTF8方式编码
        let r = Regex("~(\\d+)`") // Regex默认支持UTF8编码 http://pcre.org/current/doc/html/pcre2unicode.html

        var pos: USize = 0
        while pos < size do
          let matched = r(content, pos)
          pos = matched.end_pos()
          json_array.data.push(matched(1))
        end
      else
        var json_doc = JsonDoc
        //https://tutorial.ponylang.org/expressions/variables.html => Otherwise, the field is public and can be read or written from anywhere
        //json_doc.data = json_array // can not write to public field directly if which is a class, forbidden by compiler
        json_doc.parse(json_array.string())
        return json_doc
      end
    end

    JsonDoc


