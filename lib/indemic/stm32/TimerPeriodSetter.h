/**
 * @file TimerPeriodSetter.h
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

#include <indemic/generic/std.h>
#include <indemic/generic/RegisterVisitor.h>

namespace IndeMic
{

namespace stm32
{

/**
 * TimerPeriodSetter - helper class to set period of timer
 *
 * @param Timer
 * @param nanoseconds Period to set in nanoseconds
 */
template<typename Timer, uint64_t nanoseconds>
class TimerPeriodSetter
{
    public:
        static_assert(nanoseconds > 2 * Timer::nsPerClock, "Run function each clock cycle is too dangerous");
        constexpr static uint64_t clocks = nanoseconds / Timer::nsPerClock;
        constexpr static uint8_t clockDivision = clocks / Timer::counterResolution / Timer::counterResolution;
        static_assert(clockDivision < 4, "Period is too long"); 
        constexpr static uint8_t ckd =   (clockDivision < 1) ? 1 
                                : (clockDivision < 2) ? 2
                                                      : 4;
        constexpr static uint8_t ckdValue = ckd - 1;
        constexpr static uint64_t pscValue = clocks / ckd / Timer::counterResolution;
        constexpr static uint64_t arrValue = clocks / ckd / (pscValue + 1);

        static inline void setPeriod()
        {
            RegisterVisitor::assign<typename Timer::Ckd::template Value<ckdValue> >();
            Timer::Psc::assign(pscValue);
            Timer::Arr::assign(arrValue);
        }
};

}
}
