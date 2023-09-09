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
#include <bitset>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)
#if defined(AltNum_EnableBoostFractionalReduction) || defined(AltNum_UseOldDivisionCode)
#include <boost/multiprecision/cpp_int.hpp>
#endif

#ifdef AltNum_EnableMediumDecBasedSetValues
    #include "MediumDec.hpp"
#endif
#ifdef AltNum_EnableAltDecBasedSetValues
    #include "AltDec.hpp"
#endif

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "AltNumModChecker.hpp"
//Preprocessor options
/*
FlaggedNum_EnableFractionals =
      Enables fractional representations in attempt to preserve more accuracy during operations
	  (functions same as having all variants of AltNum_EnableDecimaledAlternativeFractionals active as well as AltNum_EnableFractionals from AltDec)
      If FlaggedNum_PreventUsageOfExtraRep is active, then functions the same as AltNum_EnableFractionals

//--Infinity based preprocessors--
AltNum_EnableInfinityRep = Enable support of positive/negative infinity representations and approaching value representations
      When DecimalHalf is -2147483648, it represents negative infinity(if IntValue is -1) or positive infinity(if IntValue is 1)
AltNum_EnableApproachingValues
      When DecimalHalf is -2147483647 and ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0..1)
	  When DecimalHalf is -2147483647 and ExtraRep==1, it represents Approaching IntValue+1 from left towards right (IntValue.9..9)
AltNum_EnableApproachingPi = AltNum_EnableApproachingMidDec for Pi based variables(Partially Implemented)
AltNum_EnableApproachingE = AltNum_EnableApproachingMidDec for e based variables(Partially Implemented)
AltNum_EnableApproachingI = AltNum_EnableApproachingMidDec for imaginary based variables(Partially Implemented)
AltNum_EnableApproachingDivided =
	Enables Approaching IntValue.49..9 and IntValue.50..1 and other Approaching values (49..9 = ExtraRep value of 2; 50..1 = ExtraRep value of -2)
	When ExtraRep is Positive, represents (when IntValue is positive) IntValue + (1/ExtraRep + ApproachingBottomValue)(approaching left towards right)
	When ExtraRep is Positive, represents (when IntValue is positive) IntValue + (1/ExtraRep + ApproachingTopValue)(approaching right towards left)
	(Assumes AltNum_EnableInfinityRep is enabled)
	(Partially Implemented)

AltNum_DisplayApproachingAsReal =
      Display approaching value as real number with 20 digits in decimal section

AltNum_EnableNaN =
      Enable NaN based representations and operations(Not Fully Implemented)

AltNum_EnableHigherPrecisionPiConversion =
      (Not Implemented)
	  
AltNum_EnableOverflowPreventionCode =
      Use to enable code to check for overflows on addition/subtraction/multiplication operations (return an exception if overflow)
      (Not Implemented Yet)
	  
AltNum_DisableInfinityRepTypeReturn = Disables infinity variables while allowing approaching variables to function(Not fully Implemented)

AltNum_DisablePiRep =
      Force toggles AltNum_EnablePiRep to off
      AltNum_EnablePiRep given greator priority if both both AltNum_DisablePiRep and AltNum_EnablePiRep
      are set by preprocessor settings of project

AltNum_EnablePiRep =
      If AltNum_UseMediumDecBasedRepresentations enabled, then
        Pi*(+- 2147483647.999999999) Representation enabled
        (When DecimalHalf is between -1 and -1000000000 (when DecimalHalf is -1000000000 is Equal to IntValue*Pi))
      Otherwise represents Pi within format of
         If DecimalHalf is positive and ExtraRep is -2147483647,
         then FlaggedDec represents +- 2147483647.999999999 * Pi (Not Fully Implemented)
         If DecimalHalf is positive and and ExtraRep is between AlternativeFractionalLowerBound and 0,
         then FlaggedDec represents (+- 2147483647.999999999 * Pi)/(ExtraRep*-1)
	  (Not Fully Implemented--Enabled by default if AltNum_DisablePiRep not set)

FlaggedNum_EnableMixedFractional =
      If DecimalHalf is negative and ExtraRep is Positive,
      then FlaggedDec represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
      (Not Fully Implemented)

AltNum_EnableERep =
      If AltNum_UseMediumDecBasedRepresentations enabled, then
    e*(+- 2147483647.999999999) Representation enabled
    (When DecimalHalf is between -1000000001 and -2000000000 (when DecimalHalf is -2000000000 is Equal to IntValue*e))
      Otherwise represents e within format of
    If DecimalHalf is positive and ExtraRep is -2147483646, then
       represents +- 2147483647.999999999 * e
    If DecimalHalf is positive and ExtraRep is between AlternativeFractionalLowerBound and 0, then
        represents (+- 2147483647.999999999 * e)/(ExtraRep*-1)
      (Not Fully Implemented)

AltNum_EnableImaginaryNum =
      If DecimalHalf is positive and ExtraRep is -2147483647, then
      represents +- 2147483647.999999999i
      If DecimalHalf is positive and ExtraRep is between AlternativeFractionalLowerBound and 0, then
      represents (+- 2147483647.999999999i)/(ExtraRep*-1)
      (Not Fully Implemented--Giving lesser priority to finish unless I need to use)

AltNum_EnablePrivateRepType =
      Sets GetRepType code to be private instead of public

FlaggedNum_TogglePreferedSettings =


AltNum_EnableUndefinedButInRange = Enable representation of unknown number between -Value and +Value for Cos operation

----===============================================================================================================

AltNum_OutputTruncatedTrailingDigits =
    Output to console trailing digits that are truncated when multiplication or division results in numbers getting too small(Not Implemented yet)
	(Impliment this before work to making working version with trailing digits such as for MixedDec (fixedpoint combined with floating point implimentations of decimal-like format classes)

AltNum_UseOldDivisionCode
AltNum_AvoidUsingLargeInt = Removes AltNum_UseOldDivisionCode toggle and forces alternative code that doesn't need int128 from boost
AltNum_UseOldRemOpCode

AltNum_EnableBoostFractionalReduction
AltNum_EnableImaginaryInfinity = Enables imaginary infinity option

FlaggedNum_UseBitset = Use bitset instead of unsigned byte value(pseudo-bitset)

ExtraRep value is used only if:
FlaggedNum_EnableFractionals, FlaggedNum_EnablePowers, or FlaggedNum_EnableMixedFractional
are enabled
Automatically toggles FlaggedNum_ExtraRepIsActive

FlaggedNum_UseFlagForInfinitesimalRep = Use flag values instead of DecimalStatus for 
                                        Approaching value and infinity representations

*/
//If Pi rep is neither disabled or enabled, default to enabling Pi representation
#include "AlNumBase.hpp"

#if defined(FlaggedNum_EnableFractionals) || defined(FlaggedNum_EnablePowers) || defined(FlaggedNum_EnableMixedFractional) || defined(AltNum_EnableApproachingDivided) || defined(AltNum_EnableApproachingPi) || defined(AltNum_EnableApproachingE) || defined(AltNum_EnableApproachingI) || defined(AltNum_EnableUndefinedButInRange)
	#if !defined(FlaggedNum_ExtraRepIsActive)//Prevent redefinition warning
		#define FlaggedNum_ExtraRepIsActive
	#endif
#endif

#if defined(FlaggedNum_EnableFractionals) && defined(AltNum_EnablePiRep) && !defined(AltNum_EnableDecimaledPiFractionals)
    #define FlaggedNum_EnableDecimaledPiFractionals
#endif

#if defined(FlaggedNum_EnableFractionals) && defined(AltNum_EnableERep) && !defined(AltNum_EnableDecimaledEFractionals)
    #define FlaggedNum_EnableDecimaledEFractionals
#endif

#if defined(FlaggedNum_EnableFractionals) && defined(AltNum_EnableImaginaryNum) && !defined(AltNum_EnableDecimaledIFractionals)
    #define FlaggedNum_EnableDecimaledIFractionals
#endif

namespace BlazesRusCode
{
    class FlaggedDec;

/*---Accuracy Tests(with MediumDec based settings):
 * 100% accuracy for all integer value multiplication operations.
 * 100% accuracy for addition/subtraction operations
 * Partial but still high accuracy for non-integer representation variations of multiplication and division because of truncation
   (values get lost if get too small) (100% accuracy except for some truncated digits lost)
 * Other operations like Ln and Sqrt contained with decent level of accuracy
   (still loses a little accuracy because of truncation etc)
 * Operations and functions will mess up if IntValue overflows/underflows
   or reaches exactly -2147483648 which is used to represent negative zero when it has decimal values
*/

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// plus support for Pi, e, i, fractionals, and infinity depending on options enabled
	///  (If neither FlaggedNum_EnableFractionals, FlaggedNum_EnablePowers, or FlaggedNum_EnableMixedFractional are used
	///  uses 9 bytes of variable storage in class per instance)
    /// (13 bytes worth of Variable Storage inside class for each instance if ExtraRep variable used)
	/// </summary>
#if defined(AltNum_DisableCommonVariablesInBase)
    class DLL_API FlaggedDec
#else
    class DLL_API FlaggedDec: public virtual AltNumBase
#endif
    {
    public:
#if defined(AltNum_DisableCommonVariablesInBase)
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;
		
    /*protected://
        /// <summary>
        /// The decimal overflow value * -1
        /// </summary>
        static signed _int64 const NegDecimalOverflowX = -1000000000;*/
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

        /// <summary>
        /// Value when IntValue is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
        static signed int const NegativeRep = -2147483648;

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        signed int IntValue;

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        signed int DecimalHalf;
#endif
		
		//BitFlag 01 = PiRep, 02 = ERep, 03 = IRep, 04 = Fractional Rep, 05 = Power of, 06 = Mixed Fractional
	#if !defined(FlaggedNum_UseBitset)
		unsigned char FlagsActive;
	#else
		std::bitset<6> FlagsActive;
	#endif
		
	#if defined(FlaggedNum_ExtraRepIsActive)
        /// <summary>
		/// (Used exclusively for alternative representations if FlaggedNum_EnableFractionals, FlaggedNum_EnablePowers, or FlaggedNum_EnableMixedFractional enabled)
        /// </summary>
        signed int ExtraRep;
	#endif

#if !defined(AltNum_StoreCommonFunctionsInBase)
        bool IsNegative()
        {
    #if !defined(FlaggedDec_UseMirroredInt)
            return IntValue<0;
    #else
            return IntValue.IsNegative();
    #endif
        }

        //Detect if at exactly zero
        bool IsZero()
        {
            return DecimalHalf==0&&IntValue==0;
        } const

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt()
        {
            return IntValue==0||IntValue==NegativeRep;
        }

        bool IsNotAtZeroInt()
        {
            return IntValue != 0 && IntValue != NegativeRep;
        }

        signed int GetIntHalf() const
        {
    #if defined(FlaggedDec_UseMirroredInt)
            return IntValue.GetValue();
    #else
            if(IntValue == NegativeRep)
                return 0;
            else
                return IntValue;
    #endif
        }

        //Return IntValue part as Absolute value
        signed int IntHalfAsAbs()
        {
    #if defined(FlaggedDec_UseMirroredInt)
            return IntValue.GetAbsValue();
            #else
            if (IsAtZeroInt())
                return 0;
            else if (IntValue < 0)
                return -IntValue;
            else
                return IntValue;
    #endif
        }

        std::string IntHalfAsString()
        {
    #if defined(FlaggedDec_UseMirroredInt)
            return (std::string) IntValue;
    #else
            if (IntValue == NegativeRep)
                return "-0";
            return VariableConversionFunctions::IntToStringConversion(IntValue);
    #endif
        }
#endif

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">Extra values for certain flagged representations</param>
	#if defined(FlaggedNum_ExtraRepIsActive)
        #if !defined(FlaggedNum_UseBitset)
        FlaggedDec(signed int intVal = 0, signed int decVal = 0, signed int extraVal = 0, unsigned char flagsActive = 0)
        #else
        FlaggedDec(signed int intVal = 0, signed int decVal = 0, signed int extraVal = 0, std::bitset<6> flagsActive = {})
        #endif
    #else
        #if !defined(FlaggedNum_UseBitset)
        FlaggedDec(signed int intVal = 0, signed int decVal = 0, unsigned char flagsActive=0)
        #else
        FlaggedDec(signed int intVal = 0, signed int decVal = 0, std::bitset<6> flagsActive = {})
        #endif
    #endif
        {
            IntValue = intVal;
            DecimalHalf = decVal;
			FlagsActive = flagsActive;
    #if defined(FlaggedNum_ExtraRepIsActive)
            ExtraRep = extraVal;
    #endif
        }

        FlaggedDec(const FlaggedDec&) = default;

        FlaggedDec& operator=(const int& rhs)
        {
            IntValue = rhs; DecimalHalf = 0;
        #if !defined(FlaggedNum_UseBitset)
            FlagsActive = 0;
        #else
            FlagsActive.reset();
        #endif
        #if defined(FlaggedNum_ExtraRepIsActive)
            ExtraRep = 0;
        #endif
            return *this;
        } const

        FlaggedDec& operator=(const FlaggedDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            FlagsActive = rhs.FlagsActive;
        #if defined(FlaggedNum_ExtraRepIsActive)
            ExtraRep = rhs.ExtraRep;
        #endif
            return *this;
        } const

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(const FlaggedDec& rhs)
        {
            IntValue = rhs.IntValue;
            DecimalHalf = rhs.DecimalHalf; 
            FlagsActive = rhs.FlagsActive;
        #if defined(FlaggedNum_ExtraRepIsActive)
            ExtraRep = rhs.ExtraRep;
        #endif
        } const

#if !defined(AltNum_StoreCommonFunctionsInBase)
        void SetAsZero()
        {
            IntValue = 0; DecimalHalf = 0;
        #if !defined(FlaggedNum_UseBitset)
            FlagsActive = 0;
        #else
            FlagsActive.reset();
        #endif
        #if defined(FlaggedNum_ExtraRepIsActive)
            ExtraRep = 0;
        #endif
        }

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
            if (IntValue == NegativeRep)
            {
                IntValue = 0;
            }
            else if (IntValue == 0)
            {
                IntValue = NegativeRep;
            }
            else
            {
                IntValue *= -1;
            }
        }

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        {
            IntValue = 2147483647; DecimalHalf = 999999999;
        #if !defined(FlaggedNum_UseBitset)
            FlagsActive = 0;
        #else
            FlagsActive.reset();
        #endif
        #if defined(FlaggedNum_ExtraRepIsActive)
            ExtraRep = 0;
        #endif
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
            IntValue = -2147483647; DecimalHalf = 999999999;
        #if !defined(FlaggedNum_UseBitset)
            FlagsActive = 0;
        #else
            FlagsActive.reset();
        #endif
        #if defined(FlaggedNum_ExtraRepIsActive)
            ExtraRep = 0;
        #endif
        }

#endif

    public:
        #pragma region Const Representation values
#if !defined(AltNum_StoreConstVariablesInBase)
    #if defined(AltNum_EnableInfinityRep)
        //Is Infinity Representation when DecimalHalf==-2147483648 (IntValue==1 for positive infinity;IntValue==-1 for negative Infinity)
        //(other values only used if AltNum_EnableInfinityPowers is enabled)
        //If AltNum_EnableImaginaryInfinity is enabled and ExtraRep = IRep, then represents either negative or positive imaginary infinity
		#if !defined(AltNum_UsePositiveInfinityRep)
        static const signed int InfinityRep = -2147483648;
		#else
        static const signed int InfinityRep = 2147483647;
		#endif
    #endif
    #if defined(AltNum_EnableApproachingValues)
        //Is Approaching Bottom when DecimalHalf==-2147483647:
        //If ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0..1)
        //If ExtraRep above 1 and 2147483645 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
        //If ExtraRep=PiRep, then it represents Approaching IntValue from right towards left (IntValue.0..1)Pi
        static const signed int ApproachingBottomRep = -2147483647;
        //Is Approaching Top i when DecimalHalf==-2147483646:
        //If ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)
        //If ExtraRep above 1 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
        //If ExtraRep=PiRep, then it represents Approaching IntValue+1 from left towards right (IntValue.9__9)Pi
        static const signed int ApproachingTopRep = -2147483646;
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)
        //Such as result of Cos of infinity
        //https://www.cuemath.com/trigonometry/domain-and-range-of-trigonometric-functions/
        static const signed int UndefinedInRangeRep = -2147483642;
        
        #if defined(AltNum_EnableWithinMinMaxRange)
        //Undefined but in ranged of IntValue to DecimalHalf
        static const signed int WithinMinMaxRangeRep = -2147483642;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
        //Is NaN when DecimalHalf==2147483647
        static const signed int NaNRep = 2147483647;
        //Is NaN when DecimalHalf==2147483646
        static const signed int UndefinedRep = 2147483646;
    #endif
    #if defined(AltNum_EnableNilRep)
        //When both IntValue and DecimalHalf equal -2147483648 it is Nil
        static signed int const NilRep = -2147483648;
    #endif
#endif
    #if defined(AltNum_EnableApproachingValues)
        #if defined(AltNum_EnableApproachingI)
        //Is Approaching Bottom i when DecimalHalf==-2147483645:
        //If ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0..1)i
        static const signed int ApproachingImaginaryBottomRep = -2147483645;
        //Is Approaching Top i when DecimalHalf==-2147483644:
        //If ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)i
        static const signed int ApproachingImaginaryTopRep = -2147483644;
        #endif
    #endif
		
	#if defined(FlaggedNum_EnablePowers)
		//BitFlag 05 = Power of flag
		static const unsigned char NumByPowerRep = 16;
	#endif
	#if defined(FlaggedNum_EnableFractionals)
		//BitFlag 04 = Fractional Rep
		static const unsigned char NumByDivRep = 8;
	#endif
	#if defined(FlaggedNum_EnableMixedFractional)
		//BitFlag 06 = Mixed Fraction flag
		static const unsigned char MixedFracRep = 32;
	#endif

#if defined(AltNum_EnablePiRep)
        //BitFlag 01 = PiRep
		static const unsigned char PiNumRep = 1;
	#if defined(FlaggedNum_EnableFractionals)
		static const unsigned char PiNumByDivRep = 9;
	#endif
	#if defined(FlaggedNum_EnablePowers)
		static const unsigned char PiByPowerRep = 17;
	#endif
	#if defined(FlaggedNum_EnableMixedFractional)
		static const unsigned char MixedFracPiRep = 33;
	#endif
#endif
#if defined(AltNum_EnableERep)
		//BitFlag 02 = ERep
		static const unsigned char ENumRep = 2;
	#if defined(FlaggedNum_EnableFractionals)
		static const unsigned char ENumByDivRep = 10;
	#endif
	#if defined(FlaggedNum_EnablePowers)
		static const unsigned char EByPowerRep = 18;
	#endif
	#if defined(FlaggedNum_EnableMixedFractional)
		static const unsigned char MixedFracERep = 34;
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		
	#if !defined(FlaggedNum_UseBitset)
		static const unsigned char INumRep = 4;
	#endif
	#if defined(FlaggedNum_EnableFractionals)
		static const unsigned char INumByDivRep = 11;
	#endif
	#if defined(FlaggedNum_EnableMixedFractional)
		static const unsigned char MixedFracIRep = 36;
	#endif
#endif

#if defined(FlaggedNum_UseFlagForInfinitesimalRep)
//(Enum Bits:7)=64;Flag 5=16;Flag 6=32;
		static const unsigned char PositiveInfinityRep = 192;//(Enum Bits:7,8)
		static const unsigned char NegativeInfinityRep = 112;//(Enum Bits:7,6,5)

		static const unsigned char ApproachingBottom = 64;//(Enum Bits:7)
		static const unsigned char ApproachingTop = 72;//(Enum Bits:7,4)

		static const unsigned char ApproachingMidLeftRep = 80;//(Enum Bits:7,5)
		static const unsigned char ApproachingMidRightRep = 96;//(Enum Bits:7,6)

    #if defined(AltNum_EnableApproachingPi)
		static const unsigned char ApproachingTopPiRep = 65;//(Enum Bits:7,1)
    #endif
    #if defined(AltNum_EnableApproachingE)
		static const unsigned char ApproachingTopERep = 66;//(Enum Bits:7,2)
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
		static const unsigned char PositiveImaginaryInfinityRep = 196;//(Enum Bits:7,3,8)
		static const unsigned char NegativeImaginaryInfinityRep = 228;//(Enum Bits:7,3,8,6)

		static const unsigned char ApproachingImaginaryBottom = 68;//(Enum Bits:7,3)
		static const unsigned char ApproachingImaginaryBottom = 76;//(Enum Bits:7,3,4)
		static const unsigned char ApproachingImaginaryMidLeftRep = 84;//(Enum Bits:7,3,5)
		static const unsigned char ApproachingImaginaryMidLeftRep = 100;//(Enum Bits:7,3,6)
    #endif
#endif
		
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        static const signed int UndefinedInRangeRep = -2147483642;
#endif
        static const signed int AlternativeFractionalLowerBound = -2147483641;
		//Upper limit for Mixed Fractions; infinite approaching type representations at and after this DecimalHalf value
		static const signed int InfinityBasedLowerBound = -2147483644;
#if defined(AltNum_EnableInfinityRep)
        //Is NaN when DecimalHalf==2147483647
        static const signed int NaNRep = 2147483647;
        //Is NaN when DecimalHalf==2147483646
        static const signed int UndefinedRep = 2147483646;
#endif

        #pragma endregion Const Representation values
        /// <summary>
        /// Enum representing value type stored
        /// </summary>
        enum class RepType: int
        {
            NormalType = 0,
            NumByDiv = 8,//with flag 04 active
#if defined(AltNum_EnablePiRep)
            PiNum = 1,//with flag 01 active
	#if defined(FlaggedNum_EnablePowers)
            PiPower = 17,//with flag 05, and 01 active
	#endif
	#if defined(FlaggedNum_EnableFractionals)
			PiNumByDiv = 9,//  (Value/ExtraRep)*Pi Representation with flag 04 and 01 active
	#endif
#endif
#if defined(AltNum_EnableERep)
            ENum = 2,//with flag 02 active
	#if defined(FlaggedNum_EnablePowers)
            EPower = 18,//with flag 05, and 02 active
	#endif
	#if defined(FlaggedNum_EnableFractionals)
            ENumByDiv = 10,//(Value/ExtraRep)*e Representation with flag 04 and 02 active
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
            INum = 4,//with flag 03 active
	#if defined(FlaggedNum_EnableFractionals)
            INumByDiv = 11,//(Value/ExtraRep)*i Representation with flag 04 and 03 active
	#endif
#endif
#if defined(FlaggedNum_EnableMixedFractional)
			MixedFrac = 32,//IntValue +- DecimalHalf/ExtraRep with flag 06 active 
			MixedPi = 33,//IntValue +- DecimalHalf/ExtraRep) with flag 06, and 01 active
			MixedE = 34,//IntValue +- DecimalHalf/ExtraRep) with flag 06, and 02 active
			MixedI = 36,//IntValue +- DecimalHalf/ExtraRep) with flag 06, and 03 active
#endif

#if defined(AltNum_EnableInfinityRep)
			PositiveInfinity = 192,//IntValue == 1 and DecimalHalf==InfinityRep
			NegativeInfinity = 112,//IntValue == -1 and DecimalHalf==InfinityRep
#endif
	#if defined(AltNum_EnableApproachingValues)
            ApproachingBottom = 64,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)  (Enum Bits:7,5)
		#if !defined(AltNum_DisableApproachingTop)
            ApproachingTop = 72,//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9) (Enum Bits:7,6)
		#endif
		#if defined(AltNum_EnableApproachingDivided)
			ApproachingMidLeft = 80,//(Enum Bits:7,5,8)
			#if !defined(AltNum_DisableApproachingTop)
            ApproachingMidRight = 96,//(Enum Bits:7,6,8)
			#endif
		#endif
	#endif
    #if defined(AltNum_EnableNaN)
            Undefined = 128,//(Enum Bits:8)
            NaN = 129,//(Enum Bits:8, 1)
    #endif
#if defined(AltNum_EnableApproachingPi)
            ApproachingTopPi = 65,//equal to IntValue.9..9 Pi (Enum Bits:7,1,5)
#endif
#if defined(AltNum_EnableApproachingE)
            ApproachingTopE = 66,//equal to IntValue.9..9 e (Enum Bits:7,2, 5)
#endif
#if defined(AltNum_EnableImaginaryInfinity)
            PositiveImaginaryInfinity = 196,//IntValue == 1 and DecimalHalf==InfinityRep and flag 03 active (Enum Bits:7,3)
			NegativeImaginaryInfinity = 228,//IntValue == -1 and DecimalHalf==InfinityRep and flag 03 active (Enum Bits:7,3,4)
	#if defined(AltNum_EnableApproachingI)
            ApproachingImaginaryBottom = 68,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
		#if !defined(AltNum_DisableApproachingTop)
            ApproachingImaginaryTop = 76,//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
		#endif
	#if defined(AltNum_EnableApproachingDivided)
            ApproachingImaginaryMidRight = 84,//(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive, IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
		#if !defined(AltNum_DisableApproachingTop)
			ApproachingImaginaryMidLeft = 100,//(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive, IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
		#endif
	#endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
            UndefinedButInRange = 130,//(Enum Bits:8, 2)
#endif
    #if defined(AltNum_EnableNilRep)
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
	#if defined(FlaggedNum_EnableFractionals)
				case RepType::NumByDiv:
					return "NumByDiv"; break;
	#endif
	#if defined(AltNum_EnablePiRep)
				case RepType::PiNum:
					return "PiNum"; break;
		#if defined(AltNum_EnablePiPowers)
				case RepType::PiPower:
					return "PiPower"; break;
		#endif
		#if defined(FlaggedNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
					return "PiNumByDiv"; break;
		#endif
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::ENum:
					return "ENum"; break;
		#if defined(FlaggedNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
					return "ENumByDiv"; break;
		#endif
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
                    return "INum"; break;
		#if defined(FlaggedNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
					return "INumByDiv"; break;
		#endif
		#ifdef AltNum_EnableComplexNumbers
				case RepType::ComplexIRep:
					return "ComplexIRep"; break;
		#endif
	#endif

	#if defined(FlaggedNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
					return "MixedFrac"; break;
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedPi"; break;
		#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedE"; break;
		#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
					return "MixedI"; break;
		#endif
	#endif

	#if defined(AltNum_EnableInfinityRep)
				case RepType::PositiveInfinity://If Positive Infinity: then convert number into MaximumValue instead when need as real number
					return "PositiveInfinity"; break;
				case RepType::NegativeInfinity://If Negative Infinity: then convert number into MinimumValue instead when need as real number
					return "NegativeInfinity"; break;
	#endif
	#if defined(AltNum_EnableApproachingValues)
				case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                    return "ApproachingBottom"; break;
		#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                    return "ApproachingTop"; break;
		#endif
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeft:
					return "ApproachingMidLeft"; break;
				case RepType::ApproachingMidRight:
					return "ApproachingMidRight"; break;
		#endif
	#endif
    #if defined(AltNum_EnableNaN)
				case RepType::Undefined:
					return "Undefined"; break;
				case RepType::NaN:
					return "NaN"; break;
    #endif
	#if defined(AltNum_EnableApproachingPi)
				case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
					return "ApproachingTopPi"; break;
	#endif
	#if defined(AltNum_EnableApproachingE)
				case RepType::ApproachingTopE://equal to IntValue.9..9 e
					return "ApproachingTopE"; break;
	#endif
	#if defined(AltNum_EnableImaginaryInfinity)
				case RepType::PositiveImaginaryInfinity:
					return "PositiveImaginaryInfinity"; break;
				case RepType::NegativeImaginaryInfinity:
					return "NegativeImaginaryInfinity"; break;
	#endif
	#if defined(AltNum_EnableApproachingI)
				case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
					return "ApproachingImaginaryBottom"; break;
		#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
					return "ApproachingImaginaryTop"; break;
		#endif
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingImaginaryMidLeft:
					return "ApproachingImaginaryMidLeft"; break;
			#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryMidRight:
					return "ApproachingImaginaryMidRight"; break;
			#endif
		#endif
    #endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range: ExtraRepValue==UndefinedInRangeRep)
				case RepType::UndefinedButInRange:
					return "UndefinedButInRange"; break;
		#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
				case RepType::WithinMinMaxRange:
					return "WithinMinMaxRange"; break;
		#endif
	#endif
    #if defined(AltNum_EnableNilRep)
				case RepType::Nil:
					return "Nil"; break;
    #endif
				default:
					return "Unknown";
			}
		}

        RepType GetRepType()
        {
#if defined(AltNum_EnableInfinityRep)
            if(DecimalHalf==InfinityRep)
            {
	#if defined(AltNum_EnableImaginaryInfinity)
		#if !defined(FlaggedNum_UseBitset)
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
#if defined(AltNum_EnableApproachingValues)
            if (DecimalHalf == ApproachingBottomRep)
            {
				if(ExtraRep==0)
					return RepType::ApproachingBottom;//Approaching from right to IntValue;(IntValue of 0 results in 0.00...1)
				else
	#if defined(AltNum_EnableApproachingDivided)//if(ExtraRep>1)
					return RepType::ApproachingMidLeft;//ExtraRep value of 2 results in 0.49999...9
	#else
                    throw "EnableApproachingDivided feature not enabled";
	#endif	
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingTop;//Approaching from left to (IntValue-1);(IntValue of 0 results in 0.99...9)
	#if defined(AltNum_EnableApproachingPi)
		#if !defined(FlaggedNum_UseBitset)
                else if (FlagsActive == PiNumRep)
		#else
				else if(FlagsActive.test(1))
		#endif
                    return RepType::ApproachingTopPi;
	#endif
	#if defined(AltNum_EnableApproachingE)
		#if !defined(FlaggedNum_UseBitset)
                else if (FlagsActive == ENumRep)
		#else
				else if(FlagsActive.test(2))
		#endif
                    return RepType::ApproachingTopE;
	#endif
                else
	#if defined(AltNum_EnableApproachingDivided)
					return RepType::ApproachingMidRight;//ExtraRep value of 2 results in 0.500...1
	#else
                    throw "EnableApproachingDivided feature not enabled";
	#endif            
            }
	#if defined(AltNum_EnableImaginaryInfinity)//ApproachingImaginaryValRep
            else if (DecimalHalf == ApproachingImaginaryBottomRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingImaginaryBottom;//Approaching from right to IntValue;(IntValue of 0 results in 0.00...1)
                else
		#if defined(AltNum_EnableApproachingDivided)
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
	#if defined(AltNum_EnableApproachingDivided)
					return RepType::ApproachingImaginaryMidRight;//ExtraRep value of 2 results in 0.500...1
	#else
                    throw "EnableApproachingDivided feature not enabled";
#endif            
            }
#endif
	#endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
			else if(ExtraRep==UndefinedButInRange)
                return RepType::UndefinedButInRange;
#endif
			else
			{
#if !defined(FlaggedNum_UseBitset)
				switch(FlagsActive)
				{
					case 0://NormalType and NaN
	#if defined(AltNum_EnableNaN)
					if(DecimalHalf==NaNRep)
						return RepType::NaN;
					else if(DecimalHalf==UndefinedRep)
						return RepType::Undefined;
					else
	#endif
						return RepType::NormalType;
						break;
	#if defined(FlaggedNum_EnableFractionals)
					case NumByDivRep:
						return RepType::NumByDiv;
						break;
	#endif
	#if defined(FlaggedNum_EnablePowers)
					case NumByPowerRep:
						return RepType::NumByPower;
						break;
	#endif
	#if defined(AltNum_EnableMixedFractional)
					case MixedFracRep:
						return RepType::MixedFrac;
						break;
	#endif
	#ifdef AltNum_EnablePiRep
					case PiNumRep:
						return RepType::PiNum;
						break;
		#if defined(FlaggedNum_EnableFractionals)
					case PiNumByDivRep:
						return RepType::PiNumByDiv;
						break;
		#endif
		#if defined(FlaggedNum_EnablePowers)
					case PiNumByPowerRep:
						return RepType::PiNumByPower;
						break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
					case MixedFracPiRep:
						return RepType::MixedPi;
						break;
		#endif
	#endif
	#ifdef AltNum_EnableENum
					case ENumRep:
						return RepType::ENum;
						break;
		#if defined(FlaggedNum_EnableFractionals)
					case ENumByDivRep:
						return RepType::ENumByDiv;
						break;
		#endif
		#if defined(FlaggedNum_EnablePowers)
					case ENumByPowerRep:
						return RepType::ENumByPower;
						break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
					case MixedFracERep:
						return RepType::MixedE;
						break;
		#endif
	#endif
	#ifdef AltNum_EnableImaginaryNum
					case INumRep:
						return RepType::INum;
						break;
		#if defined(FlaggedNum_EnableFractionals)
					case INumByDivRep:
						return RepType::INumByDiv;
						break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
					case MixedFracIRep:
						return RepType::MixedI;
						break;
		#endif
	#endif
				}
#else

#endif
			}
        }

    #pragma region PiNum Setters
    #if defined(AltNum_EnablePiRep)
        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetPiValFromMediumDec(MediumDec Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
        }
        #endif

        void SetPiVal(const FlaggedDec& Value)
        {
            if(ExtraRep==0)
            {
                IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                ExtraRep = PiRep;
            }
        }
        
        void SetPiVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = PiRep;
        }
    #endif
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
    #if defined(AltNum_EnableERep)
        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetEValFromMediumDec(const MediumDec& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
        }
        #endif

        void SetEVal(const FlaggedDec& Value)
        {
            if(ExtraRep==0)
            {
                IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                ExtraRep = ERep;
            }
        }
        
        void SetEVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = ERep;
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region Fractional Setters
    #if defined(AltNum_EnableFractionals)
        //Set value for NumByDiv
        void SetFractionalVal(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = IntHalf; DecimalHalf = IntHalf;
            ExtraRep = Divisor;
        }
        
        //Set value for NumByDiv
        void SetFractionalVal(FlaggedDec Value, int Divisor)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = Divisor;
        }

        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetFractionalVal(MediumDec Value, int Divisor)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = Divisor;
        }
        #endif
        
        //Set value for NumByDiv
        void SetFractionalVal(int Value, int Divisor)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = Divisor;
        }
    #endif
        
    #if defined(AltNum_EnableDecimaledPiFractionals)
        //Set value for PiNumByDiv
        void SetDecimaledPiFractional(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = IntHalf; DecimalHalf = DecHalf;
            ExtraRep = -Divisor;
        }
    #elif defined(AltNum_EnableDecimaledEFractionals)
        //Set value for ENumByDiv
        void SetDecimaledEFractional(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = -Divisor;
        }
    #elif defined(AltNum_EnableDecimaledIFractionals)
        //Set value for INumByDiv
        void SetDecimaledIFractional(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = -Divisor;
        }
    #endif
    #if defined(AltNum_EnableAlternativeRepFractionals)
        #if defined(AltNum_EnablePiFractional)
        void SetPiFractional(int Num, int Divisor)//IntValue/DecimalHalf*Pi Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = PiRep;
        }
        #endif
        #if defined(AltNum_EnableEFractional)
        void SetEFractional(int Num, int Divisor)//IntValue/DecimalHalf*e Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = ERep;
        }
        #endif
        #if defined(AltNum_EnableIFractional)
        void SetIFractional(int Num, int Divisor)//IntValue/DecimalHalf*i Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = IRep;
        }
        #endif
    #endif
    #pragma endregion Fractional Setters

    #pragma region MixedFrac Setters
    #if defined(AltNum_EnableMixedFractional)
        void SetMixedFractionalVal(int WholeNum, int Numerator, int Denom)
        {
            IntValue = WholeNum;
            DecimalHalf = Numerator*-1;
            ExtraRep = Denom;
        }
        
        void SetMixedFractionalValAsNeg(int WholeNum, int NumeratorAsNeg, int Denom)
        {
            IntValue = WholeNum;
            DecimalHalf = NumeratorAsNeg;
            ExtraRep = Denom;
        }
    #endif
    #pragma endregion MixedFrac Setters

    #pragma region Infinity Setters
    //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
    // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
    #if defined(AltNum_EnableInfinityRep)
        void SetAsInfinity()
        {
            IntValue = 1; DecimalHalf = InfinityRep;
            ExtraRep = 0;
        }

        void SetAsNegativeInfinity()
        {
            IntValue = -1; DecimalHalf = InfinityRep;
            ExtraRep = 0;
        }
    #endif
    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters
    #if defined(AltNum_EnableApproachingValues)
        //Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
        //If AltNum_EnableApproachingDivided is enabled and Divisor value is greator than 1, Approaching Fractional from left;ExtraRep value of 2 results in value.499...9(for positive value:value.(1/Divisor-JustAboveZero))
        #if defined(AltNum_EnableApproachingDivided)
        void SetAsApproachingBottom(int value=0, int Divisor=0)
        #else
        void SetAsApproachingBottom(int value=0)
        #endif
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
        #if defined(AltNum_EnableApproachingDivided)
            ExtraRep = Divisor;
        #else
            ExtraRep = 0;
        #endif
        }
        
        #if !defined(AltNum_DisableApproachingTop)
        //Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        //If Divisor is negative, Approaching Fractional from right;ExtraRep value of 2 results in value.500...1(for positive value:value.(1/Divisor+JustAboveZero))
            #if defined(AltNum_EnableApproachingDivided)
        void SetAsApproachingTop(int value, int Divisor=0)
            #else
        void SetAsApproachingTop(int value)
            #endif
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
            #if defined(AltNum_EnableApproachingDivided)
            ExtraRep = Divisor;
            #else
            ExtraRep = 0;
            #endif
        }
        
            #if defined(AltNum_EnableApproachingPi)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)Pi
        //If Divisor is negative, Approaching Fractional from right;ExtraRep value of 2 results in value.500...1(for positive value:value.(1/Divisor+JustAboveZero))
        void SetAsApproachingTopPi(int value)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
            ExtraRep = PiRep;
        }
            #endif
            
            #if defined(AltNum_EnableApproachingE)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)e
        //If Divisor is negative, Approaching Fractional from right;ExtraRep value of 2 results in value.500...1(for positive value:value.(1/Divisor+JustAboveZero))
        void SetAsApproachingTopPi(int value)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
            ExtraRep = ERep;
        }
            #endif
        #endif
    #endif

    #if defined(AltNum_EnableApproachingI)
        //Approaching Towards values from right to left side(IntValue.000...1)i
        //If AltNum_EnableApproachingDivided is enabled and Divisor value is greator than 1, Approaching Fractional from left;ExtraRep value of 2 results in value.499...9(for positive value:value.(1/Divisor-JustAboveZero))
            #if defined(AltNum_EnableApproachingDivided)
        void SetAsApproachingImaginaryBottom(int value=0, int Divisor=0)
            #else
        void SetAsApproachingImaginaryBottom(int value=0)
            #endif
        {
            IntValue = value; DecimalHalf = ApproachingImaginaryBottomRep;
            #if defined(AltNum_EnableApproachingDivided)
            ExtraRep = Divisor;
            #else
            ExtraRep = 0;
            #endif
        }
        
        #if !defined(AltNum_DisableApproachingTop)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)i
        //If Divisor is negative, Approaching Fractional from right;ExtraRep value of 2 results in value.500...1(for positive value:value.(1/Divisor+JustAboveZero))
            #if defined(AltNum_EnableApproachingDivided)
        void SetAsApproachingImaginaryTop(int value, int Divisor=0)
            #else
        void SetAsApproachingImaginaryTop(int value)
            #endif
        {
            IntValue = value; DecimalHalf = ApproachingImaginaryTopRep;
            #if defined(AltNum_EnableApproachingDivided)
            ExtraRep = Divisor;
            #else
            ExtraRep = 0;
            #endif
        }
        #endif
    #endif
    #pragma endregion ApproachingZero Setters

    #pragma region NaN Setters
    #if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep;
            ExtraRep = 0;
        }

        void SetAsUndefined()
        {
            IntValue = 0; DecimalHalf = UndefinedRep;
            ExtraRep = 0;
        }
    #endif
    #pragma endregion NaN Setters

    #pragma region ValueDefines
    protected:
    #if defined(AltNum_EnableNaN)
        static FlaggedDec NaNValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, NaNRep);
            return NewSelf;
        }
        
        static FlaggedDec UndefinedValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, UndefinedRep);
            return NewSelf;
        }
    #endif

    #if defined(AltNum_EnableInfinityRep)
        static FlaggedDec InfinityValue()
        {
            FlaggedDec NewSelf = FlaggedDec(1, InfinityRep);
            return NewSelf;
        }
        
        static FlaggedDec NegativeInfinityValue()
        {
            FlaggedDec NewSelf = FlaggedDec(-1, InfinityRep);
            return NewSelf;
        }
    #endif
        
    #if defined(AltNum_EnableApproachingValues)
        static FlaggedDec ApproachingZeroValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, ApproachingBottomRep);
            return NewSelf;
        }
        
        static FlaggedDec ApproachingOneFromLeftValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, ApproachingTopRep);
            return NewSelf;
        }
        
        static FlaggedDec ApproachingOneFromRightValue()
        {
            FlaggedDec NewSelf = FlaggedDec(1, ApproachingBottomRep);
            return NewSelf;
        }

        static FlaggedDec NegativeApproachingZeroValue()
        {
            FlaggedDec NewSelf = FlaggedDec(NegativeRep, ApproachingBottomRep);
            return NewSelf;
        }
    #endif
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec PiNumValue()
        {
            return FlaggedDec(3, 141592654);
        }
        
        static FlaggedDec PiValue()
        {
#if defined(AltNum_EnableERep)
            return FlaggedDec(1, PiRep);
#else
            return FlaggedDec(3, 141592654);
#endif
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static FlaggedDec HundredMilPiNumVal()
        {
            return FlaggedDec(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static FlaggedDec TenMilPiNumVal()
        {
            return FlaggedDec(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static FlaggedDec OneMilPiNumVal()
        {
            return FlaggedDec(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static FlaggedDec TenPiNumVal()
        {
            return FlaggedDec(31, 415926536);
        }
        
        static FlaggedDec ENumValue()
        {
            return FlaggedDec(2, 718281828);
        }

        static FlaggedDec EValue()
        {
#if defined(AltNum_EnableERep)
            return FlaggedDec(1, ERep);
#else
            return FlaggedDec(2, 718281828);
#endif
        }
        
        static FlaggedDec ZeroValue()
        {
            return FlaggedDec();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec OneValue()
        {
            FlaggedDec NewSelf = FlaggedDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec TwoValue()
        {
            FlaggedDec NewSelf = FlaggedDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec NegativeOneValue()
        {
            FlaggedDec NewSelf = FlaggedDec(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Point5Value()
        {
            FlaggedDec NewSelf = FlaggedDec(0, 500000000);
            return NewSelf;
        }

        static FlaggedDec JustAboveZeroValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, 1);
            return NewSelf;
        }

        static FlaggedDec OneMillionthValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, 1000);
            return NewSelf;
        }

        static FlaggedDec FiveThousandthValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, 5000000);
            return NewSelf;
        }

        static FlaggedDec FiveMillionthValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, 5000);
            return NewSelf;
        }

        static FlaggedDec TenMillionthValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, 100);
            return NewSelf;
        }

        static FlaggedDec OneHundredMillionthValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, 10);
            return NewSelf;
        }

        static FlaggedDec FiveBillionthValue()
        {
            FlaggedDec NewSelf = FlaggedDec(0, 5);
            return NewSelf;
        }

        static FlaggedDec LN10Value()
        {
            return FlaggedDec(2, 302585093);
        }

        static FlaggedDec LN10MultValue()
        {
            return FlaggedDec(0, 434294482);
        }

        static FlaggedDec HalfLN10MultValue()
        {
            return FlaggedDec(0, 868588964);
        }
        
    #if defined(AltNum_EnableNilRep)
        static FlaggedDec NilValue()
        {
            return FlaggedDec(NilRep, NilRep);
        }
    #endif

        static FlaggedDec MinimumValue()
        {
            return FlaggedDec(2147483647, 999999999);
        }

        static FlaggedDec MaximumValue()
        {
            return FlaggedDec(2147483647, 999999999);
        }
public:
        static FlaggedDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec ENum;
        
#if defined(AltNum_EnableInfinityRep)
        static FlaggedDec Infinity;
        static FlaggedDec NegativeInfinity;
        static FlaggedDec ApproachingZero;
#endif

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec E;
        
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec OneGMillionth;

        //0e-7
        static FlaggedDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static FlaggedDec FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static FlaggedDec Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static FlaggedDec Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static FlaggedDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static FlaggedDec LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static FlaggedDec HalfLN10Mult;

    #if defined(AltNum_EnableNilRep)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static FlaggedDec Nil;
    #endif
    
#if defined(AltNum_EnableApproachingValues)
        static FlaggedDec ApproachingRightRealValue(int IntValue=0)
        {
            return FlaggedDec(IntValue, 999999999);
        }

        static FlaggedDec ApproachingLeftRealValue(int IntValue=0)
        {
            return FlaggedDec(IntValue, 1);
        }

        static FlaggedDec LeftAlmostPointFiveRealValue(int IntValue=0)
        {
            return FlaggedDec(IntValue, 499999999);
        }

        static FlaggedDec RightAlmostPointFiveRealValue(int IntValue=0)
        {
            return FlaggedDec(IntValue, 500000001);
        }
#endif
    
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
        /// <returns>FlaggedDec</returns>
        FlaggedDec GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        FlaggedDec(const char* strVal)
        {
            std::string Value = strVal;
            if (Value == "Pi")
            {
                this->SetVal(Pi);
            }
            else if (Value == "E")
            {
                this->SetVal(E);
            }
            else
            {
                this->ReadString(Value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FlaggedDec(std::string Value)
        {
            if (Value == "Pi")
            {
                this->SetVal(Pi);
            }
            else if (Value == "E")
            {
                this->SetVal(E);
            }
            else
            {
                this->ReadString(Value);
            }
        }

#pragma endregion String Commands

protected:
        std::string BasicToStringOp();

        std::string BasicToFullStringOp();
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
            bool IsNegative = Value < 0.0f;
            if (IsNegative) { Value *= -1.0f; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0f)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (float)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(double Value)
        {
            bool IsNegative = Value < 0.0;
            if (IsNegative) { Value *= -1.0; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (double)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(ldouble Value)
        {
            bool IsNegative = Value < 0.0L;
            if (IsNegative) { Value *= -1.0L; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0L)
            {
                if (IsNegative)
                    IntValue = -2147483647;
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (ldouble)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(bool Value)
        {
            IntValue = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FlaggedDec(float Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FlaggedDec(double Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FlaggedDec(ldouble Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FlaggedDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FlaggedDec(bool Value)
        {
            this->SetVal(Value);
        }

    #if defined(AltNum_EnableMediumDecBasedSetValues)
        FlaggedDec(MediumDec Value)
        {
            this->SetVal(Value);
        }
    #endif
    #pragma endregion From Standard types to this type

    #pragma region MirroredIntBased Operations
#if !defined(AltNum_StoreIntSectionFunctionsInBase)

        template<IntegerType IntType=int>
        void IntHalfDivision(IntType RValue)
        {
            IntValue /= RValue;
        }

        template<IntegerType IntType=int>
        void IntHalfMultiplication(IntType RValue)
        {
            if(IntValue!=0&&IntValue!=NegativeRep)
                IntValue *= RValue;
        }

        template<IntegerType IntType=int>
        void UIntHalfMultiplication(IntType RValue)
        {
            if(IntValue!=0&&IntValue!=NegativeRep)
                IntValue *= RValue;
        }

        //Replace usage of IntValue += RValue; with IntHalfAddition(RValue);
        template<IntegerType IntType>
        void IntHalfAddition(IntType RValue)
        {//Can be used without modifying RValue (passing by value for speed) 
            if (RValue==0)
                return;
            if (IntValue == 0)
                IntValue = RValue;
            else if (IntValue == NegativeRep)
            {
                //-0.XXXX + 2 = 1.XXXX
                //-0.XXXX + 1 = 0.XXXX (Flips to other side)
                //-0.XXXX + -5 = -5.XXXX
                if (RValue < 0)
                {
                    IntValue = RValue;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
                else//(RValue>=0)
                    IntValue = (int)RValue - 1;
            }
            else if (IntValue < 0)
            {
                //-1.XXXX + -5 = -6.XXXX
                //-6.XXXX + 5 = -1.XXXX
                //-6.XXXX + 6 = -0.XXXX
                //-5.XXXX + 6 = 0.XXXX (Flips to other side)
                //-5.XXXX + 7 = 1.XXXX
                int InvertedLValue = -IntValue;
                if (IntValue == InvertedLValue)
                    IntValue = NegativeRep;
                else if (RValue > InvertedLValue)
                    IntValue += (int)RValue - 1;
                else
                {
                    IntValue += (int)RValue;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
            }
            else
            {
                //5.XXXX + 5 = 10.XXXX
                //5.XXXX + -5 = 0.XXXX
                //5.XXXX + -6 = -0.XXXX (Flips to other side)
                //5.XXXX + -7 = -1.XXXX 
                int InversionPoint = -IntValue - 1;
                if (RValue == InversionPoint)
                    IntValue = NegativeRep;
                else if (RValue < InversionPoint)
                {
                    IntValue = (int)RValue + 1;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
                else
                    IntValue += (int)RValue;
            }
        }

        //Replace usage of IntValue -= RValue; with IntHalfSubtraction(RValue);
        template<IntegerType IntType>
        void IntHalfSubtraction(IntType RValue)
        {
            if (RValue==0)
                return;
            if (IntValue == 0)
                IntValue = -(int)RValue;
            else if (IntValue == NegativeRep)
            {
                //-0.XXXX - -2 = 1.XXXX
                //-0.XXXX - -1 = 0.XXXX (Flips to other side)
                //-0.XXXX - 1 = -1.XXXX 
                //-0.XXXX - 6 = -6.XXXX
                if (RValue < 0)
                    IntValue = -(int)RValue - 1;
                else
                {
                    IntValue = -(int)RValue;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
            }
            else if (IntValue < 0)
            {
                //-5.XXXX - -7 = 1.XXXX 
                //-5.XXXX - -6 = 0.XXXX            
                //-5.XXXX - -5 = -0.XXXX (Flips to other side)
                //-5.XXXX - -4 = -1.XXXX
                if (RValue == IntValue)
                    IntValue = NegativeRep;
                else if (RValue < IntValue)
                    IntValue -= (int)RValue - 1;
                else//(RValue>=LValue.Value)
                {
                    IntValue -= (int)RValue;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
            }
            else
            {
                //5.XXXX - -5 = 10.XXXX
                //5.XXXX - 5 = 0.XXXX
                //5.XXXX - 6 = -0.XXXX (Flips to other side)
                //5.XXXX - 7 = -1.XXXX
                int InversionPoint = IntValue + 1;
                if (RValue == InversionPoint)
                    IntValue = NegativeRep;
                else if (RValue > InversionPoint)
                {
                    IntValue -= (int)RValue - 1;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
                else
                    IntValue += (int)RValue;
            }
        }
#endif
    #pragma endregion MirroredIntBased Operations

    #pragma region From this type to Standard types
        /// <summary>
        /// FlaggedDec to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float()
        {
            float Value;
            if (IntValue < 0)
            {
    #if !defined(FlaggedDec_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
    #if !defined(FlaggedDec_UseMirroredInt)
                Value = (float)IntValue;
    #else
                Value = (float)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
        }

        /// <summary>
        /// FlaggedDec to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double()
        {
            double Value;
            if (IntValue < 0)
            {
    #if !defined(FlaggedDec_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0 : (double)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0 : (double)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
    #if !defined(FlaggedDec_UseMirroredInt)
                Value = (double)IntValue;
    #else
                Value = (double)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
        }

        /// <summary>
        /// FlaggedDec to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble()
        {
            ldouble Value;
            if (IntValue < 0)
            {
    #if !defined(FlaggedDec_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
    #if !defined(FlaggedDec_UseMirroredInt)
                Value = (ldouble)IntValue;
    #else
                Value = (ldouble)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
            }
            return Value;
        }

        /// <summary>
        /// FlaggedDec to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return GetIntHalf(); }

        explicit operator bool() { return IntValue == 0 ? false : true; }
    #pragma endregion From this type to Standard types

    #pragma region Pi Conversion
    #if defined(AltNum_EnablePiRep)
        //3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844 *selfNum
        //3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844 *selfNum
        void ConvertPiToNum()
        {
            ExtraRep = 0;
            // Can only convert to up 683565275.1688666254437963172038917047964296646843381624484789109135725652864987887127902610635528943x PiRepresentation
            //Can Represent up ? before hitting Maximum FlaggedDec value on reconversion when AltNum_UseLowerPrecisionPi is enabled
            //otherwise can represent up to ???(when adding up value from each decimal place of IntValue + (PiNum*DecimalHalf/1000000000))
            //Calculations from HiPer Calc
            //683565275.168866625 x 3.141592654 = 2147483646.99999999860577275
            //683565275.168866626 x 3.141592654 = 2147483647.000000001747365404
            if (IntValue >= 683565275 && DecimalHalf >= 168866626)//Exceeding Storage limit of NormalRep
            {
                throw "Conversion of Pi multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
                IntValue = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
            }
            else if (IntValue <= -683565275 && DecimalHalf >= 168866626)//Exceeding Storage limit of NormalRep
            {
                throw "Conversion of Pi multiplication into MediumDec format resulted in underflow(setting value to minimum MediumDec value)";
                IntValue = -2147483647; DecimalHalf = 999999999;//set value as minimum value(since not truely infinite just bit above storage range)
            }
            //Maximum result value              = 2147483647.999999999
            //Pi * 2147483647              =   6,746,518,849.1194168257096980859855
            //Int32 Max=
            //2147483647
            //683565275.168866625 x 3.141592654 = 2147483646.99999999860577275
            //Pi * 683565275                    = 2147483646.189086752242857401518383790899531254705429038668838768350655406412544182854647248094561223
            //Int64Max                 = 9223372036854775807
            //                     9,223,372,036,854,775,807
            //3,141,592653.5897932384626433832795 * 2147483647
            //                          =6746518849119416825.7096980859855
            //Pi =
            //3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844
            //For X.0 * Pi results
            //3141592653 * 683565275    =2147483645785924575
            //IntValue = 2147483645 (Res / 1000000000)
            //DecimalHalf = 785924575
            //3141592654 * 683565275    =2147483646469489850
            //IntValue = 2147483646 (Res / 1000000000)
            //DecimalHalf = 469489850
            //For 0.X * Pi results
            //Pi * 0.999999999        = 3.141592650448200584872850144816859500917666515177936421599838771332871813978392592341825826714082243
            //3141592654 * 999999999    =3141592650858407346
            //IntValue = 3 (Res / 1000000000000000000)
            //DecimalHalf = 141592650 (Rounded up techically equals 141592651) ((SRep - 1000000000000000000 * divRes)/DecimalOverflowX)
            __int64 SRep;
            __int64 divRes;
            if (DecimalHalf == 0)
            {
                bool IsNegative = IntValue < 0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = 3141592654;
                SRep *= IntValue;
                //__int64 divRes = SRep / DecimalOverflowX;
                //__int64 C = SRep - DecimalOverflowX * divRes;
                divRes = SRep / DecimalOverflowX;
                DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
                if (divRes == 0 && IsNegative)
                {
                    if (DecimalHalf == 0)
                        IntValue = 0;
                    else
                        IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-divRes;
                else
                    IntValue = (int)divRes;
            }
            else if (IntValue == 0)
            {
                SRep = 3141592654;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
            }
            else if (IntValue == NegativeRep)
            {
                SRep = 3141592654;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
                if (divRes == 0)
                    IntValue = NegativeRep;
                else
                    IntValue = (int)-divRes;
            }
            else
            {
                bool IsNegative = IntValue < 0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
                //X.Y *.V
                __int64 Temp03 = (__int64)141592654ll * IntValue;//Temp03 holds __int64 version of X *.V
                __int64 Temp04 = (__int64)DecimalHalf * 141592654ll;
                Temp04 /= FlaggedDec::DecimalOverflow;
                //Temp04 holds __int64 version of .Y * .V
                __int64 IntegerRep = SRep + Temp03 + Temp04;
                __int64 IntHalf = IntegerRep / FlaggedDec::DecimalOverflow;
                IntegerRep -= IntHalf * (__int64)FlaggedDec::DecimalOverflow;
                DecimalHalf = (signed int)IntegerRep;
                if (IntHalf == 0 && IsNegative)
                {
                    IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-IntHalf;
                else
                    IntValue = (int)IntHalf;
            }
        }

        #if defined(AltNum_EnableDecimaledPiFractionals)
        //Convert from PiByDiv to NumByDivisor representation
        void ConvertPiByDivToNumByDiv()
        {
            __int64 SRep;
            __int64 divRes;
            if (DecimalHalf == 0)
            {
                bool IsNegative = IntValue < 0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = 3141592654;
                SRep *= IntValue;
                //__int64 divRes = SRep / DecimalOverflowX;
                //__int64 C = SRep - DecimalOverflowX * divRes;
                divRes = SRep / DecimalOverflowX;
                DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
                if (divRes == 0 && IsNegative)
                {
                    if (DecimalHalf == 0)
                        IntValue = 0;
                    else
                        IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-divRes;
                else
                    IntValue = (int)divRes;
            }
            else if (IntValue == 0)
            {
                SRep = 3141592654;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
            }
            else if (IntValue == NegativeRep)
            {
                SRep = 3141592654;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
                if (divRes == 0)
                    IntValue = NegativeRep;
                else
                    IntValue = (int)-divRes;
            }
            else
            {
                bool IsNegative = IntValue < 0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
                //X.Y *.V
                __int64 Temp03 = (__int64)141592654ll * IntValue;//Temp03 holds __int64 version of X *.V
                __int64 Temp04 = (__int64)DecimalHalf * 141592654ll;
                Temp04 /= FlaggedDec::DecimalOverflow;
                //Temp04 holds __int64 version of .Y * .V
                __int64 IntegerRep = SRep + Temp03 + Temp04;
                __int64 IntHalf = IntegerRep / FlaggedDec::DecimalOverflow;
                IntegerRep -= IntHalf * (__int64)FlaggedDec::DecimalOverflow;
                DecimalHalf = (signed int)IntegerRep;
                if (IntHalf == 0 && IsNegative)
                {
                    IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-IntHalf;
                else
                    IntValue = (int)IntHalf;
            }
            ExtraRep *= -1;
        }

        //Convert from PiByDiv to NormalType representation
        void ConvertPiByDivToNorm()
        {
            BasicIntDivOp(-ExtraRep);
            ExtraRep = 0;
            __int64 SRep;
            __int64 divRes;
            if (DecimalHalf == 0)
            {
                bool IsNegative = IntValue < 0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = 3141592654;
                SRep *= GetIntHalf();
                divRes = SRep / DecimalOverflowX;
                DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
                if (divRes == 0 && IsNegative)
                {
                    if (DecimalHalf == 0)
                        IntValue = 0;
                    else
                        IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-divRes;
                else
                    IntValue = (int)divRes;
            }
            else if (IntValue == 0)
            {
                SRep = 3141592654;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
            }
            else if (IntValue == NegativeRep)
            {
                SRep = 3141592654;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
                if (divRes == 0)
                    IntValue = NegativeRep;
                else
                    IntValue = (int)-divRes;
            }
            else
            {
                bool IsNegative = IntValue < 0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
                //X.Y *.V
                __int64 Temp03 = (__int64)141592654ll * IntValue;//Temp03 holds __int64 version of X *.V
                __int64 Temp04 = (__int64)DecimalHalf * 141592654ll;
                Temp04 /= FlaggedDec::DecimalOverflow;
                //Temp04 holds __int64 version of .Y * .V
                __int64 IntegerRep = SRep + Temp03 + Temp04;
                __int64 IntHalf = IntegerRep / FlaggedDec::DecimalOverflow;
                IntegerRep -= IntHalf * (__int64)FlaggedDec::DecimalOverflow;
                DecimalHalf = (signed int)IntegerRep;
                if (IntHalf == 0 && IsNegative)
                {
                    IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-IntHalf;
                else
                    IntValue = (int)IntHalf;
            }
        }
        #else
        //Convert from PiFractional to NormalType representation
        void ConvertPiFracToNorm()
        {
            int divisor = DecimalHalf;
            DecimalHalf = 0;
            ExtraRep = 0;
            bool IsNegative = IntValue<0;
            if (IsNegative)
                IntValue *= -1;
            __int64 SRep = 3141592654ll;
            SRep *= IntValue;
            __int64 divRes = SRep / DecimalOverflowX;
            DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
            if (divRes == 0 && IsNegative)
            {
                if (DecimalHalf == 0)
                    IntValue = 0;
                else
                    IntValue = NegativeRep;
            }
            else if (IsNegative)
                IntValue = (int)-divRes;
            else
                IntValue = (int)divRes;
            BasicIntDivOp(divisor);
        }

        #endif

        void ConvertPiPowerToNum();

        FlaggedDec PiPowerNum(int powerExponent);

        void ConvertPiPowerToPiRep();

        void ConvertToPiRep(RepType repType)
        {
            switch (repType)
            {
                case RepType::PiNum:
                    return;
                    break;
    #if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    ConvertPiPowerToPiRep();
                    break;
    #endif
    #if defined(AltNum_EnableAlternativeRepFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                {
                    BasicUIntDivOp(-ExtraRep);
                }
        #else
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                {
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicUnsignedDivOp(divisor);
                }
        #endif
                break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi:
                    return;//Add Conversion Code from MixedPi later
    #endif
                default:
                    break;
            }
            ExtraRep = PiRep;
        }

        FlaggedDec ConvertAsPiRep(RepType repType)
        {
            switch (repType)
            {
                case RepType::PiNum:
                    return *this;
                    break;
    #if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                {
                    FlaggedDec Res = *this;
                    Res.ConvertPiPowerToPiRep();
                    return Res;
                    break;
                }
    #endif
    #if defined(AltNum_EnableAlternativeRepFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                {
                    FlaggedDec Res = FlaggedDec(IntValue, DecimalHalf, PiRep);
                    Res.BasicUIntDivOp(-ExtraRep);
                    return Res;
                }
        #else
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                {
                    FlaggedDec Res = FlaggedDec(IntValue, 0, PiRep);
                    Res.BasicUIntDivOp(DecimalHalf);
                    return Res;
                }
        #endif
                break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi:
                    return *this;//Add Conversion Code from MixedPi later
    #endif
                default:
                    break;
            }
            ExtraRep = PiRep;
            return *this;
        } //2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69995 * selfNum
        const
    #endif
    #pragma endregion Pi Conversion

    #pragma region E Conversion
    #if defined(AltNum_EnableERep)
        //2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69995 * selfNum
        void ConvertENumToNum()
        {
            if (IntValue >= 790015084 && DecimalHalf >= 351050349)//Exceeding Storage limit of NormalRep
            {
                throw "Conversion of e multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
                IntValue = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
            }
            else if (IntValue <= -790015084 && DecimalHalf >= 351050349)//Exceeding Storage limit of NormalRep
            {
                throw "Conversion of e multiplication into MediumDec format resulted in underflow(setting value to minimum MediumDec value)";
                IntValue = -2147483647; DecimalHalf = 999999999;//set value as minimum value(since not truely infinite just bit above storage range)
        }
            __int64 SRep;
            __int64 divRes;
            if (DecimalHalf == 0)
            {
                bool IsNegative = IntValue<0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = 2718281828;
                SRep *= IntValue;
                divRes = SRep / DecimalOverflowX;
                DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
                if (divRes == 0 && IsNegative)
                {
                    if (DecimalHalf == 0)
                        IntValue = 0;
                    else
                        IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-divRes;
                else
                    IntValue = (int)divRes;
            }
            else if (IntValue == 0)
            {
                SRep = 2718281828;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
            }
            else if (IntValue == NegativeRep)
            {
                SRep = 2718281828;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
                if (divRes == 0)
                    IntValue = NegativeRep;
                else
                    IntValue = (int)-divRes;
            }
            else
            {
                bool IsNegative = IntValue<0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= 2ll;//SRep holds __int64 version of X.Y * Z
                //X.Y *.V
                __int64 Temp03 = (__int64)IntValue * 718281828ll;//Temp03 holds __int64 version of X *.V
                __int64 Temp04 = (__int64)DecimalHalf * 718281828ll;
                Temp04 /= FlaggedDec::DecimalOverflow;
                //Temp04 holds __int64 version of .Y * .V
                __int64 IntegerRep = SRep + Temp03 + Temp04;
                __int64 IntHalf = IntegerRep / FlaggedDec::DecimalOverflow;
                IntegerRep -= IntHalf * (__int64)FlaggedDec::DecimalOverflow;
                DecimalHalf = (signed int)IntegerRep;
                if (IntHalf == 0 && IsNegative)
                {
                    IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-IntHalf;
                else
                    IntValue = (int)IntHalf;
            }
            ExtraRep = 0;
}

        #if defined(AltNum_EnableDecimaledEFractionals)
        //Convert from ENumByDiv into NumByDivisor representation
        void ConvertEByDivToNumByDiv();

        //Convert from ENumByDiv into normal type representation
        void ConvertEByDivToNorm()
        {
            __int64 SRep;
            __int64 divRes;
            if (DecimalHalf == 0)
            {
                bool IsNegative = IntValue<0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = 2718281828;
                SRep *= IntValue;
                divRes = SRep / FlaggedDec::DecimalOverflowX;
                DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
                if (divRes == 0 && IsNegative)
                {
                    if (DecimalHalf == 0)
                        IntValue = 0;
                    else
                        IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-divRes;
                else
                    IntValue = (int)divRes;
            }
            else if (IntValue == 0)
            {
                SRep = 2718281828;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
            }
            else if (IntValue == NegativeRep)
            {
                SRep = 2718281828;
                SRep *= DecimalHalf;
                divRes = SRep / 1000000000000000000;
                DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
                if (divRes == 0)
                    IntValue = NegativeRep;
                else
                    IntValue = (int)-divRes;
            }
            else
            {
                bool IsNegative = IntValue<0;
                if (IsNegative)
                    IntValue *= -1;
                SRep = DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= 2ll;//SRep holds __int64 version of X.Y * Z
                //X.Y *.V
                __int64 Temp03 = (__int64)IntValue * 718281828ll;//Temp03 holds __int64 version of X *.V
                __int64 Temp04 = (__int64)DecimalHalf * 718281828ll;
                Temp04 /= FlaggedDec::DecimalOverflow;
                //Temp04 holds __int64 version of .Y * .V
                __int64 IntegerRep = SRep + Temp03 + Temp04;
                __int64 IntHalf = IntegerRep / FlaggedDec::DecimalOverflow;
                IntegerRep -= IntHalf * (__int64)FlaggedDec::DecimalOverflow;
                DecimalHalf = (signed int)IntegerRep;
                if (IntHalf == 0 && IsNegative)
                {
                    IntValue = NegativeRep;
                }
                else if (IsNegative)
                    IntValue = (int)-IntHalf;
                else
                    IntValue = (int)IntHalf;
            }
}
        #else
        //Convert from EFractional representation into normal representation
        void ConvertEFracToNorm()
        {
            int divisor = DecimalHalf;
            DecimalHalf = 0;
            ExtraRep = 0;
            bool IsNegative = IntValue < 0;
            if (IsNegative)
                IntValue *= -1;
            __int64 SRep = 2718281828ll;
            SRep *= IntValue;
            __int64 divRes = SRep / FlaggedDec::DecimalOverflowX;
            DecimalHalf = (int)(SRep - FlaggedDec::DecimalOverflowX * divRes);
            if (divRes == 0 && IsNegative)
            {
                if (DecimalHalf == 0)
                    IntValue = 0;
                else
                    IntValue = FlaggedDec::NegativeRep;
            }
            else if (IsNegative)
                IntValue = (int)-divRes;
            else
                IntValue = (int)divRes;
            BasicIntDivOp(divisor);
        }
        #endif
        void ConvertToERep(RepType repType)
        {
            switch (repType)
            {
                case RepType::ENum:
                    return;
                    break;
    #if defined(AltNum_EnableAlternativeRepFractionals)
        #if defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
                {
                    BasicUIntDivOp(-ExtraRep);
                }
        #else
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                {
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicUIntDivOp(divisor);
                    ExtraRep = ERep;
                }
        #endif
                break;
    #endif
    #if defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
                    return;//Add Conversion Code from MixedPi later
    #endif
                default:
                    break;
            }
            ExtraRep = ERep;
        }

        FlaggedDec ConvertAsERep(RepType repType)
        {
            switch (repType)
            {
                case RepType::ENum:
                    return *this;
                    break;
    #if defined(AltNum_EnableAlternativeRepFractionals)
        #if defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
                {
                    FlaggedDec Res = FlaggedDec(IntValue, DecimalHalf, ERep);
                    Res.BasicUIntDivOp(-ExtraRep);
                    return Res;
                }
        #else
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                {
                    FlaggedDec Res = FlaggedDec(IntValue, 0, ERep);
                    Res.BasicUIntDivOp(DecimalHalf);
                    return Res;

                }
        #endif
                break;
    #endif
    #if defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
                    return *this;//Add Conversion Code from MixedPi later
    #endif
                default:
                    break;
            }
            ExtraRep = ERep;
            return *this;
        } const
    #endif
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion
#if defined(AltNum_EnableApproachingDivided)
        void ConvertFromApproachingMidLeftToNorm()
        {//0.9..9/2 = 0.49..9
            DecimalHalf = (DecimalOverflow / ExtraRep) - 1;
            ExtraRep = 0;
        }

    #if !defined(AltNum_DisableApproachingTop)
        void ConvertFromApproachingMidRightToNorm()
        {
            DecimalHalf = (DecimalOverflow / ExtraRep) + 1;
            ExtraRep = 0;
        }
    #endif

    #if defined(AltNum_EnableApproachingI)
        void ConvertFromApproachingIMidLeftToINum()
        {
            DecimalHalf = (DecimalOverflow / ExtraRep) - 1;
            ExtraRep = IRep;
        }

        #if !defined(AltNum_DisableApproachingTop)
        void ConvertFromApproachingIMidRightToINum()
        {
            DecimalHalf = (DecimalOverflow / ExtraRep) + 1;
            ExtraRep = IRep;
        }
        #endif
    #endif
#endif

        //Switch based version of ConvertToNormType(use ConvertAsNormType instead to return converted value without modifying base value)
        void ConvertToNormType(RepType repType)
        {
            switch (repType)
            {
            case RepType::NormalType:
                break;
#if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
            {
                BasicIntDivOp(ExtraRep);
                ExtraRep = 0;
            }
            break;
#endif
#if defined(AltNum_EnableInfinityRep)
            case RepType::PositiveInfinity:
                IntValue = 2147483647; DecimalHalf = 999999999; ExtraRep = 0;
                break;
            case RepType::NegativeInfinity:
                IntValue = -2147483647; DecimalHalf = 999999999; ExtraRep = 0;
                break;
#endif
#if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
                DecimalHalf = 1;
                break;
    #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTop:
                DecimalHalf = 999999999;
                break;
    #endif
    #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                ConvertFromApproachingMidLeftToNorm(); break;
        #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingMidRight:
                ConvertFromApproachingMidRightToNorm(); break;
        #endif
    #endif
#endif
#if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                ConvertPiToNum(); break;
    #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                ConvertPiPowerToNum(); break;
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                ConvertPiByDivToNorm(); break;
    #elif defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                ConvertPiFracToNorm(); break;
    #endif
#endif
    #if defined(AltNum_EnableERep)
            case RepType::ENum:
                ConvertENumToNum(); break;
#if defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv:
                ConvertEByDivToNorm(); break;
#elif defined(AltNum_EnableEFractional)
            case RepType::EFractional://IntValue/DecimalHalf*e Representation
                ConvertEFracToNorm(); break;
#endif
#endif

#if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
            {
                FlaggedDec Res = IntValue < 0 ? FlaggedDec(DecimalHalf, 0) : FlaggedDec(DecimalHalf, 0);
                Res /= ExtraRep;
                if (IntValue != 0 && IntValue != NegativeRep)
                    Res += IntValue;
                IntValue = Res.IntValue;
                DecimalHalf = Res.DecimalHalf;
                ExtraRep = 0;
            }
            break;
#endif
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
    #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
    #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
    #endif
            {
                if (IntValue == 0 && DecimalHalf != 0)
                    ExtraRep = 0;
                else
                    throw "Can't convert imaginary number into real number unless is zero.";
            }
            break;
#endif
#if defined(AltNum_EnableApproachingI)
            case RepType::ApproachingImaginaryBottom:
    #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingImaginaryTop:
    #endif
    #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingImaginaryMidLeft:
        #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingImaginaryMidRight:
        #endif
    #endif
                throw "Can't convert imaginary number into real number unless is zero.";
                break;
#endif
#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
                throw "Can't convert imaginary number into real number unless is zero.";
                break;
#endif
            default:
                throw "Conversion to normal number not supported yet?";
                break;
            }
        } const

        //Switch based return of value as normal type representation
        FlaggedDec ConvertAsNormType(RepType repType)
        {
            FlaggedDec Res = *this;
            Res.ConvertToNormType(repType);
            return Res;
        } const

        //Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }
        
        //Returns value as normal type representation
        FlaggedDec ConvertAsNormTypeV2()
        {
            FlaggedDec Res = *this;
            Res.ConvertToNormTypeV2();
            return Res;
        }
    #if defined(AltNum_EnableImaginaryNum)
        void ConvertIRepToNormal(RepType repType)
        {//Assuming not zero(should not reach needing to convert the representation if RValue is zero)
            switch (repType)
            {
                case RepType::INum:
                {
                    break;
                }
                #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
                {
                    int Divisor = -ExtraRep;
                    BasicUnsignedDivOp(Divisor);
                    break;
                }
                #elif defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                {
                    int Divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicIntDivOp(Divisor);
                    break;
                }
                #endif
	#if defined(AltNum_EnableApproachingI)
				case RepType::ApproachingImaginaryBottom:
					DecimalHalf = 1;
					break;
		#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryTop:
					DecimalHalf = 999999999;
					break;
		#endif
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingImaginaryMidLeft:
					ConvertFromApproachingIMidLeftToNorm(); break;
			#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryMidRight:
					ConvertFromApproachingIMidRightToNorm(); break;
			#endif
		#endif
	#endif
                #ifdef AltNum_EnableComplexNumbers
                case RepType::ComplexIRep:
                {
                    throw "Conversion from complex number to real number not supported yet.";
                    break;
                }
				#endif
                default:
                    throw "Conversion not supported.";
                    break;
            }
        } const
        
        void ConvertToNormalIRep(RepType repType)
        {//Assuming not zero(should not reach needing to convert the representation)
            ConvertIRepToNormal(repType);
            ExtraRep = IRep;
        } const
        
        FlaggedDec ConvertAsNormalIRep(RepType repType)
        {
            FlaggedDec Res = *this;
            Res.ConvertToNormalIRep(repType);
            return Res;
        } const
        
        void ConvertToNormalIRepV2()
        {
            RepType repType = GetRepType();
            ConvertToNormalIRep(repType);
        }

        void ConvertIRepToNormalV2()
        {
            RepType repType = GetRepType();
            ConvertIRepToNormal(repType);
        }

        FlaggedDec ConvertIRepAsNormalV2()
        {
            FlaggedDec Res = *this;
            RepType repType = GetRepType();
            Res.ConvertIRepToNormal(repType);
            return Res;
        }
        
        FlaggedDec ConvertAsNormalIRepV2()
        {
            FlaggedDec Res = *this;
            Res.ConvertToNormalIRepV2();
            return Res;
        }

    #endif
    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators
    //Use strong-ordering in cases were non-real numbers don't need to be compared with real numbers(excluding infinity which is just counted as higher limit on decimalHalf)

    //Spaceship operator for comparing FlaggedDec to integer number
    auto operator<=>(const int& that) const
    {
        FlaggedDec LValue = *this;
        RepType LRep = LValue.GetRepType();
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
		switch (LRep)
		{
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(RValue/(ExtraRep*-1))*i Representation
			#endif
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
		#endif
		#if defined(AltNum_EnableImaginaryInfinity)
            case RepType::PositiveImaginaryInfinity:
            case RepType::NegativeImaginaryInfinity:
		#endif
                //throw "Can't compare imaginary number with real number";
                return std::partial_ordering::unordered;
                break;
	#endif
	#if defined(AltNum_EnableInfinityRep)
            case RepType::PositiveInfinity:
                LValue.IntValue = 2147483647;
                LValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeInfinity:
                LValue.IntValue = -2147483648;
                LValue.DecimalHalf = 2147483647;
                break;
	#endif
            default:
#endif
                LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
                break;
		}
#endif
#if !defined(AltNum_EnableImaginaryNum)&&!defined(AltNum_EnableNaN)&&!defined(AltNum_EnableUndefinedButInRange)&&!defined(AltNum_EnableNilRep)
        if (std::strong_ordering IntHalfCmp = LValue.IntValue <=> that; IntHalfCmp != 0)
            return IntHalfCmp;
        return (std::strong_ordering)LValue.DecimalHalf <=> 0;
#else
        if (std::partial_ordering IntHalfCmp = LValue.IntValue <=> that; IntHalfCmp != 0)
            return IntHalfCmp;
        return (std::partial_ordering)LValue.DecimalHalf <=> 0;
#endif
    }

    //Spaceship operator for comparing FlaggedDec with FlaggedDec
    auto operator<=>(const FlaggedDec& that) const
    {
        FlaggedDec LValue = *this;
        FlaggedDec RValue = that;
        RepType LRep = LValue.GetRepType();
        RepType RRep = RValue.GetRepType();
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
		switch (LRep)
		{
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(RValue/(ExtraRep*-1))*i Representation
			#endif
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
		#endif
                LValue.ConvertToNormalIRep(LRep);
                break;
            //Don't convert infinity into real number
		#if defined(AltNum_EnableImaginaryInfinity)
            case RepType::PositiveImaginaryInfinity:
                LValue.IntValue = 2147483647;
                LValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeImaginaryInfinity:
                LValue.IntValue = -2147483648;
                LValue.DecimalHalf = 2147483647;
                break;
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
            case RepType::PositiveInfinity:
                LValue.IntValue = 2147483647;
                LValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeInfinity:
                LValue.IntValue = -2147483648;
                LValue.DecimalHalf = 2147483647;
                break;
	#endif
            default:
#endif
                LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
                break;
		}
#endif
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
		switch (RRep)
		{
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(RValue/(ExtraRep*-1))*i Representation
			#endif
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
		#endif
                RValue.ConvertToNormalIRep(RRep);
                break;
		#if defined(AltNum_EnableImaginaryInfinity)
            case RepType::PositiveImaginaryInfinity:
                RValue.IntValue = 2147483647;
                RValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeImaginaryInfinity:
                RValue.IntValue = -2147483648;
                RValue.DecimalHalf = 2147483647;
                break;
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
            case RepType::PositiveInfinity:
                RValue.IntValue = 2147483647;
                RValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeInfinity:
                RValue.IntValue = -2147483648;
                RValue.DecimalHalf = 2147483647;
                break;
	#endif
            default:
#endif
                RValue.ConvertToNormType(RRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
                break;
		}
#endif

#if defined(AltNum_EnableImaginaryNum)
        if (LValue.ExtraRep != RValue.ExtraRep)
        {
                //throw "Can't compare imaginary number with real number";
                return std::partial_ordering::unordered;
        }
#endif
#if !defined(AltNum_EnableImaginaryNum)//&&!defined(AltNum_EnableNaN)&&!defined(AltNum_EnableUndefinedButInRange)&&!defined(AltNum_EnableNilRep)
        if (std::strong_ordering IntHalfCmp = LValue.IntValue <=> RValue.IntValue; IntHalfCmp != 0)
            return IntHalfCmp;
        std::strong_ordering DecimalHalfCmp = LValue.DecimalHalf <=> RValue.DecimalHalf;
        return DecimalHalfCmp;
#else
        if (std::partial_ordering IntHalfCmp = LValue.IntValue <=> RValue.IntValue; IntHalfCmp != 0)
            return IntHalfCmp;
        std::partial_ordering DecimalHalfCmp = LValue.DecimalHalf <=> RValue.DecimalHalf;
        return DecimalHalfCmp;
#endif
    }

    /*
    bool operator==(const int& that) const
    {
        FlaggedDec LValue = *this;
        RepType LRep = LValue.GetRepType();
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
		switch (LRep)
		{
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(RValue/(ExtraRep*-1))*i Representation
			#endif
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
		#endif
		#if defined(AltNum_EnableImaginaryInfinity)
            case RepType::PositiveImaginaryInfinity:
            case RepType::NegativeImaginaryInfinity:
		#endif
                return false;
                break;
	#endif
	#if defined(AltNum_EnableInfinityRep)
            case RepType::PositiveInfinity:
                LValue.IntValue = 2147483647;
                LValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeInfinity:
                LValue.IntValue = -2147483648;
                LValue.DecimalHalf = 2147483647;
                break;
	#endif
            default:
#endif
                LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
                break;
		}
#endif
        if (LValue.IntValue!=that)
            return false;
        if (LValue.DecimalHalf!=0)
            return false;
        if (LValue.ExtraRep!=0)
            return false;
        return true;
    }

    bool operator==(const FlaggedDec& that) const
    {
        FlaggedDec LValue = *this;
        RepType LRep = LValue.GetRepType();
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
		switch (LRep)
		{
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(RValue/(ExtraRep*-1))*i Representation
			#endif
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
		#endif
		#if defined(AltNum_EnableImaginaryInfinity)
            case RepType::PositiveImaginaryInfinity:
            case RepType::NegativeImaginaryInfinity:
		#endif
                return false;
                break;
	#endif
	#if defined(AltNum_EnableInfinityRep)
            case RepType::PositiveInfinity:
                LValue.IntValue = 2147483647;
                LValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeInfinity:
                LValue.IntValue = -2147483648;
                LValue.DecimalHalf = 2147483647;
                break;
	#endif
            default:
#endif
                LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
                break;
		}
#endif
        FlaggedDec RValue = that;
        RepType RRep = RValue.GetRepType();
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)//||defined(AltNum_EnableNaN)||defined(AltNum_EnableNil)||defined(AltNum_EnableUndefinedButInRange)
		switch (RRep)
		{
	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(RValue/(ExtraRep*-1))*i Representation
			#endif
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
		#endif
                RValue.ConvertToNormalIRep(RRep);
                break;
		#if defined(AltNum_EnableImaginaryInfinity)
            case RepType::PositiveImaginaryInfinity:
                RValue.IntValue = 2147483647;
                RValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeImaginaryInfinity:
                RValue.IntValue = -2147483648;
                RValue.DecimalHalf = 2147483647;
                break;
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
            case RepType::PositiveInfinity:
                RValue.IntValue = 2147483647;
                RValue.DecimalHalf = 2147483647;
                break;
            case RepType::NegativeInfinity:
                RValue.IntValue = -2147483648;
                RValue.DecimalHalf = 2147483647;
                break;
	#endif
            default:
#endif
                RValue.ConvertToNormType(RRep);
#if !defined(AltNum_EnableImaginaryNum)&&!defined(AltNum_EnableNaN)&&!defined(AltNum_EnableUndefinedButInRange)&&!defined(AltNum_EnableNilRep)
                break;
		}
#endif
        if (LValue.IntValue!=RValue.IntValue)
            return false;
        if (LValue.DecimalHalf!=RValue.DecimalHalf)
            return false;
        if (LValue.ExtraRep!=RValue.ExtraRep)
            return false;
        return true;
    }
    */
    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
#if !defined(AltNum_StoreBasicFunctionsInBase)
protected:

        template<IntegerType IntType=int>
        bool PartialIntDivOp(const IntType& rValue)
        {
            bool ResIsNegative = IntValue < 0;
            signed _int64 SelfRes;
            signed _int64 Res;
            signed _int64 IntHalfRes;
            signed _int64 DecimalRes;
            if (DecimalHalf == 0)
            {
                if (ResIsNegative)
                {
                    SelfRes = NegDecimalOverflowX * IntValue;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue;
                }
            }
            else
            {
                if (ResIsNegative)
                {
                    if(IntValue==NegativeRep)
                        SelfRes = DecimalHalf;
                    else
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                }
            }
            if(rValue<0)
            {
                ResIsNegative = !ResIsNegative;
                IntType invertedValue = -rValue;
                Res = SelfRes / invertedValue;
            #if defined(AltNum_OutputTruncatedTrailingDigits)
                TruncatedDigits = SelfRes - invertedValue * IntHalfRes;
            #endif
                IntHalfRes = Res/DecimalOverflowX;
                DecimalRes = Res - DecimalOverflowX * IntHalfRes;
            }
            else
            {
                Res = SelfRes / rValue;
            #if defined(AltNum_OutputTruncatedTrailingDigits)
                TruncatedDigits = SelfRes - rValue * IntHalfRes;
            #endif
                IntHalfRes = Res/DecimalOverflowX;
                DecimalRes = Res - DecimalOverflowX * IntHalfRes;
            }
            if(ResIsNegative)
            {
                IntValue = IntHalfRes==0? NegativeRep: (int)(-IntHalfRes);
                DecimalHalf = (int) DecimalRes;
            }
            else
            {
                IntValue = (int)IntHalfRes;
                DecimalHalf = DecimalRes;
            }
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }

        template<IntegerType IntType=int>
        bool PartialUIntDivOp(const IntType& rValue)
        {
            bool ResIsNegative = IntValue < 0;
            signed _int64 SelfRes;
            signed _int64 Res;
            signed _int64 IntHalfRes;
            signed _int64 DecimalRes;
        #if defined(AltNum_OutputTruncatedTrailingDigits)
            signed _int64 TruncatedDigits; 
        #endif
            if (DecimalHalf == 0)
            {
                if (ResIsNegative)
                {
                    SelfRes = NegDecimalOverflowX * IntValue;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue;
                }
            }
            else
            {
                if (ResIsNegative)
                {
                    if(IntValue==NegativeRep)
                        SelfRes = DecimalHalf;
                    else
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                }
                else
                {
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                }
            }
            //One has SelfRes of 1000000000
            //1000000000/2 = 500000000
            Res = SelfRes / rValue;
        #if defined(AltNum_OutputTruncatedTrailingDigits)
            TruncatedDigits = SelfRes - rValue * IntHalfRes;
        #endif
            IntHalfRes = SelfRes/DecimalOverflowX;
            DecimalRes = SelfRes - DecimalOverflowX * IntHalfRes;
            if(ResIsNegative)
            {
                IntValue = IntHalfRes==0? NegativeRep: (int)(-IntHalfRes);
                DecimalHalf = (int) DecimalRes;
            }
            else
            {
                IntValue = (int)IntHalfRes;
                DecimalHalf = DecimalRes;
            }
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }
#endif

protected:
#if !defined(AltNum_StoreBasicFunctionsInBase)
        template<IntegerType IntType=int>
        FlaggedDec& BasicUIntDivOp(const IntType& rValue)
        {
            if (rValue == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return *this;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IsZero())
                return *this;
            if(PartialUIntDivOp(rValue))
                DecimalHalf = 1;//Prevent Dividing into nothing
            return *this;
        }

        template<IntegerType IntType=int>
        void BasicIntDivOp(const IntType& rValue)
        {
            if (rValue == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IsZero())
                return;
            if (rValue < 0)
            {
                int invertedValue = -(int)rValue;
                SwapNegativeStatus();
                if(PartialUIntDivOp(invertedValue))
                    DecimalHalf = 1;//Prevent Dividing into nothing
            }
            else if(PartialUIntDivOp(rValue))
                DecimalHalf = 1;//Prevent Dividing into nothing
        }
#endif

public:

        void BasicInt32DivOp(signed int& rValue) { BasicIntDivOp(rValue); }

        void BasicInt32DivOpV2(signed int& rValue) { BasicUIntDivOp(rValue); }

        void BasicInt64DivOp(signed long long& rValue) { BasicIntDivOp(rValue); }

        /// <summary>
        /// Division Operation Between FlaggedDec and unsigned Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=int>
        static FlaggedDec BasicDivideByUIntOp(FlaggedDec& lValue, const IntType& rValue) { return lValue.BasicUIntDivOp(rValue); }

        /// <summary>
        /// Division Operation Between FlaggedDec and unsigned Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=int>
        static FlaggedDec BasicDivideByIntOp(FlaggedDec& lValue, const IntType& rValue) { return lValue.BasicIntDivOp(rValue); }

        /// <summary>
        /// Division Operation Between FlaggedDec and Integer Value that ignores special representation status
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=int>
        static FlaggedDec BasicDivideByUInt(FlaggedDec lValue, const IntType& rValue) { return lValue.BasicUIntDivOp(rValue); }

        /// <summary>
        /// Division Operation Between FlaggedDec and unsigned Integer Value that ignores special representation status
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=int>
        static FlaggedDec BasicDivideByInt(FlaggedDec lValue, const IntType& rValue) { return lValue.BasicIntDivOp(rValue); }

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep Integer Multiplication Operations
#if !defined(AltNum_StoreBasicFunctionsInBase)
protected:
        /// <summary>
        /// Partial Multiplication Operation Between FlaggedDec and Integer Value
        /// (Modifies owner object) 
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType=int>
        void PartialIntMultOp(const IntType& rValue)
        {
            if (DecimalHalf == 0)
            {
                IntValue *= rValue;
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    if (IntValue == NegativeRep) { IntValue = 0; }
                    else { IntValue *= -1; }
                }
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                if(rValue<0)
                {
                    SelfIsNegative = !SelfIsNegative;
                    SRep *= -(int)rValue;
                }
                else
                    SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = SelfIsNegative ? NegativeRep : 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
        }

        /// <summary>
        /// Partial Multiplication Operation Between FlaggedDec and Integer Value
        /// Applies after making sure rValue is positive
        /// (Modifies owner object)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType=int>
        void PartialIntMultOpV2(const IntType& rValue)
        {
            if (DecimalHalf == 0)
            {
                IntValue *= rValue;
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    if (IntValue == NegativeRep) { IntValue = 0; }
                    else { IntValue *= -1; }
                }
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue + DecimalHalf;
                SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = SelfIsNegative ? NegativeRep : 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
        }
#endif

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and Integer Value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType>
        FlaggedDec BasicIntMultOp(const IntType& rValue)
        {
            if (IntValue == 0 && DecimalHalf == 0)
                return *this;
            if (rValue == 0)
                SetAsZero();
            else
                PartialIntMultOp(rValue);
            return *this;
        }

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and unsigned Integer Value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType>
        FlaggedDec BasicUIntMultOp(const IntType& rValue)
        {
            if (IntValue == 0 && DecimalHalf == 0)
                return *this;
            if (rValue == 0)
                SetAsZero();
            else
                PartialIntMultOpV2(rValue);
            return *this;
        }

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and Integer Value
        /// (Avoids modifying owner object by copying lValue)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType = int>
        FlaggedDec BasicMultiplyByInt(const IntType& rValue) { FlaggedDec self = *this; self.BasicIntMultOp(rValue); return self; }

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and unsigned Integer Value
        /// (Avoids modifying owner object by copying lValue)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType = unsigned int>
        void BasicMultiplyByUInt(const IntType& rValue) { FlaggedDec self = *this; self.BasicUIntMultOp(rValue); return self; }

public:
        /// <summary>
        /// Multiplication Operation Between FlaggedDec and Integer Value
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        FlaggedDec BasicInt32MultOp(const int& rValue) { return BasicIntMultOp(rValue); }

        //BasicInt32MultOp but instead using unsigned version of function
        FlaggedDec BasicInt32MultOpV2(const int& rValue) { return BasicUIntMultOp(rValue); }

        FlaggedDec BasicInt64MultOp(const signed __int64& rValue) { return BasicIntMultOp(rValue); }

        FlaggedDec BasicUInt32MultOp(const unsigned int& rValue) { return BasicUIntMultOp(rValue); }
        FlaggedDec BasicUInt64MultOp(const unsigned int& rValue) { return BasicUIntMultOp(rValue); }

        static FlaggedDec BasicMultiplyByInt32Op(FlaggedDec& lValue, const int& rValue) { return lValue.BasicIntMultOp(rValue); }

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and Integer Value
        /// (Avoids modifying owner object by copying lValue)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        FlaggedDec BasicMultiplyByInt32(const int& rValue) { FlaggedDec self = *this; self.BasicIntMultOp(rValue); return self; }

        //BasicMultiplyByInt32 but instead using unsigned version of function
        FlaggedDec BasicMultiplyByInt32V2(const int& rValue) { FlaggedDec self = *this; self.BasicUIntMultOp(rValue); return self; }

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations
#if !defined(AltNum_StoreBasicFunctionsInBase)
protected:
        /// <summary>
        /// Addition Operation that skips negative zero(for when decimal half is empty)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>void</returns>
        template<IntegerType IntType=int>
        void NRepSkippingIntAddOp(const IntType& rValue)
        {
            if (rValue == 0)
                return;
            if (IntValue == 0)
                IntValue = (int)rValue;
            else
                IntHalfAddition(rValue);
            return;
        }
#endif
public:

        /// <summary>
        /// Basic Addition Operation between FlaggedDec and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>FlaggedDec&</returns>
        template<IntegerType IntType=int>
        FlaggedDec& BasicIntAddOp(const IntType& rValue)
        {
            if(DecimalHalf==0)
                NRepSkippingIntAddOp(rValue);
            else
            {
                bool WasNegative = IntValue < 0;
                IntHalfAddition(rValue);
                //If flips to other side of negative, invert the decimals
                if ((IntValue<0)!=WasNegative)
                    DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
            }
            return *this;
        }

        /// <summary>
        /// Addition Operation Between FlaggedDec and  Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=int>
        static FlaggedDec BasicAddByIntOp(FlaggedDec& lValue, const IntType& rValue) { return lValue.BasicIntAddOp(rValue); }

        /// <summary>
        /// Subtraction Operation Between FlaggedDec and unsigned Integer Value that ignores special representation status
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=int>
        static FlaggedDec BasicAddByInt(FlaggedDec lValue, const IntType& rValue) { return lValue.BasicIntAddOp(rValue); }
    #pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations
#if !defined(AltNum_StoreBasicFunctionsInBase)
protected:
        /// <summary>
        /// Subtraction Operation that skips negative zero(for when decimal half is empty)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>void</returns>
        template<IntegerType IntType=int>
        void NRepSkippingIntSubOp(const IntType& rValue)
        {
            if (rValue == 0)
                return;
            if (IntValue == 0)
                IntValue = -(int)rValue;
            else
                IntHalfSubtraction(rValue);
            return;
        }

#else
protected:
        //Derived Functions test to check if properly automatically impli
        template<IntegerType IntType=int>
        FlaggedDec& BasicIntSubOverrideOp(const IntType& rValue)
        {
            BasicIntSubBaseOp(rValue);
        }
#endif
public:
        /// <summary>
        /// Basic Subtraction Operation between FlaggedDec and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec&</returns>
        template<IntegerType IntType=int>
        FlaggedDec BasicIntSubOp(const IntType& rValue)
        {
            if (DecimalHalf == 0)
                NRepSkippingIntSubOp(rValue);
            else
            {
                bool WasNegative = IntValue < 0;
                IntHalfSubtraction(rValue);
                //If flips to other side of negative, invert the decimals
                if ((IntValue<0)!=WasNegative)
                    DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
            }
            return *this;
        }

        /// <summary>
        /// Subtraction Operation Between FlaggedDec and unsigned Integer Value that ignores special representation status
        /// (Modifies lValue during operation) 
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=int>
        static FlaggedDec BasicSubtractByIntOp(FlaggedDec& lValue, const IntType& rValue) { return lValue.BasicIntSubOp(rValue); }

        /// <summary>
        /// Subtraction Operation Between FlaggedDec and unsigned Integer Value that ignores special representation status
        /// </summary>
        /// <param name="lValue">The left side value.</param>
        /// <param name="rValue">The right side value.</param>
        template<IntegerType IntType=int>
        static FlaggedDec BasicSubtractByInt(FlaggedDec lValue, const IntType& rValue) { return lValue.BasicIntSubOp(rValue); }

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep Integer Bitwise Operations

    #pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations
    #if defined(AltNum_EnableMixedFractional)
        //Assumes NormalRep + Normal MixedFraction operation
        void BasicMixedFracAddOp(FlaggedDec& rValue)
        {
            if(DecimalHalf==0)//Avoid needing to convert if Leftside is not decimal format representation
            {
                if(IntValue<0)
                {
                    if(rValue.IntValue==NegativeRep)
                    {
                        DecimalHalf = rValue.DecimalHalf;
                        ExtraRep = rValue.ExtraRep;
                    }
                    else if(rValue.IntValue<0)
                    {
                        IntValue += rValue.IntValue;
                        DecimalHalf = rValue.DecimalHalf;
                        ExtraRep = rValue.ExtraRep;
                    }
                    else//(rValue.IntValue>0)
                    {
                        if(rValue.IntValue>-IntValue)//check for flipping of sign
                        {
                            IntValue += rValue.IntValue - 1;
                            DecimalHalf = rValue.ExtraRep - rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue += rValue.IntValue;
                            DecimalHalf = rValue.ExtraRep - rValue.DecimalHalf;
                        }
                        ExtraRep = rValue.ExtraRep;
                    }
                }
                else//(IntValue>0)
                {
                    if(rValue.IntValue==NegativeRep)
                    {
                        DecimalHalf = rValue.ExtraRep - rValue.DecimalHalf;
                        ExtraRep = rValue.ExtraRep;
                    }
                    else if(rValue.IntValue<0)
                    {
                        IntValue += rValue.IntValue;
                        if(-rValue.IntValue>IntValue)//check for flipping of sign
                        {
                            IntValue += rValue.IntValue;
                            if(IntValue==-1)
                                IntValue = NegativeRep;
                            else
                                ++IntValue;
                        }
                        DecimalHalf = rValue.ExtraRep - rValue.DecimalHalf;
                        ExtraRep = rValue.ExtraRep;
                    }
                    else//(rValue.IntValue>0)
                    {
                        IntValue += rValue.IntValue;
                        DecimalHalf = rValue.DecimalHalf;
                        ExtraRep = rValue.ExtraRep;
                    }
                }     
            }
            else
            {
                FlaggedDec RightSideNum = FlaggedDec(rValue.IntValue==0?-rValue.DecimalHalf:rValue.IntValue*rValue.ExtraRep - rValue.DecimalHalf);
                BasicIntMultOp(rValue.ExtraRep);
                BasicAddOp(RightSideNum);//self += RightSideNum;
                if(DecimalHalf==0)
                {
                    if(IntValue!=0)//Set as Zero if both are zero
                    {
                        DecimalHalf = -DecimalHalf;
                        ExtraRep = rValue.ExtraRep;
                    }
                }
                else
                {
                    if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
                        DecimalHalf = -DecimalHalf;
                    ExtraRep = rValue.ExtraRep;
                }
            }
        }
        
    #if defined(AltNum_EnableMixedPiFractional)
        void BasicMixedPiFracAddOp(FlaggedDec& rValue)
    #elif defined(AltNum_EnableMixedEFractional)
        void BasicMixedEFracAddOp(FlaggedDec& rValue)
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
        {
            FlaggedDec RightSideNum = FlaggedDec(rValue.IntValue==0?-rValue.DecimalHalf:(rValue.IntValue*-rValue.ExtraRep) - rValue.DecimalHalf);
        #if defined(AltNum_EnableMixedPiFractional)
            RightSideNum *= PiNum;
        #else
            RightSideNum *= ENum;
        #endif
            BasicIntMultOp(-rValue.ExtraRep);
            BasicAddOp(RightSideNum);
            if(DecimalHalf==0)
            {
                if(IntValue!=0)//Set as Zero if both are zero
                {
                    DecimalHalf = -DecimalHalf;
                    ExtraRep = -rValue.ExtraRep;
                }
            }
            else
            {
                if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
                    DecimalHalf = -DecimalHalf;
                ExtraRep = -rValue.ExtraRep;
            }
        }
    #endif
        
        //Assumes NormalRep - Normal MixedFraction operation
        void BasicMixedFracSubOp(FlaggedDec& rValue)
        {
            FlaggedDec RightSideNum = FlaggedDec(rValue.IntValue==0?-rValue.DecimalHalf:rValue.IntValue*rValue.ExtraRep - rValue.DecimalHalf);
            BasicMultOp(rValue.ExtraRep);
            BasicSubOp(RightSideNum);
            if(DecimalHalf==0)
            {
                if(IntValue!=0)//Set as Zero if both are zero
                {
                    DecimalHalf = -DecimalHalf;
                    ExtraRep = rValue.ExtraRep;
                }
            }
            else
            {
                if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
                    DecimalHalf = -DecimalHalf;
                ExtraRep = rValue.ExtraRep;
            }
        }
        
    #if defined(AltNum_EnableMixedPiFractional)
        void BasicMixedPiFracSubOp(FlaggedDec& rValue)
    #elif defined(AltNum_EnableMixedEFractional)
        void BasicMixedEFracSubOp(FlaggedDec& rValue)
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
        {
            FlaggedDec RightSideNum = FlaggedDec(rValue.IntValue==0?-rValue.DecimalHalf:(rValue.IntValue*-rValue.ExtraRep) - rValue.DecimalHalf);
        #if defined(AltNum_EnableMixedPiFractional)
            RightSideNum *= PiNum;
        #else
            RightSideNum *= ENum;
        #endif
            BasicMultOp(-rValue.ExtraRep);
            BasicSubOp(RightSideNum);
            if(DecimalHalf==0)
            {
                if(IntValue!=0)//Set as Zero if both are zero
                {
                    DecimalHalf = -DecimalHalf;
                    ExtraRep = -rValue.ExtraRep;
                }
            }
            else
            {
                if(IntValue!=0&&IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
                    DecimalHalf = -DecimalHalf;
                ExtraRep = -rValue.ExtraRep;
            }
        }
    #endif

		//NormalType,PiNum, PiPower, or ENum multiplied with Mixed Fraction representation
		static FlaggedDec MixedFracRtRMult_WithNormal(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			FlaggedDec LeftSide = LValue*RValue.IntValue;
			FlaggedDec RightSide = LValue/RValue.ExtraRep;
			RightSide *= -RValue.DecimalHalf;
			return LeftSide+RightSide;
		}

		//NumByDiv multiplied with Mixed Fraction representation
		static FlaggedDec MixedFracRtRMult_WithNumByDiv(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			//FlaggedDec(LValue.IntValue, LValue.DecimalHalf)/LValue.ExtraRep * RValue.IntValue+(-RValue.DecimalHalf)/RValue.ExtraRep
			//= RValue.IntValue*(FlaggedDec(LValue.IntValue, LValue.DecimalHalf)/LValue.ExtraRep) + (FlaggedDec(LValue.IntValue, LValue.DecimalHalf)/LValue.ExtraRep)((-RValue.DecimalHalf)/RValue.ExtraRep)
			//= (RValue.IntValue*FlaggedDec(LValue.IntValue, LValue.DecimalHalf)/LValue.ExtraRep) + (FlaggedDec(LValue.IntValue, LValue.DecimalHalf)*-RValue.DecimalHalf)/(LValue.ExtraRep*RValue.ExtraRep)
			//= ((RValue.IntValue*FlaggedDec(LValue.IntValue, LValue.DecimalHalf))+((FlaggedDec(LValue.IntValue, LValue.DecimalHalf)*-RValue.DecimalHalf)/RValue.ExtraRep))/LValue.ExtraRep
			FlaggedDec NumSide = FlaggedDec(LValue.IntValue, LValue.DecimalHalf)*-RValue.DecimalHalf;
			NumSide.BasicDivOp(RValue.ExtraRep);//Avoid converting to NumByDiv because need to combine with (RValue.IntValue*FlaggedDec(LValue.IntValue, LValue.DecimalHalf))/LValue.ExtraRep
			FlaggedDec additionalVal = FlaggedDec(LValue.IntValue, LValue.DecimalHalf)*RValue.IntValue;
			NumSide.BasicAddOp(additionalVal);
			NumSide.ExtraRep = LValue.ExtraRep;
			return NumSide;
		}
		
	#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
		//NumByDiv multiplied with Mixed Pi or E Fraction representation
		static FlaggedDec MixedAltFracRtRMult_WithNumByDiv(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			FlaggedDec NumSide = FlaggedDec(LValue.IntValue, LValue.DecimalHalf)*-RValue.DecimalHalf;
			NumSide.BasicDivOp(-RValue.ExtraRep);//Avoid converting to NumByDiv because need to combine with (RValue.IntValue*FlaggedDec(LValue.IntValue, LValue.DecimalHalf))/LValue.ExtraRep
			FlaggedDec additionalVal = FlaggedDec(LValue.IntValue, LValue.DecimalHalf)*RValue.IntValue;
			NumSide.BasicAddOp(additionalVal);
		#if (defined(AltNum_EnableMixedPiFractional)&&defined(AltNum_EnableDecimaledPiFractionals))||(defined(AltNum_EnableMixedEFractional)&&defined(AltNum_EnableDecimaledEFractionals))
			NumSide.ExtraRep = -LValue.ExtraRep;
			return NumSide;
		#else
			NumSide.BasicDivOp(LValue.ExtraRep);
			#if defined(AltNum_EnableMixedPiFractional)
			NumSide.ExtraRep = PiRep;
			#else
			NumSide.ExtraRep = ERep;
			#endif
			return NumSide;
		#endif
		}
	#endif

	#if defined(AltNum_EnableMixedPiFractional)
		//NormalType or ENum multiplied with Mixed Pi Fraction representation
		static FlaggedDec MixedPiFracRtRMult_WithNormal(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			FlaggedDec LeftSide = LValue*RValue.IntValue;
			FlaggedDec RightSide = LValue/-RValue.ExtraRep;
			RightSide *= -RValue.DecimalHalf;
			FlaggedDec Res = LeftSide+RightSide;
			Res.BasicMultOp(FlaggedDec::PiNum);
			return Res;
		}

		//PiNum multiplied with Mixed Pi Fraction representation
		static FlaggedDec MixedPiFracRtRMult_WithPiNum(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			FlaggedDec LeftSide = LValue*RValue.IntValue;
			FlaggedDec RightSide = LValue/-RValue.ExtraRep;
			RightSide *= -RValue.DecimalHalf;
			FlaggedDec Res = LeftSide+RightSide;
			#if defined(AltNum_EnablePiPowers)//Become Value*Pi^2
				Res.ExtraRep = -1;
			#else
				Res.BasicDivOp(FlaggedDec::PiNum);
			#endif
			return Res;
		}

		//PiPower multiplied with Mixed Pi Fraction representation
		static FlaggedDec MixedPiFracRtRMult_WithPiPower(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			FlaggedDec LeftSide = LValue*RValue.IntValue;
			FlaggedDec RightSide = LValue/-RValue.ExtraRep;
			RightSide *= -RValue.DecimalHalf;
			FlaggedDec Res = LeftSide+RightSide;
			++Res.ExtraRep;
			return Res;
		}

		static FlaggedDec MixedPiFracRtRMult_WithOther(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			FlaggedDec LeftSide = LValue*RValue.IntValue;
			FlaggedDec RightSide = LValue/-RValue.ExtraRep;
			RightSide *= -RValue.DecimalHalf;
			FlaggedDec Res = LeftSide+RightSide;
			Res.UnsignedMultOp(FlaggedDec::PiNum);
			return Res;
		}
	#elif defined(AltNum_EnableMixedEFractional)

		//PiNum, PiPower, ENum, or NormalType multiplied with Mixed Fraction representation
		static FlaggedDec MixedEFracRtRMult_WithNormal(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			FlaggedDec LeftSide = self*Value.IntValue;
			FlaggedDec RightSide = self/-Value.ExtraRep;
			RightSide *= -Value.DecimalHalf;
			FlaggedDec Res = LeftSide+RightSide;
			Res.BasicMultOp(FlaggedDec::ENum);
			return Res;
		}

		static FlaggedDec MixedEFracRtRMult_WithOther(const FlaggedDec& LValue, const FlaggedDec& RValue)
		{
			FlaggedDec LeftSide = self*Value.IntValue;
			FlaggedDec RightSide = self/-Value.ExtraRep;
			RightSide *= -Value.DecimalHalf;
			FlaggedDec Res = LeftSide+RightSide;
			Res.UnsignedMultOp(FlaggedDec::ENum);
			return Res;
		}
	#endif
#endif
    #pragma endregion Mixed Fraction Operations

    #pragma region NormalRep AltNum Division Operations
#if !defined(AltNum_StoreBasicFunctionsInBase)
protected:
        /// <summary>
        /// Basic Division Operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param
        bool PartialDivOp(const FlaggedDec& rValue)
        {
            bool ResIsPositive = true;
            signed _int64 SelfRes;
            signed _int64 ValueRes;
            if(IntValue<0)
            {
                if (IntValue == NegativeRep)
                    SelfRes = DecimalHalf;
                else
                    SelfRes = NegDecimalOverflowX*IntValue + DecimalHalf;
                if (rValue < 0)
                {
                    if (rValue.IntValue == NegativeRep)
                        ValueRes = rValue.DecimalHalf;
                    else
                        ValueRes =  NegDecimalOverflowX* rValue.IntValue + rValue.DecimalHalf;
                }
                else
                {
                    ResIsPositive = false;
                    ValueRes = DecimalOverflowX * rValue.IntValue +rValue.DecimalHalf;
                }
            }
            else
            {
                SelfRes = DecimalOverflowX* IntValue+DecimalHalf;
                if(rValue<0)
                {
                    ResIsPositive = false;
                    ValueRes = rValue.IntValue==NegativeRep ? DecimalHalf: NegDecimalOverflowX*IntValue +rValue.DecimalHalf;
                }
                else
                    ValueRes = DecimalOverflowX* rValue.IntValue +rValue.DecimalHalf;
            }

            signed _int64 IntHalfRes = SelfRes / ValueRes;
            signed _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
            IntValue = IntHalfRes==0&&ResIsPositive==false?NegativeRep:IntHalfRes;
            DecimalHalf = DecimalRes;
            if(IntHalfRes==0&&DecimalRes==0)
                return true;
            else
                return false;
        }

        bool UnsignedPartialDivOp(const FlaggedDec& rValue)
        {
            bool ResIsPositive = true;
            signed _int64 SelfRes;
            signed _int64 ValueRes;
            if (IntValue < 0)
            {
                if (IntValue == NegativeRep)
                    SelfRes = DecimalHalf;
                else
                    SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
                ResIsPositive = false;
                ValueRes = DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
            }
            else
            {

                SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                ValueRes = DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
            }

            signed _int64 IntHalfRes = SelfRes / ValueRes;
            signed _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
            IntValue = IntHalfRes == 0 && ResIsPositive == false ? NegativeRep : IntHalfRes;
            DecimalHalf = DecimalRes;
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }
#else
        /// <summary>
        /// Basic Division Operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param
        bool PartialDivOp(const FlaggedDec& rValue)
        {
            PartialDivBaseOp(rValue);
        }

        /// <summary>
        /// Basic Division Operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param
        bool PartialUnsignedDivOp(const FlaggedDec& rValue)
        {
            PartialUnsignedDivBaseOp(rValue);
        }
#endif
public:

        /// <summary>
        /// Basic Division Operation(without checking for special representation variations or zero)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        FlaggedDec& BasicDivOp(const FlaggedDec& rValue)
        {
            if (PartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf = 1;
            return *this;
        }

        /// <summary>
        /// Basic Division Operation(without checking for special representation variations or zero)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        FlaggedDec& BasicUnsignedDivOp(const FlaggedDec& rValue)
        {
            if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf = 1;
            return *this;
        }

        //Performs division operation ignoring non-normal representation with right side FlaggedDec
        FlaggedDec BasicDiv(const FlaggedDec& rValue)
        { FlaggedDec self = *this; self.BasicUnsignedDivOp(rValue); return self; }

        //Performs division operation (without checking negative) ignoring non-normal representation with right side FlaggedDec
        FlaggedDec BasicUnsignedDiv(const FlaggedDec& rValue) 
        { FlaggedDec self = *this; self.BasicUnsignedDivOp(rValue);  return self; }

        FlaggedDec BasicDivision(FlaggedDec self, const FlaggedDec& rValue)
        {
            if (self.PartialDivOp(rValue))//Prevent Dividing into nothing
                self.DecimalHalf = 1;
            return self;
        }

        FlaggedDec BasicUnsignedDivision(FlaggedDec self, const FlaggedDec& rValue)
        {
            if (self.UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                self.DecimalHalf = 1;
            return self;
        }

    #pragma endregion NormalRep AltNum Division Operations

    #pragma region NormalRep AltNum Multiplication Operations
public:
#if !defined(AltNum_StoreBasicFunctionsInBase)
        /// <summary>
        /// Basic Multiplication Operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        FlaggedDec& BasicMultOp(const FlaggedDec& rValue)
        {
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                {
                    IntValue *= rValue.IntValue;
                }
                else
                {
                    __int64 rRep;
                    bool ResIsNegative = rValue < 0;
                    if(ResIsNegative)
                    {
                        rRep = rValue.IntValue == NegativeRep ? rValue.DecimalHalf : DecimalOverflowX * -rValue.IntValue + rValue.DecimalHalf;
                        if(IntValue<0)
                        {
                            ResIsNegative = false;
                            rRep *= IntHalfAsAbs();
                        }
                        else
                        {
                            ResIsNegative = true;
                            rRep *= IntValue;
                        }
                    }
                    else
                    {
                        rRep = rValue.IntValue == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
                        if(IntValue<0)
                        {
                            ResIsNegative = true;
                            rRep *= IntHalfAsAbs();
                        }
                        else
                        {
                            ResIsNegative = false;
                            rRep *= IntValue;
                        }
                    }
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)ResIsNegative ? OverflowVal * -1 : OverflowVal;
                        DecimalHalf = (signed int)rRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                        if(DecimalHalf==0)
                        {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
    #else
                            DecimalHalf = 1;
    #endif
                        }
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
                if (IntValue == 0)
                {
                    __int64 SRep = (__int64)DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= FlaggedDec::DecimalOverflowX;
                    if (rValue.IntValue == 0)
                    {
                        DecimalHalf = (signed int)SRep;
                        if(SelfIsNegative&&DecimalHalf!=0)
                            IntValue = NegativeRep;
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {
                        DecimalHalf = (signed int)SRep;
                        if(SelfIsNegative==false&&DecimalHalf!=0)
                            IntValue = NegativeRep;
                    }
                    else
                    {
                        SRep += (__int64)DecimalHalf * rValue.IntValue;
                        if (SRep >= FlaggedDec::DecimalOverflowX)
                        {
                            __int64 OverflowVal = SRep / FlaggedDec::DecimalOverflowX;
                            SRep -= OverflowVal * FlaggedDec::DecimalOverflowX;
                            IntValue = SelfIsNegative?-(int)OverflowVal:OverflowVal;
                            DecimalHalf = (signed int)SRep;
                            return *this;
                        }
                        else
                        {
                            DecimalHalf = (signed int)SRep;
                            if(DecimalHalf==0)
                            {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                                IntValue = 0;
                                if(ExtraRep!=0)
                                    ExtraRep = 0;
                                return *this;
    #else
                                DecimalHalf = 1;
    #endif
                            }
                            if(SelfIsNegative)
                                IntValue = NegativeRep;
                            return *this;
                        }
                    }
                }
                else if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = FlaggedDec::DecimalOverflowX * IntValue + DecimalHalf;
                    if (rValue < 0)
                    {
                        SRep *= -rValue.IntValue;
                        SelfIsNegative = !SelfIsNegative;
                    }
                    else
                        SRep *= rValue.IntValue;
                    if (SRep >= FlaggedDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / FlaggedDec::DecimalOverflowX;
                        SRep -= OverflowVal * FlaggedDec::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
                        return *this;
                    }
                    else
                    {
                            DecimalHalf = (signed int)SRep;
                            if(DecimalHalf==0)
                            {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                                IntValue = 0;
                                if(ExtraRep!=0)
                                    ExtraRep = 0;
                                return *this;
    #else
                                DecimalHalf = 1;
    #endif
                            }
                            IntValue = SelfIsNegative ? FlaggedDec::NegativeRep : 0;
                            return *this;
                    }
                }
                else if (rValue.IntValue == 0)
                {
                    __int64 SRep = FlaggedDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= FlaggedDec::DecimalOverflowX;
                    if (SRep >= FlaggedDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / FlaggedDec::DecimalOverflowX;
                        SRep -= OverflowVal * FlaggedDec::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
    #else
                            DecimalHalf = 1;
    #endif
                        }
                        IntValue = SelfIsNegative ? FlaggedDec::NegativeRep : 0;
                        return *this;
                    }
                }
                else if (rValue.IntValue == NegativeRep)
                {
                    SelfIsNegative = !SelfIsNegative;
                    __int64 SRep = FlaggedDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= FlaggedDec::DecimalOverflowX;
                    if (SRep >= FlaggedDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / FlaggedDec::DecimalOverflowX;
                        SRep -= OverflowVal * FlaggedDec::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = SelfIsNegative ? FlaggedDec::NegativeRep : 0;
                        return *this;
                    }
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : FlaggedDec::DecimalOverflowX * IntValue + DecimalHalf;
                    //X.Y *.V
                    if(rValue<0)
                    {
                        SRep *= -rValue.IntValue;
                        SelfIsNegative = !SelfIsNegative;
                    }
                    else
                        SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= FlaggedDec::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / FlaggedDec::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)FlaggedDec::DecimalOverflow;
                    if (IntHalf == 0)
                        IntValue = (signed int)SelfIsNegative ? FlaggedDec::NegativeRep : 0;
                    else
                        IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf;
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0&&ExtraRep!=0)
                ExtraRep = 0;
#else
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
#endif
            return *this;
        }

        /// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned FlaggedDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        FlaggedDec& BasicUnsignedMultOp(const FlaggedDec& rValue)
        {
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                {
                    IntValue *= rValue.IntValue;
                }
                else
                {
                    __int64 rRep = rValue.IntValue == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
                    bool ResIsNegative = IntValue<0;
                    if(ResIsNegative)
                    {
                        ResIsNegative = false;
                        rRep *= IntHalfAsAbs();
                    }
                    else
                    {
                        ResIsNegative = true;
                        rRep *= IntValue;
                    }
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)ResIsNegative ? OverflowVal * -1 : OverflowVal;
                        DecimalHalf = (signed int)rRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                        if(DecimalHalf==0)
                        {
#if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
#else
                            DecimalHalf = 1;
#endif
                        }
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
            else if (IntValue == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= FlaggedDec::DecimalOverflowX;
                if (rValue.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf = 1;
                #else
                    if (DecimalHalf == 0&&ExtraRep!=0)
                        ExtraRep = 0;
                #endif
                    return *this;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntValue;
                    if (SRep >= FlaggedDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / FlaggedDec::DecimalOverflowX;
                        SRep -= OverflowVal * FlaggedDec::DecimalOverflowX;
                        IntValue = OverflowVal;
                        DecimalHalf = (signed int)SRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0&&ExtraRep!=0)
                            ExtraRep = 0;
                #else
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
                #endif
                        return *this;
                    }
                }
            }
            else if (IntValue == FlaggedDec::NegativeRep)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= FlaggedDec::DecimalOverflowX;
                if (rValue.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                    if(DecimalHalf==0)
                    {
                        IntValue = 0;
                        if(ExtraRep!=0)
                            ExtraRep = 0;
                    }
                    return *this;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntValue;
                    if (SRep >= FlaggedDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / FlaggedDec::DecimalOverflowX;
                        SRep -= OverflowVal * FlaggedDec::DecimalOverflowX;
                        IntValue = -OverflowVal;
                        DecimalHalf = (signed int)SRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        return *this;
                    }
                }
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = FlaggedDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;
                    if (SRep >= FlaggedDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / FlaggedDec::DecimalOverflowX;
                        SRep -= OverflowVal * FlaggedDec::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        IntValue = SelfIsNegative ? FlaggedDec::NegativeRep : 0;
                    }
                    return *this;
                }
                else if (rValue.IntValue == 0)
                {
                    __int64 SRep = FlaggedDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= FlaggedDec::DecimalOverflowX;
                    if (SRep >= FlaggedDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / FlaggedDec::DecimalOverflowX;
                        SRep -= OverflowVal * FlaggedDec::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            IntValue = 0;
                            if(ExtraRep!=0)
                                ExtraRep = 0;
                            return *this;
                    #else
                            DecimalHalf = 1;
                    #endif
                        }
                        IntValue = SelfIsNegative ? FlaggedDec::NegativeRep : 0;
                    }
                    return *this;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : FlaggedDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= FlaggedDec::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / FlaggedDec::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)FlaggedDec::DecimalOverflow;
                    if (IntHalf == 0)
                        IntValue = (signed int)SelfIsNegative ? FlaggedDec::NegativeRep : 0;
                    else
                        IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf;
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0&&ExtraRep!=0)
                ExtraRep = 0;
    #else
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
    #endif
            return *this;
        }
#else
        /// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned FlaggedDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        FlaggedDec& BasicMultOp(const FlaggedDec& rValue)
        {
            FlaggedDec& Res =  BasicMultBaseOp(rValue);
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0&&ExtraRep!=0)
                ExtraRep = 0;
    #endif
            return Res;
        }

        /// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned FlaggedDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        FlaggedDec& BasicUnsignedMultOp(const FlaggedDec& rValue)
        {
            FlaggedDec& Res =  BasicUnsignedMultBaseOp(rValue);
    #if defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0&&ExtraRep!=0)
                ExtraRep = 0;
    #endif
            return Res;
        }
#endif

        FlaggedDec BasicMult(const FlaggedDec& rValue) { FlaggedDec self = *this; self.BasicUnsignedDivOp(rValue); return self; }

        FlaggedDec BasicUnsignedMult(const FlaggedDec& rValue) { FlaggedDec self = *this; self.BasicUnsignedMultOp(rValue); return self; }

        FlaggedDec BasicMultiplication(FlaggedDec self, const FlaggedDec& rValue)
        {
            return self.BasicMultOp(rValue);
        }

        FlaggedDec BasicUnsignedMultiplication(FlaggedDec self, const FlaggedDec& rValue)
        {
            return self.BasicUnsignedMultOp(rValue);
        }

    #pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep AltNum Addition Operations
#if !defined(AltNum_StoreBasicFunctionsInBase)
        /// <summary>
        /// Basic Addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicAddOp(const FlaggedDec& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfAddition(rValue.IntValue);
            if (rValue.DecimalHalf != 0)
            {
                if (rValue.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if ((IntValue<0)!=WasNegative)
                DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
        }

        /// <summary>
        /// Basic Addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedAddOp(const FlaggedDec& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfAddition(rValue.IntValue);
            if (rValue.DecimalHalf != 0)
            {
                if (WasNegative)
                {
                    DecimalHalf -= rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; ++IntValue; }
                    else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; --IntValue; }
                }
                else
                {
                    DecimalHalf += rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; --IntValue; }
                    else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; ++IntValue; }
                }
            }
            //If flips to other side of negative, invert the decimals
            if ((IntValue<0)!=WasNegative)
                DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
        }
#else
        /// <summary>
        /// Basic Addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicAddOp(const FlaggedDec& rValue) { BasicAddBaseOp(rValue); }

        /// <summary>
        /// Basic Addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedAddOp(const FlaggedDec& rValue) { BasicUnsignedAddBaseOp(rValue); }
#endif

    #pragma endregion NormalRep AltNum Addition Operations

    #pragma region NormalRep AltNum Subtraction Operations
#if !defined(AltNum_StoreBasicFunctionsInBase)
        /// <summary>
        /// Basic Subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicSubOp(const FlaggedDec& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfSubtraction(rValue.IntValue);
            //Now deal with the decimal section
            if(rValue.DecimalHalf!=0)
            {
                if (rValue.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; --IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; --IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if ((IntValue<0)!=WasNegative)
                DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
        }

        /// <summary>
        /// Basic Subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedSubOp(const FlaggedDec& rValue)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfSubtraction(rValue.IntValue);
            //Now deal with the decimal section
            if(rValue.DecimalHalf!=0)
            {
                if (WasNegative)//-4.5 - 5.6
                {
                    DecimalHalf += rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; ++IntValue; }
                    else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; --IntValue; }
                }
                else//0.995 - 1
                {
                    DecimalHalf -= rValue.DecimalHalf;
                    if (DecimalHalf < 0) { DecimalHalf += FlaggedDec::DecimalOverflow; --IntValue; }
                    else if (DecimalHalf >= FlaggedDec::DecimalOverflow) { DecimalHalf -= FlaggedDec::DecimalOverflow; ++IntValue; }
                }
            }
            //If flips to other side of negative, invert the decimals
            if ((IntValue<0)!=WasNegative)
                DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
        }
#else
        /// <summary>
        /// Basic Subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicSubOp(const FlaggedDec& rValue) { BasicSubBaseOp(rValue); }

        /// <summary>
        /// Basic Subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedSubOp(const FlaggedDec& rValue) { BasicUnsignedSubBaseOp(rValue); }
#endif
    #pragma endregion NormalRep AltNum Subtraction Operations

    #pragma region Other Division Operations

        template<IntegerType IntType=int>
        void CatchAllIntDivision(const IntType& rValue, RepType LRep)
        {
            ConvertToNormType(LRep);
            BasicIntDivOp(rValue);
        }

        template<IntegerType IntType=int>
        void CatchAllUIntDivision(const IntType& rValue, RepType LRep)
        {
            ConvertToNormType(LRep);
            BasicUIntDivOp(rValue);
        }

        template<IntegerType IntType=int>
        void CatchAllIntDivisionV2(const IntType& rValue)
        {
            ConvertToNormTypeV2();
            BasicIntDivOp(rValue);
        }

protected:
        /// <summary>
        /// Division Operation Between FlaggedDec and Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>FlaggedDec&</returns>
        template<IntegerType IntType = int>
        FlaggedDec& IntDivOp(const IntType& rightSideValue)
        {
            if (rightSideValue == 1)
                return *this;
            if (rightSideValue == 0)
            {
                #if defined(AltNum_EnableInfinityRep)
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
                #else
                throw "Target rightSideValue can not be divided by zero";
                #endif
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
                {
                    #if defined(AltNum_EnableFractionals)
                    if (rightSideValue < 0)
                    {
                        ExtraRep = -(int)rightSideValue;
                        SwapNegativeStatus();
                    }
                    else
                        ExtraRep = rightSideValue;
                    #else
                    BasicIntDivOp(rightSideValue);
                    #endif
                }
                break;
    #if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = ExtraRep;
                    if (rightSideValue < 0)
                    {
                        int invertedR = -(int)rightSideValue;
                        SwapNegativeStatus();
                        result *= invertedR;
                        if (ExtraRep == result / invertedR)//checking for overflow
                            ExtraRep = result;
                        else
                            BasicUIntDivOp(invertedR);
                    }
                    else
                    {
                        result *= rightSideValue;
                        if (ExtraRep == result / rightSideValue)//checking for overflow
                            ExtraRep = result;
                        else
                            BasicUIntDivOp(rightSideValue);
                    }
                }
                break;
    #endif
    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = DecimalHalf;
                    if (rightSideValue < 0)
                    {
                        int invertedR = -(int)rightSideValue;
                        SwapNegativeStatus();
                        result *= invertedR;
                        if (DecimalHalf == result / invertedR)//checking for overflow
                            DecimalHalf = result;
                        else
                            CatchAllUIntDivision(invertedR, LRep);
                    }
                    else
                    {
                        result *= rightSideValue;
                        if (DecimalHalf == result / rightSideValue)//checking for overflow
                            DecimalHalf = result;
                        else
                            CatchAllUIntDivision(rightSideValue, LRep);
                    }
                }
                break;
    #endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
                #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
                #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv:
                #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
                #endif
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = ExtraRep;
                    if (rightSideValue < 0)
                    {
                        int invertedR = -(int)rightSideValue;
                        SwapNegativeStatus();
                        result *= invertedR;
                        if (ExtraRep == result / invertedR)//checking for overflow
                            ExtraRep = result;
                        else
                            BasicUIntDivOp(invertedR);
                    }
                    else
                    {
                        result *= rightSideValue;
                        if (ExtraRep == result / rightSideValue)//checking for overflow
                            ExtraRep = result;
                        else
                            BasicUIntDivOp(rightSideValue);
                    }
                }
                break;
    #endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    if(rightSideValue<0)
                        IntValue *= -1;
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                {
                    #if defined(AltNum_EnableDecimaledIFractionals)
                    if (rightSideValue < 0)
                    {
                        ExtraRep = rightSideValue;
                        SwapNegativeStatus();
                    }
                    else
                        ExtraRep = -rightSideValue;
                    #elif defined(AltNum_EnableIFractional)
                    if (DecimalHalf == 0)
                    {
                        if (rightSideValue < 0)
                        {
                            DecimalHalf = -(int)rightSideValue;
                            SwapNegativeStatus();
                        }
                        else
                            DecimalHalf = rightSideValue;
                        ExtraRep = IByDivisorRep;
                    }
                    else
                        BasicUIntDivOp(rightSideValue);
                    #else
                    BasicUIntDivOp(rightSideValue);
                    #endif
                }
                break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                {
                    if (IsAtZeroInt())
                    {
                        if (rightSideValue < 0)
                            SwapNegativeStatus();
                        return *this;
                    }
                    ConvertToNormType(LRep);
                    BasicIntDivOp(rightSideValue);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
                #endif
        #endif
                {
                    ConvertToNormType(LRep);
                    BasicUIntDivOp(rightSideValue);
                }
                break;
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    if(rightSideValue<0)
                        IntValue *= -1;
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
            #endif
        #endif
                {
                    ConvertToNormalIRep(LRep);
                    BasicIntDivOp(rightSideValue);
                }
                break;
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become NumByDiv
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            if (IntValue == NegativeRep)
                                IntValue = divRes;
                            else
                                IntValue = -divRes;
                            DecimalHalf = 0;
                        }
                        else
                        {
                            if (IntValue == NegativeRep)
                                IntValue = DecimalHalf;
                            else
                                IntValue = -DecimalHalf;
                            DecimalHalf = 0;
                            ExtraRep *= rightSideValue;
                        }
                        return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Implement code here";
                        }
                        else
                        {
                            throw "ToDo: Implement code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Implement code here";
                    }
                }
                break;
        #if defined(AltNum_EnableAlternativeMixedFrac)
            #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            #endif
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become Fractional
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/-3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Implement code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
                            throw "ToDo: Implement code here";
                            #endif
                        }
                        else
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Implement code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
                            throw "ToDo: Implement code here";
                            #endif
                        }
                        //return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Implement code here";
                        }
                        else
                        {
                            throw "ToDo: Implement code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Implement code here";
                    }
                }
                break;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif
                default:
                    BasicIntDivOp(rightSideValue);
                    break;
            }
            return *this;
        }

        /// <summary>
        /// Division Operation Between FlaggedDec and unsigned Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>FlaggedDec&</returns>
        template<IntegerType IntType=int>
        FlaggedDec& UIntDivOp(const IntType& rValue)
        {
            if (rValue == 1)
                return *this;
            if (rValue == 0)
            {
                #if defined(AltNum_EnableInfinityRep)
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
                #else
                throw "Target rValue can not be divided by zero";
                #endif
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
                {
                    #if defined(AltNum_EnableFractionals)
                    ExtraRep = rValue;
                    #else
                    BasicUIntDivOp(rValue);
                    #endif
                }
                break;
    #if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                {
                    int result = ExtraRep * rValue;
                    if (ExtraRep == result / rValue)//checking for overflow
                        ExtraRep = result;
                    else
                        BasicUIntDivOp(rValue);
                }
                break;
    #endif
    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = ExtraRep * rValue;
                    if (DecimalHalf == result / rValue)//checking for overflow
                        DecimalHalf = result;
                    else
                        CatchAllIntDivision(rValue, LRep);
                }
                break;
    #endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
                #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
                    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv:
                    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
                    #endif
                {
                    int result = ExtraRep * rValue;
                    if (ExtraRep == result / rValue)//checking for overflow
                        ExtraRep = result;
                    else
                        BasicUIntDivOp(rValue);
                }
                break;
    #endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                {
                    #if defined(AltNum_EnableDecimaledIFractionals)
                    ExtraRep = -rValue;
                    #elif defined(AltNum_EnableIFractional)
                    if (DecimalHalf == 0)
                    {
                        DecimalHalf = rValue;
                        ExtraRep = IByDivisorRep;
                    }
                    else
                        BasicUIntDivOp(rValue);
                    #else
                    BasicUIntDivOp(rValue);
                    #endif
                }
                break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                {
                    if (IsAtZeroInt())
                        return *this;
                    ConvertToNormType(LRep);
                    BasicUIntDivOp(rValue);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                #endif
        #endif
                {
                    ConvertToNormType(LRep);
                    BasicUIntDivOp(rValue);
                }
                break;
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
            #endif
        #endif
                {
                    ConvertToNormalIRep(LRep);
                    BasicIntDivOp(rValue);
                }
                break;
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become NumByDiv
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            if (IntValue == NegativeRep)
                                IntValue = divRes;
                            else
                                IntValue = -divRes;
                            DecimalHalf = 0;
                        }
                        else
                        {
                            if (IntValue == NegativeRep)
                                IntValue = DecimalHalf;
                            else
                                IntValue = -DecimalHalf;
                            DecimalHalf = 0;
                            ExtraRep *= rValue;
                        }
                        return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Implement code here";
                        }
                        else
                        {
                            throw "ToDo: Implement code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Implement code here";
                    }
                }
                break;
        #if defined(AltNum_EnableAlternativeMixedFrac)
            #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            #endif
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become Fractional
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/-3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Implement code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
                            throw "ToDo: Implement code here";
                            #endif
                        }
                        else
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Implement code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Implement code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
                            throw "ToDo: Implement code here";
                            #endif
                        }
                        //return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Implement code here";
                        }
                        else
                        {
                            throw "ToDo: Implement code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Implement code here";
                    }
                }
                break;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif
                default:
                    BasicIntDivOp(rValue);
                    break;
            }
            return *this;
        }

        template<IntegerType IntType = int>
        FlaggedDec DivideByInt(const IntType& rValue) { FlaggedDec self = *this; return self.IntDivOp(rValue); }

        template<IntegerType IntType = int>
        FlaggedDec DivideByUInt(const IntType& rValue) { FlaggedDec self = *this; return self.UIntDivOp(rValue); }

        /// <summary>
        /// Division Operation Between FlaggedDec and Integer rValue.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType = int>
        static FlaggedDec& IntDivision(FlaggedDec self, const IntType& rValue) { return self.IntDivOp(rValue); }
public:
        FlaggedDec& Int32DivOp(const int& rValue) { return IntDivOp(rValue); }

        FlaggedDec DivideByInt32(const int& rValue) { FlaggedDec self = *this; return self.IntDivOp(rValue); }
        FlaggedDec DivideByUInt32(const int& rValue) { FlaggedDec self = *this; return self.UIntDivOp(rValue); }

        FlaggedDec DivideByInt32V2(const int& rValue) { FlaggedDec self = *this; return self.UIntDivOp(rValue); }

    #pragma endregion Other Division Operations

    #pragma region Other Multiplication Operations
public:

        template<IntegerType IntType=int>
        void CatchAllIntMultiplication(const IntType& rValue, RepType LRep)
        {
            ConvertToNormType(LRep);
            BasicIntMultOp(rValue);
        }

        template<IntegerType IntType=int>
        void CatchAllUIntMultiplication(const IntType& rValue, RepType LRep)
        {
            ConvertToNormType(LRep);
            BasicUIntMultOp(rValue);
        }

        template<IntegerType IntType=int>
        void CatchAllIntMultiplicationV2(const IntType& rValue)
        {
            ConvertToNormTypeV2();
            BasicIntMultOp(rValue);
        }

protected:
        /// <summary>
        /// Multiplication Operation Between FlaggedDec and Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>FlaggedDec&</returns>
        template<IntegerType IntType=int>
        FlaggedDec& IntMultOp(const IntType& rightSideValue)
        {
            if (IsZero()||rightSideValue==1)
                return *this;
            if (rightSideValue == 0)
            {
                SetAsZero(); return *this;
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
                    BasicIntMultOp(rightSideValue);
                    break;
    #if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                {//Reduce divisor if possible
                    int rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int divRes = ExtraRep / rValue;
                    if((ExtraRep - rValue * divRes)==0)
                        ExtraRep = divRes;
                    else
                        BasicUIntMultOp(rValue);
                }
                break;
    #endif
    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {
                    IntValue *= rightSideValue;
                    if(IntValue==0)
                    {
                        DecimalHalf = 0; ExtraRep = 0;
                    }
                }
                break;
    #endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
        #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv:
        #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
        #endif
                {
                    int rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int divRes = ExtraRep / rValue;
                    if((ExtraRep - rValue * divRes)==0)
                        ExtraRep = divRes;
                    else
                        BasicUIntMultOp(rValue);
                }
                break;
    #endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                    BasicIntMultOp(rightSideValue);
                    break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                    if(IntValue==0||IntValue==NegativeRep)
                    {
                        if(rightSideValue<0)
                            IntValue = NegativeRep;
                    }
                    else
                        CatchAllIntMultiplication(rightSideValue, LRep);
                    break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                {
                    int rValue = rightSideValue;
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (IntValue == NegativeRep)
                        IntValue = -rValue;
                    else if (IntValue == 0)
                        IntValue = rValue - 1;
                    else if (IntValue < 0)//-5.9..9 * 100
                        IntValue = (IntValue - 1) * rValue + 1;
                    else//5.9..9 * 100 = 599.9..9
                        IntValue = (IntValue + 1) * rValue - 1;
                }
                break;
                #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
                {
                    int rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if(IntValue==0)
                    {
                        //0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
                        int divRes = ExtraRep/rValue;
                        if((ExtraRep - rValue * divRes)==0)
                        {
                            if(divRes == 0)//Become 0.9..9
                            {
                                ExtraRep = 0;
                                DecimalHalf = ApproachingTopRep;
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else if(IntValue==NegativeRep)
                    {
                        //-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
                        int divRes = ExtraRep/rValue;
                        if((ExtraRep - rValue * divRes)==0)
                        {
                            if(divRes == 0)//Become 0.9..9
                            {
            #if !defined(AltNum_DisableApproachingTop)
                                ExtraRep = 0;
                                DecimalHalf = ApproachingTopRep;
            #else
                                DecimalHalf = 999999999;
            #endif
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else
                        CatchAllUIntMultiplication(rValue, LRep);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
                {
                    int rValue = rightSideValue;
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (IntValue == 0)
                    {
                        //0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
                        int divRes = ExtraRep / rightSideValue;
                        if ((ExtraRep - rightSideValue * divRes) == 0)
                        {
                            ExtraRep = divRes;
                            if (divRes == 0)
                            {
                                IntValue = 1;
                                DecimalHalf = ApproachingBottomRep;
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else if (IntValue == NegativeRep)
                    {
                        //-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
                        int divRes = ExtraRep / rightSideValue;
                        if ((ExtraRep - rightSideValue * divRes) == 0)
                        {
                            ExtraRep = divRes;
                            if (divRes == 0)
                            {
                                IntValue = -1;
                                DecimalHalf = ApproachingBottomRep;
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else
                        CatchAllUIntMultiplication(rightSideValue, LRep);
                }
                break;
                #endif
        #endif
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
                {
                    if (IntValue == 0 || IntValue == NegativeRep)
                    {
                        if (rightSideValue < 0)
                            IntValue = NegativeRep;
                    }
                    else
                        CatchAllIntMultiplication(rightSideValue, LRep);
                }
                break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
                {
                    int rValue = rightSideValue;
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (IntValue == NegativeRep)
                        IntValue = -rValue;
                    else if (IntValue == 0)
                    {
                        IntValue = rValue - 1;
                    }
                    else if (IntValue < 0)//-5.9..9 * 100
                        IntValue = (IntValue - 1) * rValue + 1;
                    else//5.9..9 * 100 = 599.9..9
                        IntValue = (IntValue + 1) * rValue - 1;
                }
                break;
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
                {
                    int rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if(IntValue==0)
                    {
                        //0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
                        int divRes = ExtraRep/rValue;
                        if((ExtraRep - rValue * divRes)==0)
                        {
                            if(divRes == 0)//Become 0.9..9
                            {
                                ExtraRep = 0;
                                DecimalHalf = ApproachingTopRep;
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else if(IntValue==NegativeRep)
                    {
                        //-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
                        int divRes = ExtraRep/rValue;
                        if((ExtraRep - rValue * divRes)==0)
                        {
                            if(divRes == 0)//Become 0.9..9
                            {
            #if !defined(AltNum_DisableApproachingTop)
                                ExtraRep = 0;
                                DecimalHalf = ApproachingTopRep;
            #else
                                DecimalHalf = 999999999;
            #endif
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else
                        CatchAllUIntMultiplication(rValue, LRep);
                }
                break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
                {
                    int rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if(IntValue==0)
                    {
                        //0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
                        int divRes = ExtraRep/rValue;
                        if((ExtraRep - rValue * divRes)==0)
                        {
                            ExtraRep = divRes;
                            if(divRes == 0)
                            {
                                IntValue = 1;
                                DecimalHalf = ApproachingBottomRep;
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else if(IntValue==NegativeRep)
                    {
                        //-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
                        int divRes = ExtraRep/rValue;
                        if((ExtraRep - rValue * divRes)==0)
                        {
                            ExtraRep = divRes;
                            if(divRes == 0)
                            {
                                IntValue = -1;
                                DecimalHalf = ApproachingBottomRep;
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else
                    {
                        ConvertToNormalIRep(LRep);
                        BasicIntMultOp(rValue);
                    }
                }
                break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
                    int rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    DecimalHalf *= rValue;
                    IntHalfMultiplication(rValue);
                    int divRes = DecimalHalf / -ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtraction(increment);
                        else
                            IntHalfAddition(increment);
                        DecimalHalf = DecimalHalf + increment;
                    }
                }
                break;
        #if defined(AltNum_EnableAlternativeMixedFrac)
            #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            #endif
                {
                    int rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    DecimalHalf *= rValue;
                    IntHalfMultiplication(rValue);
                    int divRes = DecimalHalf / ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtraction(increment);
                        else
                            IntHalfAddition(increment);
                        DecimalHalf = DecimalHalf - increment;
                    }
                    break;
                }
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif
                default:
                    CatchAllIntMultiplication(rightSideValue, LRep);
                    break;
            }
            return *this;
        }

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and unsigned Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>FlaggedDec&</returns>
        template<IntegerType IntType=int>
        FlaggedDec& UIntMultOp(const IntType& rightSideValue)
        {
            if (IsZero()||rightSideValue==1)
                return *this;
            if (rightSideValue == 0)
            {
                SetAsZero(); return *this;
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
                    BasicUIntMultOp(rightSideValue);
                    break;
    #if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                {//Reduce divisor if possible
                    int divRes = ExtraRep / rightSideValue;
                    if((ExtraRep - rightSideValue * divRes)==0)
                        ExtraRep = divRes;
                    else
                        BasicUIntMultOp(rightSideValue);
                }
                break;
    #endif
    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {
                    IntValue *= rightSideValue;
                    if(IntValue==0)
                    {
                        DecimalHalf = 0; ExtraRep = 0;
                    }
                }
                break;
    #endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
		#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv:
		#elif defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv:
		#elif defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv:
		#endif
                {
                    int divRes = ExtraRep / rightSideValue;
                    if((ExtraRep - rightSideValue * divRes)==0)
                        ExtraRep = divRes;
                    else
                        BasicUIntMultOp(rightSideValue);
                }
                break;
    #endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                    BasicUIntMultOp(rightSideValue);
                    break;
    #endif
    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
                    if(IntValue!=0&&IntValue!=NegativeRep)
                        CatchAllUIntMultiplication(rightSideValue, LRep);
                    break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                    if(IntValue==NegativeRep)
                        IntValue = -(int)rightSideValue;
                    else if(IntValue==0)
                        IntValue = (int)rightSideValue - 1;
                    else if(IntValue<0)//-5.9..9 * 100
                        IntValue = (IntValue-1)*(int)rightSideValue + 1;
                    else//5.9..9 * 100 = 599.9..9
                        IntValue = (IntValue+1)*(int)rightSideValue - 1;
                    break;
                #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
                {
                    if(IntValue==0)
                    {
                        //0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
                        int divRes = ExtraRep/rightSideValue;
                        if((ExtraRep - rightSideValue * divRes)==0)
                        {
                            if(divRes == 0)//Become 0.9..9
                            {
                                ExtraRep = 0;
                                DecimalHalf = ApproachingTopRep;
                            }
                            else
                            {
                            }
                        }
                    }
                    else if(IntValue==NegativeRep)
                    {
                        //-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
                        int divRes = ExtraRep/rightSideValue;
                        if((ExtraRep - rightSideValue * divRes)==0)
                        {
                            if(divRes == 0)//Become 0.9..9
                            {
            #if !defined(AltNum_DisableApproachingTop)
                                ExtraRep = 0;
                                DecimalHalf = ApproachingTopRep;
            #else
                                DecimalHalf = 999999999;
            #endif
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else
                        CatchAllUIntMultiplication(rightSideValue, LRep);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
                    if(IntValue==0)
                    {
                        //0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
                        int divRes = ExtraRep/rightSideValue;
                        if((ExtraRep - rightSideValue * divRes)==0)
                        {
                            ExtraRep = divRes;
                            if(divRes == 0)
                            {
                                IntValue = 1;
                                DecimalHalf = ApproachingBottomRep;
                            }
                        }
                    }
                    else if(IntValue==NegativeRep)
                    {
                        //-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
                        int divRes = ExtraRep/rightSideValue;
                        if((ExtraRep - rightSideValue * divRes)==0)
                        {
                            ExtraRep = divRes;
                            if(divRes == 0)
                            {
                                IntValue = -1;
                                DecimalHalf = ApproachingBottomRep;
                            }
                        }
                    }
                    else
                        CatchAllUIntMultiplication(rightSideValue, LRep);
                #endif
        #endif
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
                    if(IntValue==0||IntValue==NegativeRep)
                    {
                        if(rightSideValue<0)
                            IntValue = NegativeRep;
                    }
                    else
                        CatchAllUIntMultiplication(rightSideValue, LRep);
                    break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
                    if(IntValue==NegativeRep)
                        IntValue = -(int)rightSideValue;
                    else if(IntValue==0)
                        IntValue = (int)rightSideValue - 1;
                    else if(IntValue<0)//-5.9..9 * 100
                        IntValue = (IntValue-1)*(int)rightSideValue + 1;
                    else//5.9..9 * 100 = 599.9..9
                        IntValue = (IntValue+1)*(int)rightSideValue - 1;
                    break;
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
                {
                    if(IntValue==0)
                    {
                        //0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
                        int divRes = ExtraRep/rightSideValue;
                        if((ExtraRep - rightSideValue * divRes)==0)
                        {
                            if(divRes == 0)//Become 0.9..9
                            {
                                ExtraRep = 0;
                                DecimalHalf = ApproachingTopRep;
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else if(IntValue==NegativeRep)
                    {
                        //-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
                        int divRes = ExtraRep/rightSideValue;
                        if((ExtraRep - rightSideValue * divRes)==0)
                        {
                            if(divRes == 0)//Become 0.9..9
                            {
            #if !defined(AltNum_DisableApproachingTop)
                                ExtraRep = 0;
                                DecimalHalf = ApproachingTopRep;
            #else
                                DecimalHalf = 999999999;
            #endif
                            }
                            else
                                ExtraRep = divRes;
                        }
                    }
                    else
                        CatchAllUIntMultiplication(rightSideValue, LRep);
                }
                break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
                {
                    if(IntValue==0)
                    {
                        //0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
                        int divRes = ExtraRep/rightSideValue;
                        if((ExtraRep - rightSideValue * divRes)==0)
                        {
                            ExtraRep = divRes;
                            if(divRes == 0)
                            {
                                IntValue = 1;
                                DecimalHalf = ApproachingBottomRep;
                            }
                        }
                    }
                    else if(IntValue==NegativeRep)
                    {
                        //-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
                        int divRes = ExtraRep/rightSideValue;
                        if((ExtraRep - rightSideValue * divRes)==0)
                        {
                            ExtraRep = divRes;
                            if(divRes == 0)
                            {
                                IntValue = -1;
                                DecimalHalf = ApproachingBottomRep;
                            }
                        }
                    }
                    else
                    {
                        ConvertToNormalIRep(LRep);
                        BasicUIntMultOp(rightSideValue);
                    }
                }
                break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
                    DecimalHalf *= rightSideValue;
                    UIntHalfMultiplication(rightSideValue);
                    int divRes = DecimalHalf / -ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtraction(increment);
                        else
                            IntHalfAddition(increment);
                        DecimalHalf = DecimalHalf + increment;
                    }
                }
                break;
        #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
        #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
        #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
        #endif
        #if defined(AltNum_EnableAlternativeMixedFrac)
                {
                    DecimalHalf *= rightSideValue;
                    UIntHalfMultiplication(rightSideValue);
                    int divRes = DecimalHalf / ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtraction(increment);
                        else
                            IntHalfAddition(increment);
                        DecimalHalf = DecimalHalf - increment;
                    }
                }
                break;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif
                default:
                    CatchAllUIntMultiplication(rightSideValue, LRep);
                    break;
            }
            return *this;
        }

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and Integer rValue.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType = int>
        static FlaggedDec& IntMultiplication(FlaggedDec self, const IntType& rValue) { return self.IntMultOp(rValue); }

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and unsigned Integer rValue.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType = int>
        static FlaggedDec& UIntMultiplication(FlaggedDec self, const IntType& rValue) { return self.UIntMultOp(rValue); }

        template<IntegerType IntType = int>
        FlaggedDec MultiplyByInt(const IntType& rValue) { FlaggedDec self = *this; return self.IntMultOp(rValue); }

        template<IntegerType IntType = int>
        FlaggedDec MultiplyByUInt(const IntType& rValue) { FlaggedDec self = *this; return self.UIntMultOp(rValue); }

public:
        FlaggedDec& Int32MultOp(const int& rightSideValue) { return IntMultOp(rightSideValue); }

        FlaggedDec& Int32MultOpV2(const int& rightSideValue) { return UIntMultOp(rightSideValue); }

        FlaggedDec& Int64MultOp(const signed __int64& rightSideValue) { return IntMultOp(rightSideValue); }

        FlaggedDec& UInt32MultOp(const unsigned int& rightSideValue) { return UIntMultOp(rightSideValue); }

        /// <summary>
        /// Multiplication Operation Between FlaggedDec and Integer Value.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>FlaggedDec</returns>
        FlaggedDec MultiplyByInt32(const int& rValue) { FlaggedDec self = *this; return self.IntMultOp(rValue); }

        //MultiplyByInt32 but using unsigned version of function
        FlaggedDec MultiplyByInt32V2(const int& rValue) { FlaggedDec self = *this; return self.UIntMultOp(rValue); }

        FlaggedDec MultiplyByInt64(const signed __int64& rValue) { FlaggedDec self = *this; return self.IntMultOp(rValue); }

        FlaggedDec MultiplyByUInt32(const unsigned int& rValue) { FlaggedDec self = *this; return self.UIntMultOp(rValue); }

    #pragma endregion Other Multiplication Operations

    #pragma region Other Addition Operations
public:

        template<IntegerType IntType=int>
        void CatchAllIntAddition(const IntType& rValue, RepType LRep)
        {
            ConvertToNormType(LRep);
            BasicIntAddOp(rValue);
        }

        template<IntegerType IntType=int>
        void CatchAllIntAdditionV2(const IntType& rValue)
        {
            ConvertToNormTypeV2();
            BasicIntAddOp(rValue);
        }

protected:
        /// <summary>
        /// Addition FlaggedDec modifying operation with right side Integer rValue.
        /// (copy by parameter and pointer version)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>FlaggedDec&</returns>
        template<IntegerType IntType=int>
        FlaggedDec& IntAddOp(const IntType& rValue)
        {
            if (rValue == 0)
                return *this;
    #if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert FlaggedDec into complex number at moment";
                return *this;
            }
    #endif
    #if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
    #endif
    #if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
            else
    #endif
            if(ExtraRep!=0)//Don't convert if mixed fraction
                ConvertToNormTypeV2();
            bool WasNegative = IntValue < 0;
            IntHalfAddition(rValue);
            //If flips to other side of negative, invert the decimals
    #if defined(AltNum_EnableMixedFractional)
            if ((IntValue<0)!=WasNegative)
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
                    DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
            }
    #else
            if(WasNegative ^ (IntValue >= 0))
                DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
    #endif
            return *this;
        }

        /// <summary>
        /// Addition Operation Between FlaggedDec and Integer Value.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType = int>
        static FlaggedDec& IntAddition(FlaggedDec self, const IntType& rValue) { return self.IntAddOp(rValue); }

        template<IntegerType IntType = int>
        FlaggedDec AdditionByInt(const int& rValue) { FlaggedDec self = *this; return self.IntAddOp(rValue); }
public:
        FlaggedDec& Int32AddOp(const int& rValue) { return IntAddOp(rValue); }
        FlaggedDec& Int64AddOp(const __int64& rValue) { return IntAddOp(rValue); }

        FlaggedDec AddByInt32(const int& rValue) { FlaggedDec self = *this; return self.IntAddOp(rValue); }
    #pragma endregion Other Addition Operations

    #pragma region Other Subtraction Operations

        template<IntegerType IntType=int>
        void CatchAllIntSubtraction(const IntType& rValue, RepType LRep)
        {
            ConvertToNormType(LRep);
            BasicIntSubOp(rValue);
        }

        template<IntegerType IntType=int>
        void CatchAllIntSubtractionV2(const IntType& rValue)
        {
            ConvertToNormTypeV2();
            BasicIntSubOp(rValue);
        }

protected:
        /// <summary>
        /// Subtraction FlaggedDec modifying operation with right side Integer rValue.
        /// (copy by parameter and pointer version)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType=int>
        FlaggedDec& IntSubOp(const IntType& rValue)
        {
            if (rValue == 0)
                return *this;
    #if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert FlaggedDec into complex number at moment";
                return *this;
            }
    #endif
    #if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
    #endif
    #if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
            else
    #endif
            if(ExtraRep!=0)//Don't convert if mixed fraction
                ConvertToNormTypeV2();
            bool WasNegative = IntValue < 0;
            IntHalfAddition(rValue);
            //If flips to other side of negative, invert the decimals
    #if defined(AltNum_EnableMixedFractional)
            if ((IntValue<0)!=WasNegative)
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
                    DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
            }
    #else
            if ((IntValue<0)!=WasNegative)
                DecimalHalf = FlaggedDec::DecimalOverflow - DecimalHalf;
    #endif
            return *this;
        }

        /// <summary>
        /// Subtraction Operation Between FlaggedDec and Integer Value.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType = int>
        static FlaggedDec& IntSubtraction(FlaggedDec self, const IntType& rValue) { return self.IntSubOp(rValue); }

        template<IntegerType IntType = int>
        FlaggedDec SubtractionByInt(const int& rValue) { FlaggedDec self = *this; return self.IntSubOp(rValue); }
public:
        FlaggedDec& Int32SubOp(const int& rValue) { return IntSubOp(rValue); }
        FlaggedDec& Int64SubOp(const __int64& rValue) { return IntSubOp(rValue); }

        FlaggedDec SubtractByInt32(const int& rValue) { FlaggedDec self = *this; return self.IntSubOp(rValue); }
    #pragma endregion Other Subtraction Operations

    #pragma region Main AltNum Operations
public:

        void CatchAllDivision(const FlaggedDec& rValue, RepType LRep, RepType RRep)
        {
            ConvertToNormType(LRep);
            FlaggedDec convertedRVal = rValue;
            convertedRVal.ConvertToNormType(RRep);
            BasicMultOp(convertedRVal);
        }

        FlaggedDec CatchAllDivisionAsCopies(const FlaggedDec& rValue, RepType LRep, RepType RRep)
        { FlaggedDec self = *this; CatchAllDivision(rValue, LRep, RRep); return self; }
        
        void CatchAllDivisionV2(const FlaggedDec& rValue, RepType SameRep)
        {
            ConvertToNormType(SameRep);
            FlaggedDec convertedRVal = rValue;
            convertedRVal.ConvertToNormType(SameRep);
            BasicUnsignedDivOp(convertedRVal);
        }

        FlaggedDec CatchAllDivisionAsCopiesV2(FlaggedDec& rValue, RepType SameRep)
        { FlaggedDec self = *this; CatchAllDivisionV2(rValue, SameRep); return self; }
    
       void CatchAllDivisionV3(FlaggedDec& rValue)
       {
            ConvertToNormTypeV2();
            FlaggedDec convertedRVal = rValue.ConvertAsNormTypeV2();
            BasicUnsignedDivOp(convertedRVal);
       }

        FlaggedDec CatchAllDivisionAsCopiesV3(FlaggedDec& rValue)
        { FlaggedDec self = *this; CatchAllDivisionV3(rValue); return self; }
        
    //Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
    #if defined(AltNum_EnableImaginaryNum)
        void CatchAllImaginaryDivision(FlaggedDec& rValue, RepType LRep, RepType RRep)
        {
           ConvertIRepToNormal(LRep);
           FlaggedDec convertedRVal = rValue.ConvertAsNormalIRep(RRep);
           BasicUnsignedDivOp(convertedRVal);
           ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginaryDivisionAsCopies(FlaggedDec& rValue, RepType LRep, RepType RRep)
        { FlaggedDec self = *this; CatchAllImaginaryDivision(rValue, LRep, RRep); return self; }
        
        void CatchAllImaginaryDivisionV2(FlaggedDec& rValue, RepType SameRep)
        {
            ConvertIRepToNormal(SameRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormalIRep(SameRep);
            BasicUnsignedDivOp(convertedRVal);
            ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginaryDivisionAsCopiesV2(FlaggedDec& rValue, RepType SameRep)
        { FlaggedDec self = *this; CatchAllImaginaryDivisionV2(rValue, SameRep); return self; }
    
        void CatchAllImaginaryDivisionV3(FlaggedDec& rValue)
        {
           ConvertIRepToNormalV2();
           FlaggedDec convertedRVal = rValue.ConvertAsNormalIRepV2();
           BasicUnsignedDivOp(convertedRVal);
           ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginaryDivisionAsCopiesV3(FlaggedDec& rValue)
        { FlaggedDec self = *this; CatchAllImaginaryDivisionV3(rValue); return self; }
    #endif
protected:
        static void Division_LRepImaginaryOverride(RepType& RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
    #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingTop:
                if (Value.IntValue == 0)
                {
    #if defined(AltNum_EnableImaginaryInfinity)
                    if (self.IntValue < 0)//NegativeValue / 0.0..1 = Negative Infinity
                        self.IntValue = -1;
                    else//PositiveValue / 0.0..1 = Infinity
                        self.IntValue = 1;
                    self.DecimalHal = InfinityRep;
                    self.ExtraRep = IRep;
    #else
                    throw "Result is Infinity times i";
                    if (self.IntValue < 0)
                        self.SetAsMaximum();
                    else
                        self.SetAsMinimum();
                    self.ExtraRep = FlaggedDec::IRep;
    #endif
                    return;
                }
                else
                {
                    Value.DecimalHalf = 1;
                    RRep = RepType::NormalType;
                }
                break;
    #endif
    #if defined(AltNum_EnableApproachingI)
            case RepType::ApproachingImaginaryBottom:
                if (Value.IntValue == 0)
                {
    #if defined(AltNum_EnableInfinityRep)
                    if (self.IntValue < 0)//NegativeValue / 0.0..1 = Negative Infinity
                        self.IntValue = -1;
                    else//PositiveValue / 0.0..1 = Infinity
                        self.IntValue = 1;
                    self.DecimalHalf = FlaggedDec::InfinityRep;
                    self.ExtraRep = 0;
    #else
                    throw "Result is Infinity";
                    if (self.IntValue < 0)
                        self.SetAsMaximum();
                    else
                        self.SetAsMinimum();
    #endif
                    return;
                }
                else
                {
                    Value.DecimalHalf = 1;
                    RRep = RepType::NormalType;
                }
                break;
            case RepType::ApproachingImaginaryTop:
    #endif
    #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
    #endif
    #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
    #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
    #elif defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #endif
    #if defined(AltNum_EnableERep)
            case RepType::ENum:
    #if defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
    #elif defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #endif
    #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
            case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative) 
    #endif
    #if defined(AltNum_EnableNearPi)
            case RepType::NearPi://(Approaching Away from Zero is equal to 0.9999...Pi)
    #endif
    #if defined(AltNum_EnableNearE)
            case RepType::NearE://(Approaching Away from Zero is equal to 0.9999...e)
    #endif
    #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
    #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
    #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
    #endif
    #endif
                Value.ConvertToNormType(RRep);
                RRep = RepType::NormalType;
                break;
            default:
                break;
            }
        }

        static void Multiplication_LRepImaginaryOverride(RepType& RRep, FlaggedDec& Value)
        {
            switch (RRep)
            {
    #if !defined(AltNum_EnableComplexNumbers)
            case RepType::NormalType:
    #endif
            case RepType::NumByDiv:
    #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
    #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
    #elif defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #endif

    #if defined(AltNum_EnableERep)
            case RepType::ENum:
    #if defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
    #elif defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #endif

    #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
            case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
    #endif
    #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
            case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative) 
    #endif

    #if defined(AltNum_EnableNearPi)
            case RepType::NearPi://(Approaching Away from Zero is equal to 0.9999...Pi)
    #endif
    #if defined(AltNum_EnableNearE)
            case RepType::NearE://(Approaching Away from Zero is equal to 0.9999...e)
    #endif
    #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
    #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
    #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
    #endif
    #endif
                Value.ConvertToNormType(RRep);
                RRep = RepType::NormalType;
                break;
            }
        }
        static void LRepImaginaryOverridePt2(RepType& RRep, FlaggedDec& Value)
        {
            switch (RRep)
            {
    #if !defined(AltNum_EnableComplexNumbers)
            case RepType::NormalType:
    #endif
            case RepType::NumByDiv:
    #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
    #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
    #elif defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #endif

    #if defined(AltNum_EnableERep)
            case RepType::ENum:
    #if defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
    #elif defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #endif

    #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
            case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
    #endif
    #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
            case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative) 
    #endif

    #if defined(AltNum_EnableNearPi)
            case RepType::NearPi://(Approaching Away from Zero is equal to 0.9999...Pi)
    #endif
    #if defined(AltNum_EnableNearE)
            case RepType::NearE://(Approaching Away from Zero is equal to 0.9999...e)
    #endif
    #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
    #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
    #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
    #endif
    #endif
    #if defined(AltNum_EnableComplexNumbers)
                Value.ConvertToNormType(RRep);
                RRep = RepType::NormalType;
    #else
                throw "Complex number operations not enabled";
    #endif
                break;
            default:
                break;
            }
        }

        //NormalType representation multiplied by other representation types
        static void NormalRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType://Fail safe for when converted before switch
                self.BasicUnsignedDivOp(Value);
                break;
                #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv://X / (Y / Z) = (XZ)/Y
            {
                self.BasicUnsignedDivOp(Value);
                self.BasicInt32MultOpV2(Value.ExtraRep);
                break;
            }
            #if defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            {
                //X / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(YPi)
                self.BasicInt32MultOpV2(Value.DecimalHalf);
                self.BasicUnsignedDivOp(FlaggedDec::FlaggedDec::PiNum * Value.IntValue);//self.BasicUnsignedDivOp(FlaggedDec::PiNumMultByInt(Value.IntValue))
                break;
            }
            #endif
            #if defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            {
                self.BasicInt32MultOpV2(Value.DecimalHalf);
                self.BasicUnsignedDivOp(FlaggedDec::ENum * Value.IntValue);
                break;
            }
            #endif

            #if defined(AltNum_EnableDecimaledPiOrEFractionals)
            #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                #elif defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                #endif
            {
                self.BasicInt32MultOpV2(-Value.ExtraRep);
                #if defined(AltNum_EnableDecimaledPiFractionals)
                Value.ConvertToNormType(RepType::PiNum);
                #elif defined(AltNum_EnableDecimaledEFractionals)
                Value.ConvertToNormType(RepType::ENum);
                #endif
                self.BasicUnsignedDivOp(Value);
                break;
            }
            #endif
            #endif

            #if defined(AltNum_EnableImaginaryNum)
            //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
            case RepType::INum:
            {
                self.BasicUnsignedDivOp(Value);
                if (self.IntValue == FlaggedDec::FlaggedDec::NegativeRep)
                    self.IntValue = 0;
                else
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::FlaggedDec::NegativeRep : -self.IntValue;
                self.ExtraRep = FlaggedDec::IRep;
                break;
            }
            #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                //Placeholder code(Converting to i rep for now)
                #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
            {
                Value.ConvertToNormalIRep(RRep);
                self.BasicUnsignedDivOp(Value);
                if (self.IntValue == FlaggedDec::FlaggedDec::NegativeRep)
                    self.IntValue = 0;
                else
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::FlaggedDec::NegativeRep : -self.IntValue;
                self.ExtraRep = FlaggedDec::IRep;
                break;
            }
                                #endif
                                #endif
            default:
                Value.ConvertToNormType(RRep);
                self.BasicUnsignedDivOp(Value);
                break;
            }
        }

        #if defined(AltNum_EnableFractionals)
        //NumByDivisor representation multiplied by other representation types
        static void NumByDivisorRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                if (Value.DecimalHalf == 0)
                    self.ExtraRep *= Value.IntValue;
                else
                    self.BasicDivOp(Value);
                break;
                #if defined(AltNum_EnableImaginaryNum)
                //ToDo::Add more specific DivOperation code later
            case RepType::INum:
                #if defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #elif defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                //Skip to performing DivOperation as normal type
                self.ConvertToNormType(RepType::NumByDiv);
                NormalRtRDivision(RRep, self, Value);
                break;
                #endif
            default:
                Value.ConvertToNormType(RRep);
                if (Value.DecimalHalf == 0)
                    self.ExtraRep *= Value.IntValue;
                else
                    self.BasicDivOp(Value);
                break;
            }
        }
        #endif

        #if defined(AltNum_EnablePiRep)
        //PiNum representation multiplied by other representation types
        static void PiNumRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:// X*Pi * Y
                self.BasicDivOp(Value);
                break;
                #if defined(AltNum_EnablePiPowers)
                //case RepType::PiPower:
                //	//Add Pi powers code here later
                //	break;
                #endif
            case RepType::NumByDiv://(X*Pi) / (Y / Z) = (XZ)/Y
                self.BasicDivOp(Value);
                self.BasicInt32MultOpV2(Value.ExtraRep);
                break;

                #if defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            {
                //(XPi) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(Y)
                self.ExtraRep = 0;
                self.BasicInt32MultOpV2(Value.DecimalHalf);
                self.BasicInt32DivOpV2(Value.IntValue);
            }
            break;
            #endif
            #if defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            {
                self.BasicInt32MultOpV2(Value.DecimalHalf);
                FlaggedDec rVal = FlaggedDec::ENum * Value.IntValue;
                self.BasicDivOp(rVal);
            }
            break;
            #endif

            #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
            {
                //(XPi) / (Y.Value*Pi / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value)
                self.ExtraRep = 0;
                int rVal = -Value.ExtraRep;
                self.BasicInt32DivOpV2(rVal);
                self.BasicUnsignedMultOp(Value);
            }
            break;
            #elif defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
            {
                //(XPi) / (Y.Value*e / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value*e)
                int rVal = -Value.ExtraRep;
                self.BasicUnsignedMultOp(rVal);
                Value.ConvertToNormType(RepType::ENum);
                self.BasicDivOp(Value);
            }
            break;
            #endif

            #if defined(AltNum_EnableImaginaryNum)
            //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
            case RepType::INum:
                self.BasicDivOp(Value);
                self.BasicDivOp(FlaggedDec::FlaggedDec::PiNum);
                if (self.IntValue == FlaggedDec::FlaggedDec::NegativeRep)
                    self.IntValue = 0;
                else
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::FlaggedDec::NegativeRep : -self.IntValue;
                self.ExtraRep = FlaggedDec::IRep;
                break;
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                //Placeholder code(Converting to i rep for now)
                #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
                Value.ConvertToNormalIRep(RRep);
                self.BasicDivOp(Value);
                self.BasicDivOp(FlaggedDec::PiNum);
                if (self.IntValue == FlaggedDec::NegativeRep)
                    self.IntValue = 0;
                else
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::NegativeRep : -self.IntValue;
                self.ExtraRep = FlaggedDec::IRep;
                break;
                #endif
                #endif
            default:
                self.CatchAllDivision(Value, RepType::PiNum, RRep);
                break;
            }
        }
        #endif
        #if defined(AltNum_EnablePiPowers)
        //PiPower representation multiplied by other representation types
        static void PiPowerRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                self.BasicDivOp(Value);
                break;
            case RepType::PiNum://(IntValue.DecimalHalf)*Pi^-ExtraRep representation
                --self.ExtraRep;
                self.BasicDivOp(Value);
                break;

            #if defined(AltNum_EnableImaginaryNum)
            //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
            case RepType::INum:
            {
                self.BasicDivOp(Value);
                self.BasicDivOp(PiPowerNum(-self.ExtraRep));
                if (self.IntValue == FlaggedDec::NegativeRep)
                    self.IntValue = 0;
                else
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::NegativeRep : -self.IntValue;
                self.ExtraRep = IRep;
            }
            break;
            #endif
            #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
            #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
            #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
            #endif
            //Placeholder code(Converting to i rep for now)
            #if defined(AltNum_EnableAlternativeRepFractionals)
            {
                Value.ConvertIRepToNormal(RRep);
                self.BasicDivOp(Value);
                Value = PiPowerNum(-self.ExtraRep);//Reusing Value to store divisor
                self.BasicDivOp(Value);
                if (self.IntValue == FlaggedDec::NegativeRep)
                    self.IntValue = 0;
                else
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::NegativeRep : -self.IntValue;
                self.ExtraRep = FlaggedDec::IRep;
            }
            break;
            #endif
            default:
                self.CatchAllDivision(Value, RepType::PiPower, RRep);
                break;
            }
        }
        #endif
        #if defined(AltNum_EnablePiFractional)
        //PiFractional representation multiplied by other representation types
        static void PiFractionalRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                case RepType::NormalType:
                    if (Value.DecimalHalf == 0)
                        self.DecimalHalf *= Value.IntValue;
                    else
                    {
                        //Convert to NumByDiv instead
                        self.ExtraRep = self.DecimalHalf;
                        self.DecimalHalf = 0;
                        self.BasicUnsignedMultOp(FlaggedDec::PiNum);
                        self.BasicDivOp(Value);
                    }
                    break;
                #if defined(AltNum_EnableImaginaryNum)
                    //ToDo::Add more specific operation code later
                case RepType::INum:
                    #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                    #endif
                    #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    #endif
                    //Skip to performing DivOperation as normal type
                    self.ConvertToNormType(RepType::PiFractional);
                    RepToRepDivision(RepType::NormalType, RRep, self, Value);
                    break;
                #endif
                default:
                    self.CatchAllDivision(Value, RepType::PiFractional, RRep);
                    break;
            }
        }
        #endif

        #if defined(AltNum_EnableERep)
        //ENum representation multiplied by other representation types
        static void ENumRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                self.BasicDivOp(Value);
                break;
        #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                Value.ConvertPiToNum();
                self.BasicDivOp(Value);
                break;
        #endif
        #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                Value.ConvertPiPowerToNum();
                self.BasicDivOp(Value);
                break;
        #endif

        #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv://(X*E) / (Y / Z) = (XZ*E)/Y
                self.BasicDivOp(Value);
                self.BasicInt32MultOpV2(Value.ExtraRep);
                break;
        #endif
        #if defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                //(Xe) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf*e)/(Y)
                self.BasicInt32MultOpV2(Value.DecimalHalf);
                self.BasicUnsignedDiv(FlaggedDec::PiNumValue() * Value.IntValue);
                break;
        #elif defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                self.BasicInt32MultOpV2(-Value.ExtraRep);
                Value.ConvertToNormType(RepType::PiNum);
                self.BasicDivOp(Value);
                break;
        #endif
        #if defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                self.ExtraRep = 0;
                self.BasicInt32MultOpV2(Value.DecimalHalf);
                self.BasicInt32DivOpV2(Value.IntValue);
                break;
        #elif defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                self.BasicUnsignedIntMult(-Value.ExtraRep);
                self.ExtraRep = 0;
                self.BasicDivOp(Value);
                break;
        #endif

        #if defined(AltNum_EnableImaginaryNum)
                //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
            case RepType::INum:
                self.BasicDivOp(Value);
                self.BasicDivOp(FlaggedDec::ENum);
                if (self.IntValue == FlaggedDec::NegativeRep)
                    self.IntValue = 0;
                else
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::NegativeRep : -self.IntValue;
                self.ExtraRep = FlaggedDec::IRep;
                break;
        #endif
        #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
        #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
        #endif
        #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
        #endif
        //Placeholder code(Converting to i rep for now)
        #if defined(AltNum_EnableAlternativeRepFractionals)
                Value.ConvertToNormalIRep(RRep);
                self.BasicDivOp(Value);
                self.BasicDivOp(FlaggedDec::ENum);
                if (self.IntValue == FlaggedDec::NegativeRep)
                    self.IntValue = 0;
                else
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::NegativeRep : -self.IntValue;
                self.ExtraRep = FlaggedDec::IRep;
                break;
        #endif
            default:
                self.CatchAllDivision(Value, RepType::ENum, RRep);
                break;
            }
        }
        #endif
        #if defined(AltNum_EnableEFractional)
        //EFractional representation multiplied by other representation types
        static void EFractionalRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                if (Value.DecimalHalf == 0)
                    self.DecimalHalf *= Value.IntValue;
                else
                {
                    //Convert to NumByDiv instead
                    self.ExtraRep = self.DecimalHalf;
                    self.DecimalHalf = 0;
                    self.BasicMultOp(ENum);
                    self.BasicDivOp(Value);
                }
                break;
                #if defined(AltNum_EnableImaginaryNum)
                //ToDo::Add more specific DivOperation code later
            case RepType::INum:
                #if defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #elif defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                //Skip to performing DivOperation as normal type
                self.ConvertToNormType(RepType::EFractional);
                NormalRtRDivision(RRep, self, Value);
                break;
                #endif
            default:
                self.CatchAllDivision(Value, RepType::EFractional, RRep);
                break;
            }
}
        #endif

    #if defined(AltNum_EnableDecimaledPiOrEFractionals)
        //PiByDivisor or EByDivisor representation multiplied by other representation types
        static void PiOrEByDivisorRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                case RepType::NormalType://Normalize denom later
                    self.BasicDivOp(Value);
                    break;
            #if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
                {
                    #if defined(AltNum_EnableDecimaledPiFractionals)//Becomes NumByDiv instead
                    self.ExtraRep *= -1;
                    if (Value.DecimalHalf == 0)
                        self.ExtraRep *= Value.IntValue;
                    else
                        self.BasicUnsignedDivOp(Value);
                    #else
                    if (Value.DecimalHalf == 0)
                        self.ExtraRep *= Value.IntValue;
                    else
                        self.BasicUnsignedDivOp(Value);
                    self.BasicUnsignedDivOp(PiNum);
                    #endif
                }
                break;
            #endif
            #if defined(AltNum_EnableERep)
                case RepType::ENum:
                {
                #if defined(AltNum_EnableDecimaledEFractionals)//Becomes NumByDiv instead
                    if (Value.DecimalHalf == 0)
                    {
                        self.ExtraRep *= -Value.IntValue;
                    }
                    else
                    {
                        self.BasicUnsignedDivOp(Value);
                        self.ExtraRep *= -1;
                    }
                #else
                    if (Value.DecimalHalf == 0)
                        self.ExtraRep *= Value.IntValue;
                    else
                        self.BasicUnsignedDivOp(Value);
                    self.BasicUnsignedDivOp(ENum);
                #endif
                }
                    break;
            #endif
            #if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                    if(Value.DecimalHalf==0)
					    self.ExtraRep *= Value.IntValue;
				    else
					    self.BasicUnsignedDivOp(Value);
				    self.BasicInt32MultOpV2(Value.ExtraRep);
                    break;
            #endif
            #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
				    self.ExtraRep *= Value.IntValue;
				    self.BasicInt32MultOpV2(Value.DecimalHalf);
				    self.BasicUnsignedDivOp(PiNum);
                    break;
            #endif
            #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
				    self.ExtraRep *= Value.IntValue;
				    self.BasicInt32MultOpV2(Value.DecimalHalf);
				    self.BasicUnsignedDivOp(ENum);
                    break;
            #endif
            #if defined(AltNum_EnableImaginaryNum)
                    //ToDo::Add more specific DivOperation code later
                case RepType::INum:
                    #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                    #endif
                    #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    #endif
                    //Skip to performing DivOperation as normal type
                    #if defined(AltNum_EnableDecimaledPiFractionals)
                    self.ConvertToNormType(RepType::PiNumByDiv);
                    #else
                    self.ConvertToNormType(RepType::PiNumByDiv);
                    #endif
                    NormalRtRDivision(RRep, self, Value);
                    break;
            #endif
                default:
                    #if defined(AltNum_EnableDecimaledPiFractionals)
                    self.CatchAllDivision(Value, RepType::PiNumByDiv, RRep);
                    #else
                    self.CatchAllDivision(Value, RepType::ENumByDiv, RRep);
                    #endif
                    break;
            }
        }
    #endif

        #if defined(AltNum_EnableApproachingValues)
        //ApproachingBottom representation multiplied by other representation types
        static void ApproachingBottomRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                #if defined(AltNum_EnableImaginaryNum)
                    //ToDo::Add more specific DivOperation code later
                case RepType::INum:
                    #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                    #elif defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    #endif
                    #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    #endif
                    #if defined(AltNum_EnableApproachingI)
                    if (self.IntValue == 0 && self.IntValue == NegativeRep)
                    {
                        FlaggedDec convertedVal = Value.ConvertAsNormalIRep(RRep);
                        if (convertedVal.DecimalHalf == 0)
                        {
                            self.ExtraRep = convertedVal.IntValue;
                            self.DecimalHalf = ApproachingImaginaryBottomRep;
                            return;
                        }
                    }
                    #endif
                    //Skip to performing DivOperation as normal type
                    self.ConvertToNormType(RepType::ApproachingBottom);
                    NormalRtRDivision(RRep, self, Value);
                    break;
                #endif
                default:
                    self.CatchAllDivision(Value, RepType::ApproachingBottom, RRep);
                    break;
            }
        }
	        #if !defined(AltNum_DisableApproachingTop)
        //ApproachingTop representation multiplied by other representation types
        static void ApproachingTopRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                    #if defined(AltNum_EnableImaginaryNum)
                    //ToDo::Add more specific operation code later
                case RepType::INum:
                    #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                    #elif defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    #endif
                    #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    #endif
            #if defined(AltNum_EnableApproachingI)
                    if (self.IntValue == 0 && self.IntValue == NegativeRep)
                    {
                        FlaggedDec convertedVal = Value.ConvertAsNormalIRep(RRep);
                        if (convertedVal.DecimalHalf == 0)
                        {
                            self.ExtraRep = convertedVal.IntValue;
                            self.DecimalHalf = ApproachingImaginaryTopRep;
                            return;
                        }
                    }
            #endif
                    //Skip to performing DivOperation as normal type
                    self.ConvertToNormType(RepType::ApproachingTop);
                    NormalRtRDivision(RRep, self, Value);
                    break;
                    #endif
                default:
                    self.CatchAllDivision(Value, RepType::ApproachingTop, RRep);
                    break;
            }
        }
            #endif
        #endif
        #if defined(AltNum_EnableApproachingDivided)
        //ApproachingMidLeft representation multiplied by other representation types
        static void ApproachingMidLeftRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
    #if defined(AltNum_EnableImaginaryNum)
                //ToDo::Add more specific operation code later
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
        #if defined(AltNum_EnableApproachingI)
                if (self.IntValue == 0 && self.IntValue == NegativeRep)
                {
                    FlaggedDec convertedVal = Value.ConvertAsNormalIRep(RRep);
                    if (convertedVal.DecimalHalf == 0)
                    {
                        self.ExtraRep *= convertedVal.IntValue;
                        self.DecimalHalf = ApproachingImaginaryBottomRep;
                        return;
                    }
                }
        #endif
                //Skip to performing DivOperation as normal type
                self.ConvertToNormType(RepType::ApproachingMidLeft);
                NormalRtRDivision(RRep, self, Value);
                break;
    #endif
            default:
                self.CatchAllDivision(Value, RepType::ApproachingMidLeft, RRep);
                break;
            }
        }
	        #if !defined(AltNum_DisableApproachingTop)
        //ApproachingMidRight representation multiplied by other representation types
        static void ApproachingMidRightRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
        #if defined(AltNum_EnableImaginaryNum)
                    //ToDo::Add more specific operation code later
                case RepType::INum:
                    #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                    #elif defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    #endif
                    #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    #endif
            #if defined(AltNum_EnableApproachingI)
                    if (self.IntValue == 0 && self.IntValue == NegativeRep)
                    {
                        FlaggedDec convertedVal = Value.ConvertAsNormalIRep(RRep);
                        if (convertedVal.DecimalHalf == 0)
                        {
                            self.ExtraRep *= convertedVal.IntValue;
                            self.DecimalHalf = ApproachingImaginaryTopRep;
                            return;
                        }
                    }
            #endif
                    //Skip to performing DivOperation as normal type
                    self.ConvertToNormType(RepType::ApproachingMidRight);
                    NormalRtRDivision(RRep, self, Value);
                    break;
        #endif
                default:
                    self.CatchAllDivision(Value, RepType::ApproachingMidRight, RRep);
                    break;
            }
        }
            #endif
        #endif

#if defined(AltNum_EnableImaginaryNum)
        //INum representation multiplied by other representation types
        static void INumRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                case RepType::NormalType:
                    self.BasicDivOp(Value);
                    break;
        #if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
                    Value.ConvertPiToNum();
                    self.BasicDivOp(Value);
                    break;
        #endif
        #if defined(AltNum_EnableERep)
                case RepType::ENum:
                    Value.ConvertToNormType(RepType::PiNum);
                    self.BasicDivOp(Value);
                    break;
        #endif

        #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    //selfNum*i / (Value.IntValue/Value.DecimalHalf*i)
                    //selfNum / (Value.IntValue/Value.DecimalHalf)
                    //(selfNum*Value.DecimalHalf) / Value.IntValue
                    self.BasicInt32MultOpV2(Value.DecimalHalf);
                    self.ExtraRep = Value.IntValue;
                    break;//Return as NumByDiv
        #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://Value/(-ExtraRep))*i Representation
                    //(FlaggedDec(self.IntValue, self.DecimalHalf)i)/(Value/(-Value.ExtraRep))*i)
                    //=(FlaggedDec(self.IntValue, self.DecimalHalf))/(Value/(-Value.ExtraRep)))
                    //=(FlaggedDec(self.IntValue, self.DecimalHalf)*-Value.ExtraRep)/Value
                    self.BasicUnsignedIntDiv(-Value.ExtraRep);
                    if (Value.DecimalHalf == 0)
                    {
                        self.ExtraRep = Value.ExtraRep;
                    }
                    else
                    {
                        self.ExtraRep = 0;
                        self.BasicDivOp(Value);
                    }
                    break;
        #endif
                default:
                    Value.ConvertToNormType(RRep);
                    self.BasicDivOp(Value);
                    break;
            }
        }
#endif
#if defined(AltNum_EnableDecimaledIFractionals)
        //INumByDivisor representation multiplied by other representation types
        static void INumByDivisorRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                case RepType::NormalType:
                    self.BasicDivOp(Value);
                    break;
                case RepType::INum:
                    //SelfNum*i / ValueNum*i
                    //SelfNum/ValueNum
                    if (Value.DecimalHalf == 0)
                        self.ExtraRep = Value.ExtraRep;
                    else
                    {
                        self.ExtraRep = 0;
                        self.BasicDivOp(Value);
                    }
                    break;
                #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://Return result as either MixedFraction or NumByDivisor
			    {
				    //if(self.DecimalHalf==0)//Add code here that returns return as MixedFraction
				    self.SwapNegativeStatus();
				    self.BasicInt32MultOpV2(-Value.ExtraRep);
				    self.ExtraRep *= (Value.IntValue*Value.ExtraRep) + Value.DecimalHalf;//ToDo::Check for overflow and perform alternative code on overflow
			    }
                break;
                #endif
                default:
                    Value.ConvertToNormType(RRep);
                    self.BasicDivOp(Value);
            }
        }

#elif defined(AltNum_EnableIFractional)
        //IFractional representation multiplied by other representation types
        static void IFractionalRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                if (Value.DecimalHalf == 0)
                    self.DecimalHalf *= Value.IntValue;
                else//Convert to INum
                {//self.IntValue / self.DecimalHalf / Value
                    self.ExtraRep = IRep;
                    int LSideDivisor = self.DecimalHalf;
                    self.DecimalHalf = 0;
                    self.BasicInt32DivOpV2(LSideDivisor);
                    self.BasicUnsignedDivOp(Value);
                }
                break;
            default:
                Value.ConvertToNormType(RRep);
                if (Value.DecimalHalf == 0)
                    self.DecimalHalf *= Value.IntValue;
                else//Convert to INum
                {
                    self.ExtraRep = IRep;
                    int LSideDivisor = self.DecimalHalf;
                    self.DecimalHalf = 0;
                    self.BasicInt32DivOpV2(LSideDivisor);
                    self.BasicUnsignedDivOp(Value);
                }
            }
        }
#endif

#if defined(AltNum_EnableApproachingI)
        //ApproachingImaginaryBottom representation multiplied by other representation types
        static void ApproachingImaginaryBottomRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
        #if defined(AltNum_EnableImaginaryNum)
                    //ToDo::Add more specific operation code later
                case RepType::INum:
                    #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                    #elif defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    #endif
                    #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    #endif
                {
                    //Skip to performing DivOperation as normal type
                    self.ConvertIRepToNormal(RepType::ApproachingImaginaryBottom);
                    self.SwapNegativeStatus();
                    self.ExtraRep = 0;
                    NormalRtRDivision(RRep, self, Value);
                }
                break;
        #endif
                default:
                    Value.ConvertToNormType(RRep);
                    if (Value.DecimalHalf == 0 && (self.IntValue == 0 || self.IntValue == NegativeRep))
                    {
                        self.ExtraRep = Value.IntValue;
                    }
                    else
                    {
                        self.ConvertToNormalIRep(RepType::ApproachingImaginaryBottom);
                        self.BasicUnsignedDivOp(Value);
                    }
                    break;
            }
        }
    #if !defined(AltNum_DisableApproachingTop)
        //ApproachingImaginaryTop representation multiplied by other representation types
        static void ApproachingImaginaryTopRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                #if defined(AltNum_EnableImaginaryNum)
                //ToDo::Add more specific DivOperation code later
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                //Skip to performing DivOperation as normal type
                self.ConvertIRepToNormal(RepType::ApproachingImaginaryTop);
                self.SwapNegativeStatus();
                self.ExtraRep = 0;
                NormalRtRDivision(RRep, self, Value);
                #endif
                break;
            default:
                Value.ConvertToNormType(RRep);
                if (Value.DecimalHalf == 0 && (self.IntValue == 0 || self.IntValue == NegativeRep))
                    self.ExtraRep = Value.IntValue;
                else
                {
                    self.ConvertToNormalIRep(RepType::ApproachingImaginaryTop);
                    self.BasicUnsignedDivOp(Value);
                }
                break;
            }
        }
    #endif
    #if defined(AltNum_EnableApproachingDivided)
        //ApproachingImaginaryMidLeft representation multiplied by other representation types
        static void ApproachingImaginaryMidLeftRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                #if defined(AltNum_EnableImaginaryNum)
                //ToDo::Add more specific DivOperation code later
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                //Skip to performing DivOperation as normal type
                self.ConvertIRepToNormal(RepType::ApproachingImaginaryMidLeft);
                self.SwapNegativeStatus();
                self.ExtraRep = 0;
                NormalRtRDivision(RRep, self, Value);
                #endif
            default:
                Value.ConvertToNormType(RRep);
                if (Value.DecimalHalf == 0 && (self.IntValue == 0 || self.IntValue == NegativeRep))
                    self.ExtraRep *= Value.IntValue;
                else
                {
                    self.ConvertToNormalIRep(RepType::ApproachingImaginaryMidLeft);
                    self.BasicUnsignedDivOp(Value);
                }
                break;
            }
        }

	    #if !defined(AltNum_DisableApproachingTop)
        //ApproachingImaginaryMidRight representation multiplied by other representation types
        static void ApproachingImaginaryMidRightRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                #if defined(AltNum_EnableImaginaryNum)
                //ToDo::Add more specific DivOperation code later
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                //Skip to performing DivOperation as normal type
                self.ConvertIRepToNormal(RepType::ApproachingImaginaryMidRight);
                self.SwapNegativeStatus();
                self.ExtraRep = 0;
                NormalRtRDivision(RRep, self, Value);
                break;
                #endif
            default:
                Value.ConvertToNormType(RRep);
                if (Value.DecimalHalf == 0 && (self.IntValue == 0 || self.IntValue == NegativeRep))
                    self.ExtraRep *= Value.IntValue;
                else
                {
                    self.ConvertToNormalIRep(RepType::ApproachingImaginaryMidRight);
                    self.BasicUnsignedDivOp(Value);
                }
                break;
            }
        }
        #endif
    #endif
#endif

        #if defined(AltNum_EnableMixedFractional)
        //MixedFraction representation multiplied by other representation types
        static void MixedFracRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                //case RepType::NormalType:
                //    break;
                default:
                    self.CatchAllDivision(Value, RepType::MixedFrac, RRep);
            }
        }
        #endif
        #if defined(AltNum_MixedPiOrEEnabled)
        //MixedPiFraction or MixedEFraction representation multiplied by other representation types
        static void MixedPiOrERtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                if (Value.DecimalHalf == 0)
                {
                    int LeftSideAbs = self.IntHalfAsAbs();
                    int divRes = LeftSideAbs / Value.IntValue;
                    int C = LeftSideAbs - Value.IntValue * divRes;
                    if (C == 0)
                    {
                        if (divRes == 0)//Convert into non-mixed fractional
                        {
                            throw "ToDo:Finish this code later";
                        }
                        else
                        {
                            throw "ToDo:Finish this code later";
                        }
                    }
                }
                else//Convert to NumByDiv
                {
                    throw "ToDo:Finish this code later";
                }
                break;
            default:
                self.CatchAllDivision(Value, RepType::MixedPi, RRep);
            }
        }
        #elif defined(AltNum_EnableMixedIFractional)
        //MixedIFraction representation multiplied by other representation types
        static void MixedIRtRDivision(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {

            }
        }
        #endif

        static void RepToRepDivision(RepType& LRep, RepType& RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            //LRep Overrides
            #if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)
            switch (LRep)
            {
                #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                throw "UndefinedButInRange operations not supported yet(from left side)"; return;
                break;
                #endif
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                #if defined(AltNum_EnableAlternativeRepFractionals)
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                BlazesRusCode::FlaggedDec::Division_LRepImaginaryOverride(RRep, self, Value);
                break;
                #endif
            default:
                break;
            }
            #endif
            //RRep Overrides before Main RepToRep Code
            switch (RRep)
            {
                #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
                //Any real number / 0.0..1 = Infinity
                if (Value.IntValue == 0)
                {
                    self.DecimalHalf = InfinityRep;
                    if (self.IntValue<0)
                        self.IntValue = -1;
                    else
                        self.IntValue = 1;
                    self.ExtraRep = 0;
                    return;
                }
                else
                {
                    Value.DecimalHalf = 1;
                    RRep = RepType::NormalType;
                }
                break;

            case RepType::ApproachingTop:
            {
                if (Value.IntValue == 0 && LRep == RepType::NormalType)//1/0.9..9 = 1.0..1
                {//(For positive left side values);Technically should return self.IntValue + 0.0..self.IntValue
                    self.DecimalHalf = ApproachingBottomRep;
                    self.ExtraRep = 0;
                    return;
                }
                else
                {
                    Value.DecimalHalf = 999999999;
                    Value.ExtraRep = 0;
                    RRep = RepType::NormalType;
                }
                break;
            }
            #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                Value.ConvertToNormType(RepType::ApproachingMidLeft);
                RRep = RepType::NormalType;
                break;
            case RepType::ApproachingMidRight:
                Value.ConvertToNormType(RepType::ApproachingMidRight);
                RRep = RepType::NormalType;
                break;
                #endif
                #endif
                #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                throw "UndefinedButInRange operations not supported yet(from right side)";
                break;
                #endif
                #if defined(AltNum_EnableNaN)
            case RepType::Undefined:
            case RepType::NaN:
                throw "Can't perform operations with NaN or Undefined number";
                break;
                #endif
            default:
                break;
            }
            switch (LRep)//Main switch block starts here
            {
            case RepType::NormalType:
                NormalRtRDivision(RRep, self, Value); break;
                #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                PiNumRtRDivision(RRep, self, Value); break;
                #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                PiPowerRtRDivision(RRep, self, Value); break;
                #endif
                #endif
                #if defined(AltNum_EnableERep)
            case RepType::ENum:
                ENumRtRDivision(RRep, self, Value); break;
                #endif
                #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
                ApproachingBottomRtRDivision(RRep, self, Value); break;
                #if defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTRtRDivision:
                ApproachingTopRtRDivision(RRep, self, Value); break;
                #endif
                #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                ApproachingMidLeftRtRDivision(RRep, self, Value); break;
                #if defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingMidRight:
                ApproachingMidRightRtRDivision(RRep, self, Value); break;
                #endif
                #endif
                #endif
                #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                NumByDivisorRtRDivision(RRep, self, Value); break;
                #if defined(AltNum_EnableAlternativeRepFractionals)
                #if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                PiFractionalRtRDivision(RRep, self, Value); break;
                #endif
                #if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                EFractionalRtRDivision(RRep, self, Value); break;
                #endif

                #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                PiOrEByDivisorRtRDivision(RRep, self, Value); break;
                #elif defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                PiOrEByDivisorRtRDivision(RRep, self, Value); break;
                #endif
                #endif
                #endif
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                INumRtRDivision(RRep, self, Value); break;
                #if defined(AltNum_EnableFractionals)
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                INumByDivisorRtRDivision(RRep, self, Value); break;
                #else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                IFractionalRtRDivision(RRep, self, Value); break;
                #endif
                #endif
                #endif
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
                EFractionalRtRDivision(RRep, self, Value); break;
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
                MixedPiOrERtRDivision(RRep, self, Value); break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                MixedPiOrERtRDivision(RRep, self, Value); break;
                #elif defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                MixedIRtRDivision(RRep, self, Value); break;
                #endif
                #endif
                #if defined(AltNum_EnableNaN)
            case RepType::Undefined:
            case RepType::NaN:
                throw "Can't perform RtRDivisionerations with NaN or Undefined number";
                break;
                #endif
            default:
                throw FlaggedDec::RepTypeAsString(LRep) + " RepType division with" + FlaggedDec::RepTypeAsString(RRep) + "not supported yet";
                break;
            }
        }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="Value">The rightside rValue</param>
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& DivOp(const FlaggedDec& rightSideValue)
        {
            //Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
            if (IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (rightSideValue.DecimalHalf == InfinityRep)
            {
                if (DecimalHalf == InfinityRep)//https://www.philforhumanity.com/Infinity_Divided_by_Infinity.html
    #if defined(AltNum_EnableNaN)
                    DecimalHalf = NaNRep;//Infinity divided by infinity equals undefined
    #else
                    throw "Result is undefined(Infinity by Infinity)";
    #endif
                else
    #if defined(AltNum_EnableApproachingValues)
                {
                    DecimalHalf = ApproachingBottomRep;
                    if(rightSideValue.IntValue<0)
                    {
                        if (IntValue<0)
                            IntValue = 0;
                        else
                            IntValue = NegativeRep;
                    }
                    else
                    {
                        if (IntValue<0)
                            IntValue = NegativeRep;
                        else
                            IntValue = 0;
                    }
                    ExtraRep = 0;
                }
    #else
                    SetAsZero();
    #endif
                return *this;
            }
            if (rightSideValue.IntValue==0&&rightSideValue.DecimalHalf==0)
            {
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
            }
#else
            if (rightSideValue.IsZero())
                throw "Target value can not be divided by zero unless infinity enabled";
#endif
            RepType LRep = GetRepType();
            RepType RRep = rightSideValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicUnsignedDivOp(rightSideValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableERep)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum://Xi / Yi = (X(Sqrt(-1))/(Y(Sqrt(-1)) = X/Y
#endif
#if (defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)) || defined(AltNum_EnableERep) || defined(AltNum_EnableImaginaryNum)
                    ExtraRep = 0;
                    BasicUnsignedDivOp(rightSideValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    BasicUnsignedDivOp(rightSideValue);
                    if (rightSideValue.ExtraRep<ExtraRep)
                    {
                        FlaggedDec PiPowerDivisor = PiPowerNum(ExtraRep - rightSideValue.ExtraRep);
                        ExtraRep = 0;
                        BasicUnsignedDivOp(PiPowerDivisor);
                    }
                    else
                    {
                        ExtraRep -= rightSideValue.ExtraRep;
                    }
                    break;
#endif

#if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                {
                    int rInverted = rightSideValue.IntValue==NegativeRep?0:-rightSideValue.IntValue;
                    if (IntValue == rightSideValue.IntValue)
                    {
                        IntValue = 1;
                        DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == 0 && rightSideValue.IntValue == NegativeRep)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rightSideValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else
                    {
                        CatchAllDivisionV2(rightSideValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
                case RepType::ApproachingTop:
                {
                    int rInverted = rightSideValue.IntValue==NegativeRep?0:-rightSideValue.IntValue;
                    if (IntValue == rightSideValue.IntValue)
                    {
                        IntValue = 1; DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rightSideValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;

                    }
                    else
                    {
                        CatchAllDivisionV2(rightSideValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    if (ExtraRep == rightSideValue.ExtraRep)
                    {// 0.249..9 / 0.249..9 = 1
                        int InvertedrightSideValue = rightSideValue.IntValue==NegativeRep?0:-(int)rightSideValue.IntValue;
                        if (IntValue == rightSideValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrightSideValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == NegativeRep && rightSideValue.IntValue == 0)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else
                        {
                            CatchAllDivisionV2(rightSideValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rightSideValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                case RepType::ApproachingMidRight:
                    if (ExtraRep == rightSideValue.ExtraRep)
                    {//0.50..1 / 0.50..1 = 1
                        int InvertedrightSideValue = rightSideValue.IntValue==NegativeRep?0:-(int)rightSideValue.IntValue;
                        if (IntValue == rightSideValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrightSideValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else
                        {
                            CatchAllDivisionV2(rightSideValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rightSideValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
    #endif
#endif

#if defined(AltNum_EnableFractionals)//Unfinished code
                case RepType::NumByDiv://(FlaggedDec(IntValue,DecimalHalf))/ExtraRep
                {
                    //((FlaggedDec(IntValue,DecimalHalf))/ExtraRep) / (FlaggedDec(rightSideValue.IntValue,rightSideValue.DecimalHalf))/rightSideValue.ExtraRep) = 
                    //((FlaggedDec(IntValue,DecimalHalf))* rightSideValue.ExtraRep/ExtraRep) /(FlaggedDec(rightSideValue.IntValue,rightSideValue.DecimalHalf)))
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {//CatchAllDivisionV2(rValue, LRep);
                        BasicUIntMultOp(rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
#endif
#if defined(AltNum_UsingAltFractional)
                //(Self.IntValue/DecimalHalf)/(rightSideValue.IntValue/rightSideValue.DecimalHalf) =
                //(IntValue*rightSideValue.DecimalHalf)/(DecimalHalf*rightSideValue.IntValue)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
    #endif
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int NumRes = rValue.DecimalHalf * IntValue;
                    int DenomRes = DecimalHalf * rValue.IntValue;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
#endif
#if defined(AltNum_EnableDecimaledAlternativeFractionals)
    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
    #endif
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
#endif

                //Turn MixedFrac into fractional and then apply	
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int rvDivisor = -rValue.ExtraRep;
                    //=LeftSideNum*rValue.ExtraRep / RightSideNum;
                    FlaggedDec LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = FlaggedDec(DecimalHalf);
                    else if (IntValue < 0)
                        LeftSideNum = FlaggedDec(IntValue * ExtraRep + DecimalHalf);
                    else if (IntValue == 0)
                        LeftSideNum = FlaggedDec(-DecimalHalf);
                    else
                        LeftSideNum = FlaggedDec(IntValue * ExtraRep - DecimalHalf);
                    LeftSideNum.UIntDivOp(rValue.ExtraRep);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else
                    {
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==NegativeRep ? -rValue.DecimalHalf : -rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                    }
                }
                break;//Result as NumByDiv
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
    #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
    #endif
    #if defined(AltNum_MixedAltEnabled)
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    //=LeftSideNum*-rValue.ExtraRep / RightSideNum;
                    int rvDivisor = -rValue.ExtraRep;
                    FlaggedDec LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    LeftSideNum.UIntDivOp(rvDivisor);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else//Result as NumByDiv
                    {   //Need to have positive ExtraRep value
                        //int RightSideNum = rValue.IntValue==0?-DecimalHalf:(rValue.IntValue*-rValue.ExtraRep)-rValue.DecimalHalf;
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == NegativeRep ? DecimalHalf : (-rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == 0 ? DecimalHalf : (rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                    }
                }
                break;
    #endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rightSideValue.DecimalHalf == InfinityRep)
                    {
                        if (DecimalHalf == InfinityRep)
                            SetVal(One);
                        else
                            #if defined(AltNum_EnableNaN)
                            DecimalHalf = UndefinedRep;
                        #else
                            throw "Undefined result(results in undefined expression)";
                        #endif
                    }
                    else
                        BasicUnsignedDivOp(rightSideValue);
                }
                break;
    #if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
    #endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw FlaggedDec::RepTypeAsString(LRep) + " RepType division not supported yet";
                    break;
                }
            }
            else
            {
                FlaggedDec rValue = rightSideValue;
                if(rValue<0)
                {
                    rValue *= -1;
                    SwapNegativeStatus();
                }
                RepToRepDivision(LRep, RRep, *this, rValue);
            }
            return *this;
        }
   
        FlaggedDec DivideBy(const FlaggedDec& rValue) { FlaggedDec self = *this; self.DivOp(rValue); return self; }

        /// <summary>
        /// Division Operation with only positive right side values
        /// </summary>
        /// <param name="Value">The rightside rValue</param>
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& UnsignedDivOp(FlaggedDec rValue)
        {
            //Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
            if (IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (rValue.DecimalHalf == InfinityRep)
            {
                if (DecimalHalf == InfinityRep)//https://www.philforhumanity.com/Infinity_Divided_by_Infinity.html
    #if defined(AltNum_EnableNaN)
                    DecimalHalf = NaNRep;//Infinity divided by infinity equals undefined
    #else
                    throw "Result is undefined(Infinity by Infinity)";
    #endif
                else
    #if defined(AltNum_EnableApproachingValues)
                {
                    DecimalHalf = ApproachingBottomRep;
                    if(rValue.IntValue<0)
                    {
                        if (IntValue<0)
                            IntValue = 0;
                        else
                            IntValue = NegativeRep;
                    }
                    else
                    {
                        if (IntValue<0)
                            IntValue = NegativeRep;
                        else
                            IntValue = 0;
                    }
                    ExtraRep = 0;
                }
    #else            
                    SetAsZero();
    #endif
                return *this;
            }
            if (rValue.IntValue==0&&rValue.DecimalHalf==0)
            {
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
            }
#else
            if (rValue.IsZero())
                throw "Target value can not be divided by zero unless infinity enabled";
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicUnsignedDivOp(rValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableERep)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum://Xi / Yi = (X(Sqrt(-1))/(Y(Sqrt(-1)) = X/Y
#endif
#if (defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)) || defined(AltNum_EnableERep) || defined(AltNum_EnableImaginaryNum)
                    ExtraRep = 0;
                    BasicUnsignedDivOp(rValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    BasicUnsignedDivOp(rValue);
                    if (rValue.ExtraRep<ExtraRep)
                    {
                        FlaggedDec PiPowerDivisor = PiPowerNum(ExtraRep - rValue.ExtraRep);
                        ExtraRep = 0;
                        BasicUnsignedDivOp(PiPowerDivisor);
                    }
                    else
                    {
                        ExtraRep -= rValue.ExtraRep;
                    }
                    break;
#endif

#if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                {
                    int rInverted = -rValue.IntValue;
                    if (IntValue == rValue.IntValue)
                    {
                        IntValue = 1;
                        DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == 0 && rValue.IntValue == NegativeRep)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
                case RepType::ApproachingTop:
                {
                    int rInverted = -rValue.IntValue;
                    if (IntValue == rValue.IntValue)
                    {
                        IntValue = 1; DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;

                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    if (ExtraRep == rValue.ExtraRep)
                    {// 0.249..9 / 0.249..9 = 1
                        int InvertedrValue = rValue.IntValue==NegativeRep?0:-(int)rValue.IntValue;
                        if (IntValue == rValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == NegativeRep && rValue.IntValue == 0)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else
                        {
                            CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                case RepType::ApproachingMidRight:
                    if (ExtraRep == rValue.ExtraRep)
                    {//0.50..1 / 0.50..1 = 1
                        int InvertedrValue = rValue.IntValue==NegativeRep?0:-(int)rValue.IntValue;
                        if (IntValue == rValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else
                        {
                            CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
    #endif
#endif

#if defined(AltNum_EnableFractionals)//Unfinished code
                case RepType::NumByDiv://(FlaggedDec(IntValue,DecimalHalf))/ExtraRep
                {
                    //((FlaggedDec(IntValue,DecimalHalf))/ExtraRep) / (FlaggedDec(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((FlaggedDec(IntValue,DecimalHalf))* rValue.ExtraRep/ExtraRep) /(FlaggedDec(rValue.IntValue,rValue.DecimalHalf)))
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {
                        BasicUIntMultOp(rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
#endif
#if defined(AltNum_UsingAltFractional)
                //(Self.IntValue/DecimalHalf)/(rValue.IntValue/rValue.DecimalHalf) =
                //(IntValue*rValue.DecimalHalf)/(DecimalHalf*rValue.IntValue)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
    #endif
                {
                    int NumRes = rValue.DecimalHalf * IntValue;
                    int DenomRes = DecimalHalf * rValue.IntValue;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
#endif
#if defined(AltNum_EnableDecimaledAlternativeFractionals)
    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
    #endif
                {
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
#endif

                //Turn MixedFrac into fractional and then apply			
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    int rvDivisor = -rValue.ExtraRep;
                    //=LeftSideNum*rValue.ExtraRep / RightSideNum;
                    FlaggedDec LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = FlaggedDec(DecimalHalf);
                    else if (IntValue < 0)
                        LeftSideNum = FlaggedDec(IntValue * ExtraRep + DecimalHalf);
                    else if (IntValue == 0)
                        LeftSideNum = FlaggedDec(-DecimalHalf);
                    else
                        LeftSideNum = FlaggedDec(IntValue * ExtraRep - DecimalHalf);
                    LeftSideNum.UIntDivOp(rValue.ExtraRep);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else
                    {
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==NegativeRep ? -rValue.DecimalHalf : -rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                    }
                }
                break;//Result as NumByDiv
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
    #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
    #endif
    #if defined(AltNum_MixedAltEnabled)
                {
                    //=LeftSideNum*-rValue.ExtraRep / RightSideNum;
                    int rvDivisor = -rValue.ExtraRep;
                    FlaggedDec LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    LeftSideNum.UIntDivOp(rvDivisor);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else//Result as NumByDiv
                    {   //Need to have positive ExtraRep value
                        //int RightSideNum = rValue.IntValue==0?-DecimalHalf:(rValue.IntValue*-rValue.ExtraRep)-rValue.DecimalHalf;
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == NegativeRep ? DecimalHalf : (-rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == 0 ? DecimalHalf : (rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                    }
                }
                break;
    #endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rValue.DecimalHalf == InfinityRep)
                    {
                        if (DecimalHalf == InfinityRep)
                            SetVal(One);
                        else
                            #if defined(AltNum_EnableNaN)
                            DecimalHalf = UndefinedRep;
                        #else
                            throw "Undefined result(results in undefined expression)";
                        #endif
                    }
                    else
                        BasicUnsignedDivOp(rValue);
                }
                break;
    #if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
    #endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw FlaggedDec::RepTypeAsString(LRep) + " RepType division not supported yet";
                    break;
                }
            }
            else
                RepToRepDivision(LRep, RRep, *this, rValue);
            return *this;
        }

        FlaggedDec DivideByUnsigned(const FlaggedDec& rValue) { FlaggedDec self = *this; self.UnsignedDivOp(rValue); return self; }

        void CatchAllMultiplication(FlaggedDec rValue, RepType LRep, RepType RRep)
        {
            ConvertToNormType(LRep);
            rValue.ConvertToNormType(RRep);
            BasicMultOp(rValue);
        }

        void CatchAllMultiplicationV2(FlaggedDec rValue, RepType SameRep)
        {
            ConvertToNormType(SameRep);
            rValue.ConvertToNormType(SameRep);
            BasicMultOp(rValue);
        }

        void CatchAllMultiplicationV3(FlaggedDec rValue)
        {
            ConvertToNormTypeV2();
            rValue.ConvertToNormTypeV2();
            BasicMultOp(rValue);
        }

    //Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
    #if defined(AltNum_EnableImaginaryNum)
        void CatchAllImaginaryMultiplication(FlaggedDec rValue, RepType LRep, RepType RRep)
        {
           ConvertIRepToNormal(LRep);
           rValue.ConvertToNormalIRep(RRep);
           BasicMultOp(rValue);
           ExtraRep = 0;
           SwapNegativeStatus();
        }

        FlaggedDec CatchAllImaginaryMultiplicationAsCopies(const FlaggedDec& rValue, RepType LRep, RepType RRep)
        { FlaggedDec self = *this; CatchAllImaginaryMultiplication(rValue, LRep, RRep); return self; }
        
        void CatchAllImaginaryMultiplicationV2(FlaggedDec rValue, RepType SameRep)
        {
            ConvertIRepToNormal(SameRep);
            rValue.ConvertToNormalIRep(SameRep);
            BasicMultOp(rValue);
            ExtraRep = 0;
            SwapNegativeStatus();
        }

        FlaggedDec CatchAllImaginaryMultiplicationAsCopiesV2(const FlaggedDec& rValue, RepType SameRep) { FlaggedDec self = *this; CatchAllImaginaryMultiplicationV2(rValue, SameRep); return self; }
    
       void CatchAllImaginaryMultiplicationV3(FlaggedDec rValue)
       {
           ConvertIRepToNormalV2();
           rValue.ConvertToNormalIRepV2();
           BasicMultOp(rValue);
           ExtraRep = 0;
           SwapNegativeStatus();
       }

        FlaggedDec CatchAllImaginaryMultiplicationAsCopiesV3(FlaggedDec& rValue) { FlaggedDec self = *this; CatchAllImaginaryMultiplicationV3(rValue); return self; }
    #endif

protected:

        //NormalType representation multiplied by other representation types
    static void NormalRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
    {
        switch (RRep)
        {
        case RepType::NormalType://Fail safe for when converted before switch
            self.BasicMultOp(Value);
            break;
            #if defined(AltNum_EnablePiRep)
        case RepType::PiNum://X * (Y*Pi)
            self.BasicMultOp(Value);
            self.ExtraRep = FlaggedDec::PiRep;
            break;
            #if defined(AltNum_EnablePiPowers)
        case RepType::PiPower://X * (Y*Pi)^-ExtraRep
            self.ExtraRep = Value.ExtraRep - 1;
            self.BasicMultOp(Value);
            break;
            #endif
            #endif
            #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
            self = FlaggedDec::MixedFracRtRMult_WithNormal(self, Value);
            break;
            #if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
            self = FlaggedDec::MixedPiFracRtRMult_WithNormal(self, Value);
            break;
            #elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE:
            self = FlaggedDec::MixedEFracRtRMult_WithNormal(self, Value);
            break;
            #endif
            #endif
            #if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            self.BasicMultOp(Value);
            self.ExtraRep = FlaggedDec::IRep;
            break;
            #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledIFractionals)
        case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
            #else
        case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
            #endif
            #if defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI:
            #endif
            //Placeholder code(Converting to i rep for now)
            #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
            Value.ConvertToNormalIRep(RRep);
            self.BasicMultOp(Value);
            self.ExtraRep = FlaggedDec::IRep;
            #endif
            #endif
        default:
            self.CatchAllMultiplication(Value, RepType::NormalType, RRep);
            break;
        }
    }

        #if defined(AltNum_EnableFractionals)
        //NumByDivisor representation multiplied by other representation types
        static void NumByDivisorRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType://Later normalize fractional when integer when viable
                self.BasicMultOp(Value);
                break;
                #if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
            case RepType::PiNum:
                #if defined(AltNum_EnableDecimaledPiFractionals)
                self.BasicMultOp(Value);
                self.ExtraRep *= -1;
                #else
                self.CatchAllMultiplication(Value, RepType::NumByDiv, RRep);
                #endif
                break;
                #endif
                #if defined(AltNum_EnableERep)
            case RepType::ENum:
                #if defined(AltNum_EnableDecimaledEFractionals)
                self.BasicMultOp(Value);
                self.ExtraRep *= -1;
                #else
                self.CatchAllMultiplication(Value, RepType::NumByDiv, RRep);
                #endif
                break;
                #endif
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                self = FlaggedDec::MixedFracRtRMult_WithNormal(self, Value);
                break;
                #endif
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
                self = FlaggedDec::MixedPiFracRtRMult_WithNormal(self, Value);
                break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                self = FlaggedDec::MixedEFracRtRMult_WithNormal(self, Value);
                break;
                #endif
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
                self.BasicMultOp(Value);
                self.ExtraRep *= -1;
                #else
                self.BasicMultOp(Value);
                self.ConvertToNormType(RepType::NumByDiv);
                self.ExtraRep = FlaggedDec::IRep;
                #endif
                break;
                #endif
            default:
                self.CatchAllMultiplication(Value, RepType::NumByDiv, RRep);
                break;
            }
        }
        #endif

        #if defined(AltNum_EnablePiRep)
        //PiNum representation multiplied by other representation types
        static void PiNumRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                self.BasicMultOp(Value);
                break;
                #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower://XPi * (Y*Pi)^-ExtraRep
                self.ExtraRep = Value.ExtraRep - 1;
                self.BasicMultOp(Value);
                break;
                #endif
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                self.BasicMultOp(Value);
                self.ExtraRep = FlaggedDec::IRep;
                self.BasicMultOp(FlaggedDec::PiNum);
                break;
                #endif
                #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                #if defined(AltNum_EnableDecimaledPiFractionals)//Convert result to PiNumByDiv
                self.ExtraRep = Value.ExtraRep;
                self.BasicMultOp(Value);
                #elif defined(AltNum_EnablePiFractional)
                if (self.DecimalHalf == 0 && Value.DecimalHalf == 0)//If both left and right side values are whole numbers, convert result into PiFractional
                {//Becoming IntValue/DecimalHalf*Pi Representation
                    self.IntValue *= Value.IntValue;
                    self.DecimalHalf = Value.ExtraRep;
                    self.ExtraRep = PiByDivisorRep;
                }
                else
                {
                    self /= Value.ExtraRep;
                    self.BasicMultOp(Value);
        }
                #else//Else just keep as PiNum type
                self /= Value.ExtraRep;
                self.BasicMultOp(Value);
                #endif
                break;
                #endif
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                self = FlaggedDec::MixedFracRtRMult_WithNormal(self, Value);
                break;
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
                self = FlaggedDec::MixedPiFracRtRMult_WithPiNum(self, Value);
                break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                self = FlaggedDec::MixedEFracRtRMult_WithNormal(self, Value);
                break;
                #endif

                #endif
            default:
                self.CatchAllMultiplication(Value, RepType::PiNum, RRep);
                break;
    }
}
        #endif
        #if defined(AltNum_EnablePiPowers)
        //PiPower representation multiplied by other representation types
        static void PiPowerRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                self.BasicMultOp(Value);
                break;
            case RepType::PiNum://(IntValue.DecimalHalf)*Pi^-ExtraRep representation
                --self.ExtraRep;
                self.BasicMultOp(Value);
                break;
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                self = FlaggedDec::MixedFracRtRMult_WithNormal(self, Value);
                break;
                #endif
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
                self = FlaggedDec::MixedPiFracRtRMult_WithPiPower(self, Value);
                break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                self = FlaggedDec::MixedEFracRtRMult_WithNormal(self, Value);
                break;
                #endif
            default:
                self.CatchAllMultiplication(Value, RepType::PiPower, RRep);
                break;
            }
        }
        #endif
        #if defined(AltNum_EnablePiFractional)
        //PiFractional representation multiplied by other representation types
        static void PiFractionalRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                if (Value.DecimalHalf == 0)
                    self.IntValue *= Value.IntValue;
                else
                    self.CatchAllMultiplication(Value, LRep, RRep);
                break;
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                self = FlaggedDec::MixedFracRtRMult_WithNormal(self, Value);
                break;
                #endif
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
                self = FlaggedDec::MixedPiFracRtRMult_WithOther(self, Value);
                break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                self = FlaggedDec::MixedEFracRtRMult_WithOther(self, Value);
                break;
                #endif
            default:
                self.CatchAllMultiplication(Value, RepType::PiFractional, RRep);
                break;
            }
}
        #endif

        #if defined(AltNum_EnableERep)
        //ENum representation multiplied by other representation types
        static void ENumRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType://Fail safe for when converted before switch
                self.BasicMultOp(Value);
                break;
                #if defined(AltNum_EnablePiRep)
            case RepType::PiNum://Xe * (Y*Pi)
                Value.ConvertPiToNum();
                self.BasicMultOp(Value);
                break;
                #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower://Xe * (Y*Pi)^-ExtraRep
                FlaggedDec Res = Value;
                Res.BasicMultOp(self);
                Res.BasicMultOp(ENum);
                self.SetVal(Res);
                break;
                #endif
                #endif
                #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                #if defined(AltNum_EnableAlternativeRepFractionals)
                #if defined(AltNum_EnableDecimaledEFractionals)//Convert result to PiNumByDiv
                self.ExtraRep = Value.ExtraRep;
                self.BasicMultOp(Value);
                #else
                if (self.DecimalHalf == 0 && Value.DecimalHalf == 0)//If both left and right side values are whole numbers, convert result into EFractional
                {//Becoming IntValue/DecimalHalf*e Representation
                    self.IntValue *= Value.IntValue;
                    self.DecimalHalf = Value.ExtraRep;
                    self.ExtraRep = FlaggedDec::EByDivisorRep;
                }
                else
                {
                    self /= Value.ExtraRep;
                    self.BasicMultOp(Value);
                }
                #endif
                #else//Else just keep as ENum type
                self /= Value.ExtraRep;
                self.BasicMultOp(Value);
                #endif
                break;
                #endif
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                self.BasicMultOp(Value);
                self.ConvertToNormType(RepType::ENum);
                self.ExtraRep = FlaggedDec::IRep;
                break;
                #endif
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                self = FlaggedDec::MixedFracRtRMult_WithNormal(self, Value);
                break;
                #endif
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
                self = FlaggedDec::MixedPiFracRtRMult_WithNormal(self, Value);
                break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                self = FlaggedDec::MixedEFracRtRMult_WithNormal(self, Value);
                break;
                #endif
            default:
                self.CatchAllMultiplication(Value, RepType::ENum, RRep);
                break;
            }
        }
        #endif
        #if defined(AltNum_EnableEFractional)
        //EFractional representation multiplied by other representation types
        static void EFractionalRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                if (Value.DecimalHalf == 0)
                    self.IntValue *= Value.IntValue;
                else
                    self.CatchAllMultiplication(Value, RepType::EFractional, RRep);
                break;
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                self = FlaggedDec::MixedFracRtRMult_WithNormal(self, Value);
                break;
                #endif
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
                self = FlaggedDec::MixedPiFracRtRMult_WithOther(self, Value);
                break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                self = FlaggedDec::MixedEFracRtRMult_WithOther(self, Value);
                break;
                #endif
            default:
                self.CatchAllMultiplication(Value, RepType::EFractional, RRep);
                break;
            }
        }
        #endif

        //PiByDivisor or EByDivisor representation multiplied by other representation types
        static void PiOrEByDivMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType://ToDo:Normalize denom later
                self.BasicMultOp(Value);
                break;
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                self = FlaggedDec::MixedFracRtRMult_WithNormal(self, Value);
                break;
                #endif
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
                self = FlaggedDec::MixedPiFracRtRMult_WithNormal(self, Value);
                break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                self = FlaggedDec::MixedEFracRtRMult_WithNormal(self, Value);
                break;
                #endif
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                self.BasicMultOp(Value);
                #if defined(AltNum_EnableDecimaledPiFractionals)
                self.ConvertToNormType(RepType::PiNumByDiv);
                #else
                self.ConvertToNormType(RepType::ENumByDiv);
                #endif
                self.ExtraRep = FlaggedDec::IRep;
                break;
                #endif
            default:
                Value.ConvertToNormType(RRep);
                self.BasicMultOp(Value);
                break;
            }
        }

        #if defined(AltNum_EnableApproachingValues)
        //ApproachingBottm representation multiplied by other representation types
        static void ApproachingBottomRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                if (self.IntValue = 0 || self.IntValue == FlaggedDec::NegativeRep)
                {
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::NegativeRep : 0;
                    #if defined(AltNum_EnableImaginaryInfinity)
                    self.DecimalHalf = ApproachingImaginaryBottomRep;
                    #else
                    self.DecimalHalf = 1;
                    #endif
                    self.ExtraRep = FlaggedDec::IRep;
                }
                else
                {
                    self.DecimalHalf = 1;
                    self.BasicMultOp(Value);
                    self.ExtraRep = FlaggedDec::IRep;
                }
                break;
                #if defined(AltNum_EnableAlternativeRepFractionals)
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation	
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                if (self.IntValue = 0 || self.IntValue == FlaggedDec::NegativeRep)
                {
                    self.IntValue = self.IntValue == 0 ? FlaggedDec::NegativeRep : 0;
                    #if defined(AltNum_EnableImaginaryInfinity)
                    self.DecimalHalf = ApproachingImaginaryBottomRep;
                    #else
                    self.DecimalHalf = 1;
                    #endif
                    self.ExtraRep = FlaggedDec::IRep;
                }
                else
                {
                    Value.ConvertToNormalIRep(RRep);
                    self.DecimalHalf = 1;
                    self.BasicMultOp(Value);
                    self.ExtraRep = FlaggedDec::IRep;
                }
                break;
                #endif
                #endif
            default:
                self.CatchAllMultiplication(Value, RepType::ApproachingBottom, RRep);
                break;
            }
        }
	        #if !defined(AltNum_DisableApproachingTop)
        //ApproachingTop representation multiplied by other representation types
        static void ApproachingTopRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            //switch (RRep)
            //{
            //  default:
            self.CatchAllMultiplication(Value, RepType::ApproachingTop, RRep);
            //      break;
            //}
        }
            #endif
        #endif
        #if defined(AltNum_EnableApproachingDivided)
        //ApproachingMidLeft representation multiplied by other representation types
        static void ApproachingMidLeftRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            //switch (RRep)
            //{
            //  default:
            self.CatchAllMultiplication(Value, RepType::ApproachingTop, RRep);
            //      break;
            //}
        }
	        #if !defined(AltNum_DisableApproachingTop)
        //ApproachingMidRight representation multiplied by other representation types
        static void ApproachingMidRightRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            //switch (RRep)
            //{
            //  default:
            self.CatchAllMultiplication(Value, RepType::ApproachingTop, RRep);
            //      break;
            //}
        }
            #endif
        #endif

        #if defined(AltNum_EnableImaginaryNum)
        //INum representation multiplied by other representation types
        static void INumRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                self.BasicMultOp(Value);
                break;
            case RepType::INum:
                self.BasicMultOp(Value);
                self.ExtraRep = 0;
                break;
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv:
                #elif defined(AltNum_EnableIFractionals)
            case RepType::IFractional:
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                Value.ConvertToNormalIRep(RRep);
                self.ExtraRep = 0;
                self.BasicMultOp(-Value);
                break;
            default:
                Value.ConvertToNormType(RRep);
                self.BasicMultOp(Value);
                break;
            }
        }
        #endif
        #if defined(AltNum_EnableDecimaledIFractionals)
        //INumByDivisor representation multiplied by other representation types
        static void INumByDivisorRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                self.BasicMultOp(Value);
                break;
            case RepType::INum://Convert result into NumByDiv
                self.BasicMultOp(-Value);
                self.ExtraRep *= -1;
                break;
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                Value.ConvertToNormalIRep(RepType::MixedI);
                self.BasicMultOp(-Value);
                self.ExtraRep *= -1;
                break;
                #endif
            default:
                Value.ConvertToNormType(&RRep);
                self.BasicMultOp(Value);
            }
}
        #elif defined(AltNum_EnableIFractional)
        //IFractional representation multiplied by other representation types
        static void IFractionalRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType:
                self.ConvertToNormalIRep(RepType::IFractional);
                self.BasicMultOp(Value);
                break;
            case RepType::INum://Convert result into NumByDiv
                self.ExtraRep = self.DecimalHalf;
                self.DecimalHalf = 0;
                self.BasicMultOp(-Value);
                break;
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                Value.ConvertToNormalIRep(RepType::MixedI);
                self.ExtraRep = self.DecimalHalf;
                self.DecimalHalf = 0;
                self.BasicMultOp(-Value);
                break;
                #endif
            default:
                Value.ConvertToNormType(RRep);
                self.BasicMultOp(Value);
            }
        }
        #endif

#if defined(AltNum_EnableApproachingI)

        //ApproachingImaginaryBottom representation multiplied by other representation types
        static void ApproachingImaginaryBottomRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv:
                #elif defined(AltNum_EnableIFractionals)
            case RepType::IFractional:
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                Value.ConvertIRepToNormal(RRep);
                self.ConvertIRepToNormal(RepType::ApproachingImaginaryBottom);
                self.ExtraRep = 0;
                self.BasicMultOp(-Value);
                break;
              default:
                  self.BasicMultOp(Value);
                  break;
            }
        }
    #if !defined(AltNum_DisableApproachingTop)
        //ApproachingImaginaryTop representation multiplied by other representation types
        static void ApproachingImaginaryTopRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv:
                #elif defined(AltNum_EnableIFractionals)
            case RepType::IFractional:
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                Value.ConvertIRepToNormal(RRep);
                self.ConvertIRepToNormal(RepType::ApproachingImaginaryTop);
                self.ExtraRep = 0;
                self.BasicMultOp(-Value);
                break;
              default:
                  self.BasicMultOp(Value);
                  break;
            }
        }
    #endif
    #if defined(AltNum_EnableApproachingDivided)
        //ApproachingImaginaryMidLeft representation multiplied by other representation types
        static void ApproachingImaginaryMidLeftRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv:
                #elif defined(AltNum_EnableIFractionals)
            case RepType::IFractional:
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                Value.ConvertIRepToNormal(RRep);
                self.ConvertIRepToNormal(RepType::ApproachingImaginaryMidLeft);
                self.ExtraRep = 0;
                self.BasicMultOp(-Value);
                break;
              default:
                  self.BasicMultOp(Value);
                  break;
            }
        }
	    #if !defined(AltNum_DisableApproachingTop)
        //ApproachingImaginaryMidRight representation multiplied by other representation types
        static void ApproachingImaginaryMidRightRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv:
                #elif defined(AltNum_EnableIFractionals)
            case RepType::IFractional:
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                Value.ConvertIRepToNormal(RRep);
                self.ConvertIRepToNormal(RepType::ApproachingImaginaryMidRight);
                self.ExtraRep = 0;
                self.BasicMultOp(-Value);
                break;
              default:
                  self.BasicMultOp(Value);
                  break;
            }
        }
        #endif
    #endif
#endif

        #if defined(AltNum_EnableMixedFractional)
        //MixedFraction representation multiplied by other representation types
        static void MixedFracRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                /* //Unfinished code
                case RepType::NormalType:
                if(Value.DecimalHalf==0)
                {
                self.IntValue *= Value.IntValue;
                self.DecimalHalf *= Value.IntValue;
                int invertedDecimalHalf = -self.DecimalHalf
                if(invertedDecimalHalf >= self.ExtraRep)
                {
                int divRes = invertedDecimalHalf / self.ExtraRep;
                int C = invertedDecimalHalf - self.ExtraRep * divRes;
                if(C==0)//If no longer a mixed fraction so instead convert into NormalType
                {
                throw "ToDo:Finish this code later";
                }
                else
                {
                throw "ToDo:Finish this code later";
                }
                }
                else
                {
                throw "ToDo:Finish this code later";
                }
                }
                else//Convert result as NumByDiv
                {
                throw "ToDo:Finish this code later";
                }
                */
            case RepType::NormalType:
                #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                #endif
                #endif
                #if defined(AltNum_EnableERep)
            case RepType::ENum:
                #endif
                self = FlaggedDec::MixedFracRtRMult_WithNormal(Value, self);
                break;
                //ToDo::replace Mixed Pi/E code with more precise code later
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi:
                FlaggedDec::MixedPiFracRtRMult_WithOther(Value, self);
                break;
                #else
            case RepType::MixedE:
                FlaggedDec::MixedEFracRtRMult_WithOther(Value, self);
                break;
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                throw "ToDo: Add code here later";
                break;
                #endif
            default:
                self.CatchAllMultiplication(Value, RepType::MixedFrac, RRep);
                break;
            }
        }
        #endif
        #if defined(AltNum_MixedPiOrEEnabled)
        //MixedPiFraction or MixedEFraction representation multiplied by other representation types
        static void MixedPiOrERtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                /*
                case RepType::NormalType:
                if(Value.DecimalHalf==0)
                {
                self.IntValue *= Value.IntValue;
                self.DecimalHalf *= Value.IntValue;
                if(self.DecimalHalf <= self.ExtraRep)
                {//(-4 - -3)*1 = -1;
                int divRes = self.DecimalHalf / self.ExtraRep;
                int C = self.DecimalHalf - self.ExtraRep * divRes;
                if(C==0)//If no longer a mixed fraction so instead convert into PiNum/ENum
                {
                self.DecimalHalf = 0;
                #if defined(AltNum_EnableMixedPiFractional)
                self.ExtraRep = PiRep;
                #else
                self.ExtraRep = ERep;
                #endif
                if(self.IntValue<0)
                self.IntValue -= divRes;
                else
                self.IntValue += divRes;
                }
                else
                {
                throw "ToDo:Finish this code later";
                }
                }
                }
                else//Convert result as NumByDiv, PiNumByDiv, or ENumByDiv
                {
                throw "ToDo:Finish this code later";
                }
                break;
                */
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::NormalType:
                #if defined(AltNum_EnableERep)
            case RepType::ENum:
                #endif
                self = FlaggedDec::MixedPiFracRtRMult_WithNormal(Value, self);
                break;
                #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                self = FlaggedDec::MixedPiFracRtRMult_WithPiNum(Value, self);
                break;
                #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                self = FlaggedDec::MixedPiFracRtRMult_WithPiPower(Value, self);
                break;
                #endif
                #endif
                #if defined(AltNum_EnablePiFractional)
            case RepType::PiFractional:
                MixedPiFracRtRMult_WithOther(Value, self);
                break;
                #endif
                #if defined(AltNum_EnableEFractional)
            case RepType::EFractional:
                FlaggedDec::MixedPiFracRtRMult_WithOther(Value, self);
                break;
                #endif
                #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                #elif defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                #endif
                FlaggedDec::MixedPiFracRtRMult_WithNormal(Value, self);
                break;
                #endif
                #else
            case RepType::NormalType:
                #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                #endif
                #endif
                #if defined(AltNum_EnableERep)
            case RepType::ENum:
                #endif
                self = FlaggedDec::MixedEFracRtRMult_WithNormal(Value, self);
                break;
                #if defined(AltNum_EnablePiFractional)
            case RepType::PiFractional:
                MixedEFracRtRMult_WithOther(Value, self);
                break;
                #endif
                #if defined(AltNum_EnableEFractional)
            case RepType::EFractional:
                MixedEFracRtRMult_WithOther(Value, self);
                break;
                #endif
                #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                #elif defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                #endif
                MixedEFracRtRMult_WithNormal(Value, self);
                break;
                #endif
                #endif
            default:
                #if defined(AltNum_EnableMixedPiFractional)
                self.ConvertToNormType(RepType::MixedPi);
                #else
                self.ConvertToNormType(RepType::MixedE);
                #endif
                Value.ConvertToNormType(RRep);
                self.BasicMultOp(Value);
    }
}
        #elif defined(AltNum_EnableMixedIFractional)
        //MixedIFraction representation multiplied by other representation types
        static void MixedIRtRMultiplication(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv:
                #elif defined(AltNum_EnableFractionals)
            case RepType::IFractional:
                #endif
                self.CatchAllImaginaryMultiplication(Value, LRep, RRep);
            default:
                self.ConvertToNormalIRep(RepType::MixedI);
                Value.ConvertToNormType(RRep);
                self.BasicMultOp(Value);
            }
}
        #endif

        static void RepToRepMultiplication(RepType& LRep, RepType& RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            //LRep Overrides
            #if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)
            switch (LRep)
            {
                #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                throw "UndefinedButInRange MultOperations not supported yet(from left side)"; return;
                break;
                #endif
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                #if defined(AltNum_EnableAlternativeRepFractionals)
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
                #else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                FlaggedDec::Multiplication_LRepImaginaryOverride(RRep, Value);
                break;
                #endif
            default:
                break;
            }
            #endif
            //RRep Overrides before Main RtR Code
            switch (RRep)
            {
                #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
            {
                if (Value.IntValue == 0)
                {
                    self.IntValue = self.IntValue < 0 ? NegativeRep : 0;
                    self.DecimalHalf = ApproachingBottomRep;
                    self.ExtraRep = 0;
                    return;
                }
                else
                {
                    Value.DecimalHalf = 1;
                    RRep = RepType::NormalType;
                }
            }
            break;

            case RepType::ApproachingTop:
            {
                Value.DecimalHalf = 999999999;
                Value.ExtraRep = 0;
                RRep = RepType::NormalType;
            }
            break;
            #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                Value.ConvertToNormType(RepType::ApproachingMidLeft);
                RRep = RepType::NormalType;
                break;
            case RepType::ApproachingMidRight:
                Value.ConvertToNormType(RepType::ApproachingMidRight);
                RRep = RepType::NormalType;
                break;
                #endif
                #endif
                #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                throw "UndefinedButInRange operations not supported yet(from right side)";
                break;
                #endif
                #if defined(AltNum_EnableNaN)
            case RepType::Undefined:
            case RepType::NaN:
                throw "Can't perform operations with NaN or Undefined number";
                break;
                #endif
            default:
                break;
            }
            switch (LRep)//Main switch block starts here
            {
            case RepType::NormalType:
                NormalRtRMultiplication(RRep, self, Value); break;
                #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                PiNumRtRMultiplication(RRep, self, Value); break;
                #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                PiPowerRtRMultiplication(RRep, self, Value); break;
                #endif
                #endif
                #if defined(AltNum_EnableERep)
            case RepType::ENum:
                ENumRtRMultiplication(RRep, self, Value); break;
                #endif
                #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
                ApproachingBottomRtRMultiplication(RRep, self, Value); break;
                #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTop:
                self.CatchAllMultiplication(Value, RepType::ApproachingTop, RRep); break;
                //ApproachingTopRtRMultiplication(RRep, self, Value); break;
                #endif
                #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                //ApproachingMidLeftRtRMultiplication(RRep, self, Value); break;
                #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingMidRight:
                //ApproachingMidRightRtRMultiplication(RRep, self, Value); break;
                #endif
                self.CatchAllMultiplication(Value, LRep, RRep); break;
                #endif
                #endif
                #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                NumByDivisorRtRMultiplication(RRep, self, Value); break;
                #if defined(AltNum_EnableAlternativeRepFractionals)
                #if defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                PiFractionalRtRMultiplication(RRep, self, Value); break;
                #endif
                #if defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                EFractionalRtRMultiplication(RRep, self, Value); break;
                #endif

                #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                PiOrEByDivMultiplication(RRep, self, Value); break;
                #elif defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                PiOrEByDivMultiplication(RRep, self, Value); break;
                #endif
                #endif
                #endif
                #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                INumRtRMultiplication(RRep, self, Value); break;
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                INumByDivisorRtRMultiplication(RRep, self, Value); break;
                #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                IFractionalRtRMultiplication(RRep, self, Value); break;
                #endif
                #endif
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
                MixedFracRtRMultiplication(RRep, self, Value); break;
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
                MixedPiOrERtRMultiplication(RRep, self, Value); break;
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                MixedPiOrERtRMultiplication(RRep, self, Value); break;
                #elif defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                MixedIRtRMultiplication(RRep, self, Value); break;
                #endif
                #endif
                #if defined(AltNum_EnableNaN)
            case RepType::Undefined:
            case RepType::NaN:
                throw "Can't perform RtRMultiplicationerations with NaN or Undefined number";
                break;
                #endif
            default:
                throw FlaggedDec::RepTypeAsString(LRep) + " RepType multiplication with" + FlaggedDec::RepTypeAsString(RRep) + "not supported yet";
                break;
            }
        }

public:

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rightSideValue.">The right side rValue</param>
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& MultOp(const FlaggedDec& rightSideValue)
        {
            if (IsZero() || rightSideValue == FlaggedDec::One)
                return *this;
            if (rightSideValue.IntValue==0&&rightSideValue.DecimalHalf==0) { SetAsZero(); return *this; }
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
            {
                if(rightSideValue<0)
                    IntValue *= -1;
                return *this;
            }
#endif
            RepType LRep = GetRepType();
            RepType RRep = rightSideValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicMultOp(rightSideValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
                    BasicMultOp(rightSideValue);
#if defined(AltNum_EnablePiPowers)
                    ExtraRep = -2;
#else
                    BasicMultOp(PiNum);
#endif
                    break;
#endif
#if defined(AltNum_EnableERep)
                case RepType::ENum:
                    BasicMultOp(rightSideValue);
                    BasicMultOp(ENum);
                    break;
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum://Xi * Yi = -XY
                {
                    ExtraRep = 0;
                    FlaggedDec rValue = rightSideValue;
                    rValue.SwapNegativeStatus();
                    BasicMultOp(rValue);
                }
                break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    ExtraRep += rightSideValue.ExtraRep;
                    BasicMultOp(rightSideValue);
                    break;
#endif

#if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                {
                    if (IntValue == NegativeRep)
                    {
                        if(rightSideValue<0)
                            IntValue = 0;
                    }
                    else if (IntValue == 0)
                    {
                        if(rightSideValue<0)
                            IntValue = NegativeRep;
                    }
                    else if (IntValue < 0)
                    {
                        if(rightSideValue<0)
                        {
                            if (rightSideValue.IntValue == NegativeRep)//-1.0..1 * -0.0..1
                                IntValue = 0;
                            else//-1.0..1 * -2.0..1
                                IntValue *= rightSideValue.IntValue;
                        }
                        else
                        {
                            if (rightSideValue.IntValue == 0)//-1.0..1 * 0.0..1
                                IntValue = NegativeRep;
                            else//-1.0..1 * 2.0..1
                                IntValue *= rightSideValue.IntValue;
                        }
                    }
                    else
                    {
                        if(rightSideValue<0)
                        {
                            if (rightSideValue.IntValue == NegativeRep)//1.0..1 * -0.0..1
                                IntValue = NegativeRep;
                            else//1.0..1 * 2.0..1
                                IntValue *= rightSideValue.IntValue;
                        }
                        else
                        {
                            if (rightSideValue.IntValue == 0)//1.0..1 * 0.0..1
                                IntValue = 0;
                            else//1.0..1 * 2.0..1
                                IntValue *= rightSideValue.IntValue;
                        }
                    }
                    return *this;
                }
                break;
                case RepType::ApproachingTop://Just going to convert into normal numbers for now
                {
                    CatchAllMultiplicationV2(rightSideValue, LRep);
                }
                break;

    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                {
                    if(rightSideValue.IntValue==NegativeRep)
                    {
                        if (IntValue == 0
                        {
                            IntValue = NegativeRep;
                            ExtraRep *= rightSideValue.ExtraRep;
                        )
                        else if(IntValue == NegativeRep)
                        {
                            IntValue = 0;
                            ExtraRep *= rightSideValue.ExtraRep;
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rightSideValue, LRep);
                        }
                    }
                    if (rightSideValue.IntValue == 0)
                    {
                        //-0.49..9 * 0.49..9 =  ~-0.249..9 (IntValue:0 DecimalHalf:ApproachingrightSideValue.Rep ExtraRep:4)
                        //0.49..9 * 0.49..9(IntValue:0 DecimalHalf:ApproachingrightSideValue.Rep ExtraRep:2)
                        // =  ~0.249..9 (IntValue:0 DecimalHalf:ApproachingrightSideValue.Rep ExtraRep:4)
                        // 0.249..9 * 0.249..9 = ~0.06249..9(IntValue:0 DecimalHalf:ApproachingrightSideValue.Rep ExtraRep:16)
                        if (IntValue == 0 || IntValue == NegativeRep)
                            ExtraRep *= rightSideValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rightSideValue, LRep);
                        }
                    }
                    else if(rightSideValue<0)
                    {
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0? -IntValue;
                        IsNegative = !IsNegative;
                        int InvertedrightSideValue = -(int)rightSideValue.IntValue;
                        int XZ = IntValue * InvertedrightSideValue;
                        FlaggedDec XV = SetAsApproachingMid(0, rightSideValue.ExtraRep) * IntValue;
                        FlaggedDec YZ = SetAsApproachingMid(0, ExtraRep) * InvertedrightSideValue;
                        FlaggedDec YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rightSideValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        int XZ = IntValue * rightSideValue.IntValue;
                        FlaggedDec XV = SetAsApproachingMid(0, rightSideValue.ExtraRep) * IntValue;
                        FlaggedDec YZ = SetAsApproachingMid(0, ExtraRep) * rightSideValue.IntValue;
                        FlaggedDec YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rightSideValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                    break;
                }
                case RepType::ApproachingMidRight:
                {
                    if(rightSideValue.IntValue==NegativeRep)
                    {
                        if (IntValue == 0)
                        {
                            IntValue = NegativeRep;
                            ExtraRep *= rightSideValue.ExtraRep;
                        )
                        else if(IntValue == NegativeRep)
                        {
                            IntValue = 0;
                            ExtraRep *= rightSideValue.ExtraRep;
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rightSideValue, LRep);
                        }
                    }
                    else if (rightSideValue.IntValue == 0)
                    {
                        if (IntValue == 0 || IntValue == NegativeRep)
                            ExtraRep *= rightSideValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rightSideValue, LRep);
                        }
                    }
                    else if(rightSideValue.IntValue<0)
                    {
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        IsNegative = !IsNegative;
                        int InvertedrightSideValue = -(int)rightSideValue.IntValue;
                        int XZ = IntValue * InvertedrightSideValue;
                        FlaggedDec XV = SetAsApproachingMid(0, rightSideValue.ExtraRep) * IntValue;
                        FlaggedDec YZ = SetAsApproachingMid(0, ExtraRep) * InvertedrightSideValue;
                        FlaggedDec YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rightSideValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        int XZ = IntValue * rightSideValue.IntValue;
                        FlaggedDec XV = SetAsApproachingMid(0, rightSideValue.ExtraRep) * IntValue;
                        FlaggedDec YZ = SetAsApproachingMid(0, ExtraRep) * rightSideValue.IntValue;
                        FlaggedDec YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rightSideValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                }
                break;
    #endif
#endif

#if defined(AltNum_EnableFractionals)//Unfinished code
                case RepType::NumByDiv://(FlaggedDec(IntValue,DecimalHalf))/ExtraRep
                {
                    //((FlaggedDec(IntValue,DecimalHalf))/ExtraRep) * (FlaggedDec(rightSideValue.IntValue,rightSideValue.DecimalHalf))/rightSideValue.ExtraRep) = 
                    //((FlaggedDec(IntValue,DecimalHalf))*FlaggedDec(rightSideValue.IntValue,rightSideValue.DecimalHalf)))/(ExtraRep*rightSideValue.ExtraRep)
                    BasicMultOp(rightSideValue);
                    ExtraRep *= rightSideValue.ExtraRep;
                }
                break;
#endif
    //(IntValue*rightSideValue.IntValue)*Pi^2/(DecimalHalf*rightSideValue.DecimalHalf)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                {//Become either PiNum, NumByDiv, or PiPower
        #if defined(AltNum_EnablePiPowers)//Becoming PiPower
                    IntValue *= rightSideValue.IntValue;
                    int divisor = DecimalHalf*rightSideValue.DecimalHalf;
                    DecimalHalf = 0;
                    BasicUIntDiv(divisor);
                    ExtraRep = -2;//Pi^2
        #else//Becoming PiNum
                    IntValue *= rightSideValue.IntValue;
                    DecimalHalf *= rightSideValue.DecimalHalf;
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicMultOp(PiNum);
                    BasicUIntDivOp(divisor);
                    ExtraRep = PiRep;
        #endif
                }
                break;
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                {
                    IntValue *= rightSideValue.IntValue;
                    DecimalHalf *= rightSideValue.DecimalHalf;
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicMultOp(ENum);
                    BasicUIntDivOp(divisor);
                    ExtraRep = ERep;
                }
                break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                {
                    //(IntValue/DecimalHalf)i*(rightSideValue.IntValue/rightSideValue.DecimalHalf)i
                    //==-1*(IntValue/DecimalHalf)*(rightSideValue.IntValue/rightSideValue.DecimalHalf)
                    int NumRes = -IntValue * rightSideValue.IntValue;
                    int DenomRes = DecimalHalf * rightSideValue.DecimalHalf;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
    #endif

    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rightSideValue/-ExtraRep)*Pi Representation
                    //(rightSideValue/(-ExtraRep))*Pi * (rightSideValue.rightSideValue/(-rightSideValue.ExtraRep))*Pi
        #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rightSideValue/-ExtraRep)*e Representation
        #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rightSideValue/-ExtraRep)*i Representation
                    //(rightSideValue/(-ExtraRep))*i * (rightSideValue.rightSideValue/(-rightSideValue.ExtraRep))*i
        #endif
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    ExtraRep *= -rValue.ExtraRep;
                    BasicUnsignedMultOp(rValue);
            #if defined(AltNum_EnableDecimaledPiFractionals)
                #ifdef AltNum_EnablePiPowers//Convert to PiPower representation
                    BasicUnsignedDivOp(-ExtraRep);
                    ExtraRep = -2;
                #else
                    BasicUnsignedMultOp(PiNum);
                #endif
            #elif defined(AltNum_EnableDecimaledEFractionals)
                    BasicUnsignedMultOp(ENum);
            #else
                    SwapNegativeStatus();
            #endif
                }
                break;
    #endif
    #if defined(AltNum_EnableMixedFractional) //Turn MixedFrac into fractional and then apply
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    //IntValue +- (-DecimalHalf/ExtraRep) = 
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                    int RightSideNum = rValue.IntValue == 0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
        #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;

                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes PiNumByDiv
                    //And then multiply by Pi
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #elif defined(AltNum_EnablePiPowers)
                    //Or convert to PiPower (of 2)
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #else
                    //Or convert PiNum and multiply by Pi
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicMultOp(PiNum);
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #endif
                }
                break;

        #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes ENumByDiv
                    //And then multiply by e
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #else
                    //Or convert ENum and multiply by e
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicMultOp(PiNum);
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #endif
                }
                break;
        #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                {
                    FlaggedDec rValue = rightSideValue;
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = -DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = DecimalHalf;
                    else
                        LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
        #endif
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rightSideValue.DecimalHalf == InfinityRep)
                        DecimalHalf = InfinityRep;
                    else
                        BasicMultOp(rightSideValue);
                }
                break;
        #if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif
                default:
                    throw FlaggedDec::RepTypeAsString(LRep) + " RepType multiplication not supported yet";
                    break;
                }
            }
            else
            {
                FlaggedDec rValue = rightSideValue;
                if(rValue<0)
                {
                    rValue *= -1;
                    SwapNegativeStatus();
                }
                RepToRepMultiplication(LRep, RRep, *this, rValue);
            }
            return *this;
        }

        FlaggedDec MultiplyBy(const FlaggedDec& rValue) { FlaggedDec self = *this; self.MultOp(rValue); return self; }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>FlaggedDec&</returns>
        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& UnsignedMultOp(FlaggedDec rValue)
        {
            if (rValue.IntValue==0&&rValue.DecimalHalf==0) { SetAsZero(); return *this; }
            if (IsZero() || rValue == FlaggedDec::One)
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
            {
                if(rValue<0)
                    IntValue *= -1;
                return *this;
            }
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                    case RepType::NormalType:
                        BasicUnsignedMultOp(rValue);
                        break;
    #if defined(AltNum_EnablePiRep)
                    case RepType::PiNum:
                        BasicUnsignedMultOp(rValue);
    #if defined(AltNum_EnablePiPowers)
                        ExtraRep = -2;
    #else
                        BasicUnsignedMultOp(PiNum);
    #endif
                        break;
    #endif
    #if defined(AltNum_EnableERep)
                    case RepType::ENum:
                        BasicUnsignedMultOp(rValue);
                        BasicUnsignedMultOp(ENum);
                        break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                    case RepType::INum://Xi * Yi = -XY
                        ExtraRep = 0;
                        BasicUnsignedMultOp(-rValue);
                        break;
    #endif

    #if defined(AltNum_EnablePiPowers)
                    case RepType::PiPower:
                        ExtraRep += rValue.ExtraRep;
                        BasicUnsignedMultOp(rValue);
                        break;
    #endif

    #if defined(AltNum_EnableApproachingValues)
                    case RepType::ApproachingBottom:
                    {
                        if (IntValue < 0)
                        {
                            if (rValue.IntValue == 0)//-1.0..1 * 0.0..1
                                IntValue = NegativeRep;
                            else//-1.0..1 * 2.0..1
                                IntValue *= rValue.IntValue;
                        }
                        else if (IntValue != NegativeRep||IntValue!=0)
                        {
                            if (rValue.IntValue == 0)//1.0..1 * 0.0..1
                                IntValue = 0;
                            else//1.0..1 * 2.0..1
                                IntValue *= rValue.IntValue;
                        }
                        return *this;
                    }
                    break;
                    case RepType::ApproachingTop://Just going to convert into normal numbers for now
                    {
                        CatchAllMultiplicationV2(rValue, LRep);
                    }
                    break;

        #if defined(AltNum_EnableApproachingDivided)
                    case RepType::ApproachingMidLeft:
                    {
                        if (rValue.IntValue == 0)
                        {
                            //-0.49..9 * 0.49..9 =  ~-0.249..9 (IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                            //0.49..9 * 0.49..9(IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:2)
                            // =  ~0.249..9 (IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                            // 0.249..9 * 0.249..9 = ~0.06249..9(IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:16)
                            if (IntValue == 0 || IntValue == NegativeRep)
                                ExtraRep *= rValue.ExtraRep;
                            else
                            {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                                CatchAllMultiplicationV2(rValue, LRep);
                            }
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            bool IsNegative = IntValue<0;
                            if (IsNegative)
                                IntValue = IntValue == NegativeRep:0 ? -IntValue;
                            int XZ = IntValue * rValue.IntValue;
                            FlaggedDec XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                            FlaggedDec YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntValue;
                            FlaggedDec YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                            XV += XZ;
                            XV += YZ + YV;
                            if (IsNegative)
                                IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                            DecimalHalf = XV.DecimalHalf;
                            ExtraRep = XV.ExtraRep;
                        }
                        return *this;
                        break;
                    }
                    case RepType::ApproachingMidRight:
                    {
                        if (rValue.IntValue == 0)
                        {
                            if (IntValue == 0 || IntValue == NegativeRep)
                                ExtraRep *= rValue.ExtraRep;
                            else
                            {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                                CatchAllMultiplicationV2(rValue, LRep);
                            }
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            bool IsNegative = IntValue<0;
                            if (IsNegative)
                                IntValue = IntValue == NegativeRep:0 ? -IntValue;
                            int XZ = IntValue * rValue.IntValue;
                            FlaggedDec XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                            FlaggedDec YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntValue;
                            FlaggedDec YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                            XV += XZ;
                            XV += YZ + YV;
                            if (IsNegative)
                                IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                            DecimalHalf = XV.DecimalHalf;
                            ExtraRep = XV.ExtraRep;
                        }
                        return *this;
                    }
                    break;
        #endif
    #endif

    #if defined(AltNum_EnableFractionals)//Unfinished code
                    case RepType::NumByDiv://(FlaggedDec(IntValue,DecimalHalf))/ExtraRep
                    {
                        //((FlaggedDec(IntValue,DecimalHalf))/ExtraRep) * (FlaggedDec(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
                        //((FlaggedDec(IntValue,DecimalHalf))*FlaggedDec(rValue.IntValue,rValue.DecimalHalf)))/(ExtraRep*rValue.ExtraRep)
                        BasicUnsignedMultOp(rValue);
                        ExtraRep *= rValue.ExtraRep;
                    }
                    break;
    #endif
                    //(IntValue*rValue.IntValue)*Pi^2/(DecimalHalf/rValue.DecimalHalf)
        #if defined(AltNum_EnablePiFractional)
                    case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                    {
            #if defined(AltNum_EnablePiPowers)//Becoming PiPower
                        IntValue *= rValue.IntValue;
                        int divisor = DecimalHalf*rValue.DecimalHalf;
                        DecimalHalf = 0;
                        BasicUIntDiv(divisor);
                        ExtraRep = -2;//Pi^2
            #else//Becoming PiNum
                        IntValue *= rValue.IntValue;
                        DecimalHalf *= rValue.DecimalHalf;
                        int divisor = DecimalHalf;
                        DecimalHalf = 0;
                        BasicMultOp(PiNum);
                        BasicUIntDivOp(divisor);
                        ExtraRep = PiRep;
            #endif
                    }
                    break;
        #endif
        #if defined(AltNum_EnableEFractional)
                    case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                    {
                        IntValue *= rValue.IntValue;
                        DecimalHalf *= rValue.DecimalHalf;
                        int divisor = DecimalHalf;
                        DecimalHalf = 0;
                        BasicMultOp(ENum);
                        BasicUIntDivOp(divisor);
                        ExtraRep = ERep;
                    }
                    break;
        #endif
        #if defined(AltNum_EnableImaginaryNum)
                    case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    {
                        //(IntValue/DecimalHalf)i*(rValue.IntValue/rValue.DecimalHalf)i
                        //==-1*(IntValue/DecimalHalf)*(rValue.IntValue/rValue.DecimalHalf)
                        int NumRes = -IntValue * rValue.IntValue;
                        int DenomRes = DecimalHalf * rValue.DecimalHalf;
                        signed int divRes = NumRes / DenomRes;
                        signed int RemRes = NumRes - DenomRes * NumRes;
                        DecimalHalf = 0;
                        if (RemRes == 0)
                        {
                            IntValue = divRes;
                            ExtraRep = 0;
                        }
                        else
                        {
                            IntValue = NumRes;
                            ExtraRep = DenomRes;
                        }
                    }
                    break;
        #endif
        #if defined(AltNum_EnableDecimaledAlternativeFractionals)
            #if defined(AltNum_EnableDecimaledPiFractionals)
                    case RepType::PiNumByDiv://  (rValue/-ExtraRep)*Pi Representation
                        //(rValue/(-ExtraRep))*Pi * (rValue.rValue/(-rValue.ExtraRep))*Pi
            #elif defined(AltNum_EnableDecimaledEFractionals)
                    case RepType::ENumByDiv://(rValue/-ExtraRep)*e Representation
            #elif defined(AltNum_EnableDecimaledIFractionals)
                    case RepType::INumByDiv://(rValue/-ExtraRep)*i Representation
                        //(rValue/(-ExtraRep))*i * (rValue.rValue/(-rValue.ExtraRep))*i
            #endif
                    {
                        ExtraRep *= -rValue.ExtraRep;
                        BasicUnsignedMultOp(rValue);
                #if defined(AltNum_EnableDecimaledPiFractionals)
                    #ifdef AltNum_EnablePiPowers//Convert to PiPower representation
                        BasicUnsignedDivOp(-ExtraRep);
                        ExtraRep = -2;
                    #else
                        BasicUnsignedMultOp(PiNum);
                    #endif
                #elif defined(AltNum_EnableDecimaledEFractionals)
                        BasicUnsignedMultOp(ENum);
                #else
                        SwapNegativeStatus();
                #endif
                    }
                    break;
        #endif
        #if defined(AltNum_EnableMixedFractional)//Turn MixedFrac into fractional and then apply
                    case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                    {
                        //IntValue +- (-DecimalHalf/ExtraRep) = 
                        int LeftSideNum;
                        if (IntValue == NegativeRep)
                            LeftSideNum = DecimalHalf;
                        else if (IntValue < 0)
                            LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                        else if (IntValue == 0)
                            LeftSideNum = -DecimalHalf;
                        else
                            LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                        int RightSideNum = rValue.IntValue == 0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        //Becomes NumByDiv now
                        IntValue = LeftSideNum * RightSideNum;
                        DecimalHalf = 0;
                        ExtraRep = ExtraRep * -rValue.ExtraRep;
                    }
                    break;
            #if defined(AltNum_EnableMixedPiFractional)
                    case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                    {
                        int LeftSideNum;
                        if (IntValue == NegativeRep)
                            LeftSideNum = DecimalHalf;
                        else if (IntValue < 0)
                            LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                        else if (IntValue == 0)
                            LeftSideNum = -DecimalHalf;
                        else
                            LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                        break;
                        int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;

                        #if defined(AltNum_EnableDecimaledEFractionals)
                        //Becomes PiNumByDiv
                        //And then multiply by Pi
                        IntValue = LeftSideNum * RightSideNum;
                        DecimalHalf = 0;
                        ExtraRep = ExtraRep * rValue.ExtraRep;
                        #elif defined(AltNum_EnablePiPowers)
                        //Or convert to PiPower (of 2)
                        IntValue = LeftSideNum * RightSideNum;
                        DecimalHalf = 0;
                        ExtraRep = -2;
                        BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                        #else
                        //Or convert PiNum and multiply by Pi
                        IntValue = LeftSideNum * RightSideNum;
                        DecimalHalf = 0;
                        ExtraRep = -2;
                        BasicUnsignedMultOp(PiNum);
                        BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                        #endif
                    }
                    break;

            #elif defined(AltNum_EnableMixedEFractional)
                    case RepType::MixedE:
                    {
                        int LeftSideNum;
                        if (IntValue == NegativeRep)
                            LeftSideNum = DecimalHalf;
                        else if (IntValue < 0)
                            LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                        else if (IntValue == 0)
                            LeftSideNum = -DecimalHalf;
                        else
                            LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                        int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                        #if defined(AltNum_EnableDecimaledEFractionals)
                        //Becomes ENumByDiv
                        //And then multiply by e
                        IntValue = LeftSideNum * RightSideNum;
                        DecimalHalf = 0;
                        ExtraRep = ExtraRep * rValue.ExtraRep;
                        #else
                        //Or convert ENum and multiply by e
                        IntValue = LeftSideNum * RightSideNum;
                        DecimalHalf = 0;
                        ExtraRep = -2;
                        BasicUnsignedMultOp(PiNum);
                        BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                        #endif
                    }
                    break;
            #elif defined(AltNum_EnableMixedIFractional)
                    case RepType::MixedI:
                    {
                        int LeftSideNum;
                        if (IntValue == NegativeRep)
                            LeftSideNum = -DecimalHalf;
                        else if (IntValue < 0)
                            LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                        else if (IntValue == 0)
                            LeftSideNum = DecimalHalf;
                        else
                            LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                        break;
                        int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                        //Becomes NumByDiv now
                        IntValue = LeftSideNum * RightSideNum;
                        DecimalHalf = 0;
                        ExtraRep = ExtraRep * -rValue.ExtraRep;
                    }
                    break;
            #endif
        #endif
        #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                    case RepType::UndefinedButInRange:
                    {
                        if (rValue.DecimalHalf == InfinityRep)
                            DecimalHalf = InfinityRep;
                        else
                            BasicUnsignedMultOp(rValue);
                    }
                    break;
            #if defined(AltNum_EnableWithinMinMaxRange)
                    case RepType::WithinMinMaxRange:
                        throw "Uncertain how to perform operation with unbalanced ranged";
                        break;
            #endif
        #endif
        #if defined(AltNum_EnableNaN)
                    case RepType::Undefined:
                    case RepType::NaN:
                        throw "Can't perform operations with NaN or Undefined number";
                        break;
        #endif
                    default:
                        throw FlaggedDec::RepTypeAsString(LRep) + " RepType multiplication not supported yet";
                        break;
                }
            }
            else
            {
                FlaggedDec rightSideValue = rValue;
                RepToRepMultiplication(LRep, RRep, *this, rightSideValue);
            }
            return *this;
        }

        FlaggedDec MultiplyByUnsigned(const FlaggedDec& rValue) { FlaggedDec self = *this; self.UnsignedMultOp(rValue); return self; }

        void CatchAllAddition(FlaggedDec& rValue, RepType LRep, RepType RRep)
        {
            ConvertToNormType(LRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormType(RRep);
            BasicMultOp(convertedRVal);
        }

        FlaggedDec CatchAllAdditionAsCopies(FlaggedDec& rValue, RepType LRep, RepType RRep)
        { FlaggedDec self = *this; CatchAllAddition(rValue, LRep, RRep); return self; }
        
        void CatchAllAdditionV2(FlaggedDec& rValue, RepType SameRep)
        {
            ConvertToNormType(SameRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormType(SameRep);
            BasicAddOp(convertedRVal);
        }

        FlaggedDec CatchAllAdditionAsCopiesV2(FlaggedDec& rValue, RepType SameRep)
        { FlaggedDec self = *this; CatchAllAdditionV2(rValue, SameRep); return self; }
    
       void CatchAllAdditionV3(FlaggedDec& rValue)
       {
            ConvertToNormTypeV2();
            FlaggedDec convertedRVal = rValue.ConvertAsNormTypeV2();
            BasicAddOp(convertedRVal);
       }

        FlaggedDec CatchAllAdditionAsCopiesV3(FlaggedDec& rValue)
        { FlaggedDec self = *this; CatchAllAdditionV3(rValue); return self; }
        
    //Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
    #if defined(AltNum_EnableImaginaryNum)
        void CatchAllImaginaryAddition(FlaggedDec& rValue, RepType LRep, RepType RRep)
        {
           ConvertIRepToNormal(LRep);
           FlaggedDec convertedRVal = rValue.ConvertAsNormalIRep(RRep);
           BasicAddOp(convertedRVal);
           ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginaryAdditionAsCopies(FlaggedDec& rValue, RepType LRep, RepType RRep)
        { FlaggedDec self = *this; CatchAllImaginaryAddition(rValue, LRep, RRep); return self; }
        
        void CatchAllImaginaryAdditionV2(FlaggedDec& rValue, RepType SameRep)
        {
            ConvertIRepToNormal(SameRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormalIRep(SameRep);
            BasicAddOp(convertedRVal);
            ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginaryAdditionAsCopiesV2(FlaggedDec& rValue, RepType SameRep)
        { FlaggedDec self = *this; CatchAllImaginaryAdditionV2(rValue, SameRep); return self; }
    
        void CatchAllImaginaryAdditionV3(FlaggedDec& rValue)
        {
           ConvertIRepToNormalV2();
           FlaggedDec convertedRVal = rValue.ConvertAsNormalIRepV2();
           BasicAddOp(convertedRVal);
           ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginaryAdditionAsCopiesV3(FlaggedDec& rValue)
        { FlaggedDec self = *this; CatchAllImaginaryAdditionV3(rValue); return self; }
    #endif

protected:
        static void NormalAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
	        switch (RRep)
	        {
		        case RepType::NormalType://Fail safe for when converted before switch
			        self.BasicAddOp(Value);
			        break;
	        #if defined(AltNum_EnableMixedFractional)
		        case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			        self.BasicMixedFracAddOp(Value);
			        break;
		        #if defined(AltNum_EnableMixedPiFractional)
		        case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
		        #elif defined(AltNum_EnableMixedEFractional)
		        case RepType::MixedE:
		        #endif
		        #if defined(AltNum_MixedPiOrEEnabled)
			        #if defined(AltNum_EnableMixedPiFractional) 
			        self.BasicMixedPiFracAddOp(Value);
			        #else
			        self.BasicMixedEFracAddOp(Value);
			        #endif
			        break;
		        #endif
	        #endif
		        default:
			        Value.ConvertToNormType(RRep);
			        self.BasicAddOp(Value);
			        break;
	        }
        }

        #if defined(AltNum_EnableFractionals)
        static void NumByDivAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif

        #if defined(AltNum_EnablePiRep)
        static void PiNumAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
	        switch (RRep)
	        {
            #if defined(AltNum_EnablePiPowers)//Combine PiPower in certain cases
            #endif
	        #if defined(AltNum_EnableMixedFractional)
		        case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			        self.ConvertToNormType(RepType::PiNum);
			        self.BasicMixedFracAddOp(Value);
			        break;
	        #endif
	        #if defined(AltNum_EnableMixedPiFractional)
		        case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
		        #if defined(AltNum_EnablePiFractional)
    			        if(self.DecimalHalf==0)//Result is PiFractional
    			        {
                            int divisor = -Value.ExtraRep;
                            if(Value.IntValue!=0)
                                self.IntValue += Value.IntValue;
                            if(self.IntValue==0)
                            {
                                self.IntValue = -Value.DecimalHalf;
                            }
                            else
                            {
                                self.IntValue *= divisor;
                                self.IntValue -= self.DecimalHalf;//adding as negative of the representation
                                if(self.IntValue==0)//Become zero instead of fractional
                                {
                                    self.ExtraRep = 0; return;
                                }
                            }
                            self.DecimalHalf = divisor;
                            self.ExtraRep = PiByDivisorRep;
    			        }
    			        else
    			        {
    	                    self.ConvertToNormType(RepType::PiNum);
    	                    self.BasicMixedPiFracSubAddOp(Value);
    			        }
		        #elif defined(AltNum_EnableDecimaledPiFractionals)//Result is DecimaledPiFractional
                        if(Value.IntValue!=0)
                            self.IntHalfSubtraction(Value.IntValue);
                        if(self.IntValue==0)
                        {
                            self.IntValue = -Value.DecimalHalf;
                        }
                        else
                        {
                            self.IntValue *= -Value.ExtraRep;
                            self.IntHalfSubtraction(self.DecimalHalf);//adding as negative of the representation
                            if(self.IntValue==0&&self.DecimalHalf==0)//Become zero instead of fractional
                            {
                                self.ExtraRep = 0; return;
                            }
                        }
                        self.ExtraRep = Value.ExtraRep;
		        #endif
			        break;
	        #elif defined(AltNum_EnableMixedEFractional)
		        case RepType::MixedE:
			        self.ConvertToNormType(RepType::PiNum);
			        self.BasicMixedEFracSubAddOp(Value);
			        break;
	        #endif
		        default:
			        self.CatchAllAddition(Value, RepType::PiNum, RRep);
			        break;
	        }
        }

        #if defined(AltNum_EnablePiPowers)
        static void PiPowerAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif
	
        #if defined(AltNum_EnableDecimaledPiFractionals)
        static void PiNumByDivAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
	        switch (RRep)
	        {
            #if defined(AltNum_EnableMixedFractional)
		        case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			        self.ConvertToNormType(RepType::PiNumByDiv);
			        self.BasicMixedFracAddOp(Value);
			        break;
	        #endif
	        #if defined(AltNum_EnableMixedPiFractional)
		        case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                    //ToDo:Add more precise AddOperation code here later
                    self.ConvertToNormType(RepType::PiNumByDiv);
			        self.BasicMixedPiFracAddOp(Value);
			        break;
	        #elif defined(AltNum_EnableMixedEFractional)
		        case RepType::MixedE:
                    self.ConvertToNormType(RepType::PiNumByDiv);
			        self.BasicMixedEFracAddOp(Value);
			        break;
	        #endif
		        default:
			        self.CatchAllAddition(Value, RepType::PiNumByDiv, RRep);
			        break;
	        }
        }
        #elif defined(AltNum_EnablePiFractional)
        static void PiFractionalAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif
        #endif

        #if defined(AltNum_EnableERep)
        static void ENumAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
	        switch (RRep)
	        {
	        #if defined(AltNum_EnableMixedFractional)
		        case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			        self.ConvertToNormType(RepType::ENum);
			        self.BasicMixedFracAddOp(Value);
			        break;
	        #endif
	        #if defined(AltNum_EnableMixedPiFractional)
		        case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
			        self.ConvertToNormType(RepType::ENum);
			        self.BasicMixedPiFracAddOp(Value);
			        break;
	        #elif defined(AltNum_EnableMixedEFractional)
		        case RepType::MixedE:
                #if defined(AltNum_EnableEFractional)
    			        if(self.DecimalHalf==0)//Result is EFractional
    			        {
                            int divisor = -Value.ExtraRep;
                            if(Value.IntValue!=0)
                                self.IntValue += Value.IntValue;
                            if(self.IntValue==0)
                            {
                                self.IntValue = -Value.DecimalHalf;
                            }
                            else
                            {
                                self.IntValue *= divisor;
                                self.IntValue -= self.DecimalHalf;//adding as negative of the representation
                                if(self.IntValue==0)//Become zero instead of fractional
                                {
                                    self.ExtraRep = 0; return;
                                }
                            }
                            self.DecimalHalf = divisor;
                            self.ExtraRep = EByDivisorRep;
    			        }
    			        else
    			        {
    	                    self.ConvertToNormType(RepType::ENum);
    	                    self.BasicMixedPiFracSubAddOp(Value);
    			        }
                #elif defined(AltNum_EnableDecimaledEFractionals)//Result is DecimaledEFractional
                        if(Value.IntValue!=0)
                            self.IntHalfSubtraction(Value.IntValue);
                        if(self.IntValue==0)
                        {
                            self.IntValue = -Value.DecimalHalf;
                        }
                        else
                        {
                            self.IntValue *= -Value.ExtraRep;
                            self.IntHalfSubtraction(self.DecimalHalf);//adding as negative of the representation
                            if(self.IntValue==0&&self.DecimalHalf==0)//Become zero instead of fractional
                            {
                                self.ExtraRep = 0; return;
                            }
                        }
                        self.ExtraRep = Value.ExtraRep;
                #endif
			        break;
            #endif
		        default:
			        self.CatchAllAddition(Value, RepType::ENum, RRep);
			        break;
	        }
        }

	        #if defined(AltNum_EnableDecimaledEFractionals)
        static void ENumByDivAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
	        #elif defined(AltNum_EnableEFractional)
        static void EFractionalAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
	        #endif
        #endif

        #if defined(AltNum_EnableApproachingValues)
        static void ApproachingBottomAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }

	        #if !defined(AltNum_DisableApproachingTop)
        static void ApproachingTopAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
	        #endif
	        #if defined(AltNum_EnableApproachingDivided)
        static void ApproachingMidRightAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }

		        #if !defined(AltNum_DisableApproachingTop)
        static void ApproachingMidLeftAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
		        #endif
	        #endif
        #endif

        #if defined(AltNum_EnableImaginaryNum)
        static void ImaginaryNumberAddOp(RepType& LRep, RepType& RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                case RepType::INum:
        #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
        #elif defined(AltNum_EnableFractionals)
                case RepType::IFractional:
        #endif
        #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
        #endif
                    self.CatchAllImaginaryAddition(Value, LRep, RRep);
                    break;
                default:
                    throw FlaggedDec::RepTypeAsString(LRep)+" RepType addition with"+FlaggedDec::RepTypeAsString(RRep)+"not supported yet";
	                break;
            }
        }

        #endif

        #if defined(AltNum_EnableMixedFractional)
        static void MixedFracAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
	        switch (RRep)
	        {
        #if defined(AltNum_EnableMixedPiFractional)
		        case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
        #elif defined(AltNum_EnableMixedEFractional)
		        case RepType::MixedE:
        #endif
        #if defined(AltNum_MixedPiOrEEnabled)
		        {
			        //#if defined(AltNum_MixedPiHasDecimaledFracAccess)
			        //            break;//Give result as PiByDiv
			        //#elif defined(AltNum_MixedEHasDecimaledFracAccess)
			        //            break;//Give result as EByDiv
			        //#else
			        int LeftSide;
			        if (self.IntValue == FlaggedDec::NegativeRep)
				        LeftSide = self.DecimalHalf;
			        else if (self.IntValue < 0)
				        LeftSide = self.IntValue * self.ExtraRep + self.DecimalHalf;
			        else if (self.IntValue == 0)
				        LeftSide = -self.DecimalHalf;
			        else
				        LeftSide = self.IntValue * self.ExtraRep - self.DecimalHalf;
			        FlaggedDec RightSide = FlaggedDec(Value.IntValue == 0 ? -Value.DecimalHalf : (Value.IntValue * -Value.ExtraRep) - Value.DecimalHalf);
			        #if defined(AltNum_EnableMixedPiFractional)
			        RightSide *= FlaggedDec::PiNum;
			        #else
			        RightSide *= FlaggedDec::ENum;
			        #endif
			        int InvertedVDivisor = -Value.ExtraRep;
			        if (self.ExtraRep == InvertedVDivisor)
			        {
				        RightSide.BasicAddOp(LeftSide);
				        self.IntValue = RightSide.IntValue;
				        self.DecimalHalf = RightSide.DecimalHalf;
			        }
			        else
			        {
				        self.ExtraRep *= InvertedVDivisor;
				        LeftSide *= InvertedVDivisor;
				        RightSide *= self.ExtraRep;
				        RightSide.BasicAddOp(LeftSide);
				        self.IntValue = RightSide.IntValue;
				        self.DecimalHalf = RightSide.DecimalHalf;
			        }
			        break;//Give result as NumByDiv
			        //#endif
		        }
        #endif
		        default:
			        self.CatchAllAddition(Value, RepType::MixedFrac, RRep);
			        break;
            }
        }
        #endif

        #if defined(AltNum_MixedPiOrEEnabled)
        static void MixedPiEAddOp(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
	        switch (RRep)
	        {
                case RepType::MixedFrac:
		        {
			        FlaggedDec LeftSide;
			        if (self.IntValue == FlaggedDec::NegativeRep)
				        LeftSide = FlaggedDec(self.DecimalHalf);
			        else if (self.IntValue < 0)
				        LeftSide = FlaggedDec(self.IntValue * -self.ExtraRep + self.DecimalHalf);
			        else if (self.IntValue == 0)
				        LeftSide = FlaggedDec(-self.DecimalHalf);
			        else
				        LeftSide = FlaggedDec(self.IntValue * -self.ExtraRep + -self.DecimalHalf);
			        #if defined(AltNum_EnableMixedPiFractional)
			        LeftSide *= FlaggedDec::PiNum;
			        #elif defined(AltNum_EnableMixedEFractional)
			        LeftSide *= FlaggedDec::ENum;
			        #endif
			        int RightSide = Value.IntValue == 0 ? -Value.DecimalHalf : Value.IntValue * Value.ExtraRep - Value.DecimalHalf;
			        self.ExtraRep = -self.ExtraRep;
			        if (Value.ExtraRep == self.ExtraRep)
			        {
				        LeftSide.BasicAddOp(RightSide);
				        self.IntValue = LeftSide.IntValue;
				        self.DecimalHalf = LeftSide.DecimalHalf;
			        }
			        else
			        {
				        self.ExtraRep *= Value.ExtraRep;
				        LeftSide *= Value.ExtraRep;
				        RightSide *= self.ExtraRep;
				        LeftSide.BasicAddOp(RightSide);
				        self.IntValue = LeftSide.IntValue;
				        self.DecimalHalf = LeftSide.DecimalHalf;
			        }
			        break;
		        }
        /*
            #if defined(AltNum_MixedPiOrEHasDecimaledFracAccess)
	            #if defined(AltNum_EnableDecimaledPiFractionals)
                        case RepType::PiNumByDiv:
                #else
                        case RepType::ENumByDiv:
                #endif
                            //ToDo:Add more precise code here later
                            break;
            #elif defined(AltNum_EnablePiFractional)|| defined(AltNum_EnableEFractional)
	            #if defined(AltNum_EnablePiFractional)

                #else

                #endif
                            //ToDo:Add more precise code here later
                            break;
            #endif
        */
		        default:
        #if defined(AltNum_EnableMixedPiFractional)
			        self.CatchAllAddition(Value, RepType::MixedPi, RRep);
        #else
			        self.CatchAllAddition(Value, RepType::MixedE, RRep);
        #endif
			        break;
            }
        }
        #endif

        static void RepToRepAddition(RepType LRep, RepType RRep, FlaggedDec& self, FlaggedDec rValue)
        {
            bool LeftIsNegative = self.IntValue<0;
#if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)//LRep Overrides
            switch (LRep)
            {
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                throw "UndefinedButInRange AddOperations not supported yet(from left side)";
                break;
                #endif
        #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
            #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
            #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
            #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
            #endif
                BlazesRusCode::FlaggedDec::LRepImaginaryOverridePt2(RRep, rValue);
                break;
        #endif
            default:
                break;
            }
            #endif
            //RRep Overrides before Main RepToRep Code
            switch (RRep)
            {
                #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
            {
                if (LRep == RepType::ApproachingTop && LeftIsNegative == false)
                {
                    self.DecimalHalf = 0;
                    self.IntValue += rValue.IntValue + 1;
                    return;
                }
                else
                {
                    rValue.DecimalHalf = 1;
                    RRep = RepType::NormalType;
                }
                break;
            }
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTop:
            {
                if (LRep == RepType::ApproachingBottom && LeftIsNegative == false)
                {
                    self.DecimalHalf = 0;
                    self.IntValue += rValue.IntValue + 1;
                    return;
                }
                else
                {
                    rValue.DecimalHalf = 999999999;
                    rValue.ExtraRep = 0;
                    RRep = RepType::NormalType;
                }
                break;
            }
            #endif
            #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
            {
                if (LRep == RepType::ApproachingMidRight && self.ExtraRep == rValue.ExtraRep && LeftIsNegative == RightIsNegative)
                {
                    self.DecimalHalf = 0;
                    self.ExtraRep = 0;
                    self.IntValue += rValue.IntValue + 1;
                    return;
                }
                else
                {
                    rValue.ConvertFromApproachingMidLeftToNorm().
                    RRep = RepType::NormalType;
                }
                break;
            }
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingMidRight:
            {//0.49..9+0.0.50..1
                if (LRep == RepType::ApproachingMidLeft && self.ExtraRep == rValue.ExtraRep && LeftIsNegative == false)
                {
                    self.DecimalHalf = 0;
                    self.ExtraRep = 0;
                    self.IntValue += rValue.IntValue + 1;
                    return;
                }
                else
                {
                    rValue.ConvertFromApproachingMidRightToNorm();
                    RRep = RepType::NormalType;
                }
                break;
            }
            #endif
            #endif
            #endif
            #if defined(AltNum_EnableApproachingI)
            case RepType::ApproachingImaginaryBottom:
            {
                if (LRep == RepType::ApproachingImaginaryTop && LeftIsNegative == false)
                {
                    self.DecimalHalf = 0;
                    self.ExtraRep = IRep;
                    self.IntValue += rValue.IntValue + 1;
                    return;
                }
                else
                {
                    rValue.DecimalHalf = 1;
                    rValue.ExtraRep = IRep;
                    RRep = RepType::INum;
                }
                break;
            }
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingImaginaryTop:
            {
                if (LRep == RepType::ApproachingImaginaryBottom && LeftIsNegative == false)
                {
                    self.DecimalHalf = 0;
                    self.ExtraRep = IRep;
                    self.IntValue += rValue.IntValue + 1;
                    return;
                }
                else
                {
                    rValue.DecimalHalf = 999999999;
                    rValue.ExtraRep = IRep;
                    RRep = RepType::INum;
                }
                break;
            }
            #endif
            #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingImaginaryMidLeft:
            {
                if (LRep == RepType::ApproachingImaginaryMidRight && self.ExtraRep == rValue.ExtraRep && LeftIsNegative == RightIsNegative)
                {
                    self.DecimalHalf = 0;
                    self.ExtraRep = IRep;
                    self.IntValue += rValue.IntValue + 1;
                    return;
                }
                else
                {
                    rValue.ConvertFromApproachingIMidLeftToINum();
                    RRep = RepType::INum;
                }
                break;
            }
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingImaginaryMidRight:
            {
                if (LRep == RepType::ApproachingImaginaryMidLeft && self.ExtraRep == rValue.ExtraRep && LeftIsNegative == RightIsNegative)
                {
                    self.DecimalHalf = 0;
                    self.ExtraRep = IRep;
                    self.IntValue += rValue.IntValue + 1;
                    return;
                }
                else
                {
                    rValue.ConvertFromApproachingIMidRightToINum();
                    RRep = RepType::INum;
                }
                break;
            }
            #endif
            #endif
            #endif
            #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            case RepType::UndefinedButInRange:
                throw "UndefinedButInRange AddOperations not supported yet(from right side)";
                break;
                #endif
                #if defined(AltNum_EnableNaN)
            case RepType::Undefined:
            case RepType::NaN:
                throw "Can't perform AddOperations with NaN or Undefined number";
                break;
                #endif
            default:
                break;
            }
            switch (LRep)//Main switch block starts here
            {
                #if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                PiNumAddOp(RRep, self, rValue);
                break;
                #endif
                #if defined(AltNum_EnableERep)
            case RepType::ENum:
                ENumAddOp(RRep, self, rValue);
                break;
                #endif
            case RepType::NormalType:
                #if defined(AltNum_EnablePiPowers)
            case RepType::PiPower:
                #endif
                #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
                #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTop:
                #endif
                #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingMidRight:
                #endif
                #endif
                #endif
                #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                #if defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                #endif
                #if defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                #endif
                #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                #elif defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                #endif
                #endif
                #if defined(AltNum_EnableMixedFractional)
                if (RRep == RepType::MixedFrac)
                {
                    self.ConvertToNormType(LRep);
                    self.BasicMixedFracAddOp(rValue);
                }
                #if defined(AltNum_MixedPiOrEEnabled)
                #if defined(AltNum_EnableMixedPiFractional)
                else if (RRep == RepType::MixedPi)
                    #else
                else if (RRep == RepType::MixedE)
                    #endif
                    self.ConvertToNormType(LRep);
                #if defined(AltNum_EnableMixedPiFractional)
                self.BasicMixedPiFracAddOp(rValue);
                #else
                self.BasicMixedEFracAddOp(rValue);
                #endif
                #else
                else
                    #endif
                    #endif
                    self.CatchAllAddition(rValue, LRep, RRep);
                break;

                #if defined(AltNum_EnableImaginaryNum)//Replace with specific code instead of catchall code later
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://  (Value/(-ExtraRep))*i Representation
                #elif defined(AltNum_EnableFractionals)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                ImaginaryNumberAddOp(LRep, RRep, self, rValue);
                break;
                #endif
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                MixedFracAddOp(RRep, self, rValue);
                break;
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                #endif
                #if defined(AltNum_MixedPiOrEEnabled)
                MixedPiEAddOp(RRep, self, rValue);
                break;
                #endif
                #endif
                #if defined(AltNum_EnableNaN)
            case RepType::Undefined:
            case RepType::NaN:
                throw "Can't perform AddOperations with NaN or Undefined number";
                break;
                #endif
            default:
                throw FlaggedDec::RepTypeAsString(LRep) + " RepType addition with " + FlaggedDec::RepTypeAsString(RRep) + " not supported yet";
                break;
            }
        }

public:

        /// <summary>
        /// Addition Operation  with right side FlaggedDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& AddOp(FlaggedDec rValue)
        {
            if (IsZero())
            {
                IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                ExtraRep = rValue.ExtraRep; return *this;
            }
            else if (rValue.IsZero())
                return *this;
    #if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            if (rValue.DecimalHalf == InfinityRep)
            {
                rValue.IntValue == 1 ? SetAsInfinity() : SetAsNegativeInfinity();
                return *this;
            }
    #endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
    #if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
    #endif
    #if defined(AltNum_EnableERep)
                case RepType::ENum:
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
    #endif
                    BasicAddOp(rValue);
                    break;

    #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.0..1 + 0.0..1 = 0
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//-0.0..1 - 0.0..1 = -0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue < 0)//-0.0..1 - 1.0..1 = -1.0..1
                        {
                            IntValue = rValue.IntValue;
                        }
                        else//-0.0..1 + 5.0..1 = 5
                        {
                            DecimalHalf = 0;
                            IntValue = rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.0..1 + 0.0..1 = 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//0.0..1 - 0.0..1 = 0
                            SetAsZero();
                        else if (rValue.IntValue < 0)//0.0..1 - 1.0..1 = -1
                        {
                            DecimalHalf = 0;
                            IntValue = rValue.IntValue;
                        }
                        else//0.0..1 + 5.0..1 = 5.0..1
                        {
                            IntValue = rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.0..1 + 0.0..1  = -1
                            DecimalHalf = 0;
                        else if (rValue.IntValue == NegativeRep)//-1.0..1 - 0.0..1 = -1.0..1
                        {/*Do Nothing*/
                        }
                        else if (IntValue == -rValue.IntValue)//-1.01 + 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.0..1 - 2.0..1
                        {
                            IntValue += rValue.IntValue;
                        }
                        else//-1.0..1 + 2.0..1 = 1
                        {
                            DecimalHalf = 0;
                            IntValue += rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.0..1 + 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//1.0..1 - 0.0..1
                            DecimalHalf = 0;
                        else if (IntValue == -rValue.IntValue)//1.0..1 - 1.0..1
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.0..1  - 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue += rValue.IntValue;
                        }
                        else//1.0..1 + 1.0..1
                        {
                            IntValue += rValue.IntValue;
                        }
                    }
                    break;
                case RepType::ApproachingTop:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.9..9 + 0.9..9 = 0
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//-0.9..9 - 0.9..9 = -1.9..8
                            IntValue = -1;
                        else if (rValue.IntValue < 0)//-0.9..9 - 1.9..9 = -2.9..8
                        {
                            IntValue = rValue.IntValue - 1;
                        }
                        else//-0.9..9 + 5.9..9 = 5
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.9..9 + 0.9..9 = 1.9..8
                            IntValue = 1;
                        else if (rValue.IntValue == NegativeRep)//0.9..9 - 0.9..9 = 0
                            SetAsZero();
                        else if (rValue.IntValue < 0)//0.9..9 - 1.9..9 = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = rValue.IntValue;
                        }
                        else//0.9..9 + 5.9..9 = 6.9..8
                        {
                            IntValue = rValue.IntValue + 1;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.9..9 + 0.9..9  = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (rValue.IntValue == NegativeRep)//-1.9..9 - 0.9..9 = -2.9..9
                            IntValue += rValue.IntValue;
                        else if (IntValue == -rValue.IntValue)//-1.9..9 + 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.9..9 - 2.9..9
                        {
                            IntValue += rValue.IntValue;
                        }
                        else//-1.9..9 + 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue += rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.9..9 + 0.9..9 = 2.9..8
                        {
                            if (IntValue == -1)
                                IntValue = NegativeRep;
                            else
                                ++IntValue;
                        }
                        else if (rValue.IntValue == NegativeRep)//1.9..9 - 0.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == -rValue.IntValue)//1.9..9 - 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.9..9  - 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue += rValue.IntValue;
                        }
                        else//1.9..9 + 1.9..9 = 3.9..8
                        {
                            IntValue += rValue.IntValue + 1;
                        }
                    }
                    break;

        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    CatchAllAdditionV2(rValue, RepType::ApproachingMidLeft);
                    break;
                case RepType::ApproachingMidRight:
                    CatchAllAdditionV2(rValue, RepType::ApproachingMidRight);
                    break;
        #endif
    #endif
    #if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                    if (ExtraRep == rValue.ExtraRep)
                        BasicAddOp(rValue);
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicAddOp(rValue);
                    }
                    break;
    #endif
    #if defined(AltNum_UsingAltFractional)
        #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
        #endif
        #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
        #endif
        #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*e Representation
        #endif
                    if (DecimalHalf == rValue.DecimalHalf)
                    {

                        *this += rValue.IntValue;
                    }
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicAddOp(rValue);
                    }
                    break;
    #endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/(ExtraRep*-1))*Pi Representation
        #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/(ExtraRep*-1))*e Representation
        #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/(ExtraRep*-1))*e Representation
        #endif
                    if (ExtraRep == rValue.ExtraRep)
                        BasicAddOp(rValue);
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicAddOp(rValue);
                    }
                    break;
    #endif
    #if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    if (ExtraRep != rValue.ExtraRep)
                    {
                        ConvertPiPowerToPiRep(); rValue.ConvertPiPowerToPiRep();
                    }
                    BasicAddOp(rValue);
                    break;
    #endif
    #if defined(AltNum_EnableMixedFractional)
                    //Ignoring the fact that mixed fraction could be improperly formatted for spend
                    //because should be corrected after every operation
                case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue += rValue.IntValue;
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= rValue.ExtraRep;
                        IntValue += rValue.IntValue * ExtraRep;
                        DecimalHalf += rValue.DecimalHalf * ExtraRep;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= rValue.ExtraRep;
                    }
                    break;
        #if defined(AltNum_EnableAlternativeMixedFrac)
            #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
            #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
            #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
            #endif
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue += rValue.IntValue;
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= -rValue.ExtraRep;
                        IntValue += rValue.IntValue * -ExtraRep;
                        DecimalHalf += rValue.DecimalHalf * -ExtraRep;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= -rValue.ExtraRep;
                    }
                    break;
        #endif
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                    BasicAddOp(rValue);
                    break;
        #if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    if (IntValue == NegativeRep)
                    {
                        //Do nothing?
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {
                        IntValue = NegativeRep;
                    }
                    else
                        IntValue += rValue.IntValue;
                    if (DecimalHalf == InfinityRep)
                    {
                        //Do Nothing?
                    }
                    else if (rValue.DecimalHalf == InfinityRep)
                    {// + positive infinity to right side
                        DecimalHalf = InfinityRep;
                    }
                    else
                        DecimalHalf += rValue.DecimalHalf;
                    break;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif
                default:
                    throw FlaggedDec::RepTypeAsString(LRep) + " RepType addition not supported yet";
                    break;
                }
            }
            else
                RepToRepAddition(LRep, RRep, *this, rValue);
            return *this;
        }

        FlaggedDec AddBy(const FlaggedDec& rValue) { FlaggedDec self = *this; self.AddOp(rValue); return self; }

        void CatchAllSubtraction(FlaggedDec& rValue, RepType LRep, RepType RRep)
        {
            ConvertToNormType(LRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormType(RRep);
            BasicMultOp(convertedRVal);
        }

        FlaggedDec CatchAllSubtractionAsCopies(FlaggedDec& rValue, RepType LRep, RepType RRep) { FlaggedDec self = *this; CatchAllSubtraction(rValue, LRep, RRep); return self; }

        void CatchAllSubtractionV2(FlaggedDec& rValue, RepType SameRep)
        {
            ConvertToNormType(SameRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormType(SameRep);
            BasicSubOp(convertedRVal);
        }

        FlaggedDec CatchAllSubtractionAsCopiesV2(FlaggedDec& rValue, RepType SameRep) { FlaggedDec self = *this; CatchAllSubtractionV2(rValue, SameRep); return self; }

       void CatchAllSubtractionV3(FlaggedDec& rValue)
       {
            ConvertToNormTypeV2();
            FlaggedDec convertedRVal = rValue.ConvertAsNormTypeV2();
            BasicSubOp(convertedRVal);
       }

        FlaggedDec CatchAllSubtractionAsCopiesV3(FlaggedDec& rValue) { FlaggedDec self = *this; CatchAllSubtractionV3(rValue); return self; }

        //Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
    #if defined(AltNum_EnableImaginaryNum)
        void CatchAllImaginarySubtraction(FlaggedDec& rValue, RepType LRep, RepType RRep)
        {
           ConvertIRepToNormal(LRep);
           FlaggedDec convertedRVal = rValue.ConvertAsNormalIRep(RRep);
           BasicSubOp(convertedRVal);
           ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginarySubtractionAsCopies(FlaggedDec& rValue, RepType LRep, RepType RRep) { FlaggedDec self = *this; CatchAllImaginarySubtraction(rValue, LRep, RRep); return self; }

        void CatchAllImaginarySubtractionV2(FlaggedDec& rValue, RepType SameRep)
        {
            ConvertIRepToNormal(SameRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormalIRep(SameRep);
            BasicSubOp(convertedRVal);
            ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginarySubtractionAsCopiesV2(FlaggedDec& rValue, RepType SameRep) { FlaggedDec self = *this; CatchAllImaginarySubtractionV2(rValue, SameRep); return self; }

        void CatchAllImaginarySubtractionV3(FlaggedDec& rValue)
        {
           ConvertIRepToNormalV2();
           FlaggedDec convertedRVal = rValue.ConvertAsNormalIRepV2();
           BasicSubOp(convertedRVal);
           ExtraRep = 0;
        }

        FlaggedDec CatchAllImaginarySubtractionAsCopiesV3(FlaggedDec& rValue) { FlaggedDec self = *this; CatchAllImaginarySubtractionV3(rValue); return self; }
    #endif

protected:

        //NormalType representation subtracted by other representation types
        static void NormalRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::NormalType://Fail safe for when converted before switch
                self.BasicSubOp(Value);
                break;
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                self.BasicMixedFracSubOp(Value);
                break;
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                #endif
                #if defined(AltNum_MixedPiOrEEnabled)
                #if defined(AltNum_EnableMixedPiFractional)
                self.BasicMixedPiFracSubOp(Value);
                #else
                self.BasicMixedEFracSubOp(Value);
                #endif
                break;
                #endif
                #endif
            default:
                Value.ConvertToNormType(RRep);
                self.BasicSubOp(Value);
                break;
            }
        }

#if defined(AltNum_EnablePiRep)
        //PiNum representation subtracted by other representation types
        static void PiNumRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                #if defined(AltNum_EnablePiPowers)//Combine PiPower in certain cases
                #endif
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                self.ConvertToNormType(RepType::PiNum);
                self.BasicMixedFracSubOp(Value);
                break;
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                #endif
                #if defined(AltNum_MixedPiOrEEnabled)
                #if defined(AltNum_EnableMixedEFractional)
                self.ConvertToNormType(RepType::);
                self.BasicMixedEFracSubOp(Value);
                #else
                #if defined(AltNum_EnablePiFractional)
                if (self.DecimalHalf == 0)//Result is PiFractional
                {
                    int divisor = -Value.ExtraRep;
                    if (Value.IntValue != 0)
                        self.IntValue -= Value.IntValue;
                    if (self.IntValue == 0)
                    {
                        self.IntValue = Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue *= divisor;
                        self.IntValue += self.DecimalHalf;//adding as negative of the representation
                        if (self.IntValue == 0)//Become zero instead of fractional
                        {
                            self.ExtraRep = 0; return;
                        }
                    }
                    self.DecimalHalf = divisor;
                    self.ExtraRep = PiByDivisorRep;
                }
                else
                {
                    self.ConvertToNormType(RepType::);
                    self.BasicMixedPiFracSubOp(Value);
                }
                #elif defined(AltNum_EnableDecimaledPiFractionals)//Result is DecimaledPiFractional
                if (Value.IntValue != 0)
                    self.IntHalfSubtraction(Value.IntValue);
                if (self.IntValue == 0)
                {
                    self.IntValue = Value.DecimalHalf;
                }
                else
                {
                    self.IntValue *= -Value.ExtraRep;
                    self.IntHalfAddition(self.DecimalHalf);//adding as negative of the representation
                    if (self.IntValue == 0 && self.DecimalHalf == 0)//Become zero instead of fractional
                    {
                        self.ExtraRep = 0; return;
                    }
                }
                self.ExtraRep = Value.ExtraRep;
                #endif
                #endif
                break;
                #endif
                #endif
            default:
                self.CatchAllSubtraction(Value, RepType::PiNum, RRep);
                break;
            }
        }
#endif

#if defined(AltNum_EnableERep)
        //ENum representation subtracted by other representation types
        static void ENumRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                self.ConvertENumToNum();
                self.BasicMixedFracSubOp(Value);
                break;
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                #endif
                #if defined(AltNum_MixedPiOrEEnabled)
                #if defined(AltNum_EnableMixedPiFractional)
                self.ConvertENumToNum();
                self.BasicMixedPiFracSubOp(Value); return;
                #else
                #if defined(AltNum_EnableEFractional)
                if (self.DecimalHalf == 0)//Result is EFractional
                {
                    int divisor = -Value.ExtraRep;
                    if (Value.IntValue != 0)
                        self.IntValue -= Value.IntValue;
                    if (self.IntValue == 0)
                    {
                        self.IntValue = Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue *= divisor;
                        self.IntValue += self.DecimalHalf;//adding as negative of the representation
                        if (self.IntValue == 0)//Become zero instead of fractional
                        {
                            self.ExtraRep = 0; return;
                        }
                    }
                    self.DecimalHalf = divisor;
                    self.ExtraRep = FlaggedDec::EByDivisorRep;
                }
                else
                {
                    self.ConvertENumToNum();
                    self.BasicMixedEFracSubOp(Value);
                }
                #elif defined(AltNum_EnableDecimaledEFractionals)//Result is DecimaledEFractional
                if (Value.IntValue != 0)
                    self.IntHalfSubtraction(Value.IntValue);
                if (self.IntValue == 0)
                {
                    self.IntValue = Value.DecimalHalf;
                }
                else
                {
                    self.IntValue *= -Value.ExtraRep;
                    self.IntHalfAddition(self.DecimalHalf);//adding as negative of the representation
                    if (self.IntValue == 0 && self.DecimalHalf == 0)//Become zero instead of fractional
                    {
                        self.ExtraRep = 0; return;
                    }
                }
                self.ExtraRep = Value.ExtraRep;
                #endif
                #endif
                break;
                #endif
                #endif
            default:
                self.CatchAllSubtraction(Value, RepType::ENum, RRep);
                break;
            }
        }
#endif

#if defined(AltNum_EnableImaginaryNum)
        //INum,INumByDivisor, IFractional, or MixedI representation subtracted by other representation types
        static void ImaginaryNumberRtRSubtraction(RepType LRep, RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::INum:
                #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv:
                #elif defined(AltNum_EnableFractionals)
            case RepType::IFractional:
                #endif
                #if defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI:
                #endif
                self.CatchAllImaginarySubtraction(Value, LRep, RRep);
                break;
            default:
                throw FlaggedDec::RepTypeAsString(LRep) + " RepType subtraction with" + FlaggedDec::RepTypeAsString(RRep) + "not supported yet";
                break;
            }
        }
#endif

#if defined(AltNum_EnableMixedFractional)
        //MixedFraction representation subtracted by other representation types
        static void MixedFracRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
                #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE:
                #endif
                #if defined(AltNum_MixedPiOrEEnabled)
            {
                //#if defined(AltNum_MixedPiHasDecimaledFracAccess)
                //            break;//Give result as PiByDiv
                //#elif defined(AltNum_MixedEHasDecimaledFracAccess)
                //            break;//Give result as EByDiv
                //#else
                FlaggedDec LeftSide;
                if (self.IntValue == FlaggedDec::NegativeRep)
                    LeftSide = FlaggedDec(self.DecimalHalf);
                else if (self.IntValue < 0)
                    LeftSide = FlaggedDec(self.IntValue * self.ExtraRep + self.DecimalHalf);
                else if (self.IntValue == 0)
                    LeftSide = FlaggedDec(-self.DecimalHalf);
                else
                    LeftSide = FlaggedDec(self.IntValue * self.ExtraRep - self.DecimalHalf);
                FlaggedDec RightSide = FlaggedDec(Value.IntValue == 0 ? -Value.DecimalHalf : (Value.IntValue * -Value.ExtraRep) - Value.DecimalHalf);
                #if defined(AltNum_EnableMixedPiFractional)
                RightSide *= FlaggedDec::PiNum;
                #else
                RightSide *= FlaggedDec::ENum;
                #endif
                int InvertedVDivisor = -Value.ExtraRep;
                if (self.ExtraRep == InvertedVDivisor)
                {
                    LeftSide.BasicSubOp(RightSide);
                    self.IntValue = LeftSide.IntValue;
                    self.DecimalHalf = LeftSide.DecimalHalf;
                }
                else
                {
                    self.ExtraRep *= InvertedVDivisor;
                    LeftSide *= InvertedVDivisor;
                    RightSide *= self.ExtraRep;
                    LeftSide.BasicSubOp(RightSide);
                    self.IntValue = LeftSide.IntValue;
                    self.DecimalHalf = LeftSide.DecimalHalf;
                }
                break;//Give result as NumByDiv
                //#endif
            }
                                #endif
            default:
                self.CatchAllSubtraction(Value, RepType::MixedFrac, RRep);
                break;
            }
        }
#endif

#if defined(AltNum_MixedPiOrEEnabled)
        //MixedPiFraction or MixedEFraction representation subtracted by other representation types
        static void MixedPiOrERtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
            switch (RRep)
            {
            case RepType::MixedFrac:
            {
                //Left side Numerator
                FlaggedDec LeftSide;
                if (self.IntValue == FlaggedDec::NegativeRep)
                    LeftSide = FlaggedDec(self.DecimalHalf);
                else if (self.IntValue < 0)
                    LeftSide = FlaggedDec(self.IntValue * -self.ExtraRep + self.DecimalHalf);
                else if (self.IntValue == 0)
                    LeftSide = FlaggedDec(-self.DecimalHalf);
                else
                    LeftSide = FlaggedDec(self.IntValue * -self.ExtraRep + -self.DecimalHalf);
            #if defined(AltNum_EnableMixedPiFractional)
                LeftSide *= FlaggedDec::PiNum;
            #elif defined(AltNum_EnableMixedEFractional)
                LeftSide *= FlaggedDec::ENum;
            #endif
                //Right side Numerator
                int RightSide = Value.IntValue == 0 ? -Value.DecimalHalf : Value.IntValue * Value.ExtraRep - Value.DecimalHalf;
                self.ExtraRep = -self.ExtraRep;
                if (Value.ExtraRep == self.ExtraRep)
                {
                    LeftSide.BasicSubOp(RightSide);
                    self.IntValue = LeftSide.IntValue;
                    self.DecimalHalf = LeftSide.DecimalHalf;
                }
                else
                {
                    self.ExtraRep *= Value.ExtraRep;
                    LeftSide *= Value.ExtraRep;
                    RightSide *= self.ExtraRep;
                    LeftSide.BasicSubOp(RightSide);
                    self.IntValue = LeftSide.IntValue;
                    self.DecimalHalf = LeftSide.DecimalHalf;
                }
                break;
                /*
                #if defined(AltNum_MixedPiOrEHasDecimaledFracAccess)
                #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
                #else
                case RepType::ENumByDiv:
                #endif
                //ToDo:Add more precise code here later
                break;
                #elif defined(AltNum_EnablePiFractional)|| defined(AltNum_EnableEFractional)
                #if defined(AltNum_EnablePiFractional)

                #else

                #endif
                //ToDo:Add more precise code here later
                break;
                #endif
                */
            }
            default:
                #if defined(AltNum_EnableMixedPiFractional)
                self.CatchAllSubtraction(Value, RepType::MixedPi, RRep);
                #else
                self.CatchAllSubtraction(Value, RepType::MixedE, RRep);
                #endif
                break;
            }
        }
#endif

        /*
        #if defined(AltNum_EnableFractionals)
        static void NumByDivRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif

        #if defined(AltNum_EnablePiPowers)
        static void PiPowerRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif

        #if defined(AltNum_EnableDecimaledPiFractionals)
        static void PiNumByDivRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif
        #if defined(AltNum_EnablePiFractional)
        static void PiFractionalRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif
        #if defined(AltNum_EnableEFractional)
        static void EFractionalRtRSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif
        #if defined(AltNum_EnableDecimaledPiOrEFractionals)
        static void PiOrEByDivSubtraction(RepType RRep, FlaggedDec& self, FlaggedDec& Value)
        {
        }
        #endif
        */

        static void RepToRepSubtraction(RepType LRep, RepType RRep, FlaggedDec& self, FlaggedDec Value)
        {
        	bool LeftIsNegative = self.IntValue<0;
        #if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)//LRep Overrides
            switch(LRep)
            {
            #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
        			throw "UndefinedButInRange SubOperations not supported yet(from left side)";
                    break;
            #endif
        	#if defined(AltNum_EnableImaginaryNum)
        		case RepType::INum:
        		#if defined(AltNum_EnableAlternativeRepFractionals)
        			#if defined(AltNum_EnableDecimaledIFractionals)
        		case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
        			#else
        		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
        			#endif
        		#endif
                #if defined(AltNum_EnableMixedIFractional)
        		case RepType::MixedI:
                #endif
        			BlazesRusCode::FlaggedDec::LRepImaginaryOverridePt2(RRep, Value);
                    break;
        	#endif
        		default:
        			break;
        	}
        #endif
            //RRep Overrides before Main RepToRep Code
            switch(RRep)
            {
        	#if defined(AltNum_EnableApproachingValues)
        		case RepType::ApproachingBottom:
        		{
        			if(LRep==RepType::ApproachingTop&&LeftIsNegative)
        			{
        				self.DecimalHalf = 0;
                        if(self.IntValue==NegativeRep)
                            self.IntValue = -1 - Value.GetIntHalf();
                        else
                            self.IntValue -= Value.GetIntHalf() + 1;
        				return;
        			}
        			else
        			{
        			   Value.DecimalHalf = 1;
        			   RRep = RepType::NormalType;
        			}
        			break;
        		}
        		#if !defined(AltNum_DisableApproachingTop)
        		case RepType::ApproachingTop:
        		{
        			if(LRep==RepType::ApproachingBottom&&LeftIsNegative)
        			{
        				self.DecimalHalf = 0;
                        if(self.IntValue==NegativeRep)
                            self.IntValue = -1 - Value.GetIntHalf();
                        else
                            self.IntValue -= Value.GetIntHalf() + 1;
        				return;
        			}
        			else
        			{
        				Value.DecimalHalf = 999999999;
        				Value.ExtraRep = 0;
        				RRep = RepType::NormalType;
        			}
        			break;
        		}
        		#endif
        		#if defined(AltNum_EnableApproachingDivided)
        		case RepType::ApproachingMidLeft:
        		{
        			if(LRep==RepType::ApproachingMidRight&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative)
        			{
        				self.DecimalHalf = 0;
        				self.ExtraRep = 0;
                        if(self.IntValue==NegativeRep)
                            self.IntValue = -1 - Value.GetIntHalf();
                        else
                            self.IntValue -= Value.GetIntHalf() + 1;
        				return;
        			}
        			else
        			{
        				Value.ConvertFromApproachingMidLeftToNorm().
        				RRep = RepType::NormalType;
        			}
        			break;
        		}
        			#if !defined(AltNum_DisableApproachingTop)
        		case RepType::ApproachingMidRight:
        		{//-0.50..1-0.0.49..9
        			if(LRep==RepType::ApproachingMidLeft&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative)
        			{
        				self.DecimalHalf = 0;
        				self.ExtraRep = 0;
                        if(self.IntValue==NegativeRep)
                            self.IntValue = -1 - Value.GetIntHalf();
                        else
                            self.IntValue -= Value.GetIntHalf() + 1;
        				return;
        			}
        			else
        			{
        				Value.ConvertFromApproachingMidRightToNorm();
        				RRep = RepType::NormalType;
        			}
        			break;
        		}
        			#endif
        		#endif
        	#endif
        	#if defined(AltNum_EnableApproachingI)
        		case RepType::ApproachingImaginaryBottom:
        		{//-1.0..1 - 0.9..9
        			if(LRep==RepType::ApproachingImaginaryTop&&LeftIsNegative)
        			{
        				self.DecimalHalf = 0;
        				self.ExtraRep = IRep;
                        if(self.IntValue==NegativeRep)
                            self.IntValue = -1 - Value.GetIntHalf();
                        else
                            self.IntValue -= Value.GetIntHalf() + 1;
        				return;
        			}
        			else
        			{
        			   Value.DecimalHalf = 1;
        			   Value.ExtraRep = IRep;
        			   RRep = RepType::INum;
        			}
        			break;
        		}
        		#if !defined(AltNum_DisableApproachingTop)
        		case RepType::ApproachingImaginaryTop:
        		{
        			if(LRep==RepType::ApproachingImaginaryBottom)
        			{
        				self.DecimalHalf = 0;
        				self.ExtraRep = IRep;
                        if(self.IntValue==NegativeRep)
                            self.IntValue = -1 - Value.GetIntHalf();
                        else
                            self.IntValue -= Value.GetIntHalf() + 1;
                        return;
        			}
        			else
        			{
        				Value.DecimalHalf = 999999999;
        				Value.ExtraRep = IRep;
        				RRep = RepType::INum;
        			}
        			break;
        		}
        		#endif
        
        		#if defined(AltNum_EnableApproachingDivided)
        		case RepType::ApproachingImaginaryMidLeft:
        		{
        			if(LRep==RepType::ApproachingImaginaryMidRight&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative)
        			{
        				self.DecimalHalf = 0;
        				self.ExtraRep = IRep;
                        if(self.IntValue==NegativeRep)
                            self.IntValue = -1 - Value.GetIntHalf();
                        else
                            self.IntValue -= Value.GetIntHalf() + 1;
        				return;
        			}
        			else
        			{
        				Value.ConvertFromApproachingIMidLeftToINum();
        				RRep = RepType::INum;
        			}
        			break;
        		}
        			#if !defined(AltNum_DisableApproachingTop)
        		case RepType::ApproachingImaginaryMidRight:
        		{
        			if(LRep==RepType::ApproachingImaginaryMidLeft&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative)
        			{
        				self.DecimalHalf = 0;
        				self.ExtraRep = IRep;
                        if(self.IntValue==NegativeRep)
                            self.IntValue = -1 - Value.GetIntHalf();
                        else
                            self.IntValue -= Value.GetIntHalf() + 1;
        				return;
        			}
        			else
        			{
        				Value.ConvertFromApproachingIMidRightToINum();
        				RRep = RepType::INum;
        			}
        			break;
        		}
        			#endif
        		#endif
        	#endif
            #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
        			throw "UndefinedButInRange SubOperations not supported yet(from right side)";
                    break;
            #endif
        	#if defined(AltNum_EnableNaN)
        		case RepType::Undefined:
        		case RepType::NaN:
        			throw "Can't perform SubOperations with NaN or Undefined number";
        			break;
        	#endif
        		default:
        			break;
        	}
            switch (LRep)//Main switch block starts here
            {
        #if defined(AltNum_EnablePiRep)
        		case RepType::PiNum:
                    PiNumRtRSubtraction(RRep, self, Value);
                    break;
        #endif
        #if defined(AltNum_EnableERep)
        		case RepType::ENum:
                    ENumRtRSubtraction(RRep, self, Value);
                    break;
        #endif
        		case RepType::NormalType:
        #if defined(AltNum_EnablePiPowers)
        		case RepType::PiPower:
        #endif
        #if defined(AltNum_EnableApproachingValues)
        		case RepType::ApproachingBottom:
            #if !defined(AltNum_DisableApproachingTop)
        		case RepType::ApproachingTop:
            #endif
            #if defined(AltNum_EnableApproachingDivided)
        		case RepType::ApproachingMidLeft:
                #if !defined(AltNum_DisableApproachingTop)
        		case RepType::ApproachingMidRight:
                #endif
            #endif
        #endif
        #if defined(AltNum_EnableFractionals)
        		case RepType::NumByDiv:
            #if defined(AltNum_EnablePiFractional)
        		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            #endif
            #if defined(AltNum_EnableEFractional)
        		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            #endif
            #if defined(AltNum_EnableDecimaledPiFractionals)
        		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
            #elif defined(AltNum_EnableDecimaledEFractionals)
        		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
            #endif
        #endif
        #if defined(AltNum_EnableMixedFractional)
                    if(RRep==RepType::MixedFrac)
        			{
        				self.ConvertToNormType(LRep);
        				self.BasicMixedFracSubOp(Value);
        			}
        	#if defined(AltNum_EnableMixedPiFractional)
                    else if(RRep==RepType::MixedPi)
        	#elif defined(AltNum_EnableMixedEFractional)
                    else if(RRep==RepType::MixedE)
        	#endif
        	#if defined(AltNum_MixedPiOrEEnabled)
            			self.ConvertToNormType(LRep);
            		#if defined(AltNum_EnableMixedPiFractional)
            			self.BasicMixedPiFracSubOp(Value);
            		#else
            			self.BasicMixedEFracSubOp(Value);
            		#endif
        	#else
                    else
        	#endif
        			    self.CatchAllSubtraction(Value, LRep, RRep);
        			break;
        #endif
        #if defined(AltNum_EnableImaginaryNum)//Replace with specific code instead of catchall code later
        		case RepType::INum:
            #if defined(AltNum_EnableDecimaledIFractionals)
        		case RepType::INumByDiv://  (Value/(-ExtraRep))*i Representation
            #elif defined(AltNum_EnableFractionals)
        		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
        	#if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
            #endif
        			ImaginaryNumberRtRSubtraction(LRep, RRep, self, Value);
                    break;
        #endif
        #if defined(AltNum_EnableMixedFractional)
        		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                    MixedFracRtRSubtraction(RRep, self, Value);
                    break;
            #if defined(AltNum_EnableMixedPiFractional)
        		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
            #elif defined(AltNum_EnableMixedEFractional)
        		case RepType::MixedE:
            #endif
            #if defined(AltNum_MixedPiOrEEnabled)
                    MixedPiOrERtRSubtraction(RRep, self, Value);
                    break;
            #endif
        #endif
        	#if defined(AltNum_EnableNaN)
        		case RepType::Undefined:
        		case RepType::NaN:
        			throw "Can't perform Operations with NaN or Undefined number";
        			break;
        	#endif
        		default:
        			throw FlaggedDec::RepTypeAsString(LRep)+" RepType subtraction with "+FlaggedDec::RepTypeAsString(RRep)+" not supported yet";
        			break;
        	}
        }

public:

        /// <summary>
        /// Subtraction Operation  with right side FlaggedDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& SubOp(FlaggedDec rValue)
        {
            if (IsZero())
            {
                IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                ExtraRep = rValue.ExtraRep; return *this;
            }
            else if (rValue.IsZero())
                return *this;
    #if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            if (rValue.DecimalHalf == InfinityRep)
            {
                rValue.IntValue == 1 ? SetAsInfinity() : SetAsNegativeInfinity();
                return *this;
            }
    #endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
    #if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
    #endif
    #if defined(AltNum_EnableERep)
                case RepType::ENum:
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
    #endif
                    BasicSubOp(rValue);
                    break;

    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingBottom:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.0..1 - 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//-0.0..1 + 0.0..1 
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-0.0..1 + 1.0..1
                        {
                            DecimalHalf = 0;
                            IntValue = -rValue.IntValue;
                        }
                        else//-0.0..1 - 5.0..1
                        {
                            IntValue = -rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.0..1 - 0.0..1 = 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//0.0..1 + 0.0..1 = 0

                        else if (rValue.IntValue < 0)//0.0..1 + 1.0..1
                        {
                            IntValue = -rValue.IntValue;
                        }
                        else//0.0..1 - 5.0..1
                        {
                            DecimalHalf = 0;
                            IntValue = -rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.0..1 - 0.0..1  = -1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//-1.0..1 + 0.0..1
                            DecimalHalf = 0;
                        else if (IntValue == rValue.IntValue)//-1.01 - 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.0..1 + 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//-1.0..1 - 2.0..1 = 1
                        {
                            IntValue -= rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.0..1 - 0.0..1
                            DecimalHalf = 0;
                        else if (rValue.IntValue == NegativeRep)//1.0..1 + 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (IntValue == -rValue.IntValue)//1.0..1 - 1.0..1
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.0..1  - 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//1.0..1 - 1.0..1
                        {
                            IntValue -= rValue.IntValue;
                        }
                    }
                    break;
                case RepType::ApproachingTop:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.9..9 - 0.9..9 = 0
                            IntValue = -1;
                        else if (rValue.IntValue == NegativeRep)//-0.9..9 + 0.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-0.9..9 + 1.9..9

                        else//-0.9..9 - 5.9..9 = -6.9..8
                            IntValue = -rValue.IntValue - 1;
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.9..9 - 0.9..9
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//0.9..9 + 0.9..9 = 1.9..8
                            IntValue = 1;
                        else if (rValue.IntValue < 0)//0.9..9 + 1.9..9 = 1.9..8
                            IntValue = -rValue.IntValue;
                        else//0.9..9 - 5.9..9 = -5
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = -rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.9..9 - 0.9..9  = -2.9..8
                            --IntValue;
                        else if (rValue.IntValue == NegativeRep)//-1.9..9  + 0.9..9 = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == rValue.IntValue)//-1.01 + 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.9..9 + 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//-1.9..9 - 2.9..9
                        {
                            IntValue -= rValue.IntValue + 1;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.9..9 - 0.9..9 = 2.9..8
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (rValue.IntValue == NegativeRep)//1.9..9 + 0.9..9
                            ++IntValue;
                        else if (IntValue == -rValue.IntValue)//1.9..9 - 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.9..9  - 2.9..9
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//1.9..9 - 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue -= rValue.IntValue;
                        }
                    }
                    break;

        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    CatchAllSubtractionV2(rValue, RepType::ApproachingMidLeft);
                    break;
                case RepType::ApproachingMidRight:
                    CatchAllSubtractionV2(rValue, RepType::ApproachingMidRight);
                    break;
        #endif
    #endif
    #if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                    if (ExtraRep == rValue.ExtraRep)
                        BasicSubOp(rValue);
                    else
                        CatchAllSubtractionV2(rValue, RepType::NumByDiv);
                    break;
    #endif
    #if defined(AltNum_UsingAltFractional)
        #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
        #endif
        #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
        #endif
        #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*e Representation
        #endif
                    if (DecimalHalf == rValue.DecimalHalf)
                    {
                        *this -= rValue.IntValue;
                    }
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicSubOp(rValue);
                    }
                    break;
    #endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/(ExtraRep*-1))*Pi Representation
        #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/(ExtraRep*-1))*e Representation
        #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/(ExtraRep*-1))*e Representation
        #endif
                    if (ExtraRep == rValue.ExtraRep)
                        BasicSubOp(rValue);
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicSubOp(rValue);
                    }
                    break;
    #endif
    #if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    if (ExtraRep != rValue.ExtraRep)
                    {
                        ConvertPiPowerToPiRep(); rValue.ConvertPiPowerToPiRep();
                    }
                    BasicSubOp(rValue);
                    break;
    #endif
    #if defined(AltNum_EnableMixedFractional)
                    //Ignoring the fact that mixed fraction could be improperly formatted for spend
                    //because should be corrected after every operation
                case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue -= rValue.IntValue;
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf < 0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= rValue.ExtraRep;
                        IntValue -= rValue.IntValue * ExtraRep;
                        DecimalHalf -= rValue.DecimalHalf * ExtraRep;
                        if (DecimalHalf < 0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= rValue.ExtraRep;
                    }
                    break;
        #if defined(AltNum_EnableAlternativeMixedFrac)
            #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
            #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
            #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
            #endif
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue -= rValue.IntValue;
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf > 0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= -rValue.ExtraRep;
                        IntValue -= rValue.IntValue * -ExtraRep;
                        DecimalHalf -= rValue.DecimalHalf * -ExtraRep;
                        if (DecimalHalf > 0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= -rValue.ExtraRep;
                    }
                    break;
        #endif
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                    BasicSubOp(rValue);
                    break;
        #if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == NegativeRep)
                            IntValue = 0;
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {//+positive infinity to left side
            #if defined(AltNum_EnableNaN)
                        DecimalHalf = UndefinedRep;
            #else
                        throw "Uncertain result";
            #endif
                    }
                    else
                        IntValue -= rValue.IntValue;
                    if (DecimalHalf == InfinityRep)
                    {
                        if (rValue.DecimalHalf == InfinityRep)
                            DecimalHalf = 0;
                    }
                    else if (rValue.DecimalHalf == InfinityRep)
                    {// subtractive infinity from real number
            #if defined(AltNum_EnableNaN)
                        DecimalHalf = UndefinedRep;
            #else
                        throw "Uncertain result";
            #endif
                    }
                    else
                        DecimalHalf -= rValue.DecimalHalf;
                    break;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
    #endif
                default:
                    throw FlaggedDec::RepTypeAsString(LRep) + " RepType subtraction not supported yet";
                    break;
                }
            }
            else
                RepToRepSubtraction(LRep, RRep, *this, rValue);
            return *this;
        }

        FlaggedDec SubtractBy(const FlaggedDec& rValue) { FlaggedDec self = *this; self.SubOp(rValue); return self; }

    #pragma endregion Main AltNum Operations

    #pragma region Main Operator Overrides
        //ToDo:Add left side integer operator overrides later

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec operator/(FlaggedDec self, const FlaggedDec& rValue) { return self.DivOp(rValue); }
        friend FlaggedDec operator/(FlaggedDec self, const int& rValue) { return self.IntDivOp(rValue); }
        friend FlaggedDec operator/(FlaggedDec self, const unsigned int rValue) { return self.UIntDivOp(rValue); }
        friend FlaggedDec operator/(FlaggedDec self, const signed long long rValue) { return self.IntDivOp(rValue); }
        friend FlaggedDec operator/(FlaggedDec self, const unsigned __int64 rValue) { return self.UIntDivOp(rValue); }
        friend FlaggedDec operator/(FlaggedDec self, const __int16& rValue) { return self.IntDivOp(rValue); }
        friend FlaggedDec operator/(FlaggedDec self, const unsigned __int16& rValue) { return self.UIntDivOp(rValue); }
        friend FlaggedDec operator/(FlaggedDec self, const signed __int8& rValue) { return self.IntDivOp(rValue); }
        friend FlaggedDec operator/(FlaggedDec self, const unsigned __int64& rValue) { return self.UIntDivOp(rValue); }

        /// <summary>
        /// Division Operation with Integer right side value
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator/=(FlaggedDec& self, const int& rValue) { return self.Int32DivOp(rValue); }
        friend FlaggedDec& operator/=(FlaggedDec& self, const __int64& rValue) { return self.IntDivOp(rValue); }
        friend FlaggedDec& operator/=(FlaggedDec& self, const unsigned int& rValue) { return self.UIntDivOp(rValue); }
        friend FlaggedDec& operator/=(FlaggedDec& self, const unsigned __int64& rValue) { return self.UIntDivOp(rValue); }
        friend FlaggedDec& operator/=(FlaggedDec& self, const __int16& rValue) { return self.IntDivOp(rValue); }
        friend FlaggedDec& operator/=(FlaggedDec& self, const __int8& rValue) { return self.IntDivOp(rValue); }
        friend FlaggedDec& operator/=(FlaggedDec& self, const unsigned __int16& rValue) { return self.UIntDivOp(rValue); }
        friend FlaggedDec& operator/=(FlaggedDec& self, const unsigned __int8& rValue) { return self.UIntDivOp(rValue); }

        /// <summary>
        /// Division Operation with Integer right side value
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator/=(FlaggedDec* self, const FlaggedDec& rValue) { return self->DivOp(rValue); }

        /// <summary>
        /// Division Operation with Integer right side value
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator/=(FlaggedDec& self, const FlaggedDec& rValue) { return self.DivOp(rValue); }

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec operator*(FlaggedDec self, const FlaggedDec& rValue) { return self.MultOp(rValue); }
        friend FlaggedDec operator*(FlaggedDec self, const int& rValue) { return self.Int32MultOp(rValue); }
        friend FlaggedDec operator*(FlaggedDec self, const unsigned int rValue) { return self.UInt32MultOp(rValue); }
        friend FlaggedDec operator*(FlaggedDec self, const signed long long rValue) { return self.Int64MultOp(rValue); }
        friend FlaggedDec operator*(FlaggedDec self, const unsigned __int64 rValue) { return self.UIntMultOp(rValue); }
        friend FlaggedDec operator*(FlaggedDec self, const __int16& rValue) { return self.IntMultOp(rValue); }
        friend FlaggedDec operator*(FlaggedDec self, const unsigned __int16& rValue) { return self.UIntMultOp(rValue); }
        friend FlaggedDec operator*(FlaggedDec self, const signed __int8& rValue) { return self.IntMultOp(rValue); }
        friend FlaggedDec operator*(FlaggedDec self, const unsigned __int64& rValue) { return self.UIntMultOp(rValue); }

        /// <summary>
        /// Multiplication Operation with Integer right side value
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator*=(FlaggedDec& self, const int& rValue) { return self.Int32MultOp(rValue); }
        friend FlaggedDec& operator*=(FlaggedDec& self, const __int64& rValue) { return self.Int64MultOp(rValue); }
        friend FlaggedDec& operator*=(FlaggedDec& self, const unsigned int& rValue) { return self.UIntMultOp(rValue); }
        friend FlaggedDec& operator*=(FlaggedDec& self, const unsigned __int64& rValue) { return self.UIntMultOp(rValue); }
        friend FlaggedDec& operator*=(FlaggedDec& self, const __int16& rValue) { return self.Int32MultOp(rValue); }
        friend FlaggedDec& operator*=(FlaggedDec& self, const __int8& rValue) { return self.Int64MultOp(rValue); }
        friend FlaggedDec& operator*=(FlaggedDec& self, const unsigned __int16& rValue) { return self.UIntMultOp(rValue); }
        friend FlaggedDec& operator*=(FlaggedDec& self, const unsigned __int8& rValue) { return self.UIntMultOp(rValue); }

        /// <summary>
        /// Multiplication Operation with Integer right side value
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator*=(FlaggedDec* self, const FlaggedDec& rValue) { return self->MultOp(rValue); }

        /// <summary>
        /// Multiplication Operation with Integer right side value
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator*=(FlaggedDec& self, const FlaggedDec& rValue) { return self.MultOp(rValue); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec operator+(FlaggedDec self, const FlaggedDec& rValue) { return self.AddOp(rValue); }
        friend FlaggedDec operator+(FlaggedDec self, const int& rValue) { return self.Int32AddOp(rValue); }
        friend FlaggedDec operator+(FlaggedDec self, const unsigned int rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec operator+(FlaggedDec self, const signed long long rValue) { return self.Int64AddOp(rValue); }
        friend FlaggedDec operator+(FlaggedDec self, const unsigned __int64 rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec operator+(FlaggedDec self, const __int16& rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec operator+(FlaggedDec self, const unsigned __int16& rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec operator+(FlaggedDec self, const signed __int8& rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec operator+(FlaggedDec self, const unsigned __int64& rValue) { return self.IntAddOp(rValue); }

        /// <summary>
        /// Addition Operation with Integer right side value
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator+=(FlaggedDec& self, const int& rValue) { return self.Int32AddOp(rValue); }
        friend FlaggedDec& operator+=(FlaggedDec& self, const __int64& rValue) { return self.Int64AddOp(rValue); }
        friend FlaggedDec& operator+=(FlaggedDec& self, const unsigned int& rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec& operator+=(FlaggedDec& self, const unsigned __int64& rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec& operator+=(FlaggedDec& self, const unsigned __int16& rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec& operator+=(FlaggedDec& self, const signed __int16& rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec& operator+=(FlaggedDec& self, const unsigned __int8& rValue) { return self.IntAddOp(rValue); }
        friend FlaggedDec& operator+=(FlaggedDec& self, const signed __int8& rValue) { return self.IntAddOp(rValue); }

        /// <summary>
        /// Addition Operation with Integer right side value
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator+=(FlaggedDec* self, const FlaggedDec& rValue) { return self->AddOp(rValue); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator+=(FlaggedDec& self, const FlaggedDec& rValue) { return self.AddOp(rValue); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec operator-(FlaggedDec self, const FlaggedDec& rValue) { return self.SubOp(rValue); }
        friend FlaggedDec operator-(FlaggedDec self, const int& rValue) { return self.Int32SubOp(rValue); }
        friend FlaggedDec operator-(FlaggedDec self, const unsigned int rValue) { return self.IntSubOp(rValue); }
        friend FlaggedDec operator-(FlaggedDec self, const signed long long rValue) { return self.Int64SubOp(rValue); }
        friend FlaggedDec operator-(FlaggedDec self, const unsigned __int64 rValue) { return self.IntSubOp(rValue); }
        friend FlaggedDec operator-(FlaggedDec self, const __int16& rValue) { return self.IntSubOp(rValue); }
        friend FlaggedDec operator-(FlaggedDec self, const unsigned __int16& rValue) { return self.IntSubOp(rValue); }
        friend FlaggedDec operator-(FlaggedDec self, const signed __int8& rValue) { return self.IntSubOp(rValue); }
        friend FlaggedDec operator-(FlaggedDec self, const unsigned __int64& rValue) { return self.IntSubOp(rValue); }

        /// <summary>
        /// Subtraction Operation Between FlaggedDec and Integer Value
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator-=(FlaggedDec& self, const int& rValue) { return self.Int32SubOp(rValue); }
        friend FlaggedDec& operator-=(FlaggedDec& self, const __int64& rValue) { return self.Int64SubOp(rValue); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator-=(FlaggedDec* self, const FlaggedDec& rValue) { return self->SubOp(rValue); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator-=(FlaggedDec& self, const FlaggedDec& rValue) { return self.SubOp(rValue); }



    #pragma endregion Main Operator Overrides

    #pragma region Other Operators

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>FlaggedDec</returns>
        friend FlaggedDec& operator-(FlaggedDec& self)
        {
            self.SwapNegativeStatus(); return self;
        }

        /// <summary>
        /// ++FlaggedDec Operator
        /// </summary>
        /// <returns>FlaggedDec &</returns>
        FlaggedDec& operator ++()
        {
    #if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
    #endif
            IntHalfAddition(1);
            return *this;
        }

        /// <summary>
        /// ++FlaggedDec Operator
        /// </summary>
        /// <returns>FlaggedDec &</returns>
        FlaggedDec& operator --()
        {
    #if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
    #endif
            IntHalfSubtraction(1);
            return *this;
        }

        /// <summary>
        /// FlaggedDec++ Operator
        /// </summary>
        /// <returns>FlaggedDec</returns>
        FlaggedDec operator ++(int)
        {
            FlaggedDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// FlaggedDec-- Operator
        /// </summary>
        /// <returns>FlaggedDec</returns>
        FlaggedDec operator --(int)
        {
            FlaggedDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// FlaggedDec* Operator
        /// </summary>
        /// <returns>FlaggedDec &</returns>
        FlaggedDec& operator *()
        {
            return *this;
        }

        /// <summary>
        /// LeftShift operation lValue*(2ToPowOf(rValue))
        /// (Variation without modifying owner object)
        /// Based on (https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/)
        /// </summary>
        /// <returns>FlaggedDec &</returns>
        FlaggedDec operator<<(const FlaggedDec& rValue)//FlaggedDec& operator<<(const FlaggedDec& rValue)
        {//Allowing negative shift operations based on formula instead of returning undefined
            FlaggedDec rightSideMultiplier = Two;
            rightSideMultiplier.PowOp(rValue);
            return MultiplyByUnsigned(rightSideMultiplier);
        }

        /// <summary>
        /// RightShift operation lValue/(2ToPowOf(rValue))
        /// (Variation without modifying owner object)
        /// Based on (https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/)
        /// </summary>
        /// <returns>FlaggedDec &</returns>
        FlaggedDec operator>>(const FlaggedDec& rValue)//operator<<(const FlaggedDec& rValue)
        {//Allowing negative shift operations based on formula instead of returning undefined
            FlaggedDec rightSideDivisor = Two;
            rightSideDivisor.PowOp(rValue);
            return DivideByUnsigned(rightSideDivisor);
        }

    #pragma endregion Other Operators

    #pragma region Modulus Operations
    #if !defined(AltNum_PreventModulusOverride)
        //Performs modulus operation based on "C = A - B * (A / B)" formula
        //Designed for use with normal, decimaled fractionals if both have same ExtraRep field, PiNum, and ENum  representation types
        //Will not work with non-decimaled format fractionals or mixed fractions
        //Modifies left side value with result
        template<IntegerType IntType=int>
        FlaggedDec BasicIntRemOp(IntType rValue)
        {
            if (DecimalHalf == 0)
                IntValue %= rValue;
            else
            {
                if (rValue < 0)
                    rValue *= -1;
                __int64 SRep = DecimalOverflowX * IntHalfAsAbs() + DecimalHalf;
                __int64 divRes = SRep / rValue;
                __int64 C = SRep - rValue * divRes;
                if (C == 0)
                    SetAsZero();
                else
                {
                    divRes = C / DecimalOverflowX;
                    C = C - DecimalOverflowX * divRes;
                    IntValue = divRes;
                    DecimalHalf = C;
                }
            }
            return *this;
        }

        FlaggedDec Int32BasicRemOp(signed int& RValue) { return BasicIntRemOp(RValue); }
        FlaggedDec UInt32BasicRemOp(unsigned int& RValue) { return BasicIntRemOp(RValue); }
        FlaggedDec Int64BasicRemOp(signed long long& RValue) { return BasicIntRemOp(RValue); }
        FlaggedDec UInt64BasicRemOp(unsigned long long& RValue) { return BasicIntRemOp(RValue); }

        FlaggedDec Int32BasicRem(signed int RValue) { FlaggedDec self = *this; return BasicIntRemOp(RValue); }
        FlaggedDec UInt32BasicRem(unsigned int RValue) { FlaggedDec self = *this; return BasicIntRemOp(RValue); }
        FlaggedDec Int64BasicRem(signed long long RValue) { FlaggedDec self = *this; return BasicIntRemOp(RValue); }
        FlaggedDec UInt64BasicRem(unsigned long long RValue) { FlaggedDec self = *this; return BasicIntRemOp(RValue); }

        static FlaggedDec Int32BasicModulus(FlaggedDec self, signed int RValue) { return self.BasicIntRemOp(RValue); }
        static FlaggedDec UInt32BasicModulus(FlaggedDec self, unsigned int RValue) { return self.BasicIntRemOp(RValue); }
        static FlaggedDec Int64BasicModulus(FlaggedDec self, signed long long RValue) { return self.BasicIntRemOp(RValue); }
        static FlaggedDec UInt64BasicModulus(FlaggedDec self, unsigned long long RValue) { return self.BasicIntRemOp(RValue); }

        //Performs modulus operation based on "C = A - B * (A / B)" formula
        template<IntegerType IntType=int>
        FlaggedDec IntRemOp(const IntType& rValue)
        {
            FlaggedDec divRes = DivideByInt(rValue);
            FlaggedDec C = *this - divRes.MultiplyByInt(rValue);
            return C;
        }

        /// <summary>
        ///  Modulus Operation
        /// </summary>
        /// <param name="RValue">The value.</param>
        FlaggedDec Int32RemOp(signed int& rValue)
        { 
            FlaggedDec divRes = DivideByInt32(rValue);
            FlaggedDec C = *this - divRes.MultiplyByInt32(rValue);
            return C;
        }
        FlaggedDec UInt32RemOp(unsigned int& RValue) { return IntRemOp(RValue); }
        FlaggedDec Int64RemOp(signed long long& RValue) { return IntRemOp(RValue); }
        FlaggedDec UInt64RemOp(unsigned long long& RValue) { return IntRemOp(RValue); }

        FlaggedDec Int32Rem(signed int RValue) { FlaggedDec self = *this; return self.IntRemOp(RValue); }
        FlaggedDec UInt32Rem(unsigned int RValue) { FlaggedDec self = *this; return self.IntRemOp(RValue); }
        FlaggedDec Int64Rem(signed long long RValue) { FlaggedDec self = *this; return self.IntRemOp(RValue); }
        FlaggedDec UInt64Rem(unsigned long long RValue) { FlaggedDec self = *this; return self.IntRemOp(RValue); }

        static FlaggedDec Int32Modulus(FlaggedDec self, signed int RValue) { return self.IntRemOp(RValue); }
        static FlaggedDec UInt32Modulus(FlaggedDec self, unsigned int RValue) { return self.IntRemOp(RValue); }
        static FlaggedDec Int64Modulus(FlaggedDec self, signed long long RValue) { return self.IntRemOp(RValue); }
        static FlaggedDec UInt64Modulus(FlaggedDec self, unsigned long long RValue) { return self.IntRemOp(RValue); }


        //Performs modulus operation based on "C = A - B * (A / B)" formula
        //Designed for use with normal, decimaled fractionals if both have same ExtraRep field, PiNum, and ENum  representation types
        //Will not work with non-decimaled format fractionals or mixed fractions
        //Modifies left side value with result
        FlaggedDec& BasicRemOp(const FlaggedDec& rValue)
        {
            if (rValue.DecimalHalf == 0)
            {
                if (DecimalHalf == 0)
                {
                    IntValue %= rValue.IntValue;
                }
                else
                {
                    __int64 SRep = DecimalOverflowX * IntHalfAsAbs() + DecimalHalf;
                    __int64 divRes; __int64 C;
                    if (rValue < 0)
                    {
                        divRes = SRep / -rValue.IntValue;
                        C = SRep - (-rValue.IntValue * divRes);
                    }
                    else
                    {
                        divRes = SRep / rValue.IntValue;
                        C = SRep - (rValue.IntValue * divRes);
                    }
                    if (C == 0)
                        SetAsZero();
                    else
                    {
                        divRes = C / DecimalOverflowX;
                        C = C - DecimalOverflowX * divRes;
                        IntValue = divRes;
                        DecimalHalf = C;
                    }
                }
            }
            else
            {
                FlaggedDec divRes = *this / rValue;
                FlaggedDec C = *this - rValue * divRes;
                IntValue = C.IntValue; DecimalHalf = C.DecimalHalf;
                ExtraRep = C.ExtraRep;
            }
        }

        FlaggedDec BasicRem(FlaggedDec rValue) { FlaggedDec self = *this; BasicRemOp(rValue); return self; }

        /// <summary>
        //	Performs modulus operation based on "C = A - B * (A / B)" formula
        /// </summary>
        /// <param name="rValue">The rightside value.</param>
        /// <returns>FlaggedDec</returns>
        FlaggedDec RemOp(const FlaggedDec& rValue)
        {
            FlaggedDec divRes = *this / rValue;
            FlaggedDec C = *this - rValue * divRes;
            return C;
        }

        FlaggedDec Rem(const FlaggedDec& rValue) { FlaggedDec self = *this; return self.RemOp(rValue); }

        /// <summary>
        /// Modulus Operation (Division operation that returns the remainder result)
        /// </summary>
        /// <param name="self">The leftside value.</param>
        /// <param name="Value">The rightside value.</param>
        /// <returns>FlaggedDec&</returns>
        static FlaggedDec Modulus(FlaggedDec self, FlaggedDec Value) { return self.RemOp(Value); }

        void CatchAllRem(FlaggedDec& rValue, RepType LRep, RepType RRep)
        {
            ConvertToNormType(LRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormType(RRep);
            BasicRemOp(convertedRVal);
        }
        
        void CatchAllRemV2(FlaggedDec& rValue, RepType SameRep)
        {
            ConvertToNormType(SameRep);
            FlaggedDec convertedRVal = rValue.ConvertAsNormType(SameRep);
            BasicRemOp(convertedRVal);
        }
        
        void CatchAllRemV3(FlaggedDec& rValue)
        {
            ConvertToNormTypeV2();
            FlaggedDec convertedRVal = rValue.ConvertAsNormTypeV2();
            BasicRemOp(convertedRVal);
        }

        friend FlaggedDec operator%=(FlaggedDec& self, FlaggedDec Value) { return self.RemOp(Value); }
        friend FlaggedDec operator%=(FlaggedDec& self, int Value) { return self.Int32RemOp(Value); }
        friend FlaggedDec operator%=(FlaggedDec& self, unsigned int Value) { return self.UInt32RemOp(Value); }
        friend FlaggedDec operator%=(FlaggedDec& self, signed long long Value) { return self.Int64RemOp(Value); }
        friend FlaggedDec operator%=(FlaggedDec& self, unsigned __int64 Value) { return self.UInt64RemOp(Value); }

        friend FlaggedDec operator%(FlaggedDec self, FlaggedDec Value) { return self.RemOp(Value); }
        friend FlaggedDec operator%(FlaggedDec self, int Value) { return self.Int32RemOp(Value); }
        friend FlaggedDec operator%(FlaggedDec self, unsigned int Value) { return self.UInt32RemOp(Value); }
        friend FlaggedDec operator%(FlaggedDec self, signed long long Value) { return self.Int64RemOp(Value); }
        friend FlaggedDec operator%(FlaggedDec self, unsigned __int64 Value) { return self.UInt64RemOp(Value); }


        #if defined(AltNum_EnableAlternativeModulusResult)//Return AltNumModChecker<FlaggedDec> Result with both Remainder and division result
        template<IntegerType IntType=int>
        static AltNumModChecker<FlaggedDec> IntModulusOp(IntType RValue)
        {
            AltNumModChecker<FlaggedDec> Res;
            Res.CalcIfZero(*this, AltNum(RValue));
            return Res;
        }

        /// <summary>
        ///  Modulus Operation
        /// </summary>
        /// <param name="RValue">The value.</param>
        AltNumModChecker<FlaggedDec> ModulusOp(signed int& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<FlaggedDec> ModulusOp(unsigned int& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<FlaggedDec> ModulusOp(signed long long& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<FlaggedDec> ModulusOp(unsigned long long& RValue) { IntModulusOp(RValue); }

        static AltNumModChecker<FlaggedDec> ModulusOp(FlaggedDec& self, signed int& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<FlaggedDec> ModulusOp(FlaggedDec& self, unsigned int& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<FlaggedDec> ModulusOp(FlaggedDec& self, signed long long& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<FlaggedDec> ModulusOp(FlaggedDec& self, unsigned long long& RValue) { self.IntModulusOp(RValue); }


        static AltNumModChecker<FlaggedDec> ModulusOp(const FlaggedDec& RValue)
        {
            AltNumModChecker<FlaggedDec> Res;
            Res.CalcIfZero(*this, RValue);
            return Res;
        }

        static AltNumModChecker<FlaggedDec> ModulusOp(FlaggedDec& self, FlaggedDec& RValue) { self.ModulusOp(RValue); }
        #endif

    #endif
    #pragma endregion Modulus Operations

    #pragma region Bitwise Functions
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between FlaggedDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType=int>
        friend FlaggedDec operator^(FlaggedDec self, IntType Value)
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
        /// Bitwise Or Operation Between FlaggedDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType=int>
        friend FlaggedDec operator|(FlaggedDec self, IntType Value)
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
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& Abs()
        {
            if (IntValue == NegativeRep)
                IntValue = 0;
            else if (IntValue < 0)
                IntValue *= -1;
            return *this;
        }

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Abs(FlaggedDec tValue)
        {
            return tValue.Abs();
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& FloorOp()
        {
            RepType repType = GetRepType();
            switch (repType)
            {
            case RepType::NormalType:
                break;
            case RepType::PiNum:
                ConvertPiToNum();
                break;
            case RepType::ApproachingBottom:
            case RepType::ApproachingTop:
                if (IntValue == NegativeRep)
                    IntValue = 0;
                ExtraRep = 0;
                return *this;
                break;
            default:
                ConvertToNormTypeV2();
                break;
            }
            if (DecimalHalf == 0)
            {
                return *this;
            }
            DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        static FlaggedDec Floor(FlaggedDec Value, int precision=0)
        {
            Value.ConvertToNormTypeV2();
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
        /// <returns>FlaggedDec&</returns>
        FlaggedDec& CeilOp()
        {
            RepType repType = GetRepType();
            switch (repType)
            {
            case RepType::NormalType:
                break;
            case RepType::PiNum:
                ConvertPiToNum();
                break;
            case RepType::ApproachingBottom:
            case RepType::ApproachingTop:
                if (IntValue == NegativeRep)
                    IntValue = 0;
                else
                    ++IntValue;
                ExtraRep = 0;
                return *this;
                break;
            default:
                //ConvertToNormTypeV2();//Prevent losing imaginary numbers
                break;
            }
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

        static FlaggedDec Ceil(FlaggedDec Value) { return Value.CeilOp(); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>FlaggedDec&</returns>
        static int FloorInt(FlaggedDec Value)
        {
            RepType repType = Value.GetRepType();
            switch (repType)
            {
            case RepType::NormalType:
                break;
            case RepType::PiNum:
                Value.ConvertPiToNum();
                break;
            //case RepType::ApproachingTop:
            //case RepType::ApproachingBottom:
            //    return Value.IntValue;
            //    break;
            default:
                Value.ConvertToNormTypeV2();
                break;
            }
            if (Value.DecimalHalf == 0)
            {
                return Value.GetIntHalf();
            }
            if (Value.IntValue == NegativeRep) { return -1; }
            else
            {
                return Value.GetIntHalf() - 1;
            }
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>FlaggedDec&</returns>
        int CeilIntOp()
        {
            RepType repType = GetRepType();
            switch (repType)
            {
                case RepType::NormalType:
                    break;
                case RepType::PiNum:
                {
                    ConvertPiToNum();//return CeilInt(ConvertPiToNum());
                    break;
                }
    #if defined(AltNum_EnableERep)
                case RepType::ENum:
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(Value/(ExtraRep*-1))*E Representation
            #else
                case RepType::EFractional://  IntValue/DecimalHalf*E Representation
            #endif
        #endif
                    ConvertToNormType(repType);
                    break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                    break;
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
            #else
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
                    ConvertToNormalIRepV2();
                    break;
        #endif
    #endif
                //case RepType::ApproachingTop:
                //case RepType::ApproachingBottom:
                default:
                    ConvertToNormType(repType);
                    break;
            }
            if (DecimalHalf == 0)
            {
                return GetIntHalf();
            }
            if (IntValue == NegativeRep) { return 0; }
            else
            {
                return GetIntHalf() + 1;
            }
        }
        
        static int CeilInt(FlaggedDec Value) { return Value.CeilIntOp(); }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>FlaggedDec &</returns>
        FlaggedDec& Trunc()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            else if (DecimalHalf == ApproachingBottomRep)
            {
                DecimalHalf = 0; ExtraRep = 0;
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
                DecimalHalf = 999999999; ExtraRep = 0;
            }
            else
            {
#endif
                ConvertToNormTypeV2();
                DecimalHalf = 0;
#if defined(AltNum_EnableInfinityRep)
            }
#endif
            return *this;
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Trunc(FlaggedDec Value)
        {
            return Value.Trunc();
        }
    #pragma endregion Math Etc Functions

    #pragma region Pow and Sqrt Functions
        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static FlaggedDec BasicSqrt(FlaggedDec& value, int precision=7)
        {//Ignores Alternate representations use Sqrt instead to check based on RepType
            if (value.DecimalHalf == 0)
            {
                bool AutoSetValue = true;
                switch (value.GetIntHalf())
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
            FlaggedDec number = value;
            FlaggedDec start = 0, end = number;
            FlaggedDec mid;

            // variable to store the answer 
            FlaggedDec ans;

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
            FlaggedDec increment = "0.1";
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
        static FlaggedDec Sqrt(FlaggedDec value, int precision=7)
        {
            value.ConvertToNormTypeV2();
            BasicSqrt(value, precision);
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="exponent">The exponent value.</param>
        template<IntegerType IntType>
        FlaggedDec BasicIntPowOp(const IntType& exponent)
        {
            if (exponent == 1) { return *this; }//Return self
            else if (exponent == 0)
            {
                IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
            }
            else if (exponent < 0)//Negative Pow
            {
                if (DecimalHalf == 0 && IntValue == 10 && exponent >= -9)
                {
                    IntValue = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[exponent * -1];
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    int expValue = exponent * -1;
                    FlaggedDec self = *this;
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
                IntValue = VariableConversionFunctions::PowerOfTens[exponent];
            else if (DecimalHalf == 0 && IntValue == -10 && ExtraRep == 0)
                IntValue = exponent % 2 ? VariableConversionFunctions::PowerOfTens[exponent] : VariableConversionFunctions::PowerOfTens[exponent] * -1;
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                int expValue = exponent;
                FlaggedDec self = *this;
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

        /// <summary>
        /// Applies Power of operation on references(for unsigned integer exponents)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="exponent">The exponent value.</param>
        template<IntegerType IntType>
        FlaggedDec BasicUIntPowOp(const IntType& exponent)
        {
            if (exponent == 1) { return *this; }//Return self
            else if (exponent == 0)
            {
                IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
            }
            else if (DecimalHalf == 0 && IntValue == 10 && ExtraRep == 0)
                IntValue = VariableConversionFunctions::PowerOfTens[exponent];
            else if (DecimalHalf == 0 && IntValue == -10 && ExtraRep == 0)
                IntValue = exponent % 2 ? VariableConversionFunctions::PowerOfTens[exponent] : VariableConversionFunctions::PowerOfTens[exponent] * -1;
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                int expValue = exponent;
                FlaggedDec self = *this;
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

        template<IntegerType IntType>
        FlaggedDec BasicIntPow(const IntType& expValue) { FlaggedDec self = *this; return self.BasicIntPowOp(expValue); }

        template<IntegerType IntType>
        FlaggedDec BasicUIntPow(const IntType& expValue) { FlaggedDec self = *this; return self.BasicUIntPowOp(expValue); }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<IntegerType IntType>
        FlaggedDec IntPowOp(const IntType& expValue)
        {
            if (DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    SetAsUndefined();
#else
                    throw "Infinity to power of Zero returns Undefined value";
#endif
                else if (expValue < 0)
                    return Zero;
                else if (IntValue == -1 && expValue % 2 == 0)
                    IntValue = 1;
                else
                    return *this;//Returns infinity
                return *this;
            }
            else
                return BasicIntPowOp(expValue);
        }

        template<IntegerType IntType>
        FlaggedDec UIntPowOp(const IntType& expValue)
        {
            if (DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    SetAsUndefined;
#else
                    throw "Infinity to power of Zero returns Undefined value";
#endif
                else if (IntValue == -1 && expValue % 2 == 0)
                    IntValue = 1;
                else
                    return *this;//Returns infinity
                return *this;
            }
            else
                return BasicUIntPowOp(expValue);
        }

        template<IntegerType IntType>
        FlaggedDec IntPow(const IntType& expValue) { FlaggedDec self = *this; return self.IntPowOp(expValue); }

        template<IntegerType IntType>
        FlaggedDec UnsignedIntPow(const IntType& expValue) { FlaggedDec self = *this; return self.UIntPowOp(expValue); }
public:
        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="nValue">The nth value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>FlaggedDec</returns>
        FlaggedDec NthRootOp(int& n, FlaggedDec& precision = FlaggedDec::JustAboveZero)
        {
            //Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            FlaggedDec xPre = One;
            xPre += (*this - One)/n;
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            FlaggedDec delX = FlaggedDec(2147483647, 0);

            //  xK denotes current value of x 
            FlaggedDec xK;
            FlaggedDec xPrePower;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = xPre * nMinus1;
                xPrePower = xPre.IntPow(nMinus1);
                xK += DivideBy(xPrePower); //*this/xPrePower;
                xK.IntDivOp(n);
                delX = FlaggedDec::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        FlaggedDec NthRoot(FlaggedDec value, int n, FlaggedDec precision = FlaggedDec::JustAboveZero) { return value.NthRootOp(n, precision); }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        FlaggedDec FractionalPow(int expNum, int expDenom);

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        FlaggedDec FractionalPow(boost::rational<int>& Frac);

        FlaggedDec BasicPowOp(FlaggedDec& expValue);

        FlaggedDec BasicPow(FlaggedDec expValue) { FlaggedDec self = *this; return self.BasicPowOp(expValue); }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="exponent">The exponent value.</param>
        FlaggedDec PowOp(const FlaggedDec& exponent);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        FlaggedDec Pow(FlaggedDec expValue) { FlaggedDec self = *this; return self.PowOp(expValue); }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="targetValue">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static FlaggedDec PowV2(FlaggedDec targetValue, FlaggedDec expValue) { return targetValue.PowOp(expValue); }
    #pragma endregion Pow and Sqrt Functions

    #pragma region Log Functions
        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static FlaggedDec NthRootV2(FlaggedDec targetValue, int n, FlaggedDec& Precision = FlaggedDec::FiveBillionth)
        {
            int nMinus1 = n - 1;
            FlaggedDec x[2] = { (FlaggedDec::One / n) * ((targetValue*nMinus1) + (targetValue / targetValue.IntPow(nMinus1))), targetValue };
            while (FlaggedDec::Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (FlaggedDec::One / n) * ((x[1]*nMinus1) + (targetValue / x[1].IntPow(nMinus1)));
            }
            return x[0];
        }

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Exp(FlaggedDec& x)
        {
            //x.ConvertToNormTypeV2();//Prevent losing imaginary number status
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
             *      - x: (FlaggedDec float) power of e to evaluate
             * Returns:
             *      - (FlaggedDec float) approximation of e^x in FlaggedDec precision
             */
             // Check that x is a valid input.
            assert(-709 <= x.IntValue && x.IntValue <= 709);
            // When x = 0 we already know e^x = 1.
            if (x.IsZero()) {
                return FlaggedDec::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            FlaggedDec x0 = FlaggedDec::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            FlaggedDec tn = FlaggedDec::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = FlaggedDec::CeilInt(x0 * FlaggedDec::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + FlaggedDec::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = FlaggedDec::One / tn;
            }
            return tn;
        }
protected:
    static FlaggedDec LnRef_Part02(FlaggedDec& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
        FlaggedDec TotalRes = (value - 1) / (value + 1);
        FlaggedDec LastPow = TotalRes;
        FlaggedDec WSquared = TotalRes * TotalRes;
        FlaggedDec AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > FlaggedDec::JustAboveZero);
        return TotalRes * 2;
    }
public:
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::FlaggedDec</returns>
        static FlaggedDec LnRef(FlaggedDec& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == FlaggedDec::One)
                return FlaggedDec::Zero;
            RepType repType = value.GetRepType();
            FlaggedDec ConvertedVal;
            switch (repType)
            {
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
#endif
#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
        }
            if (ConvertedVal.IntValue>=0&&ConvertedVal.IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                FlaggedDec threshold = FlaggedDec::FiveMillionth;
                FlaggedDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                FlaggedDec term = base;       // First term
                FlaggedDec prev;          // Previous sum
                FlaggedDec result = term;     // Kick it off

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
                } while (FlaggedDec::Abs(prev - result) > threshold);

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
        /// <returns>BlazesRusCode::FlaggedDec</returns>
        static FlaggedDec LnRefV2(FlaggedDec& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == FlaggedDec::One)
                return FlaggedDec::Zero;
            RepType repType = value.GetRepType();
            FlaggedDec ConvertedVal;
            switch (repType)
            {
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
#endif
#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
            }
            if(ConvertedVal.IntValue==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                FlaggedDec W = (value - 1)/ (value + 1);
                FlaggedDec TotalRes = W;
                W.SwapNegativeStatus();
                FlaggedDec LastPow = W;
                FlaggedDec WSquared = W * W;
                int WPow = 3;
                FlaggedDec AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > FlaggedDec::JustAboveZero);
                return TotalRes * 2;
            }
            else if (ConvertedVal.IntValue==1)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                FlaggedDec threshold = FlaggedDec::FiveMillionth;
                FlaggedDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                FlaggedDec term = base;       // First term
                FlaggedDec prev;          // Previous sum
                FlaggedDec result = term;     // Kick it off

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
                } while (FlaggedDec::Abs(prev - result) > threshold);

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
        static FlaggedDec Ln(FlaggedDec value)
        {
            return LnRef(value);
        }

protected:
    static FlaggedDec Log10_Part02(FlaggedDec& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        FlaggedDec TotalRes = (value - 1) / (value + 1);
        FlaggedDec LastPow = TotalRes;
        FlaggedDec WSquared = TotalRes * TotalRes;
        FlaggedDec AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > FlaggedDec::JustAboveZero);
        return TotalRes * FlaggedDec::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Log10(FlaggedDec value)
        {
            if (value == FlaggedDec::One)
                return FlaggedDec::Zero;
            RepType repType = value.GetRepType();
            FlaggedDec ConvertedVal;
            switch (repType)
            {
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
#endif
#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
            }
            if (ConvertedVal.DecimalHalf == 0 && ConvertedVal.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return FlaggedDec(index, 0);
                }
                return FlaggedDec(9, 0);
            }
            if (ConvertedVal.IntValue>=0&&ConvertedVal.IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                FlaggedDec threshold = FlaggedDec::FiveBillionth;
                FlaggedDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                FlaggedDec term = base;       // First term
                FlaggedDec prev = 0;          // Previous sum
                FlaggedDec result = term;     // Kick it off

                while (FlaggedDec::Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return result*FlaggedDec::LN10Mult;// result/FlaggedDec::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return Log10_Part02(value);
            }
        }

protected:
    template<IntegerType IntType=int>
    static FlaggedDec Log10_IntPart02(const IntType& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        FlaggedDec TotalRes = FlaggedDec((value - 1), 0) / FlaggedDec((value + 1), 0);
        FlaggedDec LastPow = TotalRes;
        FlaggedDec WSquared = TotalRes * TotalRes;
        FlaggedDec AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > FlaggedDec::JustAboveZero);
        return TotalRes * FlaggedDec::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>FlaggedDec</returns>
        template<IntegerType IntType=int>
        static FlaggedDec IntegerLog10(const IntType& value)
        {
            if (value == 1)
                return FlaggedDec::Zero;
            if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return FlaggedDec(index, 0);
                }
                return FlaggedDec(9, 0);
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
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Log(FlaggedDec value, FlaggedDec baseVal)
        {
            RepType repType = value.GetRepType();
            FlaggedDec ConvertedVal;
            switch (repType)
            {
    #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
            #else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
        #endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
    #endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
            }
            if (ConvertedVal == FlaggedDec::One)
                return FlaggedDec::Zero;
            return Log10(value) / Log10(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec IntegerLog(FlaggedDec value, int baseVal)
        {
            RepType repType = value.GetRepType();
            FlaggedDec ConvertedVal;
            switch (repType)
            {
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
#else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
#endif
#endif
                ConvertedVal = value.ConvertAsNormalIRep(repType);
                break;
#endif
            default:
                ConvertedVal = value.ConvertAsNormType(repType);
                break;
        }
            if (ConvertedVal == FlaggedDec::One)
                return FlaggedDec::Zero;
            //Calculate Base log first
            FlaggedDec baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = FlaggedDec(index, 0);
                        break;
                    }
                }
                baseTotalRes = FlaggedDec(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = FlaggedDec((baseVal - 1), 0) / FlaggedDec((baseVal + 1), 0);
                FlaggedDec baseLastPow = baseTotalRes;
                FlaggedDec baseWSquared = baseTotalRes * baseTotalRes;
                FlaggedDec baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > FlaggedDec::JustAboveZero);
            }

            //Now calculate other log
            if (ConvertedVal.DecimalHalf == 0 && ConvertedVal.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? FlaggedDec(index, 0) / (baseTotalRes * FlaggedDec::HalfLN10Mult): FlaggedDec(index, 0)/ baseTotalRes;
                }
                return lnMultLog? FlaggedDec(9, 0) / (baseTotalRes*FlaggedDec::HalfLN10Mult):FlaggedDec(9, 0)/baseTotalRes;
            }
            if (ConvertedVal.IntValue>=0&&ConvertedVal.IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                FlaggedDec threshold = FlaggedDec::FiveBillionth;
                FlaggedDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                FlaggedDec term = base;       // First term
                FlaggedDec prev = 0;          // Previous sum
                FlaggedDec result = term;     // Kick it off

                while (FlaggedDec::Abs(prev - result) > threshold) {
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
                FlaggedDec W = (value - 1) / (value + 1);
                FlaggedDec TotalRes = W;
                FlaggedDec AddRes;
                int WPow = 3;
                do
                {
                    AddRes = W.IntPow(WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > FlaggedDec::JustAboveZero);
                return lnMultLog? TotalRes/baseTotalRes:(TotalRes * FlaggedDec::HalfLN10Mult)/ baseTotalRes;
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
        /// <returns>FlaggedDec</returns>
        static FlaggedDec SinFromAngle(FlaggedDec Value)
        {
    #if defined(AltNum_EnableInfinityRep)
            if (Value.DecimalHalf == InfinityRep)
        #if defined(AltNum_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
        #elif defined(AltNum_EnableUndefinedButInRange)
            //Return undefined value between -1 and 1
        #else
                throw "Operation results in NaN";
        #endif
    #endif
            RepType repType = Value.GetRepType();
            switch (repType)
            {
    #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
            #else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
        #endif
                Value.ConvertToNormalIRep(repType);
                break;
    #endif
            default:
                Value.ConvertToNormType(repType);
                break;
            }
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
            if (Value == Zero) { return FlaggedDec::Zero; }
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
                return FlaggedDec::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else
            {
                FlaggedDec NewValue = Zero;
                //Angle as Radian
                FlaggedDec Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's Taylor series!!
                    NewValue += Radius.Pow(2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                }//x[1].IntPow(nMinus1)
                return NewValue;
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns></returns>
        static FlaggedDec CosFromAngle(FlaggedDec Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if (Value.DecimalHalf == InfinityRep)
#if defined(AltNum_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
#elif defined(AltNum_EnableUndefinedButInRange)
//Return undefined value between -1 and 1
#else
                throw "Operation results in NaN";
#endif
#endif
            RepType repType = Value.GetRepType();
            switch (repType)
            {
    #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
            #else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
        #endif
                Value.ConvertToNormalIRep(repType);
                break;
    #endif
            default:
                Value.ConvertToNormType(repType);
                break;
            }
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
                return FlaggedDec::Zero;
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return FlaggedDec::Zero;
            }
            else
            {
                FlaggedDec NewValue = Zero;
                //Angle as Radian
                FlaggedDec Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's also Taylor series!!
                    NewValue += Radius.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
                }
                return NewValue;
            }
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec BasicSinOperation(FlaggedDec& Value)
        {
            FlaggedDec SinValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += Value.Pow(2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

       /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Sin(FlaggedDec Value)
        {
/*
            if (Value.ExtraRep == PiRep)
            {


            }
#if defined(AltNum_EnableInfinityRep)
            else if(Value.DecimalHalf==InfinityRep)

#endif*/
            RepType repType = Value.GetRepType();
            switch (repType)
            {
                case RepType::PiNum://0 to 2Pi range (2Pi == 0Pi)
                {
                    if (Value.IntValue < 0)
                    {
                        if (Value.IntValue == NegativeRep)
                        {
                            Value.IntValue = 1; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                        }
                        else
                        {
                            Value.IntValue *= -1;
                            Value.IntValue %= 2;
                            Value.IntValue = 2 - Value.IntValue;
                            if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                        }
                    }
                    else
                    {
                        Value.IntValue %= 2;
                    }
                    if (Value.DecimalHalf == 0&&(Value.IntValue==0||Value.IntValue==1))//0 and Pi
                        return Zero;
                    if (Value.DecimalHalf == 500000000)//0.5 Pi = 1; 1.5Pi = -1
                        return Value.IntValue==0?NegativeOne:One;
                    FlaggedDec SinValue = Zero;
                    for (int i = 0; i < 7; ++i)
                    {
                        SinValue += Value.Pow(2 * i + 1) * (i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                    }
                    return SinValue;
                    break;
                }
        #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnablePiPowers)
            #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            #elif defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
            #else
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            #endif
            #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            #endif
                {
                    Value.ConvertToPiRep(repType);
                    if (Value.IntValue < 0)
                    {
                        if (Value.IntValue == NegativeRep)
                        {
                            Value.IntValue = 1; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                        }
                        else
                        {
                            Value.IntValue *= -1;
                            Value.IntValue %= 2;
                            Value.IntValue = 2 - Value.IntValue;
                            if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                        }
                    }
                    else
                    {
                        Value.IntValue %= 2;
                    }
                    if (Value.DecimalHalf == 0&&(Value.IntValue==0||Value.IntValue==1))//0 and Pi
                        return Zero;
                    if (Value.DecimalHalf == 500000000)//0.5 Pi = 1; 1.5Pi = -1
                        return Value.IntValue==0?NegativeOne:One;
                    FlaggedDec SinValue = Zero;
                    for (int i = 0; i < 7; ++i)
                    {
                        SinValue += Value.Pow(2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                    }
                    return SinValue;
                    break;
                }
        #endif
#if defined(AltNum_EnableInfinityRep)
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
#if defined(AltNum_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
#elif defined(AltNum_EnableUndefinedButInRange)
//Return undefined value between -1 and 1
#else
                throw "Operation results in NaN";
#endif
                    break;
#endif
            default:
                break;
            }
            return BasicSinOperation(Value);
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec BasicCosOperation(FlaggedDec& Value)
        {
            FlaggedDec CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                CosValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Cos(FlaggedDec Value)
        {
/*
            if (Value.ExtraRep == PiRep)//0 to 2Pi range (2Pi == 0Pi)
            {
                if (Value.IntValue < 0)
                {
                    if (Value.IntValue == NegativeRep)
                    {
                        Value.IntValue = 1; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                    }
                    else
                    {
                        Value.IntValue *= -1;
                        Value.IntValue %= 2;
                        Value.IntValue = 2 - Value.IntValue;
                        if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                    }
                }
                else
                {
                    Value.IntValue %= 2;
                }
                //Cos table listed on https://byjus.com/maths/value-of-cos-180/
                if (Value.DecimalHalf == 0)//cos(0) = 1;cos(Pi) = -1;
                    return Value.IntValue == 0?One:NegativeOne;
                if (Value.DecimalHalf == 500000000)//cos(0.5) Pi = 0; cos(1.5Pi) = 0;
                    return Zero;
            }
    #if defined(AltNum_EnableInfinityRep)
            else if(Value.DecimalHalf==InfinityRep)
        #if defined(AltNum_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
        #elif defined(AltNum_EnableUndefinedButInRange)
            //Return undefined value between -1 and 1
        #else
                throw "Operation results in NaN";
        #endif
    #endif
*/
            return BasicCosOperation(Value);
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec Tan(FlaggedDec Value)
        {
            FlaggedDec SinValue = Zero;
            FlaggedDec CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1)  / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            {
                CosValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>FlaggedDec</returns>
        static FlaggedDec TanFromAngle(FlaggedDec Value)
        {
            RepType repType = Value.GetRepType();
            switch (repType)
            {
    #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
            #else
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
        #endif
                Value.ConvertToNormalIRep(repType);
                break;
    #endif
            default:
                Value.ConvertToNormType(repType);
                break;
            }
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
            if (Value == Zero) { return FlaggedDec::Zero; }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                return Infinity;
#else
                return FlaggedDec::Maximum;//Positive Infinity
#endif
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return FlaggedDec::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                return FlaggedDec::NegativeInfinity;
#else
                return FlaggedDec::Minimum;//Negative Infinity
#endif
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
        /// <returns>FlaggedDec</returns>
        static FlaggedDec ATan(FlaggedDec Value)
        {
            FlaggedDec SinValue = Zero;
            FlaggedDec CosValue = Zero;
            //Angle as Radian
            for (int i = 0; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            { // That's also Taylor series!!
                CosValue += Value.Pow(2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
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
        /// <returns>FlaggedDec</returns>
        static FlaggedDec ArcTan2(FlaggedDec y, FlaggedDec x)
        {
#if defined(AltNum_EnablePiRep)
    #if defined(AltNum_EnableDecimaledPiFractionals)
            FlaggedDec coeff_1 = FlaggedDec(1, 0, -4);
    #elif defined(AltNum_EnablePiFractional)
            FlaggedDec coeff_1 = FlaggedDec(1, 4, PiByDivisorRep);
    #else
            FlaggedDec coeff_1 = FlaggedDec(0, 250000000, PiRep);//Pi / 4;
    #endif
#else
            FlaggedDec coeff_1 = PiNum / 4;
#endif
#if defined(AltNum_EnablePiRep)
    #if defined(AltNum_EnableDecimaledPiFractionals)
            FlaggedDec coeff_2 = FlaggedDec(3, 0, -4);
    #elif defined(AltNum_EnablePiFractional)
            FlaggedDec coeff_2 = FlaggedDec(3, 4, PiByDivisorRep);
    #else
            FlaggedDec coeff_2 = FlaggedDec(0, 750000000, PiRep);//Pi / 4;
    #endif
#else
            FlaggedDec coeff_2 = PiNum * FlaggedDec(0, 750000000);
#endif
            FlaggedDec abs_y = FlaggedDec::Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            FlaggedDec r;
            FlaggedDec angle;
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

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The target value to convert into FlaggedDec</param>
    inline void FlaggedDec::ReadString(std::string Value)
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
                IntHalfAddition(TempInt02);
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
    /// <returns>FlaggedDec</returns>
    inline FlaggedDec FlaggedDec::GetValueFromString(std::string Value)
    {
        FlaggedDec NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }
}
