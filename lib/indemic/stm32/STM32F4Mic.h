/**
 * @file microcontrollers.h
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
 *
 * @section DESCRIPTION
 * This file contains class tags for STM32 products
 */
#pragma once

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include <indemic/stm32/STM32Mic.h>
#include <indemic/stm32/Port.h>
#include <indemic/stm32/IOPin.h>
#include <indemic/stm32/Timer.h>
#include <indemic/stm32/UsartPeriphery.h>
#include <indemic/stm32/RCCInitializer.h>

namespace IndeMic
{
namespace stm32
{
    template<typename R>
    class STM32F4Mic : public STM32Mic<R>
    {
        public:
            typedef STM32Mic<R> parent;
            typedef STM32F4Mic M;
            typedef RCCInitializerSTM32F4<STM32F4Mic<R>, 168000000, 84000000, 42000000, 48000000> Clock;

            typedef Port<M, GPIOA> PortA;
            typedef Port<M, GPIOB> PortB;
            typedef Port<M, GPIOC> PortC;
            typedef Port<M, GPIOD> PortD;

            typedef IOPin<M, PortA, 2> PA2Pin;
            typedef IOPin<M, PortA, 3> PA3Pin;

            typedef IOPin<M, PortD, 12> PD12Pin;
            typedef IOPin<M, PortD, 13> PD13Pin;
            typedef IOPin<M, PortD, 14> PD14Pin;
            typedef IOPin<M, PortD, 15> PD15Pin;

            class RccCr      : public RegisterBase<M, RCC_BASE + 0x00, RccCr> {};
            class RccPllCfgr : public RegisterBase<M, RCC_BASE + 0x04, RccPllCfgr> {};
            class RccCfgr    : public RegisterBase<M, RCC_BASE + 0x08, RccCfgr> {};
            class RccApb1Enr : public RegisterBase<M, RCC_BASE + 0x40, RccApb1Enr> {};
            class RccApb2Enr : public RegisterBase<M, RCC_BASE + 0x44, RccApb2Enr> {};

            class RccCrHsiOn  : public RegisterBit<RccCr,  0> {};
            class RccCrHsiRdy : public RegisterBit<RccCr,  1> {};
            class RccCrHseOn  : public RegisterBit<RccCr, 16> {};
            class RccCrHseRdy : public RegisterBit<RccCr, 17> {};
            class RccCrPllOn  : public RegisterBit<RccCr, 24> {};
            class RccCrPllRdy : public RegisterBit<RccCr, 25> {};

            class RccPllCfgrQ   : public RegisterBit<RccPllCfgr, 24, 4> {};
            class RccPllCfgrSrc : public RegisterBit<RccPllCfgr, 22   >
            {
                public:
                    using B   = RegisterBit<RccPllCfgr, 22>;
                    using Hsi = typename B::template Value<0>;
                    using Hse = typename B::template Value<1>;
            };
            class RccPllCfgrP   : public RegisterBit<RccPllCfgr, 16, 2> {};
            class RccPllCfgrN   : public RegisterBit<RccPllCfgr,  6, 9> {};
            class RccPllCfgrM   : public RegisterBit<RccPllCfgr,  0, 6> {};

            class RccCfgrPpre2 : public RegisterBit<RccCfgr, 13, 3> {};
            class RccCfgrPpre1 : public RegisterBit<RccCfgr, 10, 3> {};
            class RccCfgrSws   : public RegisterBit<RccCfgr,  2, 2>
            {
                public:
                    using B   =  RegisterBit<RccCfgr, 2, 2>;
                    using Hsi = typename B::template Value<0>;
                    using Hse = typename B::template Value<1>;
                    using Pll = typename B::template Value<2>;
            };
            class RccCfgrSw : public RegisterBit<RccCfgr, 0, 2>
            {
                public:
                    using B = RegisterBit<RccCfgr, 0, 2>;
                    using Hsi = typename B::template Value<0>;
                    using Hse = typename B::template Value<1>;
                    using Pll = typename B::template Value<2>;
            };

