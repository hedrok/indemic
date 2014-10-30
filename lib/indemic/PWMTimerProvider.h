/**
 * @file PWMTimerProvider.h
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

#include <indemic/PWMTimer.h>

namespace IndeMic
{

/**
 * PWMTimerProvider
 * Provides PWMTimer by period
 *
 * @param Microcontroller
 * @param Timer - which timer to use
 * @param Channels - list of PWMChannel classes
 */
template<typename M, typename Timer, typename... Channels>
class PWMTimerProvider
{
    typedef PWMTimerProvider<typename M::parent, Timer, Channels...> P;
    public:
        template<uint64_t periodNs>
        using PWMTimer = PWMTimer<M, Timer, periodNs, Channels...>;
};

}
