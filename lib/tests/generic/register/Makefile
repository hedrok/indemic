all:
	cxxtestgen --error-printer -o runner.cpp RegisterTest.h RegisterMultiSetterTest.h RegisterVisitorTest.h RegisterDoubleTest.h
	g++ -std=c++11 -I../../..  -Wall runner.cpp -o run-tests
	./run-tests

#TODO: check error (should not compile)
test-fail-wrong-register:
	g++ -std=c++11 -I../../..  -Wall RegisterCompileErrorWrongRegisterTest.cpp 
test-fail-width-check-constructor:
	g++ -std=c++11 -I../../..  -Wall RegisterCompileErrorWidthCheckConstructorTest.cpp 
test-fail-width-check-operator:
	g++ -std=c++11 -I../../..  -Wall RegisterCompileErrorWidthCheckOperatorTest.cpp 

clean:
	rm -f *.elf *.o runner.cpp run-tests
