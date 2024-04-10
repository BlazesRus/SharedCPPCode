// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//Preprocessor options
/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult

MixedDec_DeriveFromAltDec =
	Deriving MixedDec from AltDec
	(includes flags for Pi, E, and i within same bytes that store DecimalHalf)
	(includes ExtraRep field for extended representation)
MixedDec_DeriveFromMediumDecV2 =
	Deriving MixedDec from MediumDecV2
	(includes flags for Pi, E, and i within same bytes that store DecimalHalf)

MixedDec_EnableAltFloat = Use custom floating point class that acts similar to float in how digits are stored to store trailing digits
MixedDec_EnableRestrictedFloat = Uses builtin class RestrictedFloat to store trailing digits (representing only fractional range between 0 and 1 of trailing digits)
If neither MixedDec_EnableAltFloat or MixedDec_EnableRestrictedFloat are enabled then trailing digits are stored inside float
*/

#if defined(MixedDec_DeriveFromAltDec)
	#if defined(AltNum_EnableFractionals)
		#define MixedDec_EnableFractionals
	#endif
    #if defined(AltNum_EnablePiRep)
	    #if defined(AltNum_EnablePiPowers)
            #define MixedDec_EnablePiRep
	    #endif
	    #if defined(AltNum_EnableDecimaledPiFractionals)
            #define MixedDec_EnableDecimaledPiFractionals//  (Value/(ExtraRep*-1))*Pi Representation
	    #elif defined(AltNum_EnablePiFractional)
            #define MixedDec_EnablePiFractional//  IntValue/DecimalHalf*Pi Representation
	    #endif
	#endif
    #if defined(AltNum_EnableERep)
            #define MixedDec_EnableERep
        #if defined(AltNum_EnableDecimaledEFractionals)
            #define MixedDec_EnableDecimaledEFractional//(Value/(ExtraRep*-1))*e Representation
	    #elif defined(AltNum_EnableEFractional)
            #define MixedDec_EnableEFractional//  IntValue/DecimalHalf*e Representation
	    #endif
    #endif
    #if defined(AltNum_EnableImaginaryNum)
            #define MixedDec_EnableImaginaryNum
	    #if defined(AltNum_EnableDecimaledIFractionals)
            #define MixedDec_EnableDecimaledIFractionals//(Value/(ExtraRep*-1))*i Representation
	    #elif defined(AltNum_EnableIFractional)
            #define MixedDec_EnableIFractional//  IntValue/DecimalHalf*i Representation
	    #endif
        #ifdef AltNum_EnableComplexNumbers
            #define MixedDec_EnableComplexNumbers
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
        #define MixedDec_EnableMixedFractional//IntValue +- (-DecimalHalf)/ExtraRep
    #endif
	#if defined(AltNum_EnableMixedPiFractional)
        #define MixedDec_EnableMixedPiFractional//IntValue +- (-DecimalHalf/-ExtraRep)
	#elif defined(AltNum_EnableMixedEFractional)
        #define MixedDec_EnableMixedEFractional//IntValue +- (-DecimalHalf/-ExtraRep)
	#elif defined(AltNum_EnableMixedIFractional)
        #define MixedDec_EnableMixedIFractional//IntValue +- (-DecimalHalf/-ExtraRep)
	#endif
    #if defined(AltNum_EnableInfinityRep)
        #define MixedDec_EnableInfinityRep
    #endif
    #if defined(AltNum_EnableApproachingValues)
        #define MixedDec_EnableApproachingValues
    #endif
	#if defined(AltNum_DisableApproachingTop)
		#define MixedDec_DisableApproachingTop
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		#define MixedDec_EnableApproachingDivided
	#endif
    #if defined(AltNum_EnableNaN)
        #define MixedDec_EnableNaN
    #endif
    #if defined(AltNum_EnableApproachingPi)
        #define MixedDec_EnableApproachingPi//equal to IntValue.9..9 Pi
    #endif
    #if defined(AltNum_EnableApproachingE)
        #define MixedDec_EnableApproachingE//equal to IntValue.9..9 e
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
        #define MixedDec_EnableImaginaryInfinityEnableImaginaryInfinity
	#endif
    #if defined(AltNum_EnableApproachingI)
        #define MixedDec_ApproachingI
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
        #define MixedDec_EnableUndefinedButInRange
        #if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
            #define MixedDec_EnableWithinMinMaxRange
        #endif
    #endif
    #if defined(AltNum_EnableNil)
        #define MixedDec_EnableNilRep
    #endif
#endif

#if	defined(MixedDec_DeriveFromAltDec) || defined(MixedDec_DeriveFromMediumDecV2)
	#define MixedDec_EnableAlternativeRepresentations
#endif