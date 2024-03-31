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
    class MediumDecV2Base;

    /// <summary>
    /// Separating functions that don't use static variables inside this base class for deriving
    /// Completed class inside MediumDec
	/// </summary>
    class DLL_API MediumDecV2Base : public virtual MediumDecBase
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2Base(const IntHalfType& intVal, const DecimalHalfType& decVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
        }

        MediumDecV2Base(const MediumDecV2Base&) = default;

        MediumDecV2Base& operator=(const MediumDecV2Base& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

        //Is at either zero or negative zero IntHalf of AltNum
        constexpr auto IsAtZeroInt = MediumDecBase::IsAtZeroInt;

        //alias function
        constexpr auto IsNotAtZeroInt = MediumDecBase::IsNotAtZeroInt;

        //Detect if at exactly zero
        constexpr auto IsZero = MediumDecBase::IsZero;

/*
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void SetVal(VariantType Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }*/


        constexpr auto SetAsZero = MediumDecBase::SetAsZero;

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        constexpr auto SwapNegativeStatus = MediumDecBase::SwapNegativeStatus;

    #pragma region Const Representation values

    #pragma endregion Const Representation values

    #pragma region RepType
        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        virtual RepType const GetRepType()
        {
#if !defined(AltNum_UseIntForDecimalHalf)
            if(DecimalHalf.Flag==0)
			{
                return RepType::NormalType;
			}
    #if defined(MediumDecV2_EnablePiRep)
            else if(DecimalHalf.Flag==1)
				return RepType::PiNum;
    #endif
    #if defined(MediumDecV2_EnableERep)
            else if(DecimalHalf.Flag==2)
				return RepType::ENum;
    #endif
	#if defined(MediumDecV2_EnableImaginaryNum)
            else if(DecimalHalf.Flag==3)
				return RepType::INum;
	#endif
	#if defined(MediumDecV2_EnableUndefinedButInRange)//Such as result of Cos of infinity
           else if(DecimalHalf.Flag==3)
		#if defined(MediumDecV2_EnableWithinMinMaxRange)
				//If IntValue==NegativeRep, then left side range value equals negative infinity
				//If DecimalHalf.Value==InfinityRep, then right side range value equals positive infinity
				return IntHalf==0&&DecimalHalf.Value==0? RepType::UndefinedButInRange: RepType::WithinMinMaxRange;
		#else
				//If DecimalHalf equals InfinityRep, than equals undefined value with range between negative infinity and positive infinity (negative range values indicates inverted range--any but the range of values)
                return RepType::UndefinedButInRange;
		#endif
	#endif
	#if defined(MediumDecV2_EnableNaN)
			else if(DecimalHalf==NaNRep)
				return RepType::NaN;
			else if(DecimalHalf==UndefinedRep)
				return RepType::Undefined;
	#endif
            else
				throw "Unknown or non-enabled representation type detected";
#else//Using signed int
	//Only supports NormalType, Infinity, and approaching value types
#endif
            return RepType::UnknownType;//Catch-All Value;
        }

    #pragma endregion RepType

public:
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
    #if defined(MediumDecV2_EnablePiRep)
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        virtual void SetPiVal(const MediumDecBaseV2& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
        }
        
        virtual void SetPiValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,1);
        }
    #endif
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
    #if defined(MediumDecV2_EnableERep)
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        virtual void SetEVal(const MediumDecBaseV2& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
        }
        
        virtual void SetEValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,2);
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region INum Setters
    #if defined(MediumDecV2_EnableIRep)
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        virtual void SetIVal(const MediumDecBaseV2& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,3);
        }
        
        virtual void SetIValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,3);
        }
    #endif
    #pragma endregion INum Setters

    #pragma region Fractional Setters
	//Not used for this variant(Used in AltDecBase and others)
    #pragma endregion Fractional Setters
    
    #pragma region MixedFrac Setters
	//Not used for this variant(Used in AltDecBase and others)
    #pragma endregion MixedFrac Setters

    #pragma region Infinity Setters
    //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
    // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
    #if defined(AltNum_EnableInfinityRep)
        void SetAsInfinity()
        {
	#if defined(AltNum_EnableMirroredIntV2)
            IntValue.IsNegative = 0; DecimalHalf = InfinityRep;
    #else
            IntValue = 1; DecimalHalf = InfinityRep;
    #endif
        }

        void SetAsNegativeInfinity()
        {
	#if defined(AltNum_EnableMirroredIntV2)
            IntValue.IsNegative = 1; DecimalHalf = InfinityRep;
    #else
            IntValue = -1; DecimalHalf = InfinityRep;
    #endif
        }
	#endif
    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters
	#if defined(AltNum_EnableApproachingValues)

		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
        void SetAsApproachingBottom(int value=0)
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
        }

		#if !defined(AltNum_DisableApproachingTop)
		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        void SetAsApproachingTop(int value)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
        }
        #endif
    #endif
    #pragma endregion ApproachingZero Setters

    #pragma region NaN Setters
	#if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep;
        }

        void SetAsUndefined()
        {
            IntValue = 0; DecimalHalf = UndefinedRep;
        }
	#endif
    #pragma endregion NaN Setters

    #pragma region ValueDefines

    #pragma endregion ValueDefines

    #pragma region String Commands
	
    #pragma endregion String Commands

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