// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "..\..\DLLAPI.h"

#include <compare>
#include "..\Concepts\IntegerConcept.hpp"
#include "..\..\OtherFunctions\VariableConversionFunctions.h"

namespace BlazesRusCode
{
  class MirroredInt;

    /// <summary>
    /// Integer alternative with magnitude and sign field
	/// </summary>
	class DLL_API MirroredInt{
	public:
  #pragma region DigitStorage

  #if defined(AltNum_UseInvertedSign)
    inline static constexpr NegativeSign = 0;
    inline static constexpr PositiveSign = 1;
  #else
    inline static constexpr NegativeSign = 1;
    inline static constexpr PositiveSign = 0;
  #endif

		//If(Sign==NegativeSign), then the sign is negative.
    //Otherwise, the sign is positive
		unsigned int Sign:1;

		//Stores non-signed part of value
		unsigned int Value:31;

  #pragma endregion DigitStorage

	#pragma region class_constructors

	/// <summary>
	/// Initializes a new instance from unsigned value and sign.
	/// </summary>
	constexpr MirroredInt(const unsigned int& value = 0,
							const unsigned int& sign = PositiveSign)
		: Value(value), Sign(sign) {}

	/// <summary>
	/// Initializes a new instance from signed value.
	/// </summary>
	constexpr MirroredInt(const signed int& value)
		: Value(value < 0 ? static_cast<unsigned int>(-value)
							: static_cast<unsigned int>(value)),
			Sign(value < 0 ? PositiveSign : NegativeSign) {}

	/// <summary>
	/// Copy constructor.
	/// </summary>
	constexpr MirroredInt(const MirroredInt& rvalue)
		: Value(rvalue.Value), Sign(rvalue.Sign) {}

	/// <summary>
	/// Copy assignment.
	/// </summary>
	constexpr MirroredInt& operator=(const MirroredInt& rhs)
	{
		if (this != &rhs)
		{
			Value = rhs.Value;
			Sign = rhs.Sign;
		}
		return *this;
	}

	/// <summary>
	/// Assignment from unsigned int.
	/// </summary>
	constexpr MirroredInt& operator=(const unsigned int& rhs)
	{
		Value = rhs;
		Sign = PositiveSign;
		return *this;
	}

	/// <summary>
	/// Assignment from signed int.
	/// </summary>
	constexpr MirroredInt& operator=(const signed int& rhs)
	{
		if (rhs < 0)
		{
			Sign = PositiveSign;
			Value = static_cast<unsigned int>(-rhs);
		}
		else
		{
			Sign = NegativeSign;
			Value = static_cast<unsigned int>(rhs);
		}
		return *this;
	}

	/// <summary>
	/// Sets the value from another MirroredInt.
	/// </summary>
	constexpr void SetValueV2(const MirroredInt& rValue)
	{
		Value = rValue.Value;
		Sign = rValue.Sign;
	}

	/// <summary>
	/// Sets the value from unsigned int and sign.
	/// </summary>
	constexpr void SetValue(const unsigned int& value = 0,
							const unsigned int& sign = PositiveSign)
	{
		Value = value;
		Sign = sign;
	}

	/// <summary>
	/// Sets the value from signed int.
	/// </summary>
	constexpr void SetSignedValue(const signed int& val = 0)
	{
		if (val < 0)
		{
			Sign = PositiveSign;
			Value = static_cast<unsigned int>(-val);
		}
		else
		{
			Sign = NegativeSign;
			Value = static_cast<unsigned int>(val);
		}
	}

	#pragma endregion class_constructors


  #pragma region Negative_Status

		bool IsNegative() const;

		bool IsPositive() const;

    void SetAsPositive();

    void SetAsNegative();

    /// <summary>
    /// Swaps the negative status.
    /// </summary>
    void SwapNegativeStatus();

    MirroredInt operator-() const
    { MirroredInt self = *this; self.SwapNegativeStatus(); return self; }

  #pragma endregion Negative_Status

  #pragma region Check_if_value

    //Set as zero without changing sign(including negative zero)
    void SetAsZeroVal();

    //Return value as real number(negative zero counts as zero)
    int GetValue() const;

    void SetAsZero();

    void SetAsNegativeZero();

