#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#ifndef DONT_USE_INDEMIC
#include <indemic/avr/AT90USB162Mic.h>
#include <indemic/avr/PeriodicRunner.h>

using namespace IndeMic::avr;

// clock: 1000000
// 1 000 000 Hz
// 1 / 1 000 000 = 10^-6 = 1 us = 1000 ns

// how many cycles? 125 * 8 = 1000 so that period is 1 ms
typedef AT90USB162Mic<1000> M;

template<typename F>
class MyRunner : public PeriodicRunner<M, M::Timer0, F> {};

class Functor
{
    public:
        INDEMIC_INTERRUPT_FUNCTION
        {
            PORTC ^= 1;
        }
};
#else
ISR(TIMER0_OVF_vect)
{
    PORTC ^= 1;
}
#endif

int main()
{	
    DDRC |= 1;
    PORTC &= ~1;

#ifndef DONT_USE_INDEMIC
    MyRunner<Functor>::setPeriod<1000000>();
    MyRunner<Functor>::enable();
    MyRunner<Functor>::clearCounter();
#else
    TCCR0A &= ~(1 << WGM00);
    TCCR0B &= ~(1 << WGM02);
    TCCR0A |= 1 << WGM01;
    TCCR0B |= 1 << CS01;
    TIMSK0 |= 1 << TOIE0;
    OCR0A = 124;
    TCNT0 = 0;
#endif

    sei();

    while (true) {
        continue;
    }

    return 0;
}
