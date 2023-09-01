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
#include <cmath>//Needed for floating point floor function
#include "..\OtherFunctions\VariableConversionFunctions.h"

#if defined(AltNum_EnableAutoToggleOfPreferedSettings)
    #define AltNum_EnablePiRep
    #define AltNum_EnableInfinityRep
	#define AltNum_EnableAlternativeRepFractionals
    #define AltNum_EnableDecimaledPiFractionals
    #define AltNum_EnableApproachingValues
    #define AltNum_UseDeveloperExtraDefaults//Turns on extra defaults just for testing
#endif

#if defined(AltNum_UseDeveloperExtraDefaults)
    #define AltNum_EnableImaginaryNum
    #define AltNum_EnableApproachingI
    #define AltNum_EnableMixedPiFractional
    #define AltNum_EnableERep
#endif

//If Pi rep is neither disabled or enabled, default to enabling Pi representation
#if !defined(AltNum_DisablePiRep) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(AltNum_EnablePiRep) && defined(AltNum_DisablePiRep)
    #undef AltNum_DisablePiRep
#endif

#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional) || defined(AltNum_EnableMixedIFractional)
    #define AltNum_EnableAlternativeMixedFrac
    #if !defined(AltNum_EnableMixedFractional)
        #define AltNum_EnableMixedFractional
    #endif
    #if !defined(AltNum_EnableAlternativeRepFractionals)
        #define AltNum_EnableAlternativeRepFractionals
    #endif
    #if !defined(AltNum_EnablePiRep) && defined(AltNum_EnableMixedPiFractional)
        #define AltNum_EnablePiRep
    #endif
    #if !defined(AltNum_EnableERep) && defined(AltNum_EnableMixedEFractional)
        #define AltNum_EnableERep
    #endif
    #if !defined(AltNum_EnableImaginaryNum) && defined(AltNum_EnableMixedIFractional)
        #define AltNum_EnableImaginaryNum
    #endif
#endif

#if defined(AltNum_EnableImaginaryInfinity)
    #define AltNum_EnableImaginaryNum
	#define AltNum_EnableInfinityRep
#endif

//Force required preprocessor flag for AltNum_EnableAlternativeRepFractionals
#if defined(AltNum_EnableAlternativeRepFractionals)
	#if !defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableERep)&&!defined(AltNum_EnableImaginaryNum)
		#undef AltNum_EnableAlternativeRepFractionals//Alternative Fractionals require the related representations enabled
	#elif !defined(AltNum_EnableFractionals)
		#define AltNum_EnableFractionals
	#endif
#endif

#if defined(AltNum_AvoidUsingLargeInt)
    #undef AltNum_UseOldDivisionCode
#endif

//Force required flags to be enabled if AltNum_EnableApproachingDivided toggled
#if !defined(AltNum_EnableApproachingValues)
	#if defined(AltNum_EnableApproachingDivided)
		#define AltNum_EnableApproachingValues
	#elif defined(AltNum_EnableApproachingPi) || defined(AltNum_EnableApproachingE) || defined(AltNum_EnableApproachingI)
		#define AltNum_EnableApproachingValues
	#endif
#endif

#if !defined(AltNum_EnableDecimaledAlternativeFractionals) && defined(AltNum_EnableDecimaledPiFractionals)
    #define AltNum_EnableDecimaledAlternativeFractionals
#endif

#if !defined(AltNum_EnableDecimaledAlternativeFractionals) && defined(AltNum_EnableDecimaledEFractionals)
    #define AltNum_EnableDecimaledAlternativeFractionals
#endif

#if !defined(AltNum_EnableDecimaledAlternativeFractionals) && defined(AltNum_EnableDecimaledIFractionals)
    #define AltNum_EnableDecimaledAlternativeFractionals
#endif

#if defined(AltNum_EnableDecimaledPiFractionals) && defined(AltNum_EnableDecimaledEFractionals)
    #undef AltNum_EnableDecimaledEFractionals
#endif

#if defined(AltNum_EnableDecimaledPiFractionals) && defined(AltNum_EnableDecimaledIFractionals)
    #undef AltNum_EnableDecimaledIFractionals
#endif

//Turn off Pi Power's feature if AltNum_EnableDecimaledAlternativeFractionals enabled
#if defined(AltNum_EnableDecimaledAlternativeFractionals) && defined(AltNum_EnablePiPowers)
	#undef AltNum_EnablePiPowers
#endif

#if (defined(AltNum_EnableDecimaledAlternativeFractionals)||defined(AltNum_EnablePiPowers)) && defined(AltNum_EnableNormalPowers)
	#undef AltNum_EnableNormalPowers
#endif

//Force enable Pi features if near Pi enabled
#if defined(AltNum_EnableApproachingPi) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(AltNum_EnableApproachingE) && !defined(AltNum_EnableERep)
	#define AltNum_EnableERep
#endif

#if defined(AltNum_EnableApproachingI) && !defined(AltNum_EnableImaginaryNum)
    #define AltNum_EnableImaginaryNum
#endif

#if !defined(AltNum_EnablePiFractional) &&defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnablePiFractional//PiFractional enabled (AltNum_EnablePiRep and AltNum_EnableAlternativeRepFractionals active)
#endif
#if !defined(AltNum_EnableEFractional) &&defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnableEFractional//EFractional enabled (AltNum_EnableERep and AltNum_EnableAlternativeRepFractionals active)
#endif
#if !defined(AltNum_EnableIFractional) &&defined(AltNum_EnableImaginaryNum)&&!defined(AltNum_EnableDecimaledIFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnableIFractional//IFractional enabled (AltNum_EnableImaginaryNum and AltNum_EnableAlternativeRepFractionals active)
#endif

#if defined(AltNum_EnablePiFractional) || defined(AltNum_EnableEFractional) || defined(AltNum_EnableIFractional)
	#define AltNum_UsingAltFractional//AltNum_EnablePiFractional, AltNum_EnableEFractional, or AltNum_EnableIFractional active
#endif

#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional)
    #define AltNum_MixedPiOrEEnabled//Either MixedPi or MixedE enabled
