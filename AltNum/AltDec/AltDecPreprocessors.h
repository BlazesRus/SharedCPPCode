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
AltNum_EnableNegativePowerRep = 
	Allows negative powers when AltNum_EnablePowerOfRepresentation is enabled

//--Infinity based preprocessors--
AltNum_EnableApproaching
	Depreciated Defined Values:
      When DecimalHalf is -2147483647 and ExtraRep==1, it represents Approaching IntValue from right towards left (IntValue.0..1)
	  When DecimalHalf is -2147483647 and ExtraRep==1, it represents Approaching IntValue+1 from left towards right (IntValue.9..9)
AltNum_EnableApproachingDivided =
	Enables Approaching IntValue.49..9 and IntValue.50..1 and other Approaching values (49..9 = ExtraRep value of 2; 50..1 = ExtraRep value of -2)
	When ExtraRep is Positive, represents (when IntValue is positive) IntValue + (1/ExtraRep + ApproachingBottomValue)(approaching left towards right)
	When ExtraRep is Positive, represents (when IntValue is positive) IntValue + (1/ExtraRep + ApproachingTopValue)(approaching right towards left)
	(Assumes AltNum_EnableInfinityRep is enabled)
	(Partially Implimented)

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
	
AltNum_EnableImaginaryInfinity = Enables imaginary infinity option (Depreciated--Enabled by default if both infinity and Imaginary numbers are enabled)
AltNum_DisableApproachingTop = Disables IntValue.9..9 representation

AltNum_EnableNil = Enables Nil representation(detection not in code right now)


AltNum_EnableUndefinedButInRange = Enable representation of unknown number between -Value and +Value for Cos of infinity etc
AltNum_EnableWithinMinMaxRange
AltNum_EnableUnknownTrigExpressions = (Not Implemented)

AltNum_PreventModulusOverride = Turns off modulus overrides if toggled
AltNum_EnableAlternativeModulusResult = Add addition modulus operations that give AltNumModChecker<AltNum> result
AltNum_EnableBitwiseOverride = Enables bitwise operation overrides if enabled

AltNum_UseDeveloperExtraDefaults = Auto toggles extra settings to more fully test feature sets

AltNum_DisableMultiplyDownToNothingPrevention = Disables preventing multiplication operations from multiplying into nothing as result of getting too small
	
AltNum_EnableIndeterminateForms = Enables extra representations for outputing and preserving the indeterminate form such 
	as 0/0, 0 to power of 0, 0 * infinity, infinity - infinity, 1 to power of infinity, and infinity to power of 0
	(Not fully implimented yet)
	
AltNum_AllowNegativeZero = Allows zero to keep the negative sign when multiplying(not implimented yet)
AltNum_EnableEnhancedDivideByZeroForm = 
	Auto toggles either AltNum_ReserveZeroRepForDivideByZero or AltNum_ReserveLastRepForDivideByZero
AltNum_ReserveZeroRepForDivideByZero = Reserves 0 ExtraRep value for Divide by Zero Indeterminate form (Autotoggled if AltNum_EnableEnhancedDivideByZeroForm is set and AltNum_EnableWithinMinMaxRange is not set)
	(Not Implimented yet)
AltNum_ReserveLastRepForDivideByZero
	(Not Implimented yet)
*/

#pragma region PreprocessorToggles

//Require AltNum_DisableApproachingTop not to be toggled to allow AltNum_EnableApproachingDivided to be enabled
#if defined(AltNum_DisableApproachingTop) && defined(AltNum_EnableApproachingDivided)
	#undef AltNum_EnableApproachingDivided
#endif

#if defined(AltNum_EnableNegativePowerRep)&&defined(AltNum_EnableWithinMinMaxRange)
	#undef AltNum_EnableWithinMinMaxRange
#endif

#if defined(AltNum_ReserveZeroRepForDivideByZero)
	#undef AltNum_ReserveZeroRepForDivideByZero
#endif

#if defined(AltNum_EnableNegativePowerRep)&&defined(AltNum_EnableFractionals)
	#undef AltNum_EnableFractionals
#endif

#if defined(AltNum_EnableNegativePowerRep)&&defined(AltNum_EnableMixedFractional)
	#undef AltNum_EnableMixedFractional
#endif

#if defined(AltNum_ReserveLastRepForDivideByZero)&&defined(AltNum_ReserveZeroRepForDivideByZero)
	#undef AltNum_ReserveLastRepForDivideByZero
#endif

#if define(AltNum_EnableEnhancedDivideByZeroForm)
	#if !defined(AltNum_EnableWithinMinMaxRange)&&!defined(AltNum_ReserveZeroRepForDivideByZero)
		#define AltNum_ReserveZeroRepForDivideByZero
		#if defined(AltNum_ReserveLastRepForDivideByZero)
			#undef AltNum_ReserveLastRepForDivideByZero
		#endif
	#else
		#define AltNum_ReserveLastRepForDivideByZero
	#elif
#endif

#pragma endregion PreprocessorToggles
