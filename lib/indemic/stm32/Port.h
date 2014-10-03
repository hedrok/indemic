/**
 * @file Port.h
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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

namespace IndeMic
{
namespace stm32
{

/**
 * Incapsulates STM32 port
 */
template<typename M, int base_address>
class Port
{
    static_assert(
           GPIOB - GPIOA == GPIOC - GPIOB
        && GPIOB - GPIOA == GPIOD - GPIOC
        && GPIOB - GPIOA == GPIOE - GPIOD
        && GPIOB - GPIOA == GPIOF - GPIOE,
        "Assumption that all GPIOn regions are of same width failed");
    static_assert(
           base_address == GPIOA
        || base_address == GPIOB
        || base_address == GPIOC
        || base_address == GPIOD
        || base_address == GPIOE
        || base_address == GPIOF,
        "base_address must be one of GPIOn");

public:
    enum t { base = base_address };
    static void initClock() __attribute__((constructor))
                            __attribute__((used))
    {
	    //rcc_periph_clock_enable(RCC_GPIOn);
        RCC_AHB1ENR |= (1 << ((base_address - GPIOA) / (GPIOB - GPIOA)));
    }
};

}
}
