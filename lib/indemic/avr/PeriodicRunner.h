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

#include <indemic/avr/micro_types.h>
#include <indemic/generic/std.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterVisitor.h>


namespace IndeMic
{
namespace avr
{

/**
 * PeriodicRunner
 * Runs function periodically.
 * It can be stopped or restored, period can be set
 * statically.
 *
 * @param Microcontroller
 * @param Timer - which timer to use
 * @param Functor - a class that should have static call
 *                  function that will be called periodically
 */
template<typename M, typename Timer, typename Functor>
class PeriodicRunner
{
    static_assert((Timer::template CompAInterrupt<Functor>::t == 1), "Something very wrong. This line should just instantiate Interrupt template");
    private:
        static constexpr uint64_t ocrValue(const uint64_t clocks, const uint64_t prescaler)
        {
            return clocks / prescaler - 1;
        }
        template<uint64_t clocks, typename CsValue, int tst = 0>
        class PeriodSetterHelper
        {
            public:
                static void f()
                {
                    static_assert(std::is_same<CsValue, void>::value == 0, "Period is too long");
                    constexpr const uint64_t val = ocrValue(clocks, CsValue::prescaler);
                    constexpr const bool t = 0 < val && val < Timer::counterResolution;
                    PeriodSetterHelper<clocks, CsValue, (t ? 1 : 2)>::f();
                }
        };
    public:

        /**
         * Sets timer period to nanoseconds
         */
        template<uint64_t nanoseconds>
        static inline void setPeriod()
        {
            static_assert(nanoseconds > 2 * M::nsPerClock, "Run function each clock cycle is too dangerous");
            constexpr uint64_t clocks = nanoseconds / M::nsPerClock;
            PeriodSetterHelper<clocks, typename Timer::CsValueFirst>::f();
            Timer::setCTCMode();
        }

        static inline void clearCounter()
        {
            Timer::Tcnt::assign(0);
        }
        static inline void enable()
        {
            RegisterVisitor::set<typename Timer::OcieA>();
        }
        static inline void disable()
        {
            RegisterVisitor::clear<typename Timer::OcieA>();
        }
};

template<typename M, typename Timer, typename Functor>
template<uint64_t clocks, typename CsValue>
class PeriodicRunner<M, Timer, Functor>::PeriodSetterHelper<clocks, CsValue, 1>
{
    public:
        static void f()
        {
            RegisterVisitor::set<typename Timer::Cs::template Value<CsValue::key> >();
            constexpr const uint64_t val = clocks / CsValue::prescaler - 1;
            Timer::OcrA::assign(val);
        }
};

template<typename M, typename Timer, typename Functor>
template<uint64_t clocks, typename CsValue>
class PeriodicRunner<M, Timer, Functor>::PeriodSetterHelper<clocks, CsValue, 2>
{
    public:
        static void f()
        {
            PeriodSetterHelper<clocks, typename CsValue::NextValue>::f();
        }
};

}
}
