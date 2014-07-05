#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <indemic/avr/products.h>
#include <indemic/avr/ExternalInterrupt.h>

/*
ISR(INT0_vect)
{
    PORTC |= 1;
}
*/

class Functor
{
    public:
        static inline void call()
        {
            PORTC |= 1;
        }
};

using namespace IndeMic::avr;

typedef AT90USB162Mic M;

extern "C" void __init();
extern "C" void __vector_default()
{
    while (true) {}
}

__attribute__ ((used)) __attribute__ ((section (".reset_vector"))) const uint16_t reset[2] = {0x940c, ((uint16_t)(&__init))};
__attribute__ ((used)) __attribute__ ((section (".int0_vector"))) const uint16_t int0[2] = {0x940c, ((uint16_t)(&Functor::call))};

class ExtInt : public ExternalInterrupt<M, M::Int0, Functor> {};

int main()
{	
    // Set PC0 as output, clear it:
    DDRC |= 1;
    PORTC &= ~1;

    // Set PD0 as output, clear it:
    DDRD |= 1;
    PORTD &= ~1;

    /*
    // Enable external interrupt on rising edge on INT0 (PD0)
    EICRA |= (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0);
    */
    ExtInt::interruptOnRisingEdge();
    ExtInt::enable();

    sei();
    
    // Test external interrupt

    PORTD |= 1;

    while (true) {
        continue;
    }

    return 0;
}
