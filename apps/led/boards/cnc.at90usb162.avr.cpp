/**
 * @file AVRBoard.cpp
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

#include <indemic/avr/IOPin.h>
#include <indemic/avr/AT90USB162Mic.h>
#include <indemic/avr/PeriodicRunner.h>

#include "../HelloLed.h"

namespace Led
{
    // Hm... 62.5 (16 MHz crystal) * 8 (CKDIV8) ns
    typedef IndeMic::avr::AT90USB162Mic<500> M;

    typedef IndeMic::IOPin<M, M::PortC, 2> YellowLed;

    template<typename F>
    class MyRunner : public IndeMic::PeriodicRunner<M, M::Timer1, F> {};
};

int main()
{
    HelloLed<Led::M, Led::YellowLed, Led::MyRunner>::main();
}
