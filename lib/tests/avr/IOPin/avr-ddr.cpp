#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <indemic/avr/products.h>
#include <indemic/avr/IOPin.h>

using namespace IndeMic::avr;

typedef AT90USB162Mic M;

class TestPin0 : public IOPin<M, M::PortC, 0> {};
class TestPin1 : public IOPin<M, M::PortC, 1> {};

int main()
{	
    //DDRC |= 1;
    TestPin0::makeOutput();
    //DDRC |= 2;
    TestPin1::makeOutput();
    //DDRC &= ~1;
    TestPin0::makeInput();
    //DDRC &= ~2;
    TestPin1::makeInput();
    return 0;
}
