#include "AltDec.hpp"

bool MediumDecVariant::RepToRepSubOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
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
	#if defined(AltNum_EnableComplexNumbers)
		case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#endif
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
		#endif
		case RepType::ComplexIRep:
			switch(RRep)
			{
				case RepType::NormalType:
				case RepType::NumByDiv:
		#if defined(AltNum_EnablePiRep)
				case RepType::PiNum:
			#if defined(AltNum_EnablePiPowers)
				case RepType::PiPower:
			#endif
			#if defined(AltNum_EnableAlternativeRepFractionals)
				#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
				#endif
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			#endif
		#endif
	
		#if defined(AltNum_EnableENum)
				case RepType::ENum:
			#if defined(AltNum_EnableAlternativeRepFractionals)
				#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
				#endif
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			#endif
		#endif
	
		#if defined(AltNum_EnableInfinityRep)
				case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
				case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
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
					Value.ConvertToNormalRep(RRep);
					break;
				default:
					break;
			}
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
			if(LRep==RepType::ApproachingTop)
			{
				if(LeftIsNegative^RightIsNegative)
				{
					self.DecimalHalf = 0;
					int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
					if(LeftIsNegative)//-X.9..9 - Y.0..1
						--RightSide;
					else//X.9..9 
						++RightSide;
					if(self.IntValue==NegativeRep)
					   self.IntValue = 0;
					self.IntValue -= RightSide;
					return;
				}
				else
				{
					Value.DecimalHalf = 1;
					RRep = RepType::NormalType;
				}
			}
			else
			{
			   Value.DecimalHalf = 1;
			   RRep = RepType::NormalType;
			}
			break;

		case RepType::ApproachingTop:
			if(LRep==RepType::ApproachingBottom)
			{
				if(LeftIsNegative^RightIsNegative)
				{
					self.DecimalHalf = 0;
					int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
				   if(LeftIsNegative)//-X.0..1 - Y.9..9 = X+Y+1
						--RightSide;
				   else//X.0..1 - -Y.9..9 = X+Y+1
						++RightSide;
					if(self.IntValue==NegativeRep)
					   self.IntValue = 0;
					self.IntValue -= RightSide;
					return;
				}
				else
				{
					Value.DecimalHalf = 999999999;
					Value.ExtraRep = 0;
					RRep = RepType::NormalType;
				}
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
			if(LRep==RepType::ApproachingTopDiv&&self.ExtraRep==-Value.ExtraRep&&LeftIsNegative^RightIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = 0;
				int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
				if(LeftIsNegative)
					--RightSide;
				else
					++RightSide;
				if(self.IntValue==NegativeRep)
				   self.IntValue = 0;
				self.IntValue -= RightSide;
				return;
			}
			else
			{
				Value.ConvertToNormType(RepType::ApproachingBottomDiv);
				RRep = RepType::NormalType;
			}
			break;
		case RepType::ApproachingTopDiv:
			if(LRep==RepType::ApproachingBottomDiv&&self.ExtraRep==-Value.ExtraRep&&LeftIsNegative^RightIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = 0;
				int RightSide = Value.IntValue==NegativeRep?0:Value.IntValue;
				if(LeftIsNegative)
					--RightSide;
				else
					++RightSide;
				if(self.IntValue==NegativeRep)
				   self.IntValue = 0;
				self.IntValue -= RightSide;
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
					self.BasicSubOp(Value);
					break;
    #if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					BasicMixedFracSubOp(self, Value);
                    break;
	    #if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	    #elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	    #endif
	    #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
            #if defined(AltNum_EnableMixedPiFractional) 
					BasicMixedPiFracSubOp(self, Value);
            #else
					BasicMixedEFracSubOp(self, Value);
            #endif
					break;
	    #endif
    #endif
				default:
					Value.ConvertToNormType(RRep);
					self.BasicSubOp(Value);
					break;
			}
			break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
	#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
	#endif
#endif
#if defined(AltNum_EnableENum)
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
    #if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
    #endif
    #if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingBottomDiv:
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
        #if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTopDiv:
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
        #endif
    #endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
			switch (RRep)
			{
				case RepType::NormalType://Later normalize fractional when integer when viable
						//ToDo:Catch potential overflow of numerator and use catchall method instead on overflow
                        self.BasicSubOp(Value.BasicMultOp(self.DecimalHalf));
						if(self.IntValue==0&&self.DecimalHalf==0)
							self.ExtraRep = 0;
					break;
    #if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
					int ReversedVExtraRep = -Value.ExtraRep;
					if(self.DecimalHalf==ReversedVExtraRep)
					{
						if(RRep==PiNumByDiv)
							Value.ConvertToNormType(RepType::PiNum);
						else
							Value.ConvertToNormType(RepType::ENum);
						self.BasicSubOp(Value);
					}
					else
					{
						Value.ConvertToNormType(RRep);
						self.BasicSubOp(Value.BasicMultOp(self.DecimalHalf));
					}
					break;
    #endif

				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
    #if defined(AltNum_EnablePiRep)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
    #endif
    #if defined(AltNum_EnableENum)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
    #endif
    #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://  (Value/(-ExtraRep))*I Representation
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
    #endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			switch (RRep)
			{
				default:
    //#if defined(AltNum_EnableComplexNumbers)
    //    throw static_cast"Complex number operations not enabled right now";
    //#else
        throw static_cast<RepType>(LRep)-" RepType subtraction with"-static_cast<RepType>(RRep)-"not supported yet";
    //#endif
					break;
            }
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi:
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #elif defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
    #endif
			switch (RRep)
			{
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
            }
#endif
		default:
			throw static_cast<RepType>(LRep)-" RepType subtraction with"-static_cast<RepType>(RRep)-"not supported yet";
			break;
	}
}