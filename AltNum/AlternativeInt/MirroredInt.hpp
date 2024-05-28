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
#include "..\DLLAPI.h"
#endif


#include <compare>
#include "..\IntegerConcept.hpp"

namespace BlazesRusCode
{
    class MirroredInt;

	class MirroredInt {
	public:
		#pragma options align=bit_packed
		//If value is one then is negative
		unsigned int IsPositive:1;
		//Stores non-signed part of value
		unsigned int Value:31;
		#pragma options align=reset

		MirroredInt(const unsigned int& value=0, const unsigned int& isPositive=1)
		{
			Value = value;
			IsPositive = isPositive;
		}

		bool IsNegative() const
		{
			return IsPositive!=1;
		}

		bool IsPositive() const
		{
			return IsPositive==1;
		}

        void SetSignedValue(const signed int& val=0)
        {
			if(val<0){
				IsPositive = 0;
				Value = -val;
            } else {
                IsPositive = 1;
                Value = val;
            }
        }

        void SetValue(const unsigned int& value=0, const unsigned int& isPositive=1)
        {
			Value = value;
			IsPositive = isPositive;
        }

        //Set as zero without changing sign(including negative zero)
        void SetAsZeroVal()
        {
			Value = 0;
        }

        //Return value as real number(negative zero counts as zero)
        int GetValue()
        {
            if(IsPositive==1)
                return (signed int)Value;
            else
                return -((signed int)Value);
        }

        void SetAsZero()
        {
			Value = 0; IsPositive = 1;
        }

        void SetAsNegativeZero()
        {
			Value = 0; IsPositive = 0;
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
            if(IsPositive==1)
                return *this;
            else
                return MirroredInt(Value);
        }
		
		std::strong_ordering operator<=>(const MirroredInt& that) const
		{
			//Comparing if number is negative vs positive
			if (auto SignCmp = IsPositive <=> that.IsPositive; SignCmp != 0)
				return SignCmp;
			if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
				return ValueCmp;
		}
		
		bool operator==(const MirroredInt& that) const
		{
			if (Value!=that.Value)
				return false;
			if (IsPositive!=that.IsPositive)
				return false;
			return true;
		}

		/*
        bool operator==(const unsigned int& that) const
		{
			if (Value!=that)
				return false;
			return true;
		}
        */

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
            IsPositive ^= 1;
		}
		
	protected:

        /// <summary>
        /// Returns maximum stored value(2147483647)
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt MaximumValue()
        {
            return MirroredInt(2147483647,1);
        }
	
        /// <summary>
        /// Returns minimum stored value(-2147483647)
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt MinimumValue()
        {
            return MirroredInt(2147483647,0);
        }
		
        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt NegativeOneValue()
        {
            return MirroredInt(1,0);
        }
		
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt OneValue()
        {
            return MirroredInt(1,1);
        }
		
        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt TwoValue()
        {
            return MirroredInt(2,1);
        }
		
