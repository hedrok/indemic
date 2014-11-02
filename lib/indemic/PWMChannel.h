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

namespace IndeMic
{

/**
 * PWMChannel
 * Responsible for one channel of timer
 *
 * @param Microcontroller
 * @param Timer - which timer to use
 * @param Pin - class representing microcontroller pin on which PWM will be enabled
 * @param periodNs - period of timer in nanoseconds. All operations of this class
 *                   assume that period is set to periodNs, behaviour is undefined in
 *                   another case
 */
template<typename M, typename Timer, typename Pin, uint64_t periodNs>
class PWMChannel
{
    typedef PWMChannel<typename M::parent, Timer, Pin, periodNs> P;
    public:
        /**
         * Sets pulse width on this channel
         */
        template<uint64_t nanoseconds>
        static inline void setPulseWidth()
        {
            P::template setPulseWidth<nanoseconds>();
        }
        /**
         * Enable PWM on this channel
         */
        static inline void enable()
        {
            P::enable();
        }
        /**
         * Disable PWM on this channel
         */
        static inline void disable()
        {
            P::disable();
        }
};

}
