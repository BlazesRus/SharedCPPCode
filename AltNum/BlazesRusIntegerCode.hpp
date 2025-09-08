#pragma once

#include "..\DLLAPI.h"

namespace BlazesRusIntegerCode
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
      BaseType base = value - 1;    // Base of the numerator; exponent will be explicit
      int den = 1;        // Denominator of the nth term
      bool posSign = true;       // Used to swap the sign of each term
      BaseType term = base;     // First term
      BaseType prev;      // Previous sum
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

}