    //Is at either zero or negative zero
		bool IsAtZeroInt() const;

    //Is at neither zero or negative zero
		bool IsNotAtZeroInt() const;

    //Is at either zero or negative one
		bool IsAtOneInt() const;

    //Is at neither zero or negative one
		bool IsNotAtOneInt() const;

		bool IsEven() const;

		bool IsOdd() const;

		bool IsZero() const;

    //Returns copy of value as Absolute value
    MirroredInt Abs() const;

    void ApplyAbs();

  #pragma endregion Check_if_value

	#pragma region StringOperations

		void ReadString(const std::string& value);

    /// <summary>
    /// Initializes a new instance of the <see cref="MirroredInt"/> class from string literal
    /// </summary>
    /// <param name="strVal">The value.</param>
    MirroredInt(const char* strVal);

    /// <summary>
    /// Initializes a new instance of the <see cref="MirroredInt"/> class.
    /// </summary>
    /// <param name="tValue">The value.</param>
    MirroredInt(const std::string& Value);

    std::string ToString() const;

		/// <summary>
    /// MirroredInt to int explicit conversion
    /// </summary>
    explicit operator std::string() { return ToString(); }

	#pragma endregion StringOperations

	#pragma region Comparison Operators

	/// Compare only the sign bits
	constexpr std::strong_ordering SignComparison(const MirroredInt& that) const
	{
			// Comparing if number is negative vs positive
	#if defined(AltNum_UseInvertedSign)
			auto SignCmp = Sign <=> that.Sign;
	#else   // (inverted comparison so sign of zero == positive)
			auto SignCmp = that.Sign <=> Sign;
	#endif
			return SignCmp;
	}

	constexpr std::strong_ordering operator<=>(const MirroredInt& that) const
	{
			// Compare sign first
	#if defined(AltNum_UseInvertedSign)
			auto SignCmp = Sign <=> that.Sign;
	#else
			auto SignCmp = that.Sign <=> Sign;
	#endif
			if (SignCmp != 0)
					return SignCmp;

			// Compare magnitude
			return Value <=> that.Value;
	}

	constexpr std::strong_ordering operator<=>(const unsigned int& that) const
	{
			// Compare sign first
	#if defined(AltNum_UseInvertedSign)
			auto SignCmp = Sign <=> PositiveSign;
	#else
			auto SignCmp = PositiveSign <=> Sign;
	#endif
			if (SignCmp != 0)
					return SignCmp;

			// Compare magnitude
			return Value <=> that;
	}

	constexpr std::strong_ordering operator<=>(const signed int& that) const
	{
			if (that < 0)
			{
					// Compare sign first
	#if defined(AltNum_UseInvertedSign)
					auto SignCmp = Sign <=> NegativeSign;
	#else
					auto SignCmp = NegativeSign <=> Sign;
	#endif
					if (SignCmp != 0)
							return SignCmp;

					return Value <=> static_cast<unsigned int>(-that);
			}
			else
			{
					// Compare sign first
	#if defined(AltNum_UseInvertedSign)
					auto SignCmp = Sign <=> PositiveSign;
	#else
					auto SignCmp = PositiveSign <=> Sign;
	#endif
					if (SignCmp != 0)
							return SignCmp;

					return Value <=> static_cast<unsigned int>(that);
			}
	}

	constexpr bool operator==(const MirroredInt& that) const
	{
			return (Value == that.Value) && (Sign == that.Sign);
	}

	constexpr bool operator==(const unsigned int& that) const
	{
			return (Value == that) && (Sign == PositiveSign);
	}

	constexpr bool operator==(const signed int& that) const
	{
			if (that < 0)
					return (Sign == NegativeSign) && (Value == static_cast<unsigned int>(-that));
			else
					return (Sign == PositiveSign) && (Value == static_cast<unsigned int>(that));
	}

	#pragma endregion Comparison Operators


        /// <summary>
        /// to int explicit conversion
        /// </summary>
        explicit operator unsigned int() { return Value; }

        /// <summary>
        /// to int explicit conversion
        /// </summary>
        explicit operator unsigned __int64() { return Value; }

        /// <summary>
        /// to int explicit conversion
        /// </summary>
        explicit operator signed int() { return GetValue(); }

