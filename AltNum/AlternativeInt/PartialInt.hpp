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

  // Stores Digits XXX XXX XXX
  unsigned int Value : 30;

  // Can store up to 4 Flag states including normal state at 0
  unsigned int Flags : 2;

#pragma endregion DigitStorage

  inline static constexpr unsigned int MaximumInt = 1073741823;

#pragma region class_constructors

  constexpr PartialInt(unsigned int value = 0, unsigned int flags = 0)
  : Value(value), Flags(flags) {}

  constexpr PartialInt(signed int value, unsigned int flags = 0)
  : Value(value < 0 ? static_cast<unsigned int>(-value) : static_cast<unsigned int>(value)),
  Flags(flags) {}

  constexpr PartialInt(const PartialInt& rhs)
  : Value(rhs.Value), Flags(rhs.Flags) {}

  constexpr PartialInt& operator=(const PartialInt& rhs)
  {
    if (this != &rhs)
    {
      Value = rhs.Value;
      Flags = rhs.Flags;
    }
    return *this;
  }

  constexpr PartialInt& operator=(const unsigned int& rhs)
  {
    Value = rhs;
    Flags = 0;
    return *this;
  }

  constexpr PartialInt& operator=(const signed int& rhs)
  {
    Value = rhs < 0 ? static_cast<unsigned int>(-rhs)
            : static_cast<unsigned int>(rhs);
    Flags = 0;
    return *this;
  }

  constexpr void SetValueV2(const PartialInt& rValue)
  {
    Value = rValue.Value;
    Flags = rValue.Flags;
  }

  constexpr void SetValue(unsigned int value = 0, unsigned int flags = 0)
  {
    Value = value;
    Flags = flags;
  }

#pragma endregion class_constructors

#pragma region Comparison Operators

  constexpr std::strong_ordering operator<=>(const PartialInt& that) const
  {
    if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
      return ValueCmp;
    return Flags <=> that.Flags;
  }

  constexpr std::strong_ordering operator<=>(const unsigned int& that) const
  {
    return Value <=> that;
  }

  constexpr std::strong_ordering operator<=>(const signed int& that) const
  {
    if (that < 0)
      return Value <=> 0u;
    return Value <=> static_cast<unsigned int>(that);
  }

  constexpr bool operator==(const PartialInt& that) const
  {
    return (Value == that.Value) && (Flags == that.Flags);
  }

  constexpr bool operator==(const unsigned int& that) const
  {
    return Value == that;
  }

  constexpr bool operator==(const signed int& that) const
  {
    return (that >= 0) && (Value == static_cast<unsigned int>(that));
  }

#pragma endregion Comparison Operators

#pragma region Conversions

  constexpr explicit operator unsigned int() const { return Value; }
  constexpr explicit operator unsigned __int64() const { return static_cast<unsigned __int64>(Value); }
  constexpr explicit operator signed int() const { return static_cast<signed int>(Value); }
  constexpr explicit operator signed __int64() const { return static_cast<signed __int64>(Value); }

#pragma endregion Conversions

#if !defined(AltNum_PreventExtra_PartialInt_Constants)
    inline static constexpr unsigned int DecimalOverflow = 1000000000;
    inline static constexpr unsigned int PiRep = 1;
    inline static constexpr unsigned int ERep = 2;
    inline static constexpr unsigned int IRep = 3;
    inline static constexpr unsigned int InfinityRep = 1073741805;
    inline static constexpr unsigned int ApproachingBottomRep = 1073741806;
    inline static constexpr unsigned int ApproachingTopRep = 1073741807;
    inline static constexpr unsigned int ApproachingMidLeftRep = 1073741808;
    inline static constexpr unsigned int ApproachingMidRightRep = 1073741809;
#endif

    inline static constexpr PartialInt Zero = PartialInt(0, 0);


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