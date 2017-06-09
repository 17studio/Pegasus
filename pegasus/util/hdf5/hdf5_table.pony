
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
use @H5TBmake_table[HerrType](
  table_title: Pointer[U8] tag, // title, optional
  loc_id: HidType, // file id belongs to
  dset_name: Pointer[U8] tag, // table name
  nfields: U64, // the number of fields
  nrecords: U64 tag, // The number of records
  type_size: U32, // size_t, win64 should be U64, The size in bytes of the structure associated with the table. This value is obtained with sizeof.
  const char *field_names [ ], // An array containing the names of the fields.
  const size_t *field_offset, // An array containing the offsets of the fields.
  const hid_t *field_types, // An array containing the type of the fields.
  hsize_t chunk_size, // The chunk size.
  void *fill_data, // Fill values data. Think as default values.
  int compress, // Flag that turns compression on or off.
  const void *data // Buffer with data to be written to the table.
  )

class Hdf5table
  let name: String
  let hid: HidType
  let _self: HidType

  new create(hid': HidType, name': String) =>
    name = name'
    hid = hid'

  fun _final() =>
    if _hid > 0 then
      // do something
      @H5Fflush(_hid, H5FSCopeLocal())
      @H5Fclose(_hid)
    end