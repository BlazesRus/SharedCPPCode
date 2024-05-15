// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//Preprocessor options
/*
AltNum_EnableFractionals =
	Enables fractional representations in attempt to preserve more accuracy during operations
	(Enables NumByDiv and other variants of Fractionals if AltNum_EnableAlternativeRepFractionals is enabled at same time)
	Can't be used at same time as AltNum_EnableNegativePowerRep
AltNum_EnableDecimaledAlternativeFractionals =
	Automatically toggled if Pi, e, or i features enabled and AltNum_EnableFractionals enabled
	(Automatically enabled if AltNum_EnableDecimaledPiFractionals, AltNum_EnableDecimaledEFractionals, or AltNum_EnableDecimaledEFractionals enabled)
AltNum_EnableNegativePowerRep = 
	Allows negative powers when AltNum_EnablePowerOfRepresentation is enabled

//--Infinity based preprocessors--
AltNum_EnableApproaching
	Depreciated Defined Values:
      When DecimalHalf is -2147483647 and ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0..1)
	  When DecimalHalf is -2147483647 and ExtraRep==1, it represents Approaching IntValue+1 from left towards right (IntValue.9..9)
AltNum_EnableApproachingDivided =
	Enables Approaching IntValue.49..9 and IntValue.50..1 and other Approaching values (49..9 = ExtraRep value of 2; 50..1 = ExtraRep value of -2)
	When ExtraRep is Positive, represents (when IntValue is positive) IntValue + (1/ExtraRep + ApproachingBottomValue)(approaching left towards right)
	When ExtraRep is Positive, represents (when IntValue is positive) IntValue + (1/ExtraRep + ApproachingTopValue)(approaching right towards left)
	(Assumes AltNum_EnableInfinityRep is enabled)
	(Partially Implimented)

AltNum_EnableInfinityPowers =
Allows powers of infinity for operations where infinity is somehow more infinite than normal
      (Not Implemented)

AltNum_DisplayApproachingAsReal =
      Display approaching value as real number with 20 digits in decimal section
//----

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
      AltNum_EnablePiRep given greater priority if both both AltNum_DisablePiRep and AltNum_EnablePiRep
      are set by preprocessor settings of project

AltNum_EnablePiRep =
	Depreciated Defined Values(Used if AltNum_UseIntForDecimalHalf is enabled):
      If AltNum_UseAltDecBasedRepresentations enabled, then
      Pi*(+- 2147483647.999999999) Representation enabled
        (When DecimalHalf is between -1 and -1000000000 (when DecimalHalf is -1000000000 is Equal to IntValue*Pi))
      Otherwise represents Pi within format of
         If DecimalHalf is positive and ExtraRep is -2147483647,
         then AltDec represents +- 2147483647.999999999 * Pi (Not Fully Implemented)
         If DecimalHalf is positive and and ExtraRep is between AlternativeFractionalLowerBound and 0,
         then AltDec represents (+- 2147483647.999999999 * Pi)/(ExtraRep*-1)
      (Not Fully Implemented--Enabled by default if AltNum_DisablePiRep not set)

AltNum_EnableComplexNum =
      Enable Representation of complex numbers with Imaginary number operations
	  (Requires ExtraRep.IsNegative not to be used by other values
If ExtraRep.IsNegative == 1, then represents value as IntValue.DecimalHalf + (ExtraRep.Value i)
If AltNum_EnableFractionals is disabled and ExtraRep.IsNegative == 0, then represents value as IntValue + (DecimalHalf.(ExtraRep.Value) i)
(Requires AltNum_EnableIRep, Not Implemented)

AltNum_EnableERep =
	Depreciated Defined Values(Used if AltNum_UseIntForDecimalHalf is enabled):
      If AltNum_UseAltDecBasedRepresentations enabled, then
    e*(+- 2147483647.999999999) Representation enabled
    (When DecimalHalf is between -1000000001 and -2000000000 (when DecimalHalf is -2000000000 is Equal to IntValue*e))
      Otherwise represents e within format of
    If DecimalHalf is positive and ExtraRep is -2147483646, then
       represents +- 2147483647.999999999 * e
    If DecimalHalf is positive and ExtraRep is between AlternativeFractionalLowerBound and 0, then
        represents (+- 2147483647.999999999 * e)/(ExtraRep*-1)
        (Not Fully Implemented)

AltNum_EnableIRep =
	Depreciated Defined Values(Used if AltNum_UseIntForDecimalHalf is enabled):
      If DecimalHalf is positive and ExtraRep is -2147483647, then
      represents +- 2147483647.999999999i
      If DecimalHalf is positive and ExtraRep is between AlternativeFractionalLowerBound and 0, then
      represents (+- 2147483647.999999999i)/(ExtraRep*-1)
      (Not Fully Implemented--Giving lesser priority to finish unless I need to use)

//----
AltNum_EnablePrivateRepType =
      Sets GetRepType code to be private instead of public

AltNum_EnableAutoToggleOfPreferredSettings =
      Force enables AltNum_EnablePiRep, AltNum_EnableInfinityRep,
      AltNum_EnableAlternativeRepFractionals & AltNum_EnableDecimaledPiFractionals

AltNum_DisableSwitchBasedConversion =

AltNum_EnableAltDecBasedSetValues =
   

---Only these switches can be enabled at once(when ExtraRep is Negative):
AltNum_EnablePowerOfRepresentation =
      Represents IntValue.DecimalHalf^(ExtraRep.Value)
	  If AltNum_EnableNegativePowerRep is enabled, then ExtraRep is Negative is used for negative powers instead
      (Not Implemented)
AltNum_EnableMixedFractional =
      Represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf.Value)/ExtraRep.Value
      (Not Fully Implemented)

----===============================================================================================================
AltNum_EnablePiPowers =
	Depreciated Defined Values(Used if AltNum_UseIntForDecimalHalf is enabled):
      If ExtraRep value is between 1 and 2147483640, then represents IntValue.DecimalHalf * Pi^(ExtraRep.Value)
      Can't be enabled at same time as AltNum_EnableDecimaledAlternativeFractionals
	If AltNum_UseIntForDecimalHalf is not enabled, then this is autotoggled if AltNum_EnablePowerOfRepresentation is toggled
      (Not Fully Implemented)

AltNum_OutputTruncatedTrailingDigits =
    Output to console trailing digits that are truncated when multiplication or division results in numbers getting too small(Not Implemented yet)
    (Implement this before work to making working version with trailing digits such as for MixedDec (fixed point combined with floating point implementations of decimal-like format classes)

AltNum_UseOldDivisionCode = Deprecated (Working on removing all traces)
AltNum_AvoidUsingLargeInt = Removes AltNum_UseOldDivisionCode toggle and forces alternative code that doesn't need int128 from boost
AltNum_UseOldRemOpCode

AltNum_EnableBoostFractionalReduction
AltNum_EnableImaginaryInfinity = Enables imaginary infinity option
AltNum_DisableApproachingTop = Disables IntValue.9..9 representation and .50..1 etc

Auto toggles AltNum_EnableAlternativeMixedFrac if any of 3 above are toggled
Auto toggles AltNum_MixedPiOrEEnabled if AltNum_EnableMixedPiFractional or AltNum_EnableMixedEFractional are active

AltNum_EnableNil = Enables Nil representation(detection not in code right now)


AltNum_EnableUndefinedButInRange = Enable representation of unknown number between -Value and +Value for Cos of infinity etc
AltNum_EnableWithinMinMaxRange
AltNum_EnableUnknownTrigExpressions = (Not Implemented)

AltNum_PreventModulusOverride = Turns off modulus overrides if toggled
AltNum_EnableAlternativeModulusResult = Add addition modulus operations that give AltNumModChecker<AltNum> result
AltNum_EnableBitwiseOverride = Enables bitwise operation overrides if enabled

AltNum_UseDeveloperExtraDefaults = Auto toggles extra settings to more fully test feature sets (but planning to use all these extra toggles for most projects that plan to use AltNum with)
AltDec_SeekRepTypeViaBitwise = Alternative code that checks enum case blocks with bit checks instead of case statements(Not Implemented yet)

AltNum_DisableMultiplyDownToNothingPrevention = Disables preventing multiplication operations from multiplying into nothing as result of getting too small

AltNum_UsePositiveInfinityRep
AltNum_AllowOverflowRep = Allow representation of numbers greator than 2147483647.999999999 and smaller than -2147483647.999999999
	If AltNum_EnableFractionals is disabled, represents numbers 2147483647.999999999 x 10^ExtraRep with the smallest digits truncated for each value above normal limit
	If AltNum_EnableFractionals is enabled and negative ExtraRep values are not used for any representation, represents numbers 2147483647.999999999 x 10^-ExtraRep with similar above truncation
	(Not Implimented--Depreciated)
*/

