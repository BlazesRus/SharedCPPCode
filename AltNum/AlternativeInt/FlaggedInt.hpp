// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "..\..\DLLAPI.h"

#include <compare>
#include "..\Concepts\IntegerConcept.hpp"
#include "..\..\OtherFunctions\VariableConversionFunctions.h"
#include "PartialInt.hpp"

namespace BlazesRusCode
{
	struct DLL_API FlaggedInt {
    public:
		//Is either mixed fraction or power of representation if this value is one
		unsigned int IsAltRep:1;
		//Stores non-signed part of value
		unsigned int Value:31;

		FlaggedInt(const unsigned int& value=0, const unsigned int& isAltRep=0);

        FlaggedInt(const FlaggedInt& rhs);

        FlaggedInt& operator=(const FlaggedInt& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            Value = rhs.Value; IsAltRep = rhs.IsAltRep;
            return *this;
        }

/*
        FlaggedInt& operator=(const unsigned int& rhs)
        {
		    Value = rhs;
            IsAltRep = 0;
            return *this;
        }

*/

        bool IsAlternative() const;

		bool IsNormal();

		void SwitchToAlternative();

		void SwitchToNormal();

        //Is at zero value
        bool IsAtZero() const;

        //Is not at zero value
        bool IsNotAtZero() const;

        //Is at one value
        bool IsAtOne() const;

        //Is at neither zero or negative one
        bool IsNotOne() const;

		bool IsEven() const;

		bool IsOdd() const;

#pragma region StringOperations

		void ReadString(const std::string& value)
		{
			Value = 0;
			std::string WholeNumberBuffer = "";

			int charAsNumber;
			int charAsNumberInPlace;
			for (char const& StringChar : value)
			{
				if (VariableConversionFunctions::IsDigit(StringChar))
					WholeNumberBuffer += StringChar;
				else if (StringChar != ' ')
					break;//Stop Extracting after encounter non-number character such as i or .
			}
	        unsigned int PlaceNumber = WholeNumberBuffer.length() - 1;//Last character is digit one
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
        /// Initializes a new instance of the <see cref="FlaggedInt"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        FlaggedInt(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedInt"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        FlaggedInt(const std::string& Value)
        {
            this->ReadString(Value);
        }

        std::string ToString() const;

        std::string ToDetailedString(const PartialInt& DecHalf) const;

        explicit operator std::string();

#pragma endregion StringOperations

		std::strong_ordering operator<=>(const FlaggedInt& that) const
		{
			if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
				return ValueCmp;
		}

		bool operator==(const FlaggedInt& that) const
		{
			if (Value!=that.Value)
				return false;
            if(IsAltRep!=that.IsAltRep)
                return false;
			return true;
		}

		std::strong_ordering operator<=>(const unsigned int& that) const
		{
			if (auto ValueCmp = Value <=> that; ValueCmp != 0)
				return ValueCmp;
		}

		bool operator==(const unsigned int& that) const
		{
			if (Value==that)
				return true;
		}

		bool operator!=(const unsigned int& that) const
		{
			if (Value!=that)
				return true;
			return true;
		}

		bool operator==(const signed int& that) const
		{
			if (that < 0)
				return false;
			else if (Value==(unsigned int)that)
				return false;
		}

		bool operator!=(const signed int& that) const
		{
			if (that < 0)
				return false;
			else if (Value!=(unsigned int)that)
				return true;
		}

        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return Value; }

        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned __int64() { return Value; }

        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return (signed int)Value; }

        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed __int64() { return (signed __int64) Value; }

	protected:
        /// <summary>
        /// Returns maximum stored value(2147483647)
        /// </summary>
        static FlaggedInt MaximumValue();

        /// <summary>
        /// Returns minimum stored value(-2147483647)
        /// </summary>
        static FlaggedInt AltMaximumValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        static FlaggedInt OneValue();

        /// <summary>
        /// Returns the value at two
        /// </summary>
        static FlaggedInt TwoValue();

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        static FlaggedInt ZeroValue();

	public:

        static FlaggedInt Maximum;

        static FlaggedInt AltMaximum;

        static FlaggedInt One;

        static FlaggedInt Two;

		static FlaggedInt Zero;

		void UInt64DivOp(const unsigned __int64& rValue);

		void Int64DivOp(const signed __int64& rValue);

		void UInt64MultOp(const unsigned __int64& rValue);

		void Int64MultOp(const signed __int64& rValue);

 		friend FlaggedInt& operator/=(FlaggedInt& lValue, const FlaggedInt& rValue){
			lValue.Value /= rValue.Value; return lValue;
        }

        friend FlaggedInt& operator/=(FlaggedInt& lValue, const unsigned int& rValue){
            lValue.Value /= rValue; return lValue;
        }

		friend FlaggedInt& operator/=(FlaggedInt& lValue, const signed int& rValue){
			lValue.Value /= rValue; return lValue;
        }

		friend FlaggedInt operator/(const FlaggedInt& lValue, const FlaggedInt& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value /= rValue.Value; return newVal;
        }

		friend FlaggedInt operator/(const FlaggedInt& lValue, const unsigned int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value /= rValue; return newVal;
        }

		friend FlaggedInt operator/(const FlaggedInt& lValue, const signed int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value /= rValue; return newVal;
        }

        friend FlaggedInt& operator*=(FlaggedInt& lValue, const FlaggedInt& rValue){
            lValue.Value *= rValue.Value; return lValue;
        }

