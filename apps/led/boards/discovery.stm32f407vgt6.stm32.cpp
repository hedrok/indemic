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

/* Set STM32 to 168 MHz. */
static void clock_setup(void)
{
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);

	/* Enable GPIOD clock. */
	rcc_periph_clock_enable(RCC_GPIOD);
}

static void gpio_setup(void)
{
	/* Set GPIO12-15 (in GPIO port D) to 'output push-pull'. */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
}

int main(void)
{
	int i;

	clock_setup();
	gpio_setup();

	/* Set two LEDs for wigwag effect when toggling. */
	gpio_set(GPIOD, GPIO12 | GPIO14);

	/* Blink the LEDs (PD12, PD13, PD14 and PD15) on the board. */
	while (1) {
		/* Toggle LEDs. */
		gpio_toggle(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);
		for (i = 0; i < 6000000; i++) { /* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
