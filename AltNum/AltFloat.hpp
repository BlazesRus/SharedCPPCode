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
#include "..\MediumDec\MediumDec.hpp"
/*
AltFloat_IncludeFixedPoint = Not Implimented yet
AltFloat_IncludeFractionRepresentation = Requires AltFloat_IncludeFixedPoint as well
AltFloat_UseLeadingZeroInSignificant
AltFloat_TreatZeroAsZeroExponent
AltFloat_ExtendedRange = Extends SignifNum range to 2147483647
AltFloat_EnableApproachingZero = Not Implimented yet
AltFloat_EnableInfinity = Not Implimented yet
*/
#if defined(AltFloat_UseLeadingZeroInSignificant) && !defined(AltFloat_TreatZeroAsZeroExponent)
	#define AltFloat_TreatZeroAsZeroExponent
#endif

namespace BlazesRusCode
{
	//Right side operations are assumed to be positive
	//^ in comments refers to power of instead of XOR

    class AltFloat;

    /// <summary>
    /// Alternative fixed point number representation designed for use with MixedDec
#if AltFloat_IncludeFixedPoint
    /// When SignifNum is negative, AltFloat is in in fixed point mode and represents range of 0 to 9.9999999 x 10^127
	#if AltFloat_ExtendedRange
	//Represents floating range between 0 and (1+(2147483647/2147483648))*2^127
	#else
	//Represents floating range between 0 and (1+(8388607/8388608))*2^127
	#endif
#elif AltFloat_ExtendedRange//Extended range can represent more digits of decimal places 
	//Represents floating range between 0 and (1+(2147483647/2147483648))*2^127(Approximately 340 282 366 841 710 300 949 110 269 838 224 261 120)
#else
	//Represents floating range between 0 and (1+(8388607/8388608))*2^127      (Approximately 340 282 346 638 528 859 811 704 183 484 516 925 440)
#endif
	// Floating formula representation is SignifNum>=0?"(1+(SignifNum/DenomMax))*2^Exponent":"(1+(-SignifNum/DenomMax))*2^Exponent";
	// Floating range maximum at "(1+(AlmostApproachingOne/DenomMax))*2^127"
	// Which in scientific notation is equal to 3.40282 x 10^38 (same approximate range as float maximum)
	// When Exponent<0, floating formula can also be represented as: "(1+(SignifNum/DenomMax))*(1/(2^-Exponent))"
	// Floating formula representation when Exponent is < 0 also equivalant to "(1+(AlmostApproachingOne/DenomMax))*2^Exponent"
    /// (5 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API AltFloat
    {
	protected:
		//Keeping DenomMax as a power of 2 to make easier to calculate into Int equalant etc
#if defined(AltFloat_ExtendedRange)
		//Equal to 2^31
		static signed long long DenomMax = 2147483648;
		//Equal to (2^31) - 1
		static signed long long AlmostApproachingTop = 2147483647;
		static signed int DenomMaxExponent = 31;
#else
		//Equal to 2^23
		static signed int DenomMax = 8388608;
		//Equal to (2^23) - 1
		static signed int AlmostApproachingTop = 8388607;
		static signed int DenomMaxExponent = 23;
#endif
		static unsigned long long uDenomMax = (unsigned long long) DenomMax;
		static signed long long sDenomMax = (signed long long) DenomMax;
		//Largest Exponent side calculation(2^127):170141183460469231731687303715884105728

#if defined(AltFloat_TreatZeroAsZeroExponent)
#if !defined(AltFloat_EnableApproachingZero) || defined(AltFloat_EnableInfinity)
		//Special Status representations not enabled if AltFloat_TreatZeroAsZeroExponent is not toggled for now(unless exponent range is reduced)
		//If Exponent==-128 and SignifNum!=-128, then represents approaching zero range with SignifNum representing 
		static signed int SpecialStatusRep = -128;
#endif
#if AltFloat_EnableInfinity
		//If Exponent==-128 and SignifNum==-128, then represents infinity range with SignifNum==1 when positive and SignifNum==-1 when negative 
		static signed int InfinityRep = -128;
#endif
#else
	#if defined(AltFloat_EnableApproachingZero)
		//When SignifNum==-128 and Exponent!=-128, than value is equal to Exponent.0..01
		//When SignifNum==-128 and Exponent==-128, than value is equal to -0.0..01
		static signed int ApproachingZeroRep = -128;
	#endif
#endif

		//Exponent value that zero is defined at
#if !defined(AltFloat_TreatZeroAsZeroExponent)
		static signed char ZeroRep = -128;
#else
		static signed char ZeroRep = 0;
#endif
 
		//Only 3 Bytes of this is actually used unless AltFloat_ExtendedRange is toggled (Value is the Numberator/8388608)
		//If AltFloat_ExtendedRange is enabled, Numerator can fill to max of int 32 with denominator of 2147483648.
		signed int SignifNum;

#if AltFloat_IncludeFixedPoint
        //In fixed point mode refers to the Exponent of 10 in "(SignifNum first 2 bytes).(SignifNum last 2 bytes)) * 10^Exponent" formula
	#if !defined(AltFloat_EnableApproachingZero&&!defined(AltFloat_EnableInfinity)
        //Otherwise in floating point mode, refers to Exponent inside "Significant * 2^Exponent" formula
		#if !defined(AltFloat_TreatZeroAsZeroExponent)
		//Unless Exponent==-128 and SignifNum==0, in which case the value is at zero
		#endif
	#elif defined(AltFloat_TreatZeroAsZeroExponent)
	#endif
#else
	#if !defined(AltFloat_EnableApproachingZero&&!defined(AltFloat_EnableInfinity)
        //Otherwise in floating point mode, refers to Exponent inside "Significant * 2^Exponent" formula
		#if !defined(AltFloat_TreatZeroAsZeroExponent)
		//Unless Exponent==-128 and SignifNum==0, in which case the value is at zero
		#endif
	#elif defined(AltFloat_TreatZeroAsZeroExponent)
	#endif
#endif
		signed char Exponent;
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        AltFloat(signed int signifNum=0, signed char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            Exponent = exponent;
        }

        AltFloat(const AltFloat&) = default;

        AltFloat& operator=(const AltFloat&) = default;

        //Detect if at exactly zero
		bool IsZero()
		{
#if !defined(AltFloat_TreatZeroAsZeroExponent)
            return SignifNum==0&&Exponent==-128;
#else
            return SignifNum==0&&Exponent==0;
#endif
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(AltFloat Value)
        {
            SignifNum = Value.SignifNum;
            Exponent = Value.Exponent;
        }

        void SetAsZero()
        {
			//If AltFloat_TreatZeroAsZeroExponent or AltFloat_UseLeadingZeroInSignificant toggled, treats SignifNum 0 with Exponent 0 as zero. 
			//Otherwise, treat Exponent -128 as for special values and zero so that formula for exact value is exact to formula except if Exponent is -128
            SignifNum = 0;
            Exponent = ZeroRep;
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
#if !defined(AltFloat_ExtendedRange)
            SignifNum = 8388607;
#else
			SignifNum = 2147483647;
#endif
			Exponent = 127;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
#if !defined(AltFloat_ExtendedRange)
            SignifNum = -8388607;
#else
			SignifNum = -2147483647;
#endif
			Exponent = 127;
        }

        /// <summary>
        /// Sets value as smallest non-zero whole number that is not approaching zero
        /// </summary>
        void SetAsSmallestNonZero()
        {
            SignifNum = 1;
			Exponent = -127;
        }

    #pragma region ApproachingZero Setters

    #pragma endregion ApproachingZero Setters

    #pragma region ValueDefines
protected:
        static AltFloat ZeroValue()
        {
            return AltFloat();
        }
		
#if !defined(AltFloat_TreatZeroAsZeroExponent)
        static AltFloat OneValue()
        {
            return AltFloat(0,0);
        }
		
        static AltFloat TwoValue()
        {
            return AltFloat(0,1);
        }
#endif

public:

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat Zero;

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

		//Outputs string in "1+SignifNum/DenomMax * 2^Exponent" format 
		std::string ToFormulaFormat()
		{
			if(IsZero())
				return "0";
	#if defined(AltFloat_EnableApproachingZero)
	#endif
#if defined(AltFloat_UseThreeVariableFormula)
			else if(SignifNum<0)
			{
				string outputStr = 
	#if defined(AltFloat_TreatZeroAsZeroExponent)
				"-1+"+(std::string)-SignifNum;
	#else
				(std::string)SignifNum;
	#endif
	#if defined(AltFloat_ExtendedRange)
				outputStr += "/2147483648 * 2^"+(std::string)Exponent;
	#else
				outputStr += "/8388607 * 2^"+(std::string)Exponent;
	#endif
				return outputStr;
			}
			else
			{
				string outputStr = 
	#if defined(AltFloat_TreatZeroAsZeroExponent)
				"1+"+(std::string)SignifNum;
	#else
				(std::string)SignifNum;
	#endif
	#if defined(AltFloat_ExtendedRange)
				outputStr += "/2147483648 * 2^"+(std::string)Exponent;
	#else
				outputStr += "/8388607 * 2^"+(std::string)Exponent;
	#endif
				return outputStr;
			}
#else
			signed int ExponentMultiplier = Exponent - DenomMaxExponent;
			else if(SignifNum<0)
			{
				string outputStr = 
	#if defined(AltFloat_TreatZeroAsZeroExponent)
				"-1+"+(std::string)-SignifNum;
	#else
				(std::string)SignifNum;
	#endif
				if(ExponentMultiplier<0)
					outputStr += " /2^"+-ExponentMultiplier;
				else
					outputStr += " *2^"+ExponentMultiplier;
				return outputStr;
			}
			else
			{
				string outputStr = 
	#if defined(AltFloat_TreatZeroAsZeroExponent)
				"1+"+(std::string)SignifNum;
	#else
				(std::string)SignifNum;
	#endif
				if(ExponentMultiplier<0)
					outputStr += " /2^"+-ExponentMultiplier;
				else
					outputStr += " *2^"+ExponentMultiplier;
				return outputStr;
			}
#endif
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
        explicit operator std::string() { return ToFormulaFormat(); }

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

        float toFloat()
        {
            float Value;
            //Add code here later
            return 0.0f;//placeholder //Value;
        }

        /// <summary>
        /// AltFloat to float explicit conversion
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
        /// AltFloat to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double()
        {
            return toFloat();
        }

        template<IntegerType IntType=int>
        IntType toIntType()
        {
            IntType Value;
            if(Exponent==ZeroRep)
            {
				if(SignifNum==0)
					return 0;
	#if !defined(AltFloat_TreatZeroAsZeroExponent)
				else//Should only be used for special status
				{
	#if defined(AltFloat_TreatZeroAsZeroExponent)
					//Add code here later
	#else
					//Add code here later
	#endif
				}
	#endif
            }
	#if !defined(AltFloat_TreatZeroAsZeroExponent)
			else if(Exponent==0)
			{
				bool IsNegative = SignifNum<0;
				signed long long numSide = sDenomMax+IsNegative?-SignifNum:SignifNum;
				numSide /= sDenomMax;
				return (IntType) numSide;
			}
	#endif
            else if(SignifNum==0)
				//Will overflow at: 
				//exponent value 32 for signed int
				//exponent value 33 for unsigned int
				//exponent value 64 for signed int 64
				//exponent value 65 for unsigned int 64
				//exponent value 128 for signed int 128
				//exponent value 129 for unsigned int 128
                return BlazesFloatingCode::IntPow(2,Exponent);//2^Exponent
			else
			{
				bool IsNegative = SignifNum<0;
				unsigned long long numSide = uDenomMax+IsNegative?-SignifNum:SignifNum;
				//unsigned long long denomSide = DenomMax;
				if(Exponent<0)
				{
#if defined(AltFloat_ExtendedRange)
					signed int denomExponent = 31+-Exponent;
#else
					signed int denomExponent = 23+-Exponent;
#endif
					//Add code here later
					//numSide%2
				}
				else
				{
					if(Exponent>=65)
					{
						//Add Code here later(need to deal with overflow so need to use alternative code)
					}
					else
					{
						//numSide *= BlazesFloatingCode::IntPow(2,Exponent);
						//Add code here later
					}
				}
			}
//            else if(Exponent<0)
//            {
//				if(Exponent<=-65)
//				{
//					//Add Code here later
//				}
//				else
//				{
//
//					
//					
//					//unsigned long long ExpDenomTotal = BlazesFloatingCode::IntPow(2,-Exponent);
//					//signed long long Result = ;//(1+(SignifNum/DenomMax))/ExpDenomTotal
//					//Add code here later
//				}
//            }
//            else
//            {
//				if(Exponent<=65)
//				{
//					//Add Code here later
//				}
//				else
//				{
//					bool IsNegative = SignifNum<0;
//					unsigned long long numSide = DenomMax+IsNegative?-SignifNum:SignifNum;
//						
//					//unsigned long long ExpTotal = BlazesFloatingCode::IntPow(2,Exponent);
//					
//					//signed long long Result = ;//(1+(SignifNum/DenomMax))*ExpTotal
//					//Add code here later
//				}
//            }
            return 0;//Placeholder;
        }

        /// <summary>
        /// AltFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int()
        {
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator long long()
        {
            return toIntType();
        }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
    //To-Do:Place code here
    #pragma endregion Comparison Operators

	protected:
	#pragma region Division Operations
        template<IntegerType IntType=int>
        void DivByIntOp(const IntType& rValue)
		{

		}
		
        //Division by AltFloat operation
        void DivOp(const AltFloat& rValue)
		{
#if !defined(AltFloat_UseLeadingZeroInSignificant)
			//((1+(lValue.SignifNum/lValue.DenomMax))*(2^lValue.Exponent))/((1+(rValue.SignifNum/rValue.DenomMax))* (2^rValue.Exponent))
#else
			//((lValue.SignifNum/lValue.DenomMax)*(2^lValue.Exponent))/((rValue.SignifNum/rValue.DenomMax) * (2^rValue.Exponent))
#endif
		}
	#pragma endregion Division Operations

	#pragma region Multiplication Operations
        //Multiply by Integer Operation
        template<IntegerType IntType=int>
        void MultByIntOp(const IntType& rValue)
		{

		}
		
        //Multiply operation
        void MultOp(const AltFloat& rValue)
		{
#if !defined(AltFloat_UseLeadingZeroInSignificant)
			//((1+(lValue.SignifNum/lValue.DenomMax))*(2^lValue.Exponent))*((1+(rValue.SignifNum/rValue.DenomMax))* (2^rValue.Exponent))
#else
			//((lValue.SignifNum/lValue.DenomMax)*(2^lValue.Exponent))*((rValue.SignifNum/rValue.DenomMax) * (2^rValue.Exponent))
#endif
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
#if !defined(AltFloat_UseLeadingZeroInSignificant)
			//((1+(lValue.SignifNum/lValue.DenomMax))*(2^lValue.Exponent))+((1+(rValue.SignifNum/rValue.DenomMax))* (2^rValue.Exponent))
#else
			//((lValue.SignifNum/lValue.DenomMax)*(2^lValue.Exponent))+((rValue.SignifNum/rValue.DenomMax) * (2^rValue.Exponent))
#endif
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
#if !defined(AltFloat_UseLeadingZeroInSignificant)
			//((1+(lValue.SignifNum/lValue.DenomMax))*(2^lValue.Exponent))-((1+(rValue.SignifNum/rValue.DenomMax))* (2^rValue.Exponent))
#else
			//((lValue.SignifNum/lValue.DenomMax)*(2^lValue.Exponent))-((rValue.SignifNum/rValue.DenomMax) * (2^rValue.Exponent))
#endif
		}
	#pragma endregion Subtraction Operations
	
	public:
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
            signed long long ValueAsInt = (signed long long)*this;
			SetInt64Value(ValueAsInt);
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