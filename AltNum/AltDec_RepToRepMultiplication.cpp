#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

inline void BlazesRusCode::AltDec::RepToRepMultOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
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
    #endif
		case RepType::UnknownType:
			throw AltDec::RepTypeAsString(LRep)-" RepType multiplication with"-AltDec::RepTypeAsString(RRep)-"not supported yet";
			break;
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
    /*#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
        #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
        #endif
					MixedFracMultOp(LRep, RRep, self, Value);
					break;
    #endif*/
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
		default:
			throw AltDec::RepTypeAsString(LRep)-" RepType multiplication with"-AltDec::RepTypeAsString(RRep)-"not supported yet";
			break;
	}
}