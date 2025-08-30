// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "AltNumBase.h"

#include <string>
#include <cmath>
#include "..\OtherFunctions\VariableConversionFunctions.h"

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "..\Concepts\IntegerConcept.hpp"
#include "..\Concepts\MediumDecVariantConcept.hpp"
#include <boost/multiprecision/cpp_int.hpp>//Need to use the int 128 in order to fit all inside during multiplication

/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
*/
#include "MediumDecPreprocessors.h"
#include "RepType.h"

namespace BlazesRusCode
{
  using UInt128 = boost::multiprecision::uint128_t;
  using UInt64 = unsigned __int64;
  class ExpandedUDec;

  /// <summary>
  /// Alternative Non-Integer number representation with focus on accuracy and catching overflow-underflow and trailing digit loss for MixedDec varients
  /// Represents 0 - 18446744073709551615.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
  /// (16 bytes worth of Variable Storage inside class for each instance)
  class DLL_API ExpandedUDec
  {
  private:
    static unsigned _int64 const UInt64Max = 18446744073709551615;

    /// <summary>
    /// The decimal overflow
    /// </summary>
    static unsigned int const DecimalOverflow = 1000000000;
    
    /// <summary>
    /// The decimal overflow
    /// </summary>
    static UInt128 const DecimalOverflowX = 1000000000;
    
  public:

    //Performs remainder/Mod operation then saves division result
    class DLL_API ModResult : public AltNumModChecker<ExpandedUDec>{};
    
    /// <summary>
    /// long double (Extended precision double)
    /// </summary>
    using long double = long double;

    /// <summary>
    /// Stores whole half of number(Including positive/negative status)
    /// (in the case of infinity is used to determine if positive vs negative infinity)
    /// </summary>
    unsigned _int64 IntHalf;

    //Return IntHalf as signed int
    unsigned _int64 GetIntHalf()
    {
      return IntHalf;
    }

    bool IsNegative()
    {
      return false;
    }

    /// <summary>
    /// Stores decimal section info and other special info
    /// </summary>
    unsigned int DecimalHalf;

    //Divisor of Fractional
    unsigned int ExtraRep;

    static const unsigned int InitialExtraRep = 1;

    /// <summary>
    /// Initializes a new instance of the <see cref="ExpandedUDec"/> class.
    /// </summary>
    /// <param name="intVal">The whole number based half of the representation</param>
    /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
    ExpandedUDec(const unsigned _int64& intVal = 0, const unsigned _int64& decVal = 0, const unsigned int& extraRep = 1)
    {
      IntHalf = intVal;
      DecimalHalf = decVal;
      ExtraRep = extraRep;
    }

    ExpandedUDec(const ExpandedUDec&) = default;

    ExpandedUDec& operator=(const unsigned _int64& rhs)
    {
      IntHalf = rhs;
      DecimalHalf = 0;
      ExtraRep = InitialExtraRep;
      return *this;
    } const

    ExpandedUDec& operator=(const ExpandedUDec& rhs)
    {
      // Check for self-assignment
      if (this == &rhs)    // Same object?
        return *this;    // Yes, so skip assignment, and just return *this.
      IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
      ExtraRep = rhs.ExtraRep;
      return *this;
    } const

  #pragma region Check_if_Zero

    //Is at either zero or negative zero IntHalf of AltNum
    bool IsAtZeroInt()
    {
      return IntHalf==0;
    }

    bool IsNotAtZeroInt()
    {
      return IntHalf!=0;
    }

    //Detect if at exactly zero
    bool IsZero()
    {
      return DecimalHalf==0&&IntHalf==0;
    }

    void SetAsZero()
    {
      IntHalf = 0;
      DecimalHalf = 0; ExtraRep = InitialExtraRep;
    }

  #pragma endregion Check_if_Zero
    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetVal(const PartialInt& Value)
    {
      IntHalf = Value.IntHalf;
      DecimalHalf = Value.DecimalHalf;
      ExtraRep = Value.ExtraRep;
    }
    
    /// <summary>
    /// Swaps the negative status.(Does nothing)
    /// </summary>
    void SwapNegativeStatus(){}

    // For the IntHalf type (could be uint64_t or __int128 depending on your build)
		static unsigned _int64 gcd_int(unsigned _int64 a, unsigned _int64 b) noexcept {
				// Euclidean algorithm
				while (b != 0) {
						unsigned _int64 t = b;
						b = a % b;
						a = t;
				}
				return a;
		}
		
		unsigned _int64 gcd(unsigned _int64 b) noexcept {
				// Euclidean algorithm
				int attempt = 0;
				ExpandedUDec a = *this;
				ExpandedUDec bResult = b;
				ExpandedUDec t;
				while (bResult!=ExpandedUDec::Zero && ++attempt < 64) {
						t = bResult;
						bResult = a % bResult;
						a = t;
				}
				if(ExtraRep%a!=0)
				  return 0;
			  return a;
		}
		
    // Reduce numerator/denominator by their greatest common divisor
    void normalizeSafe() noexcept {
        auto g = gcd(ExtraRep);
        if (g > 1) {
				  PartialUInt64DivOpV1(g);
          ExtraRep /= g;
        }
    }

		void scaleDown(unsigned int d) {
				// Try gcd reduction before multiply
				auto g = gcd(*this, d);
				if (g > 1) { *this /= g; d /= g; }
				divisor *= d;
		}

  #pragma region Const Representation values
  protected:

  #pragma endregion Const Representation values

  #pragma region RepType

  #pragma endregion RepType

public:
  #pragma region RangeLimits

    /// <summary>
    /// Sets value to the highest non-infinite/Special Decimal State Value that it store
    /// </summary>
    void SetAsMaximum()
    {
      IntHalf = 18446744073709551615;
      DecimalHalf = 999999999;
      ExtraRep = InitialExtraRep;
    }
  
    /// <summary>
    /// Sets value to the lowest value that can be stored(zero)
    /// </summary>
    constexpr auto SetAsMinimum = SetAsZero;

  #pragma endregion RangeLimits

  #pragma region ValueDefines
    
    /// <summary>
    /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
    /// (Stored as 3.141592654)
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec PiValue()
    {
      return ExpandedUDec(3, 141592654);
    }

    //100,000,000xPi(Rounded to 9th decimal digit)
    static ExpandedUDec HundredMilPiNumVal()
    {
      return ExpandedUDec(314159265, 358979324);
    }

    //10,000,000xPi(Rounded to 9th decimal digit)
    static ExpandedUDec TenMilPiNumVal()
    {
      return ExpandedUDec(31415926, 535897932);
    }

    //1,000,000xPi(Rounded to 9th decimal digit)
    static ExpandedUDec OneMilPiNumVal()
    {
      return ExpandedUDec(3141592, 653589793);
    }

    //10xPi(Rounded to 9th decimal digit)
    static ExpandedUDec TenPiNumVal()
    {
      return ExpandedUDec(31, 415926536);
    }
    
    static ExpandedUDec ENumValue()
    {
      return ExpandedUDec(2, 718281828);
    }

    static ExpandedUDec EValue()
    {
      return ExpandedUDec(2, 718281828);
    }
    
    static ExpandedUDec ZeroValue()
    {
      return ExpandedUDec();
    }

    /// <summary>
    /// Returns the value at one
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec OneValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(1);
      return NewSelf;
    }

