#include "RepType.h"
using RepType = BlazesRusCode::RepType;

#if defined(AltNum_UseRepTypeAsClass)
using RepTypeEnum = BlazesRusCode::RepTypeEnum;

#pragma region ValueDefine Source

inline const RepType RepType::NormalTypeValue(){ return RepTypeEnum::NormalType; }
	#if defined(AltNum_EnableFractionals)
inline RepType RepType::NumByDivValue(){ return RepTypeEnum::NumByDiv; }
	#endif
	#if defined(AltNum_EnablePowerOfRepresentation)
inline RepType RepType::ToPowerOfValue(){ return RepTypeEnum::ToPowerOf; }
	#endif
	#if defined(AltNum_EnablePiRep)
inline RepType RepType::PiNumValue(){ return RepTypeEnum::PiNum; }
		#if defined(AltNum_EnablePowerOfRepresentation)
inline RepType RepType::PiPowerValue(){ return RepTypeEnum::PiPower; }
		#endif
		#if defined(AltNum_EnableFractionals)
inline RepType RepType::PiNumByDivValue(){ return RepTypeEnum::PiNumByDiv; }
		#endif
		#if defined(AltNum_EnableApproaching)
inline RepType RepType::ApproachingBottomPiValue(){ return RepTypeEnum::ApproachingBottomPi; }
inline RepType RepType::ApproachingTopPiValue(){ return RepTypeEnum::ApproachingTopPi; }
			#if defined(AltNum_EnableApproachingDivided)
inline RepType RepType::ApproachingMidLeftPiValue(){ return RepTypeEnum::ApproachingMidLeftPi; }
inline RepType RepType::ApproachingMidRightPiValue(){ return RepTypeEnum::ApproachingMidRightPi; }
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableERep)
inline RepType RepType::ENumValue(){ return RepTypeEnum::ENum; }
		#if defined(AltNum_EnablePowerOfRepresentation)
inline RepType RepType::EPowerValue(){ return RepTypeEnum::EPower; }
		#endif
		#if defined(AltNum_EnableFractionals)
inline RepType RepType::ENumByDivValue(){ return RepTypeEnum::ENumByDiv; }
		#endif
		#if defined(AltNum_EnableApproaching)
inline RepType RepType::ApproachingBottomEValue(){ return RepTypeEnum::ApproachingBottomE; }
inline RepType RepType::ApproachingTopEValue(){ return RepTypeEnum::ApproachingTopE; }
			#if defined(AltNum_EnableApproachingDivided)
inline RepType RepType::ApproachingMidLeftEValue(){ return RepTypeEnum::ApproachingMidLeftE; }
inline RepType RepType::ApproachingMidRightEValue(){ return RepTypeEnum::ApproachingMidRightE; }
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
inline RepType RepType::InfinityValue(){ return RepTypeEnum::Infinity; }
	#endif
	#if defined(AltNum_EnableApproaching)
inline RepType RepType::ApproachingBottomValue(){ return RepTypeEnum::ApproachingBottom; }
inline RepType RepType::ApproachingTopValue(){ return RepTypeEnum::ApproachingTop; }
		#if defined(AltNum_EnableApproachingDivided)
inline RepType RepType::ApproachingMidLeftValue(){ return RepTypeEnum::ApproachingMidLeft; }
inline RepType RepType::ApproachingMidRightValue(){ return RepTypeEnum::ApproachingMidRight; }
		#endif
	#endif
	#if defined(AltNum_EnableIRep)
inline RepType RepType::INumValue(){ return RepTypeEnum::INum; }
		#if defined(AltNum_EnableFractionals)
inline RepType RepType::INumByDivValue(){ return RepTypeEnum::INumByDiv; }
		#endif
		#if defined(AltNum_EnableInfinity)
inline RepType RepType::ImaginaryInfinityValue(){ return RepTypeEnum::ImaginaryInfinity; }
		#endif
		#if defined(AltNum_EnableApproaching)
inline RepType RepType::ApproachingImaginaryBottomValue(){ return RepTypeEnum::ApproachingImaginaryBottom; }
inline RepType RepType::ApproachingImaginaryTopValue(){ return RepTypeEnum::ApproachingImaginaryTop; }
			#if defined(AltNum_EnableApproachingDivided)
inline RepType RepType::ApproachingImaginaryMidLeftValue(){ return RepTypeEnum::ApproachingImaginaryMidLeft; }
inline RepType RepType::ApproachingImaginaryMidRightValue(){ return RepTypeEnum::ApproachingImaginaryMidRight; }
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableMixedFractional)
inline RepType RepType::MixedFracValue(){ return RepTypeEnum::MixedFrac; }
		#if defined(AltNum_EnablePiRep)
