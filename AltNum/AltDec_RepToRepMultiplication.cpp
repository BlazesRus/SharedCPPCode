#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
			switch (RRep)
			{
				case RepType::NormalType://Fail safe for when converted before switch
					self.BasicMultOp(Value);
					break;
	#if defined(AltNum_EnablePiRep)
				case RepType::PiNum://X * (Y*Pi)
					self.BasicMultOp(Value);
					self.ExtraRep = PiRep;
					break;
		#if defined(AltNum_EnablePiPowers)
				case RepType::PiPower://X * (Y*Pi)^-ExtraRep
					self.ExtraRep = Value.ExtraRep-1;
					self.BasicMultOp(Value);
					break;
		#endif
	#endif
    #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
        #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
        #endif
					MixedFracMultOp(LRep, RRep, self, Value);
					break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					self.BasicMultOp(Value);
                    self.ExtraRep = IRep;
                    break;
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
					Value.ConvertToNormalIRep(RRep);
					self.BasicMultOp(Value);
                    self.ExtraRep = IRep;
        #endif
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::NormalType, RRep);
            break;
    }
}

#if defined(AltNum_EnableFractionals)
void NumByDivisorMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
			switch (RRep)
			{
				case RepType::NormalType:
					self.BasicMultOp(Value);
					break;
	#if defined(AltNum_EnablePiPowers)
				case RepType::PiPower://XPi * (Y*Pi)^-ExtraRep
					self.ExtraRep = Value.ExtraRep-1;
					self.BasicMultOp(Value);
					break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					self.BasicMultOp(Value);
					self.ExtraRep = IRep;
					self.BasicMultOp(PiNum);
					break;
	#endif
	#if defined(AltNum_EnableFractionals)
				case RepType::NumByDiv:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)//Convert result to PiNumByDiv
					self.ExtraRep = Value.ExtraRep;
					self.BasicMultOp(Value);
			#else
				if(self.DecimalHalf==0&&Value.DecimalHalf==0)//If both left and right side values are whole numbers, convert result into PiFractional
				{//Becoming IntValue/DecimalHalf*Pi Representation
					self.IntValue *= Value.IntValue;
					self.DecimalHalf = Value.ExtraRep;
					self.ExtraRep = PiByDivisorRep;
				}
				else
				{
					self /= Value.ExtraRep;
					self.BasicMultOp(Value);
				}
			#endif
		#else//Else just keep as PiNum type
					self /= Value.ExtraRep;
					self.BasicMultOp(Value);
		#endif
					break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					self.BasicMultOp(Value);
					self.ConvertPiToNum();
					self.ExtraRep = IRep;
					break;
	#endif
    #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                    MixedFracRtRMult_WithNormal(self, Value);
                    break;
        #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
        #endif
					MixedAltFracRtRMult_WithPi(self, Value);
					break;
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::PiNum, RRep);
            break;
    }
}
#endif
#if defined(AltNum_EnablePiPowers)
void PiPowerMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            self.BasicMultOp(Value);
            break;
        case RepType::PiNum://(IntValue.DecimalHalf)*Pi^-ExtraRep representation
            --self.ExtraRep;
            self.BasicMultOp(Value);
            break;
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
            MixedFracRtRMult_WithNormal(self, Value);
            break;
        #if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
        #endif
			MixedAltFracRtRMult_WithPiPower(self, Value);
			break;
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::PiPower, RRep);
            break;
    }
}
#endif
#if defined(AltNum_EnablePiFractional)
void PiFractionalMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            if(Value.DecimalHalf==0)
                self.IntValue *= Value.IntValue;
            else
                self.CatchAllMultiplication(Value, LRep, RRep);
            break;
        default:
            self.CatchAllMultiplication(Value, RepType::PiFractional, RRep);
            break;
    }
}
#endif

