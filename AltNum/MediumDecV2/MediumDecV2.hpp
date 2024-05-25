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

	#pragma region Mixed Fraction Operations

	#pragma endregion Mixed Fraction Operations

	#pragma region division operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntDivOpV1 = MediumDecBase::PartialUIntDivOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntDivOpV1 = MediumDecBase::PartialIntDivOpV1<IntType>;

        /// <summary>
        /// Basic division operation between MediumDecV2 and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntDivOpV1 = MediumDecBase::BasicUIntDivOpV1<IntType>;

        /// <summary>
        /// Basic division operation between MediumDecV2 and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntDivOpV1 = MediumDecBase::BasicIntDivOpV1<IntType>;

        /// <summary>
        /// Basic division operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicDivideByUIntV1 = MediumDecBase::BasicDivideByIntV1<IntType>;

        /// <summary>
        /// Basic division operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicDivideByIntV1 = MediumDecBase::BasicDivideByIntV1<IntType>;
    		
public:

        constexpr auto PartialUIntDivOpV1 = PartialUIntDivOpV1<unsigned int>;
        constexpr auto PartialIntDivOpV1 = PartialIntDivOpV1<signed int>;
        constexpr auto UnsignedPartialIntDivOpV1 = PartialUIntDivOpV1<signed int>;
        constexpr auto PartialUInt64DivOpV1 = PartialUIntDivOpV1<UInt64>;
        constexpr auto PartialInt64DivOpV1 = PartialIntDivOpV1<Int64>;

        constexpr auto BasicUIntDivOp = BasicUIntDivOpV1<unsigned int>;
        constexpr auto BasicIntDivOp = BasicIntDivOpV1<signed int>;
        constexpr auto UnsignedBasicIntDivOp = BasicUIntDivOpV1<signed int>;
        constexpr auto BasicUInt64DivOp = BasicUIntDivOpV1<UInt64>;
        constexpr auto BasicInt64DivOp = BasicIntDivOpV1<Int64>;
        constexpr auto UnsignedBasicInt64DivOp = BasicUIntDivOpV1<signed int>;

        constexpr auto BasicUInt8DivOp = BasicUIntDivOpV1<unsigned char>;
        constexpr auto BasicInt8DivOp = BasicIntDivOpV1<signed char>;
        constexpr auto BasicUInt16DivOp = BasicUIntDivOpV1<unsigned short>;
        constexpr auto BasicInt16DivOp = BasicIntDivOpV1<signed short>;

        constexpr auto BasicDivideByUInt = BasicDivideByUIntV1<unsigned int>;
        constexpr auto BasicDivideByInt = BasicDivideByIntV1<signed int>;
        constexpr auto UnsignedBasicDivideByInt = BasicDivideByUIntV1<signed int>;
        constexpr auto BasicDivideByUInt64 = BasicDivideByUIntV1<UInt64>;
        constexpr auto BasicDivideByInt64 = BasicDivideByIntV1<Int64>;
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
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto DivideByTwo = MediumDecV2Base::DivideByTwo;

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto DivideByFour = MediumDecV2Base::DivideByFour;
		
protected:

        /// <summary>
        /// Division operation between MediumDecV2 and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntDivOpV1 = MediumDecV2Base::UIntDivOpV1<IntType>;

        /// <summary>
        /// Division operation between MediumDecV2 and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntDivOpV1 = MediumDecV2Base::IntDivOpV1<IntType>;
		
        /// <summary>
        /// Division operation between MediumDecV2 and unsigned Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto DivideByUIntV1 = MediumDecV2Base::UIntDivOpV1<IntType>;

        /// <summary>
        /// Division operation between MediumDecV2 and Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2</returns>
        template<IntegerType IntType= signed int>
        constexpr auto DivideByIntV1 = MediumDecV2Base::IntDivOpV1<IntType>;

public:

        constexpr auto UIntDivOp = MediumDecV2Base::UIntDivOp;
        constexpr auto IntDivOp = MediumDecV2Base::IntDivOp;
        constexpr auto UnsignedBasicIntDivOp = MediumDecV2Base::UnsignedBasicIntDivOp;
        constexpr auto UInt64DivOp = MediumDecV2Base::UInt64DivOp;
        constexpr auto Int64DivOp = MediumDecV2Base::Int64DivOp;
        constexpr auto UnsignedBasicIntDivOp = MediumDecV2Base::UnsignedBasicIntDivOp;
	
        constexpr auto UInt8DivOpV1 = MediumDecV2Base::UInt8DivOpV1;
        constexpr auto Int8DivOpV1 = MediumDecV2Base::Int8DivOpV1;
        constexpr auto UnsignedInt8DivOp = MediumDecV2Base::UnsignedInt8DivOp;
        constexpr auto UInt16DivOp = MediumDecV2Base::UInt16DivOp;
        constexpr auto Int16DivOp = MediumDecV2Base::Int16DivOp;
		constexpr auto UnsignedInt16DivOp = MediumDecV2Base::UnsignedInt16DivOp;
	
        constexpr auto DivideByUInt = MediumDecV2Base::DivideByUInt;
        constexpr auto DivideByInt = MediumDecV2Base::DivideByInt;
        constexpr auto UnsignedDivideByInt = MediumDecV2Base::UnsignedDivideByInt;
        constexpr auto DivideByUInt64 = MediumDecV2Base::DivideByUInt64;
        constexpr auto DivideByInt64 = MediumDecV2Base::DivideByInt64;
        constexpr auto UnsignedDivideByInt64 = MediumDecV2Base::UnsignedDivideByInt64;
		
        constexpr auto DivideByUInt8 = MediumDecV2Base::DivideByUInt8;
        constexpr auto DivideByInt8 = MediumDecV2Base::DivideByInt8;
        constexpr auto DivideByUInt16 = MediumDecV2Base::DivideByUInt16;
        constexpr auto DivideByInt16 = MediumDecV2Base::DivideByInt16;

