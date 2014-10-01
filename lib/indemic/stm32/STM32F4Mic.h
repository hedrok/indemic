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
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include <indemic/stm32/STM32Mic.h>
#include <indemic/stm32/Port.h>
#include <indemic/stm32/Timer.h>
#include <indemic/stm32/RCCInitializer.h>

namespace IndeMic
{
namespace stm32
{
    template<typename R>
    class STM32F4Mic : public STM32Mic<R>
    {
        public:
            typedef STM32Mic<R> parent;
            typedef STM32F4Mic M;
            typedef RCCInitializerSTM32F4<STM32F4Mic<R>, 168000000, 84000000, 42000000, 48000000> Clock;

            typedef Port<M, GPIOB> PortB;
            typedef Port<M, GPIOC> PortC;
            typedef Port<M, GPIOD> PortD;

            class RccApb1Enr : public RegisterBase<M, RCC_BASE + 0x40, RccApb1Enr> {};

            class Timer4 : public TimerBase<M, TIM4, 16, InterruptTimer4, Clock::APB1TimerFreq>
            {
                public:
                    class RccEn : public RegisterBit<RccApb1Enr, 2> {};
                    enum { irqNumber = NVIC_TIM4_IRQ };
            };
    };
}
}
