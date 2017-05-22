
#include <Python.h>
#include <numpy/arrayobject.h>

static int _init = 0;

void _Numpy_import_array()
{
    if (!_init)
    {
        printf("import_array()\n");
        import_array();
        _init = 1;
    }
}

PyObject* _PyArray_SimpleNew(int nd, npy_intp* dims, int typenum)
{
    return PyArray_SimpleNew(nd, dims, typenum);
}