#pragma region PreprocessorToggles

#if defined(AltNum_EnableFractionals)&&define(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
	#define AltNum_EnableDecimaledPiFractionals
#endif

#if defined(AltNum_EnableFractionals)&&define(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
	#define AltNum_EnableDecimaledEFractionals
#endif

#if defined(AltNum_EnableFractionals)&&define(AltNum_EnableIRep)&&!defined(AltNum_EnableDecimaledIFractionals)
	#define AltNum_EnableDecimaledIFractionals
#endif

#if defined(AltNum_EnablePowerOfRepresentation)&&define(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
	#define AltNum_EnablePiPowers
#endif

#if defined(AltNum_EnablePowerOfRepresentation)&&define(AltNum_EnableERep)&&!defined(AltNum_EnableEPowers)
	#define AltNum_EnableEPowers
#endif

#if defined(AltNum_EnableApproachingPi)&&defined(AltNum_EnableApproachingDivided)&&!defined(AltNum_EnableApproachingPiDivided)
	#define AltNum_EnableApproachingPiDivided
#endif

#if defined(AltNum_EnableApproachingE)&&defined(AltNum_EnableApproachingDivided)&&!defined(AltNum_EnableApproachingEDivided)
	#define AltNum_EnableApproachingEDivided
#endif

#if defined(AltNum_EnableApproachingI)&&defined(AltNum_EnableApproachingDivided)&&!defined(AltNum_EnableApproachingIDivided)
	#define AltNum_EnableApproachingIDivided
#endif

#pragma endregion PreprocessorToggles

#if !defined(AltNum_DisableAltDecBaseDefaultToggles)
    #define AltNum_EnablePiRep
    #define AltNum_EnableInfinityRep
    #define AltNum_EnableAlternativeRepFractionals
    #define AltNum_EnableDecimaledPiFractionals
    #define AltNum_EnableApproaching
#define AltNum_UseDeveloperExtraDefaults//Turns on extra defaults just for testing
    //#define AltNum_EnableERep
#endif
