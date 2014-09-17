#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <indemic/avr/products.h>
#include <indemic/avr/IOPin.h>

typedef IndeMic::avr::AT90USB162Mic<1> M;

class TestPin0 : public IndeMic::IOPin<M, M::PortC, 0> {};
class TestPin1 : public IndeMic::IOPin<M, M::PortC, 1> {};

int main()
{	
    DDRC = 0x03;
    TestPin0::setHigh();
    TestPin1::setHigh();
    TestPin0::setLow();
    TestPin1::setLow();
    TestPin1::setValue(1);
    TestPin0::setValue(1);
    TestPin0::setValue(0);
    TestPin1::setValue(0);
    return 0;
}
