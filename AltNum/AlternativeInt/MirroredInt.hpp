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
	class DLL_API MirroredInt
    {
	public:
    #if defined(AltNum_UseInvertedSign)
		//If Sign is one, then the sign is positive.
        //Otherwise, the sign is negative
    #else
		//If Sign is one, then the sign is negative.
        //Otherwise, the sign is positive
    #endif
		unsigned int Sign:1;
		//Stores non-signed part of value
		unsigned int Value:31;

    #if defined(AltNum_UseInvertedSign)
        static const unsigned int NegativeSign = 0;
        static const unsigned int PositiveSign = 1;
    #else
        static const unsigned int NegativeSign = 1;
        static const unsigned int PositiveSign = 0;
    #endif

    #pragma region class_constructors

		MirroredInt(const unsigned int& value=0, const unsigned int& sign=PositiveSign);

		MirroredInt(const signed int& value);

		//MirroredInt(const MirroredInt& rvalue)
  //      {
  //          Value = rvalue.Value; Sign = rvalue.Sign;
  //      }

        MirroredInt& operator=(const MirroredInt& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            Value = rhs.Value; Sign = rhs.Sign;
            return *this;
        }

        MirroredInt& operator=(const unsigned int& rhs)
        {
		    Value = rhs;
            Sign = PositiveSign;
            return *this;
        }

        MirroredInt& operator=(const signed int& rhs)
        {
	        if (rhs<0) {
		        Sign = PositiveSign;
		        Value = -rhs;
	        }
	        else {
		        Sign = NegativeSign;
		        Value = rhs;
	        }
            return *this;
        }

        void SetValue(const unsigned int& value=0, const unsigned int& sign=PositiveSign);

        void SetSignedValue(const signed int& val=0);

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

	#pragma region StringOperations

		void ReadString(const std::string& value)
		{
			Value = 0; Sign = PositiveSign;
			std::string WholeNumberBuffer = "";

			int charAsNumber;
			int charAsNumberInPlace;
			for (char const& StringChar : value)
			{
				if (VariableConversionFunctions::IsDigit(StringChar))
					WholeNumberBuffer += StringChar;
				else if (StringChar == '-')
					Sign = NegativeSign;
				else if (StringChar != ' ')
					break;//Stop Extracting after encounter non-number character such as i or .
			}
	        unsigned int PlaceNumber = WholeNumberBuffer.length() - 1;//Last character is digit value 0-9
	        for (char const& StringChar : WholeNumberBuffer)
	        {
				charAsNumber = VariableConversionFunctions::CharAsInt(StringChar);
				charAsNumberInPlace = (charAsNumber * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
				if (StringChar != '0')
				{
					Value += charAsNumberInPlace;
				}
                PlaceNumber--;
			}
		}

        /// <summary>
        /// Initializes a new instance of the <see cref="MirroredInt"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MirroredInt(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MirroredInt"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MirroredInt(const std::string& Value)
        {
            this->ReadString(Value);
        }

        std::string ToString() const;

		/// <summary>
        /// MirroredInt to int explicit conversion
        /// </summary>
        explicit operator std::string() { return ToString(); }

	#pragma endregion StringOperations

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

    #pragma endregion Check_if_value

        //Returns copy of value as Absolute value
        MirroredInt Abs() const;

        void ApplyAbs();

		std::strong_ordering SignComparison(const MirroredInt& that) const
		{
			//Comparing if number is negative vs positive
    #if defined(AltNum_UseInvertedSign)
			auto SignCmp = Sign <=> that.Sign;
    #else   //(inverted comparison so sign of zero==positive)
            auto SignCmp = that.Sign <=> Sign;
    #endif
            return SignCmp;
        }
/*
        //Comparison between left side positive infinity and right side real number
		std::strong_ordering LeftSidePosInfinityComparison(const MirroredInt& that) const
		{

        }

        //Comparison between left side negative infinity and right side real number
		std::strong_ordering LeftSideNegInfinityComparison(const MirroredInt& that) const
		{

        }

        //Comparison between left side real number and right side positive infinity
		std::strong_ordering RightSidePosInfinityComparison(const MirroredInt& that) const
		{

        }

        //Comparison between left side real number and right side negative infinity
		std::strong_ordering RightSideNegInfinityComparison(const MirroredInt& that) const
		{

        }
*/

		std::strong_ordering operator<=>(const MirroredInt& that) const
		{
			//Comparing if number is negative vs positive
    #if defined(AltNum_UseInvertedSign)
			auto SignCmp = Sign <=> that.Sign;
    #else   //(inverted comparison so sign of zero==positive)
            auto SignCmp = that.Sign <=> Sign;
    #endif
	        if (SignCmp != 0)
				return SignCmp;
			auto ValueCmp = Value <=> that.Value;
				return ValueCmp;
		}

		bool operator==(const MirroredInt& that) const
		{
			if (Value!=that.Value)
				return false;
			if (Sign!=that.Sign)
				return false;
			return true;
		}

		std::strong_ordering operator<=>(const unsigned int& that) const
		{
			//Comparing if number is negative vs positive
    #if defined(AltNum_UseInvertedSign)
			auto SignCmp = Sign <=> PositiveSign;
    #else   //(inverted comparison so sign of zero==positive)
            auto SignCmp = PositiveSign <=> Sign;
    #endif
	        if (SignCmp != 0)
				return SignCmp;
			auto ValueCmp = Value <=> that;
				return ValueCmp;
		}

		std::strong_ordering operator<=>(const signed int& that) const
		{
            if(that<0){
    			//Comparing if number is negative vs positive
        #if defined(AltNum_UseInvertedSign)
    			auto SignCmp = Sign <=> NegativeSign;
        #else   //(inverted comparison so sign of zero==positive)
                auto SignCmp = NegativeSign <=> Sign;
        #endif
    	        if (SignCmp != 0)
    				return SignCmp;
    			if (auto ValueCmp = Value <=> (unsigned int)that; ValueCmp != 0)
    				return ValueCmp;
            } else {
    			//Comparing if number is negative vs positive
        #if defined(AltNum_UseInvertedSign)
    			auto SignCmp = Sign <=> PositiveSign;
        #else   //(inverted comparison so sign of zero==positive)
                auto SignCmp = PositiveSign <=> Sign;
        #endif
    	        if (SignCmp != 0)
    				return SignCmp;
    			auto ValueCmp = Value <=> (unsigned int)(-that);
    				return ValueCmp;
            }
		}

        bool operator==(const unsigned int& that) const
		{
			if (Value!=that)
				return false;
			if (Sign!=PositiveSign)
				return false;
			return true;
		}

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
        static MirroredInt Maximum;

        //Alias:MinIntHalf
        //Minimum value(-2147483647) that can be stored inside IntHalf field
        static MirroredInt Minimum;

        static MirroredInt NegativeOne;

        static MirroredInt One;

        static MirroredInt Two;

        //Alias:NegativeZeroRep
        static MirroredInt NegativeZero;

		static MirroredInt Zero;

//These templates require size int 32 or greater in order to preserve storage of value (commenting out for now to reduce strain on extension)
/*		template<IntegerType IntType=unsigned int>
		void UIntDivOpV1(const IntType& rValue)
		{
			IntType result = (IntType) Value;
			result /= rValue;
			Value = (unsigned int) result;
		}

		template<IntegerType IntType=signed int>
		void IntDivOpV1(const IntType& rValue)
		{
			IntType result = (IntType) Value;
			if (rValue<0) {
				SwapNegativeStatus();
				result /= -rValue;
			}
			else
				result /= rValue;
			Value = (unsigned int) result;
		}

		template<IntegerType IntType=unsigned int>
		void UIntMultOpV1(const IntType& rValue)
		{
			IntType result = (IntType) Value;
			result *= rValue;
			Value = (unsigned int) result;
		}

		template<IntegerType IntType=signed int>
		void IntMultOpV1(const IntType& rValue)
		{
			IntType result = (IntType) Value;
			if (rValue<0) {
				SwapNegativeStatus();
				result *= -rValue;
			}
			else
				result *= rValue;
			Value = (unsigned int) result;
		}*/

		void UInt64DivOp(const unsigned __int64& rValue);

		void Int64DivOp(const signed __int64& rValue);

		void UInt64MultOp(const unsigned __int64& rValue);

		void Int64MultOp(const signed __int64& rValue);

        //Division operation
        void DivOp(const MirroredInt& rValue);

        void UIntDivOp(const unsigned int& rValue);

        void IntDivOp(const signed int& rValue);

        //Multiplication operation
        void MultOp(const MirroredInt& rValue);

        void UIntMultOp(const unsigned int& rValue);

        void IntMultOp(const signed int& rValue);

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void UnsignedAddOp(const MirroredInt& rValue);

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void AddOp(const MirroredInt& rValue);

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void UIntAddOp(const unsigned int& rValue);

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void IntAddOp(const signed int& rValue);

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void UnsignedSubOp(const MirroredInt& rValue);

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void SubOp(const MirroredInt& rValue);

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void UIntSubOp(const unsigned int& rValue);

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void IntSubOp(const signed int& rValue);

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