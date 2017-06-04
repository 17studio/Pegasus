/*
 完成对stock功能的测试，包括：
 1. 远端获取数据
 2. 读写配置文件
 3. 读写数据文件
 */
use "files"
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

class _TestSinaCrawler is UnitTest
  fun name(): String => "SinaCrawler"

  fun apply(h: TestHelper) =>
    None

class _TestHdf5file is UnitTest
  fun name(): String => "Hdf5 file"

  fun apply(h: TestHelper) ? =>
    var path: FilePath = FilePath(h.env.root as AmbientAuth, "/var/tmp/tmphdf5")
    h.env.out.print(path.path)
    var hdf5: Hdf5file = Hdf5file.create(path)
