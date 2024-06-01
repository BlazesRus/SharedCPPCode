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
        void SetDecimalVal(const ldouble& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			ldouble lValue = Value;
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
                lValue -= (ldouble)WholeValue;
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
        MediumDecBase(const ldouble& Value){ this->SetDecimalVal(Value); }

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
        ldouble toDecimal()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            ldouble Value;
            if (IntHalf < 0)
            {
                Value = (ldouble)-IntHalf.Value;
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
                Value = (ldouble)IntHalf.Value;
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
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
        explicit operator ldouble() { return toDecimal(); }

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

    };
    #pragma region ValueDefine Source

	auto MediumDecBase::NegativeRep = MirroredInt::NegativeZero;
	auto MediumDecBase::MaxIntHalf = MirroredInt::Maximum;
	auto MediumDecBase::MinIntHalf = MirroredInt::Minimum;

    #pragma endregion ValueDefine Source

    #pragma region String Function Source

    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void MediumDecBase::ReadString(const std::string& Value)
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
				IntHalf.Sign = 0;
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

    std::string MediumDecBase::ToString()
    {
        std::string Value = std::string(IntHalf);
        if (DecimalHalf != 0)
        {
            Value += ".";
            Value += std::string(DecimalHalf)
        }
        return Value;
    }

    std::string MediumDecBase::ToFullString()
    {
        std::string Value = std::string(IntHalf);
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