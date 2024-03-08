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
AltFloat_IncludeFractionRepresentation
*/

namespace BlazesRusCode
{

    class AltFloat;

    /// <summary>
    /// Alternative fixed point number representation designed for use with AltFloat
    /// Represents 0 to 127.9999 x 10^127 when in fixed point mode(when SignificantPt1 is greater than 128)
    /// When not in fixed point mode, represents range of approximately 0 to 3.4028235 × 10^38
    /// (switches out of fixed point mode when value would truncate instead to nearest floating format representation)
    /// (4 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API AltFloat
    {
	protected:

		//Holds first 7 bits of Significant field
		//If last bit is 1, then treat Significant as in fixed point mode(with support for value 0 to 127.9999 in signicant field)
		unsigned char SignificantPt1;
		//Last 16 Bits of Significant field stored here
        //If AltFloat_IncludeFractionRepresentation and AltFloat_IncludeFixedPoint is enabled
        // and the last bit is 1,
        // then represents a fractional or mixed fraction 
        // if SignificantPt1 not equal to 128
		unsigned short SignificantPt2;

		short Exponent;
    public:
    #if defined(AltFloat_IncludeFixedPoint)
        signed int GetIntHalf()
        {
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
        #if defined(AltFloat_IncludeFractionRepresentation)
        //Sets decimal half as denominator
        void SetDenominator(unsigned char rValue)
        {

        }
        #endif

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
        AltFloat(unsigned char significantPt1=0, unsigned short significantPt2=0, signed short exponent=0)
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

    #else

    #endif
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
            SetAsZero();
        }

	#pragma region Fractional Setters
	//Not used for this variant
	#pragma endregion Fractional Setters
	
	#pragma region ApproachingZero Setters
	//Not used for this variant
	#pragma endregion ApproachingZero Setters

    #pragma region ValueDefines
    protected:
	#if defined(AltNum_EnableNaN)

	#endif
        
        static AltFloat ZeroValue()
        {
            return AltFloat();
        }

public:

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat Zero;
        
    #pragma endregion ValueDefines

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

#pragma endregion String Commands

//private:
        //std::string BasicToStringOp();
		
		//std::string BasicToFullStringOp();
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

    #pragma region From Standard types to this type
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(float Value)
        {
            //To-Do:Place code here
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(double Value)
        {
            //To-Do:Place code here
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(ldouble Value)
        {
            //To-Do:Place code here
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(bool Value)
        {
            //To-Do:Place code here
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(int Value)
        {
            //To-Do:Place code here
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltFloat(float Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltFloat(double Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltFloat(ldouble Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltFloat(bool Value)
        {
            this->SetVal(Value);
        }

    #pragma endregion From Standard types to this type

    #pragma region From this type to Standard types
        /// <summary>
        /// AltFloat to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float()
        {
            float Value;
            //To-Do:Place code here
            return Value;
        }

        /// <summary>
        /// AltFloat to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double()
        {
            double Value;
            //To-Do:Place code here
            return Value;
        }

        /// <summary>
        /// AltFloat to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble()
        {
            ldouble Value;
            //To-Do:Place code here
            return Value;
        }
		
        /// <summary>
        /// AltFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return IntValue.GetV; }

        explicit operator bool() { return IntValue.IsZero() ? false : true; }
    #pragma endregion From this type to Standard types

    #pragma region Pi Conversion
	//Not used for this variant
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	//Not used for this variant
    #pragma endregion E Conversion
	
    #pragma region Other RepType Conversion
	//Not used for this variant
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators
    //To-Do:Place code here
    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:
        template<typename IntType>
        void PartialIntDivOp(IntType& Value)
        {
            //To-Do:Place code here
        }
public:

		void PartialDivOp(signed int& Value) { PartialIntDivOp(Value); }
		void PartialDivOp(unsigned int& Value) { PartialIntDivOp(Value); }
		void PartialDivOp(signed long long& Value) { PartialIntDivOp(Value); }
        void PartialDivOp(unsigned long long& Value) { PartialIntDivOp(Value); }

		static void PartialDivOp(AltFloat& self, signed int& Value) { self.PartialIntDivOp(Value); }
		static void PartialDivOp(AltFloat& self, unsigned int& Value) { self.PartialIntDivOp(Value); }
		static void PartialDivOp(AltFloat& self, signed long long& Value) { self.PartialIntDivOp(Value); }
        static void PartialDivOp(AltFloat& self, unsigned long long& Value) { self.PartialIntDivOp(Value); }

		AltFloat PartialDiv(signed int Value)
        { AltFloat self = *this; PartialIntDivOp(Value); return self; }
		AltFloat PartialDiv(unsigned int Value)
        { AltFloat self = *this; PartialIntDivOp(Value); return self; }
		AltFloat PartialDiv(signed long long Value)
        { AltFloat self = *this; PartialIntDivOp(Value); return self; }
        AltFloat PartialDiv(unsigned long long Value)
        { AltFloat self = *this; PartialIntDivOp(Value); return self; }

		static AltFloat PartialDiv(AltFloat& self, signed int Value) { self.PartialIntDivOp(Value); return self; }
		static AltFloat PartialDiv(AltFloat& self, unsigned int Value) { self.PartialIntDivOp(Value); return self; }
		static AltFloat PartialDiv(AltFloat& self, signed long long Value) { self.PartialIntDivOp(Value); return self; }
        static AltFloat PartialDiv(AltFloat& self, unsigned long long Value) { self.PartialIntDivOp(Value); return self; }

protected:
        template<typename IntType>
        AltFloat& BasicIntDivOp(IntType& Value)
        {
            //To-Do:Place code here
        }

public:

		void BasicDivOp(signed int& Value) { BasicIntDivOp(Value); }
		void BasicDivOp(unsigned int& Value) { BasicUnsignedIntDivOp(Value); }
		void BasicDivOp(signed long long& Value) { BasicIntDivOp(Value); }
        void BasicDivOp(unsigned long long& Value) { BasicUnsignedIntDivOp(Value); }

		static void BasicDivOp(AltFloat& self, signed int& Value) { self.BasicIntDivOp(Value); }
		static void BasicDivOp(AltFloat& self, unsigned int& Value) { self.BasicUnsignedIntDivOp(Value); }
		static void BasicDivOp(AltFloat& self, signed long long& Value) { self.BasicIntDivOp(Value); }
        static void BasicDivOp(AltFloat& self, unsigned long long& Value) { self.BasicUnsignedIntDivOp(Value); }

		AltFloat BasicDiv(signed int Value)
        { AltFloat self = *this; BasicIntDivOp(Value); return self; }
		AltFloat BasicDiv(unsigned int Value)
        { AltFloat self = *this; BasicUnsignedIntDivOp(Value); return self; }
		AltFloat BasicDiv(signed long long Value)
        { AltFloat self = *this; BasicIntDivOp(Value); return self; }
        AltFloat BasicDiv(unsigned long long Value)
        { AltFloat self = *this; BasicUnsignedIntDivOp(Value); return self; }

		static AltFloat BasicDiv(AltFloat& self, signed int Value) { self.BasicIntDivOp(Value); return self; }
		static AltFloat BasicDiv(AltFloat& self, unsigned int Value) { self.BasicUnsignedIntDivOp(Value); return self; }
		static AltFloat BasicDiv(AltFloat& self, signed long long Value) { self.BasicIntDivOp(Value); return self; }
        static AltFloat BasicDiv(AltFloat& self, unsigned long long Value) { AltFloat self = *this; BasicUnsignedIntDivOp(Value); return self; }

//protected:
//        template<typename IntType>
//        void BasicIntDivOpV2(IntType& Value)
//        {
//            if (IsZero())
//                return;
//            if (Value < 0)
//            {
//                Value *= -1;
//                SwapNegativeStatus();
//            }
//            PartialIntDivOp(Value);
//            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
//        }
//
//        template<typename IntType>
//        void BasicUnsignedIntDivOpV2(IntType& Value)
//        {
//            if (IsZero())
//                return;
//            PartialIntDivOp(Value);
//            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
//        }
//public:
//
//		void BasicDivOpV2(signed int& Value) { BasicIntDivOpV2(Value); }
//		void BasicDivOpV2(unsigned int& Value) { BasicUnsignedIntDivOpV2(Value); }
//		void BasicDivOpV2(signed long long& Value) { BasicIntDivOpV2(Value); }
//        void BasicDivOpV2(unsigned long long& Value) { BasicUnsignedIntDivOpV2(Value); }
//
//		static void BasicDivOpV2(AltFloat& self, signed int& Value) { self.BasicIntDivOpV2(Value); }
//		static void BasicDivOpV2(AltFloat& self, unsigned int& Value) { self.BasicUnsignedIntDivOpV2(Value); }
//		static void BasicDivOpV2(AltFloat& self, signed long long& Value) { self.BasicIntDivOpV2(Value); }
//        static void BasicDivOpV2(AltFloat& self, unsigned long long& Value) { self.BasicUnsignedIntDivOpV2(Value); }
//
//		AltFloat BasicDivV2(signed int Value)
//        { AltFloat self = *this; BasicIntDivOpV2(Value); return self; }
//		AltFloat BasicDivV2(unsigned int Value)
//        { AltFloat self = *this; BasicUnsignedIntDivOpV2(Value); return self; }
//		AltFloat BasicDivV2(signed long long Value)
//        { AltFloat self = *this; BasicIntDivOpV2(Value); return self; }
//        AltFloat BasicDivV2(unsigned long long Value)
//        { AltFloat self = *this; BasicUnsignedIntDivOpV2(Value); return self; }
//
//		static AltFloat BasicDivV2(signed int Value) { self.BasicIntDivOpV2(Value); return self; }
//		static AltFloat BasicDivV2(unsigned int Value) { self.BasicUnsignedIntDivOpV2(Value); return self; }
//		static AltFloat BasicDivV2(signed long long Value) { self.BasicIntDivOpV2(Value); return self; }
//        static AltFloat BasicDivV2(unsigned long long Value) { AltFloat self = *this; BasicUnsignedIntDivOpV2(Value); return self; }

    #pragma endregion NormalRep Integer Division Operations
	
	#pragma region NormalRep AltNumToAltNum Operations
protected:
        //Return true if divide into zero
        bool PartialDivOp(AltFloat& Value)
        {
            //To-Do:Place code here
        }

public:
		
        void BasicDivOp(AltFloat& Value)
        {
            //To-Do:Place code here
        }

        AltFloat BasicDiv(AltFloat Value)
        {
            AltFloat self = *this;
            self.BasicDivOp(Value);
            return self;
        }

        //void CatchAllDivision;

public:
        //bool RepToRepDivOp(RepType& LRep, RepType& RRep, AltFloat& self, AltFloat& Value);

        /// <summary>
        /// Division Operation
        /// </summary>
        AltFloat& DivOp(AltFloat& Value) { BasicDivOp(Value); return *this; }

        AltFloat DivideAsCopy(AltFloat Value) { AltFloat self = *this; self.BasicDivOp(Value); return self; }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        AltFloat& MultOp(AltFloat& Value) { BasicMultOp(Value); return *this; }

        AltFloat MultipleAsCopy(AltFloat Value) { AltFloat self = *this; self.BasicMultOp(Value); return self; }

        /// <summary>
        /// Addition Operation
        /// </summary>
        AltFloat& AddOp(AltFloat& Value) { BasicAddOp(Value); return *this; }


        AltFloat AddAsCopy(AltFloat Value) { AltFloat self = *this; self.BasicAddOp(Value); return self; }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        AltFloat& SubOp(AltFloat& Value) { BasicSubOp(Value); return *this; }


        AltFloat SubtractAsCopy(AltFloat Value) { AltFloat self = *this; self.BasicSubOp(Value); return self; }

        //AltFloat& RemOp(AltFloat& Value) { BasicRemOp(Value); return *this; }

        //AltFloat ModulusAsCopy(AltFloat Value) { AltFloat self = *this; self.BasicRemOp(Value); return self; }

    #pragma endregion NormalRep AltNumToAltNum Operations
	
    #pragma region Other Integer Operations
        /// <summary>
        /// Division Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat&</returns>
        template<typename IntType>
        static AltFloat& IntDivOp(IntType& Value)
        {
            //To-Do:Place code here
            return;
        }

        /// <summary>
        /// Division Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        static AltFloat& IntDivOp(AltFloat& self, IntType& Value) { return self.IntDivOp(Value); }

		/// <summary>
        /// Basic Multiplication Operation(main code block)
        /// </summary>
        /// <param name="Value">The value.</param>
		bool BasicMultOpPt2(AltFloat& Value)
		{
            //To-Do:Place code here
		}
		
		/// <summary>
        /// Basic Multiplication Operation(before ensuring doesn't multiply into nothing)
        /// </summary>
        /// <param name="Value">The value.</param>
		bool BasicMultOpPt1(AltFloat& Value)
        {
            //To-Do:Place code here
        }

		/// <summary>
        /// Basic Multiplication Operation(without checking for special representation variations or zero)
		/// Returns true if prevented from multiplying into nothing(except when multipling by zero)
        /// </summary>
        /// <param name="Value">The value.</param>
		bool BasicMultOp(AltFloat& Value)
		{
            //To-Do:Place code here
		}

        //BasicMultOp without negative number check
		bool BasicMultOpV2(AltFloat& Value)
		{
            //To-Do:Place code here
		}

protected:
		//void CatchAllMultiplication(AltFloat& Value, RepType& LRep, RepType& RRep)

public:
        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat&</returns>
        static AltFloat& MultOp(AltFloat& self, AltFloat& Value);

		//Multiplies 2 AltNum variables together (Use normal AltNum + AltNum operation if need to use on 2 coPies of variables)
		static AltFloat& MultOpV2(AltFloat& self, AltFloat Value)
		{
			return MultOp(self, Value);
		}
		
		/// <summary>
        /// Partial Multiplication Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        void PartialIntMultOp(IntType& Value)
        {
            //To-Do:Place code here
        }

        /// <summary>
        /// Multiplication Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        void BasicIntMultOp(IntType& Value)
        {
            //To-Do:Place code here
        }

        /// <summary>
        /// Multiplication Operation Between AltFloat and Integer Value(Without negative flipping)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        void BasicIntMultOpV2(IntType& Value)
        {
            //To-Do:Place code here
        }

        template<typename IntType>
        static AltFloat& IntMultOpPt2(IntType& Value)
        {
            //To-Do:Place code here
        }

        //IntMultOp without negative check
        template<typename IntType>
        static AltFloat& UnsignedIntMultOp(IntType& Value)
        {
            //To-Do:Place code here
        }

        /// <summary>
        /// Multiplication Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        static AltFloat& IntMultOp(IntType& Value)
        {
            //To-Do:Place code here
        }

        static AltFloat& MultOp(int& Value) { return IntMultOp(Value); }

        /// <summary>
        /// Multiplication Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        static AltFloat& IntMultOp(AltFloat& self, IntType& Value) { return self.IntMultOp(Value); }

        static AltFloat& MultOp(AltFloat& self, int& Value) { return self.IntMultOp(Value); }

        /// <summary>
        /// Multiplication Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        static AltFloat& UnsignedMultOp(AltFloat& self, IntType& Value)
        {
            if (self == Zero) {}
            else if (Value == 0) { self.IntValue = 0; self.DecimalHalf = 0; }
            else if (self.DecimalHalf == 0)
            {
                self.IntValue *= Value;
            }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                if (SelfIsNegative)
                {
                    if (self.IntValue == NegativeRep) { self.IntValue = 0; }
                    else { self.IntValue *= -1; }
                }
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.IntValue + self.DecimalHalf;
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
                    self.IntValue = SelfIsNegative ? NegativeRep : 0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            return self;
        }

        void RepToRepMultOp(RepType& LRep, RepType& RRep, AltFloat& self, AltFloat& Value);
    #pragma endregion Multiplication/Division Operations

#pragma region Addition/Subtraction Operations
        /// <summary>
        /// Basic Addition Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicAddOp(AltFloat& Value)
        {
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntValue += Value.IntValue;
            if (Value.DecimalHalf != 0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltFloat::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= AltFloat::DecimalOverflow) { DecimalHalf -= AltFloat::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltFloat::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= AltFloat::DecimalOverflow) { DecimalHalf -= AltFloat::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltFloat::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= AltFloat::DecimalOverflow) { DecimalHalf -= AltFloat::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltFloat::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= AltFloat::DecimalOverflow) { DecimalHalf -= AltFloat::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = AltFloat::DecimalOverflow - DecimalHalf;
        }

protected:
        /// <summary>
        /// Addition Operation Between AltFloat and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltFloat&</returns>
        template<typename IntType>
        static AltFloat& IntAddOp(IntType& value)
        {
            if (value == 0)
                return;
	#if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert AltFloat into complex number at moment";
				return;
            }
	#endif
	#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return;
	#endif
	#if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
			else
	#endif
			if(ExtraRep!=0)//Don't convert if mixed fraction
				ConvertToNormType();
			bool WasNegative = IntValue < 0;
			IntValue += value;
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ IntValue >= 0)//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
			{
				if(DecimalHalf<0)//Flip the fractional half of mixed fraction if flips to other side
				{
		#if defined(AltNum_EnableAlternativeMixedFrac)
					if(ExtraRep<0)// DecimalHalf:-2,ExtraRep:-3 becomes DecimalHalf:-1, ExtraRep:-3
						DecimalHalf = ExtraRep - DecimalHalf;
					else
		#endif			
						DecimalHalf = -(ExtraRep+DecimalHalf);// DecimalHalf:-2,ExtraRep:3 becomes DecimalHalf:-1, ExtraRep:3
				}
				else
					DecimalHalf = AltFloat::DecimalOverflow - DecimalHalf;
			}
	#else
            if(WasNegative ^ IntValue >= 0)
				DecimalHalf = AltFloat::DecimalOverflow - DecimalHalf;
	#endif
            return;
        }

        /// <summary>
        /// Addition Operation Between AltFloat and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltFloat&</returns>
        template<typename IntType>
        static AltFloat& IntAddOp(AltFloat& self, IntType& value)
        {
            return self.IntAddOp(value);
        }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        static AltFloat& AddOp(AltFloat& self, AltFloat& Value);

        //void CatchAllAddition(AltFloat& Value, RepType& LRep, RepType& RRep)
public:
        //void RepToRepAddOp(RepType& LRep, RepType& RRep, AltFloat& self, AltFloat& Value);

		/// <summary>
        /// Basic Subtraction Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicSubOp(AltFloat& Value)
        {
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntValue -= Value.IntValue;
            //Now deal with the decimal section
            if(Value.DecimalHalf!=0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltFloat::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= AltFloat::DecimalOverflow) { DecimalHalf -= AltFloat::DecimalOverflow; --IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltFloat::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= AltFloat::DecimalOverflow) { DecimalHalf -= AltFloat::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltFloat::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= AltFloat::DecimalOverflow) { DecimalHalf -= AltFloat::DecimalOverflow; --IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltFloat::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= AltFloat::DecimalOverflow) { DecimalHalf -= AltFloat::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = AltFloat::DecimalOverflow - DecimalHalf;
        }

                /// <summary>
        /// Subtraction Operation Between AltFloat and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        static AltFloat& IntSubOp(IntType& value)
        {
            if (value == 0)
                return;
	#if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert AltFloat into complex number at moment";
				return;
            }
	#endif
	#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return;
	#endif
	#if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
			else
	#endif
			if(ExtraRep!=0)//Don't convert if mixed fraction
				ConvertToNormType();
			bool WasNegative = IntValue < 0;
			IntValue += value;
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ IntValue >= 0)//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
			{
				if(DecimalHalf<0)//Flip the fractional half of mixed fraction if flips to other side
				{
		#if defined(AltNum_EnableAlternativeMixedFrac)
					if(ExtraRep<0)// DecimalHalf:-2,ExtraRep:-3 becomes DecimalHalf:-1, ExtraRep:-3
						DecimalHalf = ExtraRep - DecimalHalf;
					else
		#endif			
						DecimalHalf = -(ExtraRep+DecimalHalf);// DecimalHalf:-2,ExtraRep:3 becomes DecimalHalf:-1, ExtraRep:3
				}
				else
					DecimalHalf = AltFloat::DecimalOverflow - DecimalHalf;
			}
	#else
            if(WasNegative ^ (IntValue >= 0))
				DecimalHalf = AltFloat::DecimalOverflow - DecimalHalf;
	#endif
            return;
        }

        /// <summary>
        /// Subtraction Operation Between AltFloat and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        static AltFloat& IntSubOp(AltFloat& self, IntType& value)
        {
            return self.IntSubOp(value);
        }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat&</returns>
        static AltFloat& SubOp(AltFloat& self, AltFloat& Value);

		//void CatchAllSubtraction(AltFloat& Value, RepType& LRep, RepType& RRep)


        //void RepToRepSubOp(RepType& LRep, RepType& RRep, AltFloat& self, AltFloat& Value);
#pragma endregion Addition/Subtraction Operations

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
        friend AltFloat operator-(AltFloat self, AltFloat Value) { return SubOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat& operator-=(AltFloat& self, AltFloat Value) { return SubOp(self, Value); }

        /// <summary>
        /// Addition Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator+(AltFloat self, int Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between AltFloat and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltFloat</returns>
        template<typename IntType>
        friend AltFloat& operator+=(AltFloat& self, int Value) { return IntAddOp(self, Value); }

        //friend AltFloat operator+=(AltFloat* self, int Value) { return IntAddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator-(AltFloat self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat& operator-=(AltFloat& self, int Value) { return IntSubOp(self, Value); }

        //friend AltFloat& operator-=(AltFloat* self, int Value) { return IntSubOp(**self, Value); }

        /// <summary>
        /// Multiplication Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator*(AltFloat self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        friend AltFloat& operator*=(AltFloat& self, int Value) { return IntMultOp(self, Value); }

        ///// <summary>
        ///// *= Operation Between AltFloat and Integer Value (from pointer)
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltFloat</returns>
        //friend AltFloat operator*=(AltFloat* self, int Value) { return IntMultOp(**self, Value); }

        /// <summary>
        /// Division Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator/(AltFloat self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat& operator/=(AltFloat& self, int Value) { return IntDivOp(self, Value); }

        //friend AltFloat operator/=(AltFloat* self, int Value) { return IntDivOp(**self, Value); }

        /// <summary>
        /// Addition Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator+(AltFloat self, signed long long Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between AltFloat and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltFloat</returns>
        template<typename IntType>
        friend AltFloat& operator+=(AltFloat& self, signed long long Value) { return IntAddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator-(AltFloat self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat& operator-=(AltFloat& self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator*(AltFloat self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        friend AltFloat operator*=(AltFloat& self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator/(AltFloat self, signed long long Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat operator/=(AltFloat& self, signed long long Value) { return IntDivOp(self, Value); }
    
        friend AltFloat operator+(AltFloat self, float Value) { return self + (AltFloat)Value; }
        friend AltFloat operator-(AltFloat self, float Value) { return self - (AltFloat)Value; }
        friend AltFloat operator*(AltFloat self, float Value) { return self * (AltFloat)Value; }
        friend AltFloat operator/(AltFloat self, float Value) { return self / (AltFloat)Value; }

        friend AltFloat operator+(float Value, AltFloat self) { return (AltFloat)Value + self; }
        friend AltFloat operator-(float Value, AltFloat self) { return (AltFloat)Value - self; }
        friend AltFloat operator*(float Value, AltFloat self) { return (AltFloat)Value * self; }
        friend AltFloat operator/(float Value, AltFloat self) { return (AltFloat)Value / self; }

        friend AltFloat operator+(AltFloat self, double Value) { return self + (AltFloat)Value; }
        friend AltFloat operator-(AltFloat self, double Value) { return self - (AltFloat)Value; }
        friend AltFloat operator*(AltFloat self, double Value) { return self * (AltFloat)Value; }
        friend AltFloat operator/(AltFloat self, double Value) { return self / (AltFloat)Value; }

        friend AltFloat operator+(AltFloat self, ldouble Value) { return self + (AltFloat)Value; }
        friend AltFloat operator-(AltFloat self, ldouble Value) { return self - (AltFloat)Value; }
        friend AltFloat operator*(AltFloat self, ldouble Value) { return self * (AltFloat)Value; }
        friend AltFloat operator/(AltFloat self, ldouble Value) { return self / (AltFloat)Value; }

        friend AltFloat operator+(ldouble Value, AltFloat self) { return (AltFloat)Value + self; }
        friend AltFloat operator-(ldouble Value, AltFloat self) { return (AltFloat)Value - self; }
        friend AltFloat operator*(ldouble Value, AltFloat self) { return (AltFloat)Value * self; }
        friend AltFloat operator/(ldouble Value, AltFloat self) { return (AltFloat)Value / self; }


        friend AltFloat operator+(AltFloat self, unsigned char Value) { return IntAddOp(self, Value); }
        friend AltFloat operator-(AltFloat self, unsigned char Value) { return IntSubOp(self, Value); }
        friend AltFloat operator*(AltFloat self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend AltFloat operator/(AltFloat self, unsigned char Value) { return self.UnsignedIntDivOp(Value); }

        friend AltFloat operator+=(AltFloat& self, unsigned char Value) { return IntAddOp(self, Value); }
        friend AltFloat operator-=(AltFloat& self, unsigned char Value) { return IntSubOp(self, Value); }
        friend AltFloat operator*=(AltFloat& self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend AltFloat operator/=(AltFloat& self, unsigned char Value) { return UnsignedDivOp(self, Value); }
        

        friend AltFloat operator+(AltFloat self, unsigned short Value) { return IntAddOp(self, Value); }
        friend AltFloat operator-(AltFloat self, unsigned short Value) { return IntSubOp(self, Value); }
        friend AltFloat operator*(AltFloat self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend AltFloat operator/(AltFloat self, unsigned short Value) { return UnsignedDivOp(self, Value); }

        friend AltFloat operator+=(AltFloat& self, unsigned short Value) { return IntAddOp(self, Value); }
        friend AltFloat operator-=(AltFloat& self, unsigned short Value) { return IntSubOp(self, Value); }
        friend AltFloat operator*=(AltFloat& self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend AltFloat operator/=(AltFloat& self, unsigned short Value) { return UnsignedDivOp(self, Value); } 

        friend AltFloat operator+(AltFloat self, unsigned int Value) { return IntAddOp(self, Value); }
        friend AltFloat operator-(AltFloat self, unsigned int Value) { return IntSubOp(self, Value); }
        friend AltFloat operator*(AltFloat self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend AltFloat operator/(AltFloat self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        

        friend AltFloat operator+=(AltFloat& self, unsigned int Value) { return IntAddOp(self, Value); }
        friend AltFloat operator-=(AltFloat& self, unsigned int Value) { return IntSubOp(self, Value); }
        friend AltFloat operator*=(AltFloat& self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend AltFloat operator/=(AltFloat& self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        
        friend AltFloat operator+(AltFloat self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend AltFloat operator-(AltFloat self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend AltFloat operator*(AltFloat self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend AltFloat operator/(AltFloat self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

        friend AltFloat operator+=(AltFloat& self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend AltFloat operator-=(AltFloat& self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend AltFloat operator*=(AltFloat& self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend AltFloat operator/=(AltFloat& self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma endregion Other Operators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>AltFloat</returns>
        friend AltFloat& operator-(AltFloat& self)
        {
            self.SwapNegativeStatus(); return self;
        }

        /// <summary>
        /// ++AltFloat Operator
        /// </summary>
        /// <returns>AltFloat &</returns>
        AltFloat& operator ++()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
#endif
            if (DecimalHalf == 0)
                ++IntValue.Value;
            else if (IntValue == NegativeRep)
                IntValue = MirroredInt::Zero;
            else
                ++IntValue.Value;
            return *this;
        }

        /// <summary>
        /// ++AltFloat Operator
        /// </summary>
        /// <returns>AltFloat &</returns>
        AltFloat& operator --()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
#endif
            if (DecimalHalf == 0)
                --IntValue.Value;
            else if (IntValue.Value == 0)
                IntValue = NegativeRep;
            else
                --IntValue.Value;
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
    #if !defined(AltNum_PreventModulusOverride)

        friend AltFloat operator%(AltFloat& self, int Value) { return IntRemOp(self, Value); }
        friend AltFloat operator%(AltFloat& self, signed long long Value) { return IntRemOp(self, Value); }

        friend AltFloat operator%=(AltFloat& self, int Value) { return IntRemOp(self, Value); }
        friend AltFloat operator%=(AltFloat& self, signed long long Value) { return IntRemOp(self, Value); }

        friend AltFloat operator%=(AltFloat* self, int Value) { return IntRemOp(**self, Value); }
        friend AltFloat operator%=(AltFloat* self, signed long long Value) { return IntRemOp(**self, Value); }
        
        friend AltFloat operator%(AltFloat self, unsigned __int64 Value) { return UnsignedRemOp(self, Value); }
    
        #if defined(AltNum_EnableAlternativeModulusResult)
        //friend AltFloat operator%(AltFloat& self, int Value) { return IntRemOp(self, Value); }
        //friend AltFloat operator%(AltFloat& self, signed long long Value) { return IntRemOp(self, Value); }
        //friend AltFloat operator%(AltFloat self, unsigned __int64 Value) { return UnsignedRemOp(self, Value); }
        #endif
    #endif
    #pragma endregion Modulus Operations

    #pragma region Bitwise Functions
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        friend AltFloat operator^(AltFloat self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntValue ^= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
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
        /// Bitwise Or Operation Between AltFloat and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename IntType>
        friend AltFloat operator|(AltFloat self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntValue |= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
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
    #endif
    #pragma endregion Bitwise Functions

	#pragma region Math Etc Functions
        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>AltFloat&</returns>
        AltFloat& Abs()
        {
            if (IntValue == NegativeRep)
                IntValue.Value = 0;
            else if (IntValue.Value < 0)
                IntValue *= -1;
            return *this;
        }

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>AltFloat</returns>
        static AltFloat Abs(AltFloat tValue)
        {
            return tValue.Abs();
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>AltFloat&</returns>
        AltFloat& Floor()
        {
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>AltFloat&</returns>
        static AltFloat Floor(AltFloat Value)
        {
            return Value.Floor();
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        static AltFloat Floor(AltFloat Value, int precision)
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
        /// <returns>AltFloat&</returns>
        AltFloat& Ceil()
        {
            if (DecimalHalf != 0)
            {
                DecimalHalf = 0;
                if (IntValue == NegativeRep) { IntValue = 0; }
                else
                {
                    ++IntValue;
                }
            }
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>AltFloat&</returns>
        static int FloorInt(AltFloat Value)
        {
            if (Value.DecimalHalf == 0)
            {
                return Value.IntValue.GetValue();
            }
            if (Value.IntValue == NegativeRep) { return -1; }
            else
            {
                return Value.IntValue.GetValue() - 1;
            }
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>AltFloat&</returns>
        static int CeilInt(AltFloat Value)
        {
            if (Value.DecimalHalf == 0)
            {
                return Value.IntValue.GetValue();
            }
            if (Value.IntValue == NegativeRep) { return 0; }
            else
            {
                return Value.IntValue.GetValue() + 1;
            }
        }
        
        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>AltFloat</returns>
        static AltFloat Ceil(AltFloat Value)
        {
            return Value.Ceil();
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>AltFloat &</returns>
        AltFloat& Trunc()
        {
            DecimalHalf = 0;
            return *this;
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        static AltFloat Trunc(AltFloat Value)
        {
            return Value.Trunc();
        }
	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions	
        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static AltFloat BasicSqrt(AltFloat& value, int precision=7)
        {//Ignores Alternate representations use Sqrt instead to check based on RepType
            if (value.DecimalHalf == 0)
            {
                bool AutoSetValue = true;
                switch (value.IntValue.GetValue())
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
            AltFloat number = value;
            AltFloat start = 0, end = number;
            AltFloat mid;

            // variable to store the answer 
            AltFloat ans;

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
            AltFloat increment = "0.1";
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
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
		static AltFloat Sqrt(AltFloat value, int precision=7)
		{
		    value.ConvertToNormType();
			BasicSqrt(value, precision);
		}

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        AltFloat BasicIntPowOp(ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
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
                    AltFloat self = *this;
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
            else if (DecimalHalf == 0 && IntValue == 10 && ExtraRep == 0)
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            else if (DecimalHalf == 0 && IntValue == -10 && ExtraRep == 0)
                IntValue = expValue % 2 ? VariableConversionFunctions::PowerOfTens[expValue] : VariableConversionFunctions::PowerOfTens[expValue] * -1;
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                AltFloat self = *this;
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

        AltFloat BasicPowOp(int& expValue) { return BasicIntPowOp(expValue); }
        AltFloat BasicPowOp(signed long long& expValue) { return BasicIntPowOp(expValue); }
        AltFloat BasicPow(int expValue) { return BasicIntPowOp(expValue); }
        AltFloat BasicPow(signed long long expValue) { return BasicIntPowOp(expValue); }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static AltFloat IntPowOp(AltFloat& targetValue, ValueType& expValue)
        {
            if (value.DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    targetValue.SetAsUndefined();
#else
                    throw "Infinity to power of Zero returns Undefined value";
#endif
                else if (expValue < 0)
                    targetValue.SetAsZero();
                else if (targetValue.IntValue.Value == -1 && expValue % 2 == 0)
                    targetValue.IntValue.Value = 1;
                else
                    return targetValue;//Returns infinity
                return *this;
            }
            else
                targetValue.BasicIntPowOp(expValue);
            return targetValue;
        }

        static AltFloat PowOp(AltFloat& targetValue, int& expValue) { return IntPowOp(targetValue, expValue); }
        static AltFloat PowOp(AltFloat& targetValue, signed long long& expValue) { return IntPowOp(targetValue, expValue); }
        static AltFloat Pow(AltFloat targetValue, int expValue) { return IntPowOp(targetValue, expValue); }
        static AltFloat Pow(AltFloat targetValue, signed long long expValue) { return IntPowOp(targetValue, expValue); }

        /// <summary>
        /// Applies Power of operation on references with const expValue(for integer exponents)(C3892 fix)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        AltFloat BasicIntPowConstOp(const ValueType& expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
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
                    AltFloat self = *this;
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
            else if (DecimalHalf == 0 && IntValue == 10 && targetValue.ExtraRep == 0)
            {
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                int expVal = expValue;
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                AltFloat self = *this;
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

        AltFloat BasicPowConstOp(const int& expValue) { return BasicIntPowConstOp(expValue); }
        AltFloat BasicPowConstOp(const signed long long& expValue) { return BasicIntPowConstOp(expValue); }
        AltFloat BasicPowConst(const int expValue) { return BasicIntPowConstOp(expValue); }
        AltFloat BasicPowConst(const signed long long expValue) { return BasicIntPowConstOp(expValue); }

        /// <summary>
        /// Applies Power of operation on references with const expValue(for integer exponents)(C3892 fix)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static AltFloat IntPowConstOp(AltFloat& targetValue, const ValueType& expValue)
        {
            if (value.DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    targetValue.SetAsUndefined();
#else
                    throw "Infinity to power of Zero returns Undefined value";
#endif
                else if (expValue < 0)
                    return Zero;
                else if (value.IntValue.Value == -1 && expValue % 2 == 0)
                    IntValue.Value = 1;
                else
                    return;//Returns infinity
                return *this;
            }
            else
                targetValue.BasicIntPowConstOp(expValue);
            return *this;
        }

        AltFloat PowConstOp(AltFloat& targetValue, const int& expValue) { return IntPowConstOp(targetValue, expValue); }
        AltFloat PowConstOp(AltFloat& targetValue, const long long& expValue) { return IntPowConstOp(targetValue, expValue); }
        AltFloat PowConst(AltFloat& targetValue, const int& expValue) { return IntPowConstOp(targetValue, expValue); }
        AltFloat PowConst(AltFloat& targetValue, const long long& expValue) { return IntPowConstOp(targetValue, expValue); }


        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="nValue">The nth value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>AltFloat</returns>
        static AltFloat NthRoot(AltFloat value, int n, AltFloat precision = AltFloat::JustAboveZero)
        {
            AltFloat xPre = ((value - 1) / n) + 1;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            AltFloat delX = AltFloat(2147483647, 0);

            //  xK denotes current value of x 
            AltFloat xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = (xPre * nMinus1 + value / AltFloat::Pow(xPre, nMinus1)) / n;
                delX = AltFloat::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        static AltFloat FractionalPow(AltFloat value, int expNum, int expDenom);

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        static AltFloat FractionalPow(AltFloat& value, boost::rational<int>& Frac);

        void BasicPowOp(AltFloat& expValue);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        AltFloat PowOp(AltFloat& expValue);

        static AltFloat PowOp(AltFloat& targetValue, AltFloat& expValue)
        {
            return targetValue.PowOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="targetValue">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static AltFloat Pow(AltFloat targetValue, AltFloat expValue)
        {
            return PowOp(targetValue, expValue);
        }
	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static AltFloat NthRootV2(AltFloat targetValue, int n, AltFloat& Precision = AltFloat::FiveBillionth)
        {
            int nMinus1 = n - 1;
            AltFloat x[2] = { (AltFloat::One / n) * ((targetValue*nMinus1) + (targetValue / AltFloat::Pow(targetValue, nMinus1))), targetValue };
            while (AltFloat::Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (AltFloat::One / n) * ((x[1]*nMinus1) + (targetValue / AltFloat::Pow(x[1], nMinus1)));
            }
            return x[0];
        }

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>AltFloat</returns>
        static AltFloat Exp(AltFloat& x)
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
             *      - x: (AltFloat float) power of e to evaluate
             * Returns:
             *      - (AltFloat float) approximation of e^x in AltFloat precision
             */
             // Check that x is a valid input.
            assert(-709 <= x.IntValue && x.IntValue <= 709);
            // When x = 0 we already know e^x = 1.
            if (x.IsZero()) {
                return AltFloat::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            AltFloat x0 = AltFloat::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            AltFloat tn = AltFloat::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = AltFloat::CeilInt(x0 * AltFloat::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + AltFloat::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = AltFloat::One / tn;
            }
            return tn;
        }
protected:
    static AltFloat LnRef_Part02(AltFloat& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
        AltFloat TotalRes = (value - 1) / (value + 1);
        AltFloat LastPow = TotalRes;
        AltFloat WSquared = TotalRes * TotalRes;
        AltFloat AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > AltFloat::JustAboveZero);
        return TotalRes * 2;
    }
public:
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::AltFloat</returns>
        static AltFloat LnRef(AltFloat& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == AltFloat::One)
                return AltFloat::Zero;
            if (IntValue>=0&&IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                AltFloat threshold = AltFloat::FiveMillionth;
                AltFloat base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                AltFloat term = base;       // First term
                AltFloat prev;          // Previous sum
                AltFloat result = term;     // Kick it off

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
                } while (AltFloat::Abs(prev - result) > threshold);

                return result;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
                return LnRef_Part02(value);
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::AltFloat</returns>
        static AltFloat LnRefV2(AltFloat& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == AltFloat::One)
                return AltFloat::Zero;
            if(IntValue==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                AltFloat W = (value - 1)/ (value + 1);
                AltFloat TotalRes = W;
                W.SwapNegativeStatus();
                AltFloat LastPow = W;
                AltFloat WSquared = W * W;
                int WPow = 3;
                AltFloat AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > AltFloat::JustAboveZero);
                return TotalRes * 2;
            }
            else if (IntValue==1)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                AltFloat threshold = AltFloat::FiveMillionth;
                AltFloat base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                AltFloat term = base;       // First term
                AltFloat prev;          // Previous sum
                AltFloat result = term;     // Kick it off

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
                } while (AltFloat::Abs(prev - result) > threshold);

                return result;
            }
            else
            {
                return LnRef_Part02(value);
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        static AltFloat Ln(AltFloat value)
        {
            return LnRef(value);
        }

protected:
    static AltFloat Log10_Part02(AltFloat& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        AltFloat TotalRes = (value - 1) / (value + 1);
        AltFloat LastPow = TotalRes;
        AltFloat WSquared = TotalRes * TotalRes;
        AltFloat AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > AltFloat::JustAboveZero);
        return TotalRes * AltFloat::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        static AltFloat Log10(AltFloat value)
        {
            if (value == AltFloat::One)
                return AltFloat::Zero;
            if (DecimalHalf == 0 && IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return AltFloat(index, 0);
                }
                return AltFloat(9, 0);
            }
            if (IntValue>=0&&IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                AltFloat threshold = AltFloat::FiveBillionth;
                AltFloat base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                AltFloat term = base;       // First term
                AltFloat prev = 0;          // Previous sum
                AltFloat result = term;     // Kick it off

                while (AltFloat::Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return result*AltFloat::LN10Mult;// result/AltFloat::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return Log10_Part02(value);
            }
        }

protected:
    template<typename ValueType>
    static AltFloat Log10_IntPart02(ValueType& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        AltFloat TotalRes = AltFloat((value - 1), 0) / AltFloat((value + 1), 0);
        AltFloat LastPow = TotalRes;
        AltFloat WSquared = TotalRes * TotalRes;
        AltFloat AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > AltFloat::JustAboveZero);
        return TotalRes * AltFloat::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        template<typename ValueType>
        static AltFloat Log10(ValueType value)
        {
            if (value == 1)
                return AltFloat::Zero;
            if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return AltFloat(index, 0);
                }
                return AltFloat(9, 0);
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return Log10_IntPart02(value);
            }
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>AltFloat</returns>
        static AltFloat Log(AltFloat value, AltFloat baseVal)
        {
            if (ConvertedVal == AltFloat::One)
                return AltFloat::Zero;
            return Log10(value) / Log10(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>AltFloat</returns>
        static AltFloat Log(AltFloat value, int baseVal)
        {
            //Calculate Base log first
            AltFloat baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = AltFloat(index, 0);
                        break;
                    }
                }
                baseTotalRes = AltFloat(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = AltFloat((baseVal - 1), 0) / AltFloat((baseVal + 1), 0);
                AltFloat baseLastPow = baseTotalRes;
                AltFloat baseWSquared = baseTotalRes * baseTotalRes;
                AltFloat baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > AltFloat::JustAboveZero);
            }

            //Now calculate other log
            if (ConvertedVal.DecimalHalf == 0 && ConvertedVal.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? AltFloat(index, 0) / (baseTotalRes * AltFloat::HalfLN10Mult): AltFloat(index, 0)/ baseTotalRes;
                }
                return lnMultLog? AltFloat(9, 0) / (baseTotalRes*AltFloat::HalfLN10Mult):AltFloat(9, 0)/baseTotalRes;
            }
            if (ConvertedVal.IntValue>=0&&ConvertedVal.IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                AltFloat threshold = AltFloat::FiveBillionth;
                AltFloat base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                AltFloat term = base;       // First term
                AltFloat prev = 0;          // Previous sum
                AltFloat result = term;     // Kick it off

                while (AltFloat::Abs(prev - result) > threshold) {
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
                AltFloat W = (value - 1) / (value + 1);
                AltFloat TotalRes = W;
                AltFloat AddRes;
                int WPow = 3;
                do
                {
                    AddRes = AltFloat::Pow(W, WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > AltFloat::JustAboveZero);
                return lnMultLog? TotalRes/baseTotalRes:(TotalRes * AltFloat::HalfLN10Mult)/ baseTotalRes;
            }
            //return Log10(Value) / Log10(BaseVal);
        }
	#pragma endregion Log Functions

    #pragma region Trigonomic Etc Functions
        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        static AltFloat SinFromAngle(AltFloat Value)
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
                    Value.IntValue = 360 - Value.IntValue;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return AltFloat::Zero; }
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
                return AltFloat::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else
            {
                AltFloat NewValue = Zero;
                //Angle as Radian
                AltFloat Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's Taylor series!!
                    NewValue += AltFloat::Pow(Radius, 2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
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
        static AltFloat CosFromAngle(AltFloat Value)
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
                    Value.IntValue = 360 - Value.IntValue;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return One; }
            else if (Value.IntValue == 60 && Value.DecimalHalf == 0)
            {
                return PointFive;
            }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
                return AltFloat::Zero;
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return AltFloat::Zero;
            }
            else
            {
                AltFloat NewValue = Zero;
                //Angle as Radian
                AltFloat Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's also Taylor series!!
                    NewValue += AltFloat::Pow(Radius, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
                }
                return NewValue;
            }
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>AltFloat</returns>
        //static AltFloat BasicSinOperation(AltFloat& Value);

       /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>AltFloat</returns>
        static AltFloat Sin(AltFloat Value)
        {
            AltFloat SinValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += AltFloat::Pow(Value, 2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>AltFloat</returns>
        //static AltFloat BasicCosOperation(AltFloat& Value);

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>AltFloat</returns>
        static AltFloat Cos(AltFloat Value)
        {
            AltFloat CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                CosValue += AltFloat::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>AltFloat</returns>
        static AltFloat Tan(AltFloat Value)
        {
            AltFloat SinValue = Zero;
            AltFloat CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += AltFloat::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1)  / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            {
                CosValue += AltFloat::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        static AltFloat TanFromAngle(AltFloat Value)
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
                    Value.IntValue = 360 - Value.IntValue;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return AltFloat::Zero; }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
                return AltFloat::Maximum;//Positive Infinity
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return AltFloat::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return AltFloat::Minimum;//Negative Infinity
            }
            else
            {
                return Tan(Pi * Value / 180);
            }
        }

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltFloat</returns>
        static AltFloat ATan(AltFloat Value)
        {
            AltFloat SinValue = Zero;
            AltFloat CosValue = Zero;
            //Angle as Radian
            for (int i = 0; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += AltFloat::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            { // That's also Taylor series!!
                CosValue += AltFloat::Pow(Value, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
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
        /// <returns>AltFloat</returns>
        static AltFloat ArcTan2(AltFloat y, AltFloat x)
        {
            AltFloat coeff_1 = PiNum / 4;
            AltFloat coeff_2 = coeff_1 * 3;
            AltFloat abs_y = AltFloat::Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            AltFloat r;
            AltFloat angle;
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

	MirroredInt AltFloat::NegativeRep = MirroredInt::NegativeZero;
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproachingValues)
    AltFloat AltFloat::AlmostOne = ApproachingRightRealValue();
#endif
    AltFloat AltFloat::Pi = PiNumValue();
    AltFloat AltFloat::One = OneValue();
    AltFloat AltFloat::Two = TwoValue();
    AltFloat AltFloat::NegativeOne = NegativeOneValue();
    AltFloat AltFloat::Zero = ZeroValue();
    AltFloat AltFloat::PointFive = Point5Value();
    AltFloat AltFloat::JustAboveZero = JustAboveZeroValue();
    AltFloat AltFloat::OneMillionth = OneMillionthValue();
    AltFloat AltFloat::FiveThousandth = FiveThousandthValue();
    AltFloat AltFloat::Minimum = MinimumValue();
    AltFloat AltFloat::Maximum = MaximumValue();
    AltFloat AltFloat::E = ENumValue();
    AltFloat AltFloat::LN10 = LN10Value();
    AltFloat AltFloat::LN10Mult = LN10MultValue();
    AltFloat AltFloat::HalfLN10Mult = HalfLN10MultValue();
    AltFloat AltFloat::TenMillionth = TenMillionthValue();
    AltFloat AltFloat::FiveMillionth = FiveMillionthValue();
    AltFloat AltFloat::FiveBillionth = FiveBillionthValue();
    AltFloat AltFloat::OneGMillionth = OneHundredMillionthValue();
    #if defined(AltNum_EnableNilRep)
    AltFloat AltFloat::Nil = NilValue();
    #endif

    AltFloat AltFloat::PiNum = PiNumValue();
    AltFloat AltFloat::ENum = ENumValue();
    
#if defined(AltNum_EnableNaN)
    AltFloat AltFloat::NaN = NaNValue();
	AltFloat AltFloat::Undefined = UndefinedValue();
#endif
    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void AltFloat::ReadString(std::string Value)
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
        {
            if (IntValue == 0) { IntValue = NegativeRep; }
            else { IntValue *= -1; }
        }
    }

    /// <summary>
    /// Gets the value from string.
    /// </summary>
    /// <param name="Value">The value.</param>
    /// <returns>AltFloat</returns>
    inline AltFloat AltFloat::GetValueFromString(std::string Value)
    {
        AltFloat NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

	std::string AltFloat::BasicToStringOp()
	{
        std::string Value = (std::string)IntValue;
        if (DecimalHalf != 0)
        {
			unsigned __int8 CurrentDigit;
			std::string DecBuffer = "";
            Value += ".";
            int CurrentSection = DecimalHalf;
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

    std::string AltFloat::ToString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
	#if defined(AltNum_EnableInfinityRep)
        case RepType::PositiveInfinity:
            return "∞";
            break;
        case RepType::NegativeInfinity:
            return "-∞";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToStringOp();
			#else
            return (std::string)IntValue + ".0..1";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp();
			#else
            return (std::string)IntValue + ".9..9";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingMidRight:
        case RepType::ApproachingMidLeft:
            ConvertToNormType(repType);
			return BasicToStringOp();
			break;
            #endif
        #endif
	#endif
    #if defined(AltNum_EnableFractionals)
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
    #endif
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            return BasicToStringOp()+"π";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"π";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToStringOp()+"e";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
            break;
        #endif
	#endif

	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            return BasicToStringOp()+"i";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"π";
			#else
            return (std::string)IntValue + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"e";
			#else
            return (std::string)IntValue + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::PositiveImaginaryInfinity:
            return "∞i";
            break;
        case RepType::NegativeImaginaryInfinity:
            return "-∞i";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToStringOp()+"i";
			#else
            return (std::string)IntValue + ".0..1i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"i";
			#else
            return (std::string)IntValue + ".9..9i";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingImaginaryMidRight:
        case RepType::ApproachingImaginaryMidLeft:
            ConvertToNormType(repType);
			return BasicToStringOp()+"i";
			break;
        #endif
            #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
		#elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
		#elif defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
		#endif
    #endif
	#if defined(AltNum_EnableNaN)
        case RepType::Undefined:
            return "Undefined";
        case RepType::NaN:
            return "NaN";
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
        case UndefinedButInRange:
            return "UndefinedButInRange";
            break;
		#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
        case WithinMinMaxRange:
		    return "WithinMinMaxRange";
            break;
        #endif
	#endif
    #if defined(AltNum_EnableNilRep)
        case RepType::Nil:
            return "Nil";
    #endif
        default:
			ConvertToNormType(repType);
			return BasicToStringOp();
            break;
        }
    }

	std::string AltFloat::BasicToFullStringOp()
	{
        std::string Value = (std::string)IntValue;
        if (DecimalHalf != 0)
        {
			unsigned __int8 CurrentDigit;
            Value += ".";
            bool HasDigitsUsed = false;
            int CurrentSection = DecimalHalf;
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

    std::string AltFloat::ToFullString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
	#if defined(AltNum_EnableInfinityRep)
        case RepType::PositiveInfinity:
            return "∞";
            break;
        case RepType::NegativeInfinity:
            return "-∞";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToFullStringOp();
			#else
            return (std::string)IntValue + ".0..1";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp();
			#else
            return (std::string)IntValue + ".9..9";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingMidRight:
        case RepType::ApproachingMidLeft:
            ConvertToNormType(repType);
			return BasicToFullStringOp();
			break;
            #endif
        #endif
	#endif
    #if defined(AltNum_EnableFractionals)
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
    #endif
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            return BasicToFullStringOp()+"π";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"π";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToFullStringOp()+"e";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
            break;
        #endif
	#endif

	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            return BasicToFullStringOp()+"i";
            break;
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
            return BasicToFullStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
            break;
        #endif
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"π";
			#else
            return (std::string)IntValue + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"e";
			#else
            return (std::string)IntValue + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::PositiveImaginaryInfinity:
            return "∞i";
            break;
        case RepType::NegativeImaginaryInfinity:
            return "-∞i";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToFullStringOp()+"i";
			#else
            return (std::string)IntValue + ".0..1i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"i";
			#else
            return (std::string)IntValue + ".9..9i";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingImaginaryMidRight:
        case RepType::ApproachingImaginaryMidLeft:
            ConvertToNormType(repType);
			return BasicToFullStringOp()+"i";
			break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
		#elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
		#elif defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
		#endif
    #endif
	#if defined(AltNum_EnableNaN)
        case RepType::Undefined:
            return "Undefined";
        case RepType::NaN:
            return "NaN";
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
        case UndefinedButInRange:
            return "UndefinedButInRange";
            break;
		#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
        case WithinMinMaxRange:
		    return "WithinMinMaxRange";
            break;
        #endif
	#endif
    #if defined(AltNum_EnableNilRep)
        case RepType::Nil:
            return "Nil";
    #endif
        default:
			ConvertToNormType(repType);
			return BasicToFullStringOp();
            break;
        }
    }
    #pragma endregion String Function Source

#if defined(AltNum_EnableAlternativeModulusResult)
    /// <summary>
    /// (AltFloat Version)Performs remainder operation then saves division result
    /// C = A - B * (A / B)
    /// </summary>
    class DLL_API AltModChecker : public AltNumModChecker<AltFloat>
    {
    };
#endif
}