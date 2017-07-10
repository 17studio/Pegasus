
#include <hdf5.h>
#include <hdf5_hl.h>

/**
 * realtime
 * demo: http://hq.sinajs.cn/?format=text&list=sz000001
 */
struct stock_snapshot_t {
    char name[16];  // UTF-8
    float open;
    float close;
    float now;
    float high;
    float low;
    float buy;
    float sell;
    float turnover;
    float volumn;
    float bid1_volumn;
    float bid1;
    float bid2_volumn;
    float bid2;
    float bid3_volumn;
    float bid3;
    float bid4_volumn;
    float bid4;
    float bid5_volumn;
    float bid5;
    float ask1_volumn;
    float ask1;
    float ask2_volumn;
    float ask2;
    float ask3_volumn;
    float ask3;
    float ask4_volumn;
    float ask4;
    float ask5_volumn;
    float ask5;
    char date[10];
    char time[11];
};

/**
  date：日期
  open：开盘价
  high：最高价
  close：收盘价
  low：最低价
  volume：成交量
  [amount]: 成交金额 成交量x成交价 考虑到在一天的时间内，成交价可能不同于收盘价，在此时成交金额有意义
  [turnover]:换手率[注：指数无此项]，换手率=某一段时期内的成交量/发行总股数×100%（在中国：成交量/流通股本×100%）

  follow the format of tushare api, see util/stock/stock_history_crawler.pony
 */
struct stock_history_t {
    char date[10];
    char time[11];
    float open;
    float high;
    float close;
    float low;
    float volume;
};

hid_t _h5f_create(const char* name, unsigned flags, hid_t fcpl_id, hid_t fapl_id) {
// TODO: MUST be true, or call failed. a bug generated by ponyc? only happened will first FFI
#if 1
    printf("_h5f_create: %s, %d, %ld, %ld\n", name, flags, sizeof(fcpl_id), fapl_id);
    printf("_h5f_create: %d, %ld, %ld\n", H5F_ACC_TRUNC, sizeof(H5P_DEFAULT), H5P_DEFAULT);
#endif
    hid_t f = H5Fcreate(name, flags, fcpl_id, fapl_id);
    printf("_h5f_create: %ld\n", f);
    return f;
}

hid_t _h5f_open(const char* name, unsigned flags, hid_t fapl_id) {
#if 0
    printf("%s, %d, %ld\n", name, flags, fapl_id);
    printf("%d, %ld\n", H5F_ACC_RDWR, H5P_DEFAULT);
#endif
    hid_t f = H5Fopen(name, flags, fapl_id);
    printf("%ld\n", f);
    return f;
}

/*
HerrType H5TBmake_table(
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
  );
*/

herr_t _h5tb_stock_make_table(const char* table_title, hid_t loc_id, const char* dset_name) {}