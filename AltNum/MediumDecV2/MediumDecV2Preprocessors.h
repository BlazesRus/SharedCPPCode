// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//Preprocessor options
/*

//--Infinity based preprocessors--
AltNum_EnableInfinityRep = Enable support of positive/negative infinity representations
		When DecimalHalf.Value is InfinityRep and IntValue is positive, it represents positive infinity
		When DecimalHalf.Value is InfinityRep and IntValue is negative, it represents negative infinity
AltNum_EnableApproaching
      When DecimalHalf.Value is ApproachingBottomRep, it represents Approaching IntValue from right towards left (IntValue.0..1)
      When DecimalHalf.Value is ApproachingTopRep, it represents Approaching IntValue from left towards right (IntValue.9..9)

MediumDecV2_EnableWithinMinMaxRange = Can't be enabled at as time as MediumDecV2_EnableIRep (Uses flag 3)

AltNum_DefineDivideByZeroAsInfinity = Sets value as infinity when divide by zero instead of undefined result

AltNum_EnableIndeterminateForms = Enables extra representations for outputing and preserving the indeterminate form such 
	as 0/0, 0 to power of 0, 0 * infinity, infinity - infinity, 1 to power of infinity, and infinity to power of 0
	(Not fully implimented yet)
	
AltNum_EnableInfinityPowers =
Allows powers of infinity for operations where infinity is somehow more infinite than normal
      (Not Implemented)
*/

#if defined(AltNum_DefineDivideByZeroAsInfinity)&& !defined(AltNum_EnableInfinityRep)
	#define AltNum_EnableInfinityRep
#endif

//If Pi rep is neither disabled or enabled, default to enabling Pi representation
#if !defined(AltNum_DisablePiRep) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(AltNum_EnableDefaultSettings)//Assuming AltNum_UseIntForDecimalHalf not toggled
	#define AltNum_EnablePiRep
	#define AltNum_EnableERep
	//#define AltNum_EnableIndeterminateForms//use this toggle once have all the needed parts coded
#endif

//Forcing rename of toggle if alternative toggle used
#if defined(AltNum_EnableImaginaryNum)&&!defined(AltNum_EnableIRep)
	#define AltNum_EnableIRep
	#undef AltNum_EnableIRep
#endif

#if defined(MediumDecV2_EnableWithinMinMaxRange)&&defined(AltNum_EnableIRep)
	//Don't enable if MediumDecV2_EnableWithinMinMaxRange if Imaginary numbers are enabled because using flag 3 for MediumDecV2_EnableWithinMinMaxRange inside MediumDecV2
	#undef MediumDecV2_EnableWithinMinMaxRange
#elif defined(AltNum_EnableWithinMinMaxRange)&&!defined(AltNum_EnableIRep)
	//Don't enable if MediumDecV2_EnableWithinMinMaxRange if Imaginary numbers are enabled because using flag 3 for MediumDecV2_EnableWithinMinMaxRange inside MediumDecV2
	#define MediumDecV2_EnableWithinMinMaxRange
#endif

#if defined(AltNum_EnableInfinity)&&defined(AltNum_EnableIRep)&&!defined(AltNum_EnableImaginaryInfinity)
	#define AltNum_EnableImaginaryInfinity
#endif
