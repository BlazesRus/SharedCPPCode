#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
		case RepType::NormalType://Fail safe for when converted before switch
			self.BasicDivOp(Value);
			break;
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv://X / (Y / Z) = (XZ)/Y
			self.BasicDivOp(Value);
			self.BasicIntMultOp(Value.ExtraRep);
			break;
					
#if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			//X / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(YPi)
			self.BasicIntMultOp(Value.DecimalHalf);
			self.BasicDivOp(PiNumValue()*Value.IntValue);
			break;
#endif
#if defined(AltNum_EnableEFractional)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			self.BasicIntMultOp(Value.DecimalHalf);
			self.BasicDivOp(ENumValue()*Value.IntValue);
			break;
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
#endif
#if defined(AltNum_EnableDecimaledPiOrEFractionals)
			self.BasicMult(-Value.ExtraRep);
	#if defined(AltNum_EnableDecimaledPiFractionals)
			Value.ConvertToNormType(RepType::PiNum);
	#elif defined(AltNum_EnableDecimaledEFractionals)
			Value.ConvertToNormType(RepType::ENum);
	#endif
			self.BasicDivOp(Value);
			break;
#endif
#endif					

#if defined(AltNum_EnableImaginaryNum)
//Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
		case RepType::INum:
			self.BasicDivOp(Value);
			if(self.IntValue==NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
			break;
			self.ExtraRep = IRep;
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
			self.BasicDivOp(Value);
			if(self.IntValue==NegativeRep)
				self.IntValue = 0;
			else
				self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
			break;
			self.ExtraRep = IRep;
			break;
#endif
#endif
		default:
			Value.ConvertToNormType(&RRep);
			self.BasicDivOp(Value);
			break;
		}
		break;
		
	}
}

#if defined(AltNum_EnableFractionals)
void NumByDivDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnablePiPowers)
void PiPowerDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	
	#if defined(AltNum_EnableDecimaledPiFractionals)
void PiNumByDivDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnablePiFractional)
void PiFractionalDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableERep)
void ENumDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableEFractional)
void EFractionalDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingValues)
void ApproachingBottomDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingTopDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidRightDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

		#if !defined(AltNum_DisableApproachingTop)
void ApproachingMidLeftDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
		#endif
	#endif
#endif

#if defined(AltNum_EnableImaginaryNum)
void INumDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if defined(AltNum_EnableDecimaledIFractionals)
void INumByDivDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableIFractional)
void IFractionalDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	
	#if defined(AltNum_EnableApproachingI)
void ApproachingImaginaryBottomDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

		#if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryTopDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
		#endif
		#if defined(AltNum_EnableApproachingDivided)
void ApproachingImaginaryMidRightDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

			#if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryMidLeftDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
			#endif
		#endif
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
void MixedFracDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#if defined(AltNum_MixedPiOrEEnabled)
void MixedPiESubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableMixedIFractional)
void MixedIDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

