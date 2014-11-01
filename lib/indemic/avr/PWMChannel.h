/**
 * @file PWMChannel.h
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

#include <indemic/avr/TimerPeriodSetter.h>
#include <indemic/generic/RegisterVisitor.h>
#include <indemic/PWMChannel.h>

namespace IndeMic
{

/**
 * PWMChannel, AVR Implementation
 * Responsible for one channel of timer
 *
 * @param Timer - which timer to use
 * @param Pin - class representing microcontroller pin on which PWM will be enabled
 * @param periodNs - period of timer in nanoseconds. All operations of this class
 *                   assume that period is set to periodNs, behaviour is undefined in
 *                   another case
 * @param ns - one system tick duration
 */
template<typename Timer, typename Pin, uint64_t periodNs, uint64_t ns>
class PWMChannel<avr::AVRMic<ns>, Timer, Pin, periodNs>
{
    typedef avr::AVRMic<ns> M;
    typedef typename Timer::template OutputCompareUnit<Pin> OutputCompareUnit;
    public:
        /**
         * Sets pulse width on this channel
         */
        template<uint64_t nanoseconds>
        static inline void setPulseWidth()
        {
            static_assert(nanoseconds < periodNs, "Pulse width must be less than period");
            constexpr uint64_t ocraValue = avr::TimerPeriodSetter<M, Timer, periodNs>::ocraValue;
            constexpr typename Timer::counter_t value = (ocraValue + 1) * static_cast<double>(nanoseconds) / periodNs - 1;
            OutputCompareUnit::Ocr::set(value);
        }
        /**
         * Enable PWM on this channel
         */
        static inline void enable()
        {
            disable();
            Pin::makeOutput();
            RegisterVisitor::set<typename OutputCompareUnit::Com1>();
        }
        /**
         * Disable PWM on this channel
         */
        static inline void disable()
        {
            RegisterVisitor::clear<typename OutputCompareUnit::Com0, typename OutputCompareUnit::Com1>();
        }
};

}
