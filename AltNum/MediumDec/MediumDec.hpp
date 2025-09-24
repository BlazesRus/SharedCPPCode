// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecPreprocessors.h"
//#include "..\VirtualTableBase.hpp"//Virtual Structure for the class to make sure can override virtually

#include <string>
#include <cmath>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "..\Concepts\MediumDecVariant.hpp"


#include "..\AlternativeInt\MirroredInt.hpp"
#include "..\AlternativeInt\PartialInt.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumDec;

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// (8 bytes worth of Variable Storage inside class for each instance)
    /// </summary>
    class DLL_API MediumDec : public AltNumBase//AltNumBase is used as base class to identify as MediumDec variant
    {
    public:
    #pragma region DigitStorage

    /// <summary>
    /// Stores whole half of number(Including positive/negative status)
    /// (in the case of infinity is used to determine if positive vs negative infinity)
    /// </summary>
    MirroredInt IntHalf;

    /// <summary>
    /// Stores decimal section info and other special info
    /// </summary>
    PartialInt DecimalHalf;

    //Return IntHalf as signed int
    inline signed int GetIntHalf() const
    {
      return IntHalf.GetValue();
    }

    inline unsigned int GetFlags() const
    {
      return DecimalHalf.Flags;
    }

    inline void SetFlag(const unsigned int& flagVal)
    {
      DecimalHalf.Flags = flagVal;
    }
        
    #pragma endregion DigitStorage

    /// <summary>
    /// The decimal overflow
    /// </summary>
    static constexpr unsigned int DecimalOverflow = 1'000'000'000u;
    
    static constexpr unsigned int DecimalMax = 999'999'999u;

    /// <summary>
    /// The decimal overflow in _int64 so don't need to widen
    /// </summary>
    static constexpr unsigned _int64 DecimalOverflowX = 1'000'000'000;

    /// <summary>
    /// Value when IntHalf is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
    /// </summary>
    static constexpr MirroredInt NegativeRep = MirroredInt::NegativeZero;

    #pragma region class_constructors

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumDec"/> class using IntHalf's type field.
    /// </summary>
    /// <param name="intVal">The whole number based half of the representation</param>
    /// <param name="decVal">The non-whole based half of the representation (and other special statuses)</param>
    constexpr MediumDec(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
    : IntHalf(intVal), DecimalHalf(decVal) {}

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumDec"/> class using signed integer.
    /// </summary>
    /// <param name="intVal">The whole number based half of the representation</param>
    /// <param name="decVal">The non-whole based half of the representation (and other special statuses)</param>
    constexpr MediumDec(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero)
    : IntHalf(intVal), DecimalHalf(decVal) {}

    /// <summary>
    /// Copy assignment.
    /// </summary>
    constexpr MediumDec& operator=(const MediumDec& rhs)
    {
        if (this != &rhs)
        {
            IntHalf = rhs.IntHalf;
            DecimalHalf = rhs.DecimalHalf;
        }
        return *this;
    }

    /// <summary>
    /// Assignment from int.
    /// </summary>
    constexpr MediumDec& operator=(const signed int& rhs)
    {
        IntHalf = rhs;
        DecimalHalf = PartialInt(0);
        return *this;
    }

    /// <summary>
    /// Creates class from derived class into this class (subscript operator of [])
    /// </summary>
    template<MediumDecVariant VariantType>
    constexpr MediumDec operator[](VariantType variantValue) const
    {
        return MediumDec(variantValue.IntHalf, variantValue.DecimalHalf);
    }

    /// <summary>
    /// Fix for C2440 error during static template class
    /// </summary>
    static constexpr MediumDec Initialize(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
    {
        return MediumDec(intVal, decVal);
    }

    /// <summary>
    /// Sets the value.
    /// </summary>
    constexpr void SetValue(const MediumDec& rValue)
    {
        IntHalf = rValue.IntHalf;
        DecimalHalf.SetValueV2(rValue.DecimalHalf);
    }

    #pragma endregion class_constructors

    #pragma region Negative_Status

    bool IsPositive() const;

    bool IsNegative() const;

    /// <summary>
    /// Swaps the negative status.
    /// </summary>
    void SwapNegativeStatus();

    /// <summary>
    /// Negative Unary Operator(Flips negative status)
    /// </summary>
    /// <param name="self">The self.</param>
    /// <returns>MediumDec</returns>
    MediumDec operator-() const
    { MediumDec self = *this; self.SwapNegativeStatus(); return self; }

    #pragma endregion Negative_Status


    #pragma region Check_if_value

    //Set value as exactly zero
    void SetAsZero();

    //Set value as exactly one
    void SetAsOne();

    //Set as +-1 while keeping current sign
    void SetAsOneVal();

    void SetAsValues(const MirroredInt& intVal = MirroredInt::Zero, const PartialInt& decVal = PartialInt::Zero);

    //Is at either zero or negative zero IntHalf of AltNum
    bool IsAtZeroInt() const;

    bool IsNotAtZeroInt() const;

    bool IsAtOneInt() const;

    bool IsNotAtOneInt() const;

    //Detect if at exactly zero(only overridden with MixedDec)
    bool IsZero() const;

    bool IsOne() const;

    bool IsNegOne() const;

    bool IsOneVal() const;

    bool IsOneVariantVal() const;

    #pragma endregion Check_if_value

    #pragma region RangeLimits

    /// <summary>
    /// Sets value to the highest non-infinite/Special Decimal State tValue that it store
    /// </summary>
    void SetAsMaximum();

    /// <summary>
    /// Sets value to the lowest non-infinite/Special Decimal State tValue that it store
    /// </summary>
    void SetAsMinimum();

    #pragma endregion RangeLimits

    #pragma region ValueSetters
protected://Work around for not allowing to use incomplete class statics during forming of class
    static const unsigned int LN10Div_DecSection = 434294482;
    static const unsigned int TwiceLN10Div_DecSection = 868588964;

public:

    /// <summary>
    /// Sets value to Pi(3.1415926535897932384626433) with tenth digit rounded up
    /// (Stored as 3.141592654)
    /// </summary>
    void  SetValueToPiNum();

    //100,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToHundredMilPiNum();

    //10,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToTenMilPiNum();

    //1,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToOneMilPiNum();

    //10xPi(Rounded to 9th decimal digit)
    void  SetValueToTenPiNum();

    /// <summary>
    /// Euler's number rounded to 9th digit(2.718281828)
    /// Irrational number equal to about (1 + 1/n)^n
    /// (about 2.71828182845904523536028747135266249775724709369995)
    /// </summary>
    void  SetValueToENum();

    //Sets value to value at 0.5
    void  SetValueToPoint5();

    void  SetValueToJustAboveZero();

    /// <summary>
    /// Sets the value at .000001000
    /// </summary>
    void  SetValueToOneMillionth();

    /// <summary>
    /// Sets the value at "0.005"
    /// </summary>
    /// <returns>MediumDec</returns>
    void  SetValueToFiveThousandth();

    /// <summary>
    /// Sets the value at "0.000005"
    /// </summary>
    void  SetValueToFiveMillionth();

    //0e-7
    void  SetValueToTenMillionth();

    /// <summary>
    /// Sets the value to .000000010
    /// </summary>
    void  SetValueToOneHundredMillionth();

    /// <summary>
    /// 2.3025850929940456840179914546844
    /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
    /// </summary>
    void  SetValueToLN10();

    /// <summary>
    /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    void  SetValueToLN10Div();

    /// <summary>
    /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    void  SetValueToTwiceLN10Div();

    void SetValueToPointOne();

    #pragma endregion ValueSetters

    #pragma region ValueDefines
public:
    #pragma region Integer constants
    static constexpr MediumDec Zero = MediumDec(MirroredInt::Zero);
    static constexpr MediumDec One = MediumDec(MirroredInt::One);
    static constexpr MediumDec Two = MediumDec(MirroredInt::Two);
    static constexpr MediumDec NegativeOne = MediumDec(MirroredInt::NegativeZero);
    #pragma endregion Integer constants

    #pragma region Core fractional constants
    //-0.5
    static constexpr MediumDec NegativePointFive = MediumDec(MirroredInt::NegativeZero, 500'000'000);
    //Almost one defined as nearest representable real number
    static constexpr MediumDec AlmostOne = MediumDec(MirroredInt::Zero, 999'999'999);
    static constexpr MediumDec PointFive = MediumDec(MirroredInt::Zero, 500'000'000);
    static constexpr MediumDec JustAboveZero = MediumDec(MirroredInt::Zero, 1);
    #pragma endregion Core fractional constants

    #pragma region Range limit constants
    static constexpr MediumDec Minimum = MediumDec(MirroredInt::Maximum, 999999999);
    static constexpr MediumDec Maximum = MediumDec(MirroredInt::Minimum, 999999999);
    #pragma endregion Range limit constants
    
    #pragma region Mathematical constants
    // Pi
    // π = ~ 3.141'592'653'589'793'238'462'643'3
    static constexpr MediumDec PiNum = MediumDec(3, 141592654);
    // Euler's number (Non-Alternative Representation)
    // Irrational number equal to about (1 + 1/n)^n
    // e = ~2.71828182845904523536028747135266249775724709369995
    static constexpr MediumDec ENum = MediumDec(2, 718281828);
    // π * e
    // = ~8.539'734'222'673567065463550869546574495034888535765084881233717265981654348037954472832304065619300439
    static constexpr MediumDec PiENum = MediumDec(8, 539'734'223);
    
    //Pi, and e are different in MediumDecV2 and higher variants(using FlagState multiplier constants)
    //static constexpr MediumDec Pi = PiNum;
    //static constexpr MediumDec E = ENum;
    //PiE constant only defined if PiE FlagState enabled
    
    static constexpr MediumDec LN10 = MediumDec(2, 302585093);
    static constexpr MediumDec LN10Div = MediumDec(MirroredInt::Zero, 434294482);
    static constexpr MediumDec TwiceLN10Div = MediumDec(MirroredInt::Zero, 868588964);
    
    //0.693147180559945309417232121458176568075500134360255254120680009493393621969694715605863326996418688
    static constexpr MediumDec Ln2 = MediumDec(MirroredInt::Zero, 693'147'181);
    
    static constexpr MediumDec NegLn2 = MediumDec(MirroredInt::NegativeZero, 693'147'181);
    #pragma endregion Mathematical constants
    
    #pragma region Pi Multipliers constants
    //Commenting Pi multipiers constants out since not needed until MediumDecV2
    //static constexpr MediumDec TenPiNum = MediumDec(31, 415'926'536);
    //static constexpr MediumDec OneMilPiNum = MediumDec(3'141'592, 653'589'793);
    //static constexpr MediumDec TenMilPiNum = MediumDec(31'415'926, 535'897'932);
    //static constexpr MediumDec HundredMilPiNum = MediumDec(314'159'265, 358'979'324);
    #pragma endregion Pi Multipliers constants
    
    #pragma region Small magnitude constants
    static constexpr MediumDec PointOne =       MediumDec(MirroredInt::Zero, 100'000'000);
    static constexpr MediumDec FiveThousandth = MediumDec(MirroredInt::Zero, 005'000'000);
    static constexpr MediumDec OneMillionth =   MediumDec(MirroredInt::Zero, 000'001'000);
    static constexpr MediumDec FiveMillionth =  MediumDec(MirroredInt::Zero, 000'005'000);
    static constexpr MediumDec TenMillionth =   MediumDec(MirroredInt::Zero, 000'010'000);
    static constexpr MediumDec FiveBillionth =  MediumDec(MirroredInt::Zero, 000'000'050);
    #pragma endregion Small magnitude constants
    
    #pragma endregion ValueDefines

    #pragma region String Commands

    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="tValue">The value.</param>
    void ReadString(const std::string& Value);

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
    /// </summary>
    /// <param name="strVal">The value.</param>
    MediumDec(const char* strVal);

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumDec"/> class.
    /// </summary>
    /// <param name="tValue">The value.</param>
    MediumDec(const std::string& Value);

public:

    /// <summary>
    /// Converts to string.
    /// </summary>
    /// <returns>std.string</returns>
    std::string ToString();

    /// <summary>
    /// Converts to string with digits filled in even when empty
    /// </summary>
    /// <returns>std.string</returns>
    std::string ToFullString();

    /// <summary>
    /// Implements the operator std::string operator.
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator std::string() { return ToString(); }

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

  #if defined(AltNum_EnableFloatingConversion)

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="tValue">The value.</param>
    void SetFloatVal(const float& tValue);

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="tValue">The value.</param>
    void SetDoubleVal(const double& tValue);

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="tValue">The value.</param>
    void SetDecimalVal(const long double& tValue);

  #endif

    /// <summary>
    /// Sets the value(false equals zero; otherwise is true).
    /// </summary>
    /// <param name="tValue">The value.</param>
    void SetBoolVal(const bool& Value);

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="tValue">The value.</param>
    void SetIntVal(const int& Value);

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="tValue">The value.</param>
    void SetUIntVal(const unsigned int& Value);

  #if defined(AltNum_EnableFloatingConversion)

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumDec"/> class.
    /// </summary>
    /// <param name="tValue">The value.</param>
    MediumDec(const float& tValue){ this->SetFloatVal(tValue); }

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumDec"/> class.
    /// </summary>
    /// <param name="tValue">The value.</param>
    MediumDec(const double& tValue){ this->SetDoubleVal(tValue); }

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumDec"/> class.
    /// </summary>
    /// <param name="tValue">The value.</param>
    MediumDec(const long double& tValue){ this->SetDecimalVal(tValue); }

  #endif

    MediumDec(const unsigned __int64& Value){ this->SetUIntVal(Value); }
    MediumDec(const signed __int64& Value){ this->SetIntVal(Value); }
    MediumDec(const unsigned char& Value){ this->SetUIntVal(Value); }
    MediumDec(const signed char& Value){ this->SetIntVal(Value); }
    MediumDec(const unsigned short& Value){ this->SetUIntVal(Value); }
    MediumDec(const signed short& Value){ this->SetIntVal(Value); }
    MediumDec(const unsigned int& Value){ this->SetUIntVal(Value); }

    /// <summary>
    /// Initializes a new instance of the <see cref="MediumDec"/> class.
    /// </summary>
    /// <param name="tValue">The value.</param>
    MediumDec(const bool& Value){ this->SetBoolVal(Value); }

  #pragma endregion ConvertFromOtherTypes

  #pragma region ConvertToOtherTypes

  #if defined(AltNum_EnableFloatingConversion)

    /// <summary>
    /// MediumDec Variant to float explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    float toFloat() const;

    /// <summary>
    /// MediumDec Variant to double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    double toDouble() const;

    /// <summary>
    /// MediumDec Variant to long double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    long double toDecimal() const;

  #endif

    /// <summary>
    /// MediumDec Variant to int explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    signed int toInt() const { return IntHalf.GetValue(); }

    /// <summary>
    /// MediumDec Variant to int explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    unsigned int toUInt() const { return IntHalf.IsNegative()?0:IntHalf.Value; }

    bool toBool() const { return IntHalf.IsZero() ? false : true; }

  #if defined(AltNum_EnableFloatingConversion)

    /// <summary>
    /// MediumDec Variant to float explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator float() { return toFloat(); }

    /// <summary>
    /// MediumDec Variant to double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator double() { return toDouble(); }

    /// <summary>
    /// MediumDec Variant to decimal explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator long double() { return toDecimal(); }

  #endif

    /// <summary>
    /// MediumDec Variant to int explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator signed int() { return toInt(); }

    /// <summary>
    /// MediumDec Variant to uint explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator unsigned int() { return toUInt(); }

    /// <summary>
    /// MediumDec Variant to int64 explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator signed __int64() { return toInt(); }

    /// <summary>
    /// MediumDec Variant to ubyte explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator unsigned char() { return toUInt(); }

    /// <summary>
    /// MediumDec Variant to byte explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator signed char() { return toInt(); }

    /// <summary>
    /// MediumDec Variant to ushort explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator unsigned short() { return toUInt(); }

    /// <summary>
    /// MediumDec Variant to short explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator signed short() { return toInt(); }

    /// <summary>
    /// MediumDec Variant to bool explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
protected:
    //Compare only as if in NormalType representation mode
    template<MediumDecVariant VariantType=MediumDec>
    std::strong_ordering BasicComparisonV1(const VariantType& that) const
    {
      if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
        return IntHalfCmp;
      //Counting negative zero as same as zero IntHalf but with negative DecimalHalf
      unsigned int lVal = IsNegative()?DecimalMax-DecimalHalf.Value:DecimalHalf.Value;
      unsigned int rVal = IsNegative()?DecimalMax-that.DecimalHalf.Value:that.DecimalHalf.Value;
      if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }

    std::strong_ordering BasicComparison(const MediumDec& that) const
    {
      return BasicComparisonV1(that);
    }

    //Compare only as if in NormalType representation mode
    std::strong_ordering BasicUIntComparison(const int& that) const
    {
      if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
        return IntHalfCmp;
      //Counting negative zero as same as zero IntHalf but with negative DecimalHalf
      unsigned int lVal = DecimalHalf.Value > 0 ? 1 : 0;
      if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }

    //Compare only as if in NormalType representation mode
    std::strong_ordering BasicIntComparison(const int& that) const
    {
      if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
        return IntHalfCmp;
      //Counting negative zero as same as zero IntHalf but with negative DecimalHalf
      unsigned int lVal = DecimalHalf.Value > 0 ? 1 : 0;
      if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }

public:

    std::strong_ordering operator<=>(const MediumDec& that) const
    {//return BasicComparison(that);
      if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
        return IntHalfCmp;
      //Counting negative zero as same as zero IntHalf but with negative DecimalHalf
      unsigned int lVal = IsNegative()?DecimalMax-DecimalHalf.Value:DecimalHalf.Value;
      unsigned int rVal = that.IsNegative()?DecimalMax-that.DecimalHalf.Value:that.DecimalHalf.Value;
      if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }

    std::strong_ordering operator<=>(const unsigned int& that) const
    {
      return BasicUIntComparison(that);
    }

    std::strong_ordering operator<=>(const signed int& that) const
    {
      return BasicIntComparison(that);
    }

    bool operator==(const MediumDec& that) const
    {
      if (IntHalf!=that.IntHalf)
        return false;
      if (DecimalHalf!=that.DecimalHalf)
        return false;
      return true;
    }

    bool operator!=(const MediumDec& that) const
    {
      if (IntHalf!=that.IntHalf)
        return true;
      if (DecimalHalf!=that.DecimalHalf)
        return true;
      return false;
    }

    bool operator==(const unsigned int& that) const
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator!=(const unsigned int& that) const
    {
      if (IntHalf!=that)
        return true;
      if (DecimalHalf!=0)
        return true;
      return false;
    }

    bool operator==(const signed int& that) const
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator!=(const signed int& that) const
    {
      if (IntHalf!=that)
        return true;
      if (DecimalHalf!=0)
        return true;
      return false;
    }

    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:

        template<IntegerType IntType=unsigned int>
        void PartialUIntDivOpV1(const IntType& rValue)
        {//Avoid using with special status representations such as approaching zero or result will be incorrect
            unsigned _int64 SelfRes;
            unsigned _int64 Res;
            unsigned _int64 IntHalfRes;
            unsigned _int64 DecimalRes;
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntHalf.Value: DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
            Res = SelfRes / rValue;

            IntHalfRes = Res/DecimalOverflowX;
            DecimalRes = Res - DecimalOverflowX * IntHalfRes;
            IntHalf.Value = (unsigned int)IntHalfRes;
            DecimalHalf.Value = (unsigned int)DecimalRes;
        }

        template<IntegerType IntType=signed int>
        void PartialIntDivOpV1(const IntType& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                PartialUIntDivOp(-Value);
            }
            else
                PartialUIntDivOp(Value);
        }

public:
        void PartialUIntDivOp(const unsigned int& rValue) { PartialUIntDivOpV1(rValue); }
        void PartialIntDivOp(const signed int& rValue) { PartialIntDivOpV1(rValue); }
        void PartialUInt64DivOp(const unsigned int& rValue) { PartialUIntDivOpV1(rValue); }
        void PartialInt64DivOp(const signed __int64& rValue) { PartialIntDivOpV1(rValue); }

        void UnsignedPartialIntDivOp(const signed int& rValue) { PartialUIntDivOpV1(rValue); }
        void UnsignedPartialInt64DivOp(const signed __int64& rValue) { PartialUIntDivOpV1(rValue); }

protected:

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void UIntDivOpV1(const IntType& Value)
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
        }

        template<IntegerType IntType=signed int>
        void IntDivOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialIntDivOpV1(Value);
            if (IntHalf == 0 && DecimalHalf == 0)
                DecimalHalf = 1;//Prevent Dividing into nothing
        }

        template<IntegerType IntType=unsigned int>
        auto& UIntDivOperationV1(const IntType& rValue)
        { UIntDivOpV1(rValue); return *this; }

        template<IntegerType IntType=unsigned int>
        auto& IntDivOperationV1(const IntType& rValue)
        { IntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        const auto DivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntDivOperationV1(rValue); }

        /// <summary>
        /// Basic division operation between MediumDec Variant and Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        const auto DivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntDivOperationV1(rValue); }

