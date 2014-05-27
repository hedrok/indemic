/**
 * @file RegisterValue.h
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

namespace IndeMic
{

template<typename Register, uint8_t bitIndex>
class RegisterBit;

/**
 * Represents one microcontroller register value
 * @param Register - register class for which this value is
 */
template <typename Register>
class RegisterValue
{
    public:
        template <uint8_t bitIndex>
        constexpr RegisterValue<Register> operator|(const RegisterBit<Register, bitIndex>& other)
        {
            return RegisterValue<Register>(_value | (other._value << bitIndex));
        }
    //private:
        //friend class Register;
        //friend template<bitIndex> class RegisterBit<Register, bitIndex>;
        constexpr RegisterValue(typename Register::value_t v) : _value(v) {};
        typename Register::value_t _value;
};

}
