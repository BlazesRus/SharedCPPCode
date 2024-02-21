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
      AltNum_EnableAlternativeRepFractionals =
      Enables integer based fractionals as long as Decimaled version isn't active
      (Enables PiFractional, EFractional, and IFractional depending on other preprocessor flags)

//--Infinity based preprocessors--
AltNum_EnableInfinityRep = Enable support of positive/negative infinity representations and approaching value representations
      When DecimalHalf is -2147483648, it represents negative infinity(if IntValue is -1) or positive infinity(if IntValue is 1)
AltNum_EnableApproachingValues
      When DecimalHalf is -2147483647 and ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0..1)
	  When DecimalHalf is -2147483647 and ExtraRep==1, it represents Approaching IntValue+1 from left towards right (IntValue.9..9)
AltNum_EnableApproachingPi = AltNum_EnableApproachingMidDec for Pi based variables(Partially Implimented)
AltNum_EnableApproachingE = AltNum_EnableApproachingMidDec for e based variables(Partially Implimented)
AltNum_EnableApproachingI = AltNum_EnableApproachingMidDec for imaginary based variables(Partially Implimented)
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

AltNum_UseAltDecBasedRepresentations =
      Forces to calculate certain representations like AltDec does 
      (preference for storing non-normal representations within value of negative DecimalHalf)
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
If AltNum_EnableFractionals not enabled, store value as IntValue.DecimalHalf + ExtraRePi
Otherwise requires AltNum_EnableBasicComplexNumber and ExtraRep value as ?, and stores value as IntValue + DecimalHalfi
(Might be better to just store as formula class feature or as another number class holding 2 AltDec or other AltNum values)
      (Requires AltNum_EnableImaginaryNum, Not Implemented)

AltNum_EnableMixedFractional =
      If DecimalHalf is negative and ExtraRep is Positive,
      then AltDec represents mixed fraction of -2147483648 to 2147483647 + (DecimalHalf*-1)/ExtraRep
      (Not Fully Implemented)

      AltNum_EnableERep =
      If AltNum_UseAltDecBasedRepresentations enabled, then
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

//----
AltNum_EnablePrivateRepType =
      Sets GetRepType code to be private instead of public

AltNum_EnableAutoToggleOfPreferredSettings =
      Force enables AltNum_EnablePiRep, AltNum_EnableInfinityRep,
      AltNum_EnableAlternativeRepFractionals & AltNum_EnableDecimaledPiFractionals

AltNum_DisableSwitchBasedConversion =

AltNum_EnableAltDecBasedSetValues =
      
AltNum_EnableDecimaledAlternativeFractionals = 
   Automatically enabled if AltNum_EnableDecimaledPiFractionals, AltNum_EnableDecimaledEFractionals, or AltNum_EnableDecimaledEFractionals enabled
   Not to be confused with AltNum_EnableAlternativeRepFractionals(which only enabled Integer based alternative rep fractionals)

