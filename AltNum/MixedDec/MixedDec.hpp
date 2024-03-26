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
#include "..\..\DLLAPI.h"
#endif

#pragma region PreprocessorToggles
#include "MixedDecPreprocessors.h"
#pragma endregion PreprocessorToggles

#ifdef MixedDec_DeriveFromAltDec
	#include "..\AltDec\AltDecBase.hpp"
#elif MixedDec_DeriveFromFlaggedDec
	#include "..\FlaggedDec\FlaggedDecBase.hpp"
#elif MixedDec_DeriveFromMediumDecV2
	#include "..\MediumDecV2\MediumDecV2Base.hpp"
	#include "..\MediumDecV2\MediumDecV2.hpp"
#else
	#include "..\MediumDec\MediumDecBase.hpp"
	#include "..\MediumDec\MediumDec.hpp"
#endif

namespace BlazesRusCode
{

    class MixedDec;
	
	//Hybrid fixed point storage with trailing digits stored as float
	// (or optionally larger floating point models if preprocessor flag is set to toggle)
	class DLL_API MixedDec:
#ifdef MixedDec_DeriveFromAltDec
    public virtual AltDecBase
#elif MixedDec_DeriveFromFlaggedDec
    public virtual FlaggedDecBase
#elif MixedDec_DeriveFromMediumDecV2
    public virtual MediumDecV2Base
#else
    public virtual MediumDecBase
#endif
    {
protected:
		//How many trailing digits to display when converted to string
		static TrailingDigitsDisplayed = 9;
#pragma region DigitStorage
	#if defined(MixedDec_EnableRestrictedFloat)
	
	class RestrictedFloat;
	public class RestrictFloatResult
	{
	public:
		bool UnderFlowed;
		bool Overflowed;
		RestrictedFloat Result;
		RestrictedFloat ExcessAmount;
		
		RestrictFloatResult()
		{
			UnderFlowed = false;
			Overflowed = false;
		}
		
	protected:
	#pragma region Division Operations
        template<IntegerType IntType=int>
        void DivByIntOp(const IntType& rValue)
		{
			//To-Do:Add code here
		}
		
        //Division by AltFloat operation
        void DivOp(const AltFloat& rValue)
		{
			//To-Do:Add code here
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void DivByMediumDecVOp(const VariantType& rValue)
		{
			//To-Do:Add code here
		}
	#pragma endregion Division Operations

	#pragma region Multiplication Operations
        //Multiply by Integer Operation
        template<IntegerType IntType=int>
        void MultByIntOp(const IntType& rValue)
		{
			//To-Do:Add code here
		}
		
        //Multiply operation
        void MultOp(const AltFloat& rValue)
		{
			//To-Do:Add code here
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void MultByMediumDecVOp(const VariantType& rValue)
		{
			//To-Do:Add code here
		}
	#pragma endregion Multiplication Operations

	#pragma region Addition Operations
        //Addition by Integer Operation
        template<IntegerType IntType=int>
        void AddByIntOp(const IntType& rValue)
		{
			//To-Do:Add code here
		}
		
        //Addition by AltFloat Operation
        void AddOp(const AltFloat& rValue)
		{
			//To-Do:Add code here
		}
		
        //Addition by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void AddByMediumDecVOp(const VariantType& rValue)
		{
			//To-Do:Add code here
		}
	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations
        //Subtraction by Integer Operation
        template<IntegerType IntType=int>
        void SubtractByIntOp(const IntType& rValue)
		{
			//To-Do:Add code here
		}
		
        //Subtraction by AltFloat Operation
        void SubtractOp(const AltFloat& rValue)
		{
			//To-Do:Add code here
		}
		
        //Subtraction by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void SubtractByMediumDecVOp(const VariantType& rValue)
		{
			//To-Do:Add code here
		}
	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations
        template<IntegerType IntType=int>
        void ModByIntOp(const IntType& rValue)
		{
			//To-Do:Add code here
		}
		
        //Modulus by AltFloat operation
        void ModOp(const AltFloat& rValue)
		{
			//To-Do:Add code here
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=MediumDec>
        void ModByMediumDecVOp(const VariantType& rValue)
		{
			//To-Do:Add code here
		}
	#pragma endregion Modulus Operations
		
    #pragma region Main Operator Overrides
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>RestrictedFloat</returns>
        friend TrailingDigitResult operator/(RestrictedFloat self, RestrictedFloat Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>RestrictedFloat</returns>
        friend TrailingDigitResult& operator/=(RestrictedFloat& self, RestrictedFloat Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>RestrictedFloat</returns>
        friend TrailingDigitResult operator*(RestrictedFloat self, RestrictedFloat Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>RestrictedFloat</returns>
        friend TrailingDigitResult& operator*=(RestrictedFloat& self, RestrictedFloat Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>RestrictedFloat</returns>
        friend TrailingDigitResult operator+(RestrictedFloat self, RestrictedFloat Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>RestrictedFloat</returns>
        friend TrailingDigitResult& operator+=(RestrictedFloat& self, RestrictedFloat Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>RestrictedFloat</returns>
        friend TrailingDigitResult operator-(RestrictedFloat self, RestrictedFloat Value) { return SubtractOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>RestrictedFloat</returns>
        friend TrailingDigitResult& operator-=(RestrictedFloat& self, RestrictedFloat Value) { return SubtractOp(self, Value); }

        friend TrailingDigitResult operator/(RestrictedFloat self, int Value) { return DivByIntOp(self, Value); }
        friend TrailingDigitResult& operator/=(RestrictedFloat& self, int Value) { return DivByIntOp(self, Value); }
		
        friend TrailingDigitResult operator*(RestrictedFloat self, int Value) { return MultByIntOp(self, Value); }
        friend TrailingDigitResult& operator*=(RestrictedFloat& self, int Value) { return MultByIntOp(self, Value); }

        friend TrailingDigitResult operator+(RestrictedFloat self, int Value) { return AddByIntOp(self, Value); }
        friend TrailingDigitResult& operator+=(RestrictedFloat& self, int Value) { return AddByIntOp(self, Value); }
		
        friend TrailingDigitResult operator-(RestrictedFloat self, int Value) { return SubtractByIntOp(self, Value); }
        friend TrailingDigitResult& operator-=(RestrictedFloat& self, int Value) { return SubtractByIntOp(self, Value); }
		
        friend TrailingDigitResult operator/(RestrictedFloat self, MediumDec Value) { return DivByMediumDecVOp(self, Value); }
        friend TrailingDigitResult& operator/=(RestrictedFloat& self, MediumDec Value) { return DivByMediumDecVOp(self, Value); }
		
        friend TrailingDigitResult operator*(RestrictedFloat self, MediumDec Value) { return MultByMediumDecVOp(self, Value); }
        friend TrailingDigitResult& operator*=(RestrictedFloat& self, MediumDec Value) { return MultByMediumDecVOp(self, Value); }

        friend TrailingDigitResult operator+(RestrictedFloat self, MediumDec Value) { return AddByMediumDecVOp(self, Value); }
        friend TrailingDigitResult& operator+=(RestrictedFloat& self, MediumDec Value) { return AddByMediumDecVOp(self, Value); }
		
        friend TrailingDigitResult operator-(RestrictedFloat self, MediumDec Value) { return SubtractByMediumDecVOp(self, Value); }
        friend TrailingDigitResult& operator-=(RestrictedFloat& self, MediumDec Value) { return SubtractByMediumDecVOp(self, Value); }

    #pragma endregion Main Operator Overrides
	}
	
    /// <summary>
    /// Alternative fixed point number representation designed for use with MixedDec
	/// Represents floating range between 0 and just before 1
    /// Stores Exponents in reverse order as AltFloat_UseNormalFloatingRange toggle
    /// Each InvertedExp holds DenomMax number of fractionals between InvertedExp and next highest InvertedExp
    /// (4-5 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
	public class RestrictedFloat
	{
	public:
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
		
		//#pragma options align=bit_packed
    #if defined(RestrictedFloat_UseSmallerFractional)
		unsigned short SignifNum;// : 16;
    #else
		unsigned int SignifNum;// : 32;
    #endif
	    //Refers to InvertedExp inside "1/2^InvertedExp + (1/2^InvertedExp)*SignifNum/DenomMax" formula
		//Unless InvertedExp==256 and SignifNum==0, in which case the value is at zero
		unsigned char InvertedExp;
		/#pragma options align=reset
		
		static unsigned char ZeroRep = 256;
		
    #if defined(RestrictedFloat_UseSmallerFractional)
        /// <summary>
        /// Initializes a new instance of the <see cref="RestrictedFloat"/> class.
        /// </summary>
        RestrictedFloat(unsigned short signifNum=0, unsigned char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            InvertedExp = exponent;
        }
    #else
        /// <summary>
        /// Initializes a new instance of the <see cref="RestrictedFloat"/> class.
        /// </summary>
        RestrictedFloat(unsigned int signifNum=0, unsigned char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            InvertedExp = exponent;
        }
    #endif
	
        //Detect if at exactly zero
		bool IsZero()
		{
            return SignifNum==0&&InvertedExp==256;
		}
	
        //Detect if at exactly one
		bool IsOne()
		{
            return SignifNum==0&&InvertedExp==OneRep;
		}
	
        void SetAsNegativeOne()
        {
            SignifNum.IsNegative = 1;
            SignifNum.Numerator = 0;
            Exponent = 0;
        }
		
        /// <summary>
        /// Sets value as smallest non-zero whole number that is not approaching zero
        /// </summary>
        void SetAsSmallestNonZero()
        {
            SignifNum = 0;
            InvertedExp = 255;
        }
		
    #pragma region ValueDefines
protected:
        static AltFloat ZeroValue()
        {
            return AltFloat();
        }
		
        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltFloat</returns>
        static AltFloat Point5Value()
        {
            return AltFloat(0, 1);
		}
		
        static AltFloat JustAboveZeroValue()
        {
            return AltFloat(0, 255);
        }
		
	#pragma endregion ValueDefines
public:

		//Outputs string in "2^Exponent + SignifNum*(2^(Exponent - DenomMaxExponent))" format 
		std::string ToFormulaFormat()
		{
			if(IsZero())
				return "0";
			unsigned int InvertedExpMult = InvertedExp + DenomMaxExponent;
			string outputStr = "1/2^"
            outputStr += (std::string)InvertedExp;
            outputStr += " + ";
            outputStr += (std::string)SignifNum;
            outputStr += " * ";
			string outputStr = "1/2^"
            outputStr += (std::string)InvertedExpMult;
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

	};

		RestrictedFloat TrailingDigits;
	#elif defined(MixedDec_EnableAltFloat)
		AltFloat TrailingDigits;
	#else
		float TrailingDigits;
	#endif
#pragma endregion DigitStorage

public:

#pragma region Check_if_Zero
        //Detect if at exactly zero
		bool IsZero() const
		{
            return DecimalHalf==0&&IntValue==0&&TrailingDigits==0;
		}

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0;
	#if defined(MixedDec_DeriveFromAltDec)
			ExtraRep = 0;
	#endif
	#if !defined(MixedDec_UseAltFloat)
			TrailingDigits = 0.0f;
	#else//AltFloat range planned to be from 0.0 to (0.9..9) with no support for infinity
		//AltFloat support range planned to be extended to 0.0 to 9..9.9..9 range during multiplication/division operations
		//AltFloat support range planned to be extended to 1.9..9 range during addition/subtraction operation
			TrailingDigits = 0;
	#endif
        }
#pragma endregion Check_if_Zero

        bool IsWholeNumber()
        {
#if defined(MixedDec_DeriveFromAltDec)||defined(MixedDec_DeriveFromFlaggedDec)

#else
            return DecimalHalf==0 && TrailingDigits==0.0f;
#endif
        }

        bool IsAtOrBelowTwo()
        {
#if defined(MixedDec_DeriveFromAltDec)||defined(MixedDec_DeriveFromFlaggedDec)

#else
            if(IntValue<=1)
                return true;
            else
                return IntValue==2 && DecimalHalf==0 && TrailingDigits==0.0f;
#endif
        }

        bool IsAtOrBelowOne()
        {
#if defined(MixedDec_DeriveFromAltDec)||defined(MixedDec_DeriveFromFlaggedDec)

#else
            if(IntValue<=0)
                return true;
            else
                return IntValue==1 && DecimalHalf==0 && TrailingDigits==0.0f;
#endif
        }

#pragma region class_constructors
        /// <summary>
        /// Initializes a new instance of the <see cref="MixedDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
#if defined(AltDec_UseMirroredInt)
	#if defined(MixedDec_DontInitializeFromConstRef)
        MixedDec(MirroredInt intVal, signed int decVal = 0
	#else
        MixedDec(const MirroredInt& intVal, const signed int& decVal = 0
	#endif
#else
	#if defined(MixedDec_DontInitializeFromConstRef)
        MixedDec(signed int intVal, signed int decVal = 0
	#else
        MixedDec(const int& intVal, signed int decVal = 0
	#endif
#endif
#if defined(MixedDec_ExtraRepEnabled)
	    #if defined(MixedDec_DontInitializeFromConstRef)
        , signed int extraVal = 0
	    #else
        , const int& extraVal = 0
	    #endif
#endif
#if defined(MixedDec_DeriveFromFlaggedDec)
	#if defined(MixedDec_DontInitializeFromConstRef)
        #if !defined(FlaggedNum_UseBitset)
        unsigned char flagsActive=0
        #else
        std::bitset<6> flagsActive = {})
        #endif
	#else
        #if !defined(FlaggedNum_UseBitset)
        const unsigned char& flagsActive=0
        #else
        const std::bitset<6>& flagsActive = {})
        #endif
	#endif
#endif
#if !defined(MixedDec_UseAltFloat)
		, float trailingDigits = 0.0f)
#else
		, AltFloat trailingDigits = 0)
#endif
        {
            IntValue.Value = intVal;
            DecimalHalf = decVal;
#if defined(MixedDec_ExtraRepEnabled)
			ExtraRep = extraVal;
#endif
#if defined(MixedDec_DeriveFromFlaggedDec)
			FlagsActive = flagsActive;
#endif
			TrailingDigits = trailingDigits;
        }

        MixedDec(const MixedDec&) = default;

        MixedDec& operator=(const MediumDec&) = default;
#pragma endregion class_constructors

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MixedDec Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
#if defined(MixedDec_ExtraRepEnabled)
			ExtraRep = extraVal;
#endif
#if defined(MixedDec_DeriveFromFlaggedDec)
			FlagsActive = flagsActive;
#endif
			TrailingDigits = trailingDigits;
        }
		
    protected:
    #pragma region Const Representation values
    //No new member variables for this section(reference parent values instead)
    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Enum representing value type stored
        /// </summary>
        enum class RepType: int
        {
            NormalType = 0,
            NumByDiv = 8,//with flag 04 active
#if defined(MixedDec_EnablePiRep)
            PiNum = 1,//with flag 01 active
	#if defined(MixedDec_EnablePowers)
            PiPower = 17,//with flag 05, and 01 active
	#endif
	#if defined(MixedDec_EnablePiFractions)
			PiNumByDiv = 9,//  (Value/ExtraRep)*Pi Representation with flag 04 and 01 active
	#endif
#endif
#if defined(MixedDec_EnableERep)
            ENum = 2,//with flag 02 active
	#if defined(MixedDec_EnablePowers)
            EPower = 18,//with flag 05, and 02 active
	#endif
	#if defined(MixedDec_EnableEFractions)
            ENumByDiv = 10,//(Value/ExtraRep)*e Representation with flag 04 and 02 active
	#endif
#endif
#if defined(MixedDec_EnableImaginaryNum)
            INum = 4,//with flag 03 active
	#if defined(MixedDec_EnableIFractions)
            INumByDiv = 11,//(Value/ExtraRep)*i Representation with flag 04 and 03 active
	#endif
#endif
#if defined(MixedDec_EnableMixedFractional)
			MixedFrac = 32,//IntValue +- DecimalHalf/ExtraRep with flag 06 active 
			MixedPi = 33,//IntValue +- DecimalHalf/ExtraRep) with flag 06, and 01 active
			MixedE = 34,//IntValue +- DecimalHalf/ExtraRep) with flag 06, and 02 active
			MixedI = 36,//IntValue +- DecimalHalf/ExtraRep) with flag 06, and 03 active
#endif

#if defined(MixedDec_EnableInfinityRep)
			PositiveInfinity = 192,//IntValue == 1 and DecimalHalf==InfinityRep
			NegativeInfinity = 112,//IntValue == -1 and DecimalHalf==InfinityRep
#endif
	#if defined(MixedDec_EnableApproachingValues)
            ApproachingBottom = 64,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)  (Enum Bits:7,5)
		#if !defined(MixedDec_DisableApproachingTop)
            ApproachingTop = 72,//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9) (Enum Bits:7,6)
		#endif
		#if defined(MixedDec_EnableApproachingDivided)
			ApproachingMidLeft = 80,//(Enum Bits:7,5,8)
			#if !defined(MixedDec_DisableApproachingTop)
            ApproachingMidRight = 96,//(Enum Bits:7,6,8)
			#endif
		#endif
	#endif
    #if defined(MixedDec_EnableNaN)
            Undefined = 128,//(Enum Bits:8)
            NaN = 129,//(Enum Bits:8, 1)
    #endif
#if defined(MixedDec_EnableApproachingPi)
            ApproachingTopPi = 65,//equal to IntValue.9..9 Pi (Enum Bits:7,1,5)
#endif
#if defined(MixedDec_EnableApproachingE)
            ApproachingTopE = 66,//equal to IntValue.9..9 e (Enum Bits:7,2, 5)
#endif
#if defined(MixedDec_EnableImaginaryInfinity)
            PositiveImaginaryInfinity = 196,//IntValue == 1 and DecimalHalf==InfinityRep and flag 03 active (Enum Bits:7,3)
			NegativeImaginaryInfinity = 228,//IntValue == -1 and DecimalHalf==InfinityRep and flag 03 active (Enum Bits:7,3,4)
	#if defined(MixedDec_EnableApproachingI)
            ApproachingImaginaryBottom = 68,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
		#if !defined(MixedDec_DisableApproachingTop)
            ApproachingImaginaryTop = 76,//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
		#endif
	#if defined(MixedDec_EnableApproachingDivided)
            ApproachingImaginaryMidRight = 84,//(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive, IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
		#if !defined(MixedDec_DisableApproachingTop)
			ApproachingImaginaryMidLeft = 100,//(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive, IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
		#endif
	#endif
#endif
#if defined(MixedDec_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
            UndefinedButInRange = 130,//(Enum Bits:8, 2)
#endif
    #if defined(MixedDec_EnableNilRep)
            Nil = 131,
    #endif
            UnknownType = 132
        };

		static std::string RepTypeAsString(RepType& repType)
		{
			switch(repType)
			{
				case RepType::NormalType:
					return "NormalType"; break;
	#if defined(MixedDec_EnableFractionals)
				case RepType::NumByDiv:
					return "NumByDiv"; break;
	#endif
	#if defined(MixedDec_EnablePiRep)
				case RepType::PiNum:
					return "PiNum"; break;
		#if defined(MixedDec_EnablePiPowers)
				case RepType::PiPower:
					return "PiPower"; break;
		#endif
		#if defined(MixedDec_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
					return "PiNumByDiv"; break;
		#endif
	#endif
	#if defined(MixedDec_EnableERep)
				case RepType::ENum:
					return "ENum"; break;
		#if defined(MixedDec_EnableFractionals)
				case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
					return "ENumByDiv"; break;
		#endif
	#endif
	#if defined(MixedDec_EnableImaginaryNum)
				case RepType::INum:
                    return "INum"; break;
		#if defined(MixedDec_EnableFractionals)
				case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
					return "INumByDiv"; break;
		#endif
		#ifdef MixedDec_EnableComplexNumbers
				case RepType::ComplexIRep:
					return "ComplexIRep"; break;
		#endif
	#endif

	#if defined(MixedDec_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
					return "MixedFrac"; break;
		#if defined(MixedDec_EnableMixedPiFractional)
				case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedPi"; break;
		#elif defined(MixedDec_EnableMixedEFractional)
				case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedE"; break;
		#elif defined(MixedDec_EnableMixedIFractional)
				case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedI"; break;
		#endif
	#endif

	#if defined(MixedDec_EnableInfinityRep)
				case RepType::PositiveInfinity://If Positive Infinity: then convert number into MaximumValue instead when need as real number
					return "PositiveInfinity"; break;
				case RepType::NegativeInfinity://If Negative Infinity: then convert number into MinimumValue instead when need as real number
					return "NegativeInfinity"; break;
	#endif
	#if defined(MixedDec_EnableApproachingValues)
				case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                    return "ApproachingBottom"; break;
		#if !defined(MixedDec_DisableApproachingTop)
				case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                    return "ApproachingTop"; break;
		#endif
		#if defined(MixedDec_EnableApproachingDivided)
				case RepType::ApproachingMidLeft:
					return "ApproachingMidLeft"; break;
				case RepType::ApproachingMidRight:
					return "ApproachingMidRight"; break;
		#endif
	#endif
    #if defined(MixedDec_EnableNaN)
				case RepType::Undefined:
					return "Undefined"; break;
				case RepType::NaN:
					return "NaN"; break;
    #endif
	#if defined(MixedDec_EnableApproachingPi)
				case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
					return "ApproachingTopPi"; break;
	#endif
	#if defined(MixedDec_EnableApproachingE)
				case RepType::ApproachingTopE://equal to IntValue.9..9 e
					return "ApproachingTopE"; break;
	#endif
	#if defined(MixedDec_EnableImaginaryInfinity)
				case RepType::PositiveImaginaryInfinity:
					return "PositiveImaginaryInfinity"; break;
				case RepType::NegativeImaginaryInfinity:
					return "NegativeImaginaryInfinity"; break;
	#endif
	#if defined(MixedDec_EnableApproachingI)
				case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
					return "ApproachingImaginaryBottom"; break;
		#if !defined(MixedDec_DisableApproachingTop)
				case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
					return "ApproachingImaginaryTop"; break;
		#endif
		#if defined(MixedDec_EnableApproachingDivided)
				case RepType::ApproachingImaginaryMidLeft:
					return "ApproachingImaginaryMidLeft"; break;
			#if !defined(MixedDec_DisableApproachingTop)
				case RepType::ApproachingImaginaryMidRight:
					return "ApproachingImaginaryMidRight"; break;
			#endif
		#endif
    #endif
	#if defined(MixedDec_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range: ExtraRepValue==UndefinedInRangeRep)
				case RepType::UndefinedButInRange:
					return "UndefinedButInRange"; break;
		#if defined(MixedDec_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
				case RepType::WithinMinMaxRange:
					return "WithinMinMaxRange"; break;
		#endif
	#endif
    #if defined(MixedDec_EnableNilRep)
				case RepType::Nil:
					return "Nil"; break;
    #endif
				default:
					return "Unknown";
			}
		}

        RepType GetRepType()
        {
#if defined(MixedDec_DeriveFromAltDec)
        #if defined(MixedDec_EnableInfinityRep)
            if(DecimalHalf==InfinityRep)
            {
            #if defined(MixedDec_EnableImaginaryInfinity)
                if (ExtraRep == IRep)
                    return IntValue==1?RepType::PositiveImaginaryInfinity:RepType::NegativeImaginaryInfinity;
                else
            #endif
            #if defined(MixedDec_EnableUndefinedButInRange)
                if (ExtraRep == UndefinedInRangeRep)
                    return RepType::UndefinedButInRange;
                else
            #endif
            #if defined(WithinMinMaxRangeRep)
                if (ExtraRep == WithinMinMaxRangeRep)
                    return RepType::WithinMinMaxRange;
                else
            #endif
                    return IntValue==1?RepType::PositiveInfinity:RepType::NegativeInfinity;
            }
            else
        #endif
        #if defined(MixedDec_EnableApproachingValues)//old value = ApproachingValRep
            if (DecimalHalf == ApproachingBottomRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingBottom;//Approaching from right to IntValue;(IntValue of 0 results in 0.00...1)
                else
            #if defined(MixedDec_EnableApproachingDivided)//if(ExtraRep>1)
                    return RepType::ApproachingMidLeft;//ExtraRep value of 2 results in 0.49999...9
            #else
                    throw "EnableApproachingDivided feature not enabled";
            #endif
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingTop;//Approaching from left to (IntValue-1);(IntValue of 0 results in 0.99...9)
            #if defined(MixedDec_EnableApproachingPi)
                else if (ExtraRep == PiRep)
                    return RepType::ApproachingTopPi;
            #endif
            #if defined(MixedDec_EnableApproachingE)
                else if (ExtraRep == ERep)
                    return RepType::ApproachingTopE;
            #endif
                else
            #if defined(MixedDec_EnableApproachingDivided)
                    return RepType::ApproachingMidRight;//ExtraRep value of 2 results in 0.500...1
            #else
                    throw "EnableApproachingDivided feature not enabled";
            #endif
            }
            #if defined(MixedDec_EnableImaginaryInfinity)//ApproachingImaginaryValRep
            else if (DecimalHalf == ApproachingImaginaryBottomRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingImaginaryBottom;//Approaching from right to IntValue;(IntValue of 0 results in 0.00...1)
                else
                #if defined(MixedDec_EnableApproachingDivided)
                    return RepType::ApproachingImaginaryMidLeft;//ExtraRep value of 2 results in 0.49999...9
                #else
                    throw "EnableApproachingDivided feature not enabled";
                #endif
            }
            else if (DecimalHalf == ApproachingImaginaryTopRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingImaginaryTop;//Approaching from left to (IntValue-1);(IntValue of 0 results in 0.99...9)
                else
                #if defined(MixedDec_EnableApproachingDivided)
                    return RepType::ApproachingImaginaryMidRight;//ExtraRep value of 2 results in 0.500...1
                #else
                    throw "EnableApproachingDivided feature not enabled";
                #endif
            }
            #endif
        #endif
            if(ExtraRep==0)
            {
    #if defined(MixedDec_EnableNaN)
                if(DecimalHalf==NaNRep)
                    return RepType::NaN;
                else if(DecimalHalf==UndefinedRep)
                    return RepType::Undefined;
    #endif
                return RepType::NormalType;
            }
            else if(IntValue==0&&DecimalHalf==0)
            {
                return RepType::NormalType;
            }
    #ifdef MixedDec_EnablePiRep
            else if(ExtraRep==PiRep)
                return RepType::PiNum;
        #if defined(MixedDec_EnablePiFractional)
            else if(ExtraRep==PiByDivisorRep)
                return RepType::PiFractional;
        #endif
    #endif
            else if(ExtraRep>0)
            {
    #if defined(MixedDec_EnableMixedFractional)
                if(DecimalHalf<0)
                    return RepType::MixedFrac;
    #endif
    #if defined(MixedDec_EnableFractionals)
                return RepType::NumByDiv;
    #endif
                throw "Non-enabled representation detected";
            }
    #if defined(MixedDec_EnableERep)
            else if(ExtraRep==ERep)
            {
                return RepType::ENum;
            }
        #if defined(MixedDec_EnableEFractional)
            else if(ExtraRep==EByDivisorRep)//(IntValue/DecimalHalf)*e
                return RepType::EFractional;
        #endif
    #endif

    #if defined(MixedDec_EnableImaginaryNum)
            else if(ExtraRep==IRep)
            {
                return RepType::INum;
            }
        #if defined(MixedDec_EnableIFractional)
            else if(ExtraRep==IByDivisorRep)
                    return RepType::IFractional;
        #endif
    #endif
    #if defined(MixedDec_EnableUndefinedButInRange)//Such as result of Cos of infinity
           else if(ExtraRep==UndefinedButInRange)
                return RepType::UndefinedButInRange;//If DecimalHalf equals InfinityRep, than equals undefined value with range between negative infinity and positive infinity (negative range values indicates inverted range--any but the range of values)
        #if defined(MixedDec_EnableWithinMinMaxRange)
            //If IntValue==NegativeRep, then left side range value equals negative infinity
            //If DecimalHalf==InfinityRep, then right side range value equals positive infinity
           else if(ExtraRep==WithinMinMaxRangeRep)
                return RepType::WithinMinMaxRange;
        #endif
    #endif
            else if(ExtraRep<0)
    #if defined(MixedDec_EnableAlternativeMixedFrac)
                if(DecimalHalf<0)
        #if defined(MixedDec_EnableMixedPiFractional)
                    return RepType::MixedPi;
        #elif defined(MixedDec_EnableMixedEFractional)
                    return RepType::MixedE;
        #elif defined(MixedDec_EnableMixedIFractional)
                    return RepType::MixedI;
        #else
                    throw "Non-enabled Alternative Mixed Fraction representation type detected";
        #endif
                else
    #endif
    #if defined(MixedDec_EnableDecimaledPiFractionals)
                    return RepType::PiNumByDiv;
    #elif defined(MixedDec_EnableDecimaledEFractionals)
                    return RepType::ENumByDiv;
    #elif defined(MixedDec_EnableDecimaledIFractionals)
                    return RepType::INumByDiv;
    #else
                    throw "Non-enabled Negative ExtraRep representation type detected";
    #endif
            else
                throw "Unknown or non-enabled representation type detected";
            return RepType::UnknownType;//Catch-All Value;
#elif defined(MixedDec_DeriveFromFlaggedDec)
    #if defined(MixedDec_EnableInfinityRep)
            if(DecimalHalf==InfinityRep)
            {
	    #if defined(MixedDec_EnableImaginaryInfinity)
		    #if !defined(MixedDec_UseBitset)
                if (FlagsActive == INumRep)
		    #else
				if(FlagsActive.test(3))
		    #endif
				    return IntValue==1?RepType::PositiveImaginaryInfinity:RepType::NegativeImaginaryInfinity;
				else
	    #endif
					return IntValue==1?RepType::PositiveInfinity:RepType::NegativeInfinity;
            }
			else
    #endif
    #if defined(MixedDec_EnableApproachingValues)
            if (DecimalHalf == ApproachingBottomRep)
            {
				if(ExtraRep==0)
					return RepType::ApproachingBottom;//Approaching from right to IntValue;(IntValue of 0 results in 0.00...1)
				else
	    #if defined(MixedDec_EnableApproachingDivided)//if(ExtraRep>1)
					return RepType::ApproachingMidLeft;//ExtraRep value of 2 results in 0.49999...9
	    #else
                    throw "EnableApproachingDivided feature not enabled";
	    #endif	
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingTop;//Approaching from left to (IntValue-1);(IntValue of 0 results in 0.99...9)
	    #if defined(MixedDec_EnableApproachingPi)
		    #if !defined(MixedDec_UseBitset)
                else if (FlagsActive == PiNumRep)
		    #else
				else if(FlagsActive.test(1))
		    #endif
                    return RepType::ApproachingTopPi;
	    #endif
	    #if defined(MixedDec_EnableApproachingE)
		    #if !defined(MixedDec_UseBitset)
                else if (FlagsActive == ENumRep)
		    #else
				else if(FlagsActive.test(2))
		    #endif
                    return RepType::ApproachingTopE;
	    #endif
                else
	    #if defined(MixedDec_EnableApproachingDivided)
					return RepType::ApproachingMidRight;//ExtraRep value of 2 results in 0.500...1
	    #else
                    throw "EnableApproachingDivided feature not enabled";
	    #endif            
            }
	    #if defined(MixedDec_EnableImaginaryInfinity)//ApproachingImaginaryValRep
            else if (DecimalHalf == ApproachingImaginaryBottomRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingImaginaryBottom;//Approaching from right to IntValue;(IntValue of 0 results in 0.00...1)
                else
		    #if defined(MixedDec_EnableApproachingDivided)
					return RepType::ApproachingImaginaryMidLeft;//ExtraRep value of 2 results in 0.49999...9
		    #else
                    throw "EnableApproachingDivided feature not enabled";
		    #endif            
            }
            else if (DecimalHalf == ApproachingImaginaryTopRep)
            {
				if(ExtraRep==0)
				    return RepType::ApproachingImaginaryTop;//Approaching from left to (IntValue-1);(IntValue of 0 results in 0.99...9)
				else
	        #if defined(MixedDec_EnableApproachingDivided)
					return RepType::ApproachingImaginaryMidRight;//ExtraRep value of 2 results in 0.500...1
	        #else
                    throw "EnableApproachingDivided feature not enabled";
            #endif            
            }
        #endif
    #endif
    #if defined(MixedDec_EnableUndefinedButInRange)//Such as result of Cos of infinity
			else if(ExtraRep==UndefinedButInRange)
                return RepType::UndefinedButInRange;
    #endif
			else
			{
    #if !defined(MixedDec_UseBitset)
				switch(FlagsActive)
				{
					case 0://NormalType and NaN
	    #if defined(MixedDec_EnableNaN)
					if(DecimalHalf==NaNRep)
						return RepType::NaN;
					else if(DecimalHalf==UndefinedRep)
						return RepType::Undefined;
					else
	    #endif
						return RepType::NormalType;
						break;
	    #if defined(MixedDec_EnableFractionals)
					case NumByDivRep:
						return RepType::NumByDiv;
						break;
	    #endif
	    #if defined(MixedDec_EnablePowers)
					case NumByPowerRep:
						return RepType::NumByPower;
						break;
	    #endif
	    #if defined(MixedDec_EnableMixedFractional)
					case MixedFracRep:
						return RepType::MixedFrac;
						break;
	    #endif
	    #ifdef MixedDec_EnablePiRep
					case PiNumRep:
						return RepType::PiNum;
						break;
		    #if defined(MixedDec_EnableFractionals)
					case PiNumByDivRep:
						return RepType::PiNumByDiv;
						break;
		    #endif
		    #if defined(MixedDec_EnablePowers)
					case PiNumByPowerRep:
						return RepType::PiNumByPower;
						break;
		    #endif
		    #if defined(MixedDec_EnableMixedFractional)
					case MixedFracPiRep:
						return RepType::MixedPi;
						break;
		    #endif
	    #endif
	    #ifdef MixedDec_EnableENum
					case ENumRep:
						return RepType::ENum;
						break;
		    #if defined(MixedDec_EnableFractionals)
					case ENumByDivRep:
						return RepType::ENumByDiv;
						break;
		    #endif
		    #if defined(MixedDec_EnablePowers)
					case ENumByPowerRep:
						return RepType::ENumByPower;
						break;
		    #endif
		    #if defined(MixedDec_EnableMixedFractional)
					case MixedFracERep:
						return RepType::MixedE;
						break;
		    #endif
	    #endif
	    #ifdef MixedDec_EnableImaginaryNum
					case INumRep:
						return RepType::INum;
						break;
		    #if defined(MixedDec_EnableFractionals)
					case INumByDivRep:
						return RepType::INumByDiv;
						break;
		    #endif
		    #if defined(MixedDec_EnableMixedFractional)
					case MixedFracIRep:
						return RepType::MixedI;
						break;
		    #endif
	    #endif
				}
    #else

    #endif
			}
#else
    return RepType::NormalType;//Includes values with trailing digits as well(for now at least)
#endif
        }

    #pragma endregion RepType

public:

    #pragma region PiNum Setters

    #pragma endregion PiNum Setters

    #pragma region ENum Setters

    #pragma endregion ENum Setters

    #pragma region Fractional Setters

    #pragma endregion Fractional Setters
    
    #pragma region MixedFrac Setters

    #pragma endregion MixedFrac Setters

    #pragma region Infinity Setters

    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters

    #pragma endregion ApproachingZero Setters

    #pragma region NaN Setters

    #pragma endregion NaN Setters

    #pragma region ValueDefines

    #pragma endregion ValueDefines

    #pragma region String Commands
	
    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

    #pragma endregion ConvertToOtherTypes

    #pragma region MirroredIntBased Operations

    #pragma endregion MirroredIntBased Operations

    #pragma region Pi Conversion
	
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion

	protected:
	#pragma region Division Operations
        template<IntegerType IntType=int>
        void DivByIntOp(const IntType& rValue)
		{
			//Add Code here later
		}
		
        //Division by MixedDec operation
        void DivOp(const MixedDec& rValue)
		{
			//Add Code here later
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void DivByMediumDecVOp(const VariantType& rValue)
		{
			//Add Code here later
		}
		
#if defined(MixedDec_EnableAltFloat)
        //Division by AltFloat Operation
        void AltFloatDivByOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_UseRestrictedRange)
	#elif defined(AltFloat_DontUseBitfieldInSignif)
	#else
		// lValue /(2^rValue.Exponent + rValue.SignifNum.Value*(2^(rValue.Exponent - rValue.DenomMaxExponent)))*rValue.SignifNum.IsNegative?-1:1;
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
#endif
	#pragma endregion Division Operations

	#pragma region Multiplication Operations
        //Multiply by Integer Operation
        template<IntegerType IntType=int>
        void MultByIntOp(const IntType& rValue)
		{
#if defined(MixedDec_EnableRestrictedFloat)
#elif defined(MixedDec_EnableAltFloat)
#else
			//Add Code here later
#endif
		}
		
        //Multiply operation
        void MultOp(const MixedDec& rValue)
		{
#if defined(MixedDec_EnableRestrictedFloat)
#elif defined(MixedDec_EnableAltFloat)
#else
			//Add Code here later
#endif
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void MultByMediumDecVOp(const VariantType& rValue)
		{
#if defined(MixedDec_EnableRestrictedFloat)
			//Add Code here later
#elif defined(MixedDec_EnableAltFloat)
#else
			//Add Code here later
#endif
		}
		
#if defined(MixedDec_EnableAltFloat)
        //Multiply by AltFloat Operation
        void AltFloatMultByOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_DontUseBitfieldInSignif)
	#else
		// lValue *(2^rValue.Exponent + rValue.SignifNum.Value*(2^(rValue.Exponent - rValue.DenomMaxExponent)))*rValue.SignifNum.IsNegative?-1:1;
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
#endif
	#pragma endregion Multiplication Operations

	#pragma region Addition Operations
        //Addition by Integer Operation
        template<IntegerType IntType=int>
        void AddByIntOp(const IntType& rValue)
		{
			//Add Code here later
		}
		
        //Addition by MixedDec Operation
        void AddOp(const MixedDec& rValue)
		{
#if defined(MixedDec_EnableRestrictedFloat)
			//Add Code here later
#elif defined(MixedDec_EnableAltFloat)
#else
			//Add Code here later
#endif
		}
		
        //Addition by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void AddByMediumDecVOp(const VariantType& rValue)
		{
			//Add Code here later
		}
		
#if defined(MixedDec_EnableAltFloat)
        //Addition by AltFloat Operation
        void AltFloatAddByOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_DontUseBitfieldInSignif)
	#else
		// lValue +(2^rValue.Exponent + rValue.SignifNum.Value*(2^(rValue.Exponent - rValue.DenomMaxExponent)))*rValue.SignifNum.IsNegative?-1:1;
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
#endif
	#pragma endregion Addition Operations
	
	#pragma region Subtraction Operations
        //Subtraction by Integer Operation
        template<IntegerType IntType=int>
        void SubtractByIntOp(const IntType& rValue)
		{
			//Add Code here later
		}
		
        //Subtraction by MixedDec Operation
        void SubtractOp(const MixedDec& rValue)
		{
#if defined(MixedDec_EnableRestrictedFloat)
			//Add Code here later
#elif defined(MixedDec_EnableAltFloat)
#else
			//Add Code here later
#endif
		}
		
        //Subtraction by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void SubtractByMediumDecVOp(const VariantType& rValue)
		{
			//Add Code here later
		}
		
#if defined(MixedDec_EnableAltFloat)
        //Subtraction by AltFloat Operation
        void AltFloatSubtractByOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_DontUseBitfieldInSignif)
	#else
		// lValue -(2^rValue.Exponent + rValue.SignifNum.Value*(2^(rValue.Exponent - rValue.DenomMaxExponent)))*rValue.SignifNum.IsNegative?-1:1;
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
#endif
	#pragma endregion Subtraction Operations
	
	#pragma region Modulus Operations
        template<IntegerType IntType=int>
        void ModByIntOp(const IntType& rValue)
		{
			//Add Code here later
		}
		
        //Modulus by MixedDec operation
        void ModOp(const MixedDec& rValue)
		{
			//Add Code here later
		}
		
        //Multiply by MediumDec variant Operation
        template<MediumDecVariant VariantType=int>
        void ModByMediumDecVOp(const VariantType& rValue)
		{
			//Add Code here later
		}
		
#if defined(MixedDec_EnableAltFloat)
        //Modulus by AltFloat Operation
        void AltFloatModByOp(const AltFloat& rValue)
		{
	#if defined(AltFloat_DontUseBitfieldInSignif)
	#else
		// lValue %(2^rValue.Exponent + rValue.SignifNum.Value*(2^(rValue.Exponent - rValue.DenomMaxExponent)))*rValue.SignifNum.IsNegative?-1:1;
		#if defined(AltFloat_ExtendedRange)
		#else
		#endif
	#endif
		}
#endif
	#pragma endregion Modulus Operations
	
	public:
    #pragma region Main Operator Overrides
	
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MixedDec</returns>
        friend MixedDec operator/(MixedDec self, MixedDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MixedDec</returns>
        friend MixedDec& operator/=(MixedDec& self, MixedDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MixedDec</returns>
        friend MixedDec operator*(MixedDec self, MixedDec Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>MixedDec</returns>
        friend MixedDec& operator*=(MixedDec& self, MixedDec Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MixedDec</returns>
        friend MixedDec operator+(MixedDec self, MixedDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MixedDec</returns>
        friend MixedDec& operator+=(MixedDec& self, MixedDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MixedDec</returns>
        friend MixedDec operator-(MixedDec self, MixedDec Value) { return SubtractOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MixedDec</returns>
        friend MixedDec& operator-=(MixedDec& self, MixedDec Value) { return SubtractOp(self, Value); }

        friend MixedDec operator/(MixedDec self, int Value) { return DivByIntOp(self, Value); }
        friend MixedDec& operator/=(MixedDec& self, int Value) { return DivByIntOp(self, Value); }
		
        friend MixedDec operator*(MixedDec self, int Value) { return MultByIntOp(self, Value); }
        friend MixedDec& operator*=(MixedDec& self, int Value) { return MultByIntOp(self, Value); }

        friend MixedDec operator+(MixedDec self, int Value) { return AddByIntOp(self, Value); }
        friend MixedDec& operator+=(MixedDec& self, int Value) { return AddByIntOp(self, Value); }
		
        friend MixedDec operator-(MixedDec self, int Value) { return SubtractByIntOp(self, Value); }
        friend MixedDec& operator-=(MixedDec& self, int Value) { return SubtractByIntOp(self, Value); }
	
        friend MixedDec operator/(MixedDec self, MediumDec Value) { return DivByMediumDecVOp(self, Value); }
        friend MixedDec& operator/=(MixedDec& self, MediumDec Value) { return DivByMediumDecVOp(self, Value); }
		
        friend MixedDec operator*(MixedDec self, MediumDec Value) { return MultByMediumDecVOp(self, Value); }
        friend MixedDec& operator*=(MixedDec& self, MediumDec Value) { return MultByMediumDecVOp(self, Value); }

        friend MixedDec operator+(MixedDec self, MediumDec Value) { return AddByMediumDecVOp(self, Value); }
        friend MixedDec& operator+=(MixedDec& self, MediumDec Value) { return AddByMediumDecVOp(self, Value); }
		
        friend MixedDec operator-(MixedDec self, MediumDec Value) { return SubtractByMediumDecVOp(self, Value); }
        friend MixedDec& operator-=(MixedDec& self, MediumDec Value) { return SubtractByMediumDecVOp(self, Value); }
	
        friend AltDec operator/(AltFloat self, MixedDec Value) { return AltFloatDivByOp(self, Value); }
        friend AltDec& operator/=(AltFloat& self, MixedDec Value) { return AltFloatDivByOp(self, Value); }
		
        friend AltDec operator*(AltFloat self, MixedDec Value) { return AltFloatMultByOp(self, Value); }
        friend AltDec& operator*=(AltFloat& self, MixedDec Value) { return AltFloatMultByOp(self, Value); }

        friend AltDec operator+(AltFloat self, MixedDec Value) { return AltFloatAddByOp(self, Value); }
        friend AltDec& operator+=(AltFloat& self, MixedDec Value) { return AltFloatAddByOp(self, Value); }
		
        friend AltDec operator-(AltFloat self, MixedDec Value) { return AltFloatSubtractByOp(self, Value); }
        friend AltDec& operator-=(AltFloat& self, MixedDec Value) { return AltFloatSubtractByOp(self, Value); }
	
    #pragma endregion Main Operator Overrides
	
    #pragma region Other Operators
	
        /// <summary>
        /// ++MixedDec Operator
        /// </summary>
        /// <returns>MixedDec &</returns>
        MixedDec& operator ++()
        {
            *this = *this + One;
            return *this;
        }

        /// <summary>
        /// --MixedDec Operator
        /// </summary>
        /// <returns>MixedDec &</returns>
        MixedDec& operator --()
        {
            *this = *this - One;
            return *this;
        }

        /// <summary>
        /// MixedDec++ Operator
        /// </summary>
        /// <returns>MixedDec</returns>
        MixedDec operator ++(int)
        {
            MixedDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MixedDec-- Operator
        /// </summary>
        /// <returns>MixedDec</returns>
        MixedDec operator --(int)
        {
            MixedDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MixedDec* Operator
        /// </summary>
        /// <returns>MixedDec &</returns>
        MixedDec& operator *()
        {
            return *this;
        }
	
    #pragma endregion Other Operators
	
	
    }
    #pragma region String Function Source
}