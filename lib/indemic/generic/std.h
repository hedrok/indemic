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

#include <stdint.h>

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

    /**
     * Tuple. To get type of ith 
     * std::tuple_element<channel, std::tuple<T...> >::type
     */
    namespace
    {
        template<uint64_t i, typename T1, typename... T>
        class HelperGetElement
        {
            static_assert(i <= sizeof...(T), "Index is too big");
            public:
                typedef typename HelperGetElement<i - 1, T...>::type type;
        };
        template<typename T1, typename... T>
        class HelperGetElement<0, T1, T...>
        {
            public:
                typedef T1 type;
        };
    }
    template<uint64_t i, typename T>
    class tuple_element
    {
        public:
            typedef typename T::template GetElementType<i> type;
    };
    template<typename... T>
    class tuple
    {
        private:
            template<uint64_t i, typename>
            friend class tuple_element;

            template<uint64_t i>
            using GetElementType = typename HelperGetElement<i, T...>::type;
    };
}
}
