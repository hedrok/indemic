simavr_includes=$(shell pkg-config --cflags simavr)
simavr_libs=$(shell pkg-config --libs simavr)
all:
	cxxtestgen --error-printer -o runner.cpp IOPinTest.h
	g++ -std=c++11 -Wall runner.cpp $(simavr_includes) $(simavr_libs) -o run-tests
	avr-gcc -std=c++11 -O2 -I../../../ -mmcu=at90usb162 $(simavr_includes) -Wall avr-output.cpp avr-mcu.c -o avr-output.elf
	avr-gcc -std=c++11 -O2 -I../../../ -mmcu=at90usb162 $(simavr_includes) -Wall avr-ddr.cpp avr-mcu.c -o avr-ddr.elf
	avr-objdump -d avr-output.elf > avr-output-tmp.s
	avr-objdump -d avr-ddr.elf > avr-ddr-tmp.s
	./run-tests

clean:
	rm -f *.elf *.o runner.cpp run-tests *-tmp.s