protected:

        constexpr auto PartialDivOp = MediumDecV2Base::PartialDivOp;
        constexpr auto UnsignedPartialDivOp = MediumDecV2Base::UnsignedPartialDivOp;

public:
	
		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedDivOp = MediumDecV2Base::BasicUnsignedDivOp;

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivOp = MediumDecV2Base::BasicDivOp;
    
		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedDivision = MediumDecV2Base::BasicUnsignedDivision;

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivision = MediumDecV2Base::BasicDivision;

        /// <summary>
        /// Division operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto UnsignedDivOp = MediumDecV2Base::BasicUnsignedDivOp;

        /// <summary>
        /// Division operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto DivOp = MediumDecV2Base::BasicDivOp;

        /// <summary>
        /// Division operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto DivideByUnsigned = MediumDecV2Base::BasicUnsignedDivision;

        /// <summary>
        /// Division operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto DivideBy = MediumDecV2Base::BasicDivision;

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator/(const MediumDecV2& self, const MediumDecV2& Value) { return self.DivideBy(Value); }
		
        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator/=(MediumDecV2& self, const MediumDecV2& Value) { return self.DivOp(Value); }
		
        /// <summary>
        /// Division operation between MediumDecV2 and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator/(const MediumDecV2& self, const signed int& Value) { return self.DivideByInt(Value); }
        friend MediumDecV2 operator/(const MediumDecV2& self, const Int64& Value) { return self.DivideByInt64(Value); }
        friend MediumDecV2 operator/(const MediumDecV2& self, const unsigned int& Value) { return self.DivideByUInt(Value); }
        friend MediumDecV2 operator/(const MediumDecV2& self, const UInt64& Value) { return self.DivideByUInt64(Value); }
		
        friend MediumDecV2 operator/(const signed int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const Int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const UInt64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }

        friend MediumDecV2 operator/(const MediumDecV2& self, const signed char& Value) { return self.DivideByInt8(Value); }
        friend MediumDecV2 operator/(const MediumDecV2& self, const signed short& Value) { return self.DivideByInt16(Value); }
        friend MediumDecV2 operator/(const MediumDecV2& self, const unsigned char& Value) { return self.DivideByUInt8(Value); }
        friend MediumDecV2 operator/(const MediumDecV2& self, const unsigned short& Value) { return self.DivideByUInt16(Value); }

        friend MediumDecV2 operator/(const signed char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const signed short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }


        /// <summary>
        /// *= operation between MediumDecV2 and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator/=(MediumDecV2& self, const signed int& Value) { return self.IntDivOp(Value); }
        friend MediumDecV2& operator/=(MediumDecV2& self, const Int64& Value) { return self.Int64DivOp(Value); }
        friend MediumDecV2& operator/=(MediumDecV2& self, const unsigned int& Value) { return self.UIntDivOp(Value); }
        friend MediumDecV2& operator/=(MediumDecV2& self, const UInt64& Value) { return self.UInt64DivOp(Value); }

        friend MediumDecV2& operator/=(MediumDecV2& self, const signed char& Value) { return self.Int8DivOp(Value); }
        friend MediumDecV2& operator/=(MediumDecV2& self, const signed short& Value) { return self.Int16DivOp(Value); }
        friend MediumDecV2& operator/=(MediumDecV2& self, const unsigned char& Value) { return self.UInt8DivOp(Value); }
        friend MediumDecV2& operator/=(MediumDecV2& self, const unsigned short& Value) { return self.UInt16DivOp(Value); }

	#pragma endregion division operations

	#pragma region Multiplication Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntMultOpV1 = MediumDecV2Base::PartialUIntMultOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntMultOpV1 = MediumDecV2Base::PartialIntMultOpV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDecV2 and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntMultOpV1 = MediumDecV2Base::BasicUIntMultOpV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDecV2 and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntMultOpV1 = MediumDecV2Base::BasicIntMultOpV1<IntType>;
	
        /// <summary>
        /// Basic multiplication operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicMultipleByUIntV1 = MediumDecV2Base::BasicMultipleByUIntV1<IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicMultipleByIntV1 = MediumDecV2Base::BasicMultipleByIntV1<IntType>;
    	
