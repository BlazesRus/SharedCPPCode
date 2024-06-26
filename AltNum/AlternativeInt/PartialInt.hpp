// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "..\..\DLLAPI.h"

#include <compare>
#include "..\Concepts\IntegerConcept.hpp"
#include "..\..\OtherFunctions\VariableConversionFunctions.h"
//#include "..\MediumDecV2\MediumDecV2Preprocessors.h"
//#include "..\AltDec\AltDecPreprocessors.h"

namespace BlazesRusCode
{
	struct DLL_API PartialInt {
    public:
    #pragma region DigitStorage

		//Stores Digits XXX XXX XXX
		unsigned int Value:30;

		//Can store up to 4 Flag states including normal state at 0
		unsigned int Flags:2;

    #pragma endregion DigitStorage

        static const unsigned int MaximumInt = 1073741823;

    #pragma region class_constructors

		PartialInt(unsigned int value=0, unsigned int flags=0);

		PartialInt(signed int value, unsigned int flags=0);

        PartialInt(const PartialInt& rhs);

        PartialInt& operator=(const PartialInt& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            Value = rhs.Value; Flags = rhs.Flags;
            return *this;
        }

        PartialInt& operator=(const unsigned int& rhs)
        {
		    Value = rhs;
            Flags = 0;
            return *this;
        }

        PartialInt& operator=(const signed int& rhs)
        {
		    Value = rhs;
            Flags = 0;
            return *this;
        }

    #pragma endregion class_constructors

    #pragma region Check_if_value

		inline bool IsNormalVariant() const;

		inline bool IsPiVariant() const;

		inline bool IsEVariant() const;

		inline bool IsIVariant() const;

		inline void SwitchToNormal();

		inline void SwitchToPiVariant();

		inline void SwitchToEVariant();

		inline void SwitchToIVariant();

        //Is at zero value
        inline bool IsAtZero() const;

        //Is not at zero value
        inline bool IsNotAtZero() const;

        //Is at one value
        inline bool IsAtOne() const;

		inline bool IsEven() const;

		inline bool IsOdd() const;

    #pragma endregion Check_if_value

	#pragma region StringOperations

		inline void ReadString(const std::string& value);

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialInt"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        PartialInt(const char* strVal);

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialInt"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        PartialInt(const std::string& Value);

        std::string ToString() const;

        std::string ToDetailedString() const;

        explicit operator std::string();

	#pragma endregion StringOperations

    #pragma region Comparison Operators

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
			auto ValueCmp = Value <=> that;
			return ValueCmp;
		}

		bool operator==(const unsigned int& that) const
		{
			if (Value==that)
				return true;
            return false;
		}

		bool operator!=(const unsigned int& that) const
		{
			if (Value!=that)
				return true;
			return false;
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
        /// to signed int explicit conversion
        /// </summary>
        explicit operator unsigned int() { return Value; }

        /// <summary>
        /// to signed int 64 explicit conversion
        /// </summary>
        explicit operator unsigned __int64() { return (unsigned __int64) Value; }

        /// <summary>
        /// to signed int explicit conversion
        /// </summary>
        explicit operator signed int() { return (signed int)Value; }

        /// <summary>
        /// to signed int 64 explicit conversion
        /// </summary>
        explicit operator signed __int64() { return (signed __int64)Value; }

protected:
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MirroredInt</returns>
        inline static PartialInt ZeroValue();

public:

#if !defined(AltNum_PreventExtra_PartialInt_Constants)

        /// <summary>
        /// The decimal overflow
        /// </summary>
        inline static unsigned int const DecimalOverflow = 1000000000;

	//#if defined(AltNum_EnablePiRep)
        //Pi*Value representation(when DecimalHalf.Flags==1)
        inline static const unsigned int PiRep = 1;
	//#endif
    //
	//#if defined(AltNum_EnableERep)
        //e*Value representation(DecimalHalf.Flags==2)
        inline static const unsigned int ERep = 2;
	//#endif
    //
	//#if defined(AltNum_EnableIRep)
        //e*Value representation(DecimalHalf.Flags==3)
        inline static const unsigned int IRep = 3;
	//#endif
    //
	//#if defined(AltNum_EnableInfinityRep)
        //When DecimalHalf.Value equals this value, it represents infinity (sign of IntHalf determines if either negative or positive inifity)
		inline static const unsigned int InfinityRep = 1073741805;
	//#endif
    //
	//#if defined(AltNum_EnableApproaching)
        //When DecimalHalf.Value equals this value, it represents Approaching IntHalf from right towards left (IntHalf.0..01)
        inline static const unsigned int ApproachingBottomRep = 1073741806;
        //When DecimalHalf.Value equals this value, it represents Approaching IntHalf from left towards right (IntHalf.9..9)
		inline static const unsigned int ApproachingTopRep = 1073741807;
	//#endif
    //
	//#if defined(AltNum_EnableApproachingDivided)
        //When DecimalHalf.Value equals this value, the DecimalHalf part equals DecimalOverflow/ExtraRep.Value-1
        inline static const unsigned int ApproachingMidLeftRep = 1073741808;
		//When DecimalHalf.Value equals this value, the DecimalHalf part equals DecimalOverflow/ExtraRep.Value+1
		inline static const unsigned int ApproachingMidRightRep = 1073741809;
	//#endif

#endif

		static const PartialInt Zero;

private:

		void UInt64DivOp(const unsigned __int64& rValue);

		void Int64DivOp(const signed __int64& rValue);

		void UInt64MultOp(const unsigned __int64& rValue);

		void Int64MultOp(const signed __int64& rValue);

public:

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

		friend PartialInt& operator%=(PartialInt& lValue, const unsigned int& rValue)
		{ lValue.Value %= rValue; return lValue; }

		friend PartialInt operator%(const PartialInt& lValue, const unsigned int& rValue){
            PartialInt newVal = lValue;
			newVal.Value %= rValue; return newVal;
        }

		friend PartialInt& operator%=(PartialInt& lValue, const PartialInt& rValue)
		{
			lValue.Value %= rValue.Value; return lValue;
		}

		friend PartialInt operator%(const PartialInt& lValue, const PartialInt& rValue){
            PartialInt newVal = lValue;
			return newVal%=rValue;
        }

    #pragma region Other Operators

        /// <summary>
        /// ++PartialInt Operator
        /// </summary>
        PartialInt& operator ++()
        {
            Value += 1;
            return *this;
        }

        /// <summary>
        /// ++PartialInt Operator
        /// </summary>
        PartialInt& operator --()
        {
            Value -= 1;
            return *this;
        }

        /// <summary>
        /// PartialInt++ Operator
        /// </summary>
        PartialInt operator ++(int)
        {
            PartialInt tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// PartialInt-- Operator
        /// </summary>
        PartialInt operator --(int)
        {
            PartialInt tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// PartialInt* Operator
        /// </summary>
        PartialInt& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	};
}