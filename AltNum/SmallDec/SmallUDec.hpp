// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "SmallDecPreprocessors.h"

#include <string>
#include <cmath>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator

#include "SmallDecVariant.hpp"
#include "..\Concepts\IntegerConcept.hpp"

namespace BlazesRusCode
{
    class SmallUDec;

    //Used to store value range between 0 - 33554431.99 for things like receipts
    class DLL_API SmallUDec : SmallUNumBase
    {
    public:
    #pragma region DigitStorage

        /// <summary>
        /// Stores whole half of number (value range 0-33554431;Bitfield uses 25 bytes)
        /// </summary>
        unsigned int IntHalf:25;

        /// <summary>
        /// Stores decimal section info (value range 0-99;Bitfield uses 7 bytes)
        /// </summary>
        unsigned int DecimalHalf:7;

        //Return IntHalf as unsigned int
        inline unsigned int GetIntHalf() const
        {
            return IntHalf;
        }

        //Return DecimalHalf as unsigned char
        inline unsigned char GetDecHalf() const
        {
            return DecimalHalf;
        }
        
    #pragma endregion DigitStorage

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned int const DecimalOverflow = 100;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned _int64 const DecimalOverflowX = 100;

    #pragma region class_constructors


        /// <summary>
        /// Initializes a new instance of the <see cref="SmallUDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        SmallUDec(const unsigned int& intVal = 0, const unsigned int& decVal = 0)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        SmallUDec& operator=(const SmallUDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        SmallUDec& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<SmallUDecVariant VariantType>
        SmallUDec operator[](VariantType variantValue) const
        {
            SmallUDec newSelf = SmallUDec(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

        //Fix for C2440 error during static template class
        static SmallUDec Initialize(const unsigned int& intVal, const unsigned char& decVal = 0)
        {
            return SmallUDec(intVal,decVal);
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetValue(const SmallUDec& rValue)
        {
            IntHalf = rValue.IntHalf;
            DecimalHalf = rValue.DecimalHalf;
        }

    #pragma endregion class_constructors

    #pragma region Check_if_value

        //Set value as exactly zero
        void SetAsZero();

        //Set value as exactly one
        void SetAsOne();

        //Set as +-1 while keeping current sign
        void SetAsOneVal();

        void SetAsValues(const unsigned int& intVal = 0, const unsigned short& decVal = 0);

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
public:

        //Sets value to value at 0.5
        void  SetValueToPoint5();

        void  SetValueToJustAboveZero();

        void SetValueToPointOne();

    #pragma endregion ValueSetters

    #pragma region ValueDefines
public:

        static SmallUDec AlmostOneValue();

        static SmallUDec ZeroValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>SmallUDec</returns>
        static SmallUDec OneValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>SmallUDec</returns>
        static SmallUDec TwoValue();

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>SmallUDec</returns>
        static SmallUDec Point5Value();

        static SmallUDec JustAboveZeroValue();

        static SmallUDec MinimumValue();

        static SmallUDec MaximumValue();

        static SmallUDec PointOneValue();

        static const SmallUDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec PiNum;

        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec ENum;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec Pi;

        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec Zero;

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>SmallUDec</returns>
        static const SmallUDec OneGMillionth;

        //0e-7
        static const SmallUDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static const SmallUDec FiveMillionth;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State tValue that can store
        /// (-2147483647.999999999)
        /// </summary>
        static const SmallUDec Minimum;

        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State tValue that can store
        /// (2147483647.999999999)
        /// </summary>
        static const SmallUDec Maximum;

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static const SmallUDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static const SmallUDec LN10Div;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static const SmallUDec TwiceLN10Div;

        static const SmallUDec PointOne;

    #pragma endregion ValueDefines

    #pragma region String Commands

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(const std::string& Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="SmallUDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        SmallUDec(const char* strVal);

        /// <summary>
        /// Initializes a new instance of the <see cref="SmallUDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        SmallUDec(const std::string& Value);

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
        /// <param name="Value">The value.</param>
        void SetFloatVal(const float& tValue);

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDoubleVal(const double& tValue);

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDecimalVal(const long double& tValue);

    #endif

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetBoolVal(const bool& Value);

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(const unsigned int& Value);

    #if defined(AltNum_EnableFloatingConversion)

        /// <summary>
        /// Initializes a new instance of the <see cref="SmallUDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        SmallUDec(const float& tValue){ this->SetFloatVal(tValue); }

        /// <summary>
        /// Initializes a new instance of the <see cref="SmallUDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        SmallUDec(const double& tValue){ this->SetDoubleVal(tValue); }

        /// <summary>
        /// Initializes a new instance of the <see cref="SmallUDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        SmallUDec(const long double& tValue){ this->SetDecimalVal(tValue); }

    #endif

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #if defined(AltNum_EnableFloatingConversion)

        /// <summary>
        /// SmallUDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        float toFloat() const;

        /// <summary>
        /// SmallUDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        double toDouble() const;

        /// <summary>
        /// SmallUDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        long double toDecimal() const;

    #endif

        /// <summary>
        /// SmallUDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        signed int toInt() const { return IntHalf; }

        /// <summary>
        /// SmallUDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        unsigned int toUInt() const { return IntHalf; }

        bool toBool() const { return IntHalf==0 ? false : true; }

    #if defined(AltNum_EnableFloatingConversion)

        /// <summary>
        /// SmallUDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }

        /// <summary>
        /// SmallUDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDouble(); }

        /// <summary>
        /// SmallUDec Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator long double() { return toDecimal(); }

    #endif

        /// <summary>
        /// SmallUDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return toInt(); }

        /// <summary>
        /// SmallUDec Variant to unsigned int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return toUInt(); }

        /// <summary>
        /// SmallUDec Variant to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed __int64() { return toInt(); }

        /// <summary>
        /// SmallUDec Variant to unsigned byte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned char() { return toUInt(); }

        /// <summary>
        /// SmallUDec Variant to byte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed char() { return toInt(); }

        /// <summary>
        /// SmallUDec Variant to unsigned short explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned short() { return toUInt(); }

        /// <summary>
        /// SmallUDec Variant to short explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed short() { return toInt(); }

        /// <summary>
        /// SmallUDec Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
protected:
        //Compare only as if in NormalType representation mode
        template<SmallUDecVariant VariantType=SmallUDec>
        std::strong_ordering BasicComparisonV1(const VariantType& that) const
        {
            if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
                return IntHalfCmp;
            if (auto DecimalHalfCmp = DecimalHalf <=> that.DecimalHalf; DecimalHalfCmp != 0)
                return DecimalHalfCmp;
        }

        std::strong_ordering BasicComparison(const SmallUDec& that) const
        {
            return BasicComparisonV1(that);
        }

        //Compare only as if in NormalType representation mode
        std::strong_ordering BasicUIntComparison(const unsigned int& that) const
        {
            if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
                return IntHalfCmp;
            if (auto DecimalHalfCmp = DecimalHalf <=> 0; DecimalHalfCmp != 0)
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
                if (auto DecimalHalfCmp = DecimalHalf <=> 0; DecimalHalfCmp != 0)
                    return DecimalHalfCmp;
            }
        }

public:

        std::strong_ordering operator<=>(const SmallUDec& that) const
        {//return BasicComparison(that);
            if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
                return IntHalfCmp;
            if (auto DecimalHalfCmp = DecimalHalf <=> that.DecimalHalf; DecimalHalfCmp != 0)
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

        bool operator==(const SmallUDec& that) const
        {
            if (IntHalf!=that.IntHalf)
                return false;
            if (DecimalHalf!=that.DecimalHalf)
                return false;
            return true;
        }

        bool operator!=(const SmallUDec& that) const
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
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntHalf: DecimalOverflowX * IntHalf + DecimalHalf;
            Res = SelfRes / rValue;

            IntHalfRes = Res/DecimalOverflowX;
            DecimalRes = Res - DecimalOverflowX * IntHalfRes;
            IntHalf = (unsigned int)IntHalfRes;
            DecimalHalf = (unsigned int)DecimalRes;
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
        /// Basic division operation between SmallUDec Variant and unsigned Integer value
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
            PartialUIntDivOpV1(Value);
            if (IntHalf == 0 && DecimalHalf == 0)
                DecimalHalf = 1;//Prevent Dividing into nothing
        }

        template<IntegerType IntType=unsigned int>
        auto& IntDivOperationV1(const IntType& rValue)
        { IntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Basic division operation between SmallUDec Variant and unsigned Integer value
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

        void UnsignedIntegerDivOp(const signed int& rValue) { IntDivOpV1(rValue); }

        void UInt8DivOp(const unsigned char& rValue) { IntDivOpV1(rValue); }
        void Int8DivOp(const signed char& rValue) { IntDivOpV1(rValue); }
        void UInt16DivOp(const unsigned short& rValue) { IntDivOpV1(rValue); }
        void Int16DivOp(const signed short& rValue) { IntDivOpV1(rValue); }

        SmallUDec& UIntDivOperation(const unsigned int& rValue) { return IntDivOperationV1(rValue); }
        SmallUDec& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
        SmallUDec& UInt64DivOperation(const unsigned __int64& rValue) { return IntDivOperationV1(rValue); }
        SmallUDec& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
        SmallUDec& UInt8DivOperation(const unsigned char& rValue) { return IntDivOperationV1(rValue); }
        SmallUDec& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
        SmallUDec& UInt16DivOperation(const unsigned short& rValue) { return IntDivOperationV1(rValue); }
        SmallUDec& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

        const SmallUDec DivideByUInt(const unsigned int& rValue) { return DivideByIntV1(rValue); }
        const SmallUDec DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        const SmallUDec DivideByUInt64(const unsigned __int64& rValue) { return DivideByIntV1(rValue); }
        const SmallUDec DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        const SmallUDec UnsignedDivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        const SmallUDec UnsignedDivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        const SmallUDec DivideByUInt8(const unsigned char& rValue) { return DivideByIntV1(rValue); }
        const SmallUDec DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
        const SmallUDec DivideByUInt16(const unsigned short& rValue) { return DivideByIntV1(rValue); }
        const SmallUDec DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep AltNum Division Operations
protected:

        /// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        bool PartialDivOpV1(const VariantType& rValue)
        {
            unsigned _int64 SelfRes = DecimalOverflowX * IntHalf + DecimalHalf;
            unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf;

            unsigned _int64 IntHalfRes = SelfRes / ValueRes;
            unsigned _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
            IntHalf = (unsigned int) IntHalfRes;
            DecimalHalf = DecimalRes;
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
        template<SmallUDecVariant VariantType=SmallUDec>
        void DivOpV1(const VariantType& rValue)
        {
            if(DecimalHalf==0)
            {
                if(rValue.DecimalHalf==0)
                {
                    switch(rValue.IntHalf)
                    {
                        case 2:
                            if((IntHalf&1)==1)//Check if number is odd
                                UIntDivOp(2);
                            else
                                IntHalf /= 2;
                            break;
                        case 4:
                            if(((IntHalf >> 2) << 2) == IntHalf)
                                IntHalf /= 4;
                            else
                                UIntDivOp(4);
                            break;
                        case 8:
                            if(((IntHalf >> 3) << 3) == IntHalf)
                                IntHalf /= 8;
                            else
                                UIntDivOp(4);
                            break;
                        case 16:
                            if(((IntHalf >> 4) << 4) == IntHalf)
                                IntHalf /= 16;
                            else
                                UIntDivOp(4);
                            break;
                        case 32:
                            if(((IntHalf >> 5) << 5) == IntHalf)
                                IntHalf /= 32;
                            else
                                UIntDivOp(4);
                            break;
                        case 0:
                            throw "Target rValue can not be divided by zero";
                            break;
                        default:
                            UIntDivOp(rValue.IntHalf);
                            break;
                    }
                }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
                else if (PartialDivOp(rValue))//Prevent Dividing into nothing
                        DecimalHalf = 1;
#else
                else
                    PartialDivOp(rValue);
#endif
            }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
            else if (PartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf = 1;
#else
            else
                PartialDivOp(rValue);
#endif
        }

        template<SmallUDecVariant VariantType=SmallUDec>
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
        bool PartialDivOp(const SmallUDec& rValue){ return PartialDivOpV1(rValue); }

        /// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void DivOp(const SmallUDec& rValue){ DivOpV1(rValue); }

        /// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        SmallUDec& DivOperation(const SmallUDec& rValue)
        { DivOp(rValue); return *this; }

        /// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        SmallUDec DivideBy(const SmallUDec& rValue) const
        { SmallUDec lValue = *this; return lValue.DivOperation(rValue); }

    #pragma endregion NormalRep AltNum Division Operations

    #pragma region Other Division Operations

        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>SmallUDec&</returns>
        void DivideByTwo();

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>SmallUDec&</returns>
        void DivideByFour();

        //Return copy of result divided by two
        SmallUDec DividedByTwo() const;

        //Return copy of result divided by four
        SmallUDec DividedByFour() const;

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec& operator/=(SmallUDec& lValue, const SmallUDec& rValue) { return lValue.DivOperation(rValue); }

        /// <summary>
        /// *= operation between SmallUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec& operator/=(SmallUDec& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend SmallUDec& operator/=(SmallUDec& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend SmallUDec& operator/=(SmallUDec& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend SmallUDec& operator/=(SmallUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend SmallUDec& operator/=(SmallUDec& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend SmallUDec& operator/=(SmallUDec& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend SmallUDec& operator/=(SmallUDec& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend SmallUDec& operator/=(SmallUDec& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec operator/(SmallUDec lValue, const SmallUDec& rValue) { return lValue.DivideBy(rValue); }

        /// <summary>
        /// Division operation between SmallUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec operator/(SmallUDec lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend SmallUDec operator/(SmallUDec lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend SmallUDec operator/(SmallUDec lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend SmallUDec operator/(SmallUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend SmallUDec operator/(SmallUDec lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend SmallUDec operator/(SmallUDec lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend SmallUDec operator/(SmallUDec lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend SmallUDec operator/(SmallUDec lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        friend SmallUDec operator/(const signed int& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).DivideBy(rValue); }
        friend SmallUDec operator/(const signed __int64& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).DivideBy(rValue); }
        friend SmallUDec operator/(const unsigned int& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).DivideBy(rValue); }
        friend SmallUDec operator/(const unsigned __int64& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).DivideBy(rValue); }

        friend SmallUDec operator/(const signed char& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).DivideBy(rValue); }
        friend SmallUDec operator/(const signed short& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).DivideBy(rValue); }
        friend SmallUDec operator/(const unsigned char& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).DivideBy(rValue); }
        friend SmallUDec operator/(const unsigned short& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).DivideBy(rValue); }

