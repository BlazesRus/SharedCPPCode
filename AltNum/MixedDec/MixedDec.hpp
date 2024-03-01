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
	class DLL_API MixedDec:
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
        //Detect if at exactly zero
		bool IsZero() const
		{
            return DecimalHalf==0&&IntValue==0&&TrailingDigits==0;
		}

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0;
	#if defined(MixedDec_DeriveFromAltDec)
			ExtraRep = 0;
	#endif
			TrailingDigits = 0.0f;	
        }
#pragma endregion Check_if_Zero

        bool IsWholeNumber()
        {
#if defined(MixedDec_DeriveFromAltDec)||defined(MixedDec_DeriveFromFlaggedDec)

#else
            return DecimalHalf==0 && TrailingDigits==0.0f;
#endif
        }

        bool IsAtOrBelowTwo()
        {
#if defined(MixedDec_DeriveFromAltDec)||defined(MixedDec_DeriveFromFlaggedDec)

#else
            if(IntValue<=1)
                return true;
            else
                return IntValue==2 && DecimalHalf==0 && TrailingDigits==0.0f;
#endif
        }

        bool IsAtOrBelowOne()
        {
#if defined(MixedDec_DeriveFromAltDec)||defined(MixedDec_DeriveFromFlaggedDec)

#else
            if(IntValue<=0)
                return true;
            else
                return IntValue==1 && DecimalHalf==0 && TrailingDigits==0.0f;
#endif
        }

#pragma region class_constructors
        /// <summary>
        /// Initializes a new instance of the <see cref="MixedDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
#if defined(AltDec_UseMirroredInt)
	#if defined(MixedDec_DontInitializeFromConstRef)
        MixedDec(MirroredInt intVal, signed int decVal = 0
	#else
        MixedDec(const MirroredInt& intVal, const signed int& decVal = 0
	#endif
#else
	#if defined(MixedDec_DontInitializeFromConstRef)
        MixedDec(signed int intVal, signed int decVal = 0
	#else
        MixedDec(const int& intVal, signed int decVal = 0
	#endif
#endif
#if defined(MixedDec_ExtraRepEnabled)
	    #if defined(MixedDec_DontInitializeFromConstRef)
        , signed int extraVal = 0
	    #else
        , const int& extraVal = 0
	    #endif
#endif
#if defined(MixedDec_DeriveFromFlaggedDec)
	#if defined(MixedDec_DontInitializeFromConstRef)
        #if !defined(FlaggedNum_UseBitset)
        unsigned char flagsActive=0
        #else
        std::bitset<6> flagsActive = {})
        #endif
	#else
        #if !defined(FlaggedNum_UseBitset)
        const unsigned char& flagsActive=0
        #else
        const std::bitset<6>& flagsActive = {})
        #endif
	#endif
#endif
		, float trailingDigits = 0.0f)
        {
            IntValue.Value = intVal;
            DecimalHalf = decVal;
#if defined(MixedDec_ExtraRepEnabled)
			ExtraRep = extraVal;
#endif
#if defined(MixedDec_DeriveFromFlaggedDec)
			FlagsActive = flagsActive;
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
#if defined(MixedDec_ExtraRepEnabled)
			ExtraRep = extraVal;
#endif
#if defined(MixedDec_DeriveFromFlaggedDec)
			FlagsActive = flagsActive;
#endif
			TrailingDigits = trailingDigits;
        }
		
protected:
    #pragma region RepType

    #pragma endregion RepType

    #pragma region Const Representation values

    #pragma endregion Const Representation values

public:

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

    #pragma region ConvertFromOtherTypes

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #pragma endregion ConvertToOtherTypes

    #pragma region MirroredIntBased Operations

    #pragma endregion MirroredIntBased Operations

    #pragma region Pi Conversion
	
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion

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
    }
    #pragma region String Function Source
}