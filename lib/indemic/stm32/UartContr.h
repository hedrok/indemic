/**
 * @file UartContr.h
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

#include <indemic/UartContr.h>
#include <indemic/generic/RegisterVisitor.h>

namespace IndeMic
{

/**
 * UartContr, STM32 implementation
 *
 * @param Microcontroller
 * @param UartConfig - uart configuration, nine-bit is unsupported
 * @param Uart - which uart to use
 *
 * @see template<typename M, typename UartConfig, typename UartPeriphery>
 *      class UartContr
 *      for methods description
 */
template<typename UartConfig, typename UartPeriphery, typename Clock, typename RxPin, typename TxPin>
class UartContr<stm32::STM32Mic<Clock>, UartConfig, UartPeriphery, RxPin, TxPin>
{
    public:
        static inline bool canWriteData()
        {
            return UartPeriphery::TxeBit::getValue();
        }
        static inline void disable()
        {
            RegisterVisitor::clear<
                typename UartPeriphery::UeBit
            >();
        }
        static void disableReadInt()
        {
            RegisterVisitor::clear<typename UartPeriphery::RxneIeBit>();
        }
        static void disableWriteInt()
        {
            RegisterVisitor::clear<typename UartPeriphery::TxeIeBit>();
        }
        static inline void enable()
        {
            constexpr double baudRateDouble = 1e9 / (UartPeriphery::nsPerClock * UartConfig::baudRate);
            constexpr uint64_t fracDiv = (1 << UartPeriphery::DivFractionBits::width);
            constexpr uint64_t fractionPart = static_cast<uint64_t>(baudRateDouble + 0.5) % fracDiv;
            constexpr uint64_t mantissaPart = static_cast<uint64_t>(baudRateDouble + 0.5) / fracDiv;
            static_assert(mantissaPart < (1 << UartPeriphery::DivMantissaBits::width), "baudRate too low");
            static_assert(mantissaPart != 0 || fractionPart != 0, "baudRate too high");
            static_assert(UartConfig::charWidth >= 8 && UartConfig::charWidth <= 9, "Unsupported charWidth");

            RxPin::Port::template setAlternateFunction<RxPin>(UartPeriphery::alternateFunctionValue);
            RxPin::Port::template setMode<RxPin>(GPIO_MODE_AF);
            TxPin::Port::template setAlternateFunction<TxPin>(UartPeriphery::alternateFunctionValue);
            TxPin::Port::template setMode<TxPin>(GPIO_MODE_AF);

            using UartCharacterWidthValue = typename std::tuple_element<
                UartConfig::charWidth - 8,
                std::tuple<
                    typename UartPeriphery::MBit::EightBit,
                    typename UartPeriphery::MBit::NineBit
                >
            >::type;
            using UartParityValue = typename std::tuple_element<
                static_cast<uint8_t>(UartConfig::parityCheck),
                std::tuple<
                    typename UartPeriphery::PceBit::template Value<0>,
                    RegisterBitBundle<
                        typename UartPeriphery::PceBit,
                        typename UartPeriphery::PsBit::EvenParity
                    >,
                    RegisterBitBundle<
                        typename UartPeriphery::PceBit,
                        typename UartPeriphery::PsBit::OddParity
                    >
                >
            >::type;
            using UartStopBitValue = typename std::tuple_element<
                UartConfig::stopBits - 1,
                std::tuple<
                    typename UartPeriphery::StopBits::OneStopBit,
                    typename UartPeriphery::StopBits::TwoStopBits
                >
            >::type;
            RegisterVisitor::assign<
                typename UartPeriphery::DivFractionBits::template Value<fractionPart>,
                typename UartPeriphery::DivMantissaBits::template Value<mantissaPart>,
                UartCharacterWidthValue,
                UartParityValue,
                UartStopBitValue,
                typename UartPeriphery::UeBit,
                typename UartPeriphery::ReBit,
                typename UartPeriphery::TeBit
            >();
        }
        static void enableReadInt()
        {
            RegisterVisitor::set<typename UartPeriphery::RxneIeBit>();
        }
        static void enableWriteInt()
        {
            RegisterVisitor::set<typename UartPeriphery::TxeIeBit>();
        }
        static inline bool isDataAvailable()
        {
            return UartPeriphery::RxneBit::getValue();
        }
        static inline uint8_t readNonBlocking()
        {
            return UartPeriphery::DrRegS::get();
        }
        static inline void writeNonBlocking(uint8_t byte)
        {
            UartPeriphery::DrRegS::assign(byte);
        }

        template<typename Bit, typename Functor>
        class CheckBitAndCallFunctor
        {
            public:
                static void call()
                {
                    if (Bit::getValue()) {
                        Functor::INDEMIC_INTERRUPT_FUNCTION_NAME();
                    }
                }
        };
        class EmptyInterrupt
        {
            public:
                static void call() {}
        };

        template<typename FunctorReadAvailable, typename FunctorWriteAvailable>
        class FunctorWrapper
        {
            using ReadFunctor = typename std::tuple_element<
                std::is_same<FunctorReadAvailable, void>::value ? 0 : 1,
                std::tuple<
                    EmptyInterrupt,
                    CheckBitAndCallFunctor<
                        typename UartPeriphery::RxneBit,
                        FunctorReadAvailable
                    >
                >
            >::type;
            using WriteFunctor = typename std::tuple_element<
                std::is_same<FunctorWriteAvailable, void>::value ? 0 : 1,
                std::tuple<
                    EmptyInterrupt,
                    CheckBitAndCallFunctor<
                        typename UartPeriphery::TxeBit,
                        FunctorWriteAvailable
                    >
                >
            >::type;
            public:
                INDEMIC_INTERRUPT_FUNCTION __attribute__((interrupt)) __attribute__((used))
                {
                    ReadFunctor::call();
                    WriteFunctor::call();
                }
                enum {t = 1};
        };
        template<typename FunctorReadAvailable, typename FunctorWriteAvailable>
        class Interrupts
        {
            static_assert(
                UartPeriphery::template Interrupt<
                    FunctorWrapper<
                        FunctorReadAvailable,
                        FunctorWriteAvailable
                    >
                >::t == 1,
                "instantiate Interrupt::interrupt"
            );
            static void initNvic() __attribute__((constructor))
                                    __attribute__((used))
            {
                nvic_enable_irq(UartPeriphery::irqNumber);
            }
            public:
                enum {t = 1};
        };
    private:
        /**
         * Enabling RCC beforehand, because some functionality starts
         * working after this. For example, setting RxneIe bit before 
         * enabling RCC does not have effect even after RCC is set,
         * so without this
         * ::enableReadInt()
         * ::enable()
         * won't work for STM32 which is really unexpected.
         */
        static void initClock() __attribute__((constructor))
                                __attribute__((used))
        {
            RegisterVisitor::assign<
                typename UartPeriphery::RccEnBit
            >();
        }
};

}
