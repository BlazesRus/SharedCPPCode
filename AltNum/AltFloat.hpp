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
AltFloat_UseSmallerFractional = Restricts SignifNum range to 16 bits
AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion
*/

namespace BlazesRusCode
{
	//Right side operations are assumed to be positive
	//^ in comments refers to power of instead of XOR

    class AltFloat;

    /// <summary>
    /// Alternative fixed point number representation designed for use with MixedDec
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
    /// (4-5 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API AltFloat
    {
	protected:
		//Keeping DenomMax as a power of 2 to make easier to calculate into Int equalant etc
		#if defined(AltFloat_ExtendedRange)
		static signed int DenomMaxExponent = 31;
		#else
		static signed int DenomMaxExponent = 23;
		#endif
		//Largest Exponent side calculation(2^127):170141183460469231731687303715884105728

		//If AltFloat_ExtendedRange is enabled, Numerator can fill to max of int 32 with denominator of 2147483648.
		//Forcing bits to be packed https://www.ibm.com/docs/en/xcfbg/121.141?topic=modes-alignment-bit-fields
		struct SignifBitfield {
		#pragma options align=bit_packed
        unsigned int IsNegative:1;
		#if defined(AltFloat_ExtendedRange)
        unsigned int Numerator : 31;
		#else
		unsigned int Numerator : 23;
		#endif
		#pragma options align=reset
			//If specialStatus==1, then is negative number
			//If specialStatus==1 and signifNum, then is "-(1+SignifNum/DenomMax)" range with -zero exponent field
			SignifBitfield(unsigned int signifNum=0, unsigned char specialStatus=0)
			{
				switch(specialStatus)
				{
					case 1:
						IsNegative = 1;
						Numerator = signifNum;
						break;
					case 0:
						IsNegative = 0;
						Numerator = signifNum;
						break;
				}
			}
			
			//Separate constructor for translating from signed number
			SignifBitfield(signed int signifNum=0)
			{
				if(signifNum<0)
				{
					IsNegative = 1;
					Numerator = -signifNum;
				}
				else
				{
					IsNegative = 0;
					Numerator = signifNum;
				}
			}
		}SignifNum;

        //Refers to Exponent inside "2^Exponent + (2^Exponent)*SignifNum/DenomMax" formula
		//If Exponent==-128 and SignifNum==0, in which case the value is at zero
		#if defined(AltFloat_DontUseBitfieldInSignif)
		//If Exponent==-128 and SignifNum>0, then is "-(1+SignifNum/DenomMax)" range with -zero exponent field
		#endif
		signed char Exponent;

			#if defined(AltFloat_ExtendedRange)
		//Equal to 2^31
		static unsigned long long DenomMax = 2147483648;
		//Equal to (2^31) - 1
		static SignifBitfield AlmostApproachingTop = 2147483647;
		static SignifBitfield NegAlmostApproachingTop = -2147483647;
			#else
		//Equal to 2^23
		static unsigned int DenomMax = 8388608;
		//Equal to (2^23) - 1
		static SignifBitfield AlmostApproachingTop = 8388607;
		static SignifBitfield NegAlmostApproachingTop = -8388607;
			#endif

		//Exponent value that zero is defined at
		static signed char ZeroRep = -128;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        AltFloat(SignifBitfield signifNum=0, signed char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            Exponent = exponent;
        }

        AltFloat(const AltFloat&) = default;

        AltFloat& operator=(const AltFloat&) = default;

        //Detect if at exactly zero
		bool IsZero()
		{
            return SignifNum==0&&Exponent==ZeroRep;
		}

        //Detect if at exactly one
		bool IsOne()
		{
            return SignifNum==0&&Exponent==0;
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

        void SetAsNegativeOne()
        {
            SignifNum = SignifNumBitfield(0,1);
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
			SignifNum.IsNegative = 0;
    #if !defined(AltFloat_ExtendedRange)
            SignifNum.Numerator = 8388607;
    #else
			SignifNum.Numerator = 2147483647;
    #endif
			Exponent = 127;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
			SignifNum.IsNegative = 1;
    #if !defined(AltFloat_ExtendedRange)
            SignifNum.Numerator = 8388607;
    #else
			SignifNum.Numerator = 2147483647;
    #endif
			Exponent = 127;
        }

        /// <summary>
        /// Sets value as smallest non-zero whole number that is not approaching zero
        /// </summary>
        void SetAsSmallestNonZero()
        {
            SignifNum = 0;
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
		
        static AltFloat OneValue()
        {
            return AltFloat(0,0);
        }
		
        static AltFloat TwoValue()
        {
            return AltFloat(0,1);
        }
	
        static AltFloat NegativeOneValue()
        {
            return AltFloat(SignifBitfield(0,true),0);
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat Point5Value()
        {
            return AltFloat(0, -1);
        }

        static AltFloat JustAboveZeroValue()
        {
            return AltFloat(0, -127);
        }

        static AltFloat MinimumValue()
        {
            return AltFloat(NegAlmostApproachingTop, 127);
        }

        static AltFloat MaximumValue()
        {
            return AltFloat(AlmostApproachingTop, 127);
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

    #pragma endregion ValueDefines

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
			SignifNum.IsNegative ^= 1;//Flip the last bit
        }

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
			signed int ExponentMultiplier = Exponent - DenomMaxExponent;
			string outputStr = SignifNum.IsNegative==1?"-2^":"2^"+(std::string)Exponent;
			outputStr += " + ";
			outputStr += (std::string)SignifNum.Numerator;
			outputStr += " * ";
			outputStr += "2^"+(std::string)ExponentMultiplier;
			return outputStr;
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
        //(Extract value of power of 2 and then deal with remaining)
		//https://hackernoon.com/bit-manipulation-in-c-and-c-1cs2bux

        void SetUIntVal(const unsigned int& Value)
        {
            if(Value==0)
                SetAsZero();
            else if(Value==1)
                SetAsOne();
            else
            {
				unsigned int RemainingVal = Value;
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
				unsigned _int64 denom = ValAtHighestPos<<1;
				RemainingVal -=  ValAtHighestPos;
				unsigned _int64 numerator = DenomMax;
				numerator *= RemainingVal;
				numerator /= denom;
				SignifNum = SignifBitfield(numerator,0)
				Exponent = highestPower;
            }
        }

        void SetIntVal(const signed int& Value)
        {
            if(Value==0)
                SetAsZero();
            else if(Value==1)
                SetAsOne();
            else if(Value==-1)
                SetAsNegativeOne();
            else
            {
                bool IsNegative = Value<0;
                unsigned int RemainingVal = IsNegative?-Value:Value;
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
				unsigned _int64 denom = ValAtHighestPos<<1;
				RemainingVal -=  ValAtHighestPos;
				unsigned _int64 numerator = DenomMax;
				numerator *= RemainingVal;
				numerator /= denom;
				if(IsNegative)
				{

					if(highestPower==0)//At "-(1+SignifNum/DenomMax)" range with -zero exponent field
					{
						SignifNum = SignifBitfield(numerator,1)
						Exponent = 0;
					}
					else
					{
						SignifNum = SignifBitfield(numerator,1)
						Exponent = highestPower;
					}
				}
				else
				{
					SignifNum = SignifBitfield(numerator,0)
					Exponent = highestPower;
				}
            }
        }
		
		template<MediumDecVariant VariantType=MediumDec>
		void SetMediumDecVVal(const VariantType& Value)
		{
            if(Value==0)
                SetAsZero();
            else if(Value==1)
                SetAsOne();
            else if(Value==-1)
                SetAsNegativeOne();
            else
            {
				unsigned int RemainingVal = Value.IntHalfAsAbs();
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
				unsigned int denom = ValAtHighestPos<<1;
				RemainingVal -=  ValAtHighestPos;
				#if defined(AltFloat_ExtendedRange)
				VariantType numerator = VariantType.Maximum();//Can't fit 2147483648 in exactly inside MediumDec 
				numerator /= denom;
				numerator *= VariantType(RemainingVal, Value.GetDecimalHalf());
				#else
				VariantType numerator = VariantType(DenomMax);
				numerator *= VariantType(RemainingVal, Value.GetDecimalHalf());
				numerator /= denom;
				#endif
				//return Value.IsNegative()?-numerator.GetIntegerHalf():numerator.GetIntegerHalf();
				if(Value.IsNegative())
				{
					if(Exponent==0)//Negative 1 Exponent
					{
						SignifNum = SignifBitfield(numerator.GetIntegerHalf(),2);
						Exponent==0;
					}
					else
					{
						SignifNum = SignifBitfield(numerator.GetIntegerHalf(),1);
						Exponent = highestPower;
					}
				}
				else
				{
					SignifNum = SignifBitfield(numerator.GetIntegerHalf(),0);
					Exponent = highestPower;
				}
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
			//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))" format 
			signed int ExponentMultiplier = Exponent - DenomMaxExponent;
			if(SignifNum<0)
			{
			}
			else
			{
			}
            return 0;//Placeholder;
        }

        /// <summary>
        /// AltFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int()
        {
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
			#else
				if(Exponent>=31)//Max value it can hold is 2147483647
					return 2147483647;
			#endif
			}
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int()
        {
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
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed long long()
        {
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
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned long long()
        {
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
	//https://www.foonathan.net/2018/09/three-way-comparison/

    std::strong_ordering operator<=>(const AltFloat& that) const
    {
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		
		//Comparing if number is negative vs positive
        if (auto SignCmp = SignifNum.IsNegative <=> that.SignifNum.IsNegative; SignCmp != 0)
			return SignCmp;
		//The Smaller Exponent is the closer to zero(-128 is exactly at zero)
        if (auto ExponentCmp = Exponent <=> that.Exponent; ExponentCmp != 0)
			return ExponentCmp;
        if (auto SignifFracCmp = SignifNum.Value <=> that.SignifNum.Value; SignifFracCmp != 0)
			return SignifFracCmp;
    }
	
    bool operator==(const AltFloat& that) const
    {
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
        if (IsNegative!=that.IsNegative)
            return false;
		if (SignifNum!=that.SignifNum)
            return false;
        if (Exponent!=that.Exponent)
            return false;
		return true;
    }

    #pragma endregion Comparison Operators

	protected:
	#pragma region Division Operations
        template<IntegerType IntType=int>
        void DivByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Division by AltFloat operation
        void DivOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void DivByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
	#pragma endregion Division Operations

	#pragma region Multiplication Operations
        //Multiply by Integer Operation
        template<IntegerType IntType=int>
        void MultByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Multiply operation
        void MultOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void MultByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
	#pragma endregion Multiplication Operations

	#pragma region Addition Operations
        //Addition by Integer Operation
        template<IntegerType IntType=int>
        void AddByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Addition by AltFloat Operation
        void AddOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Addition by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void AddByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations
        //Subtraction by Integer Operation
        template<IntegerType IntType=int>
        void SubtractByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Subtraction by AltFloat Operation
        void SubtractOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Subtraction by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void SubtractByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations
        template<IntegerType IntType=int>
        void ModByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Modulus by AltFloat operation
        void ModOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void ModByMediumDecVOp(const VariantType& rValue)
		{
	#if defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
	#pragma endregion Modulus Operations
	
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

    AltFloat AltFloat::NegativeOne = NegativeOneValue();
    AltFloat AltFloat::Two = TwoValue();

    AltFloat AltFloat::JustAboveZero = JustAboveZeroValue();

    AltFloat AltFloat::Minimum = MinimumValue();
    AltFloat AltFloat::Maximum = MaximumValue();

    AltFloat AltFloat::PointFive = Point5Value();

    #pragma endregion ValueDefine Source

    #pragma region String Function Source

    #pragma endregion String Function Source
	