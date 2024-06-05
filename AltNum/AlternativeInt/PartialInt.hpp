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
	struct PartialInt {public:
		//Stores Digits XXX XXX XXX
		unsigned int Value:30;
		//Can store up to 4 Flag states including normal state at 0
		unsigned int Flags:2;
		
        static const unsigned int MaximumInt = 1073741823;

		PartialInt(unsigned int value=0, unsigned int flags=0);
		
		bool IsNormalVariant() const;

		bool IsPiVariant() const;

		bool IsEVariant() const;

		bool IsIVariant() const;

		void SwitchToNormal();

		void SwitchToPiVariant();

		void SwitchToEVariant();

		void SwitchToIVariant();

        //Is at zero value
        bool IsAtZero() const;

        //Is not at zero value
        bool IsNotAtZero() const;
		
        //Is at one value
        bool IsAtOne() const;
		
		bool IsEven() const;
		
		bool IsOdd() const;

		std::strong_ordering operator<=>(const PartialInt& that) const
		{
			if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
				return ValueCmp;
		}
		
		bool operator==(const PartialInt& that) const
		{
			if (Value!=that.Value)
				return false;
            if(Flags!=that.Flags)
                return false;
			return true;
		}

		bool operator!=(const PartialInt& that) const
		{
			if (Value!=that.Value)
				return true;
            if(Flags!=that.Flags)
                return true;
			return false;
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

		std::strong_ordering operator<=>(const signed int& that) const
		{
			if (that < 0){
				if (auto ValueCmp = Value <=> 0; ValueCmp != 0)
					return ValueCmp;
			} else if (auto ValueCmp = Value <=> (unsigned int)that; ValueCmp != 0)
				return ValueCmp;
		}

		bool operator==(const signed int& that) const
		{
			if (that < 0)
				return false;
			else if (Value==(unsigned int)that)
				return true;
		}

		bool operator!=(const signed int& that) const
		{
			if (that < 0)
				return false;
			else if (Value!=(unsigned int)that)
				return true;
		}
		
        /// <summary>
        /// to signed int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return Value; }
		
        /// <summary>
        /// to signed int 64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned __int64() { return (signed __int64) Value; }

        /// <summary>
        /// to signed int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return (signed int)Value; }
		
        /// <summary>
        /// to signed int 64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed __int64() { return (signed __int64)Value; }

protected:
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MirroredInt</returns>
        static PartialInt ZeroValue();

public:

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned int const DecimalOverflow = 1000000000;

		static PartialInt Zero;

		void UInt64DivOp(const unsigned __int64& rValue);

		void Int64DivOp(const signed __int64& rValue);

		void UInt64MultOp(const unsigned __int64& rValue);

		void Int64MultOp(const signed __int64& rValue);

 		friend PartialInt& operator/=(PartialInt& lValue, const PartialInt& rValue){
			lValue.Value /= rValue.Value; return lValue;
        }

        friend PartialInt& operator/=(PartialInt& lValue, const unsigned int& rValue){
            lValue.Value /= rValue; return lValue;
        }

		friend PartialInt& operator/=(PartialInt& lValue, const signed int& rValue){
			lValue.Value /= rValue; return lValue;
        }
		
		friend PartialInt operator/(const PartialInt& lValue, const PartialInt& rValue){
            PartialInt newVal = lValue;
			newVal.Value /= rValue.Value; return newVal;
        }

		friend PartialInt operator/(const PartialInt& lValue, const unsigned int& rValue){
            PartialInt newVal = lValue;
			newVal.Value /= rValue; return newVal;
        }
		
		friend PartialInt operator/(const PartialInt& lValue, const signed int& rValue){
            PartialInt newVal = lValue;
			newVal.Value /= rValue; return newVal;
        }

        friend PartialInt& operator*=(PartialInt& lValue, const PartialInt& rValue){
            lValue.Value *= rValue.Value; return lValue;
        }

        friend PartialInt& operator*=(PartialInt& lValue, const unsigned int& rValue){
            lValue.Value *= rValue; return lValue;
        }

        friend PartialInt& operator*=(PartialInt& lValue, const signed int& rValue){
            lValue.Value *= rValue; return lValue;
        }
		
		friend PartialInt operator*(const PartialInt& lValue, const PartialInt& rValue){
            PartialInt newVal = lValue;
			newVal.Value *= rValue.Value; return newVal;
        }

		friend PartialInt operator*(const PartialInt& lValue, const unsigned int& rValue){
            PartialInt newVal = lValue;
			newVal.Value *= rValue; return newVal;
        }
		
		friend PartialInt operator*(const PartialInt& lValue, const signed int& rValue){
            PartialInt newVal = lValue;
			newVal.Value *= rValue; return newVal;
        }

		friend PartialInt& operator+=(PartialInt& lValue, const PartialInt& rValue){
			lValue.Value += rValue.Value; return lValue;
        }
		
		friend PartialInt& operator+=(PartialInt& lValue, const unsigned int& rValue){
			lValue.Value += rValue; return lValue;
        }

		friend PartialInt& operator+=(PartialInt& lValue, const signed int& rValue){
			lValue.Value += rValue; return lValue;
        }

		friend PartialInt operator+(const PartialInt& lValue, const PartialInt& rValue){
            PartialInt newVal = lValue;
			newVal.Value += rValue.Value; return newVal;
        }

		friend PartialInt operator+(const PartialInt& lValue, const unsigned int& rValue){
            PartialInt newVal = lValue;
			newVal.Value += rValue; return newVal;
        }
		
		friend PartialInt operator+(const PartialInt& lValue, const signed int& rValue){
            PartialInt newVal = lValue;
			newVal.Value += rValue; return newVal;
        }

		friend PartialInt& operator-=(PartialInt& lValue, const PartialInt& rValue){
            lValue.Value -= rValue.Value; return lValue;
        }

		friend PartialInt& operator-=(PartialInt& lValue, const unsigned int& rValue){
            lValue.Value -= rValue; return lValue;
        }

		friend PartialInt& operator-=(PartialInt& lValue, const signed int& rValue){
            lValue.Value -= rValue; return lValue;
        }
		
		friend PartialInt operator-(const PartialInt& lValue, const PartialInt& rValue){
            PartialInt newVal = lValue;
			newVal.Value -= rValue.Value; return newVal;
        }

		friend PartialInt operator-(const PartialInt& lValue, const unsigned int& rValue){
            PartialInt newVal = lValue;
			newVal.Value -= rValue; return newVal;
        }
		
		friend PartialInt operator-(const PartialInt& lValue, const signed int& rValue){
            PartialInt newVal = lValue;
			newVal.Value -= rValue; return newVal;
        }

        friend PartialInt& operator/=(PartialInt& lValue, const signed __int64& rValue) {
            lValue.Int64DivOp(rValue); return lValue;
        }

        friend PartialInt& operator/=(PartialInt& lValue, const unsigned __int64& rValue) {
            lValue.UInt64DivOp(rValue);
            return lValue;
        }

        friend PartialInt operator/(const PartialInt& lValue, const unsigned __int64& rValue) {
            PartialInt newVal = lValue;
            newVal.UInt64DivOp(rValue); return newVal;
        }

        friend PartialInt operator/(const PartialInt& lValue, const signed __int64& rValue) {
            PartialInt newVal = lValue;
            newVal.Int64DivOp(rValue); return newVal;
        }

        friend PartialInt& operator*=(PartialInt& lValue, const signed __int64& rValue) {
            lValue.Int64MultOp(rValue); return lValue;
        }

        friend PartialInt& operator*=(PartialInt& lValue, const unsigned __int64& rValue) {
            lValue.UInt64MultOp(rValue); return lValue;
        }

        friend PartialInt operator*(const PartialInt& lValue, const unsigned __int64& rValue) {
            PartialInt newVal = lValue;
            newVal.UInt64MultOp(rValue); return newVal;
        }

        friend PartialInt operator*(const PartialInt& lValue, const signed __int64& rValue) {
            PartialInt newVal = lValue;
            newVal.Int64MultOp(rValue); return newVal;
        }

    #pragma region Other Operators

        /// <summary>
        /// ++PartialInt Operator
        /// </summary>
        /// <returns>PartialInt &</returns>
        PartialInt& operator ++()
        {
            Value += 1;
            return *this;
        }

        /// <summary>
        /// ++PartialInt Operator
        /// </summary>
        /// <returns>PartialInt &</returns>
        PartialInt& operator --()
        {
            Value -= 1;
            return *this;
        }

        /// <summary>
        /// PartialInt++ Operator
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        PartialInt operator ++(int)
        {
            PartialInt tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// PartialInt-- Operator
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        PartialInt operator --(int)
        {
            PartialInt tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// PartialInt* Operator
        /// </summary>
        /// <returns>PartialInt &</returns>
        PartialInt& operator *()
        {
            return *this;
        }
		
    #pragma endregion Other Operators

#pragma region StringOperations

		/// <summary>
        /// MirroredInt to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string();

#pragma endregion StringOperations

	};

}