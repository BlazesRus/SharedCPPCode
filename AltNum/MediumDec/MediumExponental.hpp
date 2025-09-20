// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecPreprocessors.h"

#include <cmath>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)
#include "..\Concepts\IntegerConcept.hpp"
#include "MediumDec.hpp"

namespace BlazesRusCode
{
	class DLL_API MediumDecExponental
	{
  #pragma region Pow and Sqrt Functions
protected:

    /// <summary>
    /// Perform square root on this instance.
    /// (Code other than switch statement adjusted from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    template<typename VariantType>
    static VariantType UnsignedSqrtV1(VariantType value, const int& precision=7)
    {
      if (value.DecimalHalf == 0)
      {
        bool AutoSetValue = true;
        switch (value.IntHalf.Value)
        {
        case 1: value.IntHalf = 1; break;
        case 4: value.IntHalf = 2; break;
        case 9: value.IntHalf = 3; break;
        case 16: value.IntHalf = 4; break;
        case 25: value.IntHalf = 5; break;
        case 36: value.IntHalf = 6; break;
        case 49: value.IntHalf = 7; break;
        case 64: value.IntHalf = 8; break;
        case 81: value.IntHalf = 9; break;
        case 100: value.IntHalf = 10; break;
        case 121: value.IntHalf = 11; break;
        case 144: value.IntHalf = 12; break;
        case 169: value.IntHalf = 13; break;
        case 196: value.IntHalf = 14; break;
        case 225: value.IntHalf = 15; break;
        case 256: value.IntHalf = 16; break;
        case 289: value.IntHalf = 17; break;
        case 324: value.IntHalf = 18; break;
        case 361: value.IntHalf = 19; break;
        case 400: value.IntHalf = 20; break;
        case 1600: value.IntHalf = 40; break;
        default:
          AutoSetValue = false;
          break;
        }
        if(AutoSetValue)
          return value;//Technically both positive and negative numbers of same equal the result
      }

      VariantType start = VariantType::Zero, end = value;
      VariantType mid;

      // variable to store the answer
      VariantType ans;

      // for computing integral part
      // of square root of number
      while (start <= end) {
        mid = (start + end);
        mid.DivideByTwo();
        if (mid * mid == value) {
          ans = mid;
          break;
        }

        // incrementing start if integral
        // part lies on right side of the mid
        if (mid * mid < value) {
          start = mid + 1;
          ans = mid;
        }

        // decrementing end if integral part
        // lies on the left side of the mid
        else {
          end = mid - 1;
        }
      }

      // For computing the fractional part
      // of square root up to given precision
      VariantType increment = VariantType::PointOne;//0.1
      for (int i = 0; i < precision; ++i) {
        while (ans * ans <= value) {
          ans += increment;
        }

        // loop terminates when ans * ans > number
        ans = ans - increment;
        increment = increment / 10;
      }
      return ans;
    }

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    template<typename VariantType>
    static VariantType Sqrt(VariantType value,const unsigned int& precision=7)
    {
      if(value.IsNegative())
        throw "Can't display result of negative square root without imaginary number support";
      return UnsignedSqrtV1(value, precision);
    }

    /// <summary>
    /// Applies Power of operation (for unsigned integer exponents)
    /// </summary>
    /// <param name="tValue">The target value to perform power of operation.</param>
    /// <param name="expValue">The exponent value.</param>
    template<typename VariantType, IntegerType IntType=unsigned int>
    static VariantType UIntPow(const VariantType& tValue, const IntType& expValue)
    {
      if (expValue == 1)
        return tValue;
      else if (expValue == 0)
        return VariantType::One;
      else if (tValue.DecimalHalf == 0 && tValue.IntHalf.Value == 10)
      {
        VariantType result = tValue;
        if(tValue.IsNegative()&&(expValue&1)==1)
          result.IntHalf.Sign = MirroredInt::PositiveSign;
        result.IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
        return result;
      }
      else
      {
        IntType exp = expValue;
        //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
        bool IsNegative = tValue.IsPositive()?false:(exp&1)==1?false:true;
        VariantType self = tValue.AbsOf();
        VariantType result = VariantType::One;
        while (exp > 0)
        {
          // If expValue is odd, multiply self with result
          if ((exp&1) == 1)
            result.UnsignedMultOp(self);
          // n must be even now
          exp = exp >> 1; // y = y/2
          self.UnsignedMultOp(self); // Change x to x^2
        }
        if(IsNegative)
          result.IntHalf.Sign = MirroredInt::NegativeSign;
        return result;
      }
    }

