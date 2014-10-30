/**
 * @file PWMSample.h
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
 * PWM Sample using multi-platform library IndeMic
 */

template<typename M, typename PWMProvider >
class PWMSample
{
public:
    static void main()
    {
        // 1/3 Hz
        using LedPWM = typename PWMProvider::template PWMTimer<3000000000>;
        LedPWM::setPeriod();
        // 2 seconds led would be on, 1 second off
        LedPWM::template Channel<0>::template setPulseWidth<2000000000>();
        LedPWM::template Channel<0>::enable();
        LedPWM::enable();
        while (true) {
            M::sleep();
        }
    }
};
