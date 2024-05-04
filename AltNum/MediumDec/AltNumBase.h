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

//Main headers
#if defined(AltNum_UseBuiltinVirtualTable)
	#include "..\VirtualTableBase.h"
#endif

#pragma region PreprocessorToggles

#pragma endregion PreprocessorToggles

namespace BlazesRusCode
{//"Not used for this variant" comment used as placeholder between unused regions to help with code compare between variants and keep structure similar
//Going to try to only virtually inherit functions as needed to prevent accessing functions from previous parents 

	//Base class for AltDec and MediumDec to help initial structure of classes
    virtual class DLL_API AltNumBase
    {
#if !defined(AltNum_AvoidVirtualBase)
protected:
	#pragma region DigitStorage

	#pragma endregion DigitStorage

public:
	#pragma region Check_if_Zero
        //Is at either zero or negative zero IntHalf of AltNum
        //virtual bool IsAtZeroInt() = 0;
		
        //virtual bool IsNotAtZeroInt() = 0;
	#pragma endregion Check_if_Zero

	#pragma region class_constructors

	#pragma endregion class_constructors
protected:
	#pragma region Const Representation values

	#pragma endregion Const Representation values

	#pragma region RepType

	#pragma endregion RepType

public:
	#pragma region PiNum Setters

	#pragma endregion PiNum Setters

	#pragma region ENum Setters

	#pragma endregion ENum Setters

	#pragma region INum Setters

	#pragma endregion INum Setters

	#pragma region Fractional Setters
	//Used only in AltDec/MixedDec
	#pragma endregion Fractional Setters

	#pragma region MixedFrac Setters
	//Used only in AltDec/MixedDec
	#pragma endregion MixedFrac Setters

	#pragma region Infinity Setters

	#pragma endregion Infinity Setters

	#pragma region ApproachingZero Setters

	#pragma endregion ApproachingZero Setters

	#pragma region NaN Setters

	#pragma endregion NaN Setters

	#pragma region ValueDefines
    //Static functions that return values go here inside full class
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
	
	#pragma region I Conversion
	
	#pragma endregion I Conversion

	#pragma region Other RepType Conversion

	#pragma endregion Other RepType Conversion

	#pragma region NormalRep Integer division operations

	#pragma endregion NormalRep Integer division operations

	#pragma region NormalRep Integer Multiplication Operations

	#pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep Integer Addition Operations

	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations

	#pragma endregion NormalRep Integer Subtraction Operations

	#pragma region NormalRep AltNum division operations

	#pragma endregion NormalRep AltNum division operations

	#pragma region NormalRep AltNum Multiplication Operations

	#pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep AltNum Addition Operations

	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations

	#pragma endregion NormalRep AltNum Subtraction Operations

	#pragma region Mixed Fraction Operations
	//Used only in AltDec/MixedDec
	#pragma endregion Mixed Fraction Operations

	#pragma region Other division operations

	#pragma endregion Other division operations	

	#pragma region Other Multiplication Operations

	#pragma endregion Other Multiplication Operations

	#pragma region Other Addition Operations

	#pragma endregion Other Addition Operations

	#pragma region Other Subtraction Operations

	#pragma endregion Other Subtraction Operations

	#pragma region Modulus Operations
    //Modulus functions and overrides go here
	#pragma endregion Modulus Operations

	#pragma region Bitwise Operations
    //Bitwise functions and overrides go here
	#pragma endregion Bitwise Operations

    #pragma region Main Operator Overrides
    //Operator overrides for /*+- go here
    #pragma endregion Main Operator Overrides

    #pragma region Other Operators
    //Operators such as prefix operator go here
    #pragma region Other Operators

//Place Member version of Math based functions with post-fix:'Of' inside Base class
//With static versions of the math functions only in the full versions of the MediumDec variant classes
	#pragma region Math Etc Functions

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions

	#pragma endregion Log Functions

    #pragma region Trigonomic Etc Functions

    #pragma endregion Trigonomic Etc Functions

#endif
    };
    #pragma region ValueDefine Source

    #pragma endregion ValueDefine Source
	
    #pragma region String Function Source
	
    #pragma endregion String Function Source
}