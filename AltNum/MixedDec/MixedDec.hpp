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
#elif MixedDec_DeriveFromMediumDecV2
	#include "..\MediumDecV2\MediumDecV2Base.hpp"
#else
	#include "..\MediumDec\MediumDecBase.hpp"
#endif

#include "..\MediumDec\MediumDec.hpp"
#include "..\MediumDecV2\MediumDecV2.hpp"
#include "..\AltDec\AltDec.hpp"

#if defined(MixedDec_EnableAltFloat)
	#include "..\AltFloat.hpp"
#endif

namespace BlazesRusCode
{

    class MixedDec;
	
	//Hybrid fixed point storage with trailing digits stored as float
	// (or optionally larger floating point models if preprocessor flag is set to toggle)
	class DLL_API MixedDec:
#ifdef MixedDec_DeriveFromAltDec
    public virtual AltDecBase
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
	
    /// <summary>
    /// Alternative fixed point number representation designed for use with MixedDec
	/// Represents floating range between 0 and just before 1
    /// Stores Exponents in reverse order as AltFloat_UseNormalFloatingRange toggle
    /// Each Exp holds DenomMax number of fractionals between Exp and next highest Exp
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
		//Inverted Exponent Section of Formula Representation
	    //Refers to Exp inside "1/2^Exp + (1/2^Exp)*SignifNum/DenomMax" formula
		//Unless Exp==256 and SignifNum==0, in which case the value is at zero
		unsigned char Exp;
		/#pragma options align=reset
		
		static unsigned char ZeroRep = 256;
		
    #if defined(RestrictedFloat_UseSmallerFractional)
        /// <summary>
        /// Initializes a new instance of the <see cref="RestrictedFloat"/> class.
        /// </summary>
        RestrictedFloat(unsigned short signifNum=0, unsigned char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            Exp = exponent;
        }
    #else
        /// <summary>
        /// Initializes a new instance of the <see cref="RestrictedFloat"/> class.
        /// </summary>
        RestrictedFloat(unsigned int signifNum=0, unsigned char exponent=ZeroRep)
        {
            SignifNum = signifNum;
            Exp = exponent;
        }
    #endif
	
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
            Exp = 255;
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
		//"1/2^Exp + (1/2^Exp)*SignifNum/DenomMax"
		
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

        void SetTrailingDigitAsZero()
        {
	#if defined(MixedDec_StoreTrailingInFloat)
			TrailingDigits = 0.0f;
	#else
			TrailingDigits = 0;
	#endif
        }

        bool IsTrailingDigitZero()
        {
	#if defined(MixedDec_StoreTrailingInFloat)
            return TrailingDigits==0.0f;
    #else
            return TrailingDigits==0;
    #endif
        }

        bool IsTrailingDigitNotZero()
        {
	#if defined(MixedDec_StoreTrailingInFloat)
            return TrailingDigits!=0.0f;
    #else
            return TrailingDigits!=0;
    #endif
        }

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0;
	#if defined(MixedDec_DeriveFromAltDec)
			ExtraRep = 0;
	#endif
            SetTrailingDigitAsZero();
        }
#pragma endregion Check_if_Zero

        bool IsWholeNumber()
        {
#if defined(MixedDec_DeriveFromAltDec)
	#if defined(AltNum_UseIntForDecimalHalf)
		//Add code here
	#else
			if(ExtraRep>0)
			{
				//Convert to NormatType and then check
				//Add code here
			}
		#if defined(AltNum_EnableIRep)
			else if((DecimalHalf.Flags==3||DecimalHalf.Flags==0)
            &&DecimalHalf.Value<DecimalOverflow && IsTrailingDigitZero())
		#else
			else if(DecimalHalf.Flags==0&&DecimalHalf.Value<DecimalOverflow
            &&IsTrailingDigitZero())
		#endif
	#endif
				return DecimalHalf==0;
			else
				return false;
#elif defined(MixedDec_DeriveFromMediumDecV2)
	#if defined(AltNum_UseIntForDecimalHalf)
		//Add code here
	#else
		#if defined(MediumDecV2_EnableIRep)
			if((DecimalHalf.Flags==3||DecimalHalf.Flags==0)&&DecimalHalf.Value<DecimalOverflow&&TrailingDigits==0.0f)
		#else
			if(DecimalHalf.Flags==0&&DecimalHalf.Value<DecimalOverflow&& TrailingDigits==0.0f)
		#endif
	#endif
				return DecimalHalf==0;
			else
				return false;
#else
            return DecimalHalf==0 && TrailingDigits==0.0f;
#endif
        }

        bool IsAtOrBelowTwo()
        {
#if defined(MixedDec_DeriveFromAltDec)
			//Add code here
#else if defined(MixedDec_DeriveFromMediumDecV2)
			//Add code here
#else
            if(IntValue<=1)
                return true;
            else
                return IntValue==2 && DecimalHalf==0 && TrailingDigits==0.0f;
#endif
        }

        bool IsAtOrBelowOne()
        {
#if defined(MixedDec_DeriveFromAltDec)
			//Add code here
#else if defined(MixedDec_DeriveFromMediumDecV2)
			//Add code here
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
        AltDecBase(const IntHalfType& intVal, const DecimalHalfType& decVal = 0,
#if defined(MixedDec_EnableRestrictedFloat)
		RestrictedFloat trailingDigits = 0)
#elif defined(MixedDec_EnableAltFloat)
		AltFloat trailingDigits = 0)
