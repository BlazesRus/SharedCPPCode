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

#include <boost/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#ifdef AltDec_EnableMediumDecBasedSetValues
    #include "MediumDec.hpp"
#endif

#include "AltNumModChecker.hpp"
//Preprocessor options
/*
AltDec_EnableByDecimaledFractionals =
      Enables fractional representations in attempt to preserve more accuracy during operations
AltDec_EnableAlternativeRepFractionals =
      Enables integer based fractionals for alternative representations such as Pi(partially implimented)

AltDec_EnableNegativeZero =
      (Not Implimented)

//--Infinity based preprocessors--
AltDec_EnableInfinityRep = Enable support of positive/negative infinity representations and approaching value representations
      When DecimalHalf is -2147483648, it represents negative infinity(if IntValue is -1) or positive infinity(if IntValue is 1)
      When DecimalHalf is -2147483647, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)
      When DecimalHalf is -2147483646, it represents Approaching IntValue from right towards left (IntValue.0__1)
     (Not Fully Implimented)
AltDec_EnableApproachingMidDec = 
      When DecimalHalf is -2147483645, it represents Approaching Half way point of {IntValue,IntValue+1} from left towards right (IntValue.49__9)
      When DecimalHalf is -2147483644, it represents Approaching Half way point of {IntValue,IntValue+1} from right towards left (IntValue.50__1)
      Assumes AltDec_EnableInfinityRep is enabled
      (Not Implimented)

AltDec_EnableNearPI = (Not Implimented)
AltDec_EnableNearE = (Not Implimented)
AltDec_EnableNearI = (Not Implimented)

AltDec_EnableInfinityPowers =
      Allows powers of infinity for operations where infinity is somehow more infinite than normal
      (Not Implimented)

AltDec_DisplayApproachingAsReal =
      Display approaching value as real number with 20 digits in decimal section

//--Can't be currently Enabled if any EnableNear Options enabled:
AltDec_EnableApproachingDivided = (Not Implimented)
AltDec_EnableApproachingPointFive = Enables Approaching IntValue.49_infinitely_9 and .50_infinitely_1 as ExtraRep values -1 and 1(Not Implimented)
//--

//----

AltDec_EnableNaN =
      Enable NaN based representations and operations(Not Fully Implimented)

AltDec_EnableHigherPrecisionPIConversion =
      (Not Implimented)

AltDec_UseMediumDecBasedRepresentations =
      Forces to calculate certain representations like MediumDec does 
      (preference for storing non-normal representations within value of negative DecimalHalf)
	  (Not Implimented Yet)
	  
AltDec_EnableOverflowPreventionCode =
      Use to enable code to check for overflows on addition/subtraction/multiplication operations (return an exception if overflow)
      (Not Implimented Yet)
	  
AltDec_DisableInfinityRepTypeReturn =

AltDec_DisablePIRep =
      Force toggles AltDec_EnablePIRep to off
      AltDec_EnablePIRep given greator priority if both both AltDec_DisablePIRep and AltDec_EnablePIRep
      are set by preprocessor settings of project

AltDec_EnablePIRep =
      If AltDec_UseMediumDecBasedRepresentations enabled, then
        PI*(+- 2147483647.999999999) Representation enabled
        (When DecimalHalf is between -1 and -1000000000 (when DecimalHalf is -1000000000 is Equal to IntValue*PI))
      Otherwise represents pi within format of
         If DecimalHalf is positive and ExtraRep is -2147483647,
         then AltDec represents +- 2147483647.999999999 * Pi (Not Fully Implimented)
         If DecimalHalf is positive and and ExtraRep is between AlternativeFractionalLowerBound and 0,
         then AltDec represents (+- 2147483647.999999999 * Pi)/(ExtraRep*-1)
	  (Not Fully Implimented--Enabled by default if AltDec_DisablePIRep not set)

AltDec_EnableComplexNum =
      Enable Representation of complex numbers with Imaginary number operations
      (Requires AltDec_EnableImaginaryNum, Not Implimented Yet)

AltDec_EnableMixedFractional =
      If DecimalHalf is negative and ExtraRep is Positive,
      then AltDec represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
      (Not Fully Implimented)

AltDec_EnableERep =
      If AltDec_UseMediumDecBasedRepresentations enabled, then
    e*(+- 2147483647.999999999) Representation enabled
    (When DecimalHalf is between -1000000001 and -2000000000 (when DecimalHalf is -2000000000 is Equal to IntValue*e))
      Otherwise represents e within format of
    If DecimalHalf is positive and ExtraRep is -2147483646, then
       represents +- 2147483647.999999999 * e
    If DecimalHalf is positive and ExtraRep is between AlternativeFractionalLowerBound and 0, then
        represents (+- 2147483647.999999999 * e)/(ExtraRep*-1)
      (Not Fully Implimented)

AltDec_EnableImaginaryNum =
      If DecimalHalf is positive and ExtraRep is -2147483647, then
      represents +- 2147483647.999999999i
      If DecimalHalf is positive and ExtraRep is between AlternativeFractionalLowerBound and 0, then
      represents (+- 2147483647.999999999i)/(ExtraRep*-1)
      (Not Fully Implimented)

//----
AltDec_EnablePrivateRepType =
      Sets GetRepType code to be private instead of public

AltDec_TogglePreferedSettings =
      Force enables AltDec_EnablePIRep, AltDec_EnableInfinityRep, AltDec_EnableByDecimaledFractionals,
      and AltDec_EnablePublicRepType

AltDec_EnableUndefinedButInRange =

AltDec_DisableSwitchBasedConversion =

AltDec_EnableMediumDecBasedSetValues =

AltDec_EnablePIPowers =
      If ExtraRep value is between -1 and -2147483640, then represents IntValue.DecimalHalf * Pi^(ExtraRep*-1)
	  Can't be enabled at same time as AltDec_EnableDecimaledAlternativeFractionals
      (Not Fully Implimented)
AltDec_EnableNormalPowers =
      If ExtraRep value is between -1 and -2147483640, then represents IntValue.DecimalHalf^(ExtraRep*-1)
	  Can't be enabled at same time as AltDec_EnableDecimaledAlternativeFractionals or AltDec_EnablePIPowers
      (Not Implimented)
	  
AltDec_EnableDecimaledAlternativeFractionals = 
   Automatically enabled if AltDec_EnableDecimaledPiFractionals, AltDec_EnableDecimaledEFractionals, or AltDec_EnableDecimaledEFractionals enabled
   Not to be confused with AltDec_EnableAlternativeRepFractionals(which only enabled Integer based alternative rep fractionals)
AltDec_EnableDecimaledPiFractionals = Enables fractionals for Pi with non-integer numbers(not implimented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound
AltDec_EnableDecimaledEFractionals = Enables fractionals for e with non-integer numbers(not implimented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound
AltDec_EnableDecimaledIFractionals = Enables fractionals for e with non-integer numbers(not implimented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound
*/

#ifdef 

#if defined(AltDec_EnableDecimaledAlternativeFractionals) && defined(AltDec_EnablePIPowers)
#undef AltDec_EnablePIPowers
#endif

#if (defined(AltDec_EnableDecimaledAlternativeFractionals)||defined(AltDec_EnablePIPowers)) && defined(AltDec_EnableNormalPowers)
#undef AltDec_EnableNormalPowers
#endif

#if defined(AltDec_TogglePreferedSettings)
    #define AltDec_EnablePIRep
    #define AltDec_EnableInfinityRep
#endif

//if 
#if defined(AltDec_EnablePIRep) && defined(AltDec_DisablePIRep)
    #undef AltDec_DisablePIRep
#endif

//If Pi rep is neither disabled or enabled, default to enabling PI representation
#if !defined(AltDec_DisablePIRep) && !defined(AltDec_EnablePIRep)
    #define AltDec_EnablePIRep
#endif

#if defined(AltDec_EnableENum) && (defined(AltDec_EnableImaginaryNum)||defined(AltDec_EnablePIPowers))
    #undef AltDec_EnableENum
#endif

#if defined(AltDec_EnableENum) || defined(AltDec_EnableImaginaryNum) || defined(AltDec_EnablePIPowers)
    #define AltDec_OtherNegativeExtraRepsDefined
#endif

#if defined(AltDec_EnableNearPI) && !defined(AltDec_EnablePIRep)
    #undef AltDec_EnableNearPI
#endif

#if defined(AltDec_EnableNearE) && !defined(AltDec_EnableERep)
    #undef AltDec_EnableNearE
#endif

#if defined(AltDec_EnableNearI) && !defined(AltDec_EnableImaginaryNum)
    #undef AltDec_EnableNearI
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
 * Partial but still high accuracy for non-integer representation variations of multiplication because of truncation
   (values get lost if get too small)
 * Partial but still high accuracy for division because of truncation
   (values get lost if get too small)
 * Other operations like Ln and Sqrt contained with decent level of accuracy
   (still loses a little accuracy because of truncation etc)
 * Operations and functions will mess up if IntValue overflows/underflows
   or reaches exactly -2147483648 which is used to represent negative zero when it has decimal values
