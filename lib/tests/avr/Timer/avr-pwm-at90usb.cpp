#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <indemic/avr/AT90USB162Mic.h>

int main()
{	
    // PD0 = OC.0B
    DDRD = 0xff;

    // Division: 8, OCR0A: 125, so period = 8*125 = 1000
    OCR0A = 124;
    // Making pulse 2/5th of millisecond high, 3/5th of millisecond low
    OCR0B = 49;

    // Fast PWM-mode, top = OCR0A, setting period to 1 millisecond
    TCCR0A = (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << WGM02);
    TCNT0 = 0;

    while (true) {
        continue;
    }

    return 0;
}