            class FlashAcr : public RegisterBase<M, FLASH_MEM_INTERFACE_BASE + 0x00, FlashAcr> {};
            
            class FlashAcrDCRst   : public RegisterBit<FlashAcr, 12> {};
            class FlashAcrICRst   : public RegisterBit<FlashAcr, 11> {};
            class FlashAcrDCEn    : public RegisterBit<FlashAcr, 10> {};
            class FlashAcrICEn    : public RegisterBit<FlashAcr,  9> {};
            class FlashAcrPrftEn  : public RegisterBit<FlashAcr,  8> {};
            class FlashAcrLatency : public RegisterBit<FlashAcr,  0, 3> {};


            class Timer4 : public TimerBase<M, TIM4, 16, InterruptTimer4, Clock::APB1TimerFreq, GPIO_AF2>
            {
                public:
                    class RccEn : public RegisterBit<RccApb1Enr, 2> {};
                    enum { irqNumber = NVIC_TIM4_IRQ };
            };

            class Usart1 : public UsartPeriphery<M, USART1, InterruptUsart1, Clock::APB2Freq, GPIO_AF7>
            {
                public:
                    using RccEnBit = RegisterBit<RccApb2Enr, 4>;
                    enum { irqNumber = NVIC_USART1_IRQ };
            };
            class Usart2 : public UsartPeriphery<M, USART2, InterruptUsart2, Clock::APB1Freq, GPIO_AF7>
            {
                public:
                    using RccEnBit = RegisterBit<RccApb1Enr, 17>;
                    enum { irqNumber = NVIC_USART2_IRQ };
            };
            class Usart3 : public UsartPeriphery<M, USART3, InterruptUsart3, Clock::APB1Freq, GPIO_AF7>
            {
                public:
                    using RccEnBit = RegisterBit<RccApb1Enr, 18>;
                    enum { irqNumber = NVIC_USART3_IRQ };
            };
            class Uart4 : public UsartPeriphery<M, UART4, InterruptUart4, Clock::APB1Freq, GPIO_AF8>
            {
                public:
                    using RccEnBit = RegisterBit<RccApb1Enr, 19>;
                    enum { irqNumber = NVIC_UART4_IRQ };
            };
            class Uart5 : public UsartPeriphery<M, UART5, InterruptUart5, Clock::APB1Freq, GPIO_AF8>
            {
                public:
                    using RccEnBit = RegisterBit<RccApb1Enr, 20>;
                    enum { irqNumber = NVIC_UART5_IRQ };
            };
            class Usart6 : public UsartPeriphery<M, USART6, InterruptUsart6, Clock::APB2Freq, GPIO_AF8>
            {
                public:
                    class RccEnBit : public RegisterBit<RccApb2Enr, 5> {};
                    enum { irqNumber = NVIC_USART6_IRQ };
            };
    };
    template<typename R>
    class OutputCompareUnitSelector<STM32F4Mic<R>, typename STM32F4Mic<R>::PD12Pin>
    {
        public:
            using type = OutputCompareChannel<typename STM32F4Mic<R>::Timer4, 1>;
    };
    template<typename R>
    class OutputCompareUnitSelector<STM32F4Mic<R>, typename STM32F4Mic<R>::PD13Pin>
    {
        public:
            using type = OutputCompareChannel<typename STM32F4Mic<R>::Timer4, 2>;
    };
    template<typename R>
    class OutputCompareUnitSelector<STM32F4Mic<R>, typename STM32F4Mic<R>::PD14Pin>
    {
        public:
            using type = OutputCompareChannel<typename STM32F4Mic<R>::Timer4, 3>;
    };
    template<typename R>
    class OutputCompareUnitSelector<STM32F4Mic<R>, typename STM32F4Mic<R>::PD15Pin>
    {
        public:
            using type = OutputCompareChannel<typename STM32F4Mic<R>::Timer4, 4>;
    };
}
}