#else
		float trailingDigits = 0.0f)
#endif
        {
			IntValue = intVal;
            DecimalHalf = decVal;
#if defined(MixedDec_DeriveFromAltDec)
			ExtraRep = extraVal;
#endif
			TrailingDigits = trailingDigits;
        }

        MixedDec(const MixedDec&) = default;
		
        MixedDec& operator=(const int& rhs)
        {
	#if defined(AltNum_EnableMirroredSection)
			if(rhs<0)
			{
				IntValue.Value = -rhs;
				IntValue.IsPositive = 0;
			}
			else
	#endif
				IntValue = rhs;
			DecimalHalf = 0;
	#if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
	#endif
            SetTrailingDigitAsZero();
            return *this;
        } const

        MixedDec& operator=(const MediumDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
		#if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = rhs.ExtraRep;
		#endif
            SetTrailingDigitAsZero();
            return *this;
        } const
		
        MixedDec& operator=(const MediumDecV2& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
		#if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = rhs.ExtraRep;
		#endif
            SetTrailingDigitAsZero();
            return *this;
        } const

        MixedDec& operator=(const AltDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
		#if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = rhs.ExtraRep;
		#endif
            SetTrailingDigitAsZero();
            return *this;
        } const

        MixedDec& operator=(const MixedDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
		#if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = rhs.ExtraRep;
		#endif
			TrailingDigits = rhs.TrailingDigits;
            return *this;
        } const
		
#pragma endregion class_constructors

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MixedDec Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
#if defined(MixedDec_DeriveFromAltDec)
			ExtraRep = Value.extraVal;
