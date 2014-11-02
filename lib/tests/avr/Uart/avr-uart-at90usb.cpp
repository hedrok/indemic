#include <avr/io.h>
#include <indemic/avr/AT90USB162Mic.h>

#define DONT_USE_INDEMIC

#ifndef DONT_USE_INDEMIC
    // TODO
#endif

int main()
{	
#ifndef DONT_USE_INDEMIC
    // TODO
#else
    // Use quickest baudrate possible
    UBRR1 = 0;
    // Enable transmitter and receiver
    UCSR1B = (1 << TXEN1) | (1 << RXEN1);
    UCSR1C = (1 << USBS1) | (3 << UCSZ10);

    uint8_t b;
    while (true) {
        while (!(UCSR1A & (1 << RXC1))) {
            continue;
        }
        b = UDR1;
        while (!(UCSR1A & (1 << UDRE1))) {
            continue;
        }
        // Send byte
        UDR1 = b + 1;
    }
#endif

    return 0;
}
