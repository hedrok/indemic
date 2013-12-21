#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <indemic/avr/products.h>
#include <indemic/avr/IOPin.h>

using namespace IndeMic::avr;

typedef AT90USB162Mic M;

class TestPin : public IOPin<M, M::PortB, 1> {};

int main()
{	
    TestPin::setHigh();
    sleep_mode();
    return 0;
}
