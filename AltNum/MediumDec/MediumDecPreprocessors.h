// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//Preprocessor options
/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
	
AltNum_UseLegacyFloatingConversion = 
		Use old method to convert from floating point number into MediumDec variant
		If not set, than convert number into equivalent "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format to get more exact conversion
AltNum_UseBuiltinVirtualTable =
		Store a built-in virtual stable structure and use it to access virtual functions
*/

#if !defined(AltNum_DisableAltDecDefaultToggles)
    #define AltNum_EnablePiRep
    #define AltNum_EnableApproaching
	//AltNum_EnableIndeterminateForms
#endif

#if defined(AltNum_EnableAutoToggleOfPreferedSettings)
    #define AltNum_EnablePiRep
    #define AltNum_EnableERep
    #define AltNum_EnableApproaching
    #define AltNum_UseDeveloperExtraDefaults//Turns on extra defaults just for testing
#endif

#if defined(AltNum_UseDeveloperExtraDefaults)
    #define AltNum_EnableIRep
    #define AltNum_EnableApproaching
	#define AltNum_EnableFractionals
	#define AltNum_EnableInfinityRep
	#define AltNum_EnableMixedFractional
	//#define AltNum_EnablePowerOfRepresentation
	//#define AltNum_EnableIndeterminateForms
	//#define AltNum_EnableUndefinedButInRange
	//#define AltNum_EnableWithinMinMaxRange
#endif

#if defined(AltNum_EnablePiRep) && defined(AltNum_DisablePiRep)
    #undef AltNum_DisablePiRep
#endif

#if defined(AltNum_EnableImaginaryInfinity)
    #define AltNum_EnableIRep
	#define AltNum_EnableInfinityRep
#endif

#if defined(AltNum_EnableImaginaryRep)
	#define AltNum_EnableIRep
#endif

//Force required flags to be enabled if AltNum_EnableApproachingDivided toggled
#if !defined(AltNum_EnableApproaching)
	#if defined(AltNum_EnableApproachingDivided)
		#define AltNum_EnableApproaching
	#endif
#endif

#if !defined(UInt64)
	//Or use Using UInt64 = unsigned __int64;
	#define UInt64 UInt64
#endif

#if !defined(Int64)
	//Or use Using Int64 = signed __int64;
	#define Int64 Int64
#endif