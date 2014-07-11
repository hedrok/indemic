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
 * @param R - Register containing this bit
 * @param bitIndex
 * @param width - maximum number of bits of value
 * @param v - static value of register bit(s)
 */
template <typename R, uint8_t bitIndex, uint8_t width = 1, uint64_t tvalue = ((1 << width) - 1)>
class RegisterBit
{
    static_assert(bitIndex >= 0, "bitIndex cannot be negative");
    static_assert(width > 0, "width must be positive");
    static_assert(bitIndex + width <= (sizeof(typename R::value_t) * 8), "too wide bitfield");
    static_assert(tvalue >= 0, "value cannot be negative");
    static_assert(tvalue < (1 << width), "value cannot be larger than (1 << width)");

    public:
        typedef R Register;
        enum {value = (tvalue << bitIndex)};

        template<uint64_t v>
        class Value : public RegisterBit<R, bitIndex, width, v> {};

        /**
         * Constructor
         */
        constexpr RegisterBit()
            : _value((1 << width) - 1)
        {
        }

        constexpr operator RegisterValue<R>()
        {
            return RegisterValue<R>(_value << bitIndex);
        }

        template<uint8_t v = ((1 << width) - 1)>
        constexpr RegisterValue<R> mask()
        {
            static_assert(v >= 0, "Value of register bit cannot be negative");
            static_assert(v < (1 << width), "Value of register bit doesn't fit");
            return RegisterValue<R>((v) << bitIndex);
        }

        template<uint8_t otherBitIndex, uint8_t otherWidth, uint64_t otherValue>
        constexpr RegisterValue<R> operator|(const RegisterBit<R, otherBitIndex, otherWidth, otherValue>& other)
        {
            return RegisterValue<R>((other._value << otherBitIndex) | (_value << bitIndex));
        }

        constexpr RegisterValue<R> operator|(const RegisterValue<R>& other)
        {
            return RegisterValue<R>(other._value | (_value << bitIndex));
        }
    private:
        friend class RegisterValue<R>;

        template<typename _R, uint8_t _v, uint8_t _w, uint64_t _tv>
        friend class RegisterBit;

        uint8_t _value;
};

}