inline RepType RepType::MixedPiValue(){ return RepTypeEnum::MixedPi; }
		#endif
		#if defined(AltNum_EnableERep)
inline RepType RepType::MixedEValue(){ return RepTypeEnum::MixedE; }
		#endif
		#if defined(AltNum_EnableIRep)
inline RepType RepType::MixedIValue(){ return RepTypeEnum::MixedI; }
		#endif
	#endif
	#if defined(AltNum_EnableNaN)
inline RepType RepType::UndefinedValue(){ return RepTypeEnum::Undefined; }
inline RepType RepType::NaNValue(){ return RepTypeEnum::NaN; }
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)
inline RepType RepType::UndefinedButInRangeValue(){ return RepTypeEnum::UndefinedButInRange; }
		#if defined(AltNum_EnableWithinMinMaxRange)
inline RepType RepType::WithinMinMaxRangeValue(){ return RepTypeEnum::WithinMinMaxRange; }
			#if defined(AltNum_EnablePiRep)
inline RepType RepType::WithinMinMaxRangePiValue(){ return RepTypeEnum::WithinMinMaxRangePi; }
			#endif
			#if defined(AltNum_EnableERep)
inline RepType RepType::WithinMinMaxRangeEValue(){ return RepTypeEnum::WithinMinMaxRangeE; }
			#endif
			#if defined(AltNum_EnableIRep)
inline RepType RepType::WithinMinMaxRangeIValue(){ return RepTypeEnum::WithinMinMaxRangeI; }
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableNil)
inline RepType RepType::NilValue(){ return RepTypeEnum::Nil; }
	#endif
	#if defined(AltNum_EnableIndeterminateForms)
inline RepType RepType::IndeterminateFormValue(){ return RepTypeEnum::IndeterminateForm; }
	#endif
	#if defined(AltNum_EnableEnhancedDivideByZeroForm)
inline RepType RepType::DividedByZeroValue(){ return RepTypeEnum::DividedByZero; }
	#endif

	#if defined(AltNum_EnableTrigExpressions)
inline RepType RepType::SinValue(){ return RepTypeEnum::Sin; }
		#if defined(AltNum_EnablePiRep)
inline RepType RepType::SinPiValue(){ return RepTypeEnum::SinPi; }
		#endif
		#if defined(AltNum_EnableERep)
inline RepType RepType::SinEValue(){ return RepTypeEnum::SinE; }
		#endif
		#if defined(AltNum_EnableIRep)
inline RepType RepType::SinIValue(){ return RepTypeEnum::SinI; }
		#endif

inline RepType RepType::CosValue(){ return RepTypeEnum::Cos; }
		#if defined(AltNum_EnablePiRep)
inline RepType RepType::CosPiValue(){ return RepTypeEnum::CosPi; }
		#endif
		#if defined(AltNum_EnableERep)
inline RepType RepType::CosEValue(){ return RepTypeEnum::CosE; }
		#endif
		#if defined(AltNum_EnableIRep)
inline RepType RepType::CosIValue(){ return RepTypeEnum::CosI; }
		#endif

inline RepType RepType::TanValue(){ return RepTypeEnum::Tan; }
		#if defined(AltNum_EnablePiRep)
inline RepType RepType::TanPiValue(){ return RepTypeEnum::TanPi; }
		#endif
		#if defined(AltNum_EnableERep)
inline RepType RepType::TanEValue(){ return RepTypeEnum::TanE; }
		#endif
		#if defined(AltNum_EnableIRep)
inline RepType RepType::TanIValue(){ return RepTypeEnum::TanI; }

		#endif
	#endif

const RepType RepType::NormalType = RepType::NormalTypeValue();
	#if defined(AltNum_EnableFractionals)
const RepType RepType::NumByDiv = RepType::NumByDivValue();
	#endif
	#if defined(AltNum_EnablePowerOfRepresentation)
const RepType RepType::ToPowerOf = RepType::ToPowerOfValue();
	#endif
	#if defined(AltNum_EnablePiRep)
const RepType RepType::PiNum = RepType::PiNumValue();
		#if defined(AltNum_EnablePowerOfRepresentation)
const RepType RepType::PiPower = RepType::PiPowerValue();
		#endif
		#if defined(AltNum_EnableFractionals)
const RepType RepType::PiNumByDiv = RepType::PiNumByDivValue();
		#endif
		#if defined(AltNum_EnableApproaching)
const RepType RepType::ApproachingBottomPi = RepType::ApproachingBottomPiValue();
const RepType RepType::ApproachingTopPi = RepType::ApproachingTopPiValue();
			#if defined(AltNum_EnableApproachingDivided)
