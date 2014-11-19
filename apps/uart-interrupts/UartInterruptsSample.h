/**
 * @file UartInterruptsSample.h
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
 *
 * UART with interrupts sample using multi-platform library IndeMic
 */

#include <indemic/UartConfig.h>

/**
 * Uart Interrupts Sample
 *
 * a turns on LED
 * b turns off LED
 * t toggles LED
 * Any other symbol prints out help
 *
 * All transmissions are done via interrupts
 *
 * @param M Microcontroller
 * @param Led Led to show that board has power
 * @param Uart1Provider Uart template
 */
template<typename M, typename Led, template<typename> class Uart1Provider>
class UartSample
{
public:
    using Uart1Config = IndeMic::UartConfig<9600, 8, IndeMic::UartParity::off, 1>;
    using Uart1 = Uart1Provider<Uart1Config>;

    static constexpr const char * const lowStr = "Led set low";
    static constexpr const char * const highStr = "Led set high";
    static constexpr const char * const helpStr = "IndeMic UART sample, recognized commands: h (set LED high) and l(set LED low)";

    /**
     * Functor to write c-strings to output,
     * if transmittion is in progress, it is interrupted
     * and new transmission starts. Each string transmission
     * ends with \n\r
     */
    class WriteFunctor
    {
        public:
            /**
             * Interrupt function. Should be called
             * each time write operation is possible.
             */
            INDEMIC_INTERRUPT_FUNCTION
            {
                _instance.call();
            }
            /**
             * Interrupt function. Should be called
             * each time write operation is possible.
             */
            static void sendString(const char * str)
            {
                _instance._sendString(str);
            }
        private:
            /** Constructor */
            WriteFunctor()
                : _cur(0)
                , _state(State::idle)
            {
            }
            void call()
            {
                switch (_state) {
                    case State::transmitting:
                        Uart1::writeNonBlocking(*_cur++);
                        if (!*_cur) {
                            _state = State::eofAndCrLeft;
                            _cur = 0;
                        }
                        break;
                    case State::eofAndCrLeft:
                        Uart1::writeNonBlocking('\n');
                        _state = State::crLeft;
                        break;
                    case State::crLeft:
                        Uart1::writeNonBlocking('\r');
                        if (_cur) {
                            _state = State::transmitting;
                        } else {
                            _state = State::idle;
                            Uart1::disableWriteInt();
                        }
                        break;
                    case State::idle:
                    default:
                        // Shouldn't be reached theoretically
                        Uart1::disableWriteInt();
                        break;
                }
            }
            void _sendString(const char *str)
            {
                _cur = str;
                if (_state == State::transmitting) {
                    _state = State::eofAndCrLeft;
                } else if (_state == State::idle) {
                    _state = State::transmitting;
                    Uart1::enableWriteInt();
                }
            }

            static WriteFunctor _instance;

            const char * _cur;
            enum class State
            {
                idle,
                transmitting,
                eofAndCrLeft,
                crLeft,
            };
            State _state;
    };

    struct ReadFunctor
    {
        INDEMIC_INTERRUPT_FUNCTION
        {
            char c = Uart1::readNonBlocking();
            const char * str;
            switch (c) {
                case 'l':
                    Led::setLow();
                    str = lowStr;
                    break;
                case 'h':
                    Led::setHigh();
                    str = highStr;
                    break;
                default:
                    str = helpStr;
                    break;
            }
            WriteFunctor::sendString(str);
        }
    };

    static void main()
    {
        Uart1::template Interrupts<ReadFunctor, WriteFunctor>::init();
        Uart1::enableReadInt();
        Uart1::enable();
        M::enableInterrupts();
        Led::makeOutput();
        Led::setHigh();
        while (true) {
            M::sleep();
        }
    }
};

template<typename M, typename Led, template<typename> class Uart1Provider>
typename UartSample<M, Led, Uart1Provider>::WriteFunctor UartSample<M, Led, Uart1Provider>::WriteFunctor::_instance;
