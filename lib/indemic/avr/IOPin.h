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

#include <indemic/IOPin.h>
#include <indemic/avr/AVRMic.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterVisitor.h>

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
            RegisterVisitor::clear<DdrBit>();
        }

        /**
         * Make pin output
         */
        static inline void makeOutput()
        {
            RegisterVisitor::set<DdrBit>();
        }

        /**
         * Sets pin to logical one.
         *
         * All preferences like setting it output pin etc
         * should be done beforehand
         */
        static inline void setHigh()
        {
            RegisterVisitor::set<PortBit>();
        }

        /**
         * Sets pin value to logical zero.
         *
         * Set to output etc beforehand.
         */
        static inline void setLow()
        {
            RegisterVisitor::clear<PortBit>();
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

        using PortBit = RegisterBit<typename Port::PORT, pin>;
        using PinBit = RegisterBit<typename Port::PIN, pin>;
        using DdrBit = RegisterBit<typename Port::DDR, pin>;

        using Value0 = typename PortBit::template Value<0>;
        using Value1 = typename PortBit::template Value<1>;
        using MakeOutputValue = typename DdrBit::template Value<1>;
        using MakeInputValue = typename DdrBit::template Value<0>;
};

}
