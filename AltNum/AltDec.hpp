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

#ifdef AltNum_EnableMediumDecBasedSetValues
    #include "MediumDec.hpp"
#endif

#include "AltNumModChecker.hpp"

#include "MirroredInt.hpp"
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
	(Assumes AltNum_EnableInfinityRep is enabled)
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

AltNum_UseMediumDecBasedRepresentations =
      Forces to calculate certain representations like MediumDec does 
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
      If AltNum_UseMediumDecBasedRepresentations enabled, then
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
(Might be better to just store as formula class feature or as another number class holding 2 MediumDec or other AltNum values)
      (Requires AltNum_EnableImaginaryNum, Not Implimented)

AltNum_EnableMixedFractional =
      If DecimalHalf is negative and ExtraRep is Positive,
      then AltDec represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
      (Not Fully Implimented)

AltNum_EnableERep =
      If AltNum_UseMediumDecBasedRepresentations enabled, then
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

AltNum_EnableUndefinedButInRange = Enable representation of unknown number between -Value and +Value for Cos operation

AltNum_DisableSwitchBasedConversion =

AltNum_EnableMediumDecBasedSetValues =
	  
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

AltNum_UseOldDivisionCode
AltNum_AvoidUsingLargeInt = Removes AltNum_UseOldDivisionCode toggle and forces alternative code that doesn't need int128 from boost
AltNum_UseOldRemOpCode

AltNum_EnableBoostFractionalReduction
AltNum_EnableImaginaryInfinity = Enables imaginary infinity option
AltNum_DisableApproachingTop = Disables IntValue.9..9 representation and .5..1 etc

Only one of 3 alternative mixed fraction representations can be enabled at a time(use FlaggedDec variant if need all at once):
AltNum_EnableMixedPiFractional
AltNum_EnableMixedEFractional
AltNum_EnableMixedIFractional

AltNum_EnableNilRep = Enables Nil representation(detection not in code right now)
*/
#if !defined(AltNum_DisableAutoToggleOfPreferedSettings)
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
    #define AltNum_EnableFractionals
#endif

#if defined(AltNum_AvoidUsingLargeInt)
    #undef AltNum_UseOldDivisionCode
#endif

//Force required flags to be enabled if AltNum_EnableApproachingDivided toggled
#if defined(AltNum_EnableApproachingDivided)
    #define AltNum_EnableInfinityRep
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

namespace BlazesRusCode
{
    class AltDec;
/* Other Varient of AltDec = ExtendedAltDec
DecimalHalf and ExtraRep instead as unsigned ints
ExtraFlags variable as a byte/char variable for determining extra variable states such as infinity and Imaginary Numbers
ExtraFlags treated as bitwise flag storage
(13 bytes worth of Variable Storage inside class for each instance)
*/

/*---Accuracy Tests(with MediumDec based settings):
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
    #undefine MediumDecVariant
    #define MediumDecVariant AltDec
	public:
		class ModRes
		{
			//Division Result
			MediumDecVariant DivRes;
			//Modulo Operation Result
			MediumDecVariant RemRes;
		}
    public:
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;
		
	private:
        /// <summary>
        /// The decimal overflow value * -1
        /// </summary>
		static signed _int64 const NegDecimalOverflowX = -1000000000;
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;
    public:
        /// <summary>
        /// Value when IntValue is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
        static signed int const NegativeRep = MirroredInt::NegativeRep;

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        MirroredInt IntValue;

        signed int GetIntHalf()
        {
            return IntValue.GetValue();
        }

        bool int IsNegative()
        {
            return IntValue.IsNegative();
        }

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        signed int DecimalHalf;

        /// <summary>
		/// (Used exclusively for alternative represents of numbers including imaginery numbers and for fractionals)
        /// If both DecimalHalf&ExtraRep are Positive with ExtraRep as non-zero, then ExtraRep acts as denominator
        /// If DecimalHalf is negative and ExtraRep is Positive, then MediumDecVariant represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
        /// If ExtraRep is zero and DecimalHalf is positive, then MediumDecVariant represents +- 2147483647.999999999
        ///-----------------------------------------------
        /// If ExtraRep is negative, it acts as representation type similar to MediumDecVariant:
		/// If ExtraRep is between 0 and , it acts as representation type similar to MediumDecVariant:
        /// If DecimalHalf is positive and ExtraRep is -2147483648 and AltNum_EnablePiRep is enabled, then MediumDecVariant represents +- 2147483647.999999999 * Pi
        /// </summary>
        signed int ExtraRep;

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        /// <param name="extraVal">ExtraRep flags etc</param>
        MediumDecVariant(signed int intVal = 0, signed int decVal = 0, signed int extraVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

		bool IsZero()
		{
            return DecimalHalf==0&&IntValue.Value==0;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MediumDecVariant Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf; ExtraRep = Value.ExtraRep;
        }

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0; ExtraRep = 0;
        }

    #pragma region Const Representation values
    private:
	#if defined(AltNum_EnableInfinityRep)
        //Is Infinity Representation when DecimalHalf==-2147483648 (IntValue==1 for positive infinity;IntValue==-1 for negative Infinity)
		//(other values only used if AltNum_EnableInfinityPowers is enabled)
		//If AltNum_EnableImaginaryInfinity is enabled and ExtraRep = IRep, then represents either negative or positive imaginary infinity
        static const signed int InfinityRep = -2147483648;
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
		#endif
		//Is Approaching Bottom i when DecimalHalf==-2147483645:
		//If ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0__1)i
        static const signed int ApproachingImaginaryBottomRep = -2147483645;
		//Is Approaching Top i when DecimalHalf==-2147483644:
		//If ExtraRep==0, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)i
		static const signed int ApproachingImaginaryTopRep = -2147483644;
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

    #if defined(AltNum_EnableNilRep)
        //When both IntValue and DecimalHalf equal -2147483648 it is Nil
        static signed int const NilRep = -2147483648;
    #endif

    #pragma endregion Const Representation values

	#if !defined(AltNum_EnablePrivateRepType)
    public:
	#endif

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
			#endif
            PiFractional,//  IntValue/DecimalHalf*Pi Representation
		#endif
	#endif
	#if defined(AltNum_EnableERep)
            ENum,
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledEFractionals)
            ENumByDiv,//(Value/(ExtraRep*-1))*e Representation
			#endif
            EFractional,//  IntValue/DecimalHalf*e Representation
		#endif
	#endif
	#if defined(AltNum_EnableImaginaryNum)
            INum,
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            INumByDiv,//(Value/(ExtraRep*-1))*i Representation
			#endif
            IFractional,//  IntValue/DecimalHalf*i Representation
		#endif
		#ifdef AltNum_EnableComplexNumbers
            ComplexIRep,
		#endif
	#endif
	#if defined(AltNum_EnableMixedFractional)
            MixedFrac,//IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnableMixedPiFractional)
            MixedPi,//IntValue +- (DecimalHalf/-ExtraRep)
		#endif
		#if defined(AltNum_EnableMixedEFractional)
            MixedE,//IntValue +- (DecimalHalf/-ExtraRep)
		#endif
		#if defined(AltNum_EnableMixedIFractional)
            MixedI,//IntValue +- (DecimalHalf/-ExtraRep)
		#endif
	#endif

	#if defined(AltNum_EnableInfinityRep)
			PositiveInfinity,//If Positive Infinity, then convert number into MaximumValue instead when need as real number
			NegativeInfinity,//If Negative Infinity, then convert number into MinimumValue instead when need as real number
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
    #endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
            UndefinedButInRange,
	#endif
    #if defined(AltNum_EnableNilRep)
            Nil,
    #endif
            UnknownType
        };

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        RepType GetRepType()
        {
	#if defined(AltNum_EnableInfinityRep)
		#if !defined(AltNum_DisableInfinityRepTypeReturn)
            if(DecimalHalf==InfinityRep)
            {
			#if defined(AltNum_EnableImaginaryInfinity)
                if (ExtraRep == IRep)
				    return IntValue==1?RepType::PositiveImaginaryInfinity:RepType::NegativeImaginaryInfinity;
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
	#ifdef AltNum_EnablePiRep
            else if(ExtraRep==PiRep)
                return RepType::PiNum;
            else if(ExtraRep==PiByDivisorRep)
				return RepType::PiIntNumByDiv;
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
            else if(ExtraRep==EByDivisorRep)//(IntValue/DecimalHalf)*e
				return RepType::ENumByDiv;
    #endif

	#if defined(AltNum_EnableImaginaryNum)
            else if(ExtraRep==IRep)
			{
				return RepType::INum;
			}
		#if defined(AltNum_EnableAlternativeRepFractionals)
            else if(ExtraRep==IByDivisorRep)
					return RepType::INumByDiv;
		#endif
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
           else if(ExtraRep==UndefinedButInRange)
                return RepType::UndefinedButInRange;
	#endif
            else if(ExtraRep<0)
	#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)||defined(AltNum_EnableMixedIFractional)
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

	#if defined(AltNum_EnablePrivateRepType)
    public:
	#endif

	#if defined(AltNum_EnablePiRep)
        void ConvertPiToNum()
        {

            ExtraRep = 0;
            // Can only convert to up 683565275.1688666254437963172038917047964296646843381624484789109135725652864987887127902610635528943x PiRepresentation
            //Can Represent up ? before hitting Maximum MediumDecVariant value on reconversion when AltNum_UseLowerPrecisionPi is enabled
            //otherwise can represent up to ???(when adding up value from each decimal place of IntValue + (PiNum*DecimalHalf/1000000000))
            //Calculations from HiPer Calc
            //683565275.168866625 x 3.141592654 = 2147483646.99999999860577275
            //683565275.168866626 x 3.141592654 = 2147483647.000000001747365404
            if(IntValue>=683565275&&DecimalHalf>=168866626)//Exceeding Storage limit of NormalRep
            {
                throw "Conversion of Pi multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
                IntValue = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
            }
            else if(IntValue<=-683565275&&DecimalHalf>=168866626)//Exceeding Storage limit of NormalRep
            {
                throw "Conversion of Pi multiplication into MediumDec format resulted in underflow(setting value to minimum MediumDec value)";
                IntValue = -2147483647; DecimalHalf = 999999999;//set value as minimum value(since not truely infinite just bit above storage range)
            }
		#if defined(AltNum_DisableSwitchBasedConversion)
            else if (IntValue == NegativeRep)//-0.XXX... * Pi
            {
                BasicMultOp(PiNum);
            }
            else if (DecimalHalf == 0 && IntValue == 10)
            {
                IntValue = 31; DecimalHalf = 415926536; 
            }
            else
            {
                BasicMultOp(PiNum);
            }
		#else
            else
            {
                switch (IntValue)
                {
                case NegativeRep:
                    BasicMultOp(PiNum);
                    break;
                case 5:
                    if (DecimalHalf == 0)
                    {
                        IntValue = 15; DecimalHalf = 707963268;
                    }
                    else
                        BasicMultOp(PiNum);
                    break;
                case 10:
                    if (DecimalHalf == 0)
                    {
                        IntValue = 31; DecimalHalf = 415926536;
                    }
                    else
                        BasicMultOp(PiNum);
                    break;
                //3.1415926535897932384626433
                case 100:
                    if (DecimalHalf == 0)
                    {
                        IntValue = 314; DecimalHalf = 159265359;
                    }
                    else
                        BasicMultOp(PiNum);
                    break;
                case 1000:
                    if (DecimalHalf == 0)
                    {
                        IntValue = 3141; DecimalHalf = 592653590;
                    }
                    else
                        BasicMultOp(PiNum);
                    break;
                case -10:
                    if (DecimalHalf == 0)
                    {
                        IntValue = -31; DecimalHalf = 415926536;
                    }
                    else
                        BasicMultOp(PiNum);
                    break;
                default:
                    BasicMultOp(PiNum);
                    break;
                }
            }
		#endif
        }
		
        void ConvertPiPowerToNum()
        {
			int powerExponent = -self.ExtraRep;
			ExtraRep = 0;
			MediumDecVariant PiSide = PiNumValue();
			PiSide.PowOp(powerExponent);
			BasicMultOp(PiSide);
		}
		
		MediumDecVariant PiPowerNum(int powerExponent)
        {
			ExtraRep = 0;
			MediumDecVariant PiSide = PiNumValue();
			PiSide.PowOp(powerExponent);
			return PiSide;
		}
		
		void ConvertPiPowerToPiRep()
		{
			int powerExponent = -self.ExtraRep;
			if(powerExponent==0)
				self.ExtraRep = 0;//Pi^0 = 1
			else
			{
				self.ExtraRep = PiRep;
				if(powerExponent!=1)
				{
					MediumDecVariant PiSide = PiNumValue();
					PiSide.PowOp(powerExponent-1);
					BasicMultOp(PiSide);
				}
			}
		}
	#endif

	#if defined(AltNum_EnableERep)
        void ConvertEToNum()
        {
            BasicMultOp(ENum);
            if(ExtraRep!=IERep)
            {
                int TempDiv = ExtraRep*-1;
                BasicIntDivOp(TempDiv);
            }
            ExtraRep = 0;
        }
	#endif
        public:
        void ConvertToNumRep()
        {
			RepType repType = GetRepType();
			ConvertToNormType(repType);
        }
        
        //Switch based version of ConvertToNumRep
        void ConvertToNormType(RepType& repType)
        {
            switch (repType)
            {
            case RepType::NormalType:
                break;
	#if defined(AltNum_EnableInfinityRep)
			case RepType::PositiveInfinity:
                IntValue = 2147483647; DecimalHalf = 999999999; ExtraRep = 0;
                break;
			case RepType::NegativeInfinity:
                IntValue = -2147483647; DecimalHalf = 999999999; ExtraRep = 0;
                break;
		#if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
                DecimalHalf = 1; ExtraRep = 0;
                break;
            case RepType::ApproachingTop:
                DecimalHalf = 999999999; ExtraRep = 0;
                break;
			#if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidRight:
				int InvertedExtraRep = ExtraRep*-1;
				if(DecimalOverflow%InvertedExtraRep!=0)//Only cut off the traiing digits for those that can't have all digits stored
					DecimalHalf = DecimalOverflow/InvertedExtraRep;
				else
					DecimalHalf = (DecimalOverflow/InvertedExtraRep)+1;
				ExtraRep = 0;
				break;
            case RepType::ApproachingMidLeft:
				if(DecimalOverflow%ExtraRep==0)//Only cut off the traiing digits for those that can't have all digits stored
					DecimalHalf = DecimalOverflow/ExtraRep;
				else
					DecimalHalf = (DecimalOverflow/ExtraRep)-1;
				ExtraRep = 0;
                break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                BasicMultOp(PiNum);
                ExtraRep = 0;
                break;
		#if defined(AltNum_EnablePiPowers)
			case RepType::PiPower:
                ConvertPiPowerToNum();
                break;
		#endif
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                BasicMultOp(PiNum);
                ExtraRep *= -1;
                BasicIntDivOp(ExtraRep);
                ExtraRep = 0;
			#endif
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
				signed _int64 FirstHalf = 3141592654;
				FirstHalf *= IntValue;
				signed int divRes = FirstHalf / DecimalHalf;
			#ifdef AltNum_OutputTruncatedTrailingDigits//keep remainder after dividing and output to console
				signed _int64 TruncatedDigits = FirstHalf - DecimalHalf * divRes;
				//Add output code here later
			#else
				FirstHalf = divRes;
			#endif
				//convert from Int64 into normal AltNum format
				signed  _int64 FirstPart =  FirstHalf/DecimalOverflowX;
				signed _int64 SecondPart = FirstHalf - DecimalOverflowX * FirstPart;
				if(IntValue<0&&FirstPart==0)//Results in -0.XXXXXXXX
					IntValue = NegativeRep;
				else
					IntValue = FirstPart;
				DecimalHalf = SecondPart;
			    ExtraRep = 0;
                break;
		#endif		
	#endif
            case RepType::NumByDiv:
                BasicIntDivOp(ExtraRep);
                ExtraRep = 0;
                break;
	#if defined(AltNum_EnableERep)
            case RepType::ENum:
                BasicMultOp(ENum);
                ExtraRep = 0;
                break;
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv:
                BasicMultOp(ENum);
                ExtraRep *= -1;
                BasicIntDivOp(ExtraRep);
                ExtraRep = 0;
                break;
			#endif
            case RepType::EFractional://IntValue/DecimalHalf*e Representation
				signed _int64 FirstHalf = 2718281828;
				FirstHalf *= IntValue;
				signed int divRes = FirstHalf / DecimalHalf;
			#ifdef AltNum_OutputTruncatedTrailingDigits//keep remainder after dividing and output to console
				signed _int64 TruncatedDigits = FirstHalf - DecimalHalf * divRes;
				//Add output code here later
			#else
				FirstHalf /= DecimalHalf;
			#endif
				//convert from Int64 into normal AltNum format
				signed  _int64 FirstPart =  FirstHalf/DecimalOverflowX;
				signed _int64 SecondPart = FirstHalf - DecimalOverflowX * FirstPart;
				if(IntValue<0&&FirstPart==0)//Results in -0.XXXXXXXX
					IntValue = NegativeRep;
				else
					IntValue = FirstPart;
				DecimalHalf = SecondPart;
			    ExtraRep = 0;
                break;
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
				MediumDecVariant Res = IntValue<0?MediumDecVariant(DecimalHalf, 0):MediumDecVariant(DecimalHalf, 0);
				Res /= ExtraRep;
				if(IntValue!=0&&IntValue!=NegativeRep)
					Res += IntValue;
				IntValue = Res.IntValue;
				DecimalHalf = Res.DecimalHalf;
				ExtraRep = 0;
				break;
	#endif

	#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
				if(IntValue==0&&DecimalHalf==0)
					ExtraRep = 0
				else
					throw "Can't convert imaginery number into real number unless is zero.";
				break;
			#endif
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
				if(IntValue==0&&DecimalHalf!=0)
					ExtraRep = 0
				else
					throw "Can't convert imaginery number into real number unless is zero.";
				break;
	#endif
	#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
				throw "Conversion from complex number to real number not supported yet.";
				break;
	#endif
            default:
                ConvertToNumRep();
                break;
            }
        }
		
		void ConvertAltFracWithInvertedExtra(RepType& repType)
        {
            switch (repType)
            {
			case RepType::MixedFrac:
				MediumDecVariant Res = IntValue<0?MediumDecVariant(DecimalHalf, 0):MediumDecVariant(DecimalHalf, 0);
				Res /= -ExtraRep;
				if(IntValue!=0&&IntValue!=NegativeRep)
					Res += IntValue;
				IntValue = Res.IntValue;
				DecimalHalf = Res.DecimalHalf;
				ExtraRep = 0;
				break;
		}

    #if defined(AltNum_EnableImaginaryNum)
		void ConvertToNormalIRep(RepType& repType)
        {
            switch (repType)
            {
            case RepType::INum:
				if(IntValue==0&&DecimalHalf==0)
					ExtraRep = 0
				break;
        #if defined(AltNum_EnableAlternativeRepFractionals)
            #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
				if(IntValue==0&&DecimalHalf==0)
					ExtraRep = 0
				else
				{
					int Divisor = -ExtraRep;
					ExtraRep = IRep;
					this /= Divisor;
				}
				break;
            #endif
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
        #endif
				if(IntValue==0&&DecimalHalf!=0)
					ExtraRep = 0
				else
				{
					int Divisor = DecimalHalf;
					ExtraRep = IRep; DecimalHalf = 0;
					this /= Divisor;
				}
				break;
        #ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
				throw "Conversion from complex number to real number not supported yet.";
				break;
        #endif
            default:
                throw "Conversion not supported.";
                break;
			}
		}
    #endif
        
    #if defined(AltNum_EnablePiRep)
        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetPiValFromMediumDec(MediumDec Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = PiRep;
        }
        #endif

        void SetPiVal(MediumDecVariant Value)
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

    #if defined(AltNum_EnableERep)
        #if defined(AltNum_EnableMediumDecBasedSetValues)
        void SetEValFromMediumDec(MediumDec Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = ERep;
        }
        #endif

        void SetEVal(MediumDecVariant Value)
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
    #if defined(AltNum_EnableFractionals)
        //Set value for NumByDiv
        void SetFractionalVal(int IntHalf, int DecHalf, int Divisor)
        {
            IntValue = IntHalf; DecimalHalf = IntHalf;
            ExtraRep = Divisor;
        }
		
        //Set value for NumByDiv
        void SetFractionalVal(MediumDec Value, int Divisor)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = Divisor;
        }
        
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
	    #if defined(AltNum_EnablePiRep)
        void SetPiFractional(int Num, int Divisor)//IntValue/DecimalHalf*Pi Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = PiRep;
        }
	    #endif
	    #if defined(AltNum_EnableERep)
        void SetEFractional(int Num, int Divisor)//IntValue/DecimalHalf*e Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = ERep;
        }
	    #endif
	    #if defined(AltNum_EnableImaginaryNum)
        void SetIFractional(int Num, int Divisor)//IntValue/DecimalHalf*i Representation
        {
            IntValue = Num; DecimalHalf = Divisor;
            ExtraRep = IRep;
        }
	    #endif
    #endif
        
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
private:
        static MediumDecVariant InfinityValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(1, InfinityRep);
            return NewSelf;
        }
        
        static MediumDecVariant NegativeInfinityValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(-1, InfinityRep);
            return NewSelf;
        }
        
#if defined(AltNum_EnableApproachingValues)
        static MediumDecVariant ApproachingZeroValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, ApproachingBottomRep);
            return NewSelf;
        }
#endif
public:
#endif
#if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep;
            ExtraRep = 0;
        }

        static MediumDecVariant NaNValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, NaNRep);
            return NewSelf;
        }
		
        void SetAsUndefined()
        {
            IntValue = 0; DecimalHalf = UndefinedRep;
            ExtraRep = 0;
        }

        static MediumDecVariant UndefinedValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, UndefinedRep);
            return NewSelf;
        }
#endif

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
private:
#if defined(AltNum_EnableInfinityRep)
        static MediumDecVariant ApproachingRightRealValue(int IntValue=0)
        {
            return MediumDecVariant(IntValue, 999999999);
        }

        static MediumDecVariant ApproachingLeftRealValue(int IntValue=0)
        {
            return MediumDecVariant(IntValue, 1);
        }

        static MediumDecVariant LeftAlmostPointFiveRealValue(int IntValue=0)
        {
            return MediumDecVariant(IntValue, 499999999);
        }

        static MediumDecVariant RightAlmostPointFiveRealValue(int IntValue=0)
        {
            return MediumDecVariant(IntValue, 500000001);
        }
#endif

    #pragma region ValueDefines
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant PiNumValue()
        {
            return MediumDecVariant(3, 141592654, 0);
        }
        
        static MediumDecVariant PiValue()
        {
            return MediumDecVariant(1, 0, PiRep);
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecVariant HundredMilPiNumVal()
        {
            return MediumDecVariant(314159265, 358979324, 0);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecVariant TenMilPiNumVal()
        {
            return MediumDecVariant(31415926, 535897932, 0);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecVariant OneMilPiNumVal()
        {
            return MediumDecVariant(3141592, 653589793, 0);
        }

        //10xPi(Rounded to 9th decimal digit)
        static MediumDecVariant TenPiNumVal()
        {
            return MediumDecVariant(31, 415926536, 0);
        }
        
        static MediumDecVariant ZeroValue()
        {
            return MediumDecVariant(0, 0, 0);
        }
        
        static MediumDecVariant ENumValue()
        {
            return MediumDecVariant(2, 718281828, 0);
        }

        static MediumDecVariant EValue()
        {
#if defined(AltNum_EnableERep)
            return MediumDecVariant(1, 0, ERep);
#else
            return MediumDecVariant(2, 718281828, 0);
#endif
        }
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant OneValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant TwoValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant NegativeOneValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant Point5Value()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, 500000000);
            return NewSelf;
        }

        static MediumDecVariant JustAboveZeroValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, 1);
            return NewSelf;
        }

        static MediumDecVariant OneMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, 1000);
            return NewSelf;
        }

        static MediumDecVariant FiveThousandthValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, 5000000);
            return NewSelf;
        }

        static MediumDecVariant FiveMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, 5000);
            return NewSelf;
        }

        static MediumDecVariant TenMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, 100);
            return NewSelf;
        }

        static MediumDecVariant OneHundredMillionthValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, 10);
            return NewSelf;
        }

        static MediumDecVariant FiveBillionthValue()
        {
            MediumDecVariant NewSelf = MediumDecVariant(0, 5);
            return NewSelf;
        }

        static MediumDecVariant LN10Value()
        {
            return MediumDecVariant(2, 302585093);
        }

        static MediumDecVariant LN10MultValue()
        {
            return MediumDecVariant(0, 434294482);
        }

        static MediumDecVariant HalfLN10MultValue()
        {
            return MediumDecVariant(0, 868588964);
        }
        
        static MediumDecVariant MinimumValue()
        {
            return MediumDecVariant(2147483647, 999999999);
        }

        static MediumDecVariant MaximumValue()
        {
            return MediumDecVariant(2147483647, 999999999);
        }
public:
        static MediumDecVariant AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant ENum;
        
#if defined(AltNum_EnableInfinityRep)
        static MediumDecVariant Infinity;
        static MediumDecVariant NegativeInfinity;
        static MediumDecVariant ApproachingZero;
#endif
        
    #if defined(AltNum_EnableNilRep)
        static MediumDecVariant NilValue()
        {
            return MediumDecVariant(NilRep, NilRep);
        }
    #endif

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant E;
        
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant OneGMillionth;

        //0e-7
        static MediumDecVariant TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDecVariant FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant NegativeOne;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDecVariant Maximum;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDecVariant Minimum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDecVariant LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecVariant LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecVariant HalfLN10Mult;

    #if defined(AltNum_EnableNilRep)
        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static MediumDecVariant Nil;
    #endif
    #pragma endregion ValueDefines
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
        MediumDecVariant GetValueFromString(std::string Value);

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecVariant(const char* strVal)
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
        MediumDecVariant(std::string Value)
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

        ///// <summary>
        ///// += MediumDecVariant operation added to std::string
        ///// </summary>
        ///// <param name="targetString">The target string(left side).</param>
        ///// <param name="Value">The right side value</param>
        ///// <returns>string</returns>
        //friend std::string operator+=(std::string targetString, MediumDecVariant self) { return targetString + self.ToString(); }
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
            if (Value >= 2147483647.0f)
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
                    IntValue = 0;
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
            if (Value >= 2147483647.0)
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
                    IntValue = 0;
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
                Value -= (ldouble)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = 0;
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
        MediumDecVariant(float Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecVariant(double Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecVariant(ldouble Value)
        {
            this->SetVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecVariant(bool Value)
        {
            this->SetVal(Value);
        }

        MediumDecVariant(MediumDec Value)
        {
            this->SetVal(Value);
        }
    #pragma endregion From Standard types to this type

    #pragma region From this type to Standard types
        /// <summary>
        /// MediumDecVariant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float()
        {
            float Value;
            if (IntValue < 0)
            {
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue;
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
                Value = (float)IntValue;
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
        }
        /// <summary>
        /// MediumDecVariant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double()
        {
            double Value;
            if (IntValue < 0)
            {
                Value = IntValue == NegativeRep ? 0.0 : (double)IntValue;
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
                Value = (double)IntValue;
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
        }

        /// <summary>
        /// MediumDecVariant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble()
        {
            ldouble Value;
            if (IntValue < 0)
            {
                Value = IntValue == NegativeRep ? 0.0L : (ldouble)IntValue;
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
                Value = (ldouble)IntValue;
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
            }
            return Value;
        }

        /// <summary>
        /// MediumDecVariant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return IntValue; }

        explicit operator bool() { return IntValue == 0 ? false : true; }

        /// <summary>
        /// MediumDecVariant to integer type explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        template<typename IntType>
        explicit operator IntType() { return IntValue; }
    #pragma endregion From this type to Standard types

    #pragma region Comparison Operators
        /// <summary>
        /// Equal to Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator==(MediumDecVariant self, MediumDecVariant Value)
        {
			RepType LRep = self.GetRepType();
			RepType RRep = Value.GetRepType();
			if(LRep!=RRep)
			{//ToDo:Check bitvalue of RepType instead maybe
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(LRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						ConvertToNormalIRep(LRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						self.ConvertToNormType(LRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(RRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						Value.ConvertToNormalIRep(RRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						Value.ConvertToNormType(RRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			}
			
            return (self.IntValue.Value == Value.IntValue.Value && self.DecimalHalf == Value.DecimalHalf && self.ExtraRep==self.ExtraRep);
        }

        /// <summary>
        /// Not equal to Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator!=(MediumDecVariant self, MediumDecVariant Value)
        {
			RepType LRep = self.GetRepType();
			RepType RRep = Value.GetRepType();
			if(LRep!=RRep)
			{//ToDo:Check bitvalue of RepType instead maybe
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(LRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						self.ConvertToNormalIRep(LRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						self.ConvertToNormType(LRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(RRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						Value.ConvertToNormalIRep(RRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						Value.ConvertToNormType(RRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			}
            return (self.IntValue.Value != Value.IntValue.Value || self.DecimalHalf != Value.DecimalHalf);
        }

        /// <summary>
        /// Lesser than Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<(MediumDecVariant self, MediumDecVariant Value)
        {
			RepType LRep = self.GetRepType();
			RepType RRep = Value.GetRepType();
			if(LRep!=RRep)
			{//ToDo:Check bitvalue of RepType instead maybe
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(LRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						self.ConvertToNormalIRep(LRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						self.ConvertToNormType(LRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(RRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						Value.ConvertToNormalIRep(RRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						Value.ConvertToNormType(RRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			}
#if defined(AltNum_EnableImaginaryNum)
			if(self.ExtraRep!=Value.ExtraRep)
				throw "Can't compare imaginary number with real number";
#endif
#if defined(AltNum_EnableInfinityRep)
            if(self.DecimalHalf==InfinityRep)
            {
	#if defined(AltNum_EnableImaginaryInfinity)
				if(self.ExtraRep==IRep)//LeftSide is Imaginary infinity
				{
					if(Value.DecimalHalf==InfinityRep)//both left and right are infinity types
					{
						if(Value.IntValue.Value==1&&self.IntValue.Value==-1)
							return true;
						else
							return false;
					}
					else if(self.IntValue==1)//Left is Positive Imaginary Infinity
						return false;
					else//Left Negative Imaginary Infinity
						return true;
				}
	#endif
                if(Value.DecimalHalf==InfinityRep)//both left and right are infinity types
				{
					//return self.IntValue<Value.IntValue;
					/*
						+inf < +inf = false
						+inf < -inf = false
						-inf < +inf = true
						-inf < -inf = false
					*/
					if(Value.IntValue.Value==1&&self.IntValue.Value==-1)
						return true;
					else
						return false;
				}
				else if(self.IntValue==1)//Left is Positive Infinity
					return false;//+inf < 99
				else//Left is Negative Infinity
					return true;//-inf < 99
            }
            else if(Value.DecimalHalf==InfinityRep)//Right side is infinity
            {
	#if defined(AltNum_EnableImaginaryInfinity)
				if(Value.ExtraRep==IRep)//RightSide is Imaginary infinity
				{
					if(self.IntValue==1)//Right is Positive Imaginary Infinity
						return true;
					else//Right is Negative Imaginary Infinity
						return false;
				}
	#endif
				if(Value.IntValue==1)//Right is Positive Infinity
					return true;//99 < +inf
				else//Right is Negative Infinity
					return false;//99 < -inf
            }
