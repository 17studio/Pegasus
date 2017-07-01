
use "lib:hdf5-static"
use "lib:hdf5"
use "files"

use @H5Fcreate[HidType](str: Pointer[U8] tag, flags: U32, fcpl_id: I64, fapl_id: I64)
use @_h5f_create[HidType](str: Pointer[U8] tag, flags: U32, fcpl_id: I64, fapl_id: I64)
use @H5Fopen[HidType](str: Pointer[U8] tag, flags: U32, fapl_id: I64)
use @_h5f_open[HidType](str: Pointer[U8] tag, flags: U32, fapl_id: I64)
use @H5Fflush[HerrType](object_id: HidType, scope: I32)
use @H5Fclose[HerrType](file_id: HidType)

class Hdf5file
  /*
   * 在实际使用中发现，如果首次调用@H5Fcreate会失败，但是当先把传过去的参数实际使用一次后，再调用@H5Fcreate则成功，
   * 不知道该BUG是不是由于编译器引发的，待测试。
   *
   * 2017.6.30 在LLVM 3.9.1下测试，依然存在该问题
   */
  let path: FilePath
  var _hid: HidType = 0

  new create(from: FilePath, flags: H5FFileFlag = H5FAccTRUNC) =>
    path = from
    _hid = @_h5f_create(from.path.cstring(), flags(), H5PDefault(), H5PDefault())

  new create2(from: FilePath, flags: H5FFileFlag = H5FAccTRUNC) =>
    path = from
    _hid = @H5Fcreate(from.path.cstring(), flags(), H5PDefault(), H5PDefault())

  new open(from: FilePath, flags: H5FFileFlag = H5FAccRDWR) =>
    path = from
    _hid = @H5Fopen(from.path.cstring(), flags(), H5PDefault())

  fun _final() =>
    if _hid > 0 then
      // do something
      @H5Fflush(_hid, H5FSCopeLocal())
      @H5Fclose(_hid)
    end