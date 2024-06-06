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
    class MediumDecV2Base;

    //Reduced version of MediumDec result for modulus result and other stuff
    class DLL_API MediumDecV2Base : public AltNumBase//AltNumBase is used as base class to identify as MediumDec variant
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
        signed int GetIntHalf() const
        {
			return IntHalf.GetValue();
        }

    #pragma endregion DigitStorage

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
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2Base(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2Base(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        MediumDecV2Base& operator=(const MediumDecV2Base& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumDecV2Base& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<MediumDecVariant VariantType>
        MediumDecV2Base operator[](VariantType variantValue) const
        {
            MediumDecV2Base newSelf = MediumDecV2Base(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

    #pragma endregion class_constructors

    #pragma region Negative_Status

        bool IsPositive() const
        { return IntHalf.IsPositive(); }

        bool IsNegative() const
        { return IntHalf.IsNegative(); }

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        { IntHalf.Sign ^= 1; }

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecV2Base</returns>
        MediumDecV2Base operator-() const
        { MediumDecV2Base self = *this; self.SwapNegativeStatus(); return self; }

    #pragma endregion Negative_Status


    #pragma region Check_if_value

		//Set value as exactly zero
        void SetAsZero();

		//Set value as exactly one
        void SetAsOne();
		
		//Set as +-1 while keeping current sign
        void SetAsOneVal();

        void SetAsValues(const MirroredInt& intVal = MirroredInt::Zero, const PartialInt& decVal = PartialInt::Zero);

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt() const;

        bool IsNotAtZeroInt() const;

        bool IsAtOneInt() const;

        bool IsNotAtOneInt() const;

        //Detect if at exactly zero(only overridden with MixedDec)
		bool IsZero() const;
		
		bool IsOne() const;
		
		bool IsNegOne() const;
		
		bool IsOneVal() const;

		bool IsOneVariantVal() const;

    #pragma endregion Check_if_value

    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum();

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum();
	
    #pragma endregion RangeLimits

    #pragma region ValueSetters
protected://Work around for not allowing to use incomplete class statics during forming of class
        static const unsigned int LN10Div_DecSection = 434294482;
        static const unsigned int TwiceLN10Div_DecSection = 868588964;
    
public:

        /// <summary>
        /// Sets value to Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        void  SetValueToPiNum();

        //100,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToHundredMilPiNum();

        //10,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToTenMilPiNum();

        //1,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToOneMilPiNum();

        //10xPi(Rounded to 9th decimal digit)
        void  SetValueToTenPiNum();

        /// <summary>
        /// Euler's number rounded to 9th digit(2.718281828)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        void  SetValueToENum();
        
        //Sets value to value at 0.5
        void  SetValueToPoint5();

        void  SetValueToJustAboveZero();

        /// <summary>
        /// Sets the value at .000001000
        /// </summary>
        void  SetValueToOneMillionth();

        /// <summary>
        /// Sets the value at "0.005"
        /// </summary>
        /// <returns>MediumDec</returns>
        void  SetValueToFiveThousandth();

        /// <summary>
        /// Sets the value at "0.000005"
        /// </summary>
        void  SetValueToFiveMillionth();

        //0e-7
        void  SetValueToTenMillionth();

        /// <summary>
        /// Sets the value to .000000010
        /// </summary>
        void  SetValueToOneHundredMillionth();

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        void  SetValueToLN10();

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        void  SetValueToLN10Div();

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        void  SetValueToTwiceLN10Div();

    #pragma endregion ValueSetters

    #pragma region ValueDefines
    private://Each class needs to define it's own
        
        static MediumDecV2Base AlmostOneValue();

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base PiNumValue();

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2Base HundredMilPiNumValue();

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2Base TenMilPiNumValue();

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2Base OneMilPiNumValue();

        //10xPi(Rounded to 9th decimal digit)
        static MediumDecV2Base TenPiNumValue();
        
        static MediumDecV2Base ENumValue();
        
        static MediumDecV2Base ZeroValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base OneValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base TwoValue();

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base NegativeOneValue();

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Point5Value();

        static MediumDecV2Base JustAboveZeroValue();

        static MediumDecV2Base OneMillionthValue();

        static MediumDecV2Base FiveThousandthValue();

        static MediumDecV2Base FiveMillionthValue();

        static MediumDecV2Base TenMillionthValue();

        static MediumDecV2Base OneHundredMillionthValue();

        static MediumDecV2Base FiveBillionthValue();

        static MediumDecV2Base LN10Value();

        static MediumDecV2Base LN10DivValue();

        static MediumDecV2Base TwiceLN10DivValue();

        static MediumDecV2Base MinimumValue();

        static MediumDecV2Base MaximumValue();

        static MediumDecV2Base NegativePointFiveValue();

        static MediumDecV2Base NegativePointFive;

        static MediumDecV2Base AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base ENum;
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base OneGMillionth;

        //0e-7
        static MediumDecV2Base TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDecV2Base FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDecV2Base Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDecV2Base Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDecV2Base LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecV2Base LN10Div;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecV2Base TwiceLN10Div;

public:
    #pragma endregion ValueDefines

    #pragma region String Commands

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(const std::string& Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecV2Base(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const std::string& Value)
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
        void SetFloatVal(const float& Value);

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDoubleVal(const double& Value);

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDecimalVal(const long double& Value);

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetBoolVal(const bool& Value);

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(const int& Value);

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetUIntVal(const unsigned int& Value);

/*
        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const float& Value){ this->SetFloatVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const double& Value){ this->SetDoubleVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const long double& Value){ this->SetDecimalVal(Value); }
*/

        MediumDecV2Base(const unsigned __int64& Value){ this->SetUIntVal(Value); }
        MediumDecV2Base(const signed __int64& Value){ this->SetIntVal(Value); }
        MediumDecV2Base(const unsigned char& Value){ this->SetUIntVal(Value); }
        MediumDecV2Base(const signed char& Value){ this->SetIntVal(Value); }
        MediumDecV2Base(const unsigned short& Value){ this->SetUIntVal(Value); }
        MediumDecV2Base(const signed short& Value){ this->SetIntVal(Value); }
        MediumDecV2Base(const unsigned int& Value){ this->SetUIntVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const bool& Value){ this->SetBoolVal(Value); }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

        //To-Do: Add more exact conversion from floating point format to MediumDec variant

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        float toFloat() const;

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        double toDouble() const;

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        long double toDecimal() const;

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toInt() const { return IntHalf.GetValue(); }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toUInt() const { return IntHalf.IsNegative()?0:IntHalf.Value; }

        bool toBool() const { return IntHalf.IsZero() ? false : true; }

/*
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
*/

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to uint explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed __int64() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to ubyte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned char() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to byte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed char() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to ushort explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned short() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to short explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed short() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
		template<MediumDecVariant VariantType=MediumDecV2Base>
		std::strong_ordering BasicComparisonV1(const VariantType& that) const
		{
			if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			unsigned int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (MediumDecV2Base DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
		//Compare only as if in NormalType representation mode
		std::strong_ordering BasicIntComparison(const int& that) const;

public:

		std::strong_ordering operator<=>(const MediumDecV2Base& that) const
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

		bool operator==(const MediumDecV2Base& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.DecimalHalf)
				return false;
            return true;
		}

		bool operator!=(const MediumDecV2Base& that) const
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
        void PartialUIntDivOpV1(const IntType& rValue)
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
        void PartialUIntDivOp(const unsigned int& rValue) { PartialUIntDivOpV1(rValue); }
        void PartialIntDivOp(const signed int& rValue) { PartialIntDivOpV1(rValue); }
        void PartialUInt64DivOp(const unsigned int& rValue) { PartialUIntDivOpV1(rValue); }
        void PartialInt64DivOp(const signed __int64& rValue) { PartialIntDivOpV1(rValue); }

        void UnsignedPartialIntDivOp(const signed int& rValue) { PartialUIntDivOpV1(rValue); }
        void UnsignedPartialInt64DivOp(const signed __int64& rValue) { PartialUIntDivOpV1(rValue); }

protected:

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        void UIntDivOpV1(const IntType& Value)
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
		
        template<IntegerType IntType=signed int>
        void IntDivOpV1(const IntType& Value)
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
        MediumDecV2Base& UIntDivOperationV1(const IntType& rValue)
        { UIntDivOpV1(rValue); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDecV2Base& IntDivOperationV1(const IntType& rValue)
        { IntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        const auto DivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntDivOperationV1(rValue); }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        const auto DivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntDivOperationV1(rValue); }

public:

        void UIntDivOp(const unsigned int& rValue) { UIntDivOpV1(rValue); }
        void IntDivOp(const signed int& rValue) { IntDivOpV1(rValue); }
        void UInt64DivOp(const unsigned __int64& rValue) { UIntDivOpV1(rValue); }
        void Int64DivOp(const signed __int64& rValue) { IntDivOpV1(rValue); }

        void UnsignedIntDivOp(const signed int& rValue) { UIntDivOpV1(rValue); }
        void UnsignedInt64DivOp(const signed __int64& rValue) { UIntDivOpV1(rValue); }

        void UInt8DivOp(const unsigned char& rValue) { UIntDivOpV1(rValue); }
        void Int8DivOp(const signed char& rValue) { IntDivOpV1(rValue); }
        void UInt16DivOp(const unsigned short& rValue) { UIntDivOpV1(rValue); }
        void Int16DivOp(const signed short& rValue) { IntDivOpV1(rValue); }

        MediumDecV2Base& UIntDivOperation(const unsigned int& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2Base& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2Base& UInt64DivOperation(const unsigned __int64& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2Base& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2Base& UInt8DivOperation(const unsigned char& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2Base& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2Base& UInt16DivOperation(const unsigned short& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2Base& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

        const MediumDecV2Base DivideByUInt(const unsigned int& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        const MediumDecV2Base DivideByUInt64(const unsigned __int64& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        const MediumDecV2Base UnsignedDivideByInt(const signed int& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base UnsignedDivideByInt64(const signed __int64& rValue) { return DivideByUIntV1(rValue); }

        const MediumDecV2Base DivideByUInt8(const unsigned char& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
        const MediumDecV2Base DivideByUInt16(const unsigned short& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations
protected:

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        template<MediumDecVariant VariantType=MediumDecV2Base>
        bool UnsignedPartialDivOpV1(const VariantType& rValue)
        {
            unsigned _int64 SelfRes = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
            unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf.Value + rValue.DecimalHalf.Value;

            unsigned _int64 IntHalfRes = SelfRes / ValueRes;
            unsigned _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
			IntHalf.Value = (unsigned int) IntHalfRes;
            DecimalHalf.Value = DecimalRes;
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        template<MediumDecVariant VariantType=MediumDecV2Base>
        void UnsignedDivOpV1(const VariantType& rValue)
		{
			if(DecimalHalf==0)
			{
				if(rValue.DecimalHalf==0)
				{
					switch(rValue.IntHalf.Value)
					{
						case 2:
							if((IntHalf.Value&1)==1)//Check if number is odd
								UnsignedIntDivOp(2);
							else
								IntHalf.Value /= 2;
							break;
						case 4:
							if(((IntHalf.Value >> 2) << 2) == IntHalf.Value)
								IntHalf.Value /= 4;
							else
								UnsignedIntDivOp(4);
							break;
						case 8:
							if(((IntHalf.Value >> 3) << 3) == IntHalf.Value)
								IntHalf.Value /= 8;
							else
								UnsignedIntDivOp(4);
							break;
						case 16:
							if(((IntHalf.Value >> 4) << 4) == IntHalf.Value)
								IntHalf.Value /= 16;
							else
								UnsignedIntDivOp(4);
							break;
						case 32:
							if(((IntHalf.Value >> 5) << 5) == IntHalf.Value)
								IntHalf.Value /= 32;
							else
								UnsignedIntDivOp(4);
							break;
                        case 0:
                            throw "Target rValue can not be divided by zero";
                            break;
						default:
							UnsignedIntDivOp(rValue.IntHalf.Value);
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
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        template<MediumDecVariant VariantType=MediumDecV2Base>
        void DivOpV1(const VariantType& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                UnsignedDivOp(-Value);
            }
            else
                UnsignedDivOp(Value);
        }

public:

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool UnsignedPartialDivOp(const MediumDecV2Base& rValue){ return UnsignedPartialDivOpV1(rValue); }
		
		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void UnsignedDivOp(const MediumDecV2Base& rValue){ UnsignedDivOpV1(rValue); }
		
		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void DivOp(const MediumDecV2Base& rValue){ DivOpV1(rValue); }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& UnsignedDivOperation(const MediumDecV2Base& rValue)
		{ UnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& DivOperation(const MediumDecV2Base& rValue)
		{ DivOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        const MediumDecV2Base DivideByUnsigned(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.UnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        const MediumDecV2Base DivideBy(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.DivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecV2Base&</returns>
        void DivideByTwo();

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecV2Base&</returns>
        void DivideByFour();

protected:

        //Return copy of result divided by two
        template<MediumDecVariant VariantType = MediumDecV2Base>
        VariantType DividedByTwoV1() const
        {
            VariantType result = *this; result.DivideByTwo();
            return result;
        }

        //Return copy of result divided by four
        template<MediumDecVariant VariantType = MediumDecV2Base>
        VariantType DividedByFourV1() const
        {
            VariantType result = *this; result.DivideByFour();
            return result;
        }

public:

        //Return copy of result divided by two
        MediumDecV2Base DividedByTwo() const;

        //Return copy of result divided by four
        MediumDecV2Base DividedByFour() const;

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const MediumDecV2Base& rValue) { return lValue.DivOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const MediumDecV2Base& rValue) { return lValue.DivideBy(rValue); }
		
        /// <summary>
        /// Division operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }
		
        friend MediumDecV2Base operator/(const signed int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const signed __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }

        friend MediumDecV2Base operator/(const signed char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const signed short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }

	#pragma endregion Other Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
		/// <summary>
        /// Partial version of UIntMultOpV1 without zero checks
        /// (Modifies owner object) 
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=int>
        void PartialUIntMultOpV1(const IntType& rValue)
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
                PartialUIntMultOpV1(-Value);
            }
            else
                PartialUIntMultOp(Value);
        }
public:
		
        void PartialUIntMultOp(const unsigned int& rValue) { PartialUIntMultOpV1(rValue); }
        void PartialIntMultOp(const signed int& rValue) { PartialIntMultOpV1(rValue); }
        void PartialUInt64MultOp(const unsigned __int64& rValue) { PartialUIntMultOpV1(rValue); }
        void PartialInt64MultOp(const signed __int64& rValue) { PartialIntMultOpV1(rValue); }

        void UnsignedPartialIntMultOp(const signed int& rValue) { PartialUIntMultOpV1(rValue); }
        void UnsignedPartialInt64MultOp(const signed __int64& rValue) { PartialUIntMultOpV1(rValue); }
		
protected:
        template<IntegerType IntType=signed int>
        void UIntMultOpV1(const IntType& rValue)
        {
            if (rValue == 0)
            {
                SetAsZero();
                return;
            }
            else if (IsZero())
                return;
            PartialUIntMultOpV1(rValue);
        }

        template<IntegerType IntType=signed int>
        void IntMultOpV1(const IntType& Value)
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
        auto& UIntMultOperationV1(const IntType& Value)
        { UIntMultOpV1(Value); return *this; }

        template<IntegerType IntType=unsigned int>
        auto& IntMultOperationV1(const IntType& Value)
        { IntMultOpV1(Value); return *this; }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        const MediumDecV2Base MultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntMultOperationV1(rValue); }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        const MediumDecV2Base MultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntMultOperationV1(rValue); }

public:

        void UIntMultOp(const unsigned int& rValue) { UIntMultOpV1(rValue); }
        void IntMultOp(const signed int& rValue) { IntMultOpV1(rValue); }
        void UInt64MultOp(const unsigned __int64& rValue) { UIntMultOpV1(rValue); }
        void Int64MultOp(const signed __int64& rValue) { IntMultOpV1(rValue); }

        void UnsignedIntMultOp(const signed int& rValue) { UIntMultOpV1(rValue); }
        void UnsignedInt64MultOp(const signed __int64& rValue) { UIntMultOpV1(rValue); }

        void UInt8MultOp(const unsigned char& rValue) { UIntMultOpV1(rValue); }
        void Int8MultOp(const signed char& rValue) { IntMultOpV1(rValue); }
        void UInt16MultOp(const unsigned short& rValue) { UIntMultOpV1(rValue); }
        void Int16MultOp(const signed short& rValue) { IntMultOpV1(rValue); }

        MediumDecV2Base& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2Base& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2Base& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2Base& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2Base& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2Base& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2Base& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2Base& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

        const MediumDecV2Base MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
        const MediumDecV2Base MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

        const MediumDecV2Base UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

        const MediumDecV2Base MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
        const MediumDecV2Base MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations
protected:

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        template<MediumDecVariant VariantType=MediumDecV2Base>
        void UnsignedMultOpV1(const VariantType& rValue)
		{
            if (DecimalHalf == 0)
            {
                if (IntHalf.Value == 1)
                {
					if(IntHalf.IsNegative())
						IntHalf = -rValue.IntHalf;
					else
						IntHalf = rValue.IntHalf;
					DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                    IntHalf *= rValue.IntHalf;
                else {
                    __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf.Value : DecimalOverflowX * rValue.IntHalf.Value + rValue.DecimalHalf.Value;
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
                    SRep += (__int64)DecimalHalf.Value * rValue.IntHalf.Value;
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
                    SRep *= rValue.IntHalf.Value;
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
                    SRep *= rValue.DecimalHalf.Value;
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
                    unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.IntHalf.Value;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    unsigned __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf.Value);//Temp03 holds __int64 version of X *.V
                    unsigned __int64 Temp04 = (__int64)DecimalHalf.Value * (__int64)rValue.DecimalHalf.Value;
                    Temp04 /= DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    unsigned __int64 IntegerRep = SRep + Temp03 + Temp04;
                    unsigned __int64 intHalf = IntegerRep / DecimalOverflow;
                    IntegerRep -= intHalf * DecimalOverflowX;
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

        template<MediumDecVariant VariantType=MediumDecV2Base>
        void MultOpV1(const VariantType& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                UnsignedMultOp(-Value);
            }
            else
                UnsignedMultOp(Value);
        }

public:

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        void UnsignedMultOp(const MediumDecV2Base& rValue){ UnsignedMultOpV1(rValue); }
		
        void MultOp(const MediumDecV2Base& rValue){ MultOpV1(rValue); }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& UnsignedMultOperation(const MediumDecV2Base& rValue)
        { UnsignedMultOp(rValue); return *this; }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& MultOperation(const MediumDecV2Base& rValue)
		{ MultOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base MultiplyByUnsigned(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.UnsignedMultOperation(rValue); }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecV2Base MultiplyBy(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.MultOperation(rValue); }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region Other multiplication operations

        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>void</returns>
        void MultiplyByTwo();

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>void</returns>
        void MultiplyByFour();

protected:

    //Return copy of result divided by two
    template<MediumDecVariant VariantType = MediumDecV2Base>
    VariantType MultipliedByTwoV1() const
    {
        VariantType result = *this; result.MultiplyByTwo();
        return result;
    }

    //Return copy of result divided by four
    template<MediumDecVariant VariantType = MediumDecV2Base>
    VariantType MultipliedByFourV1() const
    {
        VariantType result = *this; result.MultiplyByFour();
        return result;
    }

public:

    //Return copy of result divided by two
    MediumDecV2Base MultipliedByTwo() const;

    //Return copy of result divided by four
    MediumDecV2Base MultipliedByFour() const;

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const MediumDecV2Base& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const MediumDecV2Base& rValue) { return lValue.MultOperation(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }
		
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }
		
        friend MediumDecV2Base operator*(signed int lValue, MediumDecV2Base rValue)  { return rValue.IntMultOperation(lValue); }
        friend MediumDecV2Base operator*(signed __int64 lValue, MediumDecV2Base& rValue)  { return rValue.Int64MultOperation(lValue); }
        friend MediumDecV2Base operator*(unsigned int lValue, MediumDecV2Base& rValue)  { return rValue.UIntMultOperation(lValue); }
        friend MediumDecV2Base operator*(unsigned __int64 lValue, MediumDecV2Base& rValue)  { return rValue.UInt64MultOperation(lValue); }
		
        friend MediumDecV2Base operator*(signed char lValue, MediumDecV2Base& rValue)  { return rValue.Int8MultOperation(lValue); }
        friend MediumDecV2Base operator*(signed short lValue, MediumDecV2Base& rValue)  { return rValue.Int16MultOperation(lValue); }
        friend MediumDecV2Base operator*(unsigned char lValue, MediumDecV2Base& rValue)  { return rValue.UInt8MultOperation(lValue); }
        friend MediumDecV2Base operator*(unsigned short lValue, MediumDecV2Base& rValue)  { return rValue.UInt16MultOperation(lValue); }

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
        void UIntAddOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingUnsignedIntegerAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntAddOp((unsigned int)rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void IntAddOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingIntegerAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue;
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        template<IntegerType IntType=unsigned int>
        auto& UIntAddOperationV1(const IntType& rValue)
        { UIntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& IntAddOperationV1(const IntType& rValue)
        { IntAddOpV1(rValue); return *this; }

		/// <summary>
        ///  addition operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto AddByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntAddOperationV1(rValue); }

		/// <summary>
        /// Basic addition operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto AddByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntAddOperationV1(rValue); }

public:

        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UnsignedIntegerAddition(const MirroredInt& rValue);

        /// <summary>
        /// Basic addition operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void IntegerAddition(const MirroredInt& rValue);

        void UIntAddOp(const unsigned int& rValue);

        void IntAddOp(const signed int& rValue) { IntAddOpV1(rValue); }
        void UInt64AddOp(const unsigned __int64& rValue) { UIntAddOpV1(rValue); }
        void Int64AddOp(const signed __int64& rValue) { IntAddOpV1(rValue); }

        void UnsignedIntAddOp(const signed int& rValue) { UIntAddOpV1(rValue); }
        void UnsignedInt64AddOp(const signed __int64& rValue) { UIntAddOpV1(rValue); }

        void UInt8AddOp(const unsigned char& rValue) { UIntAddOpV1(rValue); }
        void Int8AddOp(const signed char& rValue) { IntAddOpV1(rValue); }
        void UInt16AddOp(const unsigned short& rValue) { UIntAddOpV1(rValue); }
        void Int16AddOp(const signed short& rValue) { IntAddOpV1(rValue); }

        MediumDecV2Base& UIntAddOperation(const unsigned int& rValue);

        MediumDecV2Base& IntAddOperation(const signed int& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2Base& UInt64AddOperation(const unsigned __int64& rValue) { return UIntAddOperationV1(rValue); }
        MediumDecV2Base& Int64AddOperation(const signed __int64& rValue) { return IntAddOperationV1(rValue); }
		
        MediumDecV2Base& UInt8AddOperation(const unsigned char& rValue) { return UIntAddOperationV1(rValue); }
        MediumDecV2Base& Int8AddOperation(const signed char& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2Base& UInt16AddOperation(const unsigned short& rValue) { return UIntAddOperationV1(rValue); }
        MediumDecV2Base& Int16AddOperation(const signed short& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2Base AddByUInt(const unsigned int& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2Base AddByInt(const signed int& rValue) { return AddByIntV1(rValue); }
        MediumDecV2Base AddByUInt64(const unsigned __int64& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2Base AddByInt64(const signed __int64& rValue) { return AddByIntV1(rValue); }

        MediumDecV2Base UnsignedAddByInt(const signed int& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2Base UnsignedAddByInt64(const signed __int64& rValue) { return AddByUIntV1(rValue); }

        MediumDecV2Base AddByUInt8(const unsigned char& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2Base AddByInt8(const signed char rValue) { return AddByIntV1(rValue); }
        MediumDecV2Base AddByUInt16(const unsigned short& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2Base AddByInt16(const signed short& rValue) { return AddByIntV1(rValue); }
    	
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
        void UIntSubOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingUnsignedIntegerSubOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntSubOp((unsigned int)rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void IntSubOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingIntegerSubOp(rValue);
			else {
				unsigned int signBeforeOp = IntHalf.Sign;
				IntHalf -= rValue;
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        template<IntegerType IntType=unsigned int>
        auto& UIntSubOperationV1(const IntType& rValue)
        { UIntSubOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& IntSubOperationV1(const IntType& rValue)
        { IntSubOpV1(rValue); return *this; }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto SubtractByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntSubOperationV1(rValue); }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto SubtractByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntSubOperationV1(rValue); }

public:

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UnsignedIntegerSubtraction(const MirroredInt& rValue);

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void IntegerSubtraction(const MirroredInt& rValue);

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UIntSubOp(const unsigned int& rValue);

        void IntSubOp(const signed int& rValue) { IntSubOpV1(rValue); }
        void UInt64SubOp(const unsigned __int64& rValue) { UIntSubOpV1(rValue); }
        void Int64SubOp(const signed __int64& rValue) { IntSubOpV1(rValue); }

        void UnsignedIntSubOp(const signed int& rValue) { UIntSubOpV1(rValue); }
        void UnsignedInt64SubOp(const signed __int64& rValue) { UIntSubOpV1(rValue); }

        void UInt8SubOp(const unsigned char& rValue) { UIntSubOpV1(rValue); }
        void Int8SubOp(const signed char& rValue) { IntSubOpV1(rValue); }
        void UInt16SubOp(const unsigned short& rValue) { UIntSubOpV1(rValue); }
        void Int16SubOp(const signed short& rValue) { IntSubOpV1(rValue); }

        MediumDecV2Base& UIntSubOperation(const unsigned int& rValue);

        MediumDecV2Base& IntSubOperation(const signed int& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2Base& UInt64SubOperation(const unsigned __int64& rValue) { return UIntSubOperationV1(rValue); }
        MediumDecV2Base& Int64SubOperation(const signed __int64& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2Base& UInt8SubOperation(const unsigned char& rValue) { return UIntSubOperationV1(rValue); }
        MediumDecV2Base& Int8SubOperation(const signed char& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2Base& UInt16SubOperation(const unsigned short& rValue) { return UIntSubOperationV1(rValue); }
        MediumDecV2Base& Int16SubOperation(const signed short& rValue) { return IntSubOperationV1(rValue); }

        MediumDecV2Base SubtractByUInt(const unsigned int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2Base SubtractByInt(const signed int& rValue) { return SubtractByIntV1(rValue); }
        MediumDecV2Base SubtractByUInt64(const unsigned __int64& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2Base SubtractByInt64(const signed __int64& rValue) { return SubtractByIntV1(rValue); }

        MediumDecV2Base UnsignedSubtractByInt(const signed int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2Base UnsignedSubtractByInt64(const signed __int64& rValue) { return SubtractByUIntV1(rValue); }

        MediumDecV2Base SubtractByUInt8(const unsigned char& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2Base SubtractByInt8(const signed char rValue) { return SubtractByIntV1(rValue); }
        MediumDecV2Base SubtractByUInt16(const unsigned short& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2Base SubtractByInt16(const signed short& rValue) { return SubtractByIntV1(rValue); }
    	
	#pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep AltNum Addition/Subtraction Operations
protected:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDecV2Base>
        void UnsignedAddOpV1(const VariantType& rValue)
        {
			if(rValue.DecimalHalf==0)
                UnsignedIntegerAddition(rValue.IntHalf);
			else 
            {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UnsignedAddOp(rValue.IntHalf);
                if (signBeforeOp==MirroredInt::NegativeSign)
                {
					if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -4.5
						if(IntHalf.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					}
                    else if(rValue.DecimalHalf.Value>DecimalHalf.Value)
                    {
						++IntHalf;
						if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
							DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                        else//-1.6 + .7 = -0.9
							DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
					} else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
						DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10
					else
						DecimalHalf.Value -= rValue.DecimalHalf.Value;
                } else {
					unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
					if(decResult==DecimalOverflow){//5.4 + 4.6
						++IntHalf;
						if(IntHalf.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					} else if(decResult>DecimalOverflow){//5.4 + 4.7
						++IntHalf;
						DecimalHalf.Value = decResult - DecimalOverflow;
					}
					else if(signBeforeOp!=IntHalf.Sign)
						DecimalHalf.Value = DecimalOverflow - decResult;
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
        template<MediumDecVariant VariantType=MediumDecV2Base>
        void AddOpV1(const VariantType& rValue)
        {
            if (rValue.DecimalHalf == 0)
                IntegerAddition(rValue.IntHalf);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue.IntHalf;
				
                if (signBeforeOp==MirroredInt::NegativeSign){
					if(rValue.IsPositive()){
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							++IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
								DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
							else//-1.6 + .7 = -0.9
								DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
						} else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
							DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10 
						else
							DecimalHalf.Value -= rValue.DecimalHalf.Value;
					} else {
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                        if (decResult == DecimalOverflow) {//-5.4 + - 5.6
                            --IntHalf;
                            if (IntHalf.Value == 0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1 
							--IntHalf;
							DecimalHalf.Value = decResult - DecimalOverflow;
						} else//-5.2 - 5.2 = -10.4
							DecimalHalf.Value = decResult;
					}
                } else {
					if(rValue.IsPositive()){
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
						if(decResult==DecimalOverflow){//5.5 + 4.5 = 10
							++IntHalf;
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(decResult>DecimalOverflow){//5.5 + 4.6 = 10.1
							++IntHalf;
							DecimalHalf.Value = decResult - DecimalOverflow;
						} else//5.4 + 5.3 = 10.7
							DecimalHalf.Value = decResult;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -5.5
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							--IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//4.3 - 5.4 = -1.1
								DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
							else//4.3 - 2.4 = 1.9
								DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
						} else if(signBeforeOp!=IntHalf.Sign)//5.4 + - 6.3 = -0.9
							DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 + 3 - 4
						else//4.4 + -2.3 = 2.1
							DecimalHalf.Value -= rValue.DecimalHalf.Value;
					}
                }
			}
		}
	
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDecV2Base>
        void UnsignedSubOpV1(const VariantType& rValue)
        {
            if (rValue.DecimalHalf == 0)
                UnsignedIntegerSubtraction(rValue.IntHalf);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UnsignedSubOp(rValue.IntHalf);
                if (signBeforeOp==MirroredInt::NegativeSign){//-5 - B 
					unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                    if (decResult == DecimalOverflow){//-5.4 - 5.6
                        --IntHalf;
                        if (IntHalf.Value == 0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1 
						--IntHalf;
						DecimalHalf.Value = decResult - DecimalOverflow;
					} else//-5.2 - 5.2 = -10.4
						DecimalHalf.Value = decResult;
                } else {//5.XX - B
					if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - 5.5 = 10
						if(IntHalf.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
						--IntHalf;
						if(signBeforeOp!=IntHalf.Sign)//5.4 - 5.7 = -0.3
							DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
						else//5.4 - 3.6 = 1.8
							DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
					} else if(signBeforeOp!=IntHalf.Sign)//5.3 - 7.2 = -1.9
						DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
					else//5.4 - 5.3 = 0.1 
						DecimalHalf.Value -= rValue.DecimalHalf.Value;
                }
			}
		}
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDecV2Base>
        void SubOpV1(const VariantType& rValue)
        {
			if(rValue.DecimalHalf==0)
                IntegerSubtraction(rValue.IntHalf);
			else 
			{
				int signBeforeOp = IntHalf.Sign;
				IntHalf -= rValue.IntHalf;
                if (signBeforeOp==MirroredInt::NegativeSign)
				{
					if(rValue.IsPositive())
					{
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
						if(decResult==DecimalOverflow){//-5.4 - 5.6
							--IntHalf;
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(decResult>DecimalOverflow){//-5.4 - 5.7 = -11.1 
							--IntHalf;
							DecimalHalf.Value = decResult - DecimalOverflow;
						} else//-5.2 - 5.2 = -10.4
							DecimalHalf.Value = decResult;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//-5.4 - -4.4
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							++IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//-5.4 - -6.5 = 1.1
								DecimalHalf = DecimalOverflow - DecimalHalf.Value - rValue.DecimalHalf;
							else//-5.4 - -4.5 = -0.9 == -5.4 + 4.5
								DecimalHalf.Value += rValue.DecimalHalf.Value;
						} else if(signBeforeOp!=IntHalf.Sign)//-5.4 - -7.3 = 1.9
							DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;
						else//-5.4 - -3.3 = -2.1
							DecimalHalf.Value -= rValue.DecimalHalf.Value;
					}
                } else {
					if(rValue.IsPositive()){
                        if (DecimalHalf.Value == rValue.DecimalHalf.Value) {//5.5 - 5.5 = 10
                            if (IntHalf.Value == 0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        }
                        else if (rValue.DecimalHalf.Value > DecimalHalf.Value) {
                            --IntHalf;
                            if (signBeforeOp != IntHalf.Sign)//5.4 - 5.7 = -0.3
                                DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                            else//5.4 - 3.6 = 1.8
                                DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                        }
                        else if (signBeforeOp != IntHalf.Sign)//5.3 - 7.2 = -1.9
                            DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
                        else//5.4 - 5.3 = 0.1 
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - -5.5 = 11
							++IntHalf;
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){//5.4 - -5.7 = 11.1
							++IntHalf;
							DecimalHalf.Value = DecimalHalf.Value + rValue.DecimalHalf.Value - DecimalOverflow;
						} else//5.4 - -5.3 = 10.7 
							DecimalHalf.Value += rValue.DecimalHalf.Value;
					}
                }
			}
		}

public:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedAddOp(const MediumDecV2Base& rValue){ UnsignedAddOpV1(rValue); }

	    /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOp(const MediumDecV2Base& rValue){ AddOpV1(rValue); }

	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedSubOp(const MediumDecV2Base& rValue){ UnsignedSubOpV1(rValue); }
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const MediumDecV2Base& rValue){ SubOpV1(rValue); }

        //Basic addition operation
        MediumDecV2Base& UnsignedAddOperation(const MediumDecV2Base& rValue)
        { UnsignedAddOp(rValue); return *this; }
		
        //Basic addition operation
        MediumDecV2Base& AddOperation(const MediumDecV2Base& rValue)
        { AddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base AddByUnsigned(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.UnsignedAddOperation(rValue); } const

		/// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecV2Base AddBy(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.AddOperation(rValue); } const

        //Basic subtraction operation
        MediumDecV2Base& UnsignedSubOperation(const MediumDecV2Base& rValue)
        { UnsignedSubOp(rValue); return *this; }

        //Basic subtraction operation
        MediumDecV2Base& SubOperation(const MediumDecV2Base& rValue)
        { SubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base SubtractByUnsigned(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.UnsignedSubOperation(rValue); } const

		/// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecV2Base SubtractBy(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.SubOperation(rValue); } const

    #pragma endregion NormalRep AltNum Addition/Subtraction Operations

	#pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const MediumDecV2Base& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// += operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const MediumDecV2Base& rValue) { return lValue.AddOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }
		
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }
		
        friend MediumDecV2Base operator+(const signed int& lValue, MediumDecV2Base rValue)  { return rValue.IntAddOperation(lValue); }
        friend MediumDecV2Base operator+(const signed __int64& lValue, MediumDecV2Base rValue)  { return rValue.Int64AddOperation(lValue); }
        friend MediumDecV2Base operator+(const unsigned int& lValue, MediumDecV2Base rValue)  { return rValue.UIntAddOperation(lValue); }
        friend MediumDecV2Base operator+(const unsigned __int64& lValue, MediumDecV2Base rValue)  { return rValue.UInt64AddOperation(lValue); }
		
        friend MediumDecV2Base operator+(const signed char& lValue, MediumDecV2Base rValue)  { return rValue.Int8AddOperation(lValue); }
        friend MediumDecV2Base operator+(const signed short& lValue, MediumDecV2Base rValue)  { return rValue.Int16AddOperation(lValue); }
        friend MediumDecV2Base operator+(const unsigned char& lValue, MediumDecV2Base rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend MediumDecV2Base operator+(const unsigned short& lValue, MediumDecV2Base rValue)  { return rValue.UInt16AddOperation(lValue); }

	#pragma endregion Other addition operations

	#pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const MediumDecV2Base& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// -= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const MediumDecV2Base& rValue) { return lValue.SubOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }
		
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }
		
        friend MediumDecV2Base operator-(const signed int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const signed __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
		
        friend MediumDecV2Base operator-(const signed char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const signed short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }

	#pragma endregion Other subtraction operations

	#pragma region Modulus Operations
	#pragma endregion Modulus Operations

	#pragma region Bitwise Operations
    //Update code later
    /*
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=signed int>
        friend MediumDec operator^(MediumDec self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntHalf ^= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntHalf < 0;
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntHalf == NegativeRep)
                {
                    self.IntHalf = (0 & Value) * -1;
                    self.DecimalHalf ^= Value;
                }
                else
                {
                    self.IntHalf ^= Value; self.DecimalHalf ^= Value;
                }
            }
            return self;
        }

        /// <summary>
        /// Bitwise Or Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=signed int>
        friend MediumDec operator|(MediumDec self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntHalf |= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntHalf < 0;
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntHalf == NegativeRep)
                {
                    self.IntHalf = (0 & Value) * -1;
                    self.DecimalHalf |= Value;
                }
                else
                {
                    self.IntHalf |= Value; self.DecimalHalf |= Value;
                }
            }
            return self;
        }
    #endif
    */
	#pragma endregion Bitwise Operations

    /*
    #pragma region Floating Operator Overrides
    
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const float& Value) { return self + (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const float& Value) { return self - (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const float& Value) { return self * (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const float& Value) { return self / (MediumDecV2Base)Value; }

        friend MediumDecV2Base operator+(const float& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value + self; }
        friend MediumDecV2Base operator-(const float& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value - self; }
        friend MediumDecV2Base operator*(const float& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value * self; }
        friend MediumDecV2Base operator/(const float& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value / self; }

        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const double& Value) { return self + (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const double& Value) { return self - (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const double& Value) { return self * (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const double& Value) { return self / (MediumDecV2Base)Value; }

        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const long double& Value) { return self + (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const long double& Value) { return self - (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const long double& Value) { return self * (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const long double& Value) { return self / (MediumDecV2Base)Value; }

        friend MediumDecV2Base operator+(const long double& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value + self; }
        friend MediumDecV2Base operator-(const long double& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value - self; }
        friend MediumDecV2Base operator*(const long double& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value * self; }
        friend MediumDecV2Base operator/(const long double& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value / self; }

    #pragma endregion Floating Operator Overrides
    */


    #pragma region Other Operators

        /// <summary>
        /// ++MediumDecV2Base Operator
        /// </summary>
        /// <returns>MediumDecV2Base &</returns>
        MediumDecV2Base& operator ++()
        {
            if (DecimalHalf == 0)
                ++IntHalf;
            else if (IntHalf == MirroredInt::NegativeZero)
                IntHalf = MirroredInt::Zero;
            else
                ++IntHalf;
            return *this;
        }

        /// <summary>
        /// ++MediumDecV2Base Operator
        /// </summary>
        /// <returns>MediumDecV2Base &</returns>
        MediumDecV2Base& operator --()
        {
            if (DecimalHalf == 0)
                --IntHalf;
            else if (IntHalf == MirroredInt::Zero)
                IntHalf = MirroredInt::NegativeZero;
            else
                --IntHalf;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        MediumDecV2Base operator ++(int)
        {
            MediumDecV2Base tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        MediumDecV2Base operator --(int)
        {
            MediumDecV2Base tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDecV2Base &</returns>
        MediumDecV2Base& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	#pragma region Truncation Functions

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
        void ApplyAbs(){ IntHalf.ApplyAbs(); }

protected:

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
        template<MediumDecVariant VariantType = MediumDecV2Base>
        VariantType AbsOfV1() {
            VariantType result = *this; result.ApplyAbs();
            return result;
        }

public:

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
        MediumDecV2Base AbsOf() { return AbsOfV1(); }

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
        static MediumDecV2Base Abs(const MediumDecV2Base& tValue);



        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        void ApplyFloorOf(const int& precision = 0);

protected:

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType CeilOfV1() const
        {
            if (DecimalHalf != 0)
				return VariantType(IntHalf+1);
            else
				return *this;
        }
		
        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>VariantType&</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType CeilV1(const VariantType& tValue)
        {
			if(tValue.IntHalf==MirroredInt::NegativeZero)
				return VariantType::One;
            else if (tValue.DecimalHalf != 0)
				return VariantType(tValue.IntHalf+1);
            else
				return tValue;
        }
		
        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        template<MediumDecVariant VariantType = MediumDecV2Base>
        static VariantType FloorV1(const VariantType& tValue, const int& precision = 0)
        {
			unsigned int decimalRes = tValue.DecimalHalf.Value;
            switch (precision)
            {
            case 8: decimalRes /= 10; decimalRes *= 10; break;
            case 7: decimalRes /= 100; decimalRes *= 100; break;
            case 6: decimalRes /= 1000; decimalRes *= 1000; break;
            case 5: decimalRes /= 10000; decimalRes *= 10000; break;
            case 4: decimalRes /= 100000; decimalRes *= 100000; break;
            case 3: decimalRes /= 1000000; decimalRes *= 1000000; break;
            case 2: decimalRes /= 10000000; decimalRes *= 10000000; break;
            case 1: decimalRes /= 100000000; decimalRes *= 100000000; break;
            default: decimalRes = 0; break;
            }
			if(decimalRes==0&&tValue.IntHalf==MirroredInt::NegativeZero)
				return VariantType();
			else
				return VariantType(tValue.IntHalf, PartialInt(decimalRes,tValue.DecimalHalf.Flags));
        }
		
        template<MediumDecVariant VariantType = MediumDecV2Base>
        VariantType TruncOfV1() const
        {
            return VariantType(IntHalf == NegativeRep?0:IntHalf, 0);
        }
public:

		MediumDecV2Base CeilOf() { return CeilOfV1(); }
		
		static MediumDecV2Base Ceil(const MediumDecV2Base& tValue) { return CeilV1(tValue); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
        signed int FloorIntOf() const;

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
		static signed int FloorInt(const MediumDecV2Base& tValue) { return tValue.FloorIntOf(); }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
        int CeilIntOf() const;

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecV2Base&</returns>
		static signed int CeilInt(const MediumDecV2Base& tValue) { return tValue.CeilIntOf(); }

		static MediumDecV2Base Trunc(const MediumDecV2Base& tValue) { return tValue.TruncOfV1(); }

	#pragma endregion Truncation Functions

	#pragma region Pow and Sqrt Functions
protected:

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        template<MediumDecVariant VariantType = MediumDecV2Base>
        VariantType SqrtOfV1(const int& precision=7)
        { 
            if(IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
            else if (DecimalHalf == 0)
            {
                VariantType value = *this;
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

            VariantType number = *this;
            VariantType start = VariantType(), end = number;
            VariantType mid;

            // variable to store the answer 
            VariantType ans;

            // for computing integral part 
            // of square root of number 
            while (start <= end) {
                mid = (start + end);
                mid.DivideByTwo();
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
            VariantType increment = VariantType(0,100000000);//0.1
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

public:

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    MediumDecV2Base SqrtOf(const int& precision = 7) {
        return SqrtOfV1(precision);
    }

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    static auto Sqrt(const auto& value, const int& precision = 7)
    {
        return value.SqrtOf(precision);
    }

protected:
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=unsigned int>
        void UIntPowOfOperationV1(const IntType& expValue)
        {
            if (expValue == 1) { return; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&(expValue&1)==1)
                    IntHalf.Sign = MirroredInt::PositiveSign;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
				IntType exp = expValue;
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:(exp&1)==1?false:true;
                VariantType self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (exp > 0)
                {
                    // If expValue is odd, multiply self with result
                    if ((exp&1) == 1)
                        UnsignedMultOp(self);
                    // n must be even now
                    exp = exp >> 1; // y = y/2
                    self.UnsignedMultOp(self); // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.Sign = MirroredInt::NegativeSign;
            }
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=signed int>
        void IntPowOfOperationV1(const IntType& expValue)
        {
            if (expValue == 1) { return; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (expValue < 0)//Negative Pow
            {
                IntType exp = expValue * -1;
                if (DecimalHalf == 0 && IntHalf == 10 && expValue >= -9)
                {
                    IntHalf = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[exp];
                    if(IsNegative()&&(exp&1)==1)
                        IntHalf.Sign = MirroredInt::PositiveSign;
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    //Switches from negative to positive if exp is odd number
                    bool IsNegative = IsPositive()?false:(exp&1)==1?false:true;
                    VariantType self = AbsOf();//Prevent needing to flip the sign
                    IntHalf = 1; DecimalHalf = 0;// Initialize result
                    while (exp > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if ((exp & 1) == 1)
                            UnsignedDivOp(self);
                        // n must be even now
                        exp = exp >> 1; // y = y/2
                        self.UnsignedMultOp(self); //  Change x to x^2
                    }
                    if(IsNegative)
                        IntHalf.Sign = MirroredInt::NegativeSign;
                }
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&(expValue&1)==1)
                    IntHalf.Sign = MirroredInt::PositiveSign;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                //Switches from negative to positive if exp is odd number
				IntType exp = expValue;
                bool IsNegative = IsPositive()?false:(exp&1)==1?false:true;
                VariantType self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (exp > 0)
                {
                    // If expValue is odd, multiply self with result
                    if ((exp & 1) == 1)
                        UnsignedMultOp(self);
                    // n must be even now
                    exp = exp >> 1; // y = y/2
                    self.UnsignedMultOp(self); // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.Sign = MirroredInt::NegativeSign;
            }
        }

		template<MediumDecVariant VariantType=MediumDecV2Base>
		VariantType UnsignedNegIntPowerV1(const unsigned int& expValue)
		{
			unsigned int exp = expValue;
			//Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
			//Switches from negative to positive if exp is odd number
			bool IsNegative = IsPositive()?false:(exp&1)==1?false:true;
            VariantType self = AbsOf();
			IntHalf = 1; DecimalHalf = 0;// Initialize result
			while (exp > 0)
			{
				// If expValue is odd, divide self with result
				if ((exp & 1) == 1)
                    UnsignedDivOp(self);
				// n must be even now
				exp = exp >> 1; // y = y/2
                self.UnsignedMultOp(self); // Change x to x^2
			}
			if(IsNegative)
				IntHalf.Sign = 0;
		}

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// (Doesn't modify owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<MediumDecVariant VariantType = MediumDecV2Base, IntegerType IntType = signed int>
        MediumDecV2Base UIntPowOfV1(const signed int& expValue) const
        {
            VariantType result = *this; result.UIntPowOfOperationV1(expValue);
            return result;
        }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// (Doesn't modify owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<MediumDecVariant VariantType = MediumDecV2Base, IntegerType IntType = signed int>
        MediumDecV2Base IntPowOfV1(const signed int& expValue) const
        {
            VariantType result = *this; result.IntPowOfOperationV1(expValue);
            return result;
        }

public:

        MediumDecV2Base UnsignedNegIntPower(const unsigned int& expValue)
		{ return UnsignedNegIntPowerV1(expValue); }

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
		/// (Modifies owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecV2Base UIntPowOfOp(const unsigned int& expValue)
		{ UIntPowOfOperationV1(expValue); return *this; }
        MediumDecV2Base UInt64PowOfOp(const unsigned __int64& expValue)
		{ IntPowOfOperationV1(expValue); return *this; }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
		/// (Modifies owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecV2Base IntPowOfOp(const signed int& expValue)
		{ IntPowOfOperationV1(expValue); return *this; }
        MediumDecV2Base Int64PowOfOp(const signed __int64& expValue)
		{ IntPowOfOperationV1(expValue); return *this; }
		
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
		/// (Doesn't modify owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecV2Base UIntPowOf(const unsigned int& expValue) const
        { return UIntPowOfV1(expValue); }
        MediumDecV2Base UInt64PowOf(const unsigned __int64& expValue) const
        { return UIntPowOfV1(expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
		/// (Doesn't modify owner object)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecV2Base IntPowOf(const signed int& expValue) const
		{ return IntPowOfV1(expValue); }
        MediumDecV2Base Int64PowOf(const signed __int64& expValue) const
		{ return IntPowOfV1(expValue); }

protected:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType NthRootOfV1(const unsigned int& n, const VariantType& precision = VariantType::JustAboveZero)
        {
            if (IsNegative())
                throw "Nth root of a negative number requires imaginary number support";
            VariantType xPre = ((*this - 1) / n) + 1;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two
            // roots by INT_MAX
            VariantType delX = VariantType(2147483647);

            //  xK denotes current value of x
            VariantType xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                
                xK = (xPre * nMinus1 + DivideByUnsigned(xPre.UIntPowOfV1(nMinus1))) / n;
                delX = VariantType::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }
		
        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType NthRootOfV2(const unsigned int& n, const auto& Precision = FiveBillionth) const
        {
			if(n==0)
				throw "Can't return results of zeroth root";//Negative roots require imaginary numbers to support
            unsigned int nMinus1 = n - 1;
			VariantType OneByN = VariantType::One.DivideByUInt(n);
            VariantType x[2] = { OneByN *VariantType::MultiplyByUInt(nMinus1)+DivideByUnsigned(UIntPowOf(nMinus1)), *this };
            while (Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = OneByN * ((x[1].MultiplyByUInt(nMinus1)) + DivideBy(x[1].UIntPowOf(nMinus1)));
            }
            return x[0];
        }
		
public:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
        MediumDecV2Base NthRootOf(const unsigned int& n, const MediumDecV2Base& precision = MediumDecV2Base::JustAboveZero){ return NthRootOfV1(n, precision); }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType NthRootV2(const VariantType& targetValue, const unsigned int& n, const VariantType& Precision = VariantType::FiveBillionth)
        { return targetValue.NthRootOfV1(n, Precision); }

protected:

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType FractionalPowV1(const auto& value, const boost::rational<unsigned int>& Frac)
		{
			VariantType targetVal = UIntPowOf(Frac.numerator());
			VariantType CalcVal = MediumDecVariant::NthRoot(targetVal, Frac.denominator());
			return CalcVal;
		}

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType FractionalPowV2(const VariantType& value, const signed int& expNum, const unsigned int& expDenom)
		{
			auto targetVal = IntPowOf(expNum);
			auto CalcVal = MediumDecVariant::NthRoot(targetVal, expDenom);
			return CalcVal;
		}

        /// <summary>
        /// Calculate to power of unsigned expValue
		/// (Doesn't modify owner object)
        /// </summary>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType UnsignedPowOfV1(const auto& expValue)
		{
			boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
			if(expValue.IntHalf.Value==0)
				return FractionalPowV1(Frac);
			else {
				VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
				CalcVal *= FractionalPowV1(Frac);
				return CalcVal;
			}
		}

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType PowOfV1(const auto& expValue)
		{
			boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
			if (expValue.IntHalf.IsNegative()){//Negative Exponent
				if(expValue.IntHalf.Value==0)
					return VariantType::One/FractionalPowV1(Frac);
				else {
					VariantType CalcVal = One / UIntPowOf(expValue.IntHalf.Value);
					CalcVal /= FractionalPowV1(Frac);
					return CalcVal;
				}
			} else {
				if(expValue.IntHalf.Value==0)
					return FractionalPowV1(Frac);
				else {
					VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
					CalcVal *= FractionalPowV1(Frac);
					return CalcVal;
				}
			}
		}

public:

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
        MediumDecV2Base UnsignedPowOf(const auto& expValue)
		{ return UnsignedPowOfV1(expValue); }

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
        MediumDecV2Base PowOf(const auto& expValue)
		{ return PowOfV1(expValue); }

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:	
	
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>VariantType</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType ExpV1(const VariantType& x)
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
             *      - approximation of e^x in VariantType precision
             */
             // Check that x is a valid input.
            assert(x.IntHalf.Value < 709);
            // When x = 0 we already know e^x = 1.
            if (x.IsZero()) {
                return VariantType::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            VariantType x0 = VariantType::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            VariantType tn = VariantType::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = VariantType::CeilInt(x0 * VariantType::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + VariantType::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x.IsNegative()) {
                tn = VariantType::One / tn;
            }
            return tn;
        }
		
		//Common log calculations for when value is between 0 and one
		template<MediumDecVariant VariantType=MediumDecV2Base>
		VariantType LogZeroRangePart02(const VariantType& AccuracyLevel=VariantType::JustAboveZero) const
		{
			VariantType TotalRes = (*this - 1)/ (*this + 1);
			VariantType WSquared = TotalRes * TotalRes;
			VariantType LastPow = -TotalRes;
			int WPow = 3;
			VariantType AddRes;

			do
			{
				LastPow *= WSquared;
				AddRes = LastPow / WPow;
				TotalRes -= AddRes;
				WPow += 2;
			} while (AddRes > VariantType::JustAboveZero);
			return TotalRes;
		}
		
		//Common natural log calculations for range one to two
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType LnOfOneSection(const VariantType& threshold = VariantType::FiveBillionth) const
        {
            VariantType base = *this - 1;        // Base of the numerator; exponent will be explicit
            bool posSign = true;             // Used to swap the sign of each term
            VariantType term = base;       // First term
            VariantType prev;          // Previous sum
            VariantType result = term;     // Kick it off
            // den = Denominator of the nth term
            for(unsigned int den = 2;VariantType::Abs(prev - result) > threshold;++den){
                posSign = !posSign;
                term *= base;
                prev = result;
                if (posSign)
                    result += term / den;
                else
                    result -= term / den;
            }
            return result;
        }
		
		//Common log calculations for when value is greater than one
		template<MediumDecVariant VariantType=MediumDecV2Base>
		VariantType LogGreaterRangePart02(const VariantType& AccuracyLevel=VariantType::JustAboveZero) const
		{
			//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
			VariantType TotalRes = (*this - 1) / (*this + 1);
			VariantType LastPow = TotalRes;
			VariantType WSquared = TotalRes * TotalRes;
			VariantType AddRes;
			int WPow = 3;
			do
			{
				LastPow *= WSquared;
				AddRes = LastPow / WPow;
				TotalRes += AddRes; WPow += 2;
			} while (AddRes > AccuracyLevel);
			return TotalRes;
		}
		
		template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType = unsigned int>
		static VariantType LogGreaterRangeIntPart02(const IntType& value, const VariantType& AccuracyLevel=VariantType::JustAboveZero)
		{
			VariantType tValue = VariantType(value);
			return tValue.LogGreaterRangePart02(AccuracyLevel);
		}
		
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumDec variant</returns>
        template<MediumDecVariant VariantType = MediumDecV2Base>
        #if defined(AltNum_UseCustomLnAccuracy)
        VariantType LnOfV1(const VariantType& threshold = VariantType::FiveMillionth) const
        #else
        VariantType LnOfV1() const
        #endif
        {//Negative values for natural log return value of LnV1(-value) * i
            //if (value <= 0) {}else//Error if equal or less than 0
            if (IsOne())
                return VariantType::Zero;
            if (IntHalf == MirroredInt::Zero)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogZeroRangePart02(threshold).MultipliedByTwo();
                #else
                return LogZeroRangePart02().MultipliedByTwo();
                #endif
            }
            else if (IntHalf == MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                #if defined(AltNum_UseCustomLnAccuracy)
                return LnOfOneSection(threshold);
                #else
                return LnOfOneSection();
                #endif
            }
            else
            {//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogGreaterRangePart02(threshold).MultipliedByTwo();
                #else
                return LogGreaterRangePart02().MultipliedByTwo();
                #endif
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumDec variant</returns>
        template<MediumDecVariant VariantType = MediumDecV2Base>
        static VariantType LnV1(const VariantType& value)
        {
            return value.LnOfV1();
        }
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<MediumDecVariant VariantType = MediumDecV2Base>
        #if defined(AltNum_UseCustomLnAccuracy)
        VariantType Log10OfV1(const VariantType& threshold = VariantType::FiveMillionth) const
        #else
        VariantType Log10OfV1() const
        #endif
        {
            if (IsOne())
                return VariantType::Zero;
            #if !defined(AltNum_PreventLog10IntegerLoop)
            if (DecimalHalf == 0 && IntHalf.Value % 10 == 0)
            {//Might not be worth using checking to use this alternative code since since 10s aren't that common
                for (int index = 1; index < 9; ++index)
                {
                    if (IntHalf.Value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return VariantType(index, 0);
                }
                return VariantType(9, 0);
            }
            #endif
            const VariantType lnMultiplier = VariantType(0, LN10Div_DecSection);
            if (IntHalf == MirroredInt::Zero)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogZeroRangePart02(threshold).MultiplyByUnsigned(lnMultiplier);
                #else
                return LogZeroRangePart02().MultiplyByUnsigned(lnMultiplier);
                #endif
            }
            else if (IntHalf == MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                #if defined(AltNum_UseCustomLnAccuracy)
                return LnOfOneSection(threshold).MultiplyByUnsigned(lnMultiplier);
                #else
                return LnOfOneSection().MultiplyByUnsigned(lnMultiplier);
                #endif
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogGreaterRangePart02(threshold).MultiplyByUnsigned(lnMultiplier);
                #else
                return LogGreaterRangePart02().MultiplyByUnsigned(lnMultiplier);
                #endif
            }
        }

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<MediumDecVariant VariantType = MediumDecV2Base>
        static VariantType Log10V1(const VariantType& value)
        {
            return value.Log10OfV1();
        }
		
        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType = unsigned int>
        static VariantType Log10OfIntV1(const IntType& value)
        {
            if (value == 1)
                return VariantType::Zero;
            if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return VariantType(index);
                }
                return VariantType(9);
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                VariantType lnMultiplier = VariantType(0, TwiceLN10Div_DecSection);
                return LogGreaterRangeIntPart02(value).MultiplyByUnsigned(lnMultiplier);
            }
        }
		
        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDec Variant</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType LogV1(const VariantType& value, const VariantType& baseVal)
        {
            if (value == VariantType::One)
                return VariantType::Zero;
            return Log10V1(value) / Log10V1(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>VariantType</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType = unsigned int>
        static VariantType LogOfIntV1(const VariantType& value, const IntType& baseVal)
        {
            //Calculate Base log first
            VariantType baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = VariantType(index, 0);
                        break;
                    }
                }
                baseTotalRes = VariantType(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = LogGreaterRangeIntPart02(baseVal);
            }
            VariantType lnMultiplier = VariantType(0, TwiceLN10Div_DecSection);
            //Now calculate other log
            if (value.DecimalHalf == 0 && value.IntHalf.Value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? VariantType(index, 0) / (baseTotalRes * lnMultiplier): VariantType(index, 0)/ baseTotalRes;
                }
                return lnMultLog? VariantType(9, 0) / (baseTotalRes.MultiplyByUnsigned(lnMultiplier)):VariantType(9, 0)/baseTotalRes;
            }
			if(value.IntHalf==MirroredInt::Zero)//Not tested this block but should work
			{
                VariantType TotalRes = value.LogZeroRangePart02();
				if(lnMultLog)
					return TotalRes.DivideByUnsigned(baseTotalRes);
				else
					return (TotalRes.MultiplyByUnsigned(lnMultiplier)).DivideByUnsigned(baseTotalRes);
			}
            else if (value.IntHalf==MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
				if(lnMultLog)
					return value.LnOfOneSection()/baseTotalRes;
				else
					return (value.LnOfOneSection().MultipliedByTwo())/ baseTotalRes;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                VariantType TotalRes = value.LogGreaterRangePart02();
				if(lnMultLog)
					return TotalRes.DivideByUnsigned(baseTotalRes);
				else
					return (TotalRes.MultiplyByUnsigned(lnMultiplier)).DivideByUnsigned(baseTotalRes);
            }
        }
		
public:

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Exp(const MediumDecV2Base& x) { return ExpV1(x); }
		
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Ln(const MediumDecV2Base& value)
        { return LnV1(value); }

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecV2Base Log10(const MediumDecV2Base& value)
        { return Log10V1(value); }

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecV2Base Log10OfInt(const unsigned int& value)
        { return Log10OfIntV1(value); }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDec Variant</returns>
        static MediumDecV2Base Log(const MediumDecV2Base& value, const MediumDecV2Base& baseVal)
        { return LogV1(value, baseVal); }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base LogOfInt(const MediumDecV2Base& value, const unsigned int& baseVal)
        { return LogOfIntV1(value, baseVal); }

	#pragma endregion Log Functions

    #pragma region Trigonomic Functions
protected:

        /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecV2Base</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType SinOfV1() const
        {
            VariantType SinValue = VariantType::One  / VariableConversionFunctions::Fact(1);
			unsigned int expTotal;
			bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
				expTotal = 2 * i + 1;
				if(AddToResult)
					SinValue += UIntPowOf(expTotal) / VariableConversionFunctions::Fact(expTotal);
				else
					SinValue -= UIntPowOf(expTotal) / VariableConversionFunctions::Fact(expTotal);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Cos from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecV2Base</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType CosOfV1() const
        {
            VariantType CosValue = VariantType::One / VariableConversionFunctions::Fact(0);
			unsigned int expTotal;
			bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
				expTotal = 2 * i;
				if(AddToResult)
					CosValue += UIntPowOf(expTotal) / VariableConversionFunctions::Fact(expTotal);
				else
					CosValue -= UIntPowOf(expTotal) / VariableConversionFunctions::Fact(expTotal);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecV2Base</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType TanOfV1() const
        {
            VariantType SinValue = VariantType::One  / VariableConversionFunctions::Fact(1);
            VariantType CosValue = VariantType::One / VariableConversionFunctions::Fact(0);
			unsigned int sinExp; unsigned int cosExp;
			bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
				sinExp = 2 * i + 1; cosExp = 2 * i;
				if(AddToResult){
					SinValue += UIntPowOf(sinExp) / VariableConversionFunctions::Fact(sinExp);
					CosValue += UIntPowOf(cosExp) / VariableConversionFunctions::Fact(cosExp);
				} else{
					SinValue -= UIntPowOf(sinExp) / VariableConversionFunctions::Fact(sinExp);
					CosValue -= UIntPowOf(cosExp) / VariableConversionFunctions::Fact(cosExp);
				}
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecV2Base</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        VariantType ATanOfV1() const
        {
            VariantType SinValue = One  / VariableConversionFunctions::Fact(1);
            VariantType CosValue = One / VariableConversionFunctions::Fact(0);
            //Angle as Radian
			unsigned int sinExp; unsigned int cosExp;
			bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
				sinExp = 2 * i + 1; cosExp = 2 * i;
				if(AddToResult){
					SinValue += UIntPowOf(sinExp) / VariableConversionFunctions::Fact(sinExp);
					CosValue += UIntPowOf(cosExp) / VariableConversionFunctions::Fact(cosExp);
				} else{
					SinValue -= UIntPowOf(sinExp) / VariableConversionFunctions::Fact(sinExp);
					CosValue -= UIntPowOf(cosExp) / VariableConversionFunctions::Fact(cosExp);
				}
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
        /// <returns>MediumDec</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType ArcTan2V1(const VariantType& y, const VariantType& x)
        {
            VariantType coeff_1 = PiNum.DividedByFour();
            VariantType coeff_2 = coeff_1.MultiplyByUInt(3);
            VariantType abs_y = VariantType::Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            VariantType r;
            VariantType angle;
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
        /// <param name="value">The target VariantType variant value to perform function on.</param>
        /// <returns>VariantTypeBase</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType SinFromAngleV1(VariantType Value)
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
                    Value.IntHalf.Value = 360 - Value.IntHalf.Value;
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
                        return NegativePointFive;
                    default:
                        //Angle as Radian
                        VariantType Radius = Pi * Value / 180;
                        return Sin(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = Pi * Value / 180;
                return Sin(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target VariantType variant value to perform function on.</param>
        /// <returns></returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType CosFromAngleV1(VariantType Value)
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
                    Value.IntHalf.Value = 360 - Value.IntHalf.Value;
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
                        return VariantType::One;
                        break;
                    case 60:
                        return VariantType::PointFive;
                        break;
                    case 90://0.5 Pi Radians
                    case 270://1.5 Pi Radians
                        return VariantType::Zero;
                        break;
                    case 180://Pi Radians
                        return VariantType::NegativeOne;
                        break;
                    case 120:
                    case 240:
                        return VariantType::NegativePointFive;
                    default:
                        //Angle as Radian
                        VariantType Radius = Pi * Value / 180;
                        return Cos(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = Pi * Value / 180;
                return Cos(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target VariantType variant value to perform function on.</param>
        /// <returns>VariantTypeBase</returns>
		template<MediumDecVariant VariantType=MediumDecV2Base>
        static VariantType TanFromAngleV1(VariantType Value)
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
                    Value.IntHalf.Value = 360 - Value.IntHalf.Value;
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

public:

        /// <summary>
        /// Get Sin from value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target VariantType variant tValue to perform function on.</param>
        /// <returns>VariantTypeBase</returns>
        static MediumDecV2Base SinFromAngle(const MediumDecV2Base& tValue)
        { return SinFromAngleV1(tValue); }
		
        /// <summary>
        /// Get Cos() from value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target VariantType variant tValue to perform function on.</param>
        /// <returns></returns>
        static MediumDecV2Base CosFromAngle(const MediumDecV2Base& tValue)
        { return CosFromAngleV1(tValue); }

        /// <summary>
        /// Get Tangent from value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target VariantType variant tValue to perform function on.</param>
        /// <returns>VariantTypeBase</returns>
        static MediumDecV2Base TanFromAngle(const MediumDecV2Base& tValue)
        { return TanFromAngleV1(tValue); }
		
		MediumDecV2Base SinOf()
        { return SinOfV1(); }
		
		MediumDecV2Base CosOf()
        { return CosOfV1(); }
		
		MediumDecV2Base TanOf()
        { return TanOfV1(); }
		
		MediumDecV2Base ATanOf()
        { return ATanOfV1(); }
		
        /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Sin(const MediumDecV2Base& tValue)
        { return tValue.CosOfV1(); }
		
        /// <summary>
        /// Get cosine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target VariantType variant value to perform function on.</param>
        /// <returns></returns>
        static MediumDecV2Base Cos(const MediumDecV2Base& tValue)
        { return tValue.CosOfV1(); }
		
        /// <summary>
        /// Get Tan from value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Tan(const MediumDecV2Base& tValue)
        { return tValue.TanOfV1(); }
		
        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base ATan(const MediumDecV2Base& tValue)
        { return tValue.ATanOfV1(); }
		
        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        /// <returns>MediumDec</returns>
        static MediumDecV2Base ArcTan2(const MediumDecV2Base& y, const MediumDecV2Base& x)
        { return ArcTan2V1(y, x); }

    #pragma endregion Trigonomic Functions
    };
	
}