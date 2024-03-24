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
#include <compare>
#include "..\OtherFunctions\VariableConversionFunctions.h"
#include "..\IntegerConcept.hpp"

#pragma region PreprocessorToggles
#include "MediumDecPreprocessors.h"
#pragma endregion PreprocessorToggles

namespace BlazesRusCode
{
#if !defined(AltNum_UseIntForDecimalHalf)
	struct PartialInt {
	#pragma options align=bit_packed
	//Stores Digits XXX XXX XXX
	unsigned int Value:30;
	//Can store up to 4 Flag states including normal state at 0
	unsigned int Flags:2;
	#pragma options align=reset
		PartialInt(unsigned int value=0, unsigned int flags=0)
		{
			Value = value;
			Flags = flags;
		}
	};
#endif

	//Base class for MediumDec to help initial structure of classes
	//MediumDec class holds finished implimentation of AltNum data including static variables
	// that might mess up polymorphism functionality of the class
    class DLL_API MediumDecBase
    {
#if !defined(AltNum_DisableDefaultStringFormatOption)
	enum class DefaultStringFormatEnum : int
	{
	
	}
#endif
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

	protected:
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned _int64 const DecimalOverflowX = 1000000000;

#if defined(AltNum_UseIntForDecimalHalf)

#endif

        /// <summary>
        /// Value when IntValue is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
        static signed int const NegativeRep = -2147483648;

	#pragma region DigitStorage
        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        signed int IntValue;
	
        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
#if defined(AltNum_UseIntForDecimalHalf)
        signed int DecimalHalf;
#else
        PartialInt DecimalHalf;
#endif

#pragma endregion DigitStorage

public:

        bool IsNegative()
        {
    #if !defined(AltDec_UseMirroredInt)
            return IntValue<0;
    #else
            return IntValue.IsNegative();
    #endif
        }

#pragma region Check_if_Zero
        //Detect if at exactly zero
		virtual bool IsZero() const
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

        virtual void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0;
        }
#pragma endregion Check_if_Zero

        signed int GetIntegerPartition() const
        {
            if(IntValue == NegativeRep)
                return 0;
            else
                return IntValue;
        }
		
		//Get DecimalHalf without special status flags after ExtraRep applied
        virtual signed int GetIntegerHalf()
        {
            if(IntValue == NegativeRep)
                return 0;
            else
                return IntValue;
        }

        void SetIntegerPartition(int intPart) const
        {
            IntValue = intPart;
        }

        virtual bool IsWholeNumber()
        {
            return DecimalHalf==0;
        }

        bool IsAtOrBelowTwo()
        {
            if(IntValue<=1)
                return true;
            else
                return IntValue==2 && DecimalHalf==0;
        }

        bool IsAtOrBelowOne()
        {
            if(IntValue<=0)
                return true;
            else
                return IntValue==1 && DecimalHalf==0;
        }

        //Return IntValue part as Absolute value
        signed int IntHalfAsAbs() const
        {
            if (IsAtZeroInt())
                return 0;
            else if (IntValue < 0)
                return -IntValue;
            else
                return IntValue;
        }
		
		//Get DecimalHalf without special status flags after ExtraRep applied
		virtual unsigned int GetDecimalHalf()
		{
	#if defined(AltNum_UseIntForDecimalHalf)
			return DecimalHalf;
	#else
			return DecimalHalf.Value;
	#endif
		}

