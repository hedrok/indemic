/**
 * @file microcontrollers.h
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
 *
 * @section DESCRIPTION
 * This file contains class tags for STM32 products
 */
#pragma once

#include <libopencm3/stm32/gpio.h>
#include <indemic/stm32/Port.h>

namespace IndeMic
{
namespace stm32
{
    /** Generic STM32 microcontroller */
    template<uint64_t ns = 0>
    class STM32Mic
    {
        public:
            enum {nsPerClock = ns};

            typedef uint8_t register_value_t;
            typedef volatile uint16_t register_double_t;
            typedef uint16_t register_double_value_t;

            /** Represents type of pin index */
            typedef uint8_t pin_ind_t;

            /** Represents type of PORT */
            typedef volatile uint32_t& register_t;

            /** Represents type of port mask */
            typedef uint32_t port_mask_t;

            /** Represents type of logic entry - high or low */
            typedef uint8_t logic_t;
    };

    template<uint64_t ns = 0>
    class STM32F4Mic : public STM32Mic<ns>
    {
        public:
            typedef STM32Mic<ns> parent;
            typedef STM32F4Mic M;

            typedef Port<M, GPIOB> PortB;
            typedef Port<M, GPIOC> PortC;
            typedef Port<M, GPIOD> PortD;

    };
}
}
