lib:
	cp deps/api/ctp/lib/*.a lib/
	cp deps/api/ctp/ctp_future_api/lib/*.* lib/
	cp deps/cmake/talib/lib/* lib/
	cp deps/hdf5/build/bin/libhdf5-* lib/

clean:
	rm lib/*

.PHONY: all test clean lib