#endif
            if(self.DecimalHalf==0)
            {
                if(Value.DecimalHalf==0)
                    return self.IntValue<Value.IntValue;
                else
                {
                    if(self.IntValue<Value.IntValue)
                        return self.DecimalHalf<Value.DecimalHalf;
                    else
                        return false;
                }
            }
            else if(self.IntValue<Value.IntValue)
                return self.DecimalHalf<Value.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Lesser than or Equal to Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<=(MediumDecVariant self, MediumDecVariant Value)
        {
			RepType LRep = self.GetRepType();
			RepType RRep = Value.GetRepType();
			if(LRep!=RRep)
			{//ToDo:Check bitvalue of RepType instead maybe
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(LRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						self.ConvertToNormalIRep(LRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						self.ConvertToNormType(LRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(RRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						Value.ConvertToNormalIRep(RRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						Value.ConvertToNormType(RRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			}
#if defined(AltNum_EnableImaginaryNum)
			if(self.ExtraRep!=Value.ExtraRep)
				throw "Can't compare imaginary number with real number";
#endif
#if defined(AltNum_EnableInfinityRep)
            if(self.DecimalHalf==InfinityRep)
            {
	#if defined(AltNum_EnableImaginaryInfinity)
				if(self.ExtraRep==IRep)//LeftSide is Imaginary infinity
				{
					if(Value.DecimalHalf==InfinityRep)//both left and right are infinity types
					{
						if(Value.IntValue.Value==1&&self.IntValue.Value==-1)
							return false;
						else
							return true;
					}
					else if(self.IntValue==1)//Left is Positive Imaginary Infinity
						return false;
					else//Left Negative Imaginary Infinity
						return true;
				}
	#endif
                if(Value.DecimalHalf==InfinityRep)//both left and right are infinity types
				{
					//return self.IntValue<=Value.IntValue;
					/*
						(+inf <= +inf) = true
						(+inf <= -inf) = false
						(-inf <= +inf) = true
						(-inf <= -inf) = true
					*/
					if(Value.IntValue.Value==-1&&self.IntValue.Value==1)
						return false;
					else
						return true;
				}
				else if(self.IntValue==1)//Left is Positive Infinity
					return false;//+inf <= 99
				else//Left is Negative Infinity
					return true;//-inf <= 99
            }
            else if(Value.DecimalHalf==InfinityRep)//Right side is infinity
            {
	#if defined(AltNum_EnableImaginaryInfinity)
				if(Value.ExtraRep==IRep)//RightSide is Imaginary infinity
				{
					if(self.IntValue==1)//Right is Positive Imaginary Infinity
						return true;
					else//Right is Negative Imaginary Infinity
						return false;
				}
	#endif
				if(Value.IntValue==1)//Right is Positive Infinity
					return true;//99 <= +inf
				else//Right is Negative Infinity
					return false;//99 <= -inf
            }
#endif
            if(self.DecimalHalf==0)
            {
                if(Value.DecimalHalf==0)
                    return self.IntValue<=Value.IntValue;
                else
                {
                    if(self.IntValue<=Value.IntValue)
                        return self.DecimalHalf<=Value.DecimalHalf;
                    else
                        return false;
                }
            }
            else if(self.IntValue<=Value.IntValue)
                return self.DecimalHalf<=Value.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Greater than Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>bool</returns>
        friend bool operator>(MediumDecVariant self, MediumDecVariant Value)
        {
			RepType LRep = self.GetRepType();
			RepType RRep = Value.GetRepType();
			if(LRep!=RRep)
			{//ToDo:Check bitvalue of RepType instead maybe
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(LRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						self.ConvertToNormalIRep(LRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						self.ConvertToNormType(LRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(RRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						Value.ConvertToNormalIRep(RRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						Value.ConvertToNormType(RRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			}
#if defined(AltNum_EnableImaginaryNum)
			if(self.ExtraRep!=Value.ExtraRep)
				throw "Can't compare imaginary number with real number";
#endif
#if defined(AltNum_EnableInfinityRep)
            if(self.DecimalHalf==InfinityRep)
            {
	#if defined(AltNum_EnableImaginaryInfinity)
				if(self.ExtraRep==IRep)//LeftSide is Imaginary infinity
				{
					if(Value.DecimalHalf==InfinityRep)//both left and right are infinity types
					{
						if(Value.IntValue.Value==-1&&self.IntValue.Value==1)
							return true;
						else
							return false;
					}
					else if(self.IntValue==1)//Left is Positive Imaginary Infinity
						return true;
					else//Left Negative Imaginary Infinity
						return false;
				}
	#endif
                if(Value.DecimalHalf==InfinityRep)//both left and right are infinity types
				{
					//return self.IntValue>Value.IntValue;
					/*
						+inf > +inf = false
						+inf > -inf = true
						-inf > +inf = false
						-inf > -inf = false
					*/
					if(Value.IntValue.Value==-1&&self.IntValue.Value==1)
						return true;
					else
						return false;
				}
				else if(self.IntValue==1)//Left is Positive Infinity
					return true;//+inf > 99
				else//Left is Negative Infinity
					return false;//-inf > 99
            }
            else if(Value.DecimalHalf==InfinityRep)//Right side is infinity
            {
	#if defined(AltNum_EnableImaginaryInfinity)
				if(Value.ExtraRep==IRep)//RightSide is Imaginary infinity
				{
					if(self.IntValue==1)//Right is Positive Imaginary Infinity
						return false;
					else//Right is Negative Imaginary Infinity
						return true;
				}
	#endif
				if(Value.IntValue==1)//Right is Positive Infinity
					return false;//99 > +inf
				else//Right is Negative Infinity
					return true;//99 > -inf
            }
#endif
            if(self.DecimalHalf==0)
            {
                if(Value.DecimalHalf==0)
                    return self.IntValue>Value.IntValue;
                else
                {
                    if(self.IntValue>Value.IntValue)
                        return self.DecimalHalf>Value.DecimalHalf;
                    else
                        return false;
                }
            }
            else if(self.IntValue>Value.IntValue)
                return self.DecimalHalf>Value.DecimalHalf;
            else
                return false;
        }

        /// <summary>
        /// Greater than or Equal to Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>=(MediumDecVariant self, MediumDecVariant Value)
        {
			RepType LRep = self.GetRepType();
			RepType RRep = Value.GetRepType();
			if(LRep!=RRep)
			{//ToDo:Check bitvalue of RepType instead maybe
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(LRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						self.ConvertToNormalIRep(LRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						self.ConvertToNormType(LRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
				switch(RRep)
				{
				#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
					case INum:
					#if defined(AltNum_EnableAlternativeRepFractionals)
						#if defined(AltNum_EnableDecimaledIFractionals)
					case INumByDiv://(Value/(ExtraRep*-1))*i Representation
						#endif
					case IFractional://  IntValue/DecimalHalf*i Representation
					#endif
					#ifdef AltNum_EnableComplexNumbers
					case ComplexIRep:
					#endif
						Value.ConvertToNormalIRep(RRep);
						break;
					//Don't convert infinity into real number
					#if defined(AltNum_EnableImaginaryInfinity)
					case PositiveImaginaryInfinity:
					case NegativeImaginaryInfinity:
						break;
					#endif
				#endif
				#if defined(AltNum_EnableInfinityRep)
					//Don't convert infinity into real number
					case PositiveInfinity:
					case NegativeInfinity:
						break;
				#endif
					default:
			#endif
						Value.ConvertToNormType(RRep);
			#if defined(AltNum_EnableImaginaryNum)||defined(AltNum_EnableInfinityRep)
						break;
				}
			#endif
			}
#if defined(AltNum_EnableImaginaryNum)
			if(self.ExtraRep!=Value.ExtraRep)
				throw "Can't compare imaginary number with real number";
#endif
#if defined(AltNum_EnableInfinityRep)
            if(self.DecimalHalf==InfinityRep)
            {
	#if defined(AltNum_EnableImaginaryInfinity)
				if(self.ExtraRep==IRep)//LeftSide is Imaginary infinity
				{
					if(Value.DecimalHalf==InfinityRep)//both left and right are infinity types
					{
						if(Value.IntValue.Value==1&&self.IntValue.Value==-1)
							return false;
						else
							return true;
					}
					else if(self.IntValue==1)//Left is Positive Imaginary Infinity
						return true;
					else//Left Negative Imaginary Infinity
						return false;
				}
	#endif
                if(Value.DecimalHalf==InfinityRep)//both left and right are infinity types
				{
					//return self.IntValue>=Value.IntValue;
					/*
						(+inf >= +inf) = true
						(+inf >= -inf) = true
						(-inf >= +inf) = false
						(-inf >= -inf) = true
					*/
					if(Value.IntValue.Value==1&&self.IntValue.Value==-1)
						return false;
					else
						return true;
				}
				else if(self.IntValue==1)//Left is Positive Infinity
					return true;//+inf >= 99 
				else//Left is Negative Infinity
					return false;//-inf >= 99
            }
            else if(Value.DecimalHalf==InfinityRep)//Right side is infinity
            {
	#if defined(AltNum_EnableImaginaryInfinity)
				if(Value.ExtraRep==IRep)//RightSide is Imaginary infinity
				{
					if(self.IntValue==1)//Right is Positive Imaginary Infinity
						return false;
					else//Right is Negative Imaginary Infinity
						return true;
				}
	#endif
				if(Value.IntValue==1)//Right is Positive Infinity
					return false;//99 >= +inf
				else//Right is Negative Infinity
					return true;//99 >= -inf
            }
#endif
            if(self.DecimalHalf==0)
            {
                if(Value.DecimalHalf==0)
                    return self.IntValue>=Value.IntValue;
                else
                {
                    if(self.IntValue>=Value.IntValue)
                        return self.DecimalHalf>=Value.DecimalHalf;
                    else
                        return false;
                }
            }
            else if(self.IntValue>=Value.IntValue)
                return self.DecimalHalf>=Value.DecimalHalf;
            else
                return false;
        }

    #pragma endregion Comparison Operators

#pragma region RepToRepCode_AdditionSubtraction
    private:
#if defined(AltNum_EnableMixedFractional)
		void MixedFracAddOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value);
		void MixedFracSubOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value);
		void MixedFracMultOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value);
		void MixedFracDivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value);
		
		//Assumes NormalRep + Normal MixedFraction operation
		void BasicMixedFracAddOp(MediumDecVariant& self, MediumDecVariant& Value)
		{
			if(self.DecimalHalf==0)//Avoid needing to convert if Leftside is not decimal format representation
			{
				if(self.IntValue<0)
				{
					if(Value.IntValue==NegativeRep)
					{
						self.DecimalHalf = Value.DecimalHalf;
						self.ExtraRep = Value.ExtraRep;
					}
					else if(Value.IntValue<0)
					{
						self.IntValue += Value.IntValue;
						self.DecimalHalf = Value.DecimalHalf;
						self.ExtraRep = Value.ExtraRep;
					}
					else//(Value.IntValue>0)
					{
						if(Value.IntValue>-self.IntValue)//check for flipping of sign
						{
							self.IntValue += Value.IntValue - 1;
							self.DecimalHalf = Value.ExtraRep - Value.DecimalHalf;
						}
						else
						{
							self.IntValue += Value.IntValue;
							self.DecimalHalf = Value.ExtraRep - Value.DecimalHalf;
						}
						self.ExtraRep = Value.ExtraRep;
					}
				}
				else//(self.IntValue>0)
				{
					if(Value.IntValue==NegativeRep)
					{
						self.DecimalHalf = Value.ExtraRep - Value.DecimalHalf;
						self.ExtraRep = Value.ExtraRep;
					}
					else if(Value.IntValue<0)
					{
						self.IntValue += Value.IntValue;
						if(-Value.IntValue>self.IntValue)//check for flipping of sign
						{
							self.IntValue += Value.IntValue;
							if(self.IntValue==-1)
								self.IntValue = NegativeRep;
							else
								++self.IntValue;
						}
						self.DecimalHalf = Value.ExtraRep - Value.DecimalHalf;
						self.ExtraRep = Value.ExtraRep;
					}
					else//(Value.IntValue>0)
					{
						self.IntValue += Value.IntValue;
						self.DecimalHalf = Value.DecimalHalf;
						self.ExtraRep = Value.ExtraRep;
					}
				}     
			}
			else
			{
				MediumDecVariant RightSideNum = MediumDecVariant(Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf);
				self.BasicMultOp(Value.ExtraRep);
				self += RightSideNum;
				if(self.DecimalHalf==0)
				{
					if(self.IntValue!=0)//Set as Zero if both are zero
					{
						self.DecimalHalf = -self.DecimalHalf;
						self.ExtraRep = Value.ExtraRep;
					}
				}
				else
				{
					if(self.IntValue!=0&&self.IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
						self.DecimalHalf = -self.DecimalHalf;
					self.ExtraRep = Value.ExtraRep;
				}
			}
		}
		
		#if defined(AltNum_EnableMixedPiFractional)
		void BasicMixedPiFracAddOp(MediumDecVariant& self, MediumDecVariant& Value)
		#elif defined(AltNum_EnableMixedEFractional)
		void BasicMixedEFracAddOp(MediumDecVariant& self, MediumDecVariant& Value)
		#endif
		#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional)
		{
			MediumDecVariant RightSideNum = MediumDecVariant(Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep) - Value.DecimalHalf);
		#if defined(AltNum_EnableMixedPiFractional)
			RightSideNum *= PiNum;
		#else
			RightSideNum *= ENum;
		#endif
			self.BasicMultOp(-Value.ExtraRep);
			self += RightSideNum;
			if(self.DecimalHalf==0)
			{
				if(self.IntValue!=0)//Set as Zero if both are zero
				{
					self.DecimalHalf = -self.DecimalHalf;
					self.ExtraRep = -Value.ExtraRep;
				}
			}
			else
			{
				if(self.IntValue!=0&&self.IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
					self.DecimalHalf = -self.DecimalHalf;
				self.ExtraRep = -Value.ExtraRep;
			}
		}
		#endif
		
		//Assumes NormalRep - Normal MixedFraction operation
		void BasicMixedFracSubOp(MediumDecVariant& self, MediumDecVariant& Value)
		{
			MediumDecVariant RightSideNum = MediumDecVariant(Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf);
			self.BasicMultOp(Value.ExtraRep);
			self -= RightSideNum;
			if(self.DecimalHalf==0)
			{
				if(self.IntValue!=0)//Set as Zero if both are zero
				{
					self.DecimalHalf = -self.DecimalHalf;
					self.ExtraRep = Value.ExtraRep;
				}
			}
			else
			{
				if(self.IntValue!=0&&self.IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
					self.DecimalHalf = -self.DecimalHalf;
				self.ExtraRep = Value.ExtraRep;
			}
		}
		
		#if defined(AltNum_EnableMixedPiFractional)
		void BasicMixedPiFracSubOp(MediumDecVariant& self, MediumDecVariant& Value)
		#elif defined(AltNum_EnableMixedEFractional)
		void BasicMixedEFracSubOp(MediumDecVariant& self, MediumDecVariant& Value)
		#endif
		#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional)
		{
			MediumDecVariant RightSideNum = MediumDecVariant(Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep) - Value.DecimalHalf);
		#if defined(AltNum_EnableMixedPiFractional)
			RightSideNum *= PiNum;
		#else
			RightSideNum *= ENum;
		#endif
			self.BasicMultOp(-Value.ExtraRep);
			self -= RightSideNum;
			if(self.DecimalHalf==0)
			{
				if(self.IntValue!=0)//Set as Zero if both are zero
				{
					self.DecimalHalf = -self.DecimalHalf;
					self.ExtraRep = -Value.ExtraRep;
				}
			}
			else
			{
				if(self.IntValue!=0&&self.IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
					self.DecimalHalf = -self.DecimalHalf;
				self.ExtraRep = -Value.ExtraRep;
			}
		}
		#endif
#endif
	
		void RepToRepAddOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value);
		
		bool RepToRepSubOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value);

		#pragma endregion RepToRepCode_AdditionSubtraction
#pragma region RepToRepCode_MultiplicationDivision
		
		void RepToRepMultOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value);
		
		bool RepToRepDivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value);
		
    public:
#pragma endregion RepToRepCode_MultiplicationDivision

#pragma region Addition/Subtraction Operations
        /// <summary>
        /// Basic Addition Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicAddOp(MediumDecVariant& Value)
        {
            bool NegativeBeforeOperation = IntValue < 0;
            //Deal with Int section first
            IntValue += Value.IntValue;
            if (Value.DecimalHalf != 0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecVariant::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecVariant::DecimalOverflow) { DecimalHalf -= MediumDecVariant::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecVariant::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecVariant::DecimalOverflow) { DecimalHalf -= MediumDecVariant::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecVariant::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecVariant::DecimalOverflow) { DecimalHalf -= MediumDecVariant::DecimalOverflow; --IntValue; }
                    }
                    else
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecVariant::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecVariant::DecimalOverflow) { DecimalHalf -= MediumDecVariant::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(NegativeBeforeOperation^(IntValue<0))
                DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
        }

private:
        void CatchAllAddition(MediumDecVariant& Value, RepType& LRep, RepType& RRep)
        {
            ConvertToNormType(LRep);
            Value.ConvertToNormType(RRep);
            BasicAddOp(Value);
        }
		
        void CatchAllAddition(MediumDecVariant& Value, RepType& SameRep)
        {
            ConvertToNormType(SameRep);
            Value.ConvertToNormType(SameRep);
            BasicAddOp(Value);
        }
public:
        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant& AddOp(MediumDecVariant& self, MediumDecVariant& Value);
		
		/// <summary>
        /// Basic Subtraction Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicSubOp(MediumDecVariant& Value)
        {
            bool NegativeBeforeOperation = IntValue < 0;
            //Deal with Int section first
            IntValue -= Value.IntValue;
            //Now deal with the decimal section
            if(Value.DecimalHalf!=0)
            {
                if (Value.IntValue < 0)
                {
                    if (WasNegative)//-4.0 - -0.5 = -3.5
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecVariant::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecVariant::DecimalOverflow) { DecimalHalf -= MediumDecVariant::DecimalOverflow; --IntValue; }
                    }
                    else//4.3 -  - 1.8
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecVariant::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecVariant::DecimalOverflow) { DecimalHalf -= MediumDecVariant::DecimalOverflow; ++IntValue; }
                    }
                }
                else
                {
                    if (WasNegative)//-4.5 - 5.6
                    {
                        DecimalHalf += Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecVariant::DecimalOverflow; ++IntValue; }
                        else if (DecimalHalf >= MediumDecVariant::DecimalOverflow) { DecimalHalf -= MediumDecVariant::DecimalOverflow; --IntValue; }
                    }
                    else//0.995 - 1 = 
                    {
                        DecimalHalf -= Value.DecimalHalf;
                        if (DecimalHalf < 0) { DecimalHalf += MediumDecVariant::DecimalOverflow; --IntValue; }
                        else if (DecimalHalf >= MediumDecVariant::DecimalOverflow) { DecimalHalf -= MediumDecVariant::DecimalOverflow; ++IntValue; }
                    }
                }
            }
            //If flips to other side of negative, invert the decimals
            if(NegativeBeforeOperation^(IntValue<0))
                DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
        }

private:
    void CatchAllSubtraction(MediumDecVariant& Value, RepType& LRep, RepType& RRep)
    {
        ConvertToNormType(LRep);
        Value.ConvertToNormType(RRep);
        BasicSubOp(Value);
    }
	
    void CatchAllSubtraction(MediumDecVariant& Value, RepType& SameRep)
    {
        ConvertToNormType(SameRep);
        Value.ConvertToNormType(SameRep);
        BasicSubOp(Value);
    }
public:

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& SubOp(MediumDecVariant& self, MediumDecVariant& Value);

        /// <summary>
        /// Partial Addition Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="value">The value.</param>
        template<typename IntType>
        void PartialIntAddition(IntType& value)
        {
            if (DecimalHalf == 0)
                IntValue += value;
            else
            {
                bool WasNegative = IntValue < 0;
                if (WasNegative)
                    IntValue = IntValue == MediumDecVariant::NegativeRep ? -1 : --IntValue;
                IntValue += value;
                if (IntValue == -1)
                    IntValue = DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                else if (IntValue < 0)
                    ++IntValue;
                //If flips to other side of negative, invert the decimals
                if ((WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0))
                    DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
            }
        }

        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& IntAddOp(MediumDecVariant& self, IntType& value)
        {
            if (value == 0)
                return self;
#if defined(AltNum_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
#endif
            if(self.ExtraRep==PiRep)//Value*Pi Representation
            {
                self.ConvertToNumRep();
                self.PartialIntAddition(value);
            }
#if defined(AltNum_EnableImaginaryNum)
            else if(self.ExtraRep==IERep)
            {
                throw "Can't convert MediumDecVariant into complex number at moment";
            }
            else if(self.ExtraRep>0)
#elif defined(AltNum_EnableERep)
            else if(self.ExtraRep==IERep)
            {
            }
            else if(self.ExtraRep>0)
#else
            else//(Value/ExtraRep) Representation and Normal Representation
#endif
                self.ConvertToNumRep();
            if(self.ExtraRep==0)
            {
                self.PartialIntAddition(value);
            }
            return self;
        }

        /// <summary>
        /// Partial Subtraction Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="value">The value.</param>
        template<typename IntType>
        void PartialIntSubtraction(IntType& value)
        {
            if (DecimalHalf == 0)
                IntValue -= value;
            else
            {
                bool WasNegative = IntValue < 0;
                if (WasNegative)
                    IntValue = IntValue == MediumDecVariant::NegativeRep ? -1 : --IntValue;
                IntValue -= value;
                if (IntValue == -1)
                    IntValue = DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                else if (IntValue < 0)
                    ++IntValue;
                //If flips to other side of negative, invert the decimals
                if ((WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0))
                    DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
            }
        }
        
        /// <summary>
        /// Subtraction Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        static MediumDecVariant& IntSubOp(MediumDecVariant& self, IntType& value)
        {
            if (value == 0)
                return self;
#if defined(AltNum_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
#endif
            if(self.ExtraRep==PiRep)//Value*Pi Representation
            {
                self.ConvertToNumRep();
                self.PartialIntSubtraction(value);
            }
#if defined(AltNum_EnableImaginaryNum)
            else if(self.ExtraRep==IERep)
            {
                throw "Can't convert MediumDecVariant into complex number at moment";
            }
            else if(self.ExtraRep>0)
#elif defined(AltNum_EnableERep)
            else if(self.ExtraRep==IERep)
            {
            }
            else if(self.ExtraRep>0)
#else
            else//(Value/ExtraRep) Representation and Normal Representation
#endif
                self.ConvertToNumRep();
            if(self.ExtraRep==0)
            {
                self.PartialIntSubtraction(value);
            }
            return self;
        }

        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        void PartialUnsignedAddition(IntType& value)
        {
            if (DecimalHalf == 0 || IntValue > 0)
                IntValue += value;
            else
            {
                bool WasNegative = IntValue < 0;
                if (WasNegative)
                {
                    IntValue = IntValue == MediumDecVariant::NegativeRep ? -1 : --IntValue;
                    IntValue += value;
                    if (IntValue == -1)
                        IntValue = DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                    else if (IntValue < 0)
                        ++IntValue;
                    //If flips to other side of negative, invert the decimals
                    if ((WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0))
                        DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
                }
                else//Don't need to check if flipPing to other sign if adding positive to positive
                {
                    IntValue += value;
                }
            }
        }

        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedAddOp(MediumDecVariant& self, IntType& value)
        {
            if (value == 0)
                return self;
#if defined(AltNum_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
#endif
            if(self.ExtraRep==0)
            {
                self.PartialUnsignedAddition(value);
            }
#if defined(AltNum_EnableImaginaryNum)
            else if(self.ExtraRep==IRep)
            {
                throw "Can't convert MediumDecVariant into complex number at moment";
            }
#endif
			else
			{
                self.ConvertToNumRep();
                self.PartialUnsignedAddition(value);
			}
            return self;
        }

        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        void PartialUnsignedSubtraction(IntType& value)
        {
            if (DecimalHalf == 0)
                IntValue -= value;
            else if (IntValue == NegativeRep)
                IntValue = (signed int)value * -1;
            else if (IntValue < 0)
                IntValue -= value;
            else
            {
                bool WasNegative = IntValue < 0;
                if (WasNegative)//Don't need to check if negative value if going farther negative
                {
                    if(IntValue == MediumDecVariant::NegativeRep)
                    {
                        if(value==1)
                            IntValue = 0;
                        else
                            IntValue = ((signed int) value)*-1;
                    }
                    else
                    {
                        IntValue -= value;
                    }
                }
                else
                {
                    IntValue -= value;
                    if (IntValue == -1)
                        IntValue = DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                    else if (IntValue < 0)
                        ++IntValue;
                    //If flips to other side of negative, invert the decimals
                    if ((WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0))
                        DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
                }
            }
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecVariant and Integer value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedSubOp(MediumDecVariant& self, IntType& value)
        {
            if (value == 0)
                return self;
#if defined(AltNum_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
#endif
            if(self.ExtraRep==PiRep)//Value*Pi Representation
            {
                self.ConvertToNumRep();
                self.PartialUnsignedSubtraction(value);
            }
#if defined(AltNum_EnableImaginaryNum)
            else if(self.ExtraRep==IERep)
            {
                throw "Can't convert MediumDecVariant into complex number at moment";
            }
            else if(self.ExtraRep>0)
#elif defined(AltNum_EnableERep)
            else if(self.ExtraRep==IERep)
            {
            }
            else if(self.ExtraRep>0)
#else
            else//(Value/ExtraRep) Representation and Normal Representation
#endif
                self.ConvertToNumRep();
            if(self.ExtraRep==0)
            {
                self.PartialUnsignedSubtraction(value);
            }
            return self;
        }
#pragma endregion Addition/Subtraction Operations

#pragma region Multiplication/Division Operations
		/// <summary>
        /// Basic Multiplication Operation(before ensuring doesn't multiply into nothing)
        /// </summary>
        /// <param name="Value">The value.</param>
		bool BasicMultOpPt1(MediumDecVariant& Value)
        {//Warning:Modifies Value to make it a positive variable
        //Only checking for zero multiplication in main multiplication method
        //Not checking for special representation variations in this method(closer to MediumDec operation code)
            if (Value.IntValue < 0)
            {
                Value.SwapNegativeStatus();
                SwapNegativeStatus();
            }
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
                SRep /= MediumDecVariant::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
					return DecimalHalf==0?true:false;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * Value.IntValue;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
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
            else if (IntValue == MediumDecVariant::NegativeRep)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= MediumDecVariant::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * Value.IntValue;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
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
                    __int64 SRep = MediumDecVariant::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.DecimalHalf;
                    SRep /= MediumDecVariant::DecimalOverflowX;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? MediumDecVariant::NegativeRep : 0;
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
                else if (Value.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = MediumDecVariant::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.IntValue;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return false;
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? MediumDecVariant::NegativeRep : 0;
                        DecimalHalf = (signed int)SRep;
						return DecimalHalf==0?true:false;
                    }
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : MediumDecVariant::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.IntValue;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)IntValue * Value.DecimalHalf;//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)Value.DecimalHalf;
                    Temp04 /= MediumDecVariant::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / MediumDecVariant::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)MediumDecVariant::DecimalOverflow;
                    if (IntHalf == 0) { IntValue = (signed int)SelfIsNegative ? MediumDecVariant::NegativeRep : 0; }
                    else { IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf; }
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
            if(DecimalHalf==0&&IntValue==0)
                return true;
            else
                return false;
        }

		/// <summary>
        /// Basic Multiplication Operation(without checking for special representation variations or zero)
        /// </summary>
        /// <param name="Value">The value.</param>
		void BasicMultOp(MediumDecVariant& Value)
		{
			if(BasicMultOpPt1(Value))//Prevent multiplying into zero
/*#if defined(AltNum_EnableApproachingDivided)//Might adjust later to set to approaching zero in only certain situations(might be overkill to set to .0..1 in most cases)
			{	
				DecimalHalf = ApproachingBottomRep; ExtraRep = 0; 
			}
#else*/
				DecimalHalf = 1;
//#endif
		}

private:
		void CatchAllMultiplication(MediumDecVariant& Value, RepType& LRep, RepType& RRep)
		{
			ConvertToNumRep(LRep);
			Value.ConvertToNumRep(RRep);
			BasicMultOp(Value);
		}
		
		void CatchAllMultiplication(MediumDecVariant& Value, RepType& SameRep)
		{
			ConvertToNumRep(SameRep);
			Value.ConvertToNumRep(SameRep);
			BasicMultOp(Value);
		}
public:
        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& MultOp(MediumDecVariant& self, MediumDecVariant& Value);

		//Multiplies 2 AltNum variables together (Use normal AltNum + AltNum operation if need to use on 2 coPies of variables)
		static MediumDecVariant& MultOpV2(MediumDecVariant& self, MediumDecVariant Value)
		{
			return MultOp(self, Value);
		}
		
		/// <summary>
        /// Partial Multiplication Operation Between MediumDecVariant and Integer Value
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

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        void BasicIntMultOp(IntType& Value)
        {
            if (Value < 0)
            {
                if (Value == NegativeRep) { Value = 0; }
                else { Value *= -1; }
                SwapNegativeStatus();
            }
            if (IntValue == 0 && DecimalHalf == 0)
                return;
            if (Value == 0)
                SetAsZero();
            else
                PartialIntMultOp(Value);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        static MediumDecVariant& IntMultOp(MediumDecVariant& self, IntType& Value)
        {
            if (Value < 0)
            {
                if (Value == NegativeRep) { Value = 0; }
                else { Value *= -1; }
                self.SwapNegativeStatus();
            }
            if (self == Zero)
                return self;
            if (Value == 0)
            {
                self.SetAsZero(); return self;
            }
			switch (LRep)
			{
	#if defined(AltNum_EnablePiRep)
		#if defined(AltNum_EnablePiPowers)
		#endif
	#endif
	#if defined(AltNum_EnableERep)
	#endif
	#if defined(AltNum_EnableImaginaryNum)
	#endif
	#if defined(AltNum_EnableApproachingValues)
	#endif
	#if defined(AltNum_EnableFractionals)
	#endif
	#if defined(AltNum_EnableMixedFractional)

	#endif
	#if defined(AltNum_EnableNaN)
				case RepType::Undefined:
				case RepType::NaN:
					throw "Can't perform operations with NaN or Undefined number";
					break;
	#endif			
				default:
					self.BasicIntMultOp(Value);
					break;
			}
            return self;
        }

#ifdef AltNum_UseOldDivisionCode
        void PartialDivOp(MediumDecVariant& Value)
#else
        bool PartialDivOp(MediumDecVariant& Value)//Return true if divide into zero
#endif
        {
#ifdef AltNum_UseOldDivisionCode
            if (DecimalHalf == 0)
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
                if (Value.DecimalHalf == 0)//Both are integers
                {
                    __int64 SRep = IntValue * MediumDecVariant::DecimalOverflowX;
                    __int64 YRep = Value.IntValue;
                    SRep /= Value.IntValue;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntValue = IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? MediumDecVariant::NegativeRep : 0;
                    }
                    DecimalHalf = (signed int)SRep;
                }
                else//Only self is integer while Value has both sides
                {
                    boost::multiprecision::uint128_t SRep02 = MediumDecVariant::DecimalOverflowX * MediumDecVariant::DecimalOverflowX;
                    SRep02 *= IntValue;
                    __int64 VRep = MediumDecVariant::DecimalOverflowX * Value.IntValue + Value.DecimalHalf;
                    SRep02 /= VRep;
                    __int64 SRep = (__int64)SRep02;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntValue = (signed int)SelfIsNegative ? -OverflowVal : OverflowVal;
                    }
                    else
                    {
                        IntValue = 0;
                    }
                    DecimalHalf = (signed int)SRep;
                }
            }
            else if (IntValue == 0)
            {
                __int64 SRep = (__int64)DecimalHalf * MediumDecVariant::DecimalOverflowX;
                SRep /= Value.IntValue == 0 ? Value.DecimalHalf : MediumDecVariant::DecimalOverflowX * Value.IntValue + (__int64)Value.DecimalHalf;
                int IntHalf = SRep / MediumDecVariant::DecimalOverflowX;
                SRep -= IntHalf * MediumDecVariant::DecimalOverflowX;
                IntValue = IntHalf;
                DecimalHalf = (signed int)SRep;
            }
            else if (IntValue == MediumDecVariant::NegativeRep)
            {
                __int64 SRep = (__int64)DecimalHalf * MediumDecVariant::DecimalOverflowX;
                SRep /= Value.IntValue == 0 ? Value.DecimalHalf : MediumDecVariant::DecimalOverflowX * Value.IntValue + (__int64)Value.DecimalHalf;
                int IntHalf = SRep / MediumDecVariant::DecimalOverflowX;
                SRep -= IntHalf * MediumDecVariant::DecimalOverflowX;
                IntValue = IntHalf == 0 ? MediumDecVariant::NegativeRep : -IntHalf;
                DecimalHalf = (signed int)SRep;
            }
            else
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
                if (Value.DecimalHalf == 0)//Y is integer but self is not
                {
                    __int64 SRep = MediumDecVariant::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep /= Value.IntValue;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntValue = (signed int)SelfIsNegative ? -OverflowVal : OverflowVal;
                    }
                    else
                    {
                        IntValue = 0;
                    }
                    DecimalHalf = (signed int)SRep;
                }
                else
                {//Splitting Integer Half and Decimal Half Division
                    __int64 SRep_DecHalf = (__int64)DecimalHalf * MediumDecVariant::DecimalOverflowX;
                    SRep_DecHalf /= Value.IntValue == 0 ? Value.DecimalHalf : MediumDecVariant::DecimalOverflowX * Value.IntValue + (__int64)Value.DecimalHalf;
                    int IntHalf = SRep_DecHalf / MediumDecVariant::DecimalOverflowX;
                    SRep_DecHalf -= IntHalf * MediumDecVariant::DecimalOverflowX;

                    boost::multiprecision::uint128_t SRep02 = MediumDecVariant::DecimalOverflowX * MediumDecVariant::DecimalOverflowX;
                    //std::cout << "Multi-precision as String:" << SRep02 << std::endl;
                    SRep02 *= IntValue;
                    __int64 VRep = MediumDecVariant::DecimalOverflowX * Value.IntValue + Value.DecimalHalf;
                    SRep02 /= VRep;
                    __int64 SRep = (__int64)SRep02 + SRep_DecHalf;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntHalf += OverflowVal;
                    }
                    if (IntHalf == 0) { IntValue = (signed int)SelfIsNegative ? MediumDecVariant::NegativeRep : 0; }
                    else { IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf; }
                    DecimalHalf = (signed int)SRep;
                }
            }
