// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//Preprocessor options
/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
AltNum_UseIntForDecimalHalf = Use signed int instead of using a custom bitfield structure for DecimalHalf
	(uses old behavior); Depreciating and removing old legacy code soon
	
AltNum_EnableMirroredSection = Enable new Bitfield structure usage instead of using signed int (Not fully implimented yet)
AltNum_UseLegacyFloatingConversion = 
		Use old method to convert from floating point number into MediumDec variant
		If not set, than convert number into equivalant "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format to get more exact conversion
AltNum_UseBuiltinVirtualTable =
		Store a built-in virtual stable structure and use it to access virtual functions
*/

#if defined(AltNum_EnableAutoToggleOfPreferedSettings)
    #define AltNum_EnablePiRep
    #define AltNum_EnableERep
    //#define AltNum_EnableInfinityRep
	//#define AltNum_EnableFractionals
    //#define AltNum_EnableDecimaledPiFractionals
    #define AltNum_EnableApproachingValues
    //#define AltNum_UseDeveloperExtraDefaults//Turns on extra defaults just for testing
	//#define AltNum_EnableMirroredSection
#endif

//#if defined(AltNum_EnableMirroredSection)
	#include "..\MirroredInt.hpp"
	#define IntHalfType MirroredInt
/*#else
	#define IntHalfType signed int
	#if !defined(AltNum_UseLegacyFloatingConversion)
		#define AltNum_UseLegacyFloatingConversion
	#endif
#endif*/

/*#if defined(AltNum_UseIntForDecimalHalf)
	#define DecimalHalfType signed int
	#if !defined(AltNum_UseLegacyFloatingConversion)
		#define AltNum_UseLegacyFloatingConversion
	#endif
#else*/
	#include "..\PartialInt.hpp"
	#define DecimalHalfType PartialInt
//#endif

#if defined(AltNum_EnablePiRep) && defined(AltNum_DisablePiRep)
    #undef AltNum_DisablePiRep
#endif

#if defined(AltNum_EnableImaginaryInfinity)
    #define AltNum_EnableImaginaryNum
	#define AltNum_EnableInfinityRep
#endif

#if define(AltNum_EnableIRep)
	#define AltNum_EnableImaginaryNum
#endif

//Force required flags to be enabled if AltNum_EnableApproachingDivided toggled
#if !defined(AltNum_EnableApproachingValues)
	#if defined(AltNum_EnableApproachingDivided)
		#define AltNum_EnableApproachingValues
	#elif defined(AltNum_EnableApproachingPi) || defined(AltNum_EnableApproachingE) || defined(AltNum_EnableApproachingI)
		#define AltNum_EnableApproachingValues
	#endif
#endif

#if defined(AltNum_UseIntForDecimalHalf)

	//Disable Mixed Fractionals for now if PartialInt is not being used
	#if defined(AltNum_EnableMixedFractional)
		#undef AltNum_EnableMixedFractional
	#endif

/*	#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional) || defined(AltNum_EnableMixedIFractional)
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
	#endif*/

/*	#if defined(AltNum_EnableMixedPiFractional) && defined(AltNum_EnableDecimaledPiFractionals)
		#define AltNum_MixedPiHasDecimaledFracAccess
	#elif defined(AltNum_EnableMixedEFractional) && defined(AltNum_EnableDecimaledEFractionals)
		#define AltNum_MixedEHasDecimaledFracAccess
	#elif defined(AltNum_EnableMixedIFractional) && defined(AltNum_EnableDecimaledIFractionals)
		#define AltNum_MixedIHasDecimaledFracAccess
	#endif*/

/*	#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional) || defined(AltNum_EnableMixedEFractional)
		#define AltNum_MixedAltEnabled//Either MixedPi, MixedE, or MixedI enabled
	#endif*/

	#if defined(AltNum_EnablePiRep)
		#if defined(AltNum_EnableFractionals)
			#define AltNum_EnablePiFractional
		#endif
	#endif
	
	#if defined(AltNum_EnableERep)
		#if defined(AltNum_EnableFractionals)
			#define AltNum_EnableEFractional
		#endif
	#endif
	
	#if defined(AltNum_EnableImaginaryNum)
		#if defined(AltNum_EnableFractionals)
			#define AltNum_EnableIFractional
		#endif
	#endif

#else

	#if defined(AltNum_EnablePiRep)
		#if defined(AltNum_EnableFractionals)
			#define AltNum_EnableDecimaledPiFractionals
		#endif
	#endif
	
	#if defined(AltNum_EnableERep)
		#if defined(AltNum_EnableFractionals)
			#define AltNum_EnableDecimaledEFractionals
		#endif
	#endif
	
	#if defined(AltNum_EnableImaginaryNum)
		#if defined(AltNum_EnableFractionals)
			#define AltNum_EnableDecimaledIFractionals
		#endif
	#endif

#endif

#if defined(AltNum_AvoidUsingLargeInt)
    #undef AltNum_UseOldDivisionCode
#endif

#if defined(AltNum_EnableApproachingPi) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(AltNum_EnableApproachingE) && !defined(AltNum_EnableERep)
	#define AltNum_EnableERep
#endif

#if defined(AltNum_EnableApproachingI) && !defined(AltNum_EnableImaginaryNum)
    #define AltNum_EnableImaginaryNum
#endif

#if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional)
    #define AltNum_MixedPiOrEEnabled//Either MixedPi or MixedE enabled
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