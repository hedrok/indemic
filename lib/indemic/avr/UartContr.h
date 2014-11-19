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
 * UartContr, AVR implementation
 *
 * @param Microcontroller
 * @param UartConfig - uart configuration, nine-bit is unsupported
 * @param Uart - which uart to use
 *
 * @see template<typename M, typename UartConfig, typename UartPeriphery>
 *      class UartContr
 *      for methods description
 */
template<typename UartConfig, typename UartPeriphery, uint64_t ns>
class UartContr<avr::AVRMic<ns>, UartConfig, UartPeriphery>
{
    public:
        static inline bool canWriteData()
        {
            return UartPeriphery::Udre::getValue();
        }
        static inline void disable()
        {
            RegisterVisitor::clear<
                typename UartPeriphery::RxEn,
                typename UartPeriphery::TxEn
            >();
        }
        static void disableReadInt()
        {
            RegisterVisitor::clear<typename UartPeriphery::RxcIe>();
        }
        static void disableWriteInt()
        {
            RegisterVisitor::clear<typename UartPeriphery::UdreIe>();
        }
        static inline void enable()
        {
            static_assert((16 * ns * UartConfig::baudRate) <= 1000000000., "Too high baud rate");
            constexpr uint16_t ubrrValue = static_cast<uint16_t>(
                (1000000000. / (16 * ns * UartConfig::baudRate) - 1.) + 0.5
            );
            using UartCharacterWidthValue = typename std::tuple_element<
                UartConfig::charWidth - 5,
                std::tuple<
                    typename UartPeriphery::Ucsz::FiveBit,
                    typename UartPeriphery::Ucsz::SixBit,
                    typename UartPeriphery::Ucsz::SevenBit,
                    typename UartPeriphery::Ucsz::EightBit
                >
            >::type;
            using UartParityValue = typename std::tuple_element<
                static_cast<uint8_t>(UartConfig::parityCheck),
                std::tuple<
                    typename UartPeriphery::Upm::Disabled,
                    typename UartPeriphery::Upm::EnabledEven,
                    typename UartPeriphery::Upm::EnabledOdd
                >
            >::type;
            using UartStopBitValue = typename std::tuple_element<
                UartConfig::stopBits - 1,
                std::tuple<
                    typename UartPeriphery::Usbs::OneStopBit,
                    typename UartPeriphery::Usbs::TwoStopBits
                >
            >::type;
            UartPeriphery::Ubrr::set(ubrrValue);
            RegisterVisitor::set<
                UartCharacterWidthValue,
                UartParityValue,
                UartStopBitValue,
                typename UartPeriphery::RxEn,
                typename UartPeriphery::TxEn
            >();
        }
        static void enableReadInt()
        {
            RegisterVisitor::set<typename UartPeriphery::RxcIe>();
        }
        static void enableWriteInt()
        {
            RegisterVisitor::set<typename UartPeriphery::UdreIe>();
        }
        static inline bool isDataAvailable()
        {
            return UartPeriphery::Rxc::getValue();
        }
        static inline uint8_t readNonBlocking()
        {
            return UartPeriphery::Udr::get();
        }
        static inline void writeNonBlocking(uint8_t byte)
        {
            UartPeriphery::Udr::assign(byte);
        }

        template<typename Functor>
        class ReadAvailableInterrupt
        {
            static_assert(
                (UartPeriphery::template ReceiveCompleteInterrupt<Functor>::t == 1),
                "Something very wrong. This line should just instantiate Interrupt template"
            );
            public:
                enum {t = 1};
        };

        template<typename Functor>
        class WriteAvailableInterrupt
        {
            static_assert(
                (UartPeriphery::template DataRegisterEmptyInterrupt<Functor>::t == 1),
                "Something very wrong. This line should just instantiate Interrupt template"
            );
            public:
                enum {t = 1};
        };

        class EmptyInterrupt
        {
            public:
                enum {t = 1};
        };

        template<typename FunctorReadAvailable, typename FunctorWriteAvailable>
        class Interrupts
        {
            using ReadInterrupt = typename std::tuple_element<
                std::is_same<FunctorReadAvailable, void>::value ? 0 : 1,
                std::tuple<
                    EmptyInterrupt,
                    ReadAvailableInterrupt<FunctorReadAvailable>
                >
            >::type;
            using WriteInterrupt = typename std::tuple_element<
                std::is_same<FunctorWriteAvailable, void>::value ? 0 : 1,
                std::tuple<
                    EmptyInterrupt,
                    WriteAvailableInterrupt<FunctorWriteAvailable>
                >
            >::type;
            static_assert(ReadInterrupt::t == 1, "Instantiate ReadInterrupt failed");
            static_assert(WriteInterrupt::t == 1, "Instantiate WriteInterrupt failed");
            public:
                enum {t = 1};
        };
};

}