#else//Instead use modulus based code to divide
			bool ResIsPositive = true;
			signed _int64 SelfRes;
			signed _int64 ValueRes;
			if(IntValue<0)
			{
			    SelfRes = IntValue==NegativeRep?DecimalHalf:IntValue*NegDecimalOverflowX+DecimalHalf;
			    if(Value<0)
					ValueRes = Value.IntValue==NegativeRep?DecimalHalf:Value.IntValue*NegDecimalOverflowX+Value.DecimalHalf;
				else
				{
				    ResIsPositive = false;
					ValueRes = Value.IntValue*DecimalOverflowX+Value.DecimalHalf;
				}
			}
			else
			{
				SelfRes = IntValue*DecimalOverflowX+DecimalHalf;
			    if(Value<0)
				{
				    ResIsPositive = false;
					ValueRes = Value.IntValue==NegativeRep?DecimalHalf:IntValue*NegDecimalOverflowX+Value.DecimalHalf;
				}
				else
					ValueRes = Value.IntValue*DecimalOverflowX+Value.DecimalHalf;
			}
			
			signed _int64 IntHalfRes = SelfRes / ValueRes;
			signed _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
			IntValue = IntHalfRes==0&&ResIsPositive==false?NegativeRep:IntHalfRes;
			DecimalHalf = DecimalRes;
			if(IntHalfRes==0&&DecimalRes==0)
				return true;
			else
				return false;
