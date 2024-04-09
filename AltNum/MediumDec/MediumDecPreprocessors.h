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
	(uses old behavior); Disabled for now(forcing usage of bitfield instead)
AltNum_EnableMirroredSection = Enable new Bitfield structure usage instead of using signed int (Not fully implimented yet)
AltNum_DefineInfinityAsSignedReps = Have Separate RepTypes stored for negative vs positive infinity(old behavior)
*/
#if defined(AltNum_EnableMirroredSection)
	#include "..\MirroredIntV2.hpp"
	#define IntHalfType MirroredIntV2
#else
	#define IntHalfType signed int
#endif

#if defined(AltNum_UseIntForDecimalHalf)
	#define DecimalHalfType signed int
#else
	#include "..\PartialInt.hpp"
	#define DecimalHalfType PartialInt
#endif

#if defined(AltNum_MinimizeRepTypeEnum)
	#define RepTypeUnderlayer unsigned short
#else
	#define RepTypeUnderlayer unsigned int
#endif