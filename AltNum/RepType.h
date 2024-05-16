// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#if defined(AltNum_MinimizeRepTypeEnum)
	#define RepTypeUnderlayer unsigned short
#else
	#define RepTypeUnderlayer unsigned int
#endif

namespace BlazesRusCode
{
	/// <summary>
	/// Enum representing value type stored
	/// </summary>
	enum class RepType: RepTypeUnderlayer
	{
		//Sign = IntValue.IsNegative()?-1:1;
		NormalType = 0,
#if defined(AltNum_EnableFractionals)
		NumByDiv = 8,
#endif
#if defined(AltNum_EnablePowerOfRepresentation)
		ToPowerOf = 16,
#endif
#if defined(AltNum_EnablePiRep)
		PiNum = 1,
	#if defined(AltNum_EnablePowerOfRepresentation)
		PiPower = 17,
	#endif
	#if defined(AltNum_EnableFractionals)
		PiNumByDiv = 9,//  (Value/(ExtraRep.Value))*Pi Representation
	#endif
	#if defined(AltNum_EnableApproaching)
		//(Enum Bits:7,1)
		//equal to IntValue.0..01 Pi
		ApproachingBottomPi = 65,
		#if !defined(AltNum_DisableApproachingTop)
		//(Enum Bits:7,1,4)
		//equal to IntValue.9..9 Pi
		ApproachingTopPi = 73,
		#endif
		#if defined(AltNum_EnableApproachingDivided)
		//(Enum Bits:7,5,1)
		ApproachingMidLeftPi = 81,
			#if !defined(AltNum_DisableApproachingTop)
		//(Enum Bits:7,4,5,1)
		ApproachingMidRightPi = 89,
			#endif
		#endif
	#endif
#endif
#if defined(AltNum_EnableERep)
		ENum = 2,
	#if defined(AltNum_EnablePowerOfRepresentation)
		EPower = 18,
	#endif
	#if defined(AltNum_EnableFractionals)
		ENumByDiv = 10,//(Value/(ExtraRep.Value))*e Representation
	#endif
	#if defined(AltNum_EnableApproaching)
		//(Enum Bits:7,2)
		//equal to IntValue.0..01 e
		ApproachingBottomE = 66,
		#if !defined(AltNum_DisableApproachingTop)
		//(Enum Bits:7,2)
		//equal to IntValue.9..9 e
		ApproachingTopE = 74,
		#endif
		#if defined(AltNum_EnableApproachingDivided)
		//(Enum Bits:7,5,2)
		ApproachingMidLeftE = 82,
			#if !defined(AltNum_DisableApproachingTop)
		//(Enum Bits:7,4,5,2)
		ApproachingMidRightE = 90,
			#endif
		#endif
	#endif
#endif
#if defined(AltNum_EnableIRep)
		INum = 4,
	#if defined(AltNum_EnableFractionals)
		//(Value/(ExtraRep.Value))*i Representation
		INumByDiv = 11,
	#endif
#endif
#if defined(AltNum_EnableMixedFractional)
		//Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
		MixedFrac = 32,
	#if defined(AltNum_EnablePiRep)
		//Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
		MixedPi = 33,
	#endif
	#if defined(AltNum_EnableERep)
		//Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
		MixedE = 34,
	#endif
	#if defined(AltNum_EnableIRep)
		//Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
		MixedI = 36,
	#endif
#endif

#if defined(AltNum_EnableInfinityRep)
		//(Enum Bits:7,6)
		//If Positive Infinity, then convert number into MaximumValue instead when need as real number
		//If Negative Infinity, then convert number into MinimumValue instead when need as real number
		Infinity = 96,
#endif
#if defined(AltNum_EnableApproaching)
		//(Enum Bits:7)
		ApproachingBottom = 64,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
	#if !defined(AltNum_DisableApproachingTop)
		//(Enum Bits:7,4)
		//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
		ApproachingTop = 72,
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		//(Enum Bits:7,5)
		//DecimalHalf:1000000000/ExtraRep - ApproachingZero (AlternativeName:ApproachingMidLeft)
		ApproachingMidLeft = 80,
		#if !defined(AltNum_DisableApproachingTop)
		//(Enum Bits:7,4,5)
		//DecimalHalf:1000000000/ExtraRep + ApproachingZero (AlternativeName:ApproachingMidRight)
		ApproachingMidRight = 88,
		#endif
	#endif
#endif
#if defined(AltNum_EnableImaginaryInfinity)
		//(Enum Bits:7,6,3)
		ImaginaryInfinity = 100,
#endif
#if defined(AltNum_EnableApproaching)&&defined(AltNum_EnableIRep)
		//(Enum Bits:7,3)
		//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
		ApproachingImaginaryBottom = 68,
	#if !defined(AltNum_DisableApproachingTop)
		//(Enum Bits:7,3,4)
		//(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
		ApproachingImaginaryTop = 76,
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		//(Enum Bits:7,3,5)
		//DecimalHalf:1000000000/ExtraRep - ApproachingImaginaryZero
		ApproachingImaginaryMidLeft = 84,
		#if !defined(AltNum_DisableApproachingTop)
		//(Enum Bits:7,3,5,4)
		//DecimalHalf:1000000000/ExtraRep + ApproachingImaginaryZero
		ApproachingImaginaryMidRight = 92,
		#endif
	#endif
#endif
#if defined(AltNum_EnableNaN)
		//(Enum Bits:8)
		Undefined = 128,
		//(Enum Bits:8, 1)
		NaN = 129,
#endif
#if defined(AltNum_EnableUndefinedButInRange)
		//(Enum Bits:8, 2)
		//Such as result of Cos of infinity(value format part uses for +- range, DecimalHalf==UndefinedInRangeRep)
		UndefinedButInRange = 130,
	#if defined(AltNum_EnableWithinMinMaxRange)
		//(Enum Bits:8,6)
		//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
		WithinMinMaxRange = 160,
	#endif
#endif
#if defined(AltNum_EnableNil)
		//(Enum Bits:8, 1, 2)
		Nil = 131,
#endif
	#ifdef AltNum_EnableComplexNumbers
		//Enum Bits subject to change for Complec Number later(Not completely used yet)
		ComplexIRep = 255,
	#endif
		//(Enum Bits:8, 1, 2, 3)
		UnknownType = 135
	}
]