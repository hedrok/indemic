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

#include <indemic/IOPin.h>
#include <indemic/avr/AVRMic.h>
#include <indemic/generic/RegisterBit.h>

namespace IndeMic
{

/**
 * One Input/Output Pin class
 * AVR implementation
 */
template<typename Port, uint8_t pin, uint64_t ns>
class IOPin<avr::AVRMic<ns>, Port, pin>
{
    typedef avr::AVRMic<ns> M;
    typedef typename M::logic_t logic_t;
    public:
        /**
         * Read value from pin
         */
        static inline logic_t get()
        {
            return 0;
        }

        /**
         * Make pin input
         */
        static inline void makeInput()
        {
            Port::DDR::clear(_ddrBit);
        }

        /**
         * Make pin output
         */
        static inline void makeOutput()
        {
            Port::DDR::set(_ddrBit);
        }

        /**
         * Sets pin to logical one.
         *
         * All preferences like setting it output pin etc
         * should be done beforehand
         */
        static inline void setHigh()
        {
            Port::PORT::set(_portBit);
        }

        /**
         * Sets pin value to logical zero.
         *
         * Set to output etc beforehand.
         */
        static inline void setLow()
        {
            Port::PORT::clear(_portBit);
        }

        /**
         * Set value of output pin
         *
         * Set to output etc beforehand.
         */
        static inline void setValue(logic_t value)
        {
            if (value) {
                setHigh();
            } else {
                setLow();
            }
        }
    private:
        constexpr static RegisterBit<typename Port::PORT, pin> _portBit = RegisterBit<typename Port::PORT, pin>();
        constexpr static RegisterBit<typename Port::PIN, pin> _pinBit = RegisterBit<typename Port::PIN, pin>();
        constexpr static RegisterBit<typename Port::DDR, pin> _ddrBit = RegisterBit<typename Port::DDR, pin>();
};

}