#endif
        }
		
        void BasicDivOp(MediumDecVariant& Value)
        {
#if defined(AltNum_UseOldDivisionCode)
            if (Value.IntValue < 0)
            {
                if (Value.IntValue == MediumDecVariant::NegativeRep) { Value.IntValue = 0; }
                else { Value.IntValue *= -1; }
                SwapNegativeStatus();
            }
#endif
            PartialDivOp(Value);
#if defined(AltNum_UseOldDivisionCode)
			if ((IntValue==NegativeRep||IntValue==0)&&DecimalHalf==0)//Prevent Dividing into nothing
#else
			if (PartialDivOp(Value))//Prevent Dividing into nothing
#endif
#if defined(AltNum_EnableApproachingDivided)
			{	DecimalHalf = ApproachingBottomRep; ExtraRep = 0; }
#else
				DecimalHalf = 1;
#endif
        }

private:
	void CatchAllDivision(MediumDecVariant& Value, RepType& LRep, RepType& RRep)
	{
		ConvertToNumRep(LRep);
		Value.ConvertToNumRep(RRep);
		BasicDivOp(Value);
	}
	
	void CatchAllDivision(MediumDecVariant& Value, RepType& SameRep)
	{
		ConvertToNumRep(SameRep);
		Value.ConvertToNumRep(SameRep);
		BasicDivOp(Value);
	}
	
 //   void CatchAllDivision(MediumDecVariant& Value)
 //   {
 //       ConvertToNumRep();
 //       Value.ConvertToNumRep();
 //       BasicDivOp(Value);
 //   }
