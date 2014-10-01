/**
 * @file RCCInitializer.h
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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>

namespace IndeMic
{

namespace stm32
{

/**
 * Initializes various STM32 clocks, flash latency
 * Currently only STM32F4 is supported, need to read carefully documentation for
 * other microcontrollers before using this class for them
 */
template<typename M, uint64_t maxAHBFreq, uint64_t maxAPB2Freq, uint64_t maxAPB1Freq, uint64_t maxPeriphFreq>
class RCCInitializerSTM32F4
{
    public:
        // Setting frequencies that we wish to get
        constexpr static uint64_t AHBFreq = M::RCCConf::AHBFreq == 0. ? maxAHBFreq : M::RCCConf::AHBFreq;
        constexpr static uint64_t APB2Freq =   M::RCCConf::APB2Freq != 0. ? M::RCCConf::APB2Freq
                                           : AHBFreq > maxAPB2Freq      ? maxAPB2Freq
                                           :                              AHBFreq;
        constexpr static uint64_t APB1Freq =   M::RCCConf::APB1Freq != 0. ? M::RCCConf::APB1Freq
                                           : AHBFreq > maxAPB1Freq      ? maxAPB1Freq
                                           :                              AHBFreq;
        constexpr static uint64_t PeriphFreq =   M::RCCConf::PeriphFreq != 0. ? M::RCCConf::PeriphFreq
                                             : AHBFreq > maxPeriphFreq      ? maxPeriphFreq
                                             :                                AHBFreq;
        constexpr static uint64_t APB2TimerFreq = APB2Freq == AHBFreq ? APB2Freq : APB2Freq * 2;
        constexpr static uint64_t APB1TimerFreq = APB1Freq == AHBFreq ? APB1Freq : APB1Freq * 2;

        static_assert(AHBFreq  <= maxAHBFreq,  "Required AHB frequence is higher than premitted");
        static_assert(APB2Freq <= maxAPB2Freq, "Required APB2 frequence is higher than premitted");
        static_assert(APB1Freq <= maxAPB1Freq, "Required APB1 frequence is higher than premitted");
        static_assert(PeriphFreq <= maxPeriphFreq, "Required Peripheral frequence is higher than premitted");

        // Same for PPRE1 and PPRE2
        template <int ppreDiv>
        static constexpr int getPpreValue()
        {
            static_assert(   ppreDiv == 1
                          || ppreDiv == 2
                          || ppreDiv == 4
                          || ppreDiv == 8
                          || ppreDiv == 16,
                          "PPRE division selection failed. Check ration of APBn and AHB");
            return   ppreDiv ==  1 ? 0
                   : ppreDiv ==  2 ? 4
                   : ppreDiv ==  4 ? 5
                   : ppreDiv ==  8 ? 6
                   : ppreDiv == 16 ? 7
                   :                 0;

        }

