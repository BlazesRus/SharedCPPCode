#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

#if defined(AltDec_UseMirroredInt)
	MirroredInt AltDec::NegativeRep = MirroredInt::NegativeZero;
#endif
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproachingValues)
    AltDec AltDec::AlmostOne = AltDec::ApproachingRightRealValue();
#endif
    AltDec AltDec::Pi = AltDec::PiValue();
    AltDec AltDec::One = AltDec::OneValue();
    AltDec AltDec::Two = AltDec::TwoValue();
    AltDec AltDec::NegativeOne = AltDec::NegativeOneValue();
    AltDec AltDec::Zero = AltDec::ZeroValue();
    AltDec AltDec::PointFive = AltDec::Point5Value();
    AltDec AltDec::JustAboveZero = AltDec::JustAboveZeroValue();
    AltDec AltDec::OneMillionth = AltDec::OneMillionthValue();
    AltDec AltDec::FiveThousandth = AltDec::FiveThousandthValue();
    AltDec AltDec::Minimum = AltDec::MinimumValue();
    AltDec AltDec::Maximum = AltDec::MaximumValue();
    AltDec AltDec::E = AltDec::EValue();
    AltDec AltDec::LN10 = AltDec::LN10Value();
    AltDec AltDec::LN10Mult = AltDec::LN10MultValue();
    AltDec AltDec::HalfLN10Mult = AltDec::HalfLN10MultValue();
    AltDec AltDec::TenMillionth = AltDec::TenMillionthValue();
    AltDec AltDec::FiveMillionth = AltDec::FiveMillionthValue();
    AltDec AltDec::FiveBillionth = AltDec::FiveBillionthValue();
    AltDec AltDec::OneGMillionth = AltDec::OneHundredMillionthValue();
    #if defined(AltNum_EnableNilRep)
    AltDec AltDec::Nil = AltDec::NilValue();
    #endif

    AltDec AltDec::PiNum = AltDec::PiNumValue();
    AltDec AltDec::ENum = AltDec::ENumValue();
    
#if defined(AltNum_EnableInfinityRep)
    AltDec AltDec::Infinity = AltDec::InfinityValue();
#endif
#if defined(AltNum_EnableApproachingValues)
    AltDec AltDec::NegativeInfinity = AltDec::NegativeInfinityValue();
    AltDec AltDec::ApproachingZero = AltDec::ApproachingZeroValue();
#endif
#if defined(AltNum_EnableNaN)
    AltDec AltDec::NaN = AltDec::NaNValue();
	AltDec AltDec::Undefined = AltDec::UndefinedValue();
#endif
    #pragma endregion ValueDefine Source
	
#if defined(AltNum_EnableImaginaryNum)
inline void BlazesRusCode::AltDec::DivOp_LRepImaginaryOverride(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
	#if defined(AltNum_EnableApproachingValues)
	case RepType::ApproachingTop:
		if (Value.IntValue == 0)
		{
		#if defined(AltNum_EnableImaginaryInfinity)
			if (self.IntValue < 0)//NegativeValue / 0.0..1 = Negative Infinity
				self.IntValue = -1;
			else//PositiveValue / 0.0..1 = Infinity
				self.IntValue = 1;
			self.DecimalHal = InfinityRep;
			self.ExtraRep = IRep;
		#else
			throw "Result is Infinity times i";
			if (self.IntValue < 0)
				self.SetAsMaximum();
			else
				self.SetAsMinimum();
			self.ExtraRep = AltDec::IRep;
		#endif
			return;
		}
		else
		{
			Value.DecimalHalf = 1;
			RRep = RepType::NormalType;
		}
		break;
	#endif
	#if defined(AltNum_EnableApproachingI)
	case RepType::ApproachingImaginaryBottom:
		if (Value.IntValue == 0)
		{
		#if defined(AltNum_EnableInfinityRep)
			if (self.IntValue < 0)//NegativeValue / 0.0..1 = Negative Infinity
				self.IntValue = -1;
			else//PositiveValue / 0.0..1 = Infinity
				self.IntValue = 1;
			self.DecimalHalf = AltDec::InfinityRep;
			self.ExtraRep = 0;
		#else
			throw "Result is Infinity";
			if (self.IntValue < 0)
				self.SetAsMaximum();
			else
				self.SetAsMinimum();
		#endif
			return;
		}
		else
		{
			Value.DecimalHalf = 1;
			RRep = RepType::NormalType;
		}
		break;
	case RepType::ApproachingImaginaryTop:
	#endif
	#if defined(AltNum_EnableFractionals)
	case RepType::NumByDiv:
	#endif
	#if defined(AltNum_EnablePiRep)
	case RepType::PiNum:
		#if defined(AltNum_EnablePiPowers)
	case RepType::PiPower:
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
	case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
		#elif defined(AltNum_EnablePiFractional)
	case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
	#endif
	#if defined(AltNum_EnableERep)
	case RepType::ENum:
		#if defined(AltNum_EnableDecimaledEFractionals)
	case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
		#elif defined(AltNum_EnableEFractional)
	case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		#endif
	#endif
	#if defined(AltNum_EnableApproachingDivided)
	case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
	case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative) 
	#endif
	#if defined(AltNum_EnableNearPi)
	case RepType::NearPi://(Approaching Away from Zero is equal to 0.9999...Pi)
	#endif
	#if defined(AltNum_EnableNearE)
	case RepType::NearE://(Approaching Away from Zero is equal to 0.9999...e)
	#endif
	#if defined(AltNum_EnableMixedFractional)
	case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnableMixedPiFractional)
	case RepType::MixedPi:
		#elif defined(AltNum_EnableMixedEFractional)
	case RepType::MixedE:
		#endif
	#endif
		Value.ConvertToNormType(RRep);
		RRep = RepType::NormalType;
		break;
	default:
		break;
	}
}