    /// <summary>
    /// Returns the value at one
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec TwoValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(2);
      return NewSelf;
    }

    /// <summary>
    /// Returns the value at 0.5
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec Point5Value()
    {
      ExpandedUDec NewSelf = ExpandedUDec(0, 500000000);
      return NewSelf;
    }

    static ExpandedUDec JustAboveZeroValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(0, 1);
      return NewSelf;
    }

    static ExpandedUDec OneMillionthValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(0, 1000);
      return NewSelf;
    }

    static ExpandedUDec FiveThousandthValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(0, 5000000);
      return NewSelf;
    }

    static ExpandedUDec FiveMillionthValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(0, 5000);
      return NewSelf;
    }

    static ExpandedUDec TenMillionthValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(0, 100);
      return NewSelf;
    }

    static ExpandedUDec OneHundredMillionthValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(0, 10);
      return NewSelf;
    }

    static ExpandedUDec FiveBillionthValue()
    {
      ExpandedUDec NewSelf = ExpandedUDec(0, 5);
      return NewSelf;
    }

    static ExpandedUDec LN10Value()
    {
      return ExpandedUDec(2, 302585093);
    }

    static ExpandedUDec TenthLN10Value()
    {
      return ExpandedUDec(0, 434294482);
    }

    static ExpandedUDec FifthLN10Value()
    {
      return ExpandedUDec(0, 868588964);
    }

    static ExpandedUDec MaximumValue()
    {
      return ExpandedUDec(18446744073709551615, 999999999);
    }
