/**
 * @file micro_types.h
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
 * This file contains basic types typedefs for AVR architecture
 */
#pragma once

#include <inttypes.h>

namespace IndeMic
{
namespace avr
{
    /** Represents type of pin index */
    typedef uint8_t pin_ind_t;

    /** Represents type of PORT */
    typedef volatile uint8_t& register_t;

    /** Represents type of port mask */
    typedef uint8_t port_mask_t;

    /** Represents type of logic entry - high or low */
    typedef uint8_t logic_t;

}
}
