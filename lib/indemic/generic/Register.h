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
template<typename M, uint32_t address, typename Derived, typename register_value_t = typename M::register_value_t, typename register_t = typename M::register_t>
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
        static inline register_t& reg()
        {
            return static_cast<register_t&>(*reinterpret_cast<register_t*>(address));
        }
        typedef register_value_t value_t;
};

template<typename M, uint32_t address, typename Derived, typename register_value_t = typename M::register_value_t, typename register_t = typename M::register_t>
class RegisterSettable : public RegisterBase<M, address, Derived, register_value_t, register_t>
{
    public:
        static inline void set(const typename RegisterBase<M, address, Derived, register_value_t, register_t>::value_t v)
        {
            Derived::reg() |= v;
        }
        static inline void clear(const typename RegisterBase<M, address, Derived, register_value_t, register_t>::value_t v)
        {
            Derived::reg() &= ~v;
        }
        static inline void assign(const typename RegisterBase<M, address, Derived, register_value_t, register_t>::value_t v)
        {
            Derived::reg() = v;
        }
};

template<typename M, uint32_t address, typename Derived>
class RegisterDoubleBase : public RegisterBase<M, address, Derived, typename M::register_double_value_t, typename M::register_double_t>
{
};

template<typename M, uint32_t address, typename Derived>
class RegisterDoubleSettable : public RegisterSettable<M, address, Derived, typename M::register_double_value_t, typename M::register_double_t>
{
};

}
