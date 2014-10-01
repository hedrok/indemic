/**
 * @file discovery.stm32f4.stm32.cpp
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

#include <libopencm3/stm32/rcc.h>

#include <indemic/stm32/RCCConfig.h>
#include <indemic/stm32/STM32F4Mic.h>
#include <indemic/stm32/IOPin.h>
#include <indemic/stm32/PeriodicRunner.h>

#include "../HelloLed.h"

namespace Led
{
    /* 8 MHz */
    typedef IndeMic::stm32::STM32F4Mic<IndeMic::stm32::RCCConfig<8000000> > M;

    typedef IndeMic::IOPin<M, M::PortD, 12> LedGreen;
    typedef IndeMic::IOPin<M, M::PortD, 13> LedOrange;
    typedef IndeMic::IOPin<M, M::PortD, 14> LedRed;
    typedef IndeMic::IOPin<M, M::PortD, 15> LedBlue;

    template<typename F>
    class MyRunner : public IndeMic::PeriodicRunner<M, M::Timer4, F> {};
};

int main()
{
    HelloLed<Led::M, Led::LedGreen, Led::MyRunner>::main();
}
