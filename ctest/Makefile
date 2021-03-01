CFLAGS := -I../record/src -Wall -g

test: test.cpp
	g++ $(CFLAGS) test.cpp -o test

.PHONY: run_test clean

run_test: test
	./test

clean:
	-rm test

