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

#include "..\IntegerConcept.hpp"
#include "..\MediumDec\MediumDec.hpp"
#include "..\PartialDec\PartialDec.hpp"//Used to keep all digits while dividing my two
#if defined(FixedUFloat_UseQuadRange)
	#include "..\PartialDec\FloatingQuadRangeDec.hpp"
	#define FixedUFloat_SignifType FloatingQuadRangeDec
#else
	#include "..\PartialDec\FloatingDuoRangeDec.hpp"
	#define FixedUFloat_SignifType FloatingDuoRangeDec
#endif
//Int 128 needed to extract trailing digits lost from division and multiplication
#include <boost/multiprecision/cpp_int.hpp>
/*
FixedUFloat_ExtendedRange = Extends SignifNum range to 2147483647
FixedUFloat_EnableApproachingZero = Not Implimented yet
FixedUFloat_EnableInfinity = Not Implimented yet
FixedUFloat_EnableBitwiseOperations = Not Implimented yet
FixedUFloat_UseXorAsPowerOf = Not Implimented yet
FixedUFloat_UseQuadRange
FixedUFloat_GiveErrorInsteadOfMaxingOnOverflowConversion
*/


namespace BlazesRusCode
{
	//Right side operations are assumed to be positive
	//^ in comments refers to power of instead of XOR

    class FixedUFloat;
	using UInt128 = boost::multiprecision::uint128_t;