public:

        constexpr auto PartialUIntMultOpV1 = PartialUIntMultOpV1<unsigned int>;
        constexpr auto PartialIntMultOpV1 = PartialIntMultOpV1<signed int>;
        constexpr auto UnsignedPartialIntMultOpV1 = PartialUIntMultOpV1<signed int>;
        constexpr auto PartialUInt64MultOpV1 = PartialUIntMultOpV1<UInt64>;
        constexpr auto PartialInt64MultOpV1 = PartialIntMultOpV1<Int64>;

        constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<unsigned int>;
        constexpr auto BasicIntMultOp = BasicIntMultOpV1<signed int>;
        constexpr auto UnsignedBasicIntMultOp = BasicUIntMultOpV1<signed int>;
        constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<UInt64>;
        constexpr auto BasicInt64MultOp = BasicIntMultOpV1<Int64>;
        constexpr auto UnsignedBasicInt64MultOp = BasicUIntMultOpV1<signed int>;

        constexpr auto BasicUInt8MultOp = BasicUIntMultOpV1<unsigned char>;
        constexpr auto BasicInt8MultOp = BasicIntMultOpV1<signed char>;
        constexpr auto BasicUInt16MultOp = BasicUIntMultOpV1<unsigned short>;
        constexpr auto BasicInt16MultOp = BasicIntMultOpV1<signed short>;

        constexpr auto BasicMultipleByUInt = BasicMultipleByUIntV1<unsigned int>;
        constexpr auto BasicMultipleByInt = BasicMultipleByIntV1<signed int>;
        constexpr auto UnsignedBasicMultipleByInt = BasicMultipleByUIntV1<signed int>;
        constexpr auto BasicMultipleByUInt64 = BasicMultipleByUIntV1<UInt64>;
        constexpr auto BasicMultipleByInt64 = BasicMultipleByIntV1<Int64>;
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
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto MultipleByTwo = MediumDecV2Base::MultipleByTwo;

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto MultipleByFour = MediumDecV2Base::MultipleByFour;
		
protected:

        /// <summary>
        /// Multiplication operation between MediumDecV2 and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntMultOpV1 = MediumDecV2Base::UIntMultOpV1<IntType>;

        /// <summary>
        /// Multiplication operation between MediumDecV2 and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntMultOpV1 = MediumDecV2Base::IntMultOpV1<IntType>;
		
        /// <summary>
        /// Multiplication operation between MediumDecV2 and unsigned Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto MultiplyByUIntV1 = MediumDecV2Base::UIntDivOpV1<IntType>;

        /// <summary>
        /// Multiplication operation between MediumDecV2 and Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto MultiplyByIntV1 = MediumDecV2Base::IntDivOpV1<IntType>;

public:

        constexpr auto UIntMultOp = MediumDecV2Base::UIntMultOp;
        constexpr auto IntMultOp = MediumDecV2Base::IntMultOp;
        constexpr auto UnsignedBasicIntMultOp = MediumDecV2Base::UnsignedBasicIntMultOp;
        constexpr auto UInt64MultOp = MediumDecV2Base::UInt64MultOp;
        constexpr auto Int64MultOp = MediumDecV2Base::Int64MultOp;
        constexpr auto UnsignedBasicIntMultOp = MediumDecV2Base::UnsignedBasicIntMultOp;
	
        constexpr auto UInt8MultOpV1 = MediumDecV2Base::UInt8MultOpV1;
        constexpr auto Int8MultOpV1 = MediumDecV2Base::Int8MultOpV1;
        constexpr auto UnsignedInt8MultOp = MediumDecV2Base::UnsignedInt8MultOp;
        constexpr auto UInt16MultOp = MediumDecV2Base::UInt16MultOp;
        constexpr auto Int16MultOp = MediumDecV2Base::Int16MultOp;
		constexpr auto UnsignedInt16MultOp = MediumDecV2Base::UnsignedInt16MultOp;
	
        constexpr auto MultiplyByUInt = MediumDecV2Base::MultiplyByUInt;
        constexpr auto MultiplyByInt = MediumDecV2Base::MultiplyByInt;
        constexpr auto UnsignedMultiplyByInt = MediumDecV2Base::UnsignedMultiplyByInt;
        constexpr auto MultiplyByUInt64 = MediumDecV2Base::MultiplyByUInt64;
        constexpr auto MultiplyByInt64 = MediumDecV2Base::MultiplyByInt64;
        constexpr auto UnsignedMultiplyByInt64 = MediumDecV2Base::UnsignedMultiplyByInt64;
		
        constexpr auto MultiplyByUInt8 = MediumDecV2Base::MultiplyByUInt8;
        constexpr auto MultiplyByInt8 = MediumDecV2Base::MultiplyByInt8;
        constexpr auto MultiplyByUInt16 = MediumDecV2Base::MultiplyByUInt16;
        constexpr auto MultiplyByInt16 = MediumDecV2Base::MultiplyByInt16;

protected:

        constexpr auto PartialMultOp = MediumDecV2Base::PartialMultOp;
        constexpr auto UnsignedPartialMultOp = MediumDecV2Base::UnsignedPartialMultOp;

