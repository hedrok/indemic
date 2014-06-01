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
 * @param width - maximum number of bits of value
 */
template <typename Register, uint8_t bitIndex, uint8_t width = 1>
class RegisterBit
{
    static_assert(bitIndex >= 0, "bitIndex cannot be negative");
    static_assert(width > 0, "width must be positive");
    static_assert(bitIndex + width <= (sizeof(typename Register::value_t) * 8), "too wide bitfield");

    public:
        /**
         * Constructor
         */
        constexpr RegisterBit()
            : _value((1 << width) - 1)
        {
        }

        constexpr operator RegisterValue<Register>()
        {
            return RegisterValue<Register>(_value << bitIndex);
        }

        template<uint8_t v = ((1 << width) - 1)>
        constexpr RegisterValue<Register> mask()
        {
            static_assert(v >= 0, "Value of register bit cannot be negative");
            static_assert(v < (1 << width), "Value of register bit doesn't fit");
            return RegisterValue<Register>((v) << bitIndex);
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
    private:
        friend class RegisterValue<Register>;

        uint8_t _value;
};

}
