/**
 * @file cnc.at90usb162.avr.cpp
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

#include <indemic/avr/AT90USB162Mic.h>
#include <indemic/avr/PWMChannel.h>
#include <indemic/avr/PWMTimer.h>
#include <indemic/PWMChannelProvider.h>
#include <indemic/PWMTimerProvider.h>

#include "../PWMSample.h"

namespace Cnc
{
    typedef IndeMic::avr::AT90USB162Mic<500> M;

    using PWMChannelProvider1 = IndeMic::PWMChannelProvider<M, M::Timer1, M::PC5Pin>;
    using PWMProvider = IndeMic::PWMTimerProvider<M, M::Timer1, PWMChannelProvider1>;
};

int main()
{
    PWMSample<Cnc::M, Cnc::PWMProvider>::main();
}
