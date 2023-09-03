#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
		case RepType::NormalType://Fail safe for when converted before switch
			self.BasicSubOp(Value);
			break;
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedPiFractional) 
			self.BasicMixedPiFracSubOp(Value);
	    #else
			self.BasicMixedEFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			Value.ConvertToNormType(RRep);
			self.BasicSubOp(Value);
			break;
	}
}

#if defined(AltNum_EnableFractionals)
void NumByDivSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
    #if defined(AltNum_EnablePiPowers)//Combine PiPower in certain cases
    #endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(RepType::PiNum);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedEFractional)
			self.ConvertToNormType(RepType::);
			self.BasicMixedEFracSubOp(Value);
        #else
            #if defined(AltNum_EnablePiFractional)
    			if(self.DecimalHalf==0)//Result is PiFractional
    			{
                    int divisor = -Value.ExtraRep;
                    if(Value.IntValue!=0)
                        self.IntValue -= Value.IntValue;
                    if(self.IntValue==0)
                    {
                        self.IntValue = Value.DecimalHalf;
                    }
                    else
                    {
                        self.IntValue *= divisor;
                        self.IntValue += self.DecimalHalf;//adding as negative of the representation
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
    	            self.ConvertToNormType(RepType::);
    	            self.BasicMixedPiFracSubOp(Value);
    			}
            #elif defined(AltNum_EnableDecimaledPiFractionals)//Result is DecimaledPiFractional
                if(Value.IntValue!=0)
                    self.IntHalfSubtraction(Value.IntValue);
                if(self.IntValue==0)
                {
                    self.IntValue = Value.DecimalHalf;
                }
                else
                {
                    self.IntValue *= -Value.ExtraRep;
                    self.IntHalfAddition(self.DecimalHalf);//adding as negative of the representation
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
			self.CatchAllSubtraction(Value, RepType::PiNum, RRep);
			break;
	}
}

	#if defined(AltNum_EnablePiPowers)
void PiPowerSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	
	#if defined(AltNum_EnableDecimaledPiFractionals)
void PiNumByDivSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnablePiFractional)
void PiFractionalSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableERep)
void ENumSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
    #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertENumToNum();
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
        #if defined(AltNum_EnableMixedPiFractional)
			self.ConvertENumToNum();
			self.BasicMixedPiFracSubOp(Value); return;
        #else
            #if defined(AltNum_EnableEFractional)
			if(self.DecimalHalf==0)//Result is EFractional
			{
                int divisor = -Value.ExtraRep;
                if(Value.IntValue!=0)
                    self.IntValue -= Value.IntValue;
                if(self.IntValue==0)
                {
                    self.IntValue = Value.DecimalHalf;
                }
                else
                {
                    self.IntValue *= divisor;
                    self.IntValue += self.DecimalHalf;//adding as negative of the representation
                    if(self.IntValue==0)//Become zero instead of fractional
                    {
                        self.ExtraRep = 0; return;
                    }
                }
                self.DecimalHalf = divisor;
                self.ExtraRep = AltDec::EByDivisorRep;
			}
			else
			{
	            self.ConvertENumToNum();
	            self.BasicMixedEFracSubOp(Value);
			}
            #elif defined(AltNum_EnableDecimaledEFractionals)//Result is DecimaledEFractional
            if(Value.IntValue!=0)
                self.IntHalfSubtraction(Value.IntValue);
            if(self.IntValue==0)
            {
                self.IntValue = Value.DecimalHalf;
            }
            else
            {
                self.IntValue *= -Value.ExtraRep;
                self.IntHalfAddition(self.DecimalHalf);//adding as negative of the representation
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
			self.CatchAllSubtraction(Value, RepType::ENum, RRep);
			break;
	}
}

	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#elif defined(AltNum_EnableEFractional)
void EFractionalSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif

#if defined(AltNum_EnableApproachingValues)
void ApproachingBottomSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingTopSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidRightSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

		#if !defined(AltNum_DisableApproachingTop)
void ApproachingMidLeftSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
		#endif
	#endif
#endif