public:

        void UIntDivOp(const unsigned int& rValue) { UIntDivOpV1(rValue); }
        void IntDivOp(const signed int& rValue) { IntDivOpV1(rValue); }
        void UInt64DivOp(const unsigned __int64& rValue) { UIntDivOpV1(rValue); }
        void Int64DivOp(const signed __int64& rValue) { IntDivOpV1(rValue); }

        void UnsignedIntegerDivOp(const signed int& rValue) { UIntDivOpV1(rValue); }

        void UInt8DivOp(const unsigned char& rValue) { UIntDivOpV1(rValue); }
        void Int8DivOp(const signed char& rValue) { IntDivOpV1(rValue); }
        void UInt16DivOp(const unsigned short& rValue) { UIntDivOpV1(rValue); }
        void Int16DivOp(const signed short& rValue) { IntDivOpV1(rValue); }

        MediumDec& UIntDivOperation(const unsigned int& rValue) { return UIntDivOperationV1(rValue); }
        MediumDec& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
        MediumDec& UInt64DivOperation(const unsigned __int64& rValue) { return UIntDivOperationV1(rValue); }
        MediumDec& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
        MediumDec& UInt8DivOperation(const unsigned char& rValue) { return UIntDivOperationV1(rValue); }
        MediumDec& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
        MediumDec& UInt16DivOperation(const unsigned short& rValue) { return UIntDivOperationV1(rValue); }
        MediumDec& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

        const MediumDec DivideByUInt(const unsigned int& rValue) { return DivideByUIntV1(rValue); }
        const MediumDec DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        const MediumDec DivideByUInt64(const unsigned __int64& rValue) { return DivideByUIntV1(rValue); }
        const MediumDec DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        const MediumDec UnsignedDivideByInt(const signed int& rValue) { return DivideByUIntV1(rValue); }
        const MediumDec UnsignedDivideByInt64(const signed __int64& rValue) { return DivideByUIntV1(rValue); }

        const MediumDec DivideByUInt8(const unsigned char& rValue) { return DivideByUIntV1(rValue); }
        const MediumDec DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
        const MediumDec DivideByUInt16(const unsigned short& rValue) { return DivideByUIntV1(rValue); }
        const MediumDec DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep AltNum Division Operations
