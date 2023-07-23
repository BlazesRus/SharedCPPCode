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

#include <string>
#include <cmath>
#include "..\OtherFunctions\VariableConversionFunctions.h"

#include <boost/rational.hpp>//Requires boost to reduce fractional
#if defined(AltNum_UseOldDivisionCode)
#include <boost/multiprecision/cpp_int.hpp>
#endif

#if !defined(AltNum_UseLegacyIntValueType)
	#include "MirroredInt.hpp"
#endif
//Preprocessor options
//RunAsC = Run code as c code instead of cpp(not implimented)
/*
*/

namespace BlazesRusCode
{
    class MediumDec;

/*---Accuracy Tests:
 * 100% accuracy for all integer value multiplication operations.
 * 100% accuracy for addition/subtraction operations
 * Partial but still high accuracy for non-integer representation variations of multiplication because of truncation
   (values get lost if get too small)
 * Partial but still high accuracy for division because of truncation
   (values get lost if get too small)
 * Other operations like Ln and Sqrt contained with decent level of accuracy
   (still loses a little accuracy because of truncation etc)
 * Operations and functions will mess up if IntValue overflows/underflows
*/

    /// <summary>
    /// Fixed point based number class representing +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// (values will be lost past 9th decimal digit)
	/// (Use MediumDecV2, ExtendedMDec, AltDecV2, or AltDec instead for infinity, Pi, and e features)
    /// (8 bytes worth of Variable Storage inside class for each instance)
    /// </summary>
    class DLL_API MediumDec
    {
#undefine MediumDecVariant
#define MediumDecVariant MediumDec
    public:
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;
    public:
        /// <summary>
        /// Value when IntValue is at negative zero (when has decimal part)
        /// </summary>
    #if defined(AltNum_UseLegacyGetIntHalf()Type)
        static signed int const NegativeRep = -2147483648;
    #else
        static signed int const NegativeRep = MirroredInt::NegativeRep;
    #endif

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
        /// </summary>
    #if defined(AltNum_UseLegacyIntValueType)
        signed int IntValue;
    #else
        MirroredInt IntValue;

        signed int GetIntHalf()
        {
            return IntValue.GetValue();
        }

        bool int IsNegative()
        {
            return IntValue.IsNegative();
        }
    #endif

        /// <summary>
        /// Stores decimal section info
        /// </summary>
        signed int DecimalHalf;

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="intVal">The int value.</param>
        /// <param name="decVal01">The decimal val01.</param>
        /// <param name="decVal02">The decimal val02.</param>
        MediumDec(signed int intVal = 0, signed int decVal01 = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal01;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetVal(MediumDecVariant Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }
        
        virtual void SetAsZero()
        {
            IntValue = 0; DecimalHalf = 0;
        }

    #pragma region ValueDefines
    private:
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant ZeroValue()
        {
            MediumDecVariant NewSelf = MediumDec();
            return NewSelf;
        }
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant OneValue()
        {
            MediumDecVariant NewSelf = MediumDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant TwoValue()
        {
            MediumDecVariant NewSelf = MediumDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant NegativeOneValue()
        {
            MediumDecVariant NewSelf = MediumDec(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Point5Value()
        {
            MediumDecVariant NewSelf = MediumDec(0, 500000000);
            return NewSelf;
        }

        static MediumDecVariant JustAboveZeroValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 1);
            return NewSelf;
        }

        static MediumDecVariant OneMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 1000);
            return NewSelf;
        }

        static MediumDecVariant FiveThousandthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 5000000);
            return NewSelf;
        }

