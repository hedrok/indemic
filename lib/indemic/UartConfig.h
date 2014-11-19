/**
 * @file UartConfig.h
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

enum class UartParity
{
    off,
    even,
    odd
};

/**
 * UartConfig
 *
 * UART configuration class
 * 
 * @param baud rate in bits per second
 * @param charWidth - width of one character, should be in range [5, 9], but
 *                    9 is currently unsupported by controller
 * @param parityCheck - off, even or odd (use constants from UartParity
 * @param stopBits - number of stop bits (either 1 or 2)
 */
template<
    uint64_t _baudRate,
    uint8_t _charWidth,
    UartParity _parityCheck,
    uint8_t _stopBits
> class UartConfig
{
    static_assert(_charWidth >= 5 && _charWidth <= 9, "Wrong charWidth");
    static_assert(_stopBits >= 1 && _stopBits <= 2, "Wrong stopBits");
    public:
        static constexpr uint64_t baudRate = _baudRate;
        static constexpr uint8_t charWidth = _charWidth;
        static constexpr UartParity parityCheck = _parityCheck;
        static constexpr uint8_t stopBits = _stopBits;
};

}
