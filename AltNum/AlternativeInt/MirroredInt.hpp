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
    class MirroredInt;

	class MirroredInt {
	public:
		#pragma options align=bit_packed
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
		#pragma options align=reset

    #if defined(AltNum_UseInvertedSign)
        static const unsigned int NegativeSign = 0;
        static const unsigned int PositiveSign = 1;
    #else
        static const unsigned int NegativeSign = 1;
        static const unsigned int PositiveSign = 0;
    #endif

		MirroredInt(const unsigned int& value=0, const unsigned int& sign=PositiveSign)
		{
			Value = value;
			Sign = sign;
		}

		bool IsNegative() const
		{
			return Sign==NegativeSign;
		}

		bool IsPositive() const
		{
			return Sign==PositiveSign;
		}

        void SetAsPositive()
        {
			Sign = PositiveSign;
        }

        void SetAsNegative()
        {
			Sign = NegativeSign;
        }

        void SetSignedValue(const signed int& val=0)
        {
			if(val<0){
				Sign = PositiveSign;
				Value = -val;
            } else {
                Sign = NegativeSign;
                Value = val;
            }
        }

    #if defined(AltNum_UseInvertedSign)
		//If Sign is one, then the sign is positive.
        //Otherwise, the sign is negative
    #else
		//If Sign is one, then the sign is negative.
        //Otherwise, the sign is positive
    #endif
        void SetValue(const unsigned int& value=0, const unsigned int& sign=PositiveSign)
        {
			Value = value;
			Sign = sign;
        }

        //Set as zero without changing sign(including negative zero)
        void SetAsZeroVal()
        {
			Value = 0;
        }

        //Return value as real number(negative zero counts as zero)
        int GetValue()
        {
            if(IsPositive())
                return (signed int)Value;
            else
                return -((signed int)Value);
        }

        void SetAsZero()
        {
			Value = 0; Sign = PositiveSign;
        }

        void SetAsNegativeZero()
        {
			Value = 0; Sign = NegativeSign;
        }
		
        //Is at either zero or negative zero
		bool IsAtZeroInt() const
        {
			return Value==0;
        }

        //Is at neither zero or negative zero
		bool IsNotAtZeroInt() const
        {
			return Value!=0;
        }
		
        //Is at either zero or negative one
		bool IsAtOneInt() const
        {
			return Value==1;
        }

        //Is at neither zero or negative one
		bool IsNotAtOneInt() const
        {
			return Value!=1;
        }
		
		bool IsEven() const
		{
			return (Value&1)==0;
		}
		
		bool IsOdd() const
		{
			return (Value&1)==1;
		}

        //Returns copy of value as Absolute value
        MirroredInt Abs() const
        {
            if(IsPositive())
                return *this;
            else
                return MirroredInt(Value);
        }
		
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
			if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
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
			if (auto ValueCmp = Value <=> that; ValueCmp != 0)
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
    			if (auto ValueCmp = Value <=> (unsigned int)(-that); ValueCmp != 0)
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
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return Value; }
		
        /// <summary>
        /// to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned long long() { return Value; }

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
            Sign ^= 1;
		}
		
	protected:

        /// <summary>
        /// Returns maximum stored value(2147483647)
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt MaximumValue()
        {
            return MirroredInt(2147483647,PositiveSign);
        }
	
        /// <summary>
        /// Returns minimum stored value(-2147483647)
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt MinimumValue()
        {
            return MirroredInt(2147483647,NegativeSign);
        }
		
        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt NegativeOneValue()
        {
            return MirroredInt(1,NegativeSign);
        }
		
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt OneValue()
        {
            return MirroredInt(1);
        }
		
        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt TwoValue()
        {
            return MirroredInt(2);
        }
		
        /// <summary>
        /// Returns the value at negative zero(for negative fractions)
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt NegativeZeroValue()
        {
            return MirroredInt(0,NegativeSign);
        }
		
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt ZeroValue()
        {
            return MirroredInt();
        }

	public:

        static MirroredInt Maximum;
		
        static MirroredInt Minimum;
		
        static MirroredInt NegativeOne;
		
        static MirroredInt One;
		
        static MirroredInt Two;
		
        static MirroredInt NegativeZero;
		
		static MirroredInt Zero;

        //Division operation
        void DivOp(const MirroredInt& rValue) {
			if(rValue.IsNegative()){
                SwapNegativeStatus();
                Value /= rValue.Value;
            }
            else
                Value /= rValue.Value;
        }

        void UIntDivOp(const unsigned int& rValue)
        {
            Value /= rValue;
        }

        void IntDivOp(const signed int& rValue)
        {
            if(rValue<0){
                SwapNegativeStatus();
                Value /= (unsigned int) -rValue;
            }
            else
                Value /= rValue;
        }

        //Multiplication operation
        void MultOp(const MirroredInt& rValue) {
			if(rValue.IsNegative()){
                SwapNegativeStatus();
                Value *= rValue.Value;
            }
            else
                Value *= rValue.Value;
        }

        void UIntMultOp(const unsigned int& rValue)
        {
            Value *= rValue;
        }

        void IntMultOp(const signed int& rValue)
        {
            if(rValue<0){
                SwapNegativeStatus();
                Value *= (unsigned int) -rValue;
            }
            else
                Value *= rValue;
        }

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void AddOp(const MirroredInt& rValue){
            if(IsPositive()){
				if(rValue.IsPositive())
					Value += rValue.Value;
				else if(rValue.Value>Value){//Becoming negative
					Sign = NegativeSign;
					Value = rValue.Value - Value;
				} else
					Value -= rValue.Value;
            } else {
				if(rValue.IsNegative())
					Value += rValue.Value;
				else if(rValue.Value>Value){//Becoming positive
					Sign = PositiveSign;
					Value = rValue.Value - Value - 1;
				} else
					Value -= rValue.Value;
            }
        }

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void UIntAddOp(const unsigned int& rValue){
            if(IsPositive())
				Value += rValue;
            else {
				if(rValue>Value){//Becoming positive
					Sign = PositiveSign;
					Value = rValue - Value - 1;
				} else
					Value -= rValue;
            }
        }

        //Default Negative zero including addition operation(When DecimalHalf.Value!=0)
        void IntAddOp(const signed int& rValue){
            if(IsPositive()){
				if(rValue>=0)
					Value += rValue;
				else {
                    auto negRValue = -rValue;
                    if(negRValue>Value){//Becoming negative
    					Sign = NegativeSign;
    					Value = negRValue - Value;
    				} else
    					Value += rValue;
                }
            } else {
				if(rValue<0)
					Value -= rValue;
				else if(rValue>Value){//Becoming positive
					Sign = PositiveSign;
					Value = rValue - Value - 1;
				} else
					Value -= rValue;
            }
        }

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void SubOp(const MirroredInt& rValue){
            if(IsPositive()){
				if(rValue.IsNegative())
					Value += rValue.Value;
				else if(rValue.Value>Value){//Becoming positive
					Sign = PositiveSign;
					Value = rValue.Value - Value - 1;
				} else
					Value -= rValue.Value;
            } else {
				if(rValue.IsPositive())
					Value += rValue.Value;
				else if(rValue.Value>Value){//Becoming negative
					Sign = NegativeSign;
					Value = rValue.Value - Value;
				} else
					Value -= rValue.Value;
            }
        }

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void UIntSubOp(const unsigned int& rValue){
            if(IsPositive()){
				if(rValue>Value){//Becoming positive
					Sign = PositiveSign;
					Value = rValue - Value - 1;
				} else
					Value -= rValue;
            } else
				Value += rValue;
        }

        //Default Negative zero including subtraction operation(When DecimalHalf.Value!=0)
        void IntSubOp(const signed int& rValue){
            if(IsPositive()){
				if(rValue<0)
					Value -= rValue;
				else if(rValue>Value){//Becoming positive
					Sign = PositiveSign;
					Value = rValue - Value - 1;
				} else
					Value -= rValue;
            } else {
				if(rValue>=0)
					Value += rValue;
				else {
                    auto negRValue = -rValue;
                    if(negRValue>Value){//Becoming negative
    					Sign = NegativeSign;
    					Value = negRValue - Value;
    				} else
    					Value += rValue;
                }
            }
        }

		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingAddOp(const MirroredInt& rValue){
            if(IsPositive()){
				if(rValue.IsPositive())
					Value += rValue.Value;
                else if (rValue.Value > Value) {//Becoming negative
                    Sign = NegativeSign;
                    Value = rValue.Value - Value;
                } else
					Value -= rValue.Value;
            } else {
				if(rValue.IsNegative())
					Value += rValue.Value;
                else if (rValue.Value >= Value) {//Becoming positive
                    Sign = PositiveSign;
                    Value = rValue.Value - Value;//Skipping negative zero
                } else
					Value -= rValue.Value;
            }
        }

		
		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingSubOp(const MirroredInt& rValue){
            if(IsPositive()){
				if(rValue.IsNegative())
					Value += rValue.Value;
                else if (rValue.Value >= Value) {//Becoming positive
                    Sign = PositiveSign;
                    Value = rValue.Value - Value;//Skipping negative zero
                } else
					Value -= rValue.Value;
            } else {
				if(rValue.IsPositive())
					Value += rValue.Value;
                else if (rValue.Value > Value) {//Becoming negative
                    Sign = NegativeSign;
                    Value = rValue.Value - Value;
                } else
					Value -= rValue.Value;
            }
        }

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

        //Including negative zero by default use NRepSkippingAddOp when DecimalHalf==0
		friend MirroredInt& operator+=(MirroredInt& lValue, const MirroredInt& rValue){
			lValue.AddOp(rValue); return lValue;
        }
		
		friend MirroredInt& operator+=(MirroredInt& lValue, const unsigned int& rValue){
			lValue.UIntAddOp(rValue); return lValue;
        }

		friend MirroredInt& operator+=(MirroredInt& lValue, const signed int& rValue){
			lValue.IntAddOp(rValue); return lValue;
        }

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
		friend MirroredInt& operator-=(MirroredInt& lValue, const MirroredInt& rValue){
			lValue.SubOp(rValue); return lValue;
        }

		friend MirroredInt& operator-=(MirroredInt& lValue, const unsigned int& rValue){
			lValue.UIntSubOp(rValue); return lValue;
        }

		friend MirroredInt& operator-=(MirroredInt& lValue, const signed int& rValue){
			lValue.IntSubOp(rValue); return lValue;
        }
		
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

    #pragma region Other Operators
	
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecVariant</returns>
        MirroredInt operator- ()
        {
			auto self = *this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// ++MirroredInt Operator
        /// </summary>
        /// <returns>MirroredInt &</returns>
        MirroredInt& operator ++()
        {
            UIntAddOp(1);
            return *this;
        }

        /// <summary>
        /// ++MirroredInt Operator
        /// </summary>
        /// <returns>MirroredInt &</returns>
        MirroredInt& operator --()
        {
            UIntSubOp(1);
            return *this;
        }

        /// <summary>
        /// MirroredInt++ Operator
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        MirroredInt operator ++(int)
        {
            MirroredInt tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MirroredInt-- Operator
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        MirroredInt operator --(int)
        {
            MirroredInt tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MirroredInt* Operator
        /// </summary>
        /// <returns>MirroredInt &</returns>
        MirroredInt& operator *()
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
            if(IsNegative())
                return "-"+VariableConversionFunctions::UIntToStringConversion(Value);
            else
                return VariableConversionFunctions::UIntToStringConversion(Value);
		}

#pragma endregion StringOperations

	};
	
	MirroredInt MirroredInt::NegativeZero = MirroredInt::NegativeZeroValue();
    MirroredInt MirroredInt::Zero = MirroredInt::ZeroValue();
	MirroredInt MirroredInt::Maximum = MirroredInt::MaximumValue();
	MirroredInt MirroredInt::Minimum = MirroredInt::MinimumValue();
	
	MirroredInt MirroredInt::NegativeOne = MirroredInt::NegativeOneValue();
	MirroredInt MirroredInt::One = MirroredInt::OneValue();
	MirroredInt MirroredInt::Two = MirroredInt::TwoValue();
}