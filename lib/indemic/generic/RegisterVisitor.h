/**
 * @file RegisterVisitor.h
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

#include <indemic/generic/RegisterValue.h>
#include <indemic/generic/RegisterMultiSetter.h>

namespace IndeMic
{

/**
 * This class is responsible for assigning/setting/clearing bits of
 * different registers. All bits must be known at compilation
 * time.
 * Bits are grouped togeather by register and each
 * register is modified by one operation. (possibly two in case of
 * assign)
 * Registers are modified in order in which they first appear
 * in Bit's
 */
class RegisterVisitor
{
    public:
        /**
         * Clear bits of registers
         * @param... Bits - arbitrarary number of register bits that should be assigned
         *           Each of them should have ::Register typedef that is unique for
         *           each register, and ::value of uint64_t type.
         */
        template<typename... Bits>
        inline static void assign()
        {
            RegisterMultiSetter<FunctorAssign, Bits...>::work();
        }
        /**
         * Set bits of registers
         * @param... Bits - arbitrarary number of register bits that should be set
         *           Each of them should have ::Register typedef that is unique for
         *           each register, and ::value of uint64_t type.
         */
        template<typename... Bits>
        inline static void set()
        {
            RegisterMultiSetter<FunctorSet, Bits...>::work();
        }
        /**
         * Clear bits of registers
         * @param... Bits - arbitrarary number of register bits that should be cleared
         *           Each of them should have ::Register typedef that is unique for
         *           each register, and ::value of uint64_t type.
         */
        template<typename... Bits>
        inline static void clear()
        {
            RegisterMultiSetter<FunctorClear, Bits...>::work();
        }
    private:
        class FunctorAssign
        {
            public:
                template<typename C>
                static void processRegister()
                {
                    // std::numeric_limits<typename C::Register::value_t>::max()
                    if ((C::valueZeroes | C::value) == static_cast<typename C::Register::value_t>(-1)) {
                        C::Register::assign(RegisterValue<typename C::Register>(C::value));
                    } else {
                        if (C::valueZeroes) {
                            C::Register::clear(RegisterValue<typename C::Register>(C::valueZeroes));
                        }
                        if (C::value) {
                            C::Register::set(RegisterValue<typename C::Register>(C::value));
                        }
                    }
                }
        };
        class FunctorSet
        {
            public:
                template<typename C>
                static void processRegister()
                {
                    C::Register::set(RegisterValue<typename C::Register>(C::value));
                }
        };
        class FunctorClear
        {
            public:
                template<typename C>
                static void processRegister()
                {
                    C::Register::clear(RegisterValue<typename C::Register>(C::value));
                }
        };
};

}
