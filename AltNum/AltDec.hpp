﻿// ***********************************************************************
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

#ifdef AltNum_EnableMediumDecBasedSetValues
    #include "MediumDec.hpp"
#endif

#include "AltNumModChecker.hpp"

//Preprocessor options
/*
AltNum_EnableFractionals =
      Enables fractional representations in attempt to preserve more accuracy during operations
      (Enables NumByDiv etc)
AltNum_EnableAlternativeRepFractionals =
      Enables integer based fractionals for alternative representations such as Pi(partially implimented)

//--Infinity based preprocessors--
AltNum_EnableInfinityRep = Enable support of positive/negative infinity representations and approaching value representations
      When DecimalHalf is -2147483648, it represents negative infinity(if IntValue is -1) or positive infinity(if IntValue is 1)
	 (Mostly Implimented)
AltNum_EnableApproachingValues
      When DecimalHalf is -2147483647 and ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0..1)
	  When DecimalHalf is -2147483646 and ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9..9)
     (Not Fully Implimented)
AltNum_EnableApproachingPi = Enables IntValue.9..9*Pi representation
AltNum_EnableApproachingE = Enables IntValue.9..9*e representation
AltNum_EnableApproachingI = Enables IntValue.9..9*i representation
AltNum_EnableApproachingDivided =
	Enables Approaching IntValue.49..9 and IntValue.50..1 and other Approaching values (49..9 = ExtraRep value of 2; 50..1 = ExtraRep value of -2)
	When DecimalHalf is -2147483647 and ExtraRep>1, represents (when IntValue is positive) IntValue + (1/ExtraRep + ApproachingBottomValue)(approaching left towards right)
	When DecimalHalf is -2147483646 and ExtraRep>1, represents (when IntValue is positive) IntValue + (1/ExtraRep + ApproachingTopValue)(approaching right towards left)
	(Partially Implimented)


AltNum_EnableInfinityPowers =
      Allows powers of infinity for operations where infinity is somehow more infinite than normal
      (Not Implimented)

AltNum_DisplayApproachingAsReal =
      Display approaching value as real number with 20 digits in decimal section
//----

AltNum_EnableNaN =
      Enable NaN based representations and operations(Not Fully Implimented)

AltNum_EnableHigherPrecisionPiConversion =
      (Not Implimented)

AltNum_UseAltDecBasedRepresentations =
      Forces to calculate certain representations like AltDec does 
      (preference for storing non-normal representations within value of negative DecimalHalf)
	  (Not Implimented)
	  
AltNum_EnableOverflowPreventionCode =
      Use to enable code to check for overflows on addition/subtraction/multiplication operations (return an exception if overflow)
      (Not Implimented Yet)
	  
AltNum_DisableInfinityRepTypeReturn = Disables infinity variables while allowing approaching variables to function(Not fully implimented)

AltNum_DisablePiRep =
      Force toggles AltNum_EnablePiRep to off
      AltNum_EnablePiRep given greator priority if both both AltNum_DisablePiRep and AltNum_EnablePiRep
      are set by preprocessor settings of project

AltNum_EnablePiRep =
      If AltNum_UseAltDecBasedRepresentations enabled, then
        Pi*(+- 2147483647.999999999) Representation enabled
        (When DecimalHalf is between -1 and -1000000000 (when DecimalHalf is -1000000000 is Equal to IntValue*Pi))
      Otherwise represents Pi within format of
         If DecimalHalf is positive and ExtraRep is -2147483647,
         then AltDec represents +- 2147483647.999999999 * Pi (Not Fully Implimented)
         If DecimalHalf is positive and and ExtraRep is between AlternativeFractionalLowerBound and 0,
         then AltDec represents (+- 2147483647.999999999 * Pi)/(ExtraRep*-1)
	  (Not Fully Implimented--Enabled by default if AltNum_DisablePiRep not set)

AltNum_EnableComplexNum =
      Enable Representation of complex numbers with Imaginary number operations
If AltNum_EnableFractionals not enabled, store value as IntValue.DecimalHalf + ExtraRePi
Otherwise requires AltNum_EnableBasicComplexNumber and ExtraRep value as ?, and stores value as IntValue + DecimalHalfi
(Might be better to just store as formula class feature or as another number class holding 2 AltDec or other AltNum values)
      (Requires AltNum_EnableImaginaryNum, Not Implimented)

AltNum_EnableMixedFractional =
      If DecimalHalf is negative and ExtraRep is Positive,
      then AltDec represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
      (Not Fully Implimented)

AltNum_EnableERep =
      If AltNum_UseAltDecBasedRepresentations enabled, then
    e*(+- 2147483647.999999999) Representation enabled
    (When DecimalHalf is between -1000000001 and -2000000000 (when DecimalHalf is -2000000000 is Equal to IntValue*e))
      Otherwise represents e within format of
    If DecimalHalf is positive and ExtraRep is -2147483646, then
       represents +- 2147483647.999999999 * e
    If DecimalHalf is positive and ExtraRep is between AlternativeFractionalLowerBound and 0, then
        represents (+- 2147483647.999999999 * e)/(ExtraRep*-1)
      (Not Fully Implimented)

AltNum_EnableImaginaryNum =
      If DecimalHalf is positive and ExtraRep is -2147483647, then
      represents +- 2147483647.999999999i
      If DecimalHalf is positive and ExtraRep is between AlternativeFractionalLowerBound and 0, then
      represents (+- 2147483647.999999999i)/(ExtraRep*-1)
      (Not Fully Implimented--Giving lesser priority to finish unless I need to use)

//----
AltNum_EnablePrivateRepType =
      Sets GetRepType code to be private instead of public

AltNum_DisableAutoToggleOfPreferedSettings =
      If not toggled, force enables AltNum_EnablePiRep, AltNum_EnableInfinityRep,
	  AltNum_EnableAlternativeRepFractionals & AltNum_EnableDecimaledPiFractionals

AltNum_DisableSwitchBasedConversion =

AltNum_EnableAltDecBasedSetValues =
	  
AltNum_EnableDecimaledAlternativeFractionals = 
   Automatically enabled if AltNum_EnableDecimaledPiFractionals, AltNum_EnableDecimaledEFractionals, or AltNum_EnableDecimaledEFractionals enabled
   Not to be confused with AltNum_EnableAlternativeRepFractionals(which only enabled Integer based alternative rep fractionals)

---Only one of the next set of 5 switches can be enabled at once:
AltNum_EnableDecimaledPiFractionals = Enables fractionals for Pi with non-integer numbers(not implimented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound
AltNum_EnableDecimaledEFractionals = Enables fractionals for e with non-integer numbers(not implimented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound
AltNum_EnableDecimaledIFractionals = Enables fractionals for i with non-integer numbers(not implimented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound

AltNum_EnablePiPowers =
      If ExtraRep value is between -1 and -2147483640, then represents IntValue.DecimalHalf * Pi^(ExtraRep*-1)
	  Can't be enabled at same time as AltNum_EnableDecimaledAlternativeFractionals
      (Not Fully Implimented)
AltNum_EnableNormalPowers =
      If ExtraRep value is between -1 and -2147483640, then represents IntValue.DecimalHalf^(ExtraRep*-1)
	  Can't be enabled at same time as AltNum_EnableDecimaledAlternativeFractionals or AltNum_EnablePiPowers
      (Not Implimented)
----===============================================================================================================

AltNum_OutputTruncatedTrailingDigits =
    Output to console trailing digits that are truncated when multiplication or division results in numbers getting too small(Not Implimented yet)
	(Impliment this before work to making working version with trailing digits such as for MixedDec (fixedpoint combined with floating point implimentations of decimal-like format classes)

AltNum_UseOldDivisionCode = Depressated (Working on removing all traces)
AltNum_AvoidUsingLargeInt = Removes AltNum_UseOldDivisionCode toggle and forces alternative code that doesn't need int128 from boost
AltNum_UseOldRemOpCode

AltNum_EnableBoostFractionalReduction
AltNum_EnableImaginaryInfinity = Enables imaginary infinity option
AltNum_DisableApproachingTop = Disables IntValue.9..9 representation and .5..1 etc

Only one of 3 alternative mixed fraction representations can be enabled at a time(use FlaggedDec variant if need all at once):
AltNum_EnableMixedPiFractional
AltNum_EnableMixedEFractional
AltNum_EnableMixedIFractional
Autotoggles AltNum_EnableAlternativeMixedFrac if any of 3 above are toggled

AltNum_EnableNilRep = Enables Nil representation(detection not in code right now)


AltNum_EnableUndefinedButInRange = Enable representation of unknown number between -Value and +Value for Cos of infinity etc
AltNum_EnableWithinMinMaxRange
AltNum_EnableUnknownTrigExpressions = (Not Implimented)

AltNum_PreventModulusOverride = Turns off modulus overrides if toggled
AltNum_EnableAlternativeModulusResult = Add addition modulus operations that give AltNumModChecker<AltNum> result
AltNum_EnableBitwiseOverride = Enables bitwise operation overrides if enabled

AltNum_EnablePiFractional = Autotoggled if AltNum_EnableAlternativeRepFractionals and AltNum_EnablePiRep enabled without AltNum_EnableDecimaledPiFractionals toggled
AltNum_EnableEFractional = Autotoggled if AltNum_EnableAlternativeRepFractionals and AltNum_EnableERep enabled without AltNum_EnableDecimaledEFractionals toggled
AltNum_EnableIFractional = Autotoggled if AltNum_EnableAlternativeRepFractionals and AltNum_EnableImaginaryNum enabled without AltNum_EnableDecimaledIFractionals toggled
AltNum_UsingAltFractional = Autotoggled if any of the above 3 are toggled

AltDec_UseMirroredInt
*/
#if !defined(AltNum_DisableAutoToggleOfPreferedSettings)||defined(AltNum_EnableAutoToggleOfPreferedSettings)
    #define AltNum_EnablePiRep
    #define AltNum_EnableInfinityRep
	#define AltNum_EnableAlternativeRepFractionals
    #define AltNum_EnableDecimaledPiFractionals
    #define AltNum_EnableApproachingValues
#endif

#if defined(AltNum_EnableImaginaryInfinity)
    #define AltNum_EnableImaginaryNum
	#define AltNum_EnableInfinityRep
#endif

//Force required preprocessor flag for AltNum_EnableAlternativeRepFractionals
#if defined(AltNum_EnableAlternativeRepFractionals)
	#if !defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableERep)&&!defined(AltNum_EnableIRep)
		#undef AltNum_EnableAlternativeRepFractionals//Alternative Fractionals require the related representations enabled
	#elif !defined(AltNum_EnableFractionals)
		#define AltNum_EnableFractionals
	#endif
#endif

#if defined(AltNum_AvoidUsingLargeInt)
    #undef AltNum_UseOldDivisionCode
#endif

//Force required flags to be enabled if AltNum_EnableApproachingDivided toggled
#if defined(AltNum_EnableApproachingDivided)
	#define AltNum_EnableApproachingValues
#endif

#if !defined(AltNum_EnableDecimaledAlternativeFractionals) && defined(AltNum_EnableDecimaledPiFractionals)
    #define AltNum_EnableDecimaledAlternativeFractionals
#endif

#if !defined(AltNum_EnableDecimaledAlternativeFractionals) && defined(AltNum_EnableDecimaledEFractionals)
    #define AltNum_EnableDecimaledAlternativeFractionals
#endif

#if !defined(AltNum_EnableDecimaledAlternativeFractionals) && defined(AltNum_EnableDecimaledIFractionals)
    #define AltNum_EnableDecimaledAlternativeFractionals
#endif

#if defined(AltNum_EnableDecimaledPiFractionals) && defined(AltNum_EnableDecimaledEFractionals)
    #undef AltNum_EnableDecimaledEFractionals
#endif

#if defined(AltNum_EnableDecimaledPiFractionals) && defined(AltNum_EnableDecimaledIFractionals)
    #undef AltNum_EnableDecimaledIFractionals
#endif

//Turn off Pi Power's feature if AltNum_EnableDecimaledAlternativeFractionals enabled
#if defined(AltNum_EnableDecimaledAlternativeFractionals) && defined(AltNum_EnablePiPowers)
	#undef AltNum_EnablePiPowers
#endif

#if (defined(AltNum_EnableDecimaledAlternativeFractionals)||defined(AltNum_EnablePiPowers)) && defined(AltNum_EnableNormalPowers)
	#undef AltNum_EnableNormalPowers
#endif

//If Pi rep is neither disabled or enabled, default to enabling Pi representation
#if !defined(AltNum_DisablePiRep) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(AltNum_EnablePiRep) && defined(AltNum_DisablePiRep)
    #undef AltNum_DisablePiRep
#endif

//Force enable Pi features if near Pi enabled
#if defined(AltNum_EnableApproachingPi) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(AltNum_EnableERep) && (defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnablePiPowers))
    #undef AltNum_EnableERep
#endif

#if defined(AltNum_EnableApproachingPi) && !defined(AltNum_EnablePiRep)
    #undef AltNum_EnableApproachingPi
#endif

#if defined(AltNum_EnableApproachingE) && !defined(AltNum_EnableERep)
    #undef AltNum_EnableApproachingE
#endif

#if defined(AltNum_EnableApproachingI) && !defined(AltNum_EnableImaginaryNum)
    #undef AltNum_EnableApproachingI
#endif

#if !defined(AltNum_EnablePiFractional) &&defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnablePiFractional
#endif
#if !defined(AltNum_EnableEFractional) &&defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnableEFractional
#endif
#if !defined(AltNum_EnablePiFractional) &&defined(AltNum_EnableIRep)&&!defined(AltNum_EnableDecimaledIFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnableIFractional
#endif

#if defined(AltNum_EnablePiFractional) || defined(AltNum_EnableEFractional) || defined(AltNum_EnableIFractional)
	#defined AltNum_UsingAltFractional//Shorthand for having any of above toggles active
#endif

#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional) || defined(AltNum_EnableMixedIFractional)
	#define AltNum_EnableAlternativeMixedFrac
#endif

#if (defined(AltNum_EnableMixedPiFractional)&&defined(AltNum_EnablePiFractional))||(defined(AltNum_EnableMixedEFractional)&&defined(AltNum_EnableEFractional))||(defined(AltNum_EnableMixedIFractional)&&defined(AltNum_EnableIFractional))
	#define AltNum_MixedAltFracHasFractionalAccess
#endif

#if (defined(AltNum_EnableMixedPiFractional)&&defined(AltNum_EnableDecimaledPiFractionals))||(defined(AltNum_EnableMixedEFractional)&&defined(AltNum_EnableDecimaledEFractionals))||(defined(AltNum_EnableMixedIFractional)&&defined(AltNum_EnableDecimaledIFractionals))
	#define AltNum_MixedAltFracHasDecimaledFractionalAccess
#endif

namespace BlazesRusCode
{
    class AltDec;

/*---Accuracy Tests(with AltDec based settings):
 * 100% accuracy for all integer value multiplication operations.
 * 100% accuracy for addition/subtraction operations
 * Partial but still high accuracy for non-integer representation variations of multiplication and division because of truncation
   (values get lost if get too small) (100% accuracy except for some truncated digits lost)
 * Other operations like Ln and Sqrt contained with decent level of accuracy
   (still loses a little accuracy because of truncation etc)
 * Operations and functions will mess up if IntValue overflows/underflows
*/

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// plus support for some fractal operations, and other representations like Pi(and optionally things like e or imaginary numbers)
    /// (12 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API AltDec
    {
    public:
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;

	protected:
        /// <summary>
        /// The decimal overflow value * -1
        /// </summary>
		static signed _int64 const NegDecimalOverflowX = -1000000000;
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

        /// <summary>
        /// Value when IntValue is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
#if !defined(AltDec_UseMirroredInt)
        static signed int const NegativeRep = -2147483648;
#else
        static MirroredInt NegativeRep;
        static signed int const NegativeRepVal = MirroredInt::NegativeRep;
#endif

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
#if !defined(AltDec_UseMirroredInt)
        signed int IntValue;
#else
        MirroredInt IntValue;
#endif

        bool IsNegative()
        {
#if !defined(AltDec_UseMirroredInt)
            return IntValue<0;
#else
            return IntValue.IsNegative();
#endif
        }

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        signed int DecimalHalf;
		
        /// <summary>
		/// (Used exclusively for alternative represents of numbers including imaginery numbers and for fractionals)
        /// If both DecimalHalf&ExtraRep are Positive with ExtraRep as non-zero, then ExtraRep acts as denominator
        /// If DecimalHalf is negative and ExtraRep is Positive, then AltDec represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
        /// If ExtraRep is zero and DecimalHalf is positive, then AltDec represents +- 2147483647.999999999
        ///-----------------------------------------------
        /// If ExtraRep is negative, it acts as representation type similar to AltDec:
		/// If ExtraRep is between 0 and , it acts as representation type similar to AltDec:
        /// If DecimalHalf is positive and ExtraRep is -2147483648 and AltNum_EnablePiRep is enabled, then AltDec represents +- 2147483647.999999999 * Pi
        /// </summary>
        signed int ExtraRep;

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt()
        {
            return IntValue==0||IntValue==NegativeRep;
        }

        bool IsNotAtZeroInt()
        {
            return IntValue != 0 && IntValue != NegativeRep;
        }

        signed int GetIntHalf()
        {
#if defined(AltDec_UseMirroredInt)
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
#if defined(AltDec_UseMirroredInt)
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
#if defined(AltDec_UseMirroredInt)
            return (std::string) IntValue;
#else
            if (IntValue == NegativeRep)
                return "-0";
            return VariableConversionFunctions::IntToStringConversion(IntValue);
#endif
        }

#if defined(AltDec_UseMirroredInt)
        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.(Default constructor)
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        AltDec(MirroredInt intVal = MirroredInt::Zero, signed int decVal = 0, signed int extraVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }
#endif

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
#if !defined(AltDec_UseMirroredInt)
        AltDec(int intVal=0, signed int decVal = 0, signed int extraVal = 0)
#else
        AltDec(int intVal, signed int decVal = 0, signed int extraVal = 0)
#endif
        {
#if defined(AltDec_UseMirroredInt)&&defined(BlazesMirroredInt_UseLegacyValueBehavior)
            IntValue.Value = intVal;
#else
            IntValue = intVal;
#endif
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

        AltDec(const AltDec&) = default;

        AltDec& operator=(const AltDec&) = default;

        //Detect if at exactly zero
		bool IsZero()
		{
            return DecimalHalf==0&&IntValue==0;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(AltDec Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf; ExtraRep = Value.ExtraRep;
        }

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0; ExtraRep = 0;
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

    #pragma region Const Representation values
    protected:
	#if defined(AltNum_EnableInfinityRep)
        //Is Infinity Representation when DecimalHalf==-2147483648 (IntValue==1 for positive infinity;IntValue==-1 for negative Infinity)
		//(other values only used if AltNum_EnableInfinityPowers is enabled)
		//If AltNum_EnableImaginaryInfinity is enabled and ExtraRep = IRep, then represents either negative or positive imaginary infinity
        static const signed int InfinityRep = -2147483648;
	#endif
	#if defined(AltNum_EnableApproachingValues)
        //Is Approaching Bottom when DecimalHalf==-2147483647:
        //If ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0__1)
		//If ExtraRep above 1 and 2147483645 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
		//If ExtraRep=PiRep, then it represents Approaching IntValue from right towards left (IntValue.0__1)Pi
        static const signed int ApproachingBottomRep = -2147483647;
		//Is Approaching Top i when DecimalHalf==-2147483646:
		//If ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)
		//If ExtraRep above 1 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
		//If ExtraRep=PiRep, then it represents Approaching IntValue+1 from left towards right (IntValue.9__9)Pi
		static const signed int ApproachingTopRep = -2147483646;
        #if defined(AltNum_EnableApproachingI)
		//Is Approaching Bottom i when DecimalHalf==-2147483645:
		//If ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0__1)i
        static const signed int ApproachingImaginaryBottomRep = -2147483645;
		//Is Approaching Top i when DecimalHalf==-2147483644:
		//If ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)i
		static const signed int ApproachingImaginaryTopRep = -2147483644;
        #endif
	#endif
	#if defined(AltNum_EnablePiRep)
        //Is Pi*Value representation when ExtraRep==-2147483648
        static const signed int PiRep = -2147483648;
		#ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483645, then represents (IntValue/DecimalHalf)*Pi
        static const signed int PiByDivisorRep = -2147483645;
		#endif
	#endif
	#if defined(AltNum_EnableImaginaryNum)
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483646, then represents Value*i 
        static const signed int IRep = -2147483647;
		#ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483644, then represents (IntValue/DecimalHalf)*i
        static const signed int IByDivisorRep = -2147483644;
		#endif
	#endif
	#if defined(AltNum_EnableERep)
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483646, then represents Value*e
        static const signed int ERep = -2147483646;
		#ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableImaginaryNum is enabled and ExtraRep== -2147483643, then represents (IntValue/DecimalHalf)*e
        static const signed int EByDivisorRep = -2147483643;
		#endif
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
        static const signed int AlternativeFractionalLowerBound = -2147483640;
		//Upper limit for Mixed Fractions; infinite approaching type representations at and after this DecimalHalf value
		static const signed int InfinityBasedLowerBound = -2147483644;
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

    #pragma endregion Const Representation values
        /// <summary>
        /// Enum representing value type stored
        /// </summary>
        enum class RepType: int
        {
            NormalType = 0,
	#if defined(AltNum_EnableFractionals)
            NumByDiv,
	#endif
	#if defined(AltNum_EnablePiRep)
            PiNum,
		#if defined(AltNum_EnablePiPowers)
            PiPower,
		#endif
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
            PiNumByDiv,//  (Value/(ExtraRep*-1))*Pi Representation
			#else
            PiFractional,//  IntValue/DecimalHalf*Pi Representation
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableERep)
            ENum,
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledEFractionals)
            ENumByDiv,//(Value/(ExtraRep*-1))*e Representation
			#else
            EFractional,//  IntValue/DecimalHalf*e Representation
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableImaginaryNum)
            INum,
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            INumByDiv,//(Value/(ExtraRep*-1))*i Representation
			#else
            IFractional,//  IntValue/DecimalHalf*i Representation
			#endif
		#endif
		#ifdef AltNum_EnableComplexNumbers
            ComplexIRep,
		#endif
	#endif
	#if defined(AltNum_EnableMixedFractional)
            MixedFrac,//IntValue +- (-DecimalHalf)/ExtraRep
		#if defined(AltNum_EnableMixedPiFractional)
            MixedPi,//IntValue +- (-DecimalHalf/-ExtraRep)
		#elif defined(AltNum_EnableMixedEFractional)
            MixedE,//IntValue +- (-DecimalHalf/-ExtraRep)
		#elif defined(AltNum_EnableMixedIFractional)
            MixedI,//IntValue +- (-DecimalHalf/-ExtraRep)
		#endif
	#endif

	#if defined(AltNum_EnableInfinityRep)
			PositiveInfinity,//If Positive Infinity, then convert number into MaximumValue instead when need as real number
			NegativeInfinity,//If Negative Infinity, then convert number into MinimumValue instead when need as real number
	#endif
	#if defined(AltNum_EnableApproachingValues)
            ApproachingBottom,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
		#if !defined(AltNum_DisableApproachingTop)
            ApproachingTop,//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
		#endif
		#if defined(AltNum_EnableApproachingDivided)
            ApproachingMidRight,//(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive, IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
			#if !defined(AltNum_DisableApproachingTop)
			ApproachingMidLeft,//(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive, IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
			#endif
		#endif
	#endif
    #if defined(AltNum_EnableNaN)
            Undefined,
            NaN,
    #endif
	#if defined(AltNum_EnableApproachingPi)
            ApproachingTopPi,//equal to IntValue.9..9 Pi
	#endif
	#if defined(AltNum_EnableApproachingE)
            ApproachingTopE,//equal to IntValue.9..9 e
	#endif
	#if defined(AltNum_EnableImaginaryInfinity)
            PositiveImaginaryInfinity,
			NegativeImaginaryInfinity,
	#endif
	#if defined(AltNum_EnableApproachingI)
            ApproachingImaginaryBottom,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
		#if !defined(AltNum_DisableApproachingTop)
            ApproachingImaginaryTop,//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
		#endif
		#if defined(AltNum_EnableApproachingDivided)
            ApproachingImaginaryMidRight,//(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive, IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
			#if !defined(AltNum_DisableApproachingTop)
			ApproachingImaginaryMidLeft,//(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive, IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
			#endif
		#endif
    #endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
            UndefinedButInRange,
		#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
			WithinMinMaxRange,
		#endif
	#endif
    #if defined(AltNum_EnableNilRep)
            Nil,
    #endif
            UnknownType
        };
		