    /// <summary>
    /// Alternative fixed point number representation designed for use with MixedDec
	#if defined(FixedUFloat_UseQuadRange)
	/// Floating formula representation is 4^Exp + 0-3.XXXXXXXXX*4^Exp;
	#else
	/// Floating formula representation is 2^Exp + 0-1.XXXXXXXXX*2^Exp;
	#endif
    /// (5 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API FixedUFloat
    {
	protected:
		#pragma region DigitStorage
		
	#if defined(FixedUFloat_UseQuadRange)
		//SignifNum inside "4^Exp + SignifNum*4^Exp" representation
	#else
		//SignifNum inside "2^Exp + SignifNum*2^Exp" representation
	#endif
		FixedUFloat_SignifType SignifNum;
        //Refers to Exp inside formula
		//If Exp==-128 and SignifNum==0, in which case the value is at zero
		signed char Exp;
		
		#pragma endregion DigitStorage

		//Exp value that zero is defined at
		static signed char ZeroRep = -128;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="FixedUFloat"/> class.
        /// </summary>
        FixedUFloat(const FixedUFloat_SignifType& signifNum=0, const signed char& exp=ZeroRep)
        {
            SignifNum = signifNum;
            Exp = exp;
        }

        FixedUFloat(const FixedUFloat&) = default;

        FixedUFloat& operator=(const FixedUFloat&) = default;

		bool IsNegative()
		{
			return false;
		}

		bool IsPositive()
		{
			return true;
		}
	
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus(){}

        //Detect if at exactly zero
		bool IsZero()
		{
            return SignifNum==0&&Exp==ZeroRep;
		}

        //Detect if at exactly one
		bool IsOne()
		{
            return SignifNum==0&&Exp==0;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(FixedUFloat Value)
        {
			//A
        }

        void SetAsZero()
        {
            SignifNum = 0;
            Exp = ZeroRep;
        }

        void SetAsOne()
        {
            SignifNum = 0;
            Exp = 0;
        }

    #pragma region Const Representation values
    protected:
        static FixedUFloat_SignifType AlmostApproachingTop;
	
		static unsigned _int64 const TruncMultAsInt = 10000000000000000000;//10 000 000 000 000 000 000
		//Size of this value determines how much of the truncated digits to save (19 digits of truncated digits stored by default)
		static UInt128 const TruncMult = TruncMultAsInt;
		static unsigned _int64 const SubExp1Range = 5000000000000000000;//TruncMultAsInt/2;
		static unsigned _int64 const SubExp2Range = 2500000000000000000;//SubExp1Range/2;
		static unsigned _int64 const SubExp3Range = 1250000000000000000;//SubExp2Range/2;
		static unsigned _int64 const SubExp4Range = 625000000000000000;//SubExp3Range/2;
		static unsigned _int64 const SubExp5Range = 312500000000000000;//SubExp4Range/2;
		//After SubExp19Range the value becomes non-whole number
    #pragma endregion Const Representation values
	public:
	
        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        {
			//Add Code here
        }

    #pragma region ApproachingZero Setters

        /// <summary>
        /// Sets value as smallest real number that is approaching zero
        /// </summary>
        void SetAsSmallestNonZero()
        {
			//Add Code here
        }

    #pragma endregion ApproachingZero Setters

    #pragma region ValueDefines
protected:
        static FixedUFloat ZeroValue()
        {
            return FixedUFloat();
        }
		
        static FixedUFloat OneValue()
        {
	#if defined(FixedUFloat_UseQuadRange)
			//Add Code here
	#else
			//Add Code here
	#endif
        }
		
        static FixedUFloat TwoValue()
        {
	#if defined(FixedUFloat_UseQuadRange)
			//Add Code here
	#else
			//Add Code here
	#endif
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>FixedUFloat</returns>
        static FixedUFloat Point5Value()
        {
	#if defined(FixedUFloat_UseQuadRange)
			//Add Code here
	#else
			//Add Code here
	#endif
        }

        static FixedUFloat JustAboveZeroValue()
        {
	#if defined(FixedUFloat_UseQuadRange)
			//Add Code here
	#else
			//Add Code here
	#endif
        }

        static FixedUFloat MaximumValue()
        {
            return AltFloat(AlmostApproachingTop, 127);
        }

public:

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>FixedUFloat</returns>
        static FixedUFloat Zero;
		
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>FixedUFloat</returns>
        static FixedUFloat One;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>FixedUFloat</returns>
        static FixedUFloat NegativeOne;
		
        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>FixedUFloat</returns>
        static FixedUFloat Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>FixedUFloat</returns>
        static FixedUFloat PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDec</returns>
        static FixedUFloat JustAboveZero;

        /// <summary>
        /// Returns value of lowest non-infinite value that can stored
        /// </summary>
        static FixedUFloat Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite value that can stored
        /// </summary>
        static FixedUFloat Maximum;

    #pragma endregion ValueDefines

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus(){}

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
        /// <returns>FixedUFloat</returns>
        FixedUFloat GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="FixedUFloat"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        FixedUFloat(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FixedUFloat"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FixedUFloat(std::string Value)
        {
            this->ReadString(Value);
        }

		//Outputs string in "2^Exp + " format 
		std::string ToFormulaFormat()
		{
			if(IsZero())
				return "0";
			else if(IsOne())
				return "1";
            //Add code here later
		}

		//Outputs string in digit display format 
		std::string ToDigitFormat()
		{
			if(IsZero())
				return ".0";
			else
			{
				//To-Do:Add code here
			}
			return "";//placeholder
		}

  //      /// <summary>
  //      /// Converts to string.
  //      /// </summary>
  //      /// <returns>std.string</returns>
  //      std::string ToString()
  //      {
  //          //Add code here later
  //          return ToDigitFormat();//placeholder
  //      }

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToFormulaFormat(); }

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

        void SetFloatVal(const float& Value)
        {//Should be able to extract the related similar bits
            //To-Do Add code here
        }

        void SetDoubleVal(const double& Value)
        {
            //To-Do Add code here
        }

        void SetDecimalVal(const ldouble& Value)
        {
            //To-Do Add code here
        }

        void SetBoolVal(const bool& Value)
        {
            if(Value==true)
                SetAsOne();
            else
                SetAsZero();
        }

        //Use Bitwise operations to convert fixed point into Formula Format
		//https://hackernoon.com/bit-manipulation-in-c-and-c-1cs2bux

        void SetUIntVal(const unsigned int& Value)
        {
            //Add code here later
        }

        void SetIntVal(const signed int& Value)
        {
            //Add code here later
        }

        FixedUFloat(const unsigned int& Value)
        {
            this->SetUIntVal(Value);
        }

        FixedUFloat(const signed int& Value)
        {
            this->SetIntVal(Value);
        }

        FixedUFloat(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        FixedUFloat(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        FixedUFloat(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        FixedUFloat(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

        FixedUFloat(const MediumDec& Value)
        {
            this->SetMediumDecVal(Value);
        }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

        float toFloat()
        {
            float Value;
            //Add code here later
            return 0.0f;//placeholder //Value;
        }

        /// <summary>
        /// FixedUFloat to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float()
        {
            return toFloat();
        }

        float toDouble()
        {
            double Value;
            //Add code here later
            return 0.0;//plaheholder //Value;
        }

        /// <summary>
        /// FixedUFloat to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double()
        {
            return toFloat();
        }

        template<IntegerType IntType=int>
        IntType toIntType()
        {
            if(IsZero())
                return 0;
            else if(IsOne()))
                return 1;
            //Add code here later
            return 0;//Placeholder;
        }

        /// <summary>
        /// FixedUFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int()
        {
            //Add code here later
        }

        /// <summary>
        /// FixedUFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int()
        {
            //Add code here later
        }

        /// <summary>
        /// FixedUFloat to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed long long()
        {
            //Add code here later
        }

        /// <summary>
        /// FixedUFloat to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned long long()
        {
            //Add code here later
        }

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
	//https://www.foonathan.net/2018/09/three-way-comparison/

    std::strong_ordering operator<=>(const FixedUFloat& that) const
    {
        //Add code here later
    }
	
    bool operator==(const FixedUFloat& that) const
    {
        //Add code here later
		return true;
    }
	
    auto operator<=>(const int& that) const
    {
		FixedUFloat rVal = (FixedUFloat)that;
		return this <=> rValue;
    }

    bool operator==(const int& that) const
    {
		FixedUFloat rVal = (FixedUFloat)that;
		return this==rValue;
    }

    #pragma endregion Comparison Operators

	#pragma region Trailing Digit Extraction
	
		unsigned int FindSignifNumFromRem(const _int64& TruncatedDigits, const PartialDec& RangeLimit)
		{
			//To-Do:Find SignifNum
			boost::rational<unsigned _int64>(TruncatedDigits, TruncMultAsInt) Frac;
			//Find which percentage more of RangeLimit that the exact fraction is
		}
		
		void SetTrailingDigitFromRem(const _int64& TruncatedDigits)
		{//Negative Exponent values for FixedUFloat and positive Exponent values for RestrictedFloat
			IsPositive = 0;
			if(TruncatedDigits==SubExp1Range){//Exactly 0.5 Remainder
				Exponent.Value = 1;

			}
			else if(TruncatedDigits>SubExp1Range){
				Exponent.Value = 1;

			}
			else if(TruncatedDigits==SubExp2Range){//Exactly 0.25 Remainder
				Exponent.Value = 2;

			}
			else if(TruncatedDigits>SubExp2Range){
				Exponent.Value = 2;

			}
			else if(TruncatedDigits==SubExp3Range){//Exactly 0.125 Remainder
				Exponent.Value = 3;

			}
			else if(TruncatedDigits>SubExp3Range){

			}
			else if(TruncatedDigits==SubExp4Range){
				Exponent.Value = 4;

			}
			else if(TruncatedDigits>SubExp4Range){
				Exponent.Value = 4;

			}
			else
			{
				//Automatically cyclying through the exponent ranges
				PartialDec TruncatedDigitsAsAltDec = TruncatedDigits;
				PartialDec RangeLimit = SubExp5Range;
				for(unsigned int Exp = 5;TruncatedDigits>RangeLimit;++Exp)
				{
					if(TruncatedDigits==RangeLimit)
					{
						Exponent.Value = Exp;

						return;
					}
					else if(TruncatedDigits>RangeLimit)
					{
						Exponent.Value = Exp;

						return;
					}
					RangeLimit.DivideByTwo();
				}
			}
		}
	
	#pragma endregion Trailing Digit Extraction

	protected:
	#pragma region Division Operations
        template<IntegerType IntType=int>
        void DivByIntOp(const IntType& rValue)
		{
			//Add code here later
		}
		
        //Division by FixedUFloat operation
        void DivOp(const FixedUFloat& rValue)
		{
			//Add code here later
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void DivByMediumDecVOp(const VariantType& rValue)
		{
			//Add code here later
		}
	#pragma endregion Division Operations

	#pragma region Multiplication Operations
        //Multiply by Integer Operation
        template<IntegerType IntType=int>
        void MultByIntOp(const IntType& rValue)
		{
			//Add code here later
		}
		
        //Multiply operation
        void MultOp(const FixedUFloat& rValue)
		{
			//Add code here later
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void MultByMediumDecVOp(const VariantType& rValue)
		{
			//Add code here later
		}
	#pragma endregion Multiplication Operations

	#pragma region Addition Operations
        //Addition by Integer Operation
        template<IntegerType IntType=int>
        void AddByIntOp(const IntType& rValue)
		{
			//Add code here later
		}
		
        //Addition by FixedUFloat Operation
        void AddOp(const FixedUFloat& rValue)
		{
			//Add code here later
		}
		
        //Addition by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void AddByMediumDecVOp(const VariantType& rValue)
		{
			//Add code here later
		}
	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations
        //Subtraction by Integer Operation
        template<IntegerType IntType=int>
        void SubtractByIntOp(const IntType& rValue)
		{
			//Add code here later
		}
		
        //Subtraction by FixedUFloat Operation
        void SubtractOp(const FixedUFloat& rValue)
		{
			//Add code here later
		}
		
        //Subtraction by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void SubtractByMediumDecVOp(const VariantType& rValue)
		{
			//Add code here later
		}
	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations
		
	#pragma endregion Modulus Operations
	
	public:
    #pragma region Main Operator Overrides
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FixedUFloat</returns>
        friend FixedUFloat operator/(FixedUFloat self, FixedUFloat Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FixedUFloat</returns>
        friend FixedUFloat& operator/=(FixedUFloat& self, FixedUFloat Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FixedUFloat</returns>
        friend FixedUFloat operator*(FixedUFloat self, FixedUFloat Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>FixedUFloat</returns>
        friend FixedUFloat& operator*=(FixedUFloat& self, FixedUFloat Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FixedUFloat</returns>
        friend FixedUFloat operator+(FixedUFloat self, FixedUFloat Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FixedUFloat</returns>
        friend FixedUFloat& operator+=(FixedUFloat& self, FixedUFloat Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FixedUFloat</returns>
        friend FixedUFloat operator-(FixedUFloat self, FixedUFloat Value) { return SubtractOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FixedUFloat</returns>
        friend FixedUFloat& operator-=(FixedUFloat& self, FixedUFloat Value) { return SubtractOp(self, Value); }

        friend AltDec operator/(FixedUFloat self, int Value) { return DivByIntOp(self, Value); }
        friend AltDec& operator/=(FixedUFloat& self, int Value) { return DivByIntOp(self, Value); }
		
        friend AltDec operator*(FixedUFloat self, int Value) { return MultByIntOp(self, Value); }
        friend AltDec& operator*=(FixedUFloat& self, int Value) { return MultByIntOp(self, Value); }

        friend AltDec operator+(FixedUFloat self, int Value) { return AddByIntOp(self, Value); }
        friend AltDec& operator+=(FixedUFloat& self, int Value) { return AddByIntOp(self, Value); }
		
        friend AltDec operator-(FixedUFloat self, int Value) { return SubtractByIntOp(self, Value); }
        friend AltDec& operator-=(FixedUFloat& self, int Value) { return SubtractByIntOp(self, Value); }
		
        friend AltDec operator/(FixedUFloat self, MediumDec Value) { return DivByMediumDecVOp(self, Value); }
        friend AltDec& operator/=(FixedUFloat& self, MediumDec Value) { return DivByMediumDecVOp(self, Value); }
		
        friend AltDec operator*(FixedUFloat self, MediumDec Value) { return MultByMediumDecVOp(self, Value); }
        friend AltDec& operator*=(FixedUFloat& self, MediumDec Value) { return MultByMediumDecVOp(self, Value); }

        friend AltDec operator+(FixedUFloat self, MediumDec Value) { return AddByMediumDecVOp(self, Value); }
        friend AltDec& operator+=(FixedUFloat& self, MediumDec Value) { return AddByMediumDecVOp(self, Value); }
		
        friend AltDec operator-(FixedUFloat self, MediumDec Value) { return SubtractByMediumDecVOp(self, Value); }
        friend AltDec& operator-=(FixedUFloat& self, MediumDec Value) { return SubtractByMediumDecVOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma region Other Operators

        /// <summary>
        /// ++FixedUFloat Operator
        /// </summary>
        /// <returns>FixedUFloat &</returns>
        FixedUFloat& operator ++()
        {
            *this = *this + One;
            return *this;
        }

        /// <summary>
        /// --FixedUFloat Operator
        /// </summary>
        /// <returns>FixedUFloat &</returns>
        FixedUFloat& operator --()
        {
            *this = *this - One;
            return *this;
        }

        /// <summary>
        /// FixedUFloat++ Operator
        /// </summary>
        /// <returns>FixedUFloat</returns>
        FixedUFloat operator ++(int)
        {
            FixedUFloat tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// FixedUFloat-- Operator
        /// </summary>
        /// <returns>FixedUFloat</returns>
        FixedUFloat operator --(int)
        {
            FixedUFloat tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// FixedUFloat* Operator
        /// </summary>
        /// <returns>FixedUFloat &</returns>
        FixedUFloat& operator *()
        {
            return *this;
        }
    #pragma endregion Other Operators

	#pragma region Modulus Operations
        template<IntegerType IntType=int>
        void ModulusByIntOp(const IntType& rValue)
		{
		}
		
        void ModulusOp(const FixedUFloat& rValue)
		{
		}

        friend FixedUFloat operator%(FixedUFloat& self, int Value) { return ModulusByIntOp(self, Value); }
        friend FixedUFloat operator%(FixedUFloat& self, signed long long Value) { return ModulusByIntOp(self, Value); }

        friend FixedUFloat operator%=(FixedUFloat& self, int Value) { return ModulusByIntOp(self, Value); }
        friend FixedUFloat operator%=(FixedUFloat& self, signed long long Value) { return ModulusByIntOp(self, Value); }

        friend FixedUFloat operator%=(FixedUFloat* self, int Value) { return ModulusByIntOp(**self, Value); }
        friend FixedUFloat operator%=(FixedUFloat* self, signed long long Value) { return ModulusByIntOp(**self, Value); }
        
        friend FixedUFloat operator%(FixedUFloat self, unsigned __int64 Value) { return ModulusByIntOp(self, Value); }
    
	#pragma endregion Modulus Operations

    #pragma region Bitwise Functions

    #pragma endregion Bitwise Functions

	#pragma region Math Etc Functions

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDec &</returns>
        FixedUFloat& Trunc()
        {
            signed long long ValueAsInt = (signed long long)*this;
			SetInt64Value(ValueAsInt);
            return *this;
        }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        static FixedUFloat Trunc(FixedUFloat Value)
        {
            return Value.Trunc();
        }

	#pragma endregion Math Etc Functions

    };

    #pragma region ValueDefine Source

    FixedUFloat_SignifType FixedUFloat::AlmostApproachingTop = FixedUFloat_SignifType::Maximum();

    FixedUFloat FixedUFloat::Zero = ZeroValue();
    FixedUFloat FixedUFloat::One = OneValue();

    FixedUFloat FixedUFloat::NegativeOne = NegativeOneValue();
    FixedUFloat FixedUFloat::Two = TwoValue();

    FixedUFloat FixedUFloat::JustAboveZero = JustAboveZeroValue();

    FixedUFloat FixedUFloat::Maximum = MaximumValue();

    FixedUFloat FixedUFloat::PointFive = Point5Value();

    #pragma endregion ValueDefine Source

    #pragma region String Function Source

    #pragma endregion String Function Source
}
	