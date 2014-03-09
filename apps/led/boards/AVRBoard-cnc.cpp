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
#include <indemic/avr/products.h>

#include <avr/io.h>

#include "../HelloLed.h"

namespace Led
{
    typedef IndeMic::avr::AT90USB162Mic M;

    typedef IndeMic::avr::IOPin<M, M::PortC, 2> YellowLed;
};

int main()
{
    HelloLed<Led::YellowLed>::main();
}
