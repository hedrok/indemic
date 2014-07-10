simavr_includes=$(shell pkg-config --cflags simavr)
simavr_libs=$(shell pkg-config --libs simavr)
all:
	cxxtestgen --error-printer -o runner.cpp ExternalInterruptTest.h
	g++ -std=c++11 -Wall runner.cpp $(simavr_includes) $(simavr_libs) -o run-tests
	avr-gcc -v -std=c++11 -T../avr35.x -Os -I../../../ -mmcu=at90usb162 $(simavr_includes) -Wall avr-external-at90usb.cpp avr-mcu.c -o avr-external-at90usb.elf
	./run-tests

hex:
	avr-objcopy -j .text -j .data -O ihex avr-external-at90usb.elf avr-external-at90usb.hex
clean:
	rm -f *.elf *.o runner.cpp run-tests