        /// <summary>
        /// Returns the value at negative zero(for negative fractions)
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt NegativeZeroValue()
        {
            return MirroredInt(0,0);
        }
		
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MirroredInt</returns>
        static MirroredInt ZeroValue()
        {
            return MirroredInt(0,1);
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

        template<IntegerType IntType=unsigned int>
        void UIntDivOp(const IntType& rValue)
        {
            Value /= rValue;
        }

        template<IntegerType IntType=signed int>
        void IntDivOpV1(const IntType& rValue)
        {
            if(rValue<0){
                SwapNegativeStatus();
                Value /= (unsigned int) -rValue;
            }
            else
                Value /= rValue;
        }

        //Multiplication operation
        void MultOpV1(const MirroredInt& rValue) {
			if(rValue.IsNegative()){
                SwapNegativeStatus();
                Value *= rValue.Value;
            }
            else
                Value *= rValue.Value;
        }

        template<IntegerType IntType=unsigned int>
        void UIntMultOpV1(const IntType& rValue)
        {
            Value *= rValue;
        }

        template<IntegerType IntType=signed int>
        void IntMultOpV1(const IntType& rValue)
        {
            if(rValue<0){
                SwapNegativeStatus();
                Value *= (unsigned int) -rValue;
            }
            else
                Value *= rValue;
        }

        //Default Negative zero including Addition operation(When DecimalHalf.Value!=0)
        void AddOp(const MirroredInt& rValue){
            if(IsPositive==1){
				if(rValue.IsPositive==1)
					Value += rValue.Value;
				else if(rValue.Value>Value){//Becoming negative
					IsPositive = 0;
					Value = rValue.Value - Value;
				} else
					Value -= rValue.Value;
            } else {
				if(rValue.IsPositive==0)
					Value += rValue.Value;
				else if(rValue.Value>Value){//Becoming positive
					IsPositive = 1;
					//Exclude negative zero version(When DecimalHalf.Value==0):
					//Value = rValue.Value - Value;
					Value = rValue.Value - Value - 1;
				} else
					Value -= rValue.Value;
            }
        }

		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingAddOp(const MirroredInt& rValue){
            if(IsPositive==1){
				if(rValue.IsPositive==1)
					Value += rValue.Value;
                else if (rValue.Value > Value) {//Becoming negative
                    IsPositive = 0;
                    Value = rValue.Value - Value;
                } else
					Value -= rValue.Value;
            } else {
				if(rValue.IsPositive==0)
					Value += rValue.Value;
                else if (rValue.Value >= Value) {//Becoming positive
                    IsPositive = 1;
                    Value = rValue.Value - Value;//Skipping negative zero
                } else
					Value -= rValue.Value;
            }
        }

		
		//Exclude negative zero version(When DecimalHalf.Value==0)
        void NRepSkippingSubOp(const MirroredInt& rValue){
            if(IsPositive==1){
				if(rValue.IsPositive==0)
					Value += rValue.Value;
                else if (rValue.Value >= Value) {//Becoming positive
                    IsPositive = 1;
                    Value = rValue.Value - Value;//Skipping negative zero
                } else
					Value -= rValue.Value;
            } else {
				if(rValue.IsPositive==1)
					Value += rValue.Value;
                else if (rValue.Value > Value) {//Becoming negative
                    IsPositive = 0;
                    Value = rValue.Value - Value;
                } else
					Value -= rValue.Value;
            }
        }

    #if defined(MirroredInt_EnableOverrides)

 		friend MirroredInt& operator/=(MirroredInt& lValue, const MirroredInt& rValue){
			if(rValue.IsNegative()){
                SwapNegativeStatus();
                Value /= rValue.Value;
            }
            else
                Value /= rValue;
            return lValue;
        }
		
		friend MirroredInt& operator/=(MirroredInt& lValue, const signed int& rValue){
            if(rValue<0){
                SwapNegativeStatus();
                Value /= (unsigned int) -rValue;
            }
            else
                Value /= rValue;
            return lValue;
        }

        friend MirroredInt& operator/=(MirroredInt& lValue, const unsigned int& rValue){
            Value /= rValue;
            return lValue;
        }
		
		friend MirroredInt operator/(const MirroredInt& lValue, const MirroredInt& rValue){
            MirroredInt newVal = lValue;
			newVal /= rValue; return newVal;
        }

		friend MirroredInt operator/(const MirroredInt& lValue, const unsigned int& rValue){
            MirroredInt newVal = lValue;
			newVal /= rValue; return newVal;
        }
		
		friend MirroredInt operator/(const MirroredInt& lValue, const signed int& rValue){
            MirroredInt newVal = lValue;
			newVal /= rValue; return newVal;
        }

        friend MirroredInt& operator*=(MirroredInt& lValue, const MirroredInt& rValue){
            if(rValue<0){
                SwapNegativeStatus();
                lValue.Value *= (unsigned int) -rValue;
            }
            else
                lValue.Value *= rValue;
            return *this;
        }

        friend MirroredInt& operator*=(MirroredInt& lValue, const signed int& rValue){
            if(rValue<0){
                SwapNegativeStatus();
                lValue.Value *= (unsigned int) -rValue;
            }
            else
                lValue.Value *= rValue;
            return lValue;
        }

        friend MirroredInt& operator*=(MirroredInt& lValue, const unsigned int& rValue){
            lValue.Value *= rValue;
            return lValue;
        }
		
		friend MirroredInt operator*(const MirroredInt& lValue, const MirroredInt& rValue){
            MirroredInt newVal = lValue;
			newVal *= rValue; return newVal;
        }

		friend MirroredInt operator*(const MirroredInt& lValue, const unsigned int& rValue){
            MirroredInt newVal = lValue;
			newVal *= rValue; return newVal;
        }
		
		friend MirroredInt operator*(const MirroredInt& lValue, const signed int& rValue){
            MirroredInt newVal = lValue;
			newVal *= rValue; return newVal;
        }

        //Including negative zero by default use NRepSkippingAddOp when DecimalHalf==0
		friend MirroredInt& operator+=(MirroredInt& lValue, const MirroredInt& rValue){
			lValue.AddOp(rValue); return lValue;
        }
		
		friend MirroredInt operator+(const MirroredInt& lValue, const MirroredInt& rValue){
            MirroredInt newVal = *this;
			newVal += rValue; return newVal;
        }

		friend MirroredInt operator+(const MirroredInt& lValue, const unsigned int& rValue){
            MirroredInt newVal = *this;
			newVal *= rValue; return newVal;
        }
		
		friend MirroredInt operator+(const MirroredInt& lValue, const signed int& rValue){
            MirroredInt newVal = *this;
			newVal *= rValue; return newVal;
        }

    #endif


	};
	
	MirroredInt MirroredInt::NegativeZero = MirroredInt::NegativeZeroValue();
    MirroredInt MirroredInt::Zero = MirroredInt::ZeroValue();
	MirroredInt MirroredInt::Maximum = MirroredInt::MaximumValue();
	MirroredInt MirroredInt::Minimum = MirroredInt::MinimumValue();
	
	MirroredInt MirroredInt::NegativeOne = MirroredInt::NegativeOneValue();
	MirroredInt MirroredInt::One = MirroredInt::OneValue();
	MirroredInt MirroredInt::Two = MirroredInt::TwoValue();
}