#endif

#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional) || defined(AltNum_EnableMixedEFractional)
    #define AltNum_MixedAltEnabled//Either MixedPi, MixedE, or MixedI enabled
#endif

#if defined(AltNum_EnableMixedPiFractional) && defined(AltNum_EnableDecimaledPiFractionals)
    #define AltNum_MixedPiHasDecimaledFracAccess
#elif defined(AltNum_EnableMixedEFractional) && defined(AltNum_EnableDecimaledEFractionals)
    #define AltNum_MixedEHasDecimaledFracAccess
#elif defined(AltNum_EnableMixedIFractional) && defined(AltNum_EnableDecimaledIFractionals)
    #define AltNum_MixedIHasDecimaledFracAccess
#endif

#if defined(AltNum_MixedPiHasDecimaledFracAccess) || defined(AltNum_MixedEHasDecimaledFracAccess) || defined(AltNum_MixedPiHasDecimaledFracAccess)
    #define AltNum_MixedAltFracHasDecimaledFractionalAccess
#endif

#if defined(AltNum_MixedPiHasDecimaledFracAccess)|| defined(AltNum_MixedEHasDecimaledFracAccess)
    #define AltNum_MixedPiOrEHasDecimaledFracAccess
#endif

#if defined(AltNum_EnableDecimaledPiFractionals) || defined(AltNum_EnableDecimaledEFractionals)
    #define AltNum_EnableDecimaledPiOrEFractionals
#endif

namespace BlazesRusCode
{
    //Integer type
    template<typename T>
    concept IntegerType = std::is_integral<T>::value;

	//Base class for AltDec and MediumDec to help initial structure of classes
    class DLL_API AltNumBase
    {
#if defined(AltNum_StoreCommonVariablesInBase)
    public:
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;

	protected:
        /// <summary>
        /// The decimal overflow value * -1
        /// </summary>
		static signed _int64 const NegDecimalOverflowX = -1000000000;
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

        /// <summary>
        /// Value when IntValue is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
#if !defined(AltDec_UseMirroredInt)
        static signed int const NegativeRep = -2147483648;
#else
        static MirroredInt NegativeRep;
        static signed int const NegativeRepVal = MirroredInt::NegativeRep;
#endif

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
#if !defined(AltDec_UseMirroredInt)
        signed int IntValue;
#else
        MirroredInt IntValue;
#endif
	
        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        signed int DecimalHalf;
#endif

#if defined(AltNum_StoreCommonFunctionsInBase)
        bool IsNegative()
        {
    #if !defined(AltDec_UseMirroredInt)
            return IntValue<0;
    #else
            return IntValue.IsNegative();
    #endif
        }

        //Detect if at exactly zero
		bool IsZero() const
		{
            return DecimalHalf==0&&IntValue==0;
		}

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt() const
        {
            return IntValue==0||IntValue==NegativeRep;
        }

        bool IsNotAtZeroInt() const
        {
            return IntValue != 0 && IntValue != NegativeRep;
        }

        signed int GetIntHalf() const
        {
    #if defined(AltDec_UseMirroredInt)
            return IntValue.GetValue();
    #else
            if(IntValue == NegativeRep)
                return 0;
            else
                return IntValue;
    #endif
        }

        //Return IntValue part as Absolute value
        signed int IntHalfAsAbs() const
        {
    #if defined(AltDec_UseMirroredInt)
            return IntValue.GetAbsValue();
    #else
            if (IsAtZeroInt())
                return 0;
            else if (IntValue < 0)
                return -IntValue;
            else
                return IntValue;
    #endif
        }

