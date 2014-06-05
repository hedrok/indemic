/**
 * @file RegisterMultiSetter.h
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

/**
 * This class is responsible for setting bits of
 * different registers. All bits must be known at compilation
 * time.
 * Bits are grouped togeather by register and each
 * register is modified by one operation.
 * Registers are modified in order in which they first appear
 * in Bit's
 * @param Functor - Function to call on collected register value
 *                  Must have function processRegister<RegisterValue>()
 *                  RegisterValue will have typedef Register for RegisterBase
 *                  and ::value (uint64_t)
 * @param... Bits - arbitrarary number of register bits that should be set
 *           Each of them should have ::Register typedef that is unique for
 *           each register, and ::value of uint64_t type.
 */
template<typename Functor, typename... Bits>
class RegisterMultiSetter
{
};

namespace
{
    template<typename... Bits>
    class BitsToProcess {};
    template<typename... Bits>
    class BitsForNext {};
    template<typename R, uint64_t v>
    class CurrentRegister
    {
        public:
            typedef R Register;
            enum {value = v};
    };

    template<typename... Args>
    class Worker
    {
    };
    template<typename Functor, typename Register, uint64_t value, typename... Bits2>
    class Worker<Functor, CurrentRegister<Register, value>, BitsToProcess<>, BitsForNext<Bits2...> >
    {
        public:
            static void work()
            {
                Functor::template processRegister<CurrentRegister<Register, value> >();
                IndeMic::RegisterMultiSetter<Functor, Bits2...>::work();
            }
    };
    template<typename Functor, typename Register, uint64_t value, typename FirstBit, typename... Bits1, typename... Bits2>
    class Worker<Functor, CurrentRegister<Register, value>, BitsToProcess<FirstBit, Bits1...>, BitsForNext<Bits2...> >
    {
        public:
            static void work()
            {
                if (std::is_same<Register, typename FirstBit::Register>()) {
                    Worker<Functor, CurrentRegister<Register, value | FirstBit::value>, BitsToProcess<Bits1...>, BitsForNext<Bits2...> >::work();
                } else {
                    Worker<Functor, CurrentRegister<Register, value>, BitsToProcess<Bits1...>, BitsForNext<Bits2..., FirstBit> >::work();
                }
            }
    };
}
template<typename Functor>
class RegisterMultiSetter<Functor>
{
    public:
        static void work()
        {
        }
};
template<typename Functor, typename FirstBit, typename... Others>
class RegisterMultiSetter<Functor, FirstBit, Others...>
{
    public:
        static void work()
        {
            Worker<Functor, CurrentRegister<typename FirstBit::Register, FirstBit::value>, BitsToProcess<Others...>, BitsForNext<> >::work();
        }
};

}