    /// <summary>
    /// Applies Power of operation (for signed integer exponents)
    /// </summary>
    /// <param name="tValue">The target value to perform power of operation.</param>
    /// <param name="expValue">The exponent value.</param>
    template<typename VariantType, IntegerType IntType=signed int>
    static VariantType IntPowV1(const VariantType& tValue, const IntType& expValue)
    {
      if (expValue < 0)//Negative Pow
      {
        IntType exp = expValue * -1;
        if (tValue.DecimalHalf == 0 && tValue.IntHalf == 10 && expValue >= -9)
        {
          VariantType result = VariantType(0, DecimalOverflow / VariableConversionFunctions::PowerOfTens[exp]);
          if(tValue.IsNegative()&&(exp&1)==1)
            result.IntHalf.Sign = MirroredInt::PositiveSign;
          return result;
        }
        else
        {
          //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
          //Switches from negative to positive if exp is odd number
          bool IsNegative = tValue.IsPositive()?false:(exp&1)==1?false:true;
          VariantType self = tValue.AbsOf();//Prevent needing to flip the sign
          VariantType result = VariantType::One;
          while (exp > 0)
          {
            // If expValue is odd, multiply self with result
            if ((exp & 1) == 1)
              result.UnsignedDivOp(self);
            // n must be even now
            exp = exp >> 1; // y = y/2
            self.UnsignedMultOp(self); //  Change x to x^2
          }
          if(IsNegative)
            result.IntHalf.Sign = MirroredInt::NegativeSign;
          return result;
        }
      }
      else
        return UIntPowV1(tValue, expValue);
    }

    template<typename VariantType>
    static VariantType UnsignedNegIntPowerV1(const VariantType& tValue, const unsigned int& expValue)
    {
      unsigned int exp = expValue;
      //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
      //Switches from negative to positive if exp is odd number
      bool IsNegative = tValue.IsPositive()?false:(exp&1)==1?false:true;
      VariantType self = tValue.AbsOf();
      VariantType result = VariantType::One;
      while (exp > 0)
      {
        // If expValue is odd, divide self with result
        if ((exp & 1) == 1)
          result.UnsignedDivOp(self);
        // n must be even now
        exp = exp >> 1; // y = y/2
        self.UnsignedMultOp(self); // Change x to x^2
      }
      if(IsNegative)
        result.IntHalf.Sign = MirroredInt::NegativeSign;
      return result;
    }

public:

    /// <summary>
    /// Applies Power of operation (for unsigned integer exponents)
    /// </summary>
    /// <param name="tValue">The target value to perform power of operation.</param>
    /// <param name="expValue">The exponent value.</param>
    static MediumDec UIntPow(const MediumDec& tValue, const unsigned int& expValue)
    { return IntPowV1<MediumDec>(tValue, expValue); }
    static MediumDec UInt64Pow(const MediumDec& tValue, const unsigned __int64& expValue)
    { return UIntPowV1<MediumDec>(tValue, expValue); }

    /// <summary>
    /// Applies Power of operation (for signed integer exponents)
    /// </summary>
    /// <param name="tValue">The target value to perform power of operation.</param>
    /// <param name="expValue">The exponent value.</param>
    static MediumDec IntPow(const MediumDec& tValue, const signed int& expValue)
    { return IntPowV1<MediumDec>(tValue, expValue); }
    static MediumDec Int64Pow(const MediumDec& tValue, const signed __int64& expValue)
    { return IntPowV1<MediumDec>(tValue, expValue); }

    MediumDec UnsignedNegIntPower(const MediumDec& tValue, const unsigned int& expValue)
    { return UnsignedNegIntPowerV1<MediumDec>(tValue, expValue); }


    /// <summary>
    /// Applies Power of operation (for unsigned integer exponents)
    /// </summary>
    /// <param name="expValue">The exponent value.</param>
    MediumDec UIntPowOf(const unsigned int& expValue) const
    { return UIntPow(*this, expValue); }
    MediumDec UInt64PowOf(const unsigned __int64& expValue) const
    { return UInt64Pow(*this, expValue); }