        std::string IntHalfAsString() const
        {
            if (IntValue == NegativeRep)
                return "-0";
            return VariableConversionFunctions::IntToStringConversion(IntValue);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        MediumDecBase(const int& intVal, const PartialInt& decVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
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

    protected:
    #pragma region Const Representation values
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

    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Enum representing value type stored
        /// </summary>
        enum class RepType: int
        {
            NormalType = 0,
        };

        void std::string RepTypeAsString(RepType& repType)
        {
            return "NormalType";
        }

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        virtual RepType GetRepType()
        {
            return RepType::NormalType;
        }


    #pragma endregion RepType

public:
    #pragma region PiNum Setters
	//Not used for this variant
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
	//Not used for this variant
    #pragma endregion ENum Setters

    #pragma region Fractional Setters
	//Not used for this variant
    #pragma endregion Fractional Setters
    
    #pragma region MixedFrac Setters
	//Not used for this variant
    #pragma endregion MixedFrac Setters

    #pragma region Infinity Setters
        //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
        // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
        virtual void SetAsInfinity()
        {
        #if defined(AltNum_EnableInfinityRep)
            IntValue = 1; DecimalHalf = -2147483648;
        #else
            throw "Current AltNum doesn't support setting value as infinity";
        #endif
        }

        virtual void SetAsNegativeInfinity()
        {
        #if defined(AltNum_EnableInfinityRep)
            IntValue = -1; DecimalHalf = -2147483648;
        #else
            throw "Current AltNum doesn't support setting value as negative infinity";
        #endif
        }
	#endif
    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters
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

    #pragma endregion ApproachingZero Setters

	#pragma region NaN Setters
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
    #pragma endregion NaN Setters
//public:

    #pragma region ValueDefines
    //Static member variables are in full version of the class to prevent conflict on derivation
    #pragma endregion ValueDefines

    #pragma region String Commands
	
    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes
protected:

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetFloatVal(float Value)
        {
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
                Value -= (float)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
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
        void SetDoubleVal(double Value)
        {
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
                Value -= (double)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
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
        void SetDecimalVal(ldouble Value)
        {
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
                Value -= (ldouble)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
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
        void SetBooleanVal(bool Value)
        {
            IntValue = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
        }

        //Need to copy the const reference before enable this section

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetFloatCVal(const float& value)
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
        virtual void SetDoubleCVal(const double& value)
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
        virtual void SetDecimalCVal(const ldouble& value)
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
        virtual void SetBooleanCVal(const bool& Value)
        {
            IntValue = Value ? 1 : 0;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetIntCVal(const int& Value)
        {
            IntValue = Value; DecimalHalf = 0;
        }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #pragma endregion ConvertToOtherTypes

protected:
    #pragma region MirroredIntBased Operations
    //Templates can not be virtual
        
        template<IntegerType IntType=int>
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

        template<IntegerType IntType=int>
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

        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
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
        }

    #pragma endregion MirroredIntBased Operations

    #pragma region Pi Conversion
	
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion
public:

    #pragma region Comparison Operators

        //Reference version
        template<IntegerType IntType=int>
        bool IntHalfEqualToOp(IntType& RValue) { return IntValue == RValue; }

        template<IntegerType IntType=int>
        bool IntHalfEqualTo(IntType RValue) { return IntValue == RValue; }

        //Reference version
        template<IntegerType IntType=int>
        bool IntHalfNotEqualToOp(IntType& RValue) { return IntValue != RValue; }

        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
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

        template<IntegerType IntType=int>
        bool IntHalfGreaterThanOrEqualOp(const IntType& RValue) { return IntHalfGreaterThanOrEqual(RValue); }

    auto operator<=>(const MediumDecBase& that) const
    {
    #if !defined(AltNum_EnableImaginaryNum)//&&!defined(AltNum_EnableNaN)&&!defined(AltNum_EnableUndefinedButInRange)&&!defined(AltNum_EnableNilRep)
        if (std::weak_ordering IntHalfCmp = LValue.IntValue <=> RValue.IntValue; IntHalfCmp != 0)
            return IntHalfCmp;
        if (std::weak_ordering DecimalHalfCmp = LValue.DecimalHalf <=> RValue.DecimalHalf; DecimalHalfCmp != 0)
            return DecimalHalfCmp;
    #else
        if (std::partial_ordering IntHalfCmp = IntValue <=> that.IntValue; IntHalfCmp != 0)
            return IntHalfCmp;
        if (std::partial_ordering DecimalHalfCmp = DecimalHalf <=> that.DecimalHalf; DecimalHalfCmp != 0)
            return DecimalHalfCmp;
    #endif
    }

    bool operator==(const int& that) const
    {
        if (IntValue!=that)
            return false;
        if (DecimalHalf!=0)
            return false;
        return true;
    }

    bool operator==(const MediumDecBase& that) const
    {
        if (IntValue!=that.IntValue)
            return false;
        if (DecimalHalf!=that.IntValue)
            return false;
    }
    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:
        template<IntegerType IntType=int>
        virtual void PartialIntDivOp(const IntType& rValue)
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
        }

        //Version of PartialIntDivOp that returns TruncatedDigits
        template<IntegerType IntType=int>
        virtual signed _int64 TrailingIntDivOp(const IntType& rValue)
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
                TruncatedDigits = SelfRes - rValue * IntHalfRes;
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
        #if defined(AltNum_OutputTruncatedTrailingDigits)
            return TruncatedDigits;//Return any truncated digits lost in division operation
        #endif
        }

        template<IntegerType IntType=int>
        virtual void PartialUIntDivOp(const IntType& rValue)
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
        }

        //Version of PartialUIntDivOp that returns TruncatedDigits
        template<IntegerType IntType=int>
        virtual signed _int64 TrailingUIntDivOp(const IntType& rValue)
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
            TruncatedDigits = SelfRes - rValue * IntHalfRes;
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
            return TruncatedDigits;//Return any truncated digits lost in division operation
        }
public:

        template<IntegerType IntType=int>
        virtual void BasicUIntDivOp(const IntType& rValue)
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
            if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            PartialIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
        }

        template<IntegerType IntType=int>
        virtual void BasicIntDivOp(const IntType& rValue)
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
            PartialUIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
            return *this;
        }

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
		/// <summary>
        /// Partial Multiplication Operation Between AltDec and Integer Value
        /// (Modifies owner object) 
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
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
        MediumDecBase BasicMultiplyByInt(const IntType& rValue) { AltDec self = *this; self.BasicIntMultOp(rValue); return self; }

        /// <summary>
        /// Multiplication Operation Between AltDec and unsigned Integer Value
        /// (Avoids modifying owner object by copying lValue)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<typename IntType = int>
        MediumDecBase BasicMultiplyByUInt(const IntType& rValue) { AltDec self = *this; self.BasicUIntMultOp(rValue); return self; }
*/

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations
protected:
        /// <summary>
        /// Addition Operation that skips negative zero(for when decimal half is empty)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>void</returns>
        template<IntegerType IntType=int>
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
        template<IntegerType IntType=int>
        virtual MediumDecBase& BasicIntAddOp(const IntType& rValue)
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
    #pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations
protected:
        /// <summary>
        /// Subtraction Operation that skips negative zero(for when decimal half is empty)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>void</returns>
        template<IntegerType IntType=int>
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
        template<typename ReturnType=MediumDecBase, typename IntType=int>
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

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep Integer Bitwise Operations

	#pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations

    #pragma endregion Mixed Fraction Operations

	#pragma region NormalRep AltNum Division Operations
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
		
	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region NormalRep AltNum Addition Operations
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

	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations
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