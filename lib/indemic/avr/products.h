/**
 * @file microcontrollers.h
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
 *
 * @section DESCRIPTION
 * This file contains class tags for AVR products
 */
#pragma once

#include <indemic/avr/Port.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterVisitor.h>

namespace IndeMic
{
namespace avr
{
    /** Generic AVR microcontroller */
    template<uint64_t ns = 0>
    class AVRMic
    {
        public:
            typedef volatile uint8_t register_t;
            typedef uint8_t register_value_t;
            typedef volatile uint16_t register_double_t;
            typedef uint16_t register_double_value_t;
            enum {nsPerClock = ns};
    };

    template<uint64_t ns = 0>
    class AT90USB162Mic : public AVRMic<ns>
    {
        public:
            typedef AT90USB162Mic M;

            typedef Port<AT90USB162Mic, 0x23> PortB;
            typedef Port<AT90USB162Mic, 0x26> PortC;
            typedef Port<AT90USB162Mic, 0x29> PortD;

            // External Interrupt Registers
            class EicrA : public RegisterBase<M, 0x69, EicrA> {};
            class EicrB : public RegisterBase<M, 0x6a, EicrB> {};
            class Eimsk : public RegisterBase<M, 0x3d, Eimsk> {};

            class Int0
            {
                public:
                class ISCn0 : public RegisterBit<EicrA, 0> {};
                class ISCn1 : public RegisterBit<EicrA, 1> {};

                class INTn : public RegisterBit<Eimsk, 0> {};

