
use "lib:python2.7"
use "lib:ndarray"
use "ponytest"

use @Py_Initialize[None]()
use @Py_Finalize[None]()
use @_PyArray_SimpleNew[Pointer[_PyObject]](nd: I32, dims: I32, typenum: I32)

actor Main is TestList
  new create(env: Env) =>
    @Py_Initialize()
    PonyTest(env, this)

  new make() =>
    None

  fun _final() =>
    @Py_Finalize()

  fun tag tests(test: PonyTest) =>
    test(_TestAdd)
    test(_TestSub)
    test(_TestPyObject)
    test(_TestPyArrayObject)

class _TestAdd is UnitTest
  fun name(): String => "addition"

  fun apply(h: TestHelper) =>
    h.assert_eq[U32](4, 1+3)

class _TestSub is UnitTest
  fun name(): String => "subtraction"

  fun apply(h: TestHelper) =>
    h.assert_eq[U32](2, 4-2)

/*
Definition of PyArrayObject

#define _PyObject_HEAD_EXTRA            \
    struct _object *_ob_next;           \
    struct _object *_ob_prev;

typedef struct _object {
    PyObject_HEAD
} PyObject;

typedef struct PyArrayObject {
    PyObject_HEAD
    char *data;
    int nd;
    npy_intp *dimensions;
    npy_intp *strides;
    PyObject *base;
    PyArray_Descr *descr;
    int flags;
    PyObject *weakreflist;
} PyArrayObject;

How to call:
https://docs.scipy.org/doc/numpy/reference/c-api.array.html

C Struct Pointer in Pony:
EGLDisplay eglGetDisplay(NativeDisplayType native_display); //https://www.khronos.org/registry/EGL/sdk/docs/man/html/eglGetDisplay.xhtml
@eglGetDisplay[Pointer[_EGLDisplayHandle]](x_dpy)
 */

class _TestPyObject is UnitTest
  fun name(): String => "pyobject"

  fun apply(h: TestHelper) =>
    var x: String = "print 'Hello Python C/API'"
    @PyRun_SimpleString[None](x.cstring())

/*
create hdf5 datasheet from numpy array
http://docs.h5py.org/en/latest/high/dataset.html#dataset

convert hdf5 datasheet to numpy array
http://stackoverflow.com/questions/10274476/how-to-export-hdf5-file-to-numpy-using-h5py

enum from c in pony, enum size implementation depends, most is sizeof(int):
int ponyint_o_rdonly() { return O_RDONLY; }

primitive _EscapeNone
primitive _EscapeStart
primitive _EscapeSS3
primitive _EscapeCSI
primitive _EscapeMod

type _EscapeState is
  ( _EscapeNone
  | _EscapeStart
  | _EscapeSS3
  | _EscapeCSI
  | _EscapeMod
  )

typedef npy_intp to the appropriate pointer size for different platform
typedef Py_intptr_t npy_intp;

npy_intp *dims, A pointer to the size in each dimension.
PyArray_SimpleNew() whose size in each of nd dimensions is given by the integer array, dims.
Any provided dims and strides are copied into newly allocated dimension and strides arrays for the new array object.
 */
primitive _PyObject
primitive _PyArrayObject
primitive _NpyIntp

primitive _NpyBOOL       fun apply(): U32 => 0
primitive _NpyBYTE       fun apply(): U32 => 1
primitive _NpyUBYTE      fun apply(): U32 => 2
primitive _NpySHORT      fun apply(): U32 => 3
primitive _NpyUSHORT     fun apply(): U32 => 4
primitive _NpyINT        fun apply(): U32 => 5
primitive _NpyUINT       fun apply(): U32 => 6
primitive _NpyLONG       fun apply(): U32 => 7
primitive _NpyULONG      fun apply(): U32 => 8
primitive _NpyLONGLONG   fun apply(): U32 => 9
primitive _NpyULONGLONG  fun apply(): U32 => 10
primitive _NpyFLOAT      fun apply(): U32 => 11
primitive _NpyDOUBLE     fun apply(): U32 => 12
primitive _NpyLONGDOUBLE fun apply(): U32 => 13
primitive _NpyCFLOAT     fun apply(): U32 => 14
primitive _NpyCDOUBLE    fun apply(): U32 => 15
primitive _NpyCLONGDOUBLE fun apply(): U32 => 16
primitive _NpyOBJECT     fun apply(): U32 => 17
primitive _NpySTRING     fun apply(): U32 => 18
primitive _NpyUNICODE    fun apply(): U32 => 19
primitive _NpyVOID       fun apply(): U32 => 20
primitive _NpyDATETIME   fun apply(): U32 => 21
primitive _NpyTIMEDELTA  fun apply(): U32 => 22
primitive _NpyHALF       fun apply(): U32 => 23
primitive _NpyNTYPES     fun apply(): U32 => 24
primitive _NpyNOTYPE     fun apply(): U32 => 25
primitive _NpyCHAR       fun apply(): U32 => 26
primitive _NpyUSERDEF    fun apply(): U32 => 256
primitive _NpyNTypesAbiCompatible fun apply(): U32 => 21

type _NpyTYPES is
  ( _NpyBOOL
  | _NpyBYTE
  | _NpyUBYTE
  | _NpySHORT
  | _NpyUSHORT
  | _NpyINT
  | _NpyUINT
  | _NpyLONG
  | _NpyULONG
  | _NpyLONGLONG
  | _NpyULONGLONG
  | _NpyFLOAT
  | _NpyDOUBLE
  | _NpyLONGDOUBLE
  | _NpyCFLOAT
  | _NpyCDOUBLE
  | _NpyCLONGDOUBLE
  | _NpyOBJECT
  | _NpySTRING
  | _NpyUNICODE
  | _NpyVOID
  | _NpyDATETIME
  | _NpyTIMEDELTA
  | _NpyHALF
  | _NpyNTYPES
  | _NpyNOTYPE
  | _NpyCHAR
  | _NpyUSERDEF
  | _NpyNTypesAbiCompatible
  )

class _TestPyArrayObject is UnitTest
  fun name(): String => "pyarrobject"

  fun apply(h: TestHelper) =>
    @_PyArray_SimpleNew(0, 0, 0)
