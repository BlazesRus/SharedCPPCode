#pragma once

#include "..\DLLAPI.h"

namespace BlazesRusIntegerCode
{
  /// <summary>
  /// Applies Power of operation to integers (for integer exponents)
  /// </summary>
  /// <param name="expValue">The exponent value.</param>
  template<IntegerType IntType=int>
  static IntegerType IntPow(IntType targetValue, IntType expValue)
  {
    if (expValue == 1) { return targetValue; }//Return self
    else if (expValue == 0)
    {
      return 1;
    }
    else if (expValue < 0)//Negative Pow
    {
      return 0;//Rounds down from factional to integer digit of zero
    }
    else
    {
      //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
      IntegerType self = targetValue;
      targetValue = 1;// Initialize result
      while (expValue > 0)
      {
        // If expValue is odd, multiply self with result
        if (expValue % 2 == 1)
          targetValue *= self;
        // n must be even now
        expValue = expValue >> 1; // y = y/2
        self = self * self; // Change x to x^2
      }
    }
  }
}