        std::string IntHalfAsString() const
        {
    #if defined(AltDec_UseMirroredInt)
            return (std::string) IntValue;
    #else
            if (IntValue == NegativeRep)
                return "-0";
            return VariableConversionFunctions::IntToStringConversion(IntValue);
    #endif
        }
#endif

#if defined(AltNum_StoreCommonVariablesInBase)
#if defined(AltDec_UseMirroredInt)
        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.(Default constructor)
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        AltNumBase(const MirroredInt& intVal = MirroredInt::Zero, const signed int& decVal = 0)
        {

            IntValue = intVal;
            DecimalHalf = decVal;
        }
#endif

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
#if !defined(AltDec_UseMirroredInt)
        AltNumBase(const int& intVal=0, const signed int& decVal = 0)
#else
        AltNumBase(const int& intVal, const signed int& decVal = 0)
#endif
        {
#if defined(AltDec_UseMirroredInt)&&defined(BlazesMirroredInt_UseLegacyValueBehavior)
            IntValue.Value = intVal;
#else
            IntValue = intVal;
#endif
            DecimalHalf = decVal;
        }
#endif

#if defined(AltNum_StoreCommonFunctionsInBase)
        virtual void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0;
        }
        
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
            if (IntValue == NegativeRep)
            {
                IntValue = 0;
            }
            else if (IntValue == 0)
            {
                IntValue = NegativeRep;
            }
            else
            {
                IntValue *= -1;
            }
        }

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMaximum()
        {
            IntValue = 2147483647; DecimalHalf = 999999999;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMinimum()
        {
            IntValue = -2147483647; DecimalHalf = 999999999;
        }
#endif

    #pragma region Const Representation values
#if defined(AltNum_StoreCommonVariablesInBase)
	#if defined(AltNum_EnableInfinityRep)
        //Is Infinity Representation when DecimalHalf==-2147483648 (IntValue==1 for positive infinity;IntValue==-1 for negative Infinity)
		//(other values only used if AltNum_EnableInfinityPowers is enabled)
		//If AltNum_EnableImaginaryInfinity is enabled and ExtraRep = IRep, then represents either negative or positive imaginary infinity
        static const signed int InfinityRep = -2147483648;
	#endif
	#if defined(AltNum_EnableApproachingValues)
        //Is Approaching Bottom when DecimalHalf==-2147483647:
        //If ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0__1)
		//If ExtraRep above 1 and 2147483645 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
		//If ExtraRep=PiRep, then it represents Approaching IntValue from right towards left (IntValue.0__1)Pi
        static const signed int ApproachingBottomRep = -2147483647;
		//Is Approaching Top i when DecimalHalf==-2147483646:
		//If ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)
		//If ExtraRep above 1 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
		//If ExtraRep=PiRep, then it represents Approaching IntValue+1 from left towards right (IntValue.9__9)Pi
		static const signed int ApproachingTopRep = -2147483646;
    #endif
	#if defined(AltNum_EnableUndefinedButInRange)
		//Such as result of Cos of infinity
		//https://www.cuemath.com/trigonometry/domain-and-range-of-trigonometric-functions/
        static const signed int UndefinedInRangeRep = -2147483642;
		
		#if defined(AltNum_EnableWithinMinMaxRange)
		//Undefined but in ranged of IntValue to DecimalHalf
        static const signed int WithinMinMaxRangeRep = -2147483642;
		#endif
	#endif
	#if defined(AltNum_EnableNaN)
        //Is NaN when DecimalHalf==2147483647
        static const signed int NaNRep = 2147483647;
        //Is NaN when DecimalHalf==2147483646
        static const signed int UndefinedRep = 2147483646;
	#endif
    #if defined(AltNum_EnableNilRep)
        //When both IntValue and DecimalHalf equal -2147483648 it is Nil
        static signed int const NilRep = -2147483648;
    #endif
#endif
//        virtual RepType const GetRepType()
//        {
//        }

        //virtual void const GetRepType() = 0;


    #pragma endregion Const Representation values

    #pragma region PiNum Setters

    #pragma endregion PiNum Setters

    #pragma region ENum Setters

    #pragma endregion ENum Setters

    #pragma region Fractional Setters

    #pragma endregion Fractional Setters
    
    #pragma region MixedFrac Setters

    #pragma endregion MixedFrac Setters

protected:
    #pragma region Infinity Setters
#if defined(AltNum_StoreCommonFunctionsInBase)
    //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
    // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
    #if defined(AltNum_EnableInfinityRep)
        virtual void SetAsInfinity()
        {
            IntValue = 1; DecimalHalf = -2147483648;
        }

        virtual void SetAsNegativeInfinity()
        {
            IntValue = -1; DecimalHalf = -2147483648;
        }
	#endif
#endif
    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters
#if defined(AltNum_StoreCommonFunctionsInBase)
		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
        virtual void SetAsApproachingBottom(int value=0)
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
#if defined(AltNum_StoreCommonFunctionsInBase)
	#if defined(AltNum_EnableNaN)
        virtual void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep;
        }

        virtual void SetAsUndefined()
        {
            IntValue = 0; DecimalHalf = UndefinedRep;
        }
	#endif
#endif
    #pragma endregion NaN Setters
public:

    #pragma region ValueDefines

    #pragma endregion ValueDefines

    #pragma region String Commands
	
    #pragma endregion String Commands

    #pragma region From Standard types to this type
protected:
/*//Need to copy the const reference before enable this section
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetFloatVal(const float& value)
        {
            float Value = value;
            bool IsNegative = Value < 0.0f;
            if (IsNegative) { Value *= -1.0f; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0f)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                DecimalHalf = (signed int)((Value - (float)WholeValue) * 10000000000.0f);
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDoubleVal(const double& value)
        {
            double Value = value;
            bool IsNegative = Value < 0.0;
            if (IsNegative) { Value *= -1.0; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                DecimalHalf = (signed int)((Value - (double)WholeValue) * 10000000000.0);
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDecimalVal(const ldouble& value)
        {
            ldouble Value = value;
            bool IsNegative = Value < 0.0L;
            if (IsNegative) { Value *= -1.0L; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0L)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                DecimalHalf = (signed int)(Value-(ldouble)(WholeValue) * 10000000000.0L);
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetBooleanVal(const bool& Value)
        {
            IntValue = Value ? 1 : 0;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(const int& Value)
        {
            IntValue = Value; DecimalHalf = 0;
        }
*/
public:

    #pragma endregion From this type to Standard types

    #pragma region MirroredIntBased Operations
