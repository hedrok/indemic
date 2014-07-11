/**
 * @file std.h
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

namespace IndeMic
{
/**
 * Implementation of some standard utilities
 */
namespace std
{
    /**
     * Check if types are same
     */
    template<typename A, typename B>
    class is_same
    {
        public:
            enum {value = 0};
    };
    template<typename T>
    class is_same<T, T>
    {
        public:
            enum {value = 1};
    };
}
}