public:
		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDecV2Base
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedMultOp = MediumDecV2Base::BasicUnsignedMultOp;

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDecV2Base
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicMultOp = MediumDecV2Base::BasicMultOp;

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedMultiplication = MediumDecV2Base::BasicUnsignedMultiplication;

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicMultiplication = MediumDecV2Base::BasicMultiplication;

        /// <summary>
        /// Multiplication operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto UnsignedMultOp = MediumDecV2Base::BasicUnsignedMultOp;

        /// <summary>
        /// Multiplication operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto MultOp = MediumDecV2Base::BasicMultOp;

        /// <summary>
        /// Multiplication operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto MultipleByUnsigned = MediumDecV2Base::BasicUnsignedMultiplication;

        /// <summary>
        /// Multiplication operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto MultipleBy = MediumDecV2Base::BasicMultiplication;

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator*(const MediumDecV2& self, const MediumDecV2& Value) { return self.MultipleBy(Value); }

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator*=(MediumDecV2& self, const MediumDecV2& Value) { return self.MultOp(Value); }
		
        /// <summary>
        /// Multiplication operation between MediumDecV2 and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator*(const MediumDecV2& self, const signed int& Value) { return self.MultiplyByInt(Value); }
        friend MediumDecV2 operator*(const MediumDecV2& self, const Int64& Value) { return self.MultiplyByInt64(Value); }
        friend MediumDecV2 operator*(const MediumDecV2& self, const unsigned int& Value) { return self.MultiplyByUInt(Value); }
        friend MediumDecV2 operator*(const MediumDecV2& self, const UInt64& Value) { return self.MultiplyByUInt64(Value); }
		
        friend MediumDecV2 operator*(const signed int& lValue, const MediumDecV2& rValue) { return rValue.MultiplyByInt(lValue); }
        friend MediumDecV2 operator*(const Int64& lValue, const MediumDecV2& rValue) { return rValue.MultiplyByInt64(lValue); }
        friend MediumDecV2 operator*(const unsigned int& lValue, const MediumDecV2& rValue) { return rValue.MultiplyByUInt(lValue); }
        friend MediumDecV2 operator*(const UInt64& lValue, const MediumDecV2& rValue) { return rValue.MultiplyByUInt64(lValue); }

        friend MediumDecV2 operator*(const MediumDecV2& self, const signed char& Value) { return self.MultiplyByInt8(Value); }
        friend MediumDecV2 operator*(const MediumDecV2& self, const signed short& Value) { return self.MultiplyByInt16(Value); }
        friend MediumDecV2 operator*(const MediumDecV2& self, const unsigned char& Value) { return self.MultiplyByUInt8(Value); }
        friend MediumDecV2 operator*(const MediumDecV2& self, const unsigned short& Value) { return self.MultiplyByUInt16(Value); }

        friend MediumDecV2 operator*(const signed char& lValue, const MediumDecV2& rValue) { return rValue.MultiplyByInt8(lValue); }
        friend MediumDecV2 operator*(const signed short& lValue, const MediumDecV2& rValue) { return rValue.MultiplyByInt16(lValue); }
        friend MediumDecV2 operator*(const unsigned char& lValue, const MediumDecV2& rValue) { return rValue.MultiplyByUInt8(lValue); }
        friend MediumDecV2 operator*(const unsigned short& lValue, const MediumDecV2& rValue) { return rValue.MultiplyByUInt16(lValue); }

        /// <summary>
        /// *= operation between MediumDecV2 and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator*=(MediumDecV2& self, const signed int& Value) { return self.IntMultOp(Value); }
        friend MediumDecV2& operator*=(MediumDecV2& self, const Int64& Value) { return self.Int64MultOp(Value); }
        friend MediumDecV2& operator*=(MediumDecV2& self, const unsigned int& Value) { return self.UIntMultOp(Value); }
        friend MediumDecV2& operator*=(MediumDecV2& self, const UInt64& Value) { return self.UInt64MultOp(Value); }

        friend MediumDecV2& operator*=(MediumDecV2& self, const signed char& Value) { return self.Int8MultOp(Value); }
        friend MediumDecV2& operator*=(MediumDecV2& self, const signed short& Value) { return self.Int16MultOp(Value); }
        friend MediumDecV2& operator*=(MediumDecV2& self, const unsigned char& Value) { return self.UInt8MultOp(Value); }
        friend MediumDecV2& operator*=(MediumDecV2& self, const unsigned short& Value) { return self.UInt16MultOp(Value); }

	#pragma endregion Multiplication Operations

	#pragma region Addition Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntAddOpV1 = MediumDecV2Base::PartialUIntAddOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntAddOpV1 = MediumDecV2Base::PartialIntAddOpV1<IntType>;

        /// <summary>
        /// Basic addition operation between MediumDecV2 and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntAddOpV1 = MediumDecV2Base::BasicUIntAddOpV1<IntType>;

        /// <summary>
        /// Basic addition operation between MediumDecV2 and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntAddOpV1 = MediumDecV2Base::BasicIntAddOpV1<IntType>;
	
        /// <summary>
        /// Basic addition operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicAddByUIntV1 = MediumDecV2Base::BasicAddByUIntV1<IntType>;

        /// <summary>
        /// Basic addition operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicAddByIntV1 = MediumDecV2Base::BasicAddByIntV1<IntType>;
    	
public:

        constexpr auto PartialUIntAddOpV1 = PartialUIntAddOpV1<unsigned int>;
        constexpr auto PartialIntAddOpV1 = PartialIntAddOpV1<signed int>;
        constexpr auto UnsignedPartialIntAddOpV1 = PartialUIntAddOpV1<signed int>;
        constexpr auto PartialUInt64AddOpV1 = PartialUIntAddOpV1<UInt64>;
        constexpr auto PartialInt64AddOpV1 = PartialIntAddOpV1<Int64>;

        constexpr auto BasicAddByUInt = BasicAddByUIntV1<unsigned int>;
        constexpr auto BasicAddByInt = BasicAddByIntV1<signed int>;
        constexpr auto UnsignedBasicAddByInt = BasicAddByUIntV1<signed int>;
        constexpr auto BasicAddByUInt64 = BasicAddByUIntV1<UInt64>;
        constexpr auto BasicAddByInt64 = BasicAddByIntV1<Int64>;
        constexpr auto UnsignedBasicAddByInt64 = BasicAddByUIntV1<signed int>;

        constexpr auto BasicAddByUInt8 = BasicAddByUIntV1<unsigned char>;
        constexpr auto BasicAddByInt8 = BasicAddByIntV1<signed char>;
        constexpr auto BasicAddByUInt16 = BasicAddByUIntV1<unsigned short>;
        constexpr auto BasicAddByInt16 = BasicAddByIntV1<signed short>;
		
