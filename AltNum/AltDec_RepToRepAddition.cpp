#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
		case RepType::NormalType://Fail safe for when converted before switch
			self.BasicAddOp(Value);
			break;
	#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.BasicMixedFracAddOp(Value);
			break;
		#if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
		#elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
		#endif
		#if defined(AltNum_MixedPiOrEEnabled)
			#if defined(AltNum_EnableMixedPiFractional) 
			self.BasicMixedPiFracAddOp(Value);
			#else
			self.BasicMixedEFracAddOp(Value);
			#endif
			break;
		#endif
	#endif
		default:
			Value.ConvertToNormType(&RRep);
			self.BasicAddOp(Value);
			break;
	}
}

#if defined(AltNum_EnableFractionals)
void NumByDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
    #if defined(AltNum_EnablePiPowers)//Combine PiPower in certain cases
    #endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(RepType::PiNum);
			self.BasicMixedFracAddOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedEFractional)
			self.ConvertToNormType(RepType::PiNum);
			self.BasicMixedEFracSubOp(Value);
        #else
            #if defined(AltNum_EnablePiFractional)
    			if(self.DecimalHalf==0)//Result is PiFractional
    			{
                    int divisor = -Value.ExtraRep;
                    if(Value.IntValue!=0)
                        self.IntValue += Value.IntValue;
                    if(self.IntValue==0)
                    {
                        self.IntValue = -Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue *= divisor;
                        self.IntValue -= self.DecimalHalf;//adding as negative of the representation
                        if(self.IntValue==0)//Become zero instead of fractional
                        {
                            self.ExtraRep = 0; return;
                        }
                    }
                    self.DecimalHalf = divisor;
                    self.ExtraRep = PiByDivisorRep;
    			}
    			else
    			{
    	            self.ConvertToNormType(RepType::PiNum);
    	            self.BasicMixedPiFracSubOp(Value);
    			}
            #elif defined(AltNum_EnableDecimaledPiFractionals)//Result is DecimaledPiFractional
                if(Value.IntValue!=0)
                    self.IntHalfSubtractionOp(Value.IntValue);
                if(self.IntValue==0)
                {
                    self.IntValue = -Value.DecimalHalf;
                }
                else
                {
                    self.IntValue *= -Value.ExtraRep;
                    self.IntHalfSubtractionOp(&self.DecimalHalf);//adding as negative of the representation
                    if(self.IntValue==0&&self.DecimalHalf==0)//Become zero instead of fractional
                    {
                        self.ExtraRep = 0; return;
                    }
                }
                self.ExtraRep = Value.ExtraRep;
            #endif
        #endif
			break;
    #endif
#endif
		default:
			self.CatchAllAddition(&Value, RepType::PiNum, &RRep);
			break;
	}
}

	#if defined(AltNum_EnablePiPowers)
void PiPowerOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	
	#if defined(AltNum_EnableDecimaledPiFractionals)
void PiNumByDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnablePiFractional)
void PiFractionalOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableERep)
void ENumOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(RepType::ENum);
			self.BasicMixedFracAddOp(Value);
			break;
#if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
#elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
#endif
#if defined(AltNum_MixedPiOrEEnabled)
	#if defined(AltNum_EnableMixedPiFractional)
			self.ConvertToNormType(RepType::ENum);
			self.BasicMixedPiFracAddOp(Value);
        #else
            #if defined(AltNum_EnableEFractional)
    			if(self.DecimalHalf==0)//Result is EFractional
    			{
                    int divisor = -Value.ExtraRep;
                    if(Value.IntValue!=0)
                        self.IntValue += Value.IntValue;
                    if(self.IntValue==0)
                    {
                        self.IntValue = -Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue *= divisor;
                        self.IntValue -= self.DecimalHalf;//adding as negative of the representation
                        if(self.IntValue==0)//Become zero instead of fractional
                        {
                            self.ExtraRep = 0; return;
                        }
                    }
                    self.DecimalHalf = divisor;
                    self.ExtraRep = EByDivisorRep;
    			}
    			else
    			{
    	            self.ConvertToNormType(RepType::ENum);
    	            self.BasicMixedPiFracSubOp(Value);
    			}
            #elif defined(AltNum_EnableDecimaledEFractionals)//Result is DecimaledEFractional
                if(Value.IntValue!=0)
                    self.IntHalfSubtractionOp(Value.IntValue);
                if(self.IntValue==0)
                {
                    self.IntValue = -Value.DecimalHalf;
                }
                else
                {
                    self.IntValue *= -Value.ExtraRep;
                    self.IntHalfSubtractionOp(&self.DecimalHalf);//adding as negative of the representation
                    if(self.IntValue==0&&self.DecimalHalf==0)//Become zero instead of fractional
                    {
                        self.ExtraRep = 0; return;
                    }
                }
                self.ExtraRep = Value.ExtraRep;
            #endif
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllAddition(&Value, RepType::ENum, &RRep);
			break;
	}
}

	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableEFractional)
void EFractionalOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingValues)
void ApproachingBottomOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingTopOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidRightOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}

		#if !defined(AltNum_DisableApproachingTop)
void ApproachingMidLeftOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
		#endif
	#endif
#endif

#if defined(AltNum_EnableImaginaryNum)
void ImaginaryNumberOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::INum:
#if defined(AltNum_EnableDecimaledIFractionals)
        case RepType::INumByDiv:
#elif defined(AltNum_EnableFractionals)
        case RepType::IFractional:
#endif
#if defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI:
#endif
            self.CatchAllImaginaryAddition(Value, LRep, RRep);
            break;
        default:
            throw AltDec::RepTypeAsString(LRep)+" RepType addition with"+AltDec::RepTypeAsString(RRep)+"not supported yet";
	        break;
    }
}

#endif

#if defined(AltNum_EnableMixedFractional)
void MixedFracOp(RepType& RRep, AltDec& self, AltDec& Value)
{
			switch (RRep)
			{
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	#elif defined(AltNum_EnableMixedEFractional)
				//case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
        //#if defined(AltNum_MixedPiHasDecimaledFracAccess)
        //            break;//Give result as PiByDiv
        //#elif defined(AltNum_MixedEHasDecimaledFracAccess)
        //            break;//Give result as EByDiv
        //#else
			        int LeftSide;
			        if(self.IntValue==AltDec::NegativeRep)
				        LeftSide = self.DecimalHalf;
			        else if(self.IntValue<0)
				        LeftSide = self.IntValue*self.ExtraRep + self.DecimalHalf;
			        else if(self.IntValue==0)
				        LeftSide = -self.DecimalHalf;
			        else
				        LeftSide = self.IntValue*self.ExtraRep - self.DecimalHalf;
                    AltDec RightSide = AltDec(Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf);
	    #if defined(AltNum_EnableMixedPiFractional)
                    RightSide *= AltDec::PiNum;
        #else
                    RightSide *= AltDec::ENum;
        #endif
                    int InvertedVDivisor = -Value.ExtraRep;
                    if(self.ExtraRep==InvertedVDivisor)
                    {
                        RightSide.BasicAddOp(LeftSide);
                        self.IntValue = RightSide.IntValue;
                        self.DecimalHalf = RightSide.DecimalHalf;
                    }
                    else
                    {
                        self.ExtraRep *= InvertedVDivisor;
                        LeftSide *= InvertedVDivisor;
                        RightSide *= self.ExtraRep;
                        RightSide.BasicAddOp(LeftSide);
                        self.IntValue = RightSide.IntValue;
                        self.DecimalHalf = RightSide.DecimalHalf;
                    }
                    break;//Give result as NumByDiv
        //#endif
    #endif
				default:
					self.CatchAllAddition(&Value, RepType::, &RRep);
					break;
            }
}
#endif

#if defined(AltNum_MixedPiOrEEnabled)
void MixedPiEOp(RepType& RRep, AltDec& self, AltDec& Value)
{
			switch (RRep)
			{
                case RepType::MixedFrac:
				{
					AltDec LeftSide;
					if (self.IntValue == AltDec::NegativeRep)
						LeftSide = AltDec(self.DecimalHalf);
					else if (self.IntValue < 0)
						LeftSide = AltDec(self.IntValue * -self.ExtraRep + self.DecimalHalf);
					else if (self.IntValue == 0)
						LeftSide = AltDec(-self.DecimalHalf);
					else
						LeftSide = AltDec(self.IntValue * -self.ExtraRep + -self.DecimalHalf);
					#if defined(AltNum_EnableMixedPiFractional)
					LeftSide *= AltDec::PiNum;
					#elif defined(AltNum_EnableMixedEFractional)
					LeftSide *= AltDec::ENum;
					#endif
					int RightSide = Value.IntValue == 0 ? -Value.DecimalHalf : Value.IntValue * Value.ExtraRep - Value.DecimalHalf;
					self.ExtraRep = -self.ExtraRep;
					if (Value.ExtraRep == self.ExtraRep)
					{
						LeftSide.BasicAddOp(RightSide);
						self.IntValue = LeftSide.IntValue;
						self.DecimalHalf = LeftSide.DecimalHalf;
					}
					else
					{
						self.ExtraRep *= Value.ExtraRep;
						LeftSide *= Value.ExtraRep;
						RightSide *= self.ExtraRep;
						LeftSide.BasicAddOp(RightSide);
						self.IntValue = LeftSide.IntValue;
						self.DecimalHalf = LeftSide.DecimalHalf;
					}
					break;
				}
/*
    #if defined(AltNum_MixedPiOrEHasDecimaledFracAccess)
	    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
        #else
                case RepType::ENumByDiv:
        #endif
                    //ToDo:Add more precise code here later
                    break;
    #elif defined(AltNum_EnablePiFractional)|| defined(AltNum_EnableEFractional)
	    #if defined(AltNum_EnablePiFractional)

        #else

        #endif
                    //ToDo:Add more precise code here later
                    break;
    #endif
*/
				default:
	#if defined(AltNum_EnableMixedPiFractional)
					self.CatchAllAddition(&Value, RepType::MixedPi, &RRep);
	#else
					self.CatchAllAddition(&Value, RepType::MixedE, &RRep);	
	#endif
					break;
            }
}
#endif

