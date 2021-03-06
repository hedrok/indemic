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

namespace IndeMic
{

/**
 * UartContr
 *
 * UART controller. Provides: blocking/non-blocking write/read
 * on UART and interrupt handlers for writing/reading
 * 
 * @param Microcontroller
 * @param UartConfig - uart configuration, nine-bit is unsupported
 * @param UartPeriphery - which uart to use
 * @param RxPin - IOPin of Rx, void by default as not used by all families
 * @param TxPin - IOPin of Tx, void by default as not used by all families
 */
template<
    typename M,
    typename UartConfig,
    typename UartPeriphery,
    typename RxPin = void,
    typename TxPin = void
> class UartContr
{
    typedef UartContr<typename M::parent, UartConfig, UartPeriphery, RxPin, TxPin> P;
    public:
        /**
         * canWriteData
         * @return true if writing is available now,
         * false if transmission is in progress
         */
        static inline bool canWriteData()
        {
            return P::canWriteData();
        }
        /**
         * Disable uart
         */
        static inline void disable()
        {
            P::disable();
        }
        /**
         * Disable interrupt on read available
         */
        static void disableReadInt()
        {
            P::disableReadInt();
        }
        /**
         * Disable interrupt on write available
         */
        static void disableWriteInt()
        {
            P::disableWriteInt();
        }
        /**
         * Enables uart, sets parity, byte width, parity, baud
         * (configuration can be done beforehand in constructor)
         */
        static inline void enable()
        {
            P::enable();
        }
        /**
         * Enable interrupt on read available
         */
        static void enableReadInt()
        {
            P::enableReadInt();
        }
        /**
         * Enable interrupt on write available
         */
        static void enableWriteInt()
        {
            P::enableWriteInt();
        }
        /**
         * isDataAvailable
         * @return true if data is available
         */
        static inline bool isDataAvailable()
        {
            return P::isDataAvailable();
        }
        /**
         * Read byte
         * Be careful - this function will block till byte
         * is read
         * @return Byte read.
         */
        static inline uint8_t readBlocking()
        {
            while (!isDataAvailable()) {
                continue;
            }
            return readNonBlocking();
        }
        /**
         * Read byte
         * Non-blocking version, check if there is
         * byte by isDataAvailable
         * @return Byte read or grabage if no data
         *         is available.
         */
        static inline uint8_t readNonBlocking()
        {
            return P::readNonBlocking();
        }
        /**
         * Write byte (nine-bit is not supported)
         * Call to this function may take some time 
         * because it waits till previous byte is sent.
         * @param byte Byte to send
         */
        static inline void writeBlocking(uint8_t byte)
        {
            while (!canWriteData()) {
                continue;
            }
            writeNonBlocking(byte);
        }
        /**
         * Write byte (nine-bit is not supported)
         * This function is non-blocking, check if 
         * @param byte Byte to send
         */
        static inline void writeNonBlocking(uint8_t byte)
        {
            P::writeNonBlocking(byte);
        }

        /**
         * Instantiate user interrupts
         *
         * Don't forget to call init()
         *
         * @param FunctorReadAvailable This functor will be called each time
         *                             character is ready to be read via this UART
         * @param FunctorWriteAvailable This functor will be called each time
         *                              character is ready to be written to this UART
         */
        template<typename FunctorReadAvailable, typename FunctorWriteAvailable>
        class Interrupts
        {
            using PInt = typename P::template Interrupts<FunctorReadAvailable, FunctorWriteAvailable>;
            static_assert(PInt::t == 1, "Instantiate PInt failed");
            public:
                static void init(){}
        };
};

}
