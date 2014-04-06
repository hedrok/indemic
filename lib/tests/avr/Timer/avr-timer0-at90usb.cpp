#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

ISR(TIMER0_OVF_vect)
{
    PORTC ^= 1;
}

int main()
{	
    DDRC |= 1;
    PORTC &= ~1;

    TCCR0A = 1 << WGM01;
    TCCR0B = 1 << CS01;
    TIMSK0 |= 1 << TOIE0;
    OCR0A = 124;
    TCNT0 = 0;
    sei();

    while (true) {
        continue;
    }

    return 0;
}
