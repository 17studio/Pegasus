
HDF5_LIB_PATH    	?= ../../../../lib
HDF5_SRCS_DIRS		?= .
PEGASUS_SRCS_DIRS	?= ../../../

all:
	@ponyc -p ${HDF5_LIB_PATH}:${PEGASUS_SRCS_DIRS} $(HDF5_SRCS_DIRS)

test: all
	@$(HDF5_SRCS_DIRS)/stock

clean:
	@rm stock

.PHONY: all test clean