crc_gen:
	g++ crc_gen.cpp -o crc_gen -Wall

test: test.cpp pre_test
	g++ test.cpp -o test

trace: print
	strace ./print 2> trace

.PHONY: clean run_test pre_test

clean:
	-rm test
	-rm crc_gen
	-rm crc_table.h

run_test: test
	./test

pre_test: crc_gen
	./crc_gen

