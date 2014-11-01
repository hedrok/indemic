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

template<bool condition, typename TypeIfTrue, typename TypeIfFalse>
class TypeSelectorByCondition;
template<typename TypeIfTrue, typename TypeIfFalse>
class TypeSelectorByCondition<false, TypeIfTrue, TypeIfFalse>
{
    public:
        typedef TypeIfFalse type;
};
template<typename TypeIfTrue, typename TypeIfFalse>
class TypeSelectorByCondition<true, TypeIfTrue, TypeIfFalse>
{
    public:
        typedef TypeIfTrue type;
};

template<typename M, typename Pin>
class OutputCompareUnitSelector;

namespace
{
    template<uint8_t width>
    class GetUintTypeByWidth;
    template<>
    class GetUintTypeByWidth<8>
    {
        public:
            using type = uint8_t;
    };
    template<>
    class GetUintTypeByWidth<16>
    {
        public:
            using type = uint16_t;
    };
    template<>
    class GetUintTypeByWidth<32>
    {
        public:
            using type = uint32_t;
    };
    template<>
    class GetUintTypeByWidth<64>
    {
        public:
            using type = uint64_t;
    };
}

/**
 * STM32 Timer
 */
template<typename M,
         uint64_t base_address,
         uint8_t _bits,
         template<typename> class InterruptTemplate,
         uint64_t inputFrequency,
         uint8_t alternateFunction
         >
class TimerBase
{
    public:
        using Mic = M;

        static constexpr uint64_t baseAddress = base_address;
        static constexpr uint8_t alternateFunctionValue = alternateFunction;

        class Cr1 : public RegisterBase<M, base_address + 0x0, Cr1> {};
        class Dier : public RegisterBase<M, base_address + 0x0c, Dier> {};
        class Sr : public RegisterBase<M, base_address + 0x10, Sr> {};
        class Ccmr1 : public RegisterBase<M, base_address + 0x18, Ccmr1> {};
        class Ccmr2 : public RegisterBase<M, base_address + 0x1c, Ccmr2> {};
        class Ccer : public RegisterBase<M, base_address + 0x20, Ccer> {};
        class Cnt : public RegisterSettable<M, base_address + 0x24, Cnt> {};
        class Psc : public RegisterSettable<M, base_address + 0x28, Psc> {};
        class Arr : public RegisterSettable<M, base_address + 0x2c, Arr> {};

        class Ckd : public RegisterBit<Cr1, 8, 3> {};
        class Urs : public RegisterBit<Cr1, 2> {};
        class CEn : public RegisterBit<Cr1, 0> {};

        class Uif : public RegisterBit<Sr, 0> {};

        class Tie : public RegisterBit<Dier, 6> {};
        class Uie : public RegisterBit<Dier, 0> {};

        template<uint8_t channel>
        using CcmrByChannel = typename TypeSelectorByCondition<(channel - 1) / 2, Ccmr2, Ccmr1>::type;

        constexpr static uint8_t bits = _bits;
        using counter_t = typename GetUintTypeByWidth<bits>::type;

        constexpr static uint64_t counterResolution = 1 << bits;
        constexpr static double nsPerClock = 1e9 / inputFrequency;

        template<typename F>
        using Interrupt = InterruptTemplate<F>;

        template<typename Pin>
        class OutputCompareUnit : public OutputCompareUnitSelector<M, Pin>::type
        {
        };
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

template<typename Timer, uint8_t channel>
class OutputCompareChannel
{
    public:
        using Ccmr = typename Timer::template CcmrByChannel<channel>;
        static constexpr uint8_t offsetCcmr = ((channel - 1) % 2) * 8;
        using Ccer = typename Timer::Ccer;
        static constexpr uint8_t offsetCcer = (channel - 1) * 4;

        class Ccr : public RegisterSettable<typename Timer::Mic, Timer::baseAddress + 0x34 + (channel - 1) * 4, Ccr> {};

        class Cc1s  : public RegisterBit<Ccmr, 0 + offsetCcmr, 2>
        {
            public:
                using Output = typename Cc1s::template Value<0>;
        };
        class Oc1fe : public RegisterBit<Ccmr, 2 + offsetCcmr> {};
        class Oc1pe : public RegisterBit<Ccmr, 3 + offsetCcmr> {};
        class Oc1m  : public RegisterBit<Ccmr, 4 + offsetCcmr, 3>
        {
            public:
                using PWMMode1 = typename Oc1m::template Value<6>;
        };
        class Oc1ce : public RegisterBit<Ccmr, 7 + offsetCcmr> {};

        class Cce : public RegisterBit<typename Timer::Ccer, 0 + offsetCcer> {};
};

}
}
