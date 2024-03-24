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
AltFloat_ExtendedRange = Extends SignifNum range to 2147483647
AltFloat_EnableApproachingZero = Not Implimented yet
AltFloat_EnableInfinity = Not Implimented yet
AltFloat_EnableBitwiseOperations = Not Implimented yet
AltFloat_UseXorAsPowerOf = Not Implimented yet
AltFloat_UseRestrictedRange = Restrict range to 0 and 1
AltFloat_UseSmallerFractional = Restricts SignifNum range to 16 bits
AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion
AltFloat_DontUseBitfieldInSignif = Prevent using bitfields to store significant number(uses previous int storage instead)
*/

namespace BlazesRusCode
{
	//Right side operations are assumed to be positive
	//^ in comments refers to power of instead of XOR

    class AltFloat;

    /// <summary>
    /// Alternative fixed point number representation designed for use with MixedDec
#if defined(AltFloat_UseRestrictedRange)
	///Represents floating range between 0 and 1
    ///Stores Exponents in reverse order as AltFloat_UseNormalFloatingRange toggle
    ///Each InvertedExp holds DenomMax number of fractionals between InvertedExp and next highest InvertedExp
#else
    #if defined(AltFloat_ExtendedRange)//Extended range can represent more digits of decimal places 
	//Represents floating range between 0 and (1+(2147483647/2147483648))*2^127(Approximately 340 282 366 841 710 300 949 110 269 838 224 261 120)
    #else
	//Represents floating range between 0 and (1+(8388607/8388608))*2^127      (Approximately 340 282 346 638 528 859 811 704 183 484 516 925 440)
    #endif
	// Floating formula representation is SignifNum>=0?"(1+(SignifNum/DenomMax))*2^Exponent":"(1+(-SignifNum/DenomMax))*2^Exponent";
	// Floating range maximum at "(1+(AlmostApproachingOne/DenomMax))*2^127"
	// Which in scientific notation is equal to 3.40282 x 10^38 (same approximate range as float maximum)
	// When Exponent<0, floating formula can also be represented as: "(1+(SignifNum/DenomMax))*(1/(2^-Exponent))"
	// Floating formula representation when Exponent is < 0 also equivalant to "(1+(AlmostApproachingOne/DenomMax))*2^Exponent"
#endif
    /// (4-5 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API AltFloat
    {
	protected:
		//Keeping DenomMax as a power of 2 to make easier to calculate into Int equalant etc
    #if defined(AltFloat_UseRestrictedRange)
        #if defined(AltFloat_UseSmallerFractional)//Restrict to 2 bytes worth of SignifNum instead
		//Equal to 2^16
		static unsigned long long DenomMax = 65536;
		//Equal to (2^31) - 1
		static unsigned long long AlmostApproachingTop = 65535;
		static unsigned int DenomMaxExponent = 16;
        #else
		//Equal to 2^32
		static unsigned long long DenomMax = 4294967296;
		//Equal to (2^31) - 1
		static unsigned long long AlmostApproachingTop = 4294967295;
		static unsigned int DenomMaxExponent = 32;
        #endif
	#else
		#if defined(AltFloat_ExtendedRange)
		static signed int DenomMaxExponent = 31;
		#else
		static signed int DenomMaxExponent = 23;
		#endif
		#if defined(AltFloat_DontUseBitfieldInSignif)
			#if defined(AltFloat_ExtendedRange)
		//Equal to 2^31
		static signed long long DenomMax = 2147483648;
		//Equal to (2^31) - 1
		static signed long long AlmostApproachingTop = 2147483647;
		static signed long long NegAlmostApproachingTop = -2147483647;
			#else
		//Equal to 2^23
		static signed int DenomMax = 8388608;
		//Equal to (2^23) - 1
		static signed int AlmostApproachingTop = 8388607;
		static signed int NegAlmostApproachingTop = -8388607;
			#endif
		#else
			#if defined(AltFloat_ExtendedRange)
		//Equal to 2^31
		static unsigned long long DenomMax = 2147483648;
		//Equal to (2^31) - 1
		static unsigned long long AlmostApproachingTop = 2147483647;
		static unsigned long long NegAlmostApproachingTop = -2147483647;
			#else
		//Equal to 2^23
		static unsigned int DenomMax = 8388608;
		//Equal to (2^23) - 1
		static unsigned int AlmostApproachingTop = 8388607;
		static unsigned int NegAlmostApproachingTop = -8388607;
			#endif
		#endif
	#else
	#endif
		//Largest Exponent side calculation(2^127):170141183460469231731687303715884105728
 
#if defined(AltFloat_UseRestrictedRange)
    #if defined(AltFloat_UseSmallerFractional)
		unsigned short SignifNum : 15;
    #else
		unsigned int SignifNum : 31;
    #endif

        //Refers to InvertedExp inside "1/2^InvertedExp + (1/2^InvertedExp)*SignifNum/DenomMax" formula
		//Unless InvertedExp==256 and SignifNum==0, in which case the value is at zero
		unsigned char InvertedExp;
#else

		//If AltFloat_ExtendedRange is enabled, Numerator can fill to max of int 32 with denominator of 2147483648.
	#if defined(AltFloat_DontUseBitfieldInSignif)
        signed int SignifNum;
	#else//Forcing bits to be packed https://www.ibm.com/docs/en/xcfbg/121.141?topic=modes-alignment-bit-fields
		struct SignifBitfield {
		#pragma options align=bit_packed
        unsigned int IsNegative:1;
		#if defined(AltFloat_ExtendedRange)
        unsigned int Numerator : 31;
		#else
		unsigned int Numerator : 23;
		#endif
		#pragma options align=reset
			SignifBitfield(signed int signifNum=0, bool negativeZero=false)
			{
				if(negativeZero)
				{
					IsNegative = 1;
					Numerator = 0;
				}
				else if(signifNum<0)
				{
					IsNegative = 1;
					Numerator = -signifNum
				}
				else
				{
					IsNegative = 0;
					Numerator = signifNum
				}
			}
		}SignifNum;
	#endif

        //Refers to Exponent inside "2^Exponent + (2^Exponent)*SignifNum/DenomMax" formula
		//Unless Exponent==-128 and SignifNum==0, in which case the value is at zero
		signed char Exponent;
#endif

		//Exponent value that zero is defined at
#if defined(AltFloat_UseRestrictedRange)
		static unsigned char ZeroRep = 256;
		
#else
		static signed char ZeroRep = -128;
	#if defined(AltFloat_DontUseBitfieldInSignif)
        //When Exponent is zero and NegativeOneRep == -128 (until separate into single bit flag)
        static signed char NegativeOneRep = -128;
	#else
        static signed char NegativeOneRep = 
	#endif
#endif

    public:

#if defined(AltFloat_UseRestrictedRange)
    #if defined(AltFloat_UseSmallerFractional)
        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        AltFloat(unsigned short signifNum=0, unsigned char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            InvertedExp = exponent;
        }
    #else
        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        AltFloat(unsigned int signifNum=0, unsigned char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            InvertedExp = exponent;
        }
    #endif
#else
        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
	#if defined(AltFloat_DontUseBitfieldInSignif)
        AltFloat(unsigned int signifNum=0, signed char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            Exponent = exponent;
        }
	#else
        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        AltFloat(SignifBitfield signifNum=0, signed char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            Exponent = exponent;
        }
	#endif
#endif

        AltFloat(const AltFloat&) = default;

        AltFloat& operator=(const AltFloat&) = default;

        //Detect if at exactly zero
		bool IsZero()
		{
    #if defined(AltFloat_UseRestrictedRange)
            return SignifNum==0&&InvertedExp==256;
    #else
            return SignifNum==0&&Exponent==-128;
    #endif
		}

        //Detect if at exactly one
		bool IsOne()
		{
    #if defined(AltFloat_UseRestrictedRange)
            return SignifNum==0&&InvertedExp==OneRep;
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

        void SetAsOne()
        {
            SignifNum = 0;
            Exponent = 0;
        }

#if defined(AltFloat_DontUseBitfieldInSignif)
        void SetAsNegativeOne()
        {
            SignifNum = NegativeOneRep;
            Exponent = 0;
        }
#elif !defined(AltFloat_UseRestrictedRange)
        void SetAsNegativeOne()
        {
            SignifNum.IsNegative = 1;
            SignifNum.Numerator = 0;
            Exponent = 0;
        }
#endif

    #pragma region Const Representation values
    protected:

    #pragma endregion Const Representation values
	public:
	
        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        {
#if defined(AltFloat_UseRestrictedRange)//Maximum set as One
            SignifNum = 0;
            InvertedExp = 0;
#elif defined(AltFloat_DontUseBitfieldInSignif)
    #if !defined(AltFloat_ExtendedRange)
            SignifNum = 8388607;
    #else
			SignifNum = 2147483647;
    #endif
			Exponent = 127;
#else
			SignifNum.IsNegative = 0;
    #if !defined(AltFloat_ExtendedRange)
            SignifNum.Numerator = 8388607;
    #else
			SignifNum.Numerator = 2147483647;
    #endif
			Exponent = 127;
#endif
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
#if defined(AltFloat_UseRestrictedRange)//Minimum set as Zero
            SignifNum = 0;
            InvertedExp = ZeroRep;
#elif defined(AltFloat_DontUseBitfieldInSignif)
    #if !defined(AltFloat_ExtendedRange)
            SignifNum = -8388607;
    #else
			SignifNum = -2147483647;
    #endif
			Exponent = 127;
#else
			SignifNum.IsNegative = 1;
    #if !defined(AltFloat_ExtendedRange)
            SignifNum.Numerator = 8388607;
    #else
			SignifNum.Numerator = 2147483647;
    #endif
			Exponent = 127;
#endif
        }

        /// <summary>
        /// Sets value as smallest non-zero whole number that is not approaching zero
        /// </summary>
        void SetAsSmallestNonZero()
        {
#if defined(AltFloat_UseRestrictedRange)
            SignifNum = 0;
            InvertedExp = 255;
#else
            SignifNum = 0;
			Exponent = -127;
#endif
        }

    #pragma region ApproachingZero Setters

    #pragma endregion ApproachingZero Setters

    #pragma region ValueDefines
protected:
        static AltFloat ZeroValue()
        {
            return AltFloat();
        }
		
#if defined(AltFloat_UseRestrictedRange)
        static AltFloat OneValue()
        {
            return AltFloat(0,0);
        }
#else
        static AltFloat OneValue()
        {
            return AltFloat(0,0);
        }
		
        static AltFloat TwoValue()
        {
            return AltFloat(0,1);
        }
	#if defined(AltFloat_DontUseBitfieldInSignif)
	
        static AltFloat NegativeOneValue()
        {
            return AltFloat(NegativeOneRep,0);
        }
	
	#else
	
        static AltFloat NegativeOneValue()
        {
            return AltFloat(SignifBitfield(0,true),0);
        }
	
	#endif
#endif

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat Point5Value()
        {
    #if defined(AltFloat_UseRestrictedRange)
            return AltFloat(0, 1);
    #else
            return AltFloat(0, -1);
    #endif
        }

        static AltFloat JustAboveZeroValue()
        {
    #if defined(AltFloat_UseRestrictedRange)
            return AltFloat(0, 255);
    #else
            return AltFloat(0, -127);
    #endif
        }

        static AltFloat MinimumValue()
        {
    #if defined(AltFloat_UseRestrictedRange)
            return AltFloat(0, ZeroRep);
    #else
            return AltFloat(NegAlmostApproachingTop, 127);
    #endif
        }

        static AltFloat MaximumValue()
        {
    #if defined(AltFloat_UseRestrictedRange)
            return AltFloat(0, 0);
    #else
            return AltFloat(AlmostApproachingTop, 127);
    #endif
        }

public:

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat Zero;
		
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat One;
		
	#if defined(AltFloat_UseRestrictedRange)

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat NegativeOne;
		
        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDec</returns>
        static AltFloat JustAboveZero;

        /// <summary>
        /// Returns value of lowest non-infinite value that can stored
        /// </summary>
        static AltFloat Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite value that can stored
        /// </summary>
        static AltFloat Maximum;

	#endif

    #pragma endregion ValueDefines
    #if !defined(AltFloat_UseRestrictedRange)
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
	#if defined(AltFloat_DontUseBitfieldInSignif)
			if(SignifNum==NegativeOneRep)
				SignifNum = 1;
			else if(SignifNum==1)
				SignifNum = NegativeOneRep;
			else
				SignifNum *= -1;//Flip the last bit
	#else
			SignifNum.IsNegative ^= 1;//Flip the last bit
	#endif
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

		//Outputs string in "2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))" format 
		std::string ToFormulaFormat()
		{
			if(IsZero())
				return "0";
			else if(IsOne())
				return "1";
    #if defined(AltFloat_UseRestrictedRange)
			unsigned int InvertedExpMult = InvertedExp + DenomMaxExponent;
			string outputStr = "1/2^"
            outputStr += (std::string)InvertedExp;
            outputStr += " + ";
            outputStr += (std::string)SignifNum;
            outputStr += " * ";
			string outputStr = "1/2^"
            outputStr += (std::string)InvertedExpMult;
    #else
			signed int ExponentMultiplier = Exponent - DenomMaxExponent;
		#if defined(AltFloat_DontUseBitfieldInSignif)
			if(SignifNum<0)
			{
				string outputStr = "-(";
                outputStr += "2^"+(std::string)Exponent;
                outputStr += " + ";
                outputStr += (std::string)-SignifNum;
                outputStr += " * ";
                outputStr += "2^"+(std::string)ExponentMultiplier;
				outputStr += ")";
                return outputStr;
            }
            else
            {
				string outputStr = "2^"+(std::string)Exponent;
		#else
				string outputStr = SignifNum.IsNegative==1?"-2^":"2^"+(std::string)Exponent;
		#endif
                outputStr += " + ";
		#if defined(AltFloat_DontUseBitfieldInSignif)
                outputStr += (std::string)SignifNum;
		#else
                outputStr += (std::string)SignifNum.Numerator;
		#endif
                outputStr += " * ";
                outputStr += "2^"+(std::string)ExponentMultiplier;
                return outputStr;
		#if defined(AltFloat_DontUseBitfieldInSignif)
            }
		#endif
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
        //(Extract value of power of 2 and then deal with remaining)
		//https://hackernoon.com/bit-manipulation-in-c-and-c-1cs2bux

        void SetUIntVal(const unsigned int& Value)
        {
    #if defined(AltFloat_UseRestrictedRange)
            if(Value==1)
                SetAsOne();
            else
                SetAsZero();
    #else
            if(Value==0)
                SetAsZero();
            else if(Value==1)
                SetAsOne();
            else
            {
		#if defined(AltFloat_DontUseBitfieldInSignif)
                bool IsNegative = Value<0;
                unsigned int RemainingVal = IsNegative?-Value:Value;
		#else
				unsigned int RemainingVal = Value;
		#endif
				bool bitAtPosition;
				signed int powerAtPos = 0;
				signed int highestPower = 0;
				unsigned int ValAtHighestPos = 0;
				for(unsigned int position = 1;position<=RemainingVal;position<<1&&++powerAtPos)
				{
					bitAtPosition = RemainingVal & (1 << position);
					if(bitAtPosition)
					{
						highestPower = powerAtPos;
						ValAtHighestPos = position;
					}
				}
				//To-Do Add code here
            }
    #endif
        }

        void SetIntVal(const signed int& Value)
        {
    #if defined(AltFloat_UseRestrictedRange)
            if(Value==1)
                SetAsOne();
            else
                SetAsZero();
    #else
            if(Value==0)
                SetAsZero();
            else if(Value==1)
                SetAsOne();
            else if(Value==-1)
                SetAsNegativeOne();
            else
            {
		#if defined(AltFloat_DontUseBitfieldInSignif)
                bool IsNegative = Value<0;
                unsigned int RemainingVal = IsNegative?-Value:Value;
		#else
				unsigned int RemainingVal = Value;
		#endif
				bool bitAtPosition;
				signed int powerAtPos = 0;
				signed int highestPower = 0;
				unsigned int ValAtHighestPos = 0;
				for(unsigned int position = 1;position<=RemainingVal;position<<1&&++powerAtPos)
				{
					bitAtPosition = RemainingVal & (1 << position);
					if(bitAtPosition)
					{
						highestPower = powerAtPos;
						ValAtHighestPos = position;
					}
				}
				//To-Do Add code here
            }
    #endif
        }

        AltFloat(const unsigned int& Value)
        {
            this->SetUIntVal(Value);
        }

        AltFloat(const signed int& Value)
        {
            this->SetIntVal(Value);
        }

        AltFloat(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        AltFloat(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        AltFloat(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        AltFloat(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

        AltFloat(const MediumDec& Value)
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
            if(IsZero())
                return 0;
            else if(IsOne()))
                return 1;
    #if defined(AltFloat_UseRestrictedRange)
            return 0;
    #else//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))" format 
			signed int ExponentMultiplier = Exponent - DenomMaxExponent;
			if(SignifNum<0)
			{
			}
			else
			{
			}
            return 0;//Placeholder;
    #endif
        }

        /// <summary>
        /// AltFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int()
        {
    #if !defined(AltFloat_UseRestrictedRange)
            if(SignifNum<0)
            {
				#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exponent>31)//Overflow Error
					return -2147483648;//Return Error
				else if(Exponent==32)//Max value it can hold is 4294967296
				{
					if(SignifNum==0)
						return -2147483648;
					else
						return -2147483648;//Return Error
				}
				#else
					if(Exponent>=31)//Minimum value it can hold is -2147483648
						return -2147483648;
				#endif
            }
            else
            {
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exponent>=31)//Overflow Error
					return 2147483647;//Return Error
//				else if(Exponent==30)
//				{
//				#if defined(AltFloat_ExtendedRange)
//					//1073741824 + (2147483646/2147483648)*1073741824 == 1073741824 + 1073741823
//					if(SignifNum>=2147483646)
//						return 2147483647;
//					else
//						return 2147483647;//Return Error
//
//				#else
//					//1073741824 + (8388607/8388608)*1073741824 == 1073741824 + 1073741696
//					//if(SignifNum<=8388607)
//					return 2147483647;
//				#endif
//				}
			#else
				if(Exponent>=31)//Max value it can hold is 2147483647
					return 2147483647;
			#endif
			}
    #endif
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int()
        {
    #if !defined(AltFloat_UseRestrictedRange)
            if(SignifNum<0)
            {
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				return 0;//Return Error
			#else
				return 0;
			#endif
			}
			else
			{
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exponent>32)//Overflow Error
					return 4294967296;//Return Error
				else if(Exponent==32)//Max value it can hold is 4294967296
				{
					if(SignifNum==0)
						return 4294967296;
					else
						return 4294967296;//Return Error
				}
			#else
				if(Exponent>=32)
					return 4294967296;
			#endif
			}
    #endif
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed long long()
        {
    #if !defined(AltFloat_UseRestrictedRange)
            if(SignifNum>0)
            {
				#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exponent>63)//Overflow Error
					return -9223372036854775808;//Return Error
				else if(Exponent==63)//Max value it can hold is 4294967296
				{
					if(SignifNum==0)
						return -9223372036854775808;
					else
						return -9223372036854775808;//Return Error
				}
				#else
					if(Exponent>=63)//Minimum value it can hold is -2147483648
						return -9223372036854775808;
				#endif
            }
            else
            {
        #if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
            if(Exponent>=63)//Overflow Error
                return 9223372036854775807;//Return Error
   //         else if(Exponent==62)
   //         {
			//#if defined(AltFloat_ExtendedRange)
   //             if(SignifNum==2147483647)
			//		return 9223372036854775807;
			//#else//4611686018427387903/4611686018427387904 * 8388608
			//	//4611686018427387904 + /8388608 * 4611686018427387904
   //             if(SignifNum==8388607)
			//		return 9223372036854775807;
			//#endif
   //         }
        #else
            if(Exponent>=63)
                return 9223372036854775807;
        #endif
            }           
    #endif
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned long long()
        {
    #if !defined(AltFloat_UseRestrictedRange)
            if(SignifNum>0)
            {
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				return 0;//Return Error;
			#else
				return 0;
			#endif
            }
            else
            {
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exponent>64)//Overflow Error
					return 18446744073709551616;//Return Error
				else if(Exponent==64)
				{
					if(SignifNum==0)
						return 18446744073709551616;
					else
						return 18446744073709551616;//Return Error
				}
			#else
				if(Exponent>=32)
					return 18446744073709551616;
			#endif
			}
    #endif
            return toIntType();
        }

        /// <summary>
        /// AltFloat to MediumDec explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator MediumDec()
        {
            if(IsZero())
                return 0;
            else if(IsOne())
                return 1;
    #if !defined(AltFloat_UseRestrictedRange)
            if(SignifNum<0)
            {
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exponent>=31)
					return MediumDec::MinimumValue();//Return Error
				else if(Exponent==30)
				{
					//Add Code here
				}
				else
				{
				}
			#else
				if(Exponent>=31)//Minimum value it can store is -2147483647.999999999
					return MediumDec::MinimumValue();//Return Error
				else if(Exponent==30)
				{
					//Add Code here
				}
				else
				{
					//Add Code here
				}
			#endif
            }
            else
            {
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exponent>=31)
					return MediumDec::MaximumValue();//Return Error
				else if(Exponent==30)
				{
					//Add Code here
				}
				else
				{
				}
			#else
				if(Exponent>=31)//Maximum value it can store is 2147483647.999999999
					return MediumDec::MaximumValue();
				else if(Exponent==30)//Maximum value it can store is 2147483647.999999999
				{
					//Add Code here
				}
				else
				{
					//Add Code here
				}
			#endif
			}
	#else
	
    #endif
        }

	//place AltFloat to MixedDec conversion in MixedDec class

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
/* //ToDo code comparison operators after get basic operators working
    auto operator<=>(const AltFloat& that) const
    {
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
    }
	
    bool operator==(const AltFloat& that) const
    {
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
    }
	
    auto operator<=>(const int& that) const
    {
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
    }

    bool operator==(const int& that) const
    {
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
    }

    auto operator<=>(const MediumDec& that) const
    {
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
    }

    bool operator==(const MediumDec& that) const
    {
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
    }
*/
    #pragma endregion Comparison Operators

	protected:
	#pragma region Division Operations
        template<IntegerType IntType=int>
        void DivByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
		
        //Division by AltFloat operation
        void DivOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void DivByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
	#pragma endregion Division Operations

	#pragma region Multiplication Operations
        //Multiply by Integer Operation
        template<IntegerType IntType=int>
        void MultByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
		
        //Multiply operation
        void MultOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void MultByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
	#pragma endregion Multiplication Operations

	#pragma region Addition Operations
        //Addition by Integer Operation
        template<IntegerType IntType=int>
        void AddByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
		
        //Addition by AltFloat Operation
        void AddOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
		
        //Addition by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void AddByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations
        //Subtraction by Integer Operation
        template<IntegerType IntType=int>
        void SubtractByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
		
        //Subtraction by AltFloat Operation
        void SubtractOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
	#endif
		}
		
        //Subtraction by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void SubtractByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_UseExperimentalSignedBit)
	#elif defined(AltFloat_UseLeadingZeroInSignificant)
	#else
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

        friend AltDec operator/(AltFloat self, int Value) { return DivByIntOp(self, Value); }
        friend AltDec& operator/=(AltFloat& self, int Value) { return DivByIntOp(self, Value); }
		
        friend AltDec operator*(AltFloat self, int Value) { return MultByIntOp(self, Value); }
        friend AltDec& operator*=(AltFloat& self, int Value) { return MultByIntOp(self, Value); }

        friend AltDec operator+(AltFloat self, int Value) { return AddByIntOp(self, Value); }
        friend AltDec& operator+=(AltFloat& self, int Value) { return AddByIntOp(self, Value); }
		
        friend AltDec operator-(AltFloat self, int Value) { return SubtractByIntOp(self, Value); }
        friend AltDec& operator-=(AltFloat& self, int Value) { return SubtractByIntOp(self, Value); }
		
        friend AltDec operator/(AltFloat self, MediumDec Value) { return DivByMediumDecVOp(self, Value); }
        friend AltDec& operator/=(AltFloat& self, MediumDec Value) { return DivByMediumDecVOp(self, Value); }
		
        friend AltDec operator*(AltFloat self, MediumDec Value) { return MultByMediumDecVOp(self, Value); }
        friend AltDec& operator*=(AltFloat& self, MediumDec Value) { return MultByMediumDecVOp(self, Value); }

        friend AltDec operator+(AltFloat self, MediumDec Value) { return AddByMediumDecVOp(self, Value); }
        friend AltDec& operator+=(AltFloat& self, MediumDec Value) { return AddByMediumDecVOp(self, Value); }
		
        friend AltDec operator-(AltFloat self, MediumDec Value) { return SubtractByMediumDecVOp(self, Value); }
        friend AltDec& operator-=(AltFloat& self, MediumDec Value) { return SubtractByMediumDecVOp(self, Value); }
		
//        friend AltDec operator/(AltFloat self, MixedDec Value) { return DivByMediumDecVOp(self, Value); }
//        friend AltDec& operator/=(AltFloat& self, MixedDec Value) { return DivByMediumDecVOp(self, Value); }
//		
//        friend AltDec operator*(AltFloat self, MixedDec Value) { return MultByMediumDecVOp(self, Value); }
//        friend AltDec& operator*=(AltFloat& self, MixedDec Value) { return MultByMediumDecVOp(self, Value); }
//
//        friend AltDec operator+(AltFloat self, MixedDec Value) { return AddByMediumDecVOp(self, Value); }
//        friend AltDec& operator+=(AltFloat& self, MixedDec Value) { return AddByMediumDecVOp(self, Value); }
//		
//        friend AltDec operator-(AltFloat self, MixedDec Value) { return SubtractByMediumDecVOp(self, Value); }
//        friend AltDec& operator-=(AltFloat& self, MixedDec Value) { return SubtractByMediumDecVOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma region Other Operators

        /// <summary>
        /// ++AltFloat Operator
        /// </summary>
        /// <returns>AltFloat &</returns>
        AltFloat& operator ++()
        {
            *this = *this + One;
            return *this;
        }

        /// <summary>
        /// --AltFloat Operator
        /// </summary>
        /// <returns>AltFloat &</returns>
        AltFloat& operator --()
        {
            *this = *this - One;
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

    #pragma region ValueDefine Source

    AltFloat AltFloat::Zero = ZeroValue();
    AltFloat AltFloat::One = OneValue();

	#if defined(AltFloat_UseRestrictedRange)
    AltFloat AltFloat::NegativeOne = NegativeOneValue();
    AltFloat AltFloat::Two = TwoValue();
	#endif

    AltFloat AltFloat::JustAboveZero = JustAboveZeroValue();

    AltFloat AltFloat::Minimum = MinimumValue();
    AltFloat AltFloat::Maximum = MaximumValue();

    AltFloat AltFloat::PointFive = Point5Value();

    #pragma endregion ValueDefine Source

    #pragma region String Function Source

    #pragma endregion String Function Source
	