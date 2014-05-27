/**
 * @file RegisterBit.h
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

#include <indemic/generic/RegisterValue.h>

namespace IndeMic
{

/**
 * Represents one microcontroller register bit
 * @param Register - Register containing this bit
 * @param bitIndex
 */
template <typename Register, uint8_t bitIndex>
class RegisterBit
{
    public:
        constexpr RegisterBit(uint8_t v = 1) : _value(v) {};

        constexpr operator RegisterValue<Register>()
        {
            return RegisterValue<Register>(_value << bitIndex);
        }

        template<int otherBitIndex>
        constexpr RegisterValue<Register> operator|(const RegisterBit<Register, otherBitIndex>& other)
        {
            return RegisterValue<Register>((other._value << otherBitIndex) | (_value << bitIndex));
        }

        constexpr RegisterValue<Register> operator|(const RegisterValue<Register>& other)
        {
            return RegisterValue<Register>(other._value | (_value << bitIndex));
        }
    //private:
        //friend template<bitIndex> class RegisterBit<Register, bitIndex>;
        uint8_t _value;
};

}
