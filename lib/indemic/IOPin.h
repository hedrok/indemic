/**
 * @file IOPin.h
 * @package IndeMic
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
 */
#pragma once

#include <stdint.h>

namespace IndeMic
{

/**
 * One Input/Output Pin class interface
 */
template<typename Microcontroller, typename _Port, uint8_t _pin>
class IOPin
{
    typedef IOPin<typename Microcontroller::parent, _Port, _pin> P;
    typedef typename Microcontroller::logic_t logic_t;
    public:
        using Port = _Port;
        static constexpr const uint8_t pin = _pin;
        /**
         * Read value from pin
         */
        static inline logic_t get()
        {
            return P::get();
        }

        /**
         * Make pin input
         */
        static inline void makeInput()
        {
            return P::makeInput();
        }

        /**
         * Make pin output
         */
        static inline void makeOutput()
        {
            P::makeOutput();
        }

        /**
         * Sets pin to logical one.
         *
         * All preferences like setting it output pin etc
         * should be done beforehand
         */
        static inline void setHigh()
        {
            P::setHigh();
        }

        /**
         * Sets pin value to logical zero.
         *
         * Set to output etc beforehand.
         */
        static inline void setLow()
        {
            P::setLow();
        }

        /**
         * Set value of output pin
         *
         * Set to output etc beforehand.
         */
        static inline void setValue(logic_t value)
        {
            P::setValue(value);
        }
};

}