#if defined(AltNum_EnableImaginaryNum)
void ImaginaryNumberSubOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
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
            self.CatchAllImaginarySubtraction(Value, LRep, RRep);
            break;
        default:
            throw AltDec::RepTypeAsString(LRep)+" RepType subtraction with"+AltDec::RepTypeAsString(RRep)+"not supported yet";
	        break;
    }
}
#endif

#if defined(AltNum_EnableMixedFractional)
void MixedFracSubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
		#if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
		#elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
		#endif
		#if defined(AltNum_MixedPiOrEEnabled)
		{
			//#if defined(AltNum_MixedPiHasDecimaledFracAccess)
			//            break;//Give result as PiByDiv
			//#elif defined(AltNum_MixedEHasDecimaledFracAccess)
			//            break;//Give result as EByDiv
			//#else
			AltDec LeftSide;
			if (self.IntValue == AltDec::NegativeRep)
				LeftSide = AltDec(self.DecimalHalf);
			else if (self.IntValue < 0)
				LeftSide = AltDec(self.IntValue * self.ExtraRep + self.DecimalHalf);
			else if (self.IntValue == 0)
				LeftSide = AltDec(-self.DecimalHalf);
			else
				LeftSide = AltDec(self.IntValue * self.ExtraRep - self.DecimalHalf);
			AltDec RightSide = AltDec(Value.IntValue == 0 ? -Value.DecimalHalf : (Value.IntValue * -Value.ExtraRep) - Value.DecimalHalf);
			#if defined(AltNum_EnableMixedPiFractional)
			RightSide *= AltDec::PiNum;
			#else
			RightSide *= AltDec::ENum;
			#endif
			int InvertedVDivisor = -Value.ExtraRep;
			if (self.ExtraRep == InvertedVDivisor)
			{
				LeftSide.BasicSubOp(RightSide);
				self.IntValue = LeftSide.IntValue;
				self.DecimalHalf = LeftSide.DecimalHalf;
			}
			else
			{
				self.ExtraRep *= InvertedVDivisor;
				LeftSide *= InvertedVDivisor;
				RightSide *= self.ExtraRep;
				LeftSide.BasicSubOp(RightSide);
				self.IntValue = LeftSide.IntValue;
				self.DecimalHalf = LeftSide.DecimalHalf;
			}
			break;//Give result as NumByDiv
			//#endif
		}
		#endif
		default:
			self.CatchAllSubtraction(Value, RepType::MixedFrac, RRep);
			break;
	}
}
#endif
#if defined(AltNum_MixedPiOrEEnabled)
void MixedPiESubOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
		case RepType::MixedFrac:
		{
			//Left side Numerator
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
			//Right side Numerator
			int RightSide = Value.IntValue == 0 ? -Value.DecimalHalf : Value.IntValue * Value.ExtraRep - Value.DecimalHalf;
			self.ExtraRep = -self.ExtraRep;
			if (Value.ExtraRep == self.ExtraRep)
			{
				LeftSide.BasicSubOp(RightSide);
				self.IntValue = LeftSide.IntValue;
				self.DecimalHalf = LeftSide.DecimalHalf;
			}
			else
			{
				self.ExtraRep *= Value.ExtraRep;
				LeftSide *= Value.ExtraRep;
				RightSide *= self.ExtraRep;
				LeftSide.BasicSubOp(RightSide);
				self.IntValue = LeftSide.IntValue;
				self.DecimalHalf = LeftSide.DecimalHalf;
			}
			break;
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
		}
		default:
	#if defined(AltNum_EnableMixedPiFractional)
			self.CatchAllSubtraction(Value, RepType::MixedPi, RRep);
	#else
			self.CatchAllSubtraction(Value, RepType::MixedE, RRep);
	#endif
			break;
	}
}
#endif