*/

    /// <summary>
    /// Alternative Non-Integer number representation with focus on accuracy and partially speed within certain range
    /// Represents +- 2147483647.999999999 with 100% consistency of accuracy for most operations as long as don't get too small
    /// plus support for some fractal operations, and other representations like PI(and optionally things like e or imaginary numbers)
    /// (12 bytes worth of Variable Storage inside class for each instance)
	/// </summary>
    class DLL_API AltDec
    {
#undefine MediumDecVariant
#define MediumDecVariant AltDec
    private:
#if defined(AltDec_EnableInfinityRep)
        //Is Infinity Representation when DecimalHalf==-2147483648 (IntValue==1 for positive infinity;IntValue==-1 for negative Infinity)
        static const signed int InfinityRep = -2147483648;
        //Is Approaching IntValue when DecimalHalf==-2147483647, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)
        static const signed int ApproachingTopRep = -2147483647;
        //When DecimalHalf == -2147483646, it represents Approaching IntValue from right towards left (IntValue.0__1)
        static signed int const ApproachingBottomRep = -2147483646;
#if defined(AltDec_EnableApproachingMidDec)
            static signed int const MidFromTopRep = -2147483644;
            static signed int const MidFromBottomRep = -2147483645;
#endif
#endif
#if defined(AltDec_EnablePIRep)
        //Is PI*Value representation when ExtraRep==-2147483648
        static const signed int PIRep = -2147483648;
#ifdef AltDec_EnableAlternativeRepFractionals
        //If AltDec_EnableImaginaryNum is enabled and ExtraRep== -2147483645, then represents (IntValue/DecimalHalf)*Pi
        static const signed int PIByDivisorRep = -2147483645;
#endif
#endif
#if defined(AltDec_EnableImaginaryNum)
        //If AltDec_EnableImaginaryNum is enabled and ExtraRep== -2147483646, then represents Value*i 
        static const signed int IRep = -2147483647;
#ifdef AltDec_EnableAlternativeRepFractionals
        //If AltDec_EnableImaginaryNum is enabled and ExtraRep== -2147483644, then represents (IntValue/DecimalHalf)*i
        static const signed int IByDivisorRep = -2147483644;
#endif
#endif
#if defined(AltDec_EnableENum)
        //If AltDec_EnableImaginaryNum is enabled and ExtraRep== -2147483646, then represents Value*e
        static const signed int ERep = -2147483646;
#ifdef AltDec_EnableAlternativeRepFractionals
        //If AltDec_EnableImaginaryNum is enabled and ExtraRep== -2147483643, then represents (IntValue/DecimalHalf)*e
        static const signed int EByDivisorRep = -2147483643;
#endif
#endif
        static const signed int AlternativeFractionalLowerBound = -2147483642;
        static const signed int MixedFractionalLowerBound = -2147483642;
#if defined(AltDec_EnableInfinityRep)
        //Is NaN when DecimalHalf==2147483647
        static const signed int NaNRep = 2147483647;
#endif
#if defined(AltDec_EnablePrivateRepType)
    public:
#endif
        enum class RepType: int
        {
            NormalType = 0,
            NumByDiv,
#if defined(AltDec_EnablePIRep)
            PINum,
#if defined(AltDec_EnablePIPowers)
            PIPower,
#endif
            PiNumByDivisor,//  IntValue/DecimalHalf*Pi Representation
#endif
#if defined(AltDec_EnableENum)
            ENum,
#if defined(AltDec_EnableAlternativeRepFractionals)
            ENumByDiv,
#endif
#endif
#if defined(AltDec_EnableImaginaryNum)
            INum,
#if defined(AltDec_EnableAlternativeRepFractionals)
            INumByDiv,
#endif
#endif
#if defined(AltDec_EnableMixedFractional)
            ComplexIRep,
            MixedFrac,//IntValue +- (DecimalHalf*-1)/ExtraRep
            MixedE,
            MixedI,
#endif
#if defined(AltDec_EnableInfinityRep)
            ApproachingBottom,//(Approaching Towards Zero is equal to 0.000...1)
            ApproachingTop,//(Approaching Away from Zero is equal to 0.9999...)
#if defined(AltDec_EnableApproachingDivided)
            ApproachingTopByDiv,//(Approaching Away from Zero is equal to IntValue + 0.9999.../ExtraRep if positive, IntValue - 0.9999.../ExtraRep if negative) 
#endif
#endif
            NaN,
            NegativeZero,
#if defined(AltDec_EnableNearPI)
            NearPI,//(Approaching Away from Zero is equal to 0.9999...PI)
#endif
#if defined(AltDec_EnableNearE)
            NearE,//(Approaching Away from Zero is equal to 0.9999...e)
#endif
#if defined(AltDec_EnableNearI)
            NearI,//(Approaching Away from Zero is equal to 0.9999...i)
#endif
#if defined(AltDec_EnableUndefinedButInRange)//Such as result of Cos of infinity
            UndefinedButInRange,
#endif
            UnknownType
        };
        RepType GetRepType()
        {
#if defined(AltDec_EnableInfinityRep)
#if !defined(AltDec_DisableInfinityRepTypeReturn)
            if(DecimalHalf==InfinityRep)
            {
                if(IntValue==1)//If Positive Infinity, then convert number into MaximumValue instead when need as real number
                {
                    return RepType::PositiveInfinity;
                }
                else//If Negative Infinity, then convert number into MinimumValue instead when need as real number
                {
                    return RepType::NegativeInfinity;
                }
            }
			else
#endif
            if (DecimalHalf == ApproachingValRep)
            {
                if(ExtraRep==0)
                    return RepType::ApproachingBottom;//Approaching from right to IntValue
#if defined(AltDec_EnableApproachingDivided)
				else if(ExtraRep>0)
				    return RepType::ApproachingTopByDiv;//Approaching from left divided by ExtraRep value
#else
#if defined(AltDec_EnablePIRep)
#if defined(AltDec_EnableNearPI)
                else if (ExtraRep == PIRep)
                    return RepType::NearPI;
#endif
#endif
#if defined(AltDec_EnableNearE)
                else if (ExtraRep == ERep)
                    return RepType::NearE;
#endif
#if defined(AltDec_EnableNearI)
                else if (ExtraRep == IRep)
                    return RepType::NearI;
#endif
#endif
                else
                    return RepType::ApproachingTop;//Approaching from left to (IntValue-1)
            }
#endif
            if(ExtraRep==0)
			{
#if defined(AltDec_EnableMixedFractional)
				if(DecimalHalf<0)
				 return RepType::MixedFrac;
				
#endif
                return RepType::NormalType;
			}
#ifdef AltDec_EnablePIRep
            else if(ExtraRep==PIRep)
                return RepType::PINum;
            else if(ExtraRep==PiByDivisorRep)
				return RepType::PiIntNumByDiv;
#endif
#if defined(AltDec_EnableByDecimaledFractionals)
            else if(ExtraRep>0)
                return RepType::NumByDiv;
#endif

#if defined(AltDec_EnableNaN)
            else if(DecimalHalf==NaNRep)
                return RepType::NaN;
#endif
#if defined(AltDec_EnableENum)
            else if(ExtraRep==ERep)
			{
#if AltDec_EnableMixedEFractional
				if(DecimalHalf<0)
					return RepType::MixedE;
				else
#endif
					return RepType::ENum;
			}
            else if(ExtraRep==EByDivisorRep)
					return RepType::ENumByDiv;
#endif
#if defined(AltDec_EnableENum)
            else if(ExtraRep==ERep)
			{
#if AltDec_EnableMixedEFractional
				if(DecimalHalf<0)
					return RepType::MixedE;
				else
#endif
					return RepType::ENum;
			}
            else if(ExtraRep==EByDivisorRep)
					return RepType::ENumByDiv;
#endif
            else
#if defined(AltDec_EnablePiEFractionals)
                return RepType::PiNumByDiv;
#elif defined(AltDec_EnableDecimaledEFractionals)
                return RepType::ENumByDiv;
#elif defined(AltDec_EnableDecimaledIFractionals)
                return RepType::INumByDiv;
#elif defined(AltDec_EnableMixedFractional)
                return RepType::MixedFrac;
#else
                throw "Unknown or non-enabled representation type detected from AltDec";
#endif
            throw "Unknown or non-enabled representation type detected from AltDec";
            return RepType::UnknownType;//Catch-All Value;
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

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;
    public:
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

        /// <summary>
		/// (Used exclusively for alternative represents of numbers including imaginery numbers and for fractionals)
        /// If both DecimalHalf&ExtraRep are Positive with ExtraRep as non-zero, then ExtraRep acts as denominator
        /// If DecimalHalf is negative and ExtraRep is Positive, then MediumDecVariant represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
        /// If ExtraRep is zero and DecimalHalf is positive, then MediumDecVariant represents +- 2147483647.999999999
        ///-----------------------------------------------
        /// If ExtraRep is negative, it acts as representation type similar to AltDec:
		/// If ExtraRep is between 0 and , it acts as representation type similar to AltDec:
        /// If DecimalHalf is positive and ExtraRep is -2147483648 and AltDec_EnablePIRep is enabled, then MediumDecVariant represents +- 2147483647.999999999 * PI
        /// </summary>
        signed int ExtraRep;

private:
        signed int ERep = -2;
        signed int IRep = -3;
        signed int PiPowerRep = -4;
        signed int NegativeZeroRep = -10;
public:

        /// <summary>
        /// Initializes a new instance of the <see cref="AltDec"/> class.
        /// </summary>
        /// <param name="intVal">The int value.</param>
        /// <param name="decVal">The decimal val01.</param>
        /// <param name="extraVal">ExtraRep.</param>
        AltDec(signed int intVal = 0, signed int decVal = 0, signed int extraVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
            ExtraRep = extraVal;
        }

        void SetAsZero()
        {
            IntValue = 0; DecimalHalf = 0;
            ExtraRep = 0;
        }

#if defined(AltDec_EnableNegativeZero)
        void SetAsNegativeZero()
        {
            IntValue = NegativeRep; DecimalHalf = 0;
            ExtraRep = 0;
        }
#endif
        
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(MediumDecVariant Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf; ExtraRep = Value.ExtraRep;
        }
        
#if defined(AltDec_EnablePIRep)
#if defined(AltDec_EnableMediumDecBasedSetValues)
        void SetPiValFromMediumDec(MediumDec Value)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = PIRep;
        }
#endif

        void SetPiVal(MediumDecVariant Value)
        {
            if(ExtraRep==0)
            {
                IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                ExtraRep = PIRep;
            }
        }
        
        void SetPiVal(int Value)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = PIRep;
        }
#endif
        
        void SetFractionalVal(MediumDec Value, int Divisor)
        {
            IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
            ExtraRep = Divisor;
        }
        
        void SetFractionalVal(int Value, int Divisor)
        {
            IntValue = Value; DecimalHalf = 0;
            ExtraRep = Divisor;
        }
        
#if defined(AltDec_EnableMixedFractional)
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
#if defined(AltDec_EnableInfinityRep)
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
  
        //Approaching Zero from Right(0.000...1)
        void SetAsApproachingZero()
        {
            IntValue = 0; DecimalHalf = ApproachingValRep;
            ExtraRep = 0;
        }
        
        //(0.99...9)
        void SetAsApproachingZeroFromLeft()
        {
            IntValue = 0; DecimalHalf = ApproachingValRep;
            ExtraRep = NegativeRep;
        }
        
        //Approaching Towards values from right to left side(IntValue.000...1)
        void SetAsApproachingValueFromRight(int value)
        {
            IntValue = value; DecimalHalf = ApproachingValRep;
            ExtraRep = 0;
        }
        
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        void SetAsApproachingBottomValue(int value)
        {
            IntValue = value; DecimalHalf = ApproachingValRep;
            ExtraRep = NegativeRep;
        }
private:
        static MediumDecVariant InfinityValue()
        {
            MediumDecVariant NewSelf = AltDec(1, InfinityRep);
            return NewSelf;
        }
        
        static MediumDecVariant NegativeInfinityValue()
        {
            MediumDecVariant NewSelf = AltDec(-1, InfinityRep);
            return NewSelf;
        }
        
        static MediumDecVariant ApproachingZeroValue()
        {
            MediumDecVariant NewSelf = AltDec(0, ApproachingValRep);
            return NewSelf;
        }
public:
#endif
#if defined(AltDec_EnableNaN)
        void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep;
            ExtraRep = 0;
        }

        static MediumDecVariant NaNValue()
        {
            MediumDecVariant NewSelf = AltDec(0, NaNRep);
            return NewSelf;
        }
#endif
        private:
#if defined(AltDec_EnablePIRep)
        void ConvertPIToNum()
        {

            ExtraRep = 0;
            // Can only convert to up 683565275.1688666254437963172038917047964296646843381624484789109135725652864987887127902610635528943x PIRepresentation
            //Can Represent up ? before hitting Maximum MediumDecVariant value on reconversion when AltDec_UseLowerPrecisionPI is enabled
            //otherwise can represent up to ???(when adding up value from each decimal place of IntValue + (PINum*DecimalHalf/1000000000))
            //Calculations from HiPer Calc
            //683565275.168866625 x 3.141592654 = 2147483646.99999999860577275
            //683565275.168866626 x 3.141592654 = 2147483647.000000001747365404
            if(IntValue>=683565275&&DecimalHalf>=168866626)//Exceeding Storage limit of NormalRep
            {
                //Display error/warning
                IntValue = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
            }
            else if(IntValue<=-683565275&&DecimalHalf>=168866626)//Exceeding Storage limit of NormalRep
            {
                //Display error/warning
                IntValue = -2147483647; DecimalHalf = 999999999;//set value as minimum value(since not truely infinite just bit above storage range)
            }
#if defined(AltDec_DisableSwitchBasedConversion)
            else if (IntValue == NegativeRep)//-0.XXX... * PI
            {
                BasicMultOp(PINum);
            }
            else if (DecimalHalf == 0 && IntValue == 10)
            {
                IntValue = 31; DecimalHalf = 415926536; 
            }
            else
            {
                BasicMultOp(PINum);
            }
#else
            else
            {
                switch (IntValue)
                {
                case NegativeRep:
                    BasicMultOp(PINum);
                    break;
                case 5:
                    if (DecimalHalf == 0)
                    {
                        IntValue = 15; DecimalHalf = 707963268;
                    }
                    else
                        BasicMultOp(PINum);
                    break;
                case 10:
                    if (DecimalHalf == 0)
                    {
                        IntValue = 31; DecimalHalf = 415926536;
                    }
                    else
                        BasicMultOp(PINum);
                    break;
                //3.1415926535897932384626433
                case 100:
                    if (DecimalHalf == 0)
                    {
                        IntValue = 314; DecimalHalf = 159265359;
                    }
                    else
                        BasicMultOp(PINum);
                    break;
                case 1000:
                    if (DecimalHalf == 0)
                    {
                        IntValue = 3141; DecimalHalf = 592653590;
                    }
                    else
                        BasicMultOp(PINum);
                    break;
                case -10:
                    if (DecimalHalf == 0)
                    {
                        IntValue = -31; DecimalHalf = 415926536;
                    }
                    else
                        BasicMultOp(PINum);
                    break;
                default:
                    BasicMultOp(PINum);
                    break;
                }
            }
#endif
        }
#endif

#if defined(AltDec_EnableENum)
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
            //Check for Non-ExtraRep focused special states first
#ifdef AltDec_EnableInfinityRep
            if(DecimalHalf==InfinityRep)
            {
                ExtraRep = 0;
                if(IntValue==1)//If Positive Infinity, then convert number into MaximumValue instead
                {
                    IntValue = 2147483647; DecimalHalf = 999999999;
                }
                else//If Negative Infinity, then convert number into MinimumValue instead
                {
                    IntValue = -2147483647; DecimalHalf = 999999999;
                }
                return;
            }
            else if(DecimalHalf==ApproachingValRep)
            {
                if (ExtraRep == 0)
                {
                    DecimalHalf = 1; ExtraRep = 0;
                }
                else
                {
                    DecimalHalf = 999999999; ExtraRep = 0;
                }
                return;
            }
#endif
#if defined(AltDec_EnableNaN) && defined(AltDec_EnableNaNConversionCheck)//Disable conversion check for NaN by default(unless AltDec_EnableNaNConversionCheck preprocessor added)
            if(DecimalHalf==NaNRep)//Set as Zero instead of NaN
            {
                SetAsZero(); return;
            }
#endif
            if(ExtraRep==0)//Skip converting if already normal number state(Equal to default MediumDec format)
                return;
#ifdef AltDec_EnablePIRep
            if(ExtraRep==PIRep)
            {
                ConvertPIToNum(); return;
            }
            else
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(ExtraRep<0)
            {
                IntValue = -2147483648;
            }
#elif defined(AltDec_EnableENum)
            if(ExtraRep<0)
            {
                ConvertEToNum(); return;
            }
#endif
#if defined(AltDec_OtherNegativeExtraRepsDefined) || defined(AltDec_EnablePIRep)
            else
            {
#endif
#if AltDec_EnableMixedFractional
                if(DecimalHalf<0)//Mixed Fraction
                {
                    int TempAdd = IntValue;
                    IntValue = DecimalHalf*-1; DecimalHalf = 0;
                    BasicIntDivOp(ExtraRep);
                    BasicAddOp(TempAdd);
                }
                else//Value Divided by ExtraRep
                {
#endif
#if defined(AltDec_EnableByDecimaledFractionals)
                    BasicIntDivOp(ExtraRep);
#endif
#if AltDec_EnableMixedFractional
                }
#endif
#if defined(AltDec_OtherNegativeExtraRepsDefined) || defined(AltDec_EnablePIRep)
            }
