#include <avr/io.h>
#include <indemic/avr/AT90USB162Mic.h>

#ifndef DONT_USE_INDEMIC
#include <indemic/UartConfig.h>
#include <indemic/avr/UartContr.h>

using M = IndeMic::avr::AT90USB162Mic<1000>;
using MyUartConfig = IndeMic::UartConfig<38400, 8, IndeMic::UartParity::off, 1>;
using MyUart = IndeMic::UartContr<M, MyUartConfig, M::Uart1>;
#endif

int main()
{	
#ifndef DONT_USE_INDEMIC
    MyUart::enable();
    uint8_t b;
    while (true) {
        b = MyUart::readBlocking();
        MyUart::writeBlocking(b + 1);
    }
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