#if defined(AltNum_StoreIntSectionFunctionsInBase)//Templates can not be virtual
        
        template<typename IntType=int>
        void IntHalfDivision(IntType RValue)
        {
    #if defined(AltDec_UseMirroredInt)
        #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
            #if defined(BlazesMirroredInt_UseLegacyIntOperations)
            IntValue.Value /= RValue;
            #else
            IntValue.DivideByOp(RValue);
            #endif
        #elif defined(BlazesMirroredInt_UsePseudoBitSet)
            if(RValue<0)
            {
                if(IntValue.Value>=NegativeRepVal)//Currently Negative
                {
                    IntValue.Value -= NegativeRepVal;
                    RValue -= NegativeRepVal;
                    IntValue.Value /= RValue; 
                }
                else
                {

                } 
            }
            else
            {
                if(IntValue.Value>=NegativeRepVal)//Currently Negative
                {
                }
                else
                {
                    IntValue.Value /= RValue;
                } 
            }
        #else
            IntValue.DivideByOp(RValue);
        #endif
    #else
            IntValue /= RValue;
    #endif
        }

        template<typename IntType=int>
        void IntHalfMultiplication(IntType RValue)
        {
    #if defined(AltDec_UseMirroredInt)
        #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
            #if defined(BlazesMirroredInt_UseLegacyIntOperations)
            IntValue.Value *= RValue;
            #else
            IntValue.MultiplyByOp(RValue);
            #endif
        #elif defined(BlazesMirroredInt_UsePseudoBitSet)
            #if defined(BlazesMirroredInt_UseBitwiseForIntOp)
            IntValue.MultiplyByOp(RValue);
            #else
            if(RValue<0)
            {
                if(IntValue.Value>=NegativeRepVal)//Currently Negative
                {
                    IntValue.Value -= NegativeRepVal;
                    RValue -= NegativeRepVal;
                    IntValue.Value *= RValue;
                }
                else
                {

                } 
            }
            else
            {
                if(IntValue.Value>=NegativeRepVal)//Currently Negative
                {
                }
                else
                {
                    IntValue.Value *= RValue;
                } 
            }
            #endif
        #else
            IntValue.MultiplyByOp(RValue);
        #endif
    #else
            if(IntValue!=0&&IntValue!=NegativeRep)
                IntValue *= RValue;
    #endif
        }

        template<typename IntType=int>
        void UIntHalfMultiplication(IntType RValue)
        {
    #if defined(AltDec_UseMirroredInt)
        #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
            #if defined(BlazesMirroredInt_UseLegacyIntOperations)
            IntValue.Value *= RValue;
            #else
            IntValue.MultiplyByOp(RValue);
            #endif
        #elif defined(BlazesMirroredInt_UsePseudoBitSet)
            #if defined(BlazesMirroredInt_UseBitwiseForIntOp)
            IntValue.MultiplyByOp(RValue);
            #else
            if(IntValue.Value>=NegativeRepVal)//Currently Negative
            {
            }
            else
            {
                IntValue.Value *= RValue;
            }
            #endif
        #else
            IntValue.MultiplyByOp(RValue);
        #endif
    #else
            if(IntValue!=0&&IntValue!=NegativeRep)
                IntValue *= RValue;
    #endif
        }

        //Replace usage of IntValue += RValue; with IntHalfAddition(RValue); or IntHalfAddition(RValue);
        template<typename IntType>
        void IntHalfAddition(IntType RValue)
        {
    #if defined(AltDec_UseMirroredInt)
        #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
            IntValue += RValue;
        #elif defined(BlazesMirroredInt_UsePseudoBitSet)
            #if defined(BlazesMirroredInt_UseBitwiseForIntOp)
            #else
            #endif
        #endif
    #else//Can be used without modifying RValue
            if (RValue==0)
                return;
            if (IntValue == 0)
                IntValue = RValue;
            else if (IntValue == NegativeRep)
            {
                //-0.XXXX + 2 = 1.XXXX
                //-0.XXXX + 1 = 0.XXXX (Flips to other side)
                //-0.XXXX + -5 = -5.XXXX
                if (RValue < 0)
                {
                    IntValue = RValue;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
                else//(RValue>=0)
                    IntValue = (int)RValue - 1;
            }
            else if (IntValue < 0)
            {
                //-1.XXXX + -5 = -6.XXXX
                //-6.XXXX + 5 = -1.XXXX
                //-6.XXXX + 6 = -0.XXXX
                //-5.XXXX + 6 = 0.XXXX (Flips to other side)
                //-5.XXXX + 7 = 1.XXXX
                int InvertedLValue = -IntValue;
                if (IntValue == InvertedLValue)
                    IntValue = NegativeRep;
                else if (RValue > InvertedLValue)
                    IntValue += (int)RValue - 1;
                else
                {
                    IntValue += (int)RValue;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
            }
            else
            {
                //5.XXXX + 5 = 10.XXXX
                //5.XXXX + -5 = 0.XXXX
                //5.XXXX + -6 = -0.XXXX (Flips to other side)
                //5.XXXX + -7 = -1.XXXX 
                int InversionPoint = -IntValue - 1;
                if (RValue == InversionPoint)
                    IntValue = NegativeRep;
                else if (RValue < InversionPoint)
                {
                    IntValue = (int)RValue + 1;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
                else
                    IntValue += (int)RValue;
            }
    #endif
        }

        //Replace usage of IntValue -= RValue; with IntHalfSubtraction(RValue);
        //this is copy by value and pointer version
        template<typename IntType>
        void IntHalfSubtraction(IntType RValue)
        {
    #if defined(AltDec_UseMirroredInt)
        #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
            IntValue -= RValue;
        #endif
    #else//Can be used without modifying RValue
            if (RValue==0)
                return;
            if (IntValue == 0)
                IntValue = -(int)RValue;
            else if (IntValue == NegativeRep)
            {
                //-0.XXXX - -2 = 1.XXXX
                //-0.XXXX - -1 = 0.XXXX (Flips to other side)
                //-0.XXXX - 1 = -1.XXXX 
                //-0.XXXX - 6 = -6.XXXX
                if (RValue < 0)
                    IntValue = -(int)RValue - 1;
                else
                {
                    IntValue = -(int)RValue;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
            }
            else if (IntValue < 0)
            {
                //-5.XXXX - -7 = 1.XXXX 
                //-5.XXXX - -6 = 0.XXXX            
                //-5.XXXX - -5 = -0.XXXX (Flips to other side)
                //-5.XXXX - -4 = -1.XXXX
                if (RValue == IntValue)
                    IntValue = NegativeRep;
                else if (RValue < IntValue)
                    IntValue -= (int)RValue - 1;
                else//(RValue>=LValue.Value)
                {
                    IntValue -= (int)RValue;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
            }
            else
            {
                //5.XXXX - -5 = 10.XXXX
                //5.XXXX - 5 = 0.XXXX
                //5.XXXX - 6 = -0.XXXX (Flips to other side)
                //5.XXXX - 7 = -1.XXXX
                int InversionPoint = IntValue + 1;
                if (RValue == InversionPoint)
                    IntValue = NegativeRep;
                else if (RValue > InversionPoint)
                {
                    IntValue -= (int)RValue - 1;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
                else
                    IntValue += (int)RValue;
            }
    #endif
        }

        //Reference version
        template<typename IntType=int>
        bool IntHalfEqualToOp(IntType& RValue) { return IntValue == RValue; }

        template<typename IntType=int>
        bool IntHalfEqualTo(IntType RValue) { return IntValue == RValue; }

        //Reference version
        template<typename IntType=int>
        bool IntHalfNotEqualToOp(IntType& RValue) { return IntValue != RValue; }

        template<typename IntType=int>
        bool IntHalfNotEqualTo(IntType RValue) { return IntValue != RValue; }

        /// <summary>
        /// Less than Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        template<typename AltDecVariant = AltDec>
		bool IntHalfLessThan(AltDecVariant RValue)
		{
    #if defined(AltDec_UseMirroredInt)
            if(IntValue<RValue)
                return true;
            else
                return false;
    #else
            if (IntValue == NegativeRep)
                return RValue.IntValue < 0 && RValue.IntValue != NegativeRep;
            else if (RValue.IntValue == NegativeRep)
            {
                if (IntValue < 0)
                    return true;
                else
                    return false;
            }
            else
                return IntValue<RValue.IntValue;
    #endif
		}
		
        //Reference version
        template<typename IntType=int>
        bool IntHalfLessThanOp(IntType& RValue) { return IntHalfLessThan(RValue); }
		
        /// <summary>
        /// Less than or Equal Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        template<typename AltDecVariant>
		bool IntHalfLessThanOrEqual(AltDecVariant RValue)
		{
    #if defined(AltDec_UseMirroredInt)
            if(IntValue<=RValue)
                return true;
            else
                return false;
    #else
            if (IntValue == NegativeRep)
                return RValue.IntValue < 0;
            else if (RValue.IntValue == NegativeRep)
            {
                if (IntValue >= 0)
                    return false;
                else
                    return true;
            }
            else
                return IntValue <= RValue.IntValue;
    #endif
		}
		
        //Reference version
        template<typename IntType=int>
        bool IntHalfLessThanOrEqualOp(IntType& RValue) { return IntHalfLessThanOrEqual(RValue); }
		
        /// <summary>
        /// Greater than Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        template<typename AltDecVariant>
		bool IntHalfGreaterThan(AltDecVariant RValue)
		{
    #if defined(AltDec_UseMirroredInt)
            if(IntValue>RValue)
                return true;
            else
                return false;
    #else
            if (IntValue == NegativeRep)
            {
                if (RValue.IntValue < 0)
                    return false;
                else
                    return true;
            }
            else if (RValue.IntValue == NegativeRep)
            {
                if (IntValue >= 0)
                    return true;
                else
                    return false;
            }
            else
                return IntValue > RValue.IntValue;
    #endif
		}
		
        //Reference version
        template<typename IntType=int>
        bool IntHalfGreaterThanOp(IntType& RValue) { return IntHalfGreaterThan(RValue); }

        /// <summary>
        /// Greater than or Equal to Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        template<typename AltDecVariant>
		bool IntHalfGreaterThanOrEqual(AltDecVariant RValue)
		{
    #if defined(AltDec_UseMirroredInt)
            if(IntValue>=RValue)
                return true;
            else
                return false;
    #else
            if (IntValue == RValue.IntValue)
                return true;
            if (IntValue == NegativeRep)
            {
                if (RValue.IntValue < 0)
                    return false;
                else
                    return true;
            }
            else if (RValue.IntValue == NegativeRep)
            {
                if (IntValue >= 0)
                    return true;
                else
                    return false;
            }
            else
                return IntValue >= RValue.IntValue;
    #endif
		}

        template<typename IntType=int>
        bool IntHalfGreaterThanOrEqualOp(const IntType& RValue) { return IntHalfGreaterThanOrEqual(RValue); }

#endif

    #pragma endregion MirroredIntBased Operations

protected:
    #pragma region Pi Conversion
	
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion
public:

    #pragma region Comparison Operators

    #pragma endregion Comparison Operators

    #pragma region AltDec-To-Int Comparison Functions

    #pragma endregion AltDec-To-Int Comparison Methods

    #pragma region NormalRep Integer Division Operations
protected:
#if defined(AltNum_StoreBasicFunctionsInBase)
        template<typename IntType=int>
        bool PartialIntDivOp(const IntType& rValue)
        {
            bool ResIsNegative = IntValue < 0;
            signed _int64 SelfRes;
            signed _int64 Res;
            signed _int64 IntHalfRes;
            signed _int64 DecimalRes;
            if (DecimalHalf == 0)
            {
                if (ResIsNegative)
                {
                    SelfRes = NegDecimalOverflowX * IntValue;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue;
                }
            }
            else
            {
                if (ResIsNegative)
                {
                    if(IntValue==NegativeRep)
                        SelfRes = DecimalHalf;
                    else
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                }
            }
			if(rValue<0)
			{
				ResIsNegative = !ResIsNegative;
				IntType invertedValue = -rValue;
                Res = SelfRes / invertedValue;
            #if defined(AltNum_OutputTruncatedTrailingDigits)
                TruncatedDigits = SelfRes - invertedValue * IntHalfRes;
            #endif
                IntHalfRes = Res/DecimalOverflowX;
                DecimalRes = Res - DecimalOverflowX * IntHalfRes;
			}
			else
			{
                Res = SelfRes / rValue;
            #if defined(AltNum_OutputTruncatedTrailingDigits)
                TruncatedDigits = SelfRes - rValue * IntHalfRes;
            #endif
                IntHalfRes = Res/DecimalOverflowX;
                DecimalRes = Res - DecimalOverflowX * IntHalfRes;
			}
			if(ResIsNegative)
			{
				IntValue = IntHalfRes==0? NegativeRep: (int)(-IntHalfRes);
				DecimalHalf = (int) DecimalRes;
			}
			else
			{
				IntValue = (int)IntHalfRes;
				DecimalHalf = DecimalRes;
			}
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }

        template<typename IntType=int>
        bool PartialUIntDivOp(const IntType& rValue)
        {
            bool ResIsNegative = IntValue < 0;
            signed _int64 SelfRes;
            signed _int64 Res;
            signed _int64 IntHalfRes;
            signed _int64 DecimalRes;
        #if defined(AltNum_OutputTruncatedTrailingDigits)
            signed _int64 TruncatedDigits; 
        #endif
            if (DecimalHalf == 0)
            {
                if (ResIsNegative)
                {
                    SelfRes = NegDecimalOverflowX * IntValue;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue;
                }
            }
            else
            {
                if (ResIsNegative)
                {
                    if(IntValue==NegativeRep)
                        SelfRes = DecimalHalf;
                    else
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                }
            }
            //One has SelfRes of 1000000000
            //1000000000/2 = 500000000
            Res = SelfRes / rValue;
        #if defined(AltNum_OutputTruncatedTrailingDigits)
            TruncatedDigits = SelfRes - rValue * IntHalfRes;
        #endif
            IntHalfRes = SelfRes/DecimalOverflowX;
            DecimalRes = SelfRes - DecimalOverflowX * IntHalfRes;
            if(ResIsNegative)
            {
                IntValue = IntHalfRes==0? NegativeRep: (int)(-IntHalfRes);
                DecimalHalf = (int) DecimalRes;
            }
            else
            {
                IntValue = (int)IntHalfRes;
                DecimalHalf = DecimalRes;
            }
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }
#endif
public:
/*
        template<typename IntType=int>
        void BasicUIntDivOp(const IntType& rValue)
        {
            if (rValue == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IsZero())
                return;
            if(PartialUIntDivOp(rValue))
                DecimalHalf = 1;//Prevent Dividing into nothing
            return *this;
        }

        template<typename IntType=int>
        void BasicIntDivOp(const IntType& rValue)
        {
            if (Value == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IsZero())
                return;
            if (rValue < 0)
            {
                IntType invertedValue = -rValue;
                SwapNegativeStatus();
                if(PartialUIntDivOp(invertedValue))
                    DecimalHalf = 1;//Prevent Dividing into nothing
            }
            else if(PartialUIntDivOp(rValue))
                DecimalHalf = 1;//Prevent Dividing into nothing
        }
*/

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep Integer Multiplication Operations
#if defined(AltNum_StoreBasicFunctionsInBase)
protected:
		/// <summary>
        /// Partial Multiplication Operation Between AltDec and Integer Value
        /// (Modifies owner object) 
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType=int>
        void PartialIntMultOp(const IntType& rValue)
        {
            if (DecimalHalf == 0)
            {
                IntValue *= rValue;
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    if (IntValue == NegativeRep) { IntValue = 0; }
                    else { IntValue *= -1; }
                }
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                if(rValue<0)
                {
                    SelfIsNegative = !SelfIsNegative;
                    SRep *= -rValue;
                }
                else
                    SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = SelfIsNegative ? NegativeRep : 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
        }

		/// <summary>
        /// Partial Multiplication Operation Between AltDec and Integer Value
        /// Applies after making sure rValue is positive
        /// (Modifies owner object)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType=int>
        void PartialIntMultOpV2(const IntType& rValue)
        {
            if (DecimalHalf == 0)
            {
                IntValue *= rValue;
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    if (IntValue == NegativeRep) { IntValue = 0; }
                    else { IntValue *= -1; }
                }
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = SelfIsNegative ? NegativeRep : 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
        }
#endif
public:/*
        /// <summary>
        /// Multiplication Operation Between AltDec and Integer Value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<typename IntType>
        void BasicIntMultOp(const IntType& rValue)
        {
            if (IntValue == 0 && DecimalHalf == 0)
                return *this;
            if (rValue == 0)
                SetAsZero();
            else
                PartialIntMultOp(rValue);
        }

        /// <summary>
        /// Multiplication Operation Between AltDec and unsigned Integer Value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<typename IntType>
        void BasicUIntMultOp(const IntType& rValue)
        {
            if (IntValue == 0 && DecimalHalf == 0)
                return *this;
            if (rValue == 0)
                SetAsZero();
            else
                PartialIntMultOpV2(rValue);
        }

        /// <summary>
        /// Multiplication Operation Between AltDec and unsigned Integer Value
        /// (Avoids modifying owner object by copying lValue)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<typename IntType = int>
        AltNumBase BasicMultiplyByInt(const IntType& rValue) { AltDec self = *this; self.BasicIntMultOp(rValue); return self; }

        /// <summary>
        /// Multiplication Operation Between AltDec and unsigned Integer Value
        /// (Avoids modifying owner object by copying lValue)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<typename IntType = int>
        AltNumBase BasicMultiplyByUInt(const IntType& rValue) { AltDec self = *this; self.BasicUIntMultOp(rValue); return self; }
*/

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations
#if defined(AltNum_StoreBasicFunctionsInBase)
protected:
        /// <summary>
        /// Addition Operation that skips negative zero(for when decimal half is empty)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>void</returns>
        template<typename IntType=int>
        void NRepSkippingIntAddOp(const IntType& rValue)
        {
            if (rValue == 0)
                return;
            if (IntValue == 0)
                IntValue = (int)rValue;
            else
                IntHalfAddition(rValue);
            return;
        }

public:
/*
        /// <summary>
        /// Basic Addition Operation between AltDec and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<typename IntType=int>
        virtual AltNumBase& BasicIntAddOp(const IntType& rValue)
        {
            if(DecimalHalf==0)
                NRepSkippingIntAddOp(rValue);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfAddition(rValue);
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = DecimalOverflow - DecimalHalf;
            }
            return *this;
        }*/
#endif
    #pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations
#if defined(AltNum_StoreBasicFunctionsInBase)
protected:
        /// <summary>
        /// Subtraction Operation that skips negative zero(for when decimal half is empty)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>void</returns>
        template<typename IntType=int>
        void NRepSkippingIntSubOp(const IntType& rValue)
        {
            if (rValue == 0)
                return;
            if (IntValue == 0)
                IntValue = -(int)rValue;
            else
                IntHalfSubtraction(rValue);
            return;
        }

public:

		/// <summary>
        /// Basic Subtraction Operation between AltDec and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>AltDec&</returns>
        template<typename ReturnType=AltNumBase, typename IntType=int>
        ReturnType BasicIntSubBaseOp(const IntType& rValue)
        {
            if (DecimalHalf == 0)
                NRepSkippingIntSubOp(Value);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfSubtraction(rValue);
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = DecimalOverflow - DecimalHalf;
            }
            return *this;
        }
#endif

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep Integer Bitwise Operations

	#pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations

    #pragma endregion Mixed Fraction Operations

	#pragma region NormalRep AltNum Division Operations
#if defined(AltNum_StoreBasicFunctionsInBase)
protected:
		/// <summary>
        /// Basic Division Operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param
        template<typename AltDecVariant>
        bool PartialDivBaseOp(const AltDecVariant& rValue)
        {
			bool ResIsPositive = true;
			signed _int64 SelfRes;
			signed _int64 ValueRes;
			if(IntValue<0)
			{
                if (IntValue == NegativeRep)
                    SelfRes = DecimalHalf;
                else
			        SelfRes = NegDecimalOverflowX*IntValue + DecimalHalf;
                if (rValue < 0)
                {
                    if (rValue.IntValue == NegativeRep)
                        ValueRes = rValue.DecimalHalf;
                    else
                        ValueRes =  NegDecimalOverflowX* rValue.IntValue + rValue.DecimalHalf;
                }
				else
				{
				    ResIsPositive = false;
					ValueRes = DecimalOverflowX * rValue.IntValue +rValue.DecimalHalf;
				}
			}
			else
			{
                
				SelfRes = DecimalOverflowX* IntValue+DecimalHalf;
			    if(rValue<0)
				{
				    ResIsPositive = false;
					ValueRes = rValue.IntValue==NegativeRep ? DecimalHalf: NegDecimalOverflowX*IntValue +rValue.DecimalHalf;
				}
				else
					ValueRes = DecimalOverflowX* rValue.IntValue +rValue.DecimalHalf;
			}
			
			signed _int64 IntHalfRes = SelfRes / ValueRes;
			signed _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
			IntValue = IntHalfRes==0&&ResIsPositive==false?NegativeRep:IntHalfRes;
			DecimalHalf = DecimalRes;
			if(IntHalfRes==0&&DecimalRes==0)
				return true;
			else
				return false;
        }

        template<typename AltDecVariant>
        bool PartialUnsignedDivBaseOp(const AltDecVariant& rValue)
        {
            bool ResIsPositive = true;
            signed _int64 SelfRes;
            signed _int64 ValueRes;
            if (IntValue < 0)
            {
                if (IntValue == NegativeRep)
                    SelfRes = DecimalHalf;
                else
                    SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                ResIsPositive = false;
                ValueRes = DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
            }
            else
            {

                SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                ValueRes = DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
            }

            signed _int64 IntHalfRes = SelfRes / ValueRes;
            signed _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
            IntValue = IntHalfRes == 0 && ResIsPositive == false ? NegativeRep : IntHalfRes;
            DecimalHalf = DecimalRes;
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }

public:/*
		/// <summary>
        /// Basic Division Operation(without checking for special representation variations or zero)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        template<typename AltDecVariant>
        virtual AltDecVariant& BasicUnsignedDivOp(const AltDecVariant& rValue)
        {
			if (PartialDivOp(rValue))//Prevent Dividing into nothing
				DecimalHalf = 1;
            return *this;
        }

		/// <summary>
        /// Basic Division Operation(without checking for special representation variations or zero)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        template<typename AltDecVariant>
        virtual AltDec& BasicUnsignedDivOp(const AltDecVariant& rValue)
        {
            if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf = 1;
            return *this;
        }*/
#endif
	#pragma endregion NormalRep AltNum Division Operations

	#pragma region NormalRep AltNum Multiplication Operations
#if defined(AltNum_StoreBasicFunctionsInBase)
		/// <summary>
        /// Basic Multiplication Operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        template<typename AltDecVariant>
		AltDecVariant& BasicMultBaseOp(const AltDecVariant& rValue)
		{
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                {
                    IntValue *= rValue.IntValue;
                }
                else
                {
                    __int64 rRep;
                    bool ResIsNegative = rValue < 0;
                    if(ResIsNegative)
                    {
                        rRep = rValue.IntValue == NegativeRep ? rValue.DecimalHalf : DecimalOverflowX * -rValue.IntValue + rValue.DecimalHalf;
                        if(IntValue<0)
                        {
                            ResIsNegative = false;
                            rRep *= IntHalfAsAbs();
                        }
                        else
                        {
                            ResIsNegative = true;
                            rRep *= IntValue;
                        }
                    }
                    else
                    {
                        rRep = rValue.IntValue == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
                        if(IntValue<0)
                        {
                            ResIsNegative = true;
                            rRep *= IntHalfAsAbs();
                        }
                        else
                        {
                            ResIsNegative = false;
                            rRep *= IntValue;
                        }
                    }
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)ResIsNegative ? OverflowVal * -1 : OverflowVal;
                        DecimalHalf = (signed int)rRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                        if(DecimalHalf==0)
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
                if (IntValue == 0)
                {
                    __int64 SRep = (__int64)DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= DecimalOverflowX;
                    if (rValue.IntValue == 0)
                    {
                        DecimalHalf = (signed int)SRep;
                        if(SelfIsNegative&&DecimalHalf!=0)
                            IntValue = NegativeRep;
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {
                        DecimalHalf = (signed int)SRep;
                        if(SelfIsNegative==false&&DecimalHalf!=0)
                            IntValue = NegativeRep;
                    }
                    else
                    {
                        SRep += (__int64)DecimalHalf * rValue.IntValue;
                        if (SRep >= DecimalOverflowX)
                        {
                            __int64 OverflowVal = SRep / DecimalOverflowX;
                            SRep -= OverflowVal * DecimalOverflowX;
                            IntValue = SelfIsNegative?-(int)OverflowVal:OverflowVal;
                            DecimalHalf = (signed int)SRep;
    						return *this;
                        }
                        else
                        {
                            DecimalHalf = (signed int)SRep;
                            if(DecimalHalf==0)
                            {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                                IntValue = 0;
                                //if(ExtraRep!=0)
                                //    ExtraRep = 0;
                                return *this;
    #else
                                DecimalHalf = 1;
    #endif
                            }
                            if(SelfIsNegative)
                                IntValue = NegativeRep;
                            return *this;
                        }
                    }
                }
                else if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = DecimalOverflowX * IntValue + DecimalHalf;
                    if (rValue < 0)
                    {
                        SRep *= -rValue.IntValue;
                        SelfIsNegative = !SelfIsNegative;
                    }
                    else
                        SRep *= rValue.IntValue;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                            DecimalHalf = (signed int)SRep;
                            if(DecimalHalf==0)
                            {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                                IntValue = 0;
                                //if(ExtraRep!=0)
                                //    ExtraRep = 0;
                                return *this;
    #else
                                DecimalHalf = 1;
    #endif
                            }
                            IntValue = SelfIsNegative ? NegativeRep : 0;
                            return *this;
                    }
                }
                else if (rValue.IntValue == 0)
                {
                    __int64 SRep = DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= DecimalOverflowX;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
    #else
                            DecimalHalf = 1;
    #endif
                        }
                        IntValue = SelfIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
                else if (rValue.IntValue == NegativeRep)
                {
                    SelfIsNegative = !SelfIsNegative;
                    __int64 SRep = DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= DecimalOverflowX;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = SelfIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                    //X.Y *.V
                    if(rValue<0)
                    {
                        SRep *= -rValue.IntValue;
                        SelfIsNegative = !SelfIsNegative;
                    }
                    else
                        SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)DecimalOverflow;
                    if (IntHalf == 0)
                        IntValue = (signed int)SelfIsNegative ? NegativeRep : 0;
                    else
                        IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf;
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0&&ExtraRep!=0)
                ExtraRep = 0;
#else
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
#endif
            return *this;
		}

		/// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned AltDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        template<typename AltDecVariant>
		AltDecVariant& BasicUnsignedMultBaseOp(const AltDecVariant& rValue)
		{
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                {
                    IntValue *= rValue.IntValue;
                }
                else
                {
                    __int64 rRep = rValue.IntValue == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
                    bool ResIsNegative = IntValue<0;
                    if(ResIsNegative)
                    {
                        ResIsNegative = false;
                        rRep *= IntHalfAsAbs();
                    }
                    else
                    {
                        ResIsNegative = true;
                        rRep *= IntValue;
                    }
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)ResIsNegative ? OverflowVal * -1 : OverflowVal;
                        DecimalHalf = (signed int)rRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                        if(DecimalHalf==0)
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
            else if (IntValue == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= DecimalOverflowX;
                if (rValue.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf = 1;
                #else
                    if (DecimalHalf == 0&&ExtraRep!=0)
                        ExtraRep = 0;
                #endif
                    return *this;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntValue;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntValue = OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0&&ExtraRep!=0)
                            ExtraRep = 0;
                #else
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
                #endif
                        return *this;
                    }
                }
            }
            else if (IntValue == NegativeRep)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= DecimalOverflowX;
                if (rValue.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                    if(DecimalHalf==0)
                    {
                        IntValue = 0;
                        if(ExtraRep!=0)
                            ExtraRep = 0;
                    }
                    return *this;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntValue;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntValue = -OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        return *this;
                    }
                }
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            //if(ExtraRep!=0)
                            //    ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        IntValue = SelfIsNegative ? NegativeRep : 0;
                    }
				    return *this;
                }
                else if (rValue.IntValue == 0)
                {
                    __int64 SRep = DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= DecimalOverflowX;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            //if(ExtraRep!=0)
                            //    ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        IntValue = SelfIsNegative ? NegativeRep : 0;
                    }
                    return *this;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)DecimalOverflow;
                    if (IntHalf == 0)
                        IntValue = (signed int)SelfIsNegative ? NegativeRep : 0;
                    else
                        IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf;
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
            //if(DecimalHalf==0&&IntValue==0&&ExtraRep!=0)
            //    ExtraRep = 0;
    #else
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
    #endif
            return *this;
		}
