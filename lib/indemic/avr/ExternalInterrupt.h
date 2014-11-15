/**
 * @file ExternalInterrupt.h
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

#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterVisitor.h>

namespace IndeMic
{
namespace avr
{

/**
 * One ExternalInterrupt class
 * AVR implementation
 *
 * @param Microcontroller
 * @param SourceDescription
 * @param Functor 
 */
template<typename Microcontroller, typename SourceDescription, typename Functor>
class ExternalInterrupt
{
    static_assert((SourceDescription::template Interrupt<Functor>::t == 1), "Something very wrong. This line should just instantiate Interrupt template");
    // Instantiate SourceDescription with Functor parameter to initialize interrupt
    public:
        /**
         * Enable ExternalInterrupt
         */
        static inline void enable()
        {
            //(void)(SourceDescription::template Interrupt<Functor>::interrupt);
            RegisterVisitor::set<typename SourceDescription::INTn>();
        }

        /**
         * Disable ExternalInterrupt
         */
        static inline void disable()
        {
            RegisterVisitor::clear<SourceDescription::INTn>();
        }

        /**
         * Interrupt on rising edge
         */
        static inline void interruptOnRisingEdge()
        {
            RegisterVisitor::assign<
                typename SourceDescription::ISCn1,
                typename SourceDescription::ISCn0
            >();
        }

        /**
         * Interrupt on falling edge
         */
        static inline void interruptOnFallingEdge()
        {
            RegisterVisitor::assign<
                typename SourceDescription::ISCn1,
                typename SourceDescription::ISCn0::template Value<0>
            >();
        }

        /**
         * Interrupt on any edge
         */
        static inline void interruptOnAnyEdge()
        {
            RegisterVisitor::assign<
                typename SourceDescription::ISCn1::template Value<0>,
                typename SourceDescription::ISCn0
            >();
        }

        /**
         * Interrupt on low level
         */
        static inline void interruptOnLowLevel()
        {
            RegisterVisitor::assign<
                typename SourceDescription::ISCn1::template Value<0>,
                typename SourceDescription::ISCn0::template Value<0>
            >();
        }
};

}
}