        /// <summary>
        /// to int explicit conversion
        /// </summary>
        explicit operator signed __int64() { return GetValue(); }

	protected:

        /// <summary>
        /// Returns maximum stored value(2147483647)
        /// </summary>
        static MirroredInt MaximumValue();

        /// <summary>
        /// Returns minimum stored value(-2147483647)
        /// </summary>
        static MirroredInt MinimumValue();

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        static MirroredInt NegativeOneValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        static MirroredInt OneValue();

        /// <summary>
        /// Returns the value at two
        /// </summary>
        static MirroredInt TwoValue();

        /// <summary>
        /// Returns the value at negative zero(for negative fractions)
        /// </summary>
        static MirroredInt NegativeZeroValue();

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        static MirroredInt ZeroValue();

	public:

        //Alias:MaxIntHalf
        //Maximum value(2147483647) that can be stored inside IntHalf field
        static const MirroredInt Maximum;

        //Alias:MinIntHalf
        //Minimum value(-2147483647) that can be stored inside IntHalf field
        static const MirroredInt Minimum;

        static const MirroredInt NegativeOne;

        static const MirroredInt One;

        static const MirroredInt Two;

        //Alias:NegativeZeroRep
        static const MirroredInt NegativeZero;

		static const MirroredInt Zero;

		void UInt64DivOp(const unsigned __int64& rValue);

		void UInt64MultOp(const unsigned __int64& rValue);

        void UIntDivOp(const unsigned int& rValue);

        void UIntMultOp(const unsigned int& rValue);

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void UnsignedAddOp(const MirroredInt& rValue);

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void UIntAddOp(const unsigned int& rValue);

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void UnsignedSubOp(const MirroredInt& rValue);

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void UIntSubOp(const unsigned int& rValue);

private:

		void Int64DivOp(const signed __int64& rValue);

		void Int64MultOp(const signed __int64& rValue);

        //Division operation
        void DivOp(const MirroredInt& rValue);

        void IntDivOp(const signed int& rValue);

        //Multiplication operation
        void MultOp(const MirroredInt& rValue);

        void IntMultOp(const signed int& rValue);

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void AddOp(const MirroredInt& rValue);

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void IntAddOp(const signed int& rValue);

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void SubOp(const MirroredInt& rValue);

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void IntSubOp(const signed int& rValue);

public:

		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingUnsignedAddOp(const MirroredInt& rValue);

		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingUnsignedSubOp(const MirroredInt& rValue);

		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingAddOp(const MirroredInt& rValue);


		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingSubOp(const MirroredInt& rValue);

		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingUIntAddOp(const unsigned int& rValue);

		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingUIntSubOp(const unsigned int& rValue);

		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingIntegerAddOp(const signed int& rValue);


		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingIntegerSubOp(const signed int& rValue);

 		friend MirroredInt& operator/=(MirroredInt& lValue, const MirroredInt& rValue){
			lValue.DivOp(rValue); return lValue;
        }

		friend MirroredInt& operator/=(MirroredInt& lValue, const signed int& rValue){
			lValue.IntDivOp(rValue); return lValue;
        }

        friend MirroredInt& operator/=(MirroredInt& lValue, const unsigned int& rValue){
            lValue.Value /= rValue;
            return lValue;
        }

		friend MirroredInt operator/(const MirroredInt& lValue, const MirroredInt& rValue){
            MirroredInt newVal = lValue;
			newVal.DivOp(rValue); return newVal;
        }

		friend MirroredInt operator/(const MirroredInt& lValue, const unsigned int& rValue){
            MirroredInt newVal = lValue;
			newVal.UIntDivOp(rValue); return newVal;
        }

		friend MirroredInt operator/(const MirroredInt& lValue, const signed int& rValue){
            MirroredInt newVal = lValue;
			newVal.IntDivOp(rValue); return newVal;
        }

        friend MirroredInt& operator/=(MirroredInt& lValue, const signed __int64& rValue) {
            lValue.Int64DivOp(rValue); return lValue;
        }

        friend MirroredInt& operator/=(MirroredInt& lValue, const unsigned __int64& rValue) {
            lValue.UInt64DivOp(rValue);
            return lValue;
        }

