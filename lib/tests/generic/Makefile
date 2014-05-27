all:
	cxxtestgen --error-printer -o runner.cpp RegisterTest.h
	g++ -std=c++11 -I../..  -Wall runner.cpp -o run-tests
	./run-tests

#TODO: check error (should not compile)
test-fail:
	g++ -std=c++11 -I../..  -Wall RegisterCompileErrorTest.cpp 

clean:
	rm -f *.elf *.o runner.cpp run-tests
