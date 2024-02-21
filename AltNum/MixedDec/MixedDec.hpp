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
#include "..\..\DLLAPI.h"
#endif

#pragma region PreprocessorToggles
#include "MixedDecPreprocessors.h"
#pragma endregion PreprocessorToggles

#ifdef MixedDec_DeriveFromAltDec
	#include "..\AltDec\AltDecBase.hpp"
#elif MixedDec_DeriveFromFlaggedDec
	#include "..\FlaggedDec\FlaggedDecBase.hpp"
#else
	#include "..\MediumDec\MediumDecBase.hpp"
#endif

namespace BlazesRusCode
{

    class MixedDec;
	
	//Hybrid fixed point storage with trailing digits stored as float
	// (or optionally larger floating point models if preprocessor flag is set to toggle)
	class DLL_API MixedDec :
#ifdef MixedDec_DeriveFromAltDec
    public virtual AltDecBase
#else
    public virtual MediumDecBase
#endif
    {
protected:
#pragma region DigitStorage
		float TrailingDigits;
#pragma endregion DigitStorage

public:
#pragma region Check_if_Zero
#pragma endregion Check_if_Zero

#pragma region class_constructors
        /// <summary>
        /// Initializes a new instance of the <see cref="MixedDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
#if defined(MixedDec_DeriveFromAltDec)&&!defined(AltDec_UseMirroredInt)
		MixedDec(const int& intVal, const signed int& decVal = 0, const signed int& extraVal = 0, float trailingDigits = 0.0f)
#elif defined(MixedDec_DeriveFromAltDec)
        MixedDec(const MirroredInt& intVal = MirroredInt::Zero, const signed int& decVal = 0, const signed int& extraVal = 0, float trailingDigits = 0.0f)
#elif defined(AltDec_UseMirroredInt)
        MixedDec(MirroredInt intVal, signed int decVal = 0, float trailingDigits = 0.0f)
#else
        MixedDec(int intVal, signed int decVal = 0, float trailingDigits = 0.0f)
#endif
        {
            IntValue.Value = intVal;
            DecimalHalf = decVal;
	#if defined(MixedDec_DeriveFromAltDec)
			ExtraRep = extraVal;
	#endif
			TrailingDigits = trailingDigits;
        }

        MixedDec(const MixedDec&) = default;

        MixedDec& operator=(const MediumDec&) = default;
#pragma endregion class_constructors

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MixedDec Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
	#if defined(MixedDec_DeriveFromAltDec)
			ExtraRep = extraVal;
	#endif
			TrailingDigits = trailingDigits;
        }
		
protected:

    #pragma region Const Representation values

    #pragma endregion Const Representation values

    #pragma region PiNum Setters

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

    #pragma endregion ApproachingZero Setters

    #pragma region NaN Setters

    #pragma endregion NaN Setters

    #pragma region ValueDefines

    #pragma endregion ValueDefines

    #pragma region String Commands
	
    #pragma endregion String Commands

    #pragma region From this type to Standard types

    #pragma endregion From this type to Standard types

    #pragma region MirroredIntBased Operations

    #pragma endregion MirroredIntBased Operations

    #pragma region Pi Conversion
	
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion

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
    }
    #pragma region String Function Source
}