    /// <summary>
    /// Applies Power of operation (for signed integer exponents)
    /// </summary>
    /// <param name="expValue">The exponent value.</param>
    MediumDec IntPowOf(const signed int& expValue) const
    { return IntPow(*this, expValue); }
    MediumDec Int64PowOf(const signed __int64& expValue) const
    { return Int64Pow(*this, expValue); }

protected:

    template<typename VariantType>
    static VariantType UnsignedMirroredIntPowV1(const VariantType& tValue, const MirroredInt& expValue)
    { return UIntPowV1<VariantType>(tValue, expValue.Value); }

    template<typename VariantType>
    static VariantType MirroredIntPowV1(const VariantType& tValue, const MirroredInt& expValue)
    {
      if (expValue < 0)//Negative Pow
        return UnsignedNegIntPowerV1<VariantType>(tValue, expValue.Value);
      else
        return UIntPowV1<VariantType>(tValue, expValue.Value);
    }

public:

    static MediumDec UnsignedMirroredIntPow(const MediumDec& tValue, const MirroredInt& expValue)
    {   return UnsignedMirroredIntPowV1<MediumDec>(tValue, expValue); }

    static MediumDec MirroredIntPow(const MediumDec& tValue, const MirroredInt& expValue)
    {   return MirroredIntPowV1<MediumDec>(tValue, expValue); }

protected:

    /// <summary>
    /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
    /// </summary>
    template<typename VariantType>
    static VariantType UnsignedNthRootV1(const VariantType& tValue, const unsigned int& n, const VariantType& precision)
    {
      VariantType xPre = IntHalf.Value==0 ? tValue / n: ((tValue - 1) / n) + 1;;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
      int nMinus1 = n - 1;

      // initializing difference between two
      // roots by INT_MAX
      VariantType delX = VariantType(2147483647);

      //  xK denotes current value of x
      VariantType xK;

      //  loop until we reach desired accuracy
      do
      {
        //  calculating current value from previous
        // value by newton's method

        xK = xPre * nMinus1;
        xK += UnsignedDivisionV1(tValue, UIntPowV1(xPre, nMinus1));
        xK /= n;
        delX = VariantType::Abs(xK - xPre);
        xPre = xK;
      } while (delX > precision);
      return xK;
    }

    /// <summary>
    /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
    /// </summary>
    /// <param name="tValue">The target value(radicand) to perform operation on.</param>
    /// <param name="nValue">The nth root degree value.</param>
    /// <param name="precision">Precision level (smaller = more precise)</param>
    template<typename VariantType>
    static VariantType NthRootV1(const VariantType& tValue, const unsigned int& n, const VariantType& precision)
    {
      if (tValue.IsNegative())
        throw "Nth root of a negative number requires imaginary number support";
      return UnsignedNthRootV1(tValue, n, precision);
    }

    template<typename VariantType>
    static VariantType MirroredIntRootV1(const VariantType& tValue, const MirroredInt& n, const VariantType& precision)
    {
      if(n.IsNegative())
      {
        switch(n.Value)
        {
          case 2:
            return VariantType::One/NthRootV1(tValue, 2, precision); break;
          case 1:
            return VariantType::One/tValue; break;
          default:
            throw "Negative nth root of n less than 2 requires complex numbers to support result.";
            break;
        }
      }
      else
        NthRootV1(tValue, n.Value, precision);
    }

    /// <summary>
    /// Get the (n)th Root
    /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
    /// </summary>
    /// <param name="n">The n value to apply with root.</param>
    template<typename VariantType>
    static VariantType NthRootV2(const VariantType& tValue, const unsigned int& n, const VariantType& Precision = VariantType::FiveBillionth)
    {
      if(n==0)
        throw "Can't return results of zeroth root";//Negative roots require imaginary numbers to support
      unsigned int nMinus1 = n - 1;
      VariantType OneByN = VariantType::One/n;
      VariantType InitialX1 = tValue - tValue/n;//One/n * tValue * (n- 1) == tValue/n * (n - 1) == tValue - tValue/n
      InitialX1 += UnsignedDivisionV1(tValue, tValue.UIntPowOf(nMinus1));
      VariantType x[2] = { InitialX1, tValue };
      while (Abs(x[0] - x[1]) > Precision)
      {
        x[1] = x[0];
        x[0] = OneByN * ((x[1]*nMinus1) + UnsignedDivisionV1(tValue, x[1].UIntPowOf(nMinus1)));
      }
      return x[0];
    }

public:

