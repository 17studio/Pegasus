
#include <hdf5.h>

hid_t _H5Fcreate(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id)
{
    // TODO: MUST be true, or call failed. a bug generated by ponyc? only happened will first FFI
    #if 1
    printf("_H5Fcreate: %s, %d, %ld, %ld\n", name, flags, sizeof(fcpl_id), fapl_id);
    printf("_H5Fcreate: %d, %ld, %ld\n", H5F_ACC_TRUNC, sizeof(H5P_DEFAULT), H5P_DEFAULT);
    #endif
    hid_t f = H5Fcreate(name, flags, fcpl_id, fapl_id);
    printf("_H5Fcreate: %ld\n", f);
    return f;
}

hid_t _H5Fopen(const char *name, unsigned flags, hid_t fapl_id)
{
    printf("%s, %d, %ld\n", name, flags, fapl_id);
    printf("%d, %ld\n", H5F_ACC_RDWR , H5P_DEFAULT);
    hid_t f = H5Fopen(name, flags, fapl_id);
    printf("%ld\n", f);
    return f;
}