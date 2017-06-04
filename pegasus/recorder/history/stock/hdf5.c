
#include <hdf5.h>

hid_t _H5Fcreate(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id)
{
    printf("%s, %d, %ld, %ld\n", name, flags, fcpl_id, fapl_id);
    printf("%d, %ld, %ld\n", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t f = H5Fcreate(name, flags, fcpl_id, fapl_id);
    printf("%ld\n", f);
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