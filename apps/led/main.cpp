/**
 * @file main.cpp
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
 *
 * main file of LED test using multi-platform library IndeMic
 */

#include <IOPin.h>

#include "Board.h"

int main()
{
    Led::Board board;
    Led::initBoard(board);
    IndeMic::IOPin *pin = board.led;

    pin->makeOutput();
    while (true)
    {
        pin->setHigh();
        for (uint32_t t = 0; t < 16L * 1024 * 1024; t++) {
        }
        pin->setLow();
        for (uint32_t t = 0; t < 16L* 1024 * 1024; t++) {
        }
    }
}
