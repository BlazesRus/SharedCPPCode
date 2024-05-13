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

//If Pi rep is neither disabled or enabled, default to enabling Pi representation
#if !defined(AltNum_DisablePiRep) && !defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

#if defined(MediumDecV2_EnableDefaultSettings)//Assuming AltNum_UseIntForDecimalHalf not toggled
	#define MediumDecV2_EnablePiRep
	#define MediumDecV2_EnableERep
#endif

//Force enable PiRep in derived AltDec if manually toggled for MediumDecV2
#if defined(MediumDecV2_EnablePiRep) &&!defined(AltNum_EnablePiRep)
    #define AltNum_EnablePiRep
#endif

//Force enable ERep in derived AltDec if manually toggled for MediumDecV2
#if defined(MediumDecV2_EnableERep) &&!defined(AltNum_EnableERep)
    #define AltNum_EnableERep
#endif

#if defined(AltNum_EnablePiRep)&&!defined(AltNum_UseIntForDecimalHalf)&&!defined(MediumDecV2_EnablePiRep)
	//Only Enable Pi if Partial Int is used(Is a Pi representation when DecimalHalf.Flag==1)
	#define MediumDecV2_EnablePiRep
#endif

#if defined(AltNum_EnableERep)&&!defined(AltNum_UseIntForDecimalHalf)&&!defined(MediumDecV2_EnableERep)
	//Only Enable E if Partial Int is used(Is a E representation when DecimalHalf.Flag==2)
	#define MediumDecV2_EnableERep
#endif

#if defined(AltNum_EnableImaginaryNum)&&!defined(AltNum_UseIntForDecimalHalf)&&!defined(MediumDecV2_EnableWithinMinMaxRange)&&!defined(MediumDecV2_EnableIRep)
	//Don't enable if MediumDecV2_EnableWithinMinMaxRange is toggled (because uses flag 3)
	#define MediumDecV2_EnableIRep
#endif

#if defined(MediumDecV2_EnablePiRep)&&defined(AltNum_EnableApproachingValues)&&!defined(MediumDecV2_EnableApproachingPi)
	#define MediumDecV2_EnableApproachingPi
#endif

#if defined(MediumDecV2_EnableERep)&&defined(AltNum_EnableApproachingValues)&&!defined(MediumDecV2_EnableApproachingE)
	#define MediumDecV2_EnableApproachingE
#endif

#if defined(MediumDecV2_EnableIRep)&&defined(AltNum_EnableApproachingValues)&&!defined(MediumDecV2_EnableApproachingI)
	#define MediumDecV2_EnableApproachingI
#endif

#if defined(MediumDecV2_EnableIRep)&&defined(AltNum_EnableImaginaryInfinity)&&!defined(MediumDecV2_EnableImaginaryInfinity)
	#define MediumDecV2_EnableImaginaryInfinity
#endif