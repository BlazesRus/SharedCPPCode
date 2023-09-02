#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
	case RepType::NormalType://Fail safe for when converted before switch
		self.BasicUnsignedDivOp(Value);
		break;
		#if defined(AltNum_EnableFractionals)
	case RepType::NumByDiv://X / (Y / Z) = (XZ)/Y
	{
		self.BasicUnsignedDivOp(Value);
		self.BasicInt32MultOp(Value.ExtraRep);
		break;
	}
		#if defined(AltNum_EnablePiFractional)
	case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
	{
		//X / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(YPi)
		self.BasicInt32MultOpV2(Value.DecimalHalf);
		self.BasicUnsignedDivOp(AltDec::PiNum * Value.IntValue);//self.BasicUnsignedDivOp(PiNumMultByInt(Value.IntValue))
		break;
	}
		#endif
		#if defined(AltNum_EnableEFractional)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		{
			self.BasicInt32MultOpV2(Value.DecimalHalf);
			self.BasicUnsignedDivOp(AltDec::ENum * Value.IntValue);
			break;
		}
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiOrEFractionals)
		{
			self.BasicInt32MultOp(-Value.ExtraRep);
			#if defined(AltNum_EnableDecimaledPiFractionals)
			Value.ConvertToNormType(RepType::PiNum);
			#elif defined(AltNum_EnableDecimaledEFractionals)
			Value.ConvertToNormType(RepType::ENum);
			#endif
			self.BasicUnsignedDivOp(Value);
			break;
		}
		#endif
	#endif

	#if defined(AltNum_EnableImaginaryNum)
	//Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
		case RepType::INum:
		{
			self.BasicUnsignedDivOp(Value);
			if (self.IntValue == AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue == 0 ? AltDec::NegativeRep : -self.IntValue;
			self.ExtraRep = AltDec::IRep;
			break;
		}
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
		//Placeholder code(Converting to i rep for now)
		#if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
		{
			Value.ConvertToNormalIRep(RRep);
			self.BasicUnsignedDivOp(Value);
			if (self.IntValue == AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue == 0 ? AltDec::NegativeRep : -self.IntValue;
			self.ExtraRep = AltDec::IRep;
			break;
		}
		#endif
	#endif*/
		default:
			Value.ConvertToNormType(RRep);
			self.BasicUnsignedDivOp(Value);
			break;
	}
}

#if defined(AltNum_EnableFractionals)
void NumByDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnablePiPowers)
void PiPowerOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	
	#if defined(AltNum_EnableDecimaledPiFractionals)
void PiNumByDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnablePiFractional)
void PiFractionalOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableERep)
void ENumOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableEFractional)
void EFractionalOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingValues)
void ApproachingBottomOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingTopOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidLeftOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

		#if !defined(AltNum_DisableApproachingTop)
void ApproachingMidRightOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
		#endif
	#endif
#endif

#if defined(AltNum_EnableImaginaryNum)
void INumOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnableDecimaledIFractionals)
void INumByDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableIFractional)
void IFractionalOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingI)
void ApproachingImaginaryBottomOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryTopOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingImaginaryMidLeftOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	    #if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryMidRightOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	    #endif
    #endif
#endif

#if defined(AltNum_EnableMixedFractional)
void MixedFracOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#if defined(AltNum_EnableMixedPiFractional)
void MixedPiOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableMixedEFractional)
void MixedEOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableMixedIFractional)
void MixedIOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

inline void BlazesRusCode::AltDec::RepToRepDivOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
    //LRep Overrides
#if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)
    switch(LRep)
    {
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        case RepType::UndefinedButInRange:
			throw "UndefinedButInRange operations not supported yet(from left side)"; return;
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
            BlazesRusCode::AltDec::DivOp_LRepImaginaryOverride(LRep, RRep, self, Value);
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
            {//(For positive left side values)Techically returns self.IntValue + 0.0..self.IntValue
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
			throw "UndefinedButInRange operations not supported yet(from right side)";
            break;
    #endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform operations with NaN or Undefined number";
			break;
	#endif
		default:
			break;
	}
    switch (LRep)//Main switch block starts here
    {
		case RepType::NormalType:
			NormalOp(RRep, self, Value); break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			PiNumOp(RRep, self, Value); break;
    #if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			PiPowerOp(RRep, self, Value); break;
    #endif
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
			ENumOp(RRep, self, Value); break;
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			ApproachingBottomOp(RRep, self, Value); break;
	#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
			ApproachingTopOp(RRep, self, Value); break;
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			ApproachingMidLeftOp(RRep, self, Value); break;
		#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingMidRight:
			ApproachingMidRightOp(RRep, self, Value); break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
			NumByDivOp(RRep, self, Value); break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			PiFractionalOp(RRep, self, Value); break;
		#endif
		#if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			EFractionalOp(RRep, self, Value); break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
			PiNumByDivOp(RRep, self, Value); break;
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
			ENumByDivOp(RRep, self, Value); break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			INumOp(RRep, self, Value); break;
    #if defined(AltNum_EnableFractionals)
        #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			INumByDivOp(RRep, self, Value); break;
        #else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			IFractionalOp(RRep, self, Value); break;
        #endif
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
			EFractionalOp(RRep, self, Value); break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
			MixedPiOp(RRep, self, Value); break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			MixedEOp(RRep, self, Value); break;
	#elif defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			MixedIOp(RRep, self, Value); break;
    #endif
#endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform operations with NaN or Undefined number";
			break;
	#endif
		default:
			throw AltDec::RepTypeAsString(LRep)+" RepType division with"+AltDec::RepTypeAsString(RRep)+"not supported yet";
			break;
	}
}