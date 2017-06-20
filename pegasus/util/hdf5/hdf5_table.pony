
/*
 * 受限于pony lang的设计，hdf5的table(datasheet)不能在pony定义，
 * 需要借助.c的实现，完成调用的封装
 *
 * 把需要定义的struct，在c中进行定义，然后通过c代码，获得field_offset指针，
 * 以及type_size
 *
 * 获得char**的方法，Array[Pointer[U8]].cpointer()，先生成String，然后
 * 再由Array[String]，转换到char**
 */

class Hdf5table
  let name: String
  let hid: HidType // file belongs to
  let _self: HidType

  new create(hid': HidType, name': String) =>
    name = name'
    hid = hid'
    _self = 0

  fun _final() =>
    if _self > 0 then
      // do something
      @H5Fflush(_self, H5FSCopeLocal())
      @H5Fclose(_self)
    end