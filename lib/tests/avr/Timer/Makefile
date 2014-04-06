simavr_includes=$(shell pkg-config --cflags simavr)
simavr_libs=$(shell pkg-config --libs simavr)
all:
	cxxtestgen --error-printer -o runner.cpp PeriodicRunnerTest.h
	g++ -std=c++11 -Wall runner.cpp $(simavr_includes) $(simavr_libs) -o run-tests
	avr-gcc -Os -I../../../ -mmcu=at90usb162 $(simavr_includes) -Wall avr-timer0-at90usb.cpp avr-mcu.c -o avr-timer0-at90usb.elf
	./run-tests

clean:
	rm -f *.elf *.o runner.cpp run-tests
