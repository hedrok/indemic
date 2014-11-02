/**
 * @file PeriodicRunner.h
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
#pragma once

#include <libopencm3/cm3/nvic.h>

#include <indemic/PeriodicRunner.h>
#include <indemic/generic/RegisterVisitor.h>
#include <indemic/stm32/TimerPeriodSetter.h>

namespace IndeMic
{

/**
 * PeriodicRunner
 * STM32 implementation
 */
template<typename Timer, typename Functor, typename Clock>
class PeriodicRunner<stm32::STM32Mic<Clock>, Timer, Functor>
{
    public:
        static void initClock() __attribute__((constructor))
                                __attribute__((used))
        {
            RegisterVisitor::set<typename Timer::RccEn>();
            nvic_enable_irq(Timer::irqNumber);
        }
        /**
         * Sets timer period to nanoseconds
         */
        template<uint64_t nanoseconds>
        static inline void setPeriod()
        {
            stm32::TimerPeriodSetter<Timer, nanoseconds>::setPeriod();
        }
        static inline void clearCounter()
        {
            Timer::Cnt::assign(0);
        }
        static inline void enable()
        {
            // Preventing interrupt before time
            RegisterVisitor::clear<typename Timer::Uie>();
            RegisterVisitor::set<typename Timer::CEn>();
            // Making sure update event comes quick
            Timer::Cnt::assign(Timer::counterResolution - 1);
            while (!Timer::Uif::getValue()) {
                continue;
            }
            RegisterVisitor::clear<typename Timer::Uif>();
            // Now Prescaler is loaded to shadow register, enable interrupt
            RegisterVisitor::set<typename Timer::Uie>();
        }
        static inline void disable()
        {
            RegisterVisitor::clear<typename Timer::Uie, typename Timer::CEn>();
        }
        /**
         * Wrapper for functors - need to clear Uif bit manually each time
         * interrupt is called
         */
        class WrapperFunctor
        {
            public:
                INDEMIC_INTERRUPT_FUNCTION __attribute__((interrupt)) __attribute__((used))
                {
                    RegisterVisitor::clear<typename Timer::Uif>();
                    Functor::INDEMIC_INTERRUPT_FUNCTION_NAME();
                }
        };
        static_assert(Timer::template Interrupt<WrapperFunctor>::t == 1, "instantiate Interrupt::interrupt");
};

}
