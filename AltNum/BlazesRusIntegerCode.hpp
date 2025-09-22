#pragma once

#include "..\DLLAPI.h"
#include "..\Concepts\IntegerConcept.hpp"

namespace BlazesRusCode
{
class IntegerCode
{
  //Adjusted version based on MediumDec version which is based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
  template<typename BaseType, typename IntType>
  constexpr BaseType UIntPow(BaseType base, IntType exp) noexcept {
    BaseType result = FP(1);
    while (exp > 0) {
      if (exp & 1) result *= base;
      exp >>= 1;
      base *= base;
    }
    return result;
  }

  //Adjusted version based on MediumDec version
  template<typename BaseType, typename IntType>
  constexpr BaseType IntPow(BaseType base, IntType exp) noexcept {
    if (exp < 0) return BaseType(1) / UIntPowFP(base, -exp);
    return UIntPowFP(base, exp);
  }

  /// <summary>
  /// Natural log (Equivalent to Log_E(value))
  /// </summary>
  /// <param name="value">The target value.</param>
  /// <returns>BaseType</returns>
  template<typename BaseType>
  constexpr BaseType UnsignedLog_E(BaseType value, BaseType threshold)
  {
    if (value == BaseType(1))
      return BaseType(0);
    if(value<BaseType(1))
    {
      BaseType W = (value - 1) / (value + 1);//(-0.5)/(1.5)=-1/3
      BaseType TotalRes = W;
      W *= -1;
      BaseType LastPow = W;
      BaseType WSquared = W * W;
      BaseType AddRes;
      int WPow = 3;
      do
      {
      LastPow *= WSquared;
      AddRes = LastPow / WPow;
      TotalRes -= AddRes;
      WPow += 2;
      } while (AddRes > threshold);//Total Result should be -0.346573590279972654708616060729088284037750067180127627060340004746696810984847357802931663498209344
      return TotalRes * 2;//Should result in -0.693147180559
    }
    else if (value < BaseType(2))//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
    {
      BaseType base = value - 1;  // Base of the numerator; exponent will be explicit
      int den = 1;    // Denominator of the nth term
      bool posSign = true;     // Used to swap the sign of each term
      BaseType term = base;   // First term
      BaseType prev;    // Previous sum
      BaseType result = term;   // Kick it off

      do
      {
      den++;
      posSign = !posSign;
      term *= base;
      prev = result;
      if (posSign)
        result += term / den;
      else
        result -= term / den;
      } while (abs(prev - result) > threshold);

      return result;
    }
    else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
    {//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
      //BaseType W = (value - 1) / (value + 1);
      BaseType TotalRes = (value - 1) / (value + 1);//W;
      BaseType LastPow = TotalRes;
      BaseType WSquared = TotalRes * TotalRes;
      BaseType AddRes;
      int WPow = 3;
      do
      {
      LastPow *= WSquared;
      AddRes = LastPow / WPow;//BaseType::PowRef(W, WPow) / WPow;
      TotalRes += AddRes; WPow += 2;
      } while (AddRes > threshold);
      return TotalRes * 2;
    }
  }

	template<UnsignedInteger T>
	inline constexpr bool is_power_of_two(T n) noexcept {
			return n && ((n & (n - 1)) == 0);
	}

	template<UnsignedInteger T>
	inline constexpr bool is_power_of_10(T n) noexcept {
			while (n > 1) {
					if (n % 10 != 0) return false;
					n /= 10;
			}
			return n == 1;
	}

  template<UnsignedInteger T, bool ForceCheck = false>
  inline constexpr unsigned Log2ofPowerOfTwo(T n) noexcept {
    if constexpr (ForceCheck) {
      static_assert((n & (n - 1)) == 0, "Value must be a power of two");
      assert((n & (n - 1)) == 0 && "Value must be a power of two");
    }
    unsigned result = 0;
    while (n > 1) { n >>= 1; ++result; }
    return result;
  }

  template<UnsignedInteger T, bool ForceCheck = false>
  constexpr unsigned Log10ofPowerOf10(T n) noexcept {
    if constexpr (ForceCheck) {
      auto tmp = n;
      while (tmp > 1) {
        static_assert(tmp % 10 == 0, "Value must be a power of 10");
        assert(tmp % 10 == 0 && "Value must be a power of 10");
        tmp /= 10;
      }
    }
    unsigned result = 0;
    while (n > 1) { n /= 10; ++result; }
    return result;
  }

  inline constexpr bool is_power_of_3(unsigned n) noexcept {
    while (n > 1) {
      if (n % 3 != 0) return false;
      n /= 3;
    }
    return true;
  }
  
  inline constexpr bool is_power_of_5(unsigned n) noexcept {
    while (n > 1) {
      if (n % 5 != 0) return false;
      n /= 5;
    }
    return true;
  }
  
  inline constexpr bool is_power_of_7(unsigned n) noexcept {
    while (n > 1) {
      if (n % 7 != 0) return false;
      n /= 7;
    }
    return true;
  }
  
  inline constexpr bool is_power_of_15(unsigned n) noexcept {
    while (n > 1) {
      if (n % 15 != 0) return false;
      n /= 15;
    }
    return true;
  }
  
  //Detecting if equal to form 2^a·5^b
  inline constexpr bool is_mixed_radix_2_5(unsigned n) {
    if (n < 10) return false; // smallest mixed radix is 2×5 = 10
    unsigned orig = n;
    while (n % 2 == 0) n /= 2;
    while (n % 5 == 0) n /= 5;
    return n == 1 && orig % 10 == 0;
  }
  
  struct MixedRadix2_5 {
    unsigned pow2;    // exponent of 2
    unsigned pow5;    // exponent of 5
    unsigned remainder; // leftover factor after removing 2s and 5s

    constexpr bool isPure() const noexcept { return remainder == 1; }
    constexpr bool isMixed() const noexcept { return pow2 > 0 && pow5 > 0 && remainder == 1; }
  };

  constexpr MixedRadix2_5 extract_mixed_radix_2_5(unsigned n) noexcept {
    MixedRadix2_5 result{0, 0, n};
    while ((result.remainder % 2) == 0) { result.remainder >>= 1; ++result.pow2; }
    while ((result.remainder % 5) == 0) { result.remainder /= 5; ++result.pow5; }
    return result;
  }
};
}