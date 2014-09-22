/**
 * @file PeriodicRunner.h
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

#include <indemic/PeriodicRunner.h>
#include <indemic/generic/RegisterVisitor.h>
#include <libopencm3/cm3/nvic.h>

namespace IndeMic
{

/**
 * PeriodicRunner
 * STM32 implementation
 */
template<typename Timer, typename Functor, uint64_t ns>
class PeriodicRunner<stm32::STM32Mic<ns>, Timer, Functor>
{
    typedef stm32::STM32Mic<ns> M;
    public:
        static void initClock() __attribute__((constructor))
                                __attribute__((used))
        {
            // TODO: make timer generic
            rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM4EN);
        }
        /**
         * Sets timer period to nanoseconds
         */
        template<uint64_t nanoseconds>
        static inline void setPeriod()
        {
            // TODO: make timer generic
            nvic_enable_irq(NVIC_TIM4_IRQ);
            // TODO: calculate period by nanosecods parameter
            RegisterVisitor::set<typename Timer::Ckd::template Value<3> >();
            Timer::Arr::assign(60000);
            Timer::Psc::assign(700 - 1);
        }
        static inline void clearCounter()
        {
            Timer::Cnt::assign(0);
        }
        static inline void enable()
        {
            RegisterVisitor::set<typename Timer::Uie, typename Timer::CEn>();
        }
        static inline void disable()
        {
            RegisterVisitor::clear<typename Timer::Uie, typename Timer::CEn>();
        }
};

}
