/**
 * @file Register.h
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
 * Represents one microcontroller register
 * @param M - microcontroller
 * @param address - address of register
 * @param Derived - class of register itself
 */
template<typename M, uint32_t address, typename Derived>
class RegisterBase
{
    public:
        static inline void set(const RegisterValue<Derived> v)
        {
            Derived::reg() |= v._value;
        }
        static inline void clear(const RegisterValue<Derived> v)
        {
            Derived::reg() &= ~v._value;
        }
        static inline void assign(const RegisterValue<Derived> v)
        {
            Derived::reg() = v._value;
        }
        static inline typename M::register_t& reg()
        {
            return static_cast<typename M::register_t&>(*reinterpret_cast<typename M::register_t*>(address));
        }
        typedef typename M::register_t value_t;
};

}
