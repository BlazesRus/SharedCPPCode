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


    #pragma region Const Representation values
    #pragma endregion Const Representation values
    
    #pragma region RepType
    #pragma endregion RepType

    #pragma region ValueDefines
    private:
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec PiNumValue()
        {
            return MediumDec(3, 141592654);
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec HundredMilPiNumVal()
        {
            return MediumDec(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec TenMilPiNumVal()
        {
            return MediumDec(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDec OneMilPiNumVal()
        {
            return MediumDec(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static MediumDec TenPiNumVal()
        {
            return MediumDec(31, 415926536);
        }
        
        static MediumDec ENumValue()
        {
            return MediumDec(2, 718281828);
        }
        
        static MediumDec ZeroValue()
        {
            return MediumDec();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec OneValue()
        {
            MediumDec NewSelf = MediumDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec TwoValue()
        {
            MediumDec NewSelf = MediumDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec NegativeOneValue()
        {
            MediumDec NewSelf = MediumDec(MirroredInt(1,0));
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDec Point5Value()
        {
            MediumDec NewSelf = MediumDec(0, 500000000);
            return NewSelf;
        }

        static MediumDec JustAboveZeroValue()
        {
            MediumDec NewSelf = MediumDec(0, 1);
            return NewSelf;
        }

        static MediumDec OneMillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 1000);
            return NewSelf;
        }

        static MediumDec FiveThousandthValue()
        {
            MediumDec NewSelf = MediumDec(0, 5000000);
            return NewSelf;
        }

        static MediumDec FiveMillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 5000);
            return NewSelf;
        }

        static MediumDec TenMillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 100);
            return NewSelf;
        }

        static MediumDec OneHundredMillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 10);
            return NewSelf;
        }

        static MediumDec FiveBillionthValue()
        {
            MediumDec NewSelf = MediumDec(0, 5);
            return NewSelf;
        }

        static MediumDec LN10Value()
        {
            return MediumDec(2, 302585093);
        }

        static MediumDec TenthLN10Value()
        {
            return MediumDec(0, 434294482);
        }

        static MediumDec FifthLN10Value()
        {
            return MediumDec(0, 868588964);
        }

        static MediumDec MinimumValue()
        {
            return MediumDec(MinIntHalf, 999999999);
        }

        static MediumDec MaximumValue()
        {
            return MediumDec(MaxIntHalf, 999999999);
        }

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

public:
    #pragma endregion ValueDefines

    #pragma region String Commands

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(const std::string& Value);

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

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(const bool& Value){ this->SetBoolVal(Value); }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes
		//Adding more exact conversion from floating point to MediumDec variant later (after I get the class and variants working again)

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
public:

		std::strong_ordering operator<=>(const MediumDec& that) const
		{//return BasicComparison(that);
			if (MediumDec IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			unsigned int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (MediumDec DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
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
        {
            BasicUIntDivOpV1(Value); return *this;
        }

        template<IntegerType IntType=unsigned int>
        MediumDec& BasicIntDivOperationV1(const IntType& Value)
        {
            BasicIntDivOpV1(Value); return *this;
        }

        MediumDec& BasicUIntDivOperation(const IntType& Value) { return BasicUIntDivOperationV1(rValue); }
        MediumDec& BasicIntDivOperation(const IntType& Value) { return BasicIntDivOperationV1(rValue); }
        MediumDec& BasicUInt64DivOperation(const IntType& Value) { return BasicUIntDivOperationV1(rValue); }
        MediumDec& BasicInt64DivOperation(const IntType& Value) { return BasicIntDivOperationV1(rValue); }
        MediumDec& BasicUInt8DivOperation(const IntType& Value) { return BasicUIntDivOperationV1(rValue); }
        MediumDec& BasicInt8DivOperation(const IntType& Value) { return BasicIntDivOperationV1(rValue); }
        MediumDec& BasicUInt16DivOperation(const IntType& Value) { return BasicUIntDivOperationV1(rValue); }
        MediumDec& BasicInt16DivOperation(const IntType& Value) { return BasicIntDivOperationV1(rValue); }
		
        MediumDec BasicDivideByUInt(const unsigned int& Value) { return BasicDivideByUIntV1(rValue); }
        MediumDec BasicDivideByInt(const signed int& Value) { return BasicDivideByIntV1(rValue); }
        MediumDec BasicDivideByUInt64(const unsigned long long& Value) { return BasicDivideByUIntV1(rValue); }
        MediumDec BasicDivideByInt64(const signed long long& Value) { return BasicDivideByIntV1(rValue); }

        MediumDec UnsignedBasicDivideByInt(const signed int& Value) { return BasicDivideByUIntV1(rValue); }
        MediumDec UnsignedBasicDivideByInt64(const signed long long& Value) { return BasicDivideByUIntV1(rValue); }

        MediumDec BasicDivideByUInt8(const unsigned char& Value) { return BasicDivideByUIntV1(rValue); }
        MediumDec BasicDivideByInt8(const signed char& Value) { return BasicDivideByIntV1(rValue); }
        MediumDec BasicDivideByUInt16(const unsigned short& Value) { return BasicDivideByUIntV1(rValue); }
        MediumDec BasicDivideByInt16(const signed short& Value) { return BasicDivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations

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
        { MediumDec lValue = *this; return self.BasicUnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDec BasicDivideBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return self.BasicDivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(const MediumDec& self, const MediumDec& rValue) { return self.BasicDivideBy(rValue); }
		
        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& self, const MediumDec& rValue) { return self.DivOperation(rValue); }
		
        /// <summary>
        /// Division operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator/(const MediumDec& self, const signed int& rValue) { return self.BasicDivideByInt(rValue); }
        friend MediumDec operator/(const MediumDec& self, const signed long long& rValue) { return self.BasicDivideByInt64(rValue); }
        friend MediumDec operator/(const MediumDec& self, const unsigned int& rValue) { return self.BasicDivideByUInt(rValue); }
        friend MediumDec operator/(const MediumDec& self, const unsigned long long& rValue) { return self.BasicDivideByUInt64(rValue); }
		
        friend MediumDec operator/(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const signed long long& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const unsigned long long& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }

        friend MediumDec operator/(const MediumDec& self, const signed char& rValue) { return self.BasicDivideByInt8(rValue); }
        friend MediumDec operator/(const MediumDec& self, const signed short& rValue) { return self.BasicDivideByInt16(rValue); }
        friend MediumDec operator/(const MediumDec& self, const unsigned char& rValue) { return self.BasicDivideByUInt8(rValue); }
        friend MediumDec operator/(const MediumDec& self, const unsigned short& rValue) { return self.BasicDivideByUInt16(rValue); }

        friend MediumDec operator/(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }
        friend MediumDec operator/(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicDivideBy(rValue); }


        /// <summary>
        /// *= operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator/=(MediumDec& self, const signed int& rValue) { return self.BasicIntDivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const Int64& rValue) { return self.BasicInt64DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned int& rValue) { return self.BasicUIntDivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const UInt64& rValue) { return self.BasicUInt64DivOperation(rValue); }

        friend MediumDec& operator/=(MediumDec& self, const signed char& rValue) { return self.BasicInt8DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const signed short& rValue) { return self.BasicInt16DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned char& rValue) { return self.BasicUInt8DivOperation(rValue); }
        friend MediumDec& operator/=(MediumDec& self, const unsigned short& rValue) { return self.BasicUInt16DivOperation(rValue); }

	#pragma endregion Other Division Operations	

    #pragma region NormalRep Integer Multiplication Operations

        template<IntegerType IntType=unsigned int>
        MediumDec& BasicUIntMultOperationV1(const IntType& Value)
        {
            BasicUIntMultOpV1(Value); return *this;
        }

        template<IntegerType IntType=unsigned int>
        MediumDec& BasicIntMultOperationV1(const IntType& Value)
        {
            BasicIntMultOpV1(Value); return *this;
        }

        MediumDec& BasicUIntMultOperation(const IntType& Value) { return BasicUIntMultOperationV1(rValue); }
        MediumDec& BasicIntMultOperation(const IntType& Value) { return BasicIntMultOperationV1(rValue); }
        MediumDec& BasicUInt64MultOperation(const IntType& Value) { return BasicUIntMultOperationV1(rValue); }
        MediumDec& BasicInt64MultOperation(const IntType& Value) { return BasicIntMultOperationV1(rValue); }
        MediumDec& BasicUInt8MultOperation(const IntType& Value) { return BasicUIntMultOperationV1(rValue); }
        MediumDec& BasicInt8MultOperation(const IntType& Value) { return BasicIntMultOperationV1(rValue); }
        MediumDec& BasicUInt16MultOperation(const IntType& Value) { return BasicUIntMultOperationV1(rValue); }
        MediumDec& BasicInt16MultOperation(const IntType& Value) { return BasicIntMultOperationV1(rValue); }

        MediumDec BasicMultiplyByUInt(const unsigned int& Value) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec BasicMultiplyByInt(const signed int& Value) { return BasicMultiplyByIntV1(rValue); }
        MediumDec BasicMultiplyByUInt64(const unsigned long long& Value) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec BasicMultiplyByInt64(const signed long long& Value) { return BasicMultiplyByIntV1(rValue); }

        MediumDec UnsignedBasicMultiplyByInt(const signed int& Value) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec UnsignedBasicMultiplyByInt64(const signed long long& Value) { return BasicMultiplyByUIntV1(rValue); }

        MediumDec BasicMultiplyByUInt8(const unsigned char& Value) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec BasicMultiplyByInt8(const signed char& Value) { return BasicMultiplyByIntV1(rValue); }
        MediumDec BasicMultiplyByUInt16(const unsigned short& Value) { return BasicMultiplyByUIntV1(rValue); }
        MediumDec BasicMultiplyByInt16(const signed short& Value) { return BasicMultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations

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
        /// Multiplication operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(const MediumDec& self, const MediumDec& rValue) { return self.BasicMultipleBy(rValue); }

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& self, const MediumDec& rValue) { return self.MultOperation(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator*(const MediumDec& lValue, const signed int& rValue) { return lValue.BasicMultiplyByInt(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicMultiplyByInt64(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicMultiplyByUInt(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicMultiplyByUInt64(rValue); }
		
        friend MediumDec operator*(const signed int& lValue, const MediumDec& rValue) { return rValue.BasicMultiplyByInt(rValue); }
        friend MediumDec operator*(const unsigned long long& lValue, const MediumDec& rValue) { return rValue.BasicMultiplyByInt64(rValue); }
        friend MediumDec operator*(const unsigned int& lValue, const MediumDec& rValue) { return rValue.BasicMultiplyByUInt(rValue); }
        friend MediumDec operator*(const unsigned long long& lValue, const MediumDec& rValue) { return rValue.BasicMultiplyByUInt64(rValue); }

        friend MediumDec operator*(const MediumDec& lValue, const signed char& rValue) { return lValue.BasicMultiplyByInt8(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const signed short& rValue) { return lValue.BasicMultiplyByInt16(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicMultiplyByUInt8(rValue); }
        friend MediumDec operator*(const MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicMultiplyByUInt16(rValue); }

        friend MediumDec operator*(const signed char& lValue, const MediumDec& rValue) { return rValue.BasicMultiplyByInt8(rValue); }
        friend MediumDec operator*(const signed short& lValue, const MediumDec& rValue) { return rValue.BasicMultiplyByInt16(rValue); }
        friend MediumDec operator*(const unsigned char& lValue, const MediumDec& rValue) { return rValue.BasicMultiplyByUInt8(rValue); }
        friend MediumDec operator*(const unsigned short& lValue, const MediumDec& rValue) { return rValue.BasicMultiplyByUInt16(rValue); }

        /// <summary>
        /// *= operation between MediumDec and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator*=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntMultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const signed long long& rValue) { return lValue.BasicInt64MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntMultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64MultOperation(rValue); }

        friend MediumDec& operator*=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8MultOperation(rValue); }
        friend MediumDec& operator*=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16MultOperation(rValue); }

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
        { BasicUIntAddOpV1(Value); return *this; }

        MediumDec& BasicIntAddOperation(const signed int& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDec& BasicUInt64AddOperation(const unsigned long long& Value) { return BasicUIntAddOperationV1(rValue); }
        MediumDec& BasicInt64AddOperation(const signed long long& Value) { return BasicIntAddOperationV1(rValue); }
        MediumDec& BasicUInt8AddOperation(const unsigned char& Value) { return BasicUIntAddOperationV1(rValue); }
        MediumDec& BasicInt8AddOperation(const signed char& Value) { return BasicIntAddOperationV1(rValue); }
        MediumDec& BasicUInt16AddOperation(const unsigned short& Value) { return BasicUIntAddOperationV1(rValue); }
        MediumDec& BasicInt16AddOperation(const signed short& Value) { return BasicIntAddOperationV1(rValue); }

        MediumDec BasicAddByUInt(const unsigned int& Value) { return BasicAddByUIntV1(rValue); }
        MediumDec BasicAddByInt(const signed int& Value) { return BasicAddByIntV1(rValue); }
        MediumDec BasicAddByUInt64(const unsigned long long& Value) { return BasicAddByUIntV1(rValue); }
        MediumDec BasicAddByInt64(const signed long long& Value) { return BasicAddByIntV1(rValue); }

        MediumDec UnsignedBasicAddByInt(const signed int& Value) { return BasicAddByUIntV1(rValue); }
        MediumDec UnsignedBasicAddByInt64(const signed long long& Value) { return BasicAddByUIntV1(rValue); }

        MediumDec BasicAddByUInt8(const unsigned char& Value) { return BasicAddByUIntV1(rValue); }
        MediumDec BasicAddByInt8(const signed char Value) { return BasicAddByIntV1(rValue); }
        MediumDec BasicAddByUInt16(const unsigned short& Value) { return BasicAddByUIntV1(rValue); }
        MediumDec BasicAddByInt16(const signed short& Value) { return BasicAddByIntV1(rValue); }

	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:

        template<IntegerType IntType=unsigned int>
        MediumDec& BasicUIntSubOperationV1(const IntType& rValue)
        { BasicUIntSubOpV1(rValue); return *this; }

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
        { BasicUIntSubOpV1(rValue); return *this; }

        MediumDec& BasicIntSubOperation(const signed int& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDec& BasicUInt64SubOperation(const unsigned long long& Value) { return BasicUIntSubOperationV1(rValue); }
        MediumDec& BasicInt64SubOperation(const signed long long& Value) { return BasicIntSubOperationV1(rValue); }
        MediumDec& BasicUInt8SubOperation(const unsigned char& Value) { return BasicUIntSubOperationV1(rValue); }
        MediumDec& BasicInt8SubOperation(const signed char& Value) { return BasicIntSubOperationV1(rValue); }
        MediumDec& BasicUInt16SubOperation(const unsigned short& Value) { return BasicUIntSubOperationV1(rValue); }
        MediumDec& BasicInt16SubOperation(const signed short& Value) { return BasicIntSubOperationV1(rValue); }

        MediumDec BasicSubtractByUInt(const unsigned int& Value) { return BasicSubtractByUIntV1(rValue); }
        MediumDec BasicSubtractByInt(const signed int& Value) { return BasicSubtractByIntV1(rValue); }
        MediumDec BasicSubtractByUInt64(const unsigned long long& Value) { return BasicSubtractByUIntV1(rValue); }
        MediumDec BasicSubtractByInt64(const signed long long& Value) { return BasicSubtractByIntV1(rValue); }

        MediumDec UnsignedBasicSubtractByInt(const signed int& Value) { return BasicSubtractByUIntV1(rValue); }
        MediumDec UnsignedBasicSubtractByInt64(const signed long long& Value) { return BasicSubtractByUIntV1(rValue); }

        MediumDec BasicSubtractByUInt8(const unsigned char& Value) { return BasicSubtractByUIntV1(rValue); }
        MediumDec BasicSubtractByInt8(const signed char Value) { return BasicSubtractByIntV1(rValue); }
        MediumDec BasicSubtractByUInt16(const unsigned short& Value) { return BasicSubtractByUIntV1(rValue); }
        MediumDec BasicSubtractByInt16(const signed short& Value) { return BasicSubtractByIntV1(rValue); }

	#pragma endregion NormalRep Integer Subtraction Operations
	
	#pragma region NormalRep AltNum Addition/Subtraction Operations

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
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(const MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicAddBy(rValue); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicAddOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(const MediumDec& lValue, const signed int& rValue) { return lValue.BasicAddByInt(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const signed long long& rValue) { return lValue.BasicAddByInt64(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicAddByUInt(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicAddByUInt64(rValue); }
		
        friend MediumDec operator+(const signed int& lValue, const MediumDec& rValue) { return rValue.BasicAddByInt(lrValue); }
        friend MediumDec operator+(const unsigned long long& lValue, const MediumDec& rValue) { return rValue.BasicAddByInt64(lrValue); }
        friend MediumDec operator+(const unsigned int& lValue, const MediumDec& rValue) { return rValue.BasicAddByUInt(lrValue); }
        friend MediumDec operator+(const unsigned long long& lValue, const MediumDec& rValue) { return rValue.BasicAddByUInt64(lrValue); }

        friend MediumDec operator+(const MediumDec& lValue, const signed char& rValue) { return lValue.BasicAddByInt8(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const signed short& rValue) { return lValue.BasicAddByInt16(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicAddByUInt8(rValue); }
        friend MediumDec operator+(const MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicAddByUInt16(rValue); }

        friend MediumDec operator+(const signed char& lValue, const MediumDec& rValue) { return rValue.BasicAddByInt8(lrValue); }
        friend MediumDec operator+(const signed short& lValue, const MediumDec& rValue) { return rValue.BasicAddByInt16(lrValue); }
        friend MediumDec operator+(const unsigned char& lValue, const MediumDec& rValue) { return rValue.BasicAddByUInt8(lrValue); }
        friend MediumDec operator+(const unsigned short& lValue, const MediumDec& rValue) { return rValue.BasicAddByUInt16(lrValue); }

        /// <summary>
        /// += operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed long long& rValue) { return lValue.BasicInt64AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64AddOperation(rValue); }

        friend MediumDec& operator+=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16AddOperation(rValue); }
	
	#pragma endregion Other addition operations
	
	#pragma region Other subtraction operations

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(const MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicSubtractBy(rValue); }
		
        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& lValue, const MediumDec& rValue) { return lValue.BasicSubOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(const MediumDec& lValue, const signed int& rValue) { return lValue.BasicSubtractByInt(rValue); }
        friend MediumDec operator-(const MediumDec& lValue, const signed long long& rValue) { return lValue.BasicSubtractByInt64(rValue); }
        friend MediumDec operator-(const MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicSubtractByUInt(rValue); }
        friend MediumDec operator-(const MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicSubtractByUInt64(rValue); }
		
        friend MediumDec operator-(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const signed long long& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const unsigned long long& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }

        friend MediumDec operator-(const MediumDec& lValue, const signed char& rValue) { return lValue.BasicSubtractByInt8(rValue); }
        friend MediumDec operator-(const MediumDec& lValue, const signed short& rValue) { return lValue.BasicSubtractByInt16(rValue); }
        friend MediumDec operator-(const MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicSubtractByUInt8(rValue); }
        friend MediumDec operator-(const MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicSubtractByUInt16(rValue); }

        friend MediumDec operator-(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }
        friend MediumDec operator-(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).BasicSubtractBy(rValue); }

        /// <summary>
        /// -= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& lValue, const signed int& rValue) { return lValue.BasicIntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const signed long long& rValue) { return lValue.BasicInt64SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned int& rValue) { return lValue.BasicUIntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned long long& rValue) { return lValue.BasicUInt64SubOperation(rValue); }

        friend MediumDec& operator-=(MediumDec& lValue, const signed char& rValue) { return lValue.BasicInt8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const signed short& rValue) { return lValue.BasicInt16SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned char& rValue) { return lValue.BasicUInt8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned short& rValue) { return lValue.BasicUInt16SubOperation(rValue); }

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

    #pragma endregion Floating Operator Overrides

    #pragma region Other Operators

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDec</returns>
        MediumDec operator -()
        {
			MediumDec self = this;
            self.SwapNegativeStatus(); return self;
        } const

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

// Static versions of functions for Full versions
	#pragma region Math Etc Functions

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDec&</returns>
        MediumDec& AbsOf()
        {
            if (IntHalf.IsNegative())
                IntHalf.Sign = MirroredInt::PositiveSign;
            return *this;
        }

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDec</returns>
        static MediumDec Abs(const MediumDec tValue&)
        {
            MediumDec self = tValue;
            return self.Abs();
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        MediumDec& FloorOf()
        {
            DecimalHalf.Value = 0;
            return *this;
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDec</returns>
        static MediumDec Floor(const MediumDec tValue&, const int precision&)
        {
            MediumDec self = tValue;
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
        /// <returns>MediumDec&</returns>
        MediumDec& CeilOf()
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
        /// <returns>MediumDec&</returns>
        static int FloorInt(const MediumDec& tValue)
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
        /// <returns>MediumDec&</returns>
        static int CeilInt(const MediumDec& tValue)
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
        /// <returns>MediumDec</returns>
        static MediumDec Ceil(const MediumDec& tValue)
        {
            MediumDec self = tValue;
            return self.Ceil();
        }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& Trunc()
        {
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDec Trunc(const MediumDec& Value)
        {
            MediumDec self = tValue;
            return self.Trunc();
        }

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        MediumDec SqrtOf(const int& precision=7)
        { 
            if(IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
            else if (DecimalHalf == 0)
            {
                MediumDec value = *this;
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

            MediumDec number = this;
            MediumDec start = Zero, end = number;
            MediumDec mid;

            // variable to store the answer 
            MediumDec ans;

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
            MediumDec increment = "0.1";
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
		static MediumDec Sqrt(const MediumDec& value, const int& precision=7)
		{
			return value.SqrtOf(precision);
		}

protected:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDec UIntPowOpV1(const ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.Sign = 1;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                MediumDec self = AbsOf();
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
                    IntHalf.Sign = 0;
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDec IntPowOfOpV1(const ValueType& expValue)
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
                        Sign = 1;
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    //Switches from negative to positive if exp is odd number
                    bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                    MediumDec self = AbsOf();//Prevent needing to flip the sign
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
                        IntHalf.Sign = 0;
                }
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.Sign = 1;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                //Switches from negative to positive if exp is odd number
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                MediumDec self = AbsOf();
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
                    IntHalf.Sign = 0;
            }
            return *this;
        }
		
        /// <summary>
        /// Applies Power of operation(for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDec UIntPowOfV1(const ValueType& expValue)
        {
            MediumDec self = this;
            return self.UIntPowOpV1();
		}
		
        /// <summary>
        /// Applies Power of operation(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDec IntPowOfV1(const ValueType& expValue)
        {
            MediumDec self = this;
            return self.IntPowOpV1();
		}

public:

		MediumDec UnsignedNegIntPower(const unsigned int& exp)
		{
			ResetDivisor();
			//Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
			//Switches from negative to positive if exp is odd number
			bool IsNegative = IsPositive()?false:exp&1==1?false:true;
			MediumDec self = AbsOf();
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
				IntHalf.Sign = 0;
		}

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr MediumDec UIntPowOfOp = UIntPowOfOpV1<unsigned int>;

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr MediumDec IntPowOfOp = IntPowOfOpV1<signed int>;
        constexpr MediumDec UInt64PowOfOp = UIntPowOfOpV1<UInt64>;
        constexpr MediumDec Int64PowOfOp = IntPowOpOfV1<Int64>;
        
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr MediumDec UIntPowOf = UIntPowOfV1<unsigned int>;

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr MediumDec IntPowOf = IntPowOfV1<signed int>;
        constexpr MediumDec UInt64PowOf = UIntPowOfV1<UInt64>;
        constexpr MediumDec Int64PowOf = IntPowOfV1<Int64>;

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>MediumDec</returns>
        MediumDec NthRootOf(const int& n, const MediumDec& precision = MediumDec::JustAboveZero)
        {
            MediumDec xPre = ((this - 1) / n) + 1;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            MediumDec delX = MediumDec(2147483647, 0);

            //  xK denotes current value of x 
            MediumDec xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = (xPre * nMinus1 + this / MediumDec::Pow(xPre, nMinus1)) / n;
                delX = MediumDec::Abs(xK - xPre);
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
        static MediumDec FractionalPow(const MediumDec& value, const int& expNum, const int& expDenom);

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        static MediumDec FractionalPow(const MediumDec& value, const boost::rational<int>& Frac);

        void BasicPowOp(const MediumDec& expValue);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        MediumDec PowOp(const MediumDec& expValue);

        static MediumDec PowOp(const MediumDec& targetValue, MediumDec& expValue)
        {
            return targetValue.PowOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="targetValue">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDec Pow(const MediumDec& targetValue, MediumDec expValue)
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
        /// <returns>MediumDec</returns>
        MediumDec ExpOf()
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
             *      - approximation of e^x in MediumDec precision
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
            MediumDec x0 = *this;
            x0.Abs();
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            MediumDec tn = One;
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
        /// <returns>BlazesRusCode::MediumDec</returns>
        static MediumDec Exp(const MediumDec& x)
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
        MediumDec NthRootOf(const int& n, const MediumDec& Precision = FiveBillionth)
        {
            int nMinus1 = n - 1;
            MediumDec x[2] = { (One / n) * ((*this*nMinus1) + (*this / PowOf(nMinus1))), targetValue };
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
        static MediumDec NthRoot(const MediumDec& value, const int& n, const MediumDec& Precision = FiveBillionth)
        {
            return value.NthRootOf(n, Precision);
        }

protected:

		MediumDec LnRef_Part02()
		{	
            //Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
			//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
			MediumDec TotalRes = (this - 1) / (this + 1);
			MediumDec LastPow = TotalRes;
			MediumDec WSquared = TotalRes * TotalRes;
			MediumDec AddRes;
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
		/// <returns>BlazesRusCode::MediumDec</returns>
		MediumDec NaturalLogOf()
		{
			if(IntHalf.IsNegative())//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from natural log";
			if (IsOne())
				return Zero;
            if(IntHalf==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                MediumDec W = (value - 1)/ (value + 1);
                MediumDec TotalRes = -W;
                MediumDec LastPow = W;
                MediumDec WSquared = W * W;
                int WPow = 3;
                MediumDec AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > MediumDec::JustAboveZero);
                return TotalRes * 2;
            }
            else if (IntHalf==1)//Threshold between 1 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {
                //This section gives accurate answer(for values between 1 and 2)
                MediumDec threshold = MediumDec::FiveMillionth;
                MediumDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDec term = base;       // First term
                MediumDec prev;          // Previous sum
                MediumDec result = term;     // Kick it off

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
                } while (MediumDec::Abs(prev - result) > threshold);

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
        /// <returns>BlazesRusCode::MediumDec</returns>
        static MediumDec Ln(const MediumDec& value)
        {
			return value.NaturalLogOf();
        }
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
		MediumDec Log10Of()
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
                        return MediumDec(index, 0);
                }
                return MediumDec(9, 0);
            }
            if (IntHalf<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                MediumDec threshold = FiveBillionth;
                MediumDec base = this - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDec term = base;       // First term
                MediumDec prev = 0;          // Previous sum
                MediumDec result = term;     // Kick it off

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
                return result*TenthLN10;// result/MediumDec::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return LnRef_Part02() * FifthLN10;//Gives more accurate answer than attempting to divide by Ln10
            }
		}
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
        static MediumDec Log10(const MediumDec& value)
        {
			return value.Log10Of();
        }
		
protected:

    template<IntegerType IntType=unsigned int>
    static MediumDec Log10_IntPart02(const IntType& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        MediumDec TotalRes = MediumDec(value - 1) / MediumDec(value + 1);
        MediumDec LastPow = TotalRes;
        MediumDec WSquared = TotalRes * TotalRes;
        MediumDec AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDec::JustAboveZero);
        return TotalRes * MediumDec::FifthLN10;//Gives more accurate answer than attempting to divide by Ln10
    }
	
public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
		template<IntegerType IntType=unsigned int>
        static MediumDec Log10OfInt(const IntType& value)
        {
			if(value<0)//Returns imaginary number if value is less than 0
				throw "MediumDec does not support returning imaginary number result from log base 10";
            else if (value == 1)
                return MediumDec::Zero;
            else if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDec(index, 0);
                }
                return MediumDec(9, 0);
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
        /// <returns>MediumDec</returns>
        MediumDec LogOf(const MediumDec& baseVal)
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
        /// <returns>MediumDec</returns>
        static MediumDec Log(const MediumDec& value, const MediumDec& baseVal)
        {
            return value.LogOf(baseVal);
        }

protected:

        bool LogOfInt_BaseCalculation(const int& baseVal, MediumDec& baseTotalRes)
        {
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDec(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDec(9, 0); 
                return false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDec((baseVal - 1), 0) / MediumDec((baseVal + 1), 0);
                MediumDec baseLastPow = baseTotalRes;
                MediumDec baseWSquared = baseTotalRes * baseTotalRes;
                MediumDec baseAddRes;
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

        bool LogOf_BaseCalculation(const MediumDec& baseVal, MediumDec& baseTotalRes)
        {
            if (baseVal.DecimalHalf==0&&baseVal.IntHalf.Value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDec(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDec(9, 0); 
                return false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDec((baseVal - 1), 0) / MediumDec((baseVal + 1), 0);
                MediumDec baseLastPow = baseTotalRes;
                MediumDec baseWSquared = baseTotalRes * baseTotalRes;
                MediumDec baseAddRes;
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

        MediumDec LogOf_Section02(const bool& lnMultLog, const MediumDec& baseTotalRes, const MediumDec& threshold)
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
                            return MediumDec(index, 0) / (baseTotalRes * FifthLN10);
                    }
                    return MediumDec(9, 0) / (baseTotalRes*FifthLN10);
                }
                else
                {
                    for (int index = 1; index < 9; ++index)
                    {
                        if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                            return MediumDec(index, 0)/ baseTotalRes;
                    }
                    return MediumDec(9, 0)/baseTotalRes;
                }
            }
            if (IntHalf.Value<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                MediumDec base = this - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDec term = base;       // First term
                MediumDec prev = Zero;          // Previous sum
                MediumDec result = term;     // Kick it off

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
                MediumDec W = (this - 1) / (this + 1);
                MediumDec TotalRes = W;
                MediumDec AddRes;
                int WPow = 3;
                do
                {
                    AddRes = Pow(W, WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > JustAboveZero);
                if(lnMultLog)
                    return TotalRes/baseTotalRes;
                else
                    return (TotalRes * FifthLN10)/ baseTotalRes;
            }
        }

public:

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDec</returns>
        MediumDec LogOfInt(const int& baseVal, const MediumDec& threshold = FiveBillionth)
        {
            //Calculate Base log first
            MediumDec baseTotalRes;
            bool lnMultLog = LogOfInt_BaseCalculation(baseTotalRes);
            return LogOf_Section02(lnMultLog, baseTotalRes, threshold);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDec</returns>
        MediumDec LogOfV2(const MediumDec& baseVal, const MediumDec& threshold = FiveBillionth)
        {
            //Calculate Base log first
            MediumDec baseTotalRes;
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
        /// <returns>MediumDec</returns>
        static MediumDec Sin(const MediumDec& Value)
        {
            MediumDec SinValue = One  / VariableConversionFunctions::Fact(1);
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
        /// <returns>MediumDec</returns>
        static MediumDec Cos(const MediumDec& Value)
        {
            MediumDec CosValue = One / VariableConversionFunctions::Fact(0);
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
        /// <returns>MediumDec</returns>
        static MediumDec Tan(const MediumDec& Value)
        {
            MediumDec SinValue = One  / VariableConversionFunctions::Fact(1);
            MediumDec CosValue = One / VariableConversionFunctions::Fact(0);
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
        /// <returns>MediumDec</returns>
        static MediumDec ATan(const MediumDec& Value)
        {
            MediumDec SinValue = One  / VariableConversionFunctions::Fact(1);
            MediumDec CosValue = One / VariableConversionFunctions::Fact(0);
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
        /// <returns>MediumDec</returns>
        static MediumDec ArcTan2(const MediumDec& y, const MediumDec& x)
        {
            MediumDec coeff_1 = PiNum / 4;
            MediumDec coeff_2 = coeff_1 * 3;
            MediumDec abs_y = Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            MediumDec r;
            MediumDec angle;
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
        /// <returns>MediumDec</returns>
        static MediumDec SinFromAngle(MediumDec Value)
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
                        MediumDec Radius = Pi * Value / 180;
                        return Sin(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                MediumDec Radius = Pi * Value / 180;
                return Sin(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns></returns>
        static MediumDec CosFromAngle(MediumDec Value)
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
                        MediumDec Radius = Pi * Value / 180;
                        return Cos(Radius)
                        break;
                }
            }
            else
            {
                //Angle as Radian
                MediumDec Radius = Pi * Value / 180;
                return Cos(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDec</returns>
        static MediumDec TanFromAngle(MediumDec Value)
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

    std::string MediumDec::ToString()
    {
        std::string Value = std::string(IntHalf);
        if (DecimalHalf != 0)
        {
            Value += ".";
            Value += std::string(DecimalHalf)
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