        static MediumDecVariant FiveMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 5000);
            return NewSelf;
        }

        static MediumDecVariant TenMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 100);
            return NewSelf;
        }

        static MediumDecVariant OneHundredMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 10);
            return NewSelf;
        }

        static MediumDecVariant FiveBillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 5);
            return NewSelf;
        }

        /// <summary>
        /// Returns PI(3.1415926535897932384626433) with tenth digit rounded up(3.141592654)
        /// (about 1.000000000448883 times actual value of PI with digits past 15th cut off)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant PIValue()
        {
            MediumDecVariant NewSelf = MediumDec(3, 141592654);
            return NewSelf;
        }

        /// <summary>
        /// Returns value of -2147483647.999999999
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant MinimumValue()
        {
            return MediumDec(-2147483647, 999999999);
        }

        /// <summary>
        /// Returns value of 2147483647.999999999
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant MaximumValue()
        {
            return MediumDec(2147483647, 999999999);
        }

        /// <summary>
        /// Euler's number
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant EValue()
        {
            return MediumDec(2, 718281828);
        }

        static MediumDecVariant LN10Value()
        {
            return MediumDec(2, 302585093);
        }

        static MediumDecVariant LN10MultValue()
        {
            return MediumDec(0, 434294482);
        }

        static MediumDecVariant HalfLN10MultValue()
        {
            return MediumDec(0, 868588964);
        }

        static MediumDecVariant NilValue()
        {
            return MediumDec(-2147483648, -2147483648);
        }
    public:
        static MediumDecVariant PI;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Zero;

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant OneGMillionth;

        //0e-7
        static MediumDecVariant TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDecVariant FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant NegativeOne;

        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// </summary>
        static MediumDecVariant Maximum;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// </summary>
        static MediumDecVariant Minimum;

        /// <summary>
        /// Euler's number
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant E;

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDecVariant LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecVariant LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecVariant HalfLN10Mult;

        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static MediumDecVariant Nil;
    #pragma endregion ValueDefines
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
            if(DecimalHalf==0)
                IntValue.Value *= -1;
            else
                IntValue = -IntValue;
        }
    #pragma region String Commands
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(std::string Value);

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        MediumDecVariant GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDec(const char* strVal)
        {
            std::string Value = strVal;
            if (Value == "PI")
            {
                this->SetVal(PI);
            }
            else if (Value == "E")
            {
                this->SetVal(E);
            }
            else
            {
                this->ReadString(Value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(std::string Value)
        {
            if (Value == "PI")
            {
                this->SetVal(PI);
            }
            else if (Value == "E")
            {
                this->SetVal(E);
            }
            else
            {
                this->ReadString(Value);
            }
        }
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

        ///// <summary>
        ///// += MediumDecVariant operation added to std::string
        ///// </summary>
        ///// <param name="targetString">The target string(left side).</param>
        ///// <param name="Value">The right side value</param>
        ///// <returns>string</returns>
        //friend std::string operator+=(std::string targetString, MediumDecVariant self) { return targetString + self.ToString(); }
    #pragma endregion String Commands

    #pragma region From Standard types to this type
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(float Value)
        {
            bool IsNegative = Value < 0.0f;
            if (IsNegative) { Value *= -1.0f; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0f)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (float)WholeValue;
                IntValue = IsNegative ? WholeValue * -1 : WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(double Value)
        {
            bool IsNegative = Value < 0.0;
            if (IsNegative) { Value *= -1.0; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (double)WholeValue;
                IntValue = IsNegative ? WholeValue * -1 : WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(ldouble Value)
        {
            bool IsNegative = Value < 0.0L;
            if (IsNegative) { Value *= -1.0L; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0L)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (ldouble)WholeValue;
                IntValue = IsNegative ? WholeValue * -1 : WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
            }
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(bool Value)
        {
            IntValue = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(float Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(double Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(ldouble Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDec(bool Value)
        {
            this->SetVal(Value);
        }
    #pragma endregion From Standard types to this type

    #pragma region From this type to Standard types
        /// <summary>
        /// MediumDecVariant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float()
        {
            float Value;
            if (IsNegative())
            {
                Value = IntValue == NegativeRep ? 0.0f : (float)GetIntHalf();
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
                Value = (float)GetIntHalf();
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
        }
        /// <summary>
        /// MediumDecVariant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double()
        {
            double Value;
            if (IsNegative())
            {
                Value = IntValue == NegativeRep ? 0.0 : (double)GetIntHalf();
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
                Value = (double)GetIntHalf();
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
        }

        /// <summary>
        /// MediumDecVariant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble()
        {
            ldouble Value;
            if (IsNegative())
            {
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)GetIntHalf();
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
                Value = (ldouble)GetIntHalf();
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
            }
            return Value;
        }

        /// <summary>
        /// MediumDecVariant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return GetIntHalf(); }

        explicit operator bool() { return IntValue == 0 ? false : true; }

        /// <summary>
        /// MediumDecVariant to integer type explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        template<typename IntType>
        explicit operator IntType() { return GetIntHalf(); }
    #pragma endregion From this type to Standard types

    #pragma region Comparison Operators
        /// <summary>
        /// Equal to Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator==(MediumDecVariant self, MediumDecVariant Value)
        {
            return (self.IntValue.Value == Value.IntValue.Value && self.DecimalHalf == Value.DecimalHalf);
        }

        /// <summary>
        /// Not equal to Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator!=(MediumDecVariant self, MediumDecVariant Value)
        {
            return (self.IntValue.Value != Value.IntValue.Value || self.DecimalHalf != Value.DecimalHalf);
        }

        /// <summary>
        /// Lesser than Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<(MediumDecVariant self, MediumDecVariant Value)
        {
            if(self.DecimalHalf==0)
            {
                if(Value.DecimalHalf==0)
                    return self.IntValue<Value.IntValue;
                else
                {
                    if(self.IntValue<Value.IntValue)
                        return self.DecimalHalf<Value.DecimalHalf;
                    else
                        return false;
                }
            }
            else if(self.IntValue<Value.IntValue)
                return self.DecimalHalf<Value.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Lesser than or Equal to Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<=(MediumDecVariant self, MediumDecVariant Value)
        {
            if(self.DecimalHalf==0)
            {
                if(Value.DecimalHalf==0)
                    return self.IntValue<=Value.IntValue;
                else
                {
                    if(self.IntValue<=Value.IntValue)
                        return self.DecimalHalf<=Value.DecimalHalf;
                    else
                        return false;
                }
            }
            else if(self.IntValue<=Value.IntValue)
                return self.DecimalHalf<=Value.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Greater than Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>bool</returns>
        friend bool operator>(MediumDecVariant self, MediumDecVariant Value)
        {
            if(self.DecimalHalf==0)
            {
                if(Value.DecimalHalf==0)
                    return self.IntValue>Value.IntValue;
                else
                {
                    if(self.IntValue>Value.IntValue)
                        return self.DecimalHalf>Value.DecimalHalf;
                    else
                        return false;
                }
            }
            else if(self.IntValue>Value.IntValue)
                return self.DecimalHalf>Value.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Greater than or Equal to Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>=(MediumDecVariant self, MediumDecVariant Value)
        {
            if(self.DecimalHalf==0)
            {
                if(Value.DecimalHalf==0)
                    return self.IntValue>=Value.IntValue;
                else
                {
                    if(self.IntValue>=Value.IntValue)
                        return self.DecimalHalf>=Value.DecimalHalf;
                    else
                        return false;
                }
            }
            else if(self.IntValue>=Value.IntValue)
                return self.DecimalHalf>=Value.DecimalHalf;
            else
                return false;
        }

    #pragma endregion Comparison Operators
    #pragma region MediumDec-To-MediumDecVariant Operators
    public:
        /// <summary>
        /// Addition Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant& AddOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
#if defined(AltNum_UseLegacyIntValueType)
            if (Value.DecimalHalf == 0)
            {
                if (Value.IntValue == 0)//(Value == Zero)
                    return self;
                if (self.DecimalHalf == 0)
                {
                    self.IntValue += Value.IntValue;
                }
                else
                {
                    bool WasNegative = self.IntValue < 0;
                    if (WasNegative)
                        self.IntValue = self.IntValue == MediumDecVariant::NegativeRep ? -1 : --self.IntValue;
                    self.IntValue += Value.IntValue;
                    if (self.IntValue == -1)
                        self.IntValue = self.DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                    else if (self.IntValue < 0)
                        ++self.IntValue;
                    //If flips to other side of negative, invert the decimals
                    if ((WasNegative && self.IntValue >= 0) || (WasNegative == 0 && self.IntValue < 0))
                        self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
                }
            }
            else
            {
                bool WasNegative = self.IntValue < 0;
                //Deal with Int section first
                if (WasNegative)
                    self.IntValue = self.IntValue == MediumDecVariant::NegativeRep ? -1 : --self.IntValue;
                if (Value.IntValue != 0 && Value.IntValue != MediumDecVariant::NegativeRep)
                    self.IntValue += Value.IntValue;
                //Now deal with the decimal section
                if (Value.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        self.DecimalHalf += Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; --self.IntValue; }
                    }
                    else
                    {
                        self.DecimalHalf -= Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; --self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        self.DecimalHalf -= Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; --self.IntValue; }
                    }
                    else
                    {
                        self.DecimalHalf += Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; --self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                    }
                }
                if (self.IntValue == -1)
                    self.IntValue = self.DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                else if (self.IntValue < 0)
                    ++self.IntValue;
                //If flips to other side of negative, invert the decimals
                if ((WasNegative && self.IntValue >= 0) || (WasNegative == 0 && self.IntValue < 0))
                    self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
            }
#else
            bool NegativeBeforeOperation = self.IntValue < 0;
            //Deal with Int section first
            self.IntValue.Value += Value.IntValue.Value;
            if (Value.DecimalHalf != 0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        self.DecimalHalf += Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; --self.IntValue; }
                    }
                    else
                    {
                        self.DecimalHalf -= Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; --self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        self.DecimalHalf -= Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; --self.IntValue; }
                    }
                    else
                    {
                        self.DecimalHalf += Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; --self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(NegativeBeforeOperation^(self.IntValue<0))
                self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
#endif
            return self;
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& SubOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
#if defined(AltNum_UseLegacyIntValueType)
            if (Value.DecimalHalf == 0)
            {
                if (Value.IntValue == 0)//(Value == Zero)
                    return self;
                if (self.DecimalHalf == 0)
                {
                    self.IntValue -= Value.IntValue;
                }
                else
                {
                    bool WasNegative = self.IntValue < 0;
                    if (WasNegative)
                        self.IntValue = self.IntValue == MediumDecVariant::NegativeRep ? -1 : --self.IntValue;
                    if (Value.IntValue != 0)
                        self.IntValue -= Value.IntValue;
                    if(self.IntValue==-1)
                        self.IntValue = self.DecimalHalf == 0?0:MediumDecVariant::NegativeRep;
                    else if(self.IntValue<0)
                        ++self.IntValue;
                    //If flips to other side of negative, invert the decimals
                    if ((WasNegative && self.IntValue >= 0)||(WasNegative == 0 && self.IntValue < 0))
                        self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
                }
            }
            else
            {
                bool WasNegative = self.IntValue < 0;
                //Deal with Int section first
                if (WasNegative)
                    self.IntValue = self.IntValue == MediumDecVariant::NegativeRep ? -1 : --self.IntValue;
                if(Value.IntValue!=0&&Value.IntValue!=MediumDecVariant::NegativeRep)
                    self.IntValue -= Value.IntValue;
                //Now deal with the decimal section
                if (Value.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        self.DecimalHalf -= Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; --self.IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        self.DecimalHalf += Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; --self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        self.DecimalHalf += Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; --self.IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        self.DecimalHalf -= Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; --self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                    }
                }
                if (self.IntValue == -1)
                    self.IntValue = self.DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                else if (self.IntValue < 0)
                    ++self.IntValue;
                //If flips to other side of negative, invert the decimals
                if ((WasNegative && self.IntValue >= 0) || (WasNegative == 0 && self.IntValue < 0))
                    self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
            }
#else
            bool NegativeBeforeOperation = self.IntValue < 0;
            //Deal with Int section first
            self.IntValue.Value -= Value.IntValue.Value;
            //Now deal with the decimal section
            if(Value.DecimalHalf!=0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        self.DecimalHalf -= Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; --self.IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        self.DecimalHalf += Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; --self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        self.DecimalHalf += Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; --self.IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        self.DecimalHalf -= Value.DecimalHalf;
                        if (self.DecimalHalf < 0) { self.DecimalHalf += MediumDecVariant::DecimalOverflow; --self.IntValue; }
                        else if (self.DecimalHalf >= MediumDecVariant::DecimalOverflow) { self.DecimalHalf -= MediumDecVariant::DecimalOverflow; ++self.IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(NegativeBeforeOperation^(self.IntValue<0))
                self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
#endif
            return self;
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& MultOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
            if (Value == MediumDecVariant::Zero) { self.IntValue = 0; self.DecimalHalf = 0; return self; }
            if (self == MediumDecVariant::Zero || Value == MediumDecVariant::One)
                return self;
            if (Value.IsNegative())
            {
                Value.SwapNegativeStatus();
                self.SwapNegativeStatus();
            }
            if (self.DecimalHalf == 0)
            {
                if (self.IntValue == 1) { self = Value; return self; }
                else if (Value.DecimalHalf == 0)
                {
                    self.IntValue.Value *= Value.GetIntHalf(); return self;
                }
                else
                {
                    Value *= self.GetIntHalf();
                    self = Value;
                }
            }
            else if (Value.DecimalHalf == 0)//Y is integer
            {
                __int64 SRep;
                if(self.IntValue.IsZero())
                {
                    //0.Y * 0.V == .Y * .V
                    SRep = self.DecimalHalf * Value.DecimalHalf;
                    SRep /= MediumDecVariant::DecimalOverflowX;
                }
                else
                {
                    SRep = MediumDecVariant::DecimalOverflowX * self.IntValue.Value + self.DecimalHalf;
                    SRep *= Value.IntValue.Value;
                }
                if (SRep >= MediumDecVariant::DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                    SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                    self.IntValue = (signed int)OverflowVal;
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = 0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            else if (self.IntValue == 0)
            {
                __int64 SRep = (__int64)self.DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= MediumDecVariant::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    SRep += (__int64)self.DecimalHalf * Value.GetIntHalf();
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        self.IntValue = OverflowVal;
                        self.DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        self.DecimalHalf = (signed int)SRep;
                    }
                }
            }
            else if (self.IntValue == MediumDecVariant::NegativeRep)
            {
                __int64 SRep = (__int64)self.DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= MediumDecVariant::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    SRep += (__int64)self.DecimalHalf * Value.GetIntHalf();
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        self.IntValue = -OverflowVal;
                        self.DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        self.DecimalHalf = (signed int)SRep;
                    }
                }
            }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                {
                    self.IntValue.Value *= -1;
                }
                if (Value.IntValue.Value == 0)
                {
                    __int64 SRep;
                    if(self.IntValue.Value==0)
                    {
                        //0.Y * 0.V == .Y * .V
                        SRep = self.DecimalHalf * Value.DecimalHalf;
                        SRep /= MediumDecVariant::DecimalOverflowX;
                    }
                    else
                    {
                        SRep = MediumDecVariant::DecimalOverflowX * self.IntValue.Value + self.DecimalHalf;
                        SRep *= Value.DecimalHalf;
                        SRep /= MediumDecVariant::DecimalOverflowX;
                    }
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        self.IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        self.DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        self.IntValue = SelfIsNegative ? MediumDecVariant::NegativeRep : 0;
                        self.DecimalHalf = (signed int)SRep;
                    }
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : MediumDecVariant::DecimalOverflowX * self.GetIntHalf() + self.DecimalHalf;
                    SRep *= Value.GetIntHalf();//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)self.GetIntHalf() * Value.DecimalHalf;//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)self.DecimalHalf * (__int64)Value.DecimalHalf;
                    Temp04 /= MediumDecVariant::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / MediumDecVariant::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)MediumDecVariant::DecimalOverflow;
                    if (IntHalf == 0) { self.IntValue = (signed int)SelfIsNegative ? MediumDecVariant::NegativeRep : 0; }
                    else { self.IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf; }
                    self.DecimalHalf = (signed int)IntegerRep;
                }
            }
            if (self == MediumDecVariant::Zero) { self.DecimalHalf = 1; }//Prevent Dividing into nothing
            return self;
        }

        /// <summary>
        /// Division Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& DivOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
            if (Value == MediumDecVariant::Zero)
                throw "Target value can not be divided by zero";
            if (self == MediumDecVariant::Zero)
                return self;
            if (Value.IsNegative())
            {
                Value.SwapNegativeStatus();
                self.SwapNegativeStatus();
            }
			bool ResIsPositive = true;
			signed _int64 SelfRes;
			signed _int64 ValueRes;
			if(GetIntHalf()<0)
			{
			    SelfRes = self.GetIntHalf()==MirroredInt::NegativeZero?DecimalHalf:GetIntHalf().Value*NegDecimalOverflowX+DecimalHalf;
			    if(Value<0)
					ValueRes = Value.GetIntHalf()==NegativeRep?DecimalHalf:Value.GetIntHalf()*NegDecimalOverflowX+Value.DecimalHalf;
				else
				{
				    ResIsPositive = false;
					ValueRes = Value.GetIntHalf()*DecimalOverflowX+Value.DecimalHalf;
				}
			}
			else
			{
				SelfRes = GetIntHalf()*DecimalOverflowX+DecimalHalf;
			    if(Value<0)
				{
				    ResIsPositive = false;
					ValueRes = Value.GetIntHalf()==NegativeRep?DecimalHalf:GetIntHalf().GetValue()*NegDecimalOverflowX+Value.DecimalHalf;
				}
				else
					ValueRes = Value.GetIntHalf().GetValue()*DecimalOverflowX+Value.DecimalHalf;
			}
			
			signed _int64 IntHalfRes = SelfRes / ValueRes;
			signed _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
			IntValue = IntHalfRes==0&&ResIsPositive==false?NegativeRep:IntHalfRes;
			DecimalHalf = DecimalRes;
			if(IntHalfRes==0&&DecimalRes==0)
				return true;
			else
				return false;
            return self;
        }

        /// <summary>
        /// Remainder/Modulus Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& RemOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
            bool SelfIsWholeN = self.DecimalHalf == 0;
            bool ValueIsWholeN = Value.DecimalHalf == 0;
            if (Value.IntValue == 0 && ValueIsWholeN) { self.SetAsZero(); return self; }//Return zero instead of N/A
            if (SelfIsWholeN && ValueIsWholeN)//WholeNumbers
            {
                self.IntValue %= Value.GetIntHalf();
            }
            else if (ValueIsWholeN)
            {
                self %= Value.GetIntHalf();
            }
            else
            {
                if (Value.IsNegative())
                {
                    Value.SwapNegativeStatus();
                    self.SwapNegativeStatus();
                }
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                {
                    if (self.IntValue == NegativeRep) { self.IntValue = 0; }
                    else { self.SwapNegativeStatus(); }
                }
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.GetIntHalf() + self.DecimalHalf;
                __int64 SRep_DecHalf = SRep;
                __int64 VRep = DecimalOverflowX * Value.GetIntHalf() + Value.DecimalHalf;;
                SRep %= VRep;
                __int64 IntResult = SRep;
                //Int Half Calculated now get decimal digits that got truncated off
                SRep_DecHalf -= IntResult * VRep;
                //Gives enough buffer room that doesn't lose the decimal values
                SRep_DecHalf *= DecimalOverflowX;
                SRep_DecHalf %= VRep;
                if (IntResult == 0) { self.IntValue = (signed int)SelfIsNegative ? NegativeRep : 0; }
                else { self.IntValue = (signed int)SelfIsNegative ? IntResult * -1 : IntResult; }
                self.DecimalHalf = (signed int)SRep;
            }
            return self;
        }
        
    public:
        /// <summary>
        /// Addition Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant operator+(MediumDecVariant self, MediumDecVariant Value)
        {
            return AddOp(self, Value);
        }

        /// <summary>
        /// += Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator+=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return AddOp(self, Value);
        }
        
        /// <summary>
        /// += Operation Between MediumDecs(from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator+=(MediumDecVariant* self, MediumDecVariant Value){ return AddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant operator-(MediumDecVariant self, MediumDecVariant Value)
        {
            return SubOp(self, Value);
        }

        /// <summary>
        /// -= Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator-=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return SubOp(self, Value);
        }
        
        /// <summary>
        /// -= Operation Between MediumDecs(from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator-=(MediumDecVariant* self, MediumDecVariant Value){ return SubOp(**self, Value); }

        /// <summary>
        /// Multiplication Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant operator*(MediumDecVariant self, MediumDecVariant Value)
        {
            return MultOp(self, Value);
        }

        ///// <summary>
        ///// *= Operation Between MediumDecs
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDec</returns>
        friend MediumDecVariant& operator*=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return MultOp(self, Value);
        }

        ///// <summary>
        ///// *= Operation Between MediumDecs (from pointer)
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDec</returns>
        friend MediumDecVariant& operator*=(MediumDecVariant* self, MediumDecVariant Value){ return MultOp(**self, Value); }


        /// <summary>
        /// Division Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant operator/(MediumDecVariant self, MediumDecVariant Value)
        {
            return DivOp(self, Value);
        }

        /// <summary>
        /// /= Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator/=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return DivOp(self, Value);
        }
        
        /// <summary>
        /// /= Operation Between MediumDecs (from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator/=(MediumDecVariant* self, MediumDecVariant Value){ return DivOp(**self, Value); }

        /// <summary>
        /// Remainder Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant operator%(MediumDecVariant self, MediumDecVariant Value)
        {
            return RemOp(self, Value);
        }

        /// <summary>
        /// %= Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator%=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return RemOp(self, Value);
        }
        
        /// <summary>
        /// %= Operation Between MediumDecs (from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator%=(MediumDecVariant* self, MediumDecVariant Value)
        {
            return RemOp(**self, Value);
        }

        /// <summary>
        /// XOR Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant operator^(MediumDecVariant self, MediumDecVariant Value)
        {
            if (self.DecimalHalf == 0 && Value.DecimalHalf == 0)//Whole Numbers
            {
                self.IntValue ^= Value.GetIntHalf(); return self;
            }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                bool ValIsNegative = Value.IsNegative();
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 ^ Value.GetIntHalf()) * -1;
                    self.DecimalHalf ^= Value.DecimalHalf;
                }
                else
                {
                    if (ValIsNegative && Value.IntValue == NegativeRep)
                    {
                        self.IntValue = (self.GetIntHalf() ^ 0) * -1;
                        self.DecimalHalf ^= Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue ^= Value.GetIntHalf(); self.DecimalHalf ^= Value.DecimalHalf;
                    }
                }
            }
            return self;
        }

        /// <summary>
        /// Bitwise Or Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant operator|(MediumDecVariant self, MediumDecVariant Value)
        {
            if (self.DecimalHalf == 0 && Value.DecimalHalf == 0)//Whole Numbers
            {
                self.IntValue |= Value.GetIntHalf(); return self;
            }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                bool ValIsNegative = Value.IsNegative();
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 | Value.GetIntHalf()) * -1;
                    self.DecimalHalf |= Value.DecimalHalf;
                }
                else
                {
                    if (ValIsNegative && Value.IntValue == NegativeRep)
                    {
                        self.IntValue = (self.GetIntHalf() & 0) * -1;
                        self.DecimalHalf |= Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue |= Value.GetIntHalf(); self.DecimalHalf |= Value.DecimalHalf;
                    }
                }
            }
            return self;
        }

        /// <summary>
        /// Bitwise And Operation Between MediumDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant operator&(MediumDecVariant self, MediumDecVariant Value)
        {
            if (self.DecimalHalf == 0 && Value.DecimalHalf == 0)//Whole Numbers
            {
                self.GetIntHalf() &= Value.GetIntHalf(); return self;
            }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                bool ValIsNegative = Value.IsNegative();
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 & Value.GetIntHalf()) * -1;
                    self.DecimalHalf &= Value.DecimalHalf;
                }
                else
                {
                    if (ValIsNegative && Value.IntValue == NegativeRep)
                    {
                        self.IntValue = (self.GetIntHalf() & 0) * -1;
                        self.DecimalHalf &= Value.DecimalHalf;
                    }
                    else
                    {
                        self.GetIntHalf() &= Value.GetIntHalf(); self.DecimalHalf &= Value.DecimalHalf;
                    }
                }
            }
            return self;
        }

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDec</returns>
        friend MediumDecVariant& operator-(MediumDecVariant& self)
        {
            self.SwapNegativeStatus(); return self;
        }

        /// <summary>
        /// ++MediumDecVariant Operator
        /// </summary>
        /// <returns>MediumDecVariant &</returns>
        MediumDecVariant& operator ++()
        {
            if (IntValue == NegativeRep) { IntValue = 0; }
            else if (DecimalHalf == 0) { ++IntValue; }
            else if (IntValue == -1) { IntValue = NegativeRep; }
            else { ++IntValue; }
            return *this;
        }

        /// <summary>
        /// ++MediumDecVariant Operator
        /// </summary>
        /// <returns>MediumDecVariant &</returns>
        MediumDecVariant& operator --()
        {
            if (IntValue == NegativeRep) { IntValue = -1; }
            else if (DecimalHalf == 0) { --IntValue; }
            else if (IntValue == 0) { IntValue = NegativeRep; }
            else { --IntValue; }
            return *this;
        }

        /// <summary>
        /// MediumDec++ Operator
        /// </summary>
        /// <returns>MediumDec</returns>
        MediumDecVariant operator ++(int)
        {
            MediumDecVariant tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec-- Operator
        /// </summary>
        /// <returns>MediumDec</returns>
        MediumDecVariant operator --(int)
        {
            MediumDecVariant tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecVariant* Operator
        /// </summary>
        /// <returns>MediumDecVariant &</returns>
        MediumDecVariant& operator *()
        {
            return *this;
        }

    #pragma endregion MediumDec-To-MediumDecVariant Operators

    #pragma region Other Comparison Operators

    #pragma endregion Other Comparison Operators

    #pragma region MediumDec-To-Int Comparison Operators
        /// <summary>
        /// Equality Operation between <see cref="MediumDecVariant &"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator==(MediumDecVariant self, IntType Value)
        {
            return (self.IntValue == Value && self.DecimalHalf == 0);
        }

        /// <summary>
        /// != Operation between <see cref="MediumDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator!=(MediumDecVariant self, IntType Value)
        {
            if (self.DecimalHalf > 0) { return true; }
            else if (self.IntValue == Value && self.DecimalHalf == 0) { return false; }
            else { return true; };
        }

        /// <summary>
        /// Lesser than Operation between <see cref="MediumDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<(MediumDecVariant self, IntType Value)
        {
            if(self.DecimalHalf==0)
                return self.IntValue<Value;
            else if(self.IntValue<Value)
                return true;
            else if(self.IntValue == Value)
                return self.DecimalHalf>0?false:true;
        }
        /// <summary>
        /// Lesser than or equal to operation between <see cref="MediumDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<=(MediumDecVariant self, IntType Value)
        {
            if(self.DecimalHalf==0)
                return self.IntValue<=Value;
            else if(self.IntValue<Value)
                return true;
            else if(self.IntValue == Value)
                return self.DecimalHalf>0?false:true;
        }
        /// <summary>
        /// Greater than operation between <see cref="MediumDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>(MediumDecVariant self, IntType Value)
        {
            if(self.DecimalHalf==0)
                return self.IntValue>Value;
            else if(self.IntValue>Value)
                return true;
            else if(self.IntValue == Value)
                return self.DecimalHalf>0?true:false;
        }
        /// <summary>
        /// Greater than or equal to operation between <see cref="MediumDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>=(MediumDecVariant self, IntType Value)
        {
            if(self.DecimalHalf==0)
                return self.IntValue>=Value;
            else if(self.IntValue>Value)
                return true;
            else if(self.IntValue == Value)
                return self.DecimalHalf>0?true:false;
        }

        /// <summary>
        /// Equality Operation between Integer Type and <see cref="MediumDec"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator==(IntType Value, MediumDecVariant self)
        {
            return self.IntValue == Value && self.DecimalHalf == 0;
        }

        /// <summary>
        /// != Operation between Integer Type and <see cref="MediumDec"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator!=(IntType Value, MediumDecVariant self)
        {
            if (self.DecimalHalf > 0)
                return true;
            else if (self.IntValue == Value && self.DecimalHalf == 0)
                return false;
            else
                return true;
        }

        /// <summary>
        /// Lesser than Operation between Integer Type and <see cref="MediumDec"/> .
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<(IntType Value, MediumDecVariant self)
        {
            if(self.DecimalHalf==0)
                return self.IntValue>Value;
            else if(self.IntValue>Value)
                return true;
            else if(self.IntValue == Value)
                return self.DecimalHalf>0?true:false;
        }
        /// <summary>
        /// Lesser than or equal to operation between Integer Type and <see cref="MediumDec"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<=(IntType Value, MediumDecVariant self)
        {
            if(self.DecimalHalf==0)
                return self.IntValue>=Value;
            else if(self.IntValue>Value)
                return true;
            else if(self.IntValue == Value)
                return self.DecimalHalf>0?true:false;
        }
        /// <summary>
        /// Greater than operation between Integer Type and <see cref="MediumDec"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>(IntType Value, MediumDecVariant self)
        {
            if(self.DecimalHalf==0)
                return self.IntValue>Value;
            else if(self.IntValue>Value)
                return true;
            else if(self.IntValue == Value)
                return self.DecimalHalf>0?true:false;
        }
        /// <summary>
        /// Greater than or equal to operation between <see cref="MediumDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>=(IntType Value, MediumDecVariant self)
        {
            if(self.DecimalHalf==0)
                return self.IntValue<=Value;
            else if(self.IntValue<Value)
                return true;
            else if(self.IntValue == Value)
                return self.DecimalHalf>0?false:true;
        }

    #pragma endregion MediumDec-To-Int Comparison Operators
    #pragma region Other Operations
        friend MediumDecVariant operator+(MediumDecVariant self, float Value) { return self + (MediumDec)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, float Value) { return self - (MediumDec)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, float Value) { return self * (MediumDec)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, float Value) { return self / (MediumDec)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, float Value) { return self % (MediumDec)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, float Value) { return self ^ (MediumDec)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, float Value) { return self | (MediumDec)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, float Value) { return self & (MediumDec)Value; }

        friend MediumDecVariant operator+(float Value, MediumDecVariant self) { return (MediumDec)Value + self; }
        friend MediumDecVariant operator-(float Value, MediumDecVariant self) { return (MediumDec)Value - self; }
        friend MediumDecVariant operator*(float Value, MediumDecVariant self) { return (MediumDec)Value * self; }
        friend MediumDecVariant operator/(float Value, MediumDecVariant self) { return (MediumDec)Value / self; }
        friend MediumDecVariant operator%(float Value, MediumDecVariant self) { return (MediumDec)Value % self; }
        friend MediumDecVariant operator^(float Value, MediumDecVariant self) { return (MediumDec)Value ^ self; }
        friend MediumDecVariant operator|(float Value, MediumDecVariant self) { return (MediumDec)Value | self; }
        friend MediumDecVariant operator&(float Value, MediumDecVariant self) { return (MediumDec)Value & self; }

        friend MediumDecVariant operator+(MediumDecVariant self, double Value) { return self + (MediumDec)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, double Value) { return self - (MediumDec)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, double Value) { return self * (MediumDec)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, double Value) { return self / (MediumDec)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, double Value) { return self % (MediumDec)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, double Value) { return self ^ (MediumDec)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, double Value) { return self | (MediumDec)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, double Value) { return self & (MediumDec)Value; }

        friend MediumDecVariant operator+(double Value, MediumDecVariant self) { return (MediumDec)Value + self; }
        friend MediumDecVariant operator-(double Value, MediumDecVariant self) { return (MediumDec)Value - self; }
        friend MediumDecVariant operator*(double Value, MediumDecVariant self) { return (MediumDec)Value * self; }
        friend MediumDecVariant operator/(double Value, MediumDecVariant self) { return (MediumDec)Value / self; }
        friend MediumDecVariant operator%(double Value, MediumDecVariant self) { return (MediumDec)Value % self; }
        friend MediumDecVariant operator^(double Value, MediumDecVariant self) { return (MediumDec)Value ^ self; }
        friend MediumDecVariant operator|(double Value, MediumDecVariant self) { return (MediumDec)Value | self; }
        friend MediumDecVariant operator&(double Value, MediumDecVariant self) { return (MediumDec)Value & self; }

        friend MediumDecVariant operator+(MediumDecVariant self, ldouble Value) { return self + (MediumDec)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, ldouble Value) { return self - (MediumDec)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, ldouble Value) { return self * (MediumDec)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, ldouble Value) { return self / (MediumDec)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, ldouble Value) { return self % (MediumDec)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, ldouble Value) { return self ^ (MediumDec)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, ldouble Value) { return self | (MediumDec)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, ldouble Value) { return self & (MediumDec)Value; }

        friend MediumDecVariant operator+(ldouble Value, MediumDecVariant self) { return (MediumDec)Value + self; }
        friend MediumDecVariant operator-(ldouble Value, MediumDecVariant self) { return (MediumDec)Value - self; }
        friend MediumDecVariant operator*(ldouble Value, MediumDecVariant self) { return (MediumDec)Value * self; }
        friend MediumDecVariant operator/(ldouble Value, MediumDecVariant self) { return (MediumDec)Value / self; }
        friend MediumDecVariant operator%(ldouble Value, MediumDecVariant self) { return (MediumDec)Value % self; }
        friend MediumDecVariant operator^(ldouble Value, MediumDecVariant self) { return (MediumDec)Value ^ self; }
        friend MediumDecVariant operator|(ldouble Value, MediumDecVariant self) { return (MediumDec)Value | self; }
        friend MediumDecVariant operator&(ldouble Value, MediumDecVariant self) { return (MediumDec)Value & self; }

    #pragma endregion Other Operations
    #pragma region MediumDec-To-Integer Operations
    public:
        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& IntAddOp(MediumDecVariant& self, IntType& value)
        {
            if(self.DecimalHalf==0)
                self.IntValue.NRepSkippingAddOp(value);
            else
            {
                bool NegativeBeforeOperation = self.IntValue < 0;
                self.IntValue += value;
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(self.IntValue<0))
                    self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
            }
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        static MediumDecVariant& IntSubOp(MediumDecVariant& self, IntType& value)
        {
            if(self.DecimalHalf==0)
                self.IntValue.NRepSkippingSubOp(value);
            else
            {
                bool NegativeBeforeOperation = self.IntValue < 0;
                self.IntValue -= value;
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(self.IntValue<0))
                    self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
            }
            return self;
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        static MediumDecVariant& MultOp(MediumDecVariant& self, IntType& Value)
        {
            if (Value < 0)
            {
                Value *= -1;
                self.SwapNegativeStatus();
            }
            if (self == Zero) {}
            else if (Value == 0) { self.IntValue = 0; self.DecimalHalf = 0; }
            else if (self.DecimalHalf == 0)
            {
                self.IntValue.Value *= Value;
            }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                {
                    if (self.IntValue == NegativeRep) { self.IntValue = 0; }
                    else { self.SwapNegativeStatus(); }
                }
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.GetIntHalf() + self.DecimalHalf;
                SRep *= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    self.IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = SelfIsNegative?NegativeRep:0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            return self;
        }

        /// <summary>
        /// Division Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& DivOp(MediumDecVariant& self, IntType& Value)
        {
            if (Value == 0) { throw "Target value can not be divided by zero"; }
            else if (self == Zero) { return self; }
            if (Value < 0)
            {
                Value *= -1;
                self.SwapNegativeStatus();
            }
            if (self.DecimalHalf == 0)
            {
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                    self.SwapNegativeStatus();
                __int64 SRep = DecimalOverflowX * self.GetIntHalf();
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflow;
                    SRep -= OverflowVal * DecimalOverflow;
                    self.IntValue = (signed int)(SelfIsNegative ? OverflowVal * -1 : OverflowVal);
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = SelfIsNegative?NegativeRep:0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                    self.SwapNegativeStatus();
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.GetIntHalf() + self.DecimalHalf;
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= DecimalOverflowX * OverflowVal;
                    self.IntValue = (signed int)(SelfIsNegative ? OverflowVal * -1 : OverflowVal);
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = 0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            if (self == Zero) { return JustAboveZero; }//Prevent dividing into nothing
            return self;
        }

        /// <summary>
        /// Remainder/Modulus Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& IntRemOp(MediumDecVariant& self, IntType& Value)
        {
            if (Value == 0 || self == MediumDecVariant::Zero) { self.SetAsZero(); return self; }
            if (self.DecimalHalf == 0)
            {
                if (self.IsNegative())//https://www.quora.com/How-does-the-modulo-operation-work-with-negative-numbers-and-why
                {
                    self.IntValue %= Value;
                    self.IntValue = (signed int)(Value - self.GetIntHalf());
                }
                else
                {
                    self.IntValue %= Value; return self;
                }
            }
            else//leftValue is non-whole number
            {
                if (Value < 0) { self.SwapNegativeStatus(); Value *= -1; }
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                    self.SwapNegativeStatus();
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.GetIntHalf() + self.DecimalHalf;
                SRep %= Value;
                __int64 VRep = DecimalOverflowX * Value;
                SRep /= VRep;
                __int64 IntResult = SRep;
                SRep = ((__int64)self.GetIntHalf() * DecimalOverflow) + self.DecimalHalf;
                SRep -= IntResult * VRep;
                __int64 IntHalf = SRep / DecimalOverflow;
                SRep -= IntHalf * (__int64)DecimalOverflow;
                if (IntHalf == 0) { self.IntValue = SelfIsNegative ? (signed int)NegativeRep : 0; }
                else { self.IntValue = (signed int)(SelfIsNegative ? IntHalf * -1 : IntHalf); }
                self.DecimalHalf = (signed int)SRep;
            }
            return self;
        }

        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedAddOp(MediumDecVariant& self, IntType& value)
        {
            if(self.DecimalHalf==0)
                self.IntValue.NRepSkippingAddOp(value);
            else
                self.IntValue += value;
            return self;
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedSubOp(MediumDecVariant& self, IntType& value)
        {
            if(self.DecimalHalf==0)
                self.IntValue.NRepSkippingSubOp(value);
            else
            {
                bool NegativeBeforeOperation = self.IntValue < 0;
                self.IntValue -= value;
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(self.IntValue<0))
                    self.DecimalHalf = MediumDecVariant::DecimalOverflow - self.DecimalHalf;
            }
            return self;
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedMultOp(MediumDecVariant& self, IntType& Value)
        {
            if (self == Zero) {}
            else if (Value == 0) { self.IntValue = 0; self.DecimalHalf = 0; }
            else if (self.DecimalHalf == 0)
            {
                self.IntValue.Value *= Value;
            }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                    self.SwapNegativeStatus();
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.GetIntHalf() + self.DecimalHalf;
                SRep *= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    self.IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = SelfIsNegative?NegativeRep:0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            return self;
        }

        /// <summary>
        /// Division Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedDivOp(MediumDecVariant& self, IntType& Value)
        {
            if (Value == 0) { throw "Target value can not be divided by zero"; }
            else if (self == Zero) { return self; }
            if (self.DecimalHalf == 0)
            {
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                    self.SwapNegativeStatus();
                __int64 SRep = DecimalOverflowX * self.GetIntHalf();
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflow;
                    SRep -= OverflowVal * DecimalOverflow;
                    self.IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = SelfIsNegative?NegativeRep:0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                if (SelfIsNegative)
                {
                    if (self.IntValue == NegativeRep) { self.IntValue = 0; }
                    else { self.SwapNegativeStatus(); }
                }
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.GetIntHalf() + self.DecimalHalf;
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= DecimalOverflowX * OverflowVal;
                    self.IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = 0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            return self;
        }
        
        /// <summary>
        /// Remainder/Modulus Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedRemOp(MediumDecVariant& self, IntType& Value)
        {
            if(self == MediumDecVariant::Zero) { return self; }
            if (Value == 0){ self.GetIntHalf()=0; self.DecimalHalf = 0; return self; }
            if (self.DecimalHalf == 0)
            {
                self.IntValue %= Value; return self;
            }
            else//leftValue is non-whole number
            {
                __int64 SRep;
                if (self.IntValue == NegativeRep) { SRep = (__int64)self.DecimalHalf * -1; }
                else if (self.IsNegative()) { SRep = DecimalOverflowX * self.GetIntHalf() - self.DecimalHalf; }
                else { SRep = DecimalOverflowX * self.GetIntHalf() + self.DecimalHalf; }
                bool SelfIsNegative = SRep < 0;
                if (SelfIsNegative)
                    SRep *= -1;
                SRep %= Value;
                __int64 VRep = DecimalOverflowX * Value;
                SRep /= VRep;
                __int64 IntResult = SRep;
                SRep = ((__int64)self.GetIntHalf() * DecimalOverflow) + self.DecimalHalf;
                SRep -= IntResult * VRep;
                __int64 IntHalf = SRep / DecimalOverflow;
                SRep -= IntHalf * (__int64)DecimalOverflow;
                if (IntHalf == 0) { self.IntValue = (signed int)SelfIsNegative ? NegativeRep : 0; }
                else { self.IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf; }
                self.DecimalHalf = (signed int)SRep;
            }
            return self;
        }

     //   template<typename IntType>
     //   static MediumDecVariant& SignedModulusOp(MediumDecVariant& self, IntType& Value)
     //   {
        //}
    public:
        friend MediumDecVariant operator+(MediumDecVariant self, unsigned char Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-(MediumDecVariant self, unsigned char Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*(MediumDecVariant self, unsigned char Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/(MediumDecVariant self, unsigned char Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%(MediumDecVariant self, unsigned char Value){ return UnsignedRemOp(self, Value); }
        
        friend MediumDecVariant operator+=(MediumDecVariant& self, unsigned char Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-=(MediumDecVariant& self, unsigned char Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*=(MediumDecVariant& self, unsigned char Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/=(MediumDecVariant& self, unsigned char Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%=(MediumDecVariant& self, unsigned char Value){ return UnsignedRemOp(self, Value); }

        friend MediumDecVariant operator+(MediumDecVariant self, unsigned short Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-(MediumDecVariant self, unsigned short Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*(MediumDecVariant self, unsigned short Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/(MediumDecVariant self, unsigned short Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%(MediumDecVariant self, unsigned short Value){ return UnsignedRemOp(self, Value); }
        
        friend MediumDecVariant operator+=(MediumDecVariant& self, unsigned short Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-=(MediumDecVariant& self, unsigned short Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*=(MediumDecVariant& self, unsigned short Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/=(MediumDecVariant& self, unsigned short Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%=(MediumDecVariant& self, unsigned short Value){ return UnsignedRemOp(self, Value); }

        friend MediumDecVariant operator+(MediumDecVariant self, unsigned int Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-(MediumDecVariant self, unsigned int Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*(MediumDecVariant self, unsigned int Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/(MediumDecVariant self, unsigned int Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%(MediumDecVariant self, unsigned int Value){ return UnsignedRemOp(self, Value); }
        
        friend MediumDecVariant operator+=(MediumDecVariant& self, unsigned int Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-=(MediumDecVariant& self, unsigned int Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*=(MediumDecVariant& self, unsigned int Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/=(MediumDecVariant& self, unsigned int Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%=(MediumDecVariant& self, unsigned int Value){ return UnsignedRemOp(self, Value); }
        
        //friend MediumDecVariant operator+=(MediumDecVariant* self, unsigned int Value) { return UnsignedAddOp(**self, Value); }
        //friend MediumDecVariant operator-=(MediumDecVariant* self, unsigned int Value) { return UnsignedSubOp(**self, Value); }
        //friend MediumDecVariant operator*=(MediumDecVariant* self, unsigned int Value) { return UnsignedMultOp(**self, Value); }
        //friend MediumDecVariant operator/=(MediumDecVariant* self, unsigned int Value) { return UnsignedDivOp(**self, Value); }
        //friend MediumDecVariant operator%=(MediumDecVariant* self, unsigned int Value) { return UnsignedRemOp(**self, Value); }

        friend MediumDecVariant operator+(MediumDecVariant self, unsigned __int64 Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-(MediumDecVariant self, unsigned __int64 Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*(MediumDecVariant self, unsigned __int64 Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/(MediumDecVariant self, unsigned __int64 Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%(MediumDecVariant self, unsigned __int64 Value){ return UnsignedRemOp(self, Value); }
        
        friend MediumDecVariant operator+=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedRemOp(self, Value); }

     //   friend MediumDecVariant operator+=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedAddOp(**self, Value); }
        //friend MediumDecVariant operator-=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedSubOp(**self, Value); }
     //   friend MediumDecVariant operator*=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedMultOp(**self, Value); }
        //friend MediumDecVariant operator/=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedDivOp(**self, Value); }
     //   friend MediumDecVariant operator%=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedRemOp(**self, Value); }

        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator+(MediumDecVariant self, IntType Value)
        {
            return IntAddOp(self, Value);
        }

        ///// <summary>
        ///// += Operation Between MediumDecVariant and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator+=(MediumDecVariant& self, IntType Value)
        {
            return IntAddOp(self, Value);
        }

        template<typename IntType>
        friend MediumDecVariant operator+=(MediumDecVariant* self, IntType Value){ return IntAddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator-(MediumDecVariant self, IntType Value)
        {
            return IntSubOp(self, Value);
        }

        /// <summary>
        /// -= Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator-=(MediumDecVariant& self, IntType Value)
        {
            return IntSubOp(self, Value);
        }

        template<typename IntType>
        friend MediumDecVariant operator-=(MediumDecVariant* self, IntType Value){ return IntSubOp(**self, Value); }

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator*(MediumDecVariant self, IntType Value)
        {
            return MultOp(self, Value);
        }

        /// <summary>
        /// *= Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator*=(MediumDecVariant& self, IntType Value)
        {
            return MultOp(self, Value);
        }

        /// <summary>
        /// *= Operation Between MediumDecVariant and Integer Value (from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator*=(MediumDecVariant* self, IntType Value){ return MultOp(**self, Value); }

        /// <summary>
        /// Division Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator/(MediumDecVariant self, IntType Value)
        {
            return DivOp(self, Value);
        }

        /// <summary>
        /// /= Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator/=(MediumDecVariant& self, IntType Value)
        {
            return DivOp(self, Value);
        }

        template<typename IntType>
        friend MediumDecVariant operator/=(MediumDecVariant* self, IntType Value){ return DivOp(**self, Value); }
        
        /// <summary>
        /// Modulus Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator%(MediumDecVariant self, IntType Value)
        {
            return IntRemOp(self, Value);
        }

        /// <summary>
        /// %= Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator%=(MediumDecVariant& self, IntType Value)
        {
            return IntRemOp(self, Value);
        }

        template<typename IntType>
        friend MediumDecVariant operator%=(MediumDecVariant* self, IntType Value){ return IntRemOp(**self, Value); }

        /// <summary>
        /// Bitwise XOR Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator^(MediumDecVariant self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntValue ^= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 & Value) * -1;
                    self.DecimalHalf ^= Value;
                }
                else
                {
                    self.IntValue ^= Value; self.DecimalHalf ^= Value;
                }
            }
            return self;
        }

        /// <summary>
        /// Bitwise Or Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator|(MediumDecVariant self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntValue |= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 & Value) * -1;
                    self.DecimalHalf |= Value;
                }
                else
                {
                    self.IntValue |= Value; self.DecimalHalf |= Value;
                }
            }
            return self;
        }

        /// <summary>
        /// Bitwise And Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator&(MediumDecVariant self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.GetIntHalf() &= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IsNegative();
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 & Value) * -1;
                    self.DecimalHalf &= Value;
                }
                else
                {
                    self.GetIntHalf() &= Value; self.DecimalHalf &= Value;
                }
            }
            return self;
        }
    #pragma endregion MediumDec-To-Integer Operations
    #pragma region Integer-To-MediumDecVariant Operations
        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator+(IntType Value, MediumDecVariant self)
        {
            return IntAddOp(self, Value);
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator-(IntType Value, MediumDecVariant self)
        {
            MediumDecVariant Result = Value;
            return SubOp(Result, self);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator*(IntType Value, MediumDecVariant self)
        {
            return MultOp(self, Value);
        }

        /// <summary>
        /// Division Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator/(IntType Value, MediumDecVariant self)
        {
            MediumDecVariant Result = Value;
            return DivOp(Result, self);
        }

        /// <summary>
        /// Remainder/Modulus Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator%(IntType Value, MediumDecVariant self)
        {
            MediumDecVariant Result = Value;
            return RemOp(Result, self);
        }

        /// <summary>
        /// Bitwise XOR Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator^(IntType Value, MediumDecVariant self)
        {
            return (MediumDec)Value ^ self;
        }

        /// <summary>
        /// Bitwise Or Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator|(IntType Value, MediumDecVariant self)
        {
            return (MediumDec)Value | self;
        }

        /// <summary>
        /// Bitwise And Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator&(IntType Value, MediumDecVariant self)
        {
            return (MediumDec)Value & self;
        }

    #pragma endregion Integer-To-MediumDecVariant Operations
    #pragma region Math/Trigonomic Etc Functions
        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        MediumDecVariant& Floor()
        {
            if (DecimalHalf == 0)
            {
                return *this;
            }
            DecimalHalf = 0;
            if (IntValue == NegativeRep) { IntValue = -1; }
            else
            {
                --IntValue;
            }
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant Floor(MediumDecVariant Value)
        {
            return Value.Floor();
        }
        
        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        static MediumDecVariant Floor(MediumDecVariant Value, int precision)
        {
            switch (precision)
            {
            case 9: break;
            case 8: Value.DecimalHalf /= 10; Value.DecimalHalf *= 10; break;
            case 7: Value.DecimalHalf /= 100; Value.DecimalHalf *= 100; break;
            case 6: Value.DecimalHalf /= 1000; Value.DecimalHalf *= 1000; break;
            case 5: Value.DecimalHalf /= 10000; Value.DecimalHalf *= 10000; break;
            case 4: Value.DecimalHalf /= 100000; Value.DecimalHalf *= 100000; break;
            case 3: Value.DecimalHalf /= 1000000; Value.DecimalHalf *= 1000000; break;
            case 2: Value.DecimalHalf /= 10000000; Value.DecimalHalf *= 10000000; break;
            case 1: Value.DecimalHalf /= 100000000; Value.DecimalHalf *= 100000000; break;
            default: Value.DecimalHalf = 0; break;
            }
            if (Value.IntValue == NegativeRep && Value.DecimalHalf == 0) { Value.DecimalHalf = 0; }
            return Value;
        }
        
        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        MediumDecVariant& Ceil()
        {
            if (DecimalHalf == 0)
            {
                return *this;
            }
            DecimalHalf = 0;
            if (IntValue == NegativeRep) { IntValue = 0; }
            else
            {
                ++IntValue;
            }
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        static int FloorInt(MediumDecVariant Value)
        {
            if (Value.DecimalHalf == 0)
            {
                return Value.GetIntHalf();
            }
            if (Value.IntValue == NegativeRep) { return -1; }
            else
            {
                return Value.GetIntHalf() - 1;
            }
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        static int CeilInt(MediumDecVariant Value)
        {
            if (Value.DecimalHalf == 0)
            {
                return Value.GetIntHalf();
            }
            if (Value.IntValue == NegativeRep) { return 0; }
            else
            {
                return Value.GetIntHalf()+1;
            }
        }
        
        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the nearest integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Ceil(MediumDecVariant Value)
        {
            return Value.Ceil();
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDecVariant &</returns>
        MediumDecVariant& Trunc()
        {
            DecimalHalf = 0;
            if (IntValue == NegativeRep) { IntValue = 0; }
            return *this;
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Trunc(MediumDecVariant Value)
        {
            return Value.Trunc();
        }
        
        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        MediumDecVariant& Abs()
        {
            if (IsNegative())
                SwapNegativeStatus();
            return *this;
        }
        
        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Abs(MediumDecVariant Value)
        {
            return Value.Abs();
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDecVariant PowOp(ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            { 
                IntValue = 1; DecimalHalf = 0;
            }
            else if (expValue < 0)//Negative Pow
            {
                if (DecimalHalf == 0 && IntValue == 10 && expValue >= -9)
                {
                    IntValue = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[expValue * -1];
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expValue *= -1;
                    MediumDecVariant self = *this;
                    IntValue = 1; DecimalHalf = 0;// Initialize result
                    while (expValue > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (expValue % 2 == 1)
                            *this /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return this;
                }
            }
            else if (DecimalHalf == 0 && IntValue == 10)
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            else if (DecimalHalf == 0 && IntValue == -10)
                IntValue = expValue % 2 ? VariableConversionFunctions::PowerOfTens[expValue] : VariableConversionFunctions::PowerOfTens[expValue] * -1;
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecVariant self = *this;
                IntValue = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation on references with const expValue(for integer exponents)(C3892 fix)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDecVariant PowConstOp(const ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntValue = 1; DecimalHalf = 0;
            }
            else if (expValue < 0)//Negative Pow
            {
                if (DecimalHalf == 0 && IntValue == 10 && expValue >= -9)
                {
                    int expVal = expValue * -1;
                    IntValue = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[expVal];
                }
                else
                {
                    int expVal = expValue;
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expVal *= -1;
                    MediumDecVariant self = *this;
                    IntValue = 1; DecimalHalf = 0;// Initialize result
                    while (expVal > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (expVal % 2 == 1)
                            *this /= self;
                        // n must be even now
                        expVal = expVal >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return this;
                }
            }
            else if (DecimalHalf == 0 && IntValue == 10)
            {
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                int expVal = expValue;
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecVariant self = *this;
                IntValue = 1; DecimalHalf = 0;// Initialize result
                while (expVal > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expVal % 2 == 1)
                        this *= self;
                    // n must be even now
                    expVal = expVal >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation on references with const expValue(for integer exponents)(C3892 fix)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecVariant PowConstOp(MediumDecVariant& targetValue, const ValueType& expValue)
        {
            return targetValue.PowConstOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecVariant PowOp(MediumDecVariant& targetValue, ValueType& expValue)
        {
            return targetValue.PowOp(expValue);
        }
        
        /// <summary>
        /// Applies Power of operation(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDecVariant Pow(ValueType expValue)
        {
            return this->PowOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation (for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecVariant Pow(MediumDecVariant targetValue, ValueType expValue)
        {
            if (expValue == 1) { return targetValue; }//Return self
            else if (expValue == 0)
            {
                targetValue.IntValue = 1; targetValue.DecimalHalf = 0;
                return targetValue;
            }
            else if (expValue < 0)//Negative Pow
            {
                if (targetValue.DecimalHalf == 0 && targetValue.IntValue == 10 && expValue >= -9)
                {
                    targetValue.IntValue = 0; targetValue.DecimalHalf = MediumDecVariant::DecimalOverflow / VariableConversionFunctions::PowerOfTens[expValue * -1];
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expValue *= -1;
                    MediumDecVariant self = targetValue;
                    targetValue.IntValue = 1; targetValue.DecimalHalf = 0;// Initialize result
                    while (expValue > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (expValue % 2 == 1)
                            targetValue /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return targetValue;
                }
            }
            else if (targetValue.DecimalHalf == 0 && targetValue.IntValue == 10)
            {
                targetValue.IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecVariant self = targetValue;
                targetValue.IntValue = 1; targetValue.DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        targetValue *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return targetValue;
        }

        /// <summary>
        /// Applies Power of operation (for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecVariant PowRef(MediumDecVariant& targetValue, ValueType expValue)
        {
            if (expValue == 1)
                return targetValue;//Return self
            else if (expValue == 0)
                return MediumDecVariant::One;
            else if (expValue < 0)//Negative Pow
            {
                if (targetValue.DecimalHalf == 0 && targetValue.IntValue == 10 && expValue >= -9)
                {
                    return MediumDec(0, MediumDecVariant::DecimalOverflow / VariableConversionFunctions::PowerOfTens[expValue * -1]);
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expValue *= -1;
                    MediumDecVariant self = targetValue;
                    MediumDecVariant Result = MediumDecVariant::One;
                    while (expValue > 0)
                    {
                        // If expValue is odd, divide self with result
                        if (expValue % 2 == 1)
                            Result /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return Result;
                }
            }
            else if (targetValue.DecimalHalf == 0 && targetValue.IntValue == 10)
                return MediumDec(VariableConversionFunctions::PowerOfTens[expValue], 0);
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecVariant self = targetValue;
                MediumDecVariant Result = MediumDecVariant::One;
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        Result *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
                return Result;
            }
            return targetValue;
        }

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static MediumDecVariant Sqrt(MediumDecVariant value, int precision=7)
        {
            if (value.DecimalHalf == 0)
            {
                bool AutoSetValue = true;
                switch (value.IntValue.Value)
                {
                case 1: value.IntValue = 1; break;
                case 4: value.IntValue = 2; break;
                case 9: value.IntValue = 3; break;
                case 16: value.IntValue = 4; break;
                case 25: value.IntValue = 5; break;
                case 36: value.IntValue = 6; break;
                case 49: value.IntValue = 7; break;
                case 64: value.IntValue = 8; break;
                case 81: value.IntValue = 9; break;
                case 100: value.IntValue = 10; break;
                case 121: value.IntValue = 11; break;
                case 144: value.IntValue = 12; break;
                case 169: value.IntValue = 13; break;
                case 196: value.IntValue = 14; break;
                case 225: value.IntValue = 15; break;
                case 256: value.IntValue = 16; break;
                case 289: value.IntValue = 17; break;
                case 324: value.IntValue = 18; break;
                case 361: value.IntValue = 19; break;
                case 400: value.IntValue = 20; break;
                default:
                    AutoSetValue = false;
                    break;
                }
                if(AutoSetValue)
                {
                    return value;
                }
            }
            MediumDecVariant number = value;
            MediumDecVariant start = 0, end = number;
            MediumDecVariant mid;

            // variable to store the answer 
            MediumDecVariant ans;

            // for computing integral part 
            // of square root of number 
            while (start <= end) {
                mid = (start + end) / 2;
                if (mid * mid == number) {
                    ans = mid;
                    break;
                }

                // incrementing start if integral 
                // part lies on right side of the mid 
                if (mid * mid < number) {
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
            MediumDecVariant increment = "0.1";
            for (int i = 0; i < precision; i++) {
                while (ans * ans <= number) {
                    ans += increment;
                }

                // loop terminates when ans * ans > number 
                ans = ans - increment;
                increment = increment / 10;
            }
            return ans;
        }

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="nValue">The nth value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant NthRoot(MediumDecVariant value, int n, MediumDecVariant precision = MediumDecVariant::JustAboveZero)
        {
            MediumDecVariant xPre = 1+(value-1)/n;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            MediumDecVariant delX = MediumDec(2147483647, 0);

            //  xK denotes current value of x 
            MediumDecVariant xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = (nMinus1 * xPre +
                    value / MediumDecVariant::Pow(xPre, nMinus1)) / n;
                delX = MediumDecVariant::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static MediumDecVariant NthRootV2(MediumDecVariant targetValue, int n, MediumDecVariant& Precision = MediumDecVariant::FiveBillionth)
        {
            int nMinus1 = n - 1;
            MediumDecVariant x[2] = { (MediumDecVariant::One / n) * ((nMinus1 * targetValue) + (targetValue / MediumDecVariant::Pow(targetValue, nMinus1))), targetValue };
            while (MediumDecVariant::Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (MediumDecVariant::One / n) * ((nMinus1 * x[1]) + (targetValue / MediumDecVariant::Pow(x[1], nMinus1)));
            }
            return x[0];
        }

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Exp(MediumDecVariant x)
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
             *      - x: (MediumDecVariant float) power of e to evaluate
             * Returns:
             *      - (MediumDecVariant float) approximation of e^x in MediumDecVariant precision
             */
             // Check that x is a valid input.
            assert(-709 <= x.GetIntHalf() && x.GetIntHalf() <= 709);
            // When x = 0 we already know e^x = 1.
            if (x == MediumDecVariant::Zero) {
                return MediumDecVariant::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            MediumDecVariant x0 = MediumDecVariant::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            MediumDecVariant tn = MediumDecVariant::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = MediumDecVariant::CeilInt(x0 * MediumDecVariant::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + MediumDecVariant::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = MediumDecVariant::One / tn;
            }
            return tn;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        static MediumDecVariant FractionalPow(MediumDecVariant value, int expNum, int expDenom)
        {
            MediumDecVariant CalcVal = MediumDecVariant::NthRoot(MediumDecVariant::Pow(value, expNum), expDenom);
            return CalcVal;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        static MediumDecVariant FractionalPow(MediumDecVariant& value, boost::rational<int>& Frac)
        {
            MediumDecVariant CalcVal = MediumDecVariant::NthRoot(MediumDecVariant::Pow(value, Frac.numerator()), Frac.denominator());
            return CalcVal;
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDecVariant PowOp(MediumDecVariant& value, MediumDecVariant& expValue)
        {
            if (expValue.DecimalHalf == 0)
            {
                return value.Pow(expValue.GetIntHalf());
            }
            else
            {
                boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
                switch (expValue.GetIntHalf())
                {
                    case 0:
                        return FractionalPow(value, Frac);
                        break;
                    case MediumDecVariant::NegativeRep:
                        return 1 / FractionalPow(value, Frac);
                        break;
                    default:
                    {
                        if (expValue.IsNegative())//Negative Exponent 
                        {
                            MediumDecVariant CalcVal = 1 / value.Pow(expValue.GetIntHalf() * -1);
                            CalcVal /= FractionalPow(value, Frac);
                            return CalcVal;
                        }
                        else
                        {
                            MediumDecVariant CalcVal = value.Pow(expValue.GetIntHalf());
                            CalcVal *= FractionalPow(value, Frac);
                            return CalcVal;
                        }
                        break;
                    }
                }
            }
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDecVariant Pow(MediumDecVariant value, MediumDecVariant expValue)
        {
            return PowOp(value, expValue);
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::MediumDec</returns>
        static MediumDecVariant LnRef(MediumDecVariant& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            if (value.GetIntHalf()<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                MediumDecVariant threshold = MediumDecVariant::FiveMillionth;
                MediumDecVariant base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecVariant term = base;       // First term
                MediumDecVariant prev;          // Previous sum
                MediumDecVariant result = term;     // Kick it off

                do
                {
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                    ++den;
                } while (MediumDecVariant::Abs(prev - result) > threshold);

                return result;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
                MediumDecVariant TotalRes = (value - 1) / (value + 1);
                MediumDecVariant LastPow = TotalRes;
                MediumDecVariant WSquared = TotalRes * TotalRes;
                MediumDecVariant AddRes;
                int WPow = 3;
                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > MediumDecVariant::JustAboveZero);
                return TotalRes * 2;
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::MediumDec</returns>
        static MediumDecVariant LnRefV2(MediumDecVariant& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            if(value.GetIntHalf()==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                MediumDecVariant W = (value - 1)/ (value + 1);
                MediumDecVariant TotalRes = W;
                W.SwapNegativeStatus();
                MediumDecVariant LastPow = W;
                MediumDecVariant WSquared = W * W;
                int WPow = 3;
                MediumDecVariant AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > MediumDecVariant::JustAboveZero);
                return TotalRes * 2;
            }
            else if (value.GetIntHalf()==1)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                MediumDecVariant threshold = MediumDecVariant::FiveMillionth;
                MediumDecVariant base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecVariant term = base;       // First term
                MediumDecVariant prev;          // Previous sum
                MediumDecVariant result = term;     // Kick it off

                do
                {
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                    ++den;
                } while (MediumDecVariant::Abs(prev - result) > threshold);

                return result;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
                MediumDecVariant TotalRes = (value - 1) / (value + 1);
                MediumDecVariant LastPow = TotalRes;
                MediumDecVariant WSquared = TotalRes * TotalRes;
                MediumDecVariant AddRes;
                int WPow = 3;
                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > MediumDecVariant::JustAboveZero);
                return TotalRes * 2;
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        static MediumDecVariant Ln(MediumDecVariant value)
        {
            return LnRef(value);
        }

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Log10(MediumDecVariant value)
        {
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            if (value.DecimalHalf == 0 && value.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDec(index, 0);
                }
                return MediumDec(9, 0);
            }
            if (value.GetIntHalf()<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                MediumDecVariant threshold = MediumDecVariant::FiveBillionth;
                MediumDecVariant base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecVariant term = base;       // First term
                MediumDecVariant prev = 0;          // Previous sum
                MediumDecVariant result = term;     // Kick it off

                while (MediumDecVariant::Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return result*MediumDecVariant::LN10Mult;// result/MediumDecVariant::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                MediumDecVariant TotalRes = (value - 1) / (value + 1);
                MediumDecVariant LastPow = TotalRes;
                MediumDecVariant WSquared = TotalRes * TotalRes;
                MediumDecVariant AddRes;
                int WPow = 3;
                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > MediumDecVariant::JustAboveZero);
                return TotalRes * MediumDecVariant::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
            }
            //return MediumDecVariant::Ln(value) / MediumDecVariant::HaLN10;//Slightly off because of truncation etc
        }

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<typename ValueType>
        static MediumDecVariant Log10(ValueType value)
        {
            if (value == 1)
                return MediumDecVariant::Zero;
            else if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDec(index, 0);
                }
                return MediumDec(9, 0);
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                MediumDecVariant TotalRes = MediumDec((value - 1), 0) / MediumDec((value + 1), 0);
                MediumDecVariant LastPow = TotalRes;
                MediumDecVariant WSquared = TotalRes * TotalRes;
                MediumDecVariant AddRes;
                int WPow = 3;
                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > MediumDecVariant::JustAboveZero);
                return TotalRes * MediumDecVariant::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
            }
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Log(MediumDecVariant value, MediumDecVariant baseVal)
        {
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            return Log10(value) / Log10(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Log(MediumDecVariant value, int baseVal)
        {
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            //Calculate Base log first
            MediumDecVariant baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDec(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDec(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDec((baseVal - 1), 0) / MediumDec((baseVal + 1), 0);
                MediumDecVariant baseLastPow = baseTotalRes;
                MediumDecVariant baseWSquared = baseTotalRes * baseTotalRes;
                MediumDecVariant baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > MediumDecVariant::JustAboveZero);
            }

            //Now calculate other log
            if (value.DecimalHalf == 0 && value.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? MediumDec(index, 0) / (baseTotalRes * MediumDecVariant::HalfLN10Mult): MediumDec(index, 0)/ baseTotalRes;
                }
                return lnMultLog? MediumDec(9, 0) / (baseTotalRes*MediumDecVariant::HalfLN10Mult):MediumDec(9, 0)/baseTotalRes;
            }
            if (value.GetIntHalf() < 2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                MediumDecVariant threshold = MediumDecVariant::FiveBillionth;
                MediumDecVariant base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecVariant term = base;       // First term
                MediumDecVariant prev = 0;          // Previous sum
                MediumDecVariant result = term;     // Kick it off

                while (MediumDecVariant::Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return lnMultLog? result/baseTotalRes:(result*2)/ baseTotalRes;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                MediumDecVariant W = (value - 1) / (value + 1);
                MediumDecVariant TotalRes = W;
                MediumDecVariant AddRes;
                int WPow = 3;
                do
                {
                    AddRes = MediumDecVariant::PowRef(W, WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > MediumDecVariant::JustAboveZero);
                return lnMultLog? TotalRes/baseTotalRes:(TotalRes * MediumDecVariant::HalfLN10Mult)/ baseTotalRes;
            }
            //return Log10(Value) / Log10(BaseVal);
        }

    #pragma endregion Math Etc Functions
    #pragma region Trigonomic Etc Functions
        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant SinFromAngle(MediumDecVariant Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntValue %= 360;
                    Value.IntValue = 360 - Value.GetIntHalf();
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return MediumDecVariant::Zero; }
            else if (Value.IntValue == 30 && Value.DecimalHalf == 0)
            {
                return PointFive;
            }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
                return One;
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return MediumDecVariant::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else
            {
                MediumDecVariant NewValue = Zero;
                //Angle as Radian
                MediumDecVariant Radius = PI * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's Taylor series!!
                    NewValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Radius, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
                }
                return NewValue;
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns></returns>
        static MediumDecVariant CosFromAngle(MediumDecVariant Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntValue %= 360;
                    Value.IntValue = 360 - Value.GetIntHalf();
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return One; }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
                return MediumDecVariant::Zero;
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return MediumDecVariant::Zero;
            }
            else
            {
                MediumDecVariant NewValue = Zero;
                //Angle as Radian
                MediumDecVariant Radius = PI * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's also Taylor series!!
                    NewValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Radius, 2 * i) / VariableConversionFunctions::Fact(2 * i);
                }
                return NewValue;
            }
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Sin(MediumDecVariant Value)
        {
            MediumDecVariant SinValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Cos(MediumDecVariant Value)
        {
            MediumDecVariant CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                CosValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Tan(MediumDecVariant Value)
        {
            MediumDecVariant SinValue = Zero;
            MediumDecVariant CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            {
                CosValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant TanFromAngle(MediumDecVariant Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntValue %= 360;
                    Value.IntValue = 360 - Value.GetIntHalf();
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return MediumDecVariant::Zero; }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
                return MediumDecVariant::Maximum;//Positive Infinity
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return MediumDecVariant::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return MediumDecVariant::Minimum;//Negative Infinity
            }
            else
            {
                return Tan(PI * Value / 180);
            }
        }

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecVariant ATan(MediumDecVariant Value)
        {
            MediumDecVariant SinValue = Zero;
            MediumDecVariant CosValue = Zero;
            //Angle as Radian
            for (int i = 0; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            { // That's also Taylor series!!
                CosValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i) / VariableConversionFunctions::Fact(2 * i);
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
        /// <returns>MediumDec</returns>
        static MediumDecVariant ArcTan2(MediumDecVariant y, MediumDecVariant x)
        {
            MediumDecVariant coeff_1 = PI / 4;
            MediumDecVariant coeff_2 = 3 * coeff_1;
            MediumDecVariant abs_y = MediumDecVariant::Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            MediumDecVariant r;
            MediumDecVariant angle;
            if (x >= 0)
            {
                r = (x - abs_y) / (x + abs_y);
                angle = coeff_1 - coeff_1 * r;
            }
            else
            {
                r = (x + abs_y) / (abs_y - x);
                angle = coeff_2 - coeff_1 * r;
            }
            if (y < 0)
                return -angle;// negate if in quad III or IV
            else
                return angle;
        }
    #pragma endregion Math/Trigonomic Etc Functions
    };

    #pragma region ValueDefine Source
    MediumDecVariant MediumDecVariant::PI = PIValue();
    MediumDecVariant MediumDecVariant::One = OneValue();
    MediumDecVariant MediumDecVariant::Two = TwoValue();
    MediumDecVariant MediumDecVariant::NegativeOne = NegativeOneValue();
    MediumDecVariant MediumDecVariant::Zero = ZeroValue();
    MediumDecVariant MediumDecVariant::PointFive = Point5Value();
    MediumDecVariant MediumDecVariant::JustAboveZero = JustAboveZeroValue();
    MediumDecVariant MediumDecVariant::OneMillionth = OneMillionthValue();
    MediumDecVariant MediumDecVariant::FiveThousandth = FiveThousandthValue();
    MediumDecVariant MediumDecVariant::Minimum = MinimumValue();
    MediumDecVariant MediumDecVariant::Maximum = MaximumValue();
    MediumDecVariant MediumDecVariant::E = EValue();
    MediumDecVariant MediumDecVariant::LN10 = LN10Value();
    MediumDecVariant MediumDecVariant::LN10Mult = LN10MultValue();
    MediumDecVariant MediumDecVariant::HalfLN10Mult = HalfLN10MultValue();
    MediumDecVariant MediumDecVariant::TenMillionth = TenMillionthValue();
    MediumDecVariant MediumDecVariant::FiveMillionth = FiveMillionthValue();
    MediumDecVariant MediumDecVariant::FiveBillionth = FiveBillionthValue();
    MediumDecVariant MediumDecVariant::OneGMillionth = OneHundredMillionthValue();
    MediumDecVariant MediumDecVariant::Nil = NilValue();

/*#ifdef AltDec_InheritMediumDec
    //Workaround to enable static inheritance
    class DLL_API MediumDecValues
    {
    #pragma region ValueDefines
    private:
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant ZeroValue()
        {
            MediumDecVariant NewSelf = MediumDec();
            return NewSelf;
        }
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant OneValue()
        {
            MediumDecVariant NewSelf = MediumDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant TwoValue()
        {
            MediumDecVariant NewSelf = MediumDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant NegativeOneValue()
        {
            MediumDecVariant NewSelf = MediumDec(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Point5Value()
        {
            MediumDecVariant NewSelf = MediumDec(0, 500000000);
            return NewSelf;
        }

        static MediumDecVariant JustAboveZeroValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 1);
            return NewSelf;
        }

        static MediumDecVariant OneMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 1000);
            return NewSelf;
        }

        static MediumDecVariant FiveThousandthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 5000000);
            return NewSelf;
        }

        static MediumDecVariant FiveMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 5000);
            return NewSelf;
        }

        static MediumDecVariant TenMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 100);
            return NewSelf;
        }

        static MediumDecVariant OneHundredMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 10);
            return NewSelf;
        }

        static MediumDecVariant FiveBillionthValue()
        {
            MediumDecVariant NewSelf = MediumDec(0, 5);
            return NewSelf;
        }

        /// <summary>
        /// Returns PI(3.1415926535897932384626433) with tenth digit rounded up(3.141592654)
        /// (about 1.000000000448883 times actual value of PI with digits past 15th cut off)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant PIValue()
        {
            MediumDecVariant NewSelf = MediumDec(3, 141592654);
            return NewSelf;
        }

        /// <summary>
        /// Returns value of -2147483647.999999999
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant MinimumValue()
        {
            return MediumDec(-2147483647, 999999999);
        }

        /// <summary>
        /// Returns value of 2147483647.999999999
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant MaximumValue()
        {
            return MediumDec(2147483647, 999999999);
        }

        /// <summary>
        /// Euler's number
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        virtual MediumDecVariant EValue()
        {
            return MediumDec(2, 718281828);
        }

        virtual MediumDecVariant LN10Value()
        {
            return MediumDec(2, 302585093);
        }

        virtual MediumDecVariant LN10MultValue()
        {
            return MediumDec(0, 434294482);
        }

        virtual MediumDecVariant HalfLN10MultValue()
        {
            return MediumDec(0, 868588964);
        }

        virtual MediumDecVariant NilValue()
        {
            return MediumDec(-2147483648, -2147483648);
        }
    public:
        static MediumDecVariant PI;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Zero;

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant OneGMillionth;

        //0e-7
        static MediumDecVariant TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDecVariant FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant NegativeOne;

        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// </summary>
        static MediumDecVariant Maximum;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// </summary>
        static MediumDecVariant Minimum;

        /// <summary>
        /// Euler's number
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant E;

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDecVariant LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecVariant LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecVariant HalfLN10Mult;

        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static MediumDecVariant Nil;

        ///// <summary>
        ///// 16 x 1 Matrix with Values at Zero
        ///// </summary>
        //static MediumDecVariant ZeroMatrix16[16] = { Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero };
    #pragma endregion ValueDefines
	}

    MediumDecVariant MediumDecValues::PI = PIValue();
    MediumDecVariant MediumDecValues::One = OneValue();
    MediumDecVariant MediumDecValues::Two = TwoValue();
    MediumDecVariant MediumDecValues::NegativeOne = NegativeOneValue();
    MediumDecVariant MediumDecValues::Zero = ZeroValue();
    MediumDecVariant MediumDecValues::PointFive = Point5Value();
    MediumDecVariant MediumDecValues::JustAboveZero = JustAboveZeroValue();
    MediumDecVariant MediumDecValues::OneMillionth = OneMillionthValue();
    MediumDecVariant MediumDecValues::FiveThousandth = FiveThousandthValue();
    MediumDecVariant MediumDecValues::Minimum = MinimumValue();
    MediumDecVariant MediumDecValues::Maximum = MaximumValue();
    MediumDecVariant MediumDecValues::E = EValue();
    MediumDecVariant MediumDecValues::LN10 = LN10Value();
    MediumDecVariant MediumDecValues::LN10Mult = LN10MultValue();
    MediumDecVariant MediumDecValues::HalfLN10Mult = HalfLN10MultValue();
    MediumDecVariant MediumDecValues::TenMillionth = TenMillionthValue();
    MediumDecVariant MediumDecValues::FiveMillionth = FiveMillionthValue();
    MediumDecVariant MediumDecValues::FiveBillionth = FiveBillionthValue();
    MediumDecVariant MediumDecValues::OneGMillionth = OneHundredMillionthValue();
    MediumDecVariant MediumDecValues::Nil = NilValue();
#endif*/
    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void MediumDecVariant::ReadString(std::string Value)
    {
        IntValue = 0; DecimalHalf = 0;
        bool IsNegative = false;
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
                IsNegative = true;
            }
            else if (StringChar == '.')
            {
                ReadingDecimal = true;
            }
        }
        PlaceNumber = WholeNumberBuffer.length() - 1;
        for (char const& StringChar : WholeNumberBuffer)
        {
            TempInt = VariableConversionFunctions::CharAsInt(StringChar);
            TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
            if (StringChar != '0')
            {
                IntValue += TempInt02;
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
        if (IsNegative)
            SwapNegativeStatus();
    }

    /// <summary>
    /// Gets the value from string.
    /// </summary>
    /// <param name="Value">The value.</param>
    /// <returns>MediumDec</returns>
    inline MediumDecVariant MediumDecVariant::GetValueFromString(std::string Value)
    {
        MediumDecVariant NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

    std::string MediumDecVariant::ToString()
    {
        std::string Value = "";
        int CurrentSection = GetIntHalf();
        unsigned __int8 CurrentDigit;
        std::string DecBuffer = "";
        if (IsNegative())
        {
            Value += "-";
            if (IntValue == NegativeRep) { CurrentSection = 0; }
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

    std::string MediumDecVariant::ToFullString()
    {
        std::string Value = "";
        int CurrentSection = GetIntHalf();
        unsigned __int8 CurrentDigit;
        if (IsNegative())
        {
            Value += "-";
            if (IntValue == NegativeRep) { CurrentSection = 0; }
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
	
	class MediumDecFractional;
    /// <summary>
    /// (MediumDecVariant based dividend)/(MediumDecVariant based divisor)
    /// </summary>
    class DLL_API MediumDecFractional
    {
		MediumDecVariant Dividend;
		MediumDecVariant Divisor;
		//Balances out fractional so that Divisor is as close to a whole number as viable
		void NormalizeFractional()
		{
			//Add code here later
		}
	}
}