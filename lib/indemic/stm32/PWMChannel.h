/**
 * @file PWMChannel.h
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

#include <indemic/stm32/TimerPeriodSetter.h>
#include <indemic/generic/RegisterVisitor.h>
#include <indemic/PWMChannel.h>

namespace IndeMic
{

/**
 * PWMChannel, STM32 Implementation
 * Responsible for one channel of timer
 *
 * @param Timer - which timer to use
 * @param Pin - class representing microcontroller pin on which PWM will be enabled
 * @param periodNs - period of timer in nanoseconds. All operations of this class
 *                   assume that period is set to periodNs, behaviour is undefined in
 *                   another case
 * @param Clock - CPU clock parameters
 */
template<typename Timer, typename Pin, uint64_t periodNs, typename Clock>
class PWMChannel<stm32::STM32Mic<Clock>, Timer, Pin, periodNs>
{
    typedef typename Timer::template OutputCompareUnit<Pin> OutputCompareUnit;
    public:
        /**
         * Sets pulse width on this channel
         */
        template<uint64_t nanoseconds>
        static inline void setPulseWidth()
        {
            static_assert(nanoseconds < periodNs, "Pulse width must be less than period");
            constexpr uint64_t arrValue = stm32::TimerPeriodSetter<Timer, periodNs>::arrValue;
            constexpr typename Timer::counter_t value = (arrValue + 1) * static_cast<double>(nanoseconds) / periodNs - 1;
            OutputCompareUnit::Ccr::set(value);
        }
        /**
         * Enable PWM on this channel
         */
        static inline void enable()
        {
            disable();
            Pin::makeOutput();
            Pin::setHigh();
            Pin::Port::template setAlternateFunction<Pin>(Timer::alternateFunctionValue);
            Pin::Port::template setMode<Pin>(GPIO_MODE_AF);

            RegisterVisitor::clear<
                typename OutputCompareUnit::Cc1s,
                typename OutputCompareUnit::Oc1m
            >();
            RegisterVisitor::set<
                typename OutputCompareUnit::Cc1s::Output,
                typename OutputCompareUnit::Oc1m::PWMMode1,
                typename OutputCompareUnit::Cce
            >();
        }
        /**
         * Disable PWM on this channel
         */
        static inline void disable()
        {
            RegisterVisitor::clear<typename OutputCompareUnit::Cce>();
        }
};

}