protected:

        /// <summary>
        /// Addition operation between MediumDecV2 and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntAddOpV1 = MediumDecV2Base::UIntAddOpV1<IntType>;

        /// <summary>
        /// Addition operation between MediumDecV2 and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntAddOpV1 = MediumDecV2Base::IntAddOpV1<IntType>;
		
        /// <summary>
        /// Addition operation between MediumDecV2 and unsigned Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto AddByUIntV1 = MediumDecV2Base::UIntDivOpV1<IntType>;

        /// <summary>
        /// Addition operation between MediumDecV2 and Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2</returns>
        template<IntegerType IntType= signed int>
        constexpr auto AddByIntV1 = MediumDecV2Base::IntDivOpV1<IntType>;

public:

        constexpr auto UIntAddOp = MediumDecV2Base::UIntAddOp;
        constexpr auto IntAddOp = MediumDecV2Base::IntAddOp;
        constexpr auto UnsignedBasicIntAddOp = MediumDecV2Base::UnsignedBasicIntAddOp;
        constexpr auto UInt64AddOp = MediumDecV2Base::UInt64AddOp;
        constexpr auto Int64AddOp = MediumDecV2Base::Int64AddOp;
        constexpr auto UnsignedBasicIntAddOp = MediumDecV2Base::UnsignedBasicIntAddOp;
	
        constexpr auto UInt8AddOpV1 = MediumDecV2Base::UInt8AddOpV1;
        constexpr auto Int8AddOpV1 = MediumDecV2Base::Int8AddOpV1;
        constexpr auto UnsignedInt8AddOp = MediumDecV2Base::UnsignedInt8AddOp;
        constexpr auto UInt16AddOp = MediumDecV2Base::UInt16AddOp;
        constexpr auto Int16AddOp = MediumDecV2Base::Int16AddOp;
		constexpr auto UnsignedInt16AddOp = MediumDecV2Base::UnsignedInt16AddOp;
	
        constexpr auto AddByUInt = MediumDecV2Base::AddByUInt;
        constexpr auto AddByInt = MediumDecV2Base::AddByInt;
        constexpr auto UnsignedAddByInt = MediumDecV2Base::UnsignedAddByInt;
        constexpr auto AddByUInt64 = MediumDecV2Base::AddByUInt64;
        constexpr auto AddByInt64 = MediumDecV2Base::AddByInt64;
        constexpr auto UnsignedAddByInt64 = MediumDecV2Base::UnsignedAddByInt64;
		
        constexpr auto AddByUInt8 = MediumDecV2Base::AddByUInt8;
        constexpr auto AddByInt8 = MediumDecV2Base::AddByInt8;
        constexpr auto AddByUInt16 = MediumDecV2Base::AddByUInt16;
        constexpr auto AddByInt16 = MediumDecV2Base::AddByInt16;

protected:

        constexpr auto PartialAddOp = MediumDecV2Base::PartialAddOp;
        constexpr auto UnsignedPartialAddOp = MediumDecV2Base::UnsignedPartialAddOp;

