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
        MediumDec(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDec(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

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
        MediumDec(const MediumDecBase& rhs)
        {
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
        }

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

        void SetValue(MediumDec Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf;
        }

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
        
        static MediumDec AlmostOneValue()
        { return MediumDec(0, 999999999); }

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec PiNumValue()
        { return MediumDec(3, 141592654); }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec HundredMilPiNumVal()
        { return MediumDec(314159265, 358979324); }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec TenMilPiNumVal()
        { return MediumDec(31415926, 535897932); }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec OneMilPiNumVal()
        { return MediumDec(3141592, 653589793);}

        //10xPi(Rounded to 9th decimal digit)
        static MediumDec TenPiNumVal()
        { return MediumDec(31, 415926536); }
        
        static MediumDec ENumValue()
        { return MediumDec(2, 718281828); }
        
        static MediumDec ZeroValue()
        { return MediumDec(); }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec OneValue()
        { return MediumDec(1); }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec TwoValue()
        { return MediumDec(MirroredInt::Two);}

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec NegativeOneValue()
        { return MediumDec(MirroredInt::NegativeOne);}

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Point5Value()
        { return MediumDec(0, 500000000); }

        static MediumDec JustAboveZeroValue()
        { return MediumDec(0, 1); }

        static MediumDec OneMillionthValue()
        { return MediumDec(0, 1000); }

        static MediumDec FiveThousandthValue()
        { return MediumDec(0, 5000000); }

        static MediumDec FiveMillionthValue()
        { return MediumDec(0, 5000);}

        static MediumDec TenMillionthValue()
        { return MediumDec(0, 100); }

        static MediumDec OneHundredMillionthValue()
        { return MediumDec(0, 10); }

        static MediumDec FiveBillionthValue()
        { return MediumDec(0, 5); }

        static MediumDec LN10Value()
        { return MediumDec(2, 302585093); }

        static MediumDec TenthLN10Value()
        { return MediumDec(0, 434294482); }

        static MediumDec FifthLN10Value()
        { return MediumDec(0, 868588964); }

        static MediumDec MinimumValue()
        { return MediumDec(MirroredInt::Maximum, 999999999); }

        static MediumDec MaximumValue()
        { return MediumDec(MirroredInt::Minimum, 999999999); }

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
        static MediumDec TenthLN10;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDec FifthLN10;

    #pragma endregion ValueDefines

    #pragma region String Commands

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        MediumDec GetValueFromString(std::string Value)
        {
            MediumDec NewSelf = MediumDec();
            NewSelf.ReadString(Value);
            return NewSelf;
        }

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
        MediumDec& BasicUIntDivOperationV1(const IntType& Value)
        { BasicUIntDivOpV1(Value); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDec& BasicIntDivOperationV1(const IntType& Value)
        { BasicIntDivOpV1(Value); return *this; }

protected:

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicDivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntDivOperationV1(rValue); }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        auto BasicDivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntDivOperationV1(rValue); }

public:

        MediumDec& BasicUIntDivOperation(const unsigned int& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDec& BasicIntDivOperation(const signed int& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDec& BasicUInt64DivOperation(const unsigned __int64& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDec& BasicInt64DivOperation(const signed __int64& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDec& BasicUInt8DivOperation(const unsigned char& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDec& BasicInt8DivOperation(const signed char& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDec& BasicUInt16DivOperation(const unsigned short& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDec& BasicInt16DivOperation(const signed short& rValue) { return BasicIntDivOperationV1(rValue); }

        MediumDec BasicDivideByUInt(const unsigned int& rValue) { return BasicDivideByUIntV1(rValue); }
        MediumDec BasicDivideByInt(const signed int& rValue) { return BasicDivideByIntV1(rValue); }
        MediumDec BasicDivideByUInt64(const unsigned __int64& rValue) { return BasicDivideByUIntV1(rValue); }
        MediumDec BasicDivideByInt64(const signed __int64& rValue) { return BasicDivideByIntV1(rValue); }

        MediumDec UnsignedBasicDivideByInt(const signed int& rValue) { return BasicDivideByUIntV1(rValue); }
        MediumDec UnsignedBasicDivideByInt64(const signed __int64& rValue) { return BasicDivideByUIntV1(rValue); }

        MediumDec BasicDivideByUInt8(const unsigned char& rValue) { return BasicDivideByUIntV1(rValue); }
        MediumDec BasicDivideByInt8(const signed char& rValue) { return BasicDivideByIntV1(rValue); }
        MediumDec BasicDivideByUInt16(const unsigned short& rValue) { return BasicDivideByUIntV1(rValue); }
        MediumDec BasicDivideByInt16(const signed short& rValue) { return BasicDivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool UnsignedPartialDivOp(const MediumDecBase& rValue){ return UnsignedPartialDivOpV1(rValue); }
		
		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void BasicUnsignedDivOp(const MediumDecBase& rValue){ BasicUnsignedDivOpV1(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void BasicDivOp(const MediumDecBase& rValue){ BasicDivOpV1(rValue); }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec& BasicUnsignedDivOperation(const MediumDec& rValue)
		{ BasicUnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec& BasicDivOperation(const MediumDec& rValue)
		{ BasicDivOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec BasicDivideByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.BasicUnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        auto BasicDivideBy(const MediumDec& rValue)
        { auto lValue = *this; return lValue.BasicDivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicDivOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntDivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const signed __int64& rValue) { return lValue.BasicInt64DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntDivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64DivOperation(rValue); }

        friend MediumDec& operator/=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicDivideBy(rValue); }
		
        /// <summary>
        /// Division operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(MediumDec lValue, const signed int& rValue) { return lValue.BasicIntDivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const signed __int64& rValue) { return lValue.BasicInt64DivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const unsigned int& rValue) { return lValue.BasicUIntDivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64DivOperation(rValue); }

        friend MediumDec operator/(MediumDec lValue, const signed char& rValue) { return lValue.BasicInt8DivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const signed short& rValue) { return lValue.BasicInt16DivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const unsigned char& rValue) { return lValue.BasicUInt8DivOperation(rValue); }
        friend MediumDec operator/(MediumDec lValue, const unsigned short& rValue) { return lValue.BasicUInt16DivOperation(rValue); }
		
        friend MediumDec operator/(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const signed __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const unsigned __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }

        friend MediumDec operator/(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }

	#pragma endregion Other Division Operations	

    #pragma region NormalRep Integer Multiplication Operations

        template<IntegerType IntType=unsigned int>
        MediumDec& BasicUIntMultOperationV1(const IntType& Value)
        { BasicUIntMultOpV1(Value); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDec& BasicIntMultOperationV1(const IntType& Value)
        { BasicIntMultOpV1(Value); return *this; }

protected:

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto BasicMultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntMultOperationV1(rValue); }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto BasicMultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntMultOperationV1(rValue); }

public:

        MediumDec& BasicUIntMultOperation(const unsigned int& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDec& BasicIntMultOperation(const signed int& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDec& BasicUInt64MultOperation(const unsigned __int64& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDec& BasicInt64MultOperation(const signed __int64& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDec& BasicUInt8MultOperation(const unsigned char& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDec& BasicInt8MultOperation(const signed char& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDec& BasicUInt16MultOperation(const unsigned short& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDec& BasicInt16MultOperation(const signed short& rValue) { return BasicIntMultOperationV1(rValue); }

        MediumDec BasicMultiplyByUInt(const unsigned int& rValue) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec BasicMultiplyByInt(const signed int& rValue) { return BasicMultiplyByIntV1(rValue); }
        MediumDec BasicMultiplyByUInt64(const unsigned __int64& rValue) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec BasicMultiplyByInt64(const signed __int64& rValue) { return BasicMultiplyByIntV1(rValue); }

        MediumDec UnsignedBasicMultiplyByInt(const signed int& rValue) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec UnsignedBasicMultiplyByInt64(const signed __int64& rValue) { return BasicMultiplyByUIntV1(rValue); }

        MediumDec BasicMultiplyByUInt8(const unsigned char& rValue) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec BasicMultiplyByInt8(const signed char& rValue) { return BasicMultiplyByIntV1(rValue); }
        MediumDec BasicMultiplyByUInt16(const unsigned short& rValue) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec BasicMultiplyByInt16(const signed short& rValue) { return BasicMultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        bool BasicUnsignedMultOp(const MediumDecBase& rValue){ BasicUnsignedMultOpV1(rValue); }
		
        void BasicMultOp(const MediumDecBase& rValue){ BasicMultOpV1(rValue); }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec& BasicUnsignedMultOperation(const MediumDec& rValue)
        { BasicUnsignedMultOp(rValue); return *this; }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec& BasicMultOperation(const MediumDec& rValue)
		{ BasicMultOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec BasicMultiplyByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.BasicUnsignedMultOperation(rValue); }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDec BasicMultiplyBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.BasicMultOperation(rValue); }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region Other multiplication operations

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicMultOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec& operator*=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntMultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const signed __int64& rValue) { return lValue.BasicInt64MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntMultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64MultOperation(rValue); }

        friend MediumDec& operator*=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(MediumDec lValue, const MediumDec& rValue) { return lValue.BasicMultOperation(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec operator*(MediumDec lValue, const signed int& rValue) { return lValue.BasicIntMultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const signed __int64& rValue) { return lValue.BasicInt64MultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const unsigned int& rValue) { return lValue.BasicUIntMultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64MultOperation(rValue); }
		
        friend MediumDec operator*(MediumDec lValue, const signed char& rValue) { return lValue.BasicInt8MultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const signed short& rValue) { return lValue.BasicInt16MultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const unsigned char& rValue) { return lValue.BasicUInt8MultOperation(rValue); }
        friend MediumDec operator*(MediumDec lValue, const unsigned short& rValue) { return lValue.BasicUInt16MultOperation(rValue); }
		
        friend MediumDec operator*(signed int lValue, MediumDec rValue)  { return rValue.BasicIntMultOperation(lValue); }
        friend MediumDec operator*(signed __int64 lValue, MediumDec& rValue)  { return rValue.BasicInt64MultOperation(lValue); }
        friend MediumDec operator*(unsigned int lValue, MediumDec& rValue)  { return rValue.BasicUIntMultOperation(lValue); }
        friend MediumDec operator*(unsigned __int64 lValue, MediumDec& rValue)  { return rValue.BasicUInt64MultOperation(lValue); }
		
        friend MediumDec operator*(signed char lValue, MediumDec& rValue)  { return rValue.BasicInt8MultOperation(lValue); }
        friend MediumDec operator*(signed short lValue, MediumDec& rValue)  { return rValue.BasicInt16MultOperation(lValue); }
        friend MediumDec operator*(unsigned char lValue, MediumDec& rValue)  { return rValue.BasicUInt8MultOperation(lValue); }
        friend MediumDec operator*(unsigned short lValue, MediumDec& rValue)  { return rValue.BasicUInt16MultOperation(lValue); }

	#pragma endregion Other Multiplication Operations

	#pragma region NormalRep Integer Addition Operations
protected:

        template<IntegerType IntType=unsigned int>
        MediumDec& BasicUIntAddOperationV1(const IntType& rValue)
        { BasicUIntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        MediumDec& BasicIntAddOperationV1(const IntType& rValue)
        { BasicIntAddOpV1(rValue); return *this; }

		/// <summary>
        /// Basic addition operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        MediumDec BasicAddByUIntV1(const IntType& rValue)
        { MediumDec self = *this; return self.BasicUIntAddOperationV1(rValue); }

		/// <summary>
        /// Basic addition operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDec BasicAddByIntV1(const IntType& rValue)
        { MediumDec self = *this; return self.BasicIntAddOperationV1(rValue); }

public:

        MediumDec& BasicUIntAddOperation(const unsigned int& rValue)
        { BasicUIntAddOp(rValue); return *this; }

        MediumDec& BasicIntAddOperation(const signed int& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDec& BasicUInt64AddOperation(const unsigned __int64& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDec& BasicInt64AddOperation(const signed __int64& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDec& BasicUInt8AddOperation(const unsigned char& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDec& BasicInt8AddOperation(const signed char& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDec& BasicUInt16AddOperation(const unsigned short& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDec& BasicInt16AddOperation(const signed short& rValue) { return BasicIntAddOperationV1(rValue); }

        MediumDec BasicAddByUInt(const unsigned int& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDec BasicAddByInt(const signed int& rValue) { return BasicAddByIntV1(rValue); }
        MediumDec BasicAddByUInt64(const unsigned __int64& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDec BasicAddByInt64(const signed __int64& rValue) { return BasicAddByIntV1(rValue); }

        MediumDec UnsignedBasicAddByInt(const signed int& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDec UnsignedBasicAddByInt64(const signed __int64& rValue) { return BasicAddByUIntV1(rValue); }

        MediumDec BasicAddByUInt8(const unsigned char& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDec BasicAddByInt8(const signed char rValue) { return BasicAddByIntV1(rValue); }
        MediumDec BasicAddByUInt16(const unsigned short& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDec BasicAddByInt16(const signed short& rValue) { return BasicAddByIntV1(rValue); }

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
        MediumDec& BasicUIntSubOperationV1(const IntType& rValue)
        { BasicUIntSubOpV1(rValue); return *this; }

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDec& BasicIntSubOperationV1(const IntType& rValue)
        { BasicIntSubOpV1(rValue); return *this; }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        MediumDec BasicSubtractByUIntV1(const IntType& rValue)
        { MediumDec self = *this; return self.BasicUIntSubOperationV1(rValue); }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        MediumDec BasicSubtractByIntV1(const IntType& rValue)
        { MediumDec self = *this; return self.BasicIntSubOperationV1(rValue); }

public:

        MediumDec& BasicUIntSubOperation(const unsigned int& rValue)
        { BasicUIntSubOp(rValue); return *this; }

        MediumDec& BasicIntSubOperation(const signed int& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDec& BasicUInt64SubOperation(const unsigned __int64& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDec& BasicInt64SubOperation(const signed __int64& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDec& BasicUInt8SubOperation(const unsigned char& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDec& BasicInt8SubOperation(const signed char& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDec& BasicUInt16SubOperation(const unsigned short& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDec& BasicInt16SubOperation(const signed short& rValue) { return BasicIntSubOperationV1(rValue); }

        MediumDec BasicSubtractByUInt(const unsigned int& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDec BasicSubtractByInt(const signed int& rValue) { return BasicSubtractByIntV1(rValue); }
        MediumDec BasicSubtractByUInt64(const unsigned __int64& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDec BasicSubtractByInt64(const signed __int64& rValue) { return BasicSubtractByIntV1(rValue); }

        MediumDec UnsignedBasicSubtractByInt(const signed int& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDec UnsignedBasicSubtractByInt64(const signed __int64& rValue) { return BasicSubtractByUIntV1(rValue); }

        MediumDec BasicSubtractByUInt8(const unsigned char& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDec BasicSubtractByInt8(const signed char rValue) { return BasicSubtractByIntV1(rValue); }
        MediumDec BasicSubtractByUInt16(const unsigned short& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDec BasicSubtractByInt16(const signed short& rValue) { return BasicSubtractByIntV1(rValue); }
   
	#pragma endregion NormalRep Integer Subtraction Operations
	
	#pragma region NormalRep AltNum Addition/Subtraction Operations

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedAddOp(const MediumDecBase& rValue){ BasicUnsignedAddOpV1(rValue); }

	    /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicAddOp(const MediumDecBase& rValue){ BasicAddOpV1(rValue); }

	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedSubOp(const MediumDecBase& rValue){ BasicUnsignedSubOpV1(rValue); }
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicSubOp(const MediumDecBase& rValue){ BasicSubOpV1(rValue); }

        //Basic addition operation
        MediumDec& BasicUnsignedAddOperation(const MediumDec& rValue)
        { BasicUnsignedAddOp(rValue); return *this; }
		
        //Basic addition operation
        MediumDec& BasicAddOperation(const MediumDec& rValue)
        { BasicAddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec BasicAddByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.BasicUnsignedAddOperation(rValue); }

		/// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDec BasicAddBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.BasicAddOperation(rValue); }

        //Basic subtraction operation
        MediumDec& BasicUnsignedSubOperation(const MediumDec& rValue)
        { BasicUnsignedSubOp(rValue); return *this; }

        //Basic subtraction operation
        MediumDec& BasicSubOperation(const MediumDec& rValue)
        { BasicSubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDec BasicSubtractByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.BasicUnsignedSubOperation(rValue); }

		/// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDec BasicSubtractBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.BasicSubOperation(rValue); }

	#pragma endregion NormalRep AltNum Addition/Subtraction Operations
	
	#pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicAddOperation(rValue); }

        /// <summary>
        /// += operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec& operator+=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed __int64& rValue) { return lValue.BasicInt64AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64AddOperation(rValue); }

        friend MediumDec& operator+=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(MediumDec lValue, const MediumDec& rValue) { return lValue.BasicAddOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec operator+(MediumDec lValue, const signed int& rValue) { return lValue.BasicIntAddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const signed __int64& rValue) { return lValue.BasicInt64AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned int& rValue) { return lValue.BasicUIntAddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64AddOperation(rValue); }
		
        friend MediumDec operator+(MediumDec lValue, const signed char& rValue) { return lValue.BasicInt8AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const signed short& rValue) { return lValue.BasicInt16AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned char& rValue) { return lValue.BasicUInt8AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned short& rValue) { return lValue.BasicUInt16AddOperation(rValue); }
		
        friend MediumDec operator+(const signed int& lValue, MediumDec rValue)  { return rValue.BasicIntAddOperation(lValue); }
        friend MediumDec operator+(const signed __int64& lValue, MediumDec rValue)  { return rValue.BasicInt64AddOperation(lValue); }
        friend MediumDec operator+(const unsigned int& lValue, MediumDec rValue)  { return rValue.BasicUIntAddOperation(lValue); }
        friend MediumDec operator+(const unsigned __int64& lValue, MediumDec rValue)  { return rValue.BasicUInt64AddOperation(lValue); }
		
        friend MediumDec operator+(const signed char& lValue, MediumDec rValue)  { return rValue.BasicInt8AddOperation(lValue); }
        friend MediumDec operator+(const signed short& lValue, MediumDec rValue)  { return rValue.BasicInt16AddOperation(lValue); }
        friend MediumDec operator+(const unsigned char& lValue, MediumDec rValue)  { return rValue.BasicUInt8AddOperation(lValue); }
        friend MediumDec operator+(const unsigned short& lValue, MediumDec rValue)  { return rValue.BasicUInt16AddOperation(lValue); }

	#pragma endregion Other addition operations

	#pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicSubOperation(rValue); }

        /// <summary>
        /// -= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const signed __int64& rValue) { return lValue.BasicInt64SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64SubOperation(rValue); }

        friend MediumDec& operator-=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec lValue, const MediumDec& rValue) { return lValue.BasicSubOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec lValue, const signed int& rValue) { return lValue.BasicIntSubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const signed __int64& rValue) { return lValue.BasicInt64SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned int& rValue) { return lValue.BasicUIntSubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64SubOperation(rValue); }
		
        friend MediumDec operator-(MediumDec lValue, const signed char& rValue) { return lValue.BasicInt8SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const signed short& rValue) { return lValue.BasicInt16SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned char& rValue) { return lValue.BasicUInt8SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned short& rValue) { return lValue.BasicUInt16SubOperation(rValue); }
		
        friend MediumDec operator-(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const signed __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const unsigned __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
		
        friend MediumDec operator-(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }

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
        static auto Abs(MediumDec tValue)
        { tValue.AbsOf(); return tValue; }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        static auto Floor(MediumDec tValue, const int& precision = 0)
        { return tValue.FloorOf(precision); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecBase</returns>
        static auto Ceil(MediumDec tValue)
        { return tValue.CeilOf(); }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecBase</returns>
        static auto Trunc(MediumDec tValue)
        { return tValue.TruncOf(); }

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    MediumDecBase SqrtOf(const int& precision = 7) {
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

//Experimental result saving modulus operation(use assignment to get normal result)
	#pragma region Modulus Operations
    //Global Binary functions

        MediumDec& operator%=(MediumDec& lValue, const MediumDec& rValue)
		{
            if(lValue.DecimalHalf.Value==0&&rValue.DecimalHalf.Value==0){
                if(rValue.IsNegative())
                    lValue.SwapNegativeStatus();
                lValue.IntHalf.Value %= rValue.IntHalf.Value;
            }
            else {
                MediumDec divRes = lValue / rValue;
                lValue -= rValue * divRes;
            }
            return lValue;
		}

        /// <summary>
        /// %= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        MediumDec& operator%=(MediumDec& lValue, const signed int& rValue) { return lValue %= (MediumDec)rValue; }
        MediumDec& operator%=(MediumDec& lValue, const signed __int64& rValue) { return lValue%=(MediumDec)rValue; }
        MediumDec& operator%=(MediumDec& lValue, const unsigned int& rValue) { return lValue%=(MediumDec)rValue; }
        MediumDec& operator%=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue%=(MediumDec)rValue; }

        MediumDec& operator%=(MediumDec& lValue, const signed char& rValue) { return lValue%=(MediumDec)rValue; }
        MediumDec& operator%=(MediumDec& lValue, const signed short& rValue) { return lValue%=(MediumDec)rValue; }
        MediumDec& operator%=(MediumDec& lValue, const unsigned char& rValue) { return lValue%=(MediumDec)rValue; }
        MediumDec& operator%=(MediumDec& lValue, const unsigned short& rValue) { return lValue%=(MediumDec)rValue; }
		
  //      using MediumDecModResult = MediumDec::ModResult;

  //      /// <summary>
  //      /// Modulus Operation:
		///// divRes member variables gives division result,
		///// modRes member variable and + operator gives mod result,
		///// bool conversion gives result of modRes==0
  //      /// </summary>
  //      /// <param name="self">The left side value</param>
  //      /// <param name="Value">The right side value.</param>
  //      /// <returns>MediumDecV2Base</returns>
  //      MediumDecModResult operator%(const MediumDec& LValue, const MediumDec& RValue)
		//{
		//	return MediumDecModResult(LValue, RValue);
		//}
//        /// <summary>
//        /// Modulus operation between MediumDec variant and Integer rValue.
//        /// </summary>
//        /// <param name="lValue">The left side value</param>
//        /// <param name="rValue">The right side value.</param>
//        /// <returns>MediumDec</returns>
//        friend ModResult operator%(const MediumDec& lValue, const signed int& rValue) { return ModResult(lValue, MediumDec(rValue)); }
//        friend ModResult operator%(const MediumDec& lValue, const signed __int64& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//        friend ModResult operator%(const MediumDec& lValue, const unsigned int& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//        friend ModResult operator%(const MediumDec& lValue, const unsigned __int64& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//		
//        friend ModResult operator%(const MediumDec& lValue, const signed char& rValue) { return ModResult(lValue, MediumDec(rValue)); }
//        friend ModResult operator%(const MediumDec& lValue, const signed short& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//        friend ModResult operator%(const MediumDec& lValue, const unsigned char& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//        friend ModResult operator%(const MediumDec& lValue, const unsigned short& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//		
//
//        friend ModResult operator%(const signed int& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue);  }
//        friend ModResult operator%(const signed __int64& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const unsigned int& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const unsigned __int64& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//		
//        friend ModResult operator%(const signed char& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const signed short& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const unsigned char& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const unsigned short& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }

	#pragma endregion Modulus Operations

}