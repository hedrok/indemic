simavr_includes=$(shell pkg-config --cflags simavr)
simavr_libs=$(shell pkg-config --libs simavr)
all:
	cxxtestgen --error-printer -o runner.cpp IOPinTest.h
	g++ -Wall runner.cpp $(simavr_includes) $(simavr_libs) -o run-tests
	avr-gcc -mmcu=at90usb162 $(simavr_includes) -Wall avr-output-high.cpp avr-mcu.c -o avr-output-high.elf
	./run-tests

clean:
	rm -f *.elf *.o runner.cpp run-tests
