#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

void NormalMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

#if defined(AltNum_EnableFractionals)
void NumByDivMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnablePiPowers)
void PiPowerMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	
	#if defined(AltNum_EnableDecimaledPiFractionals)
void PiNumByDivMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnablePiFractional)
void PiFractionalMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableERep)
void ENumMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableEFractional)
void EFractionalMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingValues)
void ApproachingBottomMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingTopMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidRightMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

		#if !defined(AltNum_DisableApproachingTop)
void ApproachingMidLeftMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
		#endif
	#endif
#endif

#if defined(AltNum_EnableImaginaryNum)
void INumMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnableDecimaledIFractionals)
void INumByDivMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableIFractional)
void IFractionalMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	
	#if defined(AltNum_EnableApproachingI)
void ApproachingImaginaryBottomMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

		#if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryTopMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
		#endif
		#if defined(AltNum_EnableApproachingDivided)
void ApproachingImaginaryMidRightMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

			#if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryMidLeftMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
			#endif
		#endif
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
void MixedFracMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
void MixedPiEMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableMixedIFractional)
void MixedIMultOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

inline void BlazesRusCode::AltDec::RepToRepMultOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
    //LRep Overrides
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
		case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
		#endif
        #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedI:
        #endif
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
			#if defined(AltNum_EnableAlternativeRepFractionals)
				#if defined(AltNum_EnableDecimaledPiFractionals)
			case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
				#else
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
				#endif
			#endif
		#endif

		#if defined(AltNum_EnableERep)
			case RepType::ENum:
			#if defined(AltNum_EnableAlternativeRepFractionals)
				#if defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
				#else
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
				#endif
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
			break;
    #endif
		default://No nothing for most of them
			break;
	}
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
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
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
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
    #if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
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
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
    #endif
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
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
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
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
			break;
	#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
			switch (RRep)
			{
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			switch (RRep)
			{
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
		#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingMidRight:
			switch (RRep)
			{
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
			switch (RRep)
			{
				case RepType::NormalType://Later normalize fractional when integer when viable
					self.BasicMultOp(Value);
					break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
				case RepType::PiNum:
		#if defined(AltNum_EnableDecimaledPiFractionals)
					self.BasicMultOp(Value);
					self.ExtraRep *= -1;
		#else
					self.CatchAllMultiplication(Value, LRep, RRep);
		#endif
					break;
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::ENum:
		#if defined(AltNum_EnableDecimaledEFractionals)
					self.BasicMultOp(Value);
					self.ExtraRep *= -1;
		#else
					self.CatchAllMultiplication(Value, LRep, RRep);
		#endif
					break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
		#if defined(AltNum_EnableDecimaledIFractionals)
					self.BasicMultOp(Value);
					self.ExtraRep *= -1;
		#else
					//ToDo::Add code here
		#endif
					break;
	#endif
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			switch (RRep)
			{
				case RepType::NormalType:
					if(Value.DecimalHalf==0)
						self.IntValue *= Value.IntValue;
					else
						self.CatchAllMultiplication(Value, LRep, RRep);
					break;
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
		#endif
		#if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			switch (RRep)
			{
				case RepType::NormalType:
					if(Value.DecimalHalf==0)
						self.IntValue *= Value.IntValue;
					else
						self.CatchAllMultiplication(Value, LRep, RRep);
					break;
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)//Only one of the 2 switches can be active at once
			switch (RRep)
			{
				case RepType::NormalType://Normalize denom later
					self.BasicMultOp(Value);
					break;
			#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					self.BasicMultOp(Value);
				#if defined(AltNum_EnableDecimaledPiFractionals)
					self.ConvertToNormType(RepType::PiNumByDiv);
				#else
					self.ConvertToNormType(RepType::ENumByDiv);
				#endif
					self.ExtraRep = IRep;
					break;
			#endif							
				default:
					Value.ConvertToNormType(RRep);
					self.BasicMultOp(Value);
					break;
			}
			break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			switch (RRep)
			{
				case RepType::NormalType:
					self.BasicMultOp(Value);
					break;
		        case RepType::INum:
                    self.BasicMultOp(Value);
                    self.ExtraRep = 0;
                    break;
				default:
					Value.ConvertToNormType(RRep);
					self.BasicMultOp(Value);
					break;
			}
			break;
    #if defined(AltNum_EnableFractionals)
        #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
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
					Value.ConvertToNormType(RRep);
					self.BasicMultOp(Value);
			}
			break;
        #else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			switch (RRep)
			{
				case RepType::NormalType:
					self.BasicMultOp(Value);
					break;
				case RepType::INum://Convert result into NumByDiv
					self.ExtraRep = self.DecimalHalf;
					self.DecimalHalf = 0;
					self.BasicMultOp(-Value);
					break;
				default:
					Value.ConvertToNormType(RRep);
					self.BasicMultOp(Value);
			}
			break;
        #endif
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
			switch (RRep)
			{
	#if defined(AltNum_EnableMixedPiFractional)
				//case RepType::MixedPi:
	#else
				//case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
					//MixedFracMultOp(LRep, RRep, self, Value);
					//break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)

	#endif
	#if defined(AltNum_EnableMixedIFractional)
				//case RepType::MixedI:
					//MixedFracMultOp(LRep, RRep, self, Value);
					//break;
	#endif
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional)
			switch (RRep)
			{
                case RepType::NormalType:
                    if(Value.DecimalHalf==0)
                    {
                        self.IntValue *= Value.IntValue;
                        self.DecimalHalf *= Value.IntValue;
                        if(self.DecimalHalf <= self.ExtraRep)
                        {//(-4 - -3)*1 = -1;
                            int divRes = self.DecimalHalf / self.ExtraRep;
                            int C = self.DecimalHalf - self.ExtraRep * divRes;
                            if(C==0)//If no longer a mixed fraction so instead convert into PiNum/ENum
                            {
                                self.DecimalHalf = 0;
        #if defined(AltNum_EnableMixedPiFractional)
                                self.ExtraRep = PiRep;
        #else
                                self.ExtraRep = ERep;
        #endif
                                if(self.IntValue<0)
                                    self.IntValue -= divRes;
                                else
                                    self.IntValue += divRes;
                            }
                            else
                            {
                                throw "ToDo:Finish this code later";
                            }
                        }
                    }
                    else//Convert result as NumByDiv
                    {
                        throw "ToDo:Finish this code later";
                    }
					break;
				default:
                    self.ConvertToNormType(LRep);
					Value.ConvertToNormType(RRep);
					self.BasicDivOp(Value);
			}
			break;
    #endif
	#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			switch (RRep)
			{
                case RepType::INum:
        #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
        #elif defined(AltNum_EnableFractionals)
                case RepType::IFractional:
        #endif
                    self.CatchAllImaginaryMultiplication(Value, LRep, RRep);
				default:
                    self.ConvertToNormalIRep(LRep);
					Value.ConvertToNormType(RRep);
					self.BasicDivOp(Value);
            }
            break;
    #endif
#endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform operations with NaN or Undefined number";
			break;
	#endif
		default:
			throw AltDec::RepTypeAsString(LRep)+" RepType multiplication with"+AltDec::RepTypeAsString(RRep)+"not supported yet";
			break;
	}
}