public:
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& DivOp(MediumDecVariant& self, MediumDecVariant& Value);

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
        void BasicIntDivOp(IntType& Value)
        {
            if (Value == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                IntValue < 0 ? SetAsNegativeInfinity() : SetAsInfinity(); return;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (IntValue == 0 && DecimalHalf == 0)
                return;
            if (IntValue < 0)
            {
                Value *= -1;
                SwapNegativeStatus();
            }
            PartialIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
        }

        /// <summary>
        /// Division Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& IntDivOp(MediumDecVariant& self, IntType& Value)
        {
            if (Value == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                self.IntValue < 0 ? self.SetAsNegativeInfinity() : self.SetAsInfinity(); return self;
#else
                throw "Target value can not be divided by zero";
#endif
            }
            else if (self == Zero)
                return self;
            if (Value < 0)
            {
                if (Value == NegativeRep) { Value = 0; }
                else { Value *= -1; }
                self.SwapNegativeStatus();
            }
            if (self.ExtraRep == 0)
            {
                self.ExtraRep = Value;
            }
            else if (self.ExtraRep == NegativeRep)//Value*Pi Representation
            {
                self.PartialIntDivOp(Value);
            }
#if defined(AltNum_EnableImaginaryNum) || defined(AltNum_EnableERep)
            else if (self.ExtraRep == IERep)
            {
            }
            else if (self.ExtraRep > 0)
#else
            else//(Value/ExtraRep) Representation
#endif
            {
                self.ExtraRep *= Value;
            }
#if defined(AltNum_EnableImaginaryNum) || defined(AltNum_EnableERep)
            else
            {

            }
#endif
            if (self == Zero) { return JustAboveZero; }//Prevent dividing into nothing
            return self;
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedMultOp(MediumDecVariant& self, IntType& Value)
        {
            if (self == Zero) {}
            else if (Value == 0) { self.IntValue = 0; self.DecimalHalf = 0; }
            else if (self.DecimalHalf == 0)
            {
                self.IntValue *= Value;
            }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                if (SelfIsNegative)
                {
                    if (self.IntValue == NegativeRep) { self.IntValue = 0; }
                    else { self.IntValue *= -1; }
                }
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.IntValue + self.DecimalHalf;
                SRep *= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    self.IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = SelfIsNegative ? NegativeRep : 0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            return self;
        }

        /// <summary>
        /// Division Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedDivOp(MediumDecVariant& self, IntType& Value)
        {
            if (Value == 0) { throw "Target value can not be divided by zero"; }
            else if (self == Zero) { return self; }
            if (self.DecimalHalf == 0)
            {
                bool SelfIsNegative = self.IntValue < 0;
                if (SelfIsNegative)
                    self.IntValue *= -1;
                __int64 SRep = DecimalOverflowX * self.IntValue;
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflow;
                    SRep -= OverflowVal * DecimalOverflow;
                    self.IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = SelfIsNegative ? NegativeRep : 0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                if (SelfIsNegative)
                {
                    if (self.IntValue == NegativeRep) { self.IntValue = 0; }
                    else { self.IntValue *= -1; }
                }
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.IntValue + self.DecimalHalf;
                SRep /= Value;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= DecimalOverflowX * OverflowVal;
                    self.IntValue = (signed int)SelfIsNegative ? OverflowVal * -1 : OverflowVal;
                    self.DecimalHalf = (signed int)SRep;
                }
                else
                {
                    self.IntValue = 0;
                    self.DecimalHalf = (signed int)SRep;
                }
            }
            return self;
        }
#pragma endregion Multiplication/Division Operations

#pragma region Remainder Operations
        /// <summary>
        /// Remainder Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& RemOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
#ifdef AltNum_UseOldRemOpCode
            bool SelfIsWholeN = self.DecimalHalf == 0;
            bool ValueIsWholeN = Value.DecimalHalf == 0;
            if (Value.IntValue == 0 && ValueIsWholeN) { self.SetAsZero(); return self; }//Return zero instead of N/A
            RepType LRep = self.GetRepType();
            RepType RRep = Value.GetRepType();
            if(LRep==RRep&&self.DecimalHalf>=0&&Value.DecimalHalf>=0)
            {
				if (SelfIsWholeN && ValueIsWholeN)//WholeNumbers
				{
					self.IntValue %= Value.IntValue;
				}
				else if (ValueIsWholeN)
				{
					self %= Value.IntValue;
				}
				else
				{
					//bool ValueIsNegative = Value.IntValue < 0;
					if (Value.IntValue < 0)
					{
						self.IntValue *= -1;
						if (Value.IntValue == NegativeRep) { Value.IntValue = 0; }
					}
					bool SelfIsNegative = self.IntValue < 0;
					if (SelfIsNegative)
					{
						if (self.IntValue == NegativeRep) { self.IntValue = 0; }
						else { self.IntValue *= -1; }
					}
					__int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.IntValue + self.DecimalHalf;
					__int64 SRep_DecHalf = SRep;
					__int64 VRep = DecimalOverflowX * Value.IntValue + Value.DecimalHalf;;
					SRep %= VRep;
					__int64 IntResult = SRep;
					//Int Half Calculated now get decimal digits that got truncated off
					SRep_DecHalf -= IntResult * VRep;
					//Gives enough buffer room that doesn't lose the decimal values
					SRep_DecHalf *= DecimalOverflowX;
					SRep_DecHalf %= VRep;
					if (IntResult == 0) { self.IntValue = (signed int)SelfIsNegative ? NegativeRep : 0; }
					else { self.IntValue = (signed int)SelfIsNegative ? IntResult * -1 : IntResult; }
					self.DecimalHalf = (signed int)SRep;
				}
			}
			else
			{
				if (self.IntValue < 0)
				{
					MediumDecVariant DivRes = Self / Value;
					self = self - Value * IntHalfRes;//RemResult
					self = Value - self;
				}
				else
				{
					MediumDecVariant DivRes = Self / Value;
					self = self - Value * IntHalfRes;//RemResult
				}
			}
			
#else
			if (self.IntValue < 0)
			{
				MediumDecVariant DivRes = Self / Value;
				self = self - Value * IntHalfRes;//RemResult
				self = Value - self;
			}
			else
			{
				MediumDecVariant DivRes = Self / Value;
				self = self - Value * IntHalfRes;//RemResult
			}
#endif
            return self;
        }
		
		ModRes PerformModOp(MediumDecVariant& self, IntType& Value)
		{
			ModRes Result;
			if (self.IntValue < 0)
			{
				ModRes.DivRes = Self / Value;
				ModRes.RemRes = self - Value * IntHalfRes;//RemResult
				ModRes.RemRes = Value - self;
			}
			else
			{
				ModRes.DivRes = Self / Value;
				ModRes.RemRes = self - Value * IntHalfRes;//RemResult
			}
			return Result;
		}

        /// <summary>
        /// Remainder/Modulus Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& IntRemOp(MediumDecVariant& self, IntType& Value)
        {
            if (Value == 0 || self == MediumDecVariant::Zero) { self.SetAsZero(); return self; }
            if (self.DecimalHalf == 0)
            {
                if (self.IntValue < 0)//https://www.quora.com/How-does-the-modulo-operation-work-with-negative-numbers-and-why
                {
                    self.IntValue %= Value;
                    self.IntValue = (signed int)(Value - self.IntValue);
                }
                else
                {
                    self.IntValue %= Value; return self;
                }
            }
            else//leftValue is non-whole number
            {
#ifdef AltNum_UseOldRemOpCode
                if (Value < 0) { self.SwapNegativeStatus(); Value *= -1; }
                bool SelfIsNegative = self.IntValue < 0;
                if (SelfIsNegative)
                {
                    if (self.IntValue == NegativeRep) { self.IntValue = 0; }
                    else { self.IntValue *= -1; }
                }
                __int64 SRep = self.IntValue == 0 ? self.DecimalHalf : DecimalOverflowX * self.IntValue + self.DecimalHalf;
                SRep %= Value;
                __int64 VRep = DecimalOverflowX * Value;
                SRep /= VRep;
                __int64 IntResult = SRep;
                SRep = ((__int64)self.IntValue * DecimalOverflow) + self.DecimalHalf;
                SRep -= IntResult * VRep;
                __int64 IntHalf = SRep / DecimalOverflow;
                SRep -= IntHalf * (__int64)DecimalOverflow;
                if (IntHalf == 0) { self.IntValue = SelfIsNegative ? (signed int)NegativeRep : 0; }
                else { self.IntValue = (signed int)(SelfIsNegative ? IntHalf * -1 : IntHalf); }
                self.DecimalHalf = (signed int)SRep;
#else
				if (self.IntValue < 0)
				{
					MediumDecVariant DivRes = Self / Value;
					self = self - Value * IntHalfRes;//RemResult
					self = Value - self;
				}
				else
				{
					MediumDecVariant DivRes = Self / Value;
					self = self - Value * IntHalfRes;//RemResult
				}
#endif
            }
            return self;
        }

        /// <summary>
        /// Remainder/Modulus Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        template<typename IntType>
        static MediumDecVariant& UnsignedRemOp(MediumDecVariant& self, IntType& Value)
        {
            if (self == MediumDecVariant::Zero) { return self; }
            if (Value == 0) { self.IntValue = 0; self.DecimalHalf = 0; return self; }
            if (self.DecimalHalf == 0)
            {
                self.IntValue %= Value; return self;
            }
            else//leftValue is non-whole number
            {
#ifdef AltNum_UseOldRemOpCode
                __int64 SRep;
                if (self.IntValue == NegativeRep) { SRep = (__int64)self.DecimalHalf * -1; }
                else if (self.IntValue < 0) { SRep = DecimalOverflowX * self.IntValue - self.DecimalHalf; }
                else { SRep = DecimalOverflowX * self.IntValue + self.DecimalHalf; }
                bool SelfIsNegative = SRep < 0;
                if (SelfIsNegative) { SRep *= -1; }
                SRep %= Value;
                __int64 VRep = DecimalOverflowX * Value;
                SRep /= VRep;
                __int64 IntResult = SRep;
                SRep = ((__int64)self.IntValue * DecimalOverflow) + self.DecimalHalf;
                SRep -= IntResult * VRep;
                __int64 IntHalf = SRep / DecimalOverflow;
                SRep -= IntHalf * (__int64)DecimalOverflow;
                if (IntHalf == 0) { self.IntValue = (signed int)SelfIsNegative ? NegativeRep : 0; }
                else { self.IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf; }
                self.DecimalHalf = (signed int)SRep;
#else
				if (self.IntValue < 0)
				{
					MediumDecVariant DivRes = Self / Value;
					self = self - Value * IntHalfRes;//RemResult
					self = Value - self;
				}
				else
				{
					MediumDecVariant DivRes = Self / Value;
					self = self - Value * IntHalfRes;//RemResult
				}
#endif
            }
            return self;
        }
#pragma endregion Remainder Operations

#pragma region MediumDecVariant-To-MediumDecVariant Operators
    public:
        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator+(MediumDecVariant self, MediumDecVariant Value)
        {
            return AddOp(self, Value);
        }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator+=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return AddOp(self, Value);
        }
        
        /// <summary>
        /// += Operation(from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator+=(MediumDecVariant* self, MediumDecVariant Value){ return AddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator-(MediumDecVariant self, MediumDecVariant Value)
        {
            return SubOp(self, Value);
        }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator-=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return SubOp(self, Value);
        }
        
        /// <summary>
        /// -= Operation(from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator-=(MediumDecVariant* self, MediumDecVariant Value){ return SubOp(**self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator*(MediumDecVariant self, MediumDecVariant Value)
        {
            return MultOp(self, Value);
        }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltDec</returns>
        friend MediumDecVariant& operator*=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return MultOp(self, Value);
        }

        ///// <summary>
        ///// *= Operation (from pointer)
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltDec</returns>
        friend MediumDecVariant& operator*=(MediumDecVariant* self, MediumDecVariant Value){ return MultOp(**self, Value); }


        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator/(MediumDecVariant self, MediumDecVariant Value)
        {
            return DivOp(self, Value);
        }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator/=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return DivOp(self, Value);
        }
        
        /// <summary>
        /// /= Operation (from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator/=(MediumDecVariant* self, MediumDecVariant Value){ return DivOp(**self, Value); }

        /// <summary>
        /// Remainder Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator%(MediumDecVariant self, MediumDecVariant Value)
        {
            return RemOp(self, Value);
        }

        /// <summary>
        /// %= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator%=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return RemOp(self, Value);
        }
        
        /// <summary>
        /// %= Operation (from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator%=(MediumDecVariant* self, MediumDecVariant Value)
        {
            return RemOp(**self, Value);
        }

        /// <summary>
        /// XOR Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator^(MediumDecVariant self, MediumDecVariant Value)
        {
            if (self.DecimalHalf == 0 && Value.DecimalHalf == 0)//Whole Numbers
            {
                self.IntValue ^= Value.IntValue; return self;
            }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValIsNegative = Value.IntValue < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 ^ Value.IntValue) * -1;
                    self.DecimalHalf ^= Value.DecimalHalf;
                }
                else
                {
                    if (ValIsNegative && Value.IntValue == NegativeRep)
                    {
                        self.IntValue = (self.IntValue ^ 0) * -1;
                        self.DecimalHalf ^= Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue ^= Value.IntValue; self.DecimalHalf ^= Value.DecimalHalf;
                    }
                }
            }
            return self;
        }

        /// <summary>
        /// Bitwise Or Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator|(MediumDecVariant self, MediumDecVariant Value)
        {
            if (self.DecimalHalf == 0 && Value.DecimalHalf == 0)//Whole Numbers
            {
                self.IntValue |= Value.IntValue; return self;
            }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValIsNegative = Value.IntValue < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 | Value.IntValue) * -1;
                    self.DecimalHalf |= Value.DecimalHalf;
                }
                else
                {
                    if (ValIsNegative && Value.IntValue == NegativeRep)
                    {
                        self.IntValue = (self.IntValue & 0) * -1;
                        self.DecimalHalf |= Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue |= Value.IntValue; self.DecimalHalf |= Value.DecimalHalf;
                    }
                }
            }
            return self;
        }
        /// <summary>
        /// Bitwise And Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator&(MediumDecVariant self, MediumDecVariant Value)
        {
            if (self.DecimalHalf == 0 && Value.DecimalHalf == 0)//Whole Numbers
            {
                self.IntValue &= Value.IntValue; return self;
            }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValIsNegative = Value.IntValue < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 & Value.IntValue) * -1;
                    self.DecimalHalf &= Value.DecimalHalf;
                }
                else
                {
                    if (ValIsNegative && Value.IntValue == NegativeRep)
                    {
                        self.IntValue = (self.IntValue & 0) * -1;
                        self.DecimalHalf &= Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue &= Value.IntValue; self.DecimalHalf &= Value.DecimalHalf;
                    }
                }
            }
            return self;
        }

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator-(MediumDecVariant& self)
        {
            self.SwapNegativeStatus(); return self;
        }

        /// <summary>
        /// ++MediumDecVariant Operator
        /// </summary>
        /// <returns>MediumDecVariant &</returns>
        MediumDecVariant& operator ++()
        {
#if defined(AltNum_EnableInfinityRep)
		    if(DecimalHalf==InfinityRep)
			    return *this;
#endif
            if (IntValue == NegativeRep) { IntValue = 0; }
            else if (DecimalHalf == 0) { ++IntValue; }
            else if (IntValue == -1) { IntValue = NegativeRep; }
            else { ++IntValue; }
            return *this;
        }

        /// <summary>
        /// ++MediumDecVariant Operator
        /// </summary>
        /// <returns>MediumDecVariant &</returns>
        MediumDecVariant& operator --()
        {
#if defined(AltNum_EnableInfinityRep)
		    if(DecimalHalf==InfinityRep)
			    return *this;
#endif
            if (IntValue == NegativeRep) { IntValue = -1; }
            else if (DecimalHalf == 0) { --IntValue; }
            else if (IntValue == 0) { IntValue = NegativeRep; }
            else { --IntValue; }
            return *this;
        }

        /// <summary>
        /// MediumDecVariant++ Operator
        /// </summary>
        /// <returns>AltDec</returns>
        MediumDecVariant operator ++(int)
        {
            MediumDecVariant tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecVariant-- Operator
        /// </summary>
        /// <returns>AltDec</returns>
        MediumDecVariant operator --(int)
        {
            MediumDecVariant tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDecVariant* Operator
        /// </summary>
        /// <returns>MediumDecVariant &</returns>
        MediumDecVariant& operator *()
        {
            return *this;
        }

    #pragma endregion MediumDecVariant-To-MediumDecVariant Operators

    #pragma region MediumDecVariant-To-Int Comparison Operators
        /// <summary>
        /// Equality Operation between <see cref="MediumDecVariant &"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator==(MediumDecVariant self, IntType Value)
        {
            return (self.IntValue == Value && self.DecimalHalf == 0 && self.ExtraRep==0);
        }

        /// <summary>
        /// != Operation between <see cref="MediumDecVariant"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator!=(MediumDecVariant self, IntType Value)
        {
            if (self.DecimalHalf!=0||self.ExtraRep!=0)
                return true;
            else if (self.IntValue == Value)
                return false;
            else
                return true;
        }

        /// <summary>
        /// Lesser than Operation between <see cref="MediumDecVariant"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<(MediumDecVariant self, IntType Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==-1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PiRep)
            {
                self.ConvertToNumRep();
            }
            else
                throw "Can't compare real values against imaginary.";
#else
            self.ConvertToNumRep();
#endif
            if (self.DecimalHalf == 0)
            {
                return self.IntValue < Value;
            }
            else
            {
                if (self.IntValue == NegativeRep)
                {//-0.5<0
                    if (Value >= 0)
                        return true;
                }
                else if (self.IntValue < Value) { return true; }//5.5 < 6
                else if (self.IntValue == Value) { return self.IntValue < 0 ? true : false; }//-5.5<-5 vs 5.5 > 5
            }
            return false;
        }
        /// <summary>
        /// Lesser than or equal to operation between <see cref="MediumDecVariant"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<=(MediumDecVariant self, IntType Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==-1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PiRep)
            {
                self.ConvertToNumRep();
            }
            else
                throw "Can't compare real values against imaginary.";
#else
            self.ConvertToNumRep();
#endif
            if (self.DecimalHalf == 0)
            {
                return self.IntValue <= Value;
            }
            else
            {
                if (self.IntValue == NegativeRep)
                {//-0.5<0
                    if (Value >= 0)
                        return true;
                }
                else if (self.IntValue < Value) { return true; }//5.5<=6
                else if (self.IntValue == Value) { return self.IntValue < 0 ? true : false; }
            }
            return false;
        }
        /// <summary>
        /// Greater than operation between <see cref="MediumDecVariant"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>(MediumDecVariant self, IntType Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PiRep)
            {
                self.ConvertToNumRep();
            }
            else
                throw "Can't compare real values against imaginary.";
#else
            self.ConvertToNumRep();
#endif
            if (self.DecimalHalf == 0)
            {
                return self.IntValue > Value;
            }
            else
            {
                if (self.IntValue == NegativeRep)
                {//-0.5>-1
                    if (Value <= -1)
                        return true;
                }
                else if (self.IntValue > Value) { return true; }
                else if (self.IntValue == Value) { return self.IntValue < 0 ? false : true; }
            }
            return false;
        }
        /// <summary>
        /// Greater than or equal to operation between <see cref="MediumDecVariant"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>=(MediumDecVariant self, IntType Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PiRep)
            {
                self.ConvertToNumRep();
            }
            else
                throw "Can't compare real values against imaginary.";
#else
            self.ConvertToNumRep();
#endif
            if (self.DecimalHalf == 0)
            {
                return self.IntValue >= Value;
            }
            else
            {
                if (self.IntValue == NegativeRep)
                {
                    if (Value <= -1)
                        return true;
                }
                else if (self.IntValue > Value) { return true; }
                else if (self.IntValue == Value) { return self.IntValue < 0 ? false : true; }//-5.5<-5 vs 5.5>5
            }
            return false;
        }

        /// <summary>
        /// Equality Operation between Integer Type and <see cref="MediumDecVariant"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator==(IntType Value, MediumDecVariant self)
        {
            return (self.IntValue == Value && self.DecimalHalf == 0 && self.ExtraRep==0);
        }

        /// <summary>
        /// != Operation between Integer Type and <see cref="MediumDecVariant"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator!=(IntType Value, MediumDecVariant self)
        {
            if (self.DecimalHalf!=0||self.ExtraRep!=0)
                return true;
            else if (self.IntValue == Value)
                return false;
            else
                return true;
        }

        /// <summary>
        /// Lesser than Operation between Integer Type and <see cref="MediumDecVariant"/> .
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<(IntType Value, MediumDecVariant self)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PiRep)
            {
                self.ConvertToNumRep();
            }
            else
                throw "Can't compare real values against imaginary.";
