#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

#if defined(AltNum_EnableFractionals)
#endif

#if defined(AltNum_EnablePiRep)
#endif

#if defined(AltNum_EnablePiPowers)
#endif

#if defined(AltNum_EnableERep)
#endif

#if defined(AltNum_EnablePiFractional)
#endif

#if defined(AltNum_EnableEFractional)
#endif

#if defined(AltNum_EnableDecimaledPiOrEFractionals)
#endif

#if defined(AltNum_EnableApproachingValues)

#if !defined(AltNum_DisableApproachingTop)
#endif
#endif
#if defined(AltNum_EnableApproachingDivided)
#if !defined(AltNum_DisableApproachingTop)
#endif
#endif

#if defined(AltNum_EnableImaginaryNum)
#endif

#if defined(AltNum_EnableApproachingI)
#if !defined(AltNum_DisableApproachingTop)
#endif
	#if defined(AltNum_EnableApproachingDivided)
#if !defined(AltNum_DisableApproachingTop)
#endif
    #endif
#endif

#if defined(AltNum_EnableMixedFractional)
#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
#elif defined(AltNum_EnableMixedIFractional)
#endif
#endif

inline void BlazesRusCode::AltDec::RepToRepDivision(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
    //LRep Overrides
#if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)
    switch(LRep)
    {
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        case RepType::UndefinedButInRange:
			throw "UndefinedButInRange DivOperations not supported yet(from left side)"; return;
            break;
    #endif
	#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			#else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			#endif
		#endif
        #if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
        #endif
            BlazesRusCode::AltDec::Division_LRepImaginaryOverride(RRep, self, Value);
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
            //Any real number / 0.0..1 = Infinity
            if(Value.IntValue==0)
            {
				self.DecimalHalf = InfinityRep;
                if(self.IntValue<0)
                    self.IntValue = -1;
                else
                    self.IntValue = 1;
				self.ExtraRep = 0;
                return;
            }
            else
            {
                Value.DecimalHalf = 1;
                RRep = RepType::NormalType;
            }
            break;

		case RepType::ApproachingTop:
		{
            if(Value.IntValue==0&&LRep==RepType::NormalType)//1/0.9..9 = 1.0..1
            {//(For positive left side values);Technically should return self.IntValue + 0.0..self.IntValue
				self.DecimalHalf = ApproachingBottomRep;
				self.ExtraRep = 0;
                return;
            }
            else
            {
                Value.DecimalHalf = 999999999;
                Value.ExtraRep = 0;
                RRep = RepType::NormalType;
            }
			break;
        }
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
			throw "UndefinedButInRange DivOperations not supported yet(from right side)";
            break;
    #endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform DivOperations with NaN or Undefined number";
			break;
	#endif
		default:
			break;
	}
    switch (LRep)//Main switch block starts here
    {
		case RepType::NormalType:
			NormalDivOp(RRep, self, Value); break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			PiNumDivOp(RRep, self, Value); break;
    #if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			PiPowerDivOp(RRep, self, Value); break;
    #endif
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
			ENumDivOp(RRep, self, Value); break;
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			ApproachingBottomDivOp(RRep, self, Value); break;
	#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTDivOp:
			ApproachingTopDivOp(RRep, self, Value); break;
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			ApproachingMidLeftDivOp(RRep, self, Value); break;
		#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingMidRight:
			ApproachingMidRightDivOp(RRep, self, Value); break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
			NumByDivisorDivOp(RRep, self, Value); break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			PiFractionalDivOp(RRep, self, Value); break;
		#endif
		#if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			EFractionalDivOp(RRep, self, Value); break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
			PiOrENumByDivisorDivOp(RRep, self, Value); break;
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
			PiOrENumByDivisorDivOp(RRep, self, Value); break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			INumDivOp(RRep, self, Value); break;
    #if defined(AltNum_EnableFractionals)
        #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			INumByDivisorDivOp(RRep, self, Value); break;
        #else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			IFractionalDivOp(RRep, self, Value); break;
        #endif
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
			EFractionalDivOp(RRep, self, Value); break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
			MixedPiDivOp(RRep, self, Value); break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			MixedEDivOp(RRep, self, Value); break;
	#elif defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			MixedIDivOp(RRep, self, Value); break;
    #endif
#endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform DivOperations with NaN or Undefined number";
			break;
	#endif
		default:
			throw AltDec::RepTypeAsString(LRep)+" RepType division with"+AltDec::RepTypeAsString(RRep)+"not supported yet";
			break;
	}
}