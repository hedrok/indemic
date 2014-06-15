#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

ISR(INT0_vect)
{
    PORTC |= 1;
}

int main()
{	
    // Set PC0 as output, clear it:
    DDRC |= 1;
    PORTC &= ~1;

    // Set PD0 as output, clear it:
    DDRD |= 1;
    PORTD &= ~1;

    // Enable external interrupt on rising edge on INT0 (PD0)
    EICRA |= (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0);

    sei();
    
    // Test external interrupt

    PORTD |= 1;

    while (true) {
        continue;
    }

    return 0;
}