protected:

    /// <summary>
    /// Basic unsigned division operation(main code block)
    /// Return true if divide into zero
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    template<MediumDecVariant VariantType=MediumDec>
    bool UnsignedPartialDivOpV1(const VariantType& rValue)
    {
      unsigned _int64 SelfRes = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
      unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf.Value + rValue.DecimalHalf.Value;

      unsigned _int64 IntHalfRes = SelfRes / ValueRes;
      unsigned _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
      IntHalf.Value = (unsigned int) IntHalfRes;
      DecimalHalf.Value = DecimalRes;
      if (IntHalfRes == 0 && DecimalRes == 0)
        return true;
      else
        return false;
    }

    /// <summary>
    /// Unsigned division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    template<MediumDecVariant VariantType=MediumDec>
    void UnsignedDivOpV1(const VariantType& rValue)
    {
      if(DecimalHalf==0)
      {
        if(rValue.DecimalHalf==0)
        {
          switch(rValue.IntHalf.Value)
          {
            case 2:
              if((IntHalf.Value&1)==1)//Check if number is odd
                UIntDivOp(2);
              else
                IntHalf.Value /= 2;
              break;
            case 4:
              if(((IntHalf.Value >> 2) << 2) == IntHalf.Value)
                IntHalf.Value /= 4;
              else
                UIntDivOp(4);
              break;
            case 8:
              if(((IntHalf.Value >> 3) << 3) == IntHalf.Value)
                IntHalf.Value /= 8;
              else
                UIntDivOp(4);
              break;
            case 16:
              if(((IntHalf.Value >> 4) << 4) == IntHalf.Value)
                IntHalf.Value /= 16;
              else
                UIntDivOp(4);
              break;
            case 32:
              if(((IntHalf.Value >> 5) << 5) == IntHalf.Value)
                IntHalf.Value /= 32;
              else
                UIntDivOp(4);
              break;
            case 0:
              throw "Target rValue can not be divided by zero";
              break;
            default:
              UIntDivOp(rValue.IntHalf.Value);
              break;
          }
        }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
        else if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
            DecimalHalf.Value = 1;
#else
        else
          UnsignedPartialDivOp(rValue);
#endif
      }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
      else if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
        DecimalHalf.Value = 1;
#else
      else
        UnsignedPartialDivOp(rValue);
#endif
    }

    /// <summary>
    /// Basic division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    template<MediumDecVariant VariantType=MediumDec>
    void DivOpV1(const VariantType& rValue)
    {
      if(rValue.IsNegative())
      {
        SwapNegativeStatus();
        UnsignedDivOp(-rValue);
      }
      else
        UnsignedDivOp(rValue);
    }

    template<MediumDecVariant VariantType=MediumDec>
    static VariantType UnsignedDivisionV1(VariantType lValue, const VariantType& rValue)
    {
      lValue.UnsignedDivOpV1(rValue); return lValue;
    }