#else
            self.ConvertToNumRep();
#endif
            if (self.DecimalHalf == 0)
            {
                return Value < self.IntValue;
            }
            else
            {
                if (self.IntValue == NegativeRep)
                {
                    if (Value <= -1)
                        return true;
                }
                else if (Value < self.IntValue) { return true; }// 5 < 6.5
                else if (self.IntValue == Value) { return self.IntValue < 0 ? false : true; }//5 < 5.5 vs -5 > -5.5
            }
            return false;
        }
        /// <summary>
        /// Lesser than or equal to operation between Integer Type and <see cref="MediumDecVariant"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<=(IntType Value, MediumDecVariant self)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PiRep)
            {
                self.ConvertToNumRep();
            }
            else
                throw "Can't compare real values against imaginary.";
#else
            self.ConvertToNumRep();
#endif
            if (self.DecimalHalf == 0)
            {
                return Value <= self.IntValue;
            }
            else
            {
                if (self.IntValue == NegativeRep && Value <= 1)
                {//-1<-0.5
                    if (Value <= -1)
                        return true;
                }
                else if (Value < self.IntValue) { return true; }
                else if (self.IntValue == Value) { return self.IntValue < 0 ? false : true; }//5 <= 5.5 vs -5 >= -5.5
            }
            return false;
        }
        /// <summary>
        /// Greater than operation between Integer Type and <see cref="MediumDecVariant"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>(IntType Value, MediumDecVariant self)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return false;
                else
                    return true;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PiRep)
            {
                self.ConvertToNumRep();
            }
            else
                throw "Can't compare real values against imaginary.";
#else
            self.ConvertToNumRep();
#endif
            if (self.DecimalHalf == 0)
            {
                return Value > self.IntValue;
            }
            else
            {
                if (self.IntValue == NegativeRep)
                {
                    if (Value >= 0)
                        return true;
                }
                else if (Value > self.IntValue) { return true; }
                else if (self.IntValue == Value) { return self.IntValue < 0 ? true : false; }//5 < 5.5 vs -5 > -5.5
            }
            return false;
        }
        /// <summary>
        /// Greater than or equal to operation between <see cref="MediumDecVariant"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>=(IntType Value, MediumDecVariant self)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return false;
                else
                    return true;
            }
#endif
#if defined(AltNum_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PiRep)
            {
                self.ConvertToNumRep();
            }
            else
                throw "Can't compare real values against imaginary.";
#else
            self.ConvertToNumRep();
