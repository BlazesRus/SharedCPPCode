#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

#if defined(AltNum_EnableFractionals)
void NumByDivMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnablePiPowers)
void PiPowerMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	
	#if defined(AltNum_EnableDecimaledPiFractionals)
void PiNumByDivMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnablePiFractional)
void PiFractionalMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableERep)
void ENumMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableEFractional)
void EFractionalMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingValues)
void ApproachingBottomMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTMultOp)
void ApproachingTMultOpMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidLeftMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

		#if !defined(AltNum_DisableApproachingTMultOp)
void ApproachingMidRightMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
		#endif
	#endif
#endif

#if defined(AltNum_EnableImaginaryNum)
void INumMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnableDecimaledIFractionals)
void INumByDivMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableIFractional)
void IFractionalMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingI)
void ApproachingImaginaryBottomMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTMultOp)
void ApproachingImaginaryTMultOpMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingImaginaryMidLeftMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	    #if !defined(AltNum_DisableApproachingTMultOp)
void ApproachingImaginaryMidRightMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	    #endif
    #endif
#endif

#if defined(AltNum_EnableMixedFractional)
void MixedFracMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#if defined(AltNum_EnableMixedPiFractional)
void MixedPiMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableMixedEFractional)
void MixedEMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableMixedIFractional)
void MixedIMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

inline void BlazesRusCode::AltDec::RepToRepMultOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
    //LRep Overrides
#if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)
    switch(LRep)
    {
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        case RepType::UndefinedButInRange:
			throw "UndefinedButInRange MultOperations not supported yet(from left side)"; return;
            break;
    #endif
	#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
		#endif
        #if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
        #endif
            BlazesRusCode::AltDec::MultOp_LRepImaginaryOverride(LRep, RRep, self, Value);
			break;
    #endif
		default:
			break;
	}
#endif
    //RRep Overrides before Main RepToRep Code
    switch(RRep)
    {
	#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
				if(Value.IntValue==0)
				{
					self.IntValue = self.IntValue<0?NegativeRep:0;
					self.DecimalHalf = ApproachingBottomRep;
					self.ExtraRep = 0;
					return;
				}
				else
				{
					Value.DecimalHalf = 1;
					RRep = RepType::NormalType;
				}
			break;

		case RepType::ApproachingTMultOp:
			Value.DecimalHalf = 999999999;
			Value.ExtraRep = 0;
			RRep = RepType::NormalType;
			break;
		#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			Value.ConvertToNormType(RepType::ApproachingMidLeft);
			RRep = RepType::NormalType;
			break;
		case RepType::ApproachingMidRight:
			Value.ConvertToNormType(RepType::ApproachingMidRight);
			RRep = RepType::NormalType;
			break;
		#endif
	#endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        case RepType::UndefinedButInRange:
			throw "UndefinedButInRange MultOperations not supported yet(from right side)";
            break;
    #endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform MultOperations with NaN or Undefined number";
			break;
	#endif
		default:
			break;
	}
    switch (LRep)//Main switch block starts here
    {
		case RepType::NormalType:
			NormalMultOp(RRep, self, Value); break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			PiNumMultOp(RRep, self, Value); break;
    #if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			PiPowerMultOp(RRep, self, Value); break;
    #endif
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
			ENumMultOp(RRep, self, Value); break;
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			ApproachingBottomMultOp(RRep, self, Value); break;
	#if defined(AltNum_DisableApproachingTMultOp)
		case RepType::ApproachingTMultOp:
			ApproachingTMultOpMultOp(RRep, self, Value); break;
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			ApproachingMidLeftMultOp(RRep, self, Value); break;
		#if defined(AltNum_DisableApproachingTMultOp)
		case RepType::ApproachingMidRight:
			ApproachingMidRightMultOp(RRep, self, Value); break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
			NumByDivMultOp(RRep, self, Value); break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			PiFractionalMultOp(RRep, self, Value); break;
		#endif
		#if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			EFractionalMultOp(RRep, self, Value); break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
			PiNumByDivMultOp(RRep, self, Value); break;
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
			ENumByDivMultOp(RRep, self, Value); break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			INumMultOp(RRep, self, Value); break;
    #if defined(AltNum_EnableFractionals)
        #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			INumByDivMultOp(RRep, self, Value); break;
        #else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			IFractionalMultOp(RRep, self, Value); break;
        #endif
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
			EFractionalMultOp(RRep, self, Value); break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
			MixedPiMultOp(RRep, self, Value); break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			MixedEMultOp(RRep, self, Value); break;
	#elif defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			MixedIMultOp(RRep, self, Value); break;
    #endif
#endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform MultOperations with NaN or Undefined number";
			break;
	#endif
		default:
			throw AltDec::RepTypeAsString(LRep)+" RepType multiplication with"+AltDec::RepTypeAsString(RRep)+"not supported yet";
			break;
	}
}