    static MediumDec UnsignedNthRoot(const MediumDec& tValue, const unsigned int& n, const MediumDec& precision = MediumDec::JustAboveZero);

    /// <summary>
    /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
    /// </summary>
    /// <param name="tValue">The target value(radicand) to perform operation on.</param>
    /// <param name="nValue">The nth root degree value.</param>
    /// <param name="precision">Precision level (smaller = more precise)</param>
    static MediumDec NthRoot(const MediumDec& tValue, const unsigned int& n, const MediumDec& precision = MediumDec::JustAboveZero)
    {
      return NthRootV1<MediumDec>(tValue, n, precision);
    }

    MediumDec NthRootOf(const unsigned int& n, const MediumDec& precision = MediumDec::JustAboveZero) const;

    static MediumDec AlternativeNthRoot(const MediumDec& tValue, const unsigned int& n, const MediumDec& precision = MediumDec::FiveBillionth);

protected:

    /// <summary>
    /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
    /// </summary>
    /// <param name="value">The target value.</param>
    /// <param name="Frac">The exponent value to raise the value to power of.</param>
    template<typename VariantType>
    static VariantType FractionalPowV1(const auto& value, const boost::rational<unsigned int>& Frac)
    {
      VariantType targetVal = UIntPowOf(Frac.numerator());
      VariantType CalcVal = MediumDecVariant::NthRoot(targetVal, Frac.denominator());
      return CalcVal;
    }

    /// <summary>
    /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
    /// </summary>
    /// <param name="value">The target value.</param>
    /// <param name="expNum">The numerator of the exponent value.</param>
    /// <param name="expDenom">The denominator of the exponent value.</param>
    template<typename VariantType>
    const VariantType FractionalPowV2(const VariantType& value, const signed int& expNum, const unsigned int& expDenom)
    {
      auto targetVal = IntPowOf(expNum);
      auto CalcVal = MediumDecVariant::NthRoot(targetVal, expDenom);
      return CalcVal;
    }

    /// <summary>
    /// Calculate to power of unsigned expValue
    /// (Doesn't modify owner object)
    /// </summary>
    template<typename VariantType>
    const VariantType UnsignedPowOfV1(const auto& expValue)
    {
      boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
      if(expValue.IntHalf.Value==0)
        return FractionalPowV1(Frac);
      else {
        VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
        CalcVal *= FractionalPowV1(Frac);
        return CalcVal;
      }
    }

    /// <summary>
    /// Calculate to power of expValue
    /// (Doesn't modify owner object)
    /// </summary>
    template<typename VariantType>
    const VariantType PowOfV1(const auto& expValue)
    {
      boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
      if (expValue.IntHalf.IsNegative()){//Negative Exponent
        if(expValue.IntHalf.Value==0)
          return VariantType::One/FractionalPowV1(Frac);
        else {
          VariantType CalcVal = One / UIntPowOf(expValue.IntHalf.Value);
          CalcVal /= FractionalPowV1(Frac);
          return CalcVal;
        }
      } else {
        if(expValue.IntHalf.Value==0)
          return FractionalPowV1(Frac);
        else {
          VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
          CalcVal *= FractionalPowV1(Frac);
          return CalcVal;
        }
      }
    }

public:

    /// <summary>
    /// Calculate to power of expValue
    /// (Doesn't modify owner object)
    /// </summary>
    MediumDec UnsignedPowOf(const auto& expValue)
    { return UnsignedPowOfV1(expValue); }

    /// <summary>
    /// Calculate to power of expValue
    /// (Doesn't modify owner object)
    /// </summary>
    MediumDec PowOf(const auto& expValue)
    { return PowOfV1(expValue); }

  #pragma endregion Pow and Sqrt Functions

  #pragma region Log Functions
protected:

    /// <summary>
    /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
    /// </summary>
    /// <param name="x">The value to apply the exponential function to.</param>
    /// <returns>VariantType</returns>
    template<typename VariantType>
    static VariantType ExpV1(const VariantType& x)
    {
      /*
       * Evaluates f(x) = e^x for any x in the interval [-709, 709].
       * If x < -709 or x > 709, raises an assertion error. Implemented
       * using the truncated Taylor series of e^x with ceil(|x| * e) * 12
       * terms. Achieves at least 14 and at most 16 digits of precision
       * over the entire interval.
       * Performance - There are exactly 36 * ceil(|x| * e) + 5
       * operations; 69,413 in the worst case (x = 709 or -709):
       * - (12 * ceil(|x| * e)) + 2 multiplications
       * - (12 * ceil(|x| * e)) + 1 divisions
       * - (12 * ceil(|x| * e)) additions
       * - 1 rounding
       * - 1 absolute value
       * Accuracy - Over a sample of 10,000 linearly spaced points in
       * [-709, 709] we have the following error statistics:
       * - Max relative error = 8.39803e-15
       * - Min relative error = 0.0
       * - Avg relative error = 0.0
       * - Med relative error = 1.90746e-15
       * - Var relative error = 0.0
       * - 0.88 percent of the values have less than 15 digits of precision
       * Args:
       *    - x: power of e to evaluate
       * Returns:
       *    - approximation of e^x in VariantType precision
       */
       // Check that x is a valid input.
      assert(x.IntHalf.Value < 709);
      // When x = 0 we already know e^x = 1.
      if (x.IsZero()) {
        return VariantType::One;
      }
      // Normalize x to a non-negative value to take advantage of
      // reciprocal symmetry. But keep track of the original sign
      // in case we need to return the reciprocal of e^x later.
      VariantType x0 = VariantType::Abs(x);
      // First term of Taylor expansion of e^x at a = 0 is 1.
      // tn is the variable we we will return for e^x, and its
      // value at any time is the sum of all currently evaluated
      // Taylor terms thus far.
      VariantType tn = VariantType::One;
      // Chose a truncation point for the Taylor series using the
      // heuristic bound 12 * ceil(|x| e), then work down from there
      // using Horner's method.
      int n = VariantType::CeilInt(x0 * VariantType::E) * 12;
      for (int i = n; i > 0; --i) {
        tn = tn * (x0 / i) + VariantType::One;
      }
      // If the original input x is less than 0, we want the reciprocal
      // of the e^x we calculated.
      if (x.IsNegative()) {
        tn = VariantType::One / tn;
      }
      return tn;
    }

    //Common log calculations for when value is between 0 and one
    template<typename VariantType>
    VariantType LogZeroRangePart02(const VariantType& AccuracyLevel=VariantType::JustAboveZero) const
    {
      VariantType TotalRes = (*this - 1)/ (*this + 1);
      VariantType WSquared = TotalRes * TotalRes;
      VariantType LastPow = -TotalRes;
      int WPow = 3;
      VariantType AddRes;

      do
      {
        LastPow *= WSquared;
        AddRes = LastPow / WPow;
        TotalRes -= AddRes;
        WPow += 2;
      } while (AddRes > VariantType::JustAboveZero);
      return TotalRes;
    }

    //Common natural log calculations for range one to two
    template<typename VariantType>
    const VariantType LnOfOneSection(const VariantType& threshold = VariantType::FiveBillionth) const
    {
      VariantType base = *this - 1;    // Base of the numerator; exponent will be explicit
      bool posSign = true;       // Used to swap the sign of each term
      VariantType term = base;     // First term
      VariantType prev;      // Previous sum
      VariantType result = term;   // Kick it off
      // den = Denominator of the nth term
      for(unsigned int den = 2;VariantType::Abs(prev - result) > threshold;++den){
        posSign = !posSign;
        term *= base;
        prev = result;
        if (posSign)
          result += term / den;
        else
          result -= term / den;
      }
      return result;
    }