inline void BlazesRusCode::AltDec::RepToRepSubOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec Value)
{
	bool LeftIsNegative = self.IntValue<0;
#if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)//LRep Overrides
    switch(LRep)
    {
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        case RepType::UndefinedButInRange:
			throw "UndefinedButInRange SubOperations not supported yet(from left side)";
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
			BlazesRusCode::AltDec::LRepImaginaryOverridePt2(LRep, RRep, self, Value);
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
		{
			if(LRep==RepType::ApproachingTop&&LeftIsNegative)
			{
				self.DecimalHalf = 0;
                if(self.IntValue==NegativeRep)
                    self.IntValue = -1 - Value.GetIntHalf();
                else
                    self.IntValue -= Value.GetIntHalf() + 1;
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
			if(LRep==RepType::ApproachingBottom&&LeftIsNegative)
			{
				self.DecimalHalf = 0;
                if(self.IntValue==NegativeRep)
                    self.IntValue = -1 - Value.GetIntHalf();
                else
                    self.IntValue -= Value.GetIntHalf() + 1;
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
			if(LRep==RepType::ApproachingMidRight&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = 0;
                if(self.IntValue==NegativeRep)
                    self.IntValue = -1 - Value.GetIntHalf();
                else
                    self.IntValue -= Value.GetIntHalf() + 1;
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
		{//-0.50..1-0.0.49..9
			if(LRep==RepType::ApproachingMidLeft&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = 0;
                if(self.IntValue==NegativeRep)
                    self.IntValue = -1 - Value.GetIntHalf();
                else
                    self.IntValue -= Value.GetIntHalf() + 1;
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
		case RepType::ApproachingImaginaryBottom:
		{//-1.0..1 - 0.9..9
			if(LRep==RepType::ApproachingImaginaryTop&&LeftIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = IRep;
                if(self.IntValue==NegativeRep)
                    self.IntValue = -1 - Value.GetIntHalf();
                else
                    self.IntValue -= Value.GetIntHalf() + 1;
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
			if(LRep==RepType::ApproachingImaginaryBottom)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = IRep;
                if(self.IntValue==NegativeRep)
                    self.IntValue = -1 - Value.GetIntHalf();
                else
                    self.IntValue -= Value.GetIntHalf() + 1;
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
			if(LRep==RepType::ApproachingImaginaryMidRight&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = IRep;
                if(self.IntValue==NegativeRep)
                    self.IntValue = -1 - Value.GetIntHalf();
                else
                    self.IntValue -= Value.GetIntHalf() + 1;
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
			if(LRep==RepType::ApproachingImaginaryMidLeft&&self.ExtraRep==Value.ExtraRep&&LeftIsNegative)
			{
				self.DecimalHalf = 0;
				self.ExtraRep = IRep;
                if(self.IntValue==NegativeRep)
                    self.IntValue = -1 - Value.GetIntHalf();
                else
                    self.IntValue -= Value.GetIntHalf() + 1;
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
			throw "UndefinedButInRange SubOperations not supported yet(from right side)";
            break;
    #endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform SubOperations with NaN or Undefined number";
			break;
	#endif
		default:
			break;
	}
    switch (LRep)//Main switch block starts here
    {
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
            PiNumSubOp(RRep, self, Value);
            break;
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
            ENumSubOp(RRep, self, Value);
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
            if(RRep==RepType::MixedFrac)
			{
				self.ConvertToNormType(LRep);
				self.BasicMixedFracSubOp(Value);
			}
	#if defined(AltNum_EnableMixedPiFractional)
            else if(RRep==RepType::MixedPi)
	#elif defined(AltNum_EnableMixedEFractional)
            else if(RRep==RepType::MixedE)
	#endif
	#if defined(AltNum_MixedPiOrEEnabled)
    			self.ConvertToNormType(LRep);
    		#if defined(AltNum_EnableMixedPiFractional)
    			self.BasicMixedPiFracSubOp(Value);
    		#else
    			self.BasicMixedEFracSubOp(Value);
    		#endif
	#else
            else
	#endif
			    self.CatchAllSubtraction(Value, LRep, RRep);
			break;
#endif
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
			ImaginaryNumberSubOp(LRep, RRep, self, Value);
            break;
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
            MixedFracSubOp(RRep, self, Value);
            break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
            MixedPiESubOp(RRep, self, Value);
            break;
    #endif
#endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform SubOperations with NaN or Undefined number";
			break;
	#endif
		default:
			throw AltDec::RepTypeAsString(LRep)+" RepType subtraction with "+AltDec::RepTypeAsString(RRep)+" not supported yet";
			break;
	}
}