        friend FlaggedInt& operator*=(FlaggedInt& lValue, const unsigned int& rValue){
            lValue.Value *= rValue; return lValue;
        }

        friend FlaggedInt& operator*=(FlaggedInt& lValue, const signed int& rValue){
            lValue.Value *= rValue; return lValue;
        }

		friend FlaggedInt operator*(const FlaggedInt& lValue, const FlaggedInt& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value *= rValue.Value; return newVal;
        }

		friend FlaggedInt operator*(const FlaggedInt& lValue, const unsigned int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value *= rValue; return newVal;
        }

		friend FlaggedInt operator*(const FlaggedInt& lValue, const signed int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value *= rValue; return newVal;
        }

		friend FlaggedInt& operator+=(FlaggedInt& lValue, const FlaggedInt& rValue){
			lValue.Value += rValue.Value; return lValue;
        }

		friend FlaggedInt& operator+=(FlaggedInt& lValue, const unsigned int& rValue){
			lValue.Value += rValue; return lValue;
        }

		friend FlaggedInt& operator+=(FlaggedInt& lValue, const signed int& rValue){
			lValue.Value += rValue; return lValue;
        }

		friend FlaggedInt operator+(const FlaggedInt& lValue, const FlaggedInt& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value += rValue.Value; return newVal;
        }

		friend FlaggedInt operator+(const FlaggedInt& lValue, const unsigned int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value += rValue; return newVal;
        }

		friend FlaggedInt operator+(const FlaggedInt& lValue, const signed int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value += rValue; return newVal;
        }

		friend FlaggedInt& operator-=(FlaggedInt& lValue, const FlaggedInt& rValue){
            lValue.Value -= rValue.Value; return lValue;
        }

		friend FlaggedInt& operator-=(FlaggedInt& lValue, const unsigned int& rValue){
            lValue.Value -= rValue; return lValue;
        }

		friend FlaggedInt& operator-=(FlaggedInt& lValue, const signed int& rValue){
            lValue.Value -= rValue; return lValue;
        }

		friend FlaggedInt operator-(const FlaggedInt& lValue, const FlaggedInt& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value -= rValue.Value; return newVal;
        }

		friend FlaggedInt operator-(const FlaggedInt& lValue, const unsigned int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value -= rValue; return newVal;
        }

		friend FlaggedInt operator-(const FlaggedInt& lValue, const signed int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value -= rValue; return newVal;
        }

        friend FlaggedInt& operator/=(FlaggedInt& lValue, const signed __int64& rValue) {
            lValue.Int64DivOp(rValue); return lValue;
        }

        friend FlaggedInt& operator/=(FlaggedInt& lValue, const unsigned __int64& rValue) {
            lValue.UInt64DivOp(rValue);
            return lValue;
        }

        friend FlaggedInt operator/(const FlaggedInt& lValue, const unsigned __int64& rValue) {
            FlaggedInt newVal = lValue;
            newVal.UInt64DivOp(rValue); return newVal;
        }

        friend FlaggedInt operator/(const FlaggedInt& lValue, const signed __int64& rValue) {
            FlaggedInt newVal = lValue;
            newVal.Int64DivOp(rValue); return newVal;
        }

        friend FlaggedInt& operator*=(FlaggedInt& lValue, const signed __int64& rValue) {
            lValue.Int64MultOp(rValue); return lValue;
        }

        friend FlaggedInt& operator*=(FlaggedInt& lValue, const unsigned __int64& rValue) {
            lValue.UInt64MultOp(rValue); return lValue;
        }

        friend FlaggedInt operator*(const FlaggedInt& lValue, const unsigned __int64& rValue) {
            FlaggedInt newVal = lValue;
            newVal.UInt64MultOp(rValue); return newVal;
        }

        friend FlaggedInt operator*(const FlaggedInt& lValue, const signed __int64& rValue) {
            FlaggedInt newVal = lValue;
            newVal.Int64MultOp(rValue); return newVal;
        }

		friend FlaggedInt& operator%=(FlaggedInt& lValue, const unsigned int& rValue)
		{ lValue.Value %= rValue; return lValue; }

		friend FlaggedInt operator%(const FlaggedInt& lValue, const unsigned int& rValue){
            FlaggedInt newVal = lValue;
			newVal.Value %= rValue; return newVal;
        }

		friend FlaggedInt& operator%=(FlaggedInt& lValue, const FlaggedInt& rValue)
		{
			lValue.Value %= rValue.Value; return lValue;
		}

		friend FlaggedInt operator%(const FlaggedInt& lValue, const FlaggedInt& rValue){
            FlaggedInt newVal = lValue;
			return newVal%=rValue;
        }

    #pragma region Other Operators

        /// <summary>
        /// ++FlaggedInt Operator
        /// </summary>
        FlaggedInt& operator ++()
        {
            Value += 1;
            return *this;
        }

        /// <summary>
        /// ++FlaggedInt Operator
        /// </summary>
        FlaggedInt& operator --()
        {
            Value -= 1;
            return *this;
        }

        /// <summary>
        /// FlaggedInt++ Operator
        /// </summary>
        FlaggedInt operator ++(int)
        {
            FlaggedInt tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// FlaggedInt-- Operator
        /// </summary>
        FlaggedInt operator --(int)
        {
            FlaggedInt tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// FlaggedInt* Operator
        /// </summary>
        FlaggedInt& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	};
}