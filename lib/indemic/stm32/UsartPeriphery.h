/**
 * @file UsartPeriphery.h
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

#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>

namespace IndeMic
{

namespace stm32
{

/**
 * STM32 Usart Periphery
 *
 * @param M Microcontroller
 * @param base_address Usart base address e.g. USART2_BASE
 * @param InterruptTemplate One of InterruptU[s]art[1-6]
 * @param inputFrequency Input frequency of Usart. Usually
 *                       APB1Freq or APB2Freq
 * @param alternateFunction To which GPIO_AF set Rx and Tx pin
 */
template<
    typename M,
    uint64_t base_address,
    template<typename> class InterruptTemplate,
    uint64_t inputFrequency,
    uint8_t alternateFunction
> class UsartPeriphery
{
    public:
        using Mic = M;
        constexpr static double nsPerClock = 1e9 / inputFrequency;

        static constexpr uint64_t baseAddress = base_address;
        static constexpr uint8_t alternateFunctionValue = alternateFunction;

        class SrReg : public RegisterBase<M, base_address + 0x0, SrReg> {};
        class DrRegS : public RegisterSettable<M, base_address + 0x4, DrRegS> {};
        class BrrReg : public RegisterBase<M, base_address + 0x8, BrrReg> {};
        class Cr1Reg : public RegisterBase<M, base_address + 0xc, Cr1Reg> {};
        class Cr2Reg : public RegisterBase<M, base_address + 0x10, Cr2Reg> {};

        using PeBit = RegisterBit<SrReg, 0>;
        using FeBit = RegisterBit<SrReg, 1>;
        using NfBit = RegisterBit<SrReg, 2>;
        using OreBit = RegisterBit<SrReg, 3>;
        using IdleBit = RegisterBit<SrReg, 4>;
        using RxneBit = RegisterBit<SrReg, 5>;
        using TcBit = RegisterBit<SrReg, 6>;
        using TxeBit = RegisterBit<SrReg, 7>;
        using LbdBit = RegisterBit<SrReg, 8>;
        using CtsBit = RegisterBit<SrReg, 9>;

        using DivMantissaBits = RegisterBit<BrrReg, 4, 12>;
        using DivFractionBits = RegisterBit<BrrReg, 0, 4>;

        using SbkBit = RegisterBit<Cr1Reg, 0>;
        using RwuBit = RegisterBit<Cr1Reg, 1>;
        using ReBit = RegisterBit<Cr1Reg, 2>;
        using TeBit = RegisterBit<Cr1Reg, 3>;
        using IdleIeBit = RegisterBit<Cr1Reg, 4>;
        using RxneIeBit = RegisterBit<Cr1Reg, 5>;
        using TcIeBit = RegisterBit<Cr1Reg, 6>;
        using TxeIeBit = RegisterBit<Cr1Reg, 7>;
        using PeIeBit = RegisterBit<Cr1Reg, 8>;
        struct PsBit : RegisterBit<Cr1Reg, 9>
        {
            using B = PsBit;
            using EvenParity = typename B::template Value<0>;
            using OddParity = typename B::template Value<1>;
        };
        using PceBit = RegisterBit<Cr1Reg, 10>;
        using WakeBit = RegisterBit<Cr1Reg, 11>;
        struct MBit : RegisterBit<Cr1Reg, 12>
        {
            using B = MBit;
            using EightBit = typename B::template Value<0>;
            using NineBit = typename B::template Value<1>;
        };
        using UeBit = RegisterBit<Cr1Reg, 13>;
        using Over8Bit = RegisterBit<Cr1Reg, 15>;

        using AddBits = RegisterBit<Cr2Reg, 0, 3>;
        // 4 reserved
        using LbdlBit = RegisterBit<Cr2Reg, 5>;
        using LbdIeBit = RegisterBit<Cr2Reg, 6>;
        // 7 reserved
        using LbclBit = RegisterBit<Cr2Reg, 8>;
        using CphaBit = RegisterBit<Cr2Reg, 9>;
        using CpolBit = RegisterBit<Cr2Reg, 10>;
        using CklenBit = RegisterBit<Cr2Reg, 11>;
        struct StopBits : RegisterBit<Cr2Reg, 12, 2>
        {
            using B = StopBits;
            using OneStopBit = typename B::template Value<0>;
            using HalfStopBit = typename B::template Value<1>;
            using TwoStopBits = typename B::template Value<2>;
            using OneAndHalfStopBit = typename B::template Value<3>;
        };
        using LinEnBit = RegisterBit<Cr2Reg, 14>;

        template<typename F>
        using Interrupt = InterruptTemplate<F>;
};

#define INDEMIC_IMPLEMENT_INTERRUPT(name, str) \
template<typename Functor> \
class name \
{ \
    public: \
        enum {t = 1}; \
        static __attribute__ ((used)) \
               __attribute__ ((section (str))) \
               void (*interrupt)(void); \
}; \
template<typename Functor> \
__attribute__ ((used)) \
__attribute__ ((section (str))) \
void (*name<Functor>::interrupt)(void) \
= Functor::INDEMIC_INTERRUPT_FUNCTION_NAME;

INDEMIC_IMPLEMENT_INTERRUPT(InterruptUsart1, ".indemic_interrupt_usart1");
INDEMIC_IMPLEMENT_INTERRUPT(InterruptUsart2, ".indemic_interrupt_usart2");
INDEMIC_IMPLEMENT_INTERRUPT(InterruptUsart3, ".indemic_interrupt_usart3");
INDEMIC_IMPLEMENT_INTERRUPT(InterruptUart4, ".indemic_interrupt_uart4");
INDEMIC_IMPLEMENT_INTERRUPT(InterruptUart5, ".indemic_interrupt_uart5");
INDEMIC_IMPLEMENT_INTERRUPT(InterruptUsart6, ".indemic_interrupt_usart6");
#undef INDEMIC_IMPLEMENT_INTERRUPT

}
}
