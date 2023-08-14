#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

inline void BlazesRusCode::AltDec::RepToRepAddOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
	bool LeftIsNegative = self.IntValue<0;
	const bool RightIsNegative = false;//bool RightIsNegative = Value.IntValue<0;//Should always return positive value before RepToRep method used
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
        #ifdef AltNum_EnableComplexNumbers
		case RepType::ComplexIRep:
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
        #if defined(AltNum_EnableComplexNumbers)
				Value.ConvertToNormType(RRep);
				RRep = RepType::NormalType;
        #else
                throw "Complex number operations not enabled";
        #endif
				break;
    #endif
			default:
				break;
			}
			break;
	#endif
		default:
    #if defined(AltNum_EnableMixedIFractional)
		    if(RRep==RepType::MixedI)
        #if defined(AltNum_EnableComplexNum)
	            throw "Complex number code not implimented yet.";
        #else
	            throw "Complex number operation not enabled currently.";
        #endif
    #endif
            break;
	}

	//RRep Overrides before Main RepToRep Code
	switch(RRep)
	{
	#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			if(LRep==RepType::ApproachingTop&&LeftIsNegative==RightIsNegative)
			{
				self.DecimalHalf = 0;
				int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
				if(LeftIsNegative)//&&RightIsNegative)
					--RightSide;
				else// if(LeftIsNegative==false&&RightIsNegative==false)
					++RightSide;
				if(self.IntValue==NegativeRep)
				   self.IntValue = 0;
				self.IntValue += RightSide;
				return;
			}
			else
			{
			   Value.DecimalHalf = 1;
			   RRep = RepType::NormalType;
			}
			break;

		case RepType::ApproachingTop:
			if(LRep==RepType::ApproachingBottom&&LeftIsNegative==RightIsNegative)
			{
				self.DecimalHalf = 0;
				int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
				if(LeftIsNegative)//&&RightIsNegative)
					--RightSide;
				else// if(LeftIsNegative==false&&RightIsNegative==false)
					++RightSide;
				if(self.IntValue==NegativeRep)
				   self.IntValue = 0;
				self.IntValue += RightSide;
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
			if(LRep==RepType::ApproachingTopDiv&&self.ExtraRep==-Value.ExtraRep&&LeftIsNegative==RightIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = 0;
				int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
				if(LeftIsNegative)//&&RightIsNegative)
					--RightSide;
				else// if(LeftIsNegative==false&&RightIsNegative==false)
					++RightSide;
				if(self.IntValue==NegativeRep)
				   self.IntValue = 0;
				self.IntValue += RightSide;
				return;
			}
			else
			{
				Value.ConvertToNormType(RepType::ApproachingBottomDiv);
				RRep = RepType::NormalType;
			}
			break;
		case RepType::ApproachingTopDiv:
			if(LRep==RepType::ApproachingBottomDiv&&self.ExtraRep==-Value.ExtraRep&&LeftIsNegative==RightIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = 0;
				int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
				if(LeftIsNegative)//&&RightIsNegative)
					--RightSide;
				else// if(LeftIsNegative==false&&RightIsNegative==false)
					++RightSide;
				if(self.IntValue==NegativeRep)
				   self.IntValue = 0;
				self.IntValue += RightSide;
				return;
			}
			else
			{
				Value.ConvertToNormType(RepType::ApproachingTopDiv);
				RRep = RepType::NormalType;
			}
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
		case RepType::UnknownType:
			throw "Can't perform operations with unknown/non-enabled format number";
			break;
		default://No nothing for most of them
		break;
	}

	switch (LRep)//Main switch block starts here
	{
		case RepType::NormalType:
			switch (RRep)
			{
				case RepType::NormalType://Fail safe for when converted before switch
					self.BasicAddOp(Value);
					break;
    #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					BasicMixedFracAddOp(self, Value);
                    break;
	    #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	    #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	    #endif
	    #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
            #if defined(AltNum_EnableMixedPiFractional) 
					BasicMixedPiFracAddOp(self, Value);
            #else
					BasicMixedEFracAddOp(self, Value);
            #endif
					break;
	    #endif
    #endif
				default:
					Value.ConvertToNormType(RRep);
					self.BasicAddOp(Value);
					break;
			}
			break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			switch (RRep)
			{
    #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                    self.ConvertToNormType(LRep);
					BasicMixedFracAddOp(self, Value);
                    break;
	    #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	    #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	    #endif
	    #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
            #if defined(AltNum_EnableMixedPiFractional)
                    if(self.DecimalHalf==0)
                    {
                        Value.IntValue += self.IntValue;
                        self.SetVal(Value);
                    }
                    else
                    {
                        //check if value is already a fractional equivalant of Value
                        //AltDec divRes = self / -Value.ExtraRep;//if(self%-Value.ExtraRep==0)
                        //AltDec C = self + Value.ExtraRep * divRes;
                        //if(C==0)
                        //{
                        //}
                        //else
                        //{
                            self.ConvertToNormType(LRep);
					        BasicMixedPiFracAddOp(self, Value);
                        //}
                    }
            #else
                    self.ConvertToNormType(LRep);
					BasicMixedEFracAddOp(self, Value);
            #endif
					break;
	    #endif
    #endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
	#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			switch (RRep)
			{
        #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                    self.ConvertToNormType(LRep);
					BasicMixedFracAddOp(self, Value);
                    break;
	        #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	        #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	        #endif
	        #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
                    self.ConvertToNormType(LRep);
                #if defined(AltNum_EnableMixedPiFractional)
					BasicMixedPiFracAddOp(self, Value);
                #else
					BasicMixedEFracAddOp(self, Value);
                #endif
					break;
	        #endif
        #endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
	#endif
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
			switch (RRep)
			{
    #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                    self.ConvertToNormType(LRep);
					BasicMixedFracAddOp(self, Value);
                    break;
	    #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	    #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	    #endif
	    #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
            #if defined(AltNum_EnableMixedPiFractional)
                    self.ConvertToNormType(LRep);
                    BasicMixedPiFracAddOp(self, Value);
            #else
                    if(self.DecimalHalf==0)
                    {
                        Value.IntValue += self.IntValue;
                        self.SetVal(Value);
                    }
                    else
                    {
                        //check if value is already a fractional equivalant of Value
                        //AltDec divRes = self / -Value.ExtraRep;//if(self%-Value.ExtraRep==0)
                        //AltDec C = self + Value.ExtraRep * divRes;
                        //if(C==0)
                        //{
                        //}
                        //else
                        //{
                            self.ConvertToNormType(LRep);
					        BasicMixedPiFracAddOp(self, Value);
                        //}
                    }
            #endif
					break;
	    #endif
    #endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			switch (RRep)
			{
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
    #if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
			switch (RRep)
			{
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
    #endif
    #if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingBottomDiv:
			switch (RRep)
			{
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
        #if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTopDiv:
			switch (RRep)
			{
				default:
					self.CatchAllAddition(Value, LRep, RRep);
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
    #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                    //self.ConvertToNormType(LRep);
					//BasicMixedFracAddOp(self, Value);
                    break;
    #endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
    #if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			switch (RRep)
			{
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
    #endif
    #if defined(AltNum_EnableEFractional)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			switch (RRep)
			{
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
			switch (RRep)
			{
        #if defined(AltNum_EnableMixedFractional)
                //case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                //    break;
            #if defined(AltNum_EnableMixedPiFractional)
                //case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
            #elif defined(AltNum_EnableMixedEFractional)
                //case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
            #endif
            #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
                #if defined(AltNum_EnableDecimaledPiFractionals)&&defined(AltNum_EnableMixedPiFractional)
                #elif defined(AltNum_EnableDecimaledEFractionals)&&defined(AltNum_EnableMixedEFractional)
                #else

                #endif
            #endif
        #endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
    #endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			switch (RRep)
			{
                case RepType::INum:
                    BasicAddOp(Value);
                    break;
        #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
                    self.CatchAllImaginaryAddition(Value, LRep, RRep);
                    break;
        #elif defined(AltNum_EnableFractionals)
                case RepType::IFractional:
                    self.CatchAllImaginaryAddition(Value, LRep, RRep);
                    break;
        #endif
        #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    self.CatchAllImaginaryAddition(Value, LRep, RRep);
                    break;
        #endif
				default:
    //#if defined(AltNum_EnableComplexNumbers)
    //              throw static_cast"Complex number operations not enabled right now";
    //#else
                    throw AltDec::RepTypeAsString(LRep)-" RepType addition with"-AltDec::RepTypeAsString(RRep)-"not supported yet";
    //#endif
					break;
            }
            break;
    #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://  (Value/(-ExtraRep))*i Representation
			switch (RRep)
			{
			{
                case RepType::INum:
                    self.CatchAllImaginaryAddition(Value, LRep, RRep);
                    break;
        #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    self.CatchAllImaginaryAddition(Value, LRep, RRep);
                    break;
        #endif
				default:
    //#if defined(AltNum_EnableComplexNumbers)
    //              throw static_cast"Complex number operations not enabled right now";
    //#else
                    throw AltDec::RepTypeAsString(LRep)-" RepType addition with"-AltDec::RepTypeAsString(RRep)-"not supported yet";
    //#endif
					break;
            }
            break;
    #elif defined(AltNum_EnableFractionals)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			switch (RRep)
			{
                case RepType::INum:
                    self.CatchAllImaginaryAddition(Value, LRep, RRep);
                    break;
        #if defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                    self.CatchAllImaginaryAddition(Value, LRep, RRep);
                    break;
        #endif
				default:
    //#if defined(AltNum_EnableComplexNumbers)
    //              throw static_cast"Complex number operations not enabled right now";
    //#else
                    throw AltDec::RepTypeAsString(LRep)-" RepType addition with"-AltDec::RepTypeAsString(RRep)-"not supported yet";
    //#endif
					break;
            }
            break;
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			switch (RRep)
			{
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	#elif defined(AltNum_EnableMixedEFractional)
				//case RepType::MixedE:
    #endif
    #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
        //#if defined(AltNum_EnableMixedPiFractional)&&AltNum_EnableDecimaledPiFractionals
        //            break;//Give result as PiByDiv
        //#elif defined(AltNum_EnableMixedEFractional)&&AltNum_EnableDecimaledEFractionals
        //            break;//Give result as EByDiv
        //#else
			        int LeftSideNum;
			        if(self.IntValue==NegativeRep)
				        LeftSideNum = self.DecimalHalf;
			        else if(self.IntValue<0)
				        LeftSideNum = self.IntValue*self.ExtraRep + self.DecimalHalf;
			        else if(self.IntValue==0)
				        LeftSideNum = -self.DecimalHalf;
			        else
				        LeftSideNum = self.IntValue*self.ExtraRep - self.DecimalHalf;
                    AltDec RightSideNum = AltDec(Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf);
	    #if defined(AltNum_EnableMixedPiFractional)
                    RightSideNum *= PiNum;
        #else
                    RightSideNum *= ENum;
        #endif
                    int InvertedVDivisor = -Value.ExtraValue;
                    if(self.ExtraRep==InvertedVDivisor)
                    {
                        RightSideNum.BasicAddOp(LeftSideNum);
                        self.IntValue = RightSide.IntValue;
                        self.DecimalHalf = RightSide.DecimalHalf;
                    }
                    else
                    {
                        self.ExtraRep *= InvertedVDivisor;
                        LeftSideNum *= InvertedVDivisor;
                        RightSideNum *= self.ExtraRep;
                        RightSideNum.BasicAddOp(LeftSideNum);
                        self.IntValue = RightSide.IntValue;
                        self.DecimalHalf = RightSide.DecimalHalf;
                    }
                    break;//Give result as NumByDiv
        //#endif
    #endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
			switch (RRep)
			{
        //#if defined(AltNum_EnableMixedPiFractional)&&AltNum_EnableDecimaledPiFractionals
        //            break;//Give result as PiByDiv
        //#elif defined(AltNum_EnableMixedEFractional)&&AltNum_EnableDecimaledEFractionals
        //            break;//Give result as EByDiv
        //#else
                case RepType::MixedFrac:
        			AltDec LeftSideNum;
        			if(self.IntValue==NegativeRep)
        				LeftSideNum = AltDec(self.DecimalHalf);
        			else if(self.IntValue<0)
        				LeftSideNum = AltDec(self.IntValue*-self.ExtraRep + self.DecimalHalf);
        			else if(self.IntValue==0)
        				LeftSideNum = AltDec(-self.DecimalHalf);
        			else
        				LeftSideNum = AltDec(self.IntValue*-self.ExtraRep + -self.DecimalHalf);
            #if defined(AltNum_EnableMixedPiFractional)
                    LeftSideNum *= PiNum;
            #elif defined(AltNum_EnableMixedEFractional)
                    LeftSideNum *= ENum;
            #endif
                    int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
                    self.ExtraRep = -self.ExtraRep;
                    if(Value.ExtraRep==self.ExtraRep)
                    {
                        LeftSideNum.BasicAddOp(RightSideNum);
                        self.IntValue = LeftSide.IntValue;
                        self.DecimalHalf = LeftSide.DecimalHalf;
                    }
                    else
                    {
                        self.ExtraRep *= Value.ExtraRep;
                        LeftSideNum *= Value.ExtraRep;
                        RightSideNum *= self.ExtraRep;
                        LeftSideNum.BasicAddOp(RightSideNum);
                        self.IntValue = LeftSide.IntValue;
                        self.DecimalHalf = LeftSide.DecimalHalf;
                    }
                    break;
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
            }
            break;
        //#endif
    #endif
    #if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			switch (RRep)
			{
                case RepType::MixedFrac:
        #if defined(AltNum_EnableComplexNum)
	            throw "Complex number code not implimented yet.";
        #else
	            throw "Complex number operation not enabled currently.";
        #endif
                    break;
				default:
					throw "Code not implimented yet.";
					break;
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
			throw AltDec::RepTypeAsString(LRep)-" RepType addition with"-AltDec::RepTypeAsString(RRep)-"not supported yet";
			break;
	}
}