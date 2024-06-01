// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecPreprocessors.h"
//#include "..\VirtualTableBase.hpp"//Virtual Structure for the class to make sure can override virtually

#include <string>
#include <cmath>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "..\Concepts\MediumDecVariantConcept.hpp"


#include "..\AlternativeInt\MirroredInt.hpp"
#include "..\AlternativeInt\PartialInt.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumDecBase;

    //Reduced version of MediumDec result for modulus result and other stuff
    class DLL_API MediumDecBase : public AltNumBase//AltNumBase is used as base class to identify as MediumDec variant
    {
    public:
    #pragma region DigitStorage

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        MirroredInt IntHalf;

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        PartialInt DecimalHalf;

		//Return IntHalf as signed int
        signed int GetIntHalf()
        {
			return (signed int)IntHalf;
        }

    #pragma endregion DigitStorage

		//Maximum IntHalf that can be stored inside IntHalf field
        static const MirroredInt MaxIntHalf;

		//Minimum IntHalf that can be stored inside IntHalf field
        static const MirroredInt MinIntHalf;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned _int64 const DecimalOverflowX = 1000000000;

        /// <summary>
        /// Value when IntHalf is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
        static MirroredInt const NegativeRep;

    #pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecBase(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecBase(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        MediumDecBase& operator=(const MediumDecBase& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumDecBase& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<MediumDecVariant VariantType>
        auto operator[](VariantType variantValue) const
        {
            MediumDecBase newSelf = MediumDecBase(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

    #pragma endregion class_constructors

    #pragma region Negative_Status

        bool IsNegative() const
        { return IntHalf.IsNegative(); }

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        { IntHalf.Sign ^= 1; }

    #pragma endregion Negative_Status


    #pragma region Check_if_value

		//Set value as exactly zero
        void SetAsZero()
        { IntHalf = 0; DecimalHalf = 0; }

		//Set value as exactly one
        void SetAsOne()
        { IntHalf = 1; DecimalHalf = 0; }
		
		//Set as +-1 while keeping current sign
        void SetAsOneVal()
        { IntHalf.Value = 1; DecimalHalf = 0; }

        void SetAsValues(const MirroredInt& intVal = MirroredInt::Zero, const PartialInt& decVal = PartialInt::Zero)
        { IntHalf = 0; DecimalHalf = 0; }

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt() const
        { return IntHalf.Value==0; }

        bool IsNotAtZeroInt() const
        { return IntHalf.Value!=0; }

        bool IsAtOneInt() const
        { return IntHalf.Value==1; }

        bool IsNotAtOneInt() const
        { return IntHalf.Value!=1; }

        //Detect if at exactly zero(only overridden with MixedDec)
		bool IsZero() const
		{ return DecimalHalf==0&&IntHalf.Value==0; }
		
		bool IsOne() const
        { return DecimalHalf==0&&IntHalf==MirroredInt::One; }
		
		bool IsNegOne() const
        { return DecimalHalf==0&&IntHalf==MirroredInt::NegativeOne; }
		
		bool IsOneVal() const
		{ return DecimalHalf==0&&IntHalf.Value==1; }

		bool IsOneVariantVal() const
		{ return DecimalHalf.Value==0&&IntHalf.Value==1; }

    #pragma endregion Check_if_value

    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        { IntHalf = MaxIntHalf; DecimalHalf = 999999999; }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        { IntHalf = MinIntHalf; DecimalHalf = 999999999; }
	
    #pragma endregion RangeLimits

    #pragma region ValueSetters

        /// <summary>
        /// Sets value to Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        void  SetValueToPiNum()
        {
            IntHalf = 3; DecimalHalf = 141592654;
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToHundredMilPiNum()
        {
            IntHalf = 314159265; DecimalHalf = 358979324;
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToTenMilPiNum()
        {
            IntHalf = 31415926; DecimalHalf = 535897932;
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToOneMilPiNum()
        {
            IntHalf = 3141592; DecimalHalf = 653589793;
        }

        //10xPi(Rounded to 9th decimal digit)
        void  SetValueToTenPiNum()
        {
            IntHalf = 31; DecimalHalf = 415926536;
        }

        /// <summary>
        /// Euler's number rounded to 9th digit(2.718281828)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        void  SetValueToENum()
        {
            IntHalf = 2; DecimalHalf = 718281828;
        }
        
        //Sets value to value at 0.5
        void  SetValueToPoint5()
        {
            IntHalf = 0; DecimalHalf = 500000000;
        }

        void  SetValueToJustAboveZero()
        {
            IntHalf = 0; DecimalHalf = 1;
        }

        /// <summary>
        /// Sets the value at .000001000
        /// </summary>
        void  SetValueToOneMillionth()
        {
            IntHalf = 0; DecimalHalf = 1000;
        }

        /// <summary>
        /// Sets the value at "0.005"
        /// </summary>
        /// <returns>MediumDec</returns>
        void  SetValueToFiveThousandth()
        {
            IntHalf = 0; DecimalHalf = 5000000;
        }

        /// <summary>
        /// Sets the value at "0.000005"
        /// </summary>
        void  SetValueToFiveMillionth()
        {
            IntHalf = 0; DecimalHalf = 5000;
        }

        //0e-7
        void  SetValueToTenMillionth()
        {
            IntHalf = 0; DecimalHalf = 100;
        }

        /// <summary>
        /// Sets the value to .000000010
        /// </summary>
        void  SetValueToOneHundredMillionth()
        {
            IntHalf = 0; DecimalHalf = 10;
        }

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        void  SetValueToLN10()
        {
            IntHalf = 2; DecimalHalf = 302585093;
        }

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        void  SetValueToLN10()
        {
            IntHalf = 0; DecimalHalf = 434294482;
        }

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        void  SetValueToHalfLN10Mult()
        {
            IntHalf = 0; DecimalHalf = 868588964;
        }

    #pragma endregion ValueSetters

    #pragma region String Commands

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(const std::string& Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecBase(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const std::string& Value)
        {
            this->ReadString(Value);
        }

public:

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToString();

        /// <summary>
        /// Converts to string with digits filled in even when empty
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToFullString();

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToString(); }

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetFloatVal(const float& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			float lValue = Value;
            bool IsNegative = Value < 0.0f;
            if (IsNegative) { lValue *= -1.0f; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0f)
            {
                if (IsNegative)
					IntHalf = MirroredInt(2147483647,0);
                else
                    IntHalf = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (float)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                IntHalf = MirroredInt((unsigned int)WholeValue,IsNegative?0:1);
            }
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDoubleVal(const double& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			double lValue = Value;
            bool IsNegative = Value < 0.0;
            if (IsNegative) { lValue *= -1.0; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0)
            {
                if (IsNegative)
					IntHalf = MirroredInt(2147483647,0);
                else
                    IntHalf = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (double)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                IntHalf = MirroredInt((unsigned int)WholeValue,IsNegative?0:1);
            }
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDecimalVal(const long double& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			long double lValue = Value;
            bool IsNegative = Value < 0.0L;
            if (IsNegative) { lValue *= -1.0L; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (lValue >= 2147483648.0L)
            {
                if (IsNegative)
					IntHalf = MirroredInt(2147483647,0);
                else
                    IntHalf = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(lValue);
                lValue -= (long double)WholeValue;
                DecimalHalf = (signed int)lValue * 10000000000;
                IntHalf = MirroredInt((unsigned int)WholeValue,IsNegative?0:1);
            }
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetBoolVal(const bool& Value)
        {
            IntHalf = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(const int& Value)
        {
			if(Value<0)
			{
				IntHalf.Sign = 0;
				IntHalf.Value = -Value;
			}
			else
				IntHalf = Value;
			DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const float& Value){ this->SetFloatVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const double& Value){ this->SetDoubleVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const long double& Value){ this->SetDecimalVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const bool& Value){ this->SetBoolVal(Value); }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

        //To-Do: Add more exact conversion from floating point format to MediumDec variant

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        float toFloat()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            float Value;
            if (IntHalf.IsNegative())
            {
                Value = (float)-IntHalf.Value;
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
                Value = (float)IntHalf.Value;
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntHalf.Value==0)//Exponent is negative
			{
				//To-Do:Add code here
			}
			else
			{
				//To-Do:Add code here
			}
			return 0.0f;//Placeholder
	#endif
        }

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        double toDouble()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
		    double Value;
            if (IntHalf < 0)
            {
                Value = (double)-IntHalf.Value;
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
                Value = (double)IntHalf.Value;
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntHalf.Value==0)//Exponent is negative
			{
				//To-Do:Add code here
			}
			else
			{
				//To-Do:Add code here
			}
			return 0.0;//Placeholder
	#endif
        }

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        long double toDecimal()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            long double Value;
            if (IntHalf < 0)
            {
                Value = (long double)-IntHalf.Value;
                if (DecimalHalf != 0) { Value -= ((long double)DecimalHalf * 0.000000001L); }
            }
            else
            {
                Value = (long double)IntHalf.Value;
                if (DecimalHalf != 0) { Value += ((long double)DecimalHalf * 0.000000001L); }
            }
            return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntHalf.Value==0)//Exponent is negative
			{
				//To-Do:Add code here
			}
			else
			{
				//To-Do:Add code here
			}
			return 0.0L;//Placeholder
	#endif
        }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toInt() { return (signed int) IntHalf; }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toUInt() { return (unsigned int) IntHalf; }

        bool toBool() { return IntHalf.IsZero() ? false : true; }

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }
		
        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDouble(); }
		
        /// <summary>
        /// MediumDec Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator long double() { return toDecimal(); }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
		template<MediumDecVariant VariantType=MediumDecBase>
		std::strong_ordering BasicComparisonV1(const VariantType& that) const
		{
			if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			unsigned int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
		//Compare only as if in NormalType representation mode
		std::strong_ordering BasicIntComparison(const int& that) const
		{
			if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = DecimalHalf.Value>0?1:0;
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}

public:

		std::strong_ordering operator<=>(const MediumDecBase& that) const
		{//return BasicComparison(that);
			if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			unsigned int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}

		std::strong_ordering operator<=>(const int& that) const
		{
			return BasicIntComparison(that);
		}

		bool operator==(const MediumDecBase& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.DecimalHalf)
				return false;
            return true;
		}

		bool operator!=(const MediumDecBase& that) const
		{
			if (IntHalf!=that.IntHalf)
				return true;
			if (DecimalHalf!=that.DecimalHalf)
				return true;
            return false;
		}

		bool operator==(const int& that) const
		{
			if (IntHalf!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			return true;
		}

		bool operator!=(const int& that) const
		{
			if (IntHalf!=that)
				return true;
			if (DecimalHalf!=0)
				return true;
			return false;
		}

    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:

        template<IntegerType IntType=unsigned int>
        void PartialUIntDivOpV1(const IntType& lValue)
        {//Avoid using with special status representations such as approaching zero or result will be incorrect
            unsigned _int64 SelfRes;
            unsigned _int64 Res;
            unsigned _int64 IntHalfRes;
            unsigned _int64 DecimalRes;
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntHalf.Value: DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
            Res = SelfRes / rValue;
			
            IntHalfRes = Res/DecimalOverflowX;
            DecimalRes = Res - DecimalOverflowX * IntHalfRes;
		    IntHalf.Value = (unsigned int)IntHalfRes;
			DecimalHalf.Value = (unsigned int)DecimalRes;
        }

        template<IntegerType IntType=signed int>
        void PartialIntDivOpV1(const IntType& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                PartialUIntDivOp(-Value);
            }
            else
                PartialUIntDivOp(Value);
        }

public:
        void PartialUIntDivOp() { PartialUIntDivOpV1<unsigned int>; }
        void PartialIntDivOp() { PartialIntDivOpV1<signed int>; }
        void PartialUInt64DivOp() { PartialUIntDivOpV1<unsigned long long>; }
        void PartialInt64DivOp() { PartialIntDivOpV1<signed long long>; }

        void UnsignedPartialIntDivOp() { PartialUIntDivOpV1<signed int>; }
        void UnsignedPartialInt64DivOp() { PartialUIntDivOpV1<signed long long>; }

protected:

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        void BasicUIntDivOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialUIntDivOpV1(Value);
            if (IntHalf == 0 && DecimalHalf == 0)
				DecimalHalf = 1;//Prevent Dividing into nothing
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntDivOperationV1(const IntType& Value)
        {
            BasicUIntDivOpV1(Value); return *this;
        }
		
        template<IntegerType IntType=signed int>
        void BasicIntDivOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialIntDivOpV1(Value);
            if (IntHalf == 0 && DecimalHalf == 0)
				DecimalHalf = 1;//Prevent Dividing into nothing
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicIntDivOperationV1(const IntType& Value)
        {
            BasicIntDivOpV1(Value); return *this;
        }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicDivideByUIntV1(const IntType& lValue)
        {
            auto self = *this;
            return self.BasicUIntDivOpV1(rValue);
        }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicDivideByIntV1(const IntType& lValue)
        {
            auto self = *this;
            return self.BasicIntDivOpV1(rValue);
        }

public:

        void BasicUIntDivOp() { BasicUIntDivOpV1<unsigned int>; }
        void BasicIntDivOp() { BasicIntDivOpV1<signed int>; }
        void BasicUInt64DivOp() { BasicUIntDivOpV1<unsigned long long>; }
        void BasicInt64DivOp() { BasicIntDivOpV1<signed long long>; }

        void UnsignedBasicIntDivOp() { BasicUIntDivOpV1<signed int>; }
        void UnsignedBasicInt64DivOp() { BasicUIntDivOpV1<signed long long>; }

        void BasicUInt8DivOp() { BasicUIntDivOpV1<unsigned int>; }
        void BasicInt8DivOp() { BasicIntDivOpV1<signed int>; }
        void BasicUInt16DivOp() { BasicUIntDivOpV1<unsigned long long>; }
        void BasicInt16DivOp() { BasicIntDivOpV1<signed long long>; }

        auto& BasicUIntDivOperation() { return BasicUIntDivOperationV1<unsigned int>; }
        auto& BasicIntDivOperation() { return BasicIntDivOperationV1<signed int>; }
        auto& BasicUInt64DivOperation() { return BasicUIntDivOperationV1<unsigned long long>; }
        auto& BasicInt64DivOperation() { return BasicIntDivOperationV1<signed long long>; }
        auto& BasicUInt8DivOperation() { return BasicUIntDivOperationV1<unsigned char>; }
        auto& BasicInt8DivOperation() { return BasicIntDivOperationV1<signed char>; }
        auto& BasicUInt16DivOperation() { return BasicUIntDivOperationV1<unsigned short>; }
        auto& BasicInt16DivOperation() { return BasicIntDivOperationV1<signed short>; }

        auto BasicDivideByUInt() { return BasicDivideByUIntV1<unsigned int>; }
        auto BasicDivideByInt() { return BasicDivideByIntV1<signed int>; }
        auto BasicDivideByUInt64() { return BasicDivideByUIntV1<unsigned long long>; }
        auto BasicDivideByInt64() { return BasicDivideByIntV1<signed long long>; }

        auto UnsignedBasicDivideByInt() { return BasicDivideByUIntV1<signed int>; }
        auto UnsignedBasicDivideByInt64() { return BasicDivideByUIntV1<signed long long>; }

        auto BasicDivideByUInt8() { return BasicDivideByUIntV1<unsigned char>; }
        auto BasicDivideByInt8() { return BasicDivideByIntV1<signed char>; }
        auto BasicDivideByUInt16() { return BasicDivideByUIntV1<unsigned short>; }
        auto BasicDivideByInt16() { return BasicDivideByIntV1<signed short>; }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations
protected:

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool UnsignedPartialDivOp(const auto& lValue)
        {
            unsigned _int64 SelfRes = DecimalOverflowX * IntHalf.Value + (unsigned _int64)DecimalHalf;
            unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf.Value + (unsigned _int64)rValue.DecimalHalf;	

            unsigned _int64 IntHalfRes = SelfRes / ValueRes;
            unsigned _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
			IntHalf.Value = (unsigned int) IntHalfRes;
            DecimalHalf.Value = DecimalRes;
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }
		
public:

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void BasicUnsignedDivOp(const auto& lValue)
		{
			if(DecimalHalf==0)
			{
				if(rValue.DecimalHalf==0)
				{
					switch(rValue.IntHalf.rValue)
					{
						case 2:
							if(IntHalf&1==1)//Check if number is odd
								UnsignedBasicIntDivOp(2);
							else
								IntHalf.rValue /= 2;
							break;
						case 4:
							if(((IntHalf >> 2) << 2) == IntHalf)
								IntHalf.rValue /= 4;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 8:
							if(((IntHalf >> 3) << 3) == IntHalf)
								IntHalf.rValue /= 8;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 16:
							if(((IntHalf >> 4) << 4) == IntHalf)
								IntHalf.rValue /= 16;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 32:
							if(((IntHalf >> 5) << 5) == IntHalf)
								IntHalf.rValue /= 32;
							else
								UnsignedBasicIntDivOp(4);
							break;
                        case 0:
                            throw "Target rValue can not be divided by zero";
                            break;
						default:
							UnsignedBasicIntDivOp(rValue.IntHalf.rValue);
							break;
					}
				}
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
                else if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
				        DecimalHalf.Value = 1;
#else
                else
                    UnsignedPartialDivOp(rValue);
#endif
			}
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
            else if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf.Value = 1;
#else
            else
                UnsignedPartialDivOp(rValue);
#endif
		}

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& BasicUnsignedDivOperation(const auto& lValue)
		{ BasicUnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void BasicDivOp(const auto& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                BasicUnsignedDivOp(-Value);
            }
            else
                BasicUnsignedDivOp(Value);
        }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& BasicDivOperation(const auto& lValue)
		{ BasicDivOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto BasicDivideByUnsigned(const auto& lValue)
        { auto lValue = *this; return lValue.BasicUnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicDivideBy(const auto& lValue)
        { auto lValue = *this; return lValue.BasicDivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecBase&</returns>
        void DivideByTwo()
        {
            if(DecimalHalf==0&&IntHalf&1==1)//Check if number is odd
                rValue.UnsignedBasicIntDivOp(2);
            else
                IntHalf /= 2;
        }

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecBase&</returns>
        void DivideByFour()
        {
            //Checking if divisible by 4 based on
            //https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
            //checking if divible by 8 equals (((n >> 3) << 3) == n)
            if(DecimalHalf==0&&(((IntHalf >> 2) << 2) == IntHalf))//Check if number can be perfectly divided by 4
                IntHalf /= 4;
            else
                rValue.UnsignedBasicIntDivOp(4);
        }

public:

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/(const MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicDivideBy(rValue); }
		
        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicDivOperation(rValue); }
		
        /// <summary>
        /// Division operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/(const MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicDivideByInt(rValue); }
        friend MediumDecBase operator/(const MediumDecBase& lValue, const signed long long& rValue) { return lValue.BasicDivideByInt64(rValue); }
        friend MediumDecBase operator/(const MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicDivideByUInt(rValue); }
        friend MediumDecBase operator/(const MediumDecBase& lValue, const unsigned long long& rValue) { return lValue.BasicDivideByUInt64(rValue); }
		
        friend MediumDecBase operator/(const signed int& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const signed long long& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const unsigned int& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const unsigned long long& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicDivideBy(rValue); }

        friend MediumDecBase operator/(const MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicDivideByInt8(rValue); }
        friend MediumDecBase operator/(const MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicDivideByInt16(rValue); }
        friend MediumDecBase operator/(const MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicDivideByUInt8(rValue); }
        friend MediumDecBase operator/(const MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicDivideByUInt16(rValue); }

        friend MediumDecBase operator/(const signed char& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const signed short& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const unsigned char& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const unsigned short& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicDivideBy(rValue); }


        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicIntDivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const signed long long& rValue) { return lValue.BasicInt64DivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicUIntDivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64DivOperation(rValue); }

        friend MediumDecBase& operator/=(MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicInt8DivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicInt16DivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicUInt8DivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicUInt16DivOperation(rValue); }

	#pragma endregion Other Division Operations	

    #pragma region NormalRep Integer Multiplication Operations
protected:
		/// <summary>
        /// Partial version of BasicUIntMultOpV1 without zero checks
        /// (Modifies owner object) 
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=int>
        void PartialUIntMultOpV1(const IntType& lValue)
        {
            if (DecimalHalf == 0)
                IntHalf.Value *= rValue;
            else
			{
                __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntHalf.Value = (unsigned int)OverflowVal;
                    DecimalHalf.Value = (unsigned int)SRep;
                }
                else
                {
					IntHalf.Value = 0;
                    DecimalHalf.Value = (unsigned int)SRep;
                }
            }
        }
		
		//Partial version of BasicIntMultOpV1 without zero checks
		//Modifies owner object
        template<IntegerType IntType=signed int>
        void PartialIntMultOpV1(const IntType& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                PartialUIntMultOp(-Value);
            }
            else
                PartialUIntMultOp(Value);
        }
public:
		
        void PartialUIntMultOp() { PartialUIntMultOpV1<unsigned int>; }
        void PartialIntMultOp() { PartialIntMultOpV1<signed int>; }
        void PartialUInt64MultOp() { PartialUIntMultOpV1<unsigned long long>; }
        void PartialInt64MultOp() { PartialIntMultOpV1<signed long long>; }

        void UnsignedPartialIntMultOp() { PartialUIntMultOpV1<signed int>; }
        void UnsignedPartialInt64MultOp() { PartialUIntMultOpV1<signed long long>; }
		
protected:
        template<IntegerType IntType=signed int>
        void BasicUIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return;
            }
            else if (IsZero())
                return;
            PartialUIntMultOpV1(Value);
        }
		
        template<IntegerType IntType=unsigned int>
        auto& BasicUIntMultOperationV1(const IntType& Value)
        {
            BasicUIntMultOpV1(Value); return *this;
        }

        template<IntegerType IntType=signed int>
        void BasicIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return;
            }
            else if (IsZero())
                return;
			PartialIntMultOpV1(Value);
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicIntMultOperationV1(const IntType& Value)
        {
            BasicIntMultOpV1(Value); return *this;
        }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicMultiplyByUIntV1(const IntType& lValue)
        {
            auto self = *this;
            return self.BasicUIntMultOpV1(rValue);
        }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicMultiplyByIntV1(const IntType& lValue)
        {
            auto self = *this;
            return self.BasicIntMultOpV1(rValue);
        }

public:

        void BasicUIntMultOp() { BasicUIntMultOpV1<unsigned int>; }
        void BasicIntMultOp() { BasicIntMultOpV1<signed int>; }
        void BasicUInt64MultOp() { BasicUIntMultOpV1<unsigned long long>; }
        void BasicInt64MultOp() { BasicIntMultOpV1<signed long long>; }

        void UnsignedBasicIntMultOp() { BasicUIntMultOpV1<signed int>; }
        void UnsignedBasicInt64MultOp() { BasicUIntMultOpV1<signed long long>; }

        void BasicUInt8MultOp() { BasicUIntMultOpV1<unsigned char>; }
        void BasicInt8MultOp() { BasicIntMultOpV1<signed char>; }
        void BasicUInt16MultOp() { BasicUIntMultOpV1<unsigned short>; }
        void BasicInt16MultOp() { BasicIntMultOpV1<signed short>; }

        MediumDecBase& BasicUIntMultOperation() { BasicUIntMultOperationV1<unsigned int>; }
        MediumDecBase& BasicIntMultOperation() { BasicIntMultOperationV1<signed int>; }
        MediumDecBase& BasicUInt64MultOperation() { BasicUIntMultOperationV1<unsigned long long>; }
        MediumDecBase& BasicInt64MultOperation() { BasicIntMultOperationV1<signed long long>; }
        MediumDecBase& BasicUInt8MultOperation() { BasicUIntMultOperationV1<unsigned char>; }
        MediumDecBase& BasicInt8MultOperation() { BasicIntMultOperationV1<signed char>; }
        MediumDecBase& BasicUInt16MultOperation() { BasicUIntMultOperationV1<unsigned short>; }
        MediumDecBase& BasicInt16MultOperation() { BasicIntMultOperationV1<signed short>; }

        auto BasicMultiplyByUInt() { return BasicMultiplyByUIntV1<unsigned int>; }
        auto BasicMultiplyByInt() { return BasicMultiplyByIntV1<signed int>; }
        auto BasicMultiplyByUInt64() { return BasicMultiplyByUIntV1<unsigned long long>; }
        auto BasicMultiplyByInt64() { return BasicMultiplyByIntV1<signed long long>; }

        auto UnsignedBasicMultiplyByInt() { return BasicMultiplyByUIntV1<signed int>; }
        auto UnsignedBasicMultiplyByInt64() { return BasicMultiplyByUIntV1<signed long long>; }

        auto BasicMultiplyByUInt8() { return BasicMultiplyByUIntV1<unsigned char>; }
        auto BasicMultiplyByInt8() { return BasicMultiplyByIntV1<signed char>; }
        auto BasicMultiplyByUInt16() { return BasicMultiplyByUIntV1<unsigned short>; }
        auto BasicMultiplyByInt16() { return BasicMultiplyByIntV1<signed short>; }		

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations
	
		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& BasicUnsignedMultOp(const auto& lValue)
		{
            if (DecimalHalf == 0)
            {
                if (IntHalf.Value == 1)
                {
					if(IntHalf.IsNegative()
						IntHalf = -rValue.IntHalf;
					else
						IntHalf = rValue.IntHalf;
					DecimalHalf.Value = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                    IntHalf *= rValue.IntHalf;
                else {
                    __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntHalf.Value + rValue.DecimalHalf;
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)rRep;
                        return;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                    #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf.Value = 1;
                    #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
							SetAsZero(); return; }
                    #endif
                        IntHalf.Value = 0;
                        return;
                    }
                }
            }
            else if (IntHalf.Value == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf.Value;
                SRep /= DecimalOverflowX;
                if (rValue.IntHalf == 0)
                {
                    DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf.Value = 1;
				#elif !defined(AltNum_AllowNegativeZero)
                    if(DecimalHalf==0){
						SetAsZero(); return; }
                #endif
                    return;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntHalf;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = OverflowVal;
                        DecimalHalf.Value = (signed int)SRep;
						return;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf.Value = 1;
				#elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
							SetAsZero(); return; }
                #endif
                        return;
                    }
                }
            }
            else
            {
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.IntHalf;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)SRep;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf.Value = 1;
				#elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
							SetAsZero(); return; }
                #endif
                        }
                        IntHalf.Value = 0;
                    }
				    return;
                }
                else if (rValue.IntHalf == 0)
                {
                    __int64 SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.DecimalHalf;
                    SRep /= DecimalOverflowX;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)SRep;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf.Value = 1;
				#elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
							SetAsZero(); return; }
                #endif
                        }
                        IntHalf.Value = 0;
                    }
                    return;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntHalf == 0 ? DecimalHalf : DecimalOverflowX * IntHalf + DecimalHalf;
                    SRep *= rValue.IntHalf;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf.Value);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 intHalf = IntegerRep / DecimalOverflow;
                    IntegerRep -= intHalf * (__int64)DecimalOverflow;
                    IntHalf.Value = (unsigned int) intHalf;
                    DecimalHalf.Value = (unsigned int)IntegerRep;
                }
            }
			#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntHalf==0)
                DecimalHalf.Value = 1;
			#elif !defined(AltNum_AllowNegativeZero)
            if(DecimalHalf==0)
				SetAsZero();
			#endif
		}

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& BasicUnsignedMultOperation(const auto& lValue)
        { BasicUnsignedMultOp(rValue); return *this; }

        void BasicMultOp(const auto& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                BasicUnsignedMultOp(-Value);
            }
            else
                BasicUnsignedMultOp(Value);
        }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto& BasicMultOperation(const auto& lValue)
		{ BasicMultOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto BasicMultiplyByUnsigned(const auto& lValue)
        { auto lValue = *this; return lValue.BasicUnsignedMultOperation(rValue); }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicMultiplyBy(const auto& lValue)
        { auto lValue = *this; return lValue.BasicMultOperation(rValue); }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region Other multiplication operations
		
        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>void</returns>
        void MultipleByTwo()
        {
	        UnsignedBasicIntMultOp(2);
        }

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>void</returns>
        void MultipleByFour()
        {
	        UnsignedBasicIntMultOp(4);
        }

public:

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator*(const MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicMultipleBy(rValue); }

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicMultOperation(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator*(const MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicMultiplyByInt(rValue); }
        friend MediumDecBase operator*(const MediumDecBase& lValue, const signed long long& rValue) { return lValue.BasicMultiplyByInt64(rValue); }
        friend MediumDecBase operator*(const MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicMultiplyByUInt(rValue); }
        friend MediumDecBase operator*(const MediumDecBase& lValue, const unsigned long long& rValue) { return lValue.BasicMultiplyByUInt64(rValue); }
		
        friend MediumDecBase operator*(const signed int& lValue, const MediumDecBase& rValue) { return rValue.BasicMultiplyByInt(lrValue); }
        friend MediumDecBase operator*(const unsigned long long& lValue, const MediumDecBase& rValue) { return rValue.BasicMultiplyByInt64(lrValue); }
        friend MediumDecBase operator*(const unsigned int& lValue, const MediumDecBase& rValue) { return rValue.BasicMultiplyByUInt(lrValue); }
        friend MediumDecBase operator*(const unsigned long long& lValue, const MediumDecBase& rValue) { return rValue.BasicMultiplyByUInt64(lrValue); }

        friend MediumDecBase operator*(const MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicMultiplyByInt8(rValue); }
        friend MediumDecBase operator*(const MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicMultiplyByInt16(rValue); }
        friend MediumDecBase operator*(const MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicMultiplyByUInt8(rValue); }
        friend MediumDecBase operator*(const MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicMultiplyByUInt16(rValue); }

        friend MediumDecBase operator*(const signed char& lValue, const MediumDecBase& rValue) { return rValue.BasicMultiplyByInt8(lrValue); }
        friend MediumDecBase operator*(const signed short& lValue, const MediumDecBase& rValue) { return rValue.BasicMultiplyByInt16(lrValue); }
        friend MediumDecBase operator*(const unsigned char& lValue, const MediumDecBase& rValue) { return rValue.BasicMultiplyByUInt8(lrValue); }
        friend MediumDecBase operator*(const unsigned short& lValue, const MediumDecBase& rValue) { return rValue.BasicMultiplyByUInt16(lrValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicIntMultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const signed long long& rValue) { return lValue.BasicInt64MultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicUIntMultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64MultOperation(rValue); }

        friend MediumDecBase& operator*=(MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicInt8MultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicInt16MultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicUInt8MultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicUInt16MultOperation(rValue); }

	#pragma endregion Other multiplication operations

	#pragma region NormalRep Integer Addition Operations
protected:
		
        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void BasicUIntAddOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				NRepSkippingUnsignedAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntAddOp((unsigned int)rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf = PartialInt::DecimalOverflow - DecimalHalf;
			}
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntAddOperationV1(const IntType& rValue)
        { BasicUIntAddOpV1(Value); return *this; }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void BasicIntAddOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue;
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
			}
        }

        template<IntegerType IntType=signed int>
        auto& BasicIntAddOperationV1(const IntType& rValue)
        { BasicIntAddOpV1(rValue); return *this; }

		/// <summary>
        /// Basic addition operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto BasicAddByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicAddByUIntV1(rValue); }

		/// <summary>
        /// Basic addition operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto BasicAddByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicAddByIntV1(rValue); }

public:

        void BasicUIntAddOp(const unsigned int& rValue)
        {
			if(DecimalHalf.Value==0)
				NRepSkippingUnsignedAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntAddOp(rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf = PartialInt::DecimalOverflow - DecimalHalf;
			}
        }

        void BasicIntAddOp() { BasicIntAddOpV1<signed int>; }
        void BasicUInt64AddOp() { BasicUIntAddOpV1<unsigned long long>; }
        void BasicInt64AddOp() { BasicIntAddOpV1<signed long long>; }

        void UnsignedBasicIntAddOp() { BasicUIntAddOpV1<signed int>; }
        void UnsignedBasicInt64AddOp() { BasicUIntAddOpV1<signed long long>; }

        void BasicUInt8AddOp() { BasicUIntAddOpV1<unsigned char>; }
        void BasicInt8AddOp() { BasicIntAddOpV1<signed char>; }
        void BasicUInt16AddOp() { BasicUIntAddOpV1<unsigned short>; }
        void BasicInt16AddOp() { BasicIntAddOpV1<signed short>; }

        auto& BasicUIntAddOperation(const unsigned int& rValue)
        { BasicUIntAddOpV1(Value); return *this; }

        auto& BasicIntAddOperation() { return BasicIntAddOperationV1<signed int>; }
        auto& BasicUInt64AddOperation() { return BasicUIntAddOperationV1<unsigned long long>; }
        auto& BasicInt64AddOperation() { return BasicIntAddOperationV1<signed long long>; }
        auto& BasicUInt8AddOperation() { return BasicUIntAddOperationV1<unsigned char>; }
        auto& BasicInt8AddOperation() { return BasicIntAddOperationV1<signed char>; }
        auto& BasicUInt16AddOperation() { return BasicUIntAddOperationV1<unsigned short>; }
        auto& BasicInt16AddOperation() { return BasicIntAddOperationV1<signed short>; }

        auto BasicAddByUInt() { return BasicAddByUIntV1<unsigned int>; }
        auto BasicAddByInt() { return BasicAddByIntV1<signed int>; }
        auto BasicAddByUInt64() { return BasicAddByUIntV1<unsigned long long>; }
        auto BasicAddByInt64() { return BasicAddByIntV1<signed long long>; }

        auto UnsignedBasicAddByInt() { return BasicAddByUIntV1<signed int>; }
        auto UnsignedBasicAddByInt64() { return BasicAddByUIntV1<signed long long>; }

        auto BasicAddByUInt8() { return BasicAddByUIntV1<unsigned char>; }
        auto BasicAddByInt8() { return BasicAddByIntV1<signed char>; }
        auto BasicAddByUInt16() { return BasicAddByUIntV1<unsigned short>; }
        auto BasicAddByInt16() { return BasicAddByIntV1<signed short>; }
    	
	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:
		
        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void BasicUIntSubOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				NRepSkippingUnsignedSubOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntSubOp((unsigned int)rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf = PartialInt::DecimalOverflow - DecimalHalf;
			}
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntSubOperationV1(const IntType& rValue)
        { BasicUIntSubOpV1(Value); return *this; }

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void BasicIntSubOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingSubOp(rValue);
			else {
				unsigned int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue;
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf = MediumDec::DecimalOverflow - DecimalHalf;
			}
        }

        template<IntegerType IntType=signed int>
        auto& BasicIntSubOperationV1(const IntType& rValue)
        { BasicIntSubOpV1(rValue); return *this; }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto BasicSubByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicSubByUIntV1(rValue); }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto BasicSubByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicSubByIntV1(rValue); }

