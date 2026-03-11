#pragma once

#include <concepts>

namespace BlazesRusCode
{
    //Integer type (concept)
    template<typename T>
    concept IntegerType = std::is_integral_v<T>;
    
    //Unsigned Integer type (concept)
    template<typename T>
    concept UnsignedIntegerType = std::is_unsigned_v<T>&&std::is_integral_v<T>;
    
    //Signed Integer type (concept)
    template<typename T>
    concept SignedIntegerType = std::is_signed_v<T>&&std::is_integral_v<T>;
    
#if !defined(UnsignedLike_Implimented)
    template<class T>
    concept UnsignedLike = requires(T a) {
      // construction of canonical values
      { T{0} } -> std::same_as<T>;
      { T{1} } -> std::same_as<T>;

      // bitwise ops on same width
      { a & a } -> std::same_as<T>;
      { a | a } -> std::same_as<T>;
      { a ^ a } -> std::same_as<T>;
      { ~a }    -> std::same_as<T>;
			
      { a + a } -> std::same_as<T>;
      { a - a } -> std::same_as<T>;
      { a * a } -> std::same_as<T>;
      { a / a } -> std::same_as<T>;

      // comparisons and zero checks
      { a == T{0} } -> std::convertible_to<bool>;
      { a != T{0} } -> std::convertible_to<bool>;
      { a <  a }    -> std::convertible_to<bool>;
      { a <= a }    -> std::convertible_to<bool>;
      { a >  a }    -> std::convertible_to<bool>;
      { a >= a }    -> std::convertible_to<bool>;

      // shifts with unsigned
      { a << unsigned{1} } -> std::same_as<T>;
      { a >> unsigned{1} } -> std::same_as<T>;
      { T{1} << unsigned{1} } -> std::same_as<T>;
    };
		
    template<class T>
    concept UnsignedLight = requires(T a) {
      // construction of canonical values
      { T{0} } -> std::same_as<T>;
      { T{1} } -> std::same_as<T>;
			
			//Addition and subtraction
      { a + a } -> std::same_as<T>;
      { a - a } -> std::same_as<T>;

      // comparisons and zero checks
      { a == T{0} } -> std::convertible_to<bool>;
      { a != T{0} } -> std::convertible_to<bool>;
      { a <  a }    -> std::convertible_to<bool>;
      { a <= a }    -> std::convertible_to<bool>;
      { a >  a }    -> std::convertible_to<bool>;
      { a >= a }    -> std::convertible_to<bool>;

      // shifts with unsigned
      { a << unsigned{1} } -> std::same_as<T>;
      { a >> unsigned{1} } -> std::same_as<T>;
      { T{1} << unsigned{1} } -> std::same_as<T>;
    };
		
	#define UnsignedLike_Implimented 1
#endif
		

}