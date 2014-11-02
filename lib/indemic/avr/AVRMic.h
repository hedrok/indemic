/**
 * @file microcontrollers.h
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
 *
 * @section DESCRIPTION
 * This file contains class tags for AVR products
 */
#pragma once

#include <avr/interrupt.h>

namespace IndeMic
{
namespace avr
{
    /** Generic AVR microcontroller */
    template<uint64_t ns = 0>
    class AVRMic
    {
        public:
            typedef volatile uint8_t register_t;
            typedef uint8_t register_value_t;
            typedef volatile uint16_t register_double_t;
            typedef uint16_t register_double_value_t;

            /** Represents type of port mask */
            typedef uint8_t port_mask_t;

            /** Represents type of pin index */
            typedef uint8_t pin_ind_t;

            /** Represents type of logic entry - high or low */
            typedef uint8_t logic_t;

            enum {nsPerClock = ns};

            inline static void sleep() {}
            inline static void enableInterrupts()
            {
                sei();
            }
    };
}
}

// This macro seems required to me. There are different names
// and different attributes for different architectures, so
// application can not just use some name like call.
#define INDEMIC_INTERRUPT_FUNCTION_NAME __vector_1
#define INDEMIC_INTERRUPT_FUNCTION static void __vector_1() __attribute__((signal))

extern "C" void __init();
extern "C"
__attribute__ ((section (".error_interrupt_handler")))
void __vector_default()
{
    while (true) {}
}
__attribute__ ((used)) __attribute__ ((section (".reset_vector"))) const uint16_t reset = (uint16_t)(&__init);
