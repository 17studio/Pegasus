/*
 完成对stock功能的测试，包括：
 1. 远端获取数据
 2. 读写配置文件
 3. 读写数据文件
 */
use "files"
use "time"
use "net/http"
use "json"
use "debug"
use "ponytest"
use "util/stock"
use "util/hdf5"

actor Main is TestList
  new create(env: Env) =>
    PonyTest(env, this)

  new make() =>
    None

  fun _final() =>
    None

  fun tag tests(test: PonyTest) =>
    test(_TestSinaCrawler)
    test(_TestHdf5file)
    test(_TestConfigFile)

class _TestConfigFile is UnitTest
  fun name(): String => "StockConfigFile"

  fun apply(h: TestHelper) ? =>
    try
      let payload = Payload.request("GET", URL.valid("http://www.shdjt.com/js/lib/astock.js"))
    else
      Debug.out("URL failed")
    end
    let config: StockConfigure = StockConfigure(h.env.root as AmbientAuth, "/var/tmp/tmpconfig")
    let json: JsonDoc = config.load_codes()
    /*
    let request: Payload val = config.crawl_codes_to_file()
    let timers = Timers
    let timer = Timer(_TestConfigFileTimerNotify(h.env, 5, timers), 0, 1_000_000_000) // wait: 0.1 secs, interval: 1 secs
    timers(consume timer)
    Debug.out(request.headers().size())
    */

class _TestSinaCrawler is UnitTest
  fun name(): String => "SinaCrawler"

  fun apply(h: TestHelper) =>
    None

class _TestHdf5file is UnitTest
  fun name(): String => "Hdf5 file"

  fun apply(h: TestHelper) ? =>
    var path: FilePath = FilePath(h.env.root as AmbientAuth, "/var/tmp/tmphdf5")
    let v = "create & open: ".add(path.path).add("!!")
    Debug.out(v)
    var hdf5: Hdf5file = Hdf5file.create(path)
    var hdf5_1: Hdf5file = Hdf5file.open(path)

    var path2: FilePath = FilePath(h.env.root as AmbientAuth, "/var/tmp/tmphdf5_2")
    var hdf5_2: Hdf5file = Hdf5file.create2(path2)

class _TestConfigFileTimerNotify is TimerNotify
  let _env: Env
  let _timout: U32
  let _timers: Timers
  var _counter: U32 = 0

  new iso create(env: Env, timeout: U32, timers: Timers) =>
    _env = env
    _timout = timeout
    _timers = timers

  fun ref apply(timer: Timer, count: U64): Bool =>
    if _counter >= _timout then
      _timers.dispose()
    end
    _counter = _counter + 1
    true