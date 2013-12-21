/**
 * @file IOPin.h
 * @package IndeMic
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
 */
#pragma once

#include <indemic/avr/micro_types.h>

namespace IndeMic
{
namespace avr
{

/**
 * One Input/Output Pin class
 * AVR implementation
 */
template<typename Microcontroller, typename Port, int pin_number>
class IOPin
{
    public:
        /**
         * Read value from pin
         */
        static logic_t get()
        {
            return 0;
        }

        /**
         * Make pin input
         */
        static void makeInput()
        {
        }

        /**
         * Make pin output
         */
        static void makeOutput()
        {
        }

        /**
         * Sets pin to logical one.
         *
         * All preferences like setting it output pin etc
         * should be done beforehand
         */
        static void setHigh()
        {
            Port::portRegister() |= pin_mask;
        }

        /**
         * Sets pin value to logical zero.
         *
         * Set to output etc beforehand.
         */
        static void setLow()
        {
            Port::portRegister() &= ~pin_mask;
        }

        /**
         * Set value of output pin
         *
         * Set to output etc beforehand.
         */
        static void setValue(logic_t value)
        {
            if (value) {
                setHigh();
            } else {
                setLow();
            }
        }
    private:
        enum { pin_mask = 1 << pin_number};
};

}
}
