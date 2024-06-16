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

#include "..\Concepts\IntegerConcept.hpp"
#include "..\MediumDec\MediumDec.hpp"
#include "..\PartialDec\PartialDec.hpp"//Used to keep all digits while dividing my two
//Int 128 needed to extract trailing digits lost from division and multiplication
#include <boost/multiprecision/cpp_int.hpp>
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
	using UInt128 = boost::multiprecision::uint128_t;

    /// <summary>
    /// Alternative fixed point number representation designed for use with MixedDec
    #if defined(AltFloat_ExtendedRange)//Extended range can represent more digits of decimal places 
	//Represents floating range between 0 and (1+(2147483647/2147483648))*2^127(Approximately 340 282 366 841 710 300 949 110 269 838 224 261 120)
    #else
	//Represents floating range between 0 and (1+(8388607/8388608))*2^127      (Approximately 340 282 346 638 528 859 811 704 183 484 516 925 440)
    #endif
	// Floating formula representation is SignifNum>=0?"(1+(SignifNum/DenomMax))*2^Exp":"(1+(-SignifNum/DenomMax))*2^Exp";
	// Floating range maximum at "(1+(MaxSignif/DenomMax))*2^127"
	// Which in scientific notation is equal to 3.40282 x 10^38 (same approximate range as float maximum)
	// When Exp<0, floating formula can also be represented as: "(1+(SignifNum/DenomMax))*(1/(2^-Exp))"
	// Floating formula representation when Exp is < 0 also equivalant to "(1+(AlmostApproachingOne/DenomMax))*2^Exp"
    /// (4-5 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API AltFloat
    {
	protected:
		#pragma region DigitStorage
		
		//Forcing bits to be packed https://www.ibm.com/docs/en/xcfbg/121.141?topic=modes-alignment-bit-fields
		#pragma options align=bit_packed
        unsigned int Sign:1;
		//Numerator Value
		//If AltFloat_ExtendedRange is enabled, Value can fill to max of int 32 with denominator of 2147483648.
		#if defined(AltFloat_ExtendedRange)
        unsigned int SignifNum : 31;
		#else
		unsigned int SignifNum : 23;
		#endif
		#pragma options align=reset
		
        //Refers to Exp inside "2^Exp + (2^Exp)*SignifNum/DenomMax" formula
		//If Exp==-128 and SignifNum==0, in which case the value is at zero
		signed char Exp;
		
		#pragma endregion DigitStorage

		//Exp value that zero is defined at
		static signed char ZeroRep = -128;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="AltFloat"/> class.
        /// </summary>
        AltFloat(const unsigned int& signifNum=0, const signed char& exp=ZeroRep, const unsigned int& isNegative=0)
        {
            SignifNum = signifNum;
            Exp = exp;
			Sign = isNegative;
        }

        AltFloat(const AltFloat&) = default;

        AltFloat& operator=(const AltFloat&) = default;

		bool IsNegative()
		{
			return Sign==PositiveSign;
		}

		bool IsPositive()
		{
			return Sign==NegativeSign;
		}
	
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
			Sign ~= 1;
        }

        //Detect if at exactly zero
		bool IsZero()
		{
            return SignifNum==0&&Exp==ZeroRep;
		}

        //Detect if at exactly one
		bool IsOne()
		{
            return SignifNum==0&&Exp==0&&Sign==PositiveSign;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(AltFloat Value)
        {
            SignifNum = Value.SignifNum;
            Exp = Value.Exp;
			Sign = Value.Sign;
        }

        void SetAsZero()
        {
			//If AltFloat_TreatZeroAsZeroExp or AltFloat_UseLeadingZeroInSignificant toggled, treats SignifNum 0 with Exp 0 as zero. 
			//Otherwise, treat Exp -128 as for special values and zero so that formula for exact value is exact to formula except if Exp is -128
            SignifNum = 0;
            Exp = ZeroRep;
			Sign = 1;
        }

        void SetAsOne()
        {
            SignifNum = 0;
            Exp = 0;
			Sign = 1;
        }

        void SetAsNegativeOne()
        {
            SignifNum = 0;
            Exp = 0;
			Sign = 0;
        }

    #pragma region Const Representation values
    protected:
		//Keeping DenomMax as a power of 2 to make easier to calculate into Int equalant etc
		#if defined(AltFloat_ExtendedRange)
		static signed int DenomMaxExp = 31;
		#else
		static signed int DenomMaxExp = 23;
		#endif
		//Largest Exp side calculation(2^127):170141183460469231731687303715884105728
	
		#if defined(AltFloat_ExtendedRange)
		//Equal to 2^31
		static UInt64 DenomMax = 2147483648;
		//Equal to (2^31) - 1
		static SignifBitfield MaxSignif = 2147483647;
		static SignifBitfield NegMaxSignif = -2147483647;
		#else
		//Equal to 2^23
		static unsigned int DenomMax = 8388608;
		//Equal to (2^23) - 1
		static SignifBitfield MaxSignif = 8388607;
		static SignifBitfield NegMaxSignif = -8388607;
		#endif
	
		static unsigned _int64 const TruncMultAsInt = 10000000000000000000;//10 000 000 000 000 000 000
		//Size of this value determines how much of the truncated digits to save (19 digits of truncated digits stored by default)
		static UInt128 const TruncMult = TruncMultAsInt;
		static unsigned _int64 const SubExp1Range = 5000000000000000000;//TruncMultAsInt/2;
		static unsigned _int64 const SubExp2Range = 2500000000000000000;//SubExp1Range/2;
		static unsigned _int64 const SubExp3Range = 1250000000000000000;//SubExp2Range/2;
		static unsigned _int64 const SubExp4Range = 625000000000000000;//SubExp3Range/2;
		static unsigned _int64 const SubExp5Range = 312500000000000000;//SubExp4Range/2;
		//static unsigned _int64 const SubExp6Range = 156250000000000000;//SubExp5Range/2;
		//static unsigned _int64 const SubExp7Range = 78125000000000000;//SubExp6Range/2;
		//static unsigned _int64 const SubExp8Range = 39062500000000000;//SubExp7Range/2;
		//static unsigned _int64 const SubExp9Range = 19531250000000000;//SubExp8Range/2;
		//static unsigned _int64 const SubExp10Range = 9765625000000000;//SubExp9Range/2;
		//static unsigned _int64 const SubExp11Range = 4882812500000000;//SubExp10Range/2;
		//static unsigned _int64 const SubExp12Range = 2441406250000000;//SubExp11Range/2;
		//static unsigned _int64 const SubExp13Range = 1220703125000000;//SubExp12Range/2;
		//static unsigned _int64 const SubExp14Range = 610351562500000;//SubExp13Range/2;
		//static unsigned _int64 const SubExp15Range = 305175781250000;//SubExp14Range/2;
		//static unsigned _int64 const SubExp16Range = 152587890625000;//SubExp15Range/2;
		//static unsigned _int64 const SubExp17Range = 76293945312500;//SubExp16Range/2;
		//static unsigned _int64 const SubExp18Range = 38146972656250;//SubExp17Range/2;
		//static unsigned _int64 const SubExp19Range = 19073486328125;//SubExp18Range/2;
		//After SubExp19Range the value becomes non-whole number
    #pragma endregion Const Representation values
	public:
	
        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        {
			Sign;
    #if !defined(AltFloat_ExtendedRange)
            SignifNum = 8388607;
    #else
			SignifNum = 2147483647;
    #endif
			Exp = 127;
			Sign = 0;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
			SignifNum.IsNegative = 1;
    #if !defined(AltFloat_ExtendedRange)
            SignifNum = 8388607;
    #else
			SignifNum = 2147483647;
    #endif
			Exp = 127;
			Sign = 0;
        }

    #pragma region ApproachingZero Setters

        /// <summary>
        /// Sets value as smallest real number that is approaching zero
        /// </summary>
        void SetAsSmallestNonZero()
        {
            SignifNum = 0;
			Exp = -127;
			Sign = 1;
        }
		
        /// <summary>
        /// Sets value as smallest real number that is approaching zero
		/// without setting to positive number
        /// </summary>
        void SetAsApproachingWithoutFlip()
        {
            SignifNum = 0;
			Exp = -127;
        }

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
            return AltFloat(NegMaxSignif, 127);
        }

        static AltFloat MaximumValue()
        {
            return AltFloat(MaxSignif, 127);
        }