        friend MirroredInt operator/(const MirroredInt& lValue, const unsigned __int64& rValue) {
            MirroredInt newVal = lValue;
            newVal.UInt64DivOp(rValue); return newVal;
        }

        friend MirroredInt operator/(const MirroredInt& lValue, const signed __int64& rValue) {
            MirroredInt newVal = lValue;
            newVal.Int64DivOp(rValue); return newVal;
        }

        friend MirroredInt& operator*=(MirroredInt& lValue, const MirroredInt& rValue){
            lValue.DivOp(rValue); return lValue;
        }

        friend MirroredInt& operator*=(MirroredInt& lValue, const signed int& rValue){
            lValue.IntMultOp(rValue); return lValue;
        }

        friend MirroredInt& operator*=(MirroredInt& lValue, const unsigned int& rValue){
            lValue.UIntMultOp(rValue); return lValue;
        }

		friend MirroredInt operator*(const MirroredInt& lValue, const MirroredInt& rValue){
            MirroredInt newVal = lValue;
			newVal.MultOp(rValue); return newVal;
        }

		friend MirroredInt operator*(const MirroredInt& lValue, const unsigned int& rValue){
            MirroredInt newVal = lValue;
			newVal.UIntMultOp(rValue); return newVal;
        }

		friend MirroredInt operator*(const MirroredInt& lValue, const signed int& rValue){
            MirroredInt newVal = lValue;
			newVal.IntMultOp(rValue); return newVal;
        }

        friend MirroredInt& operator*=(MirroredInt& lValue, const signed __int64& rValue) {
            lValue.Int64MultOp(rValue); return lValue;
        }

        friend MirroredInt& operator*=(MirroredInt& lValue, const unsigned __int64& rValue) {
            lValue.UInt64MultOp(rValue); return lValue;
        }

        friend MirroredInt operator*(const MirroredInt& lValue, const unsigned __int64& rValue) {
            MirroredInt newVal = lValue;
            newVal.UInt64MultOp(rValue); return newVal;
        }

        friend MirroredInt operator*(const MirroredInt& lValue, const signed __int64& rValue) {
            MirroredInt newVal = lValue;
            newVal.Int64MultOp(rValue); return newVal;
        }

        //Including negative zero by default use NRepSkippingAddOp when DecimalHalf==0
		friend MirroredInt& operator+=(MirroredInt& lValue, const MirroredInt& rValue)
		{ lValue.AddOp(rValue); return lValue;
        }

		friend MirroredInt& operator+=(MirroredInt& lValue, const unsigned int& rValue)
		{ lValue.UIntAddOp(rValue); return lValue; }

		friend MirroredInt& operator+=(MirroredInt& lValue, const signed int& rValue)
		{ lValue.IntAddOp(rValue); return lValue; }

		friend MirroredInt& operator+=(MirroredInt& lValue, const unsigned __int64& rValue)
		{ lValue.UIntAddOp(rValue); return lValue; }

		friend MirroredInt& operator+=(MirroredInt& lValue, const signed __int64& rValue)
		{ lValue.IntAddOp(rValue); return lValue; }

		friend MirroredInt& operator+=(MirroredInt& lValue, const unsigned char& rValue)
        {
            lValue.UIntAddOp(rValue); return lValue;
        }

		friend MirroredInt& operator+=(MirroredInt& lValue, const signed char& rValue)
		{ lValue.IntAddOp(rValue); return lValue; }

		friend MirroredInt& operator+=(MirroredInt& lValue, const unsigned short& rValue)
		{ lValue.UIntAddOp(rValue); return lValue; }

		friend MirroredInt& operator+=(MirroredInt& lValue, const signed short& rValue)
		{ lValue.IntAddOp(rValue); return lValue; }

		friend MirroredInt operator+(const MirroredInt& lValue, const MirroredInt& rValue){
            MirroredInt newVal = lValue;
			newVal.AddOp(rValue); return newVal;
        }

		friend MirroredInt operator+(const MirroredInt& lValue, const unsigned int& rValue){
            MirroredInt newVal = lValue;
			newVal.UIntAddOp(rValue); return newVal;
        }

		friend MirroredInt operator+(const MirroredInt& lValue, const signed int& rValue){
            MirroredInt newVal = lValue;
			newVal.IntAddOp(rValue); return newVal;
        }

