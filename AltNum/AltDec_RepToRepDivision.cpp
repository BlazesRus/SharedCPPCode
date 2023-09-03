#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
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
			self.BasicInt32MultOpV2(Value.ExtraRep);
			break;
		}
		#if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		{
			//X / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(YPi)
			self.BasicInt32MultDivOpV2(Value.DecimalHalf);
			self.BasicUnsignedDivOp(AltDec::AltDec::PiNum * Value.IntValue);//self.BasicUnsignedDivOp(AltDec::PiNumMultByInt(Value.IntValue))
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

        #if defined(AltNum_EnableDecimaledPiOrEFractionals)
			#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
			#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
			#endif
		{
			self.BasicInt32MultOpV2(-Value.ExtraRep);
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
			if (self.IntValue == AltDec::AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue == 0 ? AltDec::AltDec::NegativeRep : -self.IntValue;
			self.ExtraRep = AltDec::IRep;
			break;
		}
		#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
		#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
		#endif
		//Placeholder code(Converting to i rep for now)
		#if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
		{
			Value.ConvertToNormalIRep(RRep);
			self.BasicUnsignedDivOp(Value);
			if (self.IntValue == AltDec::AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue == 0 ? AltDec::AltDec::NegativeRep : -self.IntValue;
			self.ExtraRep = AltDec::IRep;
			break;
		}
		#endif
	#endif
		default:
			Value.ConvertToNormType(RRep);
			self.BasicUnsignedDivOp(Value);
			break;
	}
}

#if defined(AltNum_EnableFractionals)
void NumByDivisorDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            if(Value.DecimalHalf==0)
                self.ExtraRep *= Value.IntValue;
            else
                self.BasicDivOp(Value);
            break;
	#if defined(AltNum_EnableImaginaryNum)
		//ToDo::Add more specific DivOperation code later
		case RepType::INum:
		#if defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#elif defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
		#endif
		#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
		#endif
            //Skip to performing DivOperation as normal type
			self.ConvertToNormType(RepType::NumByDiv);
            NormalDivOp(RRep, self, Value);
            break;
	#endif
        default:
			Value.ConvertToNormType(RRep);
            if(Value.DecimalHalf==0)
                self.ExtraRep *= Value.IntValue;
            else
                self.BasicDivOp(Value);
			break;
    }
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
		case RepType::NormalType:// X*Pi * Y
			self.BasicDivOp(Value);
			break;
	#if defined(AltNum_EnablePiPowers)
		//case RepType::PiPower:
			//	//Add Pi powers code here later
			//	break;
	#endif
		case RepType::NumByDiv://(X*Pi) / (Y / Z) = (XZ)/Y
			self.BasicDivOp(Value);
			self.BasicInt32MultOpV2(Value.ExtraRep);
			break;

	#if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		{
			//(XPi) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(Y)
			self.ExtraRep = 0;
			self.BasicInt32MultDivOpV2(Value.DecimalHalf);
			self.BasicInt32DivOpV2(Value.IntValue);
		}
		break;
	#endif
	#if defined(AltNum_EnableEFractional)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
		{
			self.BasicInt32MultOpV2(Value.DecimalHalf);
			AltDec rVal = AltDec::ENum * Value.IntValue;
			self.BasicDivOp(rVal);
		}
		break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		{
			//(XPi) / (Y.Value*Pi / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value)
			self.ExtraRep = 0;
			int rVal = -Value.ExtraRep;
			self.BasicInt32DivOpV2(rVal);
			self.BasicUnsignedMultOp(Value);
		}
		break;
	#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		{
			//(XPi) / (Y.Value*e / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value*e)
            int rVal = -Value.ExtraRep;
			self.BasicUnsignedMultOp(rVal);
			Value.ConvertToNormType(RepType::ENum);
			self.BasicDivOp(Value);
		}
		break;
	#endif

	#if defined(AltNum_EnableImaginaryNum)
		//Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
		case RepType::INum:
			self.BasicDivOp(Value);
			self.BasicDivOp(AltDec::AltDec::PiNum);
			if(self.IntValue==AltDec::AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue==0?AltDec::AltDec::NegativeRep:-self.IntValue;
			self.ExtraRep = AltDec::IRep;
			break;
		#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
		#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
		#endif
		//Placeholder code(Converting to i rep for now)
		#if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
			Value.ConvertToNormalIRep(RRep);
			self.BasicDivOp(Value);
			self.BasicDivOp(AltDec::PiNum);
			if(self.IntValue==AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue==0?AltDec::NegativeRep:-self.IntValue;
			self.ExtraRep = AltDec::IRep;
			break;
		#endif
	#endif
		default:
			self.CatchAllDivision(Value, RepType::PiNum, RRep);
			break;
    }
}
#endif

