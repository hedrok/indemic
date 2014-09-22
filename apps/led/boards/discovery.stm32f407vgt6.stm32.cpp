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

#include <indemic/stm32/STM32F4Mic.h>
#include <indemic/stm32/IOPin.h>
#include <indemic/stm32/PeriodicRunner.h>

#include "../HelloLed.h"

namespace Led
{
    /* 168 MHz  10^9 / (168 * 10^6) */
    typedef IndeMic::stm32::STM32F4Mic<6> M;

    typedef IndeMic::IOPin<M, M::PortD, 12> LedGreen;
    typedef IndeMic::IOPin<M, M::PortD, 13> LedOrange;
    typedef IndeMic::IOPin<M, M::PortD, 14> LedRed;
    typedef IndeMic::IOPin<M, M::PortD, 15> LedBlue;

    template<typename F>
    class MyRunner : public IndeMic::PeriodicRunner<M, M::Timer4, F> {};
};

// TODO: remove this, make change linker script like in AVR
void tim4_isr()
{
    static uint8_t value = 1;
    value ^= 1;
    Led::LedRed::setValue(value);
    TIM_SR(TIM4) &= ~TIM_SR_UIF; 
}

int main()
{
    /* Set STM32 to 168 MHz. */
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
    Led::LedRed::makeOutput();
    HelloLed<Led::M, Led::LedBlue, Led::MyRunner>::main();
}
