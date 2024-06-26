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
	class DLL_API FlaggedInt {
    public:
    #pragma region DigitStorage

		//Is either mixed fraction or power of representation if this value is one
		unsigned int IsAltRep:1;

		//Stores non-signed part of value
		unsigned int Value:31;

    #pragma endregion DigitStorage

    #pragma region class_constructors

		FlaggedInt(const unsigned int& value=0, const unsigned int& isAltRep=0);

		FlaggedInt(const signed int& value, const unsigned int& isAltRep=0);


        FlaggedInt(const FlaggedInt& rhs);

        FlaggedInt& operator=(const FlaggedInt& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            Value = rhs.Value; IsAltRep = rhs.IsAltRep;
            return *this;
        }

        FlaggedInt& operator=(const unsigned int& rhs)
        {
		    Value = rhs;
            IsAltRep = 0;
            return *this;
        }

        FlaggedInt& operator=(const signed int& rhs)
        {
		    Value = rhs;
            IsAltRep = 0;
            return *this;
        }

    #pragma endregion class_constructors

    #pragma region Check_if_value

        inline bool IsAlternative() const;

		inline bool IsNormal();

		inline void SwitchToAlternative();

		inline void SwitchToNormal();

        //Is at zero value
        inline bool IsAtZero() const;

        //Is not at zero value
        inline bool IsNotAtZero() const;

        //Is at one value
        inline bool IsAtOne() const;

        //Is at neither zero or negative one
        inline bool IsNotOne() const;

		inline bool IsEven() const;

		inline bool IsOdd() const;

    #pragma endregion Check_if_value

#pragma region StringOperations

		inline void ReadString(const std::string& value);

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedInt"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        FlaggedInt(const char* strVal);

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedInt"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        FlaggedInt(const std::string& Value);

        inline std::string ToString() const;

        inline std::string ToDetailedString(const PartialInt& DecHalf) const;

        explicit operator std::string();

#pragma endregion StringOperations

    #pragma region Comparison Operators

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

    #pragma endregion Comparison Operators

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

        static const FlaggedInt Maximum;

        static const FlaggedInt AltMaximum;

        static const FlaggedInt One;

        static const FlaggedInt Two;

		static const FlaggedInt Zero;

private:

		void UInt64DivOp(const unsigned __int64& rValue);

		void Int64DivOp(const signed __int64& rValue);

		void UInt64MultOp(const unsigned __int64& rValue);

		void Int64MultOp(const signed __int64& rValue);

public:

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