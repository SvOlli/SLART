/*
 * src/libs/Common/Binary.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef BINARY_HPP
#define BINARY_HPP

#if __cplusplus < 201100L
#error __FILE__ needs C++11
#endif

/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/

/*!
 \brief C++11 only template for the recursion end of binary literals

*/
template <typename T>
constexpr T binary_helper()
{
   return 0;
}

/*!
 \brief C++11 only template for the recursion of binary literals

*/
template <typename T, char Head, char... Tail>
constexpr T binary_helper()
{
   static_assert( (Head == '0') || (Head == '1'), "illegal binary digit" );
   static_assert( (sizeof...(Tail)) < (sizeof(T) * 8), "binary size overflow" );
   return ( ((Head & 1) << sizeof...(Tail)) | binary_helper<T,Tail...>() );
}

/*!
 \brief C++11 only template to convert binaries in form of 1010_b64u to
 an unsigned long long

*/
template <char... Ts>
constexpr unsigned long long operator"" _b64u()
{
   return binary_helper<unsigned long long, Ts...>();
}

/*!
 \brief C++11 only template to convert binaries in form of 1010_b64 to
 a signed long long

*/
template <char... Ts>
constexpr long long operator"" _b64()
{
   return binary_helper<long long, Ts...>();
}

/*!
 \brief C++11 only template to convert binaries in form of 1010_b32u to
 an unsigned long

*/
template <char... Ts>
constexpr unsigned long operator"" _b32u()
{
   return binary_helper<unsigned long, Ts...>();
}

/*!
 \brief C++11 only template to convert binaries in form of 1010_b32 to
 a signed long

*/
template <char... Ts>
constexpr long operator"" _b32()
{
   return binary_helper<long, Ts...>();
}

/*!
 \brief C++11 only template to convert binaries in form of 1010_b16u to
 an unsigned short

*/
template <char... Ts>
constexpr unsigned short operator"" _b16u()
{
   return binary_helper<unsigned short, Ts...>();
}

/*!
 \brief C++11 only template to convert binaries in form of 1010_b16 to
 a signed short

*/
template <char... Ts>
constexpr short operator"" _b16()
{
   return binary_helper<short, Ts...>();
}

/*!
 \brief C++11 only template to convert binaries in form of 1010_b8u to
 an unsigned char

*/
template <char... Ts>
constexpr unsigned char operator"" _b8u()
{
   return binary_helper<unsigned char, Ts...>();
}

/*!
 \brief C++11 only template to convert binaries in form of 1010_b8 to
 a signed char

*/
template <char... Ts>
constexpr char operator"" _b8()
{
   return binary_helper<char, Ts...>();
}

#endif