#endif
	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region NormalRep AltNum Addition Operations
#if defined(AltNum_StoreBasicFunctionsInBase)
        /// <summary>
        /// Basic Addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<typename AltDecVariant>
        void BasicAddBaseOp(const AltDecVariant& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfAddition(rValue.IntValue);
            if (rValue.DecimalHalf != 0)
            {
                if (rValue.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = DecimalOverflow - DecimalHalf;
        }

        /// <summary>
        /// Basic Addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<typename AltDecVariant>
        void BasicUnsignedAddBaseOp(const AltDecVariant& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfAddition(rValue.IntValue);
            if (rValue.DecimalHalf != 0)
            {
                if (WasNegative)
                {
                    DecimalHalf -= rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; ++IntValue; }
                    else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; --IntValue; }
                }
                else
                {
                    DecimalHalf += rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; --IntValue; }
                    else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; ++IntValue; }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = DecimalOverflow - DecimalHalf;
        }
#endif

	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations
#if defined(AltNum_StoreBasicFunctionsInBase)
        /// <summary>
        /// Basic Subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<typename AltDecVariant>
		void BasicSubBaseOp(const AltDecVariant& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfSubtraction(rValue.IntValue);
            //Now deal with the decimal section
            if(rValue.DecimalHalf!=0)
            {
                if (rValue.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; --IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; --IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = DecimalOverflow - DecimalHalf;
        }

        /// <summary>
        /// Basic Subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<typename AltDecVariant>
		void BasicUnsignedSubBaseOp(const AltDecVariant& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfSubtraction(rValue.IntValue);
            //Now deal with the decimal section
            if(rValue.DecimalHalf!=0)
            {
                if (WasNegative)//-4.5 - 5.6
                {
                    DecimalHalf += rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; ++IntValue; }
                    else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; --IntValue; }
                }
                else//0.995 - 1
                {
                    DecimalHalf -= rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += DecimalOverflow; --IntValue; }
                    else if (DecimalHalf >= DecimalOverflow) { DecimalHalf -= DecimalOverflow; ++IntValue; }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = DecimalOverflow - DecimalHalf;
        }
#endif
	#pragma endregion NormalRep AltNum Subtraction Operations

    #pragma region Other Division Operations

	#pragma endregion Other Division Operations	

    #pragma region Other Multiplication Operations

    #pragma endregion Other Multiplication Operations

    #pragma region Other Addition Operations

    #pragma endregion Other Addition Operations

    #pragma region Other Subtraction Operations

    #pragma endregion Other Subtraction Operations

	};
}