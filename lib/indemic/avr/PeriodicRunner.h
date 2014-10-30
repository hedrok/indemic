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
#include <indemic/avr/AVRMic.h>
#include <indemic/avr/TimerPeriodSetter.h>
#include <indemic/generic/std.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterVisitor.h>

namespace IndeMic
{

/**
 * PeriodicRunner AVR implementation
 */
template<typename Timer, typename Functor, uint64_t ns>
class PeriodicRunner<avr::AVRMic<ns>, Timer, Functor>
{
    typedef avr::AVRMic<ns> M;
    static_assert((Timer::template CompAInterrupt<Functor>::t == 1), "Something very wrong. This line should just instantiate Interrupt template");
    public:

        /**
         * Sets timer period to nanoseconds
         */
        template<uint64_t nanoseconds>
        static inline void setPeriod()
        {
            static_assert(nanoseconds > 2 * M::nsPerClock, "Run function each clock cycle is too dangerous");
            avr::TimerPeriodSetter<M, Timer, nanoseconds>::setPeriod();
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

}