#endif
			TrailingDigits = trailingDigits;
        }
		
    protected:
    #pragma region Const Representation values
    //No new member variables for this section(reference parent values instead)
    #pragma endregion Const Representation values

    #pragma region RepType

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
				case RepType::MixedFrac://IntValue +- (DecimalHalf.Value/ExtraRep.Value)
					return "MixedFrac"; break;
		#if defined(MixedDec_EnableMixedPiFractional)
				case RepType::MixedPi://IntValue +- (DecimalHalf.Value/ExtraRep.Value)
					return "MixedPi"; break;
		#elif defined(MixedDec_EnableMixedEFractional)
				case RepType::MixedE://IntValue +- (DecimalHalf.Value/ExtraRep.Value)
					return "MixedE"; break;
		#elif defined(MixedDec_EnableMixedIFractional)
				case RepType::MixedI://IntValue +- (DecimalHalf.Value/ExtraRep.Value)
					return "MixedI"; break;
		#endif
	#endif

	#if defined(MixedDec_EnableInfinityRep)
				case RepType::Infinity:
					return "Infinity"; break;
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
		#if defined(MixedDec_EnableApproachingAlternativeDiv)
				case RepType::ApproachingMidLeftPi:
					return "ApproachingMidLeftPi"; break;
				case RepType::ApproachingMidRightPi:
					return "ApproachingMidRightPi"; break;
		#endif
	#endif
	#if defined(MixedDec_EnableApproachingE)
				case RepType::ApproachingTopE://equal to IntValue.9..9 e
					return "ApproachingTopE"; break;
		#if defined(MixedDec_EnableApproachingAlternativeDiv)
				case RepType::ApproachingMidLeftE:
					return "ApproachingMidLeftE"; break;
				case RepType::ApproachingMidRightE:
					return "ApproachingMidRightE"; break;
		#endif
	#endif
	#if defined(MixedDec_EnableImaginaryInfinity)
				case RepType::ImaginaryInfinity:
					return "ImaginaryInfinity"; break;
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

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        virtual RepType GetRepType()
        {
#if !defined(AltNum_UseIntForDecimalHalf)
            switch(DecimalHalf.Flag)
            {
#endif
		#if defined(MixedDec_EnablePiRep)
            #if defined(AltNum_UseIntForDecimalHalf)
                //Add code here later
            #else
                case 1:
                    {
                #if defined(MixedDec_EnableApproachingPi)
                        if (DecimalHalf == ApproachingTopRep)
                    #if defined(MixedDec_EnableApproachingAlternativeDiv)
                            if(ExtraRep!=0)
                                return RepType::ApproachingMidLeftPi;
                            else
                    #endif
                                return RepType::ApproachingTopPi;
                #endif
                #if defined(MixedDec_EnableApproachingAlternativeDiv)
                        else if (DecimalHalf == ApproachingBottomRep)//ExtraRep!=0
                    #if defined(MixedDec_EnableApproachingAlternativeDiv)
                             return RepType::ApproachingMidRightPi;
                    #endif
                #endif
                #if defined(AltNum_EnablePowerOfRepresentation)
                    #if defined(AltNum_EnableNegativePowerRep)
                        if(ExtraRep!=0)
                    #else
                        if(ExtraRep.IsNegative())
                    #endif
                            return RepType::PiPower;
                #endif
                #if defined(AltNum_EnableFractionals)
                        if(ExtraRep!=0)
                    #if defined(MixedDec_EnableMixedFractional)
                            if(ExtraRep.IsNegative())
                                return RepType::MixedPi;
                            else
                    #endif
                                return RepType::PiNumByDiv;
                #endif
                        return RepType::PiNum;
                    }
                    break;
            #endif
        #endif
		#if defined(MixedDec_EnableERep)
            #if defined(AltNum_UseIntForDecimalHalf)
                //Add code here later
            #else
                case 2:
                    {
                #if defined(MixedDec_EnableApproachingE)
                        if (DecimalHalf == ApproachingTopRep)
                    #if defined(MixedDec_EnableApproachingAlternativeDiv)
                            if(ExtraRep!=0)
                                return RepType::ApproachingMidLeftE;
                            else
                    #endif
                                return RepType::ApproachingTopE;
                    #if defined(MixedDec_EnableApproachingAlternativeDiv)
                        else if (DecimalHalf == ApproachingBottomRep)//ExtraRep!=0
                             return RepType::ApproachingMidRightE;
                    #endif
                #endif
                #if defined(AltNum_EnablePowerOfRepresentation)
                    #if defined(AltNum_EnableNegativePowerRep)
                        if(ExtraRep!=0)
                    #else
                        if(ExtraRep.IsNegative())
                    #endif
                            return RepType::EPower;
                #endif
                #if defined(AltNum_EnableFractionals)
                        if(ExtraRep!=0)
                    #if defined(MixedDec_EnableMixedFractional)
                            if(ExtraRep.IsNegative())
                                return RepType::MixedE;
                            else
                    #endif
                                return RepType::ENumByDiv;
                #endif
                        return RepType::ENum;
                    }
                    
                    break;
            #endif
		#endif
        #if defined(MixedDec_EnableImaginaryNum)
            #if defined(AltNum_UseIntForDecimalHalf)
                //Add code here later
            #else
                case 3:
                    {
                #if defined(MixedDec_DeriveFromMediumDecV2)&&defined(MediumDecV2_EnableWithinMinMaxRange)
				    return RepType::WithinMinMaxRange;
                #else
                    #if defined(MixedDec_EnableImaginaryInfinity)
                        if(DecimalHalf == InfinityRep)
                            return RepType::ImaginaryInfinity;
                    #endif
                    #if defined(MixedDec_EnableApproachingI)
                        if (DecimalHalf == ApproachingBottomRep)
                        {
                        #if defined(MixedDec_EnableApproachingDivided)
                            if(ExtraRep!=0)
                                return RepType::ApproachingImaginaryMidLeft;
                            else
                        #endif
                            return RepType::ApproachingImaginaryBottom;
                        }
                        else if (DecimalHalf == ApproachingTopRep)
                        {
                        #if defined(MixedDec_EnableApproachingDivided)
                            if(ExtraRep!=0)
                                return RepType::ApproachingImaginaryMidRight;
                            else
                        #endif
                            return RepType::ApproachingImaginaryTop;
                        }
                    #endif
                    #if defined(AltNum_EnablePowerOfRepresentation)
                        #if defined(AltNum_EnableNegativePowerRep)
                        if(ExtraRep!=0)
                        #else
                        if(ExtraRep.IsNegative())
                        #endif
                            return RepType::EPower;
                    #endif
                    #if defined(AltNum_EnableFractionals)
                        if(ExtraRep!=0)
                        #if defined(MixedDec_EnableMixedFractional)
                            if(ExtraRep.IsNegative())
                                return RepType::MixedI;
                            else
                        #endif
                                return RepType::INumByDiv;
                    #endif
                        return RepType::INum;
                #endif
                    }
                    break;
            #endif
        #endif
#if !defined(AltNum_UseIntForDecimalHalf)
                default:
                    {
#endif
		#if defined(MixedDec_EnableInfinityRep)
                        if(DecimalHalf == InfinityRep)
                            return RepType::Infinity;
		#endif
		#if defined(MixedDec_EnableApproachingValues)
                        if (DecimalHalf == ApproachingBottomRep)
                        {
            #if defined(MixedDec_EnableApproachingDivided)
                            if(ExtraRep!=0)
                                return RepType::ApproachingMidLeft;
                            else
            #endif
                                return RepType::ApproachingBottom;
                        }
                        else if (DecimalHalf == ApproachingTopRep)
                        {
            #if defined(MixedDec_EnableApproachingDivided)
                            if(ExtraRep!=0)
                                return RepType::ApproachingMidRight;
                            else
            #endif
                            return RepType::ApproachingTop;
                        }
	    #endif
		#if defined(AltNum_EnableNaN)
			            if(DecimalHalf==NaNRep)
				            return RepType::NaN;
			            else if(DecimalHalf==UndefinedRep)
				            return RepType::Undefined;
		#endif
        #if defined(AltNum_EnableNil)
			            if(DecimalHalf==NilRep)
				            return RepType::Nil;
		#endif
		#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            #if defined(MixedDec_EnableWithinMinMaxRange)&&defined(MixedDec_DeriveFromAltDec)
                        if (ExtraRep == WithinMinMaxRangeRep)
                            return RepType::WithinMinMaxRange;
            #endif
			            if(DecimalHalf==UndefinedInRangeRep)
				            //If IntValue equals 0, than equals undefined value with range between negative infinity and positive infinity 
                            //Otherwise, indicates either negative or positive infinity (outside range of real number representation)
                            return RepType::UndefinedButInRange;
		#endif
                #if defined(AltNum_EnablePowerOfRepresentation)
                    #if defined(AltNum_EnableNegativePowerRep)
                        if(ExtraRep!=0)
                    #else
                        if(ExtraRep.IsNegative())
                    #endif
                            return RepType::ToPowerOf;
                #endif
                #if defined(AltNum_EnableFractionals)
                        if(ExtraRep!=0)
                    #if defined(MixedDec_EnableMixedFractional)
                            if(ExtraRep.IsNegative())
                                return RepType::MixedFrac;
                            else
                    #endif
                                return RepType::NumByDiv;
                #endif
                        return RepType::NormalType;
#if !defined(AltNum_UseIntForDecimalHalf)
                    }
                    break;
            }
#endif
			throw "Unknown or non-enabled representation type detected";//Should not reach this point when code is fully working
            return RepType::UnknownType;//Catch-All Value;
        }

    #pragma endregion RepType

