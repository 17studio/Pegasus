# https://github.com/ndarray/Boost.NumPy

# http://stackoverflow.com/questions/24521787/using-cmake-for-a-c-extension-in-python-using-swig-with-dependencies-on-pre-ex
# http://stackoverflow.com/questions/24174394/cmake-is-not-able-to-find-python-libraries
# http://stackoverflow.com/questions/38003707/trouble-with-numpy-c-api

# https://cmake.org/cmake/help/v3.0/module/FindPythonLibs.html
# https://cmake.org/cmake/help/v3.0/module/FindPythonInterp.html

# 1. install python & c/c++ tool chain
# 2. install numpy
# 3. use numpy.get_include() to get the path
# 4. locate python include & lib path
PYTHON_INCLUDE_DIRS ?= /usr/include/python2.7
PYTHON_LIBRARIES    ?= /usr/lib/python2.7/config-x86_64-linux-gnu/libpython2.7.so
NUMPY_INCLUDE_DIRS  ?= /usr/local/lib/python2.7/dist-packages/numpy/core/include

NPWRAPPER_SRCS_DIRS	?= .

all:
	@ponyc $(NPWRAPPER_SRCS_DIRS)

test: all
	$(NPWRAPPER_SRCS_DIRS)/npwrapper

clean:
	@rm npwrapper

.PHONY: all test clean