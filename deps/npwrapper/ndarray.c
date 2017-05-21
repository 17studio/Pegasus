
#include <Python.h>
#include <numpy/arrayobject.h>

PyObject* _PyArray_SimpleNew(int nd, npy_intp* dims, int typenum)
{
    return PyArray_SimpleNew(nd, dims, typenum);
}