        //Including negative zero by default use NRepSkippingSubOp when DecimalHalf==0
		friend MirroredInt& operator-=(MirroredInt& lValue, const MirroredInt& rValue)
		{ lValue.SubOp(rValue); return lValue; }

		friend MirroredInt& operator-=(MirroredInt& lValue, const unsigned int& rValue)
		{ lValue.UIntSubOp(rValue); return lValue; }

		friend MirroredInt& operator-=(MirroredInt& lValue, const signed int& rValue)
		{ lValue.IntSubOp(rValue); return lValue; }

		friend MirroredInt& operator-=(MirroredInt& lValue, const unsigned __int64& rValue)
		{ lValue.UIntSubOp(rValue); return lValue; }

		friend MirroredInt& operator-=(MirroredInt& lValue, const signed __int64& rValue)
		{ lValue.IntSubOp(rValue); return lValue; }

		friend MirroredInt& operator-=(MirroredInt& lValue, const unsigned char& rValue)
		{ lValue.UIntSubOp(rValue); return lValue; }

		friend MirroredInt& operator-=(MirroredInt& lValue, const signed char& rValue)
		{ lValue.IntSubOp(rValue); return lValue; }

		friend MirroredInt& operator-=(MirroredInt& lValue, const unsigned short& rValue)
		{ lValue.UIntSubOp(rValue); return lValue; }

		friend MirroredInt& operator-=(MirroredInt& lValue, const signed short& rValue)
		{ lValue.IntSubOp(rValue); return lValue; }

		friend MirroredInt operator-(const MirroredInt& lValue, const MirroredInt& rValue){
            MirroredInt newVal = lValue;
			newVal.SubOp(rValue); return newVal;
        }

		friend MirroredInt operator-(const MirroredInt& lValue, const unsigned int& rValue){
            MirroredInt newVal = lValue;
			newVal.UIntSubOp(rValue); return newVal;
        }

		friend MirroredInt operator-(const MirroredInt& lValue, const signed int& rValue){
            MirroredInt newVal = lValue;
			newVal.IntSubOp(rValue); return newVal;
        }

		friend MirroredInt& operator%=(MirroredInt& lValue, const unsigned int& rValue)
		{ lValue.Value %= rValue; return lValue; }

		friend MirroredInt operator%(const MirroredInt& lValue, const unsigned int& rValue){
            MirroredInt newVal = lValue;
			newVal.Value %= rValue; return newVal;
        }

		friend MirroredInt& operator%=(MirroredInt& lValue, const MirroredInt& rValue)
		{
			if(rValue.IsNegative())
				lValue.SwapNegativeStatus();
			lValue.Value %= rValue.Value;
			return lValue;
		}

		friend MirroredInt operator%(const MirroredInt& lValue, const MirroredInt& rValue){
            MirroredInt newVal = lValue;
			return newVal%=rValue;
        }

		//template<IntegerType IntType=signed int>
		//MirroredInt& UIntDivisionOperation(const IntType& rValue)
		//{
		//    std::conditional<sizeof(IntType) > sizeof(int), UInt64DivOp(rValue), UIntDivOp(rValue)>;
  //          return *this;
		//}

		//template<IntegerType IntType=signed int>
	 //   MirroredInt& UIntMultiplicationOperation(const IntType& rValue)
		//{
		//	std::conditional<sizeof(IntType) > sizeof(int), UInt64MultOp(rValue), UIntMultOp(rValue)>);
		//    return *this;
  //      }

    #pragma region Other Operators

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        MirroredInt operator- ()
        {
			auto self = *this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// ++MirroredInt Operator
        /// </summary>
        MirroredInt& operator ++()
        {
            UIntAddOp(1);
            return *this;
        }

        /// <summary>
        /// ++MirroredInt Operator
        /// </summary>
        MirroredInt& operator --()
        {
            UIntSubOp(1);
            return *this;
        }

        /// <summary>
        /// MirroredInt++ Operator
        /// </summary>
        MirroredInt operator ++(int)
        {
            MirroredInt tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MirroredInt-- Operator
        /// </summary>
        MirroredInt operator --(int)
        {
            MirroredInt tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MirroredInt* Operator
        /// </summary>
        MirroredInt& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	};
}