                //Interrupt description
                template<typename Functor>
                class Interrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".int0_vector")))
                               __attribute__ ((section (".int0_vector")))
                               const uint16_t interrupt;
                };
            };

            class Int1
            {
                public:
                class ISCn0 : public RegisterBit<EicrA, 2> {};
                class ISCn1 : public RegisterBit<EicrA, 3> {};

                class INTn : public RegisterBit<Eimsk, 1> {};

                //Interrupt description
                template<typename Functor>
                class Interrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".int1_vector")))
                               const uint16_t interrupt;
                };
            };

            class Int2
            {
                public:
                class ISCn0 : public RegisterBit<EicrA, 4> {};
                class ISCn1 : public RegisterBit<EicrA, 5> {};

                class INTn : public RegisterBit<Eimsk, 2> {};

                //Interrupt description
                template<typename Functor>
                class Interrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".int2_vector")))
                               const uint16_t interrupt;
                };
            };

            // Timer0 Registers
            class Tccr0A : public RegisterBase<M, 0x44, Tccr0A> {};
            class Tccr0B : public RegisterBase<M, 0x45, Tccr0B> {};
            class Tcnt0  : public RegisterSettable<M, 0x46, Tcnt0 > {};
            class Ocr0A  : public RegisterSettable<M, 0x47, Ocr0A > {};
            class Ocr0B  : public RegisterSettable<M, 0x48, Ocr0B > {};
            class Timsk0 : public RegisterBase<M, 0x6e, Timsk0> {};
            class Tifr0  : public RegisterBase<M, 0x35, Tifr0>  {};

            class Timer0
            {
                public:
                constexpr static uint64_t counterBits = 8;
                constexpr static uint64_t counterResolution = 1 << counterBits;
                typedef Tccr0A TccrA;
                class Wgm0  : public RegisterBit<TccrA, 0> {};
                class Wgm1  : public RegisterBit<TccrA, 1> {};
                class ComB0 : public RegisterBit<TccrA, 4> {};
                class ComB1 : public RegisterBit<TccrA, 5> {};
                class ComA0 : public RegisterBit<TccrA, 6> {};
                class ComA1 : public RegisterBit<TccrA, 7> {};

                typedef Tccr0B TccrB;
                class Cs0  : public RegisterBit<TccrB, 0> {};
                class Cs1  : public RegisterBit<TccrB, 1> {};
                class Cs2  : public RegisterBit<TccrB, 2> {};
                class Wgm2 : public RegisterBit<TccrB, 3> {};
                class FocB : public RegisterBit<TccrB, 6> {};
                class FocA : public RegisterBit<TccrB, 7> {};

                class Cs : public RegisterBit<TccrB, 0, 3> {};

                typedef Tcnt0 Tcnt;

                class CsValue1;
                class CsValue2;
                class CsValue3;
                class CsValue4;
                class CsValue5;
                class CsValue1
                {
                    public:
                        typedef CsValue2 NextValue;
                        typedef void PrevValue;
                        enum {key = 1};
                        enum {prescaler = 1};
                };
                class CsValue2
                {
                    public:
                        typedef CsValue3 NextValue;
                        typedef CsValue1 PrevValue;
                        enum {key = 2};
                        enum {prescaler = 8};
                };
                class CsValue3
                {
                    public:
                        typedef CsValue4 NextValue;
                        typedef CsValue2 PrevValue;
                        enum {key = 3};
                        enum {prescaler = 64};
                };
                class CsValue4
                {
                    public:
                        typedef CsValue5 NextValue;
                        typedef CsValue3 PrevValue;
                        enum {key = 4};
                        enum {prescaler = 256};
                };
                class CsValue5
                {
                    public:
                        typedef void NextValue;
                        typedef CsValue4 PrevValue;
                        enum {key = 5};
                        enum {prescaler = 1024};
                };
                typedef CsValue1 CsValueFirst;
                typedef CsValue5 CsValueLast;

                typedef Ocr0A OcrA;
                typedef Ocr0B OcrB;

                typedef Timsk0 Timsk;
                class Toie  : public RegisterBit<Timsk, 0> {};
                class OcieA : public RegisterBit<Timsk, 1> {};
                class OcieB : public RegisterBit<Timsk, 2> {};

                typedef Tifr0 Tifr;
                class Tov  : public RegisterBit<Tifr, 0> {};
                class OcfA : public RegisterBit<Tifr, 1> {};
                class OcfB : public RegisterBit<Tifr, 2> {};


                // Overflow Interrupt
                template<typename Functor>
                class OverflowInterrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".timer0_ovf_vector")))
                               const uint16_t interrupt;
                };

                // CompA Interrupt
                template<typename Functor>
                class CompAInterrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".timer0_compa_vector")))
                               const uint16_t interrupt;
                };

                // CompB Interrupt
                template<typename Functor>
                class CompBInterrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".timer0_compb_vector")))
                               const uint16_t interrupt;
                };

                class OutputCompareUnitA
                {
                    public:
                        typedef ComA0 Com0;
                        typedef ComA1 Com1;
                        typedef FocA  Foc;
                        typedef OcrA  Ocr;
                        typedef OcieA Ocie;
                        typedef OcfA  Ocf;
                        template<typename T>
                        using Interrupt = CompAInterrupt<T>;
                };

                class OutputCompareUnitB
                {
                    public:
                        typedef ComB0 Com0;
                        typedef ComB1 Com1;
                        typedef FocB  Foc;
                        typedef OcrB  Ocr;
                        typedef OcieB Ocie;
                        typedef OcfB  Ocf;
                        template<typename T>
                        using Interrupt = CompBInterrupt<T>;
                };

                static inline void setCTCMode()
                {
                    RegisterVisitor::clear<Wgm2, Wgm0>();
                    RegisterVisitor::set<Wgm1>();
                }
            };

            // Timer1 Registers
            class Tccr1A : public RegisterBase<M, 0x80, Tccr1A> {};
            class Tccr1B : public RegisterBase<M, 0x81, Tccr1B> {};
            class Tccr1C : public RegisterBase<M, 0x82, Tccr1C> {};
            class Tcnt1  : public RegisterDoubleSettable<M, 0x84, Tcnt1> {};
            class Ocr1A  : public RegisterDoubleSettable<M, 0x88, Ocr1A> {};
            class Ocr1B  : public RegisterDoubleSettable<M, 0x8a, Ocr1B> {};
            class Ocr1C  : public RegisterDoubleSettable<M, 0x8c, Ocr1C> {};
            class Icr1   : public RegisterDoubleSettable<M, 0x86, Icr1> {};
            class Timsk1 : public RegisterBase<M, 0x6f, Timsk1>  {};
            class Tifr1  : public RegisterBase<M, 0x36, Tifr1>  {};

            class Timer1
            {
                public:
                constexpr static uint64_t counterBits = 16;
                constexpr static uint64_t counterResolution = 1L << counterBits;
                typedef Tccr1A TccrA;
                class Wgm0  : public RegisterBit<TccrA, 0> {};
                class Wgm1  : public RegisterBit<TccrA, 1> {};
                class ComC0 : public RegisterBit<TccrA, 2> {};
                class ComC1 : public RegisterBit<TccrA, 3> {};
                class ComB0 : public RegisterBit<TccrA, 4> {};
                class ComB1 : public RegisterBit<TccrA, 5> {};
                class ComA0 : public RegisterBit<TccrA, 6> {};
                class ComA1 : public RegisterBit<TccrA, 7> {};

                typedef Tccr1B TccrB;
                class Cs0  : public RegisterBit<TccrB, 0> {};
                class Cs1  : public RegisterBit<TccrB, 1> {};
                class Cs2  : public RegisterBit<TccrB, 2> {};
                class Wgm2 : public RegisterBit<TccrB, 3> {};
                class Wgm3 : public RegisterBit<TccrB, 4> {};
                class Ices : public RegisterBit<TccrB, 6> {};
                class Icnc : public RegisterBit<TccrB, 7> {};

                class Cs : public RegisterBit<TccrB, 0, 3> {};

                class CsValue1;
                class CsValue2;
                class CsValue3;
                class CsValue4;
                class CsValue5;
                class CsValue1
                {
                    public:
                        typedef CsValue2 NextValue;
                        typedef void PrevValue;
                        enum {key = 1};
                        enum {prescaler = 1};
                };
                class CsValue2
                {
                    public:
                        typedef CsValue3 NextValue;
                        typedef CsValue1 PrevValue;
                        enum {key = 2};
                        enum {prescaler = 8};
                };
                class CsValue3
                {
                    public:
                        typedef CsValue4 NextValue;
                        typedef CsValue2 PrevValue;
                        enum {key = 3};
                        enum {prescaler = 64};
                };
                class CsValue4
                {
                    public:
                        typedef CsValue5 NextValue;
                        typedef CsValue3 PrevValue;
                        enum {key = 4};
                        enum {prescaler = 256};
                };
                class CsValue5
                {
                    public:
                        typedef void NextValue;
                        typedef CsValue4 PrevValue;
                        enum {key = 5};
                        enum {prescaler = 1024};
                };
                typedef CsValue1 CsValueFirst;
                typedef CsValue5 CsValueLast;

                typedef Tccr1C TccrC;
                class FocC : public RegisterBit<TccrC, 5> {};
                class FocB : public RegisterBit<TccrC, 6> {};
                class FocA : public RegisterBit<TccrC, 7> {};

                typedef Tcnt1 Tcnt;

                typedef Ocr1A OcrA;
                typedef Ocr1B OcrB;
                typedef Ocr1C OcrC;

                typedef Icr1 Icr;

                typedef Timsk1 Timsk;
                class Toie  : public RegisterBit<Timsk, 0> {};
                class OcieA : public RegisterBit<Timsk, 1> {};
                class OcieB : public RegisterBit<Timsk, 2> {};
                class OcieC : public RegisterBit<Timsk, 3> {};
                class Icie  : public RegisterBit<Timsk, 5> {};

                typedef Tifr1 Tifr;
                class Tov  : public RegisterBit<Tifr, 0> {};
                class OcfA : public RegisterBit<Tifr, 1> {};
                class OcfB : public RegisterBit<Tifr, 2> {};
                class OcfC : public RegisterBit<Tifr, 3> {};
                class Icf  : public RegisterBit<Tifr, 5> {};


                // Overflow Interrupt
                template<typename Functor>
                class OverflowInterrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".timer1_ovf_vector")))
                               const uint16_t interrupt;
                };

                // CompA Interrupt
                template<typename Functor>
                class CompAInterrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".timer1_compa_vector")))
                               const uint16_t interrupt;
                };

                // CompB Interrupt
                template<typename Functor>
                class CompBInterrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".timer1_compb_vector")))
                               const uint16_t interrupt;
                };

                // CompC Interrupt
                template<typename Functor>
                class CompCInterrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".timer1_compc_vector")))
                               const uint16_t interrupt;
                };

                // Capture Interrupt
                template<typename Functor>
                class CaptInterrupt
                {
                    public:
                        enum {t = 1};
                        static __attribute__ ((used)) 
                               __attribute__ ((section (".timer1_capt_vector")))
                               const uint16_t interrupt;
                };

                class OutputCompareUnitA
                {
                    public:
                        typedef ComA0 Com0;
                        typedef ComA1 Com1;
                        typedef FocA  Foc;
                        typedef OcrA  Ocr;
                        typedef OcieA Ocie;
                        typedef OcfA  Ocf;
                        template<typename T>
                        using Interrupt = CompAInterrupt<T>;
                };

                class OutputCompareUnitB
                {
                    public:
                        typedef ComB0 Com0;
                        typedef ComB1 Com1;
                        typedef FocB  Foc;
                        typedef OcrB  Ocr;
                        typedef OcieB Ocie;
                        typedef OcfB  Ocf;
                        template<typename T>
                        using Interrupt = CompBInterrupt<T>;
                };

                class OutputCompareUnitC
                {
                    public:
                        typedef ComC0 Com0;
                        typedef ComC1 Com1;
                        typedef FocC  Foc;
                        typedef OcrC  Ocr;
                        typedef OcieC Ocie;
                        typedef OcfC  Ocf;
                        template<typename T>
                        using Interrupt = CompCInterrupt<T>;
                };

                static inline void setCTCMode()
                {
                    RegisterVisitor::clear<Wgm3, Wgm1, Wgm0>();
                    RegisterVisitor::set<Wgm2>();
                }
            };
    };
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".int0_vector")))
        const uint16_t AT90USB162Mic<ns>::Int0::Interrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".int1_vector")))
        const uint16_t AT90USB162Mic<ns>::Int1::Interrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".int2_vector")))
        const uint16_t AT90USB162Mic<ns>::Int2::Interrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".timer0_ovf_vector")))
        const uint16_t AT90USB162Mic<ns>::Timer0::OverflowInterrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".timer0_compa_vector")))
        const uint16_t AT90USB162Mic<ns>::Timer0::CompAInterrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".timer0_compb_vector")))
        const uint16_t AT90USB162Mic<ns>::Timer0::CompBInterrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".timer1_ovf_vector")))
        const uint16_t AT90USB162Mic<ns>::Timer1::OverflowInterrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".timer1_compa_vector")))
        const uint16_t AT90USB162Mic<ns>::Timer1::CompAInterrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".timer1_compb_vector")))
        const uint16_t AT90USB162Mic<ns>::Timer1::CompBInterrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".timer1_compc_vector")))
        const uint16_t AT90USB162Mic<ns>::Timer1::CompCInterrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
    template<uint64_t ns>
    template<typename Functor>
        __attribute__ ((used))
        __attribute__ ((section (".timer1_capt_vector")))
        const uint16_t AT90USB162Mic<ns>::Timer1::CaptInterrupt<Functor>::interrupt
            = (uint16_t)(&Functor::call);
}
}

extern "C" void __init();
extern "C"
__attribute__ ((section (".error_interrupt_handler")))
void __vector_default()
{
    while (true) {}
}
__attribute__ ((used)) __attribute__ ((section (".reset_vector"))) const uint16_t reset = (uint16_t)(&__init);
