// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#ifdef BlazesSharedCode_LocalLayout
	#ifndef DLL_API
		#ifdef UsingBlazesSharedCodeDLL
			#define DLL_API __declspec(dllimport)
		#elif defined(BLAZESSharedCode_LIBRARY)
			#define DLL_API __declspec(dllexport)
		#else
			#define DLL_API
		#endif
	#endif
#else
	#include "..\..\DLLAPI.h"
#endif

#include <compare>
#include "..\Concepts\IntegerConcept.hpp"
#include "..\..\OtherFunctions\VariableConversionFunctions.h"

namespace BlazesRusCode
{
	struct FlaggedInt {public:
		#pragma options align=bit_packed
		//Is either mixed fraction or power of representation if this value is one 
		unsigned int IsAltRep:1;
		//Stores non-signed part of value
		unsigned int Value:31;
		#pragma options align=reset
		
		FlaggedInt(const unsigned int& value=0, const unsigned int& isAltRep=0)
		{
			Value = value;
			IsAltRep = isAltRep;
		}
		
		bool IsAlternative()
		{
			return IsAltRep==1;
		}
		
		bool IsNormal()
		{
			return IsAltRep==0;
		}
		
		void SwitchToAlternative()
		{
			IsAltRep = 1;
		}
		
		void SwitchToNormal()
		{
			IsAltRep = 0;
		}
		
        //Is at zero value
        bool IsAtZero()
        {
			return Value==0;
        }

        //Is not at zero value
        bool IsNotAtZero()
        {
			return Value!=0;
        }
		
        //Is at one value
        bool IsAtOne()
        {
			return Value==1;
        }

        //Is at neither zero or negative one
        bool IsNotOne()
        {
			return Value!=1;
        }
		
		bool IsEven()
		{
			return (Value&1)==0;
		}
		
		bool IsOdd()
		{
			return (Value&1)==1;
		}
		
		std::strong_ordering operator<=>(const FlaggedInt& that) const
		{
			if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
				return ValueCmp;
		}
		
		bool operator==(const FlaggedInt& that) const
		{
			if (Value!=that.Value)
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
			if (Value!=that)
				return false;
			return true;
		}
		
		bool operator==(const signed int& that) const
		{
			if (Value!=that)
				return false;
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
        explicit operator unsigned long long() { return Value; }
		
        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return (signed int)Value; }
		
        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed long long() { return (signed long long) Value; }
		
	protected:
        /// <summary>
        /// Returns maximum stored value(2147483647)
        /// </summary>
        /// <returns>FlaggedInt</returns>
        static FlaggedInt MaximumValue()
        {
            return FlaggedInt(2147483647,1);
        }
	
        /// <summary>
        /// Returns minimum stored value(-2147483647)
        /// </summary>
        /// <returns>FlaggedInt</returns>
        static FlaggedInt AltMaximumValue()
        {
            return FlaggedInt(2147483647,0);
        }
		
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>FlaggedInt</returns>
        static FlaggedInt OneValue()
        {
            return FlaggedInt(1,1);
        }
		
        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>FlaggedInt</returns>
        static FlaggedInt TwoValue()
        {
            return FlaggedInt(2,1);
        }
		
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>FlaggedInt</returns>
        static FlaggedInt ZeroValue()
        {
            return FlaggedInt(0,1);
        }
		
	public:

        static FlaggedInt Maximum;
		
        static FlaggedInt AltMaximum;
		
        static FlaggedInt One;
		
        static FlaggedInt Two;
		
		static FlaggedInt Zero;

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

    #pragma region Other Operators

        /// <summary>
        /// ++FlaggedInt Operator
        /// </summary>
        /// <returns>FlaggedInt &</returns>
        FlaggedInt& operator ++()
        {
            Value += 1;
            return *this;
        }

        /// <summary>
        /// ++FlaggedInt Operator
        /// </summary>
        /// <returns>FlaggedInt &</returns>
        FlaggedInt& operator --()
        {
            Value -= 1;
            return *this;
        }

        /// <summary>
        /// FlaggedInt++ Operator
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        FlaggedInt operator ++(int)
        {
            FlaggedInt tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// FlaggedInt-- Operator
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        FlaggedInt operator --(int)
        {
            FlaggedInt tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// FlaggedInt* Operator
        /// </summary>
        /// <returns>FlaggedInt &</returns>
        FlaggedInt& operator *()
        {
            return *this;
        }
		
    #pragma endregion Other Operators

#pragma region StringOperations

		/// <summary>
        /// MirroredInt to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string()
		{
            return VariableConversionFunctions::UIntToStringConversion(Value);
		}

#pragma endregion StringOperations

	};
	
	FlaggedInt FlaggedInt::Maximum = FlaggedInt::MaximumValue();
	FlaggedInt FlaggedInt::AltMaximum = FlaggedInt::AltMaximumValue();
	
	FlaggedInt FlaggedInt::One = FlaggedInt::OneValue();
	FlaggedInt FlaggedInt::Two = FlaggedInt::TwoValue();
}