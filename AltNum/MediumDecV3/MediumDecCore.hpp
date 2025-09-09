// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "VariantNamePreprocessors.h"
//#include "..\VirtualTableBase.hpp"//Virtual Structure for the class to make sure can override virtually

#include <string>
#include <cmath>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "..\Concepts\IntegerConcept.hpp"
#include "MediumDecV3Base.hpp"

namespace BlazesRusCode
{
    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// (8 bytes worth of Variable Storage inside class for each instance)
    /// </summary>
    template<class VariantName>
    class MediumDecCore : public MediumDecV3Base<VariantName>
    {
    public:
    #pragma region DigitStorage
    #pragma endregion DigitStorage

        /// <summary>
        /// Value when IntHalf is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
				template<typename U = void, typename = std::enable_if_t<UsingIntHalfValue, U>>
        static IntHalfT const NegativeRep;

    #pragma region class_constructors
    #pragma endregion class_constructors

    #pragma region Negative_Status

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>VariantName</returns>
        VariantName operator-() const
        { VariantName self = *this; self.SwapNegativeStatus(); return self; }

    #pragma endregion Negative_Status


    #pragma region Check_if_value
    #pragma endregion Check_if_value

    #pragma region RangeLimits
    #pragma endregion RangeLimits

    #pragma region ValueSetters
protected://Work around for not allowing to use incomplete class statics during forming of class
        static const unsigned int LN10Div_DecSection = 434294482;
        static const unsigned int TwiceLN10Div_DecSection = 868588964;

public:
    #pragma endregion ValueSetters

    #pragma region ValueDefines
    #pragma endregion ValueDefines

    #pragma region String Commands
public:

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

    #if defined(AltNum_EnableFloatingConversion)
		
    #endif

    #if defined(AltNum_EnableFloatingConversion)

    #endif

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #if defined(AltNum_EnableFloatingConversion)

    #endif

    #if defined(AltNum_EnableFloatingConversion)

    #endif

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
protected:

public:
    #pragma endregion Comparison Operators





    #pragma region Other Division Operations

    #pragma endregion Other Division Operations



    #pragma region Other multiplication operations

    #pragma endregion Other multiplication operations





public:


    #pragma region Other addition operations

    #pragma endregion Other addition operations

    #pragma region Other subtraction operations

    #pragma endregion Other subtraction operations

    #pragma region Modulus Operations
    protected:

    public:

        void UIntModulusOp(const unsigned int& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf.Value %= rValue;
            else {
                auto divRes = DivideByUIntV1(rValue);
                UnsignedSubOp(divRes.MultiplyByUInt(rValue));
            }
        }

        void IntModulusOp(const signed int& rValue)
        {
            if (rValue<0) {
                SwapNegativeStatus();
                UnsignedIntModulusOp(-rValue);
            }
            else
                UIntModulusOp(rValue);
        }

        void UInt64ModulusOp(const unsigned __int64& rValue)
        {
            if(DecimalHalf.Value==0){
                unsigned __int64 result = IntHalf.Value;
                result %= rValue;
                IntHalf.Value = (unsigned int) result;
            } else {
                auto divRes = DivideByUIntV1(rValue);
                UnsignedSubOp(divRes.MultiplyByUInt(rValue));
            }
        }

        void Int64ModulusOp(const signed __int64& rValue)
        {
            if (rValue<0) {
                SwapNegativeStatus();
                UnsignedIntModulusOp(-rValue);
            }
            else
                UInt64ModulusOp(rValue);
        }

    #pragma region Modulus Operations

    #pragma region Bitwise Operations
    
		//Left or Right Shift
    template<UnsignedIntegerType IntType=unsigned int, bool ShiftLeft>// true = left, false = right
    VariantName& UIntShiftCore(const IntType& rhs) {
      if (rhs==0) return *this;
      auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
			if constexpr (ShiftLeft) {
        combined <<= shift;
			} else {
        combined >>= shift;
			}
      // Re-split
      IntHalf.Value     = static_cast<IntHalfValueT>(combined / DecimalOverflow);
      DecimalHalf.Value = static_cast<DecimalHalfValueT>(combined % DecimalOverflow);
      if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
        IntHalf.Sign = PositiveSign;
      return *this;
		}

