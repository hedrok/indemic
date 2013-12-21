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

#include <avr/IOPin.h>
#include <avr/io.h>
#include <microcontrollers.h>
#include "../HelloLed.h"

namespace Led
{

    class RedLed : public IndeMic::avr::IOPin<AT90USB162Mic, void, void> {};
    class YellowLed : public IndeMic::avr::IOPin<AT90USB162Mic, void, void> {};
};

int main()
{
    HelloLed<Led::RedLed, Led::YellowLed>::main();
}