inline void BlazesRusCode::AltDec::RepToRepDivOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
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
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			#else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			#endif
		#endif
        #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedI:
        #endif
			switch (RRep)
			{
		#if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingTop:
                if(Value.IntValue==0)
                {
            #if defined(AltNum_EnableImaginaryInfinity)
                    if(self.IntValue<0)//NegativeValue / 0.0..1 = Negative Infinity
                        self.IntValue = -1;
                    else//PositiveValue / 0.0..1 = Infinity
                        self.IntValue = 1;
                    self.DecimalHal = InfinityRep;
                    self.ExtraRep = IRep;
            #else
                    throw "Result is Infinity times i";
                    if(self.IntValue<0)
                        self.SetAsMaximum();
                    else
                        self.SetAsMinimum();
                    self.ExtraRep = IRep;
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
                if(Value.IntValue==0)
                {
            #if defined(AltNum_EnableInfinityRep)
                    if(self.IntValue<0)//NegativeValue / 0.0..1 = Negative Infinity
                        self.IntValue = -1;
                    else//PositiveValue / 0.0..1 = Infinity
                        self.IntValue = 1;
                    self.DecimalHal = InfinityRep;
                    self.ExtraRep = 0;
            #else
                    throw "Result is Infinity";
                    if(self.IntValue<0)
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
				Value.ConvertToNormType(&RRep);
				RRep = RepType::NormalType;
				break;
            default:
                break;
            }
            break;

    #endif
		default:
			break;
	}
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
            if(LRep==RepType::NormalType&&Value.IntValue==0)//1/0.9..9 = 1.0..1
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
        
        #if defined(AltNum_EnableApproachingDivided)
        		case RepType::ApproachingBottomDiv:
                    Value.ConvertToNormType(RepType::ApproachingBottomDiv);
                    RRep = RepType::NormalType;
        			break;
        		case RepType::ApproachingTopDiv:
                    Value.ConvertToNormType(RepType::ApproachingTopDiv);
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
			NormalDivOp(RRep, self, Value); 
			break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
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
					self.BasicIntMultOp(Value.ExtraRep);
					break;
							
		#if defined(AltNum_EnablePiFractional)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					//(XPi) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(Y)
					self.ExtraRep = 0;
					self.BasicMultOp(Value.DecimalHalf);
					self.BasicDivOp(Value.IntValue);
					break;
		#endif
		#if defined(AltNum_EnableEFractional)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					self.BasicMultOp(Value.DecimalHalf);
					self.BasicDivOp(ENumValue()*Value.IntValue);
					break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
					//(XPi) / (Y.Value*Pi / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value)
					self.ExtraRep = 0;
					self.BasicDivOp(-Value.ExtraRep);
					self.BasicMultOp(Value);
					break;
		#elif defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
					//(XPi) / (Y.Value*e / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value*e)
					self.BasicMultOp(-Value.ExtraRep);
					Value.ConvertToNormType(RepType::ENum);
					self.BasicDivOp(Value);
					break;
		#endif

		#if defined(AltNum_EnableImaginaryNum)
		//Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
				case RepType::INum:
					self.BasicDivOp(Value);
					self.BasicDivOp(PiNum);
					if(self.IntValue==NegativeRep)
						self.IntValue = 0;
					else
						self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
					break;
					self.ExtraRep = IRep;
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
					self.BasicDivOp(Value);
					self.BasicDivOp(PiNum);
					if(self.IntValue==NegativeRep)
						self.IntValue = 0;
					else
						self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
					break;
					self.ExtraRep = IRep;
					break;
			#endif
			#if defined(AltNum_EnableComplexNumbers)
				case RepType::ComplexIRep:
					break;
			#endif
		#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
			}
			break;
	#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
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
					self.BasicDivOp(Value);
					self.BasicDivOp(PiPowerNum(-self.ExtraRep));
					if(self.IntValue==NegativeRep)
						self.IntValue = 0;
					else
						self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
					break;
					self.ExtraRep = IRep;
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
					Value.ConvertIRepToNormal(RRep);
					self.BasicDivOp(Value);
                    Value = PiPowerNum(-self.ExtraRep);//Reusing Value to store divisor
					self.BasicDivOp(Value);
					if(self.IntValue==NegativeRep)
						self.IntValue = 0;
					else
						self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
					break;
					self.ExtraRep = IRep;
					break;
			#endif
		#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
			}
			break;
	#endif
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
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
			#if defined(AltNum_EnablePiPowers)
				case RepType::PiPower:
					Value.ConvertPiPowerToNum();
					self.BasicDivOp(Value);
					break;
			#endif			

			#if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::NumByDiv://(X*E) / (Y / Z) = (XZ*E)/Y
					self.BasicDivOp(Value);
					self.BasicIntMultOp(Value.ExtraRep);
					break;
					
				#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					//(Xe) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf*e)/(Y)
					self.BasicMultOp(Value.DecimalHalf);
					self.BasicUnsignedDiv(PiNumValue()*Value.IntValue);
					break;
				#endif
				#if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					self.ExtraRep = 0;
					self.BasicMultOp(Value.DecimalHalf);
					self.BasicDivOp(Value.IntValue);
					break;
				#endif

				#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
					self.BasicUnsignedIntMult(-Value.ExtraRep);
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
					self.BasicDivOp(ENum);
					if(self.IntValue==NegativeRep)
						self.IntValue = 0;
					else
						self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
					break;
					self.ExtraRep = IRep;
				#if defined(AltNum_EnableAlternativeRepFractionals)
				    #if defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
				    #else
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    #endif
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
			#endif
			//Placeholder code(Converting to i rep for now)
			#if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
					Value.ConvertToNormalIRep(RRep);
					self.BasicDivOp(Value);
					self.BasicDivOp(ENum);
					if(self.IntValue==NegativeRep)
						self.IntValue = 0;
					else
						self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
					break;
					self.ExtraRep = IRep;
					break;
			#endif
		#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
			}
			break;
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			switch (RRep)
			{
	#if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific operation code later
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
                    //Skip to performing operation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
	#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
			}
			break;
		case RepType::ApproachingTop:
			switch (RRep)
			{
	#if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific operation code later
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
                    //Skip to performing operation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
	#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
			}
			break;

	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingBottomDiv:
			switch (RRep)
			{
		#if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific operation code later
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
                    //Skip to performing operation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
		#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
			}
			break;
		case RepType::ApproachingTopDiv:
			switch (RRep)
			{
		#if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific operation code later
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
                    //Skip to performing operation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
		#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
			}
			break;
	#endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
			switch (RRep)
			{
                case RepType::NormalType:
                    if(Value.DecimalHalf==0)
                        self.ExtraRep *= Value.IntValue;
                    else
                        self.BasicDivOp(Value);
                    break;
	#if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific operation code later
				case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
			#if defined(AltNum_EnableDecimaledIFractionals)						
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			#endif
		#endif
		#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
		#endif
                    //Skip to performing operation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
	#endif
				default:
					Value.ConvertToNormType(&RRep);
                    if(Value.DecimalHalf==0)
                        self.ExtraRep *= Value.IntValue;
                    else
                        self.BasicDivOp(Value);
					break;
				}
				break;
				
			}
    #if defined(AltNum_EnableAlternativeRepFractionals)
        #if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			switch (RRep)
			{
				case RepType::NormalType:
					if(Value.DecimalHalf==0)
						self.DecimalHalf *= Value.IntValue;
					else
                    {
            #if defined(AltNum_EnableDecimaledPiFractionals)
                        self.ExtraRep = -self.DecimalHalf;
                        self.DecimalHalf = 0;
                        //Skip to performing operation as DecimaledFractional
                        RepToRepDivOp(RepType::PiNumByDiv, RepType::NormalType, self, Value);
            #else
                        //Convert to NumByDiv instead
                        self.ExtraRep = self.DecimalHalf;
                        self.DecimalHalf = 0;
                        self.BasicMultOp(PiNum);
						self.BasicDivOp(Value);
            #endif
                    }
					break;
            #if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific operation code later
				case RepType::INum:
				#if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
					#if defined(AltNum_EnableDecimaledIFractionals)						
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
					#endif
				#endif
				#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
				#endif
                    //Skip to performing operation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
            #endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
            }
        #endif
        #if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			switch (RRep)
			{
				case RepType::NormalType:
					if(Value.DecimalHalf==0)
						self.DecimalHalf *= Value.IntValue;
					else
                    {
            #if defined(AltNum_EnableDecimaledEFractionals)
                        self.ExtraRep = -self.DecimalHalf;
                        self.DecimalHalf = 0;
                        //Skip to performing operation as DecimaledFractional
                        RepToRepDivOp(RepType::ENumByDiv, RepType::NormalType, self, Value);
            #else
                        //Convert to NumByDiv instead
                        self.ExtraRep = self.DecimalHalf;
                        self.DecimalHalf = 0;
                        self.BasicMultOp(ENum);
						self.BasicDivOp(Value);
            #endif
                    }
					break;
            #if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific operation code later
				case RepType::INum:
				#if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
					#if defined(AltNum_EnableDecimaledIFractionals)						
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
					#endif
				#endif
				#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
				#endif
                    //Skip to performing operation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
            #endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
            }
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
					self.BasicDivOp(Value);
					break;
            #if defined(AltNum_EnablePiRep)
				case RepType::PiNum:
					if(Value.DecimalHalf==0)
						self.ExtraRep *= Value.IntValue;
					else
						self.BasicDivOp(Value);
					self.BasicDivOp(Value);
					if(RRep==RepType::PiNumByDiv)
						self.ExtraRep *= -1;//Becomes NumByDiv instead
					else    
						self.BasicDivOp(PiNum);
					break;
            #endif
            #if defined(AltNum_EnableERep)
				case RepType::ENum:
					if(Value.DecimalHalf==0)
						self.ExtraRep *= Value.IntValue;
					else
						self.BasicDivOp(Value);
					self.BasicDivOp(Value);
					if(RRep==RepType::ENumByDiv)
						self.ExtraRep *= -1;//Becomes NumByDiv instead
					else    
						self.BasicDivOp(ENumValue);
					break;
            #endif
                case RepType::NumByDiv:
                    // /((Value.Value/(-Value.ExtraRep))*Pi) Representation
                    throw "ToDo:Need to add code here";
                    break;
            #if !defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiFractional:
                    // /((Value.Value/(-Value.ExtraRep))*Pi) Representation
                    throw "ToDo:Need to add code here";
                    break;
            #endif
            #if !defined(AltNum_EnableDecimaledEFractionals)
                case RepType::EFractional:
                    // /((Value.Value/(-Value.ExtraRep))*Pi) Representation
                    throw "ToDo:Need to add code here";
                    break;
            #endif
            #if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific operation code later
				case RepType::INum:
				#if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
					#if defined(AltNum_EnableDecimaledIFractionals)						
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
					#endif
				#endif
				#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
				#endif
                    //Skip to performing operation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
            #endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
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
					self.BasicDivOp(Value);
					break;
		#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
				case RepType::PiNum:
					Value.ConvertPiToNum()
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
					self.BasicMultOp(Value.DecimalHalf);
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
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
					break;
			}
			break;
    #if defined(AltNum_EnableFractionals)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			switch (RRep)
			{
				case RepType::NormalType:
					if(Value.DecimalHalf==0)
						self.DecimalHalf *= Value.IntValue;
					else//Convert to INum
					{
						self.ExtraRep = IRep;
						int LSideDivisor = self.DecimalHalf;
						self.DecimalHalf = 0;
						self.BasicDivOp(LSideDivisor.BasicMultOp(Value));
					}
					break;
				default:
					Value.ConvertToNormType(&RRep);
					if(Value.DecimalHalf==0)
						self.DecimalHalf *= Value.IntValue;
					else//Convert to INum
					{
						self.ExtraRep = IRep;
						int LSideDivisor = self.DecimalHalf;
						self.DecimalHalf = 0;
						self.BasicDivOp(LSideDivisor.BasicMultOp(Value));
					}
			}
			break;
    #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			switch (RRep)
			{
				case RepType::NormalType:
					self.BasicDivOp(Value);
					break;
				case RepType::INum:
					//SelfNum*i / ValueNum*i
					//SelfNum/ValueNum
					if(Value.DecimalHalf==0)
						self.ExtraRep = Value.ExtraRep;
					else
					{
						self.ExtraRep = 0;
						self.BasicDivOp(Value);
					}
					break;
        #if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    //(SelfNum/(-self.ExtraRep))*i / (IntValue/DecimalHalf*i)
                    //(SelfNum/(-self.ExtraRep)) / (Value.IntValue/Value.DecimalHalf)
                    throw "ToDo:Need to add code here";
                    break;						
			#if defined(AltNum_EnableDecimaledIFractionals)						
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                    //(SelfNum/(-self.ExtraRep))*i / ((Value/(-ExtraRep))*i)
                    //(SelfNum/(-self.ExtraRep)) / (Value/(-ExtraRep))
                    //((SelfNum*-Value.ExtraRep)/(-self.ExtraRep)) / (Value)
                    throw "ToDo:Need to add code here";
                    break;//Convert result to NumByDiv
			#endif
		    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
                    //(SelfNum/(-self.ExtraRep))*i / ((IntValue +- (DecimalHalf*-1)/-ExtraRep)i)
                    //(SelfNum/(-self.ExtraRep)) / ((IntValue +- (DecimalHalf*-1)/-ExtraRep))
                    //(SelfNum/(-self.ExtraRep)) / (IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
                    throw "ToDo:Need to add code here";
                    break;
		    #endif
        #endif
				default:
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
			}
			break;
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/ExtraRep)
			switch (RRep)
			{
                //case RepType::NormalType:
				//	break;
				default:
                    self.ConvertToNormType(&LRep);
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
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
                        int LeftSideAbs = self.IntValue.Abs();
                        int divRes = LeftSideAbs / Value.IntValue;
                        int C = LeftSideAbs - Value.IntValue * divRes;
                        if(C==0)
                        {
                            if(divRes==0)//Convert into non-mixed fractional
                            {
                                throw "ToDo:Finish this code later";
                            }
                            else
                            {
                                throw "ToDo:Finish this code later";
                            }
                        }
                    }
                    else//Convert to NumByDiv
                    {
                        throw "ToDo:Finish this code later";
                    }
					break;
				default:
                    self.ConvertToNormType(&LRep);
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
			}
			break;
    #endif
    if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			switch (RRep)
			{
                case RepType::INum:
        #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
        #elif defined(AltNum_EnableFractionals)
                case RepType::IFractional:
        #endif
                    self.CatchAllImaginaryDivision(Value, LRep, RRep);
				default:
                    self.ConvertToNormalIRep(&LRep);
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
            }
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