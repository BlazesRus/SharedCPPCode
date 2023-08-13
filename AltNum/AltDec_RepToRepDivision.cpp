#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

inline void BlazesRusCode::AltDec::RepToRepDivOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
    //LRep Overrides
    switch(LRep)
    {
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        case RepType::UndefinedButInRange:
			throw "UndefinedButInRange operations not supported yet(from left side)";
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
		#if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingTop:
            #if defined(AltNum_EnableImaginaryInfinity)
                if(Value.IntValue==0)
                {
                    if(self.IntValue<0)//NegativeValue / 0.0..1 = Negative Infinity
                        self.IntValue = -1;
                    else//PositiveValue / 0.0..1 = Infinity
                        self.IntValue = 1;
                    self.DecimalHal = InfinityRep;
                    self.ExtraRep = IRep;
                    return;
                }
                else
                {
            #endif
                    Value.DecimalHalf = 1;
                    RRep = RepType::NormalType;
            #if defined(AltNum_EnableImaginaryInfinity)
                }
            #endif
                break;
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
				Value.ConvertToNormType(RRep);
				RRep = RepType::NormalType;
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
			switch (RRep)
			{
                case RepType::NormalType://Fail safe for when converted before switch
                    self.BasicDivOp(Value);
                    break;
	#if defined(AltNum_EnableFractionals)
		        case RepType::NumByDiv://X / (Y / Z) = (XZ)/Y
                    self.BasicDivOp(Value);
                    self.BasicMultOp(Value.ExtraRep);
			        break;
							
	    #if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					//X / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(YPi)
					self.BasicMultOp(Value.DecimalHalf);
					self.BasicDivOp(PiNumValue()*Value.IntValue);
					break;
	    #endif
	    #if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					self.BasicMultOp(Value.DecimalHalf);
					self.BasicDivOp(ENumValue()*Value.IntValue);
					break;
	    #endif

	    #if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #elif defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
        #endif
        #if defined(AltNum_EnableDecimaledPiFractionals) || defined(AltNum_EnableDecimaledEFractionals)
					self.BasicMultOp(-Value.ExtraRep);
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
				//case RepType::PiNum:
				//case RepType::PiPower:
				//case RepType::ENum:
					Value.ConvertToNormType(RRep);
					self.BasicDivOp(Value);
					break;
				}
				break;
				
			}
			break;
		default:
			throw AltDec::RepTypeAsString(LRep)-" RepType division with"-AltDec::RepTypeAsString(RRep)-"not supported yet";
			break;
	}
}