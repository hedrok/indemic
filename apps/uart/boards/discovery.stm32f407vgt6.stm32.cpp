/**
 * @file discovery.stm32f4.stm32.cpp
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

#include <libopencm3/stm32/rcc.h>

#include <indemic/stm32/RCCConfig.h>
#include <indemic/stm32/STM32F4Mic.h>
#include <indemic/stm32/UartContr.h>

#include "../UartSample.h"

namespace STM32F4Discovery
{
    /* 8 MHz, 3.3V */
    typedef IndeMic::stm32::STM32F4Mic<IndeMic::stm32::RCCConfig<8000000, 3300> > M;

    typedef M::PD12Pin LedGreen;
    typedef M::PD13Pin LedOrange;
    typedef M::PD14Pin LedRed;
    typedef M::PD15Pin LedBlue;

    typedef M::PA2Pin TxPin;
    typedef M::PA3Pin RxPin;

    template<typename Conf>
    using UartProvider = IndeMic::UartContr<M, Conf, M::Usart2, RxPin, TxPin>;
};

int main()
{
    UartSample<
        STM32F4Discovery::M,
        STM32F4Discovery::LedOrange,
        STM32F4Discovery::UartProvider
    >::main();
}
