// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecV2Base.hpp"
#include "..\AltNumModResult.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumDecV2;

	/// <summary>
	/// long double (Extended precision double)
	/// </summary>
	using ldouble = long double;

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy to 9th digit
    /// Represents +- 2147483647.999999999 with 100% accuracy 
    /// except for truncation during division and multiplication after 9th digit
	/// </summary>
    class DLL_API MediumDecV2 : public MediumDecV2Base
    {
public:
#pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero);

        MediumDecV2& operator=(const MediumDecV2& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumDecV2& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2(const MediumDecV2Base& rhs);

        MediumDecV2& operator=(const MediumDecV2Base& rhs)
        {
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<MediumDecV2Variant VariantType>
        MediumDecV2 operator[](VariantType variantValue) const
        {
            MediumDecV2 newSelf = MediumDecV2(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

#pragma endregion class_constructors

		//Performs remainder/Mod operation then saves division result
		class DLL_API ModResult : public AltNumModResult<MediumDecV2Base>{};

        void SetValue(MediumDecV2 Value);

        template<MediumDecV2Variant VariantType=MediumDecV2Base>
        void SetValueFromVariant(VariantType Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf;
        }

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecV2Base</returns>
        MediumDecV2 operator -() const
        {
			MediumDecV2 self = *this;
            self.SwapNegativeStatus(); return self;
        }

    #pragma region ValueDefines
    private://Each class needs to define it's own
        
        static MediumDecV2 AlmostOneValue();

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 PiNumValue();

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2 HundredMilPiNumValue();

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2 TenMilPiNumValue();

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2 OneMilPiNumValue();

        //10xPi(Rounded to 9th decimal digit)
        static MediumDecV2 TenPiNumValue();
        
        static MediumDecV2 ENumValue();
        
        static MediumDecV2 ZeroValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 OneValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 TwoValue();

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 NegativeOneValue();

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Point5Value();

        static MediumDecV2 JustAboveZeroValue();

        static MediumDecV2 OneMillionthValue();

        static MediumDecV2 FiveThousandthValue();

        static MediumDecV2 FiveMillionthValue();

        static MediumDecV2 TenMillionthValue();

        static MediumDecV2 OneHundredMillionthValue();

        static MediumDecV2 FiveBillionthValue();

        static MediumDecV2 LN10Value();

        static MediumDecV2 LN10DivValue();

        static MediumDecV2 TwiceLN10DivValue();

        static MediumDecV2 MinimumValue();

        static MediumDecV2 MaximumValue();

        static MediumDecV2 NegativePointFiveValue();

public:
        static MediumDecV2 AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 ENum;
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 OneGMillionth;

        //0e-7
        static MediumDecV2 TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDecV2 FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDecV2 Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDecV2 Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDecV2 LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecV2 LN10Div;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecV2 TwiceLN10Div;

        static MediumDecV2 NegativePointFive;

    #pragma endregion ValueDefines

    #pragma region String Commands

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecV2</returns>
        MediumDecV2 GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecV2(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2(const std::string& Value)
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
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2(const float& Value){ this->SetFloatVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2(const double& Value){ this->SetDoubleVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2(const long double& Value){ this->SetDecimalVal(Value); }
*/

        MediumDecV2(const unsigned __int64& Value){ this->SetUIntVal(Value); }
        MediumDecV2(const signed __int64& Value){ this->SetIntVal(Value); }
        MediumDecV2(const unsigned char& Value){ this->SetUIntVal(Value); }
        MediumDecV2(const signed char& Value){ this->SetIntVal(Value); }
        MediumDecV2(const unsigned short& Value){ this->SetUIntVal(Value); }
        MediumDecV2(const signed short& Value){ this->SetIntVal(Value); }
        MediumDecV2(const unsigned int& Value){ this->SetUIntVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2(const bool& Value){ this->SetBoolVal(Value); }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

/*
        /// <summary>
        /// MediumDecV2 Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }
		
        /// <summary>
        /// MediumDecV2 Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDouble(); }
		
        /// <summary>
        /// MediumDecV2 Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator long double() { return toDecimal(); }
*/

        /// <summary>
        /// MediumDecV2 Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return toInt(); }

        /// <summary>
        /// MediumDecV2 Variant to uint explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return toUInt(); }

        /// <summary>
        /// MediumDecV2 Variant to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed __int64() { return toInt(); }

        /// <summary>
        /// MediumDecV2 Variant to ubyte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned char() { return toUInt(); }

        /// <summary>
        /// MediumDecV2 Variant to byte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed char() { return toInt(); }

        /// <summary>
        /// MediumDecV2 Variant to ushort explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned short() { return toUInt(); }

        /// <summary>
        /// MediumDecV2 Variant to short explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed short() { return toInt(); }

        /// <summary>
        /// MediumDecV2 Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes
	
    #pragma region Comparison Operators
public:

		std::strong_ordering operator<=>(const MediumDecV2& that) const
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

		bool operator==(const MediumDecV2& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.DecimalHalf)
				return false;
            return true;
		}

		bool operator!=(const MediumDecV2& that) const
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
        MediumDecV2& UIntDivOperationV1(const IntType& Value)
        { UIntDivOpV1(Value); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDecV2& IntDivOperationV1(const IntType& Value)
        { IntDivOpV1(Value); return *this; }

protected:

        /// <summary>
        /// Basic division operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=unsigned int>
        auto DivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntDivOperationV1(rValue); }

        /// <summary>
        /// Basic division operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        auto DivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntDivOperationV1(rValue); }

public:

        MediumDecV2& UIntDivOperation(const unsigned int& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2& UInt64DivOperation(const unsigned __int64& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2& UInt8DivOperation(const unsigned char& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2& UInt16DivOperation(const unsigned short& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

        MediumDecV2 DivideByUInt(const unsigned int& rValue) { return DivideByUIntV1(rValue); }
        MediumDecV2 DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        MediumDecV2 DivideByUInt64(const unsigned __int64& rValue) { return DivideByUIntV1(rValue); }
        MediumDecV2 DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        MediumDecV2 UnsignedDivideByInt(const signed int& rValue) { return DivideByUIntV1(rValue); }
        MediumDecV2 UnsignedDivideByInt64(const signed __int64& rValue) { return DivideByUIntV1(rValue); }

        MediumDecV2 DivideByUInt8(const unsigned char& rValue) { return DivideByUIntV1(rValue); }
        MediumDecV2 DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
        MediumDecV2 DivideByUInt16(const unsigned short& rValue) { return DivideByUIntV1(rValue); }
        MediumDecV2 DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool UnsignedPartialDivOp(const MediumDecV2& rValue){ return UnsignedPartialDivOpV1(rValue); }
		
		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void UnsignedDivOp(const MediumDecV2& rValue){ UnsignedDivOpV1(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void DivOp(const MediumDecV2& rValue){ DivOpV1(rValue); }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2& UnsignedDivOperation(const MediumDecV2& rValue)
		{ UnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2& DivOperation(const MediumDecV2& rValue)
		{ DivOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2 DivideByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto DivideBy(const MediumDecV2& rValue)
        { auto lValue = *this; return lValue.DivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.DivOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDecV2 variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDecV2& operator/=(MediumDecV2& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator/(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.DivideBy(rValue); }
		
        /// <summary>
        /// Division operation between MediumDecV2 variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator/(MediumDecV2 lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDecV2 operator/(MediumDecV2 lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }
		
        friend MediumDecV2 operator/(const signed int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const signed __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }

        friend MediumDecV2 operator/(const signed char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const signed short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }

	#pragma endregion Other Division Operations	

    #pragma region NormalRep Integer Multiplication Operations

        template<IntegerType IntType=unsigned int>
        MediumDecV2& UIntMultOperationV1(const IntType& Value)
        { UIntMultOpV1(Value); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDecV2& IntMultOperationV1(const IntType& Value)
        { IntMultOpV1(Value); return *this; }

protected:

		/// <summary>
        /// Basic multiplication operation between MediumDecV2 variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto MultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntMultOperationV1(rValue); }

		/// <summary>
        /// Basic multiplication operation between MediumDecV2 variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto MultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntMultOperationV1(rValue); }

public:

        MediumDecV2& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

        MediumDecV2 MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDecV2 MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
        MediumDecV2 MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDecV2 MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

        MediumDecV2 UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDecV2 UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

        MediumDecV2 MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDecV2 MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
        MediumDecV2 MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
        MediumDecV2 MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDecV2
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        bool UnsignedMultOp(const MediumDecV2& rValue){ UnsignedMultOpV1(rValue); }
		
        void MultOp(const MediumDecV2& rValue){ MultOpV1(rValue); }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2& UnsignedMultOperation(const MediumDecV2& rValue)
        { UnsignedMultOp(rValue); return *this; }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2& MultOperation(const MediumDecV2& rValue)
		{ MultOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2 MultiplyByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedMultOperation(rValue); }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecV2 MultiplyBy(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.MultOperation(rValue); }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region Other multiplication operations

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDecV2 variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumDecV2& operator*=(MediumDecV2& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator*(MediumDecV2 lValue, const MediumDecV2& rValue) { return lValue.MultOperation(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDecV2 variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2 operator*(MediumDecV2 lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }
		
        friend MediumDecV2 operator*(MediumDecV2 lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }
		
        friend MediumDecV2 operator*(signed int lValue, MediumDecV2 rValue)  { return rValue.IntMultOperation(lValue); }
        friend MediumDecV2 operator*(signed __int64 lValue, MediumDecV2& rValue)  { return rValue.Int64MultOperation(lValue); }
        friend MediumDecV2 operator*(unsigned int lValue, MediumDecV2& rValue)  { return rValue.UIntMultOperation(lValue); }
        friend MediumDecV2 operator*(unsigned __int64 lValue, MediumDecV2& rValue)  { return rValue.UInt64MultOperation(lValue); }
		
        friend MediumDecV2 operator*(signed char lValue, MediumDecV2& rValue)  { return rValue.Int8MultOperation(lValue); }
        friend MediumDecV2 operator*(signed short lValue, MediumDecV2& rValue)  { return rValue.Int16MultOperation(lValue); }
        friend MediumDecV2 operator*(unsigned char lValue, MediumDecV2& rValue)  { return rValue.UInt8MultOperation(lValue); }
        friend MediumDecV2 operator*(unsigned short lValue, MediumDecV2& rValue)  { return rValue.UInt16MultOperation(lValue); }

	#pragma endregion Other Multiplication Operations

	#pragma region NormalRep Integer Addition Operations
protected:

        template<IntegerType IntType=unsigned int>
        MediumDecV2& UIntAddOperationV1(const IntType& rValue)
        { UIntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        MediumDecV2& IntAddOperationV1(const IntType& rValue)
        { IntAddOpV1(rValue); return *this; }

		/// <summary>
        /// Basic addition operation between MediumDecV2 variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        MediumDecV2 AddByUIntV1(const IntType& rValue)
        { MediumDecV2 self = *this; return self.UIntAddOperationV1(rValue); }

		/// <summary>
        /// Basic addition operation between MediumDecV2 variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDecV2 AddByIntV1(const IntType& rValue)
        { MediumDecV2 self = *this; return self.IntAddOperationV1(rValue); }

public:

        MediumDecV2& UIntAddOperation(const unsigned int& rValue)
        { UIntAddOp(rValue); return *this; }

        MediumDecV2& IntAddOperation(const signed int& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2& UInt64AddOperation(const unsigned __int64& rValue) { return UIntAddOperationV1(rValue); }
        MediumDecV2& Int64AddOperation(const signed __int64& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2& UInt8AddOperation(const unsigned char& rValue) { return UIntAddOperationV1(rValue); }
        MediumDecV2& Int8AddOperation(const signed char& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2& UInt16AddOperation(const unsigned short& rValue) { return UIntAddOperationV1(rValue); }
        MediumDecV2& Int16AddOperation(const signed short& rValue) { return IntAddOperationV1(rValue); }

        MediumDecV2 AddByUInt(const unsigned int& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 AddByInt(const signed int& rValue) { return AddByIntV1(rValue); }
        MediumDecV2 AddByUInt64(const unsigned __int64& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 AddByInt64(const signed __int64& rValue) { return AddByIntV1(rValue); }

        MediumDecV2 UnsignedAddByInt(const signed int& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 UnsignedAddByInt64(const signed __int64& rValue) { return AddByUIntV1(rValue); }

        MediumDecV2 AddByUInt8(const unsigned char& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 AddByInt8(const signed char rValue) { return AddByIntV1(rValue); }
        MediumDecV2 AddByUInt16(const unsigned short& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 AddByInt16(const signed short& rValue) { return AddByIntV1(rValue); }

	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:

        /// <summary>
        /// Basic Subtraction operation between MediumDecV2 Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        MediumDecV2& UIntSubOperationV1(const IntType& rValue)
        { UIntSubOpV1(rValue); return *this; }

        /// <summary>
        /// Basic Subtraction operation between MediumDecV2 Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDecV2& IntSubOperationV1(const IntType& rValue)
        { IntSubOpV1(rValue); return *this; }

		/// <summary>
        /// Basic Subtraction operation between MediumDecV2 variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        MediumDecV2 SubtractByUIntV1(const IntType& rValue)
        { MediumDecV2 self = *this; return self.UIntSubOperationV1(rValue); }

		/// <summary>
        /// Basic Subtraction operation between MediumDecV2 variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDecV2 SubtractByIntV1(const IntType& rValue)
        { MediumDecV2 self = *this; return self.IntSubOperationV1(rValue); }

public:

        MediumDecV2& UIntSubOperation(const unsigned int& rValue)
        { UIntSubOp(rValue); return *this; }

        MediumDecV2& IntSubOperation(const signed int& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2& UInt64SubOperation(const unsigned __int64& rValue) { return UIntSubOperationV1(rValue); }
        MediumDecV2& Int64SubOperation(const signed __int64& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2& UInt8SubOperation(const unsigned char& rValue) { return UIntSubOperationV1(rValue); }
        MediumDecV2& Int8SubOperation(const signed char& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2& UInt16SubOperation(const unsigned short& rValue) { return UIntSubOperationV1(rValue); }
        MediumDecV2& Int16SubOperation(const signed short& rValue) { return IntSubOperationV1(rValue); }

        MediumDecV2 SubtractByUInt(const unsigned int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 SubtractByInt(const signed int& rValue) { return SubtractByIntV1(rValue); }
        MediumDecV2 SubtractByUInt64(const unsigned __int64& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 SubtractByInt64(const signed __int64& rValue) { return SubtractByIntV1(rValue); }

        MediumDecV2 UnsignedSubtractByInt(const signed int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 UnsignedSubtractByInt64(const signed __int64& rValue) { return SubtractByUIntV1(rValue); }

        MediumDecV2 SubtractByUInt8(const unsigned char& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 SubtractByInt8(const signed char rValue) { return SubtractByIntV1(rValue); }
        MediumDecV2 SubtractByUInt16(const unsigned short& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 SubtractByInt16(const signed short& rValue) { return SubtractByIntV1(rValue); }
   
	#pragma endregion NormalRep Integer Subtraction Operations
	
	#pragma region NormalRep AltNum Addition/Subtraction Operations

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedAddOp(const MediumDecV2& rValue){ UnsignedAddOpV1(rValue); }

	    /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOp(const MediumDecV2& rValue){ AddOpV1(rValue); }

	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedSubOp(const MediumDecV2& rValue){ UnsignedSubOpV1(rValue); }
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const MediumDecV2& rValue){ SubOpV1(rValue); }

        //Basic addition operation
        MediumDecV2& UnsignedAddOperation(const MediumDecV2& rValue)
        { UnsignedAddOp(rValue); return *this; }
		
        //Basic addition operation
        MediumDecV2& AddOperation(const MediumDecV2& rValue)
        { AddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2 AddByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedAddOperation(rValue); }

		/// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecV2 AddBy(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.AddOperation(rValue); }

        //Basic subtraction operation
        MediumDecV2& UnsignedSubOperation(const MediumDecV2& rValue)
        { UnsignedSubOp(rValue); return *this; }

        //Basic subtraction operation
        MediumDecV2& SubOperation(const MediumDecV2& rValue)
        { SubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2 SubtractByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedSubOperation(rValue); }

		/// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecV2 SubtractBy(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.SubOperation(rValue); }

	#pragma endregion NormalRep AltNum Addition/Subtraction Operations
	
	#pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// += operation between MediumDecV2 variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumDecV2& operator+=(MediumDecV2& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator+(MediumDecV2 lValue, const MediumDecV2& rValue) { return lValue.AddOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDecV2 variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2 operator+(MediumDecV2 lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }
		
        friend MediumDecV2 operator+(MediumDecV2 lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }
		
        friend MediumDecV2 operator+(const signed int& lValue, MediumDecV2 rValue)  { return rValue.IntAddOperation(lValue); }
        friend MediumDecV2 operator+(const signed __int64& lValue, MediumDecV2 rValue)  { return rValue.Int64AddOperation(lValue); }
        friend MediumDecV2 operator+(const unsigned int& lValue, MediumDecV2 rValue)  { return rValue.UIntAddOperation(lValue); }
        friend MediumDecV2 operator+(const unsigned __int64& lValue, MediumDecV2 rValue)  { return rValue.UInt64AddOperation(lValue); }
		
        friend MediumDecV2 operator+(const signed char& lValue, MediumDecV2 rValue)  { return rValue.Int8AddOperation(lValue); }
        friend MediumDecV2 operator+(const signed short& lValue, MediumDecV2 rValue)  { return rValue.Int16AddOperation(lValue); }
        friend MediumDecV2 operator+(const unsigned char& lValue, MediumDecV2 rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend MediumDecV2 operator+(const unsigned short& lValue, MediumDecV2 rValue)  { return rValue.UInt16AddOperation(lValue); }

	#pragma endregion Other addition operations

	#pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// -= operation between MediumDecV2 variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumDecV2& operator-=(MediumDecV2& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator-(MediumDecV2 lValue, const MediumDecV2& rValue) { return lValue.SubOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDecV2 variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator-(MediumDecV2 lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }
		
        friend MediumDecV2 operator-(MediumDecV2 lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }
		
        friend MediumDecV2 operator-(const signed int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const signed __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
		
        friend MediumDecV2 operator-(const signed char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const signed short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }

	#pragma endregion Other subtraction operations

/*
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

        friend MediumDecV2 operator+(const MediumDecV2& self, const long double& Value) { return self + (MediumDecV2)Value; }
        friend MediumDecV2 operator-(const MediumDecV2& self, const long double& Value) { return self - (MediumDecV2)Value; }
        friend MediumDecV2 operator*(const MediumDecV2& self, const long double& Value) { return self * (MediumDecV2)Value; }
        friend MediumDecV2 operator/(const MediumDecV2& self, const long double& Value) { return self / (MediumDecV2)Value; }

        friend MediumDecV2 operator+(const long double& Value, const MediumDecV2& self) { return (MediumDecV2)Value + self; }
        friend MediumDecV2 operator-(const long double& Value, const MediumDecV2& self) { return (MediumDecV2)Value - self; }
        friend MediumDecV2 operator*(const long double& Value, const MediumDecV2& self) { return (MediumDecV2)Value * self; }
        friend MediumDecV2 operator/(const long double& Value, const MediumDecV2& self) { return (MediumDecV2)Value / self; }

    #pragma endregion Floating Operator Overrides*/

    #pragma region Other Operators

        /// <summary>
        /// ++MediumDecV2 Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator ++()
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
        /// ++MediumDecV2 Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator --()
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
        /// MediumDecV2 Variant++ Operator
        /// </summary>
        /// <returns>MediumDecV2</returns>
        MediumDecV2 operator ++(int)
        {
            MediumDecV2 tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecV2 Variant-- Operator
        /// </summary>
        /// <returns>MediumDecV2</returns>
        MediumDecV2 operator --(int)
        {
            MediumDecV2 tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecV2 Variant* Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	#pragma region Math Etc Functions

        /// <summary>
        /// Creates copy of variable with Abs applied
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Abs(MediumDecV2 tValue)
        { tValue.AbsOf(); return tValue; }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        static MediumDecV2 Floor(MediumDecV2 tValue, const int& precision = 0)
        { return tValue.FloorOf(precision); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Ceil(MediumDecV2 tValue)
        { return tValue.CeilOf(); }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Trunc(MediumDecV2 tValue)
        { return tValue.TruncOf(); }

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    MediumDecV2 SqrtOf(const int& precision = 7) {
        return SqrtOfV1(precision);
    }

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    static auto Sqrt(const auto& value, const int& precision = 7)
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