public:

    #pragma region PiNum Setters
    #if defined(MixedDec_EnablePiRep)
        virtual void SetPiVal(const MediumDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
        #else
            DecimalHalf = Value.DecimalHalf;
        #endif
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetPiVal(const MediumDecV2& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = PiRep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetPiVal(const AltDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = PiRep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }
        
        template<MediumDecVariant VariantType=AltDecBase>
        void SetPiValV0(const VariantType& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = PiRep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetPiValFromInt(const int& Value)
        {
        #if defined(AltNum_EnableMirroredSection)
            if(Value<0)
                IntValue = MirroredInt(-Value,0);
            else
        #endif
                IntValue = Value;
        #if defined(AltNum_UseIntForDecimalHalf)
        #else
            DecimalHalf = PartialInt(0,1);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }
    #endif
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
    #if defined(AltNum_EnableERep)
        virtual void SetEVal(const MediumDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
        #else
            DecimalHalf = Value.DecimalHalf;
        #endif
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetEVal(const MediumDecV2& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = ERep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetEVal(const AltDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = ERep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }
        
        template<MediumDecVariant VariantType=AltDecBase>
        void SetEValV0(const VariantType& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = ERep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetEValFromInt(const int& Value)
        {
        #if defined(AltNum_EnableMirroredSection)
            if(Value<0)
                IntValue = MirroredInt(-Value,0);
            else
        #endif
                IntValue = Value;
        #if defined(AltNum_UseIntForDecimalHalf)
        #else
            DecimalHalf = PartialInt(0,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region INum Setters
    #if defined(AltNum_EnableIRep)
        virtual void SetIVal(const MediumDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = IRep;
        #else
            DecimalHalf = Value.DecimalHalf;
        #endif
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetIVal(const MediumDecV2& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = IRep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetIVal(const AltDec& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = IRep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }
        
        template<MediumDecVariant VariantType=AltDecBase>
        void SetIValV0(const VariantType& Value)
        {
            IntValue = Value.IntValue;
        #if defined(AltNum_UseIntForDecimalHalf)
            DecimalHalf = Value.DecimalHalf;
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = IRep;
            #endif
        #else
            DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = Value.ExtraRep;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }

        virtual void SetIValFromInt(const int& Value)
        {
        #if defined(AltNum_InableMirroredSection)
            if(Value<0)
                IntValue = MirroredInt(-Value,0);
            else
        #endif
                IntValue = Value;
        #if defined(AltNum_UseIntForDecimalHalf)
        #else
            DecimalHalf = PartialInt(0,2);
            #if defined(MixedDec_DeriveFromAltDec)
            ExtraRep = 0;
            #endif
        #endif
            SetTrailingDigitAsZero();
        }
    #endif
    #pragma endregion INum Setters

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
    protected:
	#if defined(AltNum_EnableNaN)
        static MixedDec NaNValue()
        {
            MixedDec NewSelf = MixedDec(0, NaNRep);
            return NewSelf;
        }
		
        static MixedDec UndefinedValue()
        {
            MixedDec NewSelf = MixedDec(0, UndefinedRep);
            return NewSelf;
        }
	#endif
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec PiNumValue()
        {
            return MixedDec(3, 141592654);
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MixedDec HundredMilPiNumVal()
        {
            return MixedDec(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MixedDec TenMilPiNumVal()
        {
            return MixedDec(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MixedDec OneMilPiNumVal()
        {
            return MixedDec(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static MixedDec TenPiNumVal()
        {
            return MixedDec(31, 415926536);
        }
        
        static MixedDec ENumValue()
        {
            return MixedDec(2, 718281828);
        }
        
        static MixedDec ZeroValue()
        {
            return MixedDec();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec OneValue()
        {
            MixedDec NewSelf = MixedDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec TwoValue()
        {
            MixedDec NewSelf = MixedDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec NegativeOneValue()
        {
            MixedDec NewSelf = MixedDec(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec Point5Value()
        {
            MixedDec NewSelf = MixedDec(0, 500000000);
            return NewSelf;
        }

        static MixedDec JustAboveZeroValue()
        {
            MixedDec NewSelf = MixedDec(0, 1);
            return NewSelf;
        }

        static MixedDec OneMillionthValue()
        {
            MixedDec NewSelf = MixedDec(0, 1000);
            return NewSelf;
        }

        static MixedDec FiveThousandthValue()
        {
            MixedDec NewSelf = MixedDec(0, 5000000);
            return NewSelf;
        }

        static MixedDec FiveMillionthValue()
        {
            MixedDec NewSelf = MixedDec(0, 5000);
            return NewSelf;
        }

        static MixedDec TenMillionthValue()
        {
            MixedDec NewSelf = MixedDec(0, 100);
            return NewSelf;
        }

        static MixedDec OneHundredMillionthValue()
        {
            MixedDec NewSelf = MixedDec(0, 10);
            return NewSelf;
        }

        static MixedDec FiveBillionthValue()
        {
            MixedDec NewSelf = MixedDec(0, 5);
            return NewSelf;
        }

        static MixedDec LN10Value()
        {
            return MixedDec(2, 302585093);
        }

        static MixedDec LN10MultValue()
        {
            return MixedDec(0, 434294482);
        }

        static MixedDec HalfLN10MultValue()
        {
            return MixedDec(0, 868588964);
        }
        
    #if defined(AltNum_EnableNil)
        static MixedDec NilValue()
        {
            return MixedDec(NilRep, NilRep);
        }
    #endif

        static MixedDec MinimumValue()
        {
            return MixedDec(2147483647, 999999999);
        }

        static MixedDec MaximumValue()
        {
            return MixedDec(2147483647, 999999999);
        }
public:
        static MixedDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec ENum;
        
#if defined(AltNum_EnableInfinityRep)
        static MixedDec Infinity;
        static MixedDec NegativeInfinity;
        static MixedDec ApproachingZero;
#endif
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec OneGMillionth;

        //0e-7
        static MixedDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MixedDec FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MixedDec</returns>
        static MixedDec NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MixedDec Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MixedDec Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MixedDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MixedDec LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MixedDec HalfLN10Mult;

    #if defined(AltNum_EnableNil)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static MixedDec Nil;
    #endif
public:
    #pragma endregion ValueDefines

    #pragma region String Commands
        /// <summary>
        /// Initializes a new instance of the <see cref="MixedDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MixedDec(const char* strVal)
        {
            ReadFromCharString(strVal);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MixedDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MixedDec(std::string Value)
        {
            ReadFromString(Value);
        }
    #pragma endregion String Commands

public:
    #pragma region ConvertFromOtherTypes
		
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetFloatVal(const float& Value)
        {
		//#if !defined(AltNum_UseLegacyFloatingConversion)
			//To-Do:Add more exact extraction
		//#else
			#if defined(MixedDec_DeriveFromAltDec)
			AltDec::SetBoolVal(Value);
			#else
			MediumDec::SetFloatVal(Value);
			#endif
			SetTrailingDigitsAsZero();
		//#endif	
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDoubleVal(const double& Value)
        {
		//#if !defined(AltNum_UseLegacyFloatingConversion)
			//To-Do:Add more exact extraction
		//#else
			#if defined(MixedDec_DeriveFromAltDec)
			AltDec::SetBoolVal(Value);
			#else
			MediumDec::SetDoubleVal(Value);
			#endif
			SetTrailingDigitsAsZero();
		//#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDecimalVal(const ldouble& Value)
        {
		//#if !defined(AltNum_UseLegacyFloatingConversion)
			//To-Do:Add more exact extraction
		//#else
			#if defined(MixedDec_DeriveFromAltDec)
			AltDec::SetBoolVal(Value);
			#else
			MediumDec::SetDecimalVal(Value);
			#endif
			SetTrailingDigitsAsZero();
		//#endif
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetBoolVal(const bool& Value)
        {
			#if defined(MixedDec_DeriveFromAltDec)
			AltDec::SetBoolVal(Value);
			#else
			MediumDec::SetBoolVal(Value);
			#endif
			SetTrailingDigitsAsZero();
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(const int& Value)
        {
			#if defined(MixedDec_DeriveFromAltDec)
			AltDec::SetBoolVal(Value);
			#else
			MediumDec::SetIntVal(Value);
			#endif
			SetTrailingDigitsAsZero();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MixedDec(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MixedDec(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MixedDec(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MixedDec(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

#if defined(AltNum_EnableMediumDecBasedSetValues)
        MixedDec(const MediumDec& Value)
        {
            this->SetVal(Value);
        }
#endif
    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

        /// <summary>
        /// MediumDec to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }

        /// <summary>
        /// MediumDec to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double(){ return toDouble(); }

        /// <summary>
        /// MediumDec to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble() { return toDecimal(); }
		
        /// <summary>
        /// MediumDec to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return toInt(); }

        explicit operator bool() { return toBool(); }
    #pragma endregion ConvertToOtherTypes

    #pragma region Pi Conversion
	
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators
protected:
        //Updating BasicComparison to include the trailing digit added from this class

		template<MediumDecVariant VariantType=MediumDecBase>
		std::strong_ordering BasicComparisonV1(const VariantType& that) const
		{
			MediumDecBase::BasicComparisonV1(that);
		//--Trailing Digit comparison
		//Flip digits when negative to properly compare
		#if defined(MixedDec_EnableRestrictedFloat)
			RestrictedFloat lTrailingVal = IsNegative()?RestrictedFloat::One-TrailingDigit;
			RestrictedFloat rTrailingVal = that.IsNegative()?RestrictedFloat::One-that.TrailingDigit;
		#elif defined(MixedDec_EnableAltFloat)
			AltFloat lTrailingVal = IsNegative()?AltFloat::One-TrailingDigit;
			AltFloat rTrailingVal = that.IsNegative()?AltFloat::One-that.TrailingDigit;
		#else
			float lTrailingVal = IsNegative()?1.0f-TrailingDigit;
			float rTrailingVal = that.IsNegative()?1.0f-that.TrailingDigit;
		#endif
			if (auto TrailingCmp = lTrailingVal <=> rTrailingVal; TrailingCmp != 0)
				return TrailingCmp;
		}
		
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
		template<MediumDecVariant VariantType=MediumDecBase>
		std::strong_ordering BasicComparisonWithoutSignCheck(const VariantType& that) const
		{
			MediumDecBase::BasicComparisonWithoutSignCheck(that);
		//--Trailing Digit comparison
		//Flip digits when negative to properly compare
		#if defined(MixedDec_EnableRestrictedFloat)
			RestrictedFloat lTrailingVal = IsNegative()?RestrictedFloat::One-TrailingDigit;
			RestrictedFloat rTrailingVal = that.IsNegative()?RestrictedFloat::One-that.TrailingDigit;
		#elif defined(MixedDec_EnableAltFloat)
			AltFloat lTrailingVal = IsNegative()?AltFloat::One-TrailingDigit;
			AltFloat rTrailingVal = that.IsNegative()?AltFloat::One-that.TrailingDigit;
		#else
			float lTrailingVal = IsNegative()?1.0f-TrailingDigit;
			float rTrailingVal = that.IsNegative()?1.0f-that.TrailingDigit;
		#endif
			if (auto TrailingCmp = lTrailingVal <=> rTrailingVal; TrailingCmp != 0)
				return TrailingCmp;
		}
		
		//Compare only as if in NormalType representation mode
        constexpr auto BasicComparison = MediumDecBase::BasicComparisonV1<MixedDec>;

#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
        constexpr auto BasicComparisonV2 = MediumDecBase::BasicComparisonWithoutSignCheck<MixedDec>;
#endif

    #if defined(MixedDec_DefineInfinityAsSignedReps)
        constexpr auto LSideInfinityComparison = MediumDecV2Base::LSideInfinityComparison<MixedDec>;
	#endif

		//Excluding templated override for this class because MixedDec has no partial version of class unlike previous parent classes

		std::strong_ordering CompareWith(const MixedDec& that) const
		{
	#if defined(MixedDec_EnableWithinMinMaxRange)
			if(ExtraRep==WithinMinMaxRangeRep) {
				if(ExtraRep==WithinMinMaxRangeRep) {
					//To-do compare within min-max range code here
				}
				else {
					//To-do compare within min-max range code here
				}
			}
			else if(ExtraRep==WithinMinMaxRangeRep) {
				//To-do compare within min-max range code here
			}
	#endif
	#if defined(MixedDec_EnableMirroredSection)
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntValue.IsPositive <=> that.IntValue.IsPositive; SignCmp != 0)
				return SignCmp;
	#endif
	
			RepType LRep = GetRepType();
			RepType RRep = that.GetRepType();
    #if defined(MixedDec_EnableNaN)||defined(MixedDec_EnableNilRep)||defined(MixedDec_EnableUndefinedButInRange)
			if(LRep^UndefinedBit||RRep^UndefinedBit)
				throw "Can't compare undefined/nil representations";
    #endif
	#if defined(MixedDec_UseIntForDecimalHalf)
		//To-Do add code here
	#else
		#if defined(MixedDec_EnableImaginaryNum)
            if (DecimalHalf.Flags == 3)
            {
                if(that.DecimalHalf.Flags!=3)
                    throw "Can't compare imaginary number with real number";
				else if(RRep==RepType:ImaginaryInfinity)
                {
					if(that.IntValue==1)
						return 0<=>1;//Positive Infinity is greater than real number representations
					else
						return 1<=>0;
                }
                else
                {
					VariantType LValue = this;
					VariantType RValue = that;
			#if defined(MixedDec_EnablePowerOfRepresentation)
				#if defined(MixedDec_EnableNegativePowerRep)
					int LComp = (int)LValue.ExtraRep;
					int RComp = (int)RValue.ExtraRep;
					if(LComp!=0)//Left side is to power of ExtraRep.Value
					{
						if(RComp!=0)//Right side is to power of ExtraRep.Value
						{
							//Add Code here
						}
						else
						{
							//Add code here
						}
					}
					else if(RComp!=0)//Right side is to power of ExtraRep.Value
					{
						//Add code here
					}
				#else
					if(ExtraRep.IsPositive==0)//Left side is to power of ExtraRep.Value
					{
						if(that.ExtraRep.IsPositive)//Right side is to power of ExtraRep.Value
						{
							//Add Code here
						}
						else
						{
							//Add code here
						}
					}
					else if(that.ExtraRep.IsPositive)//Right side is to power of ExtraRep.Value
					{
						//Add code here
					}
				#endif
			#elif defined(MixedDec_EnableMixedFractional)
					if(ExtraRep.IsPositive==0)//Left side is a mixed Fraction
					{
						if(that.ExtraRep.IsPositive)//Right side is a mixed Fraction
						{
							//Add Code here
						}
						else
						{
							//Add code here
						}
					}
					else if(that.ExtraRep.IsPositive)//Right side is a mixed Fraction
					{
						//Add code here
					}
			#endif
					else if(ExtraRep.Value!=0)//Left side is a divisor
					{
						if(that.ExtraRep.Value!=0)//Right side is a divisor
						{
							//Add code here
						}
						else
						{
							//Add code here
						}
					}
					else if(that.ExtraRep.Value!=0)//Right side is a divisor
					{
						//Add code here
					}
			#if defined(MixedDec_EnableMirroredSection)
			    	return BasicComparisonV2(rSide);
			#else
					return BasicComparison(rSide);
			#endif
                }
            }
			else if(that.Flags==3)
				throw "Can't compare imaginary number with real number";
		#endif
	#endif
			switch(LRep)
			{
	#if defined(MixedDec_EnableInfinityRep)
                case RepType:Infinity:
                    LSideInfinityComparison(that, RRep);
                    break;
	#endif
	#if defined(MixedDec_EnableApproachingValues)
	
	#endif
	#if defined(MixedDec_EnableFractionals)
				case RepType:NumByDiv:
		#if defined(MixedDec_EnableDecimaledPiFractionals)
				case RepType:PiNumByDiv:
		#endif
		#if defined(MixedDec_EnableDecimaledEFractionals)
				case RepType:ENumByDiv:
		#endif
		#if defined(MixedDec_EnablePiFractional)
				case RepType:PiFractional:
		#endif
		#if defined(MixedDec_EnableEFractional)
				case RepType:EFractional:
		#endif
					{
						if(RRep==RepType:Infinity)
						{
							if(that.IntValue==1)
								return 0<=>1;//Positive Infinity is greater than real number representations
							else
								return 1<=>0;
						}
						else
						{
							MediumDecV2Base lSide = *this;
							MediumDecV2Base rSide = that;
							lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
		#if defined(MixedDec_EnableMirroredSection)
							return lSide.BasicComparisonV2(rSide);
		#else
							return rSide.BasicComparison(rSide);
		#endif
						}
					}
					break;
	#endif
	#if defined(MixedDec_EnablePowerOfRepresentation)
				case RepType:ToPowerOf:
				case RepType:PiPower:
				case RepType:EPower:
					{
						if(RRep==RepType:Infinity)
						{
							if(that.IntValue==1)
								return 0<=>1;//Positive Infinity is greater than real number representations
							else
								return 1<=>0;
						}
						else
						{
							MediumDecV2Base lSide = *this;
							MediumDecV2Base rSide = that;
							lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
		#if defined(MixedDec_EnableMirroredSection)
							return lSide.BasicComparisonV2(rSide);
		#else
							return rSide.BasicComparison(rSide);
		#endif
						}
					}
					break;
	#endif
	#if defined(MixedDec_EnableMixedFractional)
				case RepType:MixedFrac:
		#if defined(MixedDec_EnableMixedPiFractional)
				case RepType:MixedPi:
		#endif
		#if defined(MixedDec_EnableMixedEFractional)
				case RepType:MixedE:
		#endif
					{
						if(RRep==RepType:Infinity)
						{
							if(that.IntValue==1)
								return 0<=>1;//Positive Infinity is greater than real number representations
							else
								return 1<=>0;
						}
						else
						{
							MediumDecV2Base lSide = *this;
							MediumDecV2Base rSide = that;
							lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
		#if defined(MixedDec_EnableMirroredSection)
							return lSide.BasicComparisonV2(rSide);
		#else
							return rSide.BasicComparison(rSide);
		#endif
						}
					}
					break;
	#endif
				default:
				{
					if(LRep==RRep)
	#if defined(MixedDec_EnableMirroredSection)
						return BasicComparisonV2(that);
	#else
						return BasicComparison(that);
	#endif
					else if(RRep==RepType:Infinity)
                    {
                        if(that.IntValue==1)
							return 0<=>1;//Positive Infinity is greater than real number representations
						else
							return 1<=>0;
                    }
                    else
					{
						MixedDec lSide = *this;
						MixedDec rSide = that;
						lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
	#if defined(MixedDec_EnableMirroredSection)
						return lSide.BasicComparisonV2(rSide);
	#else
						return rSide.BasicComparison(rSide);
	#endif
					}
				}
			}
		}

		std::strong_ordering CompareWithInt(const int& that) const
		{
			int lVal; int rVal;
			//Pi and E only enabled if imbedded flags are enabled
	#if !defined(MixedDec_UseIntForDecimalHalf)
			if(DecimalHalf.Flags==0)
	#else
			if(ExtraRep==0)
	#endif
			{
				return BasicIntComparison(that);
			}
	#if defined(MixedDec_EnableImaginaryNum
		#if !defined(MixedDec_UseIntForDecimalHalf)
			else if(DecimalHalf.Flags==3)
		#elif defined(MixedDec_EnableDecimaledIFractionals)//Check if within I Fractional Representation or INum representation to check if valid
			else if((ExtraRep<0&&ExtraRep>FractionalDivisorOverflow)||ExtraRep==IRep)
		#else
			else if(ExtraRep==IRep)
		#endif
				throw "Can't compare imaginary number with real number";
	#endif
			else
			{
				MixedDec lSide = *this;
				lSide.ConvertToNormTypeV2();
				return lSide.BasicIntComparison(that);
			}
	#endif
		}

		//Alias to prevent creating function more than once with template arguments
        constexpr auto CompareWith = MediumDecBase::CompareWithV1<MixedDec>;

		//Alias to prevent creating function more than once with template arguments
        constexpr auto CompareWithInt = MediumDecBase::CompareWithIntV1<MixedDec>;

public:
		std::strong_ordering operator<=>(const MixedDec& that) const
		{
			return CompareWith(that);
		}

	/*  
		//Add comparisons to previous parent classes with more limited ranges based on supported values

		std::strong_ordering operator<=>(const MediumDec& that) const
		{

		}
		
		std::strong_ordering operator<=>(const MediumDecV2& that) const
		{

		}
		
		std::strong_ordering operator<=>(const AltDec& that) const
		{

		}

    */

		std::strong_ordering operator<=>(const int& that) const
		{
		#if	defined(MixedDec_EnableAlternativeRepresentations)
			MixedDec lValue = this;
			//
			
			int lVal = lValue.IntValue==NegativeZero?0:lValue.IntValue;
		#else	
			int lVal = IntValue==NegativeZero?0:IntValue;
		#endif
			int rVal = that;
			if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
		#if	defined(MixedDec_EnableAlternativeRepresentations)
			lVal = lValue.DecimalHalf>0?1:0;
		#else
			lVal = DecimalHalf>0?1:0;
		#endif
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		//--Trailing Digit comparison
		//Flip digits when negative to properly compare
		#if defined(MixedDec_EnableRestrictedFloat)
			RestrictedFloat lTrailingVal = IsNegative()?RestrictedFloat::One-TrailingDigit;
			RestrictedFloat rTrailingVal = that.IsNegative()?RestrictedFloat::One-that.TrailingDigit;
		#elif defined(MixedDec_EnableAltFloat)
			AltFloat lTrailingVal = IsNegative()?AltFloat::One-TrailingDigit;
			AltFloat rTrailingVal = that.IsNegative()?AltFloat::One-that.TrailingDigit;
		#else
			float lTrailingVal = IsNegative()?1.0f-TrailingDigit;
			float rTrailingVal = that.IsNegative()?1.0f-that.TrailingDigit;
		#endif
			if (auto TrailingCmp = lTrailingVal <=> rTrailingVal; TrailingCmp != 0)
				return TrailingCmp;
		}

		bool operator==(const int& that) const
		{
			if (IntValue!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			if(TrailingDigits.IsNotZero())
				return false;
			return true;
		}

		bool operator==(const MediumDec& that) const
		{
		    if(TrailingDigits.IsNotZero())
			    return false;
		#if defined(AltNum_EnableUndefinedButInRange)
			if(DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
			else if(that.DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
            #if defined(MixedDec_EnableWithinMinMaxRange)
                //ToDo:Add comparison code for comparing unknown number within range
            #endif
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)
			MixedDec LValue = this;
			LValue.ConvertToNormTypeV2();
			MediumDec RValue = that;
			RValue.ConvertToNormTypeV2();
		#else
			MixedDec LValue = this;
			MediumDec RValue = that;
			if(DecimalHalf.Flags!=0)
				return false;
		#endif
			if (LValue.IntValue!=RValue.IntValue)
				return false;
			if (LValue.DecimalHalf!=RValue.IntValue)
				return false;
		}

		bool operator==(const MediumDecV2& that) const
		{
		    if(TrailingDigits.IsNotZero())
			    return false;
		#if defined(AltNum_EnableUndefinedButInRange)
			if(DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
			else if(that.DecimalHalf==UndefinedInRangeMinMaxRep)
			    return false;
            #if defined(AltNum_EnableWithinMinMaxRange)
                //ToDo:Add comparison code for comparing unknown number within range
            #endif
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)
			MixedDec LValue = this;
			LValue.ConvertToNormTypeV2();
			MediumDecV2 RValue = that;
			RValue.ConvertToNormTypeV2();
		#else
			MixedDec LValue = this;
			MediumDecV2 RValue = that;
			if(DecimalHalf.Flags==RValue.DecimalHalf.Flags)
				LValue.ConvertDownToMediumDecV2Equiv();
			else if((DecimalHalf.Flags==3 && RValue.DecimalHalf.Flags!=3)||(RValue.DecimalHalf.Flags==3 && LValue.DecimalHalf.Flags!=3))
				throw "Can't compare imaginary number with real number";
			else if(DecimalHalf.Flags!=0)
				return false;
		#endif
			if (LValue.IntValue!=RValue.IntValue)
				return false;
			if (LValue.DecimalHalf!=RValue.IntValue)
				return false;
		}

		bool operator==(const MixedDec& that) const
		{
			if (IntValue!=that.IntValue)
				return false;
			if (DecimalHalf!=that.IntValue)
				return false;
			if (TrailingDigits!=that.TrailingDigits)
				return false;
		}
    #pragma endregion Comparison Operators

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
#elif defined(MixedDec_EnableRestrictedFloat)
        void RestrictedFloatDivByOp(const RestrictedFloat& rValue)
		{
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
#elif defined(MixedDec_EnableRestrictedFloat)
        void RestrictedFloatMultByOp(const RestrictedFloat& rValue)
		{
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
#elif defined(MixedDec_EnableRestrictedFloat)
        void RestrictedFloatAddByOp(const RestrictedFloat& rValue)
		{
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
#elif defined(MixedDec_EnableRestrictedFloat)
        void RestrictedFloatSubtractByOp(const RestrictedFloat& rValue)
		{
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
#elif defined(MixedDec_EnableRestrictedFloat)
        void RestrictedFloatModByOp(const RestrictedFloat& rValue)
		{
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