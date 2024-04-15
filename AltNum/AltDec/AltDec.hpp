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

    #pragma region NormalRep Integer Division Operations

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep Integer Multiplication Operations

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations

	#pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep Integer Bitwise Operations

    #pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations

    #pragma endregion Mixed Fraction Operations

    #pragma region NormalRep AltNum Division Operations

    #pragma endregion NormalRep AltNum Division Operations

    #pragma region NormalRep AltNum Multiplication Operations

    #pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep AltNum Addition Operations

    #pragma endregion NormalRep AltNum Addition Operations

    #pragma region NormalRep AltNum Subtraction Operations

    #pragma endregion NormalRep AltNum Subtraction Operations

    #pragma region Other Division Operations

    #pragma endregion Other Division Operations

    #pragma region Other Multiplication Operations

    #pragma endregion Other Multiplication Operations

    #pragma region Other Addition Operations

    #pragma endregion Other Addition Operations

    #pragma region Other Subtraction Operations

    #pragma endregion Other Subtraction Operations

    #pragma region Main AltNum Operations

    #pragma endregion Main AltNum Operations

    #pragma region Main Operator Overrides

    #pragma endregion Main Operator Overrides

    #pragma region Other Operators

    #pragma endregion Other Operators

    #pragma region Modulus Operations

    #pragma endregion Modulus Operations

    #pragma region Bitwise Functions

    #pragma endregion Bitwise Functions

    #pragma region Math Etc Functions

    #pragma endregion Pow and Sqrt Functions

    #pragma region Log Functions

    #pragma endregion Log Functions

    #pragma region Trigonomic Etc Functions

    #pragma endregion Math/Trigonomic Etc Functions

    };

    #pragma region String Function Source

    #pragma endregion String Function Source
}