#if defined(AltNum_EnablePiPowers)
void PiPowerDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
		case RepType::NormalType:
			self.BasicDivOp(Value);
			break;
		case RepType::PiNum://(IntValue.DecimalHalf)*Pi^-ExtraRep representation
			--self.ExtraRep;
			self.BasicDivOp(Value);
			break;

		#if defined(AltNum_EnableImaginaryNum)
		//Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
		case RepType::INum:
		{
			self.BasicDivOp(Value);
			self.BasicDivOp(PiPowerNum(-self.ExtraRep));
			if (self.IntValue == AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue == 0 ? AltDec::NegativeRep : -self.IntValue;
			self.ExtraRep = IRep;
		}
		break;
			#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			#endif
			//Placeholder code(Converting to i rep for now)
			#if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
		{
			Value.ConvertIRepToNormal(RRep);
			self.BasicDivOp(Value);
			Value = PiPowerNum(-self.ExtraRep);//Reusing Value to store divisor
			self.BasicDivOp(Value);
			if (self.IntValue == AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue == 0 ? AltDec::NegativeRep : -self.IntValue;
			self.ExtraRep = AltDec::IRep;
		}
		break;
			#endif
		#endif
		default:
			self.CatchAllDivision(Value, RepType::PiPower, RRep);
			break;
    }
}
#endif

#if defined(AltNum_EnablePiFractional)
void PiFractionalDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
		case RepType::NormalType:
			if(Value.DecimalHalf==0)
				self.DecimalHalf *= Value.IntValue;
			else
            {
                //Convert to NumByDiv instead
                self.ExtraRep = self.DecimalHalf;
                self.DecimalHalf = 0;
                self.BasicUnsignedMultOp(AltDec::PiNum);
				self.BasicDivOp(Value);
            }
			break;
		#if defined(AltNum_EnableImaginaryNum)
		//ToDo::Add more specific DivOperation code later
		case RepType::INum:
			#if defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			#elif defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			#endif
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			#endif
            //Skip to performing DivOperation as normal type
			self.ConvertToNormType(RepType::PiFractional);
            RepToRepDivOp(RepType::NormalType, RRep, self, Value);
            break;
        #endif
        default:
            self.CatchAllDivision(Value, RepType::PiFractional, RRep);
            break;
    }
}
#endif

#if defined(AltNum_EnableERep)
void ENumDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
		case RepType::NormalType:
			self.BasicDivOp(Value);
			break;
		#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			Value.ConvertPiToNum();
			self.BasicDivOp(Value);
			break;
		#endif
		#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			Value.ConvertPiPowerToNum();
			self.BasicDivOp(Value);
			break;
		#endif

		#if defined(AltNum_EnableAlternativeRepFractionals)
		case RepType::NumByDiv://(X*E) / (Y / Z) = (XZ*E)/Y
			self.BasicDivOp(Value);
			self.BasicInt32MultOpV2(Value.ExtraRep);
			break;

			#if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			//(Xe) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf*e)/(Y)
			self.BasicInt32MultOpV2(Value.DecimalHalf);
			self.BasicUnsignedDiv(AltDec::PiNumValue()*Value.IntValue);
			break;
			#endif
			#if defined(AltNum_EnableEFractional)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			self.ExtraRep = 0;
			self.BasicInt32MultOpV2(Value.DecimalHalf);
			self.BasicInt32DivOpV2(Value.IntValue);
			break;
			#endif

			#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
			self.BasicInt32MultOpV2(-Value.ExtraRep);
			Value.ConvertToNormType(RepType::PiNum);
			self.BasicDivOp(Value);
			break;
			#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
            self.BasicUnsignedIntMult(-Value.ExtraRep);
			self.ExtraRep = 0;
			self.BasicDivOp(Value);
			break;
			#endif
		#endif

		#if defined(AltNum_EnableImaginaryNum)
		//Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
		case RepType::INum:
			self.BasicDivOp(Value);
			self.BasicDivOp(AltDec::ENum);
			if(self.IntValue==AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue==0?AltDec::NegativeRep:-self.IntValue;
				self.ExtraRep = AltDec::IRep;
			break;
			#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			#endif
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			#endif
			//Placeholder code(Converting to i rep for now)
			#if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
			Value.ConvertToNormalIRep(RRep);
			self.BasicDivOp(Value);
			self.BasicDivOp(AltDec::ENum);
			if(self.IntValue==AltDec::NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue==0?AltDec::NegativeRep:-self.IntValue;
				self.ExtraRep = AltDec::IRep;
			break;
			#endif
		#endif
		default:
			self.CatchAllDivision(Value, RepType::ENum, RRep);
			break;
	}
}

	#if defined(AltNum_EnableEFractional)
void EFractionalDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)|| defined(AltNum_EnableDecimaledEFractionals)
void PiOrENumByDivisorDivOp(const RepType& RRep, AltDec& self, AltDec& Value)

#endif

#if defined(AltNum_EnableApproachingValues)

void ApproachingBottomDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
	#if defined(AltNum_EnableImaginaryNum)
		//ToDo::Add more specific DivOperation code later
		case RepType::INum:
		#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
		#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
		#endif
            //Skip to performing DivOperation as normal type
			self.ConvertToNormType(RepType::ApproachingBottom);
            NormalDivOp(RRep, self, Value);
            break;
	#endif
		default:
			self.CatchAllDivision(Value, RepType::ApproachingBottom, RRep);
			break;
    }
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingTopDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
	#if defined(AltNum_EnableImaginaryNum)
		//ToDo::Add more specific DivOperation code later
		case RepType::INum:
		#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
		#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
		#endif
            //Skip to performing DivOperation as normal type
			self.ConvertToNormType(RepType::ApproachingTop);
			NormalDivOp(RRep, self, Value);
            break;
	#endif
		default:
			self.CatchAllDivision(Value, RepType::ApproachingTop, RRep);
			break;
    }
}
	#endif
#endif
#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidLeftDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
	#if defined(AltNum_EnableImaginaryNum)
		//ToDo::Add more specific DivOperation code later
		case RepType::INum:
		#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
		#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
        #endif
		#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
		#endif
            //Skip to performing DivOperation as normal type
			self.ConvertToNormType(RepType::ApproachingMidLeft);
			RepType LRep = RepType::NormalType; RepType rRep = RRep;
            AltDec::RepToRepDivOp(LRep, rRep, self, Value);
            break;
		#endif
		default:
			self.CatchAllDivision(Value, RepType::ApproachingMidLeft, RRep);
			break;
    }
}

    #if !defined(AltNum_DisableApproachingTop)
void ApproachingMidRightDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
		#if defined(AltNum_EnableImaginaryNum)
		//ToDo::Add more specific DivOperation code later
		case RepType::INum:
		#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
		#elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
        #endif
		#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
		#endif
            //Skip to performing DivOperation as normal type
			self.ConvertToNormType(RepType::ApproachingMidRight);
			RepType LRep = RepType::NormalType; RepType rRep = RRep;
            AltDec::RepToRepDivOp(LRep, rRep, self, Value);
            break;
		#endif
		default:
			self.CatchAllDivision(Value, RepType::ApproachingMidRight, RRep);
			break;
    }
}
	#endif
#endif

#if defined(AltNum_EnableImaginaryNum)
void INumDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
				case RepType::NormalType:
					self.BasicDivOp(Value);
					break;
		#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
				case RepType::PiNum:
					Value.ConvertPiToNum();
					self.BasicDivOp(Value);
					break;
		#endif
		#if defined(AltNum_EnableENum)
				case RepType::ENum:
					Value.ConvertToNormType(RepType::PiNum);
					self.BasicDivOp(Value);
					break;
		#endif

		#if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
					//selfNum*i / (Value.IntValue/Value.DecimalHalf*i)
					//selfNum / (Value.IntValue/Value.DecimalHalf)
					//(selfNum*Value.DecimalHalf) / Value.IntValue
					self.BasicInt32MultOpV2(Value.DecimalHalf);
					self.ExtraRep = Value.IntValue;
					break;//Return as NumByDiv
			#if defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv://Value/(-ExtraRep))*i Representation
					//(AltDec(self.IntValue, self.DecimalHalf)i)/(Value/(-Value.ExtraRep))*i)
					//=(AltDec(self.IntValue, self.DecimalHalf))/(Value/(-Value.ExtraRep)))
					//=(AltDec(self.IntValue, self.DecimalHalf)*-Value.ExtraRep)/Value
					self.BasicUnsignedIntDiv(-Value.ExtraRep);
					if(Value.DecimalHalf==0)
					{
						self.ExtraRep = Value.ExtraRep;
					}
					else
					{
						self.ExtraRep = 0;
						self.BasicDivOp(Value);
					}
					break;
			#endif
		#endif
				default:
					Value.ConvertToNormType(RRep);
					self.BasicDivOp(Value);
					break;
    }
}

	#if defined(AltNum_EnableDecimaledIFractionals)
void INumByDivisorDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableIFractional)
void IFractionalDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingI)
void ApproachingImaginaryBottomDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryTDivOpDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingImaginaryMidLeftDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	    #if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryMidRightDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	    #endif
    #endif
#endif

#if defined(AltNum_EnableMixedFractional)
void MixedFracDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#if defined(AltNum_EnableMixedPiFractional)
void MixedPiDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableMixedEFractional)
void MixedEDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableMixedIFractional)
void MixedIDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
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