		std::string RepTypeAsString(RepType& repType)
		{
			switch(repType)
			{
				case RepType::NormalType:
					return "NormalType"; break;
	#if defined(AltNum_EnableFractionals)
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
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
					return "PiNumByDiv"; break;
			#else
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					return "PiFractional"; break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::ENum:
					return "ENum"; break;
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
					return "ENumByDiv"; break;
			#else
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					return "EFractional"; break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
                    return "INum"; break;
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
					return "INumByDiv"; break;
			#else
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
					return "IFractional"; break;
			#endif
		#endif
		#ifdef AltNum_EnableComplexNumbers
				case RepType::ComplexIRep:
					return "ComplexIRep"; break;
		#endif
	#endif
	#if defined(AltNum_EnableMixedFractional)
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
				case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
					return "ApproachingMidRight"; break;
			#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
					return "ApproachingMidLeft"; break;
			#endif
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
				case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
					return "ApproachingImaginaryMidRight"; break;
			#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
					return "ApproachingImaginaryMidLeft"; break;
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

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        RepType GetRepType()
        {
		#if defined(AltNum_EnableInfinityRep)
            if(DecimalHalf==InfinityRep)
            {
			#if defined(AltNum_EnableImaginaryInfinity)
                if (ExtraRep == IRep)
				    return IntValue==1?RepType::PositiveImaginaryInfinity:RepType::NegativeImaginaryInfinity;
				else
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
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
		#if defined(AltNum_EnableApproachingValues)//old value = ApproachingValRep
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
                else if (ExtraRep == PiRep)
                    return RepType::ApproachingTopPi;
			#endif
			#if defined(AltNum_EnableApproachingE)
                else if (ExtraRep == ERep)
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
            if(ExtraRep==0)
			{
	#if defined(AltNum_EnableNaN)
				if(DecimalHalf==NaNRep)
					return RepType::NaN;
				else if(DecimalHalf==UndefinedRep)
					return RepType::Undefined;
	#endif
                return RepType::NormalType;
			}
			else if(IntValue==0&&DecimalHalf==0)
			{
				ExtraRep = 0;
				return RepType::NormalType;
			}
	#ifdef AltNum_EnablePiRep
            else if(ExtraRep==PiRep)
                return RepType::PiNum;
		#if defined(AltNum_EnablePiFractional)
            else if(ExtraRep==PiByDivisorRep)
				return RepType::PiIntNumByDiv;
		#endif
	#endif
            else if(ExtraRep>0)
			{
	#if defined(AltNum_EnableMixedFractional)
				if(DecimalHalf<0)
					return RepType::MixedFrac;
    #endif
	#if defined(AltNum_EnableFractionals)
                return RepType::NumByDiv;
    #endif
				throw "Non-enabled representation detected";
			}
    #if defined(AltNum_EnableERep)
            else if(ExtraRep==ERep)
			{
				return RepType::ENum;
			}
		#if defined(AltNum_EnableEFractional)
            else if(ExtraRep==EByDivisorRep)//(IntValue/DecimalHalf)*e
				return RepType::ENumByDiv;
		#endif
    #endif

	#if defined(AltNum_EnableImaginaryNum)
            else if(ExtraRep==IRep)
			{
				return RepType::INum;
			}
		#if defined(AltNum_EnableIFractional)
            else if(ExtraRep==IByDivisorRep)
					return RepType::INumByDiv;
		#endif
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
           else if(ExtraRep==UndefinedButInRange)
                return RepType::UndefinedButInRange;//If DecimalHalf equals InfinityRep, than equals undefined value with range between negative infinity and positive infinity (negative range values indicates inverted range--any but the range of values)
		#if defined(AltNum_EnableWithinMinMaxRange)
			//If IntValue==NegativeRep, then left side range value equals negative infinity
			//If DecimalHalf==InfinityRep, then right side range value equals positive infinity
           else if(ExtraRep==WithinMinMaxRangeRep)
                return RepType::WithinMinMaxRange;
		#endif
	#endif
            else if(ExtraRep<0)
	#if defined(AltNum_EnableAlternativeMixedFrac)
				if(DecimalHalf<0)
		#if defined(AltNum_EnableMixedPiFractional)
					return RepType::MixedPi;
		#elif defined(AltNum_EnableMixedEFractional)
					return RepType::MixedE;
		#else
					return RepType::MixedI;
		#endif
				else
	#endif
	#if defined(AltNum_EnableDecimaledPiFractionals)
					return RepType::PiNumByDiv;
	#elif defined(AltNum_EnableDecimaledEFractionals)
					return RepType::ENumByDiv;
	#elif defined(AltNum_EnableDecimaledIFractionals)
					return RepType::INumByDiv;
	#else
					throw "Non-enabled Negative ExtraRep representation type detected";
	#endif
            else
				throw "Unknown or non-enabled representation type detected";
            return RepType::UnknownType;//Catch-All Value;
        }

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        {
            IntValue = 2147483647; DecimalHalf = 999999999; ExtraRep = 0;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        {
            IntValue = -2147483647; DecimalHalf = 999999999; ExtraRep = 0;
        }
		
	#pragma region PiNum Setters
    #if defined(AltNum_EnablePiRep)
        #if defined(AltNum_EnableAltDecBasedSetValues)
        void SetPiValFromAltDec(AltDec Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
        }
        #endif

        void SetPiVal(AltDec Value)
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
        void SetEValFromMediumDec(MediumDec Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
        }
        #endif

        void SetEVal(AltDec Value)
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
        void SetFractionalVal(AltDec Value, int Divisor)
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
            IntValue = Value.IntValue;
            DecimalHalf = Numerator*-1;
            ExtraRep = Denom;
        }
        
        void SetMixedFractionalValAsNeg(int WholeNum, int NumeratorAsNeg, int Denom)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Numerator;
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
        static AltDec NaNValue()
        {
            AltDec NewSelf = AltDec(0, NaNRep);
            return NewSelf;
        }
		
        static AltDec UndefinedValue()
        {
            AltDec NewSelf = AltDec(0, UndefinedRep);
            return NewSelf;
        }
	#endif

	#if defined(AltNum_EnableInfinityRep)
        static AltDec InfinityValue()
        {
            AltDec NewSelf = AltDec(1, InfinityRep);
            return NewSelf;
        }
        
        static AltDec NegativeInfinityValue()
        {
            AltDec NewSelf = AltDec(-1, InfinityRep);
            return NewSelf;
        }
	#endif
        
	#if defined(AltNum_EnableApproachingValues)
        static AltDec ApproachingZeroValue()
        {
            AltDec NewSelf = AltDec(0, ApproachingBottomRep);
            return NewSelf;
        }
		
        static AltDec ApproachingOneFromLeftValue()
        {
            AltDec NewSelf = AltDec(0, ApproachingTopRep);
            return NewSelf;
        }
		
        static AltDec ApproachingOneFromRightValue()
        {
            AltDec NewSelf = AltDec(1, ApproachingBottomRep);
            return NewSelf;
        }

        static AltDec NegativeApproachingZeroValue()
        {
            AltDec NewSelf = AltDec(NegativeRep, ApproachingBottomRep);
            return NewSelf;
        }
	#endif
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec PiNumValue()
        {
            return AltDec(3, 141592654);
        }
        
        static AltDec PiValue()
        {
#if defined(AltNum_EnableERep)
            return AltDec(1, PiRep);
#else
            return AltDec(3, 141592654);
#endif
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static AltDec HundredMilPiNumVal()
        {
            return AltDec(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static AltDec TenMilPiNumVal()
        {
            return AltDec(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static AltDec OneMilPiNumVal()
        {
            return AltDec(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static AltDec TenPiNumVal()
        {
            return AltDec(31, 415926536);
        }
        
        static AltDec ENumValue()
        {
            return AltDec(2, 718281828);
        }

        static AltDec EValue()
        {
#if defined(AltNum_EnableERep)
            return AltDec(1, ERep);
#else
            return AltDec(2, 718281828);
#endif
        }
        
        static AltDec ZeroValue()
        {
            return AltDec();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec OneValue()
        {
            AltDec NewSelf = AltDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec TwoValue()
        {
            AltDec NewSelf = AltDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec NegativeOneValue()
        {
            AltDec NewSelf = AltDec(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec Point5Value()
        {
            AltDec NewSelf = AltDec(0, 500000000);
            return NewSelf;
        }

        static AltDec JustAboveZeroValue()
        {
            AltDec NewSelf = AltDec(0, 1);
            return NewSelf;
        }

        static AltDec OneMillionthValue()
        {
            AltDec NewSelf = AltDec(0, 1000);
            return NewSelf;
        }

        static AltDec FiveThousandthValue()
        {
            AltDec NewSelf = AltDec(0, 5000000);
            return NewSelf;
        }

        static AltDec FiveMillionthValue()
        {
            AltDec NewSelf = AltDec(0, 5000);
            return NewSelf;
        }

        static AltDec TenMillionthValue()
        {
            AltDec NewSelf = AltDec(0, 100);
            return NewSelf;
        }

        static AltDec OneHundredMillionthValue()
        {
            AltDec NewSelf = AltDec(0, 10);
            return NewSelf;
        }

        static AltDec FiveBillionthValue()
        {
            AltDec NewSelf = AltDec(0, 5);
            return NewSelf;
        }

        static AltDec LN10Value()
        {
            return AltDec(2, 302585093);
        }

        static AltDec LN10MultValue()
        {
            return AltDec(0, 434294482);
        }

        static AltDec HalfLN10MultValue()
        {
            return AltDec(0, 868588964);
        }
        
    #if defined(AltNum_EnableNilRep)
        static AltDec NilValue()
        {
            return AltDec(NilRep, NilRep);
        }
    #endif

        static AltDec MinimumValue()
        {
            return AltDec(2147483647, 999999999);
        }

        static AltDec MaximumValue()
        {
            return AltDec(2147483647, 999999999);
        }
public:
        static AltDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec ENum;
        
#if defined(AltNum_EnableInfinityRep)
        static AltDec Infinity;
        static AltDec NegativeInfinity;
        static AltDec ApproachingZero;
#endif

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec E;
        
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec OneGMillionth;

        //0e-7
        static AltDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static AltDec FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDec</returns>
        static AltDec NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static AltDec Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static AltDec Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static AltDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static AltDec LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static AltDec HalfLN10Mult;

    #if defined(AltNum_EnableNilRep)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static AltDec Nil;
    #endif
	
#if defined(AltNum_EnableApproachingValues)
        static AltDec ApproachingRightRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 999999999);
        }

        static AltDec ApproachingLeftRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 1);
        }

        static AltDec LeftAlmostPointFiveRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 499999999);
        }

        static AltDec RightAlmostPointFiveRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 500000001);
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
        /// <returns>AltDec</returns>
        AltDec GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        AltDec(const char* strVal)
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
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDec(std::string Value)
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

private:
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
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDec(float Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDec(double Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDec(ldouble Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        AltDec(bool Value)
        {
            this->SetVal(Value);
        }

    #if defined(AltNum_EnableMediumDecBasedSetValues)
        AltDec(MediumDec Value)
        {
            this->SetVal(Value);
        }
    #endif
    #pragma endregion From Standard types to this type

    #pragma region MirroredIntBased Operations
        template<typename IntType>
        void IntHalfDivision(IntType RValue)
        {
#if defined(AltDec_UseMirroredInt)
    #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
        #if defined(BlazesMirroredInt_UseLegacyIntOperations)
            IntValue.Value /= RValue;
        #else
            IntValue.DivideByOp(RValue);
        #endif
    #elif defined(BlazesMirroredInt_UsePseudoBitSet)
            if(RValue<0)
            {
                if(IntValue.Value>=NegativeRepVal)//Currently Negative
                {
                    IntValue.Value -= NegativeRepVal;
                    RValue -= NegativeRepVal;
                    IntValue.Value /= RValue; 
                }
                else
                {

                } 
            }
            else
            {
                if(IntValue.Value>=NegativeRepVal)//Currently Negative
                {
                }
                else
                {
                    IntValue.Value /= RValue;
                } 
            }
    #else
            IntValue.DivideByOp(RValue);
    #endif
#else
            IntValue /= RValue;
#endif
        }

        template<typename IntType>
        void IntHalfMultiplication(IntType RValue)
        {
#if defined(AltDec_UseMirroredInt)
    #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
        #if defined(BlazesMirroredInt_UseLegacyIntOperations)
            IntValue.Value /= RValue;
        #else
            IntValue.MultipleByOp(RValue);
        #endif
    #elif defined(BlazesMirroredInt_UsePseudoBitSet)
        #if defined(BlazesMirroredInt_UseBitwiseForIntOp)
            IntValue.MultipleByOp(RValue);
        #else
            if(RValue<0)
            {
                if(IntValue.Value>=NegativeRepVal)//Currently Negative
                {
                    IntValue.Value -= NegativeRepVal;
                    RValue -= NegativeRepVal;
                    IntValue.Value *= RValue;
                }
                else
                {

                } 
            }
            else
            {
                if(IntValue.Value>=NegativeRepVal)//Currently Negative
                {
                }
                else
                {
                    IntValue.Value *= RValue;
                } 
            }
        #endif
    #else
            IntValue.MultipleByOp(RValue);
    #endif
#else
            IntValue *= RValue;
#endif
        }

        //Replace usage of IntValue += RValue; with IntHalfAddition(RValue); or IntHalfAdditionOp(RValue);
        template<typename IntType>
        void IntHalfAdditionOp(IntType& RValue)
        {
#if defined(AltDec_UseMirroredInt)
    #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
            IntValue += RValue;
    #elif defined(BlazesMirroredInt_UsePseudoBitSet)
        #if defined(BlazesMirroredInt_UseBitwiseForIntOp)
        #else
        #endif
    #endif
#else//Can be used without modifying RValue
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
                    IntValue = RValue - 1;
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
                    IntValue += RValue - 1;
                else
                {
                    IntValue += RValue;
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
                    IntValue = RValue + 1;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
                else
                    IntValue += RValue;
            }
#endif
        }

        //Replace usage of IntValue += RValue; with IntHalfAddition(RValue);
        template<typename IntType>
        void IntHalfAddition(IntType RValue)
        {
            IntHalfAdditionOp(RValue);
        }

        //Replace usage of IntValue -= RValue; with IntHalfSubtraction(RValue); or IntHalfSubtractionOp(RValue);
        template<typename IntType>
        void IntHalfSubtractionOp(IntType& RValue)
        {
#if defined(AltDec_UseMirroredInt)
    #if defined(BlazesMirroredInt_UseLegacyValueBehavior)
            IntValue -= RValue;
    #endif
#else//Can be used without modifying RValue
            if (RValue==0)
                return;
            if (IntValue == 0)
                IntValue = -RValue;
            else if (IntValue == NegativeRep)
            {
                //-0.XXXX - -2 = 1.XXXX
                //-0.XXXX - -1 = 0.XXXX (Flips to other side)
                //-0.XXXX - 1 = -1.XXXX 
                //-0.XXXX - 6 = -6.XXXX
                if (RValue < 0)
                    IntValue = -RValue - 1;
                else
                {
                    IntValue = -RValue;
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
                    IntValue -= RValue - 1;
                else//(RValue>=LValue.Value)
                {
                    IntValue -= RValue;
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
                    IntValue -= RValue - 1;
                    #if !defined(BlazesMirroredInt_PreventNZeroUnderflowCheck)
                    if (IntValue == NegativeRep)
                        throw "MirroredInt value has underflowed";
                    #endif
                }
                else
                    IntValue += RValue;
            }
#endif
        }

        //Replace usage of IntValue -= RValue; with IntHalfSubtraction(RValue);
        template<typename IntType>
        void IntHalfSubtraction(IntType RValue)
        {
            IntHalfSubtractionOp(RValue);
        }

		bool IntHalfEqualToOp(AltDec& RValue)
		{
            return IntValue == RValue.IntValue;
        }

		bool IntHalfNotEqualToOp(AltDec& RValue)
		{
            return IntValue != RValue.IntValue;
        }

		bool IntHalfEqualTo(AltDec RValue)
		{
            return IntValue == RValue.IntValue;
        }

		bool IntHalfNotEqualTo(AltDec RValue)
		{
            return IntValue != RValue.IntValue;
        }

		bool IntHalfEqualToIntOp(int& RValue)
		{
            return IntValue == RValue;
        }

		bool IntHalfNotEqualToIntOp(int& RValue)
		{
            return IntValue != RValue;
        }

		bool IntHalfEqualToInt(int RValue)
		{
            return IntValue == RValue;
        }

		bool IntHalfNotEqualToInt(int RValue)
		{
            return IntValue != RValue;
        }

        /// <summary>
        /// Less than Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
		bool IntHalfLessThanOpV2(AltDec& RValue)
		{
#if defined(AltDec_UseMirroredInt)
            if(IntValue<RValue)
                return true;
            else
                return false;
#else
            if (IntValue == NegativeRep)
                return RValue.IntValue < 0 && RValue.IntValue != NegativeRep;
            else if (RValue.IntValue == NegativeRep)
            {
                if (IntValue < 0)
                    return true;
                else
                    return false;
            }
            else
                return IntValue<RValue.IntValue;
#endif
		}
		
		static bool IntHalfLessThanOp(AltDec& LValue, AltDec& RValue)
		{
			return LValue.IntHalfLessThanOpV2(RValue);
		}
		
		static bool IntHalfLessThan(AltDec LValue, AltDec RValue)
		{
			return IntHalfLessThanOp(LValue, RValue);
		}
		
        /// <summary>
        /// Less than or Equal Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
		bool IntHalfLessThanOrEqualOpV2(AltDec& RValue)
		{
#if defined(AltDec_UseMirroredInt)
            if(IntValue<=RValue)
                return true;
            else
                return false;
#else
            if (IntValue == NegativeRep)
                return RValue.IntValue < 0;
            else if (RValue.IntValue == NegativeRep)
            {
                if (IntValue >= 0)
                    return false;
                else
                    return true;
            }
            else
                return IntValue <= RValue.IntValue;
#endif
		}
		
        /// <summary>
        /// Less than or Equal Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
		static bool IntHalfLessThanOrEqualOp(AltDec& LValue, AltDec& RValue)
		{
			return LValue.IntHalfLessThanOrEqualOpV2(RValue);
		}
		
		static bool IntHalfLessThanOrEqual(AltDec LValue, AltDec RValue)
		{
			return IntHalfLessThanOrEqualOp(LValue, RValue);
		}
		
        /// <summary>
        /// Greater than Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
		bool IntHalfGreaterThanOpV2(AltDec& RValue)
		{
#if defined(AltDec_UseMirroredInt)
            if(IntValue>RValue)
                return true;
            else
                return false;
#else
            if (IntValue == NegativeRep)
            {
                if (RValue.IntValue < 0)
                    return false;
                else
                    return true;
            }
            else if (RValue.IntValue == NegativeRep)
            {
                if (IntValue >= 0)
                    return true;
                else
                    return false;
            }
            else
                return IntValue > RValue.IntValue;
#endif
		}
		
        /// <summary>
        /// Greater than Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
		static bool IntHalfGreaterThanOp(AltDec& LValue, AltDec& RValue)
		{
			return LValue.IntHalfLessThanOrEqualOpV2(RValue);
		}
		
		static bool IntHalfGreaterThan(AltDec LValue, AltDec RValue)
		{
			return IntHalfGreaterThanOp(LValue, RValue);
		}
		
        /// <summary>
        /// Greater than or Equal to Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
		bool IntHalfGreaterThanOrEqualOpV2(AltDec& RValue)
		{
#if defined(AltDec_UseMirroredInt)
            if(IntValue>=RValue)
                return true;
            else
                return false;
#else
            if (IntValue == RValue.IntValue)
                return true;
            if (IntValue == NegativeRep)
            {
                if (RValue.IntValue < 0)
                    return false;
                else
                    return true;
            }
            else if (RValue.IntValue == NegativeRep)
            {
                if (IntValue >= 0)
                    return true;
                else
                    return false;
            }
            else
                return IntValue >= RValue.IntValue;
#endif
		}
		
        /// <summary>
        /// Greater than or Equal to Operation for just IntValue section
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
		static bool IntHalfGreaterThanOrEqualOp(AltDec& LValue, AltDec& RValue)
		{
			return LValue.IntHalfGreaterThanOrEqualOpV2(RValue);
		}
		
		static bool IntHalfGreaterThanOrEqual(AltDec LValue, AltDec RValue)
		{
			return IntHalfGreaterThanOrEqualOp(LValue, RValue);
		}

    #pragma endregion MirroredIntBased Operations

    #pragma region From this type to Standard types
        /// <summary>
        /// AltDec to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float()
        {
            float Value;
            if (IntValue < 0)
            {
    #if !defined(AltDec_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
    #if !defined(AltDec_UseMirroredInt)
                Value = (float)IntValue;
    #else
                Value = (float)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
        }

        /// <summary>
        /// AltDec to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double()
        {
            double Value;
            if (IntValue < 0)
            {
    #if !defined(AltDec_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0 : (double)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0 : (double)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
    #if !defined(AltDec_UseMirroredInt)
                Value = (double)IntValue;
    #else
                Value = (double)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
        }

        /// <summary>
        /// AltDec to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble()
        {
            ldouble Value;
            if (IntValue < 0)
            {
    #if !defined(AltDec_UseMirroredInt)
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)IntValue;
    #else
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
    #if !defined(AltDec_UseMirroredInt)
                Value = (ldouble)IntValue;
    #else
                Value = (ldouble)IntValue.GetValue();
    #endif
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
            }
            return Value;
        }
		
        /// <summary>
        /// AltDec to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return GetIntHalf(); }

        explicit operator bool() { return IntValue == 0 ? false : true; }
    #pragma endregion From this type to Standard types

    #pragma region Pi Conversion
	#if defined(AltNum_EnablePiRep)
		//3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844 *selfNum
        void ConvertPiToNum();
	
		#if defined(AltNum_EnableDecimaledPiFractionals)
        void ConvertPiByDivToNumByDiv();
		
        void ConvertFromPiByDivToNorm();
		#else
		void ConvertFromPiFractionalToNorm();
		
		#endif
		
        void ConvertPiPowerToNum();
		
        AltDec PiPowerNum(int powerExponent);
		
        void ConvertPiPowerToPiRep();
		
		void ConvertToPiRep(RepType& repType);
	#endif
    #pragma endregion Pi Conversion
	
    #pragma region E Conversion
	#if defined(AltNum_EnableERep)
		//2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69995 * selfNum
        void ConvertENumToNum();
		
		#if defined(AltNum_EnableDecimaledPiFractionals)
		void ConvertEByDivToNumByDiv();
		
		void ConvertEByDivToNorm();
		#else
		void ConvertFromEFractionalToNorm();
		#endif
        void ConvertToERep(RepType& repType);
	#endif
    #pragma endregion E Conversion
	
    #pragma region Other RepType Conversion
        //Switch based version of ConvertToNormType(use ConvertAsNormType instead to return converted value without modifying base value)
        void ConvertToNormType(RepType& repType);

		//Switch based return of value as normal type representation
		AltDec ConvertAsNormType(RepType& repType);

		//Converts value to normal type representation
        void ConvertToNormTypeV2();
		
		//Returns value as normal type representation
		AltDec ConvertAsNormTypeV2();
    #if defined(AltNum_EnableImaginaryNum)
		void ConvertIRepToNormal(RepType& repType);
		
		void ConvertToNormalIRep(RepType& repType);
		
		AltDec ConvertAsNormalIRep(RepType& repType);
		
		void ConvertToNormalIRep();
		
		AltDec ConvertAsNormalIRep();
    #endif
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators
        /// <summary>
        /// Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator==(AltDec LValue, AltDec RValue)
        {
            RepType LRep = LValue.GetRepType();
            RepType RRep = RValue.GetRepType();
            if (LRep != RRep)
            {//ToDo:Check bitvalue of RepType instead maybe
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                    ConvertToNormalIRep(LRep);
                    break;
                    //Don't convert infinity into real number
#if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                    //Don't convert infinity into real number
#if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    RValue.ConvertToNormType(RRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
            }
    #if !defined(AltDec_UseMirroredInt)
            return (LValue.IntValue == RValue.IntValue && LValue.DecimalHalf == RValue.DecimalHalf && LValue.ExtraRep == RValue.ExtraRep);
    #else
            return (LValue.IntValue.Value == RValue.IntValue.Value && LValue.DecimalHalf == RValue.DecimalHalf && LValue.ExtraRep == RValue.ExtraRep);
    #endif
        }

        /// <summary>
        /// Not equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator!=(AltDec LValue, AltDec RValue)
        {
            RepType LRep = LValue.GetRepType();
            RepType RRep = RValue.GetRepType();
            if (LRep != RRep)
            {//ToDo:Check bitvalue of RepType instead maybe
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                    //Don't convert infinity into real number
#if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    RValue.ConvertToNormType(RRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
            }
            return (LValue.IntValue != RValue.IntValue || LValue.DecimalHalf != RValue.DecimalHalf);
        }

        /// <summary>
        /// Lesser than Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<(AltDec LValue, AltDec RValue)
        {
            RepType LRep = LValue.GetRepType();
            RepType RRep = RValue.GetRepType();
            if (LRep != RRep)
            {//ToDo:Check bitvalue of RepType instead maybe
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                    //Don't convert infinity into real number
#if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    RValue.ConvertToNormType(RRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
            }
#if defined(AltNum_EnableImaginaryNum)
            if (LValue.ExtraRep != RValue.ExtraRep)
                throw "Can't compare imaginary number with real number";
#endif
#if defined(AltNum_EnableInfinityRep)
            if (LValue.DecimalHalf == InfinityRep)
            {
#if defined(AltNum_EnableImaginaryInfinity)
                if (LValue.ExtraRep == IRep)//LeftSide is Imaginary infinity
                {
                    if (RValue.DecimalHalf == InfinityRep)//both left and right are infinity types
                    {
                        if (RValue.IntValue == 1 && LValue.IntValue == -1)
                            return true;
                        else
                            return false;
                    }
                    else if (LValue.IntValue == 1)//Left is Positive Imaginary Infinity
                        return false;
                    else//Left Negative Imaginary Infinity
                        return true;
                }
#endif
                if (RValue.DecimalHalf == InfinityRep)//both left and right are infinity types
                {
                    //return LValue.IntValue<RValue.IntValue;
                    /*
                        +inf < +inf = false
                        +inf < -inf = false
                        -inf < +inf = true
                        -inf < -inf = false
                    */
                    if (RValue.IntValue == 1 && LValue.IntValue == -1)
                        return true;
                    else
                        return false;
                }
                else if (LValue.IntValue == 1)//Left is Positive Infinity
                    return false;//+inf < 99
                else//Left is Negative Infinity
                    return true;//-inf < 99
            }
            else if (RValue.DecimalHalf == InfinityRep)//Right side is infinity
            {
#if defined(AltNum_EnableImaginaryInfinity)
                if (RValue.ExtraRep == IRep)//RightSide is Imaginary infinity
                {
                    if (LValue.IntValue == 1)//Right is Positive Imaginary Infinity
                        return true;
                    else//Right is Negative Imaginary Infinity
                        return false;
                }
#endif
                if (RValue.IntValue == 1)//Right is Positive Infinity
                    return true;//99 < +inf
                else//Right is Negative Infinity
                    return false;//99 < -inf
            }
#endif
            if (LValue.DecimalHalf == 0)
            {
                if (RValue.DecimalHalf == 0)
                    return LValue.IntValue < RValue.IntValue;
                else
                {
                    if (IntHalfLessThanOp(LValue, RValue))
                        return LValue.DecimalHalf < RValue.DecimalHalf;
                    else
                        return false;
                }
            }
            else if (IntHalfLessThanOp(LValue, RValue))
                return LValue.DecimalHalf < RValue.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Lesser than or Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<=(AltDec LValue, AltDec RValue)
        {
            RepType LRep = LValue.GetRepType();
            RepType RRep = RValue.GetRepType();
            if (LRep != RRep)
            {//ToDo:Check bitvalue of RepType instead maybe
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                case RepType::NegativeImaginaryInfinity:
                    break;
        #endif
    #endif
    #if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
    #endif
                default:
#endif
                    LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                    //Don't convert infinity into real number
#if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    RValue.ConvertToNormType(RRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
            }
#if defined(AltNum_EnableImaginaryNum)
            if (LValue.ExtraRep != RValue.ExtraRep)
                throw "Can't compare imaginary number with real number";
#endif
#if defined(AltNum_EnableInfinityRep)
            if (LValue.DecimalHalf == InfinityRep)
            {
#if defined(AltNum_EnableImaginaryInfinity)
                if (LValue.ExtraRep == IRep)//LeftSide is Imaginary infinity
                {
                    if (RValue.DecimalHalf == InfinityRep)//both left and right are infinity types
                    {
                        if (RValue.IntValue == 1 && LValue.IntValue == -1)
                            return false;
                        else
                            return true;
                    }
                    else if (LValue.IntValue == 1)//Left is Positive Imaginary Infinity
                        return false;
                    else//Left Negative Imaginary Infinity
                        return true;
                }
#endif
                if (RValue.DecimalHalf == InfinityRep)//both left and right are infinity types
                {
                    //return LValue.IntValue<=RValue.IntValue;
                    /*
                        (+inf <= +inf) = true
                        (+inf <= -inf) = false
                        (-inf <= +inf) = true
                        (-inf <= -inf) = true
                    */
                    if (RValue.IntValue == -1 && LValue.IntValue == 1)
                        return false;
                    else
                        return true;
                }
                else if (LValue.IntValue == 1)//Left is Positive Infinity
                    return false;//+inf <= 99
                else//Left is Negative Infinity
                    return true;//-inf <= 99
            }
            else if (RValue.DecimalHalf == InfinityRep)//Right side is infinity
            {
#if defined(AltNum_EnableImaginaryInfinity)
                if (RValue.ExtraRep == IRep)//RightSide is Imaginary infinity
                {
                    if (LValue.IntValue == 1)//Right is Positive Imaginary Infinity
                        return true;
                    else//Right is Negative Imaginary Infinity
                        return false;
                }
#endif
                if (RValue.IntValue == 1)//Right is Positive Infinity
                    return true;//99 <= +inf
                else//Right is Negative Infinity
                    return false;//99 <= -inf
            }
#endif
            if (LValue.DecimalHalf == 0)
            {
                if (RValue.DecimalHalf == 0)
                    return LValue.IntValue <= RValue.IntValue;
                else
                {
                    if (IntHalfLessThanOrEqualOp(LValue, RValue))
                        return LValue.DecimalHalf <= RValue.DecimalHalf;
                    else
                        return false;
                }
            }
            else if (IntHalfLessThanOrEqualOp(LValue, RValue))
                return LValue.DecimalHalf <= RValue.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Greater than Operation
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="RValue">The right side value.</param>
        /// <returns>bool</returns>
        friend bool operator>(AltDec LValue, AltDec RValue)
        {
            RepType LRep = LValue.GetRepType();
            RepType RRep = RValue.GetRepType();
            if (LRep != RRep)
            {//ToDo:Check bitvalue of RepType instead maybe
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                    //Don't convert infinity into real number
#if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    RValue.ConvertToNormType(RRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
            }
#if defined(AltNum_EnableImaginaryNum)
            if (LValue.ExtraRep != RValue.ExtraRep)
                throw "Can't compare imaginary number with real number";
#endif
#if defined(AltNum_EnableInfinityRep)
            if (LValue.DecimalHalf == InfinityRep)
            {
#if defined(AltNum_EnableImaginaryInfinity)
                if (LValue.ExtraRep == IRep)//LeftSide is Imaginary infinity
                {
                    if (RValue.DecimalHalf == InfinityRep)//both left and right are infinity types
                    {
                        if (RValue.IntValue == -1 && LValue.IntValue == 1)
                            return true;
                        else
                            return false;
                    }
                    else if (LValue.IntValue == 1)//Left is Positive Imaginary Infinity
                        return true;
                    else//Left Negative Imaginary Infinity
                        return false;
                }
#endif
                if (RValue.DecimalHalf == InfinityRep)//both left and right are infinity types
                {
                    //return LValue.IntValue>RValue.IntValue;
                    /*
                        +inf > +inf = false
                        +inf > -inf = true
                        -inf > +inf = false
                        -inf > -inf = false
                    */
                    if (RValue.IntValue == -1 && LValue.IntValue == 1)
                        return true;
                    else
                        return false;
                }
                else if (LValue.IntValue == 1)//Left is Positive Infinity
                    return true;//+inf > 99
                else//Left is Negative Infinity
                    return false;//-inf > 99
            }
            else if (RValue.DecimalHalf == InfinityRep)//Right side is infinity
            {
#if defined(AltNum_EnableImaginaryInfinity)
                if (RValue.ExtraRep == IRep)//RightSide is Imaginary infinity
                {
                    if (LValue.IntValue == 1)//Right is Positive Imaginary Infinity
                        return false;
                    else//Right is Negative Imaginary Infinity
                        return true;
                }
#endif
                if (RValue.IntValue == 1)//Right is Positive Infinity
                    return false;//99 > +inf
                else//Right is Negative Infinity
                    return true;//99 > -inf
            }
#endif
            if (LValue.DecimalHalf == 0)
            {
                if (RValue.DecimalHalf == 0)
                    return LValue.IntValue > RValue.IntValue;
                else
                {
                    if (IntHalfGreaterThanOp(LValue, RValue))
                        return LValue.DecimalHalf > RValue.DecimalHalf;
                    else
                        return false;
                }
            }
            else if (IntHalfGreaterThanOp(LValue, RValue))
                return LValue.DecimalHalf > RValue.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Greater than or Equal to Operation
        /// </summary>
        /// <param name="LValue">The left side value</param>
        /// <param name="RValue">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>=(AltDec LValue, AltDec RValue)
        {
            RepType LRep = LValue.GetRepType();
            RepType RRep = RValue.GetRepType();
            if (LRep != RRep)
            {//ToDo:Check bitvalue of RepType instead maybe
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    LValue.ConvertToNormType(LRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
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
                    //Don't convert infinity into real number
#if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    break;
#endif
#endif
#if defined(AltNum_EnableInfinityRep)
                    //Don't convert infinity into real number
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    break;
#endif
                default:
#endif
                    RValue.ConvertToNormType(RRep);
#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
                    break;
                }
#endif
            }
#if defined(AltNum_EnableImaginaryNum)
            if (LValue.ExtraRep != RValue.ExtraRep)
                throw "Can't compare imaginary number with real number";
#endif
#if defined(AltNum_EnableInfinityRep)
            if (LValue.DecimalHalf == InfinityRep)
            {
#if defined(AltNum_EnableImaginaryInfinity)
                if (LValue.ExtraRep == IRep)//LeftSide is Imaginary infinity
                {
                    if (RValue.DecimalHalf == InfinityRep)//both left and right are infinity types
                    {
                        if (RValue.IntValue == 1 && LValue.IntValue == -1)
                            return false;
                        else
                            return true;
                    }
                    else if (LValue.IntValue == 1)//Left is Positive Imaginary Infinity
                        return true;
                    else//Left Negative Imaginary Infinity
                        return false;
                }
#endif
                if (RValue.DecimalHalf == InfinityRep)//both left and right are infinity types
                {
                    //return LValue.IntValue>=RValue.IntValue;
                    /*
                        (+inf >= +inf) = true
                        (+inf >= -inf) = true
                        (-inf >= +inf) = false
                        (-inf >= -inf) = true
                    */
                    if (RValue.IntValue == 1 && LValue.IntValue == -1)
                        return false;
                    else
                        return true;
                }
                else if (LValue.IntValue == 1)//Left is Positive Infinity
                    return true;//+inf >= 99 
                else//Left is Negative Infinity
                    return false;//-inf >= 99
            }
            else if (RValue.DecimalHalf == InfinityRep)//Right side is infinity
            {
#if defined(AltNum_EnableImaginaryInfinity)
                if (RValue.ExtraRep == IRep)//RightSide is Imaginary infinity
                {
                    if (LValue.IntValue == 1)//Right is Positive Imaginary Infinity
                        return false;
                    else//Right is Negative Imaginary Infinity
                        return true;
                }
#endif
                if (RValue.IntValue == 1)//Right is Positive Infinity
                    return false;//99 >= +inf
                else//Right is Negative Infinity
                    return true;//99 >= -inf
            }
#endif
            if (LValue.DecimalHalf == 0)
            {
                if (RValue.DecimalHalf == 0)
                    return LValue.IntValue >= RValue.IntValue;
                else
                {
                    if (IntHalfGreaterThanOrEqualOp(LValue, RValue))
                        return LValue.DecimalHalf >= RValue.DecimalHalf;
                    else
                        return false;
                }
            }
            else if (IntHalfGreaterThanOrEqualOp(LValue, RValue))
                return LValue.DecimalHalf >= RValue.DecimalHalf;
            else
                return false;
        }
		
    #pragma endregion Comparison Operators

    #pragma region AltDec-To-Int Comparison Functions
        /// <summary>
        /// Equality operation between <see cref="AltDec&"/> and Integer Type.
        /// </summary>
        /// <param name="LValue">Left side AltDec value</param>
        /// <param name="RValue">Right side integer value</param>
        /// <returns>bool</returns>
        template<typename IntType>
        static bool RightSideIntEqualTo(AltDec& LValue, IntType& RValue)
        {
#if defined(AltNum_EnableImaginaryNum)
    #if defined(AltNum_EnableDecimaledIFractionals)
            if (LValue.ExtraRep<0)
    #else
            if (LValue.ExtraRep == IRep)
    #endif
                return false;
    #if defined(AltNum_EnableIFractional)
            else if(LValue.ExtraRep==IByDivisorRep)
                return false;
    #endif
#endif
            if (LValue.ExtraRep != 0 || LValue.DecimalHalf < 0)
                LValue.ConvertToNormTypeV2();
            return (LValue.IntValue == RValue && LValue.DecimalHalf == 0 && LValue.ExtraRep == 0);
		}
		
        /// <summary>
        /// Not Equal to operation between <see cref="AltDec&"/> and Integer Type.
        /// </summary>
        /// <param name="LValue">Left side AltDec value</param>
        /// <param name="RValue">Right side integer value</param>
        /// <returns>bool</returns>
	    template<typename IntType>
        static bool RightSideIntNotEqualTo(AltDec& LValue, IntType& RValue)
        {
#if defined(AltNum_EnableImaginaryNum)
    #if defined(AltNum_EnableDecimaledIFractionals)
            if (LValue.ExtraRep<0)
    #else
            if (LValue.ExtraRep == IRep)
    #endif
                return false;
    #if defined(AltNum_EnableIFractional)
            else if(LValue.ExtraRep==IByDivisorRep)
                return false;
    #endif
#endif
            if (LValue.ExtraRep != 0 || LValue.DecimalHalf < 0)
                LValue.ConvertToNormTypeV2();
            if (LValue.IntValue == RValue)
                return false;
            else
                return true;
		}
		
        /// <summary>
        /// Less than operation between <see cref="AltDec"/> and Integer Type.
        /// </summary>
        /// <param name="LValue">Left side AltDec value</param>
        /// <param name="RValue">Right side integer value</param>
        /// <returns>bool</returns>
		template<typename IntType>
        static bool RightSideIntLessThan(AltDec& LValue, IntType& RValue)
        {
#if defined(AltNum_EnableInfinityRep)
            if (LValue.ExtraRep == InfinityRep)
            {
                if (LValue.IntValue == -1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
    #if defined(AltNum_EnableDecimaledIFractionals)
            if (LValue.ExtraRep<0)
    #else
            if (LValue.ExtraRep == IRep)
    #endif
                throw "Can't compare real values against imaginary.";
    #if defined(AltNum_EnableIFractional)
            else if(LValue.ExtraRep==IByDivisorRep)
                throw "Can't compare real values against imaginary.";
    #endif
#endif
            if (LValue.ExtraRep != 0 || LValue.DecimalHalf < 0)
                LValue.ConvertToNormTypeV2();
            if (LValue.DecimalHalf == 0)
            {
                return LValue.IntValue < RValue;
            }
            else
            {
                if (LValue.IntValue == NegativeRep)
                {//-0.5<0
                    if (RValue >= 0)
                        return true;
                }
                else if (LValue.IntValue < RValue) { return true; }//5.5 < 6
                else if (LValue.IntValue == RValue) { return LValue.IntValue < 0 ? true : false; }//-5.5<-5 vs 5.5 > 5
            }
            return false;
		}
		
        /// <summary>
        /// Less than or equal to operation between <see cref="AltDec&"/> and Integer Type.
        /// </summary>
        /// <param name="LValue">Left side AltDec value</param>
        /// <param name="RValue">Right side integer value</param>
        /// <returns>bool</returns>
	    template<typename IntType>
        static bool RightSideIntLessThanOrEqual(AltDec& LValue, IntType& RValue)
        {
#if defined(AltNum_EnableInfinityRep)
            if (LValue.ExtraRep == InfinityRep)
            {
                if (LValue.IntValue == -1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
    #if defined(AltNum_EnableDecimaledIFractionals)
            if (LValue.ExtraRep<0)
    #else
            if (LValue.ExtraRep == IRep)
    #endif
                throw "Can't compare real values against imaginary.";
    #if defined(AltNum_EnableIFractional)
            else if(LValue.ExtraRep==IByDivisorRep)
                throw "Can't compare real values against imaginary.";
    #endif
#endif
            if (LValue.ExtraRep != 0 || LValue.DecimalHalf < 0)
                LValue.ConvertToNormTypeV2();
            if (LValue.DecimalHalf == 0)
            {
                return LValue.IntValue <= RValue;
            }
            else
            {
                if (LValue.IntValue == NegativeRep)
                {//-0.5<0
                    if (RValue >= 0)
                        return true;
                }
                else if (LValue.IntValue < RValue) { return true; }//5.5<=6
                else if (LValue.IntValue == RValue) { return LValue.IntValue < 0 ? true : false; }
            }
            return false;
		}
		
        /// <summary>
        /// Greater than operation between <see cref="AltDec&"/> and Integer Type.
        /// </summary>
        /// <param name="LValue">Left side AltDec value</param>
        /// <param name="RValue">Right side integer value</param>
        /// <returns>bool</returns>
	    template<typename IntType>
        static bool RightSideIntGreaterThan(AltDec& LValue, IntType& RValue)
        {
#if defined(AltNum_EnableInfinityRep)
            if (LValue.ExtraRep == InfinityRep)
            {
                if (LValue.IntValue == 1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
    #if defined(AltNum_EnableDecimaledIFractionals)
            if (LValue.ExtraRep<0)
    #else
            if (LValue.ExtraRep == IRep)
    #endif
                throw "Can't compare real values against imaginary.";
    #if defined(AltNum_EnableIFractional)
            else if(LValue.ExtraRep==IByDivisorRep)
                throw "Can't compare real values against imaginary.";
    #endif
#endif
            if (LValue.ExtraRep != 0 || LValue.DecimalHalf < 0)
                LValue.ConvertToNormTypeV2();
            if (LValue.DecimalHalf == 0)
            {
                return LValue.IntValue > RValue;
            }
            else
            {
                if (LValue.IntValue == NegativeRep)
                {//-0.5>-1
                    if (RValue <= -1)
                        return true;
                }
                else if (LValue.IntValue > RValue) { return true; }
                else if (LValue.IntValue == RValue) { return LValue.IntValue < 0 ? false : true; }
            }
            return false;
		}
		
        /// <summary>
        /// Greater than or equal to operation between <see cref="AltDec&"/> and Integer Type.
        /// </summary>
        /// <param name="LValue">Left side AltDec value</param>
        /// <param name="RValue">Right side integer value</param>
        /// <returns>bool</returns>
	    template<typename IntType>
        static bool RightSideIntGreaterThanOrEqual(AltDec& LValue, IntType& RValue)
        {
#if defined(AltNum_EnableInfinityRep)
            if (LValue.ExtraRep == InfinityRep)
            {
                if (LValue.IntValue == 1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
    #if defined(AltNum_EnableDecimaledIFractionals)
            if (LValue.ExtraRep<0)
    #else
            if (LValue.ExtraRep == IRep)
    #endif
                throw "Can't compare real values against imaginary.";
    #if defined(AltNum_EnableIFractional)
            else if(LValue.ExtraRep==IByDivisorRep)
                throw "Can't compare real values against imaginary.";
    #endif
#endif
            if (LValue.ExtraRep != 0 || LValue.DecimalHalf < 0)
                LValue.ConvertToNormTypeV2();
            if (LValue.DecimalHalf == 0)
            {
                return LValue.IntValue >= RValue;
            }
            else
            {
                if (LValue.IntValue == NegativeRep)
                {
                    if (RValue <= -1)
                        return true;
                }
                else if (LValue.IntValue > RValue) { return true; }
                else if (LValue.IntValue == RValue) { return LValue.IntValue < 0 ? false : true; }//-5.5<-5 vs 5.5>5
            }
            return false;
		}
	
        /// <summary>
        /// Equality operation between Integer Type and <see cref="AltDec&"/> 
        /// </summary>
        /// <param name="LValue">Left side integer value</param>
        /// <param name="RValue">Right side AltDec value</param>
        /// <returns>bool</returns>
	    template<typename IntType>
        static bool LeftSideIntEqualTo(IntType& LValue, AltDec& RValue) { return RightSideIntEqualTo(RValue, LValue); }
	
        /// <summary>
        /// Not equal to operation between Integer Type and <see cref="AltDec&"/> 
        /// </summary>
        /// <param name="LValue">Left side integer value</param>
        /// <param name="RValue">Right side AltDec value</param>
        /// <returns>bool</returns>
	    template<typename IntType>
        static bool LeftSideIntNotEqualTo(IntType& LValue, AltDec& RValue) { return RightSideIntNotEqualTo(RValue, LValue); }
		
        /// <summary>
        /// Less than operation between Integer Type and <see cref="AltDec&"/> 
        /// </summary>
        /// <param name="LValue">Left side integer value</param>
        /// <param name="RValue">Right side AltDec value</param>
        /// <returns>bool</returns>
	    template<typename IntType>
        static bool LeftSideIntLessThan(IntType& LValue, AltDec& RValue) { return RightSideIntGreaterThan(RValue, LValue); }
		
        /// <summary>
        /// Less than or equal operation between Integer Type and <see cref="AltDec&"/> 
        /// </summary>
        /// <param name="LValue">Left side integer value</param>
        /// <param name="RValue">Right side AltDec value</param>
        /// <returns>bool</returns>
	    template<typename IntType>
        static bool LeftSideIntLessThanOrEqual(IntType& LValue, AltDec& RValue) { return RightSideIntGreaterThanOrEqual(RValue, LValue); }
		
        /// <summary>
        /// Greater than operation between Integer Type and <see cref="AltDec&"/> 
        /// </summary>
        /// <param name="LValue">Left side integer value</param>
        /// <param name="RValue">Right side AltDec value</param>
        /// <returns>bool</returns>
		template<typename IntType>
        static bool LeftSideIntGreaterThan(IntType& LValue, AltDec& RValue) { return RightSideIntLessThan(RValue, LValue); }
		
        /// <summary>
        /// Greater than or equal to operation between <see cref="AltDec&"/> and Integer Type.
        /// </summary>
        /// <param name="LValue">Left side integer value</param>
        /// <param name="RValue">Right side AltDec value</param>
        /// <returns>bool</returns>
		template<typename IntType>
        static bool LeftSideIntGreaterThanOrEqual(IntType& LValue, AltDec& RValue) { return RightSideIntLessThanOrEqual(RValue, LValue); }

    #pragma endregion AltDec-To-Int Comparison Methods

    #pragma region NormalRep Integer Division Operations
protected:
        template<typename IntType>
        void PartialIntDivOp(IntType& Value)
        {
            if (DecimalHalf == 0)
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                    IntValue *= -1;
                __int64 SRep = DecimalOverflowX * IntValue;
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflow;
                    SRep -= OverflowVal * DecimalOverflow;
                    IntValue = (signed int)(SelfIsNegative ? OverflowVal * -1 : OverflowVal);
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = SelfIsNegative ? NegativeRep : 0;
                    DecimalHalf = (signed int)SRep;
                }
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
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= DecimalOverflowX * OverflowVal;
                    IntValue = (signed int)(SelfIsNegative ? OverflowVal * -1 : OverflowVal);
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
        }
        template<typename IntType>
        void PartialIntDiv(IntType Value)
        {
            PartialIntDiv(Value);
        }
public:

		void PartialDivOp(signed int& Value) { PartialIntDivOp(Value); }
		void PartialDivOp(unsigned int& Value) { PartialIntDivOp(Value); }
		void PartialDivOp(signed long long& Value) { PartialIntDivOp(Value); }
        void PartialDivOp(unsigned long long& Value) { PartialIntDivOp(Value); }

		static void PartialDivOp(AltDec& self, signed int& Value) { self.PartialIntDivOp(Value); }
		static void PartialDivOp(AltDec& self, unsigned int& Value) { self.PartialIntDivOp(Value); }
		static void PartialDivOp(AltDec& self, signed long long& Value) { self.PartialIntDivOp(Value); }
        static void PartialDivOp(AltDec& self, unsigned long long& Value) { self.PartialIntDivOp(Value); }

		AltDec PartialDiv(signed int Value)
        { AltDec self = *this; PartialIntDivOp(Value); return self; }
		AltDec PartialDiv(unsigned int Value)
        { AltDec self = *this; PartialIntDivOp(Value); return self; }
		AltDec PartialDiv(signed long long Value)
        { AltDec self = *this; PartialIntDivOp(Value); return self; }
        AltDec PartialDiv(unsigned long long Value)
        { AltDec self = *this; PartialIntDivOp(Value); return self; }

		static AltDec PartialDiv(AltDec self, signed int Value) { self.PartialIntDivOp(Value); return self; }
		static AltDec PartialDiv(AltDec self, unsigned int Value) { self.PartialIntDivOp(Value); return self; }
		static AltDec PartialDiv(AltDec self, signed long long Value) { self.PartialIntDivOp(Value); return self; }
        static AltDec PartialDiv(AltDec self, unsigned long long Value) { self.PartialIntDivOp(Value); return self; }

protected:
        template<typename IntType>
        void BasicUnsignedIntDivOp(IntType& Value)
        {
            if (Value == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IsZero())
                return;
            PartialIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
        }
        template<typename IntType>
        void BasicUnsignedIntDiv(IntType Value)
        {
            BasicUnsignedIntDivOp(Value);
        }

        template<typename IntType>
        void BasicIntDivOp(IntType& Value)
        {
			if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            else if (Value == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IsZero())
                return;
            PartialIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
        }
        template<typename IntType>
        void BasicIntDiv(IntType Value)
        {
            BasicIntDivOp(Value);
        }
public:

		void BasicDivOp(signed int& Value) { BasicIntDivOp(Value); }
		void BasicDivOp(unsigned int& Value) { BasicUnsignedIntDivOp(Value); }
		void BasicDivOp(signed long long& Value) { BasicIntDivOp(Value); }
        void BasicDivOp(unsigned long long& Value) { BasicUnsignedIntDivOp(Value); }

		static void BasicDivOp(AltDec& self, signed int& Value) { self.BasicIntDivOp(Value); }
		static void BasicDivOp(AltDec& self, unsigned int& Value) { self.BasicUnsignedIntDivOp(Value); }
		static void BasicDivOp(AltDec& self, signed long long& Value) { self.BasicIntDivOp(Value); }
        static void BasicDivOp(AltDec& self, unsigned long long& Value) { self.BasicUnsignedIntDivOp(Value); }

		AltDec BasicDiv(signed int Value)
        { AltDec self = *this; BasicIntDivOp(Value); return self; }
		AltDec BasicDiv(unsigned int Value)
        { AltDec self = *this; BasicUnsignedIntDivOp(Value); return self; }
		AltDec BasicDiv(signed long long Value)
        { AltDec self = *this; BasicIntDivOp(Value); return self; }
        AltDec BasicDiv(unsigned long long Value)
        { AltDec self = *this; BasicUnsignedIntDivOp(Value); return self; }

		static AltDec BasicDiv(AltDec self, signed int Value) { self.BasicIntDivOp(Value); return self; }
		static AltDec BasicDiv(AltDec self, unsigned int Value) { self.BasicUnsignedIntDivOp(Value); return self; }
		static AltDec BasicDiv(AltDec self, signed long long Value) { self.BasicIntDivOp(Value); return self; }
        static AltDec BasicDiv(AltDec self, unsigned long long Value) { self.BasicUnsignedIntDivOp(Value); return self; }

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
		/// <summary>
        /// Partial Multiplication Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        void PartialIntMultOp(IntType& Value)
        {
            if (DecimalHalf == 0)
            {
                IntValue *= Value;
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
                SRep *= Value;
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
        template<typename IntType>
        void PartialIntMult(IntType Value)
        {
            PartialIntMultOp(Value);
        }
public:
        /// <summary>
        /// Multiplication Operation Between AltDec and Integer Value(Without negative flipping)
        /// </summary>
        /// <param name="Value">The value.</param>
        template<typename IntType>
        void UnsignedBasicIntMultOp(IntType& Value)
        {
            if (IntValue == 0 && DecimalHalf == 0)
                return;
            if (Value == 0)
                SetAsZero();
            else
                PartialIntMultOp(Value);
        }
        template<typename IntType>
        void UnsignedBasicIntMult(IntType Value)
        {
            UnsignedBasicIntMultOp(Value);
        }

        /// <summary>
        /// Multiplication Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        template<typename IntType>
        void BasicIntMultOp(IntType& Value)
        {
            if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            if (IntValue == 0 && DecimalHalf == 0)
                return;
            if (Value == 0)
                SetAsZero();
            else
                PartialIntMultOp(Value);
        }
        template<typename IntType>
        AltDec BasicIntMult(IntType Value)
        {
            AltDec self = *this;
            self.BasicIntMultOp(Value);
            return self;
        }
public:

		/// <summary>
        /// Basic Multiplication Operation
        /// </summary>
        /// <param name="Value">The value.</param>
		void BasicMultOp(signed int& Value) { BasicIntMultOp(Value); }
		void BasicMultOp(unsigned int& Value) { UnsignedBasicIntMultOp(Value); }
		void BasicMultOp(signed long long& Value) { BasicIntMultOp(Value); }
		void BasicMultOp(unsigned long long& Value) { UnsignedBasicIntMultOp(Value); }

		static void BasicMultOp(AltDec& self, signed int& Value) { self.BasicIntMultOp(Value); }
		static void BasicMultOp(AltDec& self, unsigned int& Value) { self.UnsignedBasicIntMultOp(Value); }
		static void BasicMultOp(AltDec& self, signed long long& Value) { self.BasicIntMultOp(Value); }
		static void BasicMultOp(AltDec& self, unsigned long long& Value) { self.UnsignedBasicIntMultOp(Value); }

		/// <summary>
        /// Basic Multiplication Operation that returns a value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
		AltDec BasicMult(signed int Value)
        { AltDec self = *this; BasicIntMultOp(Value); return self; }
		AltDec BasicMult(unsigned int Value)
        { AltDec self = *this; UnsignedBasicIntMultOp(Value); return self; }
		AltDec BasicMult(signed long long Value)
        { AltDec self = *this; BasicIntMultOp(Value); return self; }
        AltDec BasicMult(unsigned long long Value)
        { AltDec self = *this; UnsignedBasicIntMultOp(Value); return self; }

		static AltDec BasicMult(AltDec self, signed int Value) { self.BasicIntMultOp(Value); return self; }
		static AltDec BasicMult(AltDec self, unsigned int Value) { self.UnsignedBasicIntMultOp(Value); return self; }
		static AltDec BasicMult(AltDec self, signed long long Value) { self.BasicIntMultOp(Value); return self; }
        static AltDec BasicMult(AltDec self, unsigned long long Value) { self.UnsignedBasicIntMultOp(Value); return self; }

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations
protected:
        /// <summary>
        /// Addition Operation that skips negative zero(for when decimal half is empty)
        /// </summary>
        /// <param name="LValue">The LValue.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MirroredInt</returns>
        template<typename IntType>
        void NRepSkippingIntAddOp(IntType RValue)
        {
            if (RValue == 0)
                return;
            if (IntValue == 0)
                IntValue = RValue.Value;
            else
                IntHalfAdditionOp(RValue);
            return;
        }

        /// <summary>
        /// Addition Operation Between AltDec and Integer value
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>AltDec&</returns>
        template<typename IntType>
        void BasicIntAddition(IntType value)
        {
            if(DecimalHalf==0)
                NRepSkippingIntAddOp(value);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfAdditionOp(value);
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
            }
        }
public:

		/// <summary>
        /// Basic Addition Operation
        /// </summary>
        /// <param name="Value">The value.</param>
		void BasicAddOp(signed int& Value) { BasicIntAddition(Value); }
		void BasicAddOp(unsigned int& Value) { BasicIntAddition(Value); }
		void BasicAddOp(signed long long& Value) { BasicIntAddition(Value); }
		void BasicAddOp(unsigned long long& Value) { BasicIntAddition(Value); }

		static void BasicAddOp(AltDec& self, signed int& Value) { self.BasicIntAddition(Value); }
		static void BasicAddOp(AltDec& self, unsigned int& Value) { self.BasicIntAddition(Value); }
		static void BasicAddOp(AltDec& self, signed long long& Value) { self.BasicIntAddition(Value); }
		static void BasicAddOp(AltDec& self, unsigned long long& Value) { self.BasicIntAddition(Value); }

		/// <summary>
        /// Basic Addition Operation that returns a value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
		AltDec BasicAdd(signed int Value)
        { AltDec self = *this; BasicIntAddition(Value); return self; }
		AltDec BasicAdd(unsigned int Value)
        { AltDec self = *this; BasicIntAddition(Value); return self; }
		AltDec BasicAdd(signed long long Value)
        { AltDec self = *this; BasicIntAddition(Value); return self; }
        AltDec BasicAdd(unsigned long long Value)
        { AltDec self = *this; BasicIntAddition(Value); return self; }

		static AltDec BasicAdd(AltDec self, signed int Value) { self.BasicIntAddition(Value); return self; }
		static AltDec BasicAdd(AltDec self, unsigned int Value) { self.BasicIntAddition(Value); return self; }
		static AltDec BasicAdd(AltDec self, signed long long Value) { self.BasicIntAddition(Value); return self; }
        static AltDec BasicAdd(AltDec self, unsigned long long Value) { self.BasicIntAddition(Value); return self; }
#if defined(AltDec_UseMirroredInt)
        static AltDec BasicAdd(AltDec self, MirroredInt Value) { self.BasicMirroredIntAddOp(Value); return self; }
#endif

	#pragma endregion NormalRep Integer Addition Operations
	
    #pragma region NormalRep Integer Subtraction Operations
protected:

        template<typename IntType>
        void NRepSkippingIntSubOp(IntType RValue)
        {
            if (RValue == 0)
                return;
            if (IntValue == 0)
                IntValue = -RValue;
            else
                IntHalfSubtractionOp(RValue);
            return;
        }

		/// <summary>
        /// Basic Subtraction Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        template<typename IntType>
        void BasicIntSubtraction(IntType Value)
        {
            if (DecimalHalf == 0)
                NRepSkippingIntSubOp(Value);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfSubtractionOp(Value);
                //If flips to other side of negative, invert the decimals
                if(NegativeBeforeOperation^(IntValue<0))
                    DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
            }
        }

#if defined(AltDec_UseMirroredInt)
        /// <summary>
        /// Basic Subtraction Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicMirroredIntSubOp(MirroredInt& Value)
        {
            if (DecimalHalf == 0)
                IntValue.NRepSkippingSubOp(Value);
            else
            {
                bool NegativeBeforeOperation = IntValue < 0;
                IntHalfSubtractionOp(Value);
                //If flips to other side of negative, invert the decimals
                if (NegativeBeforeOperation ^ (IntValue < 0))
                    DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
            }
        }

        void BasicMirroredIntSubtraction(MirroredInt Value)
        {
            BasicMirroredIntSubOp(Value);
        }
#endif
public:

		/// <summary>
        /// Basic Subtraction Operation
        /// </summary>
        /// <param name="Value">The value.</param>
		void BasicSubOp(signed int& Value) { BasicIntSubtraction(Value); }
		void BasicSubOp(unsigned int& Value) { BasicIntSubtraction(Value); }
		void BasicSubOp(signed long long& Value) { BasicIntSubtraction(Value); }
		void BasicSubOp(unsigned long long& Value) { BasicIntSubtraction(Value); }

		static void BasicSubOp(AltDec& self, signed int& Value) { self.BasicIntSubtraction(Value); }
		static void BasicSubOp(AltDec& self, unsigned int& Value) { self.BasicIntSubtraction(Value); }
		static void BasicSubOp(AltDec& self, signed long long& Value) { self.BasicIntSubtraction(Value); }
		static void BasicSubOp(AltDec& self, unsigned long long& Value) { self.BasicIntSubtraction(Value); }

		/// <summary>
        /// Basic Subtraction Operation that returns a value
        /// </summary>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec</returns>
		AltDec BasicSub(signed int Value)
        { AltDec self = *this; BasicIntSubtraction(Value); return self; }
		AltDec BasicSub(unsigned int Value)
        { AltDec self = *this; BasicIntSubtraction(Value); return self; }
		AltDec BasicSub(signed long long Value)
        { AltDec self = *this; BasicIntSubtraction(Value); return self; }
        AltDec BasicSub(unsigned long long Value)
        { AltDec self = *this; BasicIntSubtraction(Value); return self; }

        /// <summary>
        /// Basic Subtraction Operation that returns a value
        /// </summary>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec</returns>
		static AltDec BasicSub(AltDec self, signed int Value) { self.BasicIntSubtraction(Value); return self; }
		static AltDec BasicSub(AltDec self, unsigned int Value) { self.BasicIntSubtraction(Value); return self; }
		static AltDec BasicSub(AltDec self, signed long long Value) { self.BasicIntSubtraction(Value); return self; }
        static AltDec BasicSub(AltDec self, unsigned long long Value) { self.BasicIntSubtraction(Value); return self; }
#if defined(AltDec_UseMirroredInt)
        static AltDec BasicSub(AltDec self, MirroredInt Value) { self.BasicMirroredIntSubOp(Value); return self; }
#endif

    #pragma endregion NormalRep Integer Subtraction Operations
	
    #pragma region NormalRep Integer Bitwise Operations

	#pragma endregion NormalRep Integer Bitwise Operations

	#pragma region NormalRep AltNumToAltNum Operations
protected:
        //Return true if divide into zero
        bool PartialDivOp(AltDec& Value)
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
                if (Value < 0)
                {
                    if (Value.IntValue == NegativeRep)
                        ValueRes = Value.DecimalHalf;
                    else
                        ValueRes =  NegDecimalOverflowX* Value.IntValue + Value.DecimalHalf;
                }
				else
				{
				    ResIsPositive = false;
					ValueRes = DecimalOverflowX * Value.IntValue +Value.DecimalHalf;
				}
			}
			else
			{
                
				SelfRes = DecimalOverflowX* IntValue+DecimalHalf;
			    if(Value<0)
				{
				    ResIsPositive = false;
					ValueRes = Value.IntValue==NegativeRep ? DecimalHalf: NegDecimalOverflowX*IntValue +Value.DecimalHalf;
				}
				else
					ValueRes = DecimalOverflowX* Value.IntValue +Value.DecimalHalf;
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

        bool UnsignedPartialDivOp(AltDec& Value)
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
                ValueRes = DecimalOverflowX * Value.IntValue + Value.DecimalHalf;
            }
            else
            {

                SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
                ValueRes = DecimalOverflowX * Value.IntValue + Value.DecimalHalf;
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

        bool PartialDiv(AltDec Value)
        {
            return PartialDivOp(Value);
        }
public:
		
        void BasicDivOp(AltDec& Value)
        {
			if (PartialDivOp(Value))//Prevent Dividing into nothing
				DecimalHalf = 1;
        }

        AltDec BasicDivOp(AltDec& self, AltDec Value)
        {
            if (self.PartialDivOp(Value))//Prevent Dividing into nothing
                self.DecimalHalf = 1;
            return self;
        }

        AltDec UnsignedBasicDiv(AltDec self, AltDec Value)
        {
            if (self.UnsignedPartialDivOp(Value))//Prevent Dividing into nothing
                self.DecimalHalf = 1;
            return self;
        }

        AltDec UnsignedBasicDivOp(AltDec& self, AltDec Value)
        {
            if (self.UnsignedPartialDivOp(Value))//Prevent Dividing into nothing
                self.DecimalHalf = 1;
            return self;
        }

        AltDec BasicDiv(AltDec Value)
        {
            AltDec self = *this;
            self.BasicDivOp(Value);
            return self;
        }

		void CatchAllDivision(AltDec& Value, RepType& LRep, RepType& RRep)
		{
			ConvertToNormType(LRep);
			Value.ConvertToNormType(RRep);
			BasicDivOp(Value);
		}

        AltDec CatchAllDivisionAsCopies(AltDec Value, RepType& LRep, RepType& RRep)
        { AltDec self = *this; CatchAllDivision(Value, LRep, RRep); return self; }
		
		void CatchAllDivisionV2(AltDec& Value, RepType& SameRep)
		{
			ConvertToNormType(SameRep);
			Value.ConvertToNormType(SameRep);
			BasicDivOp(Value);
		}

        AltDec CatchAllDivisionAsCopiesV2(AltDec Value, RepType& SameRep)
        { AltDec self = *this; CatchAllDivisionV2(Value, SameRep); return self; }
	
	   void CatchAllDivisionV3(AltDec& Value)
	   {
		   ConvertToNormTypeV2();
		   Value.ConvertToNormTypeV2();
		   BasicDivOp(Value);
	   }

        AltDec CatchAllDivisionAsCopiesV3(AltDec Value)
        { AltDec self = *this; CatchAllDivisionV3(Value); return self; }
		
	//Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
	#if defined(AltNum_EnableImaginaryNum)
		void CatchAllImaginaryDivision(AltDec& Value, RepType& LRep, RepType& RRep)
		{
			ConvertIRepToNormal(LRep);
			Value.ConvertIRepToNormal(RRep);
			BasicDivOp(Value);
            ExtraRep = 0;
		}

        AltDec CatchAllImaginaryDivisionAsCopies(AltDec Value, RepType& LRep, RepType& RRep)
        { AltDec self = *this; CatchAllImaginaryDivision(Value, LRep, RRep); return self; }
		
		void CatchAllImaginaryDivisionV2(AltDec& Value, RepType& SameRep)
		{
			ConvertIRepToNormal(SameRep);
			Value.ConvertIRepToNormal(SameRep);
			BasicDivOp(Value);
			ExtraRep = 0;
		}

        AltDec CatchAllImaginaryDivisionAsCopiesV2(AltDec Value, RepType& SameRep)
        { AltDec self = *this; CatchAllImaginaryDivision(Value, SameRep); return self; }
	
	   void CatchAllImaginaryDivisionV3(AltDec& Value)
	   {
		   ConvertIRepToNormal();
		   Value.ConvertIRepToNormal();
		   BasicDivOp(Value);
		   ExtraRep = 0;
	   }

        AltDec CatchAllImaginaryDivisionAsCopiesV3(AltDec Value)
        { AltDec self = *this; CatchAllImaginaryDivision(Value); return self; }
	#endif

public:
        static void RepToRepDivOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value);

        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec&</returns>
        AltDec& DivOp(AltDec& Value);
		
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The leftside value.</param>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec&</returns>
		static AltDec DivOp(AltDec& self, AltDec& Value) { return self.DivOp(Value); }

        static AltDec Divide(AltDec self, AltDec Value) { return self.DivOp(Value); }

		/// <summary>
        /// Basic Multiplication Operation(main code block)
        /// </summary>
        /// <param name="Value">The value.</param>
		bool BasicMultOpPt2(AltDec& Value)
		{
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                }
                else if (Value.DecimalHalf == 0)
                {
                    IntValue *= Value.IntValue;
                }
                else
                {
                    Value.PartialIntMultOp(IntValue);
                    IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                }
				return false;
            }
            else if (IntValue == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= AltDec::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
					return DecimalHalf==0?true:false;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * Value.IntValue;
                    if (SRep >= AltDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / AltDec::DecimalOverflowX;
                        SRep -= OverflowVal * AltDec::DecimalOverflowX;
                        IntValue = OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
            }
            else if (IntValue == AltDec::NegativeRep)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= AltDec::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * Value.IntValue;
                    if (SRep >= AltDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / AltDec::DecimalOverflowX;
                        SRep -= OverflowVal * AltDec::DecimalOverflowX;
                        IntValue = -OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
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
                if (Value.IntValue == 0)
                {
                    __int64 SRep = AltDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.DecimalHalf;
                    SRep /= AltDec::DecimalOverflowX;
                    if (SRep >= AltDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / AltDec::DecimalOverflowX;
                        SRep -= OverflowVal * AltDec::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? AltDec::NegativeRep : 0;
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
                else if (Value.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = AltDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.GetIntHalf();
                    if (SRep >= AltDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / AltDec::DecimalOverflowX;
                        SRep -= OverflowVal * AltDec::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? AltDec::NegativeRep : 0;
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : AltDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.GetIntHalf();//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(Value.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)Value.DecimalHalf;
                    Temp04 /= AltDec::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / AltDec::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)AltDec::DecimalOverflow;
                    if (IntHalf == 0) { IntValue = (signed int)SelfIsNegative ? AltDec::NegativeRep : 0; }
                    else { IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf; }
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
            if(DecimalHalf==0&&IntValue==0)
                return true;
            else
                return false;
		}
        bool BasicMultPt2(AltDec Value)
        {
            return BasicMultOpPt2(Value);
        }
		
		/// <summary>
        /// Basic Multiplication Operation(without checking for special representation variations or zero)
		/// Returns true if prevented from multiplying into nothing(except when multipling by zero)
        /// </summary>
        /// <param name="Value">The value.</param>
		bool UnsignedBasicMultOp(AltDec& Value)
		{
            if (Value.IntValue < 0)
            {
                Value.SwapNegativeStatus();
                SwapNegativeStatus();
            }
			if(BasicMultOpPt2(Value))//Prevent multiplying into zero
				DecimalHalf = 1;
			else
				return false;
            return true;
		}
        bool UnsignedBasicMult(AltDec Value)
        {
            return UnsignedBasicMultOp(Value);
        }

		/// <summary>
        /// Basic Multiplication Operation(before ensuring doesn't multiply into nothing)
        /// </summary>
        /// <param name="Value">The value.</param>
		bool BasicMultOp(AltDec& Value)
        {//Warning:Modifies Value to make it a positive variable
        //Only checking for zero multiplication in main multiplication method
        //Not checking for special representation variations in this method(closer to AltDec operation code)
            if (Value.IntValue < 0)
            {
                Value.SwapNegativeStatus();
                SwapNegativeStatus();
            }
            return UnsignedBasicMultOp(Value);
        }
        bool BasicMult(AltDec Value)
        {
            return BasicMultOp(Value);
        }

		void CatchAllMultiplication(AltDec& Value, RepType& LRep, RepType& RRep)
		{
			ConvertToNormType(LRep);
			Value.ConvertToNormType(RRep);
			BasicMultOp(Value);
		}
		
		void CatchAllMultiplicationV2(AltDec& Value, RepType& SameRep)
		{
            ConvertToNormType(SameRep);
			Value.ConvertToNormType(SameRep);
			BasicMultOp(Value);
		}
		
		void CatchAllMultiplicationV3(AltDec& Value)
		{
			ConvertToNormTypeV2();
			Value.ConvertToNormTypeV2();
			BasicMultOp(Value);
		}

	//Both sides are assumed to be imaginary number types of representations for CatchAllImaginary..
	#if defined(AltNum_EnableImaginaryNum)
		void CatchAllImaginaryMultiplication(AltDec& Value, RepType& LRep, RepType& RRep)
		{
			ConvertIRepToNormal(LRep);
			Value.ConvertIRepToNormal(RRep);
			BasicMultOp(Value);
		   ExtraRep = 0;
		   SwapNegativeStatus();
		}

        AltDec CatchAllImaginaryMultiplicationAsCopies(AltDec Value, RepType& LRep, RepType& RRep)
        { AltDec self = *this; CatchAllImaginaryMultiplication(Value, LRep, RRep); return self; }
		
		void CatchAllImaginaryMultiplication(AltDec& Value, RepType& SameRep)
		{
			ConvertIRepToNormal(SameRep);
			Value.ConvertIRepToNormal(SameRep);
			BasicMultOp(Value);
		   ExtraRep = 0;
		   SwapNegativeStatus();
		}

        AltDec CatchAllImaginaryMultiplicationAsCopies(AltDec Value, RepType& SameRep)
        { AltDec self = *this; CatchAllImaginaryMultiplication(Value, SameRep); return self; }
	
	   void CatchAllImaginaryMultiplicationV3(AltDec& Value)
	   {
		   ConvertIRepToNormal();
		   Value.ConvertIRepToNormal();
		   BasicMultOp(Value);
		   ExtraRep = 0;
		   SwapNegativeStatus();
	   }

        AltDec CatchAllImaginaryMultiplicationAsCopiesV3(AltDec Value)
        { AltDec self = *this; CatchAllImaginaryMultiplication(Value); return self; }
	#endif

        static void RepToRepMultOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value);

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec&</returns>
        AltDec& MultOp(AltDec& Value);
		
        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The leftside value.</param>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec&</returns>
		static AltDec MultOp(AltDec& self, AltDec& Value) { return self.MultOp(Value); }
		
		AltDec Multiple(AltDec self, AltDec Value) { return self.MultOp(Value); }

        /// <summary>
        /// Basic Addition Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicAddOp(AltDec& Value)
        {
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfAdditionOp(Value.IntValue);
            if (Value.DecimalHalf != 0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltDec::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= AltDec::DecimalOverflow) { DecimalHalf -= AltDec::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltDec::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= AltDec::DecimalOverflow) { DecimalHalf -= AltDec::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltDec::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= AltDec::DecimalOverflow) { DecimalHalf -= AltDec::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltDec::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= AltDec::DecimalOverflow) { DecimalHalf -= AltDec::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
        }
	
        void CatchAllAddition(AltDec& Value, RepType& LRep, RepType& RRep)
        {
            ConvertToNormType(LRep);
            Value.ConvertToNormType(RRep);
            BasicAddOp(Value);
        }
		
        void CatchAllAdditionV2(AltDec& Value, RepType& SameRep)
        {
            ConvertToNormType(SameRep);
            Value.ConvertToNormType(SameRep);
            BasicAddOp(Value);
        }
		
		void CatchAllAdditionV3(AltDec& Value)
		{
			ConvertToNormTypeV2();
			Value.ConvertToNormTypeV2();
            BasicAddOp(Value);
        }
		
	#if defined(AltNum_EnableImaginaryNum)
		void CatchAllImaginaryAddition(AltDec& Value, RepType& LRep, RepType& RRep)
		{
			ConvertAsNormalIRep(LRep);
			Value.ConvertAsNormalIRep(RRep);
			BasicAddOp(Value);
		}

        AltDec CatchAllImaginaryAdditionAsCopies(AltDec Value, RepType& LRep, RepType& RRep)
        { AltDec self = *this; CatchAllImaginaryAddition(Value, LRep, RRep); return self; }
		
		void CatchAllImaginaryAdditionV2(AltDec& Value, RepType& SameRep)
		{
			ConvertAsNormalIRep(SameRep);
			Value.ConvertAsNormalIRep(SameRep);
			BasicAddOp(Value);
		}

        AltDec CatchAllImaginaryAdditionAsCopies(AltDec Value, RepType& SameRep)
        { AltDec self = *this; CatchAllImaginaryAddition(Value, SameRep); return self; }
	
	   void CatchAllImaginaryAdditionV3(AltDec& Value)
	   {
		   ConvertAsNormalIRep();
		   Value.ConvertAsNormalIRep();
		   BasicAddOp(Value);
	   }

        AltDec CatchAllImaginaryAdditionAsCopies(AltDec Value)
        { AltDec self = *this; CatchAllImaginaryAddition(Value); return self; }
	#endif

        static bool RepToRepAddOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value);

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec&</returns>
        AltDec& AddOp(AltDec& Value);

        AltDec Add(AltDec self, AltDec Value) { return self.AddOp(Value); }

		AltDec& BasicSubOp(AltDec& Value)
        {
            //NegativeBeforeOperation
            bool WasNegative = IntValue < 0;
            //Deal with Int section first
            IntHalfSubtractionOp(Value.IntValue);
            //Now deal with the decimal section
            if(Value.DecimalHalf!=0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltDec::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= AltDec::DecimalOverflow) { DecimalHalf -= AltDec::DecimalOverflow; --IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltDec::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= AltDec::DecimalOverflow) { DecimalHalf -= AltDec::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltDec::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= AltDec::DecimalOverflow) { DecimalHalf -= AltDec::DecimalOverflow; --IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += AltDec::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= AltDec::DecimalOverflow) { DecimalHalf -= AltDec::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(WasNegative ^(IntValue<0))
                DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
        }
	
		void CatchAllSubtraction(AltDec& Value, RepType& LRep, RepType& RRep)
		{
			ConvertToNormType(LRep);
			Value.ConvertToNormType(RRep);
			BasicSubOp(Value);
		}
		
		void CatchAllSubtractionV2(AltDec& Value, RepType& SameRep)
		{
			ConvertToNormType(SameRep);
			Value.ConvertToNormType(SameRep);
			BasicSubOp(Value);
		}
		
		void CatchAllSubtractionV3(AltDec& Value)
		{
			ConvertToNormTypeV2();
			Value.ConvertToNormTypeV2();
			BasicSubOp(Value);
		}
		
	#if defined(AltNum_EnableImaginaryNum)
		void CatchAllImaginarySubtraction(AltDec& Value, RepType& LRep, RepType& RRep)
		{
			ConvertAsNormalIRep(LRep);
			Value.ConvertAsNormalIRep(RRep);
			BasicSubOp(Value);
		}

        AltDec CatchAllImaginarySubtractionAsCopies(AltDec Value, RepType& LRep, RepType& RRep)
        { AltDec self = *this; CatchAllImaginarySubtraction(Value, LRep, RRep); return self; }
		
		void CatchAllImaginarySubtraction(AltDec& Value, RepType& SameRep)
		{
			ConvertAsNormalIRep(SameRep);
			Value.ConvertAsNormalIRep(SameRep);
			BasicSubOp(Value);
		}

        AltDec CatchAllImaginarySubtractionAsCopies(AltDec Value, RepType& SameRep)
        { AltDec self = *this; CatchAllImaginarySubtraction(Value, SameRep); return self; }
	
	   void CatchAllImaginarySubtraction(AltDec& Value)
	   {
		   ConvertAsNormalIRep();
		   Value.ConvertAsNormalIRep();
		   BasicSubOp(Value);
	   }

        AltDec CatchAllImaginarySubtractionAsCopies(AltDec Value)
        { AltDec self = *this; CatchAllImaginarySubtraction(Value); return self; }
	#endif

        static bool RepToRepSubOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value);

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec&</returns>
        AltDec& SubOp(AltDec& Value);
		
		AltDec Subtract(AltDec self, AltDec Value) { return self.SubOp(Value); }

    #pragma endregion NormalRep AltNumToAltNum Operations
	
    #pragma region Other Integer Operations
        /// <summary>
        /// Division Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec&</returns>
        template<typename IntType>
        static AltDec& UnsignedIntDivOp(IntType& Value)
        {
            if (Value==1)
                return;
            if (Value == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity(); 
                return;
#else
                throw "Target value can not be divided by zero";
#endif
            }

            RepType LRep = GetRepType();
			switch (LRep)
			{
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv:
		#elif defined(AltNum_EnablePiFractional)
				case RepType::PiFractional:
		#endif
		#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv:
		#elif defined(AltNum_EnableEFractional)
				case RepType::EFractional:
		#endif
                    #if defined(AltNum_EnablePiRep)||defined(AltNum_EnableERep)
					ConvertToNormType(LRep);
					BasicIntDivOpV2(Value);
					break;
                    #endif
		#if defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv:
		#elif defined(AltNum_EnableIFractional)
				case RepType::IFractional:
		#endif
        #if defined(AltNum_EnableDecimaledIFractionals)||defined(AltNum_EnableIFractional)
					ConvertToNormType(LRep);
					BasicIntDivOpV2(Value);
                    break;
        #endif
	#endif
    #ifdef AltNum_EnableImaginaryInfinity
				case RepType::PositiveImaginaryInfinity:
				case RepType::NegativeImaginaryInfinity:
					return;
					break;
    #endif
	#if defined(AltNum_EnableImaginaryNum)
	#endif
	#if defined(AltNum_EnableApproachingValues)
				case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
					if(IntValue.IsZero())
						return;
					ConvertToNormType(LRep);
					BasicIntDivOpV2(Value);
	    #if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
	    #endif
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative)
			#endif
		#endif
        #if !defined(AltNum_DisableApproachingTop)||defined(AltNum_EnableApproachingDivided)
					ConvertToNormType(LRep);
					BasicIntDivOpV2(Value);
					break;
        #endif
	#endif
	#if defined(AltNum_EnableFractionals)
	#endif
	#if defined(AltNum_EnableImaginaryInfinity)
				case RepType::PositiveImaginaryInfinity:
				case RepType::NegativeImaginaryInfinity:
					return;
					break;
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
					ConvertToNormalIRep(LRep);
					BasicIntDivOpV2(Value);
					break;
	    #endif
    #endif
	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                    int divRes;
                    int C;
                    if(IntValue.IsZero())//Become NumByDiv
                    {
                        divRes = DecimalHalf/ExtraRep;//-4/3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if(C==0)
                        {
                            if(IntValue==NegativeRep)
                                IntValue = divRes;
                            else
                                IntValue = -divRes;
                            DecimalHalf = 0;
                        }
                        else
                        {
                            if(IntValue==NegativeRep)
                                IntValue = DecimalHalf;
                            else
                                IntValue = -DecimalHalf;
                            DecimalHalf = 0;
                            ExtraRep *= Value;
                        }
                        return;
                    }
                    divRes = IntValue/ExtraRep;
                    if(divRes!=0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if(C==0)
                        {
                            throw "ToDo: Impliment code here"; 
                        }
                        else
                        {
                            throw "ToDo: Impliment code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Impliment code here";
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
                    int divRes;
                    int C;
                    if(IntValue.IsZero())//Become Fractional
                    {
                        divRes = DecimalHalf/ExtraRep;//-4/-3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if(C==0)
                        {
            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Impliment code here";
			#elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Impliment code here";
			#elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
			#else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
			
            #endif
                        }
                        else
                        {
            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Impliment code here";
			#elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Impliment code here";
			#elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
			#else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
			
            #endif
                        }
                        //return;
                    }
                    divRes = IntValue/ExtraRep;
                    if(divRes!=0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if(C==0)
                        {
                            throw "ToDo: Impliment code here"; 
                        }
                        else
                        {
                            throw "ToDo: Impliment code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Impliment code here";
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
					BasicIntDivOpV2(Value);
					break;
			}
            return;
        }
        template<typename IntType>
        static AltDec& UnsignedIntDiv(IntType Value)
        {
            return UnsignedIntDivOp(Value);
        }

        /// <summary>
        /// Division Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec&</returns>
        template<typename IntType>
        AltDec& IntDivOp(IntType& Value)
        {
            if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            return UnsignedIntDivOp(Value);
        }
        template<typename IntType>
        AltDec& IntDiv(IntType Value)
        {
            return IntDivOp(Value);
        }
		
        /// <summary>
        /// Division Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        static AltDec& IntDivOp(AltDec& self, IntType& Value) { return self.IntDivOp(Value); }

		AltDec& DivOp(signed int& Value) { return IntDivOp(Value); }
		AltDec& DivOp(unsigned int& Value){ return UnsignedIntDivOp(Value); }
		AltDec& DivOp(signed long long& Value) { return IntDivOp(Value); }
        AltDec& DivOp(unsigned long long& Value) { return UnsignedIntDivOp(Value); }
        AltDec& DivOp(signed char& Value) { return IntDivOp(Value); }
        AltDec& DivOp(unsigned char& Value) { return UnsignedIntDivOp(Value); }
        AltDec& DivOp(signed short& Value) { return IntDivOp(Value); }
        AltDec& DivOp(unsigned short& Value) { return UnsignedIntDivOp(Value); }
		
		static AltDec& DivOp(AltDec& self, signed int& Value) { return self.IntDivOp(Value); }
		static AltDec& DivOp(AltDec& self, unsigned int& Value){ return self.UnsignedIntDivOp(Value); }
		static AltDec& DivOp(AltDec& self, signed long long& Value) { return self.IntDivOp(Value); }
        static AltDec& DivOp(AltDec& self, unsigned long long& Value) { return self.UnsignedIntDivOp(Value); }
		
		static AltDec Divide(AltDec self, signed int Value) { return self.IntDivOp(Value);}
		static AltDec Divide(AltDec self, unsigned int Value) { return self.UnsignedIntDivOp(Value);}
		static AltDec Divide(AltDec self, signed long long Value) { return self.IntDivOp(Value); }
        static AltDec Divide(AltDec self, unsigned long long Value) { return self.UnsignedIntDivOp(Value); }

    #pragma endregion Division Operations

    #pragma region Other Multiplication Operations
protected:
        template<typename IntType>
        static AltDec& IntMultOpPt2(IntType& Value)
        {
            RepType LRep = GetRepType();
			switch (LRep)
			{
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv:
		#elif defined(AltNum_EnablePiFractional)
				case RepType::PiFractional:
		#endif
		#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv:
		#elif defined(AltNum_EnableEFractional)
				case RepType::EFractional:
		#endif
					ConvertToNormType(LRep);
					BasicIntMultOpV2(Value);
					break;
		#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::INumByDiv:
		#elif defined(AltNum_EnableEFractional)
				case RepType::IFractional:
		#endif
					ConvertToNormType(LRep);
					BasicIntMultOpV2(Value);
					break;
	#endif
				case RepType::PositiveImaginaryInfinity:
				case RepType::NegativeImaginaryInfinity:
					return;
					break;
	#if defined(AltNum_EnableImaginaryNum)
	#endif
	#if defined(AltNum_EnableApproachingValues)
				case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
					if(IsZero())
						return;
					ConvertToNormType(LRep);
					BasicIntMultOpV2(Value);
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
					ConvertToNormType(LRep);
					BasicIntMultOpV2(Value);
					break;
	#endif
	#if defined(AltNum_EnableFractionals)
	#endif
	#if defined(AltNum_EnableImaginaryInfinity)
				case RepType::PositiveImaginaryInfinity:
				case RepType::NegativeImaginaryInfinity:
					return;
					break;
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
					ConvertToNormalIRep(LRep);
					BasicIntMultOpV2(Value);
					break;
	    #endif
    #endif
	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
					BasicIntMultOpV2(Value);
					DecimalHalf *= Value;
					int divRes = DecimalHalf / -ExtraRep;
					if(divRes>0)
					{
						int increment = ExtraRep * divRes;
						if(IntValue<0)
							IntHalfSubtractionOp(increment);
						else
							IntHalfAdditionOp(increment);
						DecimalHalf = DecimalHalf + increment;
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
					BasicIntMultOpV2(Value);
					DecimalHalf *= Value;
					int divRes = DecimalHalf / ExtraRep;
					if(divRes>0)
					{
						int increment = ExtraRep * divRes;
						if(IntValue<0)
							IntHalfSubtractionOp(increment);
						else
							IntHalfAdditionOp(increment);
						DecimalHalf = DecimalHalf - increment;
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
					BasicIntMultOpV2(Value);
					break;
			}
        }

        //IntMultOp without negative check
        template<typename IntType>
        static AltDec& UnsignedIntMultOp(IntType& Value)
        {
            if (IsZero()||Value==1)
                return;
            if (Value == 0)
            {
                SetAsZero(); return;
            }
            IntMultOpPt2(Value);
        }

        /// <summary>
        /// Multiplication Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        static AltDec& IntMultOp(IntType& Value)
        {
            if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            if (IsZero()||Value==1)
                return;
            if (Value == 0)
            {
                SetAsZero(); return;
            }
            IntMultOpPt2(Value);
        }
public:

        /// <summary>
        /// Multiplication Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
		AltDec& MultOp(signed int& Value) { return IntMultOp(Value); }
		AltDec& MultOp(signed long long& Value) { return IntMultOp(Value); }
		AltDec& MultOp(unsigned int& Value) { return UnsignedIntMultOp(Value); }
		AltDec& MultOp(unsigned long long& Value) { return UnsignedIntMultOp(Value); }
        AltDec& MultOp(signed char& Value) { return IntMultOp(Value); }
        AltDec& MultOp(signed short& Value) { return IntMultOp(Value); }
        AltDec& MultOp(unsigned char& Value) { return UnsignedIntMultOp(Value); }
        AltDec& MultOp(unsigned short& Value) { return UnsignedIntMultOp(Value); }

        static AltDec Multiple(AltDec self, signed int Value) { return self.IntMultOp(Value); }
        static AltDec Multiple(AltDec self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        static AltDec Multiple(AltDec self, signed long long Value) { return self.IntMultOp(Value); }
        static AltDec Multiple(AltDec self, unsigned long long Value) { return self.UnsignedIntMultOp(Value); }

#if defined(AltDec_UseMirroredInt)
        AltDec& MultOp(MirroredInt& Value)
        { 
            if (Value < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            if (IsZero() || Value == 1)
                return *this;
            if (Value == 0)
            {
                SetAsZero(); return *this;
            }
            IntMultOpPt2(Value);
            return *this;
        }
#endif
    #pragma endregion Other Multiplication Operations

    #pragma region Other Addition Operations
protected:
        /// <summary>
        /// Addition Operation Between AltDec and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltDec&</returns>
        template<typename IntType>
        static AltDec& IntAddOp(IntType& value)
        {
            if (value == 0)
                return;
	#if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert AltDec into complex number at moment";
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
				ConvertToNormTypeV2();
			bool WasNegative = IntValue < 0;
			IntHalfAdditionOp(value);
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ (IntValue >= 0))//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
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
					DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
			}
	#else
            if(WasNegative ^ (IntValue >= 0))
				DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
	#endif
            return;
        }

        /// <summary>
        /// Addition Operation Between AltDec and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltDec&</returns>
        template<typename IntType>
        static AltDec& IntAddOp(AltDec& self, IntType& value) { return self.IntAddOp(value); }

public:

    /// <summary>
    ///  Addition Operation
    /// </summary>
    /// <param name="Value">The value.</param>
    AltDec& AddOp(signed int& Value) { return IntAddOp(Value); }
    AltDec& AddOp(unsigned int& Value) { return IntAddOp(Value); }
    AltDec& AddOp(signed long long& Value) { return IntAddOp(Value); }
    AltDec& AddOp(unsigned long long& Value) { return IntAddOp(Value); }
    AltDec& AddOp(signed char& Value) { return IntAddOp(Value); }
    AltDec& AddOp(unsigned char& Value) { return IntAddOp(Value); }
    AltDec& AddOp(signed short& Value) { return IntAddOp(Value); }
    AltDec& AddOp(unsigned short& Value) { return IntAddOp(Value); }

    static AltDec& AddOp(AltDec& self, signed int& Value) { return self.IntAddOp(Value); }
    static AltDec& AddOp(AltDec& self, unsigned int& Value) { return self.IntAddOp(Value); }
    static AltDec& AddOp(AltDec& self, signed long long& Value) { return self.IntAddOp(Value); }
    static AltDec& AddOp(AltDec& self, unsigned long long& Value) { return self.IntAddOp(Value); }

    static AltDec Addition(AltDec self, signed int Value) { return self.IntAddOp(Value); }
    static AltDec Addition(AltDec self, unsigned int Value) { return self.IntAddOp(Value); }
    static AltDec Addition(AltDec self, signed long long Value) { return self.IntAddOp(Value); }
    static AltDec Addition(AltDec self, unsigned long long Value) { return self.IntAddOp(Value); }

    #pragma endregion Other Addition Operations

    #pragma region Other Subtraction Operations
protected:
        /// <summary>
        /// Subtraction Operation Between AltDec and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        static AltDec& IntSubOp(IntType& value)
        {
            if (value == 0)
                return;
	#if defined(AltNum_EnableImaginaryNum)
            if(ExtraRep==IRep)
            {
                throw "Can't convert AltDec into complex number at moment";
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
				ConvertToNormTypeV2();
			bool WasNegative = IntValue < 0;
			IntHalfAddition(value);
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ (IntValue >= 0))//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
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
					DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
			}
	#else
            if(WasNegative ^ (IntValue >= 0))
				DecimalHalf = AltDec::DecimalOverflow - DecimalHalf;
	#endif
            return;
        }

        /// <summary>
        /// Subtraction Operation Between AltDec and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        static AltDec& IntSubOp(AltDec& self, IntType& value) { return self.IntSubOp(value); }
public:

    /// <summary>
    ///  Subtraction Operation
    /// </summary>
    /// <param name="Value">The value.</param>
    AltDec& SubOp(signed int& Value) { return IntSubOp(Value); }
    AltDec& SubOp(unsigned int& Value) { return IntSubOp(Value); }
    AltDec& SubOp(signed long long& Value) { return IntSubOp(Value); }
    AltDec& SubOp(unsigned long long& Value) { return IntSubOp(Value); }
    AltDec& SubOp(signed char& Value) { return IntSubOp(Value); }
    AltDec& SubOp(unsigned char& Value) { return IntSubOp(Value); }
    AltDec& SubOp(signed short& Value) { return IntSubOp(Value); }
    AltDec& SubOp(unsigned short& Value) { return IntSubOp(Value); }

    static AltDec& SubOp(AltDec& self, signed int& Value) { return self.IntSubOp(Value); }
    static AltDec& SubOp(AltDec& self, unsigned int& Value) { return self.IntSubOp(Value); }
    static AltDec& SubOp(AltDec& self, signed long long& Value) { return self.IntSubOp(Value); }
    static AltDec& SubOp(AltDec& self, unsigned long long& Value) { return self.IntSubOp(Value); }

    static AltDec Subtract(AltDec self, signed int Value) { return self.IntSubOp(Value); }
    static AltDec Subtract(AltDec self, unsigned int Value) { return self.IntSubOp(Value); }
    static AltDec Subtract(AltDec self, signed long long Value) { return self.IntSubOp(Value); }
    static AltDec Subtract(AltDec self, unsigned long long Value) { return self.IntSubOp(Value); }

    #pragma endregion Other Subtraction Operations

    #pragma region Main Operator Overrides
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator/(AltDec self, AltDec Value) { return DivOp(self, Value); }
        friend AltDec operator/(AltDec* self, AltDec Value) { return self->DivOp(Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator/=(AltDec& self, AltDec Value) { return self.DivOp(Value); }
        friend AltDec& operator/=(AltDec* self, AltDec Value) { return self->DivOp(Value); }

        /// <summary>
        /// /= Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator/=(AltDec& self, signed char Value) { return self.IntDivOp(Value); }
        friend AltDec operator/=(AltDec& self, signed short Value) { return self.IntDivOp(Value); }
        friend AltDec operator/=(AltDec& self, signed int Value) { return self.IntDivOp(Value); }
        friend AltDec operator/=(AltDec& self, signed __int64 Value) { return self.IntDivOp(Value); }
#if defined(AltDec_UseMirroredInt)
        friend AltDec operator/=(AltDec& self, MirroredInt Value) { return self.IntDivOp(Value); }
#endif

        /// <summary>
        /// /= Operation Between AltDec and unsigned Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator/=(AltDec& self, unsigned char Value) { return self.UnsignedIntDivOp(Value); }
        friend AltDec operator/=(AltDec& self, unsigned short Value) { return self.UnsignedIntDivOp(Value); }
        friend AltDec operator/=(AltDec& self, unsigned int Value) { return self.UnsignedIntDivOp(Value); }
        friend AltDec operator/=(AltDec& self, unsigned __int64 Value) { return self.UnsignedIntDivOp(Value); }

        /// <summary>
        /// / Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator/(AltDec self, signed char Value) { return self.IntDivOp(Value); }
        friend AltDec operator/(AltDec self, signed short Value) { return self.IntDivOp(Value); }
        friend AltDec operator/(AltDec self, signed int Value) { return self.IntDivOp(Value); }
        friend AltDec operator/(AltDec self, signed __int64 Value) { return self.IntDivOp(Value); }
#if defined(AltDec_UseMirroredInt)
        friend AltDec operator/(AltDec self, MirroredInt Value) { return self.IntDivOp(Value); }
#endif

        /// <summary>
        /// / Operation Between AltDec and unsigned Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator/(AltDec self, unsigned char Value) { return self.UnsignedIntDivOp(Value); }
        friend AltDec operator/(AltDec self, unsigned short Value) { return self.UnsignedIntDivOp(Value); }
        friend AltDec operator/(AltDec self, unsigned int Value) { return self.UnsignedIntDivOp(Value); }
        friend AltDec operator/(AltDec self, unsigned __int64 Value) { return self.UnsignedIntDivOp(Value); }

        friend AltDec operator/(AltDec self, float Value) { return self / (AltDec)Value; }
        friend AltDec operator/(AltDec self, double Value) { return self / (AltDec)Value; }
        friend AltDec operator/(AltDec self, ldouble Value) { return self / (AltDec)Value; }
        friend AltDec operator/(float Value, AltDec self) { return (AltDec)Value / self; }
        friend AltDec operator/(double Value, AltDec self) { return (AltDec)Value / self; }
        friend AltDec operator/(ldouble Value, AltDec self) { return (AltDec)Value / self; }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator*(AltDec self, AltDec Value) { return self.MultOp(Value); }
        friend AltDec operator*(AltDec* self, AltDec Value) { return self->MultOp(Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltDec</returns>
        friend AltDec& operator*=(AltDec& self, AltDec Value) { return self.MultOp(Value); }
        friend AltDec& operator*=(AltDec* self, AltDec Value) { return self->MultOp(Value); }

        /// <summary>
        /// *= Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator*=(AltDec& self, signed char Value) { return self.IntMultOp(Value); }
        friend AltDec operator*=(AltDec& self, signed short Value) { return self.IntMultOp(Value); }
        friend AltDec operator*=(AltDec& self, signed int Value) { return self.IntMultOp(Value); }
        friend AltDec operator*=(AltDec& self, signed __int64 Value) { return self.IntMultOp(Value); }
#if defined(AltDec_UseMirroredInt)
        friend AltDec operator*=(AltDec& self, MirroredInt Value) { return self.MultOp(Value); }
#endif

        friend AltDec operator*=(AltDec& self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend AltDec operator*=(AltDec& self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend AltDec operator*=(AltDec& self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend AltDec operator*=(AltDec& self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }

        friend AltDec operator*(AltDec self, signed char Value) { return self.IntMultOp(Value); }
        friend AltDec operator*(AltDec self, signed short Value) { return self.IntMultOp(Value); }
        friend AltDec operator*(AltDec self, signed int Value) { return self.IntMultOp(Value); }
        friend AltDec operator*(AltDec self, signed __int64 Value) { return self.IntMultOp(Value); }
#if defined(AltDec_UseMirroredInt)
        friend AltDec operator*(AltDec self, MirroredInt Value) { return self.IntMultOp(Value); }
#endif

        friend AltDec operator*(AltDec self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend AltDec operator*(AltDec self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend AltDec operator*(AltDec self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend AltDec operator*(AltDec self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }

        friend AltDec operator*(AltDec self, float Value) { return self * (AltDec)Value; }
        friend AltDec operator*(float Value, AltDec self) { return (AltDec)Value * self; }
        friend AltDec operator*(AltDec self, double Value) { return self * (AltDec)Value; }
        friend AltDec operator*(AltDec self, ldouble Value) { return self * (AltDec)Value; }
        friend AltDec operator*(ldouble Value, AltDec self) { return (AltDec)Value * self; }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator+(AltDec self, AltDec Value) { return self.AddOp(Value); }
        friend AltDec operator+(AltDec* self, AltDec Value) { return self->AddOp(Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator+=(AltDec& self, AltDec Value) { return self.AddOp(Value); }
        friend AltDec& operator+=(AltDec* self, AltDec Value) { return self->AddOp(Value); }

        /// <summary>
        /// += Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator+=(AltDec& self, signed char Value) { return self.IntAddOp(Value); }
        friend AltDec operator+=(AltDec& self, signed short Value) { return self.IntAddOp(Value); }
        friend AltDec operator+=(AltDec& self, signed int Value) { return self.IntAddOp(Value); }
        friend AltDec operator+=(AltDec& self, signed __int64 Value) { return self.IntAddOp(Value); }
#if defined(AltDec_UseMirroredInt)
        friend AltDec operator+=(AltDec& self, MirroredInt Value) { return self.IntAddOp(Value); }
#endif

        /// <summary>
        /// += Operation Between AltDec and unsigned Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator+=(AltDec& self, unsigned char Value) { return self.IntAddOp(Value); }
        friend AltDec operator+=(AltDec& self, unsigned short Value) { return self.IntAddOp(Value); }
        friend AltDec operator+=(AltDec& self, unsigned int Value) { return self.IntAddOp(Value); }
        friend AltDec operator+=(AltDec& self, unsigned __int64 Value) { return self.IntAddOp(Value); }

        /// <summary>
        /// + Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator+(AltDec self, signed char Value) { return self.IntAddOp(Value); }
        friend AltDec operator+(AltDec self, signed short Value) { return self.IntAddOp(Value); }
        friend AltDec operator+(AltDec self, signed int Value) { return self.IntAddOp(Value); }
        friend AltDec operator+(AltDec self, signed __int64 Value) { return self.IntAddOp(Value); }
#if defined(AltDec_UseMirroredInt)
        friend AltDec operator+(AltDec self, MirroredInt Value) { return self.IntAddOp(Value); }
#endif

        /// <summary>
        /// + Operation Between AltDec and unsigned Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator+(AltDec self, unsigned char Value) { return self.IntAddOp(Value); }
        friend AltDec operator+(AltDec self, unsigned short Value) { return self.IntAddOp(Value); }
        friend AltDec operator+(AltDec self, unsigned int Value) { return self.IntAddOp(Value); }
        friend AltDec operator+(AltDec self, unsigned __int64 Value) { return self.IntAddOp(Value); }

        friend AltDec operator+(AltDec self, float Value) { return self + (AltDec)Value; }
        friend AltDec operator+(AltDec self, double Value) { return self + (AltDec)Value; }
        friend AltDec operator+(AltDec self, ldouble Value) { return self + (AltDec)Value; }
        friend AltDec operator+(float Value, AltDec self) { return (AltDec)Value + self; }
        friend AltDec operator+(double Value, AltDec self) { return (AltDec)Value + self; }
        friend AltDec operator+(ldouble Value, AltDec self) { return (AltDec)Value + self; }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator-(AltDec self, AltDec Value) { return self.SubOp(Value); }
        friend AltDec operator-(AltDec* self, AltDec Value) { return self->SubOp(Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator-=(AltDec& self, AltDec Value) { return self.SubOp(Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator-=(AltDec* self, AltDec Value) { return self->SubOp(Value); }

        /// <summary>
        /// -= Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator-=(AltDec& self, signed char Value) { return self.IntSubOp(Value); }
        friend AltDec operator-=(AltDec& self, signed short Value) { return self.IntSubOp(Value); }
        friend AltDec operator-=(AltDec& self, signed int Value) { return self.IntSubOp(Value); }
        friend AltDec operator-=(AltDec& self, signed __int64 Value) { return self.IntSubOp(Value); }
    #if defined(AltDec_UseMirroredInt)
        friend AltDec operator-=(AltDec& self, MirroredInt Value) { return self.IntSubOp(Value); }
    #endif

        /// <summary>
        /// -= Operation Between AltDec and unsigned Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator-=(AltDec& self, unsigned char Value) { return self.IntSubOp(Value); }
        friend AltDec operator-=(AltDec& self, unsigned short Value) { return self.IntSubOp(Value); }
        friend AltDec operator-=(AltDec& self, unsigned int Value) { return self.IntSubOp(Value); }
        friend AltDec operator-=(AltDec& self, unsigned __int64 Value) { return self.IntSubOp(Value); }

        /// <summary>
        /// - Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator-(AltDec self, signed char Value) { return self.IntSubOp(Value); }
        friend AltDec operator-(AltDec self, signed short Value) { return self.IntSubOp(Value); }
        friend AltDec operator-(AltDec self, signed int Value) { return self.IntSubOp(Value); }
        friend AltDec operator-(AltDec self, signed __int64 Value) { return self.IntSubOp(Value); }
    #if defined(AltDec_UseMirroredInt)
        friend AltDec operator-(AltDec self, MirroredInt Value) { return self.IntSubOp(Value); }
    #endif

        /// <summary>
        /// - Operation Between AltDec and unsigned Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend AltDec operator-(AltDec self, unsigned char Value) { return self.IntSubOp(Value); }
        friend AltDec operator-(AltDec self, unsigned short Value) { return self.IntSubOp(Value); }
        friend AltDec operator-(AltDec self, unsigned int Value) { return self.IntSubOp(Value); }
        friend AltDec operator-(AltDec self, unsigned __int64 Value) { return self.IntSubOp(Value); }

        friend AltDec operator-(AltDec self, float Value) { return self - (AltDec)Value; }
        friend AltDec operator-(AltDec self, double Value) { return self - (AltDec)Value; }
        friend AltDec operator-(AltDec self, ldouble Value) { return self - (AltDec)Value; }
        friend AltDec operator-(float Value, AltDec self) { return (AltDec)Value - self; }
        friend AltDec operator-(double Value, AltDec self) { return (AltDec)Value - self; }
        friend AltDec operator-(ldouble Value, AltDec self) { return (AltDec)Value - self; }

    #pragma endregion Main Operator Overrides

    #pragma endregion Other Operators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>AltDec</returns>
        friend AltDec& operator-(AltDec& self)
        {
            self.SwapNegativeStatus(); return self;
        }

        /// <summary>
        /// ++AltDec Operator
        /// </summary>
        /// <returns>AltDec &</returns>
        AltDec& operator ++()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
#endif
            if (DecimalHalf == 0)
                ++IntValue;
            else if (IntValue == NegativeRep)
#if defined(AltDec_UseMirroredInt)
                IntValue = MirroredInt::Zero;
#else
                IntValue = 0;
#endif
            else
                ++IntValue;
            return *this;
        }

        /// <summary>
        /// ++AltDec Operator
        /// </summary>
        /// <returns>AltDec &</returns>
        AltDec& operator --()
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
#endif
            if (DecimalHalf == 0)
                --IntValue;
            else if (IntValue == 0)
                IntValue = NegativeRep;
            else
                --IntValue;
            return *this;
        }

        /// <summary>
        /// AltDec++ Operator
        /// </summary>
        /// <returns>AltDec</returns>
        AltDec operator ++(int)
        {
            AltDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// AltDec-- Operator
        /// </summary>
        /// <returns>AltDec</returns>
        AltDec operator --(int)
        {
            AltDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// AltDec* Operator
        /// </summary>
        /// <returns>AltDec &</returns>
        AltDec& operator *()
        {
            return *this;
        }
    #pragma endregion Other Operators

    #pragma region Modulus Operations
    #if !defined(AltNum_PreventModulusOverride)
        //Performs modulus operation based on "C = A - B * (A / B)" formula
        //Designed for use with normal, decimaled fractionals if both have same ExtraRep field, PiNum, and ENum  representation types
        //Will not work with non-decimaled format fractionals or mixed fractions
        //Modifies left side value with result
        template<typename IntType>
        AltDec BasicIntRemOp(IntType& RValue)
        {
            if (DecimalHalf == 0)
                IntValue %= RValue;
            else
            {
                if (RValue < 0)
                    RValue *= -1;
                __int64 SRep = DecimalOverflowX * IntValue.GetAbsValue() + DecimalHalf;
                __int64 divRes = SRep / RValue;
                __int64 C = SRep - RValue * divRes;
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

        AltDec BasicRemOp(signed int& RValue) { return BasicIntRemOp(RValue); }
        AltDec BasicRemOp(unsigned int& RValue) { return BasicIntRemOp(RValue); }
        AltDec BasicRemOp(signed long long& RValue) { return BasicIntRemOp(RValue); }
        AltDec BasicRemOp(unsigned long long& RValue) { return BasicIntRemOp(RValue); }

        static AltDec BasicRemOp(AltDec& self, signed int& RValue) { return self.BasicIntRemOp(RValue); }
        static AltDec BasicRemOp(AltDec& self, unsigned int& RValue) { return self.BasicIntRemOp(RValue); }
        static AltDec BasicRemOp(AltDec& self, signed long long& RValue) { return self.BasicIntRemOp(RValue); }
        static AltDec BasicRemOp(AltDec& self, unsigned long long& RValue) { return self.BasicIntRemOp(RValue); }

        AltDec BasicRem(signed int RValue) { AltDec self = *this; return BasicIntRemOp(RValue); }
        AltDec BasicRem(unsigned int RValue) { AltDec self = *this; return BasicIntRemOp(RValue); }
        AltDec BasicRem(signed long long RValue) { AltDec self = *this; return BasicIntRemOp(RValue); }
        AltDec BasicRem(unsigned long long RValue) { AltDec self = *this; return BasicIntRemOp(RValue); }

        static AltDec BasicRem(AltDec self, signed int RValue) { return self.BasicIntRemOp(RValue); }
        static AltDec BasicRem(AltDec self, unsigned int RValue) { return self.BasicIntRemOp(RValue); }
        static AltDec BasicRem(AltDec self, signed long long RValue) { return self.BasicIntRemOp(RValue); }
        static AltDec BasicRem(AltDec self, unsigned long long RValue) { return self.BasicIntRemOp(RValue); }

        //Performs modulus operation based on "C = A - B * (A / B)" formula
        template<typename IntType>
        AltDec IntRemOp(IntType& RValue)
        {
            AltDec divRes = *this / RValue;
            AltDec C = *this - RValue * divRes;
            return C;
        }

        /// <summary>
        ///  Modulus Operation
        /// </summary>
        /// <param name="RValue">The value.</param>
        AltDec RemOp(signed int& RValue) { return IntRemOp(RValue); }
        AltDec RemOp(unsigned int& RValue) { return IntRemOp(RValue); }
        AltDec RemOp(signed long long& RValue) { return IntRemOp(RValue); }
        AltDec RemOp(unsigned long long& RValue) { return IntRemOp(RValue); }

        static AltDec RemOp(AltDec& self, signed int& RValue) { return self.IntRemOp(RValue); }
        static AltDec RemOp(AltDec& self, unsigned int& RValue) { return self.IntRemOp(RValue); }
        static AltDec RemOp(AltDec& self, signed long long& RValue) { return self.IntRemOp(RValue); }
        static AltDec RemOp(AltDec& self, unsigned long long& RValue) { return self.IntRemOp(RValue); }

        AltDec RemAsCopy(signed int RValue) { AltDec self = *this; return self.IntRemOp(RValue); }
        AltDec RemAsCopy(unsigned int RValue) { AltDec self = *this; return self.IntRemOp(RValue); }
        AltDec RemAsCopy(signed long long RValue) { AltDec self = *this; return self.IntRemOp(RValue); }
        AltDec RemAsCopy(unsigned long long RValue) { AltDec self = *this; return self.IntRemOp(RValue); }

        static AltDec RemAsCopy(AltDec self, signed int RValue) { return self.IntRemOp(RValue); }
        static AltDec RemAsCopy(AltDec self, unsigned int RValue) { return self.IntRemOp(RValue); }
        static AltDec RemAsCopy(AltDec self, signed long long RValue) { return self.IntRemOp(RValue); }
        static AltDec RemAsCopy(AltDec self, unsigned long long RValue) { return self.IntRemOp(RValue); }


        //Performs modulus operation based on "C = A - B * (A / B)" formula
        //Designed for use with normal, decimaled fractionals if both have same ExtraRep field, PiNum, and ENum  representation types
        //Will not work with non-decimaled format fractionals or mixed fractions
        //Modifies left side value with result
        void BasicRemOp(AltDec& RValue)
        {
            if (RValue.DecimalHalf == 0)
            {
                if (DecimalHalf == 0)
                {
                    IntValue %= RValue.IntValue;
                }
                else
                {
                    if (RValue < 0)
                        RValue *= -1;
                    __int64 SRep = DecimalOverflowX * IntHalfAsAbs() + DecimalHalf;
                    __int64 divRes = SRep / RValue.IntValue;
                    __int64 C = SRep - (RValue.IntValue * divRes);
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
                AltDec divRes = *this / RValue;
                AltDec C = *this - RValue * divRes;
                IntValue = C.IntValue; DecimalHalf = C.DecimalHalf;
                ExtraRep = C.ExtraRep;
            }
        }

        /// <summary>
        //	Performs modulus operation based on "C = A - B * (A / B)" formula
        /// </summary>
        /// <param name="RValue">The rightside value.</param>
        /// <returns>AltDec</returns>
        AltDec RemOp(AltDec& RValue)
        {
            AltDec divRes = *this / RValue;
            AltDec C = *this - RValue * divRes;
            return C;
        }

        /// <summary>
        /// Modulus Operation (Division operation that returns the remainder result)
        /// </summary>
        /// <param name="self">The leftside value.</param>
        /// <param name="Value">The rightside value.</param>
        /// <returns>AltDec&</returns>
        static AltDec RemOp(AltDec& self, AltDec& Value) { return self.RemOp(Value); }

        void CatchAllRem(AltDec& Value, RepType& LRep, RepType& RRep)
        {
            ConvertToNormType(LRep);
            Value.ConvertToNormType(RRep);
            BasicRemOp(Value);
        }

        void CatchAllRem(AltDec& Value, RepType& SameRep)
        {
            ConvertToNormType(SameRep);
            Value.ConvertToNormType(SameRep);
            BasicRemOp(Value);
        }

        void CatchAllRem(AltDec& Value)
        {
            ConvertToNormTypeV2();
            Value.ConvertToNormTypeV2();
            BasicRemOp(Value);
        }

        friend AltDec operator%(AltDec self, AltDec Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec& self, AltDec Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec self, int Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec self, unsigned int Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec self, signed long long Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec self, unsigned __int64 Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec& self, int Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec& self, unsigned int Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec& self, signed long long Value) { return RemOp(self, Value); }
        friend AltDec operator%(AltDec& self, unsigned __int64 Value) { return RemOp(self, Value); }

        #if defined(AltNum_EnableAlternativeModulusResult)//Return AltNumModChecker<AltDec> Result with both Remainder and division result
        template<typename IntType>
        static AltNumModChecker<AltDec> IntModulusOp(IntType& RValue)
        {
            AltNumModChecker<AltDec> Res;
            Res.CalcIfZero(*this, AltNum(RValue));
            return Res;
        }

        /// <summary>
        ///  Modulus Operation
        /// </summary>
        /// <param name="RValue">The value.</param>
        AltNumModChecker<AltDec> ModulusOp(signed int& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<AltDec> ModulusOp(unsigned int& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<AltDec> ModulusOp(signed long long& RValue) { IntModulusOp(RValue); }
        AltNumModChecker<AltDec> ModulusOp(unsigned long long& RValue) { IntModulusOp(RValue); }

        static AltNumModChecker<AltDec> ModulusOp(AltDec& self, signed int& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<AltDec> ModulusOp(AltDec& self, unsigned int& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<AltDec> ModulusOp(AltDec& self, signed long long& RValue) { self.IntModulusOp(RValue); }
        static AltNumModChecker<AltDec> ModulusOp(AltDec& self, unsigned long long& RValue) { self.IntModulusOp(RValue); }


        static AltNumModChecker<AltDec> ModulusOp(AltDec& RValue)
        {
            AltNumModChecker<AltDec> Res;
            Res.CalcIfZero(*this, RValue);
            return Res;
        }

        static AltNumModChecker<AltDec> ModulusOp(AltDec& self, AltDec& RValue) { self.ModulusOp(RValue); }
        #endif

    #endif
    #pragma endregion Modulus Operations

    #pragma region Bitwise Functions
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend AltDec operator^(AltDec self, IntType Value)
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
        /// Bitwise Or Operation Between AltDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend AltDec operator|(AltDec self, IntType Value)
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
        /// <returns>AltDec&</returns>
        AltDec& Abs()
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
        /// <returns>AltDec</returns>
        static AltDec Abs(AltDec tValue)
        {
            return tValue.Abs();
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>AltDec&</returns>
        AltDec& Floor()
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
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>AltDec&</returns>
        static AltDec Floor(AltDec Value)
        {
            return Value.Floor();
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        static AltDec Floor(AltDec Value, int precision)
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
        /// <returns>AltDec&</returns>
        AltDec& Ceil()
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
                //ConvertToNormTypeV2();//Prevent losing imaginery numbers
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

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>AltDec&</returns>
        static int FloorInt(AltDec Value)
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
        /// <returns>AltDec&</returns>
        static int CeilInt(AltDec Value)
        {
            RepType repType = Value.GetRepType();
            switch (repType)
            {
				case RepType::NormalType:
					break;
				case RepType::PiNum:
				{
					Value.ConvertPiToNum();//return CeilInt(Value.ConvertPiToNum());
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
					Value.ConvertToNormType(repType);
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
					Value.ConvertToNormalIRep();
					break;
		#endif
	#endif
				//case RepType::ApproachingTop:
				//case RepType::ApproachingBottom:
				default:
					Value.ConvertToNormType(repType);
					break;
            }
            if (Value.DecimalHalf == 0)
            {
                return Value.GetIntHalf();
            }
            if (Value.IntValue == NegativeRep) { return 0; }
            else
            {
                return Value.GetIntHalf() + 1;
            }
        }
        
        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>AltDec</returns>
        static AltDec Ceil(AltDec Value)
        {
            return Value.Ceil();
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>AltDec &</returns>
        AltDec& Trunc()
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
        /// <returns>AltDec</returns>
        static AltDec Trunc(AltDec Value)
        {
            return Value.Trunc();
        }
	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions	
        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static AltDec BasicSqrt(AltDec& value, int precision=7)
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
            AltDec number = value;
            AltDec start = 0, end = number;
            AltDec mid;

            // variable to store the answer 
            AltDec ans;

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
            AltDec increment = "0.1";
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
		static AltDec Sqrt(AltDec value, int precision=7)
		{
		    value.ConvertToNormTypeV2();
			BasicSqrt(value, precision);
		}

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        AltDec BasicIntPowOp(ValueType expValue)
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
                    AltDec self = *this;
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
                AltDec self = *this;
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

        template<typename ValueType>
        AltDec BasicUnsignedIntPowOp(ValueType expValue)
        {
            if (expValue == 1) { return *this; }//Return self
            else if (expValue == 0)
            {
                IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
            }
            else if (DecimalHalf == 0 && IntValue == 10 && ExtraRep == 0)
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            else if (DecimalHalf == 0 && IntValue == -10 && ExtraRep == 0)
                IntValue = expValue % 2 ? VariableConversionFunctions::PowerOfTens[expValue] : VariableConversionFunctions::PowerOfTens[expValue] * -1;
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                AltDec self = *this;
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

        AltDec BasicInt32PowOp(int& expValue) { return BasicIntPowOp(expValue); }
        AltDec BasicInt64PowOp(signed long long& expValue) { return BasicIntPowOp(expValue); }
        AltDec BasicInt32Pow(int expValue) { return BasicIntPowOp(expValue); }
        AltDec BasicInt64Pow(signed long long expValue) { return BasicIntPowOp(expValue); }
        AltDec BasicUnsignedInt32PowOp(unsigned int& expValue) { return BasicUnsignedIntPowOp(expValue); }
        AltDec BasicUnsignedInt64PowOp(unsigned long long& expValue) { return BasicUnsignedIntPowOp(expValue); }
        AltDec BasicUnsignedInt32Pow(unsigned int expValue) { return BasicUnsignedIntPowOp(expValue); }
        AltDec BasicUnsignedInt64Pow(unsigned long long expValue) { return BasicUnsignedIntPowOp(expValue); }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        AltDec IntPowOp(ValueType expValue)
        {
            if (DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    tSetAsUndefined();
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

        template<typename ValueType>
        AltDec UnsignedIntPowOp(ValueType expValue)
        {
            if (DecimalHalf == InfinityRep)
            {
                if (expValue == 0)
#if defined(AltNum_EnableNaN)
                    tSetAsUndefined();
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
                return BasicUnsignedIntPowOp(expValue);
        }

        AltDec Int32PowOp(int& expValue) { return IntPowOp(expValue); }
        AltDec Int64PowOp(signed long long& expValue) { return IntPowOp(expValue); }
        AltDec Int32Pow(int expValue) { return IntPowOp(expValue); }
        AltDec Int64Pow(signed long long expValue) { return IntPowOp(expValue); }
        AltDec UnsignedInt32PowOp(unsigned int& expValue) { return UnsignedIntPowOp(expValue); }
        AltDec UnsignedInt64PowOp(unsigned long long& expValue) { return UnsignedIntPowOp(expValue); }
        AltDec UnsignedInt32Pow(unsigned int expValue) { return UnsignedIntPowOp(expValue); }
        AltDec UnsignedInt64Pow(unsigned long long expValue) { return UnsignedIntPowOp(expValue); }

        AltDec BasicInt32PowConstOp(const int& expValue) { return BasicIntPowOp(expValue); }
        AltDec BasicInt64PowConstOp(const signed long long& expValue) { return BasicIntPowOp(expValue); }
        AltDec BasicInt32PowConst(const int expValue) { return BasicIntPowOp(expValue); }
        AltDec BasicInt64PowConst(const signed long long expValue) { return BasicIntPowOp(expValue); }
        AltDec BasicUnsignedInt32PowConstOp(const unsigned int& expValue) { return BasicUnsignedIntPowOp(expValue); }
        AltDec BasicUnsignedInt64PowConstOp(const unsigned long long& expValue) { return BasicUnsignedIntPowOp(expValue); }
        AltDec BasicUnsignedInt32PowConst(const unsigned int expValue) { return BasicUnsignedIntPowOp(expValue); }
        AltDec BasicUnsignedInt64PowConst(const unsigned long long expValue) { return BasicUnsignedIntPowOp(expValue); }

        AltDec Int32PowConstOp(const int& expValue) { return IntPowOp(expValue); }
        AltDec Int64PowConstOp(const long long& expValue) { return IntPowOp(expValue); }
        AltDec Int32PowConst(const int expValue) { return IntPowOp(expValue); }
        AltDec Int64PowConst(const long long expValue) { return IntPowOp(expValue); }
        AltDec UnsignedInt32PowConstOp(const int& expValue) { return UnsignedIntPowOp(expValue); }
        AltDec UnsignedInt64PowConstOp(const long long& expValue) { return UnsignedIntPowOp(expValue); }
        AltDec UnsignedInt32PowConst(const int expValue) { return UnsignedIntPowOp(expValue); }
        AltDec UnsignedInt64PowConst(const long long expValue) { return UnsignedIntPowOp(expValue); }


        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="nValue">The nth value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>AltDec</returns>
        static AltDec NthRoot(AltDec value, int n, AltDec precision = AltDec::JustAboveZero)
        {
            //Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            AltDec xPre = One;
            xPre += (value-1)/n;
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            AltDec delX = AltDec(2147483647, 0);

            //  xK denotes current value of x 
            AltDec xK;
            AltDec xPrePower;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = xPre.BasicIntMult(nMinus1);
                xPrePower = xPre.Int32Pow(nMinus1);
                xK += value/xPrePower;
                xK.BasicIntDivOp(n);
                delX = AltDec::Abs(xK - xPre);
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
        AltDec FractionalPow(int expNum, int expDenom);

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        AltDec FractionalPow(boost::rational<int>& Frac);

        AltDec BasicPowOp(AltDec& expValue);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        AltDec PowOp(AltDec& expValue);

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        AltDec Pow(AltDec expValue) { return PowOp(expValue); }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="targetValue">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static AltDec PowV2(AltDec targetValue, AltDec expValue) { return targetValue.PowOp(expValue); }
	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static AltDec NthRootV2(AltDec targetValue, int n, AltDec& Precision = AltDec::FiveBillionth)
        {
            int nMinus1 = n - 1;
            AltDec x[2] = { (AltDec::One / n) * ((targetValue*nMinus1) + (targetValue / targetValue.Int32Pow(nMinus1))), targetValue };
            while (AltDec::Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (AltDec::One / n) * ((x[1]*nMinus1) + (targetValue / x[1].Int32Pow(nMinus1)));
            }
            return x[0];
        }

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>AltDec</returns>
        static AltDec Exp(AltDec& x)
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
             *      - x: (AltDec float) power of e to evaluate
             * Returns:
             *      - (AltDec float) approximation of e^x in AltDec precision
             */
             // Check that x is a valid input.
            assert(-709 <= x.IntValue && x.IntValue <= 709);
            // When x = 0 we already know e^x = 1.
            if (x.IsZero()) {
                return AltDec::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            AltDec x0 = AltDec::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            AltDec tn = AltDec::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = AltDec::CeilInt(x0 * AltDec::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + AltDec::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = AltDec::One / tn;
            }
            return tn;
        }
protected:
    static AltDec LnRef_Part02(AltDec& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
        AltDec TotalRes = (value - 1) / (value + 1);
        AltDec LastPow = TotalRes;
        AltDec WSquared = TotalRes * TotalRes;
        AltDec AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > AltDec::JustAboveZero);
        return TotalRes * 2;
    }
public:
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::AltDec</returns>
        static AltDec LnRef(AltDec& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == AltDec::One)
                return AltDec::Zero;
            RepType repType = value.GetRepType();
            AltDec ConvertedVal;
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
                AltDec threshold = AltDec::FiveMillionth;
                AltDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                AltDec term = base;       // First term
                AltDec prev;          // Previous sum
                AltDec result = term;     // Kick it off

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
                } while (AltDec::Abs(prev - result) > threshold);

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
        /// <returns>BlazesRusCode::AltDec</returns>
        static AltDec LnRefV2(AltDec& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == AltDec::One)
                return AltDec::Zero;
            RepType repType = value.GetRepType();
            AltDec ConvertedVal;
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
                AltDec W = (value - 1)/ (value + 1);
                AltDec TotalRes = W;
                W.SwapNegativeStatus();
                AltDec LastPow = W;
                AltDec WSquared = W * W;
                int WPow = 3;
                AltDec AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > AltDec::JustAboveZero);
                return TotalRes * 2;
            }
            else if (ConvertedVal.IntValue==1)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                AltDec threshold = AltDec::FiveMillionth;
                AltDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                AltDec term = base;       // First term
                AltDec prev;          // Previous sum
                AltDec result = term;     // Kick it off

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
                } while (AltDec::Abs(prev - result) > threshold);

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
        static AltDec Ln(AltDec value)
        {
            return LnRef(value);
        }

protected:
    static AltDec Log10_Part02(AltDec& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        AltDec TotalRes = (value - 1) / (value + 1);
        AltDec LastPow = TotalRes;
        AltDec WSquared = TotalRes * TotalRes;
        AltDec AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > AltDec::JustAboveZero);
        return TotalRes * AltDec::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        static AltDec Log10(AltDec value)
        {
            if (value == AltDec::One)
                return AltDec::Zero;
            RepType repType = value.GetRepType();
            AltDec ConvertedVal;
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
                        return AltDec(index, 0);
                }
                return AltDec(9, 0);
            }
            if (ConvertedVal.IntValue>=0&&ConvertedVal.IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                AltDec threshold = AltDec::FiveBillionth;
                AltDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                AltDec term = base;       // First term
                AltDec prev = 0;          // Previous sum
                AltDec result = term;     // Kick it off

                while (AltDec::Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return result*AltDec::LN10Mult;// result/AltDec::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return Log10_Part02(value);
            }
        }

protected:
    template<typename ValueType>
    static AltDec Log10_IntPart02(ValueType& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        AltDec TotalRes = AltDec((value - 1), 0) / AltDec((value + 1), 0);
        AltDec LastPow = TotalRes;
        AltDec WSquared = TotalRes * TotalRes;
        AltDec AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > AltDec::JustAboveZero);
        return TotalRes * AltDec::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename ValueType>
        static AltDec Log10(ValueType value)
        {
            if (value == 1)
                return AltDec::Zero;
            if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return AltDec(index, 0);
                }
                return AltDec(9, 0);
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
        /// <returns>AltDec</returns>
        static AltDec Log(AltDec value, AltDec baseVal)
        {
            RepType repType = value.GetRepType();
            AltDec ConvertedVal;
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
            if (ConvertedVal == AltDec::One)
                return AltDec::Zero;
            return Log10(value) / Log10(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>AltDec</returns>
        static AltDec Log(AltDec value, int baseVal)
        {
            RepType repType = value.GetRepType();
            AltDec ConvertedVal;
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
            if (ConvertedVal == AltDec::One)
                return AltDec::Zero;
            //Calculate Base log first
            AltDec baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = AltDec(index, 0);
                        break;
                    }
                }
                baseTotalRes = AltDec(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = AltDec((baseVal - 1), 0) / AltDec((baseVal + 1), 0);
                AltDec baseLastPow = baseTotalRes;
                AltDec baseWSquared = baseTotalRes * baseTotalRes;
                AltDec baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > AltDec::JustAboveZero);
            }

            //Now calculate other log
            if (ConvertedVal.DecimalHalf == 0 && ConvertedVal.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? AltDec(index, 0) / (baseTotalRes * AltDec::HalfLN10Mult): AltDec(index, 0)/ baseTotalRes;
                }
                return lnMultLog? AltDec(9, 0) / (baseTotalRes*AltDec::HalfLN10Mult):AltDec(9, 0)/baseTotalRes;
            }
            if (ConvertedVal.IntValue>=0&&ConvertedVal.IntValue<2)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                AltDec threshold = AltDec::FiveBillionth;
                AltDec base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                AltDec term = base;       // First term
                AltDec prev = 0;          // Previous sum
                AltDec result = term;     // Kick it off

                while (AltDec::Abs(prev - result) > threshold) {
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
                AltDec W = (value - 1) / (value + 1);
                AltDec TotalRes = W;
                AltDec AddRes;
                int WPow = 3;
                do
                {
                    AddRes = W.Int32Pow(WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > AltDec::JustAboveZero);
                return lnMultLog? TotalRes/baseTotalRes:(TotalRes * AltDec::HalfLN10Mult)/ baseTotalRes;
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
        /// <returns>AltDec</returns>
        static AltDec SinFromAngle(AltDec Value)
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
            if (Value == Zero) { return AltDec::Zero; }
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
                return AltDec::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else
            {
                AltDec NewValue = Zero;
                //Angle as Radian
                AltDec Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's Taylor series!!
                    NewValue += Radius.Pow(2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                }//x[1].Int32Pow(nMinus1)
                return NewValue;
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns></returns>
        static AltDec CosFromAngle(AltDec Value)
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
                return AltDec::Zero;
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return AltDec::Zero;
            }
            else
            {
                AltDec NewValue = Zero;
                //Angle as Radian
                AltDec Radius = Pi * Value / 180;
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
        /// <returns>AltDec</returns>
        static AltDec BasicSinOperation(AltDec& Value)
        {
            AltDec SinValue = Zero;
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
        /// <returns>AltDec</returns>
        static AltDec Sin(AltDec Value)
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
                    AltDec SinValue = Zero;
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
                    AltDec SinValue = Zero;
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
        /// <returns>AltDec</returns>
        static AltDec BasicCosOperation(AltDec& Value)
        {
            AltDec CosValue = Zero;
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
        /// <returns>AltDec</returns>
        static AltDec Cos(AltDec Value)
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
        /// <returns>AltDec</returns>
        static AltDec Tan(AltDec Value)
        {
            AltDec SinValue = Zero;
            AltDec CosValue = Zero;
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
        /// <returns>AltDec</returns>
        static AltDec TanFromAngle(AltDec Value)
        {
            RepType repType = Value.GetRepType();
            switch (repType)
            {
#if defined(AltNum_EnableNaN)
            case RepType::NaN:
                return NaNValue();
#endif
#if defined(AltNum_EnableImaginaryNum) && defined(AltNum_EnableFractionals)
            {
                int Divisor = Value.ExtraRep*-1;
                Value.ExtraRep = 0;
                Value /= Divisor;
                Value.ExtraRep = -2147483647;
                break;
            }
#endif
#if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
#endif
#if defined(AltNum_EnableERep)
            case RepType::ENum:
#if defined(AltNum_EnableFractionals)
            case RepType::ENumByDiv:
#endif
#endif
                Value.ConvertToNormTypeV2();
                break;
            //case RepType::INum:
            default:
                Value.ConvertToNormTypeV2();//Don't convert things like imaginary numbers into real numbers
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
            if (Value == Zero) { return AltDec::Zero; }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                return Infinity;
#else
                return AltDec::Maximum;//Positive Infinity
#endif
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return AltDec::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                return AltDec::NegativeInfinity;
#else
                return AltDec::Minimum;//Negative Infinity
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
        /// <returns>AltDec</returns>
        static AltDec ATan(AltDec Value)
        {
            AltDec SinValue = Zero;
            AltDec CosValue = Zero;
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
        /// <returns>AltDec</returns>
        static AltDec ArcTan2(AltDec y, AltDec x)
        {
#if defined(AltNum_EnablePiRep)
    #if defined(AltNum_EnableDecimaledPiFractionals)
            AltDec coeff_1 = AltDec(1, 0, -4);
    #elif defined(AltNum_EnablePiFractional)
            AltDec coeff_1 = AltDec(1, 4, PiByDivisorRep);
    #else
            AltDec coeff_1 = AltDec(0, 250000000, PiRep);//Pi / 4;
    #endif
#else
            AltDec coeff_1 = PiNum / 4;
#endif
#if defined(AltNum_EnablePiRep)
    #if defined(AltNum_EnableDecimaledPiFractionals)
            AltDec coeff_2 = AltDec(3, 0, -4);
    #elif defined(AltNum_EnablePiFractional)
            AltDec coeff_2 = AltDec(3, 4, PiByDivisorRep);
    #else
            AltDec coeff_2 = AltDec(0, 750000000, PiRep);//Pi / 4;
    #endif
#else
            AltDec coeff_2 = PiNum * AltDec(0, 750000000);
#endif
            AltDec abs_y = AltDec::Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            AltDec r;
            AltDec angle;
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

    #undef MediumDecVariant
    #define MediumDecVariant AltDec

#if defined(AltDec_UseMirroredInt)
	MirroredInt AltDec::NegativeRep = MirroredInt::NegativeZero;
#endif
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproachingValues)
    AltDec AltDec::AlmostOne = ApproachingRightRealValue();
#endif
    AltDec AltDec::Pi = PiValue();
    AltDec AltDec::One = OneValue();
    AltDec AltDec::Two = TwoValue();
    AltDec AltDec::NegativeOne = NegativeOneValue();
    AltDec AltDec::Zero = ZeroValue();
    AltDec AltDec::PointFive = Point5Value();
    AltDec AltDec::JustAboveZero = JustAboveZeroValue();
    AltDec AltDec::OneMillionth = OneMillionthValue();
    AltDec AltDec::FiveThousandth = FiveThousandthValue();
    AltDec AltDec::Minimum = MinimumValue();
    AltDec AltDec::Maximum = MaximumValue();
    AltDec AltDec::E = EValue();
    AltDec AltDec::LN10 = LN10Value();
    AltDec AltDec::LN10Mult = LN10MultValue();
    AltDec AltDec::HalfLN10Mult = HalfLN10MultValue();
    AltDec AltDec::TenMillionth = TenMillionthValue();
    AltDec AltDec::FiveMillionth = FiveMillionthValue();
    AltDec AltDec::FiveBillionth = FiveBillionthValue();
    AltDec AltDec::OneGMillionth = OneHundredMillionthValue();
    #if defined(AltNum_EnableNilRep)
    AltDec AltDec::Nil = NilValue();
    #endif

    AltDec AltDec::PiNum = PiNumValue();
    AltDec AltDec::ENum = ENumValue();
    
#if defined(AltNum_EnableInfinityRep)
    AltDec AltDec::Infinity = InfinityValue();
#endif
#if defined(AltNum_EnableApproachingValues)
    AltDec AltDec::NegativeInfinity = NegativeInfinityValue();
    AltDec AltDec::ApproachingZero = ApproachingZeroValue();
#endif
#if defined(AltNum_EnableNaN)
    AltDec AltDec::NaN = NaNValue();
	AltDec AltDec::Undefined = UndefinedValue();
#endif
    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The target value to convert into AltDec</param>
    inline void AltDec::ReadString(std::string Value)
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
                IntHalfAdditionOp(TempInt02);
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
    /// <returns>AltDec</returns>
    inline AltDec AltDec::GetValueFromString(std::string Value)
    {
        AltDec NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

	std::string AltDec::BasicToStringOp()
	{
        std::string Value = IntHalfAsString();
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

    std::string AltDec::ToString()
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
            return IntHalfAsString() + ".0..1";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp();
			#else
            return IntHalfAsString() + ".9..9";
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
            return IntHalfAsString() +"/"
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
            return IntHalfAsString() +"/"
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
            return IntHalfAsString() +"/"
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
            return IntHalfAsString() + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"e";
			#else
            return IntHalfAsString() + ".9..9e";
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
            return IntHalfAsString() + ".0..1i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"i";
			#else
            return IntHalfAsString() + ".9..9i";
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

	std::string AltDec::BasicToFullStringOp()
	{
        std::string Value = IntHalfAsString();
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

    std::string AltDec::ToFullString()
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
            return IntHalfAsString() + ".0..1";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp();
			#else
            return IntHalfAsString() + ".9..9";
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
            return IntHalfAsString() +"/"
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
            return IntHalfAsString() + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"e";
			#else
            return IntHalfAsString() + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::PositiveImaginaryInfinity:
            return "∞";
            break;
        case RepType::NegativeImaginaryInfinity:
            return "-∞";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToFullStringOp()+"i";
			#else
            return IntHalfAsString() + ".0..1i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToFullStringOp()+"i";
			#else
            return IntHalfAsString() + ".9..9i";
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
            return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
		#elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
		#elif defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
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
}