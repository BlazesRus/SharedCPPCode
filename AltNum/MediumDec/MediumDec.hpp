// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecBase.hpp"
#include "..\AltNumModResult.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumDec;

	/// <summary>
	/// long double (Extended precision double)
	/// </summary>
	using ldouble = long double;

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy to 9th digit
    /// Represents +- 2147483647.999999999 with 100% accuracy 
    /// except for truncation during division and multiplication after 9th digit
	/// </summary>
    class DLL_API MediumDec : public MediumDecBase
    {
public:
#pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDec(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDec(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero);

        MediumDec& operator=(const MediumDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumDec& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const MediumDecBase& rhs);

        MediumDec& operator=(const MediumDecBase& rhs)
        {
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<MediumDecVariant VariantType>
        MediumDec operator[](VariantType variantValue) const
        {
            MediumDec newSelf = MediumDec(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

#pragma endregion class_constructors

		//Performs remainder/Mod operation then saves division result
		class DLL_API ModResult : public AltNumModResult<MediumDecBase>{};

        void SetValue(MediumDec Value);

        template<MediumDecVariant VariantType=MediumDecBase>
        void SetValueFromVariant(VariantType Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf;
        }

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecBase</returns>
        MediumDec operator -() const
        {
			MediumDec self = *this;
            self.SwapNegativeStatus(); return self;
        }

    #pragma region ValueDefines
    private://Each class needs to define it's own
        
        static MediumDec AlmostOneValue();

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec PiNumValue();

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec HundredMilPiNumValue();

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec TenMilPiNumValue();

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec OneMilPiNumValue();

        //10xPi(Rounded to 9th decimal digit)
        static MediumDec TenPiNumValue();
        
        static MediumDec ENumValue();
        
        static MediumDec ZeroValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec OneValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec TwoValue();

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec NegativeOneValue();

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Point5Value();

        static MediumDec JustAboveZeroValue();

        static MediumDec OneMillionthValue();

        static MediumDec FiveThousandthValue();

        static MediumDec FiveMillionthValue();

        static MediumDec TenMillionthValue();

        static MediumDec OneHundredMillionthValue();

        static MediumDec FiveBillionthValue();

        static MediumDec LN10Value();

        static MediumDec LN10DivValue();

        static MediumDec TwiceLN10DivValue();

        static MediumDec MinimumValue();

        static MediumDec MaximumValue();

        static MediumDec NegativePointFiveValue();

public:
        static MediumDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec ENum;
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec OneGMillionth;

        //0e-7
        static MediumDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDec FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDec Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDec Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDec LN10Div;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDec TwiceLN10Div;

        static MediumDec NegativePointFive;

    #pragma endregion ValueDefines

    #pragma region String Commands

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        MediumDec GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDec(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const std::string& Value)
        {
            this->ReadString(Value);
        }

public:

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToString(); }

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

/*
        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const float& Value){ this->SetFloatVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const double& Value){ this->SetDoubleVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const long double& Value){ this->SetDecimalVal(Value); }
*/

        MediumDec(const unsigned __int64& Value){ this->SetUIntVal(Value); }
        MediumDec(const signed __int64& Value){ this->SetIntVal(Value); }
        MediumDec(const unsigned char& Value){ this->SetUIntVal(Value); }
        MediumDec(const signed char& Value){ this->SetIntVal(Value); }
        MediumDec(const unsigned short& Value){ this->SetUIntVal(Value); }
        MediumDec(const signed short& Value){ this->SetIntVal(Value); }
        MediumDec(const unsigned int& Value){ this->SetUIntVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const bool& Value){ this->SetBoolVal(Value); }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

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
public:

		std::strong_ordering operator<=>(const MediumDec& that) const
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

		bool operator==(const MediumDec& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.DecimalHalf)
				return false;
            return true;
		}

		bool operator!=(const MediumDec& that) const
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

        template<IntegerType IntType=unsigned int>
        MediumDec& UIntDivOperationV1(const IntType& Value)
        { UIntDivOpV1(Value); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDec& IntDivOperationV1(const IntType& Value)
        { IntDivOpV1(Value); return *this; }

protected:

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto DivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntDivOperationV1(rValue); }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto DivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntDivOperationV1(rValue); }

public:

        MediumDec& UIntDivOperation(const unsigned int& rValue) { return UIntDivOperationV1(rValue); }
        MediumDec& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
        MediumDec& UInt64DivOperation(const unsigned __int64& rValue) { return UIntDivOperationV1(rValue); }
        MediumDec& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
        MediumDec& UInt8DivOperation(const unsigned char& rValue) { return UIntDivOperationV1(rValue); }
        MediumDec& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
        MediumDec& UInt16DivOperation(const unsigned short& rValue) { return UIntDivOperationV1(rValue); }
        MediumDec& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

        MediumDec DivideByUInt(const unsigned int& rValue) { return DivideByUIntV1(rValue); }
        MediumDec DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        MediumDec DivideByUInt64(const unsigned __int64& rValue) { return DivideByUIntV1(rValue); }
        MediumDec DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        MediumDec UnsignedDivideByInt(const signed int& rValue) { return DivideByUIntV1(rValue); }
        MediumDec UnsignedDivideByInt64(const signed __int64& rValue) { return DivideByUIntV1(rValue); }

        MediumDec DivideByUInt8(const unsigned char& rValue) { return DivideByUIntV1(rValue); }
        MediumDec DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
        MediumDec DivideByUInt16(const unsigned short& rValue) { return DivideByUIntV1(rValue); }
        MediumDec DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool UnsignedPartialDivOp(const MediumDec& rValue){ return UnsignedPartialDivOpV1(rValue); }
		
		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void UnsignedDivOp(const MediumDec& rValue){ UnsignedDivOpV1(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void DivOp(const MediumDec& rValue){ DivOpV1(rValue); }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec& UnsignedDivOperation(const MediumDec& rValue)
		{ UnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec& DivOperation(const MediumDec& rValue)
		{ DivOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec DivideByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.UnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto DivideBy(const MediumDec& rValue)
        { auto lValue = *this; return lValue.DivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& lValue, const MediumDec& rValue) { return lValue.DivOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDec& operator/=(MediumDec& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(MediumDec& lValue, const MediumDec& rValue) { return lValue.DivideBy(rValue); }
		
        /// <summary>
        /// Division operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(MediumDec lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDec operator/(MediumDec lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }
		
        friend MediumDec operator/(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const signed __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }

        friend MediumDec operator/(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
        friend MediumDec operator/(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }

	#pragma endregion Other Division Operations	

    #pragma region NormalRep Integer Multiplication Operations

        template<IntegerType IntType=unsigned int>
        MediumDec& UIntMultOperationV1(const IntType& Value)
        { UIntMultOpV1(Value); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDec& IntMultOperationV1(const IntType& Value)
        { IntMultOpV1(Value); return *this; }

protected:

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto MultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntMultOperationV1(rValue); }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto MultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntMultOperationV1(rValue); }

public:

        MediumDec& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
        MediumDec& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
        MediumDec& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
        MediumDec& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
        MediumDec& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
        MediumDec& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
        MediumDec& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
        MediumDec& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

        MediumDec MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDec MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
        MediumDec MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDec MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

        MediumDec UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDec UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

        MediumDec MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDec MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
        MediumDec MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDec MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        bool UnsignedMultOp(const MediumDec& rValue){ UnsignedMultOpV1(rValue); }
		
        void MultOp(const MediumDec& rValue){ MultOpV1(rValue); }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec& UnsignedMultOperation(const MediumDec& rValue)
        { UnsignedMultOp(rValue); return *this; }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec& MultOperation(const MediumDec& rValue)
		{ MultOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec MultiplyByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.UnsignedMultOperation(rValue); }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDec MultiplyBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.MultOperation(rValue); }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region Other multiplication operations

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& lValue, const MediumDec& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec& operator*=(MediumDec& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumDec& operator*=(MediumDec& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(MediumDec lValue, const MediumDec& rValue) { return lValue.MultOperation(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec operator*(MediumDec lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }
		
        friend MediumDec operator*(MediumDec lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }
		
        friend MediumDec operator*(signed int lValue, MediumDec rValue)  { return rValue.IntMultOperation(lValue); }
        friend MediumDec operator*(signed __int64 lValue, MediumDec& rValue)  { return rValue.Int64MultOperation(lValue); }
        friend MediumDec operator*(unsigned int lValue, MediumDec& rValue)  { return rValue.UIntMultOperation(lValue); }
        friend MediumDec operator*(unsigned __int64 lValue, MediumDec& rValue)  { return rValue.UInt64MultOperation(lValue); }
		
        friend MediumDec operator*(signed char lValue, MediumDec& rValue)  { return rValue.Int8MultOperation(lValue); }
        friend MediumDec operator*(signed short lValue, MediumDec& rValue)  { return rValue.Int16MultOperation(lValue); }
        friend MediumDec operator*(unsigned char lValue, MediumDec& rValue)  { return rValue.UInt8MultOperation(lValue); }
        friend MediumDec operator*(unsigned short lValue, MediumDec& rValue)  { return rValue.UInt16MultOperation(lValue); }

	#pragma endregion Other Multiplication Operations

	#pragma region NormalRep Integer Addition Operations
protected:

        template<IntegerType IntType=unsigned int>
        MediumDec& UIntAddOperationV1(const IntType& rValue)
        { UIntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        MediumDec& IntAddOperationV1(const IntType& rValue)
        { IntAddOpV1(rValue); return *this; }

		/// <summary>
        /// Basic addition operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        MediumDec AddByUIntV1(const IntType& rValue)
        { MediumDec self = *this; return self.UIntAddOperationV1(rValue); }

		/// <summary>
        /// Basic addition operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDec AddByIntV1(const IntType& rValue)
        { MediumDec self = *this; return self.IntAddOperationV1(rValue); }

public:

        MediumDec& UIntAddOperation(const unsigned int& rValue)
        { UIntAddOp(rValue); return *this; }

        MediumDec& IntAddOperation(const signed int& rValue) { return IntAddOperationV1(rValue); }
        MediumDec& UInt64AddOperation(const unsigned __int64& rValue) { return UIntAddOperationV1(rValue); }
        MediumDec& Int64AddOperation(const signed __int64& rValue) { return IntAddOperationV1(rValue); }
        MediumDec& UInt8AddOperation(const unsigned char& rValue) { return UIntAddOperationV1(rValue); }
        MediumDec& Int8AddOperation(const signed char& rValue) { return IntAddOperationV1(rValue); }
        MediumDec& UInt16AddOperation(const unsigned short& rValue) { return UIntAddOperationV1(rValue); }
        MediumDec& Int16AddOperation(const signed short& rValue) { return IntAddOperationV1(rValue); }

        MediumDec AddByUInt(const unsigned int& rValue) { return AddByUIntV1(rValue); }
        MediumDec AddByInt(const signed int& rValue) { return AddByIntV1(rValue); }
        MediumDec AddByUInt64(const unsigned __int64& rValue) { return AddByUIntV1(rValue); }
        MediumDec AddByInt64(const signed __int64& rValue) { return AddByIntV1(rValue); }

        MediumDec UnsignedAddByInt(const signed int& rValue) { return AddByUIntV1(rValue); }
        MediumDec UnsignedAddByInt64(const signed __int64& rValue) { return AddByUIntV1(rValue); }

        MediumDec AddByUInt8(const unsigned char& rValue) { return AddByUIntV1(rValue); }
        MediumDec AddByInt8(const signed char rValue) { return AddByIntV1(rValue); }
        MediumDec AddByUInt16(const unsigned short& rValue) { return AddByUIntV1(rValue); }
        MediumDec AddByInt16(const signed short& rValue) { return AddByIntV1(rValue); }

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
        MediumDec& UIntSubOperationV1(const IntType& rValue)
        { UIntSubOpV1(rValue); return *this; }

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDec& IntSubOperationV1(const IntType& rValue)
        { IntSubOpV1(rValue); return *this; }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        MediumDec SubtractByUIntV1(const IntType& rValue)
        { MediumDec self = *this; return self.UIntSubOperationV1(rValue); }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDec SubtractByIntV1(const IntType& rValue)
        { MediumDec self = *this; return self.IntSubOperationV1(rValue); }

public:

        MediumDec& UIntSubOperation(const unsigned int& rValue)
        { UIntSubOp(rValue); return *this; }

        MediumDec& IntSubOperation(const signed int& rValue) { return IntSubOperationV1(rValue); }
        MediumDec& UInt64SubOperation(const unsigned __int64& rValue) { return UIntSubOperationV1(rValue); }
        MediumDec& Int64SubOperation(const signed __int64& rValue) { return IntSubOperationV1(rValue); }
        MediumDec& UInt8SubOperation(const unsigned char& rValue) { return UIntSubOperationV1(rValue); }
        MediumDec& Int8SubOperation(const signed char& rValue) { return IntSubOperationV1(rValue); }
        MediumDec& UInt16SubOperation(const unsigned short& rValue) { return UIntSubOperationV1(rValue); }
        MediumDec& Int16SubOperation(const signed short& rValue) { return IntSubOperationV1(rValue); }

        MediumDec SubtractByUInt(const unsigned int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec SubtractByInt(const signed int& rValue) { return SubtractByIntV1(rValue); }
        MediumDec SubtractByUInt64(const unsigned __int64& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec SubtractByInt64(const signed __int64& rValue) { return SubtractByIntV1(rValue); }

        MediumDec UnsignedSubtractByInt(const signed int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec UnsignedSubtractByInt64(const signed __int64& rValue) { return SubtractByUIntV1(rValue); }

        MediumDec SubtractByUInt8(const unsigned char& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec SubtractByInt8(const signed char rValue) { return SubtractByIntV1(rValue); }
        MediumDec SubtractByUInt16(const unsigned short& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec SubtractByInt16(const signed short& rValue) { return SubtractByIntV1(rValue); }
   
	#pragma endregion NormalRep Integer Subtraction Operations
	
	#pragma region NormalRep AltNum Addition/Subtraction Operations

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedAddOp(const MediumDec& rValue){ UnsignedAddOpV1(rValue); }

	    /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOp(const MediumDec& rValue){ AddOpV1(rValue); }

	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedSubOp(const MediumDec& rValue){ UnsignedSubOpV1(rValue); }
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const MediumDec& rValue){ SubOpV1(rValue); }

        //Basic addition operation
        MediumDec& UnsignedAddOperation(const MediumDec& rValue)
        { UnsignedAddOp(rValue); return *this; }
		
        //Basic addition operation
        MediumDec& AddOperation(const MediumDec& rValue)
        { AddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec AddByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.UnsignedAddOperation(rValue); }

		/// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDec AddBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.AddOperation(rValue); }

        //Basic subtraction operation
        MediumDec& UnsignedSubOperation(const MediumDec& rValue)
        { UnsignedSubOp(rValue); return *this; }

        //Basic subtraction operation
        MediumDec& SubOperation(const MediumDec& rValue)
        { SubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec SubtractByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.UnsignedSubOperation(rValue); }

		/// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDec SubtractBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.SubOperation(rValue); }

	#pragma endregion NormalRep AltNum Addition/Subtraction Operations
	
	#pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& lValue, const MediumDec& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// += operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec& operator+=(MediumDec& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumDec& operator+=(MediumDec& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(MediumDec lValue, const MediumDec& rValue) { return lValue.AddOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec operator+(MediumDec lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }
		
        friend MediumDec operator+(MediumDec lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }
		
        friend MediumDec operator+(const signed int& lValue, MediumDec rValue)  { return rValue.IntAddOperation(lValue); }
        friend MediumDec operator+(const signed __int64& lValue, MediumDec rValue)  { return rValue.Int64AddOperation(lValue); }
        friend MediumDec operator+(const unsigned int& lValue, MediumDec rValue)  { return rValue.UIntAddOperation(lValue); }
        friend MediumDec operator+(const unsigned __int64& lValue, MediumDec rValue)  { return rValue.UInt64AddOperation(lValue); }
		
        friend MediumDec operator+(const signed char& lValue, MediumDec rValue)  { return rValue.Int8AddOperation(lValue); }
        friend MediumDec operator+(const signed short& lValue, MediumDec rValue)  { return rValue.Int16AddOperation(lValue); }
        friend MediumDec operator+(const unsigned char& lValue, MediumDec rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend MediumDec operator+(const unsigned short& lValue, MediumDec rValue)  { return rValue.UInt16AddOperation(lValue); }

	#pragma endregion Other addition operations

	#pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& lValue, const MediumDec& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// -= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumDec& operator-=(MediumDec& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec lValue, const MediumDec& rValue) { return lValue.SubOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }
		
        friend MediumDec operator-(MediumDec lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }
		
        friend MediumDec operator-(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const signed __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
		
        friend MediumDec operator-(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }

	#pragma endregion Other subtraction operations

/*
    #pragma region Floating Operator Overrides
    
        friend MediumDec operator+(const MediumDec& self, const float& Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(const MediumDec& self, const float& Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(const MediumDec& self, const float& Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(const MediumDec& self, const float& Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(const float& Value, const MediumDec& self) { return (MediumDec)Value + self; }
        friend MediumDec operator-(const float& Value, const MediumDec& self) { return (MediumDec)Value - self; }
        friend MediumDec operator*(const float& Value, const MediumDec& self) { return (MediumDec)Value * self; }
        friend MediumDec operator/(const float& Value, const MediumDec& self) { return (MediumDec)Value / self; }

        friend MediumDec operator+(const MediumDec& self, const double& Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(const MediumDec& self, const double& Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(const MediumDec& self, const double& Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(const MediumDec& self, const double& Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(const MediumDec& self, const long double& Value) { return self + (MediumDec)Value; }
        friend MediumDec operator-(const MediumDec& self, const long double& Value) { return self - (MediumDec)Value; }
        friend MediumDec operator*(const MediumDec& self, const long double& Value) { return self * (MediumDec)Value; }
        friend MediumDec operator/(const MediumDec& self, const long double& Value) { return self / (MediumDec)Value; }

        friend MediumDec operator+(const long double& Value, const MediumDec& self) { return (MediumDec)Value + self; }
        friend MediumDec operator-(const long double& Value, const MediumDec& self) { return (MediumDec)Value - self; }
        friend MediumDec operator*(const long double& Value, const MediumDec& self) { return (MediumDec)Value * self; }
        friend MediumDec operator/(const long double& Value, const MediumDec& self) { return (MediumDec)Value / self; }

    #pragma endregion Floating Operator Overrides*/

    #pragma region Other Operators

        /// <summary>
        /// ++MediumDec Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator ++()
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
        /// ++MediumDec Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator --()
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
        /// <returns>MediumDec</returns>
        MediumDec operator ++(int)
        {
            MediumDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDec</returns>
        MediumDec operator --(int)
        {
            MediumDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	#pragma region Math Etc Functions

        /// <summary>
        /// Creates copy of variable with Abs applied
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDec Abs(MediumDec tValue)
        { tValue.AbsOf(); return tValue; }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        static MediumDec Floor(MediumDec tValue, const int& precision = 0)
        { return tValue.FloorOf(precision); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDec Ceil(MediumDec tValue)
        { return tValue.CeilOf(); }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        static MediumDec Trunc(MediumDec tValue)
        { return tValue.TruncOf(); }

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    MediumDec SqrtOf(const int& precision = 7) {
        return SqrtOfV1(precision);
    }

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    static MediumDec Sqrt(const auto& value, const int& precision = 7)
    {
        return value.SqrtOf(precision);
    }

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions

	#pragma endregion Log Functions

    #pragma region Trigonomic Functions

    #pragma endregion Trigonomic Functions

    };

}