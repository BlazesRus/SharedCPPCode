// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#ifdef BlazesSharedCode_LocalLayout
#ifndef DLL_API
#ifdef UsingBlazesSharedCodeDLL
#define DLL_API __declspec(dllimport)
#elif defined(BLAZESSharedCode_LIBRARY)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif
#endif
#else
#include "..\DLLAPI.h"
#endif

#include "AltNumBase.hpp"

using RepType : AltDecBase:RepType;

namespace BlazesRusCode
{
    class AltDec;

/*---Accuracy Tests(with AltDec based settings):
 * 100% accuracy for all integer value multiplication operations.
 * 100% accuracy for addition/subtraction operations
 * Partial but still high accuracy for non-integer representation variations of multiplication and division because of truncation
   (values get lost if get too small) (100% accuracy except for some truncated digits lost)
 * Other operations like Ln and Sqrt contained with decent level of accuracy
   (still loses a little accuracy because of truncation etc)
 * Operations and functions will mess up if IntValue overflows/underflows
*/

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// plus support for some fractal operations, and other representations like Pi(and optionally things like e or imaginary numbers)
    /// (12 bytes worth of Variable Storage inside class for each instance)
    /// </summary>
    class DLL_API AltDec : public virtual AltDecBase
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        AltDec(const IntHalfType& intVal, const DecimalHalfType& decVal = 0, const MirroredInt& extraVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

        AltDec(const AltDec&) = default;

        AltDec& operator=(const int& rhs)
        {
            IntValue = rhs; DecimalHalf = 0;
            ExtraRep = 0;
            return *this;
        } const

        AltDec& operator=(const AltDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            ExtraRep = rhs.ExtraRep;
            return *this;
        } const

        constexpr auto SetAsZero = AltDecBase::SetAsZero;

    public:
    #pragma region Const Representation values
        //Code inside Derivable version of class
    #pragma endregion Const Representation values

    #pragma region RepType
        static std::string RepTypeAsString(RepType& repType)
        {
            switch(repType)
            {
                case RepType::NormalType:
                    return "NormalType"; break;
    #if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                    return "NumByDiv"; break;
    #endif
    #if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
                    return "PiNum"; break;
        #if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    return "PiPower"; break;
        #endif
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                    return "PiNumByDiv"; break;
            #else
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                    return "PiFractional"; break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableERep)
                case RepType::ENum:
                    return "ENum"; break;
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
                    return "ENumByDiv"; break;
            #else
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                    return "EFractional"; break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                    return "INum"; break;
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
                    return "INumByDiv"; break;
            #else
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    return "IFractional"; break;
            #endif
        #endif
        #ifdef AltNum_EnableComplexNumbers
                case RepType::ComplexIRep:
                    return "ComplexIRep"; break;
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                    return "MixedFrac"; break;
        #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
                    return "MixedPi"; break;
        #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
                    return "MixedE"; break;
        #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
                    return "MixedI"; break;
        #endif
    #endif

    #if defined(AltNum_EnableInfinityRep)
                case RepType::PositiveInfinity://If Positive Infinity: then convert number into MaximumValue instead when need as real number
                    return "PositiveInfinity"; break;
                case RepType::NegativeInfinity://If Negative Infinity: then convert number into MinimumValue instead when need as real number
                    return "NegativeInfinity"; break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                    return "ApproachingBottom"; break;
        #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                    return "ApproachingTop"; break;
        #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    return "ApproachingMidLeft"; break;
			#if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidRight:
                    return "ApproachingMidRight"; break;
			#endif
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                    return "Undefined"; break;
                case RepType::NaN:
                    return "NaN"; break;
    #endif
    #if defined(AltNum_EnableApproachingPi)
                case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
                    return "ApproachingTopPi"; break;
    #endif
    #if defined(AltNum_EnableApproachingE)
                case RepType::ApproachingTopE://equal to IntValue.9..9 e
                    return "ApproachingTopE"; break;
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                    return "PositiveImaginaryInfinity"; break;
                case RepType::NegativeImaginaryInfinity:
                    return "NegativeImaginaryInfinity"; break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
                    return "ApproachingImaginaryBottom"; break;
                    #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
                    return "ApproachingImaginaryTop"; break;
        #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidLeft:
                    return "ApproachingImaginaryMidLeft"; break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidRight:
                    return "ApproachingImaginaryMidRight"; break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range: ExtraRepValue==UndefinedInRangeRep)
                case RepType::UndefinedButInRange:
                    return "UndefinedButInRange"; break;
        #if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
                case RepType::WithinMinMaxRange:
                    return "WithinMinMaxRange"; break;
        #endif
    #endif
    #if defined(AltNum_EnableNil)
                case RepType::Nil:
                    return "Nil"; break;
    #endif
                default:
                    return "Unknown";
            }
        }

    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        constexpr auto SetAsMaximum = MediumDecBase::SetAsMaximum;

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        constexpr auto SetAsMinimum = MediumDecBase::SetAsMinimum;

    #pragma endregion RangeLimits

    #pragma region PiNum Setters
        //Code inside Derivable version of class
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
        //Code inside Derivable version of class
    #pragma endregion ENum Setters

    #pragma region Fractional Setters
        //Code inside Derivable version of class
    #pragma endregion Fractional Setters

    #pragma region MixedFrac Setters
        //Code inside Derivable version of class
    #pragma endregion MixedFrac Setters

    #pragma region Infinity Setters
        //Code inside Derivable version of class
    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters
        //Code inside Derivable version of class

    #pragma region NaN Setters
        //Code inside Derivable version of class
    #pragma endregion NaN Setters

    protected:
    #pragma region ValueDefines
    #if defined(AltNum_EnableNaN)
        static AltDec NaNValue()
        {
            AltDec NewSelf = AltDec(0, NaNRep);
            return NewSelf;
        }
        
        static AltDec UndefinedValue()
        {
            AltDec NewSelf = AltDec(0, UndefinedRep);
            return NewSelf;
        }
    #endif

    #if defined(AltNum_EnableInfinityRep)
        static AltDec InfinityValue()
        {
            AltDec NewSelf = AltDec(1, InfinityRep);
            return NewSelf;
        }
        
        static AltDec NegativeInfinityValue()
        {
            AltDec NewSelf = AltDec(-1, InfinityRep);
            return NewSelf;
        }
    #endif
        
    #if defined(AltNum_EnableApproachingValues)
        static AltDec ApproachingZeroValue()
        {
            AltDec NewSelf = AltDec(0, ApproachingBottomRep);
            return NewSelf;
        }
        
        static AltDec ApproachingOneFromLeftValue()
        {
            AltDec NewSelf = AltDec(0, ApproachingTopRep);
            return NewSelf;
        }
        
        static AltDec ApproachingOneFromRightValue()
        {
            AltDec NewSelf = AltDec(1, ApproachingBottomRep);
            return NewSelf;
        }

        static AltDec NegativeApproachingZeroValue()
        {
            AltDec NewSelf = AltDec(NegativeRep, ApproachingBottomRep);
            return NewSelf;
        }
    #endif
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec PiNumValue()
        {
            return AltDec(3, 141592654);
        }
        
        static AltDec PiValue()
        {
#if defined(AltNum_EnableERep)
            return AltDec(1, PiRep);
#else
            return AltDec(3, 141592654);
#endif
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static AltDec HundredMilPiNumVal()
        {
            return AltDec(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static AltDec TenMilPiNumVal()
        {
            return AltDec(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static AltDec OneMilPiNumVal()
        {
            return AltDec(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static AltDec TenPiNumVal()
        {
            return AltDec(31, 415926536);
        }
        
        static AltDec ENumValue()
        {
            return AltDec(2, 718281828);
        }

        static AltDec EValue()
        {
#if defined(AltNum_EnableERep)
            return AltDec(1, ERep);
#else
            return AltDec(2, 718281828);
#endif
        }
        
        static AltDec ZeroValue()
        {
            return AltDec();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec OneValue()
        {
            AltDec NewSelf = AltDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec TwoValue()
        {
            AltDec NewSelf = AltDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec NegativeOneValue()
        {
            AltDec NewSelf = AltDec(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec Point5Value()
        {
            AltDec NewSelf = AltDec(0, 500000000);
            return NewSelf;
        }

        static AltDec JustAboveZeroValue()
        {
            AltDec NewSelf = AltDec(0, 1);
            return NewSelf;
        }

        static AltDec OneMillionthValue()
        {
            AltDec NewSelf = AltDec(0, 1000);
            return NewSelf;
        }

        static AltDec FiveThousandthValue()
        {
            AltDec NewSelf = AltDec(0, 5000000);
            return NewSelf;
        }

        static AltDec FiveMillionthValue()
        {
            AltDec NewSelf = AltDec(0, 5000);
            return NewSelf;
        }

        static AltDec TenMillionthValue()
        {
            AltDec NewSelf = AltDec(0, 100);
            return NewSelf;
        }

        static AltDec OneHundredMillionthValue()
        {
            AltDec NewSelf = AltDec(0, 10);
            return NewSelf;
        }

        static AltDec FiveBillionthValue()
        {
            AltDec NewSelf = AltDec(0, 5);
            return NewSelf;
        }

        static AltDec LN10Value()
        {
            return AltDec(2, 302585093);
        }

        static AltDec LN10MultValue()
        {
            return AltDec(0, 434294482);
        }

        static AltDec HalfLN10MultValue()
        {
            return AltDec(0, 868588964);
        }
        
    #if defined(AltNum_EnableNil)
        static AltDec NilValue()
        {
            return AltDec(NilRep, NilRep);
        }
    #endif

        static AltDec MinimumValue()
        {
            return AltDec(2147483647, 999999999);
        }

        static AltDec MaximumValue()
        {
            return AltDec(2147483647, 999999999);
        }
public:
        static AltDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec ENum;
        
#if defined(AltNum_EnableInfinityRep)
        static AltDec Infinity;
        static AltDec NegativeInfinity;
        static AltDec ApproachingZero;
#endif

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec E;
        
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec OneGMillionth;

        //0e-7
        static AltDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static AltDec FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static AltDec Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static AltDec Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static AltDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static AltDec LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static AltDec HalfLN10Mult;

    #if defined(AltNum_EnableNil)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static AltDec Nil;
    #endif
    
#if defined(AltNum_EnableApproachingValues)
        static AltDec ApproachingRightRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 999999999);
        }

        static AltDec ApproachingLeftRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 1);
        }

        static AltDec LeftAlmostPointFiveRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 499999999);
        }

        static AltDec RightAlmostPointFiveRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 500000001);
        }
#endif
    #pragma endregion ValueDefines

    #pragma region String Commands

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #pragma endregion ConvertToOtherTypes

    #pragma region Pi Conversion

    #pragma endregion Pi Conversion

    #pragma region E Conversion

    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators

    #pragma endregion Comparison Operators

    #pragma region Mixed Fraction Operations

    #pragma endregion Mixed Fraction Operations

	#pragma region division operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntDivOpV1 = MediumDecBase::PartialUIntDivOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntDivOpV1 = MediumDecBase::PartialIntDivOpV1<IntType>;

        /// <summary>
        /// Basic division operation between AltDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntDivOpV1 = MediumDecBase::BasicUIntDivOpV1<IntType>;

        /// <summary>
        /// Basic division operation between AltDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntDivOpV1 = MediumDecBase::BasicIntDivOpV1<IntType>;

        /// <summary>
        /// Basic division operation between AltDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicDivideByUIntV1 = MediumDecBase::BasicDivideByIntV1<IntType>;

        /// <summary>
        /// Basic division operation between AltDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicDivideByIntV1 = MediumDecBase::BasicDivideByIntV1<IntType>;
    		
public:

        constexpr auto PartialUIntDivOpV1 = PartialUIntDivOpV1<unsigned int>;
        constexpr auto PartialIntDivOpV1 = PartialIntDivOpV1<signed int>;
        constexpr auto UnsignedPartialIntDivOpV1 = PartialUIntDivOpV1<signed int>;
        constexpr auto PartialUInt64DivOpV1 = PartialUIntDivOpV1<unsigned long long>;
        constexpr auto PartialInt64DivOpV1 = PartialIntDivOpV1<signed long long>;

        constexpr auto BasicUIntDivOp = BasicUIntDivOpV1<unsigned int>;
        constexpr auto BasicIntDivOp = BasicIntDivOpV1<signed int>;
        constexpr auto UnsignedBasicIntDivOp = BasicUIntDivOpV1<signed int>;
        constexpr auto BasicUInt64DivOp = BasicUIntDivOpV1<unsigned long long>;
        constexpr auto BasicInt64DivOp = BasicIntDivOpV1<signed long long>;
        constexpr auto UnsignedBasicInt64DivOp = BasicUIntDivOpV1<signed int>;

        constexpr auto BasicUInt8DivOp = BasicUIntDivOpV1<unsigned char>;
        constexpr auto BasicInt8DivOp = BasicIntDivOpV1<signed char>;
        constexpr auto BasicUInt16DivOp = BasicUIntDivOpV1<unsigned short>;
        constexpr auto BasicInt16DivOp = BasicIntDivOpV1<signed short>;

        constexpr auto BasicDivideByUInt = BasicDivideByUIntV1<unsigned int>;
        constexpr auto BasicDivideByInt = BasicDivideByIntV1<signed int>;
        constexpr auto UnsignedBasicDivideByInt = BasicDivideByUIntV1<signed int>;
        constexpr auto BasicDivideByUInt64 = BasicDivideByUIntV1<unsigned long long>;
        constexpr auto BasicDivideByInt64 = BasicDivideByIntV1<signed long long>;
        constexpr auto UnsignedBasicDivideByInt64 = BasicDivideByUIntV1<signed int>;

        constexpr auto BasicDivideByUInt8 = BasicDivideByUIntV1<unsigned char>;
        constexpr auto BasicDivideByInt8 = BasicDivideByIntV1<signed char>;
        constexpr auto BasicDivideByUInt16 = BasicDivideByUIntV1<unsigned short>;
        constexpr auto BasicDivideByInt16 = BasicDivideByIntV1<signed short>;
		
        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto DivideByTwo = AltDecBase::DivideByTwo;

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto DivideByFour = AltDecBase::DivideByFour;
		
protected:

        /// <summary>
        /// Division operation between AltDec and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntDivOpV1 = AltDecBase::UIntDivOpV1<IntType>;

        /// <summary>
        /// Division operation between AltDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntDivOpV1 = AltDecBase::IntDivOpV1<IntType>;
		
        /// <summary>
        /// Division operation between AltDec and unsigned Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto DivideByUIntV1 = AltDecBase::UIntDivOpV1<IntType>;

        /// <summary>
        /// Division operation between AltDec and Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType= signed int>
        constexpr auto DivideByIntV1 = AltDecBase::IntDivOpV1<IntType>;

public:

        constexpr auto UIntDivOp = AltDecBase::UIntDivOp;
        constexpr auto IntDivOp = AltDecBase::IntDivOp;
        constexpr auto UnsignedBasicIntDivOp = AltDecBase::UnsignedBasicIntDivOp;
        constexpr auto UInt64DivOp = AltDecBase::UInt64DivOp;
        constexpr auto Int64DivOp = AltDecBase::Int64DivOp;
        constexpr auto UnsignedBasicIntDivOp = AltDecBase::UnsignedBasicIntDivOp;
	
        constexpr auto UInt8DivOpV1 = AltDecBase::UInt8DivOpV1;
        constexpr auto Int8DivOpV1 = AltDecBase::Int8DivOpV1;
        constexpr auto UnsignedInt8DivOp = AltDecBase::UnsignedInt8DivOp;
        constexpr auto UInt16DivOp = AltDecBase::UInt16DivOp;
        constexpr auto Int16DivOp = AltDecBase::Int16DivOp;
		constexpr auto UnsignedInt16DivOp = AltDecBase::UnsignedInt16DivOp;
	
        constexpr auto DivideByUInt = AltDecBase::DivideByUInt;
        constexpr auto DivideByInt = AltDecBase::DivideByInt;
        constexpr auto UnsignedDivideByInt = AltDecBase::UnsignedDivideByInt;
        constexpr auto DivideByUInt64 = AltDecBase::DivideByUInt64;
        constexpr auto DivideByInt64 = AltDecBase::DivideByInt64;
        constexpr auto UnsignedDivideByInt64 = AltDecBase::UnsignedDivideByInt64;
		
        constexpr auto DivideByUInt8 = AltDecBase::DivideByUInt8;
        constexpr auto DivideByInt8 = AltDecBase::DivideByInt8;
        constexpr auto DivideByUInt16 = AltDecBase::DivideByUInt16;
        constexpr auto DivideByInt16 = AltDecBase::DivideByInt16;

protected:

        constexpr auto PartialDivOp = AltDecBase::PartialDivOp;
        constexpr auto UnsignedPartialDivOp = AltDecBase::UnsignedPartialDivOp;

public:
	
		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedDivOp = AltDecBase::BasicUnsignedDivOp;

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivOp = AltDecBase::BasicDivOp;
    
		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedDivision = AltDecBase::BasicUnsignedDivision;

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivision = AltDecBase::BasicDivision;

        /// <summary>
        /// Division operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto UnsignedDivOp = AltDecBase::BasicUnsignedDivOp;

        /// <summary>
        /// Division operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto DivOp = AltDecBase::BasicDivOp;

        /// <summary>
        /// Division operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto DivideByUnsigned = AltDecBase::BasicUnsignedDivision;

        /// <summary>
        /// Division operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto DivideBy = AltDecBase::BasicDivision;

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator/(const AltDec& self, const AltDec& Value) { return self.DivideBy(Value); }
		
        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator/=(AltDec& self, const AltDec& Value) { return self.DivOp(Value); }
		
        /// <summary>
        /// Division operation between AltDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator/(const AltDec& self, const signed int& Value) { return self.DivideByInt(Value); }
        friend AltDec operator/(const AltDec& self, const signed long long& Value) { return self.DivideByInt64(Value); }
        friend AltDec operator/(const AltDec& self, const unsigned int& Value) { return self.DivideByUInt(Value); }
        friend AltDec operator/(const AltDec& self, const unsigned long long& Value) { return self.DivideByUInt64(Value); }
		
        friend AltDec operator/(const signed int& lValue, const AltDec& rValue) { return ((AltDec)lValue).DivideBy(rValue); }
        friend AltDec operator/(const signed long long& lValue, const AltDec& rValue) { return ((AltDec)lValue).DivideBy(rValue); }
        friend AltDec operator/(const unsigned int& lValue, const AltDec& rValue) { return ((AltDec)lValue).DivideBy(rValue); }
        friend AltDec operator/(const unsigned long long& lValue, const AltDec& rValue) { return ((AltDec)lValue).DivideBy(rValue); }

        friend AltDec operator/(const AltDec& self, const signed char& Value) { return self.DivideByInt8(Value); }
        friend AltDec operator/(const AltDec& self, const signed short& Value) { return self.DivideByInt16(Value); }
        friend AltDec operator/(const AltDec& self, const unsigned char& Value) { return self.DivideByUInt8(Value); }
        friend AltDec operator/(const AltDec& self, const unsigned short& Value) { return self.DivideByUInt16(Value); }

        friend AltDec operator/(const signed char& lValue, const AltDec& rValue) { return ((AltDec)lValue).DivideBy(rValue); }
        friend AltDec operator/(const signed short& lValue, const AltDec& rValue) { return ((AltDec)lValue).DivideBy(rValue); }
        friend AltDec operator/(const unsigned char& lValue, const AltDec& rValue) { return ((AltDec)lValue).DivideBy(rValue); }
        friend AltDec operator/(const unsigned short& lValue, const AltDec& rValue) { return ((AltDec)lValue).DivideBy(rValue); }


        /// <summary>
        /// *= operation between AltDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator/=(AltDec& self, const signed int& Value) { return self.IntDivOp(Value); }
        friend AltDec& operator/=(AltDec& self, const signed long long& Value) { return self.Int64DivOp(Value); }
        friend AltDec& operator/=(AltDec& self, const unsigned int& Value) { return self.UIntDivOp(Value); }
        friend AltDec& operator/=(AltDec& self, const unsigned long long& Value) { return self.UInt64DivOp(Value); }

        friend AltDec& operator/=(AltDec& self, const signed char& Value) { return self.Int8DivOp(Value); }
        friend AltDec& operator/=(AltDec& self, const signed short& Value) { return self.Int16DivOp(Value); }
        friend AltDec& operator/=(AltDec& self, const unsigned char& Value) { return self.UInt8DivOp(Value); }
        friend AltDec& operator/=(AltDec& self, const unsigned short& Value) { return self.UInt16DivOp(Value); }

	#pragma endregion division operations

	#pragma region Multiplication Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntMultOpV1 = AltDecBase::PartialUIntMultOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntMultOpV1 = AltDecBase::PartialIntMultOpV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between AltDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntMultOpV1 = AltDecBase::BasicUIntMultOpV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between AltDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntMultOpV1 = AltDecBase::BasicIntMultOpV1<IntType>;
	
        /// <summary>
        /// Basic multiplication operation between AltDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicMultipleByUIntV1 = AltDecBase::BasicMultipleByUIntV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between AltDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicMultipleByIntV1 = AltDecBase::BasicMultipleByIntV1<IntType>;
    	
public:

        constexpr auto PartialUIntMultOpV1 = PartialUIntMultOpV1<unsigned int>;
        constexpr auto PartialIntMultOpV1 = PartialIntMultOpV1<signed int>;
        constexpr auto UnsignedPartialIntMultOpV1 = PartialUIntMultOpV1<signed int>;
        constexpr auto PartialUInt64MultOpV1 = PartialUIntMultOpV1<unsigned long long>;
        constexpr auto PartialInt64MultOpV1 = PartialIntMultOpV1<signed long long>;

        constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<unsigned int>;
        constexpr auto BasicIntMultOp = BasicIntMultOpV1<signed int>;
        constexpr auto UnsignedBasicIntMultOp = BasicUIntMultOpV1<signed int>;
        constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<unsigned long long>;
        constexpr auto BasicInt64MultOp = BasicIntMultOpV1<signed long long>;
        constexpr auto UnsignedBasicInt64MultOp = BasicUIntMultOpV1<signed int>;

        constexpr auto BasicUInt8MultOp = BasicUIntMultOpV1<unsigned char>;
        constexpr auto BasicInt8MultOp = BasicIntMultOpV1<signed char>;
        constexpr auto BasicUInt16MultOp = BasicUIntMultOpV1<unsigned short>;
        constexpr auto BasicInt16MultOp = BasicIntMultOpV1<signed short>;

        constexpr auto BasicMultipleByUInt = BasicMultipleByUIntV1<unsigned int>;
        constexpr auto BasicMultipleByInt = BasicMultipleByIntV1<signed int>;
        constexpr auto UnsignedBasicMultipleByInt = BasicMultipleByUIntV1<signed int>;
        constexpr auto BasicMultipleByUInt64 = BasicMultipleByUIntV1<unsigned long long>;
        constexpr auto BasicMultipleByInt64 = BasicMultipleByIntV1<signed long long>;
        constexpr auto UnsignedBasicMultipleByInt64 = BasicMultipleByUIntV1<signed int>;

        constexpr auto BasicMultipleByUInt8 = BasicMultipleByUIntV1<unsigned char>;
        constexpr auto BasicMultipleByInt8 = BasicMultipleByIntV1<signed char>;
        constexpr auto BasicMultipleByUInt16 = BasicMultipleByUIntV1<unsigned short>;
        constexpr auto BasicMultipleByInt16 = BasicMultipleByIntV1<signed short>;
		
        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto MultipleByTwo = AltDecBase::MultipleByTwo;

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto MultipleByFour = AltDecBase::MultipleByFour;
		
protected:

        /// <summary>
        /// Multiplication operation between AltDec and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntMultOpV1 = AltDecBase::UIntMultOpV1<IntType>;

        /// <summary>
        /// Multiplication operation between AltDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntMultOpV1 = AltDecBase::IntMultOpV1<IntType>;
		
        /// <summary>
        /// Multiplication operation between AltDec and unsigned Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto MultiplyByUIntV1 = AltDecBase::UIntDivOpV1<IntType>;

        /// <summary>
        /// Multiplication operation between AltDec and Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto MultiplyByIntV1 = AltDecBase::IntDivOpV1<IntType>;

public:

        constexpr auto UIntMultOp = AltDecBase::UIntMultOp;
        constexpr auto IntMultOp = AltDecBase::IntMultOp;
        constexpr auto UnsignedBasicIntMultOp = AltDecBase::UnsignedBasicIntMultOp;
        constexpr auto UInt64MultOp = AltDecBase::UInt64MultOp;
        constexpr auto Int64MultOp = AltDecBase::Int64MultOp;
        constexpr auto UnsignedBasicIntMultOp = AltDecBase::UnsignedBasicIntMultOp;
	
        constexpr auto UInt8MultOpV1 = AltDecBase::UInt8MultOpV1;
        constexpr auto Int8MultOpV1 = AltDecBase::Int8MultOpV1;
        constexpr auto UnsignedInt8MultOp = AltDecBase::UnsignedInt8MultOp;
        constexpr auto UInt16MultOp = AltDecBase::UInt16MultOp;
        constexpr auto Int16MultOp = AltDecBase::Int16MultOp;
		constexpr auto UnsignedInt16MultOp = AltDecBase::UnsignedInt16MultOp;
	
        constexpr auto MultiplyByUInt = AltDecBase::MultiplyByUInt;
        constexpr auto MultiplyByInt = AltDecBase::MultiplyByInt;
        constexpr auto UnsignedMultiplyByInt = AltDecBase::UnsignedMultiplyByInt;
        constexpr auto MultiplyByUInt64 = AltDecBase::MultiplyByUInt64;
        constexpr auto MultiplyByInt64 = AltDecBase::MultiplyByInt64;
        constexpr auto UnsignedMultiplyByInt64 = AltDecBase::UnsignedMultiplyByInt64;
		
        constexpr auto MultiplyByUInt8 = AltDecBase::MultiplyByUInt8;
        constexpr auto MultiplyByInt8 = AltDecBase::MultiplyByInt8;
        constexpr auto MultiplyByUInt16 = AltDecBase::MultiplyByUInt16;
        constexpr auto MultiplyByInt16 = AltDecBase::MultiplyByInt16;

protected:

        constexpr auto PartialMultOp = AltDecBase::PartialMultOp;
        constexpr auto UnsignedPartialMultOp = AltDecBase::UnsignedPartialMultOp;

public:
		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned AltDecBase
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedMultOp = AltDecBase::BasicUnsignedMultOp;

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned AltDecBase
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicMultOp = AltDecBase::BasicMultOp;

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedMultiplication = AltDecBase::BasicUnsignedMultiplication;

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicMultiplication = AltDecBase::BasicMultiplication;

        /// <summary>
        /// Multiplication operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto UnsignedMultOp = AltDecBase::BasicUnsignedMultOp;

        /// <summary>
        /// Multiplication operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto MultOp = AltDecBase::BasicMultOp;

        /// <summary>
        /// Multiplication operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto MultipleByUnsigned = AltDecBase::BasicUnsignedMultiplication;

        /// <summary>
        /// Multiplication operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto MultipleBy = AltDecBase::BasicMultiplication;

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator*(const AltDec& self, const AltDec& Value) { return self.MultipleBy(Value); }

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator*=(AltDec& self, const AltDec& Value) { return self.MultOp(Value); }
		
        /// <summary>
        /// Multiplication operation between AltDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator*(const AltDec& self, const signed int& Value) { return self.MultiplyByInt(Value); }
        friend AltDec operator*(const AltDec& self, const signed long long& Value) { return self.MultiplyByInt64(Value); }
        friend AltDec operator*(const AltDec& self, const unsigned int& Value) { return self.MultiplyByUInt(Value); }
        friend AltDec operator*(const AltDec& self, const unsigned long long& Value) { return self.MultiplyByUInt64(Value); }
		
        friend AltDec operator*(const signed int& lValue, const AltDec& rValue) { return rValue.MultiplyByInt(lValue); }
        friend AltDec operator*(const signed long long& lValue, const AltDec& rValue) { return rValue.MultiplyByInt64(lValue); }
        friend AltDec operator*(const unsigned int& lValue, const AltDec& rValue) { return rValue.MultiplyByUInt(lValue); }
        friend AltDec operator*(const unsigned long long& lValue, const AltDec& rValue) { return rValue.MultiplyByUInt64(lValue); }

        friend AltDec operator*(const AltDec& self, const signed char& Value) { return self.MultiplyByInt8(Value); }
        friend AltDec operator*(const AltDec& self, const signed short& Value) { return self.MultiplyByInt16(Value); }
        friend AltDec operator*(const AltDec& self, const unsigned char& Value) { return self.MultiplyByUInt8(Value); }
        friend AltDec operator*(const AltDec& self, const unsigned short& Value) { return self.MultiplyByUInt16(Value); }

        friend AltDec operator*(const signed char& lValue, const AltDec& rValue) { return rValue.MultiplyByInt8(lValue); }
        friend AltDec operator*(const signed short& lValue, const AltDec& rValue) { return rValue.MultiplyByInt16(lValue); }
        friend AltDec operator*(const unsigned char& lValue, const AltDec& rValue) { return rValue.MultiplyByUInt8(lValue); }
        friend AltDec operator*(const unsigned short& lValue, const AltDec& rValue) { return rValue.MultiplyByUInt16(lValue); }

        /// <summary>
        /// *= operation between AltDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator*=(AltDec& self, const signed int& Value) { return self.IntMultOp(Value); }
        friend AltDec& operator*=(AltDec& self, const signed long long& Value) { return self.Int64MultOp(Value); }
        friend AltDec& operator*=(AltDec& self, const unsigned int& Value) { return self.UIntMultOp(Value); }
        friend AltDec& operator*=(AltDec& self, const unsigned long long& Value) { return self.UInt64MultOp(Value); }

        friend AltDec& operator*=(AltDec& self, const signed char& Value) { return self.Int8MultOp(Value); }
        friend AltDec& operator*=(AltDec& self, const signed short& Value) { return self.Int16MultOp(Value); }
        friend AltDec& operator*=(AltDec& self, const unsigned char& Value) { return self.UInt8MultOp(Value); }
        friend AltDec& operator*=(AltDec& self, const unsigned short& Value) { return self.UInt16MultOp(Value); }

	#pragma endregion Multiplication Operations

	#pragma region Addition Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntAddOpV1 = AltDecBase::PartialUIntAddOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntAddOpV1 = AltDecBase::PartialIntAddOpV1<IntType>;

        /// <summary>
        /// Basic addition operation between AltDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntAddOpV1 = AltDecBase::BasicUIntAddOpV1<IntType>;

        /// <summary>
        /// Basic addition operation between AltDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntAddOpV1 = AltDecBase::BasicIntAddOpV1<IntType>;
	
        /// <summary>
        /// Basic addition operation between AltDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicAddByUIntV1 = AltDecBase::BasicAddByUIntV1<IntType>;

        /// <summary>
        /// Basic addition operation between AltDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicAddByIntV1 = AltDecBase::BasicAddByIntV1<IntType>;
    	
public:

        constexpr auto PartialUIntAddOpV1 = PartialUIntAddOpV1<unsigned int>;
        constexpr auto PartialIntAddOpV1 = PartialIntAddOpV1<signed int>;
        constexpr auto UnsignedPartialIntAddOpV1 = PartialUIntAddOpV1<signed int>;
        constexpr auto PartialUInt64AddOpV1 = PartialUIntAddOpV1<unsigned long long>;
        constexpr auto PartialInt64AddOpV1 = PartialIntAddOpV1<signed long long>;

        constexpr auto BasicAddByUInt = BasicAddByUIntV1<unsigned int>;
        constexpr auto BasicAddByInt = BasicAddByIntV1<signed int>;
        constexpr auto UnsignedBasicAddByInt = BasicAddByUIntV1<signed int>;
        constexpr auto BasicAddByUInt64 = BasicAddByUIntV1<unsigned long long>;
        constexpr auto BasicAddByInt64 = BasicAddByIntV1<signed long long>;
        constexpr auto UnsignedBasicAddByInt64 = BasicAddByUIntV1<signed int>;

        constexpr auto BasicAddByUInt8 = BasicAddByUIntV1<unsigned char>;
        constexpr auto BasicAddByInt8 = BasicAddByIntV1<signed char>;
        constexpr auto BasicAddByUInt16 = BasicAddByUIntV1<unsigned short>;
        constexpr auto BasicAddByInt16 = BasicAddByIntV1<signed short>;
		
protected:

        /// <summary>
        /// Addition operation between AltDec and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntAddOpV1 = AltDecBase::UIntAddOpV1<IntType>;

        /// <summary>
        /// Addition operation between AltDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntAddOpV1 = AltDecBase::IntAddOpV1<IntType>;
		
        /// <summary>
        /// Addition operation between AltDec and unsigned Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto AddByUIntV1 = AltDecBase::UIntDivOpV1<IntType>;

        /// <summary>
        /// Addition operation between AltDec and Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType= signed int>
        constexpr auto AddByIntV1 = AltDecBase::IntDivOpV1<IntType>;

public:

        constexpr auto UIntAddOp = AltDecBase::UIntAddOp;
        constexpr auto IntAddOp = AltDecBase::IntAddOp;
        constexpr auto UnsignedBasicIntAddOp = AltDecBase::UnsignedBasicIntAddOp;
        constexpr auto UInt64AddOp = AltDecBase::UInt64AddOp;
        constexpr auto Int64AddOp = AltDecBase::Int64AddOp;
        constexpr auto UnsignedBasicIntAddOp = AltDecBase::UnsignedBasicIntAddOp;
	
        constexpr auto UInt8AddOpV1 = AltDecBase::UInt8AddOpV1;
        constexpr auto Int8AddOpV1 = AltDecBase::Int8AddOpV1;
        constexpr auto UnsignedInt8AddOp = AltDecBase::UnsignedInt8AddOp;
        constexpr auto UInt16AddOp = AltDecBase::UInt16AddOp;
        constexpr auto Int16AddOp = AltDecBase::Int16AddOp;
		constexpr auto UnsignedInt16AddOp = AltDecBase::UnsignedInt16AddOp;
	
        constexpr auto AddByUInt = AltDecBase::AddByUInt;
        constexpr auto AddByInt = AltDecBase::AddByInt;
        constexpr auto UnsignedAddByInt = AltDecBase::UnsignedAddByInt;
        constexpr auto AddByUInt64 = AltDecBase::AddByUInt64;
        constexpr auto AddByInt64 = AltDecBase::AddByInt64;
        constexpr auto UnsignedAddByInt64 = AltDecBase::UnsignedAddByInt64;
		
        constexpr auto AddByUInt8 = AltDecBase::AddByUInt8;
        constexpr auto AddByInt8 = AltDecBase::AddByInt8;
        constexpr auto AddByUInt16 = AltDecBase::AddByUInt16;
        constexpr auto AddByInt16 = AltDecBase::AddByInt16;

protected:

        constexpr auto PartialAddOp = AltDecBase::PartialAddOp;
        constexpr auto UnsignedPartialAddOp = AltDecBase::UnsignedPartialAddOp;

public:
		/// <summary>
        /// Basic addition operation that ignores special decimal status with unsigned AltDecBase
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedAddOp = AltDecBase::BasicUnsignedAddOp;

		/// <summary>
        /// Basic addition operation that ignores special decimal status with unsigned AltDecBase
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicAddOp = AltDecBase::BasicAddOp;

		/// <summary>
        /// Basic unsigned addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedAddition = AltDecBase::BasicUnsignedAddition;

		/// <summary>
        /// Basic addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicAddition = AltDecBase::BasicAddition;

        /// <summary>
        /// Addition operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto UnsignedAddOp = AltDecBase::BasicUnsignedAddOp;

        /// <summary>
        /// Addition operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto AddOp = AltDecBase::BasicAddOp;

        /// <summary>
        /// Addition operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto AddByUnsigned = AltDecBase::BasicUnsignedAddition;

        /// <summary>
        /// Addition operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        constexpr auto AddBy = AltDecBase::BasicAddition;

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator+(const AltDec& self, const AltDec& Value) { return self.AddBy(Value); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator+=(AltDec& self, const AltDec& Value) { return self.AddOp(Value); }
		
        /// <summary>
        /// Addition operation between AltDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator+(const AltDec& self, const signed int& Value) { return self.AddByInt(Value); }
        friend AltDec operator+(const AltDec& self, const signed long long& Value) { return self.AddByInt64(Value); }
        friend AltDec operator+(const AltDec& self, const unsigned int& Value) { return self.AddByUInt(Value); }
        friend AltDec operator+(const AltDec& self, const unsigned long long& Value) { return self.AddByUInt64(Value); }
		
        friend AltDec operator+(const signed int& lValue, const AltDec& rValue) { return rValue.AddByInt(lValue); }
        friend AltDec operator+(const signed long long& lValue, const AltDec& rValue) { return rValue.AddByInt64(lValue); }
        friend AltDec operator+(const unsigned int& lValue, const AltDec& rValue) { return rValue.AddByUInt(lValue); }
        friend AltDec operator+(const unsigned long long& lValue, const AltDec& rValue) { return rValue.AddByUInt64(lValue); }

        friend AltDec operator+(const AltDec& self, const signed char& Value) { return self.AddByInt8(Value); }
        friend AltDec operator+(const AltDec& self, const signed short& Value) { return self.AddByInt16(Value); }
        friend AltDec operator+(const AltDec& self, const unsigned char& Value) { return self.AddByUInt8(Value); }
        friend AltDec operator+(const AltDec& self, const unsigned short& Value) { return self.AddByUInt16(Value); }

        friend AltDec operator+(const signed char& lValue, const AltDec& rValue) { return rValue.AddByInt8(lValue); }
        friend AltDec operator+(const signed short& lValue, const AltDec& rValue) { return rValue.AddByInt16(lValue); }
        friend AltDec operator+(const unsigned char& lValue, const AltDec& rValue) { return rValue.AddByUInt8(lValue); }
        friend AltDec operator+(const unsigned short& lValue, const AltDec& rValue) { return rValue.AddByUInt16(lValue); }

        /// <summary>
        /// += operation between AltDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator+=(AltDec& self, const signed int& Value) { return self.IntAddOp(Value); }
        friend AltDec& operator+=(AltDec& self, const signed long long& Value) { return self.Int64AddOp(Value); }
        friend AltDec& operator+=(AltDec& self, const unsigned int& Value) { return self.UIntAddOp(Value); }
        friend AltDec& operator+=(AltDec& self, const unsigned long long& Value) { return self.UInt64AddOp(Value); }

        friend AltDec& operator+=(AltDec& self, const signed char& Value) { return self.Int8AddOp(Value); }
        friend AltDec& operator+=(AltDec& self, const signed short& Value) { return self.Int16AddOp(Value); }
        friend AltDec& operator+=(AltDec& self, const unsigned char& Value) { return self.UInt8AddOp(Value); }
        friend AltDec& operator+=(AltDec& self, const unsigned short& Value) { return self.UInt16AddOp(Value); }

	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntSubOpV1 = AltDecBase::PartialUIntSubOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntSubOpV1 = AltDecBase::PartialIntSubOpV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between AltDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntSubOpV1 = AltDecBase::BasicUIntSubOpV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between AltDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntSubOpV1 = AltDecBase::BasicIntSubOpV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between AltDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicSubtractByUIntV1 = AltDecBase::BasicSubtractByIntV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between AltDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicSubtractByIntV1 = AltDecBase::BasicSubtractByIntV1<IntType>;
    		
public:

        constexpr auto PartialUIntSubOpV1 = PartialUIntSubOpV1<unsigned int>;
        constexpr auto PartialIntSubOpV1 = PartialIntSubOpV1<signed int>;
        constexpr auto UnsignedPartialIntSubOpV1 = PartialUIntSubOpV1<signed int>;
        constexpr auto PartialUInt64SubOpV1 = PartialUIntSubOpV1<unsigned long long>;
        constexpr auto PartialInt64SubOpV1 = PartialIntSubOpV1<signed long long>;

        constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<unsigned int>;
        constexpr auto BasicIntSubOp = BasicIntSubOpV1<signed int>;
        constexpr auto UnsignedBasicIntSubOp = BasicUIntSubOpV1<signed int>;
        constexpr auto BasicUInt64SubOp = BasicUIntSubOpV1<unsigned long long>;
        constexpr auto BasicInt64SubOp = BasicIntSubOpV1<signed long long>;
        constexpr auto UnsignedBasicInt64SubOp = BasicUIntSubOpV1<signed int>;

        constexpr auto BasicUInt8SubOp = BasicUIntSubOpV1<unsigned char>;
        constexpr auto BasicInt8SubOp = BasicIntSubOpV1<signed char>;
        constexpr auto BasicUInt16SubOp = BasicUIntSubOpV1<unsigned short>;
        constexpr auto BasicInt16SubOp = BasicIntSubOpV1<signed short>;

        constexpr auto BasicSubtractByUInt = BasicSubtractByUIntV1<unsigned int>;
        constexpr auto BasicSubtractByInt = BasicSubtractByIntV1<signed int>;
        constexpr auto UnsignedBasicSubtractByInt = BasicSubtractByUIntV1<signed int>;
        constexpr auto BasicSubtractByUInt64 = BasicSubtractByUIntV1<unsigned long long>;
        constexpr auto BasicSubtractByInt64 = BasicSubtractByIntV1<signed long long>;
        constexpr auto UnsignedBasicSubtractByInt64 = BasicSubtractByUIntV1<signed int>;

        constexpr auto BasicSubtractByUInt8 = BasicSubtractByUIntV1<unsigned char>;
        constexpr auto BasicSubtractByInt8 = BasicSubtractByIntV1<signed char>;
        constexpr auto BasicSubtractByUInt16 = BasicSubtractByUIntV1<unsigned short>;
        constexpr auto BasicSubtractByInt16 = BasicSubtractByIntV1<signed short>;
	
protected:

        /// <summary>
        /// Subtraction operation between AltDec and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntSubOpV1 = AltDecBase::UIntSubOpV1<IntType>;

        /// <summary>
        /// Subtraction operation between AltDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntSubOpV1 = AltDecBase::IntSubOpV1<IntType>;
		
        /// <summary>
        /// Subtraction operation between AltDec and unsigned Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto SubtractByUIntV1 = AltDecBase::UIntDivOpV1<IntType>;

        /// <summary>
        /// Subtraction operation between AltDec and Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType= signed int>
        constexpr auto SubtractByIntV1 = AltDecBase::IntDivOpV1<IntType>;

public:

        constexpr auto UIntSubOp = AltDecBase::UIntSubOp;
        constexpr auto IntSubOp = AltDecBase::IntSubOp;
        constexpr auto UnsignedBasicIntSubOp = AltDecBase::UnsignedBasicIntSubOp;
        constexpr auto UInt64SubOp = AltDecBase::UInt64SubOp;
        constexpr auto Int64SubOp = AltDecBase::Int64SubOp;
        constexpr auto UnsignedBasicIntSubOp = AltDecBase::UnsignedBasicIntSubOp;
	
        constexpr auto UInt8SubOpV1 = AltDecBase::UInt8SubOpV1;
        constexpr auto Int8SubOpV1 = AltDecBase::Int8SubOpV1;
        constexpr auto UnsignedInt8SubOp = AltDecBase::UnsignedInt8SubOp;
        constexpr auto UInt16SubOp = AltDecBase::UInt16SubOp;
        constexpr auto Int16SubOp = AltDecBase::Int16SubOp;
		constexpr auto UnsignedInt16SubOp = AltDecBase::UnsignedInt16SubOp;
	
        constexpr auto SubtractByUInt = AltDecBase::SubtractByUInt;
        constexpr auto SubtractByInt = AltDecBase::SubtractByInt;
        constexpr auto UnsignedSubtractByInt = AltDecBase::UnsignedSubtractByInt;
        constexpr auto SubtractByUInt64 = AltDecBase::SubtractByUInt64;
        constexpr auto SubtractByInt64 = AltDecBase::SubtractByInt64;
        constexpr auto UnsignedSubtractByInt64 = AltDecBase::UnsignedSubtractByInt64;
		
        constexpr auto SubtractByUInt8 = AltDecBase::SubtractByUInt8;
        constexpr auto SubtractByInt8 = AltDecBase::SubtractByInt8;
        constexpr auto SubtractByUInt16 = AltDecBase::SubtractByUInt16;
        constexpr auto SubtractByInt16 = AltDecBase::SubtractByInt16;
		
protected:

        constexpr auto PartialSubOp = MediumDecBaseV2::PartialSubOp;
        constexpr auto UnsignedPartialSubOp = AltDecBase::UnsignedPartialSubOp;

public:
	
		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedSubOp = AltDecBase::BasicUnsignedSubOp;

		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicSubOp = AltDecBase::BasicSubOp;
    
		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedDivision = AltDecBase::BasicUnsignedDivision;

		/// <summary>
        /// Basic subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivision = AltDecBase::BasicDivision;

        /// <summary>
        /// Subtraction operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        constexpr auto UnsignedSubOp = AltDecBase::BasicUnsignedSubOp;

        /// <summary>
        /// Subtraction operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        constexpr auto SubOp = AltDecBase::BasicSubOp;

        /// <summary>
        /// Subtraction operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        constexpr auto SubtractByUnsigned = AltDecBase::BasicUnsignedDivision;

        /// <summary>
        /// Subtraction operation between AltDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDec&</returns>
        constexpr auto SubtractBy = AltDecBase::BasicDivision;

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator-(const AltDec& self, const AltDec& Value) { return self.SubtractBy(Value); }
		
        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator-=(AltDec& self, const AltDec& Value) { return self.SubOp(Value); }
		
        /// <summary>
        /// Subtraction operation between AltDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator-(const AltDec& self, const signed int& Value) { return self.SubtractByInt(Value); }
        friend AltDec operator-(const AltDec& self, const signed long long& Value) { return self.SubtractByInt64(Value); }
        friend AltDec operator-(const AltDec& self, const unsigned int& Value) { return self.SubtractByUInt(Value); }
        friend AltDec operator-(const AltDec& self, const unsigned long long& Value) { return self.SubtractByUInt64(Value); }
		
        friend AltDec operator-(const signed int& lValue, const AltDec& rValue) { return ((AltDec)lValue).SubtractBy(rValue); }
        friend AltDec operator-(const signed long long& lValue, const AltDec& rValue) { return ((AltDec)lValue).SubtractBy(rValue); }
        friend AltDec operator-(const unsigned int& lValue, const AltDec& rValue) { return ((AltDec)lValue).SubtractBy(rValue); }
        friend AltDec operator-(const unsigned long long& lValue, const AltDec& rValue) { return ((AltDec)lValue).SubtractBy(rValue); }

        friend AltDec operator-(const AltDec& self, const signed char& Value) { return self.SubtractByInt8(Value); }
        friend AltDec operator-(const AltDec& self, const signed short& Value) { return self.SubtractByInt16(Value); }
        friend AltDec operator-(const AltDec& self, const unsigned char& Value) { return self.SubtractByUInt8(Value); }
        friend AltDec operator-(const AltDec& self, const unsigned short& Value) { return self.SubtractByUInt16(Value); }

        friend AltDec operator-(const signed char& lValue, const AltDec& rValue) { return ((AltDec)lValue).SubtractBy(rValue); }
        friend AltDec operator-(const signed short& lValue, const AltDec& rValue) { return ((AltDec)lValue).SubtractBy(rValue); }
        friend AltDec operator-(const unsigned char& lValue, const AltDec& rValue) { return ((AltDec)lValue).SubtractBy(rValue); }
        friend AltDec operator-(const unsigned short& lValue, const AltDec& rValue) { return ((AltDec)lValue).SubtractBy(rValue); }


        /// <summary>
        /// -= operation between AltDec and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator-=(AltDec& self, const signed int& Value) { return self.IntSubOp(Value); }
        friend AltDec& operator-=(AltDec& self, const signed long long& Value) { return self.Int64SubOp(Value); }
        friend AltDec& operator-=(AltDec& self, const unsigned int& Value) { return self.UIntSubOp(Value); }
        friend AltDec& operator-=(AltDec& self, const unsigned long long& Value) { return self.UInt64SubOp(Value); }

        friend AltDec& operator-=(AltDec& self, const signed char& Value) { return self.Int8SubOp(Value); }
        friend AltDec& operator-=(AltDec& self, const signed short& Value) { return self.Int16SubOp(Value); }
        friend AltDec& operator-=(AltDec& self, const unsigned char& Value) { return self.UInt8SubOp(Value); }
        friend AltDec& operator-=(AltDec& self, const unsigned short& Value) { return self.UInt16SubOp(Value); }

	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations

        /// <summary>
        /// Modulus Operation(Returning inside
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>AltDecBase</returns>
        friend ModResult operator%(const AltDec& LValue, const AltDec& RValue)
		{
			return ModResult(LValue, RValue);
		}

        friend AltDec& operator%=(AltDec& LValue, const AltDec& RValue)
		{ 
            AltDec divRes = LValue / RValue;
            LValue -= RValue * divRes;
			return *this;
		}

	#pragma endregion Modulus Operations
	
    #pragma region Floating Operator Overrides
    
        friend AltDec operator+(const AltDec& self, const float& Value) { return self + (AltDec)Value; }
        friend AltDec operator-(const AltDec& self, const float& Value) { return self - (AltDec)Value; }
        friend AltDec operator*(const AltDec& self, const float& Value) { return self * (AltDec)Value; }
        friend AltDec operator/(const AltDec& self, const float& Value) { return self / (AltDec)Value; }

        friend AltDec operator+(const float& Value, const AltDec& self) { return (AltDec)Value + self; }
        friend AltDec operator-(const float& Value, const AltDec& self) { return (AltDec)Value - self; }
        friend AltDec operator*(const float& Value, const AltDec& self) { return (AltDec)Value * self; }
        friend AltDec operator/(const float& Value, const AltDec& self) { return (AltDec)Value / self; }

        friend AltDec operator+(const AltDec& self, const double& Value) { return self + (AltDec)Value; }
        friend AltDec operator-(const AltDec& self, const double& Value) { return self - (AltDec)Value; }
        friend AltDec operator*(const AltDec& self, const double& Value) { return self * (AltDec)Value; }
        friend AltDec operator/(const AltDec& self, const double& Value) { return self / (AltDec)Value; }

        friend AltDec operator+(const AltDec& self, const ldouble& Value) { return self + (AltDec)Value; }
        friend AltDec operator-(const AltDec& self, const ldouble& Value) { return self - (AltDec)Value; }
        friend AltDec operator*(const AltDec& self, const ldouble& Value) { return self * (AltDec)Value; }
        friend AltDec operator/(const AltDec& self, const ldouble& Value) { return self / (AltDec)Value; }

        friend AltDec operator+(const ldouble& Value, const AltDec& self) { return (AltDec)Value + self; }
        friend AltDec operator-(const ldouble& Value, const AltDec& self) { return (AltDec)Value - self; }
        friend AltDec operator*(const ldouble& Value, const AltDec& self) { return (AltDec)Value * self; }
        friend AltDec operator/(const ldouble& Value, const AltDec& self) { return (AltDec)Value / self; }

    #pragma endregion Floating Operator Overrides
	
    #pragma region Bitwise Functions
	
    #pragma endregion Bitwise Functions
	
    #pragma region Other Operators
	
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator-(AltDec& self)
        {
            self.SwapNegativeStatus(); return self;
        }

        /// <summary>
        /// ++AltDec Operator
        /// </summary>
        /// <returns>AltDec &</returns>
        AltDec& operator ++()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
#endif
            if (DecimalHalf == 0)
                ++IntValue.Value;
            else if (IntValue == NegativeRep)
                IntValue = MirroredInt::Zero;
            else
                ++IntValue.Value;
            return *this;
        }

        /// <summary>
        /// ++AltDec Operator
        /// </summary>
        /// <returns>AltDec &</returns>
        AltDec& operator --()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
#endif
            if (DecimalHalf == 0)
                --IntValue.Value;
            else if (IntValue.Value == 0)
                IntValue = NegativeRep;
            else
                --IntValue.Value;
            return *this;
        }

        /// <summary>
        /// AltDec++ Operator
        /// </summary>
        /// <returns>AltDec</returns>
        AltDec operator ++(int)
        {
            AltDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// AltDec-- Operator
        /// </summary>
        /// <returns>AltDec</returns>
        AltDec operator --(int)
        {
            AltDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// AltDec* Operator
        /// </summary>
        /// <returns>AltDec &</returns>
        AltDec& operator *()
        {
            return *this;
        }
		
    #pragma endregion Other Operators
	
	#pragma region Math Etc Functions
	
	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions
	
	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
	
	#pragma endregion Log Functions

    #pragma region Trigonomic Etc Functions
	
    #pragma endregion Trigonomic Etc Functions

    };

    #pragma region String Function Source

    #pragma endregion String Function Source
}
