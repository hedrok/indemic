#include <avr/io.h>
#include <indemic/avr/AT90USB162Mic.h>

void simAvrWorkaround()
{
    // Workaround to make simavr send XON
    // Probably (!(UCSR1A & (1 << RXC1))) is expected
    (void)(UCSR1A);
}

#ifndef DONT_USE_INDEMIC
#include <indemic/UartConfig.h>
#include <indemic/avr/UartContr.h>

using M = IndeMic::avr::AT90USB162Mic<1000>;
using MyUartConfig = IndeMic::UartConfig<38400, 8, IndeMic::UartParity::off, 1>;
using MyUart = IndeMic::UartContr<M, MyUartConfig, M::Uart1>;

struct WriteFunctor
{
    INDEMIC_INTERRUPT_FUNCTION
    {
        if (_left == 0) {
            return;
        }
        MyUart::writeNonBlocking(_queue[_iqueue]++);
        _left--;
        if (_left == 0) {
            _iqueue++;
            if (_iqueue < _jqueue) {
                _left = 3;
            } else {
                MyUart::disableWriteInt();
            }
        }
    }
    static void sendData(uint8_t data)
    {
        if (_jqueue == bufferSize) {
            // we could use cyclic queue but
            // this is just a test
            return;
        }
        _queue[_jqueue++] = data;
        if (!_left) {
            _left = 3;
            MyUart::enableWriteInt();
        }
    }
    static uint8_t _left;
    static constexpr uint8_t bufferSize = 3;
    static uint8_t _queue[bufferSize];
    static uint8_t _iqueue;
    static uint8_t _jqueue;
};
uint8_t WriteFunctor::_queue[bufferSize];
uint8_t WriteFunctor::_iqueue;
uint8_t WriteFunctor::_jqueue;
uint8_t WriteFunctor::_left;

struct ReadFunctor
{
    INDEMIC_INTERRUPT_FUNCTION
    {
        uint8_t byte = MyUart::readNonBlocking();
        WriteFunctor::sendData(byte);
        simAvrWorkaround();
    }
};
#endif

int main()
{	
#ifndef DONT_USE_INDEMIC
    MyUart::Interrupts<ReadFunctor, WriteFunctor>::init();
    MyUart::enableReadInt();
    MyUart::enable();
    M::enableInterrupts();
    simAvrWorkaround();
    while (true) {
        M::sleep();
    }
#else
    //TODO
#endif

    return 0;
}