#if defined(AltNum_EnableERep)
void ENumMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType://Fail safe for when converted before switch
            self.BasicMultOp(Value);
            break;
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum://Xe * (Y*Pi)
					Value.ConvertPiToNum();
					self.BasicMultOp(Value);
					break;
		#if defined(AltNum_EnablePiPowers)
        case RepType::PiPower://Xe * (Y*Pi)^-ExtraRep
					AltDec Res = Value;
					Res.BasicMultOp(self);
					Res.BasicMultOp(ENum);
					self.SetVal(Res);
					break;
		#endif
	#endif
	#if defined(AltNum_EnableFractionals)
        case RepType::NumByDiv:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledEFractionals)//Convert result to PiNumByDiv
					self.ExtraRep = Value.ExtraRep;
					self.BasicMultOp(Value);
			#else
				if(self.DecimalHalf==0&&Value.DecimalHalf==0)//If both left and right side values are whole numbers, convert result into EFractional
				{//Becoming IntValue/DecimalHalf*e Representation
					self.IntValue *= Value.IntValue;
					self.DecimalHalf = Value.ExtraRep;
					self.ExtraRep = EByDivisorRep;
				}
				else
				{
					self /= Value.ExtraRep;
					self.BasicMultOp(Value);
				}
			#endif
		#else//Else just keep as ENum type
					self /= Value.ExtraRep;
					self.BasicMultOp(Value);
		#endif
					break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					self.BasicMultOp(Value);
					self.ConvertEToNum();
					self.ExtraRep = IRep;
					break;
	#endif
    #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                    MixedFracRtRMult_WithNormal(self, Value);
                    break;
        #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
        #endif
					MixedAltFracRtRMult_WithNormal(self, Value);
					break;
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::ENum, RRep);
            break;
    }
}
#endif
#if defined(AltNum_EnableEFractional)
void EFractionalMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)|| defined(AltNum_EnableDecimaledEFractionals)
void PiOrENumByDivisorDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{

}
#endif

#if defined(AltNum_EnableApproachingValues)
void ApproachingBottomMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					if(self.IntValue=0||self.IntValue==NegativeRep)
					{
						self.IntValue = self.IntValue==0?NegativeRep:0;
	#if defined(AltNum_EnableImaginaryInfinity)
						self.DecimalHalf = ApproachingImaginaryBottomRep;
	#else
						self.DecimalHalf = 1;
	#endif
						self.ExtraRep = IRep;
					}
					else
					{
						self.DecimalHalf = 1;
						self.BasicMultOp(Value);
						self.ExtraRep = IRep;
					}
					break;
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
					if(self.IntValue=0||self.IntValue==NegativeRep)
					{
						self.IntValue = self.IntValue==0?NegativeRep:0;
	#if defined(AltNum_EnableImaginaryInfinity)
						self.DecimalHalf = ApproachingImaginaryBottomRep;
	#else
						self.DecimalHalf = 1;
	#endif
						self.ExtraRep = IRep;
					}
					else
					{
						Value.ConvertToNormalIRep(RRep);
						self.DecimalHalf = 1;
						self.BasicMultOp(Value);
						self.ExtraRep = IRep;
					}
					break;
	#endif
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
    }
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingTopMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    //switch (RRep)
    //{
    //  default:
            self.CatchAllMultiplication(Value, RepType::ApproachingTop, RRep);
    //      break;
    //}
}
	#endif
#endif

#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidLeftMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingMidRightMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif


#if defined(AltNum_EnableImaginaryNum)
void INumMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnableDecimaledIFractionals)
void INumByDivisorMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            self.BasicMultOp(Value);
            break;
        case RepType::INum://Convert result into NumByDiv
            self.BasicMultOp(-Value);
            self.ExtraRep = -self.ExtraRep;
            break;
        default:
            Value.ConvertToNormType(&RRep);
            self.BasicMultOp(Value);
    }
}
#elif defined(AltNum_EnableIFractional)
void IFractionalMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnableApproachingI)
void ApproachingImaginaryBottomMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryTMultOpMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingImaginaryMidLeftMultOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	    #if !defined(AltNum_DisableApproachingTop)
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

		case RepType::ApproachingTop:
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
	#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
            self.CatchAllMultiplication(Value, RepType::ApproachingTop, RRep); break;
			//ApproachingTopMultOp(RRep, self, Value); break;
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			ApproachingMidLeftMultOp(RRep, self, Value); break;
		#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingMidRight:
			ApproachingMidRightMultOp(RRep, self, Value); break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
			NumByDivisorMultOp(RRep, self, Value); break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			PiFractionalMultOp(RRep, self, Value); break;
		#endif
		#if defined(AltNum_EnableEFractional)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			EFractionalMultOp(RRep, self, Value); break;
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
			INumMultOp(RRep, self, Value); break;
    #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			INumByDivisorMultOp(RRep, self, Value); break;
    #elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			IFractionalMultOp(RRep, self, Value); break;
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