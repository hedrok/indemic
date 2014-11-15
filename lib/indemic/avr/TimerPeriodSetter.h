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

namespace avr
{

/**
 * TimerPeriodSetter - helper class to set period of timer
 *
 * @param Microcontroller
 * @param Timer
 * @param nanoseconds Period to set in nanoseconds
 */
template<typename M, typename Timer, uint64_t nanoseconds>
class TimerPeriodSetter
{
    private:
        template<typename CsValue, int tst = 0>
        class PeriodSetterHelper;
    public:
        static constexpr uint64_t clocks = nanoseconds / M::nsPerClock;
        static_assert(clocks > 1, "Period should be at least two clock cycles of CPU");
        typedef PeriodSetterHelper<typename Timer::CsValueFirst> Helper;
        static constexpr const uint64_t ocraValue = Helper::ocraValue;
        
        /**
         * Sets timer period to nanoseconds
         */
        static inline void setPeriod()
        {
            Helper::f();
        }
    private:
        static constexpr uint64_t ocrValue(const uint64_t clocks, const uint64_t prescaler)
        {
            return clocks / prescaler - 1;
        }
        template<typename CsValue, int tst>
        class PeriodSetterHelper
        {
            public:
                static_assert(std::is_same<CsValue, void>::value == 0, "Period is too long");
                static constexpr const uint64_t val = ocrValue(clocks, CsValue::prescaler);
                static constexpr const bool t = 0 < val && val < Timer::counterResolution;
                using N = PeriodSetterHelper<CsValue, (t ? 1 : 2)>;
                static constexpr const uint64_t ocraValue = N::ocraValue;
                static void f()
                {
                    N::f();
                }
        };
};

template<typename M, typename Timer, uint64_t periodNs>
template<typename CsValue>
class TimerPeriodSetter<M, Timer, periodNs>::PeriodSetterHelper<CsValue, 1>
{
    public:
        static constexpr const uint64_t ocraValue = TimerPeriodSetter<M, Timer, periodNs>::clocks / CsValue::prescaler - 1;
        static void f()
        {
            RegisterVisitor::assign<typename Timer::Cs::template Value<CsValue::key> >();
            Timer::OcrA::assign(ocraValue);
        }
};

template<typename M, typename Timer, uint64_t periodNs>
template<typename CsValue>
class TimerPeriodSetter<M, Timer, periodNs>::PeriodSetterHelper<CsValue, 2>
{
    public:
        using N = PeriodSetterHelper<typename CsValue::NextValue>;
        static constexpr const uint64_t ocraValue = N::ocraValue;
        static void f()
        {
           N::f();
        }
};

}
}
