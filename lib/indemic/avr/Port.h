/**
 * @file Port.h
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

#include <indemic/generic/Register.h>

namespace IndeMic
{
namespace avr
{

/**
 * Incapsulates AVR port
 */
template<typename M, int base_address>
class Port
{
public:
    class PIN : public RegisterBase<M, base_address + 0x0, PIN> {};
    class DDR : public RegisterBase<M, base_address + 0x1, DDR> {};
    class PORT : public RegisterBase<M, base_address + 0x2, PORT> {};
};

}
}