public:

    /// <summary>
    /// Basic unsigned division operation(main code block)
    /// Return true if divide into zero
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The rValue</param>
    bool UnsignedPartialDivOp(const MediumDec& rValue){ return UnsignedPartialDivOpV1(rValue); }

    /// <summary>
    /// Unsigned division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    void UnsignedDivOp(const MediumDec& rValue){ UnsignedDivOpV1(rValue); }

    /// <summary>
    /// Basic division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    void DivOp(const MediumDec& rValue){ DivOpV1(rValue); }

    /// <summary>
    /// Basic unsigned division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    MediumDec& UnsignedDivOperation(const MediumDec& rValue)
    { UnsignedDivOp(rValue); return *this; }

    /// <summary>
    /// Basic division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    MediumDec& DivOperation(const MediumDec& rValue)
    { DivOp(rValue); return *this; }

    /// <summary>
    /// Basic unsigned division operation that ignores special decimal status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    MediumDec DivideByUnsigned(const MediumDec& rValue) const
    { MediumDec lValue = *this; return lValue.UnsignedDivOperation(rValue); }

    /// <summary>
    /// Basic division operation that ignores special decimal status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    MediumDec DivideBy(const MediumDec& rValue) const
    { MediumDec lValue = *this; return lValue.DivOperation(rValue); }

    #pragma endregion NormalRep AltNum Division Operations

    #pragma region Other Division Operations

    /// <summary>
    /// Simplified division by 2 operation(to reduce cost of operations)
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    /// <returns>MediumDec&</returns>
    void DivideByTwo();

    /// <summary>
    /// Simplified division by 4 operation(to reduce cost of operations)
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    /// <returns>MediumDec&</returns>
    void DivideByFour();

    //Return copy of result divided by two
    MediumDec DividedByTwo() const;

    //Return copy of result divided by four
    MediumDec DividedByFour() const;

    /// <summary>
    /// /= operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>MediumDec</returns>
    friend MediumDec& operator/=(MediumDec& lValue, const MediumDec& rValue) { return lValue.DivOperation(rValue); }

    /// <summary>
    /// *= operation between MediumDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>MediumDec</returns>
    friend MediumDec& operator/=(MediumDec& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
    friend MediumDec& operator/=(MediumDec& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
    friend MediumDec& operator/=(MediumDec& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
    friend MediumDec& operator/=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

    friend MediumDec& operator/=(MediumDec& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
    friend MediumDec& operator/=(MediumDec& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
    friend MediumDec& operator/=(MediumDec& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
    friend MediumDec& operator/=(MediumDec& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

    /// <summary>
    /// Division operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>MediumDec</returns>
    friend MediumDec operator/(MediumDec lValue, const MediumDec& rValue) { return lValue.DivideBy(rValue); }

    /// <summary>
    /// Division operation between MediumDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>MediumDec</returns>
    friend MediumDec operator/(MediumDec lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
    friend MediumDec operator/(MediumDec lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
    friend MediumDec operator/(MediumDec lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
    friend MediumDec operator/(MediumDec lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

    friend MediumDec operator/(MediumDec lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
    friend MediumDec operator/(MediumDec lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
    friend MediumDec operator/(MediumDec lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
    friend MediumDec operator/(MediumDec lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

    friend MediumDec operator/(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
    friend MediumDec operator/(const signed __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
    friend MediumDec operator/(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
    friend MediumDec operator/(const unsigned __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }

    friend MediumDec operator/(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
    friend MediumDec operator/(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
    friend MediumDec operator/(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }
    friend MediumDec operator/(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).DivideBy(rValue); }

    #pragma endregion Other Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
    /// <summary>
    /// Partial version of UIntMultOpV1 without zero checks
    /// (Modifies owner object)
    /// </summary>
    /// <param name="tValue">The value.</param>
    /// <returns>MediumDec</returns>
    template<IntegerType IntType=int>
    void PartialUIntMultOpV1(const IntType& rValue)
    {
      if (DecimalHalf == 0)
        IntHalf.Value *= rValue;
      else
      {
        __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
        SRep *= rValue;
        if (SRep >= DecimalOverflowX)
        {
          __int64 OverflowVal = SRep / DecimalOverflowX;
          SRep -= OverflowVal * DecimalOverflowX;
          IntHalf.Value = (unsigned int)OverflowVal;
          DecimalHalf.Value = (unsigned int)SRep;
        }
        else
        {
          IntHalf.Value = 0;
          DecimalHalf.Value = (unsigned int)SRep;
        }
      }
    }

    //Partial version of BasicIntMultOpV1 without zero checks
    //Modifies owner object
    template<IntegerType IntType=signed int>
    void PartialIntMultOpV1(const IntType& rValue)
    {
      if(rValue<0)
      {
        SwapNegativeStatus();
        PartialUIntMultOpV1(-rValue);
      }
      else
        PartialUIntMultOp(rValue);
    }
public:

    void PartialUIntMultOp(const unsigned int& rValue) { PartialUIntMultOpV1(rValue); }
    void PartialIntMultOp(const signed int& rValue) { PartialIntMultOpV1(rValue); }
    void PartialUInt64MultOp(const unsigned __int64& rValue) { PartialUIntMultOpV1(rValue); }
    void PartialInt64MultOp(const signed __int64& rValue) { PartialIntMultOpV1(rValue); }

    void UnsignedPartialIntMultOp(const signed int& rValue) { PartialUIntMultOpV1(rValue); }
    void UnsignedPartialInt64MultOp(const signed __int64& rValue) { PartialUIntMultOpV1(rValue); }

protected:
    template<IntegerType IntType=signed int>
    void UIntMultOpV1(const IntType& rValue)
    {
      if (rValue == 0)
      {
        SetAsZero();
        return;
      }
      else if (IsZero())
        return;
      PartialUIntMultOpV1(rValue);
    }

    template<IntegerType IntType=signed int>
    void IntMultOpV1(const IntType& Value)
    {
      if (Value == 0)
      {
        SetAsZero();
        return;
      }
      else if (IsZero())
        return;
      PartialIntMultOpV1(Value);
    }

    template<IntegerType IntType=unsigned int>
    auto& UIntMultOperationV1(const IntType& Value)
    { UIntMultOpV1(Value); return *this; }

    template<IntegerType IntType=unsigned int>
    auto& IntMultOperationV1(const IntType& Value)
    { IntMultOpV1(Value); return *this; }

    /// <summary>
    /// Basic multiplication operation between MediumDec variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    const MediumDec MultiplyByUIntV1(const IntType& rValue)
    { auto self = *this; return self.UIntMultOperationV1(rValue); }

    /// <summary>
    /// Basic multiplication operation between MediumDec variant and Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=signed int>
    const MediumDec MultiplyByIntV1(const IntType& rValue)
    { auto self = *this; return self.IntMultOperationV1(rValue); }

    template<MediumDecVariant VariantType=MediumDec, IntegerType IntType=unsigned int>
    static VariantType UIntMultiplicationV1(VariantType lValue, const IntType& rValue)
    { return lValue.UIntMultOperationV1(rValue); }

public:

    void UIntMultOp(const unsigned int& rValue) { UIntMultOpV1(rValue); }
    void IntMultOp(const signed int& rValue) { IntMultOpV1(rValue); }
    void UInt64MultOp(const unsigned __int64& rValue) { UIntMultOpV1(rValue); }
    void Int64MultOp(const signed __int64& rValue) { IntMultOpV1(rValue); }

    void UnsignedIntegerMultOp(const signed int& rValue) { UIntMultOpV1(rValue); }

    void UInt8MultOp(const unsigned char& rValue) { UIntMultOpV1(rValue); }
    void Int8MultOp(const signed char& rValue) { IntMultOpV1(rValue); }
    void UInt16MultOp(const unsigned short& rValue) { UIntMultOpV1(rValue); }
    void Int16MultOp(const signed short& rValue) { IntMultOpV1(rValue); }

    MediumDec& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
    MediumDec& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
    MediumDec& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
    MediumDec& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
    MediumDec& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
    MediumDec& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
    MediumDec& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
    MediumDec& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

    const MediumDec MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
    const MediumDec MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
    const MediumDec MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
    const MediumDec MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

    const MediumDec UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
    const MediumDec UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

    const MediumDec MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
    const MediumDec MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
    const MediumDec MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
    const MediumDec MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep AltNum Multiplication Operations
protected:

    /// <summary>
    /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    template<MediumDecVariant VariantType=MediumDec>
    void UnsignedMultOpV1(const VariantType& rValue)
    {
      if (DecimalHalf == 0)
      {
        if (IntHalf.Value == 1)
        {
          if(IntHalf.IsNegative())
            IntHalf = -rValue.IntHalf;
          else
            IntHalf = rValue.IntHalf;
          DecimalHalf = rValue.DecimalHalf;
        }
        else if (rValue.DecimalHalf == 0)
          IntHalf *= rValue.IntHalf;
        else {
          __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf.Value : DecimalOverflowX * rValue.IntHalf.Value + rValue.DecimalHalf.Value;
          if (rRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = rRep / DecimalOverflowX;
            rRep -= OverflowVal * DecimalOverflowX;
            IntHalf.Value = (unsigned int)OverflowVal;
            DecimalHalf.Value = (unsigned int)rRep;
            return;
          }
          else
          {
            DecimalHalf = (signed int)rRep;
          #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0)
              DecimalHalf.Value = 1;
          #elif !defined(AltNum_AllowNegativeZero)
            if(DecimalHalf==0){
              SetAsZero(); return; }
          #endif
            IntHalf.Value = 0;
            return;
          }
        }
      }
      else if (IntHalf.Value == 0)
      {
        __int64 SRep = (__int64)DecimalHalf;
        SRep *= rValue.DecimalHalf.Value;
        SRep /= DecimalOverflowX;
        if (rValue.IntHalf == 0)
        {
          DecimalHalf = (signed int)SRep;
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
          if (DecimalHalf == 0)
            DecimalHalf.Value = 1;
        #elif !defined(AltNum_AllowNegativeZero)
          if(DecimalHalf==0){
            SetAsZero(); return; }
        #endif
          return;
        }
        else
        {
          SRep += (__int64)DecimalHalf.Value * rValue.IntHalf.Value;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf.Value = OverflowVal;
            DecimalHalf.Value = (signed int)SRep;
            return;
          }
          else
          {
            DecimalHalf.Value = (unsigned int)SRep;
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0)
              DecimalHalf.Value = 1;
        #elif !defined(AltNum_AllowNegativeZero)
            if(DecimalHalf==0){
              SetAsZero(); return; }
        #endif
            return;
          }
        }
      }
      else
      {
        if (rValue.DecimalHalf == 0)//Y is integer
        {
          __int64 SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
          SRep *= rValue.IntHalf.Value;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf.Value = (unsigned int)OverflowVal;
            DecimalHalf.Value = (unsigned int)SRep;
          }
          else
          {
            DecimalHalf.Value = (unsigned int)SRep;
            if(DecimalHalf==0)
            {
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
              if(DecimalHalf==0)
                DecimalHalf.Value = 1;
        #elif !defined(AltNum_AllowNegativeZero)
            if(DecimalHalf==0){
              SetAsZero(); return; }
        #endif
            }
            IntHalf.Value = 0;
          }
          return;
        }
        else if (rValue.IntHalf == 0)
        {
          __int64 SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
          SRep *= rValue.DecimalHalf.Value;
          SRep /= DecimalOverflowX;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf.Value = (unsigned int)OverflowVal;
            DecimalHalf.Value = (unsigned int)SRep;
          }
          else
          {
            DecimalHalf.Value = (unsigned int)SRep;
            if(DecimalHalf==0)
            {
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
              if(DecimalHalf==0)
                DecimalHalf.Value = 1;
        #elif !defined(AltNum_AllowNegativeZero)
            if(DecimalHalf==0){
              SetAsZero(); return; }
        #endif
            }
            IntHalf.Value = 0;
          }
          return;
        }
        else
        {
          //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
          unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
          SRep *= rValue.IntHalf.Value;//SRep holds __int64 version of X.Y * Z
          //X.Y *.V
          unsigned __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf.Value);//Temp03 holds __int64 version of X *.V
          unsigned __int64 Temp04 = (__int64)DecimalHalf.Value * (__int64)rValue.DecimalHalf.Value;
          Temp04 /= DecimalOverflow;
          //Temp04 holds __int64 version of .Y * .V
          unsigned __int64 IntegerRep = SRep + Temp03 + Temp04;
          unsigned __int64 intHalf = IntegerRep / DecimalOverflow;
          IntegerRep -= intHalf * DecimalOverflowX;
          IntHalf.Value = (unsigned int) intHalf;
          DecimalHalf.Value = (unsigned int)IntegerRep;
        }
      }
      #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
      if(DecimalHalf==0&&IntHalf==0)
        DecimalHalf.Value = 1;
      #elif !defined(AltNum_AllowNegativeZero)
      if(DecimalHalf==0)
        SetAsZero();
      #endif
    }

    template<MediumDecVariant VariantType=MediumDec>
    void MultOpV1(const VariantType& rValue)
    {
      if(rValue.IsNegative())
      {
        SwapNegativeStatus();
        UnsignedMultOp(-rValue);
      }
      else
        UnsignedMultOp(rValue);
    }

    template<MediumDecVariant VariantType=MediumDec>
    static VariantType UnsignedMultiplicationV1(VariantType lValue, const VariantType& rValue)
    {
      lValue.UnsignedMultOpV1(rValue); return lValue;
    }

public:

    /// <summary>
    /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    void UnsignedMultOp(const MediumDec& rValue){ UnsignedMultOpV1(rValue); }

    void MultOp(const MediumDec& rValue){ MultOpV1(rValue); }

    /// <summary>
    /// Basic unsigned multiplication operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    MediumDec& UnsignedMultOperation(const MediumDec& rValue)
    { UnsignedMultOp(rValue); return *this; }

    /// <summary>
    /// Basic multiplication operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    MediumDec& MultOperation(const MediumDec& rValue)
    { MultOp(rValue); return *this; }

    /// <summary>
    /// Basic unsigned multiplication operation that ignores special decimal status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    MediumDec MultiplyByUnsigned(const MediumDec& rValue) const
    { MediumDec lValue = *this; return lValue.UnsignedMultOperation(rValue); }

    /// <summary>
    /// Basic multiplication operation that ignores special decimal status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    MediumDec MultiplyBy(const MediumDec& rValue) const
    { MediumDec lValue = *this; return lValue.MultOperation(rValue); }

    #pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region Other multiplication operations

    /// <summary>
    /// Simplified multiplication by 2 operation(to reduce cost of operations)
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    /// <returns>void</returns>
    void MultiplyByTwo();

    /// <summary>
    /// Simplified multiplication by 4 operation(to reduce cost of operations)
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    /// <returns>void</returns>
    void MultiplyByFour();

    //Return copy of result divided by two
    MediumDec MultipliedByTwo() const;

    //Return copy of result divided by four
    MediumDec MultipliedByFour() const;

    /// <summary>
    /// *= operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>MediumDec</returns>
    friend MediumDec& operator*=(MediumDec& lValue, const MediumDec& rValue) { return lValue.MultOperation(rValue); }

    /// <summary>
    /// *= operation between MediumDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    friend MediumDec& operator*=(MediumDec& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
    friend MediumDec& operator*=(MediumDec& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
    friend MediumDec& operator*=(MediumDec& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
    friend MediumDec& operator*=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

    friend MediumDec& operator*=(MediumDec& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
    friend MediumDec& operator*=(MediumDec& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
    friend MediumDec& operator*=(MediumDec& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
    friend MediumDec& operator*=(MediumDec& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

    /// <summary>
    /// Multiplication operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>MediumDec</returns>
    friend MediumDec operator*(MediumDec lValue, const MediumDec& rValue) { return lValue.MultOperation(rValue); }

    /// <summary>
    /// Multiplication operation between MediumDec variant and Integer rValue.
    /// </summary>
    /// <param name="self">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    friend MediumDec operator*(MediumDec lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
    friend MediumDec operator*(MediumDec lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
    friend MediumDec operator*(MediumDec lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
    friend MediumDec operator*(MediumDec lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

    friend MediumDec operator*(MediumDec lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
    friend MediumDec operator*(MediumDec lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
    friend MediumDec operator*(MediumDec lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
    friend MediumDec operator*(MediumDec lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

    friend MediumDec operator*(signed int lValue, MediumDec rValue)  { return rValue.IntMultOperation(lValue); }
    friend MediumDec operator*(signed __int64 lValue, MediumDec& rValue)  { return rValue.Int64MultOperation(lValue); }
    friend MediumDec operator*(unsigned int lValue, MediumDec& rValue)  { return rValue.UIntMultOperation(lValue); }
    friend MediumDec operator*(unsigned __int64 lValue, MediumDec& rValue)  { return rValue.UInt64MultOperation(lValue); }

    friend MediumDec operator*(signed char lValue, MediumDec& rValue)  { return rValue.Int8MultOperation(lValue); }
    friend MediumDec operator*(signed short lValue, MediumDec& rValue)  { return rValue.Int16MultOperation(lValue); }
    friend MediumDec operator*(unsigned char lValue, MediumDec& rValue)  { return rValue.UInt8MultOperation(lValue); }
    friend MediumDec operator*(unsigned short lValue, MediumDec& rValue)  { return rValue.UInt16MultOperation(lValue); }

    #pragma endregion Other multiplication operations

    #pragma region NormalRep Integer Addition Operations
protected:

    /// <summary>
    /// Basic addition operation between MediumDec Variant and unsigned Integer value
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    void UIntAddOpV1(const IntType& rValue)
    {
      if(DecimalHalf.Value==0)
        IntHalf.NRepSkippingUIntAddOp(rValue);
      else {
        int signBeforeOp = IntHalf.Sign;
        IntHalf.UIntAddOp(rValue);
        if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
          DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
      }
    }

    /// <summary>
    /// Basic addition operation between MediumDec Variant and Integer value
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=signed int>
    void IntAddOpV1(const IntType& rValue)
    {
      if(DecimalHalf.Value==0)
        IntHalf.NRepSkippingIntegerAddOp(rValue);
      else {
        int signBeforeOp = IntHalf.Sign;
        IntHalf += rValue;
        if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
          DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
      }
    }

    template<IntegerType IntType=unsigned int>
    auto& UIntAddOperationV1(const IntType& rValue)
    { UIntAddOpV1(rValue); return *this; }

    template<IntegerType IntType=signed int>
    auto& IntAddOperationV1(const IntType& rValue)
    { IntAddOpV1(rValue); return *this; }

    /// <summary>
    ///  addition operation between MediumDec variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    auto AddByUIntV1(const IntType& rValue) const
    { auto self = *this; return self.UIntAddOperationV1(rValue); }

    /// <summary>
    /// Basic addition operation between MediumDec variant and Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=signed int>
    auto AddByIntV1(const IntType& rValue) const
    { auto self = *this; return self.IntAddOperationV1(rValue); }

public:

    /// <summary>
    /// Basic addition operation between MediumDec Variant and unsigned MirroredInt
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    void UnsignedMirroredAddOp(const MirroredInt& rValue);

    /// <summary>
    /// Basic addition operation between MediumDec Variant and MirroredInt
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    void MirroredAddOp(const MirroredInt& rValue);

    void UInt8AddOp(const unsigned char& rValue) { UIntAddOpV1(rValue); }
    void UInt16AddOp(const unsigned short& rValue) { UIntAddOpV1(rValue); }
    void UIntAddOp(const unsigned int& rValue);
    void UInt64AddOp(const unsigned __int64& rValue) { UIntAddOpV1(rValue); }

    void UnsignedIntegerAddOp(const signed int& rValue) { UIntAddOpV1(rValue); }

    MediumDec& UInt8AddOperation(const unsigned char& rValue) { return UIntAddOperationV1(rValue); }
    MediumDec& UInt16AddOperation(const unsigned short& rValue) { return UIntAddOperationV1(rValue); }
    MediumDec& UIntAddOperation(const unsigned int& rValue);
    MediumDec& UInt64AddOperation(const unsigned __int64& rValue) { return UIntAddOperationV1(rValue); }

    MediumDec UnsignedAddByInt(const signed int& rValue) { return AddByUIntV1(rValue); }
    MediumDec UnsignedAddByInt64(const signed __int64& rValue) { return AddByUIntV1(rValue); }

    MediumDec AddByUInt8(const unsigned char& rValue) { return AddByUIntV1(rValue); }
    MediumDec AddByUInt16(const unsigned short& rValue) { return AddByUIntV1(rValue); }
    MediumDec AddByUInt(const unsigned int& rValue) { return AddByUIntV1(rValue); }
    MediumDec AddByUInt64(const unsigned __int64& rValue) { return AddByUIntV1(rValue); }

    void Int8AddOp(const signed char& rValue) { IntAddOpV1(rValue); }
    void Int16AddOp(const signed short& rValue) { IntAddOpV1(rValue); }
    void IntAddOp(const signed int& rValue) { IntAddOpV1(rValue); }
    void Int64AddOp(const signed __int64& rValue) { IntAddOpV1(rValue); }

    MediumDec& Int8AddOperation(const signed char& rValue) { return IntAddOperationV1(rValue); }
    MediumDec& Int16AddOperation(const signed short& rValue) { return IntAddOperationV1(rValue); }
    MediumDec& IntAddOperation(const signed int& rValue) { return IntAddOperationV1(rValue); }
    MediumDec& Int64AddOperation(const signed __int64& rValue) { return IntAddOperationV1(rValue); }

    MediumDec AddByInt8(const signed char rValue) { return AddByIntV1(rValue); }
    MediumDec AddByInt16(const signed short& rValue) { return AddByIntV1(rValue); }
    MediumDec AddByInt(const signed int& rValue) { return AddByIntV1(rValue); }
    MediumDec AddByInt64(const signed __int64& rValue) { return AddByIntV1(rValue); }

    #pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations
protected:

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void UIntSubOpV1(const IntType& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf.NRepSkippingUIntSubOp(rValue);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf.UIntSubOp(rValue);
                if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
            }
        }

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void IntSubOpV1(const IntType& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf.NRepSkippingIntegerSubOp(rValue);
            else {
                unsigned int signBeforeOp = IntHalf.Sign;
                IntHalf -= rValue;
                if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
            }
        }

        template<IntegerType IntType=unsigned int>
        auto& UIntSubOperationV1(const IntType& rValue)
        { UIntSubOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& IntSubOperationV1(const IntType& rValue)
        { IntSubOpV1(rValue); return *this; }

        /// <summary>
        /// Basic Subtraction operation between MediumDec variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto SubtractByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntSubOperationV1(rValue); }

        /// <summary>
        /// Basic Subtraction operation between MediumDec variant and Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto SubtractByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntSubOperationV1(rValue); }

public:

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UnsignedMirroredSubOp(const MirroredInt& rValue);

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void MirroredSubOp(const MirroredInt& rValue);

        void UInt8SubOp(const unsigned char& rValue) { UIntSubOpV1(rValue); }
        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UIntSubOp(const unsigned int& rValue);
        void UInt16SubOp(const unsigned short& rValue) { UIntSubOpV1(rValue); }
        void UInt64SubOp(const unsigned __int64& rValue) { UIntSubOpV1(rValue); }

        void UnsignedIntegerSubOp(const signed int& rValue) { UIntSubOpV1(rValue); }

        void Int8SubOp(const signed char& rValue) { IntSubOpV1(rValue); }
        void IntSubOp(const signed int& rValue) { IntSubOpV1(rValue); }
        void Int16SubOp(const signed short& rValue) { IntSubOpV1(rValue); }
        void Int64SubOp(const signed __int64& rValue) { IntSubOpV1(rValue); }

        MediumDec& UInt8SubOperation(const unsigned char& rValue) { return UIntSubOperationV1(rValue); }
        MediumDec& UInt16SubOperation(const unsigned short& rValue) { return UIntSubOperationV1(rValue); }
        MediumDec& UIntSubOperation(const unsigned int& rValue);
        MediumDec& UInt64SubOperation(const unsigned __int64& rValue) { return UIntSubOperationV1(rValue); }

        MediumDec UnsignedSubtractByInt(const signed int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec UnsignedSubtractByInt64(const signed __int64& rValue) { return SubtractByUIntV1(rValue); }

        MediumDec SubtractByUInt8(const unsigned char& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec SubtractByUInt16(const unsigned short& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec SubtractByUInt(const unsigned int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDec SubtractByUInt64(const unsigned __int64& rValue) { return SubtractByUIntV1(rValue); }

        MediumDec& Int8SubOperation(const signed char& rValue) { return IntSubOperationV1(rValue); }
        MediumDec& Int16SubOperation(const signed short& rValue) { return IntSubOperationV1(rValue); }
        MediumDec& IntSubOperation(const signed int& rValue) { return IntSubOperationV1(rValue); }
        MediumDec& Int64SubOperation(const signed __int64& rValue) { return IntSubOperationV1(rValue); }

        MediumDec SubtractByInt8(const signed char rValue) { return SubtractByIntV1(rValue); }
        MediumDec SubtractByInt16(const signed short& rValue) { return SubtractByIntV1(rValue); }
        MediumDec SubtractByInt(const signed int& rValue) { return SubtractByIntV1(rValue); }
        MediumDec SubtractByInt64(const signed __int64& rValue) { return SubtractByIntV1(rValue); }

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep AltNum Addition/Subtraction Operations
protected:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDec>
        void UnsignedAddOpV1(const VariantType& rValue)
        {
            if(rValue.DecimalHalf==0)
                UnsignedMirroredAddOp(rValue.IntHalf);
            else
            {
                int signBeforeOp = IntHalf.Sign;
                IntHalf += rValue.IntHalf;
                if (signBeforeOp==MirroredInt::NegativeSign)
                {
                    if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -4.5
                        if(IntHalf.Value==0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    }
                    else if(rValue.DecimalHalf.Value>DecimalHalf.Value)
                    {
                        ++IntHalf;
                        if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
                            DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                        else//-1.6 + .7 = -0.9
                            DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
                    } else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
                        DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10
                    else
                        DecimalHalf.Value -= rValue.DecimalHalf.Value;
                } else {
                    unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                    if(decResult==DecimalOverflow){//5.4 + 4.6
                        ++IntHalf;
                        if(IntHalf.Value==0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    } else if(decResult>DecimalOverflow){//5.4 + 4.7
                        ++IntHalf;
                        DecimalHalf.Value = decResult - DecimalOverflow;
                    }
                    else if(signBeforeOp!=IntHalf.Sign)
                        DecimalHalf.Value = DecimalOverflow - decResult;
                    else
                        DecimalHalf.Value = decResult;
                }
            }
        }

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDec>
        void AddOpV1(const VariantType& rValue)
        {
            if (rValue.DecimalHalf == 0)
                MirroredAddOp(rValue.IntHalf);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf += rValue.IntHalf;

                if (signBeforeOp==MirroredInt::NegativeSign){
                    if(rValue.IsPositive()){
                        if(DecimalHalf.Value==rValue.DecimalHalf.Value){
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
                            ++IntHalf;
                            if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
                                DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                            else//-1.6 + .7 = -0.9
                                DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
                        } else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
                            DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10
                        else
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
                    } else {
                        unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                        if (decResult == DecimalOverflow) {//-5.4 + - 5.6
                            --IntHalf;
                            if (IntHalf.Value == 0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1
                            --IntHalf;
                            DecimalHalf.Value = decResult - DecimalOverflow;
                        } else//-5.2 - 5.2 = -10.4
                            DecimalHalf.Value = decResult;
                    }
                } else {
                    if(rValue.IsPositive()){
                        unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                        if(decResult==DecimalOverflow){//5.5 + 4.5 = 10
                            ++IntHalf;
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(decResult>DecimalOverflow){//5.5 + 4.6 = 10.1
                            ++IntHalf;
                            DecimalHalf.Value = decResult - DecimalOverflow;
                        } else//5.4 + 5.3 = 10.7
                            DecimalHalf.Value = decResult;
                    } else {
                        if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -5.5
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
                            --IntHalf;
                            if(signBeforeOp!=IntHalf.Sign)//4.3 - 5.4 = -1.1
                                DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                            else//4.3 - 2.4 = 1.9
                                DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                        } else if(signBeforeOp!=IntHalf.Sign)//5.4 + - 6.3 = -0.9
                            DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 + 3 - 4
                        else//4.4 + -2.3 = 2.1
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
                    }
                }
            }
        }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDec>
        void UnsignedSubOpV1(const VariantType& rValue)
        {
            if (rValue.DecimalHalf == 0)
                UnsignedMirroredSubOp(rValue.IntHalf);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf -= rValue.IntHalf;
                if (signBeforeOp==MirroredInt::NegativeSign){//-5 - B
                    unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                    if (decResult == DecimalOverflow){//-5.4 - 5.6
                        --IntHalf;
                        if (IntHalf.Value == 0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1
                        --IntHalf;
                        DecimalHalf.Value = decResult - DecimalOverflow;
                    } else//-5.2 - 5.2 = -10.4
                        DecimalHalf.Value = decResult;
                } else {//5.XX - B
                    if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - 5.5 = 10
                        if(IntHalf.Value==0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
                        --IntHalf;
                        if(signBeforeOp!=IntHalf.Sign)//5.4 - 5.7 = -0.3
                            DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                        else//5.4 - 3.6 = 1.8
                            DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                    } else if(signBeforeOp!=IntHalf.Sign)//5.3 - 7.2 = -1.9
                        DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
                    else//5.4 - 5.3 = 0.1
                        DecimalHalf.Value -= rValue.DecimalHalf.Value;
                }
            }
        }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDec>
        void SubOpV1(const VariantType& rValue)
        {
            if(rValue.DecimalHalf==0)
                MirroredSubOp(rValue.IntHalf);
            else
            {
                int signBeforeOp = IntHalf.Sign;
                IntHalf -= rValue.IntHalf;
                if (signBeforeOp==MirroredInt::NegativeSign)
                {
                    if(rValue.IsPositive())
                    {
                        unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                        if(decResult==DecimalOverflow){//-5.4 - 5.6
                            --IntHalf;
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(decResult>DecimalOverflow){//-5.4 - 5.7 = -11.1
                            --IntHalf;
                            DecimalHalf.Value = decResult - DecimalOverflow;
                        } else//-5.2 - 5.2 = -10.4
                            DecimalHalf.Value = decResult;
                    } else {
                        if(DecimalHalf.Value==rValue.DecimalHalf.Value){//-5.4 - -4.4
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
                            ++IntHalf;
                            if(signBeforeOp!=IntHalf.Sign)//-5.4 - -6.5 = 1.1
                                DecimalHalf = DecimalOverflow - DecimalHalf.Value - rValue.DecimalHalf;
                            else//-5.4 - -4.5 = -0.9 == -5.4 + 4.5
                                DecimalHalf.Value += rValue.DecimalHalf.Value;
                        } else if(signBeforeOp!=IntHalf.Sign)//-5.4 - -7.3 = 1.9
                            DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;
                        else//-5.4 - -3.3 = -2.1
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
                    }
                } else {
                    if(rValue.IsPositive()){
                        if (DecimalHalf.Value == rValue.DecimalHalf.Value) {//5.5 - 5.5 = 10
                            if (IntHalf.Value == 0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        }
                        else if (rValue.DecimalHalf.Value > DecimalHalf.Value) {
                            --IntHalf;
                            if (signBeforeOp != IntHalf.Sign)//5.4 - 5.7 = -0.3
                                DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                            else//5.4 - 3.6 = 1.8
                                DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                        }
                        else if (signBeforeOp != IntHalf.Sign)//5.3 - 7.2 = -1.9
                            DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
                        else//5.4 - 5.3 = 0.1
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
                    } else {
                        if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - -5.5 = 11
                            ++IntHalf;
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){//5.4 - -5.7 = 11.1
                            ++IntHalf;
                            DecimalHalf.Value = DecimalHalf.Value + rValue.DecimalHalf.Value - DecimalOverflow;
                        } else//5.4 - -5.3 = 10.7
                            DecimalHalf.Value += rValue.DecimalHalf.Value;
                    }
                }
            }
        }

        template<MediumDecVariant VariantType=MediumDec>
        static VariantType UnsignedAdditionV1(VariantType lValue, const VariantType& rValue)
        {
            lValue.UnsignedAddOpV1(rValue); return lValue;
        }

        template<MediumDecVariant VariantType=MediumDec>
        static VariantType UnsignedSubtractionV1(VariantType lValue, const VariantType& rValue)
        {
            lValue.UnsignedSubOpV1(rValue); return lValue;
        }

public:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedAddOp(const MediumDec& rValue){ UnsignedAddOpV1(rValue); }

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOp(const MediumDec& rValue){ AddOpV1(rValue); }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedSubOp(const MediumDec& rValue){ UnsignedSubOpV1(rValue); }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const MediumDec& rValue){ SubOpV1(rValue); }

        //Basic addition operation
        MediumDec& UnsignedAddOperation(const MediumDec& rValue)
        { UnsignedAddOp(rValue); return *this; }

        //Basic addition operation
        MediumDec& AddOperation(const MediumDec& rValue)
        { AddOp(rValue); return *this; }

        /// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDec AddByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.UnsignedAddOperation(rValue); } const

        /// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDec AddBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.AddOperation(rValue); } const

        //Basic subtraction operation
        MediumDec& UnsignedSubOperation(const MediumDec& rValue)
        { UnsignedSubOp(rValue); return *this; }

        //Basic subtraction operation
        MediumDec& SubOperation(const MediumDec& rValue)
        { SubOp(rValue); return *this; }

        /// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDec SubtractByUnsigned(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.UnsignedSubOperation(rValue); } const

        /// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDec SubtractBy(const MediumDec& rValue)
        { MediumDec lValue = *this; return lValue.SubOperation(rValue); } const

    #pragma endregion NormalRep AltNum Addition/Subtraction Operations

    #pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator+=(MediumDec& lValue, const MediumDec& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// += operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec& operator+=(MediumDec& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumDec& operator+=(MediumDec& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDec& operator+=(MediumDec& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator+(MediumDec lValue, const MediumDec& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// Addition operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDec operator+(MediumDec lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumDec operator+(MediumDec lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDec operator+(MediumDec lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        friend MediumDec operator+(const signed int& lValue, MediumDec rValue)  { return rValue.IntAddOperation(lValue); }
        friend MediumDec operator+(const signed __int64& lValue, MediumDec rValue)  { return rValue.Int64AddOperation(lValue); }
        friend MediumDec operator+(const unsigned int& lValue, MediumDec rValue)  { return rValue.UIntAddOperation(lValue); }
        friend MediumDec operator+(const unsigned __int64& lValue, MediumDec rValue)  { return rValue.UInt64AddOperation(lValue); }

        friend MediumDec operator+(const signed char& lValue, MediumDec rValue)  { return rValue.Int8AddOperation(lValue); }
        friend MediumDec operator+(const signed short& lValue, MediumDec rValue)  { return rValue.Int16AddOperation(lValue); }
        friend MediumDec operator+(const unsigned char& lValue, MediumDec rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend MediumDec operator+(const unsigned short& lValue, MediumDec rValue)  { return rValue.UInt16AddOperation(lValue); }

    #pragma endregion Other addition operations

    #pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& lValue, const MediumDec& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// -= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec& operator-=(MediumDec& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumDec& operator-=(MediumDec& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDec& operator-=(MediumDec& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec lValue, const MediumDec& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDec operator-(MediumDec lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumDec operator-(MediumDec lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDec operator-(MediumDec lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        friend MediumDec operator-(const signed int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const signed __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned int& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned __int64& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }

        friend MediumDec operator-(const signed char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const signed short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned char& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }
        friend MediumDec operator-(const unsigned short& lValue, const MediumDec& rValue) { return ((MediumDec)lValue).SubtractBy(rValue); }

    #pragma endregion Other subtraction operations

    #pragma region Modulus Operations
    protected:

        template<MediumDecVariant VariantType=MediumDec>
        void UnsignedModulusOpV1(const VariantType& rValue)
        {
            if(DecimalHalf.Value==0&&rValue.DecimalHalf.Value==0)
                IntHalf.Value %= rValue.IntHalf.Value;
            else {
                auto divRes = DivideByUnsigned(rValue);
                UnsignedSubOp(divRes.MultiplyByUnsigned(rValue));
            }
        }

        template<MediumDecVariant VariantType=MediumDec>
        void ModulusOpV1(const VariantType& rValue)
        {
            if (rValue.IsNegative()) {
                SwapNegativeStatus();
                UnsignedModulusOp(-rValue);
            }
            else
                UnsignedModulusOp(rValue);
        }

    public:

        void UIntModulusOp(const unsigned int& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf.Value %= rValue;
            else {
                auto divRes = DivideByUIntV1(rValue);
                UnsignedSubOp(divRes.MultiplyByUInt(rValue));
            }
        }

        void IntModulusOp(const signed int& rValue)
        {
            if (rValue<0) {
                SwapNegativeStatus();
                UIntModulusOp(-rValue);
            }
            else
                UIntModulusOp(rValue);
        }

        void UInt64ModulusOp(const unsigned __int64& rValue)
        {
            if(DecimalHalf.Value==0){
                unsigned __int64 result = IntHalf.Value;
                result %= rValue;
                IntHalf.Value = (unsigned int) result;
            } else {
                auto divRes = DivideByUIntV1(rValue);
                UnsignedSubOp(divRes.MultiplyByUInt64(rValue));
            }
        }

        void Int64ModulusOp(const signed __int64& rValue)
        {
            if (rValue<0) {
                SwapNegativeStatus();
                UInt64ModulusOp(-rValue);
            }
            else
                UInt64ModulusOp(rValue);
        }

        void UnsignedModulusOp(const MediumDec& rValue){ UnsignedModulusOpV1(rValue); }

        void ModulusOp(const MediumDec& rValue){ ModulusOpV1(rValue); }

        MediumDec& UnsignedModulusOperation(const MediumDec& rValue){
            UnsignedModulusOp(rValue); return *this;
        }

        MediumDec& ModulusOperation(const MediumDec& rValue){
            ModulusOp(rValue); return *this;
        }

        friend MediumDec& operator%=(MediumDec& lValue, const MediumDec& rValue) { return lValue.ModulusOperation(rValue); }
        friend MediumDec& operator%=(MediumDec& lValue, const signed int& rValue) { lValue.IntModulusOp(rValue); return lValue; }
        friend MediumDec& operator%=(MediumDec& lValue, const unsigned int& rValue) { lValue.UIntModulusOp(rValue); return lValue; }
        friend MediumDec& operator%=(MediumDec& lValue, const unsigned __int64& rValue) { lValue.UInt64ModulusOp(rValue); return lValue;}
        friend MediumDec& operator%=(MediumDec& lValue, const signed __int64& rValue) { lValue.Int64ModulusOp(rValue); return lValue;}

    #pragma region Modulus Operations

    #pragma region Bitwise Operations

    //Left Shift
    MediumDec& operator<<=(const unsigned int shift) {
      auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
      combined <<= shift;
      // Re-split
      IntHalf.Value     = static_cast<IntHalfValueT>(combined / DecimalOverflow);
      DecimalHalf.Value = static_cast<DecimalHalfValueT>(combined % DecimalOverflow);
      if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
        IntHalf.Sign = PositiveSign;
      return *this;
    }

    //Right Shift
    MediumDec& operator>>=(const unsigned int shift) {
      auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
      combined >>= shift;
      // Re-split
      IntHalf.Value     = static_cast<IntHalfValueT>(combined / DecimalOverflow);
      DecimalHalf.Value = static_cast<DecimalHalfValueT>(combined % DecimalOverflow);
      if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
        IntHalf.Sign = PositiveSign;
      return *this;
    }

    //Left or Right Shift(with partial shifts between DecimalHalf to next IntHalf)
    template<bool ShiftLeft,           // true = left, false = right
        bool ReverseIfNegative = true>// false = flip sign, true = reverse direction
    MediumDec& ShiftCore(const MediumDec& rhs) {
        if (rhs.IsZero()) return *this;

        const bool rhsNeg  = !rhs.IsPositive();
        auto       whole   = rhs.IntHalf.Value;
        auto       fracVal = rhs.DecimalHalf.Value;

        bool effectiveLeft = ShiftLeft;

        if (rhsNeg) {
            if constexpr (ReverseIfNegative) {
                // Reverse initial direction if negative
                effectiveLeft = !effectiveLeft;
            } else {
                // Default policy: flip sign after magnitude change
                SwapNegativeStatus();
            }
        }

        // Whole‑part shift
        if (effectiveLeft) {
            *this <<= whole;
        } else {
            *this >>= whole;
        }

        // Fractional‑part shift (if any)
        if (fracVal != 0) {
            boost::rational<unsigned int> frac(fracVal, MediumDecVariant::DecimalOverflow);
            if (effectiveLeft) {
                *this *= MediumDec::Two.FractionalPowV1(frac);
            } else {
                *this /= MediumDec::Two.FractionalPowV1(frac);
            }
        }
        return *this;
    }

    //Left Shift(with partial shifts between DecimalHalf to next IntHalf)
    MediumDec& operator<<=(const MediumDec& rhs) {
      return ShiftCore<true>(rhs);
    }

    //Right Shift(with partial shifts between DecimalHalf to next IntHalf)
    MediumDec& operator>>=(const MediumDec& rhs) {
      return ShiftCore<false>(rhs);
    }

    friend MediumDec operator<<(MediumDec lValue, const MediumDec& rValue) { return lValue<<rValue; }
    friend MediumDec operator>>(MediumDec lValue, const MediumDec& rValue) { return lValue>>rValue; }

    //Bitwise XOR Operation
    MediumDec& operator^=(const unsigned int rhs) {
      auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
      combined ^= rhs;
      // Re-split
      IntHalf.Value     = static_cast<IntHalfValueT>(combined / DecimalOverflow);
      DecimalHalf.Value = static_cast<DecimalHalfValueT>(combined % DecimalOverflow);
      if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
        IntHalf.Sign = PositiveSign;
      return *this;
    }

    //Bitwise XOR(or NOR for negative) Operation(with partial XOR between DecimalHalf to next IntHalf)
    MediumDec& operator^=(const MediumDec& rhs) {
      if(rhs.DecimalHalf.Value==0){
        if(rhs.IsPositive())
          return *this^=rhs.IntHalf.Value;
        else{
          auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
          combined = ~(combined ^ rhs.IntHalf.Value);
          IntHalf.Value     = static_cast<IntHalfValueT>(combined / DecimalOverflow);
          DecimalHalf.Value = static_cast<DecimalHalfValueT>(combined % DecimalOverflow);
        }
      } else{
        auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
        auto startX = rhs.IntHalf.Value;
        auto endX = startX+1;
        if(rhs.IsPositive()){
          auto initialStart = combined^startX;
          auto upperVal = combined^endX;
        } else {
          auto initialStart = ~(combined ^ startX);
          auto upperVal = ~(combined ^ endX);
        }
      }
      if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
        IntHalf.Sign = PositiveSign;
      return *this;
    }

    //Bitwise OR Operation
    MediumDec& operator|=(const unsigned rhs) {
      auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
      combined |= rhs;
      // Re-split
      IntHalf.Value     = static_cast<IntHalfValueT>(combined / DecimalOverflow);
      DecimalHalf.Value = static_cast<DecimalHalfValueT>(combined % DecimalOverflow);
      if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
        IntHalf.Sign = PositiveSign;
      return *this;
    }
    
    //Bitwise OR Operation(with partial AND between DecimalHalf to next IntHalf)
    MediumDec& operator|=(const MediumDec& rhs) {
      if(rhs.DecimalHalf.Value==0){
        if(rhs.IsPositive())
          return *this|=rhs.IntHalf.Value;
        else
          return *this&=rhs.IntHalf.Value;
      } else{
        auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
        auto startX = rhs.IntHalf.Value;
        auto endX = startX+1;
        auto initialStart = rhs.IsPositive()? combined|startX : combined&startX;
        auto upperVal = rhs.IsPositive()? combined|endX : combined&endX;
        
        if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
          IntHalf.Sign = PositiveSign;
        return *this;
      }
    }
    
    //Bitwise AND Operation
    MediumDec& operator&=(const unsigned rhs) {
      auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
      combined &= rhs;
      // Re-split
      IntHalf.Value     = static_cast<IntHalfValueT>(combined / DecimalOverflow);
      DecimalHalf.Value = static_cast<DecimalHalfValueT>(combined % DecimalOverflow);
      if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
        IntHalf.Sign = PositiveSign;
      return *this;
    }
    
    //Bitwise AND Operation(with partial AND between DecimalHalf to next IntHalf)
    MediumDec& operator&=(const MediumDec& rhs) {
      if(rhs.DecimalHalf.Value==0){
        if(rhs.IsPositive())
          return *this&=rhs.IntHalf.Value;
        else
          return *this|=rhs.IntHalf.Value;
      } else{
        auto combined = static_cast<uint64_t>(IntHalf.Value) * DecimalOverflow + DecimalHalf.Value;
        auto startX = rhs.IntHalf.Value;
        auto endX = startX+1;
        auto initialStart = rhs.IsPositive()? combined&startX : combined|startX;
        auto upperVal = rhs.IsPositive()? combined&endX : combined|endX;
        
        if(IntHalf.Value == 0 && DecimalHalf.Value == 0 && IntHalf.Sign != PositiveSign)
          IntHalf.Sign = PositiveSign;
        return *this;
      }
    }

    #pragma endregion Bitwise Operations

    /*
    #pragma region Floating Operator Overrides

        friend MediumDec operator+(const MediumDec& self, const float& tValue) { return self + (MediumDec)tValue; }
        friend MediumDec operator-(const MediumDec& self, const float& tValue) { return self - (MediumDec)tValue; }
        friend MediumDec operator*(const MediumDec& self, const float& tValue) { return self * (MediumDec)tValue; }
        friend MediumDec operator/(const MediumDec& self, const float& tValue) { return self / (MediumDec)tValue; }

        friend MediumDec operator+(const float& tValue, const MediumDec& self) { return (MediumDec)tValue + self; }
        friend MediumDec operator-(const float& tValue, const MediumDec& self) { return (MediumDec)tValue - self; }
        friend MediumDec operator*(const float& tValue, const MediumDec& self) { return (MediumDec)tValue * self; }
        friend MediumDec operator/(const float& tValue, const MediumDec& self) { return (MediumDec)tValue / self; }

        friend MediumDec operator+(const MediumDec& self, const double& tValue) { return self + (MediumDec)tValue; }
        friend MediumDec operator-(const MediumDec& self, const double& tValue) { return self - (MediumDec)tValue; }
        friend MediumDec operator*(const MediumDec& self, const double& tValue) { return self * (MediumDec)tValue; }
        friend MediumDec operator/(const MediumDec& self, const double& tValue) { return self / (MediumDec)tValue; }

        friend MediumDec operator+(const MediumDec& self, const long double& tValue) { return self + (MediumDec)tValue; }
        friend MediumDec operator-(const MediumDec& self, const long double& tValue) { return self - (MediumDec)tValue; }
        friend MediumDec operator*(const MediumDec& self, const long double& tValue) { return self * (MediumDec)tValue; }
        friend MediumDec operator/(const MediumDec& self, const long double& tValue) { return self / (MediumDec)tValue; }

        friend MediumDec operator+(const long double& tValue, const MediumDec& self) { return (MediumDec)tValue + self; }
        friend MediumDec operator-(const long double& tValue, const MediumDec& self) { return (MediumDec)tValue - self; }
        friend MediumDec operator*(const long double& tValue, const MediumDec& self) { return (MediumDec)tValue * self; }
        friend MediumDec operator/(const long double& tValue, const MediumDec& self) { return (MediumDec)tValue / self; }

    #pragma endregion Floating Operator Overrides
    */


    #pragma region Other Operators

        /// <summary>
        /// ++MediumDec Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator ++()
        {
            if (DecimalHalf == 0)
                ++IntHalf;
            else if (IntHalf == MirroredInt::NegativeZero)
                IntHalf = MirroredInt::Zero;
            else
                ++IntHalf;
            return *this;
        }

        /// <summary>
        /// ++MediumDec Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator --()
        {
            if (DecimalHalf == 0)
                --IntHalf;
            else if (IntHalf == MirroredInt::Zero)
                IntHalf = MirroredInt::NegativeZero;
            else
                --IntHalf;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>MediumDec</returns>
        MediumDec operator ++(int)
        {
            MediumDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDec</returns>
        MediumDec operator --(int)
        {
            MediumDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDec &</returns>
        MediumDec& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

    #pragma region Truncation Functions
/*
        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        void ApplyAbs(){ IntHalf.ApplyAbs(); }

protected://Using templating as static function instead as fix for *this pointer not updating when used in other classes

        ///<summary>
        /// Returns a copy of the number converted into non-negative version
        ///</summary>
        template<MediumDecVariant VariantType = MediumDec>
        static VariantType AbsV1(VariantType tValue){
            tValue.ApplyAbs(); return tValue;
        }

public:

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        static MediumDec Abs(const MediumDec& tValue);

        /// <summary>
        /// Returns a copy of the number converted into non-negative version
        /// </summary>
        MediumDec AbsOf() const { return Abs(*this); }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="precision">precision level of digits to cut off</param>
        void ApplyFloorOf(const int& precision = 0);

protected:

        template<MediumDecVariant VariantType=MediumDec>
        static VariantType CeilV1(const VariantType& tValue)
        {
            if(tValue.IntHalf==MirroredInt::NegativeZero)
                return VariantType::One;
            else if (tValue.DecimalHalf != 0)
                return VariantType(tValue.IntHalf+1);
            else
                return tValue;
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="tValue">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        template<MediumDecVariant VariantType = MediumDec>
        static VariantType FloorV1(const VariantType& tValue, const int& precision = 0)
        {
            unsigned int decimalRes = tValue.DecimalHalf.Value;
            switch (precision)
            {
            case 8: decimalRes /= 10; decimalRes *= 10; break;
            case 7: decimalRes /= 100; decimalRes *= 100; break;
            case 6: decimalRes /= 1000; decimalRes *= 1000; break;
            case 5: decimalRes /= 10000; decimalRes *= 10000; break;
            case 4: decimalRes /= 100000; decimalRes *= 100000; break;
            case 3: decimalRes /= 1000000; decimalRes *= 1000000; break;
            case 2: decimalRes /= 10000000; decimalRes *= 10000000; break;
            case 1: decimalRes /= 100000000; decimalRes *= 100000000; break;
            default: decimalRes = 0; break;
            }
            if(decimalRes==0&&tValue.IntHalf==MirroredInt::NegativeZero)
                return VariantType();
            else
                return VariantType(tValue.IntHalf, PartialInt(decimalRes,tValue.DecimalHalf.Flags));
        }

        template<MediumDecVariant VariantType = MediumDec>
        const VariantType TruncOfV1() const
        {
            return VariantType(IntHalf == NegativeRep?0:IntHalf, 0);
        }

public:

        ///<summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        ///</summary>
        /// <param name="tValue">Variable to apply method on</param>
        static MediumDec Ceil(const MediumDec& tValue) { return CeilV1<MediumDec>(tValue); }

        ///<summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        ///</summary>
        /// <param name="tValue">Variable to apply method on</param>
        MediumDec CeilOf() const { return Ceil(*this); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to tValue (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        signed int FloorIntOf() const;

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to tValue (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        static signed int FloorInt(const MediumDec& tValue) { return tValue.FloorIntOf(); }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        int CeilIntOf() const;

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDec&</returns>
        static signed int CeilInt(const MediumDec& tValue) { return tValue.CeilIntOf(); }

        static MediumDec Trunc(const MediumDec& tValue);
*/
    #pragma endregion Truncation Functions

    #pragma region Trigonomic Functions
protected:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<MediumDecVariant VariantType=MediumDec>
        static VariantType SinV1(const VariantType& tValue)
        {
            VariantType SinValue = VariantType::One  / VariableConversionFunctions::Fact(1);
            unsigned int expTotal;
            bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
                expTotal = 2 * i + 1;
                if(AddToResult)
                    SinValue += VariantType::UIntPow(tValue, expTotal) / VariableConversionFunctions::Fact(expTotal);
                else
                    SinValue -= VariantType::UIntPow(tValue, expTotal) / VariableConversionFunctions::Fact(expTotal);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Cos from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<MediumDecVariant VariantType=MediumDec>
        static VariantType CosV1(const VariantType& tValue)
        {
            VariantType CosValue = VariantType::One / VariableConversionFunctions::Fact(0);
            unsigned int expTotal;
            bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
                expTotal = 2 * i;
                if(AddToResult)
                    CosValue += VariantType::UIntPow(tValue, expTotal) / VariableConversionFunctions::Fact(expTotal);
                else
                    CosValue -= VariantType::UIntPow(tValue, expTotal) / VariableConversionFunctions::Fact(expTotal);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<MediumDecVariant VariantType=MediumDec>
        static VariantType TanV1(const VariantType& tValue)
        {
            VariantType SinValue = VariantType::One  / VariableConversionFunctions::Fact(1);
            VariantType CosValue = VariantType::One / VariableConversionFunctions::Fact(0);
            unsigned int sinExp; unsigned int cosExp;
            bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
                sinExp = 2 * i + 1; cosExp = 2 * i;
                if(AddToResult){
                    SinValue += VariantType::UIntPow(tValue, sinExp) / VariableConversionFunctions::Fact(sinExp);
                    CosValue += VariantType::UIntPow(tValue, cosExp) / VariableConversionFunctions::Fact(cosExp);
                } else{
                    SinValue -= VariantType::UIntPow(tValue, sinExp) / VariableConversionFunctions::Fact(sinExp);
                    CosValue -= VariantType::UIntPow(tValue, cosExp) / VariableConversionFunctions::Fact(cosExp);
                }
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Gets Inverse Tangent from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<MediumDecVariant VariantType=MediumDec>
        static VariantType ATanV1(const VariantType& tValue)
        {
            VariantType SinValue = VariantType::One  / VariableConversionFunctions::Fact(1);
            VariantType CosValue = VariantType::One / VariableConversionFunctions::Fact(0);
            //Angle as Radian
            unsigned int sinExp; unsigned int cosExp;
            bool AddToResult = false;
            for (unsigned int i = 1; i < 7; ++i&&AddToResult==!AddToResult)
            {
                sinExp = 2 * i + 1; cosExp = 2 * i;
                if(AddToResult){
                    SinValue += VariantType::UIntPow(tValue, sinExp) / VariableConversionFunctions::Fact(sinExp);
                    CosValue += VariantType::UIntPow(tValue, cosExp) / VariableConversionFunctions::Fact(cosExp);
                } else{
                    SinValue -= VariantType::UIntPow(tValue, sinExp) / VariableConversionFunctions::Fact(sinExp);
                    CosValue -= VariantType::UIntPow(tValue, cosExp) / VariableConversionFunctions::Fact(cosExp);
                }
            }
            return CosValue / SinValue;
        }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        template<MediumDecVariant VariantType=MediumDec>
        static VariantType ArcTan2V1(const VariantType& y, const VariantType& x)
        {
            VariantType coeff_1 = VariantType::PiNum.DividedByFour();
            VariantType coeff_2 = coeff_1.MultiplyByUInt(3);
            VariantType abs_y = VariantType::Abs(y) + VariantType::JustAboveZero;// kludge to prevent 0/0 condition
            VariantType r;
            VariantType angle;
            if (x.IsPositive())
            {
                r = (x - abs_y) / (x + abs_y);
                angle = coeff_1 - coeff_1 * r;
            }
            else
            {
                r = (x + abs_y) / (abs_y - x);
                angle = coeff_2 - coeff_1 * r;
            }
            if (y.IsNegative())
                return -angle;// negate if in quad III or IV
            else
                return angle;
        }

        template<MediumDecVariant VariantType=MediumDec>
        static VariantType NormalizeForTrig(VariantType tValue)
        {
            if (tValue.IsNegative())
            {
                if (tValue.IntHalf.Value == 0)
                {
                    tValue.IntHalf = 359; tValue.DecimalHalf = DecimalOverflow - tValue.DecimalHalf;
                }
                else
                {
                    tValue.SwapNegativeStatus();
                    tValue.IntHalf.Value %= 360;
                    tValue.IntHalf.Value = 360 - tValue.IntHalf.Value;
                    if (tValue.DecimalHalf != 0) { tValue.DecimalHalf = DecimalOverflow - tValue.DecimalHalf; }
                }
            }
            else
            {
                tValue.IntHalf.Value %= 360;
            }
            return tValue;
        }

        /// <summary>
        /// Get Sin from tValue of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<MediumDecVariant VariantType=MediumDec>
        static VariantType SinFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return VariantType::Zero;
                        break;
                    case 90://0.5 Pi Radians
                        return VariantType::One;
                        break;
                    case 270://1.5 Pi Radians
                        return VariantType::NegativeOne;
                        break;
                    case 30://0.1666666666 Pi Radians
                    case 150://0.833333333 Pi Radians
                        return VariantType::PointFive;
                    case 210:
                    case 330:
                        return VariantType::NegativePointFive;
                    default:
                        //Angle as Radian
                        VariantType Radius = PiNum * lValue / 180;
                        return MediumDec::SinV1<VariantType>(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = PiNum * lValue / 180;
                return MediumDec::SinV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from tValue of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<MediumDecVariant VariantType=MediumDec>
        static VariantType CosFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf.Value)
                {
                    case 0:
                        return VariantType::One;
                        break;
                    case 60:
                        return VariantType::PointFive;
                        break;
                    case 90://0.5 Pi Radians
                    case 270://1.5 Pi Radians
                        return VariantType::Zero;
                        break;
                    case 180://Pi Radians
                        return VariantType::NegativeOne;
                        break;
                    case 120:
                    case 240:
                        return VariantType::NegativePointFive;
                    default:
                        //Angle as Radian
                        VariantType Radius = PiNum * lValue / 180;
                        return MediumDec::CosV1<VariantType>(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = PiNum * lValue / 180;
                return MediumDec::CosV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from tValue in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<MediumDecVariant VariantType=MediumDec>
        static VariantType TanFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return VariantType::Zero;
                        break;
                    case 90://0.5 Pi Radians
                        return VariantType::Maximum;//Positive Infinity
                        break;
                    case 270://1.5 Pi Radians
                        return VariantType::Minimum;//Negative Infinity
                        break;
                    default:
                        return MediumDec::TanV1<VariantType>(PiNum * lValue / 180);
                        break;
                }
            }
            else
                return MediumDec::TanV1<VariantType>(PiNum * lValue / 180);
        }

public:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        static MediumDec Sin(const MediumDec& tValue)
        { return SinV1<MediumDec>(tValue); }

        /// <summary>
        /// Get cosine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumDec Cos(const MediumDec& tValue)
        { return CosV1<MediumDec>(tValue); }

        /// <summary>
        /// Get Tan from value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumDec Tan(const MediumDec& tValue)
        { return TanV1<MediumDec>(tValue); }

        /// <summary>
        /// Gets Inverse Tangent from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumDec ATan(const MediumDec& tValue)
        { return ATanV1<MediumDec>(tValue); }

        /// <summary>
        /// Get Sin from value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumDec SinFromAngle(const MediumDec& tValue)
        { return SinFromAngleV1<MediumDec>(tValue); }

        /// <summary>
        /// Get Cos() from value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumDec CosFromAngle(const MediumDec& tValue)
        { return CosFromAngleV1<MediumDec>(tValue); }

        /// <summary>
        /// Get Tangent from value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumDec TanFromAngle(const MediumDec& tValue)
        { return TanFromAngleV1<MediumDec>(tValue); }

        MediumDec SinOf()
        { return Sin(*this); }

        MediumDec CosOf()
        { return Cos(*this); }

        MediumDec TanOf()
        { return Tan(*this); }

        MediumDec ATanOf()
        { return ATan(*this);; }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        static MediumDec ArcTan2(const MediumDec& y, const MediumDec& x)
        { return ArcTan2V1<MediumDec>(y, x); }

    #pragma endregion Trigonomic Functions
    };

}


