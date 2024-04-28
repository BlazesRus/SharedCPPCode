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
//Int 128 needed to extract trailing digits lost from division and multiplication
#include <boost/multiprecision/cpp_int.hpp>
/*
RestrictedFloat_ExtendedRange = Extends SignifNum range to 2147483647
RestrictedFloat_EnableApproachingZero = Not Implimented yet
RestrictedFloat_EnableInfinity = Not Implimented yet
RestrictedFloat_EnableBitwiseOperations = Not Implimented yet
RestrictedFloat_UseXorAsPowerOf = Not Implimented yet
RestrictedFloat_UseSmallerFractional = Restricts SignifNum range to 16 bits
RestrictedFloat_GiveErrorInsteadOfMaxingOnOverflowConversion
*/

#if defined(RestrictedFloat_UseSmallerFractional)
	#define RestrictedFloat_SignifType unsigned short
#else
	#define RestrictedFloat_SignifType unsigned int//Actual stored number is unsigned int:16 if RestrictedFloat_PackInSmallerBits toggled
#endif

namespace BlazesRusCode
{
	//Right side operations are assumed to be positive
	//^ in comments refers to power of instead of XOR

    class RestrictedFloat;
	using UInt128 = boost::multiprecision::uint128_t;

    /// <summary>
    /// Alternative fixed point number representation designed for use with MixedDec
	/// Represents floating range between 0 and just before 1
    /// Stores Exponents in reverse order as RestrictedFloat_UseNormalFloatingRange toggle
    /// Each Exp holds DenomMax number of fractionals between Exp and next highest Exp
    /// (3-5 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
	public class RestrictedFloat
	{
	public:
        #if defined(RestrictedFloat_UseSmallerFractional)//Restrict to 2 bytes worth of SignifNum instead
		//Equal to 2^16
		static unsigned long long DenomMax = 65536;
		//Equal to (2^31) - 1
		static unsigned long long MaxSignif = 65535;
		static unsigned int DenomMaxExponent = 16;
        #else
		//Equal to 2^32
		static unsigned long long DenomMax = 4294967296;
		//Equal to (2^31) - 1
		static unsigned long long MaxSignif = 4294967295;
		static unsigned int DenomMaxExponent = 32;
        #endif
	#if defined(RestrictedFloat_PackInSmallerBits)
		#pragma options align=bit_packed
		unsigned int SignifNum: 16;
	#elif defined(RestrictedFloat_UseSmallerFractional)
		unsigned short SignifNum;// : 16;
    #else
		unsigned int SignifNum;// : 32;
    #endif
		//Inverted Exponent Section of Formula Representation
	    //Refers to Exp inside "1/2^Exp + (1/2^Exp)*SignifNum/DenomMax" formula
		//Unless Exp==256 and SignifNum==0, in which case the value is at zero
	#if defined(RestrictedFloat_PackInSmallerBits)
		unsigned int Exp:8;
		#pragma options align=reset
	#else
		unsigned char Exp;
	#endif
		
		static unsigned char ZeroRep = 256;
		
        /// <summary>
        /// Initializes a new instance of the <see cref="RestrictedFloat"/> class.
        /// </summary>
        RestrictedFloat(const RestrictedFloat_SignifType& signifNum=0, const unsigned char& exponent=ZeroRep)
        {
            SignifNum = signifNum;
            Exp = exponent;
        }
	
        //Detect if at exactly zero
		bool IsZero()
		{
            return SignifNum==0&&Exp==256;
		}
	
        //Detect if at exactly one
		bool IsOne()
		{
            return SignifNum==0&&Exp==OneRep;
		}
		
        /// <summary>
        /// Sets value as smallest non-zero whole number that is not approaching zero
        /// </summary>
        void SetAsSmallestNonZero()
        {
            SignifNum = 0;
            Exp = 255;
        }
		
		
    #pragma region Const Representation values
    protected:
		static unsigned _int64 const TruncMultAsInt = 10000000000000000000;
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
    #pragma endregion Const Representation values
		
    #pragma region ValueDefines
protected:
        static RestrictedFloat ZeroValue()
        {
            return RestrictedFloat();
        }
		
        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>RestrictedFloat</returns>
        static RestrictedFloat Point5Value()
        {
            return RestrictedFloat(0, 1);
		}
		
        static RestrictedFloat JustAboveZeroValue()
        {
            return RestrictedFloat(0, 255);
        }
		
	#pragma endregion ValueDefines
public:

		//Outputs string in "1/2^Exp + (1/2^Exp)*SignifNum/DenomMax" format 
		std::string ToFormulaFormat()
		{
			if(IsZero())
				return "0";
			unsigned int ExpMult = Exp + DenomMaxExponent;
			string outputStr = "1/2^"
            outputStr += (std::string)Exp;
            outputStr += " + ";
            outputStr += (std::string)SignifNum;
            outputStr += " * ";
			string outputStr = "1/2^"
            outputStr += (std::string)ExpMult;
		}

		//Outputs string in digit display format 
		std::string ToDigitFormat()
		{
			if(IsZero())
				return ".0";
			else
			{
			}
			return "";//placeholder
		}

    #pragma region Other RepType Conversion

        /// <summary>
        /// RestrictedFloat to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toInt() { return IsOne()?1:0; }
		
        /// <summary>
        /// RestrictedFloat to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toBool() { return IsOne()?true:false; }

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators
    std::strong_ordering operator<=>(const RestrictedFloat& that) const
    {
		//"1/(2^Exp) + (1/(2^Exp))*SignifNum/DenomMax"
		
		//The Largest Exponent is the closest to zero(256 is exactly at zero)
		//Inverting comparison to make let spaceship operator compare in correct order
		unsigned int lVal = Exp==ZeroRep?0:255-Exp;
		unsigned int rVal = that.Exp==ZeroRep?0:255-that.Exp;
        if (auto ExponentCmp = lVal <=> rVal; ExponentCmp != 0)
			return ExponentCmp;
        if (auto SignifFracCmp = SignifNum.Value <=> that.SignifNum.Value; SignifFracCmp != 0)
			return SignifFracCmp;
    }
	
    bool operator==(const RestrictedFloat& that) const
    {
		if (SignifNum!=that.SignifNum)
            return false;
        if (Exponent!=that.Exponent)
            return false;
		return true;
    }
	
    auto operator<=>(const int& that) const
    {
		int lVal = toInt();
		return lVal <=> that;
    }

    bool operator==(const int& that) const
    {
		int lVal = toInt();
		return lVal == that;
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
		{//Negative Exponent values for RestrictedFloat and positive Exponent values for RestrictedFloat
			IsPositive = 1;
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

	};
}
	