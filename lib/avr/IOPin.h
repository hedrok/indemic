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

#include <IOPin.h>

namespace IndeMic
{
namespace avr
{

/**
 * One Input/Output Pin class
 * AVR implementation
 */
class IOPin : public IndeMic::IOPin
{
    public:
        /**
         * Constructor. No initialization of pin state is done
         * @param port port_t
         * @param pin pin_ind_t
         */
        IOPin(port_t port, pin_ind_t pin)
            : IndeMic::IOPin(port, pin)
            , _port_mask(1 << pin)
        {
        }

        /**
         * Read value from pin
         */
        logic_t get() override
        {
            return 0;
        }

        /**
         * Make pin input
         */
        void makeInput() override
        {
        }

        /**
         * Make pin output
         */
        void makeOutput()
        {
        }

        /**
         * Sets pin to logical one.
         *
         * All preferences like setting it output pin etc
         * should be done beforehand
         */
        void setHigh()
        {
        }

        /**
         * Sets pin value to logical zero.
         *
         * Set to output etc beforehand.
         */
        void setLow()
        {
        }

        /**
         * Set value of output pin
         *
         * Set to output etc beforehand.
         */
        void setValue(logic_t value)
        {
        }

    protected:
        /** Port mask of controlled pin */
        port_mask_t _port_mask;
};

}
}
