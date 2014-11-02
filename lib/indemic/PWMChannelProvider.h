/**
 * @file PWMChannelProvider.h
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

#include <indemic/PWMChannel.h>

namespace IndeMic
{

/**
 * PWMChannelProvider
 * Provides PWMChannel by period
 *
 * @param Microcontroller
 * @param Timer - which timer to use
 * @param Pin - class representing microcontroller pin on which PWM will be enabled
 */
template<typename M, typename Timer, typename Pin>
class PWMChannelProvider
{
    public:
        template<uint64_t periodNs>
        using PWMChannel = PWMChannel<M, Timer, Pin, periodNs>;
};

}
