/**
 * @file discovery.stm32f4.stm32.cpp
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

// Copied from fancyblink.c from libopencm3-examples:
// This file is added to test build system for stm32

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <indemic/stm32/STM32F4Mic.h>
#include <indemic/stm32/IOPin.h>

/* 168 MHz  10^9 / (168 * 10^6) */
typedef IndeMic::stm32::STM32F4Mic<6> M;

typedef IndeMic::IOPin<M, M::PortD, 12> LedGreen;
typedef IndeMic::IOPin<M, M::PortD, 13> LedOrange;
typedef IndeMic::IOPin<M, M::PortD, 14> LedRed;
typedef IndeMic::IOPin<M, M::PortD, 15> LedBlue;

/* Set STM32 to 168 MHz. */
static void clock_setup(void)
{
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
}

static void gpio_setup(void)
{
    LedGreen::makeOutput();
    LedOrange::makeOutput();
    LedRed::makeOutput();
    LedBlue::makeOutput();
}

int main(void)
{
	int i, v;

	clock_setup();
	gpio_setup();

    v = 1;
	while (1) {
		/* Toggle LEDs. */
        LedGreen::setValue(v);
        LedBlue::setValue(v ^ 1);
        v ^= 1;
		for (i = 0; i < 6000000; i++) { /* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