public:
		/// <summary>
        /// Basic addition operation that ignores special decimal status with unsigned MediumDecV2Base
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedAddOp = MediumDecV2Base::BasicUnsignedAddOp;

		/// <summary>
        /// Basic addition operation that ignores special decimal status with unsigned MediumDecV2Base
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicAddOp = MediumDecV2Base::BasicAddOp;

		/// <summary>
        /// Basic unsigned addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedAddition = MediumDecV2Base::BasicUnsignedAddition;

		/// <summary>
        /// Basic addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicAddition = MediumDecV2Base::BasicAddition;

        /// <summary>
        /// Addition operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto UnsignedAddOp = MediumDecV2Base::BasicUnsignedAddOp;

        /// <summary>
        /// Addition operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto AddOp = MediumDecV2Base::BasicAddOp;

        /// <summary>
        /// Addition operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto AddByUnsigned = MediumDecV2Base::BasicUnsignedAddition;

        /// <summary>
        /// Addition operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        constexpr auto AddBy = MediumDecV2Base::BasicAddition;

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator+(const MediumDecV2& self, const MediumDecV2& Value) { return self.AddBy(Value); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator+=(MediumDecV2& self, const MediumDecV2& Value) { return self.AddOp(Value); }
		
        /// <summary>
        /// Addition operation between MediumDecV2 and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator+(const MediumDecV2& self, const signed int& Value) { return self.AddByInt(Value); }
        friend MediumDecV2 operator+(const MediumDecV2& self, const Int64& Value) { return self.AddByInt64(Value); }
        friend MediumDecV2 operator+(const MediumDecV2& self, const unsigned int& Value) { return self.AddByUInt(Value); }
        friend MediumDecV2 operator+(const MediumDecV2& self, const UInt64& Value) { return self.AddByUInt64(Value); }
		
        friend MediumDecV2 operator+(const signed int& lValue, const MediumDecV2& rValue) { return rValue.AddByInt(lValue); }
        friend MediumDecV2 operator+(const Int64& lValue, const MediumDecV2& rValue) { return rValue.AddByInt64(lValue); }
        friend MediumDecV2 operator+(const unsigned int& lValue, const MediumDecV2& rValue) { return rValue.AddByUInt(lValue); }
        friend MediumDecV2 operator+(const UInt64& lValue, const MediumDecV2& rValue) { return rValue.AddByUInt64(lValue); }

        friend MediumDecV2 operator+(const MediumDecV2& self, const signed char& Value) { return self.AddByInt8(Value); }
        friend MediumDecV2 operator+(const MediumDecV2& self, const signed short& Value) { return self.AddByInt16(Value); }
        friend MediumDecV2 operator+(const MediumDecV2& self, const unsigned char& Value) { return self.AddByUInt8(Value); }
        friend MediumDecV2 operator+(const MediumDecV2& self, const unsigned short& Value) { return self.AddByUInt16(Value); }

        friend MediumDecV2 operator+(const signed char& lValue, const MediumDecV2& rValue) { return rValue.AddByInt8(lValue); }
        friend MediumDecV2 operator+(const signed short& lValue, const MediumDecV2& rValue) { return rValue.AddByInt16(lValue); }
        friend MediumDecV2 operator+(const unsigned char& lValue, const MediumDecV2& rValue) { return rValue.AddByUInt8(lValue); }
        friend MediumDecV2 operator+(const unsigned short& lValue, const MediumDecV2& rValue) { return rValue.AddByUInt16(lValue); }

        /// <summary>
        /// += operation between MediumDecV2 and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator+=(MediumDecV2& self, const signed int& Value) { return self.IntAddOp(Value); }
        friend MediumDecV2& operator+=(MediumDecV2& self, const Int64& Value) { return self.Int64AddOp(Value); }
        friend MediumDecV2& operator+=(MediumDecV2& self, const unsigned int& Value) { return self.UIntAddOp(Value); }
        friend MediumDecV2& operator+=(MediumDecV2& self, const UInt64& Value) { return self.UInt64AddOp(Value); }

        friend MediumDecV2& operator+=(MediumDecV2& self, const signed char& Value) { return self.Int8AddOp(Value); }
        friend MediumDecV2& operator+=(MediumDecV2& self, const signed short& Value) { return self.Int16AddOp(Value); }
        friend MediumDecV2& operator+=(MediumDecV2& self, const unsigned char& Value) { return self.UInt8AddOp(Value); }
        friend MediumDecV2& operator+=(MediumDecV2& self, const unsigned short& Value) { return self.UInt16AddOp(Value); }

	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations

protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntSubOpV1 = MediumDecV2Base::PartialUIntSubOpV1<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntSubOpV1 = MediumDecV2Base::PartialIntSubOpV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between MediumDecV2 and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntSubOpV1 = MediumDecV2Base::BasicUIntSubOpV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between MediumDecV2 and Integer value. 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntSubOpV1 = MediumDecV2Base::BasicIntSubOpV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicSubtractByUIntV1 = MediumDecV2Base::BasicSubtractByIntV1<IntType>;

        /// <summary>
        /// Basic subtraction operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicSubtractByIntV1 = MediumDecV2Base::BasicSubtractByIntV1<IntType>;
    		
public:

        constexpr auto PartialUIntSubOpV1 = PartialUIntSubOpV1<unsigned int>;
        constexpr auto PartialIntSubOpV1 = PartialIntSubOpV1<signed int>;
        constexpr auto UnsignedPartialIntSubOpV1 = PartialUIntSubOpV1<signed int>;
        constexpr auto PartialUInt64SubOpV1 = PartialUIntSubOpV1<UInt64>;
        constexpr auto PartialInt64SubOpV1 = PartialIntSubOpV1<Int64>;

        constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<unsigned int>;
        constexpr auto BasicIntSubOp = BasicIntSubOpV1<signed int>;
        constexpr auto UnsignedBasicIntSubOp = BasicUIntSubOpV1<signed int>;
        constexpr auto BasicUInt64SubOp = BasicUIntSubOpV1<UInt64>;
        constexpr auto BasicInt64SubOp = BasicIntSubOpV1<Int64>;
        constexpr auto UnsignedBasicInt64SubOp = BasicUIntSubOpV1<signed int>;

        constexpr auto BasicUInt8SubOp = BasicUIntSubOpV1<unsigned char>;
        constexpr auto BasicInt8SubOp = BasicIntSubOpV1<signed char>;
        constexpr auto BasicUInt16SubOp = BasicUIntSubOpV1<unsigned short>;
        constexpr auto BasicInt16SubOp = BasicIntSubOpV1<signed short>;

        constexpr auto BasicSubtractByUInt = BasicSubtractByUIntV1<unsigned int>;
        constexpr auto BasicSubtractByInt = BasicSubtractByIntV1<signed int>;
        constexpr auto UnsignedBasicSubtractByInt = BasicSubtractByUIntV1<signed int>;
        constexpr auto BasicSubtractByUInt64 = BasicSubtractByUIntV1<UInt64>;
        constexpr auto BasicSubtractByInt64 = BasicSubtractByIntV1<Int64>;
        constexpr auto UnsignedBasicSubtractByInt64 = BasicSubtractByUIntV1<signed int>;

        constexpr auto BasicSubtractByUInt8 = BasicSubtractByUIntV1<unsigned char>;
        constexpr auto BasicSubtractByInt8 = BasicSubtractByIntV1<signed char>;
        constexpr auto BasicSubtractByUInt16 = BasicSubtractByUIntV1<unsigned short>;
        constexpr auto BasicSubtractByInt16 = BasicSubtractByIntV1<signed short>;
	
protected:

        /// <summary>
        /// Subtraction operation between MediumDecV2 and unsigned Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto UIntSubOpV1 = MediumDecV2Base::UIntSubOpV1<IntType>;

        /// <summary>
        /// Subtraction operation between MediumDecV2 and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType= signed int>
        constexpr auto IntSubOpV1 = MediumDecV2Base::IntSubOpV1<IntType>;
		
        /// <summary>
        /// Subtraction operation between MediumDecV2 and unsigned Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2</returns>
        template<IntegerType IntType= unsigned int>
        constexpr auto SubtractByUIntV1 = MediumDecV2Base::UIntDivOpV1<IntType>;

        /// <summary>
        /// Subtraction operation between MediumDecV2 and Integer values
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2</returns>
        template<IntegerType IntType= signed int>
        constexpr auto SubtractByIntV1 = MediumDecV2Base::IntDivOpV1<IntType>;

public:

        constexpr auto UIntSubOp = MediumDecV2Base::UIntSubOp;
        constexpr auto IntSubOp = MediumDecV2Base::IntSubOp;
        constexpr auto UnsignedBasicIntSubOp = MediumDecV2Base::UnsignedBasicIntSubOp;
        constexpr auto UInt64SubOp = MediumDecV2Base::UInt64SubOp;
        constexpr auto Int64SubOp = MediumDecV2Base::Int64SubOp;
        constexpr auto UnsignedBasicIntSubOp = MediumDecV2Base::UnsignedBasicIntSubOp;
	
        constexpr auto UInt8SubOpV1 = MediumDecV2Base::UInt8SubOpV1;
        constexpr auto Int8SubOpV1 = MediumDecV2Base::Int8SubOpV1;
        constexpr auto UnsignedInt8SubOp = MediumDecV2Base::UnsignedInt8SubOp;
        constexpr auto UInt16SubOp = MediumDecV2Base::UInt16SubOp;
        constexpr auto Int16SubOp = MediumDecV2Base::Int16SubOp;
		constexpr auto UnsignedInt16SubOp = MediumDecV2Base::UnsignedInt16SubOp;
	
        constexpr auto SubtractByUInt = MediumDecV2Base::SubtractByUInt;
        constexpr auto SubtractByInt = MediumDecV2Base::SubtractByInt;
        constexpr auto UnsignedSubtractByInt = MediumDecV2Base::UnsignedSubtractByInt;
        constexpr auto SubtractByUInt64 = MediumDecV2Base::SubtractByUInt64;
        constexpr auto SubtractByInt64 = MediumDecV2Base::SubtractByInt64;
        constexpr auto UnsignedSubtractByInt64 = MediumDecV2Base::UnsignedSubtractByInt64;
		
        constexpr auto SubtractByUInt8 = MediumDecV2Base::SubtractByUInt8;
        constexpr auto SubtractByInt8 = MediumDecV2Base::SubtractByInt8;
        constexpr auto SubtractByUInt16 = MediumDecV2Base::SubtractByUInt16;
        constexpr auto SubtractByInt16 = MediumDecV2Base::SubtractByInt16;
		
protected:

        constexpr auto PartialSubOp = MediumDecBaseV2::PartialSubOp;
        constexpr auto UnsignedPartialSubOp = MediumDecV2Base::UnsignedPartialSubOp;

public:
	
		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        constexpr auto BasicUnsignedSubOp = MediumDecV2Base::BasicUnsignedSubOp;

		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicSubOp = MediumDecV2Base::BasicSubOp;
    
		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicUnsignedDivision = MediumDecV2Base::BasicUnsignedDivision;

		/// <summary>
        /// Basic subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        constexpr auto BasicDivision = MediumDecV2Base::BasicDivision;

        /// <summary>
        /// Subtraction operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        constexpr auto UnsignedSubOp = MediumDecV2Base::BasicUnsignedSubOp;

        /// <summary>
        /// Subtraction operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        constexpr auto SubOp = MediumDecV2Base::BasicSubOp;

        /// <summary>
        /// Subtraction operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        constexpr auto SubtractByUnsigned = MediumDecV2Base::BasicUnsignedDivision;

        /// <summary>
        /// Subtraction operation between MediumDecV2 values.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2&</returns>
        constexpr auto SubtractBy = MediumDecV2Base::BasicDivision;

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator-(const MediumDecV2& self, const MediumDecV2& Value) { return self.SubtractBy(Value); }
		
        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator-=(MediumDecV2& self, const MediumDecV2& Value) { return self.SubOp(Value); }
		
        /// <summary>
        /// Subtraction operation between MediumDecV2 and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator-(const MediumDecV2& self, const signed int& Value) { return self.SubtractByInt(Value); }
        friend MediumDecV2 operator-(const MediumDecV2& self, const Int64& Value) { return self.SubtractByInt64(Value); }
        friend MediumDecV2 operator-(const MediumDecV2& self, const unsigned int& Value) { return self.SubtractByUInt(Value); }
        friend MediumDecV2 operator-(const MediumDecV2& self, const UInt64& Value) { return self.SubtractByUInt64(Value); }
		
        friend MediumDecV2 operator-(const signed int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const Int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const UInt64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }

        friend MediumDecV2 operator-(const MediumDecV2& self, const signed char& Value) { return self.SubtractByInt8(Value); }
        friend MediumDecV2 operator-(const MediumDecV2& self, const signed short& Value) { return self.SubtractByInt16(Value); }
        friend MediumDecV2 operator-(const MediumDecV2& self, const unsigned char& Value) { return self.SubtractByUInt8(Value); }
        friend MediumDecV2 operator-(const MediumDecV2& self, const unsigned short& Value) { return self.SubtractByUInt16(Value); }

        friend MediumDecV2 operator-(const signed char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const signed short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }


        /// <summary>
        /// -= operation between MediumDecV2 and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator-=(MediumDecV2& self, const signed int& Value) { return self.IntSubOp(Value); }
        friend MediumDecV2& operator-=(MediumDecV2& self, const Int64& Value) { return self.Int64SubOp(Value); }
        friend MediumDecV2& operator-=(MediumDecV2& self, const unsigned int& Value) { return self.UIntSubOp(Value); }
        friend MediumDecV2& operator-=(MediumDecV2& self, const UInt64& Value) { return self.UInt64SubOp(Value); }

        friend MediumDecV2& operator-=(MediumDecV2& self, const signed char& Value) { return self.Int8SubOp(Value); }
        friend MediumDecV2& operator-=(MediumDecV2& self, const signed short& Value) { return self.Int16SubOp(Value); }
        friend MediumDecV2& operator-=(MediumDecV2& self, const unsigned char& Value) { return self.UInt8SubOp(Value); }
        friend MediumDecV2& operator-=(MediumDecV2& self, const unsigned short& Value) { return self.UInt16SubOp(Value); }

	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations

        /// <summary>
        /// Modulus Operation(Returning inside
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend ModResult operator%(const MediumDecV2& LValue, const MediumDecV2& RValue)
		{
			return ModResult(LValue, RValue);
		}

        friend MediumDecV2& operator%=(MediumDecV2& LValue, const MediumDecV2& RValue)
		{ 
            MediumDecV2 divRes = LValue / RValue;
            LValue -= RValue * divRes;
			return *this;
		}

	#pragma endregion Modulus Operations
	
    #pragma region Floating Operator Overrides
    
        friend MediumDecV2 operator+(const MediumDecV2& self, const float& Value) { return self + (MediumDecV2)Value; }
        friend MediumDecV2 operator-(const MediumDecV2& self, const float& Value) { return self - (MediumDecV2)Value; }
        friend MediumDecV2 operator*(const MediumDecV2& self, const float& Value) { return self * (MediumDecV2)Value; }
        friend MediumDecV2 operator/(const MediumDecV2& self, const float& Value) { return self / (MediumDecV2)Value; }

        friend MediumDecV2 operator+(const float& Value, const MediumDecV2& self) { return (MediumDecV2)Value + self; }
        friend MediumDecV2 operator-(const float& Value, const MediumDecV2& self) { return (MediumDecV2)Value - self; }
        friend MediumDecV2 operator*(const float& Value, const MediumDecV2& self) { return (MediumDecV2)Value * self; }
        friend MediumDecV2 operator/(const float& Value, const MediumDecV2& self) { return (MediumDecV2)Value / self; }

        friend MediumDecV2 operator+(const MediumDecV2& self, const double& Value) { return self + (MediumDecV2)Value; }
        friend MediumDecV2 operator-(const MediumDecV2& self, const double& Value) { return self - (MediumDecV2)Value; }
        friend MediumDecV2 operator*(const MediumDecV2& self, const double& Value) { return self * (MediumDecV2)Value; }
        friend MediumDecV2 operator/(const MediumDecV2& self, const double& Value) { return self / (MediumDecV2)Value; }

        friend MediumDecV2 operator+(const MediumDecV2& self, const ldouble& Value) { return self + (MediumDecV2)Value; }
        friend MediumDecV2 operator-(const MediumDecV2& self, const ldouble& Value) { return self - (MediumDecV2)Value; }
        friend MediumDecV2 operator*(const MediumDecV2& self, const ldouble& Value) { return self * (MediumDecV2)Value; }
        friend MediumDecV2 operator/(const MediumDecV2& self, const ldouble& Value) { return self / (MediumDecV2)Value; }

        friend MediumDecV2 operator+(const ldouble& Value, const MediumDecV2& self) { return (MediumDecV2)Value + self; }
        friend MediumDecV2 operator-(const ldouble& Value, const MediumDecV2& self) { return (MediumDecV2)Value - self; }
        friend MediumDecV2 operator*(const ldouble& Value, const MediumDecV2& self) { return (MediumDecV2)Value * self; }
        friend MediumDecV2 operator/(const ldouble& Value, const MediumDecV2& self) { return (MediumDecV2)Value / self; }

    #pragma endregion Floating Operator Overrides

    #pragma region Bitwise Functions
	
    #pragma endregion Bitwise Functions

    #pragma region Other Operators
	
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecV2 operator- ()
        {
			auto self = this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// ++MediumDecV2 Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator ++()
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
        /// ++MediumDecV2 Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator --()
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
        /// MediumDecV2++ Operator
        /// </summary>
        /// <returns>MediumDecV2</returns>
        MediumDecV2 operator ++(int)
        {
            MediumDecV2 tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecV2-- Operator
        /// </summary>
        /// <returns>MediumDecV2</returns>
        MediumDecV2 operator --(int)
        {
            MediumDecV2 tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecV2* Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator *()
        {
            return *this;
        }
		
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