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

//#if defined(MixedDec_DeriveFromFlaggedDec)&&defined(FlaggedNum_ExtraRepIsActive)
//	#define MixedDec_DerivedFromExtendFlagDec
//#endif

#if defined(MixedDec_DeriveFromAltDec)||(defined(MixedDec_DeriveFromFlaggedDec)&&defined(FlaggedNum_ExtraRepIsActive))
	#define MixedDec_ExtraRepEnabled
#endif

#if defined(MixedDec_DeriveFromAltDec)||defined(MixedDec_DeriveFromFlaggedDec)
	#define MixedDec_UsingExtendedRepresentations
#endif