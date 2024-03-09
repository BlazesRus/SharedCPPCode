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

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)
#if defined(AltNum_UseOldDivisionCode)
	#include <boost/multiprecision/cpp_int.hpp>
#endif

#include "IntegerConcept.hpp"
/*
AltFloat_IncludeFixedPoint
AltFloat_IncludeFractionRepresentation = Requires AltFloat_IncludeFixedPoint as well
*/

namespace BlazesRusCode
{

    class AltFloat;

    /// <summary>
    /// Alternative fixed point number representation designed for use with AltFloat
    /// Represents 0 to 9.9999 x 10^127 when in fixed point mode(when SignificantPt1 is greater than 128)
    /// If AltFloat_IncludeFixedPoint is not toggled, then last bit becomes sign bit instead(not needed for storage of trailing digits)
    /// (Restricting SignificantPt1 field from values 0-9 to make easier to compare operations even if could fit 0 to 127 in field)
    /// When not in fixed point mode, represents range of approximately 0 to 3.4028235 × 10^38
    /// (switches out of fixed point mode when value would truncate instead to nearest floating format representation)
    /// (4 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API AltFloat
    {//Right side operations are assumed to be positive
	protected://^ in comments refers to power of instead of XOR

		//Holds first 7 bits of Significant field
		//If last bit is 1, then treat Significant as in fixed point mode(with support for value 0 to 127.9999 in signicant field)
		unsigned char SignificantPt1;
		//Last 16 Bits of Significant field stored here
        //If AltFloat_IncludeFractionRepresentation and AltFloat_IncludeFixedPoint is enabled
        // and the last bit is 1,
        // then represents a fractional or mixed fraction 
        // if SignificantPt1 not equal to 128
		unsigned short SignificantPt2;

        //In fixed point mode refers to the Exponent of 10 in "(SignificantPt1.SignificantPt2) * 10^Exponent" formula
        //Otherwise in floating point mode, refers to Exponent inside "Significant * 2^Exponent" formula
		signed char Exponent;
    public:
    #if defined(AltFloat_IncludeFixedPoint)
        signed int GetIntHalf()
        {
            if(SignificantPt1>128)
                return SignificantPt1 - 128;
            return SignificantPt1;
        }

        void SetIntHalf(unsigned char rValue)
        {
            SignificantPt1 = rValue;
        }

        //Returns DecimalHalf
        unsigned short GetDecimalSide()
        {
            return SignificantPt2;
        }

        void SetDecimalHalf(unsigned short rValue)
        {
            SignificantPt2 = rValue;
        }
    #endif

        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        /// <param name="significantPt1">Holds first 7 bits of Significant field plus flag for fixed point mode</param>
        /// <param name="SignificantPt2">The non-whole based half of the representation(and other special statuses)</param>
        AltFloat(unsigned char significantPt1=0, unsigned short significantPt2=0, signed char exponent=0)
        {
            SignificantPt1 = significantPt1;
            SignificantPt2 = significantPt2;
            Exponent = exponent;
        }

        AltFloat(const AltFloat&) = default;

        AltFloat& operator=(const AltFloat&) = default;

        //Detect if at exactly zero
		bool IsZero()
		{
            return SignificantPt1==0&&SignificantPt2==0;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(AltFloat Value)
        {
            SignificantPt1 = Value.SignificantPt1;
            SignificantPt2 = Value.SignificantPt2;
            Exponent = Value.Exponent;
        }

        void SetAsZero()
        {
            SignificantPt1 = 0; SignificantPt2 = 0;
            Exponent = 0;
        }

    #pragma region Const Representation values
    protected:

    #pragma endregion Const Representation values
	public:
	
        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        {
    #if defined(AltFloat_IncludeFixedPoint)
            SignificantPt1 = 137;
            SignificantPt2 = 9999;
            Exponent = 127;
    #else
            //Add code here later
    #endif
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
    #if defined(AltFloat_IncludeFixedPoint)
            SetAsZero();
    #else
            //Add code here later
    #endif
        }

    #pragma region Fractional Setters
        #if defined(AltFloat_IncludeFractionRepresentation)
        //Sets decimal half as denominator
        void SetDenominator(unsigned char rValue)
        {
            //Add code here later
        }
        #endif
    #pragma endregion Fractional Setters

    #pragma region ApproachingZero Setters

    #pragma endregion ApproachingZero Setters

    #pragma region ValueDefines

        static AltFloat ZeroValue()
        {
            return AltFloat();
        }

    #pragma endregion ValueDefines
    #if !defined(AltFloat_IncludeFixedPoint)
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
            SignificantPt1 = SignificantPt1 ^ 128;//Flip the last bit (SignificantPt1 bits xor 10000000)
        }
    #endif

    #pragma region String Commands
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(std::string Value)
        {
            //Add code here later
        }

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        AltFloat GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        AltFloat(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltFloat(std::string Value)
        {
            this->ReadString(Value);
        }

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToString()
        {
            //Add code here later
            return "";//placeholder
        }

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToString(); }

    #pragma endregion String Commands

    #pragma region ConvertToOtherTypes

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
    //To-Do:Place code here
    #pragma endregion Comparison Operators

	#pragma region Division Operations
        template<IntegerType IntType=int>
        void DivByIntOp(const IntType& rValue)
		{
			//Division by 10s can directly divide Exponent by rValue
		}
		
        //Division by AltFloat operation
        void DivOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Division Operations

	#pragma region Multiplication Operations
        //Multiply by Integer Operation
        template<IntegerType IntType=int>
        void MultByIntOp(const IntType& rValue)
		{
			//Multiplication by 10s can directly multiple Exponent by rValue
		}
		
        //Multiply operation
        void MultOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Multiplication Operations

	#pragma region Addition Operations
        //Addition by Integer Operation
        template<IntegerType IntType=int>
        void AddByIntOp(const IntType& rValue)
		{
		}
		
        //Addition by AltFloat Operation
        void AddOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations
        //Subtraction by Integer Operation
        template<IntegerType IntType=int>
        void SubtractByIntOp(const IntType& rValue)
		{
		}
		
        //Subtraction by AltFloat Operation
        void SubtractOp(const AltFloat& rValue)
		{
		}
	#pragma endregion Subtraction Operations
	
    #pragma region Main Operator Overrides
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator/(AltFloat self, AltFloat Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat& operator/=(AltFloat& self, AltFloat Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator*(AltFloat self, AltFloat Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltFloat</returns>
        friend AltFloat& operator*=(AltFloat& self, AltFloat Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator+(AltFloat self, AltFloat Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat& operator+=(AltFloat& self, AltFloat Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator-(AltFloat self, AltFloat Value) { return SubtractOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat& operator-=(AltFloat& self, AltFloat Value) { return SubtractOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma region Other Operators

        /// <summary>
        /// ++AltFloat Operator
        /// </summary>
        /// <returns>AltFloat &</returns>
        AltFloat& operator ++()
        {
            *this = *this + 1;
            return *this;
        }

        /// <summary>
        /// --AltFloat Operator
        /// </summary>
        /// <returns>AltFloat &</returns>
        AltFloat& operator --()
        {
            *this = *this - 1;
            return *this;
        }

        /// <summary>
        /// AltFloat++ Operator
        /// </summary>
        /// <returns>AltFloat</returns>
        AltFloat operator ++(int)
        {
            AltFloat tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// AltFloat-- Operator
        /// </summary>
        /// <returns>AltFloat</returns>
        AltFloat operator --(int)
        {
            AltFloat tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// AltFloat* Operator
        /// </summary>
        /// <returns>AltFloat &</returns>
        AltFloat& operator *()
        {
            return *this;
        }
    #pragma endregion Other Operators

	#pragma region Modulus Operations
        template<IntegerType IntType=int>
        void ModulusByIntOp(const IntType& rValue)
		{
		}
		
        void ModulusOp(const AltFloat& rValue)
		{
		}

        friend AltFloat operator%(AltFloat& self, int Value) { return ModulusByIntOp(self, Value); }
        friend AltFloat operator%(AltFloat& self, signed long long Value) { return ModulusByIntOp(self, Value); }

        friend AltFloat operator%=(AltFloat& self, int Value) { return ModulusByIntOp(self, Value); }
        friend AltFloat operator%=(AltFloat& self, signed long long Value) { return ModulusByIntOp(self, Value); }

        friend AltFloat operator%=(AltFloat* self, int Value) { return ModulusByIntOp(**self, Value); }
        friend AltFloat operator%=(AltFloat* self, signed long long Value) { return ModulusByIntOp(**self, Value); }
        
        friend AltFloat operator%(AltFloat self, unsigned __int64 Value) { return ModulusByIntOp(self, Value); }
    
	#pragma endregion Modulus Operations

    #pragma region Bitwise Functions

    #pragma endregion Bitwise Functions

	#pragma region Math Etc Functions

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDec &</returns>
        AltFloat& Trunc()
        {
    #if defined(AltFloat_IncludeFixedPoint)
            if(SignificantPt1>128)//Fixed point mode
            {
                SignificantPt2=0;
            }
            else
            {//Floating point mode
    #endif
                //Add code here later
    #if defined(AltFloat_IncludeFixedPoint)
            }
    #endif
            return *this;
        }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static AltFloat Trunc(AltFloat Value)
        {
            return Value.Trunc();
        }

	#pragma endregion Math Etc Functions

    }
    #pragma region String Function Source

    #pragma endregion String Function Source