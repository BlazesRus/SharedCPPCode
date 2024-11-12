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

#include "..\AlternativeInt\PartialInt.hpp"

#include "..\Concepts\MediumDecVariant.hpp"

using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumUDec;

    /// <summary>
    /// Unsigned version of MediumDec
    /// Represents 4294967295.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// (8 bytes worth of Variable Storage inside class for each instance)
    /// </summary>
    class DLL_API MediumUDec : public UnsignedAltNumBase//UnsignedAltNumBase is used as base class to identify as MediumUDec variant
    {
    public:
    #pragma region DigitStorage

        /// <summary>
        /// Stores whole half of number
        /// </summary>
        unsigned int IntHalf;

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        PartialInt DecimalHalf;

        //Return IntHalf as signed int
        inline unsigned int GetIntHalf() const
        {
            return IntHalf;
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
        static unsigned int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned _int64 const DecimalOverflowX = 1000000000;

    #pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumUDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumUDec(const unsigned int& intVal=0, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        MediumUDec& operator=(const MediumUDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumUDec& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<MediumUDecVariant VariantType>
        MediumUDec operator[](VariantType variantValue) const
        {
            MediumUDec newSelf = MediumUDec(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

        //Fix for C2440 error during static template class
        static MediumUDec Initialize(const unsigned int& intVal, const PartialInt& decVal = PartialInt::Zero)
        {
            return MediumUDec(intVal,decVal);
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="tValue">The value.</param>
        void SetValue(const MediumUDec& rValue)
        {
            IntHalf = rValue.IntHalf;
            DecimalHalf.SetValueV2(rValue.DecimalHalf);
        }

    #pragma endregion class_constructors

    #pragma region Check_if_value

        //Set value as exactly zero
        void SetAsZero();

        //Set value as exactly one
        void SetAsOne();

        //Set as +-1 while keeping current sign
        void SetAsOneVal();

        void SetAsValues(const unsigned int& intVal = 0, const PartialInt& decVal = PartialInt::Zero);

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt() const;

        bool IsNotAtZeroInt() const;

        bool IsAtOneInt() const;

        bool IsNotAtOneInt() const;

        //Detect if at exactly zero(only overridden with MixedDec)
        bool IsZero() const;

        bool IsOne() const;

        bool IsOneVal() const;

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
        /// <returns>MediumUDec</returns>
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

        static MediumUDec AlmostOneValue();

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumUDec</returns>
        static MediumUDec PiNumValue();

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumUDec HundredMilPiNumValue();

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumUDec TenMilPiNumValue();

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumUDec OneMilPiNumValue();

        //10xPi(Rounded to 9th decimal digit)
        static MediumUDec TenPiNumValue();

        static MediumUDec ENumValue();

        static MediumUDec ZeroValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumUDec</returns>
        static MediumUDec OneValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumUDec</returns>
        static MediumUDec TwoValue();

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumUDec</returns>
        static MediumUDec Point5Value();

        static MediumUDec JustAboveZeroValue();

        static MediumUDec OneMillionthValue();

        static MediumUDec FiveThousandthValue();

        static MediumUDec FiveMillionthValue();

        static MediumUDec TenMillionthValue();

        static MediumUDec OneHundredMillionthValue();

        static MediumUDec FiveBillionthValue();

        static MediumUDec LN10Value();

        static MediumUDec LN10DivValue();

        static MediumUDec TwiceLN10DivValue();

        static MediumUDec MinimumValue();

        static MediumUDec MaximumValue();

        static MediumUDec PointOneValue();

        static const MediumUDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec PiNum;

        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec ENum;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec Pi;

        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec Zero;

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumUDec</returns>
        static const MediumUDec OneGMillionth;

        //0e-7
        static const MediumUDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static const MediumUDec FiveMillionth;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State tValue that can store
        /// (-2147483647.999999999)
        /// </summary>
        static const MediumUDec Minimum;

        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State tValue that can store
        /// (2147483647.999999999)
        /// </summary>
        static const MediumUDec Maximum;

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static const MediumUDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static const MediumUDec LN10Div;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static const MediumUDec TwiceLN10Div;

        static const MediumUDec PointOne;

    #pragma endregion ValueDefines

    #pragma region String Commands

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="tValue">The value.</param>
        void ReadString(const std::string& Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumUDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumUDec(const char* strVal);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumUDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumUDec(const std::string& Value);

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
        /// Initializes a new instance of the <see cref="MediumUDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumUDec(const float& tValue){ this->SetFloatVal(tValue); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumUDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumUDec(const double& tValue){ this->SetDoubleVal(tValue); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumUDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumUDec(const long double& tValue){ this->SetDecimalVal(tValue); }

    #endif

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #if defined(AltNum_EnableFloatingConversion)

        /// <summary>
        /// MediumUDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        float toFloat() const;

        /// <summary>
        /// MediumUDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        double toDouble() const;

        /// <summary>
        /// MediumUDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        long double toDecimal() const;

    #endif

        /// <summary>
        /// MediumUDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        signed int toInt() const { return IntHalf; }

        /// <summary>
        /// MediumUDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        unsigned int toUInt() const { return IntHalf; }

        bool toBool() const { return IntHalf==0 ? false : true; }

    #if defined(AltNum_EnableFloatingConversion)

        /// <summary>
        /// MediumUDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }

        /// <summary>
        /// MediumUDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDouble(); }

        /// <summary>
        /// MediumUDec Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator long double() { return toDecimal(); }

    #endif

        /// <summary>
        /// MediumUDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return toInt(); }

        /// <summary>
        /// MediumUDec Variant to uint explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return toUInt(); }

        /// <summary>
        /// MediumUDec Variant to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed __int64() { return toInt(); }

        /// <summary>
        /// MediumUDec Variant to ubyte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned char() { return toUInt(); }

        /// <summary>
        /// MediumUDec Variant to byte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed char() { return toInt(); }

        /// <summary>
        /// MediumUDec Variant to ushort explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned short() { return toUInt(); }

        /// <summary>
        /// MediumUDec Variant to short explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed short() { return toInt(); }

        /// <summary>
        /// MediumUDec Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
protected:
        //Compare only as if in NormalType representation mode
        template<MediumUDecVariant VariantType=MediumUDec>
        std::strong_ordering BasicComparisonV1(const VariantType& that) const
        {
            if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
                return IntHalfCmp;
            if (auto DecimalHalfCmp = DecimalHalf.Value <=> that.DecimalHalf.Value; DecimalHalfCmp != 0)
                return DecimalHalfCmp;
        }

        std::strong_ordering BasicComparison(const MediumUDec& that) const
        {
            return BasicComparisonV1(that);
        }

        //Compare only as if in NormalType representation mode
        std::strong_ordering BasicUIntComparison(const unsigned int& that) const
        {
            if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
                return IntHalfCmp;
            if (auto DecimalHalfCmp = DecimalHalf.Value <=> 0; DecimalHalfCmp != 0)
                return DecimalHalfCmp;
        }

        //Compare only as if in NormalType representation mode
        std::strong_ordering BasicIntComparison(const signed int& that) const
        {
            if (that < 0) {
                auto IntHalfCmp = 0 <=> 1;
                return IntHalfCmp;
            }
            else
            {
                if (auto IntHalfCmp = IntHalf <=> (unsigned int)that; IntHalfCmp != 0)
                    return IntHalfCmp;
                if (auto DecimalHalfCmp = DecimalHalf.Value <=> 0; DecimalHalfCmp != 0)
                    return DecimalHalfCmp;
            }
        }

public:

        std::strong_ordering operator<=>(const MediumUDec& that) const
        {//return BasicComparison(that);
            if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
                return IntHalfCmp;
            if (auto DecimalHalfCmp = DecimalHalf.Value <=> that.DecimalHalf.Value; DecimalHalfCmp != 0)
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

        bool operator==(const MediumUDec& that) const
        {
            if (IntHalf!=that.IntHalf)
                return false;
            if (DecimalHalf!=that.DecimalHalf)
                return false;
            return true;
        }

        bool operator!=(const MediumUDec& that) const
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
        void PartialIntDivOpV1(const IntType& rValue)
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

public:
        void PartialIntDivOp(const unsigned int& rValue) { PartialIntDivOpV1(rValue); }
        void PartialUInt64DivOp(const unsigned int& rValue) { PartialIntDivOpV1(rValue); }

        void PartialIntDivOp(const signed int& rValue) { PartialIntDivOpV1(rValue); }
        void PartialInt64DivOp(const signed __int64& rValue) { PartialIntDivOpV1(rValue); }

protected:

        /// <summary>
        /// Basic division operation between MediumUDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
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
        auto& IntDivOperationV1(const IntType& rValue)
        { IntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Basic division operation between MediumUDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        const auto DivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntDivOperationV1(rValue); }

public:

        void UIntDivOp(const unsigned int& rValue) { IntDivOpV1(rValue); }
        void IntDivOp(const signed int& rValue) { IntDivOpV1(rValue); }
        void UInt64DivOp(const unsigned __int64& rValue) { IntDivOpV1(rValue); }
        void Int64DivOp(const signed __int64& rValue) { IntDivOpV1(rValue); }

        void UInt8DivOp(const unsigned char& rValue) { IntDivOpV1(rValue); }
        void Int8DivOp(const signed char& rValue) { IntDivOpV1(rValue); }
        void UInt16DivOp(const unsigned short& rValue) { IntDivOpV1(rValue); }
        void Int16DivOp(const signed short& rValue) { IntDivOpV1(rValue); }

        MediumUDec& UIntDivOperation(const unsigned int& rValue) { return IntDivOperationV1(rValue); }
        MediumUDec& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
        MediumUDec& UInt64DivOperation(const unsigned __int64& rValue) { return IntDivOperationV1(rValue); }
        MediumUDec& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
        MediumUDec& UInt8DivOperation(const unsigned char& rValue) { return IntDivOperationV1(rValue); }
        MediumUDec& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
        MediumUDec& UInt16DivOperation(const unsigned short& rValue) { return IntDivOperationV1(rValue); }
        MediumUDec& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

        const MediumUDec DivideByUInt(const unsigned int& rValue) { return DivideByIntV1(rValue); }
        const MediumUDec DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        const MediumUDec DivideByUInt64(const unsigned __int64& rValue) { return DivideByIntV1(rValue); }
        const MediumUDec DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        const MediumUDec DivideByUInt8(const unsigned char& rValue) { return DivideByIntV1(rValue); }
        const MediumUDec DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
        const MediumUDec DivideByUInt16(const unsigned short& rValue) { return DivideByIntV1(rValue); }
        const MediumUDec DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep AltNum Division Operations
protected:

        /// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        template<MediumUDecVariant VariantType=MediumUDec>
        bool PartialDivOpV1(const VariantType& rValue)
        {
            unsigned _int64 SelfRes = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
            unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf.Value;

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
        template<MediumUDecVariant VariantType=MediumUDec>
        void DivOpV1(const VariantType& rValue)
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
                else if (PartialDivOp(rValue))//Prevent Dividing into nothing
                        DecimalHalf.Value = 1;
#else
                else
                    PartialDivOp(rValue);
#endif
            }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
            else if (PartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf.Value = 1;
#else
            else
                PartialDivOp(rValue);
#endif
        }

        template<MediumUDecVariant VariantType = MediumUDec>
        static VariantType DivisionV1(VariantType lValue, const VariantType& rValue)
        {
            lValue.DivOpV1(rValue); return lValue;
        }

public:

        /// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool PartialDivOp(const MediumUDec& rValue){ return PartialDivOpV1(rValue); }

        /// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void DivOp(const MediumUDec& rValue){ DivOpV1(rValue); }

        /// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec& DivOperation(const MediumUDec& rValue)
        { DivOp(rValue); return *this; }

        /// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec DivideBy(const MediumUDec& rValue) const
        { MediumUDec lValue = *this; return lValue.DivOperation(rValue); }

    #pragma endregion NormalRep AltNum Division Operations

    #pragma region Other Division Operations

        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumUDec&</returns>
        void DivideByTwo();

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumUDec&</returns>
        void DivideByFour();

        //Return copy of result divided by two
        MediumUDec DividedByTwo() const;

        //Return copy of result divided by four
        MediumUDec DividedByFour() const;

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec& operator/=(MediumUDec& lValue, const MediumUDec& rValue) { return lValue.DivOperation(rValue); }

        /// <summary>
        /// *= operation between MediumUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec& operator/=(MediumUDec& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumUDec& operator/=(MediumUDec& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumUDec& operator/=(MediumUDec& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumUDec& operator/=(MediumUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumUDec& operator/=(MediumUDec& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumUDec& operator/=(MediumUDec& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumUDec& operator/=(MediumUDec& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumUDec& operator/=(MediumUDec& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec operator/(MediumUDec lValue, const MediumUDec& rValue) { return lValue.DivideBy(rValue); }

        /// <summary>
        /// Division operation between MediumUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec operator/(MediumUDec lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumUDec operator/(MediumUDec lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumUDec operator/(MediumUDec lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumUDec operator/(MediumUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumUDec operator/(MediumUDec lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumUDec operator/(MediumUDec lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumUDec operator/(MediumUDec lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumUDec operator/(MediumUDec lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        friend MediumUDec operator/(const signed int& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).DivideBy(rValue); }
        friend MediumUDec operator/(const signed __int64& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).DivideBy(rValue); }
        friend MediumUDec operator/(const unsigned int& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).DivideBy(rValue); }
        friend MediumUDec operator/(const unsigned __int64& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).DivideBy(rValue); }

        friend MediumUDec operator/(const signed char& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).DivideBy(rValue); }
        friend MediumUDec operator/(const signed short& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).DivideBy(rValue); }
        friend MediumUDec operator/(const unsigned char& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).DivideBy(rValue); }
        friend MediumUDec operator/(const unsigned short& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).DivideBy(rValue); }

    #pragma endregion Other Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
        /// <summary>
        /// Partial version of UIntMultOpV1 without zero checks
        /// (Modifies owner object)
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumUDec</returns>
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
        /// Basic multiplication operation between MediumUDec variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        const MediumUDec MultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntMultOperationV1(rValue); }

        /// <summary>
        /// Basic multiplication operation between MediumUDec variant and Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        const MediumUDec MultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntMultOperationV1(rValue); }

        template<MediumUDecVariant VariantType=MediumUDec, IntegerType IntType=unsigned int>
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

        MediumUDec& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
        MediumUDec& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
        MediumUDec& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
        MediumUDec& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
        MediumUDec& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
        MediumUDec& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
        MediumUDec& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
        MediumUDec& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

        const MediumUDec MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumUDec MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
        const MediumUDec MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumUDec MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

        const MediumUDec UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumUDec UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

        const MediumUDec MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumUDec MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
        const MediumUDec MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumUDec MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep AltNum Multiplication Operations