    //Common log calculations for when value is greater than one
    template<typename VariantType>
    VariantType LogGreaterRangePart02(const VariantType& AccuracyLevel=VariantType::JustAboveZero) const
    {
      //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
      VariantType TotalRes = (*this - 1) / (*this + 1);
      VariantType LastPow = TotalRes;
      VariantType WSquared = TotalRes * TotalRes;
      VariantType AddRes;
      int WPow = 3;
      do
      {
        LastPow *= WSquared;
        AddRes = LastPow / WPow;
        TotalRes += AddRes; WPow += 2;
      } while (AddRes > AccuracyLevel);
      return TotalRes;
    }

    template<typename VariantType, IntegerType IntType = unsigned int>
    static VariantType LogGreaterRangeIntPart02(const IntType& value, const VariantType& AccuracyLevel=VariantType::JustAboveZero)
    {
      VariantType tValue = VariantType(value);
      return tValue.LogGreaterRangePart02(AccuracyLevel);
    }

    /// <summary>
    /// Natural log (Equivalent to Log_E(value))
    /// </summary>
    /// <param name="value">The target value.</param>
    /// <returns>MediumDec variant</returns>
    template<typename VariantType>
    #if defined(AltNum_UseCustomLnAccuracy)
    const VariantType LnOfV1(const VariantType& threshold = VariantType::FiveMillionth) const
    #else
    const VariantType LnOfV1() const
    #endif
    {//Negative values for natural log return value of LnV1(-value) * i
      //if (value <= 0) {}else//Error if equal or less than 0
      if (IsOne())
        return VariantType::Zero;
      if (IntHalf == MirroredInt::Zero)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
      {
        #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
        return LogZeroRangePart02(threshold).MultipliedByTwo();
        #else
        return LogZeroRangePart02().MultipliedByTwo();
        #endif
      }
      else if (IntHalf == MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
      {//This section gives accurate answer(for values between 1 and 2)
        #if defined(AltNum_UseCustomLnAccuracy)
        return LnOfOneSection(threshold);
        #else
        return LnOfOneSection();
        #endif
      }
      else
      {//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
        return LogGreaterRangePart02(threshold).MultipliedByTwo();
        #else
        return LogGreaterRangePart02().MultipliedByTwo();
        #endif
      }
    }

    /// <summary>
    /// Natural log (Equivalent to Log_E(value))
    /// </summary>
    /// <param name="value">The target value.</param>
    /// <returns>MediumDec variant</returns>
    template<typename VariantType>
    static VariantType LnV1(const VariantType& value)
    {
      return value.LnOfV1();
    }

    /// <summary>
    /// Log Base 10 of tValue
    /// </summary>
    /// <param name="tValue">The value.</param>
    /// <returns>MediumDec</returns>
    template<typename VariantType>
    #if defined(AltNum_UseCustomLnAccuracy)
    static VariantType Log10V1(const VariantType& lValue, const VariantType& threshold = VariantType::FiveMillionth)
    #else
    static VariantType Log10V1(const VariantType& lValue)
    #endif
    {
      if (lValue.IsOne())
        return VariantType::Zero;
      #if !defined(AltNum_PreventLog10IntegerLoop)
      if (lValue.DecimalHalf == 0 && lValue.IntHalf.Value % 10 == 0)
      {//Might not be worth using checking to use this alternative code since since 10s aren't that common
        for (int index = 1; index < 9; ++index)
        {
          if (lValue.IntHalf.Value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
            return VariantType::InitializeV2(index, 0);
        }
        return VariantType::InitializeV2(9, 0);
      }
      #endif
      const VariantType lnMultiplier = VariantType::InitializeV2(0, LN10Div_DecSection);
      if (lValue.IntHalf == MirroredInt::Zero)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
      {
        #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
        VariantType result = lValue.LogZeroRangePart02(threshold); return result.MultiplyByUnsigned(lnMultiplier);
        #else
        VariantType result = lValue.LogZeroRangePart02(); return result.MultiplyByUnsigned(lnMultiplier);
        #endif
      }
      else if (lValue.IntHalf == MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
      {//This section gives accurate answer for values between 1 & 2
        #if defined(AltNum_UseCustomLnAccuracy)
        VariantType result = lValue.LnOfOneSection(threshold); return result.MultiplyByUnsigned(lnMultiplier);
        #else
        VariantType result = lValue.LnOfOneSection(); return result.MultiplyByUnsigned(lnMultiplier);
        #endif
      }
      else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
      {
        #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
        VariantType result = lValue.LogGreaterRangePart02(threshold); return result.MultiplyByUnsigned(lnMultiplier);
        #else
        VariantType result = lValue.LogGreaterRangePart02(); return result.MultiplyByUnsigned(lnMultiplier);
        #endif
      }
    }

    /// <summary>
    /// Log Base 10 of tValue(integer value variant)
    /// </summary>
    /// <param name="tValue">The value.</param>
    /// <returns>MediumDec</returns>
    template<typename VariantType, IntegerType IntType = unsigned int>
    static VariantType Log10OfInt(const IntType& value)
    {
      if (value == 1)
        return VariantType::Zero;
      if (value % 10 == 0)
      {
        for (int index = 1; index < 9; ++index)
        {
          if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
            return VariantType(index);
        }
        return VariantType(9);
      }
      else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
      {
        VariantType lnMultiplier = VariantType(0, TwiceLN10Div_DecSection);
        return LogGreaterRangeIntPart02(value).MultiplyByUnsigned(lnMultiplier);
      }
    }

    /// <summary>
    /// Log with Base of BaseVal of tValue
    /// Based on http://home.windstream.net/okrebs/page57.html
    /// </summary>
    /// <param name="value">The value.</param>
    /// <param name="baseVal">The base of Log</param>
    /// <returns>MediumDec Variant</returns>
    template<typename VariantType>
    static VariantType LogV1(const VariantType& value, const VariantType& baseVal)
    {
      if (value == VariantType::One)
        return VariantType::Zero;
      return Log10V1(value) / Log10V1(baseVal);
    }

    /// <summary>
    /// Log with Base of BaseVal of tValue
    /// Based on http://home.windstream.net/okrebs/page57.html
    /// </summary>
    /// <param name="tValue">The value.</param>
    /// <param name="BaseVal">The base of Log</param>
    /// <returns>VariantType</returns>
    template<typename VariantType, IntegerType IntType = unsigned int>
    static VariantType LogOfInt(const VariantType& value, const IntType& baseVal)
    {
      //Calculate Base log first
      VariantType baseTotalRes;
      bool lnMultLog = true;
      if (baseVal % 10 == 0)
      {
        for (int index = 1; index < 9; ++index)
        {
          if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
          {
            baseTotalRes = VariantType::Initialize(index, 0);
            break;
          }
        }
        baseTotalRes = VariantType(9, 0); lnMultLog = false;
      }
      else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
      {
        baseTotalRes = LogGreaterRangeIntPart02(baseVal);
      }
      VariantType lnMultiplier = VariantType(0, TwiceLN10Div_DecSection);
      //Now calculate other log
      if (value.DecimalHalf == 0 && value.IntHalf.Value % 10 == 0)
      {
        for (int index = 1; index < 9; ++index)
        {
          if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
            return lnMultLog ? VariantType::Initialize(index, 0) / (baseTotalRes * lnMultiplier): VariantType::Initialize(index, 0)/ baseTotalRes;
        }
        return lnMultLog? VariantType(9, 0) / (baseTotalRes.MultiplyByUnsigned(lnMultiplier)):VariantType::Initialize(9, 0)/baseTotalRes;
      }
      if(value.IntHalf==MirroredInt::Zero)//Not tested this block but should work
      {
        VariantType TotalRes = value.LogZeroRangePart02();
        if(lnMultLog)
          return TotalRes.DivideByUnsigned(baseTotalRes);
        else
          return (TotalRes.MultiplyByUnsigned(lnMultiplier)).DivideByUnsigned(baseTotalRes);
      }
      else if (value.IntHalf==MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
      {//This section gives accurate answer for values between 1 & 2
        if(lnMultLog)
          return value.LnOfOneSection()/baseTotalRes;
        else
          return (value.LnOfOneSection().MultipliedByTwo())/ baseTotalRes;
      }
      else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
      {
        VariantType TotalRes = value.LogGreaterRangePart02();
        if(lnMultLog)
          return TotalRes.DivideByUnsigned(baseTotalRes);
        else
          return (TotalRes.MultiplyByUnsigned(lnMultiplier)).DivideByUnsigned(baseTotalRes);
      }
    }

  #pragma endregion Log Functions
	};
}