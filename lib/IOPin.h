/**
 * @file IOPin.h
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

#include <micro_types.h>

/**
 * One Input/Output Pin class
 * Should implement generic pin control
 */
class IOPin
{
    public:
        /**
         * Constructor. No initialization of pin state is done
         * @param port port_t
         * @param pin pin_ind_t
         */
        IOPin(port_t port, pin_ind_t pin)
            : _port(port)
            , _pin(pin)
        {
        }

        /**
         * Read value from pin
         */
        virtual logic_t get() = 0;

        /**
         * Make pin input
         */
        virtual void makeInput() = 0;

        /**
         * Make pin output
         */
        virtual void makeOutput() = 0;

        /**
         * Sets pin to logical one.
         *
         * All preferences like setting it output pin etc
         * should be done beforehand
         */
        virtual void setHigh() = 0;

        /**
         * Sets pin value to logical zero.
         *
         * Set to output etc beforehand.
         */
        virtual void setLow() = 0;

        /**
         * Set value of output pin
         *
         * Set to output etc beforehand.
         */
        virtual void setValue(logic_t value) = 0;

    protected:
        /** Pin index of controlled pin */
        pin_ind_t _pin;
        /** Port of controlled pin */
        port_t _port;
};
