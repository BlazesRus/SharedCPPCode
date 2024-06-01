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
        MediumDecBase operator[](VariantType variantValue) const
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
        void  SetValueToTenthLN10()
        {
            IntHalf = 0; DecimalHalf = 434294482;
        }

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        void  SetValueToFifthLN10()
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
			if (MediumDecBase DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
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

    #pragma region Other Operators

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator -()
        {
			MediumDecBase self = this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

    };
    #pragma region ValueDefine Source

	const MirroredInt MediumDecBase::NegativeRep = MirroredInt::NegativeZero;
	const MirroredInt MediumDecBase::MaxIntHalf = MirroredInt::Maximum;
	const MirroredInt MediumDecBase::MinIntHalf = MirroredInt::Minimum;

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
            Value += std::string(DecimalHalf);
        }
        return Value;
    }

    std::string MediumDecBase::ToFullString()
    {
        std::string Value = std::string(IntHalf);
        if (DecimalHalf != 0)
        {
			unsigned __int8 CurrentDigit;
            Value += ".";
            bool HasDigitsUsed = false;
            unsigned int CurrentSection = DecimalHalf.Value;
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