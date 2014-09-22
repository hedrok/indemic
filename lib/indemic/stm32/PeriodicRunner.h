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
            // TODO: why period is two times larger than expected?
            static_assert(nanoseconds > 2 * M::nsPerClock, "Run function each clock cycle is too dangerous");
            constexpr uint64_t clocks = nanoseconds / M::nsPerClock;
            constexpr uint8_t clockDivision = clocks / Timer::counterResolution / Timer::counterResolution;
            static_assert(clockDivision < 4, "Period is too long"); 
            constexpr uint8_t ckd =   (clockDivision < 1) ? 1 
                                    : (clockDivision < 2) ? 2
                                                          : 4;
            constexpr uint8_t ckdValue = ckd - 1;
            constexpr uint64_t pscValue = clocks / ckd / Timer::counterResolution;
            constexpr uint64_t arrValue = clocks / ckd / (pscValue + 1);
            RegisterVisitor::clear<typename Timer::Ckd>();
            RegisterVisitor::set<typename Timer::Ckd::template Value<ckdValue> >();
            Timer::Psc::assign(pscValue);
            Timer::Arr::assign(arrValue);
        }
        static inline void clearCounter()
        {
            Timer::Cnt::assign(0);
        }
        static inline void enable()
        {
            // TODO: make timer generic
            nvic_enable_irq(NVIC_TIM4_IRQ);
            RegisterVisitor::set<typename Timer::Uie, typename Timer::CEn>();
        }
        static inline void disable()
        {
            RegisterVisitor::clear<typename Timer::Uie, typename Timer::CEn>();
        }
};

}
