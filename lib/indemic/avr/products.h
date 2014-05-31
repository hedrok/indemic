/**
 * @file microcontrollers.h
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
 *
 * @section DESCRIPTION
 * This file contains class tags for AVR products
 */
#pragma once

#include <indemic/avr/Port.h>

namespace IndeMic
{
namespace avr
{
    /** Generic AVR microcontroller */
    class AVRMic
    {
        public:
            typedef volatile uint8_t register_t;
            typedef uint8_t register_value_t;
    };

    class AT90USB162Mic : public AVRMic
    {
        public:
            typedef Port<AT90USB162Mic, 0x23> PortB;
            typedef Port<AT90USB162Mic, 0x26> PortC;
            typedef Port<AT90USB162Mic, 0x29> PortD;
    };
}
}
