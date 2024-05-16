// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//Preprocessor options
/*

//--Infinity based preprocessors--
AltNum_EnableInfinityRep = Enable support of positive/negative infinity representations
	New Defined Values:
		When DecimalHalf.Value is InfinityRep and IntValue is positive, it represents positive infinity
		When DecimalHalf.Value is InfinityRep and IntValue is negative, it represents negative infinity
AltNum_EnableApproaching
	New Defined Values:
      When DecimalHalf.Value is ApproachingBottomRep, it represents Approaching IntValue from right towards left (IntValue.0..1)
      When DecimalHalf.Value is ApproachingTopRep, it represents Approaching IntValue from left towards right (IntValue.9..9)
	  
AltNum_EnableApproachingPi = Enable approaching values for PiNum representations
AltNum_EnableApproachingE = Enable approaching values for ENum representations
AltNum_EnableApproachingI = Enable approaching values for INum representations

MediumDecV2_EnableWithinMinMaxRange = Can't be enabled at as time as MediumDecV2_EnableIRep (Uses flag 3)

AltNum_DefineDivideByZeroAsInfinity = Sets value as infinity when divide by zero instead of undefined result
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
#endif

//Forcing rename of toggle if alternative toggle used
#if defined(AltNum_EnableImaginaryNum)&&!defined(AltNum_EnableIRep)
	#define AltNum_EnableIRep
	#undef AltNum_EnableIRep
#endif

#if defined(MediumDecV2_EnableWithinMinMaxRange)&&defined(AltNum_EnableIRep)
	//Don't enable if MediumDecV2_EnableWithinMinMaxRange if Imaginary numbers are enabled because using flag 3 for MediumDecV2_EnableWithinMinMaxRange inside MediumDecV2
	#undef MediumDecV2_EnableWithinMinMaxRange
#endif

/*
#if defined(AltNum_EnablePiRep)&&defined(AltNum_EnableApproaching)&&!defined(AltNum_EnableApproachingPi)
	#define AltNum_EnableApproachingPi
#endif

#if defined(AltNum_EnableERep)&&defined(AltNum_EnableApproaching)&&!defined(AltNum_EnableApproachingE)
	#define AltNum_EnableApproachingE
#endif

#if defined(AltNum_EnableIRep)&&defined(AltNum_EnableApproaching)&&!defined(AltNum_EnableApproachingI)
	#define AltNum_EnableApproachingI
#endif
*/