protected:

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumUDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        template<MediumUDecVariant VariantType=MediumUDec>
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

        template<MediumUDecVariant VariantType=MediumUDec>
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

        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType UnsignedMultiplicationV1(VariantType lValue, const VariantType& rValue)
        {
            lValue.UnsignedMultOpV1(rValue); return lValue;
        }

public:

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumUDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void UnsignedMultOp(const MediumUDec& rValue){ UnsignedMultOpV1(rValue); }

        void MultOp(const MediumUDec& rValue){ MultOpV1(rValue); }

        /// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec& UnsignedMultOperation(const MediumUDec& rValue)
        { UnsignedMultOp(rValue); return *this; }

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec& MultOperation(const MediumUDec& rValue)
        { MultOp(rValue); return *this; }

        /// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec MultiplyByUnsigned(const MediumUDec& rValue) const
        { MediumUDec lValue = *this; return lValue.UnsignedMultOperation(rValue); }

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec MultiplyBy(const MediumUDec& rValue) const
        { MediumUDec lValue = *this; return lValue.MultOperation(rValue); }

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
        MediumUDec MultipliedByTwo() const;

        //Return copy of result divided by four
        MediumUDec MultipliedByFour() const;

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec& operator*=(MediumUDec& lValue, const MediumUDec& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// *= operation between MediumUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumUDec& operator*=(MediumUDec& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumUDec& operator*=(MediumUDec& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumUDec& operator*=(MediumUDec& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumUDec& operator*=(MediumUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumUDec& operator*=(MediumUDec& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumUDec& operator*=(MediumUDec& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumUDec& operator*=(MediumUDec& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumUDec& operator*=(MediumUDec& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec operator*(MediumUDec lValue, const MediumUDec& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation between MediumUDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumUDec operator*(MediumUDec lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumUDec operator*(MediumUDec lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumUDec operator*(MediumUDec lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumUDec operator*(MediumUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumUDec operator*(MediumUDec lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumUDec operator*(MediumUDec lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumUDec operator*(MediumUDec lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumUDec operator*(MediumUDec lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        friend MediumUDec operator*(signed int lValue, MediumUDec rValue)  { return rValue.IntMultOperation(lValue); }
        friend MediumUDec operator*(signed __int64 lValue, MediumUDec& rValue)  { return rValue.Int64MultOperation(lValue); }
        friend MediumUDec operator*(unsigned int lValue, MediumUDec& rValue)  { return rValue.UIntMultOperation(lValue); }
        friend MediumUDec operator*(unsigned __int64 lValue, MediumUDec& rValue)  { return rValue.UInt64MultOperation(lValue); }

        friend MediumUDec operator*(signed char lValue, MediumUDec& rValue)  { return rValue.Int8MultOperation(lValue); }
        friend MediumUDec operator*(signed short lValue, MediumUDec& rValue)  { return rValue.Int16MultOperation(lValue); }
        friend MediumUDec operator*(unsigned char lValue, MediumUDec& rValue)  { return rValue.UInt8MultOperation(lValue); }
        friend MediumUDec operator*(unsigned short lValue, MediumUDec& rValue)  { return rValue.UInt16MultOperation(lValue); }

    #pragma endregion Other multiplication operations

    #pragma region NormalRep Integer Addition Operations
protected:

        /// <summary>
        /// Basic addition operation between variant and unsigned Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void UIntAddOpV1(const IntType& rValue)
        {
            IntHalf += rValue;
        }

        /// <summary>
        /// Basic addition operation between variant and Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void IntAddOpV1(const IntType& rValue)
        {
            if (rValue < 0) {
                if ((unsigned int)(-rValue) > IntHalf)
                    throw("Underflow error into non-supported negative number.");
                IntHalf += rValue;
            }
            else
                IntHalf += rValue;
        }

        template<IntegerType IntType=unsigned int>
        auto& UIntAddOperationV1(const IntType& rValue)
        { UIntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& IntAddOperationV1(const IntType& rValue)
        { IntAddOpV1(rValue); return *this; }

        /// <summary>
        ///  addition operation between variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto AddByUIntV1(const IntType& rValue) const
        { auto self = *this; return self.UIntAddOperationV1(rValue); }

        /// <summary>
        /// Basic addition operation between variant and Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto AddByIntV1(const IntType& rValue) const
        { auto self = *this; return self.IntAddOperationV1(rValue); }

public:

        void UInt8AddOp(const unsigned char& rValue) { UIntAddOpV1(rValue); }
        void UInt16AddOp(const unsigned short& rValue) { UIntAddOpV1(rValue); }
        void UIntAddOp(const unsigned int& rValue);
        void UInt64AddOp(const unsigned __int64& rValue) { UIntAddOpV1(rValue); }

        void UnsignedIntegerAddOp(const signed int& rValue) { UIntAddOpV1(rValue); }

        MediumUDec& UInt8AddOperation(const unsigned char& rValue) { return UIntAddOperationV1(rValue); }
        MediumUDec& UInt16AddOperation(const unsigned short& rValue) { return UIntAddOperationV1(rValue); }
        MediumUDec& UIntAddOperation(const unsigned int& rValue);
        MediumUDec& UInt64AddOperation(const unsigned __int64& rValue) { return UIntAddOperationV1(rValue); }

        MediumUDec UnsignedAddByInt(const signed int& rValue) { return AddByUIntV1(rValue); }
        MediumUDec UnsignedAddByInt64(const signed __int64& rValue) { return AddByUIntV1(rValue); }

        MediumUDec AddByUInt8(const unsigned char& rValue) { return AddByUIntV1(rValue); }
        MediumUDec AddByUInt16(const unsigned short& rValue) { return AddByUIntV1(rValue); }
        MediumUDec AddByUInt(const unsigned int& rValue) { return AddByUIntV1(rValue); }
        MediumUDec AddByUInt64(const unsigned __int64& rValue) { return AddByUIntV1(rValue); }

        void Int8AddOp(const signed char& rValue) { IntAddOpV1(rValue); }
        void Int16AddOp(const signed short& rValue) { IntAddOpV1(rValue); }
        void IntAddOp(const signed int& rValue) { IntAddOpV1(rValue); }
        void Int64AddOp(const signed __int64& rValue) { IntAddOpV1(rValue); }

        MediumUDec& Int8AddOperation(const signed char& rValue) { return IntAddOperationV1(rValue); }
        MediumUDec& Int16AddOperation(const signed short& rValue) { return IntAddOperationV1(rValue); }
        MediumUDec& IntAddOperation(const signed int& rValue) { return IntAddOperationV1(rValue); }
        MediumUDec& Int64AddOperation(const signed __int64& rValue) { return IntAddOperationV1(rValue); }

        MediumUDec AddByInt8(const signed char rValue) { return AddByIntV1(rValue); }
        MediumUDec AddByInt16(const signed short& rValue) { return AddByIntV1(rValue); }
        MediumUDec AddByInt(const signed int& rValue) { return AddByIntV1(rValue); }
        MediumUDec AddByInt64(const signed __int64& rValue) { return AddByIntV1(rValue); }

    #pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations
protected:

        /// <summary>
        /// Basic Subtraction operation between variant and unsigned Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void UIntSubOpV1(const IntType& rValue)
        {
            if(rValue>IntHalf)
                throw("Underflow error into non-existant negative number.");
            IntHalf -= rValue;
        }

        /// <summary>
        /// Basic Subtraction operation between variant and Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=int>
        void IntSubOpV1(const IntType& rValue)
        {
            if(rValue>(signed IntType)IntHalf)
                throw("Underflow error into non-existant negative number.");
            IntHalf -= rValue;
        }

        template<IntegerType IntType=unsigned int>
        auto& IntSubOperationV1(const IntType& rValue)
        { IntSubOpV1(rValue); return *this; }

        /// <summary>
        /// Basic Subtraction operation between MediumUDec variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto SubtractByUIntV1(const IntType& rValue)
        { auto self = *this; return self.IntSubOperationV1(rValue); }

        /// <summary>
        /// Basic Subtraction operation between MediumUDec variant and Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto SubtractByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntSubOperationV1(rValue); }


public:

        void UInt8SubOp(const unsigned char& rValue) { UIntSubOpV1(rValue); }
        /// <summary>
        /// Basic Subtraction operation between MediumUDec Variant and unsigned Integer value
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

        MediumUDec& UInt8SubOperation(const unsigned char& rValue) { return UIntSubOperationV1(rValue); }
        MediumUDec& UInt16SubOperation(const unsigned short& rValue) { return UIntSubOperationV1(rValue); }
        MediumUDec& UIntSubOperation(const unsigned int& rValue);
        MediumUDec& UInt64SubOperation(const unsigned __int64& rValue) { return UIntSubOperationV1(rValue); }

        MediumUDec UnsignedSubtractByInt(const signed int& rValue) { return SubtractByUIntV1(rValue); }
        MediumUDec UnsignedSubtractByInt64(const signed __int64& rValue) { return SubtractByUIntV1(rValue); }

        MediumUDec SubtractByUInt8(const unsigned char& rValue) { return SubtractByUIntV1(rValue); }
        MediumUDec SubtractByUInt16(const unsigned short& rValue) { return SubtractByUIntV1(rValue); }
        MediumUDec SubtractByUInt(const unsigned int& rValue) { return SubtractByUIntV1(rValue); }
        MediumUDec SubtractByUInt64(const unsigned __int64& rValue) { return SubtractByUIntV1(rValue); }

        MediumUDec& Int8SubOperation(const signed char& rValue) { return IntSubOperationV1(rValue); }
        MediumUDec& Int16SubOperation(const signed short& rValue) { return IntSubOperationV1(rValue); }
        MediumUDec& IntSubOperation(const signed int& rValue) { return IntSubOperationV1(rValue); }
        MediumUDec& Int64SubOperation(const signed __int64& rValue) { return IntSubOperationV1(rValue); }

        MediumUDec SubtractByInt8(const signed char rValue) { return SubtractByIntV1(rValue); }
        MediumUDec SubtractByInt16(const signed short& rValue) { return SubtractByIntV1(rValue); }
        MediumUDec SubtractByInt(const signed int& rValue) { return SubtractByIntV1(rValue); }
        MediumUDec SubtractByInt64(const signed __int64& rValue) { return SubtractByIntV1(rValue); }

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep AltNum Addition/Subtraction Operations
protected:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumUDecVariant VariantType=MediumUDec>
        void AddOpV1(const VariantType& rValue)
        {
            if(rValue.DecimalHalf==0)
                UnsignedMirroredAddOp(rValue.IntHalf);
            else
            {
                int signBeforeOp = IntHalf.Sign;
                IntHalf += rValue.IntHalf;
                if (signBeforeOp==unsigned int::NegativeSign)
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
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumUDecVariant VariantType=MediumUDec>
        void SubOpV1(const VariantType& rValue)
        {
            if (rValue.DecimalHalf == 0)
                UnsignedMirroredSubOp(rValue.IntHalf);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf -= rValue.IntHalf;
                if (signBeforeOp==unsigned int::NegativeSign){//-5 - B
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

        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType UnsignedAdditionV1(VariantType lValue, const VariantType& rValue)
        {
            lValue.AddOpV1(rValue); return lValue;
        }

        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType SubtractionV1(VariantType lValue, const VariantType& rValue)
        {
            lValue.SubOpV1(rValue); return lValue;
        }

public:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedAddOp(const MediumUDec& rValue){ AddOpV1(rValue); }

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOp(const MediumUDec& rValue){ AddOpV1(rValue); }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const MediumUDec& rValue){ SubOpV1(rValue); }

        //Basic addition operation
        MediumUDec& UnsignedAddOperation(const MediumUDec& rValue)
        { UnsignedAddOp(rValue); return *this; }

        //Basic addition operation
        MediumUDec& AddOperation(const MediumUDec& rValue)
        { AddOp(rValue); return *this; }

        /// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec AddByUnsigned(const MediumUDec& rValue)
        { MediumUDec lValue = *this; return lValue.UnsignedAddOperation(rValue); } const

        /// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec AddBy(const MediumUDec& rValue)
        { MediumUDec lValue = *this; return lValue.AddOperation(rValue); } const

        //Basic subtraction operation
        MediumUDec& UnsignedSubOperation(const MediumUDec& rValue)
        { SubOp(rValue); return *this; }

        //Basic subtraction operation
        MediumUDec& SubOperation(const MediumUDec& rValue)
        { SubOp(rValue); return *this; }

        /// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec SubtractByUnsigned(const MediumUDec& rValue)
        { MediumUDec lValue = *this; return lValue.UnsignedSubOperation(rValue); } const

        /// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumUDec SubtractBy(const MediumUDec& rValue)
        { MediumUDec lValue = *this; return lValue.SubOperation(rValue); } const

    #pragma endregion NormalRep AltNum Addition/Subtraction Operations

    #pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec& operator+=(MediumUDec& lValue, const MediumUDec& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// += operation between MediumUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumUDec& operator+=(MediumUDec& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumUDec& operator+=(MediumUDec& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumUDec& operator+=(MediumUDec& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumUDec& operator+=(MediumUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumUDec& operator+=(MediumUDec& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumUDec& operator+=(MediumUDec& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumUDec& operator+=(MediumUDec& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumUDec& operator+=(MediumUDec& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec operator+(MediumUDec lValue, const MediumUDec& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// Addition operation between MediumUDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumUDec operator+(MediumUDec lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumUDec operator+(MediumUDec lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumUDec operator+(MediumUDec lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumUDec operator+(MediumUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumUDec operator+(MediumUDec lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumUDec operator+(MediumUDec lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumUDec operator+(MediumUDec lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumUDec operator+(MediumUDec lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        friend MediumUDec operator+(const signed int& lValue, MediumUDec rValue)  { return rValue.IntAddOperation(lValue); }
        friend MediumUDec operator+(const signed __int64& lValue, MediumUDec rValue)  { return rValue.Int64AddOperation(lValue); }
        friend MediumUDec operator+(const unsigned int& lValue, MediumUDec rValue)  { return rValue.UIntAddOperation(lValue); }
        friend MediumUDec operator+(const unsigned __int64& lValue, MediumUDec rValue)  { return rValue.UInt64AddOperation(lValue); }

        friend MediumUDec operator+(const signed char& lValue, MediumUDec rValue)  { return rValue.Int8AddOperation(lValue); }
        friend MediumUDec operator+(const signed short& lValue, MediumUDec rValue)  { return rValue.Int16AddOperation(lValue); }
        friend MediumUDec operator+(const unsigned char& lValue, MediumUDec rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend MediumUDec operator+(const unsigned short& lValue, MediumUDec rValue)  { return rValue.UInt16AddOperation(lValue); }

    #pragma endregion Other addition operations

    #pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec& operator-=(MediumUDec& lValue, const MediumUDec& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// -= operation between MediumUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec& operator-=(MediumUDec& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumUDec& operator-=(MediumUDec& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumUDec& operator-=(MediumUDec& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumUDec& operator-=(MediumUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumUDec& operator-=(MediumUDec& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumUDec& operator-=(MediumUDec& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumUDec& operator-=(MediumUDec& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumUDec& operator-=(MediumUDec& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec operator-(MediumUDec lValue, const MediumUDec& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation between MediumUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumUDec</returns>
        friend MediumUDec operator-(MediumUDec lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumUDec operator-(MediumUDec lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumUDec operator-(MediumUDec lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumUDec operator-(MediumUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumUDec operator-(MediumUDec lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumUDec operator-(MediumUDec lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumUDec operator-(MediumUDec lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumUDec operator-(MediumUDec lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        friend MediumUDec operator-(const signed int& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).SubtractBy(rValue); }
        friend MediumUDec operator-(const signed __int64& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).SubtractBy(rValue); }
        friend MediumUDec operator-(const unsigned int& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).SubtractBy(rValue); }
        friend MediumUDec operator-(const unsigned __int64& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).SubtractBy(rValue); }

        friend MediumUDec operator-(const signed char& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).SubtractBy(rValue); }
        friend MediumUDec operator-(const signed short& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).SubtractBy(rValue); }
        friend MediumUDec operator-(const unsigned char& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).SubtractBy(rValue); }
        friend MediumUDec operator-(const unsigned short& lValue, const MediumUDec& rValue) { return ((MediumUDec)lValue).SubtractBy(rValue); }

    #pragma endregion Other subtraction operations

    #pragma region Modulus Operations
    protected:

        template<MediumUDecVariant VariantType=MediumUDec>
        void ModulusOpV1(const VariantType& rValue)
        {
            if(DecimalHalf.Value==0&&rValue.DecimalHalf.Value==0)
                IntHalf.Value %= rValue.IntHalf.Value;
            else {
                auto divRes = DivideBy(rValue);
                SubOp(divRes.MultiplyByUnsigned(rValue));
            }
        }

    public:

        void IntModulusOp(const unsigned int& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf %= rValue;
            else {
                auto divRes = DivideByIntV1(rValue);
                SubOp(divRes.MultiplyByUInt(rValue));
            }
        }

        void Int64ModulusOp(const unsigned __int64& rValue)
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

        void ModulusOp(const MediumUDec& rValue){ ModulusOpV1(rValue); }

        MediumUDec& ModulusOperation(const MediumUDec& rValue){
            ModulusOp(rValue); return *this;
        }

        friend MediumUDec& operator%=(MediumUDec& lValue, const MediumUDec& rValue) { return lValue.ModulusOperation(rValue); }
        friend MediumUDec& operator%=(MediumUDec& lValue, const signed int& rValue) { lValue.IntModulusOp(rValue); return lValue; }
        friend MediumUDec& operator%=(MediumUDec& lValue, const unsigned int& rValue) { lValue.IntModulusOp(rValue); return lValue; }
        friend MediumUDec& operator%=(MediumUDec& lValue, const unsigned __int64& rValue) { lValue.Int64ModulusOp(rValue); return lValue;}
        friend MediumUDec& operator%=(MediumUDec& lValue, const signed __int64& rValue) { lValue.Int64ModulusOp(rValue); return lValue;}

    #pragma region Modulus Operations

    #pragma region Bitwise Operations
    //Update code later
    /*
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between MediumUDec and Integer tValue
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumUDec</returns>
        template<IntegerType IntType=signed int>
        friend MediumUDec operator^(MediumUDec self, IntType tValue)
        {
            if (self.DecimalHalf == 0) { self.IntHalf ^= tValue; return self; }
            else
            {
                bool SelfIsNegative = self.IntHalf < 0;
                bool ValIsNegative = tValue < 0;
                if (SelfIsNegative && self.IntHalf == NegativeRep)
                {
                    self.IntHalf = (0 & tValue) * -1;
                    self.DecimalHalf ^= tValue;
                }
                else
                {
                    self.IntHalf ^= tValue; self.DecimalHalf ^= tValue;
                }
            }
            return self;
        }

        /// <summary>
        /// Bitwise Or Operation Between MediumUDec and Integer tValue
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumUDec</returns>
        template<IntegerType IntType=signed int>
        friend MediumUDec operator|(MediumUDec self, IntType tValue)
        {
            if (self.DecimalHalf == 0) { self.IntHalf |= tValue; return self; }
            else
            {
                bool SelfIsNegative = self.IntHalf < 0;
                bool ValIsNegative = tValue < 0;
                if (SelfIsNegative && self.IntHalf == NegativeRep)
                {
                    self.IntHalf = (0 & tValue) * -1;
                    self.DecimalHalf |= tValue;
                }
                else
                {
                    self.IntHalf |= tValue; self.DecimalHalf |= tValue;
                }
            }
            return self;
        }
    #endif
    */
    #pragma endregion Bitwise Operations

    /*
    #pragma region Floating Operator Overrides

        friend MediumUDec operator+(const MediumUDec& self, const float& tValue) { return self + (MediumUDec)tValue; }
        friend MediumUDec operator-(const MediumUDec& self, const float& tValue) { return self - (MediumUDec)tValue; }
        friend MediumUDec operator*(const MediumUDec& self, const float& tValue) { return self * (MediumUDec)tValue; }
        friend MediumUDec operator/(const MediumUDec& self, const float& tValue) { return self / (MediumUDec)tValue; }

        friend MediumUDec operator+(const float& tValue, const MediumUDec& self) { return (MediumUDec)tValue + self; }
        friend MediumUDec operator-(const float& tValue, const MediumUDec& self) { return (MediumUDec)tValue - self; }
        friend MediumUDec operator*(const float& tValue, const MediumUDec& self) { return (MediumUDec)tValue * self; }
        friend MediumUDec operator/(const float& tValue, const MediumUDec& self) { return (MediumUDec)tValue / self; }

        friend MediumUDec operator+(const MediumUDec& self, const double& tValue) { return self + (MediumUDec)tValue; }
        friend MediumUDec operator-(const MediumUDec& self, const double& tValue) { return self - (MediumUDec)tValue; }
        friend MediumUDec operator*(const MediumUDec& self, const double& tValue) { return self * (MediumUDec)tValue; }
        friend MediumUDec operator/(const MediumUDec& self, const double& tValue) { return self / (MediumUDec)tValue; }

        friend MediumUDec operator+(const MediumUDec& self, const long double& tValue) { return self + (MediumUDec)tValue; }
        friend MediumUDec operator-(const MediumUDec& self, const long double& tValue) { return self - (MediumUDec)tValue; }
        friend MediumUDec operator*(const MediumUDec& self, const long double& tValue) { return self * (MediumUDec)tValue; }
        friend MediumUDec operator/(const MediumUDec& self, const long double& tValue) { return self / (MediumUDec)tValue; }

        friend MediumUDec operator+(const long double& tValue, const MediumUDec& self) { return (MediumUDec)tValue + self; }
        friend MediumUDec operator-(const long double& tValue, const MediumUDec& self) { return (MediumUDec)tValue - self; }
        friend MediumUDec operator*(const long double& tValue, const MediumUDec& self) { return (MediumUDec)tValue * self; }
        friend MediumUDec operator/(const long double& tValue, const MediumUDec& self) { return (MediumUDec)tValue / self; }

    #pragma endregion Floating Operator Overrides
    */


    #pragma region Other Operators

        /// <summary>
        /// ++MediumUDec Operator
        /// </summary>
        /// <returns>MediumUDec &</returns>
        MediumUDec& operator ++()
        {
        #if defined(AltNum_PreventUnsignedUnderflowCheck)
            ++IntHalf;
        #else
            if (DecimalHalf == 0)
                ++IntHalf;
            else if (IntHalf == 0)
                IntHalf = 1;
            else
                ++IntHalf;
            return *this;
        #endif
        }

        /// <summary>
        /// ++MediumUDec Operator
        /// </summary>
        /// <returns>MediumUDec &</returns>
        MediumUDec& operator --()
        {
        #if defined(AltNum_PreventUnsignedUnderflowCheck)
            --IntHalf;
        #else
            if (DecimalHalf == 0)
                --IntHalf;
            else if (IntHalf == 0)
                throw("Unsigned underflow");
            else
                --IntHalf;
        #endif
            return *this;
        }

        /// <summary>
        /// MediumUDec Variant++ Operator
        /// </summary>
        /// <returns>MediumUDec</returns>
        MediumUDec operator ++(int)
        {
            MediumUDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumUDec Variant-- Operator
        /// </summary>
        /// <returns>MediumUDec</returns>
        MediumUDec operator --(int)
        {
            MediumUDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumUDec Variant* Operator
        /// </summary>
        /// <returns>MediumUDec &</returns>
        MediumUDec& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

    #pragma region Truncation Functions

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="precision">precision level of digits to cut off</param>
        void ApplyFloorOf(const int& precision = 0);

protected:

        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType CeilV1(const VariantType& tValue)
        {
            if(tValue.IntHalf==unsigned int::NegativeZero)
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
        template<MediumUDecVariant VariantType = MediumUDec>
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
            if(decimalRes==0&&tValue.IntHalf==unsigned int::NegativeZero)
                return VariantType();
            else
                return VariantType(tValue.IntHalf, PartialInt(decimalRes,tValue.DecimalHalf.Flags));
        }

        template<MediumUDecVariant VariantType = MediumUDec>
        const VariantType TruncOfV1() const
        {
            return VariantType(IntHalf == NegativeRep?0:IntHalf, 0);
        }

public:

        ///<summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        ///</summary>
        /// <param name="tValue">Variable to apply method on</param>
        static MediumUDec Ceil(const MediumUDec& tValue) { return CeilV1<MediumUDec>(tValue); }

        ///<summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        ///</summary>
        /// <param name="tValue">Variable to apply method on</param>
        MediumUDec CeilOf() const { return Ceil(*this); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to tValue (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumUDec&</returns>
        signed int FloorIntOf() const;

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to tValue (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumUDec&</returns>
        static signed int FloorInt(const MediumUDec& tValue) { return tValue.FloorIntOf(); }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumUDec&</returns>
        int CeilIntOf() const;

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumUDec&</returns>
        static signed int CeilInt(const MediumUDec& tValue) { return tValue.CeilIntOf(); }

        static MediumUDec Trunc(const MediumUDec& tValue);

    #pragma endregion Truncation Functions

    #pragma region Pow and Sqrt Functions
protected:

        /// <summary>
        /// Perform square root on this instance.
        /// (Code other than switch statement adjusted from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        template<MediumUDecVariant VariantType = MediumUDec>
        static VariantType UnsignedSqrtV1(VariantType value, const int& precision=7)
        {
            if (value.DecimalHalf == 0)
            {
                bool AutoSetValue = true;
                switch (value.IntHalf.Value)
                {
                case 1: value.IntHalf = 1; break;
                case 4: value.IntHalf = 2; break;
                case 9: value.IntHalf = 3; break;
                case 16: value.IntHalf = 4; break;
                case 25: value.IntHalf = 5; break;
                case 36: value.IntHalf = 6; break;
                case 49: value.IntHalf = 7; break;
                case 64: value.IntHalf = 8; break;
                case 81: value.IntHalf = 9; break;
                case 100: value.IntHalf = 10; break;
                case 121: value.IntHalf = 11; break;
                case 144: value.IntHalf = 12; break;
                case 169: value.IntHalf = 13; break;
                case 196: value.IntHalf = 14; break;
                case 225: value.IntHalf = 15; break;
                case 256: value.IntHalf = 16; break;
                case 289: value.IntHalf = 17; break;
                case 324: value.IntHalf = 18; break;
                case 361: value.IntHalf = 19; break;
                case 400: value.IntHalf = 20; break;
                case 1600: value.IntHalf = 40; break;
                default:
                    AutoSetValue = false;
                    break;
                }
                if(AutoSetValue)
                    return value;//Technically both positive and negative numbers of same equal the result
            }

            VariantType start = VariantType::Zero, end = value;
            VariantType mid;

            // variable to store the answer
            VariantType ans;

            // for computing integral part
            // of square root of number
            while (start <= end) {
                mid = (start + end);
                mid.DivideByTwo();
                if (mid * mid == value) {
                    ans = mid;
                    break;
                }

                // incrementing start if integral
                // part lies on right side of the mid
                if (mid * mid < value) {
                    start = mid + 1;
                    ans = mid;
                }

                // decrementing end if integral part
                // lies on the left side of the mid
                else {
                    end = mid - 1;
                }
            }

            // For computing the fractional part
            // of square root up to given precision
            VariantType increment = VariantType::PointOne;//0.1
            for (int i = 0; i < precision; ++i) {
                while (ans * ans <= value) {
                    ans += increment;
                }

                // loop terminates when ans * ans > number
                ans = ans - increment;
                increment = increment / 10;
            }
            return ans;
        }

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        template<MediumUDecVariant VariantType = MediumUDec>
        static VariantType SqrtV1(VariantType value,const unsigned int& precision=7)
        {
            if(value.IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
            return UnsignedSqrtV1(value, precision);
        }

public:

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static MediumUDec Sqrt(const auto& value, const int& precision = 7);

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        MediumUDec SqrtOf(const int& precision = 7) const;

protected:
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        template<MediumUDecVariant VariantType=MediumUDec, IntegerType IntType=unsigned int>
        static VariantType UIntPowV1(const VariantType& tValue, const IntType& expValue)
        {
            if (expValue == 1)
                return tValue;
            else if (expValue == 0)
                return VariantType::One;
            else if (tValue.DecimalHalf == 0 && tValue.IntHalf.Value == 10)
            {
                VariantType result = tValue;
                result.IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
                return result;
            }
            else
            {
                IntType exp = expValue;
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = tValue.IsPositive()?false:(exp&1)==1?false:true;
                VariantType self = tValue.AbsOf();
                VariantType result = VariantType::One;
                while (exp > 0)
                {
                    // If expValue is odd, multiply self with result
                    if ((exp&1) == 1)
                        result.UnsignedMultOp(self);
                    // n must be even now
                    exp = exp >> 1; // y = y/2
                    self.UnsignedMultOp(self); // Change x to x^2
                }
                return result;
            }
        }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        template<MediumUDecVariant VariantType=MediumUDec, IntegerType IntType=signed int>
        static VariantType IntPowV1(const VariantType& tValue, const IntType& expValue)
        {
            if (expValue < 0)//Negative Pow
            {
                IntType exp = expValue * -1;
                if (tValue.DecimalHalf == 0 && tValue.IntHalf == 10 && expValue >= -9)
                {
                    VariantType result = VariantType(0, DecimalOverflow / VariableConversionFunctions::PowerOfTens[exp]);
                    return result;
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    //Switches from negative to positive if exp is odd number
                    bool IsNegative = tValue.IsPositive()?false:(exp&1)==1?false:true;
                    VariantType self = tValue.AbsOf();//Prevent needing to flip the sign
                    VariantType result = VariantType::One;
                    while (exp > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if ((exp & 1) == 1)
                            result.DivOp(self);
                        // n must be even now
                        exp = exp >> 1; // y = y/2
                        self.UnsignedMultOp(self); //  Change x to x^2
                    }
                    if(IsNegative)
                        result.IntHalf.Sign = unsigned int::NegativeSign;
                }
            }
            else
                return UIntPowV1(tValue, expValue);
        }

        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType UnsignedNegIntPowerV1(const VariantType& tValue, const unsigned int& expValue)
        {
            unsigned int exp = expValue;
            //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
            //Switches from negative to positive if exp is odd number
            bool IsNegative = tValue.IsPositive()?false:(exp&1)==1?false:true;
            VariantType self = tValue.AbsOf();
            VariantType result = VariantType::One;
            while (exp > 0)
            {
                // If expValue is odd, divide self with result
                if ((exp & 1) == 1)
                    result.DivOp(self);
                // n must be even now
                exp = exp >> 1; // y = y/2
                self.UnsignedMultOp(self); // Change x to x^2
            }
            return result;
        }

public:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumUDec UIntPow(const MediumUDec& tValue, const unsigned int& expValue)
        { return IntPowV1<MediumUDec>(tValue, expValue); }
        static MediumUDec UInt64Pow(const MediumUDec& tValue, const unsigned __int64& expValue)
        { return UIntPowV1<MediumUDec>(tValue, expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumUDec IntPow(const MediumUDec& tValue, const signed int& expValue)
        { return IntPowV1<MediumUDec>(tValue, expValue); }
        static MediumUDec Int64Pow(const MediumUDec& tValue, const signed __int64& expValue)
        { return IntPowV1<MediumUDec>(tValue, expValue); }

        MediumUDec UnsignedNegIntPower(const MediumUDec& tValue, const unsigned int& expValue)
        { return UnsignedNegIntPowerV1<MediumUDec>(tValue, expValue); }


        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumUDec UIntPowOf(const unsigned int& expValue) const
        { return UIntPow(*this, expValue); }
        MediumUDec UInt64PowOf(const unsigned __int64& expValue) const
        { return UInt64Pow(*this, expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumUDec IntPowOf(const signed int& expValue) const
        { return IntPow(*this, expValue); }
        MediumUDec Int64PowOf(const signed __int64& expValue) const
        { return Int64Pow(*this, expValue); }

protected:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType NthRootV1(const VariantType& tValue, const unsigned int& n, const VariantType& precision)
        {
            VariantType xPre = ((tValue - 1) / n) + 1;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two
            // roots by INT_MAX
            VariantType delX = VariantType(2147483647);

            //  xK denotes current value of x
            VariantType xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method

                xK = xPre * nMinus1;
                xK += DivisionV1(tValue, UIntPowV1(xPre, nMinus1));
                xK /= n;
                delX = VariantType::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="tValue">The target value(radicand) to perform operation on.</param>
        /// <param name="nValue">The nth root degree value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType NthRootV1(const VariantType& tValue, const unsigned int& n, const VariantType& precision)
        {
            if (tValue.IsNegative())
                throw "Nth root of a negative number requires imaginary number support";
            return UnsignedNthRootV1(tValue, n, precision);
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType NthRootV2(const VariantType& tValue, const unsigned int& n, const VariantType& Precision = VariantType::FiveBillionth)
        {
            if(n==0)
                throw "Can't return results of zeroth root";//Negative roots require imaginary numbers to support
            unsigned int nMinus1 = n - 1;
            VariantType OneByN = VariantType::One/n;
            VariantType InitialX1 = tValue - tValue/n;//One/n * tValue * (n- 1) == tValue/n * (n - 1) == tValue - tValue/n
            InitialX1 += DivisionV1(tValue, tValue.UIntPowOf(nMinus1));
            VariantType x[2] = { InitialX1, tValue };
            while (Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = OneByN * ((x[1]*nMinus1) + DivisionV1(tValue, x[1].UIntPowOf(nMinus1)));
            }
            return x[0];
        }

public:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="tValue">The target value(radicand) to perform operation on.</param>
        /// <param name="nValue">The nth root degree value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        static MediumUDec NthRoot(const MediumUDec& tValue, const unsigned int& n, const MediumUDec& precision = MediumUDec::JustAboveZero)
        {
            return NthRootV1<MediumUDec>(tValue, n, precision);
        }

        MediumUDec NthRootOf(const unsigned int& n, const MediumUDec& precision = MediumUDec::JustAboveZero) const;

        static MediumUDec AlternativeNthRoot(const MediumUDec& tValue, const unsigned int& n, const MediumUDec& precision = MediumUDec::FiveBillionth);

protected:

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType FractionalPowV1(const auto& value, const boost::rational<unsigned int>& Frac)
        {
            VariantType targetVal = UIntPowOf(Frac.numerator());
            VariantType CalcVal = MediumUDecVariant::NthRoot(targetVal, Frac.denominator());
            return CalcVal;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        template<MediumUDecVariant VariantType=MediumUDec>
        const VariantType FractionalPowV2(const VariantType& value, const signed int& expNum, const unsigned int& expDenom)
        {
            auto targetVal = IntPowOf(expNum);
            auto CalcVal = MediumUDecVariant::NthRoot(targetVal, expDenom);
            return CalcVal;
        }

        /// <summary>
        /// Calculate to power of unsigned expValue
        /// (Doesn't modify owner object)
        /// </summary>
        template<MediumUDecVariant VariantType=MediumUDec>
        const VariantType UnsignedPowOfV1(const auto& expValue)
        {
            boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumUDecVariant::DecimalOverflow);
            if(expValue.IntHalf.Value==0)
                return FractionalPowV1(Frac);
            else {
                VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
                CalcVal *= FractionalPowV1(Frac);
                return CalcVal;
            }
        }

        /// <summary>
        /// Calculate to power of expValue
        /// (Doesn't modify owner object)
        /// </summary>
        template<MediumUDecVariant VariantType=MediumUDec>
        const VariantType PowOfV1(const auto& expValue)
        {
            boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumUDecVariant::DecimalOverflow);
            if (expValue.IntHalf.IsNegative()){//Negative Exponent
                if(expValue.IntHalf.Value==0)
                    return VariantType::One/FractionalPowV1(Frac);
                else {
                    VariantType CalcVal = One / UIntPowOf(expValue.IntHalf.Value);
                    CalcVal /= FractionalPowV1(Frac);
                    return CalcVal;
                }
            } else {
                if(expValue.IntHalf.Value==0)
                    return FractionalPowV1(Frac);
                else {
                    VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
                    CalcVal *= FractionalPowV1(Frac);
                    return CalcVal;
                }
            }
        }

public:

        /// <summary>
        /// Calculate to power of expValue
        /// (Doesn't modify owner object)
        /// </summary>
        MediumUDec UnsignedPowOf(const auto& expValue)
        { return UnsignedPowOfV1(expValue); }

        /// <summary>
        /// Calculate to power of expValue
        /// (Doesn't modify owner object)
        /// </summary>
        MediumUDec PowOf(const auto& expValue)
        { return PowOfV1(expValue); }

    #pragma endregion Pow and Sqrt Functions

    #pragma region Log Functions
protected:

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>VariantType</returns>
        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType ExpV1(const VariantType& x)
        {
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
             *      - x: power of e to evaluate
             * Returns:
             *      - approximation of e^x in VariantType precision
             */
             // Check that x is a valid input.
            assert(x.IntHalf.Value < 709);
            // When x = 0 we already know e^x = 1.
            if (x.IsZero()) {
                return VariantType::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            VariantType x0 = VariantType::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            VariantType tn = VariantType::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = VariantType::CeilInt(x0 * VariantType::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + VariantType::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x.IsNegative()) {
                tn = VariantType::One / tn;
            }
            return tn;
        }

        //Common log calculations for when value is between 0 and one
        template<MediumUDecVariant VariantType=MediumUDec>
        VariantType LogZeroRangePart02(const VariantType& AccuracyLevel=VariantType::JustAboveZero) const
        {
            VariantType TotalRes = (*this - 1)/ (*this + 1);
            VariantType WSquared = TotalRes * TotalRes;
            VariantType LastPow = -TotalRes;
            int WPow = 3;
            VariantType AddRes;

            do
            {
                LastPow *= WSquared;
                AddRes = LastPow / WPow;
                TotalRes -= AddRes;
                WPow += 2;
            } while (AddRes > VariantType::JustAboveZero);
            return TotalRes;
        }

        //Common natural log calculations for range one to two
        template<MediumUDecVariant VariantType=MediumUDec>
        const VariantType LnOfOneSection(const VariantType& threshold = VariantType::FiveBillionth) const
        {
            VariantType base = *this - 1;        // Base of the numerator; exponent will be explicit
            bool posSign = true;             // Used to swap the sign of each term
            VariantType term = base;       // First term
            VariantType prev;          // Previous sum
            VariantType result = term;     // Kick it off
            // den = Denominator of the nth term
            for(unsigned int den = 2;VariantType::Abs(prev - result) > threshold;++den){
                posSign = !posSign;
                term *= base;
                prev = result;
                if (posSign)
                    result += term / den;
                else
                    result -= term / den;
            }
            return result;
        }

        //Common log calculations for when value is greater than one
        template<MediumUDecVariant VariantType=MediumUDec>
        VariantType LogGreaterRangePart02(const VariantType& AccuracyLevel=VariantType::JustAboveZero) const
        {
            //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
            VariantType TotalRes = (*this - 1) / (*this + 1);
            VariantType LastPow = TotalRes;
            VariantType WSquared = TotalRes * TotalRes;
            VariantType AddRes;
            int WPow = 3;
            do
            {
                LastPow *= WSquared;
                AddRes = LastPow / WPow;
                TotalRes += AddRes; WPow += 2;
            } while (AddRes > AccuracyLevel);
            return TotalRes;
        }

        template<MediumUDecVariant VariantType=MediumUDec, IntegerType IntType = unsigned int>
        static VariantType LogGreaterRangeIntPart02(const IntType& value, const VariantType& AccuracyLevel=VariantType::JustAboveZero)
        {
            VariantType tValue = VariantType(value);
            return tValue.LogGreaterRangePart02(AccuracyLevel);
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumUDec variant</returns>
        template<MediumUDecVariant VariantType = MediumUDec>
        #if defined(AltNum_UseCustomLnAccuracy)
        const VariantType LnOfV1(const VariantType& threshold = VariantType::FiveMillionth) const
        #else
        const VariantType LnOfV1() const
        #endif
        {//Negative values for natural log return value of LnV1(-value) * i
            //if (value <= 0) {}else//Error if equal or less than 0
            if (IsOne())
                return VariantType::Zero;
            if (IntHalf == 0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
                throw("Unsigned class can't return negative numbers);
            else if (IntHalf == 1)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                #if defined(AltNum_UseCustomLnAccuracy)
                return LnOfOneSection(threshold);
                #else
                return LnOfOneSection();
                #endif
            }
            else
            {//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogGreaterRangePart02(threshold).MultipliedByTwo();
                #else
                return LogGreaterRangePart02().MultipliedByTwo();
                #endif
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumUDec variant</returns>
        template<MediumUDecVariant VariantType = MediumUDec>
        static VariantType LnV1(const VariantType& value)
        {
            return value.LnOfV1();
        }

        /// <summary>
        /// Log Base 10 of tValue
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumUDec</returns>
        template<MediumUDecVariant VariantType = MediumUDec>
        #if defined(AltNum_UseCustomLnAccuracy)
        static VariantType Log10V1(const VariantType& lValue, const VariantType& threshold = VariantType::FiveMillionth)
        #else
        static VariantType Log10V1(const VariantType& lValue)
        #endif
        {
            if (lValue.IsOne())
                return VariantType::Zero;
            #if !defined(AltNum_PreventLog10IntegerLoop)
            if (lValue.DecimalHalf == 0 && lValue.IntHalf.Value % 10 == 0)
            {//Might not be worth using checking to use this alternative code since since 10s aren't that common
                for (int index = 1; index < 9; ++index)
                {
                    if (lValue.IntHalf.Value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return VariantType::InitializeV2(index, 0);
                }
                return VariantType::InitializeV2(9, 0);
            }
            #endif
            const VariantType lnMultiplier = VariantType::InitializeV2(0, LN10Div_DecSection);
            if (lValue.IntHalf == unsigned int::Zero)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                VariantType result = lValue.LogZeroRangePart02(threshold); return result.MultiplyByUnsigned(lnMultiplier);
                #else
                VariantType result = lValue.LogZeroRangePart02(); return result.MultiplyByUnsigned(lnMultiplier);
                #endif
            }
            else if (lValue.IntHalf == unsigned int::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                #if defined(AltNum_UseCustomLnAccuracy)
                VariantType result = lValue.LnOfOneSection(threshold); return result.MultiplyByUnsigned(lnMultiplier);
                #else
                VariantType result = lValue.LnOfOneSection(); return result.MultiplyByUnsigned(lnMultiplier);
                #endif
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                VariantType result = lValue.LogGreaterRangePart02(threshold); return result.MultiplyByUnsigned(lnMultiplier);
                #else
                VariantType result = lValue.LogGreaterRangePart02(); return result.MultiplyByUnsigned(lnMultiplier);
                #endif
            }
        }

        /// <summary>
        /// Log Base 10 of tValue(integer value variant)
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumUDec</returns>
        template<MediumUDecVariant VariantType=MediumUDec, IntegerType IntType = unsigned int>
        static VariantType Log10OfIntV1(const IntType& value)
        {
            if (value == 1)
                return VariantType::Zero;
            if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return VariantType(index);
                }
                return VariantType(9);
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                VariantType lnMultiplier = VariantType(0, TwiceLN10Div_DecSection);
                return LogGreaterRangeIntPart02(value).MultiplyByUnsigned(lnMultiplier);
            }
        }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumUDec Variant</returns>
        template<MediumUDecVariant VariantType=MediumUDec>
        static VariantType LogV1(const VariantType& value, const VariantType& baseVal)
        {
            if (value == VariantType::One)
                return VariantType::Zero;
            return Log10V1(value) / Log10V1(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>VariantType</returns>
        template<MediumUDecVariant VariantType=MediumUDec, IntegerType IntType = unsigned int>
        static VariantType LogOfIntV1(const VariantType& value, const IntType& baseVal)
        {
            //Calculate Base log first
            VariantType baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = VariantType::Initialize(index, 0);
                        break;
                    }
                }
                baseTotalRes = VariantType(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = LogGreaterRangeIntPart02(baseVal);
            }
            VariantType lnMultiplier = VariantType(0, TwiceLN10Div_DecSection);
            //Now calculate other log
            if (value.DecimalHalf == 0 && value.IntHalf.Value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? VariantType::Initialize(index, 0) / (baseTotalRes * lnMultiplier): VariantType::Initialize(index, 0)/ baseTotalRes;
                }
                return lnMultLog? VariantType(9, 0) / (baseTotalRes.MultiplyByUnsigned(lnMultiplier)):VariantType::Initialize(9, 0)/baseTotalRes;
            }
            if(value.IntHalf==unsigned int::Zero)//Not tested this block but should work
            {
                VariantType TotalRes = value.LogZeroRangePart02();
                if(lnMultLog)
                    return TotalRes.DivideBy(baseTotalRes);
                else
                    return (TotalRes.MultiplyByUnsigned(lnMultiplier)).DivideBy(baseTotalRes);
            }
            else if (value.IntHalf==unsigned int::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                if(lnMultLog)
                    return value.LnOfOneSection()/baseTotalRes;
                else
                    return (value.LnOfOneSection().MultipliedByTwo())/ baseTotalRes;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                VariantType TotalRes = value.LogGreaterRangePart02();
                if(lnMultLog)
                    return TotalRes.DivideBy(baseTotalRes);
                else
                    return (TotalRes.MultiplyByUnsigned(lnMultiplier)).DivideBy(baseTotalRes);
            }
        }

public:

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>MediumUDec</returns>
        static MediumUDec Exp(const MediumUDec& x) { return ExpV1(x); }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumUDec</returns>
        static MediumUDec Ln(const MediumUDec& value)
        { return LnV1(value); }

        /// <summary>
        /// Log Base 10 of tValue
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumUDec</returns>
        static MediumUDec Log10(const MediumUDec& value)
        { return Log10V1(value); }

        /// <summary>
        /// Log Base 10 of tValue(integer value variant)
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumUDec</returns>
        static MediumUDec Log10OfInt(const unsigned int& value)
        { return Log10OfIntV1(value); }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumUDec Variant</returns>
        static MediumUDec Log(const MediumUDec& value, const MediumUDec& baseVal)
        { return LogV1(value, baseVal); }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumUDec</returns>
        static MediumUDec LogOfInt(const MediumUDec& value, const unsigned int& baseVal)
        { return LogOfIntV1(value, baseVal); }

    #pragma endregion Log Functions

    #pragma region Trigonomic Functions
protected:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<MediumUDecVariant VariantType=MediumUDec>
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
        template<MediumUDecVariant VariantType=MediumUDec>
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
        template<MediumUDecVariant VariantType=MediumUDec>
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
        template<MediumUDecVariant VariantType=MediumUDec>
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
        template<MediumUDecVariant VariantType=MediumUDec>
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

    	template<MediumUDecVariant VariantType=MediumUDec>
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
        template<MediumUDecVariant VariantType=MediumUDec>
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
                        return MediumUDec::SinV1<VariantType>(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = PiNum * lValue / 180;
                return MediumUDec::SinV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from tValue of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<MediumUDecVariant VariantType=MediumUDec>
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
                        return MediumUDec::CosV1<VariantType>(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = PiNum * lValue / 180;
                return MediumUDec::CosV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from tValue in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<MediumUDecVariant VariantType=MediumUDec>
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
                        return MediumUDec::TanV1<VariantType>(PiNum * lValue / 180);
                        break;
                }
            }
            else
                return MediumUDec::TanV1<VariantType>(PiNum * lValue / 180);
        }

public:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        static MediumUDec Sin(const MediumUDec& tValue)
        { return SinV1<MediumUDec>(tValue); }

        /// <summary>
        /// Get cosine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumUDec Cos(const MediumUDec& tValue)
        { return CosV1<MediumUDec>(tValue); }

        /// <summary>
        /// Get Tan from value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumUDec Tan(const MediumUDec& tValue)
        { return TanV1<MediumUDec>(tValue); }

        /// <summary>
        /// Gets Inverse Tangent from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumUDec ATan(const MediumUDec& tValue)
        { return ATanV1<MediumUDec>(tValue); }

        /// <summary>
        /// Get Sin from value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumUDec SinFromAngle(const MediumUDec& tValue)
        { return SinFromAngleV1<MediumUDec>(tValue); }

        /// <summary>
        /// Get Cos() from value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumUDec CosFromAngle(const MediumUDec& tValue)
        { return CosFromAngleV1<MediumUDec>(tValue); }

        /// <summary>
        /// Get Tangent from value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumUDec TanFromAngle(const MediumUDec& tValue)
        { return TanFromAngleV1<MediumUDec>(tValue); }

        MediumUDec SinOf()
        { return Sin(*this); }

        MediumUDec CosOf()
        { return Cos(*this); }

        MediumUDec TanOf()
        { return Tan(*this); }

        MediumUDec ATanOf()
        { return ATan(*this);; }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        static MediumUDec ArcTan2(const MediumUDec& y, const MediumUDec& x)
        { return ArcTan2V1<MediumUDec>(y, x); }

    #pragma endregion Trigonomic Functions
    };

}


