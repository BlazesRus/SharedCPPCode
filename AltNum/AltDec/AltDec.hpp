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

        #pragma endregion Const Representation values

		#pragma endregion PiNum Setters

		#pragma region ENum Setters

		#pragma endregion ENum Setters

		#pragma region Fractional Setters

		#pragma endregion Fractional Setters

		#pragma region MixedFrac Setters

		#pragma endregion MixedFrac Setters

		#pragma region Infinity Setters

		#pragma endregion Infinity Setters

		#pragma region ApproachingZero Setters

		#pragma endregion NaN Setters

		#pragma region ValueDefines
		
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