    #pragma endregion Other Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
        /// <summary>
        /// Partial version of UIntMultOpV1 without zero checks
        /// (Modifies owner object)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>SmallUDec</returns>
        template<IntegerType IntType=int>
        void PartialUIntMultOpV1(const IntType& rValue)
        {
            if (DecimalHalf == 0)
                IntHalf *= rValue;
            else
            {
                __int64 SRep = IntHalf == 0 ? DecimalHalf : DecimalOverflowX * IntHalf + DecimalHalf;
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
        /// Basic multiplication operation between SmallUDec variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        const SmallUDec MultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntMultOperationV1(rValue); }

        /// <summary>
        /// Basic multiplication operation between SmallUDec variant and Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        const SmallUDec MultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntMultOperationV1(rValue); }

        template<SmallUDecVariant VariantType=SmallUDec, IntegerType IntType=unsigned int>
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

        SmallUDec& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
        SmallUDec& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
        SmallUDec& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
        SmallUDec& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
        SmallUDec& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
        SmallUDec& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
        SmallUDec& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
        SmallUDec& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

        const SmallUDec MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
        const SmallUDec MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
        const SmallUDec MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
        const SmallUDec MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

        const SmallUDec UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
        const SmallUDec UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

        const SmallUDec MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
        const SmallUDec MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
        const SmallUDec MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
        const SmallUDec MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep AltNum Multiplication Operations
protected:

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned SmallUDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        void MultOpV1(const VariantType& rValue)
        {
            if (DecimalHalf == 0)
            {
                if (IntHalf == 1)
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
                    __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf;
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntHalf = (unsigned int)OverflowVal;
                        DecimalHalf = (unsigned int)rRep;
                        return;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                    #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
                    #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
                            SetAsZero(); return; }
                    #endif
                        IntHalf = 0;
                        return;
                    }
                }
            }
            else if (IntHalf == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= DecimalOverflowX;
                if (rValue.IntHalf == 0)
                {
                    DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf = 1;
                #elif !defined(AltNum_AllowNegativeZero)
                    if(DecimalHalf==0){
                        SetAsZero(); return; }
                #endif
                    return;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntHalf;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf = OverflowVal;
                        DecimalHalf = (signed int)SRep;
                        return;
                    }
                    else
                    {
                        DecimalHalf = (unsigned int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
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
                    __int64 SRep = DecimalOverflowX * IntHalf + DecimalHalf;
                    SRep *= rValue.IntHalf;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf = (unsigned int)OverflowVal;
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
                #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
                            SetAsZero(); return; }
                #endif
                        }
                        IntHalf = 0;
                    }
                    return;
                }
                else if (rValue.IntHalf == 0)
                {
                    __int64 SRep = DecimalOverflowX * IntHalf + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= DecimalOverflowX;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf = (unsigned int)OverflowVal;
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
                #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
                            SetAsZero(); return; }
                #endif
                        }
                        IntHalf = 0;
                    }
                    return;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf : DecimalOverflowX * IntHalf + DecimalHalf;
                    SRep *= rValue.IntHalf;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    unsigned __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf);//Temp03 holds __int64 version of X *.V
                    unsigned __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    unsigned __int64 IntegerRep = SRep + Temp03 + Temp04;
                    unsigned __int64 intHalf = IntegerRep / DecimalOverflow;
                    IntegerRep -= intHalf * DecimalOverflowX;
                    IntHalf = (unsigned int) intHalf;
                    DecimalHalf = (unsigned int)IntegerRep;
                }
            }
            #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntHalf==0)
                DecimalHalf = 1;
            #elif !defined(AltNum_AllowNegativeZero)
            if(DecimalHalf==0)
                SetAsZero();
            #endif
        }

        template<SmallUDecVariant VariantType=SmallUDec>
        void MultOpV1(const VariantType& rValue)
        {
            if(rValue.IsNegative())
            {
                SwapNegativeStatus();
                MultOp(-rValue);
            }
            else
                MultOp(rValue);
        }

        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType MultiplicationV1(VariantType lValue, const VariantType& rValue)
        {
            lValue.MultOpV1(rValue); return lValue;
        }

