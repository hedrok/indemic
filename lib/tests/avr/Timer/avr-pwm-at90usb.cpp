#include <avr/io.h>

#ifndef DONT_USE_INDEMIC
#include <indemic/avr/AT90USB162Mic.h>
#include <indemic/PWMChannelProvider.h>
#include <indemic/PWMTimerProvider.h>
#include <indemic/avr/PWMChannel.h>
#include <indemic/avr/PWMTimer.h>

// clock: 1000000
// 1 000 000 Hz
// 1 / 1 000 000 = 10^-6 = 1 us = 1000 ns

// how many cycles? 125 * 8 = 1000 so that period is 1 ms

typedef IndeMic::avr::AT90USB162Mic<1000> M;

class MyPWMChannelProvider0 : public IndeMic::PWMChannelProvider<M, M::Timer0, M::PD0Pin> {};
class MyPWMProvider : public IndeMic::PWMTimerProvider<M, M::Timer0, MyPWMChannelProvider0> {};

#endif

int main()
{	
#ifndef DONT_USE_INDEMIC
    using LedPWM = MyPWMProvider::PWMTimer<1000000>;
    LedPWM::setPeriod();
    LedPWM::Channel<0>::setPulseWidth<400000>();
    LedPWM::Channel<0>::enable();
    LedPWM::enable();
#else
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
#endif

    while (true) {
        continue;
    }

    return 0;
}