    //Left or Right Shift(with partial shifts between DecimalHalf to next IntHalf)
    template<bool ShiftLeft,           // true = left, false = right
        bool ReverseIfNegative = true>// false = flip sign, true = reverse direction
    VariantName& ShiftCore(const VariantName& rhs) {
        if (rhs.IsZero()) return *this;

        const bool rhsNeg  = !rhs.IsPositive();
        auto       whole   = rhs.IntHalf.Value;
        auto       fracVal = rhs.DecimalHalf.Value;

        bool effectiveLeft = ShiftLeft;

        if (rhsNeg) {
            if constexpr (ReverseIfNegative) {
                // Reverse initial direction if negative
                effectiveLeft = !effectiveLeft;
            } else {
                // Default policy: flip sign after magnitude change
                SwapNegativeStatus();
            }
        }

        // Whole‑part shift
        if (effectiveLeft) {
            *this <<= whole;
        } else {
            *this >>= whole;
        }

        // Fractional‑part shift (if any)
        if (fracVal != 0) {
            boost::rational<unsigned int> frac(fracVal, VariantNameVariant::DecimalOverflow);
            if (effectiveLeft) {
                *this *= VariantName::Two.FractionalPowV1(frac);
            } else {
                *this /= VariantName::Two.FractionalPowV1(frac);
            }
        }
        return *this;
    }

		//Xor/Or/And/Nor operation
		template<unsigned OpType, UnsignedIntegerType IntType = unsigned int> // 0 = Xor, 1 = Or, 2 = And, 3 = Nor
		VariantName& UIntBinaryCore(const IntType& rhs) {
				if (rhs == 0) return *this;

				uint64_t combined = 0;
				if constexpr (SplitFieldsMode) {
						if constexpr (UsingIntHalfValue) {
								combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
						} else {
								combined = static_cast<uint64_t>(IntHalf) * DecimalOverflow + DecimalHalf;
						}
				} else {
						combined = Value();
				}

				if constexpr (OpType == 0) {
						combined ^= rhs;
				} else if constexpr (OpType == 1) {
						combined |= rhs;
				} else if constexpr (OpType == 2) {
						combined &= rhs;
				} else {
						combined = ~(combined | rhs); // Nor
				}

				if (combined == 0) {
						SetAsZero();
				} else if constexpr (SplitFieldsMode) {
						if constexpr (UsingIntHalfValue) {
								IntHalf.Value     = static_cast<IntHalfValueT>(combined / DecimalOverflow);
								DecimalHalf.Value = static_cast<DecimalHalfValueT>(combined % DecimalOverflow);
						} else {
								IntHalf     = static_cast<IntHalfT>(combined / DecimalOverflow);
								DecimalHalf = static_cast<DecimalHalfT>(combined % DecimalOverflow);
						}
				} else {
						Value() = combined;
				}

				return *this;
		}

    #pragma endregion Bitwise Operations

    #pragma region Other Operators

    #pragma endregion Other Operators

    #pragma region Truncation Functions

protected:

public:

protected:

public:

    #pragma endregion Truncation Functions

    #pragma region Pow and Sqrt Functions
protected:

