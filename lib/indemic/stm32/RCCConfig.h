/**
 * @file RCCConfig.h
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

namespace stm32
{

/**
 * RCCConfig
 *
 * Use this class to configure clock frequency of STM32 mcu's
 * @param inputFrequency Frequency of input oscilator in Hz
 * @param inputVoltage in milliVolts
 * @param AHBFrequency required AHBFrequency in Hz, set to zero
 *        for default value (maximum)
 * @param APB2Frequency same for APB2 (maximum possible if zero)
 *        Set to non-default only if you know what you are doing.
 * @param APB1Frequency same for APB1 (maximum possible if zero)
 *        Set to non-default only if you know what you are doing.
 * @param PeriphFrequency same for USB OTG FS, SDIO and RNG timers.
 *        (maximum possible if zero)
 *        Set to non-default only if you know what you are doing.
 * @param eps Maximum permitted error in frequency setting in milli herz
 */
template<uint64_t inputFrequency,
         uint64_t inputVoltage,
         uint64_t AHBFrequency = 0,
         uint64_t APB2Frequency = 0,
         uint64_t APB1Frequency = 0,
         uint64_t PeripheryFrequency = 0,
         uint64_t epsilon = 500>
class RCCConfig
{
    public:
        constexpr static double inputFreq  = inputFrequency;
        constexpr static double AHBFreq    = AHBFrequency;
        constexpr static double APB2Freq   = APB2Frequency;
        constexpr static double APB1Freq   = APB1Frequency;
        constexpr static double PeriphFreq = PeripheryFrequency;
        constexpr static double eps = epsilon / 1000.;
        constexpr static double voltage = inputVoltage / 1000.;
};

}
}