const RepType RepType::ApproachingMidLeftPi = RepType::ApproachingMidLeftPiValue();
const RepType RepType::ApproachingMidRightPi = RepType::ApproachingMidRightPiValue();
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableERep)
const RepType RepType::ENum = RepType::ENumValue();
		#if defined(AltNum_EnablePowerOfRepresentation)
const RepType RepType::EPower = RepType::EPowerValue();
		#endif
		#if defined(AltNum_EnableFractionals)
const RepType RepType::ENumByDiv = RepType::ENumByDivValue();
		#endif
		#if defined(AltNum_EnableApproaching)
const RepType RepType::ApproachingBottomE = RepType::ApproachingBottomEValue();
const RepType RepType::ApproachingTopE = RepType::ApproachingTopEValue();
			#if defined(AltNum_EnableApproachingDivided)
const RepType RepType::ApproachingMidLeftE = RepType::ApproachingMidLeftEValue();
const RepType RepType::ApproachingMidRightE = RepType::ApproachingMidRightEValue();
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
const RepType RepType::Infinity = RepType::InfinityValue();
	#endif
	#if defined(AltNum_EnableApproaching)
const RepType RepType::ApproachingBottom = RepType::ApproachingBottomValue();
const RepType RepType::ApproachingTop = RepType::ApproachingTopValue();
		#if defined(AltNum_EnableApproachingDivided)
const RepType RepType::ApproachingMidLeft = RepType::ApproachingMidLeftValue();
const RepType RepType::ApproachingMidRight = RepType::ApproachingMidRightValue();
		#endif
	#endif
	#if defined(AltNum_EnableIRep)
const RepType RepType::INum = RepType::INumValue();
		#if defined(AltNum_EnableFractionals)
const RepType RepType::INumByDiv = RepType::INumByDivValue();
		#endif
		#if defined(AltNum_EnableInfinity)
const RepType RepType::ImaginaryInfinity = RepType::ImaginaryInfinityValue();
		#endif
		#if defined(AltNum_EnableApproaching)
const RepType RepType::ApproachingImaginaryBottom = RepType::ApproachingImaginaryBottomValue();
const RepType RepType::ApproachingImaginaryTop = RepType::ApproachingImaginaryTopValue();
			#if defined(AltNum_EnableApproachingDivided)
const RepType RepType::ApproachingImaginaryMidLeft = RepType::ApproachingImaginaryMidLeftValue();
const RepType RepType::ApproachingImaginaryMidRight = RepType::ApproachingImaginaryMidRightValue();
			#endif
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
const RepType RepType::MixedFrac = RepType::MixedFracValue();
		#if defined(AltNum_EnablePiRep)
const RepType RepType::MixedPi = RepType::MixedPiValue();
		#endif
		#if defined(AltNum_EnableERep)
const RepType RepType::MixedE = RepType::MixedEValue();
		#endif
		#if defined(AltNum_EnableIRep)
const RepType RepType::MixedI = RepType::MixedIValue();
		#endif
	#endif

	#if defined(AltNum_EnableNaN)
const RepType RepType::Undefined = RepType::UndefinedValue();
const RepType RepType::NaN = RepType::NaNValue();
	#endif

	#if defined(AltNum_EnableUndefinedButInRange)
const RepType RepType::UndefinedButInRange = RepType::UndefinedButInRangeValue();
		#if defined(AltNum_EnableWithinMinMaxRange)
const RepType RepType::WithinMinMaxRange = RepType::WithinMinMaxRangeValue();
			#if defined(AltNum_EnablePiRep)
const RepType RepType::WithinMinMaxRangePi = RepType::WithinMinMaxRangePiValue();
			#endif
			#if defined(AltNum_EnableERep)
const RepType RepType::WithinMinMaxRangeE = RepType::WithinMinMaxRangeEValue();
			#endif
			#if defined(AltNum_EnableIRep)
const RepType RepType::WithinMinMaxRangeI = RepType::WithinMinMaxRangeIValue();
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableNil)
const RepType RepType::Nil = RepType::NilValue();
	#endif

	#if defined(AltNum_EnableIndeterminateForms)
const RepType RepType::IndeterminateForm = RepType::IndeterminateFormValue();
	#endif
	#if defined(AltNum_EnableEnhancedDivideByZeroForm)
const RepType RepType::DividedByZero = RepType::DividedByZeroValue();
	#endif

	#if defined(AltNum_EnableTrigExpressions)
const RepType RepType::Sin = RepType::SinValue();
		#if defined(AltNum_EnablePiRep)
const RepType RepType::SinPi = RepType::SinPiValue();
		#endif
		#if defined(AltNum_EnableERep)
const RepType RepType::SinE = RepType::SinEValue();
		#endif
		#if defined(AltNum_EnableIRep)
const RepType RepType::SinI = RepType::SinIValue();
		#endif

const RepType RepType::Cos = RepType::CosValue();
		#if defined(AltNum_EnablePiRep)
