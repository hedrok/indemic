MMCU=at90usb162
FLAGS=-mmcu=$(MMCU) -O0

all: out/avrboard-cnc.hex

out/avrboard-cnc.hex: out/avrboard-cnc.elf
	avr-objcopy -j .text -j .data -O ihex out/avrboard-cnc.elf out/avrboard-cnc.hex

out/avrboard-cnc.elf: boards/AVRBoard-cnc.cpp HelloLed.h
	avr-g++ $(FLAGS) -std=gnu++11 -I../../lib -I../../lib/avr boards/AVRBoard-cnc.cpp -o out/avrboard-cnc.elf

flash: out/avrboard-cnc.hex
	avrdude -c avrftdi -pusb162 -U flash:w:out/avrboard-cnc.hex

clean:
	rm -vf out/*.o out/*.elf out/*.hex
