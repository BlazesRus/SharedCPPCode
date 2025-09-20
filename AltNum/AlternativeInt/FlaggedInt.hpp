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

  // Is either mixed fraction or power-of representation if this value is one
  unsigned int IsAltRep : 1;

  // Stores non-signed part of value
  unsigned int Value : 31;

#pragma endregion DigitStorage

#pragma region class_constructors

  constexpr FlaggedInt(const unsigned int& value = 0, const unsigned int& isAltRep = 0)
  : IsAltRep(isAltRep), Value(value) {}

  constexpr FlaggedInt(const signed int& value, const unsigned int& isAltRep = 0)
  : IsAltRep(isAltRep), Value(value < 0 ? static_cast<unsigned int>(-value) : static_cast<unsigned int>(value)) {}

  constexpr FlaggedInt(const FlaggedInt& rhs)
  : IsAltRep(rhs.IsAltRep), Value(rhs.Value) {}

  constexpr FlaggedInt& operator=(const FlaggedInt& rhs)
  {
    if (this != &rhs)
    {
      Value = rhs.Value;
      IsAltRep = rhs.IsAltRep;
    }
    return *this;
  }

  constexpr FlaggedInt& operator=(const unsigned int& rhs)
  {
    Value = rhs;
    IsAltRep = 0;
    return *this;
  }

  constexpr FlaggedInt& operator=(const signed int& rhs)
  {
    Value = rhs < 0 ? static_cast<unsigned int>(-rhs) : static_cast<unsigned int>(rhs);
    IsAltRep = 0;
    return *this;
  }

  constexpr void SetValueV2(const FlaggedInt& rValue)
  {
    Value = rValue.Value;
    IsAltRep = rValue.IsAltRep;
  }

  constexpr void SetValue(const unsigned int& value = 0, const unsigned int& isAltRep = 0)
  {
    Value = value;
    IsAltRep = isAltRep;
  }

#pragma endregion class_constructors

#pragma region Comparison Operators

  constexpr std::strong_ordering operator<=>(const FlaggedInt& that) const
  {
    if (auto ValueCmp = Value <=> that.Value; ValueCmp != 0)
      return ValueCmp;
    return IsAltRep <=> that.IsAltRep;
  }

  constexpr std::strong_ordering operator<=>(const unsigned int& that) const
  {
    return Value <=> that;
  }

  constexpr std::strong_ordering operator<=>(const signed int& that) const
  {
    return Value <=> static_cast<unsigned int>(that);
  }

  constexpr bool operator==(const FlaggedInt& that) const
  {
    return (Value == that.Value) && (IsAltRep == that.IsAltRep);
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