inline void BlazesRusCode::AltDec::MultOp_LRepImaginaryOverride(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
	#if !defined(AltNum_EnableComplexNumbers)
	case RepType::NormalType:
	#endif
	case RepType::NumByDiv:
	#if defined(AltNum_EnablePiRep)
	case RepType::PiNum:
		#if defined(AltNum_EnablePiPowers)
	case RepType::PiPower:
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
	case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
		#elif defined(AltNum_EnablePiFractional)
	case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
	#endif

	#if defined(AltNum_EnableERep)
	case RepType::ENum:
		#if defined(AltNum_EnableDecimaledEFractionals)
	case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
		#elif defined(AltNum_EnableEFractional)
	case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		#endif
	#endif

	#if defined(AltNum_EnableApproachingValues)
	case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
	case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
	#endif
	#if defined(AltNum_EnableApproachingDivided)
	case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
	case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative) 
	#endif

	#if defined(AltNum_EnableNearPi)
	case RepType::NearPi://(Approaching Away from Zero is equal to 0.9999...Pi)
	#endif
	#if defined(AltNum_EnableNearE)
	case RepType::NearE://(Approaching Away from Zero is equal to 0.9999...e)
	#endif
	#if defined(AltNum_EnableMixedFractional)
	case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnableMixedPiFractional)
	case RepType::MixedPi:
		#elif defined(AltNum_EnableMixedEFractional)
	case RepType::MixedE:
		#endif
	#endif
		Value.ConvertToNormType(RRep);
		RRep = RepType::NormalType;
		break;
	}
}

inline void BlazesRusCode::AltDec::LRepImaginaryOverridePt2(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
	#if !defined(AltNum_EnableComplexNumbers)
	case RepType::NormalType:
	#endif
	case RepType::NumByDiv:
	#if defined(AltNum_EnablePiRep)
	case RepType::PiNum:
		#if defined(AltNum_EnablePiPowers)
	case RepType::PiPower:
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
	case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
		#elif defined(AltNum_EnablePiFractional)
	case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
	#endif

	#if defined(AltNum_EnableERep)
	case RepType::ENum:
		#if defined(AltNum_EnableDecimaledEFractionals)
	case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
		#elif defined(AltNum_EnableEFractional)
	case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		#endif
	#endif

	#if defined(AltNum_EnableApproachingValues)
	case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
	case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
	#endif
	#if defined(AltNum_EnableApproachingDivided)
	case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealValue if negative)
	case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealValue if negative) 
	#endif

	#if defined(AltNum_EnableNearPi)
	case RepType::NearPi://(Approaching Away from Zero is equal to 0.9999...Pi)
	#endif
	#if defined(AltNum_EnableNearE)
	case RepType::NearE://(Approaching Away from Zero is equal to 0.9999...e)
	#endif
	#if defined(AltNum_EnableMixedFractional)
	case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnableMixedPiFractional)
	case RepType::MixedPi:
		#elif defined(AltNum_EnableMixedEFractional)
	case RepType::MixedE:
		#endif
	#endif
	#if defined(AltNum_EnableComplexNumbers)
		Value.ConvertToNormType(RRep);
		RRep = RepType::NormalType;
	#else
		throw "Complex number operations not enabled";
	#endif
		break;
	default:
		break;
	}
}
#endif