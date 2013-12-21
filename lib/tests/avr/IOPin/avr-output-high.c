#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "avr/avr_mcu_section.h"
AVR_MCU(16000000, "at90usb162");

int main()
{	
    PORTC |= 1;
    sleep_mode();
    return 0;
}