        //TODO: change libopencm3 registers and bits to indemic ones
        static void initRCC() __attribute__((constructor))
                              __attribute__((used))
        {
            // turn on HSI and HSE
            RCC_CR |= RCC_CR_HSION;
            while ((RCC_CR & RCC_CR_HSIRDY) == 0);
            RCC_CR |= RCC_CR_HSEON;
            while ((RCC_CR & RCC_CR_HSERDY) == 0);

            // vco = in * (n / m)
            // AHBFreq = vco / p

            constexpr uint64_t minN = 64;
            constexpr uint64_t maxN = 432;
            constexpr uint64_t minM = 2;
            constexpr uint64_t maxM = 63;
            constexpr uint64_t minP = 2;
            constexpr uint64_t maxP = 8;
            //constexpr uint64_t stepP = 2;
            constexpr uint64_t minQ = 2;
            constexpr uint64_t maxQ = 15;
            constexpr uint64_t minVCOFreq =  64000000;
            constexpr uint64_t maxVCOFreq = 432000000;

            constexpr uint64_t needRatio = AHBFreq / M::RCCConf::inputFreq;
            static_assert(needRatio >= minN / maxM / maxP,
                          "Input frequency is too high for needed AHBFreq. TODO: implement AHB clock prescaler\n"
                          "in RCC_CFGR (HPRE bits)");
            static_assert(needRatio <= maxN / minM / minP, "Input frequency is too low for needed AHBFreq.");

            constexpr int p =   needRatio > minN / maxM / 2 ? 2
                              : needRatio > minN / maxM / 4 ? 4
                              : needRatio > minN / maxM / 6 ? 6
                              : needRatio > minN / maxM / 8 ? 8
                              : -1;

            static_assert(p != -1, "Could not select p value");

            constexpr int pValue = p / 2 - 1;

            constexpr uint64_t x = needRatio * p;
            static_assert(x >= minN / maxM, "Error");
            static_assert(x <= maxN / minM, "Error");

            // Let x = needRatio * p,
            // then x is in range [minN / maxM, maxN / minM]
            // We can cover this range just by two cases:
            //
            // 1) m = minM
            // as x = n / m 
            // -> n = x * m
            // -> minN <= x * minM <= maxN
            // part x * minM <= maxN is always true as
            // maximum value of x is maxN / minM, so
            // maxN / minM * minM <= maxN
            // maxN <= maxN, which is true.
            // minN <= x * minM
            // So this case covers all x >= minN / minM
            //
            // 2) n = minN
            // as x = n / m
            // -> m = n / x
            // -> minM <= minN / x <= maxM
            // part minN / x <= maxM is always true as
            // minimum value of x is minN / maxM, so
            // minN / (minN / maxM) <= maxM
            // maxM <= maxM, which is true.
            // minM <= minN / x
            // This case covers all x <= minN / minM
            // 
            // So these two cases cover whole range of x.

            constexpr int m = x >= minN / minM ? minM
                                               : minN / x;
            constexpr int n = x >= minN / minM ? x * minM
                                               : minN;

            constexpr double vcoFrequency = M::RCCConf::inputFreq * n / m;
            static_assert(vcoFrequency >= minVCOFreq && vcoFrequency <= maxVCOFreq,
                          "VCO frequency wrong. Should make RCCInitializer smarter, or it is impossible"
                          "to fulfill RCCConf requirements");

            constexpr int q = M::RCCConf::inputFreq * n / m / PeriphFreq;
            static_assert(q >= minQ && q <= maxQ, "Could not select q");

            //TODO: check all frequencies, move to separate class
            constexpr double signedDiff = AHBFreq - M::RCCConf::inputFreq * n / m / p;
            constexpr double diff = signedDiff < 0. ? -signedDiff : signedDiff;
            static_assert(diff <= M::RCCConf::eps,
                          "Difference between chosen parameters and required frequency"
                          " is too big.");

            RCC_PLLCFGR = (m      << RCC_PLLCFGR_PLLM_SHIFT) | // Division factor for PLL input
                          (n      << RCC_PLLCFGR_PLLN_SHIFT) | // multiplication for VCO
                          (pValue << RCC_PLLCFGR_PLLP_SHIFT) | // Main system clock division factor
                          RCC_PLLCFGR_PLLSRC |                 // HSE clock as input
                          (q      << RCC_PLLCFGR_PLLQ_SHIFT);  // USB, SDIO, Random-number generator

            // Turning PLL on
            RCC_CR |= RCC_CR_PLLON;
            while ((RCC_CR & RCC_CR_PLLRDY) == 0);

            // TODO: select based on voltage and frequency
            //       try to understand what are all those
            //       caches 
            FLASH_ACR = 0x605;

            // Selecting prescalers for APB2 and APB1
            constexpr int ppre2Div = AHBFreq / APB2Freq;
            constexpr int ppre1Div = AHBFreq / APB1Freq;
            constexpr int ppre2Value = getPpreValue<ppre2Div>();
            constexpr int ppre1Value = getPpreValue<ppre1Div>();

            RCC_CFGR = (ppre2Value << 13) | (ppre1Value << 10) | (1 << 1);
            while ((RCC_CFGR & ((1 << 1) | (1 << 0))) != RCC_CFGR_SWS_PLL);

            // Set for libopencm3 to work properly
            rcc_ppre1_frequency = APB1Freq;
            rcc_ppre2_frequency = APB2Freq;

            // Turn HSI off - clock is fully configured and we
            // don't need internal clock anymore
            RCC_CR &= ~RCC_CR_HSION;
        }
};

}
}