#endif
            ExtraRep = 0;
        }
        
        //Switch based version of ConvertToNumRep
        void ConvertToNormType(RepType& repType)
        {
#if defined(AltDec_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
            {
                ExtraRep = 0;
                if (IntValue == 1)//If Positive Infinity, then convert number into MaximumValue instead
                {
                    IntValue = 2147483647; DecimalHalf = 999999999;
                }
                else//If Negative Infinity, then convert number into MinimumValue instead
                {
                    IntValue = -2147483647; DecimalHalf = 999999999;
                }
                return;
            }
#endif
            switch (repType)
            {
            case RepType::NormalType:
                break;
#if defined(AltDec_EnableInfinityRep)
            case RepType::ApproachingBottom:
                DecimalHalf = 1; ExtraRep = 0;
                break;
            case RepType::ApproachingTop:
                DecimalHalf = 999999999; ExtraRep = 0;
                break;
#endif
#if defined(AltDec_EnablePIRep)
            case RepType::PINum:
                BasicMultOp(PINum);
                ExtraRep = 0;
                break;
#endif
            case RepType::NumByDiv:
                BasicIntDivOp(ExtraRep);
                ExtraRep = 0;
                break;
#if defined(AltDec_EnableENum)
            case RepType::ENum:
                BasicMultOp(ENum);
                ExtraRep = 0;
                break;
            case RepType::ENumByDiv:
                BasicMultOp(ENum);
                ExtraRep *= -1;
                BasicIntDivOp(ExtraRep);
                ExtraRep = 0;
                break;
#endif
            default:
                ConvertToNumRep();
                break;
            }
        }
    #pragma region ValueDefines
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
#if defined(AltDec_EnableInfinityRep)
        static MediumDecVariant ApproachingRightRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 999999999);
        }

        static MediumDecVariant ApproachingLeftRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 1);
        }

        static MediumDecVariant LeftAlmostPointFiveRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 499999999);
        }

        static MediumDecVariant RightAlmostPointFiveRealValue(int IntValue=0)
        {
            return AltDec(IntValue, 500000001);
        }
