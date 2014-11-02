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
 * This file contains class tags for STM32 products
 */
#pragma once

// This macro seems required to me. There are different names
// and different attributes for different architectures, so
// application can not just use some name like call.
#define INDEMIC_INTERRUPT_FUNCTION_NAME indemic_interrupt
#define INDEMIC_INTERRUPT_FUNCTION static void INDEMIC_INTERRUPT_FUNCTION_NAME()

namespace IndeMic
{
namespace stm32
{
    /** Generic STM32 microcontroller */
    template<typename R>
    class STM32Mic
    {
        public:
            typedef R RCCConf;

            typedef uint32_t register_value_t;
            typedef volatile uint64_t register_double_t;
            typedef uint64_t register_double_value_t;

            /** Represents type of pin index */
            typedef uint8_t pin_ind_t;

            /** Represents type of register */
            typedef volatile uint32_t register_t;

            /** Represents type of port mask */
            typedef uint64_t port_mask_t;

            /** Represents type of logic entry - high or low */
            typedef uint8_t logic_t;

            inline static void enableInterrupts() {};
            inline static void sleep() {};
    };
}
}
