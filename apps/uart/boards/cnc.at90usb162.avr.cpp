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
#include <indemic/avr/UartContr.h>
#include <indemic/UartConfig.h>

#include "../UartSample.h"

namespace Cnc
{
    using M = IndeMic::avr::AT90USB162Mic<500>;

    using YellowLed = IndeMic::IOPin<M, M::PortC, 2>;

    template<typename Conf>
    using UartProvider = IndeMic::UartContr<M, Conf, M::Uart1>;
};

int main()
{
    UartSample<Cnc::M, Cnc::YellowLed, Cnc::UartProvider>::main();
}
