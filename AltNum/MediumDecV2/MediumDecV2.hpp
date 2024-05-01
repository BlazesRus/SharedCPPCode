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

#include "AltNumModChecker.hpp"

#include "..\MediumDec\MediumDecBase.hpp"

/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
*/

namespace BlazesRusCode
{

    class MediumDecBase;
    //AltNumBase will be changed to instead derive from MediumDecV2Base
    // while storing most of flags inside of bitfield instead of ExtraRep

    /// <summary>
    /// Same representation range as MediumDec except
    /// includes Pi, E, and I Representations and things like infinity and approaching zero
    /// (Reduced version of AltDec class making usage of bitfields to make use of
    /// more of DecimalHalf storage of 4 bytes) 
    /// (8 Bytes stored for each instance)
	/// </summary>
    class DLL_API MediumDecV2 : public virtual MediumDecV2Base
    {
public:

		//Performs remainder/Mod operation then saves division result
		class DLL_API ModResult : public AltNumModChecker<MediumDecV2>{};

	#pragma region class_constructors
        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2(const int& intVal, const signed int& decVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
        }

        MediumDecV2(const MediumDecV2Base&) = default;

        MediumDecV2& operator=(const MediumDecV2& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

	#pragma endregion class_constructors

	#pragma region Check_if_Zero

        //Is at either zero or negative zero IntHalf of AltNum
        constexpr auto IsAtZeroInt = MediumDecBase::IsAtZeroInt;

        //alias function
        constexpr auto IsNotAtZeroInt = MediumDecBase::IsNotAtZeroInt;

        //Detect if at exactly zero
        constexpr auto IsZero = MediumDecBase::IsZero;

        constexpr auto SetAsZero = MediumDecBase::SetAsZero;

	#pragma endregion Check_if_Zero

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MediumDecV2 Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MediumDec Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        constexpr auto SwapNegativeStatus = MediumDecBase::SwapNegativeStatus;

protected:
    #pragma region Const Representation values

    #pragma endregion Const Representation values

    #pragma region RepType

        constexpr auto RepTypeAsString = MediumDecV2Base::RepTypeAsString;

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        constexpr auto GetRepType = MediumDecV2Base::GetRepType;

    #pragma endregion RepType

public:
    #pragma region PiNum Setters

    #pragma endregion PiNum Setters

    #pragma region ENum Setters

    #pragma endregion ENum Setters

	#pragma region INum Setters

	#pragma endregion INum Setters

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
//        constexpr auto ConvertToNormType = MediumDecV2Base::ConvertToNormType;
//
//		//Returns value as normal type representation
//        constexpr auto ConvertAsNormType = MediumDecV2Base::ConvertAsNormType;
//
//        //Converts value to normal type representation
//        constexpr auto ConvertToNormTypeV2 = MediumDecV2Base::ConvertToNormTypeV2;
//
//		//Returns value as normal type representation
//        constexpr auto ConvertAsNormTypeV2 = MediumDecV2Base::ConvertAsNormTypeV2;
	#pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #pragma endregion ConvertToOtherTypes

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

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntDivOpV1 = MediumDecBase::PartialUIntDivOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntDivOpV1 = MediumDecBase::PartialIntDivOpV1<IntType>;

        /// <summary>
        /// Basic Division Operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntDivOpV1 = MediumDecBase::BasicUIntDivOpV1<MediumDec, IntType>;

        /// <summary>
        /// Basic Division Operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntDivOpV1 = MediumDecBase::BasicIntDivOpV1<MediumDec, IntType>;
		
public:

        constexpr auto PartialUIntDivOpV1 = PartialUIntDivOpV1<unsigned int>;
        constexpr auto PartialIntDivOpV1 = PartialIntDivOpV1<signed int>;
        constexpr auto UnsignedPartialIntDivOpV1 = PartialUIntDivOpV1<signed int>;
        constexpr auto PartialUInt64DivOpV1 = PartialUIntDivOpV1<unsigned long long>;
        constexpr auto PartialInt64DivOpV1 = PartialIntDivOpV1<signed long long>;

        constexpr auto BasicUIntDivOp = BasicUIntDivOpV1<unsigned int>;
        constexpr auto BasicIntDivOp = BasicIntDivOpV1<const signed int>;
        constexpr auto UnsignedBasicIntDivOp = BasicUIntDivOpV1<signed int>;
        constexpr auto BasicUInt64DivOp = BasicUIntDivOpV1<unsigned long long>;
        constexpr auto BasicInt64DivOp = BasicIntDivOpV1<signed long long>;
        constexpr auto UnsignedBasicInt64DivOp = BasicUIntDivOpV1<signed long long>;
		
        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivideByTwo = MediumDecBase::DivideByTwo;

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivideByFour = MediumDecBase::DivideByFour;
		
protected:
        /// <summary>
        /// Division operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntDivOpV1 = MediumDecV2Base::IntDivOpV1<IntType>;

public:

        constexpr auto UIntDivOpV1 = MediumDecV2Base::UIntDivOpV1<unsigned int>;
        constexpr auto IntDivOpV1 = MediumDecV2Base::IntDivOpV1<signed int>;
        constexpr auto UnsignedBasicIntDivOp = MediumDecV2Base::UIntDivOpV1<signed int>;
        constexpr auto UInt64DivOp = MediumDecV2Base::UIntDivOpV1<unsigned long long>;
        constexpr auto Int64DivOp = MediumDecV2Base::IntDivOpV1<signed long long>;
	
        constexpr auto DivideByUInt = MediumDecV2Base::UIntDivV1<unsigned int>;
        constexpr auto DivideByInt = MediumDecV2Base::IntDivV1<signed int>;
        constexpr auto UnsignedDivideByInt = MediumDecV2Base::UIntDivV1<signed int>;
        constexpr auto DivideByUInt64 = MediumDecV2Base::UIntDivV1<unsigned long long>;
        constexpr auto DivideByInt64 = MediumDecV2Base::IntDivV1<signed long long>;
        constexpr auto UnsignedDivideByInt64 = MediumDecV2Base::UIntDivV1<signed long long>;

        /// <summary>
        /// Division Operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivOp = MediumDecV2Base::DivOp;

        /// <summary>
        /// Division Operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto DivideBy = MediumDecV2Base::DivideBy;

	#pragma endregion Other Division Operations	

	#pragma region Multiplication Operations
protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntMultOpV1 = MediumDecBase::PartialUIntMultOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntMultOpV1 = MediumDecBase::PartialIntMultOpV1<IntType>;

        /// <summary>
        /// Basic multiplication Operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntMultOpV1 = MediumDecBase::BasicUIntMultOpV1<MediumDec, IntType>;

        /// <summary>
        /// Basic multiplication Operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntMultOpV1 = MediumDecBase::BasicIntMultOpV1<MediumDec, IntType>;
		
public:

        constexpr auto PartialUIntMultOpV1 = PartialUIntMultOpV1<unsigned int>;
        constexpr auto PartialIntMultOpV1 = PartialIntMultOpV1<signed int>;
        constexpr auto UnsignedPartialIntMultOpV1 = PartialUIntMultOpV1<signed int>;
        constexpr auto PartialUInt64MultOpV1 = PartialUIntMultOpV1<unsigned long long>;
        constexpr auto PartialInt64MultOpV1 = PartialIntMultOpV1<signed long long>;

        constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<unsigned int>;
        constexpr auto BasicIntMultOp = BasicIntMultOpV1<const signed int>;
        constexpr auto UnsignedBasicIntMultOp = BasicUIntMultOpV1<signed int>;
        constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<unsigned long long>;
        constexpr auto BasicInt64MultOp = BasicIntMultOpV1<signed long long>;
        constexpr auto UnsignedBasicInt64MultOp = BasicUIntMultOpV1<signed long long>;
		
        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultipleByTwo = MediumDecV2Base::MultipleByTwo;

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultipleByFour = MediumDecV2Base::MultipleByFour;

protected:
        /// <summary>
        /// Multiplication operation between MediumDec and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntMultOpV1 = MediumDecV2Base::IntMultOpV1<IntType>;

public:

        constexpr auto UIntMultOpV1 = MediumDecV2Base::UIntMultOpV1<unsigned int>;
        constexpr auto IntMultOpV1 = MediumDecV2Base::IntMultOpV1<signed int>;
        constexpr auto UnsignedMediumDecV2Base::IntMultOp = MediumDecV2Base::UIntMultOpV1<signed int>;
        constexpr auto UInt64MultOp = MediumDecV2Base::UIntMultOpV1<unsigned long long>;
        constexpr auto Int64MultOp = MediumDecV2Base::IntMultOpV1<signed long long>;
	
        constexpr auto MultipleByUInt = MediumDecV2Base::UIntMultV1<unsigned int>;
        constexpr auto MultipleByInt = MediumDecV2Base::IntMultV1<signed int>;
        constexpr auto UnsignedMultipleByInt = MediumDecV2Base::UIntMultV1<signed int>;
        constexpr auto MultipleByUInt64 = MediumDecV2Base::UIntMultV1<unsigned long long>;
        constexpr auto MultipleByInt64 = MediumDecV2Base::IntMultV1<signed long long>;
        constexpr auto UnsignedMultipleByInt64 = MediumDecV2Base::UIntMultV1<signed long long>;

        /// <summary>
        /// Multiplication Operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultOp = MediumDecV2Base::MultOp;

        /// <summary>
        /// Multiplication Operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto MultipleBy = MediumDecV2Base::MultipleBy;

	#pragma endregion Multiplication Operations

	#pragma region Addition Operations
protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntAddOpV1 = MediumDecBase::PartialUIntAddOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntAddOpV1 = MediumDecBase::PartialIntAddOpV1<IntType>;

        /// <summary>
        /// Basic addition operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntAddOpV1 = MediumDecBase::BasicUIntAddOpV1<MediumDec, IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntAddOpV1 = MediumDecBase::BasicIntAddOpV1<MediumDec, IntType>;
		
public:

        constexpr auto PartialUIntAddOpV1 = PartialUIntAddOpV1<unsigned int>;
        constexpr auto PartialIntAddOpV1 = PartialIntAddOpV1<signed int>;
        constexpr auto UnsignedPartialIntAddOpV1 = PartialUIntAddOpV1<signed int>;
        constexpr auto PartialUInt64AddOpV1 = PartialUIntAddOpV1<unsigned long long>;
        constexpr auto PartialInt64AddOpV1 = PartialIntAddOpV1<signed long long>;

        constexpr auto BasicUIntAddOp = BasicUIntAddOpV1<unsigned int>;
        constexpr auto BasicIntAddOp = BasicIntAddOpV1<const signed int>;
        constexpr auto UnsignedBasicIntAddOp = BasicUIntAddOpV1<signed int>;
        constexpr auto BasicUInt64AddOp = BasicUIntAddOpV1<unsigned long long>;
        constexpr auto BasicInt64AddOp = BasicIntAddOpV1<signed long long>;
        constexpr auto UnsignedBasicInt64AddOp = BasicUIntAddOpV1<signed long long>;
		
protected:
        /// <summary>
        /// Addition operation Between MediumDec and Integer value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntAddOpV1 = MediumDecV2Base::IntAddOpV1<IntType>;

public:

        constexpr auto UIntAddOpV1 = MediumDecV2Base::UIntAddOpV1<unsigned int>;
        constexpr auto IntAddOpV1 = MediumDecV2Base::IntAddOpV1<signed int>;
        constexpr auto UnsignedMediumDecV2Base::IntAddOp = MediumDecV2Base::UIntAddOpV1<signed int>;
        constexpr auto UInt64AddOp = MediumDecV2Base::UIntAddOpV1<unsigned long long>;
        constexpr auto Int64AddOp = MediumDecV2Base::IntAddOpV1<signed long long>;
	
        constexpr auto AddByUInt = MediumDecV2Base::UIntAddV1<unsigned int>;
        constexpr auto AddByInt = MediumDecV2Base::IntAddV1<signed int>;
        constexpr auto UnsignedAddByInt = MediumDecV2Base::UIntAddV1<signed int>;
        constexpr auto AddByUInt64 = MediumDecV2Base::UIntAddV1<unsigned long long>;
        constexpr auto AddByInt64 = MediumDecV2Base::IntAddV1<signed long long>;
        constexpr auto UnsignedAddByInt64 = MediumDecV2Base::UIntAddV1<signed long long>;

        /// <summary>
        /// Addition operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto AddOp = MediumDecV2Base::AddOp;

        /// <summary>
        /// Addition operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto AddBy = MediumDecV2Base::AddBy;

	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations
protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntSubOpV1 = MediumDecBase::PartialUIntSubOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntSubOpV1 = MediumDecBase::PartialIntSubOpV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntSubOpV1 = MediumDecBase::BasicUIntSubOpV1<MediumDec, IntType>;

        /// <summary>
        /// Basic subtraction operation between MediumDec and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntSubOpV1 = MediumDecBase::BasicIntSubOpV1<MediumDec, IntType>;
		
public:

        constexpr auto PartialUIntSubOpV1 = PartialUIntSubOpV1<unsigned int>;
        constexpr auto PartialIntSubOpV1 = PartialIntSubOpV1<signed int>;
        constexpr auto UnsignedPartialIntSubOpV1 = PartialUIntSubOpV1<signed int>;
        constexpr auto PartialUInt64SubOpV1 = PartialUIntSubOpV1<unsigned long long>;
        constexpr auto PartialInt64SubOpV1 = PartialIntSubOpV1<signed long long>;

        constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<unsigned int>;
        constexpr auto BasicIntSubOp = BasicIntSubOpV1<const signed int>;
        constexpr auto UnsignedBasicIntSubOp = BasicUIntSubOpV1<signed int>;
        constexpr auto BasicUInt64SubOp = BasicUIntSubOpV1<unsigned long long>;
        constexpr auto BasicInt64SubOp = BasicIntSubOpV1<signed long long>;
        constexpr auto UnsignedBasicInt64SubOp = BasicUIntSubOpV1<signed long long>;
		
protected:
        /// <summary>
        /// Subtraction operation between MediumDec and Integer value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= int>
        constexpr auto IntSubOpV1 = MediumDecV2Base::IntSubOpV1<IntType>;

public:

        constexpr auto UIntSubOpV1 = MediumDecV2Base::UIntSubOpV1<unsigned int>;
        constexpr auto IntSubOpV1 = MediumDecV2Base::IntSubOpV1<signed int>;
        constexpr auto UnsignedMediumDecV2Base::IntSubOp = MediumDecV2Base::UIntSubOpV1<signed int>;
        constexpr auto UInt64SubOp = MediumDecV2Base::UIntSubOpV1<unsigned long long>;
        constexpr auto Int64SubOp = MediumDecV2Base::IntSubOpV1<signed long long>;
	
        constexpr auto SubByUInt = MediumDecV2Base::UIntSubV1<unsigned int>;
        constexpr auto SubByInt = MediumDecV2Base::IntSubV1<signed int>;
        constexpr auto UnsignedSubByInt = MediumDecV2Base::UIntSubV1<signed int>;
        constexpr auto SubByUInt64 = MediumDecV2Base::UIntSubV1<unsigned long long>;
        constexpr auto SubByInt64 = MediumDecV2Base::IntSubV1<signed long long>;
        constexpr auto UnsignedSubByInt64 = MediumDecV2Base::UIntSubV1<signed long long>;

        /// <summary>
        /// Subtraction operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto SubOp = MediumDecV2Base::SubOp;

        /// <summary>
        /// Subtraction operation between MediumDec values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        constexpr auto SubBy = MediumDecV2Base::SubBy;

	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations

        /// <summary>
        /// Modulus Operation(Returning inside
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        friend ModResult operator%(const MediumDecV2& LValue, const MediumDecV2& RValue)
		{
			return ModResult(LValue, RValue);
		}

        friend MediumDec& operator%=(MediumDecV2& LValue, const MediumDecV2& RValue)
		{ 
            MediumDec divRes = LValue / RValue;
            LValue -= RValue * divRes;
			return *this;
		}

	#pragma endregion Modulus Operations

    #pragma region Main Operator Overrides
    //Operator overrides for /*+- go here
    #pragma endregion Main Operator Overrides

    #pragma region Other Operators
    //Operators such as prefix operator go here
    #pragma region Other Operators

    #pragma region Math/Trigonomic Etc Functions

    #pragma endregion Math/Trigonomic Etc Functions
    };
    #pragma region String Function Source

    #pragma endregion String Function Source

#if defined(AltNum_EnableAlternativeModulusResult)
    /// <summary>
    /// (MediumDecBase Version)Performs remainder operation then saves division result
    /// C = A - B * (A / B)
    /// </summary>
    class DLL_API AltModChecker : public AltNumModChecker<MediumDecBase>
    {
    };
#endif
}