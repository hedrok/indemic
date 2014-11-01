/**
 * @file PWMTimer.h
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

#include <indemic/generic/std.h>
#include <indemic/avr/TimerPeriodSetter.h>
#include <indemic/PWMTimer.h>

namespace IndeMic
{

/**
 * PWMTimer, AVR implementation
 * Responsible for setting timer period and setting PWM
 * mode. Channels should be provided by 'board', they
 * are not checked but it is assumed that they are PWMChannel
 * template instances.
 * It can be stopped or restored
 *
 * @param Timer - which timer to use
 * @param periodNs - period in nanoseconds
 * @param Channels - list of PWMChannel classes
 * @param ns - one system tick duration
 */
template<typename Timer, uint64_t periodNs, uint64_t ns, typename... Channels>
class PWMTimer<avr::AVRMic<ns>, Timer, periodNs, Channels...>
{
    typedef avr::AVRMic<ns> M;
    public:
        /**
         * Sets timer period to periodNs
         */
        static inline void setPeriod()
        {
            avr::TimerPeriodSetter<M, Timer, periodNs>::setPeriod();
        }
        /**
         * Enable PWM - you should configure each needed channel too.
         */
        static inline void enable()
        {
            Timer::setFastPWMTopOcrAMode();
        }
        /**
         * Stop PWM
         */
        static inline void disable()
        {
            Timer::setNormalMode();
        }
        /**
         * Number of channels
         */
        static constexpr uint8_t numberOfChannels = sizeof...(Channels);
        /**
         * Channel by number
         * @param Index of channel starting from 0
         */
        template<uint8_t channel>
        using Channel = typename std::tuple_element<channel, std::tuple<Channels...> >::type::template PWMTimer<periodNs>;
};

}
