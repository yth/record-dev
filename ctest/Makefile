CFLAGS := -I../record/src -Wall -g

test: test.cpp
	g++ $(CFLAGS) test.cpp -o test

asm: test.cpp
	g++ $(CFLAGS) test.cpp -C -o asm.s

.PHONY: run_test clean

run_test: test
	./test

clean:
	-rm test
	-rm *.txt
	-rm *.s
