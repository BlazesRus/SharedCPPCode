// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once
//Preprocessor options
/*
FlaggedNum_EnableFractionals =
      Enables fractional representations in attempt to preserve more accuracy during operations
	  (functions same as having all variants of FlaggedNum_EnableDecimaledAlternativeFractionals active as well as AltNum_EnableFractionals from AltDec)
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

FlaggedNum_PreventUsageOfExtraRep

FlaggedNum_EnablePiFractions = Autotoggled if FlaggedNum_EnableFractionals
    and AltNum_EnablePiRep enabled
FlaggedNum_EnableEFractions = Autotoggled if FlaggedNum_EnableFractionals
    and AltNum_EnableERep enabled
FlaggedNum_EnableIFractions = Autotoggled if FlaggedNum_EnableFractionals 
    and AltNum_EnableImaginaryNum enabled

FlaggedNum_EnablePiFractional = Autotoggled if FlaggedNum_EnablePiFractions
    and FlaggedNum_PreventUsageOfExtraRep enabled
FlaggedNum_EnableEFractional = Autotoggled if FlaggedNum_EnableEFractions
    and FlaggedNum_PreventUsageOfExtraRep enabled
FlaggedNum_EnableIFractional = Autotoggled if FlaggedNum_EnableIFractions 
    and FlaggedNum_PreventUsageOfExtraRep enabled

FlaggedNum_EnableDecimaledPiFractionals = Autotoggled if FlaggedNum_EnablePiFractions
    and FlaggedNum_PreventUsageOfExtraRep not enabled
FlaggedNum_EnableDecimaledEFractionals = Autotoggled if FlaggedNum_EnableEFractions
    and FlaggedNum_PreventUsageOfExtraRep not enabled
FlaggedNum_EnableDecimaledIFractionals = Autotoggled if FlaggedNum_EnableIFractions 
    and FlaggedNum_PreventUsageOfExtraRep not enabled
*/
//If Pi rep is neither disabled or enabled, default to enabling Pi representation
#include "AlNumBase.hpp"

#if defined(FlaggedNum_EnableFractionals) || defined(FlaggedNum_EnablePowers) || defined(FlaggedNum_EnableMixedFractional) || defined(AltNum_EnableApproachingDivided) || defined(AltNum_EnableApproachingPi) || defined(AltNum_EnableApproachingE) || defined(AltNum_EnableApproachingI) || defined(AltNum_EnableUndefinedButInRange)
	#if !defined(FlaggedNum_ExtraRepIsActive)//Prevent redefinition warning
		#define FlaggedNum_ExtraRepIsActive
	#endif
#endif

#if defined(FlaggedNum_EnableFractionals) && defined(AltNum_EnablePiRep)
    #define FlaggedNum_EnablePiFractions
    #if !defined(FlaggedNum_PreventUsageOfExtraRep) && !defined(FlaggedNum_EnableDecimaledPiFractionals)
        #define FlaggedNum_EnableDecimaledPiFractionals
    #elif defined(FlaggedNum_EnablePiFractional)
        #define FlaggedNum_EnablePiFractional
    #endif
#endif

#if defined(FlaggedNum_EnableFractionals) && defined(AltNum_EnableERep)
    #define FlaggedNum_EnableEFractions
    #if !defined(FlaggedNum_PreventUsageOfExtraRep) && !defined(FlaggedNum_EnableDecimaledEFractionals)
        #define FlaggedNum_EnableDecimaledEFractionals
    #elif defined(FlaggedNum_EnableEFractional)
        #define FlaggedNum_EnableEFractional
    #endif
#endif

#if defined(FlaggedNum_EnableFractionals) && defined(AltNum_EnableImaginaryNum)
    #define FlaggedNum_EnableIFractions
    #if !defined(FlaggedNum_PreventUsageOfExtraRep) && !defined(FlaggedNum_EnableDecimaledIFractionals)
        #define FlaggedNum_EnableDecimaledIFractionals
    #elif defined(FlaggedNum_EnableIFractional)
        #define FlaggedNum_EnableIFractional
    #endif
#endif