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

namespace IndeMic
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
    typedef PeriodicRunner<typename M::parent, Timer, Functor> P;
    public:
        /**
         * Sets timer period to nanoseconds
         */
        template<uint64_t nanoseconds>
        static inline void setPeriod()
        {
            P::template setPeriod<nanoseconds>();
        }
        /**
         * Clear timer counter (set to zero)
         */
        static inline void clearCounter()
        {
            P::clearCounter();
        }
        /**
         * Enable periodic runner - Functor's funcion
         * will be called each 'nanoseconds' after this call.
         * setPerioud should be called beforehand or result is
         * undefined.
         */
        static inline void enable()
        {
            P::enable();
        }
        /**
         * Stop PeriodicRunner
         */
        static inline void disable()
        {
            P::disable();
        }
};

}
