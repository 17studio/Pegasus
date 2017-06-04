lib:
	cp deps/gateway/ctp/lib/*.a lib/
	cp deps/gatewap/ctp/ctp_future_api/lib/*.* lib/
	cp deps/cmake/talib/lib/* lib/
	cp deps/hdf5/build/bin/libhdf5-* lib/

clean:
	rm lib/*

.PHONY: all test clean lib