/**
 * @file Timer.h
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

#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>

namespace IndeMic
{

namespace stm32
{

/**
 * STM32 Timer
 */
template<typename M, uint64_t base_address, uint8_t _bits, template<typename> class InterruptTemplate >
class TimerBase
{
    public:
        class Cr1 : public RegisterBase<M, base_address + 0x0, Cr1> {};
        class Dier : public RegisterBase<M, base_address + 0x0c, Dier> {};
        class Sr : public RegisterBase<M, base_address + 0x10, Sr> {};
        class Cnt : public RegisterSettable<M, base_address + 0x24, Cnt> {};
        class Psc : public RegisterSettable<M, base_address + 0x28, Psc> {};
        class Arr : public RegisterSettable<M, base_address + 0x2c, Arr> {};

        class Ckd : public RegisterBit<Cr1, 8, 3> {};
        class CEn : public RegisterBit<Cr1, 0> {};

        class Uif : public RegisterBit<Sr, 0> {};

        class Tie : public RegisterBit<Dier, 6> {};
        class Uie : public RegisterBit<Dier, 0> {};

        constexpr static uint8_t bits = _bits;
        constexpr static uint64_t counterResolution = 1 << bits;

        template<typename F>
        using Interrupt = InterruptTemplate<F>;
};

template<typename Functor>
class InterruptTimer4
{
    public:
        enum {t = 1};
        static __attribute__ ((used))
               __attribute__ ((section (".indemic_interrupt_tim4")))
               void (*interrupt)(void);
};
template<typename Functor>
__attribute__ ((used))
__attribute__ ((section (".indemic_interrupt_tim4")))
void (*InterruptTimer4<Functor>::interrupt)(void)
= Functor::INDEMIC_INTERRUPT_FUNCTION_NAME;


}
}