#endif
            if (self.DecimalHalf == 0)
            {
                return Value >= self.IntValue;
            }
            else
            {
                if (self.IntValue == NegativeRep)
                {//0>-0.5
                    if (Value >= 0)
                        return true;
                }
                else if (Value > self.IntValue) { return true; }
                else if (self.IntValue == Value) { return self.IntValue < 0 ? true : false; }//5 <= 5.5 vs -5 >= -5.5
            }
            return false;
        }

    #pragma endregion MediumDecVariant-To-Int Comparison Operators
    #pragma region Other Operations
        friend MediumDecVariant operator+(MediumDecVariant self, float Value) { return self + (MediumDecVariant)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, float Value) { return self - (MediumDecVariant)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, float Value) { return self * (MediumDecVariant)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, float Value) { return self / (MediumDecVariant)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, float Value) { return self % (MediumDecVariant)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, float Value) { return self ^ (MediumDecVariant)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, float Value) { return self | (MediumDecVariant)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, float Value) { return self & (MediumDecVariant)Value; }

        friend MediumDecVariant operator+(float Value, MediumDecVariant self) { return (MediumDecVariant)Value + self; }
        friend MediumDecVariant operator-(float Value, MediumDecVariant self) { return (MediumDecVariant)Value - self; }
        friend MediumDecVariant operator*(float Value, MediumDecVariant self) { return (MediumDecVariant)Value * self; }
        friend MediumDecVariant operator/(float Value, MediumDecVariant self) { return (MediumDecVariant)Value / self; }
        friend MediumDecVariant operator%(float Value, MediumDecVariant self) { return (MediumDecVariant)Value % self; }
        friend MediumDecVariant operator^(float Value, MediumDecVariant self) { return (MediumDecVariant)Value ^ self; }
        friend MediumDecVariant operator|(float Value, MediumDecVariant self) { return (MediumDecVariant)Value | self; }
        friend MediumDecVariant operator&(float Value, MediumDecVariant self) { return (MediumDecVariant)Value & self; }

        friend MediumDecVariant operator+(MediumDecVariant self, double Value) { return self + (MediumDecVariant)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, double Value) { return self - (MediumDecVariant)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, double Value) { return self * (MediumDecVariant)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, double Value) { return self / (MediumDecVariant)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, double Value) { return self % (MediumDecVariant)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, double Value) { return self ^ (MediumDecVariant)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, double Value) { return self | (MediumDecVariant)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, double Value) { return self & (MediumDecVariant)Value; }

        friend MediumDecVariant operator+(double Value, MediumDecVariant self) { return (MediumDecVariant)Value + self; }
        friend MediumDecVariant operator-(double Value, MediumDecVariant self) { return (MediumDecVariant)Value - self; }
        friend MediumDecVariant operator*(double Value, MediumDecVariant self) { return (MediumDecVariant)Value * self; }
        friend MediumDecVariant operator/(double Value, MediumDecVariant self) { return (MediumDecVariant)Value / self; }
        friend MediumDecVariant operator%(double Value, MediumDecVariant self) { return (MediumDecVariant)Value % self; }
        friend MediumDecVariant operator^(double Value, MediumDecVariant self) { return (MediumDecVariant)Value ^ self; }
        friend MediumDecVariant operator|(double Value, MediumDecVariant self) { return (MediumDecVariant)Value | self; }
        friend MediumDecVariant operator&(double Value, MediumDecVariant self) { return (MediumDecVariant)Value & self; }

        friend MediumDecVariant operator+(MediumDecVariant self, ldouble Value) { return self + (MediumDecVariant)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, ldouble Value) { return self - (MediumDecVariant)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, ldouble Value) { return self * (MediumDecVariant)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, ldouble Value) { return self / (MediumDecVariant)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, ldouble Value) { return self % (MediumDecVariant)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, ldouble Value) { return self ^ (MediumDecVariant)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, ldouble Value) { return self | (MediumDecVariant)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, ldouble Value) { return self & (MediumDecVariant)Value; }

        friend MediumDecVariant operator+(ldouble Value, MediumDecVariant self) { return (MediumDecVariant)Value + self; }
        friend MediumDecVariant operator-(ldouble Value, MediumDecVariant self) { return (MediumDecVariant)Value - self; }
        friend MediumDecVariant operator*(ldouble Value, MediumDecVariant self) { return (MediumDecVariant)Value * self; }
        friend MediumDecVariant operator/(ldouble Value, MediumDecVariant self) { return (MediumDecVariant)Value / self; }
        friend MediumDecVariant operator%(ldouble Value, MediumDecVariant self) { return (MediumDecVariant)Value % self; }
        friend MediumDecVariant operator^(ldouble Value, MediumDecVariant self) { return (MediumDecVariant)Value ^ self; }
        friend MediumDecVariant operator|(ldouble Value, MediumDecVariant self) { return (MediumDecVariant)Value | self; }
        friend MediumDecVariant operator&(ldouble Value, MediumDecVariant self) { return (MediumDecVariant)Value & self; }

    #pragma endregion Other Operations
    #pragma region MediumDecVariant-To-Integer Operations
    public:
        friend MediumDecVariant operator+(MediumDecVariant self, unsigned char Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-(MediumDecVariant self, unsigned char Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*(MediumDecVariant self, unsigned char Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/(MediumDecVariant self, unsigned char Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%(MediumDecVariant self, unsigned char Value){ return UnsignedRemOp(self, Value); }
        
        friend MediumDecVariant operator+=(MediumDecVariant& self, unsigned char Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-=(MediumDecVariant& self, unsigned char Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*=(MediumDecVariant& self, unsigned char Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/=(MediumDecVariant& self, unsigned char Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%=(MediumDecVariant& self, unsigned char Value){ return UnsignedRemOp(self, Value); }

        friend MediumDecVariant operator+(MediumDecVariant self, unsigned short Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-(MediumDecVariant self, unsigned short Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*(MediumDecVariant self, unsigned short Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/(MediumDecVariant self, unsigned short Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%(MediumDecVariant self, unsigned short Value){ return UnsignedRemOp(self, Value); }
        
        friend MediumDecVariant operator+=(MediumDecVariant& self, unsigned short Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-=(MediumDecVariant& self, unsigned short Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*=(MediumDecVariant& self, unsigned short Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/=(MediumDecVariant& self, unsigned short Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%=(MediumDecVariant& self, unsigned short Value){ return UnsignedRemOp(self, Value); }

        friend MediumDecVariant operator+(MediumDecVariant self, unsigned int Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-(MediumDecVariant self, unsigned int Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*(MediumDecVariant self, unsigned int Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/(MediumDecVariant self, unsigned int Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%(MediumDecVariant self, unsigned int Value){ return UnsignedRemOp(self, Value); }
        
        friend MediumDecVariant operator+=(MediumDecVariant& self, unsigned int Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-=(MediumDecVariant& self, unsigned int Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*=(MediumDecVariant& self, unsigned int Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/=(MediumDecVariant& self, unsigned int Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%=(MediumDecVariant& self, unsigned int Value){ return UnsignedRemOp(self, Value); }
        
        //friend MediumDecVariant operator+=(MediumDecVariant* self, unsigned int Value) { return UnsignedAddOp(**self, Value); }
        //friend MediumDecVariant operator-=(MediumDecVariant* self, unsigned int Value) { return UnsignedSubOp(**self, Value); }
        //friend MediumDecVariant operator*=(MediumDecVariant* self, unsigned int Value) { return UnsignedMultOp(**self, Value); }
        //friend MediumDecVariant operator/=(MediumDecVariant* self, unsigned int Value) { return UnsignedDivOp(**self, Value); }
        //friend MediumDecVariant operator%=(MediumDecVariant* self, unsigned int Value) { return UnsignedRemOp(**self, Value); }

        friend MediumDecVariant operator+(MediumDecVariant self, unsigned __int64 Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-(MediumDecVariant self, unsigned __int64 Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*(MediumDecVariant self, unsigned __int64 Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/(MediumDecVariant self, unsigned __int64 Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%(MediumDecVariant self, unsigned __int64 Value){ return UnsignedRemOp(self, Value); }
        
        friend MediumDecVariant operator+=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedAddOp(self, Value); }
        friend MediumDecVariant operator-=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedSubOp(self, Value); }
        friend MediumDecVariant operator*=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedMultOp(self, Value); }
        friend MediumDecVariant operator/=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedDivOp(self, Value); }
        friend MediumDecVariant operator%=(MediumDecVariant& self, unsigned __int64 Value){ return UnsignedRemOp(self, Value); }

     //   friend MediumDecVariant operator+=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedAddOp(**self, Value); }
        //friend MediumDecVariant operator-=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedSubOp(**self, Value); }
     //   friend MediumDecVariant operator*=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedMultOp(**self, Value); }
        //friend MediumDecVariant operator/=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedDivOp(**self, Value); }
     //   friend MediumDecVariant operator%=(MediumDecVariant* self, unsigned __int64 Value){ return UnsignedRemOp(**self, Value); }

        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator+(MediumDecVariant self, IntType Value)
        {
            return IntAddOp(self, Value);
        }

        ///// <summary>
        ///// += Operation Between MediumDecVariant and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator+=(MediumDecVariant& self, IntType Value)
        {
            return IntAddOp(self, Value);
        }

        template<typename IntType>
        friend MediumDecVariant operator+=(MediumDecVariant* self, IntType Value){ return IntAddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator-(MediumDecVariant self, IntType Value)
        {
            return IntSubOp(self, Value);
        }

        /// <summary>
        /// -= Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator-=(MediumDecVariant& self, IntType Value)
        {
            return IntSubOp(self, Value);
        }

        template<typename IntType>
        friend MediumDecVariant operator-=(MediumDecVariant* self, IntType Value){ return IntSubOp(**self, Value); }

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator*(MediumDecVariant self, IntType Value)
        {
            return IntMultOp(self, Value);
        }

        /// <summary>
        /// *= Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator*=(MediumDecVariant& self, IntType Value)
        {
            return IntMultOp(self, Value);
        }

        /// <summary>
        /// *= Operation Between MediumDecVariant and Integer Value (from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator*=(MediumDecVariant* self, IntType Value){ return IntMultOp(**self, Value); }

        /// <summary>
        /// Division Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator/(MediumDecVariant self, IntType Value)
        {
            return IntDivOp(self, Value);
        }

        /// <summary>
        /// /= Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator/=(MediumDecVariant& self, IntType Value)
        {
            return IntDivOp(self, Value);
        }

        template<typename IntType>
        friend MediumDecVariant operator/=(MediumDecVariant* self, IntType Value){ return IntDivOp(**self, Value); }
        
        /// <summary>
        /// Modulus Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator%(MediumDecVariant self, IntType Value)
        {
            return IntRemOp(self, Value);
        }

        /// <summary>
        /// %= Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator%=(MediumDecVariant& self, IntType Value)
        {
            return IntRemOp(self, Value);
        }

        template<typename IntType>
        friend MediumDecVariant operator%=(MediumDecVariant* self, IntType Value){ return IntRemOp(**self, Value); }

        /// <summary>
        /// Bitwise XOR Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator^(MediumDecVariant self, IntType Value)
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
        /// Bitwise Or Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator|(MediumDecVariant self, IntType Value)
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

        /// <summary>
        /// Bitwise And Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator&(MediumDecVariant self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntValue &= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntValue == NegativeRep)
                {
                    self.IntValue = (0 & Value) * -1;
                    self.DecimalHalf &= Value;
                }
                else
                {
                    self.IntValue &= Value; self.DecimalHalf &= Value;
                }
            }
            return self;
        }
    #pragma endregion MediumDecVariant-To-Integer Operations
    #pragma region Integer-To-MediumDecVariant Operations
        /// <summary>
        /// Addition Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator+(IntType Value, MediumDecVariant self)
        {
            return IntAddOp(self, Value);
        }

        /// <summary>
        /// Subtraction Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator-(IntType Value, MediumDecVariant self)
        {
            MediumDecVariant Result = Value;
            return SubOp(Result, self);
        }

        /// <summary>
        /// Multiplication Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator*(IntType Value, MediumDecVariant self)
        {
            return IntMultOp(self, Value);
        }

        /// <summary>
        /// Division Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator/(IntType Value, MediumDecVariant self)
        {
            MediumDecVariant Result = Value;
            return DivOp(Result, self);
        }

        /// <summary>
        /// Remainder/Modulus Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator%(IntType Value, MediumDecVariant self)
        {
            MediumDecVariant Result = Value;
            return RemOp(Result, self);
        }

        /// <summary>
        /// Bitwise XOR Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator^(IntType Value, MediumDecVariant self)
        {
            return (MediumDecVariant)Value ^ self;
        }
        /// <summary>
        /// Bitwise Or Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator|(IntType Value, MediumDecVariant self)
        {
            return (MediumDecVariant)Value | self;
        }
        /// <summary>
        /// Bitwise And Operation Between MediumDecVariant and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename IntType>
        friend MediumDecVariant operator&(IntType Value, MediumDecVariant self)
        {
            return (MediumDecVariant)Value & self;
        }
    #pragma endregion Integer-To-MediumDecVariant Operations

    #pragma region Math/Trigonomic Etc Functions
        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        MediumDecVariant& Floor()
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
                ConvertToNumRep();
                break;
            }
            if (DecimalHalf == 0)
            {
                return *this;
            }
            DecimalHalf = 0;
            if (IntValue == NegativeRep) { IntValue = -1; }
            else
            {
                --IntValue;
            }
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant Floor(MediumDecVariant Value)
        {
            return Value.Floor();
        }

        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        static MediumDecVariant Floor(MediumDecVariant Value, int precision)
        {
            Value.ConvertToNumRep();
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
        /// <returns>MediumDecVariant&</returns>
        MediumDecVariant& Ceil()
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
                //ConvertToNumRep();//Prevent losing imaginery numbers
                break;
            }
            if (DecimalHalf == 0)
            {
                return *this;
            }
            DecimalHalf = 0;
            if (IntValue == NegativeRep) { IntValue = 0; }
            else
            {
                ++IntValue;
            }
            return *this;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        static int FloorInt(MediumDecVariant Value)
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
                Value.ConvertToNumRep();
                break;
            }
            if (Value.DecimalHalf == 0)
            {
                return Value.IntValue;
            }
            if (Value.IntValue == NegativeRep) { return -1; }
            else
            {
                return Value.IntValue - 1;
            }
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        static int CeilInt(MediumDecVariant Value)
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
#if defined(AltNum_EnableFractionals)
            case RepType::ENumByDiv:
#endif
                Value.ConvertToNumRep();
                break;
#endif
#if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltNum_EnableFractionals)
            case RepType::INumByDiv:
#endif
                break;
#endif
            //case RepType::ApproachingTop:
            //case RepType::ApproachingBottom:
            default:
                Value.ConvertToNumRep();
                break;
            }
            if (Value.DecimalHalf == 0)
            {
                return Value.IntValue;
            }
            if (Value.IntValue == NegativeRep) { return 0; }
            else
            {
                return Value.IntValue+1;
            }
        }
        
        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the ApproachingTopEst integer).
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant Ceil(MediumDecVariant Value)
        {
            return Value.Ceil();
        }
        
        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDecVariant &</returns>
        MediumDecVariant& Trunc()
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
                ConvertToNumRep();
                DecimalHalf = 0;
                if (IntValue == NegativeRep) { IntValue = 0; }
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
        static MediumDecVariant Trunc(MediumDecVariant Value)
        {
            return Value.Trunc();
        }
        
        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecVariant&</returns>
        MediumDecVariant& Abs()
        {
/*#ifdef AltNum_EnableInfinityRep
            if (DecimalHalf == InfinityRep)
            {
#ifdef AltNum_EnableInfinityPowers//Not Reaaly needed to treat
                if (IntValue<0)//Force negative Infinity into positive Infinity
                    IntValue *= -1;
#else
                if (IntValue == -1)
                    IntValue = 1;
#endif
                return *this;
            }
#endif*/
            if (IntValue == NegativeRep)
                IntValue = 0;
            else if (IntValue < 0)
                IntValue = -IntValue;//IntValue *= -1;
            return *this;
        }
        
        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant Abs(MediumDecVariant Value)
        {
            return Value.Abs();
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDecVariant PowOp(ValueType& expValue)
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
                    MediumDecVariant self = *this;
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
            else if (DecimalHalf == 0 && IntValue == 10)
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            else if (DecimalHalf == 0 && IntValue == -10)
                IntValue = expValue % 2 ? VariableConversionFunctions::PowerOfTens[expValue] : VariableConversionFunctions::PowerOfTens[expValue] * -1;
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecVariant self = *this;
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
        /// Applies Power of operation on references with const expValue(for integer exponents)(C3892 fix)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDecVariant PowConstOp(const ValueType& expValue)
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
                    int expVal = expValue * -1;
                    IntValue = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[expVal];
                }
                else
                {
                    int expVal = expValue;
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expVal *= -1;
                    MediumDecVariant self = *this;
                    IntValue = 1; DecimalHalf = 0;// Initialize result
                    while (expVal > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (expVal % 2 == 1)
                            *this /= self;
                        // n must be even now
                        expVal = expVal >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return this;
                }
            }
            else if (DecimalHalf == 0 && IntValue == 10)
            {
                IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                int expVal = expValue;
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecVariant self = *this;
                IntValue = 1; DecimalHalf = 0;// Initialize result
                while (expVal > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expVal % 2 == 1)
                        this *= self;
                    // n must be even now
                    expVal = expVal >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation on references with const expValue(for integer exponents)(C3892 fix)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecVariant PowConstOp(MediumDecVariant& targetValue, const ValueType& expValue)
        {
            return targetValue.PowConstOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecVariant PowOp(MediumDecVariant& targetValue, ValueType& expValue)
        {
            return targetValue.PowOp(expValue);
        }
        
        /// <summary>
        /// Applies Power of operation(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        MediumDecVariant Pow(ValueType expValue)
        {
            return this->PowOp(expValue);
        }

        /// <summary>
        /// Applies Power of operation (for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecVariant Pow(MediumDecVariant targetValue, ValueType expValue)
        {
            if (expValue == 1) { return targetValue; }//Return self
            else if (expValue == 0)
                return MediumDecVariant::One;
            else if (expValue < 0)//Negative Pow
            {
                if (targetValue.DecimalHalf == 0 && targetValue.IntValue == 10 && expValue >= -9)
                {
                    targetValue.IntValue = 0; targetValue.DecimalHalf = MediumDecVariant::DecimalOverflow / VariableConversionFunctions::PowerOfTens[expValue * -1];
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expValue *= -1;
                    MediumDecVariant self = targetValue;
                    targetValue = MediumDecVariant::One;// Initialize result
                    while (expValue > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (expValue % 2 == 1)
                            targetValue /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return targetValue;
                }
            }
            else if (targetValue.DecimalHalf == 0 && targetValue.IntValue == 10)
            {
                targetValue.IntValue = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecVariant self = targetValue;
                targetValue = MediumDecVariant::One;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        targetValue *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return targetValue;
        }

        /// <summary>
        /// Applies Power of operation (for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        static MediumDecVariant PowRef(MediumDecVariant& targetValue, ValueType expValue)
        {
            if (expValue == 1)
                return targetValue;//Return self
            else if (expValue == 0)
                return MediumDecVariant::One;
            else if (expValue < 0)//Negative Pow
            {
                if (targetValue.DecimalHalf == 0 && targetValue.IntValue == 10 && expValue >= -9)
                {
                    return MediumDecVariant(0, MediumDecVariant::DecimalOverflow / VariableConversionFunctions::PowerOfTens[expValue * -1]);
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    expValue *= -1;
                    MediumDecVariant self = targetValue;
                    MediumDecVariant Result = MediumDecVariant::One;
                    while (expValue > 0)
                    {
                        // If expValue is odd, divide self with result
                        if (expValue % 2 == 1)
                            Result /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self = self / self; // Change x to x^-1
                    }
                    return Result;
                }
            }
            else if (targetValue.DecimalHalf == 0 && targetValue.IntValue == 10)
                return MediumDecVariant(VariableConversionFunctions::PowerOfTens[expValue], 0);
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                MediumDecVariant self = targetValue;
                MediumDecVariant Result = MediumDecVariant::One;
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        Result *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
                return Result;
            }
            return targetValue;
        }

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static MediumDecVariant BasicSqrt(MediumDecVariant& value, int precision=7)
        {//Ignores Alternate representations use Sqrt instead to check based on RepType
            if (value.DecimalHalf == 0)
            {
                bool AutoSetValue = true;
                switch (value.IntValue)
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
            MediumDecVariant number = value;
            MediumDecVariant start = 0, end = number;
            MediumDecVariant mid;

            // variable to store the answer 
            MediumDecVariant ans;

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
            MediumDecVariant increment = "0.1";
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
		static MediumDecVariant Sqrt(MediumDecVariant value, int precision=7)
		{
		    ValueRep = 
		    return 
		}

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="nValue">The nth value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant NthRoot(MediumDecVariant value, int n, MediumDecVariant precision = MediumDecVariant::JustAboveZero)
        {
            MediumDecVariant xPre = 1+(value-1)/n;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two 
            // roots by INT_MAX 
            MediumDecVariant delX = MediumDecVariant(2147483647, 0);

            //  xK denotes current value of x 
            MediumDecVariant xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                xK = (nMinus1 * xPre +
                    value / MediumDecVariant::Pow(xPre, nMinus1)) / n;
                delX = MediumDecVariant::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static MediumDecVariant NthRootV2(MediumDecVariant targetValue, int n, MediumDecVariant& Precision = MediumDecVariant::FiveBillionth)
        {
            int nMinus1 = n - 1;
            MediumDecVariant x[2] = { (MediumDecVariant::One / n) * ((nMinus1 * targetValue) + (targetValue / MediumDecVariant::Pow(targetValue, nMinus1))), targetValue };
            while (MediumDecVariant::Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (MediumDecVariant::One / n) * ((nMinus1 * x[1]) + (targetValue / MediumDecVariant::Pow(x[1], nMinus1)));
            }
            return x[0];
        }

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant Exp(MediumDecVariant x)
        {
            //x.ConvertToNumRep();//Prevent losing imaginary number status
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
             *      - x: (MediumDecVariant float) power of e to evaluate
             * Returns:
             *      - (MediumDecVariant float) approximation of e^x in MediumDecVariant precision
             */
             // Check that x is a valid input.
            assert(-709 <= x.IntValue && x.IntValue <= 709);
            // When x = 0 we already know e^x = 1.
            if (x == MediumDecVariant::Zero) {
                return MediumDecVariant::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            MediumDecVariant x0 = MediumDecVariant::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            MediumDecVariant tn = MediumDecVariant::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = MediumDecVariant::CeilInt(x0 * MediumDecVariant::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + MediumDecVariant::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = MediumDecVariant::One / tn;
            }
            return tn;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
        static MediumDecVariant FractionalPow(MediumDecVariant value, int expNum, int expDenom)
        {
            MediumDecVariant CalcVal = MediumDecVariant::NthRoot(MediumDecVariant::Pow(value, expNum), expDenom);
            return CalcVal;
        }

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
        static MediumDecVariant FractionalPow(MediumDecVariant& value, boost::rational<int>& Frac)
        {
            MediumDecVariant CalcVal = MediumDecVariant::NthRoot(MediumDecVariant::Pow(value, Frac.numerator()), Frac.denominator());
            return CalcVal;
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDecVariant PowOp(MediumDecVariant& value, MediumDecVariant& expValue)
        {
            if (expValue.DecimalHalf == 0)
            {
                return value.Pow(expValue.IntValue);
            }
            else
            {
                RepType repType = value.GetRepType();
                switch (repType)
                {
                case RepType::NormalType:
                    break;
                case RepType::PiNum:
                    //value.ConvertPiToNum();
                    break;
#if defined(AltNum_EnableInfinityRep)
                //case RepType::ApproachingBottom:
                //break;
                //case RepType::ApproachingTop:
                //break;
                case InfinityRep:
                    return expType.IsZero()?MediumDecVariant.One:value;
                break;
#endif
#if defined(AltNum_EnableInfinityRep)

#endif
#if defined(AltNum_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
                default:
                    //value.ConvertToNumRep();
                    break;
                }
                RepType expType = expValue.GetRepType();
                switch (expType)
                {
                case RepType::NormalType:
                    break;
#if defined(AltNum_EnableInfinityRep)
                //case RepType::ApproachingBottom:
                //break;
                //case RepType::ApproachingTop:
                //break;
                case InfinityRep:
                    //0^Infinity = 0
                    //2^PositiveInfinity = PositiveInfinity
                    //-2^PositiveInfinity = Within range of Positive and NegativeInfinity(undefined?)
                    //2^NegativeInfinity = ApproachingZero
                    //-2^NegativeInfinity = -Approaching Zero
                    //return value.IsZero()?value:(IntValue==1?expValue:);
                    if(value.IsZero())
                        return value;
                    if(value.IntValue<0)
                        if(expValue.IntValue<0)
                            return NegativeApproachingZero;//-Approaching Zero
                        else
                            return ApproachingZero;//ApproachingZero
                    else
                         if(expValue.IntValue<0)//Returns result within range of Positive and NegativeInfinity or undefined or Any infinity?
#if defined(AltNum_EnableUndefinedButInRange)
                             throw "Returns noncoded representation infinity based value";//Return value here later
#elif defined(AltNum_EnableNaN)
                             return NaN;
#else
                             throw "Returns value in undefined range with current preprocessor settings.";
#endif
                            
                        else
                            return Infinity;//PositiveInfinity
                break;
#endif
#if defined(AltNum_EnableFractionals)
                case RepType::NumByDiv:
                    return FractionalPow(value, MediumDecVariant(expValue.IntValue, expValue.DecimalHalf), expValue.ExtraRep);
#if defined(AltNum_EnableERep)
                case RepType::ENumByDiv:
                    return FractionalPow(value, value.ExtraRep*-1);
                    break;
#endif
                case RepType::INumByDiv:
                    return FractionalPow(value, value.ExtraRep*-1);
                    break;
#endif
    //            case RepType::PiNum:
    //                expValue.ConvertPiToNum();
    //                break;
/*

*/
                default:
                    //expValue.ConvertToNumRep();
                    break;
                }
                boost::rational<int> Frac = boost::rational<int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
                switch (expValue.IntValue)
                {
                    case 0:
                        return FractionalPow(value, Frac);
                        break;
                    case MediumDecVariant::NegativeRep:
                        return 1 / FractionalPow(value, Frac);
                        break;
                    default:
                    {
                        if (expValue.IntValue < 0)//Negative Exponent 
                        {
                            MediumDecVariant CalcVal = 1 / value.Pow(expValue.IntValue * -1);
                            CalcVal /= FractionalPow(value, Frac);
                            return CalcVal;
                        }
                        else
                        {
                            MediumDecVariant CalcVal = value.Pow(expValue.IntValue);
                            CalcVal *= FractionalPow(value, Frac);
                            return CalcVal;
                        }
                        break;
                    }
                }
            }
        }

        /// <summary>
        /// Applies Power of operation
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDecVariant Pow(MediumDecVariant value, MediumDecVariant expValue)
        {
            return PowOp(value, expValue);
        }

private:
    static MediumDecVariant LnRef_Part02(MediumDecVariant& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        //Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
        MediumDecVariant TotalRes = (value - 1) / (value + 1);
        MediumDecVariant LastPow = TotalRes;
        MediumDecVariant WSquared = TotalRes * TotalRes;
        MediumDecVariant AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDecVariant::JustAboveZero);
        return TotalRes * 2;
    }
public:
        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>BlazesRusCode::MediumDecVariant</returns>
        static MediumDecVariant LnRef(MediumDecVariant& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            bool WithinThresholdRange = false;
            switch(DecimalHalf)
            {
#if defined(AltNum_EnableInfinityRep)
            case InfinityRep:
                return value;
                break;
#endif
#if defined(AltNum_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
            case ApproachingBottomRep:
				if(value.IntValue < 2)
					WithinThresholdRange = true;
				else
					WithinThresholdRange = false;
                break;
            case ApproachingTopRep:
				if(value.IntValue < 2)
					WithinThresholdRange = true;
				else
					WithinThresholdRange = false;
                break;
            default:
                if(value.ExtraRep==PiRep)
                    ConvertPiToNum();
#if defined(AltNum_EnableERep)
                else if(value.ExtraRep<0)
                    ConvertEToNum();
#endif
#if defined(AltNum_EnableFractionals)
                else if(value.ExtraRep>0)
                {
                    BasicIntDivOp(ExtraRep);
                    ExtraRep = 0;
                }
#endif
                if(value.IntValue < 2)
                    WithinThresholdRange = true;
                break;
            }
            if (WithinThresholdRange)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                MediumDecVariant threshold = MediumDecVariant::FiveMillionth;
                MediumDecVariant base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecVariant term = base;       // First term
                MediumDecVariant prev;          // Previous sum
                MediumDecVariant result = term;     // Kick it off

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
                } while (MediumDecVariant::Abs(prev - result) > threshold);

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
        /// <returns>BlazesRusCode::MediumDecVariant</returns>
        static MediumDecVariant LnRefV2(MediumDecVariant& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            if(value.ExtraRep!=0)
                return LnRef_Part02(value);
            switch(DecimalHalf)
            {
#if defined(AltNum_EnableInfinityRep)
            case InfinityRep:
                return value;
                break;
#endif
#if defined(AltNum_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
            case ApproachingBottomRep:
				if(value.IntValue < 2)
					WithinThresholdRange = true;
				else
					WithinThresholdRange = false;
                break;
            case ApproachingTopRep:
				if(value.IntValue < 2)
					WithinThresholdRange = true;
				else
					WithinThresholdRange = false;
                break;
            default:
                break;
            }
            if(value.IntValue==0)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                MediumDecVariant W = (value - 1)/ (value + 1);
                MediumDecVariant TotalRes = W;
                W.SwapNegativeStatus();
                MediumDecVariant LastPow = W;
                MediumDecVariant WSquared = W * W;
                int WPow = 3;
                MediumDecVariant AddRes;

                do
                {
                    LastPow *= WSquared;
                    AddRes = LastPow / WPow;
                    TotalRes -= AddRes;
                    WPow += 2;
                } while (AddRes > MediumDecVariant::JustAboveZero);
                return TotalRes * 2;
            }
            else if (value.IntValue==1)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                MediumDecVariant threshold = MediumDecVariant::FiveMillionth;
                MediumDecVariant base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 2;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecVariant term = base;       // First term
                MediumDecVariant prev;          // Previous sum
                MediumDecVariant result = term;     // Kick it off

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
                } while (MediumDecVariant::Abs(prev - result) > threshold);

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
        static MediumDecVariant Ln(MediumDecVariant value)
        {
            return LnRef(value);
        }

private:
    static MediumDecVariant Log10_Part02(MediumDecVariant& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        MediumDecVariant TotalRes = (value - 1) / (value + 1);
        MediumDecVariant LastPow = TotalRes;
        MediumDecVariant WSquared = TotalRes * TotalRes;
        MediumDecVariant AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDecVariant::JustAboveZero);
        return TotalRes * MediumDecVariant::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant Log10(MediumDecVariant value)
        {
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            bool WithinThresholdRange = false;
            switch(DecimalHalf)
            {
#if defined(AltNum_EnableInfinityRep)
            case InfinityRep:
                return value;
                break;
#endif
#if defined(AltNum_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
            case ApproachingBottomRep:
				if(value.IntValue < 2)
					WithinThresholdRange = true;
				else
					WithinThresholdRange = false;
                break;
            case ApproachingTopRep:
				if(value.IntValue < 2)
					WithinThresholdRange = true;
				else
					WithinThresholdRange = false;
                break;
            default:
                if(value.ExtraRep==PiRep)
                    ConvertPiToNum();
#if defined(AltNum_EnableERep)
                else if(value.ExtraRep<0)
                    ConvertEToNum();
#endif
#if defined(AltNum_EnableFractionals)
                else if(value.ExtraRep>0)
                {
                    BasicIntDivOp(ExtraRep);
                    ExtraRep = 0;
                }
#endif
                if(value.IntValue < 2)
                    WithinThresholdRange = true;
                break;
            }
            if (value.ExtraRep==0 && value.DecimalHalf == 0 && value.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDecVariant(index, 0);
                }
                return MediumDecVariant(9, 0);
            }
            if (WithinThresholdRange)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                MediumDecVariant threshold = MediumDecVariant::FiveBillionth;
                MediumDecVariant base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecVariant term = base;       // First term
                MediumDecVariant prev = 0;          // Previous sum
                MediumDecVariant result = term;     // Kick it off

                while (MediumDecVariant::Abs(prev - result) > threshold) {
                    den++;
                    posSign = !posSign;
                    term *= base;
                    prev = result;
                    if (posSign)
                        result += term / den;
                    else
                        result -= term / den;
                }
                return result*MediumDecVariant::LN10Mult;// result/MediumDecVariant::LN10;//Using Multiplication instead of division for speed improvement
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                return Log10_Part02(value);
            }
        }

private:
    template<typename ValueType>
    static MediumDecVariant Log10_IntPart02(ValueType& value)
    {	//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
        MediumDecVariant TotalRes = MediumDecVariant((value - 1), 0) / MediumDecVariant((value + 1), 0);
        MediumDecVariant LastPow = TotalRes;
        MediumDecVariant WSquared = TotalRes * TotalRes;
        MediumDecVariant AddRes;
        int WPow = 3;
        do
        {
            LastPow *= WSquared;
            AddRes = LastPow / WPow;
            TotalRes += AddRes; WPow += 2;
        } while (AddRes > MediumDecVariant::JustAboveZero);
        return TotalRes * MediumDecVariant::HalfLN10Mult;//Gives more accurate answer than attempting to divide by Ln10
    }
public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        template<typename ValueType>
        static MediumDecVariant Log10(ValueType value)
        {
            if (value == 1)
                return MediumDecVariant::Zero;
            //if(value.ExtraRep!=0)
            //    return Log10_IntPart02(value);
            if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return MediumDecVariant(index, 0);
                }
                return MediumDecVariant(9, 0);
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
        static MediumDecVariant Log(MediumDecVariant value, MediumDecVariant baseVal)
        {
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            return Log10(value) / Log10(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant Log(MediumDecVariant value, int baseVal)
        {
            bool WithinThresholdRange = false;
            switch(DecimalHalf)
            {
#if defined(AltNum_EnableInfinityRep)
            case InfinityRep:
                return value;
                break;
#endif
#if defined(AltNum_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
            case ApproachingBottomRep:
				if(value.IntValue < 2)
					WithinThresholdRange = true;
				else
					WithinThresholdRange = false;
                break;
            case ApproachingTopRep:
				if(value.IntValue < 2)
					WithinThresholdRange = true;
				else
					WithinThresholdRange = false;
                break;
            default:
                if(value.ExtraRep==PiRep)
                    ConvertPiToNum();
#if defined(AltNum_EnableERep)
                else if(value.ExtraRep<0)
                    ConvertEToNum();
#endif
#if defined(AltNum_EnableFractionals)
                else if(value.ExtraRep>0)
                {
                    BasicIntDivOp(ExtraRep);
                    ExtraRep = 0;
                }
#endif
                if(value.IntValue < 2)
                    WithinThresholdRange = true;
                break;
            }
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            //Calculate Base log first
            MediumDecVariant baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = MediumDecVariant(index, 0);
                        break;
                    }
                }
                baseTotalRes = MediumDecVariant(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = MediumDecVariant((baseVal - 1), 0) / MediumDecVariant((baseVal + 1), 0);
                MediumDecVariant baseLastPow = baseTotalRes;
                MediumDecVariant baseWSquared = baseTotalRes * baseTotalRes;
                MediumDecVariant baseAddRes;
                int baseWPow = 3;
                do
                {
                    baseLastPow *= baseWSquared;
                    baseAddRes = baseLastPow / baseWPow;
                    baseTotalRes += baseAddRes; baseWPow += 2;
                } while (baseAddRes > MediumDecVariant::JustAboveZero);
            }

            //Now calculate other log
            if (value.ExtraRep==0&&value.DecimalHalf == 0 && value.IntValue % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? MediumDecVariant(index, 0) / (baseTotalRes * MediumDecVariant::HalfLN10Mult): MediumDecVariant(index, 0)/ baseTotalRes;
                }
                return lnMultLog? MediumDecVariant(9, 0) / (baseTotalRes*MediumDecVariant::HalfLN10Mult):MediumDecVariant(9, 0)/baseTotalRes;
            }
            if (WithinThresholdRange)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                MediumDecVariant threshold = MediumDecVariant::FiveBillionth;
                MediumDecVariant base = value - 1;        // Base of the numerator; exponent will be explicit
                int den = 1;              // Denominator of the nth term
                bool posSign = true;             // Used to swap the sign of each term
                MediumDecVariant term = base;       // First term
                MediumDecVariant prev = 0;          // Previous sum
                MediumDecVariant result = term;     // Kick it off

                while (MediumDecVariant::Abs(prev - result) > threshold) {
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
                MediumDecVariant W = (value - 1) / (value + 1);
                MediumDecVariant TotalRes = W;
                MediumDecVariant AddRes;
                int WPow = 3;
                do
                {
                    AddRes = MediumDecVariant::PowRef(W, WPow) / WPow;
                    TotalRes += AddRes; WPow += 2;
                } while (AddRes > MediumDecVariant::JustAboveZero);
                return lnMultLog? TotalRes/baseTotalRes:(TotalRes * MediumDecVariant::HalfLN10Mult)/ baseTotalRes;
            }
            //return Log10(Value) / Log10(BaseVal);
        }

    #pragma endregion Math Etc Functions
    #pragma region Trigonomic Etc Functions
        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant SinFromAngle(MediumDecVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
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
            case RepType::PiNum:
            case RepType::ApproachingTop:
            case RepType::ApproachingBottom:
                Value.ConvertToNumRep();
                break;
            default:
                break;
            }
            if (Value.IntValue < 0)
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.IntValue *= -1;
                    Value.IntValue %= 360;
                    Value.IntValue = 360 - Value.IntValue;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return MediumDecVariant::Zero; }
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
                return MediumDecVariant::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else
            {
                MediumDecVariant NewValue = Zero;
                //Angle as Radian
                MediumDecVariant Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's Taylor series!!
                    NewValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Radius, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
                }
                return NewValue;
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns></returns>
        static MediumDecVariant CosFromAngle(MediumDecVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
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
            case RepType::PiNum:
            case RepType::ApproachingTop:
            case RepType::ApproachingBottom:
                Value.ConvertToNumRep();
                break;
            default:
                break;
            }
            //Value.ConvertToNumRep();
            if (Value.IntValue < 0)
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.IntValue *= -1;
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
                return MediumDecVariant::Zero;
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return NegativeOne;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
                return MediumDecVariant::Zero;
            }
            else
            {
                MediumDecVariant NewValue = Zero;
                //Angle as Radian
                MediumDecVariant Radius = Pi * Value / 180;
                for (int i = 0; i < 7; ++i)
                { // That's also Taylor series!!
                    NewValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Radius, 2 * i) / VariableConversionFunctions::Fact(2 * i);
                }
                return NewValue;
            }
        }

        /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant Sin(MediumDecVariant Value)
        {
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
                if (Value.DecimalHalf == 0&&(Value.IntValue==0||Value.IntValue==1))//0 and Pi
                    return Zero;
                if (Value.DecimalHalf == 500000000)//0.5 Pi = 1; 1.5Pi = -1
                    return Value.IntValue==0?NegativeOne:One;
                MediumDecVariant SinValue = Zero;
                for (int i = 0; i < 7; ++i)
                {
                    SinValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
                }
                return SinValue;

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
            MediumDecVariant SinValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Sin from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant Cos(MediumDecVariant Value)
        {
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
            MediumDecVariant CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                CosValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant Tan(MediumDecVariant Value)
        {
            MediumDecVariant SinValue = Zero;
            MediumDecVariant CosValue = Zero;
            for (int i = 0; i < 7; ++i)
            {
                SinValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            {
                CosValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant TanFromAngle(MediumDecVariant Value)
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
            case RepType::RepType::PiNum:
#endif
#if defined(AltNum_EnableERep)
            case RepType::RepType::ENum:
#if defined(AltNum_EnableFractionals)
            case RepType::RepType::ENumByDiv:
#endif
#endif
                Value.ConvertToNumRep();
                break;
            //case RepType::INum:
            default:
                //Value.ConvertToNumRep();//Don't convert things like imaginary numbers into real numbers
                break;
            }
            if (Value.IntValue < 0)
            {
                if (Value.IntValue == NegativeRep)
                {
                    Value.IntValue = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.IntValue *= -1;
                    Value.IntValue %= 360;
                    Value.IntValue = 360 - Value.IntValue;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntValue %= 360;
            }
            if (Value == Zero) { return MediumDecVariant::Zero; }
            else if (Value.IntValue == 90 && Value.DecimalHalf == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                return MediumDecVariant::PositiveInfinity;
#else
                return MediumDecVariant::Maximum;//Positive Infinity
#endif
            }
            else if (Value.IntValue == 180 && Value.DecimalHalf == 0)
            {
                return MediumDecVariant::Zero;
            }
            else if (Value.IntValue == 270 && Value.DecimalHalf == 0)
            {
#if defined(AltNum_EnableInfinityRep)
                return MediumDecVariant::NegativeInfinity;
#else
                return MediumDecVariant::Minimum;//Negative Infinity
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
        static MediumDecVariant ATan(MediumDecVariant Value)
        {
            RepType repType = Value.GetRepType();
            switch (repType)
            {
#if defined(AltNum_EnableNaN)
            case RepType::NaN:
                return NaNValue();
#endif
            default:
                //Value.ConvertToNumRep();
                break;
            }
            MediumDecVariant SinValue = Zero;
            MediumDecVariant CosValue = Zero;
            //Angle as Radian
            for (int i = 0; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            for (int i = 0; i < 7; ++i)
            { // That's also Taylor series!!
                CosValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i) / VariableConversionFunctions::Fact(2 * i);
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
        static MediumDecVariant ArcTan2(MediumDecVariant y, MediumDecVariant x)
        {
#if defined(AltNum_EnablePiRep)
            MediumDecVariant coeff_1 = MediumDecVariant(0, 250000000, PiRep);//Pi / 4;
#else
            MediumDecVariant coeff_1 = Pi / 4;
#endif
            MediumDecVariant coeff_2 = 3 * coeff_1;
            MediumDecVariant abs_y = MediumDecVariant::Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            MediumDecVariant r;
            MediumDecVariant angle;
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

    #pragma region ValueDefine Source
#if defined(AltNum_EnableInfinityRep)
    MediumDecVariant MediumDecVariant::AlmostOne = ApproachingRightRealValue();
#endif
    MediumDecVariant MediumDecVariant::Pi = PiValue();
    MediumDecVariant MediumDecVariant::One = OneValue();
    MediumDecVariant MediumDecVariant::Two = TwoValue();
    MediumDecVariant MediumDecVariant::NegativeOne = NegativeOneValue();
    MediumDecVariant MediumDecVariant::Zero = ZeroValue();
    MediumDecVariant MediumDecVariant::PointFive = Point5Value();
    MediumDecVariant MediumDecVariant::JustAboveZero = JustAboveZeroValue();
    MediumDecVariant MediumDecVariant::OneMillionth = OneMillionthValue();
    MediumDecVariant MediumDecVariant::FiveThousandth = FiveThousandthValue();
    MediumDecVariant MediumDecVariant::Minimum = MinimumValue();
    MediumDecVariant MediumDecVariant::Maximum = MaximumValue();
    MediumDecVariant MediumDecVariant::E = EValue();
    MediumDecVariant MediumDecVariant::LN10 = LN10Value();
    MediumDecVariant MediumDecVariant::LN10Mult = LN10MultValue();
    MediumDecVariant MediumDecVariant::HalfLN10Mult = HalfLN10MultValue();
    MediumDecVariant MediumDecVariant::TenMillionth = TenMillionthValue();
    MediumDecVariant MediumDecVariant::FiveMillionth = FiveMillionthValue();
    MediumDecVariant MediumDecVariant::FiveBillionth = FiveBillionthValue();
    MediumDecVariant MediumDecVariant::OneGMillionth = OneHundredMillionthValue();
    #if defined(AltNum_EnableNilRep)
    MediumDecVariant MediumDecVariant::Nil = NilValue();
    #endif

    MediumDecVariant MediumDecVariant::PiNum = PiNumValue();
    MediumDecVariant MediumDecVariant::ENum = ENumValue();
    
#if defined(AltNum_EnableInfinityRep)
    MediumDecVariant MediumDecVariant::Infinity = InfinityValue();
#if defined(AltNum_EnableApproachingValues)
    MediumDecVariant MediumDecVariant::NegativeInfinity = NegativeInfinityValue();
    MediumDecVariant MediumDecVariant::ApproachingZero = ApproachingZeroValue();
#endif
#endif
#if defined(AltNum_EnableNaN)
    MediumDecVariant MediumDecVariant::NaN = NaNValue();
#endif
    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void MediumDecVariant::ReadString(std::string Value)
    {
        IntValue = 0; DecimalHalf = 0;
        bool IsNegative = false;
        int PlaceNumber;
        std::string WholeNumberBuffer = "";
        std::string DecimalBuffer = "";

        bool ReadingDecimal = false;
        int TemPint;
        int TemPint02;
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
            TemPint = VariableConversionFunctions::CharAsInt(StringChar);
            TemPint02 = (TemPint * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
            if (StringChar != '0')
            {
                IntValue += TemPint02;
            }
            PlaceNumber--;
        }
        PlaceNumber = 8;
        for (char const& StringChar : DecimalBuffer)
        {
            //Limit stored decimal numbers to the amount it can store
            if (PlaceNumber > -1)
            {
                TemPint = VariableConversionFunctions::CharAsInt(StringChar);
                TemPint02 = (TemPint * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
                if (StringChar != '0')
                {
                    DecimalHalf += TemPint02;
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
    inline MediumDecVariant MediumDecVariant::GetValueFromString(std::string Value)
    {
        MediumDecVariant NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

    std::string MediumDecVariant::ToString()
    {
#if defined(AltNum_EnableInfinityRep)
        if (DecimalHalf == InfinityRep)
        {
            if (IntValue == 1)
                return "Infinity";
            else
                return "-Infinity";
        }
#endif
        RepType repType = GetRepType();
        switch (repType)
        {
        case RepType::ApproachingTop:
#ifdef AltNum_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.99999999999999999999" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".99999999999999999999";
#else
            return IntValue == NegativeRep ? "-0.9___9" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".9___9";
#endif
            break;
        case RepType::ApproachingBottom:
#ifdef AltNum_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.00000000000000000001" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".00000000000000000001";
#else
            return IntValue == NegativeRep ? "-0.0___1" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".0___1";
#endif
            break;
        case RepType::ApproachingBottom:
#ifdef AltNum_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.00000000000000000001" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".00000000000000000001";
#else
            return IntValue == NegativeRep ? "-0.0___1" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".0___1";
#endif
            break;
/*
#if defined(AltNum_EnableNaN)
        case RepType::NaN:
            return "NaN";
#endif
#if defined(AltNum_EnableERep)
        case RepType::ENum:
#if defined(AltNum_EnableFractionals)
        case RepType::ENumByDiv:
#endif
            break;
#endif
#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
#if defined(AltNum_EnableFractionals)
        case RepType::INumByDiv:
#endif
            break;
#endif
*/
        default:
            break;
        }
        std::string Value = "";
        int CurrentSection = IntValue;
        unsigned __int8 CurrentDigit;
        std::string DecBuffer = "";
        if (IntValue < 0)
        {
            Value += "-";
            if (IntValue == NegativeRep) { CurrentSection = 0; }
            else { CurrentSection *= -1; }
        }
        for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
        {
            CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
            CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        if (DecimalHalf != 0)
        {
            Value += ".";
            CurrentSection = DecimalHalf;
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
        switch (repType)
        {
#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            Value += "Pi";
            break;
#if defined(AltNum_EnableERep)
        case RepType::ENum:
            Value += "e";
            break;
#if defined(AltNum_EnableFractionals)
        case RepType::ENumByDiv:
            Value += "e/";
            Value += ExtraRep*-1;
            break;
#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            Value += "i";
            break;
#if defined(AltNum_EnableFractionals)
        case RepType::INumByDiv:
            Value += "i/";
            Value += ExtraRep*-1;
            break;
#endif
#endif
        default:
            break;
        }
        return Value;
    }

    std::string MediumDecVariant::ToFullString()
    {
#if defined(AltNum_EnableInfinityRep)
        if (DecimalHalf == InfinityRep)
        {
            if (IntValue == 1)
                return "Infinity";
            else
                return "-Infinity";
        }
#endif
        RepType repType = GetRepType();
        switch (repType)
        {
        case RepType::ApproachingTop:
#ifdef AltNum_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.99999999999999999999" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".99999999999999999999";
#else
            return IntValue == NegativeRep ? "-0.9___9" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".9___9";
#endif
            break;
        case RepType::ApproachingBottom:
#ifdef AltNum_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.00000000000000000001" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".00000000000000000001";
#else
            return IntValue == NegativeRep ? "-0.0___1" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".0___1";
#endif
            break;
/*
#if defined(AltNum_EnableERep)
        case RepType::ENum:
#if defined(AltNum_EnableFractionals)
        case RepType::ENumByDiv:
#endif
            break;
#endif
#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
#if defined(AltNum_EnableFractionals)
        case RepType::INumByDiv:
#endif
            break;
#endif
*/
        default:
            break;
        }
        std::string Value = "";
        int CurrentSection = IntValue;
        unsigned __int8 CurrentDigit;
        if (IntValue < 0)
        {
            Value += "-";
            if (IntValue == NegativeRep) { CurrentSection = 0; }
            else { CurrentSection *= -1; }
        }
        for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
        {
            CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
            CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        if (DecimalHalf != 0)
        {
            Value += ".";
            bool HasDigitsUsed = false;
            CurrentSection = DecimalHalf;
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
        switch (repType)
        {
#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            Value += "Pi";
            break;
#if defined(AltNum_EnableERep)
        case RepType::ENum:
            Value += "e";
            break;
#if defined(AltNum_EnableFractionals)
        case RepType::ENumByDiv:
            Value += "e/";
            Value += ExtraRep*-1;
            break;
#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            Value += "i";
            break;
#if defined(AltNum_EnableFractionals)
        case RepType::INumByDiv:
            Value += "i/";
            Value += ExtraRep*-1;
            break;
#endif
#endif
        default:
            break;
        }
        return Value;
    }
    #pragma endregion String Function Source

    /// <summary>
    /// (MediumDecVariant Version)Performs remainder operation then saves division result
    /// C = A - B * (A / B)
    /// </summary>
    class DLL_API AltModChecker : public AltNumModChecker<MediumDecVariant>
    {
    };
}