#endif

        /// <summary>
        /// Returns PI(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant PINumValue()
        {
            return AltDec(3, 141592654, 0);
        }

        //100,000,000xPI(Rounded to 9th decimal digit)
        static MediumDecVariant HundredMilPINumVal()
        {
            return AltDec(314159265, 358979324, 0);
        }

        //10,000,000xPI(Rounded to 9th decimal digit)
        static MediumDecVariant TenMilPINumVal()
        {
            return AltDec(31415926, 535897932, 0);
        }

        //1,000,000xPI(Rounded to 9th decimal digit)
        static MediumDecVariant OneMilPINumVal()
        {
            return AltDec(3141592, 653589793, 0);
        }

        //10xPI(Rounded to 9th decimal digit)
        static MediumDecVariant TenPINumVal()
        {
            return AltDec(31, 415926536, 0);
        }
        
        static MediumDecVariant ENumValue()
        {
            return AltDec(2, 718281828, 0);
        }
        
        static MediumDecVariant PIValue()
        {
            return AltDec(1, 0, -2147483648);
        }
        
        static MediumDecVariant ZeroValue()
        {
            return AltDec(0, 0, 0);
        }
        
        static MediumDecVariant EValue()
        {
#if defined(AltDec_EnableENum)
            return AltDec(1, 0, IERep);
#else
            return AltDec(2, 718281828, 0);
#endif
        }
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant OneValue()
        {
            MediumDecVariant NewSelf = AltDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant TwoValue()
        {
            MediumDecVariant NewSelf = AltDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant NegativeOneValue()
        {
            MediumDecVariant NewSelf = AltDec(-1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant Point5Value()
        {
            MediumDecVariant NewSelf = AltDec(0, 500000000);
            return NewSelf;
        }

        static MediumDecVariant JustAboveZeroValue()
        {
            MediumDecVariant NewSelf = AltDec(0, 1);
            return NewSelf;
        }

        static MediumDecVariant OneMillionthValue()
        {
            MediumDecVariant NewSelf = AltDec(0, 1000);
            return NewSelf;
        }

        static MediumDecVariant FiveThousandthValue()
        {
            MediumDecVariant NewSelf = AltDec(0, 5000000);
            return NewSelf;
        }

        static MediumDecVariant FiveMillionthValue()
        {
            MediumDecVariant NewSelf = AltDec(0, 5000);
            return NewSelf;
        }

        static MediumDecVariant TenMillionthValue()
        {
            MediumDecVariant NewSelf = AltDec(0, 100);
            return NewSelf;
        }

        static MediumDecVariant OneHundredMillionthValue()
        {
            MediumDecVariant NewSelf = AltDec(0, 10);
            return NewSelf;
        }

        static MediumDecVariant FiveBillionthValue()
        {
            MediumDecVariant NewSelf = AltDec(0, 5);
            return NewSelf;
        }

        static MediumDecVariant LN10Value()
        {
            return AltDec(2, 302585093);
        }

        static MediumDecVariant LN10MultValue()
        {
            return AltDec(0, 434294482);
        }

        static MediumDecVariant HalfLN10MultValue()
        {
            return AltDec(0, 868588964);
        }

        static MediumDecVariant NilValue()
        {
            return AltDec(-2147483648, -2147483648);
        }
        
        static MediumDecVariant MinimumValue()
        {
            return AltDec(2147483647, 999999999);
        }

        static MediumDecVariant MaximumValue()
        {
            return AltDec(2147483647, 999999999);
        }
public:
        static MediumDecVariant AlmostOne;

        /// <summary>
        /// Returns PI(3.1415926535897932384626433) with tenth digit rounded up(3.141592654)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant PINum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDec</returns>
        static MediumDecVariant ENum;
        
#if defined(AltDec_EnableInfinityRep)
        static MediumDecVariant Infinity;
        static MediumDecVariant NegativeInfinity;
        static MediumDecVariant ApproachingZero;
#endif
        
        /// <summary>
        /// Returns PI(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant PI;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>AltDec</returns>
        static MediumDecVariant E;
        
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
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDecVariant Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDecVariant Maximum;
        
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

        /// <summary>
        /// Nil Value as proposed by https://docs.google.com/document/d/19n-E8Mu-0MWCcNt0pQnFi2Osq-qdMDW6aCBweMKiEb4/edit
        /// </summary>
        static MediumDecVariant Nil;
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
        AltDec(const char* strVal)
        {
            std::string Value = strVal;
            if (Value == "PI")
            {
                this->SetVal(PI);
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
            if (Value == "PI")
            {
                this->SetVal(PI);
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
                IntValue = 2147483647;
                if (IsNegative)
                {
                    IntValue *= -1;
                }
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (float)WholeValue;
                if(IsNegative&&WholeValue==0)
                    IntValue = NegativeRep;
                else
                    IntValue = IsNegative ? WholeValue * -1 : WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
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
                IntValue = 2147483647;
                if (IsNegative)
                {
                    IntValue *= -1;
                }
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (double)WholeValue;
                if(IsNegative&&WholeValue==0)
                    IntValue = NegativeRep;
                else
                    IntValue = IsNegative ? WholeValue * -1 : WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
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
                IntValue = 2147483647;
                if (IsNegative)
                {
                    IntValue *= -1;
                }
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                Value -= (ldouble)WholeValue;
                IntValue = IsNegative ? WholeValue * -1 : WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
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

        AltDec(MediumDec Value)
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
        /// Equal to Operation Between AltDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator==(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltDec_EnableInfinityRep)
    #if defined(AltDec_EnableImaginaryNum)
                if(self.DecimalHalf!=InfinityRep&&(self.ExtraRep>=0||self.ExtraRep==PIRep))
    #else
                if(self.DecimalHalf!=InfinityRep)
    #endif
                {
                    self.ConvertToNumRep();
                }

    #if defined(AltDec_EnableImaginaryNum)
                if(Value.DecimalHalf!=InfinityRep&&(Value.ExtraRep>=0||Value.ExtraRep==PIRep))
    #else
                if(Value.DecimalHalf!=InfinityRep)
    #endif
                {
                    Value.ConvertToNumRep();
                }
#else
    #if defined(AltDec_EnableImaginaryNum)
                if(self.ExtraRep>=0||self.ExtraRep==PIRep)
                {
                    self.ConvertToNumRep();
                }
    #else
                self.ConvertToNumRep();
    #endif

    #if defined(AltDec_EnableImaginaryNum)
                if(Value.ExtraRep>=0||Value.ExtraRep==PIRep)
                {
                    Value.ConvertToNumRep();
                }
    #else
                Value.ConvertToNumRep();
    #endif
#endif
#if defined(AltDec_EnableImaginaryNum)
            return (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf && self.ExtraRep == Value.ExtraRep);
#else
            return (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf);
#endif
        }

        /// <summary>
        /// Not equal to Operation Between AltDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator!=(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltDec_EnableInfinityRep)
    #if defined(AltDec_EnableImaginaryNum)
                if(self.DecimalHalf!=InfinityRep&&(self.ExtraRep>=0||self.ExtraRep==PIRep))
    #else
                if(self.DecimalHalf!=InfinityRep)
    #endif
                {
                    self.ConvertToNumRep();
                }

    #if defined(AltDec_EnableImaginaryNum)
                if(Value.DecimalHalf!=InfinityRep&&(Value.ExtraRep>=0||Value.ExtraRep==PIRep))
    #else
                if(Value.DecimalHalf!=InfinityRep)
    #endif
                {
                    Value.ConvertToNumRep();
                }
#else
    #if defined(AltDec_EnableImaginaryNum)
                if(self.ExtraRep>=0||self.ExtraRep==PIRep)
                {
                    self.ConvertToNumRep();
                }
    #else
                self.ConvertToNumRep();
    #endif

    #if defined(AltDec_EnableImaginaryNum)
                if(Value.ExtraRep>=0||Value.ExtraRep==PIRep)
                {
                    Value.ConvertToNumRep();
                }
    #else
                Value.ConvertToNumRep();
    #endif
#endif
#if defined(AltDec_EnableImaginaryNum)
            return (self.IntValue != Value.IntValue || self.DecimalHalf != Value.DecimalHalf || self.ExtraRep != Value.ExtraRep);
#else
            return (self.IntValue != Value.IntValue || self.DecimalHalf != Value.DecimalHalf);
#endif
        }

        /// <summary>
        /// Lesser than Operation Between AltDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(Value.ExtraRep==InfinityRep)
                {
                    return self.IntValue<Value.IntValue;
                }
            }
            else if(Value.ExtraRep==InfinityRep)
            {
            
            }
#endif
            self.ConvertToNumRep(); Value.ConvertToNumRep();
            if (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf) { return false; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValueIsNegative = Value.IntValue < 0;
                if (ValueIsNegative && SelfIsNegative == false) { return false; }// 5 > -5
                else if (ValueIsNegative == false && SelfIsNegative) { return true; }// -5 <5
                else
                {//Both are either positive or negative
                    if (Value.DecimalHalf == 0)
                    {
                        if (self.DecimalHalf == 0)
                            return self.IntValue < Value.IntValue;
                        else
                        {
                            if (self.IntValue == NegativeRep)
                            {//-0.5<0
                                if (Value.IntValue >= 0)
                                    return true;
                            }
                            else if (self.IntValue < Value.IntValue) { return true; }//5.5 < 6
                            else if (self.IntValue == Value.IntValue) { return self.IntValue < 0 ? true : false; }//-5.5<-5 vs 5.5 > 5
                        }
                    }
                    else if (self.DecimalHalf == 0)
                    {
                        if (Value.IntValue == NegativeRep)
                        {
                            if (self.IntValue <= -1)
                                return true;
                        }
                        else if (self.IntValue < Value.IntValue)
                            return true;// 5 < 6.5
                        else if (Value.IntValue == self.IntValue)
                            return Value.IntValue < 0 ? false : true;//5 < 5.5 vs -5 > -5.5
                    }
                    //Assuming both are non-whole numbers if reach here
                    if (self.IntValue == NegativeRep)
                        self.IntValue = 0;
                    if (Value.IntValue == NegativeRep)
                        Value.IntValue = 0;
                    if (SelfIsNegative)
                    {//Larger number = farther down into negative
                        if (self.IntValue > Value.IntValue)
                            return true;
                        else if (self.IntValue == Value.IntValue)
                            return self.DecimalHalf > Value.DecimalHalf;
                    }
                    else
                    {
                        if (self.IntValue < Value.IntValue)
                            return true;
                        else if (self.IntValue == Value.IntValue)
                            return self.DecimalHalf < Value.DecimalHalf;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Lesser than or Equal to Operation Between AltDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<=(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==Value.IntValue||Value.IntValue==1)
                    return true;
                else
                    return false;
            }
            else if(Value.ExtraRep==InfinityRep)
            {
            
            }
#endif
            self.ConvertToNumRep(); Value.ConvertToNumRep();
            if (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf) { return true; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValueIsNegative = Value.IntValue < 0;
                if (ValueIsNegative && SelfIsNegative == false) { return false; }//5>=-5
                else if (ValueIsNegative == false && SelfIsNegative) { return true; }//-5<=5
                else
                {
                    if (Value.DecimalHalf == 0)
                    {
                        if (self.DecimalHalf == 0)
                            return self.IntValue <= Value.IntValue;
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
                    }
                    else if (self.DecimalHalf == 0)
                    {
                        if (Value.IntValue == NegativeRep && self.IntValue <= 1)
                        {//-1<-0.5
                            if (self.IntValue <= -1)
                                return true;
                        }
                        else if (self.IntValue < Value.IntValue)
                            return true;
                        else if (Value.IntValue == self.IntValue)
                            return Value.IntValue < 0 ? false : true;//5 <= 5.5 vs -5 >= -5.5
                    }
                    //Assuming both are non-whole numbers if reach here
                    if (self.IntValue == NegativeRep)
                        self.IntValue = 0;
                    if (Value.IntValue == NegativeRep)
                        Value.IntValue = 0;
                    if (SelfIsNegative)//Both are either positive or negative
                    {//Larger number = farther down into negative
                        if (self.IntValue > Value.IntValue)
                            return true;
                        else if (self.IntValue == Value.IntValue)
                            return self.DecimalHalf > Value.DecimalHalf;
                    }
                    else
                    {
                        if (self.IntValue < Value.IntValue)
                            return true;
                        else if (self.IntValue == Value.IntValue)
                            return self.DecimalHalf < Value.DecimalHalf;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Greater than Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>bool</returns>
        friend bool operator>(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(Value.ExtraRep==InfinityRep)
                {
                    if(self.IntValue==Value.IntValue)
                        return false;
                    else if(Value.IntValue==1)
                        return false;
                    else
                        return true;
                }
            }
            else if(Value.ExtraRep==InfinityRep)
            {
            
            }
#endif
            self.ConvertToNumRep(); Value.ConvertToNumRep();
            if (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf) { return false; }
            else
            {

                bool SelfIsNegative = self.IntValue < 0;
                bool ValueIsNegative = Value.IntValue < 0;
                if (ValueIsNegative && SelfIsNegative == false) { return true; }//5 > -5
                else if (ValueIsNegative == false && SelfIsNegative) { return false; }//-5<5
                else if (Value.DecimalHalf == 0)
                {
                    if (self.DecimalHalf == 0)
                        return self.IntValue > Value.IntValue;
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
                }
                else if (self.DecimalHalf == 0)
                {
                    if (Value.IntValue == NegativeRep)
                    {
                        if (self.IntValue >= 0)
                            return true;
                    }
                    else if (self.IntValue > Value.IntValue)
                        return true;
                    else if (Value.IntValue == self.IntValue)
                        return Value.IntValue < 0 ? true : false;//5 < 5.5 vs -5 > -5.5
                }
                //Assuming both are non-whole numbers if reach here
                if (self.IntValue == NegativeRep)
                    self.IntValue = 0;
                if (Value.IntValue == NegativeRep)
                    Value.IntValue = 0;
                if (SelfIsNegative)//Both are either positive or negative
                {//Larger number = farther down into negative
                    if (self.IntValue < Value.IntValue)
                        return true;
                    else if (self.IntValue == Value.IntValue)
                        return self.DecimalHalf < Value.DecimalHalf;
                }
                else
                {
                    if (self.IntValue > Value.IntValue)
                        return true;
                    else if (self.IntValue == Value.IntValue)
                        return self.DecimalHalf > Value.DecimalHalf;
                }
            }
            return false;
        }

        /// <summary>
        /// Greater than or Equal to Operation Between AltDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>=(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(Value.ExtraRep==InfinityRep)
                {
                    if(self.IntValue==Value.IntValue||Value.IntValue==-1)
                        return true;
                    else
                        return false;
                }
                else
                {
                
                }
            }
            else if(Value.ExtraRep==InfinityRep)
            {
            
            }
#endif
            self.ConvertToNumRep(); Value.ConvertToNumRep();
            if (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf) { return true; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValueIsNegative = Value.IntValue < 0;
                if (ValueIsNegative && SelfIsNegative == false) { return true; }
                else if (ValueIsNegative == false && SelfIsNegative) { return false; }
                else if (Value.DecimalHalf == 0)
                {
                    if (self.DecimalHalf == 0)
                        return self.IntValue >= Value.IntValue;
                    else
                    {
                        if (self.IntValue == NegativeRep)
                        {
                            if (Value <= -1)
                                return true;
                        }
                        else if (self.IntValue > Value)
                            return true;
                        else if (self.IntValue == Value)
                            return self.IntValue < 0 ? false : true;//-5.5<-5 vs 5.5>5
                    }
                }
                else if (self.DecimalHalf == 0)//return self.IntValue > Value;
                {
                    if (Value.IntValue == NegativeRep)
                    {//0>-0.5
                        if (self.IntValue >= 0)
                            return true;
                    }
                    else if (self.IntValue > Value.IntValue)
                        return true;
                    else if (Value.IntValue == self.IntValue)
                        return Value.IntValue < 0 ? true : false;//5 <= 5.5 vs -5 >= -5.5
                }
                //Assuming both are non-whole numbers if reach here
                if (self.IntValue == NegativeRep)
                    self.IntValue = 0;
                if (Value.IntValue == NegativeRep)
                    Value.IntValue = 0;
                if (SelfIsNegative)//Both are either positive or negative
                {//Larger number = farther down into negative
                    if (self.IntValue < Value.IntValue)//-5.5 >= -6.5
                        return true;
                    else if (self.IntValue == Value.IntValue)//-5.5 >= -5.6
                        return self.DecimalHalf < Value.DecimalHalf;
                }
                else
                {
                    if (self.IntValue > Value.IntValue)
                        return true;
                    else if (self.IntValue == Value.IntValue)
                        return self.DecimalHalf > Value.DecimalHalf;
                }
            }
            return false;
        }

    #pragma endregion Comparison Operators

#pragma region RepToRepCode_AdditionSubtraction
    private:
        void RepToRepAddOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
        {
            switch (LRep)
            {
                //X.?? + ???
                case RepType::NormalType:
                    switch (RRep)
                    {
                        case RepType::NumByDiv:
                            Value.ConvertToNormType(RRep);
                            self.BasicAddOp(Value);
                            break;
                        case RepType::PINum:
                            Value.ConvertPIToNum();
                            self.BasicAddOp(Value);
                            break;
#if defined(AltDec_EnableENum)
                        case RepType::ENum:
                        case RepType::ENumByDiv:
                            Value.ConvertEToNum();
                            self.BasicAddOp(Value);
                            break;
#endif
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                //(X.??*PI) + ???
                case RepType::PINum:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertPIToNum();
                            self.BasicAddOp(Value);
                            break;
                        default:
                        //case RepType::ENum: //(X.??*PI) + (Y.??*e)
                        //case RepType::ENumByDiv: //(X.??*PI) + ((Y.??*e)/(Y_ExtraRep*-1))
                        //case RepType::ApproachingTop: //(X.??*PI) + Y.9...9
                        //case RepType::ApproachingBottom: //(X.??*PI) + Y.0...1
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                //(X.??/X_ExtraRep) + ???
                case RepType::NumByDiv:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertToNormType(LRep);
                            self.BasicAddOp(Value);
                            break;
                        case RepType::ApproachingTop:
                            self.ConvertToNormType(LRep);
                            self.BasicAddOp(AlmostOne);
                            break;
                        case RepType::ApproachingBottom:
                            self.ConvertToNormType(LRep);
                            self.BasicAddOp(JustAboveZero);
                            break;
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingBottom://IntValue.000...1 + ???
                    switch (RRep)
                    {
                        case RepType::ApproachingTop:
                            if(self.IntValue<0)
                            {
                                if (self.IntValue == NegativeRep)
                                {
                                    if(Value.IntValue == NegativeRep)//-0.0.......1 + -0.9.......9
                                    {
                                        self.IntValue = -1; self.DecimalHalf = 0;
                                        self.ExtraRep = 0; return self;
                                    }
                                    else if(Value.IntValue<0)//-0.0.......1 + -?.9.......9
                                    {
                                        self.IntValue--;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                        self.IntValue += Value.IntValue;
                                    }
                                    else
                                    {
                                        self.CatchAllAddition(Value, LRep, RRep);
                                    }
                                }
                                else
                                {
                                    if(Value.IntValue == NegativeRep)//-?.9.......9 + -0.0.......1
                                    {
                                        self.IntValue--;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                    }
                                    else if(Value.IntValue<0)//-?.9.......9 + -?.0.......1
                                    {
                                        self.IntValue--;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                        self.IntValue += Value.IntValue;
                                    }
                                    else
                                    {
                                        self.CatchAllAddition(Value, LRep, RRep);
                                    }
                                }
                            }
                            else
                            {
                                if(Value.IntValue<0)
                                {
                                    self.CatchAllAddition(Value, LRep, RRep);
                                }
                                else if(self.IntValue==0)//0.99...9 + Y.00...1
                                {
                                    self.IntValue = Value.IntValue+1;
                                    self.ExtraRep = 0; self.DecimalHalf = 0;
                                }
                                else//X.99...9 + Y.00...1
                                {
                                    self.IntValue++;
                                    self.ExtraRep = 0; self.DecimalHalf = 0;
                                    self.IntValue += Value.IntValue;
                                }
                            }
       //                     if (self.IntValue == NegativeRep)
       //                     {
       //                         if (Value.IntValue == NegativeRep)//-0.0.......1 + -0.9.......9
       //                         {
       //                             self.IntValue = -1; self.DecimalHalf = 0;
       //                             self.ExtraRep = 0; return self;
       //                         }
       //                     }
       //                     else if (Value.IntValue == NegativeRep)
       //                     {
       //					  	self.CatchAllAddition(Value, LRep, RRep);
       //                     }
       //                     else if (self.IntValue < 0)
       //                     {
       //                         if (Value.IntValue < 0)
       //                         {
       //                             self.IntValue++;
       //                             self.ExtraRep = 0;
       //                         }
       //                         self.IntValue += Value.IntValue;
       //                     }
       //                     else
       //                     {
       //                         if (Value.IntValue>= 0)
       //                         {
       //                             self.IntValue++;
       //                             self.ExtraRep = 0;
       //                         }
       //                         self.IntValue += Value.IntValue;
       //                     }
                            break;
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingTop://IntValue.9999... + ???
                    switch (RRep)
                    {
                        case RepType::ApproachingBottom:
                            if(self.IntValue<0)
                            {
                                if (self.IntValue == NegativeRep)
                                {
                                    if(Value.IntValue == NegativeRep)//-0.9.......9 + -0.0.......1
                                    {
                                        self.IntValue = -1; self.DecimalHalf = 0;
                                        self.ExtraRep = 0; return self;
                                    }
                                    else if(Value.IntValue<0)//-0.9.......9 + -?.0.......1
                                    {
                                        self.IntValue = Value.IntValue - 1;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                    }
                                    else
                                    {
                                        self.CatchAllAddition(Value, LRep, RRep);
                                    }
                                }
                                else
                                {
                                    if(Value.IntValue == NegativeRep)//-?.9.......9 + -0.0.......1
                                    {
                                        self.IntValue--;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                    }
                                    else if(Value.IntValue<0)//-?.9.......9 + -?.0.......1
                                    {
                                        self.IntValue--; self.DecimalHalf = 0;
                                        self.ExtraRep = 0;
                                        self.IntValue += Value.IntValue;
                                    }
                                    else
                                    {
                                        self.CatchAllAddition(Value, LRep, RRep);
                                    }
                                }
                            }
                            else
                            {
                                if(Value.IntValue<0)
                                {
                                    self.CatchAllAddition(Value, LRep, RRep);
                                }
                                else if(self.IntValue==0)//0.99...9 + Y.00...1
                                {
                                    self.IntValue = Value.IntValue+1;
                                    self.ExtraRep = 0; self.DecimalHalf = 0;
                                }
                                else//X.99...9 + Y.00...1
                                {
                                    self.IntValue++;
                                    self.ExtraRep = 0; self.DecimalHalf = 0;
                                    self.IntValue += Value.IntValue;
                                }
                            }
                            break;
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#if defined(AltDec_EnableENum)
                case RepType::ENum:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertEToNum();
                            self.BasicAddOp(Value);
                            break;
      //                  case RepType::ENumByDiv:
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ENumByDiv:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertEToNum();
                            self.BasicAddOp(Value);
                            break;
      //                  case RepType::ENum:
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#elif defined(AltDec_EnableImaginaryNum)
                case RepType::INum:
                    //if(RRep==RepType::INumByDiv)
                    //{
                    //}
                    //else
                    //{
                        throw "Can't add imaginary numbers to real numbers unless complex representation enabled.";
                    //}
                    break;
#elif defined(AltDec_EnablePIPowers)
                case RepType::PIPower:
                    switch (RRep)
                    {
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#endif
#if defined(AltDec_EnableMixedFractional)
                case RepType::MixedFrac:
                    switch (RRep)
                    {
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#if defined(AltDec_EnableENum)
                case RepType::MixedE:
                    switch (RRep)
                    {
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#elif defined(AltDec_EnableImaginaryNum)
                case RepType::MixedE:
#if defined(AltDec_EnableComplexNum)
     //               switch (RRep)
     //               {
     //                   default:
     //                       self.CatchAllAddition(Value, LRep, RRep);
     //                       break;
                    //}
                    //break;
//                case RepType::ComplexIRep:
//                    switch (RRep)
//                    {
////                        case RepType::NormalType:
////                            break;
//                        default:
//                            self.CatchAllAddition(Value, LRep, RRep);
//                            break;
//					}
//					break;
#else
                    switch (RRep)
                    {
                        //case RepType::INum:
                        //	break;
                        //case RepType::INumByDiv:
                        //	break;
                        default:
                            throw "Can't add imaginary numbers to real numbers unless complex representation enabled.";
                            break;
                    }
                    break;
#endif
#endif
                default:
                    self.CatchAllAddition(Value, LRep, RRep);
                    break;
                
            }
        }
        
        void RepToRepSubOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
        {
            switch (LRep)
            {
                case RepType::NormalType:
                    switch (RRep)
                    {
                        case RepType::NumByDiv:
                            Value.ConvertToNormType(RRep);
                            self.BasicSubOp(Value);
                            break;
                        case RepType::PINum:
                            Value.ConvertPIToNum();
                            self.BasicSubOp(Value);
                            break;
#if defined(AltDec_EnableENum)
                        case RepType::ENum:
                        case RepType::ENumByDiv:
                            Value.ConvertEToNum();
                            self.BasicSubOp(Value);
                            break;
#endif
                        default:
                            self.CatchAllSubtraction(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::PINum:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertPIToNum();
                            self.BasicSubOp(Value);
                            break;
                        default://ENum,ENumByDiv,ApproachingTop,ApproachingBottom  Included
                            self.CatchAllSubtraction(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::NumByDiv:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertToNormType(LRep);
                            self.BasicSubOp(Value);
                            break;
                        case RepType::ApproachingTop:
                            self.ConvertToNormType(LRep);
                            self.BasicSubOp(AlmostOne);
                            break;
                        case RepType::ApproachingBottom:
                            self.ConvertToNormType(LRep);
                            self.BasicSubOp(JustAboveZero);
                            break;
                        default:
                            self.CatchAllSubtraction(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingBottom://IntValue.000...1
                    switch (RRep)
                    {
                        case RepType::ApproachingTop:
                            if(self.IntValue<0)
                            {
                                if(Value.IntValue<0)//If not positive, maybe normalize both to NormalRep
                                {
                                    self.CatchAllSubtraction(Value, LRep, RRep);
                                }
                                else//-X.000...1 - Y.99...9 = (X - Y)-1
                                {
                                    if (self.IntValue==NegativeRep&&Value.IntValue == 0)//-0.0.......1 - 0.9.......9
                                    {
                                        self.IntValue = -1; self.DecimalHalf = 0;
                                        return self;
                                    }
                                    else
                                    {
                                        self.IntValue--;
                                        self.DecimalHalf = 0;
                                        self.IntValue -= Value.IntValue;
                                    }
                                }
                            }
                            else
                            {
                                if(Value.IntValue<0)//X.000...1 - -Y.99...9 = (X + Y)+1
                                {
                                    if (self.IntValue==0&&Value.IntValue == NegativeRep)//0.0.......1 + 0.9.......9
                                    {
                                        self.IntValue = 1; self.DecimalHalf = 0;
                                        return self;
                                    }
                                    else
                                    {
                                        self.IntValue++;
                                        self.DecimalHalf = 0;
                                        self.IntValue -= Value.IntValue;
                                    }
                                }
                                else//Otherwise maybe normalize both to NormalRep
                                {
                                    self.CatchAllSubtraction(Value, LRep, RRep);
                                }
                            }
                            break;
                        default:
                            self.CatchAllSubtraction(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingTop://IntValue.9999...
                    switch (RRep)
                    {
                        case RepType::ApproachingBottom:
\
                            break;
                        default:
                            self.CatchAllSubtraction(Value, LRep, RRep);
                            break;
                    }
                    break;
#if defined(AltDec_EnableENum)
                case RepType::ENum:
                    switch (RRep)
                    {
//						case RepType::NormalType:
//							self.ConvertEToNum();
//							self.BasicSubOp(Value);
//							break;
                        default:
                            self.CatchAllSubtraction(Value, LRep, RRep);
                            break;
                    }
                    break;
#endif
#if defined(AltDec_EnableImaginaryNum)
                case RepType::INum:
                    throw "Can't subtract imaginary numbers from real numbers unless complex representation enabled.";
                    break;
#endif
                default:
                    self.CatchAllSubtraction(Value, LRep, RRep);
                    break;
                
            }
        }
#pragma endregion RepToRepCode_AdditionSubtraction
#pragma region RepToRepCode_MultiplicationDivision
        void RepToRepMultOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
        {
            switch (LRep)
            {
                case RepType::NormalType:
                    switch (RRep)
                    {
                        case RepType::NumByDiv://X * Y/Z
                            self.BasicMultOp(Value);
                            self.ExtraRep = Value.ExtraRep;
                            break;
                        case RepType::PINum://X * (Y*Pi)
                            self.BasicMultOp(Value);
                            self.ExtraRep = PIRep;
                            break;
#if defined(AltDec_EnableENum)
                        case RepType::ENum://X * (Y*e)
                            self.BasicMultOp(Value);
                            self.ExtraRep = IERep;
                            break;
                        case RepType::ENumByDiv://X * (Y*e)/Z
                            self.BasicMultOp(Value);
                            self.ExtraRep = Value.ExtraRep;
                            break;
#endif
                        default:
                            self.CatchAllMultiplication(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::PINum:
                    switch (RRep)
                    {
                        case RepType::NormalType://X*Pi * Y
                            self.BasicMultOp(Value);
                            break;
                        case RepType::NumByDiv://X*Pi * Y/Z
                            self.BasicMultOp(Value);
                            self.BasicDivIntOp(Value.ExtraRep);
                            break;
#if defined(AltDec_EnableENum)
                        case RepType::ENum://X * (Y*e)
                            self.BasicMultOp(Value);
                            self.ExtraRep = IERep;
                            break;
                        case RepType::ENumByDiv://X * (Y*e)/Z
                            self.BasicMultOp(Value);
                            self /= Value.ExtraRep;
                            break;
#endif
                        case RepType::ApproachingTop://Normalize Value to normalRep before multiplying
                                Value.DecimalHalf = 999999999;
                                Value.ExtraRep = 0;
                                self.BasicMultOp(Value);
                            break;
                        case RepType::ApproachingBottom:
                            if(Value.IntValue==0)//Infinitely approaching zero so make result approaching zero
                            {
                                self.IntValue = self.IntValue<0?NegativeZero:0;
                                self.DecimalHalf = ApproachingValRep;
                                self.ExtraRep = 0;
                            }
                            else//Normalize Value instead in most other cases
                            {
                                Value.DecimalHalf = 1;
                                self.BasicMultOp(Value);
                            }
                            break;
                        default:
                            self.CatchAllMultiplication(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::NumByDiv:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.BasicMultOp(Value);
                            break;
//						case RepType::ApproachingTop:
//							self.ConvertToNormType(LRep);
//							self.BasicMultOp(AlmostOne);
//							break;
                        case RepType::ApproachingBottom:
                            if(Value.IntValue==0)//Infinitely approaching zero so make result approaching zero
                            {
                                self.IntValue = self.IntValue<0?NegativeZero:0;
                                self.DecimalHalf = ApproachingValRep;
                                self.ExtraRep = 0;
                            }
                            else//Normalize Value instead in most other cases
                            {
                                Value.DecimalHalf = 1;
                                self.BasicMultOp(Value);
                            }
                            break;
                        default:
                            self.CatchAllMultiplication(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingBottom://IntValue.000...1
                    switch (RRep)
                    {
                        default:
                            if(self.IntValue==0)
                            {
                                self.IntValue = Value.IntValue<0?NegativeRep:0;
                            }
                            else if(self.IntValue==NegativeRep)
                            {
                                self.IntValue = Value.IntValue<0?0:NegativeRep;
                            }
                            else
                                self.CatchAllMultiplication(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingTop://IntValue.9999...
                    switch (RRep)
                    {
                        case RepType::ApproachingBottom:
                            if(Value.IntValue==0)
                            {
                                self.IntValue = self.IntValue<0?NegativeRep:0; self.ExtraRep = 0;
                            }
                            else if(Value.IntValue==NegativeRep)
                            {
                                self.IntValue = self.IntValue<0?0:NegativeRep; self.ExtraRep = 0;
                            }
                            else
                                self.CatchAllMultiplication(Value, LRep, RRep);
                        default:
                            self.CatchAllMultiplication(Value, LRep, RRep);
                            break;
                    }
                    break;
#if defined(AltDec_EnableENum)
                case RepType::ENum:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.BasicMultOp(Value);
                            break;
                        case RepType::NumByDiv://X*e * Y/Z
                            self.BasicMultOp(Value);
                            self.BasicDivIntOp(Value.ExtraRep);
                            break;
                        case RepType::ApproachingBottom:
                        case RepType::ApproachingTop:
                            Value.ConvertToNormalType(RRep);
                            self.BasicMultOp(Value);
                            break;
                        default:
                            self.CatchAllMultiplication(Value, LRep, RRep);
                            break;
                    }
                    break;
#elif defined(AltDec_EnableImaginaryNum)
                case RepType::INum:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.BasicMultOp(Value);
                            break;
                        case RepType::NumByDiv://X*i * Y/Z
                            self.BasicMultOp(Value);
                            self.BasicDivIntOp(Value.ExtraRep);
                            break;
                        case RepType::ApproachingBottom:
                        case RepType::ApproachingTop:
                            Value.ConvertToNormalType(RRep);
                            self.BasicMultOp(Value);
                            break;
                        default:
                            throw "Imaginary numbers multiplication by other RepType not coded yet.";
                    }

                    break;
#endif
                default:
                    self.CatchAllMultiplication(Value, LRep, RRep);
                    break;
            }
        }
        
        void RepToRepDivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
        {
            switch (LRep)
            {
                case RepType::NormalType:
                    switch (RRep)
                    {
                        //case RepType::NumByDiv:
                        //	Value.ConvertToNormType(RRep);
                        //	self.BasicDivOp(Value);
                        //	break;
                        //case RepType::PINum:
                        //	Value.ConvertPIToNum();
                        //	self.BasicDivOp(Value);
                        //	break;
#if defined(AltDec_EnableENum)
                        //case RepType::ENum:
                        //case RepType::ENumByDiv:
                        //	Value.ConvertEToNum();
                        //	self.BasicDivisionOp(Value);
                        //	break;
#endif
                        default:
                            self.CatchAllDivision(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::PINum:
                    switch (RRep)
                    {
//						case RepType::NormalType:
//							self.ConvertPIToNum();
//							self.BasicDivOp(Value);
//							break;
                        default://ENum,ENumByDiv,ApproachingTop,ApproachingBottom  Included
                            self.CatchAllDivision(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::NumByDiv:
                    switch (RRep)
                    {
//						case RepType::NormalType:
//							self.ConvertToNormType(LRep);
//							self.BasicDivOp(Value);
//							break;
//						case RepType::ApproachingTop:
//							self.ConvertToNormType(LRep);
//							self.BasicDivOp(AlmostOne);
//							break;
//						case RepType::ApproachingBottom:
//							self.ConvertToNormType(LRep);
//							self.BasicDivOp(JustAboveZero);
//							break;
                        default:
                            self.CatchAllDivision(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingBottom://IntValue.000...1
                    switch (RRep)
                    {
//						case RepType::ApproachingTop:
                        default:
                            self.CatchAllDivision(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingTop://IntValue.9999...
                    switch (RRep)
                    {
//						case RepType::ApproachingBottom:
                        default:
                            self.CatchAllDivision(Value, LRep, RRep);
                            break;
                    }
                    break;
#if defined(AltDec_EnableENum)
                case RepType::ENum:
                    switch (RRep)
                    {
//						case RepType::NormalType:
//							self.ConvertEToNum();
//							self.BasicAddOp(Value);
//							break;
                        default:
                            self.CatchAllDivision(Value, LRep, RRep);
                            break;
                    }
                    break;
#elif defined(AltDec_EnableImaginaryNum)
                case RepType::INum:
                    switch (RRep)
                    {
                        //case RepType::NormalType:
                        //self.BasicDivisionOp(Value);
                        default:
                            throw "Imaginary numbers division by other RepType not coded yet.";
                    }

                    break;
#endif
                default:
                    self.CatchAllDivision(Value, LRep, RRep);
                    break;
                
            }
        }
    public:
#pragma endregion RepToRepCode_MultiplicationDivision
#pragma region Other_RepToRepCode
#pragma endregion Other_RepToRepCode

#pragma region Addition/Subtraction Operations
        /// <summary>
        /// Basic Addition Operation Between AltDecs
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicAddOp(MediumDecVariant& Value)
        {
            if (Value.DecimalHalf == 0)
            {
                if (Value.IntValue == 0)//(Value == Zero)
                    return;
                if (DecimalHalf == 0)
                {
                    IntValue += Value.IntValue;
                }
                else
                {
                    bool WasNegative = IntValue < 0;
                    if (WasNegative)
                        IntValue = IntValue == MediumDecVariant::NegativeRep ? -1 : --IntValue;
                    IntValue += Value.IntValue;
                    if (IntValue == -1)
                        IntValue = DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                    else if (IntValue < 0)
                        ++IntValue;
                    //If flips to other side of negative, invert the decimals
                    if ((WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0))
                        DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
                }
            }
            else
            {
                bool WasNegative = IntValue < 0;
                //Deal with Int section first
                if (WasNegative)
                    IntValue = IntValue == MediumDecVariant::NegativeRep ? -1 : --IntValue;
                if (Value.IntValue != 0 && Value.IntValue != MediumDecVariant::NegativeRep)
                    IntValue += Value.IntValue;
                //Now deal with the decimal section
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
                if (IntValue == -1)
                    IntValue = DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                else if (IntValue < 0)
                    ++IntValue;
                //If flips to other side of negative, invert the decimals
                if ((WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0))
                    DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
            }
        }

private:
        void CatchAllAddition(MediumDecVariant& Value, RepType& LRep, RepType& RRep)
        {
            ConvertToNormType(LRep);
            Value.ConvertToNormType(RRep);
            BasicAddOp(Value);
        }
public:
        /// <summary>
        /// Addition Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant& AddOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
            if (self == Zero)
            {
                self.IntValue = Value.IntValue; self.DecimalHalf = Value.DecimalHalf;
                self.ExtraRep = Value.ExtraRep; return self;
            }
            else if (Value == Zero)
                return self;
#if defined(AltDec_EnableInfinityRep)
            if (self.DecimalHalf==InfinityRep)
                return self;
            if (Value.DecimalHalf == InfinityRep)
            {
                Value.IntValue == 1 ? self.SetAsInfinity() : self.SetAsNegativeInfinity();
                return self;
            }
#endif
            RepType LRep = self.GetRepType();
            RepType RRep = Value.GetRepType();
            if(LRep==RRep)
            {
                switch (LRep)
                {
                    case RepType::NormalType:
                    case RepType::PINum:
#if defined(AltDec_EnableENum)
                    case RepType::ENum:
#elif defined(AltDec_EnableImaginaryNum)
                    case RepType::INum:
#endif
                        self.BasicAddOp(Value);
                        break;

#if AltDec_EnableMixedFractional
#endif
                    case RepType::ApproachingBottom:
                        if (self.IntValue == NegativeRep)
                        {
                            if (Value.IntValue == 0)//0.01-0.01
                                self.SetAsZero();
                            else if (Value.IntValue == NegativeRep)//-0.01 - 0.01
                            {/*Do Nothing*/}
                            else if (Value.IntValue < 0)//-0.01 - 1.01
                            {
                                self.IntValue = Value.IntValue;
                            }
                            else
                            {
                            }
                        }
                        else if (Value.IntValue == NegativeRep)
                        {
                            if (self.IntValue == 0)//0.01-0.01
                                self.SetAsZero();
                            else if (self.IntValue < 0)//-1.01 - 0.01
                            {/*Do Nothing*/}
                            else
                            {
                            }
                        }
                        else if (self.IntValue < 0)
                        {
                            if (Value.IntValue < 0)//-1.01 - 1.01
                            {
                                self.IntValue += Value.IntValue;
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            if (Value.IntValue < 0)
                            {
                            }
                            else//1.01+1.01
                            {
                                self.IntValue += Value.IntValue;
                            }
                        }
                        break;
                    case RepType::ApproachingTop:
                        if (self.IntValue == NegativeRep)
                        {
                            if (Value.IntValue == 0)//0.9-0.9
                                self.SetAsZero();
                            else if (Value.IntValue == NegativeRep)//-0.9 - 0.9
                                self.IntValue = -1;
                            else if (Value.IntValue < 0)//-0.9 - 1.9
                            {
                                self.IntValue = Value.IntValue - 1;
                            }
                            else
                            {
                            }
                        }
                        else if (Value.IntValue == NegativeRep)
                        {
                            if (self.IntValue == 0)//0.9-0.9
                                self.SetAsZero();
                            else if (self.IntValue < 0)//-1.9 - 0.9
                            {
                                --self.IntValue;
                            }
                            else
                            {
                            }
                        }
                        else if (self.IntValue < 0)
                        {
                            if (Value.IntValue < 0)//-1.9 - 1.9
                            {
                                self.IntValue += Value.IntValue - 1;
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            if (Value.IntValue < 0)
                            {
                            }
                            else
                            {
                                self.IntValue += Value.IntValue;
                            }
                        }
                        break;
                    case RepType::NumByDiv:
                        if(self.ExtraRep==Value.ExtraRep)
                            self.BasicAddOp(Value);
                        else
                            self.CatchAllAddition(Value, LRep, RRep);
                        break;
#if defined(AltDec_EnableENum)
                    case RepType::ENumByDiv:
                        if (self.ExtraRep == Value.ExtraRep)
                            self.BasicAddOp(Value);
                        else
                            self.CatchAllAddition(Value, LRep, RRep);
                        break;
#elif defined(AltDec_EnableImaginaryNum)
                    case RepType::INumByDiv:
                        if (self.ExtraRep == Value.ExtraRep)
                            self.BasicAddOp(Value);
                        else
                            self.CatchAllAddition(Value, LRep, RRep);
                        break;
#endif
                    default:
                        self.CatchAllAddition(Value, LRep, RRep);
                        break;
                }
            }
            else
            {
                RepToRepAddOp(LRep, RRep, self, Value);
            }
            return self;
        }

        /// <summary>
        /// Basic Subtraction Operation Between AltDecs
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicSubOp(MediumDecVariant& Value)
        {
            if (Value.DecimalHalf == 0)
            {
                if (Value.IntValue == 0)//(Value == Zero)
                    return;
                if (DecimalHalf == 0)
                {
                    IntValue -= Value.IntValue;
                }
                else
                {
                    bool WasNegative = IntValue < 0;
                    if (WasNegative)
                        IntValue = IntValue == MediumDecVariant::NegativeRep ? -1 : --IntValue;
                    if (Value.IntValue != 0)
                        IntValue -= Value.IntValue;
                    if(IntValue==-1)
                        IntValue = DecimalHalf == 0?0:MediumDecVariant::NegativeRep;
                    else if(IntValue<0)
                        ++IntValue;
                    //If flips to other side of negative, invert the decimals
                    if ((WasNegative && IntValue >= 0)||(WasNegative == 0 && IntValue < 0))
                        DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
                }
            }
            else
            {
                bool WasNegative = IntValue < 0;
                //Deal with Int section first
                if (WasNegative)
                    IntValue = IntValue == MediumDecVariant::NegativeRep ? -1 : --IntValue;
                if(Value.IntValue!=0&&Value.IntValue!=MediumDecVariant::NegativeRep)
                    IntValue -= Value.IntValue;
                //Now deal with the decimal section
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
                if (IntValue == -1)
                    IntValue = DecimalHalf == 0 ? 0 : MediumDecVariant::NegativeRep;
                else if (IntValue < 0)
                    ++IntValue;
                //If flips to other side of negative, invert the decimals
                if ((WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0))
                    DecimalHalf = MediumDecVariant::DecimalOverflow - DecimalHalf;
            }
        }

private:
    void CatchAllSubtraction(MediumDecVariant& Value, RepType& LRep, RepType& RRep)
    {
        ConvertToNormType(LRep);
        Value.ConvertToNormType(RRep);
        BasicSubOp(Value);
    }
public:

        /// <summary>
        /// Subtraction Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& SubOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
            if (Value.DecimalHalf == InfinityRep)
            {
                Value.IntValue == 1 ? self.SetAsInfinity() : self.SetAsNegativeInfinity();
                return self;
            }
#endif
            RepType LRep = self.GetRepType();
            RepType RRep = Value.GetRepType();
            if(LRep==RRep)
            {
                if(self.ExtraRep==0||Value.ExtraRep==PIRep)
                {
#if AltDec_EnableMixedFractional
                    if(self.DecimalHalf<0)//MixedFractional
                    {
                    
                    }
                    else
                    {
#endif
                        self.BasicSubOp(Value);
#if AltDec_EnableMixedFractional
                    }
#endif
                }
    #if defined(AltDec_EnableImaginaryNum)
                else if(self.ExtraRep==IERep)
                {
                }
                else if(self.ExtraRep>0)
    #elif defined(AltDec_EnableENum)
                else if(self.ExtraRep==IERep)
                {
                }
                else if(self.ExtraRep>0)
    #else
                else//(Value/ExtraRep) Representation
    #endif
                {
                
                }
    #if defined(AltDec_EnableImaginaryNum) || defined(AltDec_EnableENum)
                else
                {
                
                }
    #endif
                if(self.ExtraRep!=0&&self.IntValue==0&&self.DecimalHalf==0)
                    self.ExtraRep = 0;
            }
            else
            {
                RepToRepSubOp(LRep, RRep, self, Value);
            }
            return self;
        }

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
#if defined(AltDec_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
#endif
            if(self.ExtraRep==PIRep)//Value*Pi Representation
            {
                self.ConvertToNumRep();
                self.PartialIntAddition(value);
            }
#if defined(AltDec_EnableImaginaryNum)
            else if(self.ExtraRep==IERep)
            {
                throw "Can't convert MediumDecVariant into complex number at moment";
            }
            else if(self.ExtraRep>0)
#elif defined(AltDec_EnableENum)
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
#if defined(AltDec_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
#endif
            if(self.ExtraRep==PIRep)//Value*Pi Representation
            {
                self.ConvertToNumRep();
                self.PartialIntSubtraction(value);
            }
#if defined(AltDec_EnableImaginaryNum)
            else if(self.ExtraRep==IERep)
            {
                throw "Can't convert MediumDecVariant into complex number at moment";
            }
            else if(self.ExtraRep>0)
#elif defined(AltDec_EnableENum)
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
                else//Don't need to check if flipping to other sign if adding positive to positive
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
#if defined(AltDec_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
#endif
            if(self.ExtraRep==PIRep)//Value*Pi Representation
            {
                self.ConvertToNumRep();
                self.PartialUnsignedAddition(value);
            }
#if defined(AltDec_EnableImaginaryNum)
            else if(self.ExtraRep==IERep)
            {
                throw "Can't convert MediumDecVariant into complex number at moment";
            }
            else if(self.ExtraRep>0)
#elif defined(AltDec_EnableENum)
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
#if defined(AltDec_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
#endif
            if(self.ExtraRep==PIRep)//Value*Pi Representation
            {
                self.ConvertToNumRep();
                self.PartialUnsignedSubtraction(value);
            }
#if defined(AltDec_EnableImaginaryNum)
            else if(self.ExtraRep==IERep)
            {
                throw "Can't convert MediumDecVariant into complex number at moment";
            }
            else if(self.ExtraRep>0)
#elif defined(AltDec_EnableENum)
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
        void PartialMultOp(MediumDecVariant& Value)
        {
            if (Value.IntValue < 0)
            {
                if (Value.IntValue == MediumDecVariant::NegativeRep) { Value.IntValue = 0; }
                else { Value.IntValue *= -1; }
                SwapNegativeStatus();
            }
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                    ExtraRep = Value.ExtraRep; return;
                }
                else if (Value.DecimalHalf == 0)
                {
                    IntValue *= Value.IntValue; return;
                }
                else
                {
                    //Value *= IntValue;
                    Value.PartialIntMultOp(IntValue);
                    IntValue = Value.IntValue; DecimalHalf = Value.DecimalHalf;
                    ExtraRep = Value.ExtraRep;
                }
            }
            else if (IntValue == 0)
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
                        IntValue = OverflowVal;
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
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
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
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
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? MediumDecVariant::NegativeRep : 0;
                        DecimalHalf = (signed int)SRep;
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
                    }
                    else
                    {
                        IntValue = 0;
                        DecimalHalf = (signed int)SRep;
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
        }

        /// <summary>
        /// Basic Multiplication Operation Between AltDecs
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        void BasicMultOp(MediumDecVariant& Value)
        {
            if (Value == MediumDecVariant::Zero) { SetAsZero(); return; }
            if ((IntValue==0&&DecimalHalf==0) || Value == MediumDecVariant::One)
                return;
            PartialMultOp(Value);
            if (IntValue==0&&DecimalHalf==0) { DecimalHalf = 1; }//Prevent Dividing into nothing
        }

private:
    void CatchAllMultiplication(MediumDecVariant& Value)
    {
        ConvertToNumRep();
        Value.ConvertToNumRep();
        BasicMultOp(Value);
    }
public:
        /// <summary>
        /// Multiplication Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& MultOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
            {
                if (Value.DecimalHalf == InfinityRep && self.IntValue == Value.IntValue && self.IntValue == -1)
                    self.IntValue = 1;
                return self;
            }
            else if (Value.DecimalHalf == InfinityRep)
                return self;
#endif
            if (Value == MediumDecVariant::Zero) { self.SetAsZero(); return self; }
            if (self == MediumDecVariant::Zero || Value == MediumDecVariant::One)
                return self;
            RepType LRep = self.GetRepType();
            RepType RRep = Value.GetRepType();
            if(LRep==RRep)
            {
                if(self.ExtraRep==0)
                {
#if AltDec_EnableMixedFractional
                    if(self.DecimalHalf<0)//MixedFractional
                    {
                    
                    }
                    else
                    {
#endif
                        self.PartialMultOp(Value);
#if AltDec_EnableMixedFractional
                    }
#endif
                }
                else if(self.ExtraRep==NegativeRep)//Value*Pi Representation
                {
                
                }
    #if defined(AltDec_EnableImaginaryNum)
                else if(self.ExtraRep==IERep)
                {
                }
                else if(self.ExtraRep>0)
    #elif defined(AltDec_EnableENum)
                else if(self.ExtraRep==IERep)
                {
                }
                else if(self.ExtraRep>0)
    #else
                else//(Value/ExtraRep) Representation
    #endif
                {
                
                }
    #if defined(AltDec_EnableImaginaryNum) || defined(AltDec_EnableENum)
                else
                {
                
                }
    #endif
            }

            if(self.ExtraRep!=0&&self.IntValue==0&&self.DecimalHalf==0)
                self.ExtraRep = 0;
            if (self == MediumDecVariant::Zero) { self.DecimalHalf = 1; }//Prevent Dividing into nothing
            return self;
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
            if (self.ExtraRep == 0)
            {
                self.PartialIntMultOp(Value);
            }
            else if (self.ExtraRep == NegativeRep)//Value*Pi Representation
            {
                self.PartialIntMultOp(Value);
            }
#if defined(AltDec_EnableImaginaryNum) || defined(AltDec_EnableENum)
            else if (self.ExtraRep == IERep)
            {
                if (Value == self.ExtraRep)
                    self.ExtraRep = -1;
                else
                    self.PartialIntMultOp(Value);
            }
            else if (self.ExtraRep > 0)
#else
            else//(Value/ExtraRep) Representation
#endif
            {
                if (Value == self.ExtraRep)
                    self.ExtraRep = 0;
                else
                    self.PartialIntMultOp(Value);
            }
            return self;
        }

        void PartialDivOp(MediumDecVariant& Value)
        {
            if (DecimalHalf == 0)
            {
                bool SelfIsNegative = IntValue < 0;
                if (SelfIsNegative)
                {
                    IntValue *= -1;
                }
                if (Value.DecimalHalf == 0)//Both are integers
                {
                    if (IntValue < 0) { IntValue *= -1; }
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
        }

        void BasicDivOp(MediumDecVariant& Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if (Value.DecimalHalf == InfinityRep)
            {
                SetAsZero();
                return;
            }
            if (Value == MediumDecVariant::Zero)
            {
                IntValue < 0 ? SetAsNegativeInfinity():SetAsInfinity();
                return;
            }
#else
            if (Value == MediumDecVariant::Zero)
                throw "Target value can not be divided by zero";
#endif
            if (IntValue==0&&DecimalHalf==0)
                return;
            if (Value.IntValue < 0)
            {
                if (Value.IntValue == MediumDecVariant::NegativeRep) { Value.IntValue = 0; }
                else { Value.IntValue *= -1; }
                SwapNegativeStatus();
            }
            PartialDivOp(Value);
            if (IntValue==0&&DecimalHalf==0) { DecimalHalf = 1; }//Prevent Dividing into nothing
        }

private:
    void CatchAllDivision(MediumDecVariant& Value)
    {
        ConvertToNumRep();
        Value.ConvertToNumRep();
        BasicDivOp(Value);
    }
public:
        /// <summary>
        /// Division Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& DivOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if (Value.DecimalHalf == InfinityRep)
            {
                self.SetAsZero(); return self;
            }
            if (Value == MediumDecVariant::Zero)
            {
                self.IntValue < 0 ? self.SetAsNegativeInfinity() : self.SetAsInfinity(); return self;
            }
#else
            if (Value == MediumDecVariant::Zero)
                throw "Target value can not be divided by zero";
#endif
            if (self == MediumDecVariant::Zero)
                return self;
            if (Value.IntValue < 0)
            {
                if (Value.IntValue == MediumDecVariant::NegativeRep) { Value.IntValue = 0; }
                else { Value.IntValue *= -1; }
                self.SwapNegativeStatus();
            }
            RepType LRep = self.GetRepType();
            RepType RRep = Value.GetRepType();
            if(LRep==RRep)
            {
                if(self.ExtraRep==0)
                {
#if AltDec_EnableMixedFractional
                    if(self.DecimalHalf<0)//MixedFractional
                    {
                    
                    }
                    else
                    {
#endif
                        self.PartialDivOp(Value);
#if AltDec_EnableMixedFractional
                    }
#endif
                }
                else if(self.ExtraRep==NegativeRep)//Value*Pi Representation
                {
                
                }
    #if defined(AltDec_EnableImaginaryNum)
                else if(self.ExtraRep==IERep)
                {
                }
                else if(self.ExtraRep>0)
    #elif defined(AltDec_EnableENum)
                else if(self.ExtraRep==IERep)
                {
                }
                else if(self.ExtraRep>0)
    #else
                else//(Value/ExtraRep) Representation
    #endif
                {
                
                }
    #if defined(AltDec_EnableImaginaryNum) || defined(AltDec_EnableENum)
                else
                {
                
                }
    #endif
            }
            if (self == MediumDecVariant::Zero) { self.DecimalHalf = 1; }//Prevent Dividing into nothing
            return self;
        }

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
#if defined(AltDec_EnableInfinityRep)
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
#if defined(AltDec_EnableInfinityRep)
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
#if defined(AltDec_EnableImaginaryNum) || defined(AltDec_EnableENum)
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
#if defined(AltDec_EnableImaginaryNum) || defined(AltDec_EnableENum)
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
        /// Remainder Operation Between AltDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& RemOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
            bool SelfIsWholeN = self.DecimalHalf == 0;
            bool ValueIsWholeN = Value.DecimalHalf == 0;
            if (Value.IntValue == 0 && ValueIsWholeN) { self.SetAsZero(); return self; }//Return zero instead of N/A
            RepType LRep = self.GetRepType();
            RepType RRep = Value.GetRepType();
            if(LRep==RRep)
            {
                if(self.ExtraRep==0)
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
                else if(self.ExtraRep==NegativeRep)//Value*Pi Representation
                {
                
                }
    #if defined(AltDec_EnableImaginaryNum)
                else if(self.ExtraRep==IERep)
                {
                }
                else if(self.ExtraRep>0)
    #elif defined(AltDec_EnableENum)
                else if(self.ExtraRep==IERep)
                {
                }
                else if(self.ExtraRep>0)
    #else
                else//(Value/ExtraRep) Representation
    #endif
                {
                
                }
    #if defined(AltDec_EnableImaginaryNum) || defined(AltDec_EnableENum)
                else
                {
                
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
            }
            return self;
        }
#pragma endregion Remainder Operations

#pragma region AltDec-To-MediumDecVariant Operators
    public:
        /// <summary>
        /// Addition Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator+(MediumDecVariant self, MediumDecVariant Value)
        {
            return AddOp(self, Value);
        }

        /// <summary>
        /// += Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator+=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return AddOp(self, Value);
        }
        
        /// <summary>
        /// += Operation Between AltDecs(from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator+=(MediumDecVariant* self, MediumDecVariant Value){ return AddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator-(MediumDecVariant self, MediumDecVariant Value)
        {
            return SubOp(self, Value);
        }

        /// <summary>
        /// -= Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator-=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return SubOp(self, Value);
        }
        
        /// <summary>
        /// -= Operation Between AltDecs(from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator-=(MediumDecVariant* self, MediumDecVariant Value){ return SubOp(**self, Value); }

        /// <summary>
        /// Multiplication Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator*(MediumDecVariant self, MediumDecVariant Value)
        {
            return MultOp(self, Value);
        }

        ///// <summary>
        ///// *= Operation Between AltDecs
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltDec</returns>
        friend MediumDecVariant& operator*=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return MultOp(self, Value);
        }

        ///// <summary>
        ///// *= Operation Between AltDecs (from pointer)
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>AltDec</returns>
        friend MediumDecVariant& operator*=(MediumDecVariant* self, MediumDecVariant Value){ return MultOp(**self, Value); }


        /// <summary>
        /// Division Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator/(MediumDecVariant self, MediumDecVariant Value)
        {
            return DivOp(self, Value);
        }

        /// <summary>
        /// /= Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator/=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return DivOp(self, Value);
        }
        
        /// <summary>
        /// /= Operation Between AltDecs (from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator/=(MediumDecVariant* self, MediumDecVariant Value){ return DivOp(**self, Value); }

        /// <summary>
        /// Remainder Operation Between AltDecs
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant operator%(MediumDecVariant self, MediumDecVariant Value)
        {
            return RemOp(self, Value);
        }

        /// <summary>
        /// %= Operation Between AltDecs
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator%=(MediumDecVariant& self, MediumDecVariant Value)
        {
            return RemOp(self, Value);
        }
        
        /// <summary>
        /// %= Operation Between AltDecs (from pointer)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        friend MediumDecVariant& operator%=(MediumDecVariant* self, MediumDecVariant Value)
        {
            return RemOp(**self, Value);
        }

        /// <summary>
        /// XOR Operation Between AltDecs
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
        /// Bitwise Or Operation Between AltDecs
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
        /// Bitwise And Operation Between AltDecs
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
            if (IntValue == NegativeRep) { IntValue = -1; }
            else if (DecimalHalf == 0) { --IntValue; }
            else if (IntValue == 0) { IntValue = NegativeRep; }
            else { --IntValue; }
            return *this;
        }

        /// <summary>
        /// AltDec++ Operator
        /// </summary>
        /// <returns>AltDec</returns>
        MediumDecVariant operator ++(int)
        {
            MediumDecVariant tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// AltDec-- Operator
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

    #pragma endregion AltDec-To-MediumDecVariant Operators

    #pragma region AltDec-To-Int Comparison Operators
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
        /// != Operation between <see cref="AltDec"/> and Integer Type.
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
        /// Lesser than Operation between <see cref="AltDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<(MediumDecVariant self, IntType Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==-1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PIRep)
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
        /// Lesser than or equal to operation between <see cref="AltDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<=(MediumDecVariant self, IntType Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==-1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PIRep)
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
        /// Greater than operation between <see cref="AltDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>(MediumDecVariant self, IntType Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PIRep)
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
        /// Greater than or equal to operation between <see cref="AltDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>=(MediumDecVariant self, IntType Value)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PIRep)
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
        /// Equality Operation between Integer Type and <see cref="AltDec"/>.
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
        /// != Operation between Integer Type and <see cref="AltDec"/>.
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
        /// Lesser than Operation between Integer Type and <see cref="AltDec"/> .
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<(IntType Value, MediumDecVariant self)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PIRep)
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
        /// Lesser than or equal to operation between Integer Type and <see cref="AltDec"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator<=(IntType Value, MediumDecVariant self)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return true;
                else
                    return false;
            }
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PIRep)
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
        /// Greater than operation between Integer Type and <see cref="AltDec"/>.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>(IntType Value, MediumDecVariant self)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return false;
                else
                    return true;
            }
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PIRep)
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
        /// Greater than or equal to operation between <see cref="AltDec"/> and Integer Type.
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>bool</returns>
        template<typename IntType>
        friend bool operator>=(IntType Value, MediumDecVariant self)
        {
#if defined(AltDec_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==1)
                    return false;
                else
                    return true;
            }
#endif
#if defined(AltDec_EnableImaginaryNum)
            if(self.ExtraRep>=0||self.ExtraRep==PIRep)
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

    #pragma endregion AltDec-To-Int Comparison Operators
    #pragma region Other Operations
        friend MediumDecVariant operator+(MediumDecVariant self, float Value) { return self + (AltDec)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, float Value) { return self - (AltDec)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, float Value) { return self * (AltDec)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, float Value) { return self / (AltDec)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, float Value) { return self % (AltDec)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, float Value) { return self ^ (AltDec)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, float Value) { return self | (AltDec)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, float Value) { return self & (AltDec)Value; }

        friend MediumDecVariant operator+(float Value, MediumDecVariant self) { return (AltDec)Value + self; }
        friend MediumDecVariant operator-(float Value, MediumDecVariant self) { return (AltDec)Value - self; }
        friend MediumDecVariant operator*(float Value, MediumDecVariant self) { return (AltDec)Value * self; }
        friend MediumDecVariant operator/(float Value, MediumDecVariant self) { return (AltDec)Value / self; }
        friend MediumDecVariant operator%(float Value, MediumDecVariant self) { return (AltDec)Value % self; }
        friend MediumDecVariant operator^(float Value, MediumDecVariant self) { return (AltDec)Value ^ self; }
        friend MediumDecVariant operator|(float Value, MediumDecVariant self) { return (AltDec)Value | self; }
        friend MediumDecVariant operator&(float Value, MediumDecVariant self) { return (AltDec)Value & self; }

        friend MediumDecVariant operator+(MediumDecVariant self, double Value) { return self + (AltDec)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, double Value) { return self - (AltDec)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, double Value) { return self * (AltDec)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, double Value) { return self / (AltDec)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, double Value) { return self % (AltDec)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, double Value) { return self ^ (AltDec)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, double Value) { return self | (AltDec)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, double Value) { return self & (AltDec)Value; }

        friend MediumDecVariant operator+(double Value, MediumDecVariant self) { return (AltDec)Value + self; }
        friend MediumDecVariant operator-(double Value, MediumDecVariant self) { return (AltDec)Value - self; }
        friend MediumDecVariant operator*(double Value, MediumDecVariant self) { return (AltDec)Value * self; }
        friend MediumDecVariant operator/(double Value, MediumDecVariant self) { return (AltDec)Value / self; }
        friend MediumDecVariant operator%(double Value, MediumDecVariant self) { return (AltDec)Value % self; }
        friend MediumDecVariant operator^(double Value, MediumDecVariant self) { return (AltDec)Value ^ self; }
        friend MediumDecVariant operator|(double Value, MediumDecVariant self) { return (AltDec)Value | self; }
        friend MediumDecVariant operator&(double Value, MediumDecVariant self) { return (AltDec)Value & self; }

        friend MediumDecVariant operator+(MediumDecVariant self, ldouble Value) { return self + (AltDec)Value; }
        friend MediumDecVariant operator-(MediumDecVariant self, ldouble Value) { return self - (AltDec)Value; }
        friend MediumDecVariant operator*(MediumDecVariant self, ldouble Value) { return self * (AltDec)Value; }
        friend MediumDecVariant operator/(MediumDecVariant self, ldouble Value) { return self / (AltDec)Value; }
        friend MediumDecVariant operator%(MediumDecVariant self, ldouble Value) { return self % (AltDec)Value; }
        friend MediumDecVariant operator^(MediumDecVariant self, ldouble Value) { return self ^ (AltDec)Value; }
        friend MediumDecVariant operator|(MediumDecVariant self, ldouble Value) { return self | (AltDec)Value; }
        friend MediumDecVariant operator&(MediumDecVariant self, ldouble Value) { return self & (AltDec)Value; }

        friend MediumDecVariant operator+(ldouble Value, MediumDecVariant self) { return (AltDec)Value + self; }
        friend MediumDecVariant operator-(ldouble Value, MediumDecVariant self) { return (AltDec)Value - self; }
        friend MediumDecVariant operator*(ldouble Value, MediumDecVariant self) { return (AltDec)Value * self; }
        friend MediumDecVariant operator/(ldouble Value, MediumDecVariant self) { return (AltDec)Value / self; }
        friend MediumDecVariant operator%(ldouble Value, MediumDecVariant self) { return (AltDec)Value % self; }
        friend MediumDecVariant operator^(ldouble Value, MediumDecVariant self) { return (AltDec)Value ^ self; }
        friend MediumDecVariant operator|(ldouble Value, MediumDecVariant self) { return (AltDec)Value | self; }
        friend MediumDecVariant operator&(ldouble Value, MediumDecVariant self) { return (AltDec)Value & self; }

    #pragma endregion Other Operations
    #pragma region AltDec-To-Integer Operations
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
    #pragma endregion AltDec-To-Integer Operations
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
            return (AltDec)Value ^ self;
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
            return (AltDec)Value | self;
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
            return (AltDec)Value & self;
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
            case RepType::PINum:
                ConvertPIToNum();
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
            case RepType::PINum:
                ConvertPIToNum();
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
            case RepType::PINum:
                Value.ConvertPIToNum();
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
            case RepType::PINum:
            {
                Value.ConvertPIToNum();//return CeilInt(Value.ConvertPIToNum());
                break;
            }
#if defined(AltDec_EnableENum)
            case RepType::ENum:
#if defined(AltDec_EnableByDecimaledFractionals)
            case RepType::ENumByDiv:
#endif
                Value.ConvertToNumRep();
                break;
#endif
#if defined(AltDec_EnableImaginaryNum)
            case RepType::INum:
#if defined(AltDec_EnableByDecimaledFractionals)
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
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs the nearest integer).
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
#if defined(AltDec_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            else if (DecimalHalf == ApproachingValRep)
            {
                DecimalHalf = 0; ExtraRep = 0;
            }
            else
            {
#endif
                ConvertToNumRep();
                DecimalHalf = 0;
                if (IntValue == NegativeRep) { IntValue = 0; }
#if defined(AltDec_EnableInfinityRep)
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
/*#ifdef AltDec_EnableInfinityRep
            if (DecimalHalf == InfinityRep)
            {
#ifdef AltDec_EnableInfinityPowers//Not Reaaly needed to treat
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
                    return AltDec(0, MediumDecVariant::DecimalOverflow / VariableConversionFunctions::PowerOfTens[expValue * -1]);
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
                return AltDec(VariableConversionFunctions::PowerOfTens[expValue], 0);
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
        static MediumDecVariant Sqrt(MediumDecVariant value, int precision=7)
        {
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
            MediumDecVariant delX = AltDec(2147483647, 0);

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
                case RepType::PINum:
                    //value.ConvertPIToNum();
                    break;
#if defined(AltDec_EnableInfinityRep)
                //case RepType::ApproachingBottom:
                //break;
                //case RepType::ApproachingTop:
                //break;
                case InfinityRep:
                    return expType.IsZero()?AltDec.One:value;
                break;
#endif
#if defined(AltDec_EnableInfinityRep)

#endif
#if defined(AltDec_EnableNaN)
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
#if defined(AltDec_EnableInfinityRep)
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
#if defined(AltDec_EnableUndefinedButInRange)
                             throw "Returns noncoded representation infinity based value";//Return value here later
#elif defined(AltDec_EnableNaN)
                             return NaN;
#else
                             throw "Returns value in undefined range with current preprocessor settings.";
#endif
                            
                        else
                            return Infinity;//PositiveInfinity
                break;
#endif
#if defined(AltDec_EnableByDecimaledFractionals)
                case RepType::NumByDiv:
                    return FractionalPow(value, AltDec(expValue.IntValue, expValue.DecimalHalf), expValue.ExtraRep);
#if defined(AltDec_EnableENum)
                case RepType::ENumByDiv:
                    return FractionalPow(value, value.ExtraRep*-1);
                    break;
#endif
                case RepType::INumByDiv:
                    return FractionalPow(value, value.ExtraRep*-1);
                    break;
#endif
    //            case RepType::PINum:
    //                expValue.ConvertPIToNum();
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
        /// <returns>BlazesRusCode::AltDec</returns>
        static MediumDecVariant LnRef(MediumDecVariant& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            bool WithinThresholdRange = false;
            switch(DecimalHalf)
            {
#if defined(AltDec_EnableInfinityRep)
            case InfinityRep:
                return value;
                break;
#endif
#if defined(AltDec_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
            case ApproachingValRep:
                WithinThresholdRange = true;
                break;
            default:
                if(value.ExtraRep==PIRep)
                    ConvertPIToNum();
#if defined(AltDec_EnableENum)
                else if(value.ExtraRep<0)
                    ConvertEToNum();
#endif
#if defined(AltDec_EnableByDecimaledFractionals)
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
        /// <returns>BlazesRusCode::AltDec</returns>
        static MediumDecVariant LnRefV2(MediumDecVariant& value)
        {
            //if (value <= 0) {}else//Error if equal or less than 0
            if (value == MediumDecVariant::One)
                return MediumDecVariant::Zero;
            if(value.ExtraRep!=0)
                return LnRef_Part02(value);
            switch(DecimalHalf)
            {
#if defined(AltDec_EnableInfinityRep)
            case InfinityRep:
                return value;
                break;
#endif
#if defined(AltDec_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
            //case ApproachingValRep:
            //    break;
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
#if defined(AltDec_EnableInfinityRep)
            case InfinityRep:
                return value;
                break;
#endif
#if defined(AltDec_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
            case ApproachingValRep:
                WithinThresholdRange = true;
                break;
            default:
                if(value.ExtraRep==PIRep)
                    ConvertPIToNum();
#if defined(AltDec_EnableENum)
                else if(value.ExtraRep<0)
                    ConvertEToNum();
#endif
#if defined(AltDec_EnableByDecimaledFractionals)
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
                        return AltDec(index, 0);
                }
                return AltDec(9, 0);
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
        MediumDecVariant TotalRes = AltDec((value - 1), 0) / AltDec((value + 1), 0);
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
#if defined(AltDec_EnableInfinityRep)
            case InfinityRep:
                return value;
                break;
#endif
#if defined(AltDec_EnableNaN)
            case NaNRep:
                return value;
                break;
#endif
            case ApproachingValRep:
                WithinThresholdRange = true;
                break;
            default:
                if(value.ExtraRep==PIRep)
                    ConvertPIToNum();
#if defined(AltDec_EnableENum)
                else if(value.ExtraRep<0)
                    ConvertEToNum();
#endif
#if defined(AltDec_EnableByDecimaledFractionals)
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
                        baseTotalRes = AltDec(index, 0);
                        break;
                    }
                }
                baseTotalRes = AltDec(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = AltDec((baseVal - 1), 0) / AltDec((baseVal + 1), 0);
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
                        return lnMultLog ? AltDec(index, 0) / (baseTotalRes * MediumDecVariant::HalfLN10Mult): AltDec(index, 0)/ baseTotalRes;
                }
                return lnMultLog? AltDec(9, 0) / (baseTotalRes*MediumDecVariant::HalfLN10Mult):AltDec(9, 0)/baseTotalRes;
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
#if defined(AltDec_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
#if defined(AltDec_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
#elif defined(AltDec_EnableUndefinedButInRange)
//Return undefined value between -1 and 1
#else
                throw "Operation results in NaN";
#endif
#endif
            RepType repType = Value.GetRepType();
            switch (repType)
            {
            case RepType::PINum:
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
                MediumDecVariant Radius = PI * Value / 180;
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
#if defined(AltDec_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
#if defined(AltDec_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
#elif defined(AltDec_EnableUndefinedButInRange)
//Return undefined value between -1 and 1
#else
                throw "Operation results in NaN";
#endif
#endif
            RepType repType = Value.GetRepType();
            switch (repType)
            {
            case RepType::PINum:
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
                MediumDecVariant Radius = PI * Value / 180;
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
            if (Value.ExtraRep == PIRep)//0 to 2PI range (2PI == 0PI)
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
                if (Value.DecimalHalf == 0&&(Value.IntValue==0||Value.IntValue==1))//0 and PI
                    return Zero;
                if (Value.DecimalHalf == 500000000)//0.5 PI = 1; 1.5PI = -1
                    return Value.IntValue==0?NegativeOne:One;
                MediumDecVariant SinValue = Zero;
                for (int i = 0; i < 7; ++i)
                {
                    SinValue += (i % 2 == 0 ? 1 : -1) * MediumDecVariant::Pow(Value, 2 * i + 1) / VariableConversionFunctions::Fact(2 * i + 1);
                }
                return SinValue;

            }
#if defined(AltDec_EnableInfinityRep)
            else if(Value.DecimalHalf==InfinityRep)
#if defined(AltDec_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
#elif defined(AltDec_EnableUndefinedButInRange)
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
            if (Value.ExtraRep == PIRep)//0 to 2PI range (2PI == 0PI)
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
                if (Value.DecimalHalf == 0)//cos(0) = 1;cos(PI) = -1;
                    return Value.IntValue == 0?One:NegativeOne;
                if (Value.DecimalHalf == 500000000)//cos(0.5) PI = 0; cos(1.5PI) = 0;
                    return Zero;
            }
#if defined(AltDec_EnableInfinityRep)
            else if(Value.DecimalHalf==InfinityRep)
#if defined(AltDec_EnableNaN)
                return NaNValue();//https://byjus.com/questions/what-is-the-value-of-sin-and-cos-infinity/
#elif defined(AltDec_EnableUndefinedButInRange)
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
#if defined(AltDec_EnableNaN)
            case RepType::NaN:
                return NaNValue();
#endif
#if defined(AltDec_EnableImaginaryNum) && defined(AltDec_EnableByDecimaledFractionals)
            {
                int Divisor = Value.ExtraRep*-1;
                Value.ExtraRep = 0;
                Value /= Divisor;
                Value.ExtraRep = -2147483647;
                break;
            }
#endif
#if defined(AltDec_EnablePIRep)
            case RepType::RepType::PINum:
#endif
#if defined(AltDec_EnableENum)
            case RepType::RepType::ENum:
#if defined(AltDec_EnableByDecimaledFractionals)
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
#if defined(AltDec_EnableInfinityRep)
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
#if defined(AltDec_EnableInfinityRep)
                return MediumDecVariant::NegativeInfinity;
#else
                return MediumDecVariant::Minimum;//Negative Infinity
#endif
            }
            else
            {
                return Tan(PI * Value / 180);
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
#if defined(AltDec_EnableNaN)
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
#if defined(AltDec_EnablePIRep)
            MediumDecVariant coeff_1 = AltDec(0, 250000000, PIRep);//PI / 4;
#else
            MediumDecVariant coeff_1 = PI / 4;
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
#if defined(AltDec_EnableInfinityRep)
    MediumDecVariant MediumDecVariant::AlmostOne = ApproachingRightRealValue();
#endif
    MediumDecVariant MediumDecVariant::PI = PIValue();
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
    MediumDecVariant MediumDecVariant::Nil = NilValue();
    MediumDecVariant MediumDecVariant::PINum = PINumValue();
    MediumDecVariant MediumDecVariant::ENum = ENumValue();
    
#if defined(AltDec_EnableInfinityRep)
    MediumDecVariant MediumDecVariant::Infinity = InfinityValue();
    MediumDecVariant MediumDecVariant::NegativeInfinity = NegativeInfinityValue();
    MediumDecVariant MediumDecVariant::ApproachingZero = ApproachingZeroValue();
#endif
#if defined(AltDec_EnableNaN)
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
                IntValue += TempInt02;
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
    inline MediumDecVariant MediumDecVariant::GetValueFromString(std::string Value)
    {
        MediumDecVariant NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

    std::string MediumDecVariant::ToString()
    {
#if defined(AltDec_EnableInfinityRep)
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
#ifdef AltDec_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.99999999999999999999" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".99999999999999999999";
#else
            return IntValue == NegativeRep ? "-0.9___9" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".9___9";
#endif
            break;
        case RepType::ApproachingBottom:
#ifdef AltDec_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.00000000000000000001" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".00000000000000000001";
#else
            return IntValue == NegativeRep ? "-0.0___1" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".0___1";
#endif
            break;
        case RepType::ApproachingBottom:
#ifdef AltDec_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.00000000000000000001" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".00000000000000000001";
#else
            return IntValue == NegativeRep ? "-0.0___1" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".0___1";
#endif
            break;
/*
#if defined(AltDec_EnableNaN)
        case RepType::NaN:
            return "NaN";
#endif
#if defined(AltDec_EnableENum)
        case RepType::ENum:
#if defined(AltDec_EnableByDecimaledFractionals)
        case RepType::ENumByDiv:
#endif
            break;
#endif
#if defined(AltDec_EnableImaginaryNum)
        case RepType::INum:
#if defined(AltDec_EnableByDecimaledFractionals)
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
#if defined(AltDec_EnablePIRep)
        case RepType::PINum:
            Value += "Pi";
            break;
#if defined(AltDec_EnableENum)
        case RepType::ENum:
            Value += "e";
            break;
#if defined(AltDec_EnableByDecimaledFractionals)
        case RepType::ENumByDiv:
            Value += "e/";
            Value += ExtraRep*-1;
            break;
#endif
#endif
#if defined(AltDec_EnableImaginaryNum)
        case RepType::INum:
            Value += "i";
            break;
#if defined(AltDec_EnableByDecimaledFractionals)
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
#if defined(AltDec_EnableInfinityRep)
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
#ifdef AltDec_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.99999999999999999999" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".99999999999999999999";
#else
            return IntValue == NegativeRep ? "-0.9___9" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".9___9";
#endif
            break;
        case RepType::ApproachingBottom:
#ifdef AltDec_DisplayApproachingAsReal
            return IntValue == NegativeRep ? "-0.00000000000000000001" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".00000000000000000001";
#else
            return IntValue == NegativeRep ? "-0.0___1" : VariableConversionFunctions::IntToStringConversion(IntValue) + ".0___1";
#endif
            break;
/*
#if defined(AltDec_EnableENum)
        case RepType::ENum:
#if defined(AltDec_EnableByDecimaledFractionals)
        case RepType::ENumByDiv:
#endif
            break;
#endif
#if defined(AltDec_EnableImaginaryNum)
        case RepType::INum:
#if defined(AltDec_EnableByDecimaledFractionals)
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
#if defined(AltDec_EnablePIRep)
        case RepType::PINum:
            Value += "Pi";
            break;
#if defined(AltDec_EnableENum)
        case RepType::ENum:
            Value += "e";
            break;
#if defined(AltDec_EnableByDecimaledFractionals)
        case RepType::ENumByDiv:
            Value += "e/";
            Value += ExtraRep*-1;
            break;
#endif
#endif
#if defined(AltDec_EnableImaginaryNum)
        case RepType::INum:
            Value += "i";
            break;
#if defined(AltDec_EnableByDecimaledFractionals)
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
    /// (AltDec Version)Performs remainder operation then saves division result
    /// C = A - B * (A / B)
    /// </summary>
    class DLL_API AltModChecker : public AltNumModChecker<AltDec>
    {
    };
}