const RepType RepType::CosPi = RepType::CosPiValue();
		#endif
		#if defined(AltNum_EnableERep)
const RepType RepType::CosE = RepType::CosEValue();
		#endif
		#if defined(AltNum_EnableIRep)
const RepType RepType::CosI = RepType::CosIValue();
		#endif

const RepType RepType::Tan = RepType::TanValue();
		#if defined(AltNum_EnablePiRep)
const RepType RepType::TanPi = RepType::TanPiValue();
		#endif
		#if defined(AltNum_EnableERep)
const RepType RepType::TanE = RepType::TanEValue();
		#endif
		#if defined(AltNum_EnableIRep)
const RepType RepType::TanI = RepType::TanIValue();
		#endif
	#endif

#pragma endregion ValueDefine Source

inline RepType BlazesRusCode::RepType::ConvertFromNormalRep(const unsigned int Flags) const
{
	switch (Flags)
	{
		#if defined(AltNum_EnablePiRep)
	case 1:
	{
		switch (Value)
		{
		case RepTypeEnum::NormalType:
			return RepTypeEnum::PiNum; break;
			#pragma region AltDecVariantExclusive
			#pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
		case RepTypeEnum::ApproachingBottom:
			return RepTypeEnum::ApproachingBottomPi; break;
		case RepTypeEnum::ApproachingTop:
			return RepTypeEnum::ApproachingTopPi; break;
			#pragma region AltDecVariantExclusive
			#pragma endregion AltDecVariantExclusive
			#endif
		default:
			throw "Conversion not defined";
		}
	}
	break;
	#endif
	#if defined(AltNum_EnableERep)
	case 2:
	{
		switch (repType)
		{
		case RepTypeEnum::NormalType:
			return RepTypeEnum::ENum; break;
			#pragma region AltDecVariantExclusive
			#pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
		case RepTypeEnum::ApproachingBottom:
			return RepTypeEnum::ApproachingBottomE; break;
		case RepTypeEnum::ApproachingTop:
			return RepTypeEnum::ApproachingTopE; break;
			#pragma region AltDecVariantExclusive
			#pragma endregion AltDecVariantExclusive
			#endif
		default:
			throw "Conversion not defined";
		}
	}
	break;
	#endif
	#if defined(AltNum_EnableIRep)
	case 3:
	{
		switch (repType)
		{
		case RepTypeEnum::NormalType:
			return RepTypeEnum::INum; break;
			#pragma region AltDecVariantExclusive
			#pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
		case RepTypeEnum::ApproachingBottom:
			return RepTypeEnum::ApproachingImaginaryBottom; break;
		case RepTypeEnum::ApproachingTop:
			return RepTypeEnum::ApproachingImaginaryTop; break;
			#pragma region AltDecVariantExclusive
			#pragma endregion AltDecVariantExclusive
			#endif
			#ifdef AltNum_EnableInfinity
		case RepTypeEnum::Infinity:
			return RepTypeEnum::ImaginaryInfinity; break;
			#endif
		default:
			throw "Conversion not defined";
		}
	}
	break;
	#endif
	default:
		return Value;
	}
}

inline RepType BlazesRusCode::RepType::GetRepAsNormalEquavant() const
{
	switch (Value)
	{
		#if defined(AltNum_EnablePiRep)
	case RepTypeEnum::PiNum:
		#endif
		#if defined(AltNum_EnableERep)
	case RepTypeEnum::ENum:
		#endif
		#if defined(AltNum_EnableERep)
	case RepTypeEnum::INum:
		#endif
		return RepTypeEnum::NormalType; break;
		#if defined(AltNum_EnableApproaching)
		#if defined(AltNum_EnablePiRep)
	case RepTypeEnum::ApproachingBottomPi:
		#endif
		#if defined(AltNum_EnableERep)
	case RepTypeEnum::ApproachingBottomE:
		#endif
		#if defined(AltNum_EnableIRep)
	case RepTypeEnum::ApproachingImaginaryBottom:
		#endif
		return RepTypeEnum::ApproachingBottom; break;
		#if defined(AltNum_EnablePiRep)
	case RepTypeEnum::ApproachingTopPi:
		#endif
		#if defined(AltNum_EnableERep)
	case RepTypeEnum::ApproachingTopE:
		#endif
		#if defined(AltNum_EnableIRep)
	case RepTypeEnum::ApproachingImaginaryBottom:
		#endif
		return RepTypeEnum::ApproachingTop; break;
		#endif
		#if defined(AltNum_EnableInfinity)&&defined(AltNum_EnableIRep)
	case RepTypeEnum::ImaginaryInfinity:
		return RepTypeEnum::Infinity; break;
		#endif
	default:
		return Value;
	}
}

#endif