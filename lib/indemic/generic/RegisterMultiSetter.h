/**
 * @file RegisterMultiSetter.h
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

#include <indemic/generic/std.h>
#include <indemic/generic/RegisterBitBundle.h>

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
 *                  RegisterValue will have typedef Register for RegisterBase,
 *                  ::value (uint64_t) and ::valueZeroes (uint64_t) - all bits
 *                  that have zero in theri value will have 1 in this mask.
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
    template<typename R, uint64_t v, uint64_t vz>
    class CurrentRegister
    {
        public:
            typedef R Register;
            enum {value = v};
            enum {valueZeroes = vz};
    };

    template<typename... Args>
    class Worker
    {
    };
    template<typename Functor, typename Register, uint64_t value, uint64_t valueZeroes, typename... Bits2>
    class Worker<Functor, CurrentRegister<Register, value, valueZeroes>, BitsToProcess<>, BitsForNext<Bits2...> >
    {
        public:
            static void work()
            {
                Functor::template processRegister<CurrentRegister<Register, value, valueZeroes> >();
                IndeMic::RegisterMultiSetter<Functor, Bits2...>::work();
            }
    };
    template<typename Functor, typename Register, uint64_t value, uint64_t valueZeroes, typename... FirstBits, typename... Bits1, typename... Bits2>
    class Worker<Functor, CurrentRegister<Register, value, valueZeroes>, BitsToProcess<RegisterBitBundle<FirstBits...>, Bits1...>, BitsForNext<Bits2...> >
    {
        public:
            static void work()
            {
                Worker<Functor, CurrentRegister<Register, value, valueZeroes>, BitsToProcess<FirstBits..., Bits1...>, BitsForNext<Bits2...> >::work();
            }
    };
    template<typename Functor, typename Register, uint64_t value, uint64_t valueZeroes, typename FirstBit, typename... Bits1, typename... Bits2>
    class Worker<Functor, CurrentRegister<Register, value, valueZeroes>, BitsToProcess<FirstBit, Bits1...>, BitsForNext<Bits2...> >
    {
        public:
            static void work()
            {
                using Next = typename std::tuple_element<
                    std::is_same<Register, typename FirstBit::Register>::value,
                    std::tuple<
                        Worker<
                            Functor,
                            CurrentRegister<Register, value, valueZeroes>,
                            BitsToProcess<Bits1...>,
                            BitsForNext<Bits2..., FirstBit>
                        >,
                        Worker<
                            Functor,
                            CurrentRegister<
                                Register,
                                value | FirstBit::value,
                                valueZeroes | (FirstBit::value ^ FirstBit::mask)
                            >,
                            BitsToProcess<Bits1...>,
                            BitsForNext<Bits2...>
                        >
                    >
                >::type;
                Next::work();
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
template<typename Functor, typename... Bits, typename... Others>
class RegisterMultiSetter<Functor, RegisterBitBundle<Bits...>, Others...>
{
    public:
        static void work()
        {
            RegisterMultiSetter<Functor, Bits..., Others...>::work();
        }
};
template<typename Functor, typename FirstBit, typename... Others>
class RegisterMultiSetter<Functor, FirstBit, Others...>
{
    public:
        static void work()
        {
            Worker<Functor, CurrentRegister<typename FirstBit::Register, FirstBit::value, FirstBit::value ^ FirstBit::mask>, BitsToProcess<Others...>, BitsForNext<> >::work();
        }
};

}
