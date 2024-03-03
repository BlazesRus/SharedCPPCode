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
	(includes ExtraRep field for extended representation)
MixedDec_DeriveFromFlaggedDec =
	Deriving MixedDec from FlaggedDec 
	(includes flags field and optionally ExtraRep field field if needed)
MixedDec_DerivedFromExtendFlagDec = 
	Auto toggled if MixedDec_DeriveFromFlaggedDec and FlaggedNum_ExtraRepIsActive is true
MixedDec_UsingExtendedRepresentations =
	Autotoggled if MixedDec_DeriveFromAltDec or MixedDec_DeriveFromFlaggedDec enabled
*/

#if defined(MixedDec_DeriveFromAltDec)||(defined(MixedDec_DeriveFromFlaggedDec)&&defined(FlaggedNum_ExtraRepIsActive))
	#define MixedDec_ExtraRepEnabled
#endif

#if defined(MixedDec_DeriveFromAltDec)||defined(MixedDec_DeriveFromFlaggedDec)
	#define MixedDec_UsingExtendedRepresentations
#endif

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
    #if defined(AltNum_EnableNilRep)
        #define MixedDec_EnableNilRep
    #endif
#elif defined(MixedDec_DeriveFromFlaggedDec)
	#define MixedDec_EnableFractionals
	#if defined(FlaggedNum_EnablePowers)
        #define MixedDec_EnablePowers//with flag 05, and other flag active
	#endif
	#if defined(FlaggedNum_EnablePiRep)
        #define MixedDec_EnablePiRep//with flag 01 active
		#if defined(FlaggedNum_EnablePiFractions)
            #define MixedDec_EnablePiFractions//  (Value/ExtraRep)*Pi Representation with flag 04 and 01 active
		#endif
	#endif
	#if defined(FlaggedNum_EnableERep)
        #define MixedDec_EnableERep//with flag 02 active
		#if defined(FlaggedNum_EnableEFractions)
            #define MixedDec_EnableEFractions//(Value/ExtraRep)*e Representation with flag 04 and 02 active
		#endif
	#endif
	#if defined(FlaggedNum_EnableImaginaryNum)
            #define MixedDec_EnableImaginaryNum//with flag 03 active
		#if defined(FlaggedNum_EnableIFractions)
            #define MixedDec_EnableIFractions//(Value/ExtraRep)*i Representation with flag 04 and 03 active
		#endif
	#endif
	#if defined(FlaggedNum_EnableMixedFractional)
        #define MixedDec_EnableMixedFractional
	#endif
	#if defined(FlaggedNum_EnableInfinityRep)
        #define MixedDec_EnableInfinityRep
	#endif
	#if defined(FlaggedNum_EnableApproachingValues)
		#define MixedDec_EnableApproachingValues
	#endif
	#if defined(FlaggedNum_DisableApproachingTop)
		#define MixedDec_DisableApproachingTop
	#endif
	#if defined(FlaggedNum_EnableApproachingDivided)
		#define MixedDec_EnableApproachingDivided
	#endif
	#if defined(FlaggedNum_EnableNaN)
		#define MixedDec_EnableNaN
	#endif
	#if defined(FlaggedNum_EnableApproachingPi)
        #define MixedDec_EnableApproachingPi//equal to IntValue.9..9 Pi (Enum Bits:7,1,5)
	#endif
	#if defined(FlaggedNum_EnableApproachingE)
        #define MixedDec_EnableApproachingE//equal to IntValue.9..9 e (Enum Bits:7,2, 5)
	#endif
	#if defined(FlaggedNum_EnableImaginaryInfinity)
        #define MixedDec_EnableImaginaryInfinity
		#if defined(FlaggedNum_EnableApproachingI)
            #define MixedDec_EnableApproachingI
		#endif
	#endif
	#if defined(FlaggedNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
		#define MixedDec_EnableUndefinedButInRange
	#endif
	#if defined(FlaggedNum_EnableNilRep)
		#define MixedDec_EnableNilRep
	#endif
#endif