---Only one of the next set of 5 switches can be enabled at once:
AltNum_EnableDecimaledPiFractionals = Enables fractionals for Pi with non-integer numbers(not Implemented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound
AltNum_EnableDecimaledEFractionals = Enables fractionals for e with non-integer numbers(not Implemented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound
AltNum_EnableDecimaledIFractionals = Enables fractionals for i with non-integer numbers(not Implemented yet) when ExtraRep is between 0 and AlternativeFractionalLowerBound

AltNum_EnablePiPowers =
      If ExtraRep value is between -1 and -2147483640, then represents IntValue.DecimalHalf * Pi^(ExtraRep*-1)
      Can't be enabled at same time as AltNum_EnableDecimaledAlternativeFractionals
      (Not Fully Implemented)
AltNum_EnableNormalPowers =
      If ExtraRep value is between -1 and -2147483640, then represents IntValue.DecimalHalf^(ExtraRep*-1)
      Can't be enabled at same time as AltNum_EnableDecimaledAlternativeFractionals or AltNum_EnablePiPowers
      (Not Implemented)
----===============================================================================================================

AltNum_OutputTruncatedTrailingDigits =
    Output to console trailing digits that are truncated when multiplication or division results in numbers getting too small(Not Implemented yet)
    (Implement this before work to making working version with trailing digits such as for MixedDec (fixed point combined with floating point implementations of decimal-like format classes)

AltNum_UseOldDivisionCode = Deprecated (Working on removing all traces)
AltNum_AvoidUsingLargeInt = Removes AltNum_UseOldDivisionCode toggle and forces alternative code that doesn't need int128 from boost
AltNum_UseOldRemOpCode

AltNum_EnableBoostFractionalReduction
AltNum_EnableImaginaryInfinity = Enables imaginary infinity option
AltNum_DisableApproachingTop = Disables IntValue.9..9 representation and .5..1 etc

Only one of 3 alternative mixed fraction representations can be enabled at a time(use FlaggedDec variant if need all at once):
AltNum_EnableMixedPiFractional
AltNum_EnableMixedEFractional
AltNum_EnableMixedIFractional
Auto toggles AltNum_EnableAlternativeMixedFrac if any of 3 above are toggled
Auto toggles AltNum_MixedPiOrEEnabled if AltNum_EnableMixedPiFractional or AltNum_EnableMixedEFractional are active

AltNum_EnableNilRep = Enables Nil representation(detection not in code right now)


AltNum_EnableUndefinedButInRange = Enable representation of unknown number between -Value and +Value for Cos of infinity etc
AltNum_EnableWithinMinMaxRange
AltNum_EnableUnknownTrigExpressions = (Not Implemented)

AltNum_PreventModulusOverride = Turns off modulus overrides if toggled
AltNum_EnableAlternativeModulusResult = Add addition modulus operations that give AltNumModChecker<AltNum> result
AltNum_EnableBitwiseOverride = Enables bitwise operation overrides if enabled

AltNum_EnablePiFractional = Auto toggled if AltNum_EnableAlternativeRepFractionals and AltNum_EnablePiRep enabled without AltNum_EnableDecimaledPiFractionals toggled
AltNum_EnableEFractional = Auto toggled if AltNum_EnableAlternativeRepFractionals and AltNum_EnableERep enabled without AltNum_EnableDecimaledEFractionals toggled
AltNum_EnableIFractional = Auto toggled if AltNum_EnableAlternativeRepFractionals and AltNum_EnableImaginaryNum enabled without AltNum_EnableDecimaledIFractionals toggled
AltNum_UsingAltFractional = Auto toggled if any of the above 3 are toggled

AltDec_UseMirroredInt
AltNum_UseDeveloperExtraDefaults = Auto toggles extra settings to more fully test feature sets (but planning to use all these extra toggles for most projects that plan to use AltNum with)
AltDec_SeekRepTypeViaBitwise = Alternative code that checks enum case blocks with bit checks instead of case statements(Not Implemented yet)

AltNum_DisableMultiplyDownToNothingPrevention = Disables preventing multiplication operations from multiplying into nothing as result of getting too small

AltNum_StoreCommonFunctionsInBase
AltNum_DisableCommonVariablesInBase = Use to disable usage of the base class and to disable defining common AltNum variables in base class
AltNum_StoreIntSectionFunctionsInBase
AltNum_StoreBasicFunctionsInBase
AltNum_StoreConstVariablesInBase

AltNum_UsePositiveInfinityRep
AltNum_AllowOverflowRep = Allow representation of numbers greator than 2147483647.999999999 and smaller than -2147483647.999999999
	If AltNum_EnableFractionals is disabled, represents numbers 2147483647.999999999 x 10^ExtraRep with the smallest digits truncated for each value above normal limit
	If AltNum_EnableFractionals is enabled and negative ExtraRep values are not used for any representation, represents numbers 2147483647.999999999 x 10^-ExtraRep with similar above truncation
	(Not Implimented)
*/

#pragma region PreprocessorToggles

#if defined(AltNum_EnableAutoToggleOfPreferedSettings)
    #define AltNum_EnablePiRep
    #define AltNum_EnableInfinityRep
	#define AltNum_EnableAlternativeRepFractionals
    #define AltNum_EnableDecimaledPiFractionals
    #define AltNum_EnableApproachingValues
    #define AltNum_UseDeveloperExtraDefaults//Turns on extra defaults just for testing
#endif

#if defined(AltNum_UseDeveloperExtraDefaults)
    #define AltNum_EnableImaginaryNum
    #define AltNum_EnableApproachingI
    #define AltNum_EnableMixedPiFractional
    #define AltNum_EnableERep
    //#define AltNum_EnableApproachingDivided
#endif

//If Pi rep is neither disabled or enabled, default to enabling Pi representation
#if !defined(AltNum_DisablePiRep) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(AltNum_EnablePiRep) && defined(AltNum_DisablePiRep)
    #undef AltNum_DisablePiRep
#endif

#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional) || defined(AltNum_EnableMixedIFractional)
    #define AltNum_EnableAlternativeMixedFrac
    #if !defined(AltNum_EnableMixedFractional)
        #define AltNum_EnableMixedFractional
    #endif
    #if !defined(AltNum_EnableAlternativeRepFractionals)
        #define AltNum_EnableAlternativeRepFractionals
    #endif
    #if !defined(AltNum_EnablePiRep) && defined(AltNum_EnableMixedPiFractional)
        #define AltNum_EnablePiRep
    #endif
    #if !defined(AltNum_EnableERep) && defined(AltNum_EnableMixedEFractional)
        #define AltNum_EnableERep
    #endif
    #if !defined(AltNum_EnableImaginaryNum) && defined(AltNum_EnableMixedIFractional)
        #define AltNum_EnableImaginaryNum
    #endif
#endif

#if defined(AltNum_EnableImaginaryInfinity)
    #define AltNum_EnableImaginaryNum
	#define AltNum_EnableInfinityRep
#endif

//Force required preprocessor flag for AltNum_EnableAlternativeRepFractionals
#if defined(AltNum_EnableAlternativeRepFractionals)
	#if !defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableERep)&&!defined(AltNum_EnableImaginaryNum)
		#undef AltNum_EnableAlternativeRepFractionals//Alternative Fractionals require the related representations enabled
	#elif !defined(AltNum_EnableFractionals)
		#define AltNum_EnableFractionals
	#endif
#endif

#if defined(AltNum_AvoidUsingLargeInt)
    #undef AltNum_UseOldDivisionCode
#endif

//Force required flags to be enabled if AltNum_EnableApproachingDivided toggled
#if !defined(AltNum_EnableApproachingValues)
	#if defined(AltNum_EnableApproachingDivided)
		#define AltNum_EnableApproachingValues
	#elif defined(AltNum_EnableApproachingPi) || defined(AltNum_EnableApproachingE) || defined(AltNum_EnableApproachingI)
		#define AltNum_EnableApproachingValues
	#endif
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

//Force enable Pi features if near Pi enabled
#if defined(AltNum_EnableApproachingPi) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(AltNum_EnableApproachingE) && !defined(AltNum_EnableERep)
	#define AltNum_EnableERep
#endif

#if defined(AltNum_EnableApproachingI) && !defined(AltNum_EnableImaginaryNum)
    #define AltNum_EnableImaginaryNum
#endif

#if !defined(AltNum_EnablePiFractional) &&defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnablePiFractional//PiFractional enabled (AltNum_EnablePiRep and AltNum_EnableAlternativeRepFractionals active)
#endif
#if !defined(AltNum_EnableEFractional) &&defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnableEFractional//EFractional enabled (AltNum_EnableERep and AltNum_EnableAlternativeRepFractionals active)
#endif
#if !defined(AltNum_EnableIFractional) &&defined(AltNum_EnableImaginaryNum)&&!defined(AltNum_EnableDecimaledIFractionals)&&defined(AltNum_EnableAlternativeRepFractionals)
    #define AltNum_EnableIFractional//IFractional enabled (AltNum_EnableImaginaryNum and AltNum_EnableAlternativeRepFractionals active)
#endif

#if defined(AltNum_EnablePiFractional) || defined(AltNum_EnableEFractional) || defined(AltNum_EnableIFractional)
	#define AltNum_UsingAltFractional//AltNum_EnablePiFractional, AltNum_EnableEFractional, or AltNum_EnableIFractional active
#endif

#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional)
    #define AltNum_MixedPiOrEEnabled//Either MixedPi or MixedE enabled
#endif

#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional) || defined(AltNum_EnableMixedEFractional)
    #define AltNum_MixedAltEnabled//Either MixedPi, MixedE, or MixedI enabled
#endif

#if defined(AltNum_EnableMixedPiFractional) && defined(AltNum_EnableDecimaledPiFractionals)
    #define AltNum_MixedPiHasDecimaledFracAccess
#elif defined(AltNum_EnableMixedEFractional) && defined(AltNum_EnableDecimaledEFractionals)
    #define AltNum_MixedEHasDecimaledFracAccess
#elif defined(AltNum_EnableMixedIFractional) && defined(AltNum_EnableDecimaledIFractionals)
    #define AltNum_MixedIHasDecimaledFracAccess
#endif

#if defined(AltNum_MixedPiHasDecimaledFracAccess) || defined(AltNum_MixedEHasDecimaledFracAccess) || defined(AltNum_MixedPiHasDecimaledFracAccess)
    #define AltNum_MixedAltFracHasDecimaledFractionalAccess
#endif

#if defined(AltNum_MixedPiHasDecimaledFracAccess)|| defined(AltNum_MixedEHasDecimaledFracAccess)
    #define AltNum_MixedPiOrEHasDecimaledFracAccess
#endif

#if defined(AltNum_EnableDecimaledPiFractionals) || defined(AltNum_EnableDecimaledEFractionals)
    #define AltNum_EnableDecimaledPiOrEFractionals
#endif

//Require variables in base to allow functions to be in base class
#if !defined(AltNum_DisableCommonVariablesInBase)&&defined(AltNum_StoreCommonFunctionsInBase)
    #undef AltNum_StoreCommonFunctionsInBase
#endif

#pragma endregion PreprocessorToggles

#if !defined(AltNum_DisableAltDecBaseDefaultToggles)
    #define AltNum_EnablePiRep
    #define AltNum_EnableInfinityRep
    #define AltNum_EnableAlternativeRepFractionals
    #define AltNum_EnableDecimaledPiFractionals
    #define AltNum_EnableApproachingValues
#define AltNum_UseDeveloperExtraDefaults//Turns on extra defaults just for testing
    //#define AltNum_EnableERep
#endif
