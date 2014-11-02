/**
 * @file RegisterValue.h
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
#pragma once

#include <stdint.h>

namespace IndeMic
{

class RegisterVisitor;

template <typename Register, uint8_t bitIndex, uint8_t width, uint64_t value>
class RegisterBit;

/**
 * Represents one microcontroller register value
 * @param Register - register class for which this value is
 */
template <typename R>
class RegisterValue
{
    public:
        template <uint8_t bitIndex, uint8_t width, uint64_t value>
        constexpr RegisterValue<R> operator|(const RegisterBit<R, bitIndex, width, value>& other)
        {
            return RegisterValue<R>(_value | (other._value << bitIndex));
        }
    private:
        friend R;

        friend class RegisterVisitor;

        template<class Register, uint8_t bitIndex, uint8_t width, uint64_t value> 
        friend class RegisterBit;

        template<typename M, uint32_t address, typename Derived, typename rt, typename rvt>
        friend class RegisterBase;

        constexpr RegisterValue(typename R::value_t v) : _value(v) {};

        typename R::value_t _value;
};

}