        /// <summary>
        /// Perform square root on this instance.
        /// (Code other than switch statement adjusted from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static VariantType UnsignedSqrtV1(const VariantName& value, const int& precision=7)
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
        static VariantType Sqrt(VariantName value,const unsigned int& precision=7)
        {
            if(value.IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
            return UnsignedSqrt(value, precision);
        }

public:

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static VariantName Sqrt(const auto& value, const int& precision = 7);

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        VariantName SqrtOf(const int& precision = 7) const;

protected:
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        template<UnsignedIntegerType IntType=unsigned int>
        static VariantName UIntPowV1(const VariantName& tValue, const IntType expValue)
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
        template<VariantNameVariant VariantType=VariantName, IntegerType IntType=signed int>
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

        template<VariantNameVariant VariantType=VariantName>
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
        static VariantName UIntPow(const VariantName& tValue, const unsigned int& expValue)
        { return IntPowV1<VariantName>(tValue, expValue); }
        static VariantName UInt64Pow(const VariantName& tValue, const unsigned __int64& expValue)
        { return UIntPowV1<VariantName>(tValue, expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        static VariantName IntPow(const VariantName& tValue, const signed int& expValue)
        { return IntPowV1<VariantName>(tValue, expValue); }
        static VariantName Int64Pow(const VariantName& tValue, const signed __int64& expValue)
        { return IntPowV1<VariantName>(tValue, expValue); }

        VariantName UnsignedNegIntPower(const VariantName& tValue, const unsigned int& expValue)
        { return UnsignedNegIntPowerV1<VariantName>(tValue, expValue); }


        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        VariantName UIntPowOf(const unsigned int& expValue) const
        { return UIntPow(*this, expValue); }
        VariantName UInt64PowOf(const unsigned __int64& expValue) const
        { return UInt64Pow(*this, expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        VariantName IntPowOf(const signed int& expValue) const
        { return IntPow(*this, expValue); }
        VariantName Int64PowOf(const signed __int64& expValue) const
        { return Int64Pow(*this, expValue); }

protected:

        template<VariantNameVariant VariantType=VariantName>
        static VariantType UnsignedMirroredIntPowV1(const VariantType& tValue, const MirroredInt& expValue)
        { return UIntPowV1<VariantType>(tValue, expValue.Value); }

        template<VariantNameVariant VariantType=VariantName>
        static VariantType MirroredIntPowV1(const VariantType& tValue, const MirroredInt& expValue)
        {
            if (expValue < 0)//Negative Pow
                return UnsignedNegIntPowerV1<VariantType>(tValue, expValue.Value);
            else
                return UIntPowV1<VariantType>(tValue, expValue.Value);
        }

public:

        static VariantName UnsignedMirroredIntPow(const VariantName& tValue, const MirroredInt& expValue)
        {   return UnsignedMirroredIntPowV1<VariantName>(tValue, expValue); }

        static VariantName MirroredIntPow(const VariantName& tValue, const MirroredInt& expValue)
        {   return MirroredIntPowV1<VariantName>(tValue, expValue); }

protected:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        template<VariantNameVariant VariantType=VariantName>
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
        template<VariantNameVariant VariantType=VariantName>
        static VariantType NthRootV1(const VariantType& tValue, const unsigned int& n, const VariantType& precision)
        {
            if (tValue.IsNegative())
                throw "Nth root of a negative number requires imaginary number support";
            return UnsignedNthRootV1(tValue, n, precision);
        }

        template<VariantNameVariant VariantType=VariantName>
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
        template<VariantNameVariant VariantType=VariantName>
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

        static VariantName UnsignedNthRoot(const VariantName& tValue, const unsigned int& n, const VariantName& precision = VariantName::JustAboveZero);

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="tValue">The target value(radicand) to perform operation on.</param>
        /// <param name="nValue">The nth root degree value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        static VariantName NthRoot(const VariantName& tValue, const unsigned int& n, const VariantName& precision = VariantName::JustAboveZero)
        {
            return NthRootV1<VariantName>(tValue, n, precision);
        }

        VariantName NthRootOf(const unsigned int& n, const VariantName& precision = VariantName::JustAboveZero) const;

        static VariantName AlternativeNthRoot(const VariantName& tValue, const unsigned int& n, const VariantName& precision = VariantName::FiveBillionth);

protected:

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType FractionalPowV1(const auto& value, const boost::rational<unsigned int>& Frac)
        {
            VariantType targetVal = UIntPowOf(Frac.numerator());
            VariantType CalcVal = VariantNameVariant::NthRoot(targetVal, Frac.denominator());
            return CalcVal;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        template<VariantNameVariant VariantType=VariantName>
        const VariantType FractionalPowV2(const VariantType& value, const signed int& expNum, const unsigned int& expDenom)
        {
            auto targetVal = IntPowOf(expNum);
            auto CalcVal = VariantNameVariant::NthRoot(targetVal, expDenom);
            return CalcVal;
        }

        /// <summary>
        /// Calculate to power of unsigned expValue
        /// (Doesn't modify owner object)
        /// </summary>
        template<VariantNameVariant VariantType=VariantName>
        const VariantType UnsignedPowOfV1(const auto& expValue)
        {
            boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, VariantNameVariant::DecimalOverflow);
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
        template<VariantNameVariant VariantType=VariantName>
        const VariantType PowOfV1(const auto& expValue)
        {
            boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, VariantNameVariant::DecimalOverflow);
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
        VariantName UnsignedPowOf(const auto& expValue)
        { return UnsignedPowOfV1(expValue); }

        /// <summary>
        /// Calculate to power of expValue
        /// (Doesn't modify owner object)
        /// </summary>
        VariantName PowOf(const auto& expValue)
        { return PowOfV1(expValue); }

    #pragma endregion Pow and Sqrt Functions

    #pragma region Log Functions
protected:

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>VariantType</returns>
        template<VariantNameVariant VariantType=VariantName>
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
             *      - x: power of e to evaluate
             * Returns:
             *      - approximation of e^x in VariantType precision
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
        template<VariantNameVariant VariantType=VariantName>
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
        template<VariantNameVariant VariantType=VariantName>
        const VariantType LnOfOneSection(const VariantType& threshold = VariantType::FiveBillionth) const
        {
            VariantType base = *this - 1;        // Base of the numerator; exponent will be explicit
            bool posSign = true;             // Used to swap the sign of each term
            VariantType term = base;       // First term
            VariantType prev;          // Previous sum
            VariantType result = term;     // Kick it off
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
        template<VariantNameVariant VariantType=VariantName>
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

        template<VariantNameVariant VariantType=VariantName, IntegerType IntType = unsigned int>
        static VariantType LogGreaterRangeIntPart02(const IntType& value, const VariantType& AccuracyLevel=VariantType::JustAboveZero)
        {
            VariantType tValue = VariantType(value);
            return tValue.LogGreaterRangePart02(AccuracyLevel);
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>VariantName variant</returns>
        template<VariantNameVariant VariantType = VariantName>
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
        /// <returns>VariantName variant</returns>
        template<VariantNameVariant VariantType = VariantName>
        static VariantType LnV1(const VariantType& value)
        {
            return value.LnOfV1();
        }

        /// <summary>
        /// Log Base 10 of tValue
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>VariantName</returns>
        template<VariantNameVariant VariantType = VariantName>
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
        /// <returns>VariantName</returns>
        template<VariantNameVariant VariantType=VariantName, IntegerType IntType = unsigned int>
        static VariantType Log10OfIntV1(const IntType& value)
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
        /// <returns>VariantName Variant</returns>
        template<VariantNameVariant VariantType=VariantName>
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
        template<VariantNameVariant VariantType=VariantName, IntegerType IntType = unsigned int>
        static VariantType LogOfIntV1(const VariantType& value, const IntType& baseVal)
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

public:

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>VariantName</returns>
        static VariantName Exp(const VariantName& x) { return ExpV1(x); }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>VariantName</returns>
        static VariantName Ln(const VariantName& value)
        { return LnV1(value); }

        /// <summary>
        /// Log Base 10 of tValue
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>VariantName</returns>
        static VariantName Log10(const VariantName& value)
        { return Log10V1(value); }

        /// <summary>
        /// Log Base 10 of tValue(integer value variant)
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>VariantName</returns>
        static VariantName Log10OfInt(const unsigned int& value)
        { return Log10OfIntV1(value); }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>VariantName Variant</returns>
        static VariantName Log(const VariantName& value, const VariantName& baseVal)
        { return LogV1(value, baseVal); }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>VariantName</returns>
        static VariantName LogOfInt(const VariantName& value, const unsigned int& baseVal)
        { return LogOfIntV1(value, baseVal); }

    #pragma endregion Log Functions

    #pragma region Trigonomic Functions
protected:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType SinV1(const VariantType& tValue)
        {
            VariantType SinValue = VariantType::One  / VariableConversionFunctions::Fact(1);
            unsigned int expTotal;
            bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
                expTotal = 2 * i + 1;
                if(AddToResult)
                    SinValue += VariantType::UIntPow(tValue, expTotal) / VariableConversionFunctions::Fact(expTotal);
                else
                    SinValue -= VariantType::UIntPow(tValue, expTotal) / VariableConversionFunctions::Fact(expTotal);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Cos from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType CosV1(const VariantType& tValue)
        {
            VariantType CosValue = VariantType::One / VariableConversionFunctions::Fact(0);
            unsigned int expTotal;
            bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
                expTotal = 2 * i;
                if(AddToResult)
                    CosValue += VariantType::UIntPow(tValue, expTotal) / VariableConversionFunctions::Fact(expTotal);
                else
                    CosValue -= VariantType::UIntPow(tValue, expTotal) / VariableConversionFunctions::Fact(expTotal);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType TanV1(const VariantType& tValue)
        {
            VariantType SinValue = VariantType::One  / VariableConversionFunctions::Fact(1);
            VariantType CosValue = VariantType::One / VariableConversionFunctions::Fact(0);
            unsigned int sinExp; unsigned int cosExp;
            bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
                sinExp = 2 * i + 1; cosExp = 2 * i;
                if(AddToResult){
                    SinValue += VariantType::UIntPow(tValue, sinExp) / VariableConversionFunctions::Fact(sinExp);
                    CosValue += VariantType::UIntPow(tValue, cosExp) / VariableConversionFunctions::Fact(cosExp);
                } else{
                    SinValue -= VariantType::UIntPow(tValue, sinExp) / VariableConversionFunctions::Fact(sinExp);
                    CosValue -= VariantType::UIntPow(tValue, cosExp) / VariableConversionFunctions::Fact(cosExp);
                }
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Gets Inverse Tangent from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType ATanV1(const VariantType& tValue)
        {
            VariantType SinValue = VariantType::One  / VariableConversionFunctions::Fact(1);
            VariantType CosValue = VariantType::One / VariableConversionFunctions::Fact(0);
            //Angle as Radian
            unsigned int sinExp; unsigned int cosExp;
            bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
                sinExp = 2 * i + 1; cosExp = 2 * i;
                if(AddToResult){
                    SinValue += VariantType::UIntPow(tValue, sinExp) / VariableConversionFunctions::Fact(sinExp);
                    CosValue += VariantType::UIntPow(tValue, cosExp) / VariableConversionFunctions::Fact(cosExp);
                } else{
                    SinValue -= VariantType::UIntPow(tValue, sinExp) / VariableConversionFunctions::Fact(sinExp);
                    CosValue -= VariantType::UIntPow(tValue, cosExp) / VariableConversionFunctions::Fact(cosExp);
                }
            }
            return CosValue / SinValue;
        }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType ArcTan2V1(const VariantType& y, const VariantType& x)
        {
            VariantType coeff_1 = VariantType::PiNum.DividedByFour();
            VariantType coeff_2 = coeff_1.MultiplyByUInt(3);
            VariantType abs_y = VariantType::Abs(y) + VariantType::JustAboveZero;// kludge to prevent 0/0 condition
            VariantType r;
            VariantType angle;
            if (x.IsPositive())
            {
                r = (x - abs_y) / (x + abs_y);
                angle = coeff_1 - coeff_1 * r;
            }
            else
            {
                r = (x + abs_y) / (abs_y - x);
                angle = coeff_2 - coeff_1 * r;
            }
            if (y.IsNegative())
                return -angle;// negate if in quad III or IV
            else
                return angle;
        }

        template<VariantNameVariant VariantType=VariantName>
        static VariantType NormalizeForTrig(VariantType tValue)
        {
            if (tValue.IsNegative())
            {
                if (tValue.IntHalf.Value == 0)
                {
                    tValue.IntHalf = 359; tValue.DecimalHalf = DecimalOverflow - tValue.DecimalHalf;
                }
                else
                {
                    tValue.SwapNegativeStatus();
                    tValue.IntHalf.Value %= 360;
                    tValue.IntHalf.Value = 360 - tValue.IntHalf.Value;
                    if (tValue.DecimalHalf != 0) { tValue.DecimalHalf = DecimalOverflow - tValue.DecimalHalf; }
                }
            }
            else
            {
                tValue.IntHalf.Value %= 360;
            }
            return tValue;
        }

        /// <summary>
        /// Get Sin from tValue of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType SinFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return VariantType::Zero;
                        break;
                    case 90://0.5 Pi Radians
                        return VariantType::One;
                        break;
                    case 270://1.5 Pi Radians
                        return VariantType::NegativeOne;
                        break;
                    case 30://0.1666666666 Pi Radians
                    case 150://0.833333333 Pi Radians
                        return VariantType::PointFive;
                    case 210:
                    case 330:
                        return VariantType::NegativePointFive;
                    default:
                        //Angle as Radian
                        VariantType Radius = PiNum * lValue / 180;
                        return VariantName::SinV1<VariantType>(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = PiNum * lValue / 180;
                return VariantName::SinV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from tValue of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType CosFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf.Value)
                {
                    case 0:
                        return VariantType::One;
                        break;
                    case 60:
                        return VariantType::PointFive;
                        break;
                    case 90://0.5 Pi Radians
                    case 270://1.5 Pi Radians
                        return VariantType::Zero;
                        break;
                    case 180://Pi Radians
                        return VariantType::NegativeOne;
                        break;
                    case 120:
                    case 240:
                        return VariantType::NegativePointFive;
                    default:
                        //Angle as Radian
                        VariantType Radius = PiNum * lValue / 180;
                        return VariantName::CosV1<VariantType>(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = PiNum * lValue / 180;
                return VariantName::CosV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from tValue in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<VariantNameVariant VariantType=VariantName>
        static VariantType TanFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return VariantType::Zero;
                        break;
                    case 90://0.5 Pi Radians
                        return VariantType::Maximum;//Positive Infinity
                        break;
                    case 270://1.5 Pi Radians
                        return VariantType::Minimum;//Negative Infinity
                        break;
                    default:
                        return VariantName::TanV1<VariantType>(PiNum * lValue / 180);
                        break;
                }
            }
            else
                return VariantName::TanV1<VariantType>(PiNum * lValue / 180);
        }

public:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        static VariantName Sin(const VariantName& tValue)
        { return SinV1<VariantName>(tValue); }

        /// <summary>
        /// Get cosine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static VariantName Cos(const VariantName& tValue)
        { return CosV1<VariantName>(tValue); }

        /// <summary>
        /// Get Tan from value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static VariantName Tan(const VariantName& tValue)
        { return TanV1<VariantName>(tValue); }

        /// <summary>
        /// Gets Inverse Tangent from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static VariantName ATan(const VariantName& tValue)
        { return ATanV1<VariantName>(tValue); }

        /// <summary>
        /// Get Sin from value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static VariantName SinFromAngle(const VariantName& tValue)
        { return SinFromAngleV1<VariantName>(tValue); }

        /// <summary>
        /// Get Cos() from value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static VariantName CosFromAngle(const VariantName& tValue)
        { return CosFromAngleV1<VariantName>(tValue); }

        /// <summary>
        /// Get Tangent from value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static VariantName TanFromAngle(const VariantName& tValue)
        { return TanFromAngleV1<VariantName>(tValue); }

        VariantName SinOf()
        { return Sin(*this); }

        VariantName CosOf()
        { return Cos(*this); }

        VariantName TanOf()
        { return Tan(*this); }

        VariantName ATanOf()
        { return ATan(*this);; }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        static VariantName ArcTan2(const VariantName& y, const VariantName& x)
        { return ArcTan2V1<VariantName>(y, x); }

    #pragma endregion Trigonomic Functions
    };

}