public:

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        static AltFloat Zero;
		
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        static AltFloat One;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        static AltFloat NegativeOne;
		
        /// <summary>
        /// Returns the value at two
        /// </summary>
        static AltFloat Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        static AltFloat PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
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

		//Outputs string in "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format 
		std::string ToFormulaFormat()
		{
			if(IsZero())
				return "0";
			else if(IsOne())
				return "1";
			signed int ExpMultiplier = Exp - DenomMaxExp;
			string outputStr = SignifNum.IsNegative==1?"-2^":"2^"+(std::string)Exp;
			outputStr += " + ";
			outputStr += (std::string)SignifNum;
			outputStr += " * ";
			outputStr += "2^"+(std::string)ExpMultiplier;
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

        void SetDecimalVal(const long double& Value)
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
				unsigned _int64 Value = DenomMax;
				Value *= RemainingVal;
				Value /= denom;
				SignifNum = Value;
				Exp = highestPower;
                Sign = 1;
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
                bool Sign = Value>=0;
                unsigned int RemainingVal = Sign?Value:-Value;
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
				unsigned _int64 Value = DenomMax;
				Value *= RemainingVal;
				Value /= denom;
				SignifNum = Value;
				Exp = highestPower;
                Sign = Sign?1:0;
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
				VariantType Value = VariantType.Maximum;//Can't fit 2147483648 in exactly inside MediumDec 
				Value /= denom;
				Value *= VariantType(RemainingVal, Value.GetDecimalHalf());
				#else
				VariantType Value = VariantType(DenomMax);
				Value *= VariantType(RemainingVal, Value.GetDecimalHalf());
				Value /= denom;
				#endif
				//return Value.IsNegative()?-Value.GetIntegerHalf():Value.GetIntegerHalf();
				SignifNum = Value.GetIntegerHalf();
				Exp = highestPower;
                Sign = Value.IsNegative()?0:1;
            }
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

        AltFloat(const long double& Value)
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
			//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format 
			signed int ExpMultiplier = Exp - DenomMaxExp;
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
        explicit operator signed int()
        {
            if(IsNegative())
            {
				#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exp>31)//Overflow Error
					throw "Value underflowed during conversion to unsigned int";
				else if(Exp==32)//Max value it can hold is 4294967296
				{
					if(SignifNum==0)
						return -2147483648;
					else
						throw "Value underflowed during conversion to unsigned int";
				}
				#else
					if(Exp>=31)//Minimum value it can hold is -2147483648
						return -2147483648;
				#endif
            }
            else if(Exp>=31)//Max value it can hold is 2147483647
				#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				throw "Value overflowed during conversion to unsigned int";
				#else
				return 2147483647;
				#endif
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int explicit conversion
        /// </summary>
        explicit operator unsigned int()
        {
            if(IsNegative())
            {
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				throw "Value underflowed during conversion to unsigned int";
			#else
				return 0;
			#endif
			}
			#if !defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
			else if(Exp>=32)
				return 4294967296;
			#else
			else if(Exp>32)//Overflow Error
				throw "Value overflowed during conversion to unsigned int";
			else if(Exp==32)//Max value it can hold is 4294967296
			{
				if(SignifNum==0)
					return 4294967296;
				else
					throw "Value overflowed during conversion to unsigned int";
			}
			#endif
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int64 explicit conversion
        /// </summary>
        explicit operator Int64()
        {
            if(IsNegative())
            {
				#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				if(Exp>63)//Overflow Error
					throw "Value underflowed during conversion to signed int64";
				else if(Exp==63)//Max value it can hold is 4294967296
				{
					if(SignifNum==0)
						return -9223372036854775808;
					else
						throw "Value underflowed during conversion to signed int64";
				}
				#else
					if(Exp>=63)//Minimum value it can hold is -2147483648
						return -9223372036854775808;
				#endif
            }
            else if(Exp>=63)//Overflow Error
        #if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				throw "Value overflowed during conversion to signed int64";
        #else
                return 9223372036854775807;
        #endif         
            return toIntType();
        }

        /// <summary>
        /// AltFloat to int64 explicit conversion
        /// </summary>
        explicit operator UInt64()
        {
            if(IsNegative())
			#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				throw "Value underflowed during conversion to unsigned int64";
			#else
				return 0;
			#endif
			#if !defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
			else if(Exp>=32)
				return 18446744073709551616;
			#else
            else if(Exp>64)//Overflow Error
				throw "Value overflowed during conversion to unsigned int64";
			else if(Exp==64)
			{
				if(SignifNum==0)
					return 18446744073709551616;
				else
					throw "Value overflowed during conversion to unsigned int64";
			}
			#endif
            return toIntType();
        }

        /// <summary>
        /// AltFloat to MediumDec explicit conversion
        /// </summary>
        explicit operator MediumDec()
        {
            if(IsZero())
                return 0;
            else if(IsOne())
                return 1;
            if(IsNegative())
            {
				if(Exp>=31)//Minimum value it can store is -2147483647.999999999
					#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
					throw "Value underflowed during conversion to MediumDec Variant";
					#else
					return MediumDec::MinimumValue();
					#endif
				else if(Exp==30)
				{
					//Add Code here
				}
				else
				{
					//Add Code here
				}
            }
            else if(Exp>=31)//Maximum value it can store is 2147483647.999999999
				#if defined(AltFloat_GiveErrorInsteadOfMaxingOnOverflowConversion)
				throw "Value overflowed during conversion to MediumDec Variant";
				#else
				return MediumDec::MaximumValue();
				#endif
			else if(Exp==30)//Maximum value it can store is 2147483647.999999999
			{
					//Add Code here
			}
			else
			{
					//Add Code here
			}
        }

	//place AltFloat to MixedDec conversion in MixedDec class

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
	//https://www.foonathan.net/2018/09/three-way-comparison/

    std::strong_ordering operator<=>(const AltFloat& that) const
    {
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
		
		//Comparing if number is negative vs positive(inverted comparison so that Sign of zero is positive)
        if (auto SignCmp = that.Sign <=> Sign; SignCmp != 0)
			return SignCmp;
		//The Smaller Exp is the closer to zero(-128 is exactly at zero)
        if (auto ExpCmp = Exp <=> that.Exp; ExpCmp != 0)
			return ExpCmp;
        if (auto SignifFracCmp = SignifNum <=> that.SignifNum; SignifFracCmp != 0)
			return SignifFracCmp;
    }
	
    bool operator==(const AltFloat& that) const
    {
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
        if (Sign!=that.Sign)
            return false;
		if (SignifNum!=that.SignifNum)
            return false;
        if (Exp!=that.Exp)
            return false;
		return true;
    }
	
    auto operator<=>(const int& that) const
    {
		rVal = (AltFloat)that;
		return this <=> rValue;
    }

    bool operator==(const int& that) const
    {
		rVal = (AltFloat)that;
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
		{//Negative Exponent values for AltFloat and positive Exponent values for RestrictedFloat
			Sign = 0;
			if(TruncatedDigits==SubExp1Range){//Exactly 0.5 Remainder
				Exponent.Value = 1;
				SignifNum = 0;
			}
			else if(TruncatedDigits>SubExp1Range){
				Exponent.Value = 1;
				SignifNum = FindSignifNumFromRem(TruncatedDigits, SubExp1Range);
			}
			else if(TruncatedDigits==SubExp2Range){//Exactly 0.25 Remainder
				Exponent.Value = 2;
				SignifNum = 0;
			}
			else if(TruncatedDigits>SubExp2Range){
				Exponent.Value = 2;
				SignifNum = FindSignifNumFromRem(TruncatedDigits, SubExp2Range);
			}
			else if(TruncatedDigits==SubExp3Range){//Exactly 0.125 Remainder
				Exponent.Value = 3;
				SignifNum = 0;
			}
			else if(TruncatedDigits>SubExp3Range){
				Exponent.Value = 3;
				SignifNum = FindSignifNumFromRem(TruncatedDigits, SubExp3Range);
			}
			else if(TruncatedDigits==SubExp4Range){
				Exponent.Value = 4;
				SignifNum = 0;
			}
			else if(TruncatedDigits>SubExp4Range){
				Exponent.Value = 4;
				SignifNum = FindSignifNumFromRem(TruncatedDigits, SubExp4Range);
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
						SignifNum = 0;
						return;
					}
					else if(TruncatedDigits>RangeLimit)
					{
						Exponent.Value = Exp;
						SignifNum = FindSignifNumFromRem(TruncatedDigits, RangeLimit);
						return;
					}
					RangeLimit.DivideByTwo();
				}
			}
		}
	
	#pragma endregion Trailing Digit Extraction

	protected:
	#pragma region division operations
        template<IntegerType IntType=int>
        void DivByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
	#pragma endregion division operations

	#pragma region Multiplication Operations
        //Multiply by Integer Operation
        template<IntegerType IntType=int>
        void MultByIntOp(const IntType& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
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
		//"2^Exp + SignifNum*(2^(Exp - DenomMaxExp))"
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
	#pragma endregion Modulus Operations
	
	public:
    #pragma region Main Operator Overrides
        /// <summary>
        /// Division operation
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
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDec</returns>
        AltFloat operator- ()
        {
			auto self = this;
            self.SwapNegativeStatus(); return self;
        } const

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
        friend AltFloat operator%(AltFloat& self, Int64 Value) { return ModulusByIntOp(self, Value); }

        friend AltFloat operator%=(AltFloat& self, int Value) { return ModulusByIntOp(self, Value); }
        friend AltFloat operator%=(AltFloat& self, Int64 Value) { return ModulusByIntOp(self, Value); }

        friend AltFloat operator%=(AltFloat* self, int Value) { return ModulusByIntOp(**self, Value); }
        friend AltFloat operator%=(AltFloat* self, Int64 Value) { return ModulusByIntOp(**self, Value); }
        
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
            Int64 ValueAsInt = (Int64)*this;
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

    };

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
}
	