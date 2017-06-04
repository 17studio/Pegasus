
use "lib:hdf5-static"
use "files"

use @H5Fcreate[HidType](str: Pointer[U8] tag, flags: U32, fcpl_id: I64, fapl_id: I64)
use @H5Fopen[HidType](str: Pointer[U8] tag, flags: U32, fapl_id: I64)
use @H5Fflush[HerrType](object_id: HidType, scope: I32)
use @H5Fclose[HerrType](file_id: HidType)

type HidType is I64
type HerrType is I32
type H5FScopeType is (H5FSCopeLocal | H5FScopeGlobal)
type FileFlag is (H5FAccRDOnly | H5FAccRDWR | H5FAccTRUNC | H5FAccEXCL)

primitive H5PDefault    fun apply(): I64 => 0

primitive H5FAccRDOnly  fun apply(): U32 => 0
primitive H5FAccRDWR    fun apply(): U32 => 1
primitive H5FAccTRUNC   fun apply(): U32 => 2
primitive H5FAccEXCL    fun apply(): U32 => 4

primitive H5FSCopeLocal     fun apply(): I32 => 0
primitive H5FScopeGlobal    fun apply(): I32 => 1

class Hdf5file
  let path: FilePath
  var _hid: HidType

  new create(from: FilePath, flags: FileFlag = H5FAccTRUNC) =>
    path = from
    _hid = @H5Fcreate(from.path.cstring(), flags(), H5PDefault(), H5PDefault())

  new open(from: FilePath, flags: FileFlag = H5FAccRDWR) =>
    path = from
    _hid = @H5Fopen(from.path.cstring(), flags(), H5PDefault())

  fun _final() =>
    if _hid > 0 then
      // do something
      @H5Fflush(_hid, H5FSCopeLocal())
      @H5Fclose(_hid)
    end