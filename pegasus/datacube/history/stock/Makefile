
HDF5_LIB_PATH    	?= ../../../../lib
HDF5_SRCS_DIRS		?= .
PEGASUS_SRCS_DIRS	?= ../../../
HDF5_INCLUDE_DIRS  	?= ../../../../deps/hdf5/src
HDF5_INCLUDE_DIRS2 	?= ../../../../deps/hdf5/build
HDF5_INCLUDE_DIRS3 	?= ../../../../deps/hdf5/hl/src

all:
	@clang hdf5.c -I$(HDF5_INCLUDE_DIRS) -I$(HDF5_INCLUDE_DIRS2) -I$(HDF5_INCLUDE_DIRS3) -fPIC -c -o libhdf5.o
	@ar r libhdf5.a libhdf5.o
	@ponyc -d -p ${HDF5_LIB_PATH}:${PEGASUS_SRCS_DIRS}:${HDF5_SRCS_DIRS} $(HDF5_SRCS_DIRS)

test: all
	@$(HDF5_SRCS_DIRS)/stock

clean:
	@rm stock
	@rm libhdf5.*

.PHONY: all test clean
