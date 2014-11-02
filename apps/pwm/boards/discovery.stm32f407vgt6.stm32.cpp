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
#include <indemic/stm32/PeriodicRunner.h>
#include <indemic/stm32/PWMChannel.h>
#include <indemic/stm32/PWMTimer.h>
#include <indemic/PWMChannelProvider.h>
#include <indemic/PWMTimerProvider.h>

#include "../PWMSample.h"

namespace STM32F4Discovery
{
    /* 8 MHz, 3.3V */
    typedef IndeMic::stm32::STM32F4Mic<IndeMic::stm32::RCCConfig<8000000, 3300> > M;

    typedef M::PD12Pin LedGreen;
    typedef M::PD13Pin LedOrange;
    typedef M::PD14Pin LedRed;
    typedef M::PD15Pin LedBlue;

    using PWMChannelProvider1 = IndeMic::PWMChannelProvider<M, M::Timer4, LedBlue>;
    using PWMProvider = IndeMic::PWMTimerProvider<M, M::Timer4, PWMChannelProvider1>;
};

int main()
{
    PWMSample<STM32F4Discovery::M, STM32F4Discovery::PWMProvider>::main();
}