public:
    static ExpandedUDec AlmostOne;

    /// <summary>
    /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec PiNum;
    
    /// <summary>
    /// Euler's number (Non-Alternative Representation)
    /// Irrational number equal to about (1 + 1/n)^n
    /// (about 2.71828182845904523536028747135266249775724709369995)
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec ENum;

    /// <summary>
    /// Returns Pi(3.1415926535897932384626433) Representation
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec Pi;
    
    /// <summary>
    /// Euler's number (Non-Alternative Representation)
    /// Irrational number equal to about (1 + 1/n)^n
    /// (about 2.71828182845904523536028747135266249775724709369995)
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec E;
    
    /// <summary>
    /// Returns the value at zero
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec Zero;
    
    /// <summary>
    /// Returns the value at one
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec One;

    /// <summary>
    /// Returns the value at two
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec Two;

    /// <summary>
    /// Returns the value at 0.5
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec PointFive;

    /// <summary>
    /// Returns the value at digit one more than zero (0.000000001)
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec JustAboveZero;

    /// <summary>
    /// Returns the value at .000000005
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec FiveBillionth;

    /// <summary>
    /// Returns the value at .000001000
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec OneMillionth;

    /// <summary>
    /// Returns the value at "0.005"
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec FiveThousandth;

    /// <summary>
    /// Returns the value at .000000010
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    static ExpandedUDec OneGMillionth;

    //0e-7
    static ExpandedUDec TenMillionth;

    /// <summary>
    /// Returns the value at "0.000005"
    /// </summary>
    static ExpandedUDec FiveMillionth;

    /// <summary>
    /// Returns value of lowest non-infinite/Special Decimal State Value that can store
    /// (-2147483647.999999999)
    /// </summary>
    static ExpandedUDec Minimum;
    
    /// <summary>
    /// Returns value of highest non-infinite/Special Decimal State Value that can store
    /// (2147483647.999999999)
    /// </summary>
    static ExpandedUDec Maximum;
    
    /// <summary>
    /// 2.3025850929940456840179914546844
    /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
    /// </summary>
    static ExpandedUDec LN10;

    /// <summary>
    /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    static ExpandedUDec TenthLN10;

    /// <summary>
    /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    static ExpandedUDec FifthLN10;
  #pragma endregion ValueDefines

  #pragma region String Commands
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    virtual void ReadString(const std::string& Value);

    /// <summary>
    /// Gets the value from string.
    /// </summary>
    /// <param name="Value">The value.</param>
    /// <returns>ExpandedUDec</returns>
    template<MediumDecVariant VariantType=ExpandedUDec>
    VariantType GetValueFromString(std::string Value)
    {

    }

    /// <summary>
    /// Initializes a new instance of the <see cref="ExpandedUDec"/> class from string literal
    /// </summary>
    /// <param name="strVal">The value.</param>
    ExpandedUDec(const char* strVal)
    {
      std::string Value = strVal;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="ExpandedUDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    ExpandedUDec(const std::string& Value)
    {
      this->ReadString(Value);
    }

//private:
    //std::string BasicToStringOp();
    
    //std::string BasicToFullStringOp();
public:

    /// <summary>
    /// Converts to string.
    /// </summary>
    /// <returns>std.string</returns>
    virtual std::string ToString();

    /// <summary>
    /// Converts to string with digits filled in even when empty
    /// </summary>
    /// <returns>std.string</returns>
    virtual std::string ToFullString();

    /// <summary>
    /// Implements the operator std::string operator.
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator std::string() { return ToString(); }

  #pragma endregion String Commands

  #pragma region ConvertFromOtherTypes
    //Using just Legaacy floating conversion for now since not important for this variant
  
    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetFloatVal(const float& Value)
    {
      if (Value < 0.0f) { SetAsZero(); }
      else if (Value >= 18446744073709551616f){//Cap value if too big on initialize (preventing overflow on conversion)
        IntHalf = UInt64Max;
        DecimalHalf = 999999999;
      }
      else
      {
        float lValue = Value;
        signed __int64 WholeValue = (signed __int64)std::floor(Value);
        lValue -= (float)WholeValue;
        DecimalHalf = (unsigned int)Value * 10000000000;
        IntHalf = (unsigned int)WholeValue;
      }
    }

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetDoubleVal(const double& Value)
    {
      if (Value < 0.0) { SetAsZero(); }
      else if (Value >= 18446744073709551616){//Cap value if too big on initialize (preventing overflow on conversion)
        IntHalf = UInt64Max;
        DecimalHalf = 999999999;
      }
      else
      {
        auto lValue = Value;
        UInt64 WholeValue = (UInt64)std::floor(Value);
        lValue -= (double)WholeValue;
        DecimalHalf = (unsigned int)Value * 10000000000;
        IntHalf = (unsigned int)WholeValue;
      }
    }

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetDecimalVal(const long double& Value)
    {
      if (Value < 0.0L) { SetAsZero(); }
      else if (Value >= 18446744073709551616L){//Cap value if too big on initialize (preventing overflow on conversion)
        IntHalf = UInt64Max;
        DecimalHalf = 999999999;
      }
      else
      {
        auto lValue = Value;
        signed __int64 WholeValue = (UInt64)std::floor(lValue);
        lValue -= (long double)WholeValue;
        DecimalHalf = (unsigned int)lValue * 10000000000;
        IntHalf = (unsigned int)WholeValue;
      }
    }

    /// <summary>
    /// Sets the value(false equals zero; otherwise is true).
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetBoolVal(const bool& Value)
    {
      IntHalf = Value==false ? 0 : 1;
      DecimalHalf = 0;
    }

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetIntVal(UInt64& Value)
    {
      IntHalf = Value;
      DecimalHalf = 0;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="ExpandedUDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    ExpandedUDec(const float& Value)
    {
      this->SetFloatVal(Value);
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="ExpandedUDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    ExpandedUDec(const double& Value)
    {
      this->SetDoubleVal(Value);
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="ExpandedUDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    ExpandedUDec(const long double& Value)
    {
      this->SetDecimalVal(Value);
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="ExpandedUDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    ExpandedUDec(const bool& Value)
    {
      this->SetBoolVal(Value);
    }

    ExpandedUDec(const ExpandedUDec& Value)
    {
      this->SetVal(Value);
    }
  #pragma endregion ConvertFromOtherTypes

  #pragma region ConvertToOtherTypes
public:
    /// <summary>
    /// MediumDec Variant to float explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    void float toFloat()
    {
      auto lValue = *this;
      if(ExtraRep!=0)
        lValue /= ExtraRep;
      float Value = lValue.IntHalf;
      if (DecimalHalf != 0) 
         Value += ((float)lValue.DecimalHalf * 0.000000001f);
      return Value;
    }

    /// <summary>
    /// MediumDec Variant to double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    void double toDouble()
    {
      auto lValue = *this;
      if(ExtraRep!=0)
        lValue /= ExtraRep;
      double Value = lValue.IntHalf;
      if (DecimalHalf != 0) 
         Value += ((double)lValue.DecimalHalf * 0.000000001);
      return Value;
    }

    /// <summary>
    /// MediumDec Variant to long double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    void long double toDecimal()
    {
      auto lValue = *this;
      if(ExtraRep!=0)
        lValue /= ExtraRep;
      long double Value = lValue.IntHalf;
      if (DecimalHalf != 0) 
         Value += ((long double)lValue.DecimalHalf * 0.000000001L);
      return Value;
    }
    
    /// <summary>
    /// MediumDec Variant to int explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    unsigned _int64 toInt() {
      auto lValue = *this;
      if(ExtraRep!=0)
        lValue /= ExtraRep; 
      return lValue.IntHalf;
    }

    bool toBool() { return IntHalf.IsZero() ? false : true; }

    /// <summary>
    /// MediumDec Variant to float explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator float() { return toFloatV1(); }
    
    /// <summary>
    /// MediumDec Variant to double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator double() { return toDoubleV1(); }
    
    /// <summary>
    /// MediumDec Variant to decimal explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator long double() { return toDecimalV1(); }

    /// <summary>
    /// MediumDec Variant to int explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator int() { return toInt(); }

    /// <summary>
    /// MediumDec Variant to bool explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator bool() { return toBool(); }

  #pragma endregion ConvertToOtherTypes
  
  #pragma region Comparison Operators
protected:
    
    //Compare only as if in NormalType representation mode
    std::strong_ordering BasicIntComparison(const int& that) const
    {
      if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
        return IntHalfCmp;
      if (auto DecimalHalfCmp = DecimalHalf <=> 0; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }

    //Compare only as if in NormalType representation mode
    std::strong_ordering BasicComparison(const ExpandedUDec& that) const
    {
      if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
        return IntHalfCmp;
      if (auto DecimalHalfCmp = DecimalHalf <=> that.DecimalHalf; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }
  
public:

    std::strong_ordering operator<=>(const ExpandedUDec& that) const
    {
      if(ExtraRep!=0)
      {
        ExpandedUDec lValue = *this;
        lValue.BasicIntDivOp(ExtraRep);
        if(that.ExtraRep!=0){
          ExpandedUDec rValue = that;
          rValue.BasicIntDivOp(that.ExtraRep);
          return lValue.BasicComparison(rValue);
        }
        else
          return lValue.BasicComparison(that);
      }
      else if(that.ExtraRep!=0)
      {
        ExpandedUDec rValue = that;
        rValue.BasicIntDivOp(that.ExtraRep);
        return BasicComparison(rValue);
      }
      else
        return BasicComparison(that);
    }

    std::strong_ordering operator<=>(const int& that) const
    {
      if(ExtraRep!=0)
      {
        ExpandedUDec lValue = *this;
        lValue.BasicIntDivOp(ExtraRep);
        return lValue.BasicIntComparison(that);
      }
      else
        return BasicIntComparison(that);
    }

    bool operator==(const int& that) const
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator==(const ExpandedUDec& that) const
    {
      if (IntHalf!=that.IntHalf)
        return false;
      if (DecimalHalf!=that.IntHalf)
        return false;
    }
  #pragma endregion Comparison Operators

  #pragma region NormalRep Integer division operations
protected:

    template<IntegerType IntType=unsigned _int64>
    void PartialUIntDivOp(const IntType& rValue)
    {//Avoid using with special status representations such as approaching zero or result will be incorrect
      UInt128 SelfRes;
      UInt128 Res;
      UInt128 IntHalfRes;
      UInt128 DecimalRes;
      
      SelfRes = DecimalHalf == 0? DecimalOverflowX * IntHalf: DecimalOverflowX * IntHalf + DecimalHalf;
      Res = SelfRes / rValue;
      
      IntHalfRes = Res/DecimalOverflowX;
      DecimalRes = (unsigned _int64)(Res - DecimalOverflowX * IntHalfRes);
      IntHalf = (unsigned _int64)IntHalfRes;
      DecimalHalf = DecimalRes;
    }

public:
    
    constexpr auto PartialUIntDivOpV1 = PartialUIntDivOpV1<unsigned int>;
    constexpr auto PartialIntDivOpV1 = PartialUIntDivOpV1<signed int>;
    constexpr auto PartialUInt64DivOpV1 = PartialUIntDivOpV1<UInt64>;
    constexpr auto PartialInt64DivOpV1 = PartialUIntDivOpV1<Int64>;

protected:

    /// <summary>
    /// Basic division operation between MediumDec Variant and unsigned Integer value 
    /// that ignores special representation status
    /// (Doesn't modifify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    /// <returns>MediumDec&</returns>
    template<IntegerType IntType=unsigned int>
    auto& BasicUIntDivOpV1(const IntType& Value)
    {
      if (Value == 0)
      {
        throw "Target value can not be divided by zero";
      }
      else if (IsZero())
        return;
      PartialUIntDivOpV1(Value);
      if (IntHalf == 0 && DecimalHalf == 0)
        DecimalHalf = 1;//Prevent Dividing into nothing
      return *this;
    }

    /// <summary>
    /// Basic division operation between MediumDec Variant and unsigned Integer value 
    /// that ignores special representation status
    /// (Doesn't modifify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    /// <returns>ExpandedUDec&</returns>
    template<IntegerType IntType=unsigned int>
    auto BasicDivideByUIntV1(const IntType& rValue)
    {
      auto self = *this;
      return self.BasicUIntDivOpV1(rValue);
    }
    
public:

    constexpr auto BasicUIntDivOp = BasicUIntDivOpV1<unsigned int>;
    constexpr auto BasicIntDivOp = BasicUIntDivOpV1<signed int>;
    constexpr auto BasicUInt64DivOp = BasicUIntDivOpV1<UInt64>;
    constexpr auto BasicInt64DivOp = BasicUIntDivOpV1<Int64>;

    constexpr auto BasicUInt8DivOp = BasicUIntDivOpV1<unsigned char>;
    constexpr auto BasicInt8DivOp = BasicUIntDivOpV1<signed char>;
    constexpr auto BasicUInt16DivOp = BasicUIntDivOpV1<unsigned short>;
    constexpr auto BasicInt16DivOp = BasicUIntDivOpV1<signed short>;

    constexpr auto BasicDivideByUInt = BasicDivideByUIntV1<unsigned int>;
    constexpr auto BasicDivideByInt = BasicDivideByUIntV1<signed int>;
    constexpr auto BasicDivideByUInt64 = BasicDivideByUIntV1<UInt64>;
    constexpr auto BasicDivideByInt64 = BasicDivideByUIntV1<Int64>;

    constexpr auto BasicDivideByUInt8 = BasicDivideByUIntV1<unsigned char>;
    constexpr auto BasicDivideByInt8 = BasicDivideByUIntV1<signed char>;
    constexpr auto BasicDivideByUInt16 = BasicDivideByUIntV1<unsigned short>;
    constexpr auto BasicDivideByInt16 = BasicDivideByUIntV1<signed short>;

  #pragma endregion NormalRep Integer division operations
    
  #pragma region NormalRep Integer Multiplication Operations
protected:
    template<IntegerType IntType=unsigned int>
    void PartialUIntMultOpV1(const IntType& Value)
      if (DecimalHalf == 0)
        IntHalf *= rValue;
      else
      {
        __int64 SRep = IntHalf == 0 ? DecimalHalf : DecimalOverflowX * IntHalf.Value + DecimalHalf;
        SRep *= rValue;
        if (SRep >= DecimalOverflowX)
        {
          __int64 OverflowVal = SRep / DecimalOverflowX;
          SRep -= OverflowVal * DecimalOverflowX;
          IntHalf = (unsigned int)OverflowVal;
          DecimalHalf = (unsigned int)SRep;
        }
        else
        {
          IntHalf = 0;
          DecimalHalf = (unsigned int)SRep;
        }
      }
    }

public:
    
    constexpr auto PartialUIntMultOp = PartialUIntMultOpV1<unsigned int>;
    constexpr auto PartialIntMultOp = PartialUIntMultOpV1<signed int>;
    constexpr auto PartialUInt64MultOp = PartialUIntMultOpV1<UInt64>;
    constexpr auto PartialInt64MultOp = PartialUIntMultOpV1<Int64>;
    
protected:
    template<IntegerType IntType=signed int>
    auto& BasicUIntMultOpV1(const IntType& Value)
    {
      if (Value == 0)
      {
        SetAsZero();
        return *this;
      }
      else if (IsZero())
        return *this;
      PartialUIntMultOp(Value);
      return *this;
    }

    /// <summary>
    /// Basic multiplication operation between MediumDec variant and unsigned Integer value 
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    /// <returns>ExpandedUDec&</returns>
    template<IntegerType IntType=unsigned int>
    auto BasicMultiplyByUIntV1(const IntType& rValue)
    {
    {
      auto self = *this;
      return self.BasicUIntMultOpV1(rValue);
    }
    
public:

    constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<unsigned int>;
    constexpr auto BasicIntMultOp = BasicUIntMultOpV1<signed int>;
    constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<UInt64>;
    constexpr auto BasicInt64MultOp = BasicUIntMultOpV1<Int64>;

    constexpr auto BasicUInt8MultOp = BasicUIntMultOpV1<unsigned char>;
    constexpr auto BasicInt8MultOp = BasicUIntMultOpV1<signed char>;
    constexpr auto BasicUInt16MultOp = BasicUIntMultOpV1<unsigned short>;
    constexpr auto BasicInt16MultOp = BasicUIntMultOpV1<signed short>;

    constexpr auto BasicMultiplyByUInt = BasicMultiplyByUIntV1<unsigned int>;
    constexpr auto BasicMultiplyByInt = BasicMultiplyByUIntV1<signed int>;
    constexpr auto BasicMultiplyByUInt64 = BasicMultiplyByUIntV1<UInt64>;
    constexpr auto BasicMultiplyByInt64 = BasicMultiplyByUIntV1<Int64>;

    constexpr auto BasicMultiplyByUInt8 = BasicMultiplyByUIntV1<unsigned char>;
    constexpr auto BasicMultiplyByInt8 = BasicMultiplyByUIntV1<signed char>;
    constexpr auto BasicMultiplyByUInt16 = BasicMultiplyByUIntV1<unsigned short>;
    constexpr auto BasicMultiplyByInt16 = BasicMultiplyByUIntV1<signed short>;
  

  #pragma endregion NormalRep Integer Multiplication Operations

  #pragma region NormalRep Integer Addition Operations
protected:

public:

    /// <summary>
    /// Basic addition operation between ExpandedUDec and Integer value 
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    /// <returns>ExpandedUDec&</returns>
    template<IntegerType IntType=unsigned int>
    auto& BasicUIntAddOpV1(const IntType& rValue)
    {
      IntHalf += rValue;
      return *this;
    }

    /// <summary>
    /// Basic addition operation between MediumDec Variant and unsigned Integer value 
    /// that ignores special representation status
    /// (Doesn't modifify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    /// <returns>ExpandedUDec&</returns>
    template<IntegerType IntType=unsigned int>
    auto BasicAddByUIntV1(const IntType& rValue)
    {
      auto self = *this;
      return self.BasicUIntAddOpV1(rValue);
    }

    constexpr auto BasicUIntAddOp = BasicUIntAddOpV1<unsigned int>;
    constexpr auto BasicIntAddOp = BasicUIntAddOpV1<signed int>;
    constexpr auto UnsignedBasicIntAddOp = BasicUIntAddOpV1<signed int>;
    constexpr auto BasicUIntAddOp64 = BasicUIntAddOpV1<UInt64>;
    constexpr auto BasicIntAddOp64 = BasicUIntAddOpV1<Int64>;
    constexpr auto UnsignedBasicIntAddOp64 = BasicUIntAddOpV1<signed int>;

    constexpr auto BasicUIntAddOp8 = BasicUIntAddOpV1<unsigned char>;
    constexpr auto BasicIntAddOp8 = BasicUIntAddOpV1<signed char>;
    constexpr auto BasicUIntAddOp16 = BasicUIntAddOpV1<unsigned short>;
    constexpr auto BasicIntAddOp16 = BasicUIntAddOpV1<signed short>;

    constexpr auto BasicAddByUInt = BasicAddByUIntV1<unsigned int>;
    constexpr auto BasicAddByInt = BasicAddByUIntV1<signed int>;
    constexpr auto BasicAddByUInt64 = BasicAddByUIntV1<UInt64>;
    constexpr auto BasicAddByInt64 = BasicAddByUIntV1<Int64>;

    constexpr auto BasicAddByUInt8 = BasicAddByUIntV1<unsigned char>;
    constexpr auto BasicAddByInt8 = BasicAddByUIntV1<signed char>;
    constexpr auto BasicAddByUInt16 = BasicAddByUIntV1<unsigned short>;
    constexpr auto BasicAddByInt16 = BasicAddByUIntV1<signed short>;

  #pragma endregion NormalRep Integer Addition Operations

  #pragma region NormalRep Integer Subtraction Operations
protected:
  
public:

    /// <summary>
    /// Basic subtraction operation between ExpandedUDec and Integer value 
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    /// <returns>MediumDec&</returns>
    template<IntegerType IntType=unsigned int>
    auto BasicUIntSubOpV1(const IntType& rValue)
    {
      IntHalf -= rValue;
      return *this;
    }

    /// <summary>
    /// Basic subtraction operation between ExpandedUDec variant and unsigned Integer value 
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    /// <returns>MediumDec&</returns>
    template<IntegerType IntType=unsigned int>
    auto BasicSubtractByUIntV1(const IntType& rValue)
    {
    {
      auto self = *this;
      return self.BasicUIntSubOpV1(rValue);
    }

    constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<unsigned int>;
    constexpr auto BasicIntSubOp = BasicUIntSubOpV1<signed int>;
    constexpr auto UnsignedBasicIntSubOp = BasicUIntSubOpV1<signed int>;
    constexpr auto BasicUIntSubOp64 = BasicUIntSubOpV1<UInt64>;
    constexpr auto BasicIntSubOp64 = BasicUIntSubOpV1<Int64>;
    constexpr auto UnsignedBasicIntSubOp64 = BasicUIntSubOpV1<signed int>;

    constexpr auto BasicUIntSubOp8 = BasicUIntSubOpV1<unsigned char>;
    constexpr auto BasicIntSubOp8 = BasicUIntSubOpV1<signed char>;
    constexpr auto BasicUIntSubOp16 = BasicUIntSubOpV1<unsigned short>;
    constexpr auto BasicIntSubOp16 = BasicUIntSubOpV1<signed short>;

    constexpr auto BasicSubByUInt = BasicSubByUIntV1<unsigned int>;
    constexpr auto BasicSubByInt = BasicSubByUIntV1<signed int>;
    constexpr auto BasicSubByUInt64 = BasicSubByUIntV1<UInt64>;
    constexpr auto BasicSubByInt64 = BasicSubByUIntV1<Int64>;

    constexpr auto BasicSubByUInt8 = BasicSubByUIntV1<unsigned char>;
    constexpr auto BasicSubByInt8 = BasicSubByUIntV1<signed char>;
    constexpr auto BasicSubByUInt16 = BasicSubByUIntV1<unsigned short>;
    constexpr auto BasicSubByInt16 = BasicSubByUIntV1<signed short>;

  #pragma endregion NormalRep Integer Subtraction Operations

  #pragma region NormalRep AltNum division operations
protected:
  
public:
/*
    constexpr auto BasicDivOp = BasicDivOpV1<ExpandedUDec>;
*/
  #pragma endregion NormalRep AltNum division operations

  #pragma region NormalRep AltNum Multiplication Operations
protected:

public:

    /// <summary>
    /// Basic multiplication operation that ignores special decimal status with unsigned ExpandedUDec
    /// Return true if divide into zero
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side Value</param>
    ExpandedUDec& BasicMultOp(const ExpandedUDec& rValue)
    {
      if (DecimalHalf == 0)
      {
        if (IntHalf == 1)
        {
          IntHalf = rValue.IntHalf; DecimalHalf = rValue.DecimalHalf;
        }
        else if (rValue.DecimalHalf == 0)
        {
          IntHalf *= rValue.IntHalf;
        }
        else
        {
          UInt128 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf;
          if (rRep >= DecimalOverflowX)
          {
            UInt128 OverflowVal = rRep / DecimalOverflowX;
            rRep -= OverflowVal * DecimalOverflowX;
            IntHalf = (UInt64)OverflowVal;
            DecimalHalf = (unsigned int)rRep;
            return *this;
          }
          else
          {
            DecimalHalf = (unsigned int)rRep;
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0)
              DecimalHalf = 1;
#endif
            IntHalf = ResIsNegative ? NegativeRep : 0;
            return *this;
          }
        }
      }
      else if (IntHalf == 0)
      {
        UInt128 SRep = DecimalHalf;
        SRep *= rValue.DecimalHalf;
        SRep /= ExpandedUDec::DecimalOverflowX;
        if (rValue.IntHalf == 0)
        {
          DecimalHalf = (unsigned int)SRep;
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
          if (DecimalHalf == 0)
            DecimalHalf = 1;
        #endif
          return *this;
        }
        else
        {
          SRep += DecimalHalf * rValue.IntHalf;
          if (SRep >= ExpandedUDec::DecimalOverflowX)
          {
            UInt128 OverflowVal = SRep / ExpandedUDec::DecimalOverflowX;
            SRep -= OverflowVal * ExpandedUDec::DecimalOverflowX;
            IntHalf = (unsigned _int64) OverflowVal;
            DecimalHalf = (unsigned int)SRep;
            return *this;
          }
          else
          {
            DecimalHalf = (unsigned int)SRep;
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0)
              DecimalHalf = 1;
        #endif
            return *this;
          }
        }
      }
      else
      {
        if (rValue.DecimalHalf == 0)//Y is integer
        {
          UInt128 SRep = ExpandedUDec::DecimalOverflowX * IntHalf + DecimalHalf;
          SRep *= rValue.IntHalf;
          if (SRep >= ExpandedUDec::DecimalOverflowX)
          {
            UInt128 OverflowVal = SRep / ExpandedUDec::DecimalOverflowX;
            SRep -= OverflowVal * ExpandedUDec::DecimalOverflowX;
            IntHalf = (unsigned __int64)OverflowVal;
            DecimalHalf = (unsigned int)SRep;
          }
          else
          {
            DecimalHalf = (unsigned int)SRep;
            if(DecimalHalf==0)
            {
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
              if(DecimalHalf==0)
                DecimalHalf = 1;
        #endif
            }
            IntHalf = SelfIsNegative ? ExpandedUDec::NegativeRep : 0;
          }
          return *this;
        }
        else if (rValue.IntHalf == 0)
        {
          UInt128 SRep = ExpandedUDec::DecimalOverflowX * IntHalf + DecimalHalf;
          SRep *= rValue.DecimalHalf;
          SRep /= ExpandedUDec::DecimalOverflowX;
          if (SRep >= ExpandedUDec::DecimalOverflowX)
          {
            UInt128 OverflowVal = SRep / ExpandedUDec::DecimalOverflowX;
            SRep -= OverflowVal * ExpandedUDec::DecimalOverflowX;
            IntHalf = (UInt64)(SelfIsNegative ? -OverflowVal : OverflowVal);
            DecimalHalf = (unsigned int)SRep;
          }
          else
          {
            DecimalHalf = (unsigned int)SRep;
            if(DecimalHalf==0)
            {
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
              if(DecimalHalf==0)
                DecimalHalf = 1;
        #endif
            }
            IntHalf = SelfIsNegative ? ExpandedUDec::NegativeRep : 0;
          }
          return *this;
        }
        else
        {
          //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
          UInt128 SRep = IntHalf == 0 ? DecimalHalf : ExpandedUDec::DecimalOverflowX * IntHalf + DecimalHalf;
          SRep *= rValue.IntHalf;//SRep holds __int64 version of X.Y * Z
          //X.Y *.V
          UInt128 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf);//Temp03 holds __int64 version of X *.V
          UInt128 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
          Temp04 /= ExpandedUDec::DecimalOverflow;
          //Temp04 holds __int64 version of .Y * .V
          UInt128 IntegerRep = SRep + Temp03 + Temp04;
          UInt128 IntHalf = IntegerRep / ExpandedUDec::DecimalOverflow;
          IntegerRep -= ExpandedUDec::DecimalOverflowX * IntHalf;
          IntHalf = (UInt64)IntHalf;
          DecimalHalf = (unsigned int)IntegerRep;
        }
      }
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
      if(DecimalHalf==0&&IntHalf==0)
        DecimalHalf = 1;
#endif
      return *this;
    }

  #pragma endregion NormalRep AltNum Multiplication Operations

  #pragma region Other division operations
private:
    const unsigned int HalfIntLimit = std::numeric_limits<unsigned int>::max()/2;
    const unsigned int QuarterIntLimit = std::numeric_limits<unsigned int>::max()/4;

public:
    //Simplified division by 2 operation(to reduce cost of multiplication)
    void DivideByTwo()
    {
      if(ExtraRep==1)
        ExtraRep = 2;
      else if(ExtraRep<HalfIntLimit)
        ExtraRep <<= 1;
      else
      {
			  normalizeSafe();
        if(ExtraRep<HalfIntLimit)
          ExtraRep *= 2;
        else {
          // Fall back to just dividing the value by 2
          BasicUIntDivOp(2);
        }
      }
    }

    //Simplified division by 4 operation(to reduce cost of multiplication)
    void DivideByFour()
    {
      if(ExtraRep==1)
        ExtraRep = 4;
      else if(ExtraRep<QuarterIntLimit)
        ExtraRep *= 4;
      else
      {
			  normalizeSafe();
        if(ExtraRep<QuarterIntLimit)
          ExtraRep *= 4;
				else {
          BasicUIntDivOp(4);
				}
      }
    }

    void DivideByTwoN(unsigned count) {
      for (unsigned i = 0; i < count; ++i) {
        DivideByTwo();      // existing method that divides by 2
      }
    }

  #pragma endregion Other division operations  

  #pragma region Other Multiplication Operations

    //Simplified multiplication by 2 operation(to reduce cost of multiplication)
    void MultipleByTwo()
    {
      if(ExtraRep&1==1)//Check if number is odd
        rValue.UnsignedBasicIntMultOp(2);
      else
        ExtraRep /= 2;
    }

    //Simplified multiplication by 4 operation(to reduce cost of multiplication)
    void MultipleByFour()
    {
      if(((ExtraRep >> 2) << 2) == ExtraRep))
        ExtraRep /= 4;
      else
        rValue.UnsignedBasicIntMultOp(4);
    }

  #pragma endregion Other Multiplication Operations

  #pragma region NormalRep AltNum Addition/Subtraction Operations

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOp(const ExpandedUDec& rValue){ AddOpV1(rValue); }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const ExpandedUDec& rValue){ SubOpV1(rValue); }

        //Basic addition operation
        ExpandedUDec& AddOperation(const ExpandedUDec& rValue)
        { AddOp(rValue); return *this; }

        /// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        ExpandedUDec AddBy(const ExpandedUDec& rValue)
        { ExpandedUDec lValue = *this; return lValue.AddOperation(rValue); } const

        //Basic subtraction operation
        ExpandedUDec& SubOperation(const ExpandedUDec& rValue)
        { SubOp(rValue); return *this; }

        /// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        ExpandedUDec SubtractBy(const ExpandedUDec& rValue)
        { ExpandedUDec lValue = *this; return lValue.SubOperation(rValue); } const

  #pragma endregion NormalRep AltNum Addition/Subtraction Operations

  #pragma region Other Addition Operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>ExpandedUDec</returns>
        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const ExpandedUDec& rValue) { lValue.AddOp(rValue); return *lValue; }

        /// <summary>
        /// += operation between ExpandedUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend ExpandedUDec& operator+=(ExpandedUDec& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>ExpandedUDec</returns>
        friend ExpandedUDec operator+(ExpandedUDec lValue, const ExpandedUDec& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// Addition operation between ExpandedUDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend ExpandedUDec operator+(ExpandedUDec lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend ExpandedUDec operator+(ExpandedUDec lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend ExpandedUDec operator+(ExpandedUDec lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend ExpandedUDec operator+(ExpandedUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend ExpandedUDec operator+(ExpandedUDec lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend ExpandedUDec operator+(ExpandedUDec lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend ExpandedUDec operator+(ExpandedUDec lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend ExpandedUDec operator+(ExpandedUDec lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        friend ExpandedUDec operator+(const signed int& lValue, ExpandedUDec rValue)  { return rValue.IntAddOperation(lValue); }
        friend ExpandedUDec operator+(const signed __int64& lValue, ExpandedUDec rValue)  { return rValue.Int64AddOperation(lValue); }
        friend ExpandedUDec operator+(const unsigned int& lValue, ExpandedUDec rValue)  { return rValue.UIntAddOperation(lValue); }
        friend ExpandedUDec operator+(const unsigned __int64& lValue, ExpandedUDec rValue)  { return rValue.UInt64AddOperation(lValue); }

        friend ExpandedUDec operator+(const signed char& lValue, ExpandedUDec rValue)  { return rValue.Int8AddOperation(lValue); }
        friend ExpandedUDec operator+(const signed short& lValue, ExpandedUDec rValue)  { return rValue.Int16AddOperation(lValue); }
        friend ExpandedUDec operator+(const unsigned char& lValue, ExpandedUDec rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend ExpandedUDec operator+(const unsigned short& lValue, ExpandedUDec rValue)  { return rValue.UInt16AddOperation(lValue); }

  #pragma endregion Other Addition Operations

  #pragma region Other Subtraction Operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>ExpandedUDec</returns>
        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const ExpandedUDec& rValue) { lValue.SubOp(rValue); return *lValue; }

        /// <summary>
        /// -= operation between ExpandedUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>ExpandedUDec</returns>
        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend ExpandedUDec& operator-=(ExpandedUDec& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>ExpandedUDec</returns>
        friend ExpandedUDec operator-(ExpandedUDec lValue, const ExpandedUDec& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation between ExpandedUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>ExpandedUDec</returns>
        friend ExpandedUDec operator-(ExpandedUDec lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend ExpandedUDec operator-(ExpandedUDec lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend ExpandedUDec operator-(ExpandedUDec lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend ExpandedUDec operator-(ExpandedUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend ExpandedUDec operator-(ExpandedUDec lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend ExpandedUDec operator-(ExpandedUDec lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend ExpandedUDec operator-(ExpandedUDec lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend ExpandedUDec operator-(ExpandedUDec lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        friend ExpandedUDec operator-(const signed int& lValue, const ExpandedUDec& rValue) { return ((ExpandedUDec)lValue).SubtractBy(rValue); }
        friend ExpandedUDec operator-(const signed __int64& lValue, const ExpandedUDec& rValue) { return ((ExpandedUDec)lValue).SubtractBy(rValue); }
        friend ExpandedUDec operator-(const unsigned int& lValue, const ExpandedUDec& rValue) { return ((ExpandedUDec)lValue).SubtractBy(rValue); }
        friend ExpandedUDec operator-(const unsigned __int64& lValue, const ExpandedUDec& rValue) { return ((ExpandedUDec)lValue).SubtractBy(rValue); }

        friend ExpandedUDec operator-(const signed char& lValue, const ExpandedUDec& rValue) { return ((ExpandedUDec)lValue).SubtractBy(rValue); }
        friend ExpandedUDec operator-(const signed short& lValue, const ExpandedUDec& rValue) { return ((ExpandedUDec)lValue).SubtractBy(rValue); }
        friend ExpandedUDec operator-(const unsigned char& lValue, const ExpandedUDec& rValue) { return ((ExpandedUDec)lValue).SubtractBy(rValue); }
        friend ExpandedUDec operator-(const unsigned short& lValue, const ExpandedUDec& rValue) { return ((ExpandedUDec)lValue).SubtractBy(rValue); }

  #pragma endregion Other Subtraction Operations

  #pragma region Modulus Operations


    protected:

        template<ExpandedUDecVariant VariantType=ExpandedUDec>
        void ModulusOpV1(const VariantType& rValue)
        {
            if(DecimalHalf.Value==0&&rValue.DecimalHalf.Value==0)
                IntHalf %= rValue.IntHalf;
            else {
                auto divRes = DivideBy(rValue);
                SubOp(divRes.MultiplyBy(rValue));
            }
        }

    public:

        void UIntModulusOp(const unsigned int& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf %= rValue;
            else {
                auto divRes = DivideByIntV1(rValue);
                SubOp(divRes.MultiplyByUInt(rValue));
            }
        }

        void IntModulusOp(const signed int& rValue)
        {
            if (rValue < 0)
                throw("Negative number operation not supported by unsigned version.");
            else
                UIntModulusOp(rValue);
        }

        void UInt64ModulusOp(const unsigned __int64& rValue)
        {
            if(DecimalHalf.Value==0){
                unsigned __int64 result = IntHalf;
                result %= rValue;
                IntHalf = (unsigned int) result;
            } else {
                auto divRes = DivideByIntV1(rValue);
                SubOp(divRes.MultiplyByUInt64(rValue));
            }
        }

        void Int64ModulusOp(const signed __int64& rValue)
        {
            if (rValue < 0)
                throw("Negative number operation not supported by unsigned version.");
            else
                UInt64ModulusOp(rValue);
        }

        void ModulusOp(const ExpandedUDec& rValue){ ModulusOpV1(rValue); }

        ExpandedUDec& ModulusOperation(const ExpandedUDec& rValue){
            ModulusOp(rValue); return *this;
        }

        friend ExpandedUDec& operator%=(ExpandedUDec& lValue, const ExpandedUDec& rValue) { return lValue.ModulusOperation(rValue); }
        friend ExpandedUDec& operator%=(ExpandedUDec& lValue, const signed int& rValue) { lValue.IntModulusOp(rValue); return lValue; }
        friend ExpandedUDec& operator%=(ExpandedUDec& lValue, const unsigned int& rValue) { lValue.UIntModulusOp(rValue); return lValue; }
        friend ExpandedUDec& operator%=(ExpandedUDec& lValue, const unsigned __int64& rValue) { lValue.UInt64ModulusOp(rValue); return lValue;}
        friend ExpandedUDec& operator%=(ExpandedUDec& lValue, const signed __int64& rValue) { lValue.Int64ModulusOp(rValue); return lValue;}

  #pragma endregion Modulus Operations

  #pragma region Bitwise Operations
	
		// raw encodes value = floor(raw / unit) + (raw % unit)/unit
		// unit is the fixed-point denominator (e.g., 1 << frac_bits)
		// Assumes unit > 0 and typical frac_bits <= 96 (so (rem * 1e9) fits 128-bit)
		static ExpandedUDec from_binfrac_with_unit(UInt128 raw, UInt128 unit)
		{
				if (unit == 0) { throw "from_binfrac_with_unit: unit must be > 0"; }

				constexpr unsigned FRAC_BITS = 30;
				constexpr UInt128 DEC_OVER = DecimalOverflowX;

				if (unit == (UInt128(1) << FRAC_BITS)) {
						ExpandedUDec out;
						out.IntHalf = static_cast<UInt64>(raw >> FRAC_BITS);
						UInt128 mask = (UInt128(1) << FRAC_BITS) - 1;
						UInt128 fracBits = raw & mask;
						// Map the FRAC_BITS into the 9-decimal field by bit-shift alignment
						out.DecimalHalf = static_cast<unsigned>((fracBits << (30 - FRAC_BITS)).convert_to<unsigned long long>());
						out.ExtraRep = InitialExtraRep;
						return out;
				}
				
				constexpr UInt128 DEC_OVER = DecimalOverflowX; // 1'000'000'000
				ExpandedUDec out;

				// Integer part
				UInt128 q = raw / unit;
				if (q > UInt128(UInt64Max)) {
						// Saturate
						out.IntHalf = UInt64Max;
						out.DecimalHalf = 999'999'999;
						out.ExtraRep = InitialExtraRep;
						return out;
				}
				out.IntHalf = static_cast<UInt64>(q);

				// Fractional part -> 9 decimal digits (rounded half-up)
				UInt128 rem = raw - q * unit;                    // or: raw % unit
				if (rem == 0) {
						out.DecimalHalf = 0;
						out.ExtraRep = InitialExtraRep;
						return out;
				}

				// Compute dec = round(rem * 1e9 / unit)
				// Guard: for usual frac_bits <= 96, rem * 1e9 fits inside 128 bits.
				UInt128 num = rem * DEC_OVER;
				UInt128 dec = (num + (unit >> 1)) / unit;        // half-up rounding

				// If rounding bumps to 1e9, carry into IntHalf
				if (dec >= DEC_OVER) {
						if (out.IntHalf == UInt64Max) {
								// Saturate
								out.IntHalf = UInt64Max;
								out.DecimalHalf = 999'999'999;
						} else {
								++out.IntHalf;
								out.DecimalHalf = 0;
						}
				} else {
						out.DecimalHalf = static_cast<unsigned>(dec.convert_to<unsigned long long>());
				}

				out.ExtraRep = InitialExtraRep;
				return out;
		}
	
		UInt128 to_binfrac() const {
				constexpr unsigned FRAC_BITS = 30; // ~1e9 scale
				return (UInt128(IntHalf) << FRAC_BITS) |
							 (DecimalHalf >> (30 - FRAC_BITS));
		}
	
		static ExpandedUDec from_binfrac(UInt128 raw) {
			constexpr unsigned FRAC_BITS = 30;
			ExpandedUDec out;
			out.IntHalf     = uint64_t(raw >> FRAC_BITS);
			out.DecimalHalf = unsigned((raw & ((UInt128(1) << FRAC_BITS) - 1))
																 << (30 - FRAC_BITS));
			out.ExtraRep    = InitialExtraRep;
			return out;
		}
	
		static ExpandedUDec from_binfrac_with_frac_bits(UInt128 raw, unsigned frac_bits)
		{
				if (frac_bits > 120) { throw "from_binfrac_with_frac_bits: frac_bits too large"; }
				UInt128 unit = UInt128(1) << frac_bits;
				return from_binfrac_with_unit(raw, unit);
		}
	
		//Left Shift
		ExpandedUDec& operator<<=(unsigned shift) {
				if (shift == 0) return *this;
				constexpr unsigned FRAC_BITS = 30;
				
				// Combine into a wide fixed-point field
				__uint128_t combo = (__uint128_t)IntHalf << FRAC_BITS |
														(DecimalHalf >> (30 - FRAC_BITS));

				// Logical shift left
				combo <<= shift;

				// Split back out
				IntHalf = (uint64_t)(combo >> FRAC_BITS);
				DecimalHalf = (unsigned)((combo & ((1u << FRAC_BITS) - 1)) << (30 - FRAC_BITS));
				return *this;
		}
		
		//Right Shift
		ExpandedUDec& operator>>=(unsigned shift) {
				if (shift == 0) return *this;
				constexpr unsigned FRAC_BITS = 30; // or derived from DecimalOverflow
				unsigned totalBits = 64 + FRAC_BITS;

				// Pack into a wide int
				__uint128_t combo = (__uint128_t)IntHalf << FRAC_BITS | (DecimalHalf >> (30 - FRAC_BITS));

				// Logical shift right
				combo >>= shift;

				// Split back out
				IntHalf = (uint64_t)(combo >> FRAC_BITS);
				DecimalHalf = (unsigned)((combo & ((1u << FRAC_BITS) - 1)) << (30 - FRAC_BITS));
				return *this;
		}
		
  #pragma endregion Bitwise Operations

  #pragma endregion Other Operators
    /// <summary>
    /// Negative Unary Operator(Flips negative status)
    /// </summary>
    /// <param name="self">The self.</param>
    /// <returns>ExpandedUDec</returns>
    friend ExpandedUDec& operator-(ExpandedUDec& self)
    {
      return self;//Negative numbers not supported
    }

    /// <summary>
    /// ++ExpandedUDec Operator
    /// </summary>
    /// <returns>ExpandedUDec &</returns>
    ExpandedUDec& operator ++()
    {
      ++IntHalf;
      return *this;
    }

    /// <summary>
    /// ++ExpandedUDec Operator
    /// </summary>
    /// <returns>ExpandedUDec &</returns>
    ExpandedUDec& operator --()
    {
      if(IntHalf==0)
        throw "Can't reduce number into negative values";
      else
        --IntHalf;
      return *this;
    }

    /// <summary>
    /// MediumDec Variant++ Operator
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    ExpandedUDec operator ++(int)
    {
      ExpandedUDec tmp(*this);
      ++* this;
      return tmp;
    }

    /// <summary>
    /// MediumDec Variant-- Operator
    /// </summary>
    /// <returns>ExpandedUDec</returns>
    ExpandedUDec operator --(int)
    {
      ExpandedUDec tmp(*this);
      --* this;
      return tmp;
    }

    /// <summary>
    /// MediumDec Variant* Operator
    /// </summary>
    /// <returns>ExpandedUDec &</returns>
    ExpandedUDec& operator *()
    {
      return *this;
    }
		
  #pragma endregion Other Operators

  #pragma region Math Etc Functions
  //Not Supported at moment
  #pragma endregion Math Etc Functions

  #pragma region Pow and Sqrt Functions
  //Not Supported at moment
  #pragma endregion Pow and Sqrt Functions

  #pragma region Log Functions
protected:

public:
    /// <summary>
    /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
    /// Does not modifier owner object
    /// </summary>
    /// <param name="x">The value to apply the exponential function to.</param>
    /// <returns>ExpandedUDec</returns>
    auto ExpOf()
    {
      //x.ConvertToNormType();//Prevent losing imaginary number status
      /*
       * Evaluates f(x) = e^x for any x in the interval [-709, 709].
       * If x < -709 or x > 709, raises an assertion error. Implemented
       * using the truncated Taylor series of e^x with ceil(|x| * e) * 12
       * terms. Achieves at least 14 and at most 16 digits of precision
       * over the entire interval.
       * Performance - There are exactly 36 * ceil(|x| * e) + 5
       * operations; 69,413 in the worst case (x = 709 or -709):
       * - (12 * ceil(|x| * e)) + 2 multiplications
       * - (12 * ceil(|x| * e)) + 1 divisions
       * - (12 * ceil(|x| * e)) additions
       * - 1 rounding
       * - 1 absolute value
       * Accuracy - Over a sample of 10,000 linearly spaced points in
       * [-709, 709] we have the following error statistics:
       * - Max relative error = 8.39803e-15
       * - Min relative error = 0.0
       * - Avg relative error = 0.0
       * - Med relative error = 1.90746e-15
       * - Var relative error = 0.0
       * - 0.88 percent of the values have less than 15 digits of precision
       * Args:
       *    - x: power of e to evaluate
       * Returns:
       *    - approximation of e^x in ExpandedUDec precision
       */
       // Check that x is a valid input.
      assert(IntHalf < 709);
      // When x = 0 we already know e^x = 1.
      if (IsZero()) {
        return One;
      }
      // Normalize x to a non-negative value to take advantage of
      // reciprocal symmetry. But keep track of the original sign
      // in case we need to return the reciprocal of e^x later.
      VariantType x0 = *this;
      x0.Abs();
      // First term of Taylor expansion of e^x at a = 0 is 1.
      // tn is the variable we we will return for e^x, and its
      // value at any time is the sum of all currently evaluated
      // Taylor terms thus far.
      VariantType tn = One;
      // Chose a truncation point for the Taylor series using the
      // heuristic bound 12 * ceil(|x| e), then work down from there
      // using Horner's method.
      int n = VariantType::CeilInt(x0 * E) * 12;
      for (int i = n; i > 0; --i) {
        tn = tn * (x0 / i) + One;
      }
      // If the original input x is less than 0, we want the reciprocal
      // of the e^x we calculated.
      if (x < 0) {
        tn = One / tn;
      }
      return tn;
    }

    /// <summary>
    /// Get the (n)th Root
    /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
    /// Does not modify owner object
    /// </summary>
    /// <param name="n">The n value to apply with root.</param>
    /// <returns></returns>
    auto NthRootOfV2(int n, const auto& Precision = FiveBillionth)
    {
      int nMinus1 = n - 1;
      auto x[2] = { (One / n) * ((*this*nMinus1) + (*this / PowOf(nMinus1))), targetValue };
      while (Abs(x[0] - x[1]) > Precision)
      {
        x[1] = x[0];
        x[0] = (One / n) * ((x[1]*nMinus1) + (targetValue / x[1].PowOf(nMinus1)));
      }
      return x[0];
    }
  #pragma endregion Log Functions
  };

  #pragma region ValueDefine Source

  #pragma endregion ValueDefine Source

  #pragma region String Function Source
  /// <summary>
  /// Reads the string.
  /// </summary>
  /// <param name="Value">The value.</param>
  inline void MediumDec::ReadString(std::string Value)
  {
    IntHalf = 0; DecimalHalf = 0;
  #if !defined(AltNum_EnableMirroredSection)
    bool IsNegative = false;
  #endif
    int PlaceNumber;
    std::string WholeNumberBuffer = "";
    std::string DecimalBuffer = "";

    bool ReadingDecimal = false;
    int TempInt;
    int TempInt02;
    for (char const& StringChar : Value)
    {
      if (VariableConversionFunctions::IsDigit(StringChar))
      {
        if (ReadingDecimal) { DecimalBuffer += StringChar; }
        else { WholeNumberBuffer += StringChar; }
      }
      else if (StringChar == '-')
      {
  #if !defined(AltNum_EnableMirroredSection)
        IsNegative = true;
  #else
        IntHalf.Sign = 0;
  #endif
      }
      else if (StringChar == '.')
      {
        ReadingDecimal = true;
      }
      else if(StringChar!=' ')
        break;//Stop Extracting after encounter non-number character such as i
    }
    PlaceNumber = WholeNumberBuffer.length() - 1;
    for (char const& StringChar : WholeNumberBuffer)
    {
      TempInt = VariableConversionFunctions::CharAsInt(StringChar);
      TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
      if (StringChar != '0')
      {
        IntHalf += TempInt02;
      }
      PlaceNumber--;
    }
    PlaceNumber = 8;
    for (char const& StringChar : DecimalBuffer)
    {
      //Limit stored decimal numbers to the amount it can store
      if (PlaceNumber > -1)
      {
        TempInt = VariableConversionFunctions::CharAsInt(StringChar);
        TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
        if (StringChar != '0')
        {
          DecimalHalf += TempInt02;
        }
        PlaceNumber--;
      }
    }
  #if !defined(AltNum_EnableMirroredSection)
    if (IsNegative)
    {
      if (IntHalf == 0) { IntHalf = NegativeRep; }
      else { IntHalf *= -1; }
    }
  #endif
  }

  /// <summary>
  /// Gets the value from string.
  /// </summary>
  /// <param name="Value">The value.</param>
  /// <returns>MediumDec</returns>
  inline MediumDec MediumDec::GetValueFromString(const std::string& Value)
  {
    MediumDec NewSelf = Zero;
    NewSelf.ReadString(Value);
    return NewSelf;
  }

  std::string MediumDec::ToString()
  {
    std::string Value = "";
    int CurrentSection = IntHalf;
    unsigned __int8 CurrentDigit;
    std::string DecBuffer = "";
    if (IntHalf < 0)
    {
      Value += "-";
      if (IntHalf == NegativeRep) { CurrentSection = 0; }
      else { CurrentSection *= -1; }
    }
    for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
    {
      CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
      CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
      Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
    }
    if (DecimalHalf != 0)
    {
      Value += ".";
      CurrentSection = DecimalHalf;
      for (__int8 Index = 8; Index >= 0; --Index)
      {
        CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
        CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
        if (CurrentDigit != 0)
        {
          if(!DecBuffer.empty())
          {
            Value += DecBuffer;
            DecBuffer.clear();
          }
          Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        else
        {
          DecBuffer += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
      }
    }
    return Value;
  }

  std::string MediumDec::ToFullString()
  {
    std::string Value = "";
    int CurrentSection = IntHalf;
    unsigned __int8 CurrentDigit;
    if (IntHalf < 0)
    {
      Value += "-";
      if (IntHalf == NegativeRep) { CurrentSection = 0; }
      else { CurrentSection *= -1; }
    }
    for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
    {
      CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
      CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
      Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
    }
    if (DecimalHalf != 0)
    {
      Value += ".";
      bool HasDigitsUsed = false;
      CurrentSection = DecimalHalf;
      for (__int8 Index = 8; Index >= 0; --Index)
      {
        if (CurrentSection > 0)
        {
          CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
          CurrentSection -= (CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
          Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        else
          Value += "0";
      }
    }
    else
    {
      Value += ".000000000";
    }
    return Value;
  }
  #pragma endregion String Function Source
}
}