public:

        void BasicUIntSubOp(const unsigned int& rValue)
        {
			if(DecimalHalf.Value==0)
				NRepSkippingUnsignedSubOp(rValue);
			else {
				unsigned int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntSubOp(rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf = PartialInt::DecimalOverflow - DecimalHalf;
			}
        }

        void BasicIntSubOp() { BasicIntSubOpV1<signed int>; }
        void BasicUInt64SubOp() { BasicUIntSubOpV1<unsigned long long>; }
        void BasicInt64SubOp() { BasicIntSubOpV1<signed long long>; }

        void UnsignedBasicIntSubOp() { BasicUIntSubOpV1<signed int>; }
        void UnsignedBasicInt64SubOp() { BasicUIntSubOpV1<signed long long>; }

        void BasicUInt8SubOp() { BasicUIntSubOpV1<unsigned char>; }
        void BasicInt8SubOp() { BasicIntSubOpV1<signed char>; }
        void BasicUInt16SubOp() { BasicUIntSubOpV1<unsigned short>; }
        void BasicInt16SubOp() { BasicIntSubOpV1<signed short>; }

        auto& BasicUIntSubOperation(const unsigned int& rValue)
        { BasicUIntSubOpV1(Value); return *this; }

        auto& BasicIntSubOperation() { return BasicIntSubOperationV1<signed int>; }
        auto& BasicUInt64SubOperation() { return BasicUIntSubOperationV1<unsigned long long>; }
        auto& BasicInt64SubOperation() { return BasicIntSubOperationV1<signed long long>; }
        auto& BasicUInt8SubOperation() { return BasicUIntSubOperationV1<unsigned char>; }
        auto& BasicInt8SubOperation() { return BasicIntSubOperationV1<signed char>; }
        auto& BasicUInt16SubOperation() { return BasicUIntSubOperationV1<unsigned short>; }
        auto& BasicInt16SubOperation() { return BasicIntSubOperationV1<signed short>; }

        auto BasicSubByUInt() { return BasicSubByUIntV1<unsigned int>; }
        auto BasicSubByInt() { return BasicSubByIntV1<signed int>; }
        auto BasicSubByUInt64() { return BasicSubByUIntV1<unsigned long long>; }
        auto BasicSubByInt64() { return BasicSubByIntV1<signed long long>; }

        auto UnsignedBasicSubByInt() { return BasicSubByUIntV1<signed int>; }
        auto UnsignedBasicSubByInt64() { return BasicSubByUIntV1<signed long long>; }

        auto BasicSubByUInt8() { return BasicSubByUIntV1<unsigned char>; }
        auto BasicSubByInt8() { return BasicSubByIntV1<signed char>; }
        auto BasicSubByUInt16() { return BasicSubByUIntV1<unsigned short>; }
        auto BasicSubByInt16() { return BasicSubByIntV1<signed short>; }
    	
	#pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep AltNum Addition/Subtraction Operations
	
        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedAddOp(const auto& rValue)
        {
			if(rValue.DecimalHalf==0)
				BasicUIntAddOpV1(rValue.IntHalf);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UnsignedAddOp(rValue.IntValue);
                if (signBeforeOp==MirroredInt::NegativeSign){
					if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -4.5
						if(IntValue.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
						++IntHalf;
						if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
							DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
						} else//-1.6 + .7 = -0.9
							DecimalHalf.Value = PartialInt::DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
					} else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
						DecimalHalf.Value = PartialInt::DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10 
					else
						DecimalHalf.Value -= rValue.DecimalHalf.Value;
                } else {
					unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf;
					if(decResult==PartialInt::DecimalOverflow){//5.4 + 4.6
						++IntHalf;
						if(IntValue.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					} else if(decResult>PartialInt::DecimalOverflow){//5.4 + 4.7
						++IntHalf;
						if(signBeforeOp!=IntHalf.Sign)
							DecimalHalf.Value = PartialInt::DecimalOverflow - (decResult - PartialInt::DecimalOverflow);
						else
							DecimalHalf.Value = decResult - PartialInt::DecimalOverflow;
					}
					else if(signBeforeOp!=IntHalf.Sign)
						DecimalHalf.Value = PartialInt::DecimalOverflow - decResult;
					else
						DecimalHalf.Value = decResult;
                }
			}
		}
	
	    /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicAddOp(const auto& rValue)
        {
			if(rValue.DecimalHalf==0)
				BasicIntAddOpV1(rValue.IntHalf);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue.IntValue;
				
                if (signBeforeOp==MirroredInt::NegativeSign){
					if(rValue.IsPositive()){
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){
							if(IntValue.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							++IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
								DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
							} else//-1.6 + .7 = -0.9
								DecimalHalf.Value = PartialInt::DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
						} else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
							DecimalHalf.Value = PartialInt::DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10 
						else
							DecimalHalf.Value -= rValue.DecimalHalf.Value;
					} else {
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf;
						if(decResult==PartialInt::DecimalOverflow){//-5.4 + - 5.6
							--IntHalf;
							if(IntValue.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						else if(decResult>PartialInt::DecimalOverflow){//-5.4 - 5.7 = -11.1 
							--IntHalf;
							DecimalHalf.Value = decResult - PartialInt::DecimalOverflow;
						} else//-5.2 - 5.2 = -10.4
							DecimalHalf.Value = decResult;
					}
                } else {
					if(rValue.IsPositive()){
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
						if(decResult==PartialInt::DecimalOverflow){//5.5 + 4.5 = 10
							++IntHalf;
							if(IntValue.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(decResult>PartialInt::DecimalOverflow){//5.5 + 4.6 = 
							++IntHalf;
							DecimalHalf.Value = decResult - PartialInt::DecimalOverflow; 
						} else//5.4 + 5.3 = 10.7
							DecimalHalf.Value = decResult;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -5.5
							if(IntValue.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							--IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//4.3 - 5.4 = -1.1
								DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
							else//4.3 - 2.4 = 1.9
								DecimalHalf.Value = PartialInt::DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
						} else if(signBeforeOp!=IntHalf.Sign)//5.4 + - 6.3 = -0.9
							DecimalHalf.Value = PartialInt::DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 + 3 - 4 
						else//4.4 + -2.3 = 2.1
							DecimalHalf.Value -= rValue.DecimalHalf;
					}
                }

				//If flips to other side of negative, invert the decimals
				if(signBeforeOp!=IntHalf.Sign)
					DecimalHalf = DecimalOverflow - DecimalHalf;
			}
		}
	
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedSubOp(const auto& rValue)
        {
			if(rValue.DecimalHalf==0)
				BasicUIntSubOpV1(rValue.IntHalf);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UnsignedSubOp(rValue.IntValue);
				
                if (signBeforeOp==MirroredInt::NegativeSign){//-5 - B 
					unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf;
					if(decResult==PartialInt::DecimalOverflow){//-5.4 - 5.6
						--IntHalf;
						if(IntValue.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					else if(decResult>PartialInt::DecimalOverflow){//-5.4 - 5.7 = -11.1 
						--IntHalf;
						DecimalHalf.Value = decResult - PartialInt::DecimalOverflow;
					} else//-5.2 - 5.2 = -10.4
						DecimalHalf.Value = decResult;
                } else {//5.XX - B
					if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - 5.5 = 10
						if(IntValue.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
						--IntHalf;
						if(signBeforeOp!=IntHalf.Sign)//5.4 - 5.7 = -0.3
							DecimalHalf.Value = rValue.DecimalHalf - DecimalHalf.Value;
						else//5.4 - 3.6 = 1.8
							DecimalHalf.Value = PartialInt::DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
					} else if(signBeforeOp!=IntHalf.Sign)//5.3 - 7.2 = -1.9
						DecimalHalf = PartialInt::DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
					else//5.4 - 5.3 = 0.1 
						DecimalHalf.Value -= rValue.DecimalHalf;
                }
			}
		}
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicSubOp(const auto& rValue)
        {
			if(rValue.DecimalHalf==0)
				BasicIntSubOpV1(rValue.IntHalf);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf -= rValue.IntValue;
                if (signBeforeOp==MirroredInt::NegativeSign){
					if(rValue.IsPositive()){
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf;
						if(decResult==PartialInt::DecimalOverflow){//-5.4 - 5.6
							--IntHalf;
							if(IntValue.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						else if(decResult>PartialInt::DecimalOverflow){//-5.4 - 5.7 = -11.1 
							--IntHalf;
							DecimalHalf.Value = decResult - PartialInt::DecimalOverflow;
						} else//-5.2 - 5.2 = -10.4
							DecimalHalf.Value = decResult;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//-5.4 - -4.4
							if(IntValue.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							++IntValue;
							if(signBeforeOp!=IntHalf.Sign)//-5.4 - -6.5 = 1.1
								DecimalHalf = PartialInt::DecimalOverflow - DecimalHalf.Value - rValue.DecimalHalf;
							else//-5.4 - -4.5 = -0.9 == -5.4 + 4.5
								DecimalHalf.Value += rValue.DecimalHalf.Value;
						} else if(signBeforeOp!=IntHalf.Sign)//-5.4 - -7.3 = 1.9
							DecimalHalf.Value = PartialInt::DecimalOverflow + rValue.DecimalHalf - DecimalHalf.Value;
						else//-5.4 - -3.3 = -2.1
							DecimalHalf.Value -= rValue.DecimalHalf.Value;
					}
                } else {
					if(rValue.IsPositive()){
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - 5.5 = 10
							if(IntValue.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							--IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//5.4 - 5.7 = -0.3
								DecimalHalf.Value = rValue.DecimalHalf - DecimalHalf.Value;
							else//5.4 - 3.6 = 1.8
								DecimalHalf.Value = PartialInt::DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
						} else if(signBeforeOp!=IntHalf.Sign)//5.3 - 7.2 = -1.9
							DecimalHalf = PartialInt::DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
						else//5.4 - 5.3 = 0.1 
							DecimalHalf.Value -= rValue.DecimalHalf;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - -5.5 = 11
							++IntHalf;
							if(IntValue.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){//5.4 - -5.7 = 11.1
							++IntHalf;
							DecimalHalf.Value = DecimalHalf.Value + rValue.DecimalHalf - PartialInt::DecimalOverflow;
						} else//5.4 - -5.3 = 10.7 
							DecimalHalf.Value += rValue.DecimalHalf;
					}
                }
			}
		}
		
        //Basic addition operation
        auto& BasicUnsignedAddOperation(const auto& rValue)
        { BasicUnsignedAddOp(rValue); return *this; }
		
        //Basic addition operation
        auto& BasicAddOperation(const auto& rValue)
        { BasicAddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto BasicAddByUnsigned(const auto& lValue)
        { auto lValue = *this; return lValue.BasicUnsignedAddOperation(rValue); }

		/// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicAddBy(const auto& lValue)
        { auto lValue = *this; return lValue.BasicAddOperation(rValue); }

        //Basic subtraction operation
        auto& BasicUnsignedSubOperation(const auto& rValue)
        { BasicUnsignedSubOp(rValue); return *this; }

        //Basic subtraction operation
        auto& BasicSubOperation(const auto& rValue)
        { BasicSubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto BasicSubtractByUnsigned(const auto& lValue)
        { auto lValue = *this; return lValue.BasicUnsignedSubOperation(rValue); }

		/// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicSubtractBy(const auto& lValue)
        { auto lValue = *this; return lValue.BasicSubOperation(rValue); }

	#pragma endregion NormalRep AltNum Addition/Subtraction Operations

	#pragma region Other addition operations

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator+(const MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicAddBy(rValue); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicAddOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator+(const MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicAddByInt(rValue); }
        friend MediumDecBase operator+(const MediumDecBase& lValue, const signed long long& rValue) { return lValue.BasicAddByInt64(rValue); }
        friend MediumDecBase operator+(const MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicAddByUInt(rValue); }
        friend MediumDecBase operator+(const MediumDecBase& lValue, const unsigned long long& rValue) { return lValue.BasicAddByUInt64(rValue); }
		
        friend MediumDecBase operator+(const signed int& lValue, const MediumDecBase& rValue) { return rValue.BasicAddByInt(lrValue); }
        friend MediumDecBase operator+(const unsigned long long& lValue, const MediumDecBase& rValue) { return rValue.BasicAddByInt64(lrValue); }
        friend MediumDecBase operator+(const unsigned int& lValue, const MediumDecBase& rValue) { return rValue.BasicAddByUInt(lrValue); }
        friend MediumDecBase operator+(const unsigned long long& lValue, const MediumDecBase& rValue) { return rValue.BasicAddByUInt64(lrValue); }

        friend MediumDecBase operator+(const MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicAddByInt8(rValue); }
        friend MediumDecBase operator+(const MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicAddByInt16(rValue); }
        friend MediumDecBase operator+(const MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicAddByUInt8(rValue); }
        friend MediumDecBase operator+(const MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicAddByUInt16(rValue); }

        friend MediumDecBase operator+(const signed char& lValue, const MediumDecBase& rValue) { return rValue.BasicAddByInt8(lrValue); }
        friend MediumDecBase operator+(const signed short& lValue, const MediumDecBase& rValue) { return rValue.BasicAddByInt16(lrValue); }
        friend MediumDecBase operator+(const unsigned char& lValue, const MediumDecBase& rValue) { return rValue.BasicAddByUInt8(lrValue); }
        friend MediumDecBase operator+(const unsigned short& lValue, const MediumDecBase& rValue) { return rValue.BasicAddByUInt16(lrValue); }

        /// <summary>
        /// += operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicIntAddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const signed long long& rValue) { return lValue.BasicInt64AddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicUIntAddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64AddOperation(rValue); }

        friend MediumDecBase& operator+=(MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicInt8AddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicInt16AddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicUInt8AddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicUInt16AddOperation(rValue); }

	#pragma endregion Other addition operations

	#pragma region Other subtraction operations

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator-(const MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicSubtractBy(rValue); }
		
        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicSubOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator-(const MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicSubtractByInt(rValue); }
        friend MediumDecBase operator-(const MediumDecBase& lValue, const signed long long& rValue) { return lValue.BasicSubtractByInt64(rValue); }
        friend MediumDecBase operator-(const MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicSubtractByUInt(rValue); }
        friend MediumDecBase operator-(const MediumDecBase& lValue, const unsigned long long& rValue) { return lValue.BasicSubtractByUInt64(rValue); }
		
        friend MediumDecBase operator-(const signed int& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const signed long long& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const unsigned int& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const unsigned long long& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicSubtractBy(rValue); }

        friend MediumDecBase operator-(const MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicSubtractByInt8(rValue); }
        friend MediumDecBase operator-(const MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicSubtractByInt16(rValue); }
        friend MediumDecBase operator-(const MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicSubtractByUInt8(rValue); }
        friend MediumDecBase operator-(const MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicSubtractByUInt16(rValue); }

        friend MediumDecBase operator-(const signed char& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const signed short& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const unsigned char& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const unsigned short& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)rValue).BasicSubtractBy(rValue); }

        /// <summary>
        /// -= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicIntSubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const signed long long& rValue) { return lValue.BasicInt64SubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicUIntSubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64SubOperation(rValue); }

        friend MediumDecBase& operator-=(MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicInt8SubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicInt16SubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicUInt8SubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicUInt16SubOperation(rValue); }

	#pragma endregion Other subtraction operations

    #pragma region Floating Operator Overrides
    
        friend MediumDecBase operator+(const MediumDecBase& self, const float& Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(const MediumDecBase& self, const float& Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(const MediumDecBase& self, const float& Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(const MediumDecBase& self, const float& Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(const float& Value, const MediumDecBase& self) { return (MediumDecBase)Value + self; }
        friend MediumDecBase operator-(const float& Value, const MediumDecBase& self) { return (MediumDecBase)Value - self; }
        friend MediumDecBase operator*(const float& Value, const MediumDecBase& self) { return (MediumDecBase)Value * self; }
        friend MediumDecBase operator/(const float& Value, const MediumDecBase& self) { return (MediumDecBase)Value / self; }

        friend MediumDecBase operator+(const MediumDecBase& self, const double& Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(const MediumDecBase& self, const double& Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(const MediumDecBase& self, const double& Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(const MediumDecBase& self, const double& Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(const MediumDecBase& self, const long double& Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(const MediumDecBase& self, const long double& Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(const MediumDecBase& self, const long double& Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(const MediumDecBase& self, const long double& Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(const long double& Value, const MediumDecBase& self) { return (MediumDecBase)Value + self; }
        friend MediumDecBase operator-(const long double& Value, const MediumDecBase& self) { return (MediumDecBase)Value - self; }
        friend MediumDecBase operator*(const long double& Value, const MediumDecBase& self) { return (MediumDecBase)Value * self; }
        friend MediumDecBase operator/(const long double& Value, const MediumDecBase& self) { return (MediumDecBase)Value / self; }

    #pragma endregion Floating Operator Overrides

    #pragma region Other Operators

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator -()
        {
			auto self = this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// ++MediumDecBase Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator ++()
        {
            if (DecimalHalf == 0)
                ++IntHalf;
            else if (IntHalf == NegativeRep)
                IntHalf = MirroredInt::Zero;
            else
                ++IntHalf;
            return *this;
        }

        /// <summary>
        /// ++MediumDecBase Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator --()
        {
            if (DecimalHalf == 0)
                --IntHalf;
            else if (IntHalf == 0)
                IntHalf = NegativeRep;
            else
                --IntHalf;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator ++(int)
        {
            MediumDecBase tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator --(int)
        {
            MediumDecBase tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

// Static versions of functions for Full versions
	#pragma region Math Etc Functions

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        auto& AbsOf()
        {
            if (IntHalf.IsNegative())
                IntHalf.IsPositive = 1;
            return *this;
        }

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase</returns>
        static auto Abs(const auto tValue&)
        {
            auto self = tValue;
            return self.Abs();
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        auto& FloorOf()
        {
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase</returns>
        static auto Floor(const auto tValue&, const int precision&)
        {
            auto self = tValue;
            switch (precision)
            {
            case 9: break;
            case 8: self.DecimalHalf /= 10; Value.DecimalHalf *= 10; break;
            case 7: self.DecimalHalf /= 100; Value.DecimalHalf *= 100; break;
            case 6: self.DecimalHalf /= 1000; Value.DecimalHalf *= 1000; break;
            case 5: self.DecimalHalf /= 10000; Value.DecimalHalf *= 10000; break;
            case 4: self.DecimalHalf /= 100000; Value.DecimalHalf *= 100000; break;
            case 3: self.DecimalHalf /= 1000000; Value.DecimalHalf *= 1000000; break;
            case 2: self.DecimalHalf /= 10000000; Value.DecimalHalf *= 10000000; break;
            case 1: self.DecimalHalf /= 100000000; Value.DecimalHalf *= 100000000; break;
            default: self.DecimalHalf = 0; break;
            }
            if (self.IntHalf == NegativeRep && Value.DecimalHalf == 0) { self.IntHalf = 0; }
            return self;
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        auto& CeilOf()
        {
            if (DecimalHalf != 0)
            {
                DecimalHalf = 0;
                if (IntHalf == NegativeRep) { IntHalf = 0; }
                else
                {
                    ++IntHalf;
                }
            }
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        static int FloorInt(const auto& tValue)
        {
            if (tValue.DecimalHalf == 0)
            {
                return tValue.IntHalf.Value;
            }
            if (tValue.IntHalf == NegativeRep) { return -1; }
            else
            {
                return tValue.IntHalf.Value - 1;
            }
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        static int CeilInt(const auto& tValue)
        {
            if (tValue.DecimalHalf == 0)
            {
                return self.IntHalf.Value;
            }
            if (tValue.IntHalf == NegativeRep) { return 0; }
            else
            {
                return tValue.IntHalf.Value + 1;
            }
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase</returns>
        static auto Ceil(const auto& tValue)
        {
            auto self = tValue;
            return self.Ceil();
        }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        auto& Trunc()
        {
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        static auto Trunc(const auto& Value)
        {
            auto self = tValue;
            return self.Trunc();
        }

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        auto SqrtOf(const int& precision=7)
        { 
            if(IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
            else if (DecimalHalf == 0)
            {
                auto value = *this;
                bool AutoSetValue = true;
                switch (IntHalf.Value)
                {
                case 1: value.IntHalf = 1; break;
                case 4: value.IntHalf = 2; break;
                case 9: value.IntHalf = 3; break;
                case 16: value.IntHalf = 4; break;
                case 25: value.IntHalf = 5; break;
                case 36: value.IntHalf = 6; break;
                case 49: value.IntHalf = 7; break;
                case 64: value.IntHalf = 8; break;
                case 81: value.IntHalf = 9; break;
                case 100: value.IntHalf = 10; break;
                case 121: value.IntHalf = 11; break;
                case 144: value.IntHalf = 12; break;
                case 169: value.IntHalf = 13; break;
                case 196: value.IntHalf = 14; break;
                case 225: value.IntHalf = 15; break;
                case 256: value.IntHalf = 16; break;
                case 289: value.IntHalf = 17; break;
                case 324: value.IntHalf = 18; break;
                case 361: value.IntHalf = 19; break;
                case 400: value.IntHalf = 20; break;
                case 1600: value.IntHalf = 40; break;
                default:
                    AutoSetValue = false;
                    break;
                }
                if(AutoSetValue)
                    return value;//Technically both positive and negative numbers of same equal the result
            }

            auto number = this;
            auto start = Zero, end = number;
            auto mid;

            // variable to store the answer 
            auto ans;

            // for computing integral part 
            // of square root of number 
            while (start <= end) {
                mid = (start + end) / 2;
                if (mid * mid == number) {
                    ans = mid;
                    break;
                }

                // incrementing start if integral 
                // part lies on right side of the mid 
                if (mid * mid < number) {
                    start = mid + 1;
                    ans = mid;
                }

                // decrementing end if integral part 
                // lies on the left side of the mid 
                else {
                    end = mid - 1;
                }
            }

            // For computing the fractional part 
            // of square root up to given precision 
            auto increment = "0.1";
            for (int i = 0; i < precision; ++i) {
                while (ans * ans <= number) {
                    ans += increment;
                }

                // loop terminates when ans * ans > number 
                ans = ans - increment;
                increment = increment / 10;
            }
            return ans;
        }
		
		/// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
		static auto Sqrt(const auto& value, const int& precision=7)
		{
			return value.SqrtOf(precision);
		}

protected:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto UIntPowOpV1(const ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.IsPositive = 1;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                auto self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.IsPositive = 0;
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto IntPowOfOpV1(const ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (expValue < 0)//Negative Pow
            {
                ValueType exp = expValue * -1;
                if (DecimalHalf == 0 && IntHalf == 10 && expValue >= -9)
                {
                    IntHalf = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[exp];
                    if(IsNegative()&&exp&1==1)
                        IsPositive = 1;
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    //Switches from negative to positive if exp is odd number
                    bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                    auto self = AbsOf();//Prevent needing to flip the sign
                    IntHalf = 1; DecimalHalf = 0;// Initialize result
                    while (expValue > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (exp & 1 == 1)
                            *this /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self * self; //  Change x to x^2
                    }
                    if(IsNegative)
                        IntHalf.IsPositive = 0;
                }
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.IsPositive = 1;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                //Switches from negative to positive if exp is odd number
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                auto self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue & 1 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.IsPositive = 0;
            }
            return *this;
        }
		
        /// <summary>
        /// Applies Power of operation(for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto UIntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.UIntPowOpV1();
		}
		
        /// <summary>
        /// Applies Power of operation(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto IntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.IntPowOpV1();
		}

public:

		auto UnsignedNegIntPower(const unsigned int& exp)
		{
			ResetDivisor();
			//Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
			//Switches from negative to positive if exp is odd number
			bool IsNegative = IsPositive()?false:exp&1==1?false:true;
			auto self = AbsOf();
			IntHalf = 1; DecimalHalf = 0;// Initialize result
			while (expValue > 0)
			{
				// If expValue is odd, divide self with result
				if (exp & 1 == 1)
					*this /= self;
				// n must be even now
				expValue = expValue >> 1; // y = y/2
				self *= self; // Change x to x^2
			}
			if(IsNegative)
				IntHalf.IsPositive = 0;
		}

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto UIntPowOfOp = UIntPowOfOpV1<unsigned int>;

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto IntPowOfOp = IntPowOfOpV1<signed int>;
        constexpr auto UInt64PowOfOp = UIntPowOfOpV1<UInt64>;
        constexpr auto Int64PowOfOp = IntPowOpOfV1<Int64>;
        
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto UIntPowOf = UIntPowOfV1<unsigned int>;

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto IntPowOf = IntPowOfV1<signed int>;
        constexpr auto UInt64PowOf = UIntPowOfV1<UInt64>;
        constexpr auto Int64PowOf = IntPowOfV1<Int64>;

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
        auto NthRootOf(const int& n, const auto& precision = auto::JustAboveZero)
        {
            auto xPre = ((this - 1) / n) + 1;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            auto delX = auto(2147483647, 0);

            //  xK denotes current value of x 
            auto xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = (xPre * nMinus1 + this / auto::Pow(xPre, nMinus1)) / n;
                delX = auto::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        static auto FractionalPow(const auto& value, const int& expNum, const int& expDenom);

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        static auto FractionalPow(const auto& value, const boost::rational<int>& Frac);

        void BasicPowOp(const auto& expValue);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        auto PowOp(const auto& expValue);

        static auto PowOp(const auto& targetValue, auto& expValue)
        {
            return targetValue.PowOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="targetValue">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static auto Pow(const auto& targetValue, auto expValue)
        {
            return PowOp(targetValue, expValue);
        }

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:

public:
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modify owner object
        /// </summary>
        /// <returns>MediumDecBase</returns>
        auto ExpOf()
        {
            /*
             * Evaluates f(x) = e^x for any x in the interval [-709, 709].
             * If x < -709 or x > 709, raises an assertion error. Implemented
             * using the truncated Taylor series of e^x with ceil(|x| * e) * 12
             * terms. Achieves at least 14 and at most 16 digits of precision
             * over the entire interval.
             * Performance - There are exactly 36 * ceil(|x| * e) + 5
             * operations; 69,413 in the worst case (x = 709 or -709):
             * - (12 * ceil(|x| * e)) + 2 multiplications
             * - (12 * ceil(|x| * e)) + 1 divisions
             * - (12 * ceil(|x| * e)) additions
             * - 1 rounding
             * - 1 absolute value
             * Accuracy - Over a sample of 10,000 linearly spaced points in
             * [-709, 709] we have the following error statistics:
             * - Max relative error = 8.39803e-15
             * - Min relative error = 0.0
             * - Avg relative error = 0.0
             * - Med relative error = 1.90746e-15
             * - Var relative error = 0.0
             * - 0.88 percent of the values have less than 15 digits of precision
             * Args:
             *      - x: power of e to evaluate
             * Returns:
             *      - approximation of e^x in MediumDecBase precision
             */
             // Check that x is a valid input.
            assert(IntHalf.Value < 709);

            // When x = 0 we already know e^x = 1.
            if (IsZero()) {
                return One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            auto x0 = *this;
            x0.Abs();
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            auto tn = One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = CeilInt(x0 * E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = One / tn;
            }
            return tn;
        }

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modify owner object
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>BlazesRusCode::MediumDecBase</returns>
        static auto Exp(const auto& x)
        {
			return x.ExpOf();
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        auto NthRootOf(const int& n, const auto& Precision = FiveBillionth)
        {
            int nMinus1 = n - 1;
            auto x[2] = { (One / n) * ((*this*nMinus1) + (*this / PowOf(nMinus1))), targetValue };
            while (Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (One / n) * ((x[1]*nMinus1) + (targetValue / x[1].PowOf(nMinus1)));
            }
            return x[0];
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static auto NthRoot(const auto& value, const int& n, const auto& Precision = FiveBillionth)
        {
            return value.NthRootOf(n, Precision);
        }

protected:

		auto LnRef_Part02()
		{	
            //Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
			//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
			auto TotalRes = (this - 1) / (this + 1);
			auto LastPow = TotalRes;
			auto WSquared = TotalRes * TotalRes;
			auto AddRes;
			int WPow = 3;
			do
			{
				LastPow *= WSquared;
				AddRes = LastPow / WPow;
				TotalRes += AddRes; WPow += 2;
			} while (AddRes > JustAboveZero);
			return TotalRes;
		}

public:

		/// <summary>
		/// Natural log (Equivalent to Log_E(value))
		/// </summary>
		/// <returns>BlazesRusCode::MediumDecBase</returns>
		auto NaturalLogOf()
		{
			if(IntHalf.IsNegative)//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from natural log";
			if (IsOne())
				return Zero;
            if(IntHalf==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                auto W = (value - 1)/ (value + 1);
                auto TotalRes = -W;
                auto LastPow = W;
                auto WSquared = W * W;
                int WPow = 3;
                auto AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > MediumDecBase::JustAboveZero);
                return TotalRes * 2;
            }
            else if (IntHalf==1)//Threshold between 1 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {
                //This section gives accurate answer(for values between 1 and 2)
                auto threshold = MediumDecBase::FiveMillionth;
                auto base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                auto term = base;       // First term
                auto prev;          // Previous sum
                auto result = term;     // Kick it off

                do
                {
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                    ++den;
                } while (MediumDecBase::Abs(prev - result) > threshold);

                return result;
            }
			else
                //Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
                //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
				return LnRef_Part02() * 2;
		}
	
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>BlazesRusCode::MediumDecBase</returns>
        static auto Ln(const auto& value)
        {
			return value.NaturalLogOf();
        }
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
		auto Log10Of()
		{
			if(IntHalf.IsNegative)//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from log base 10";
			if (IsOne())
				return Zero;
            if (DecimalHalf == 0 && IntHalf.Value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (IntHalf.Value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDecBase(index, 0);
                }
                return MediumDecBase(9, 0);
            }
            if (IntHalf<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                auto threshold = FiveBillionth;
                auto base = this - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                auto term = base;       // First term
                auto prev = 0;          // Previous sum
                auto result = term;     // Kick it off

                while (Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return result*LN10Mult;// result/MediumDecBase::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return LnRef_Part02() * HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
            }
		}
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        static auto Log10(const auto& value)
        {
			return value.Log10Of();
        }
		
protected:

    template<IntegerType IntType=unsigned int>
    static auto Log10_IntPart02(const IntType& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        auto TotalRes = MediumDecBase(value - 1) / MediumDecBase(value + 1);
        auto LastPow = TotalRes;
        auto WSquared = TotalRes * TotalRes;
        auto AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDecBase::JustAboveZero);
        return TotalRes * MediumDecBase::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
	
public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
		template<IntegerType IntType=unsigned int>
        static auto Log10OfInt(const IntType& value)
        {
			if(value<0)//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from log base 10";
            else if (value == 1)
                return MediumDecBase::Zero;
            else if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDecBase(index, 0);
                }
                return MediumDecBase(9, 0);
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return Log10_IntPart02(value);
            }
        }
		
        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        auto LogOf(const auto& baseVal)
        {
            if (IsOne())
                return Zero;
            return Log10Of() / baseVal.Log10Of();
        }
		
        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        static auto Log(const auto& value, const auto& baseVal)
        {
            return value.LogOf(baseVal);
        }

protected:

        bool LogOfInt_BaseCalculation(const int& baseVal, auto& baseTotalRes)
        {
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDecBase(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDecBase(9, 0); 
                return false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDecBase((baseVal - 1), 0) / MediumDecBase((baseVal + 1), 0);
                auto baseLastPow = baseTotalRes;
                auto baseWSquared = baseTotalRes * baseTotalRes;
                auto baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > JustAboveZero);
            }
            return true;
        }

        bool LogOf_BaseCalculation(const auto& baseVal, auto& baseTotalRes)
        {
            if (baseVal.DecimalHalf==0&&baseVal.IntHalf.Value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDecBase(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDecBase(9, 0); 
                return false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDecBase((baseVal - 1), 0) / MediumDecBase((baseVal + 1), 0);
                auto baseLastPow = baseTotalRes;
                auto baseWSquared = baseTotalRes * baseTotalRes;
                auto baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > JustAboveZero);
            }
            return true;
        }

        auto LogOf_Section02(const bool& lnMultLog, const auto& baseTotalRes, const auto& threshold)
        {
            //Now calculate other log
			if(IsNegative())//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from log";
            if (DecimalHalf == 0 && IntHalf % 10 == 0)
            {
                if(lnMultLog)
                {
                    for (int index = 1; index < 9; ++index)
                    {
                        if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                            return MediumDecBase(index, 0) / (baseTotalRes * HalfLN10Mult);
                    }
                    return MediumDecBase(9, 0) / (baseTotalRes*HalfLN10Mult);
                }
                else
                {
                    for (int index = 1; index < 9; ++index)
                    {
                        if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                            return MediumDecBase(index, 0)/ baseTotalRes;
                    }
                    return MediumDecBase(9, 0)/baseTotalRes;
                }
            }
            if (IntHalf.Value<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                auto base = this - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                auto term = base;       // First term
                auto prev = Zero;          // Previous sum
                auto result = term;     // Kick it off

                while (Abs(prev - result) > threshold) {
                    ++den;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                if(lnMultLog)
                    return result/baseTotalRes;
                else
                    return (result*2)/ baseTotalRes;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                auto W = (this - 1) / (this + 1);
                auto TotalRes = W;
                auto AddRes;
                int WPow = 3;
                do
                {
                    AddRes = Pow(W, WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > JustAboveZero);
                if(lnMultLog)
                    return TotalRes/baseTotalRes;
                else
                    return (TotalRes * HalfLN10Mult)/ baseTotalRes;
            }
        }

public:

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        auto LogOfInt(const int& baseVal, const auto& threshold = FiveBillionth)
        {
            //Calculate Base log first
            auto baseTotalRes;
            bool lnMultLog = LogOfInt_BaseCalculation(baseTotalRes);
            return LogOf_Section02(lnMultLog, baseTotalRes, threshold);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
        auto LogOfV2(const auto& baseVal, const auto& threshold = FiveBillionth)
        {
            //Calculate Base log first
            auto baseTotalRes;
            bool lnMultLog = LogOf_BaseCalculation(baseTotalRes);
            return LogOf_Section02(lnMultLog, baseTotalRes, threshold);
        }

	#pragma endregion Log Functions

    #pragma region Trigonomic Functions

       /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        static auto Sin(const auto& Value)
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            for (int i = 1; i < 7; ++i)
            {
                SinValue += Pow(Value, 2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Cos from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        static auto Cos(const auto& Value)
        {
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            for (int i = 1; i < 7; ++i)
            {
                CosValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        static auto Tan(const auto& Value)
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            for (int i = 1; i < 7; ++i)
            {
                SinValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1)  / VariableConversionFunctions::Fact(2 * i + 1);
                CosValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        static auto ATan(const auto& Value)
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            //Angle as Radian
            for (int i = 1; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                CosValue += Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue / SinValue;
        }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        /// <returns>MediumDecBase</returns>
        static auto ArcTan2(const auto& y, const auto& x)
        {
            auto coeff_1 = PiNum / 4;
            auto coeff_2 = coeff_1 * 3;
            auto abs_y = Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            auto r;
            auto angle;
            if (x.IsPositive())
            {
                r = (x - abs_y) / (x + abs_y);
                angle = coeff_1 - coeff_1 * r;
            }
            else
            {
                r = (x + abs_y) / (abs_y - x);
                angle = coeff_2 - coeff_1 * r;
            }
            if (y.IsNegative())
                return -angle;// negate if in quad III or IV
            else
                return angle;
        }

        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        static auto SinFromAngle(auto Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntHalf.Value == 0)
                {
                    Value.IntHalf = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntHalf.Value %= 360;
                    Value.IntHalf.Value = 360 - Value.IntHalf;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntHalf.Value %= 360;
            }
            if(Value.DecimalHalf==0)
            {
                switch(Value.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return Zero;
                        break;
                    case 90://0.5 Pi Radians
                        return One;
                        break;
                    case 270://1.5 Pi Radians
                        return NegativeOne;
                        break;
                    case 30://0.1666666666 Pi Radians
                    case 150://0.833333333 Pi Radians
                        return PointFive;
                    case 210:
                    case 330:
                        return NegPointFive;
                    default:
                        //Angle as Radian
                        auto Radius = Pi * Value / 180;
                        return Sin(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                auto Radius = Pi * Value / 180;
                return Sin(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns></returns>
        static auto CosFromAngle(auto Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntHalf.Value == 0)
                {
                    Value.IntHalf = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntHalf.Value %= 360;
                    Value.IntHalf.Value = 360 - Value.IntHalf;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntHalf.Value %= 360;
            }
            if(Value.DecimalHalf==0)
            {
                switch(Value.IntHalf.Value)
                {
                    case 0:
                        return One;
                        break;
                    case 60:
                        return PointFive;
                        break;
                    case 90://0.5 Pi Radians
                    case 270://1.5 Pi Radians
                        return Zero;
                        break;
                    case 180://Pi Radians
                        return NegativeOne;
                        break;
                    case 120:
                    case 240:
                        return NegPointFive;
                    default:
                        //Angle as Radian
                        auto Radius = Pi * Value / 180;
                        return Cos(Radius)
                        break;
                }
            }
            else
            {
                //Angle as Radian
                auto Radius = Pi * Value / 180;
                return Cos(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        static auto TanFromAngle(auto Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntHalf.Value == 0)
                {
                    Value.IntHalf = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntHalf.Value %= 360;
                    Value.IntHalf.Value = 360 - Value.IntHalf;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntHalf.Value %= 360;
            }
            if(Value.DecimalHalf==0)
            {
                switch(Value.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return Zero;
                        break;
                    case 90://0.5 Pi Radians
                        return Maximum;//Positive Infinity
                        break;
                    case 270://1.5 Pi Radians
                        return Minimum;//Negative Infinity
                        break;
                    default:
                        return Tan(Pi * Value / 180);
                        break;
                }
            }
            else
                return Tan(Pi * Value / 180);
        }

    #pragma endregion Trigonomic Functions
    };

    #pragma region ValueDefine Source

	MirroredInt MediumDec::NegativeRep = MirroredInt::NegativeZero;
	MirroredInt MediumDec::MaxIntHalf = MirroredInt::Maximum;
	MirroredInt MediumDec::MinIntHalf = MirroredInt::Minimum;

    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void MediumDec::ReadString(const std::string& Value)
    {
        IntHalf = 0; DecimalHalf = 0;
        int PlaceNumber;
        std::string WholeNumberBuffer = "";
        std::string DecimalBuffer = "";

        bool ReadingDecimal = false;
        int TempInt;
        int TempInt02;
        for (char const& StringChar : Value)
        {
            if (VariableConversionFunctions::IsDigit(StringChar))
            {
                if (ReadingDecimal) { DecimalBuffer += StringChar; }
                else { WholeNumberBuffer += StringChar; }
            }
            else if (StringChar == '-')
				IntHalf.IsPositive = 0;
            else if (StringChar == '.')
                ReadingDecimal = true;
            else if(StringChar!=' ')
                break;//Stop Extracting after encounter non-number character such as i
        }
        PlaceNumber = WholeNumberBuffer.length() - 1;
        for (char const& StringChar : WholeNumberBuffer)
        {
            TempInt = VariableConversionFunctions::CharAsInt(StringChar);
            TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
            if (StringChar != '0')
            {
                IntHalf.Value += TempInt02;
            }
            PlaceNumber--;
        }
        PlaceNumber = 8;
        for (char const& StringChar : DecimalBuffer)
        {
            //Limit stored decimal numbers to the amount it can store
            if (PlaceNumber > -1)
            {
                TempInt = VariableConversionFunctions::CharAsInt(StringChar);
                TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
                if (StringChar != '0')
                {
                    DecimalHalf += TempInt02;
                }
                PlaceNumber--;
            }
        }
    }

    /// <summary>
    /// Gets the value from string.
    /// </summary>
    /// <param name="Value">The value.</param>
    /// <returns>MediumDec</returns>
    inline MediumDec MediumDec::GetValueFromString(const std::string& Value)
    {
        MediumDec NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

    std::string MediumDec::ToString()
    {
        std::string Value = "";
        unsigned int CurrentSection = IntHalf.Value;
        unsigned __int8 CurrentDigit;
        if (IsNegative())
            Value += "-";
        for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
        {
            CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
            CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        if (DecimalHalf != 0)
        {
            Value += ".";
            CurrentSection = DecimalHalf;
            for (__int8 Index = 8; Index >= 0; --Index)
            {
                CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
                CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
                if (CurrentDigit != 0)
                {
                    if(!DecBuffer.empty())
                    {
                        Value += DecBuffer;
                        DecBuffer.clear();
                    }
                    Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
                }
                else
                {
                    DecBuffer += VariableConversionFunctions::DigitAsChar(CurrentDigit);
                }
            }
        }
        return Value;
    }

    std::string MediumDec::ToFullString()
    {
        std::string Value = "";
        unsigned int CurrentSection = IntHalf.Value;
        unsigned __int8 CurrentDigit;
        if (IsNegative())
            Value += "-";
        for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
        {
            CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
            CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        if (DecimalHalf != 0)
        {
            Value += ".";
            bool HasDigitsUsed = false;
            CurrentSection = DecimalHalf;
            for (__int8 Index = 8; Index >= 0; --Index)
            {
                if (CurrentSection > 0)
                {
                    CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
                    CurrentSection -= (CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
                    Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
                }
                else
                    Value += "0";
            }
        }
        else
        {
            Value += ".000000000";
        }
        return Value;
    }
    #pragma endregion String Function Source
}