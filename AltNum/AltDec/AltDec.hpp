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

#include <string>
#include <cmath>
#include "..\OtherFunctions\VariableConversionFunctions.h"

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)
#if defined(AltNum_UseOldDivisionCode)
    #include <boost/multiprecision/cpp_int.hpp>
#endif

#ifdef AltNum_EnableMediumDecBasedSetValues
    #include "..\MediumDec\MediumDec.hpp"
#endif

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "AltNumModChecker.hpp"

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
#if defined(AltDec_UseMirroredInt)
        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.(Default constructor)
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        AltDec(const MirroredInt& intVal = MirroredInt::Zero, const signed int& decVal = 0, const signed int& extraVal = 0)
        {

            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }
#endif

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
#if !defined(AltDec_UseMirroredInt)
        AltDec(const int& intVal=0, const signed int& decVal = 0, const signed int& extraVal = 0)
#else
        AltDec(const int& intVal, const signed int& decVal = 0, const signed int& extraVal = 0)
#endif
        {
#if defined(AltDec_UseMirroredInt)&&defined(BlazesMirroredInt_UseLegacyValueBehavior)
            IntValue.Value = intVal;
#else
            IntValue = intVal;
#endif
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

    public:
        #pragma region Const Representation values
        //Code inside Derivable version of class
        #pragma endregion Const Representation values

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
        static AltDecBase NaNValue()
        {
            AltDecBase NewSelf = AltDecBase(0, NaNRep);
            return NewSelf;
        }
        
        static AltDecBase UndefinedValue()
        {
            AltDecBase NewSelf = AltDecBase(0, UndefinedRep);
            return NewSelf;
        }
    #endif

    #if defined(AltNum_EnableInfinityRep)
        static AltDecBase InfinityValue()
        {
            AltDecBase NewSelf = AltDecBase(1, InfinityRep);
            return NewSelf;
        }
        
        static AltDecBase NegativeInfinityValue()
        {
            AltDecBase NewSelf = AltDecBase(-1, InfinityRep);
            return NewSelf;
        }
    #endif
        
    #if defined(AltNum_EnableApproachingValues)
        static AltDecBase ApproachingZeroValue()
        {
            AltDecBase NewSelf = AltDecBase(0, ApproachingBottomRep);
            return NewSelf;
        }
        
        static AltDecBase ApproachingOneFromLeftValue()
        {
            AltDecBase NewSelf = AltDecBase(0, ApproachingTopRep);
            return NewSelf;
        }
        
        static AltDecBase ApproachingOneFromRightValue()
        {
            AltDecBase NewSelf = AltDecBase(1, ApproachingBottomRep);
            return NewSelf;
        }

        static AltDecBase NegativeApproachingZeroValue()
        {
            AltDecBase NewSelf = AltDecBase(NegativeRep, ApproachingBottomRep);
            return NewSelf;
        }
    #endif
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase PiNumValue()
        {
            return AltDecBase(3, 141592654);
        }
        
        static AltDecBase PiValue()
        {
#if defined(AltNum_EnableERep)
            return AltDecBase(1, PiRep);
#else
            return AltDecBase(3, 141592654);
#endif
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static AltDecBase HundredMilPiNumVal()
        {
            return AltDecBase(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static AltDecBase TenMilPiNumVal()
        {
            return AltDecBase(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static AltDecBase OneMilPiNumVal()
        {
            return AltDecBase(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static AltDecBase TenPiNumVal()
        {
            return AltDecBase(31, 415926536);
        }
        
        static AltDecBase ENumValue()
        {
            return AltDecBase(2, 718281828);
        }

        static AltDecBase EValue()
        {
#if defined(AltNum_EnableERep)
            return AltDecBase(1, ERep);
#else
            return AltDecBase(2, 718281828);
#endif
        }
        
        static AltDecBase ZeroValue()
        {
            return AltDecBase();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase OneValue()
        {
            AltDecBase NewSelf = AltDecBase(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase TwoValue()
        {
            AltDecBase NewSelf = AltDecBase(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase NegativeOneValue()
        {
            AltDecBase NewSelf = AltDecBase(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase Point5Value()
        {
            AltDecBase NewSelf = AltDecBase(0, 500000000);
            return NewSelf;
        }

        static AltDecBase JustAboveZeroValue()
        {
            AltDecBase NewSelf = AltDecBase(0, 1);
            return NewSelf;
        }

        static AltDecBase OneMillionthValue()
        {
            AltDecBase NewSelf = AltDecBase(0, 1000);
            return NewSelf;
        }

        static AltDecBase FiveThousandthValue()
        {
            AltDecBase NewSelf = AltDecBase(0, 5000000);
            return NewSelf;
        }

        static AltDecBase FiveMillionthValue()
        {
            AltDecBase NewSelf = AltDecBase(0, 5000);
            return NewSelf;
        }

        static AltDecBase TenMillionthValue()
        {
            AltDecBase NewSelf = AltDecBase(0, 100);
            return NewSelf;
        }

        static AltDecBase OneHundredMillionthValue()
        {
            AltDecBase NewSelf = AltDecBase(0, 10);
            return NewSelf;
        }

        static AltDecBase FiveBillionthValue()
        {
            AltDecBase NewSelf = AltDecBase(0, 5);
            return NewSelf;
        }

        static AltDecBase LN10Value()
        {
            return AltDecBase(2, 302585093);
        }

        static AltDecBase LN10MultValue()
        {
            return AltDecBase(0, 434294482);
        }

        static AltDecBase HalfLN10MultValue()
        {
            return AltDecBase(0, 868588964);
        }
        
    #if defined(AltNum_EnableNilRep)
        static AltDecBase NilValue()
        {
            return AltDecBase(NilRep, NilRep);
        }
    #endif

        static AltDecBase MinimumValue()
        {
            return AltDecBase(2147483647, 999999999);
        }

        static AltDecBase MaximumValue()
        {
            return AltDecBase(2147483647, 999999999);
        }
public:
        static AltDecBase AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase ENum;
        
#if defined(AltNum_EnableInfinityRep)
        static AltDecBase Infinity;
        static AltDecBase NegativeInfinity;
        static AltDecBase ApproachingZero;
#endif

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase E;
        
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase OneGMillionth;

        //0e-7
        static AltDecBase TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static AltDecBase FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDecBase</returns>
        static AltDecBase NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static AltDecBase Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static AltDecBase Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static AltDecBase LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static AltDecBase LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static AltDecBase HalfLN10Mult;

    #if defined(AltNum_EnableNilRep)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static AltDecBase Nil;
    #endif
    
#if defined(AltNum_EnableApproachingValues)
        static AltDecBase ApproachingRightRealValue(int IntValue=0)
        {
            return AltDecBase(IntValue, 999999999);
        }

        static AltDecBase ApproachingLeftRealValue(int IntValue=0)
        {
            return AltDecBase(IntValue, 1);
        }

        static AltDecBase LeftAlmostPointFiveRealValue(int IntValue=0)
        {
            return AltDecBase(IntValue, 499999999);
        }

        static AltDecBase RightAlmostPointFiveRealValue(int IntValue=0)
        {
            return AltDecBase(IntValue, 500000001);
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