#if defined(AltNum_EnableImaginaryNum)
void LRepImaginaryOverridePt2(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
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
		Value.ConvertToNormType(&RRep);
		RRep = RepType::NormalType;
#else
		throw "Complex number operations not enabled";
#endif
		break;
#endif
	default:
		break;
	}
}
#endif

inline void BlazesRusCode::AltDec::RepToRepAddOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
	bool LeftIsNegative = self.IntValue<0;
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
			LRepImaginaryOverridePt2(LRep, RRep, self, Value);
		default:
			break;
	}
    //RRep Overrides before Main RepToRep Code
    switch(RRep)
    {
	#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
		{
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
		}
		#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
		{
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
		}
		#endif
        #if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
		{
			if(LRep==RepType::ApproachingMidRight&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative==RightIsNegative)
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
				Value.ConvertFromApproachingMidLeftToNorm().
				RRep = RepType::NormalType;
			}
			break;
		}
			#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingMidRight:
        {
			if(LRep==RepType::ApproachingMidLeft&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative==RightIsNegative)
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
				Value.ConvertFromApproachingMidRightToNorm();
				RRep = RepType::NormalType;
			}
			break;
        }
			#endif
        #endif
    #endif
	#if defined(AltNum_EnableApproachingI)
		case RepType::ApproachingBottom:
		{
			if(LRep==RepType::ApproachingTop&&LeftIsNegative==RightIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = IRep;
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
				Value.ExtraRep = IRep;
				RRep = RepType::INum;
			}
			break;
		}
		#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingImaginaryTop:
		{
			if(LRep==RepType::ApproachingImaginaryBottom&&LeftIsNegative==RightIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = IRep;
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
				Value.ExtraRep = IRep;
				RRep = RepType::INum;
			}
			break;
		}
		#endif
        #if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingImaginaryMidLeft:
		{
			if(LRep==RepType::ApproachingImaginaryMidRight&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative==RightIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = IRep;
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
				Value.ConvertFromApproachingIMidLeftToINum();
				RRep = RepType::INum;
			}
			break;
		}
			#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingImaginaryMidRight:
        {
			if(LRep==RepType::ApproachingImaginaryMidLeft&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative==RightIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = IRep;
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
				Value.ConvertFromApproachingIMidRightToINum();
				RRep = RepType::INum;
			}
			break;
        }
			#endif
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
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
            PiNumOp(RRep, self, Value);
            break;
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
            ENumOp(RRep, self, Value);
            break;
#endif
		case RepType::NormalType:
#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
    #if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
    #endif
    #if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
        #if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingMidRight:
        #endif
    #endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
    #if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #if defined(AltNum_EnableEFractional)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
			if (RRep == RepType::MixedFrac)
			{
				self.ConvertToNormType(&LRep);
				self.BasicMixedFracOp(Value);
			}
#if defined(AltNum_EnableMixedPiFractional)
            else if(RRep==RepType::MixedPi)
#elif defined(AltNum_EnableMixedEFractional)
            else if(RRep==RepType::MixedE)
#endif
#if defined(AltNum_MixedPiOrEEnabled)
    			self.ConvertToNormType(&LRep);
    		#if defined(AltNum_EnableMixedPiFractional)
    			self.BasicMixedPiFracOp(Value);
    		#else
    			self.BasicMixedEFracOp(Value);
    		#endif
#else
            else
#endif
			    self.CatchAllAddition(&Value, &LRep, &RRep);
			break;
#if defined(AltNum_EnableImaginaryNum)//Replace with specific code instead of catchall code later
		case RepType::INum:
    #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://  (Value/(-ExtraRep))*i Representation
    #elif defined(AltNum_EnableFractionals)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
    #endif
	#if defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI:
    #endif
			ImaginaryNumberOp(LRep, RRep, self, Value); 
            break;
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
            MixedFracOp(RRep, self, Value);
            break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
            MixedPiEOp(RRep, self, Value);
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
			throw AltDec::RepTypeAsString(LRep)+" RepType addition with "+AltDec::RepTypeAsString(RRep)+" not supported yet";
			break;
	}
}