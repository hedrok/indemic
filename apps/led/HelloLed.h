/**
 * @file HelloLed.h
 * @author Kirill Yatsenko <kirill.yatsenko@hedrok.org>
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
 * HelloLed using multi-platform library IndeMic
 */

#include <avr/interrupt.h>

template<typename Led, template<typename F> class PR >
class HelloLed
{
public:
    class Functor
    {
        public:
            static void call() __attribute((signal))
            {
                static uint8_t value = 1;
                value ^= 1;
                Led::setValue(value);
            }
    };
    typedef PR<Functor> PRF;

    static void main()
    {
        Led::makeOutput();
        Led::setHigh();
        // 1 Hz
        PRF::template setPeriod<1000000000>();
        PRF::enable();
        PRF::clearCounter();
        sei();
        while (true) {
        }
    }
};
