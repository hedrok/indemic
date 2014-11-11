/**
 * @file UartSample.h
 * @author Kyrylo Yatsenko <kyrylo.yatsenko@hedrok.org>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * UART sample using multi-platform library IndeMic
 */

/**
 * Simple Uart Sample
 *
 * For each received byte sends byte + 1, uses blocking interface
 *
 * @param M Microcontroller
 * @param Led Led to show that board has power
 * @param Uart1Provider Uart template
 */
template<typename M, typename Led, template<typename> class Uart1Provider>
class UartSample
{
public:
    using Uart1Config = IndeMic::UartConfig<9600, 8, IndeMic::UartParity::off, 1>;
    using Uart1 = Uart1Provider<Uart1Config>;
    static void main()
    {
        Uart1::enable();
        Led::makeOutput();
        Led::setHigh();
        uint8_t b;
        while (true) {
            b = Uart1::readBlocking();
            Uart1::writeBlocking(b + 1);
        }
    }
};
