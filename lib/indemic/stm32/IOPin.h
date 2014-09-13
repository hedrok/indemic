/**
 * @file IOPin.h
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

#include <libopencm3/stm32/gpio.h>

namespace IndeMic
{
namespace stm32
{

/**
 * One Input/Output Pin class
 * STM32 implementation
 */
template<typename M, typename Port, uint8_t pin>
class IOPin
{
    static constexpr typename M::port_mask_t msk = 1 << pin;
    public:
        /**
         * Read value from pin
         */
        static inline typename M::logic_t get()
        {
            return 0;
        }

        /**
         * Make pin input
         */
        static inline void makeInput()
        {
        }

        /**
         * Make pin output
         */
        static inline void makeOutput()
        {
            gpio_mode_setup(Port::base, GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE, msk);
        }

        /**
         * Sets pin to logical one.
         *
         * All preferences like setting it output pin etc
         * should be done beforehand
         */
        static inline void setHigh()
        {
		    gpio_set(Port::base, msk);
        }

        /**
         * Sets pin value to logical zero.
         *
         * Set to output etc beforehand.
         */
        static inline void setLow()
        {
		    gpio_clear(Port::base, msk);
        }

        /**
         * Set value of output pin
         *
         * Set to output etc beforehand.
         */
        static inline void setValue(typename M::logic_t value)
        {
            if (value) {
                setHigh();
            } else {
                setLow();
            }
        }
};

}
}