public:

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned SmallUDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void MultOp(const SmallUDec& rValue){ MultOpV1(rValue); }

        void MultOp(const SmallUDec& rValue){ MultOpV1(rValue); }

        /// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        SmallUDec& MultOperation(const SmallUDec& rValue)
        { MultOp(rValue); return *this; }

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        SmallUDec& MultOperation(const SmallUDec& rValue)
        { MultOp(rValue); return *this; }

        /// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        SmallUDec MultiplyBy(const SmallUDec& rValue) const
        { SmallUDec lValue = *this; return lValue.MultOperation(rValue); }

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        SmallUDec MultiplyBy(const SmallUDec& rValue) const
        { SmallUDec lValue = *this; return lValue.MultOperation(rValue); }

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
        SmallUDec MultipliedByTwo() const;

        //Return copy of result divided by four
        SmallUDec MultipliedByFour() const;

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec& operator*=(SmallUDec& lValue, const SmallUDec& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// *= operation between SmallUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend SmallUDec& operator*=(SmallUDec& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend SmallUDec& operator*=(SmallUDec& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend SmallUDec& operator*=(SmallUDec& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend SmallUDec& operator*=(SmallUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend SmallUDec& operator*=(SmallUDec& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend SmallUDec& operator*=(SmallUDec& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend SmallUDec& operator*=(SmallUDec& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend SmallUDec& operator*=(SmallUDec& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec operator*(SmallUDec lValue, const SmallUDec& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation between SmallUDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend SmallUDec operator*(SmallUDec lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend SmallUDec operator*(SmallUDec lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend SmallUDec operator*(SmallUDec lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend SmallUDec operator*(SmallUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend SmallUDec operator*(SmallUDec lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend SmallUDec operator*(SmallUDec lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend SmallUDec operator*(SmallUDec lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend SmallUDec operator*(SmallUDec lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        friend SmallUDec operator*(signed int lValue, SmallUDec rValue)  { return rValue.IntMultOperation(lValue); }
        friend SmallUDec operator*(signed __int64 lValue, SmallUDec& rValue)  { return rValue.Int64MultOperation(lValue); }
        friend SmallUDec operator*(unsigned int lValue, SmallUDec& rValue)  { return rValue.UIntMultOperation(lValue); }
        friend SmallUDec operator*(unsigned __int64 lValue, SmallUDec& rValue)  { return rValue.UInt64MultOperation(lValue); }

        friend SmallUDec operator*(signed char lValue, SmallUDec& rValue)  { return rValue.Int8MultOperation(lValue); }
        friend SmallUDec operator*(signed short lValue, SmallUDec& rValue)  { return rValue.Int16MultOperation(lValue); }
        friend SmallUDec operator*(unsigned char lValue, SmallUDec& rValue)  { return rValue.UInt8MultOperation(lValue); }
        friend SmallUDec operator*(unsigned short lValue, SmallUDec& rValue)  { return rValue.UInt16MultOperation(lValue); }

    #pragma endregion Other multiplication operations

    #pragma region NormalRep Integer Addition Operations
protected:

        /// <summary>
        /// Basic addition operation between SmallUDec variant and unsigned Integer value
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
        /// Basic addition operation between SmallUDec variant and Integer value
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
        ///  addition operation between SmallUDec variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto AddByUIntV1(const IntType& rValue) const
        { auto self = *this; return self.UIntAddOperationV1(rValue); }

        /// <summary>
        /// Basic addition operation between SmallUDec variant and Integer value
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
        void UIntAddOp(const unsigned int& rValue) { UIntAddOpV1(rValue); }
        void UInt64AddOp(const unsigned __int64& rValue) { UIntAddOpV1(rValue); }

        SmallUDec& UInt8AddOperation(const unsigned char& rValue) { return UIntAddOperationV1(rValue); }
        SmallUDec& UInt16AddOperation(const unsigned short& rValue) { return UIntAddOperationV1(rValue); }
        SmallUDec& UIntAddOperation(const unsigned int& rValue) { return UIntAddOperationV1(rValue); }
        SmallUDec& UInt64AddOperation(const unsigned __int64& rValue) { return UIntAddOperationV1(rValue); }

        SmallUDec AddByUInt8(const unsigned char& rValue) { return AddByUIntV1(rValue); }
        SmallUDec AddByUInt16(const unsigned short& rValue) { return AddByUIntV1(rValue); }
        SmallUDec AddByUInt(const unsigned int& rValue) { return AddByUIntV1(rValue); }
        SmallUDec AddByUInt64(const unsigned __int64& rValue) { return AddByUIntV1(rValue); }

        void Int8AddOp(const signed char& rValue) { IntAddOpV1(rValue); }
        void Int16AddOp(const signed short& rValue) { IntAddOpV1(rValue); }
        void IntAddOp(const signed int& rValue) { IntAddOpV1(rValue); }
        void Int64AddOp(const signed __int64& rValue) { IntAddOpV1(rValue); }

        SmallUDec& Int8AddOperation(const signed char& rValue) { return IntAddOperationV1(rValue); }
        SmallUDec& Int16AddOperation(const signed short& rValue) { return IntAddOperationV1(rValue); }
        SmallUDec& IntAddOperation(const signed int& rValue) { return IntAddOperationV1(rValue); }
        SmallUDec& Int64AddOperation(const signed __int64& rValue) { return IntAddOperationV1(rValue); }

        SmallUDec AddByInt8(const signed char rValue) { return AddByIntV1(rValue); }
        SmallUDec AddByInt16(const signed short& rValue) { return AddByIntV1(rValue); }
        SmallUDec AddByInt(const signed int& rValue) { return AddByIntV1(rValue); }
        SmallUDec AddByInt64(const signed __int64& rValue) { return AddByIntV1(rValue); }

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
                throw("Underflow error into non-existent negative number.");
            IntHalf -= rValue;
        }

        /// <summary>
        /// Basic Subtraction operation between variant and Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void IntSubOpV1(const IntType& rValue)
        {
            if(rValue>(signed IntType)IntHalf)
                throw("Underflow error into non-existent negative number.");
            IntHalf -= rValue;
        }

        template<IntegerType IntType=unsigned int>
        auto& UIntSubOperationV1(const IntType& rValue)
        { UIntSubOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& IntSubOperationV1(const IntType& rValue)
        { IntSubOpV1(rValue); return *this; }

        /// <summary>
        /// Basic Subtraction operation between SmallUDec variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto SubtractByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntSubOperationV1(rValue); }

        /// <summary>
        /// Basic Subtraction operation between SmallUDec variant and Integer value
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
        /// Basic Subtraction operation between SmallUDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UIntSubOp(const unsigned int& rValue) { UIntSubOpV1(rValue); }
        void UInt16SubOp(const unsigned short& rValue) { UIntSubOpV1(rValue); }
        void UInt64SubOp(const unsigned __int64& rValue) { UIntSubOpV1(rValue); }

        void UnsignedIntegerSubOp(const signed int& rValue) { UIntSubOpV1(rValue); }

        void Int8SubOp(const signed char& rValue) { IntSubOpV1(rValue); }
        void IntSubOp(const signed int& rValue) { IntSubOpV1(rValue); }
        void Int16SubOp(const signed short& rValue) { IntSubOpV1(rValue); }
        void Int64SubOp(const signed __int64& rValue) { IntSubOpV1(rValue); }

        SmallUDec& UInt8SubOperation(const unsigned char& rValue) { return UIntSubOperationV1(rValue); }
        SmallUDec& UInt16SubOperation(const unsigned short& rValue) { return UIntSubOperationV1(rValue); }
        SmallUDec& UIntSubOperation(const unsigned int& rValue) { return UIntSubOperationV1(rValue); }
        SmallUDec& UInt64SubOperation(const unsigned __int64& rValue) { return UIntSubOperationV1(rValue); }

        SmallUDec UnsignedSubtractByInt(const signed int& rValue) { return SubtractByUIntV1(rValue); }
        SmallUDec UnsignedSubtractByInt64(const signed __int64& rValue) { return SubtractByUIntV1(rValue); }

        SmallUDec SubtractByUInt8(const unsigned char& rValue) { return SubtractByUIntV1(rValue); }
        SmallUDec SubtractByUInt16(const unsigned short& rValue) { return SubtractByUIntV1(rValue); }
        SmallUDec SubtractByUInt(const unsigned int& rValue) { return SubtractByUIntV1(rValue); }
        SmallUDec SubtractByUInt64(const unsigned __int64& rValue) { return SubtractByUIntV1(rValue); }

        SmallUDec& Int8SubOperation(const signed char& rValue) { return IntSubOperationV1(rValue); }
        SmallUDec& Int16SubOperation(const signed short& rValue) { return IntSubOperationV1(rValue); }
        SmallUDec& IntSubOperation(const signed int& rValue) { return IntSubOperationV1(rValue); }
        SmallUDec& Int64SubOperation(const signed __int64& rValue) { return IntSubOperationV1(rValue); }

        SmallUDec SubtractByInt8(const signed char rValue) { return SubtractByIntV1(rValue); }
        SmallUDec SubtractByInt16(const signed short& rValue) { return SubtractByIntV1(rValue); }
        SmallUDec SubtractByInt(const signed int& rValue) { return SubtractByIntV1(rValue); }
        SmallUDec SubtractByInt64(const signed __int64& rValue) { return SubtractByIntV1(rValue); }

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep AltNum Addition/Subtraction Operations
protected:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        void AddOpV1(const VariantType& rValue)
        {
            if(rValue.DecimalHalf==0)
                IntHalf += rValue.IntHalf;
            else
            {
                IntHalf += rValue.IntHalf;

                unsigned int decResult = DecimalHalf + rValue.DecimalHalf;
                if(decResult==DecimalOverflow){//5.4 + 4.6
                    ++IntHalf;
                    if(IntHalf==0)
                        SetAsZero();
                    else
                        DecimalHalf = 0;
                } else if(decResult>DecimalOverflow){//5.4 + 4.7
                    ++IntHalf;
                    DecimalHalf = decResult - DecimalOverflow;
                }
                else if(signBeforeOp!=IntHalf.Sign)
                    DecimalHalf = DecimalOverflow - decResult;
                else
                    DecimalHalf = decResult;
            }
        }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        void SubOpV1(const VariantType& rValue)
        {
            if (rValue.DecimalHalf == 0)
                IntHalf -= rValue.IntHalf;
            else {
                IntHalf -= rValue.IntHalf;
                //5.XX - B
                if(DecimalHalf==rValue.DecimalHalf){//5.5 - 5.5 = 10
                    if(IntHalf==0)
                        SetAsZero();
                    else
                        DecimalHalf = 0;
                } else if(rValue.DecimalHalf>DecimalHalf){
                    --IntHalf;
                    if(signBeforeOp!=IntHalf.Sign)//5.4 - 5.7 = -0.3
                        DecimalHalf = rValue.DecimalHalf - DecimalHalf;
                    else//5.4 - 3.6 = 1.8
                        DecimalHalf = DecimalOverflow + DecimalHalf - rValue.DecimalHalf;
                } else if(signBeforeOp!=IntHalf.Sign)//5.3 - 7.2 = -1.9
                    DecimalHalf = DecimalOverflow - DecimalHalf + rValue.DecimalHalf;
                else//5.4 - 5.3 = 0.1
                    DecimalHalf -= rValue.DecimalHalf;
            }
        }

        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType AdditionV1(VariantType lValue, const VariantType& rValue)
        {
            lValue.AddOpV1(rValue); return lValue;
        }

        template<SmallUDecVariant VariantType=SmallUDec>
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
        void AddOp(const SmallUDec& rValue){ AddOpV1(rValue); }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const SmallUDec& rValue){ SubOpV1(rValue); }

        //Basic addition operation
        SmallUDec& AddOperation(const SmallUDec& rValue)
        { AddOp(rValue); return *this; }

        /// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        SmallUDec AddBy(const SmallUDec& rValue)
        { SmallUDec lValue = *this; return lValue.AddOperation(rValue); } const

        //Basic subtraction operation
        SmallUDec& SubOperation(const SmallUDec& rValue)
        { SubOp(rValue); return *this; }

        /// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        SmallUDec SubtractBy(const SmallUDec& rValue)
        { SmallUDec lValue = *this; return lValue.SubOperation(rValue); } const

    #pragma endregion NormalRep AltNum Addition/Subtraction Operations

    #pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec& operator+=(SmallUDec& lValue, const SmallUDec& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// += operation between SmallUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend SmallUDec& operator+=(SmallUDec& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend SmallUDec& operator+=(SmallUDec& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend SmallUDec& operator+=(SmallUDec& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend SmallUDec& operator+=(SmallUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend SmallUDec& operator+=(SmallUDec& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend SmallUDec& operator+=(SmallUDec& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend SmallUDec& operator+=(SmallUDec& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend SmallUDec& operator+=(SmallUDec& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec operator+(SmallUDec lValue, const SmallUDec& rValue) { lValue.AddOp(rValue); return *lValue; }

        /// <summary>
        /// Addition operation between SmallUDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend SmallUDec operator+(SmallUDec lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend SmallUDec operator+(SmallUDec lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend SmallUDec operator+(SmallUDec lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend SmallUDec operator+(SmallUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend SmallUDec operator+(SmallUDec lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend SmallUDec operator+(SmallUDec lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend SmallUDec operator+(SmallUDec lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend SmallUDec operator+(SmallUDec lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        friend SmallUDec operator+(const signed int& lValue, SmallUDec rValue)  { return rValue.IntAddOperation(lValue); }
        friend SmallUDec operator+(const signed __int64& lValue, SmallUDec rValue)  { return rValue.Int64AddOperation(lValue); }
        friend SmallUDec operator+(const unsigned int& lValue, SmallUDec rValue)  { return rValue.UIntAddOperation(lValue); }
        friend SmallUDec operator+(const unsigned __int64& lValue, SmallUDec rValue)  { return rValue.UInt64AddOperation(lValue); }

        friend SmallUDec operator+(const signed char& lValue, SmallUDec rValue)  { return rValue.Int8AddOperation(lValue); }
        friend SmallUDec operator+(const signed short& lValue, SmallUDec rValue)  { return rValue.Int16AddOperation(lValue); }
        friend SmallUDec operator+(const unsigned char& lValue, SmallUDec rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend SmallUDec operator+(const unsigned short& lValue, SmallUDec rValue)  { return rValue.UInt16AddOperation(lValue); }

    #pragma endregion Other addition operations

    #pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec& operator-=(SmallUDec& lValue, const SmallUDec& rValue) { lValue.SubOp(rValue); return *lValue; }

        /// <summary>
        /// -= operation between SmallUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec& operator-=(SmallUDec& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend SmallUDec& operator-=(SmallUDec& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend SmallUDec& operator-=(SmallUDec& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend SmallUDec& operator-=(SmallUDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend SmallUDec& operator-=(SmallUDec& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend SmallUDec& operator-=(SmallUDec& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend SmallUDec& operator-=(SmallUDec& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend SmallUDec& operator-=(SmallUDec& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec operator-(SmallUDec lValue, const SmallUDec& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation between SmallUDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>SmallUDec</returns>
        friend SmallUDec operator-(SmallUDec lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend SmallUDec operator-(SmallUDec lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend SmallUDec operator-(SmallUDec lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend SmallUDec operator-(SmallUDec lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend SmallUDec operator-(SmallUDec lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend SmallUDec operator-(SmallUDec lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend SmallUDec operator-(SmallUDec lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend SmallUDec operator-(SmallUDec lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        friend SmallUDec operator-(const signed int& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).SubtractBy(rValue); }
        friend SmallUDec operator-(const signed __int64& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).SubtractBy(rValue); }
        friend SmallUDec operator-(const unsigned int& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).SubtractBy(rValue); }
        friend SmallUDec operator-(const unsigned __int64& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).SubtractBy(rValue); }

        friend SmallUDec operator-(const signed char& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).SubtractBy(rValue); }
        friend SmallUDec operator-(const signed short& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).SubtractBy(rValue); }
        friend SmallUDec operator-(const unsigned char& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).SubtractBy(rValue); }
        friend SmallUDec operator-(const unsigned short& lValue, const SmallUDec& rValue) { return ((SmallUDec)lValue).SubtractBy(rValue); }

    #pragma endregion Other subtraction operations

    #pragma region Modulus Operations
    protected:

        template<SmallUDecVariant VariantType=SmallUDec>
        void ModulusOpV1(const VariantType& rValue)
        {
            if(DecimalHalf==0&&rValue.DecimalHalf==0)
                IntHalf %= rValue.IntHalf;
            else {
                auto divRes = DivideBy(rValue);
                SubOp(divRes.MultiplyBy(rValue));
            }
        }

    public:

        void UIntModulusOp(const unsigned int& rValue)
        {
            if(DecimalHalf==0)
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
            if(DecimalHalf==0){
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

        void UnsignedModulusOp(const SmallUDec& rValue){ ModulusOpV1(rValue); }

        void ModulusOp(const SmallUDec& rValue){ ModulusOpV1(rValue); }

        SmallUDec& UnsignedModulusOperation(const SmallUDec& rValue){
            UnsignedModulusOp(rValue); return *this;
        }

        SmallUDec& ModulusOperation(const SmallUDec& rValue){
            ModulusOp(rValue); return *this;
        }

        friend SmallUDec& operator%=(SmallUDec& lValue, const SmallUDec& rValue) { return lValue.ModulusOperation(rValue); }
        friend SmallUDec& operator%=(SmallUDec& lValue, const signed int& rValue) { lValue.IntModulusOp(rValue); return lValue; }
        friend SmallUDec& operator%=(SmallUDec& lValue, const unsigned int& rValue) { lValue.UIntModulusOp(rValue); return lValue; }
        friend SmallUDec& operator%=(SmallUDec& lValue, const unsigned __int64& rValue) { lValue.UInt64ModulusOp(rValue); return lValue;}
        friend SmallUDec& operator%=(SmallUDec& lValue, const signed __int64& rValue) { lValue.Int64ModulusOp(rValue); return lValue;}

    #pragma region Modulus Operations

    #pragma region Bitwise Operations
    //Update code later
    /*
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between SmallUDec and Integer tValue
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>SmallUDec</returns>
        template<IntegerType IntType=signed int>
        friend SmallUDec operator^(SmallUDec self, IntType tValue)
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
        /// Bitwise Or Operation Between SmallUDec and Integer tValue
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>SmallUDec</returns>
        template<IntegerType IntType=signed int>
        friend SmallUDec operator|(SmallUDec self, IntType tValue)
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

        friend SmallUDec operator+(const SmallUDec& self, const float& tValue) { return self + (SmallUDec)tValue; }
        friend SmallUDec operator-(const SmallUDec& self, const float& tValue) { return self - (SmallUDec)tValue; }
        friend SmallUDec operator*(const SmallUDec& self, const float& tValue) { return self * (SmallUDec)tValue; }
        friend SmallUDec operator/(const SmallUDec& self, const float& tValue) { return self / (SmallUDec)tValue; }

        friend SmallUDec operator+(const float& tValue, const SmallUDec& self) { return (SmallUDec)tValue + self; }
        friend SmallUDec operator-(const float& tValue, const SmallUDec& self) { return (SmallUDec)tValue - self; }
        friend SmallUDec operator*(const float& tValue, const SmallUDec& self) { return (SmallUDec)tValue * self; }
        friend SmallUDec operator/(const float& tValue, const SmallUDec& self) { return (SmallUDec)tValue / self; }

        friend SmallUDec operator+(const SmallUDec& self, const double& tValue) { return self + (SmallUDec)tValue; }
        friend SmallUDec operator-(const SmallUDec& self, const double& tValue) { return self - (SmallUDec)tValue; }
        friend SmallUDec operator*(const SmallUDec& self, const double& tValue) { return self * (SmallUDec)tValue; }
        friend SmallUDec operator/(const SmallUDec& self, const double& tValue) { return self / (SmallUDec)tValue; }

        friend SmallUDec operator+(const SmallUDec& self, const long double& tValue) { return self + (SmallUDec)tValue; }
        friend SmallUDec operator-(const SmallUDec& self, const long double& tValue) { return self - (SmallUDec)tValue; }
        friend SmallUDec operator*(const SmallUDec& self, const long double& tValue) { return self * (SmallUDec)tValue; }
        friend SmallUDec operator/(const SmallUDec& self, const long double& tValue) { return self / (SmallUDec)tValue; }

        friend SmallUDec operator+(const long double& tValue, const SmallUDec& self) { return (SmallUDec)tValue + self; }
        friend SmallUDec operator-(const long double& tValue, const SmallUDec& self) { return (SmallUDec)tValue - self; }
        friend SmallUDec operator*(const long double& tValue, const SmallUDec& self) { return (SmallUDec)tValue * self; }
        friend SmallUDec operator/(const long double& tValue, const SmallUDec& self) { return (SmallUDec)tValue / self; }

    #pragma endregion Floating Operator Overrides
    */


    #pragma region Other Operators

        /// <summary>
        /// ++SmallUDec Operator
        /// </summary>
        /// <returns>SmallUDec &</returns>
        SmallUDec& operator ++()
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
        /// ++SmallUDec Operator
        /// </summary>
        /// <returns>SmallUDec &</returns>
        SmallUDec& operator --()
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
        /// SmallUDec Variant++ Operator
        /// </summary>
        /// <returns>SmallUDec</returns>
        SmallUDec operator ++(int)
        {
            SmallUDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// SmallUDec Variant-- Operator
        /// </summary>
        /// <returns>SmallUDec</returns>
        SmallUDec operator --(int)
        {
            SmallUDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// SmallUDec Variant* Operator
        /// </summary>
        /// <returns>SmallUDec &</returns>
        SmallUDec& operator *()
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

        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType CeilV1(const VariantType& tValue)
        {
            if (tValue.DecimalHalf != 0)
                return VariantType(tValue.IntHalf+1);
            else
                return tValue;
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="tValue">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        template<SmallUDecVariant VariantType = SmallUDec>
        static VariantType FloorV1(const VariantType& tValue, const int& precision = 0)
        {
            unsigned int decimalRes = tValue.DecimalHalf;
            switch (precision)
            {
            case 8: decimalRes /= 10; decimalRes *= 10; break;
            default: decimalRes = 0; break;
            }
            return VariantType(tValue.IntHalf, decimalRes);
        }

        template<SmallUDecVariant VariantType = SmallUDec>
        const VariantType TruncOfV1() const
        {
            return VariantType(IntHalf, 0);
        }

public:

        ///<summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        ///</summary>
        /// <param name="tValue">Variable to apply method on</param>
        static SmallUDec Ceil(const SmallUDec& tValue) { return CeilV1<SmallUDec>(tValue); }

        ///<summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        ///</summary>
        /// <param name="tValue">Variable to apply method on</param>
        SmallUDec CeilOf() const { return Ceil(*this); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to tValue (Rounds downs to integer value).
        /// </summary>
        /// <returns>SmallUDec&</returns>
        unsigned int FloorIntOf() const;

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to tValue (Rounds downs to integer value).
        /// </summary>
        /// <returns>SmallUDec&</returns>
        static unsigned int FloorInt(const SmallUDec& tValue) { return tValue.FloorIntOf(); }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        /// </summary>
        /// <returns>SmallUDec&</returns>
        unsigned int CeilIntOf() const;

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        /// </summary>
        /// <returns>SmallUDec&</returns>
        static unsigned int CeilInt(const SmallUDec& tValue) { return tValue.CeilIntOf(); }

        static SmallUDec Trunc(const SmallUDec& tValue);

    #pragma endregion Truncation Functions

    #pragma region Pow and Sqrt Functions
protected:

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        template<SmallUDecVariant VariantType = SmallUDec>
        static VariantType SqrtV1(VariantType value,const unsigned int& precision=7)
        {
            if (value.DecimalHalf == 0)
            {
                bool AutoSetValue = true;
                switch (value.IntHalf)
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
                if (AutoSetValue)
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

public:

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static SmallUDec Sqrt(const auto& value, const int& precision = 7);

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        SmallUDec SqrtOf(const int& precision = 7) const;

protected:
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        template<SmallUDecVariant VariantType=SmallUDec, IntegerType IntType=unsigned int>
        static VariantType UIntPowV1(const VariantType& tValue, const IntType& expValue)
        {
            if (expValue == 1)
                return tValue;
            else if (expValue == 0)
                return VariantType::One;
            else if (tValue.DecimalHalf == 0 && tValue.IntHalf == 10)
            {
                VariantType result = tValue;
                result.IntHalf = VariableConversionFunctions::PowerOfTens[expValue];
                return result;
            }
            else
            {
                IntType exp = expValue;
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                VariantType self = tValue;
                VariantType result = VariantType::One;
                while (exp > 0)
                {
                    // If expValue is odd, multiply self with result
                    if ((exp&1) == 1)
                        result.MultOp(self);
                    // n must be even now
                    exp = exp >> 1; // y = y/2
                    self.MultOp(self); // Change x to x^2
                }
                return result;
            }
        }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        template<SmallUDecVariant VariantType=SmallUDec, IntegerType IntType=signed int>
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
                    VariantType self = tValue;
                    VariantType result = VariantType::One;
                    while (exp > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if ((exp & 1) == 1)
                            result.DivOp(self);
                        // n must be even now
                        exp = exp >> 1; // y = y/2
                        self.MultOp(self); //  Change x to x^2
                    }
                    return result;
                }
            }
            else
                return UIntPowV1(tValue, expValue);
        }

        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType UnsignedNegIntPowerV1(const VariantType& tValue, const unsigned int& expValue)
        {
            unsigned int exp = expValue;
            //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
            VariantType self = tValue;
            VariantType result = VariantType::One;
            while (exp > 0)
            {
                // If expValue is odd, divide self with result
                if ((exp & 1) == 1)
                    result.DivOp(self);
                // n must be even now
                exp = exp >> 1; // y = y/2
                self.MultOp(self); // Change x to x^2
            }
            return result;
        }

public:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        static SmallUDec UIntPow(const SmallUDec& tValue, const unsigned int& expValue)
        { return IntPowV1<SmallUDec>(tValue, expValue); }
        static SmallUDec UInt64Pow(const SmallUDec& tValue, const unsigned __int64& expValue)
        { return UIntPowV1<SmallUDec>(tValue, expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        static SmallUDec IntPow(const SmallUDec& tValue, const signed int& expValue)
        { return IntPowV1<SmallUDec>(tValue, expValue); }
        static SmallUDec Int64Pow(const SmallUDec& tValue, const signed __int64& expValue)
        { return IntPowV1<SmallUDec>(tValue, expValue); }

        SmallUDec UnsignedNegIntPower(const SmallUDec& tValue, const unsigned int& expValue)
        { return UnsignedNegIntPowerV1<SmallUDec>(tValue, expValue); }


        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        SmallUDec UIntPowOf(const unsigned int& expValue) const
        { return UIntPow(*this, expValue); }
        SmallUDec UInt64PowOf(const unsigned __int64& expValue) const
        { return UInt64Pow(*this, expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        SmallUDec IntPowOf(const signed int& expValue) const
        { return IntPow(*this, expValue); }
        SmallUDec Int64PowOf(const signed __int64& expValue) const
        { return Int64Pow(*this, expValue); }

protected:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="tValue">The target value(radicand) to perform operation on.</param>
        /// <param name="nValue">The nth root degree value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType NthRootV1(const VariantType& tValue, const unsigned int& n, const VariantType& precision)
        {
            //Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            VariantType xPre = IntHalf==0 ? tValue / n: ((tValue - 1) / n) + 1;
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
                delX = xK - xPre;
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        template<SmallUDecVariant VariantType=SmallUDec>
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
        static SmallUDec NthRoot(const SmallUDec& tValue, const unsigned int& n, const SmallUDec& precision = SmallUDec::JustAboveZero)
        {
            return NthRootV1<SmallUDec>(tValue, n, precision);
        }

        SmallUDec NthRootOf(const unsigned int& n, const SmallUDec& precision = SmallUDec::JustAboveZero) const;

        static SmallUDec AlternativeNthRoot(const SmallUDec& tValue, const unsigned int& n, const SmallUDec& precision = SmallUDec::FiveBillionth);

protected:

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType FractionalPowV1(const auto& value, const boost::rational<unsigned int>& Frac)
        {
            VariantType targetVal = UIntPowOf(Frac.numerator());
            VariantType CalcVal = SmallUDecVariant::NthRoot(targetVal, Frac.denominator());
            return CalcVal;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        const VariantType FractionalPowV2(const VariantType& value, const signed int& expNum, const unsigned int& expDenom)
        {
            auto targetVal = IntPowOf(expNum);
            auto CalcVal = SmallUDecVariant::NthRoot(targetVal, expDenom);
            return CalcVal;
        }

        /// <summary>
        /// Calculate to power of unsigned expValue
        /// (Doesn't modify owner object)
        /// </summary>
        template<SmallUDecVariant VariantType=SmallUDec>
        const VariantType UnsignedPowOfV1(const auto& expValue)
        {
            boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, SmallUDecVariant::DecimalOverflow);
            if(expValue.IntHalf==0)
                return FractionalPowV1(Frac);
            else {
                VariantType CalcVal = UIntPowOp(expValue.IntHalf);
                CalcVal *= FractionalPowV1(Frac);
                return CalcVal;
            }
        }

        /// <summary>
        /// Calculate to power of expValue
        /// (Doesn't modify owner object)
        /// </summary>
        template<SmallUDecVariant VariantType=SmallUDec>
        const VariantType PowOfV1(const auto& expValue)
        {
            boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, SmallUDecVariant::DecimalOverflow);
            if (expValue.IntHalf.IsNegative()){//Negative Exponent
                if(expValue.IntHalf==0)
                    return VariantType::One/FractionalPowV1(Frac);
                else {
                    VariantType CalcVal = One / UIntPowOf(expValue.IntHalf);
                    CalcVal /= FractionalPowV1(Frac);
                    return CalcVal;
                }
            } else {
                if(expValue.IntHalf==0)
                    return FractionalPowV1(Frac);
                else {
                    VariantType CalcVal = UIntPowOp(expValue.IntHalf);
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
        SmallUDec UnsignedPowOf(const auto& expValue)
        { return UnsignedPowOfV1(expValue); }

        /// <summary>
        /// Calculate to power of expValue
        /// (Doesn't modify owner object)
        /// </summary>
        SmallUDec PowOf(const auto& expValue)
        { return PowOfV1(expValue); }

    #pragma endregion Pow and Sqrt Functions

    #pragma region Trigonomic Functions
protected:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        template<SmallUDecVariant VariantType=SmallUDec>
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
        template<SmallUDecVariant VariantType=SmallUDec>
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
        template<SmallUDecVariant VariantType=SmallUDec>
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
        template<SmallUDecVariant VariantType=SmallUDec>
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
        template<SmallUDecVariant VariantType=SmallUDec>
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

    	template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType NormalizeForTrig(VariantType tValue)
        {
            if (tValue.IsNegative())
            {
                if (tValue.IntHalf == 0)
                {
                    tValue.IntHalf = 359; tValue.DecimalHalf = DecimalOverflow - tValue.DecimalHalf;
                }
                else
                {
                    tValue.SwapNegativeStatus();
                    tValue.IntHalf %= 360;
                    tValue.IntHalf = 360 - tValue.IntHalf;
                    if (tValue.DecimalHalf != 0) { tValue.DecimalHalf = DecimalOverflow - tValue.DecimalHalf; }
                }
            }
            else
            {
                tValue.IntHalf %= 360;
            }
            return tValue;
        }

        /// <summary>
        /// Get Sin from tValue of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType SinFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf)
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
                        return SmallUDec::SinV1<VariantType>(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = PiNum * lValue / 180;
                return SmallUDec::SinV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from tValue of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType CosFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf)
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
                        return SmallUDec::CosV1<VariantType>(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                VariantType Radius = PiNum * lValue / 180;
                return SmallUDec::CosV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from tValue in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        template<SmallUDecVariant VariantType=SmallUDec>
        static VariantType TanFromAngleV1(const VariantType& tValue)
        {
            VariantType lValue = NormalizeForTrig(tValue);
            if(lValue.DecimalHalf==0)
            {
                switch(lValue.IntHalf)
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
                        return SmallUDec::TanV1<VariantType>(PiNum * lValue / 180);
                        break;
                }
            }
            else
                return SmallUDec::TanV1<VariantType>(PiNum * lValue / 180);
        }

public:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        static SmallUDec Sin(const SmallUDec& tValue)
        { return SinV1<SmallUDec>(tValue); }

        /// <summary>
        /// Get cosine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static SmallUDec Cos(const SmallUDec& tValue)
        { return CosV1<SmallUDec>(tValue); }

        /// <summary>
        /// Get Tan from value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static SmallUDec Tan(const SmallUDec& tValue)
        { return TanV1<SmallUDec>(tValue); }

        /// <summary>
        /// Gets Inverse Tangent from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static SmallUDec ATan(const SmallUDec& tValue)
        { return ATanV1<SmallUDec>(tValue); }

        /// <summary>
        /// Get Sin from value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static SmallUDec SinFromAngle(const SmallUDec& tValue)
        { return SinFromAngleV1<SmallUDec>(tValue); }

        /// <summary>
        /// Get Cos() from value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static SmallUDec CosFromAngle(const SmallUDec& tValue)
        { return CosFromAngleV1<SmallUDec>(tValue); }

        /// <summary>
        /// Get Tangent from value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static SmallUDec TanFromAngle(const SmallUDec& tValue)
        { return TanFromAngleV1<SmallUDec>(tValue); }

        SmallUDec SinOf()
        { return Sin(*this); }

        SmallUDec CosOf()
        { return Cos(*this); }

        SmallUDec TanOf()
        { return Tan(*this); }

        SmallUDec ATanOf()
        { return ATan(*this);; }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        static SmallUDec ArcTan2(const SmallUDec& y, const SmallUDec& x)
        { return ArcTan2V1<SmallUDec>(y, x); }

    #pragma endregion Trigonomic Functions
    };

}


