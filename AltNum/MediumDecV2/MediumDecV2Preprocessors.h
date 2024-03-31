// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//Preprocessor options
/*

//--Infinity based preprocessors--
AltNum_EnableInfinityRep = Enable support of positive/negative infinity representations and approaching value representations
	Depreciated Defined Values:
      When DecimalHalf is -2147483648, it represents negative infinity(if IntValue is -1) or positive infinity(if IntValue is 1)
	New Defined Values:
AltNum_EnableApproachingValues
	Depreciated Defined Values:
      When DecimalHalf is -2147483647 and ExtraRep==0, it represents Approaching IntValue from right towards left (IntValue.0..1)
	  When DecimalHalf is -2147483647 and ExtraRep==1, it represents Approaching IntValue+1 from left towards right (IntValue.9..9)
	New Defined Values:
      When DecimalHalf is ApproachingRToLRep, it represents Approaching IntValue from right towards left (IntValue.0..1)
	  
AltNum_EnableApproachingPi = AltNum_EnableApproachingMidDec for Pi based variables(Partially Implimented)
AltNum_EnableApproachingE = AltNum_EnableApproachingMidDec for e based variables(Partially Implimented)
AltNum_EnableApproachingI = AltNum_EnableApproachingMidDec for imaginary based variables(Partially Implimented)

MediumDecV2_EnableUndefinedButInRange = Can't be enabled at as time as MediumDecV2_EnableIRep (Uses flag 3)

*/

#if defined(AltNum_EnablePiRep)&&!defined(AltNum_UseIntForDecimalHalf)&&!defined(MediumDecV2_EnablePiRep)
	//Only Enable Pi if Partial Int is used(Is a Pi representation when DecimalHalf.Flag==1)
	#define MediumDecV2_EnablePiRep
#endif

#if defined(AltNum_EnableERep)&&!defined(AltNum_UseIntForDecimalHalf)&&!defined(MediumDecV2_EnableERep)
	//Only Enable E if Partial Int is used(Is a E representation when DecimalHalf.Flag==2)
	#define MediumDecV2_EnableERep
#endif

#if defined(AltNum_EnableImaginaryNum)&&!defined(AltNum_UseIntForDecimalHalf)&&!defined(MediumDecV2_EnableUndefinedButInRange)&&!defined(MediumDecV2_EnableIRep)
	//Only Enable E if Partial Int is used(Is a E representation when DecimalHalf.Flag==2)
	#define MediumDecV2_EnableIRep
#endif

#if defined(MediumDecV2_EnableDefaultSettings)
	#define MediumDecV2_EnablePiRep
	#define MediumDecV2_EnableERep
#endif