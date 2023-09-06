#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

inline void BlazesRusCode::AltDec::NormalRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
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

#if defined(AltNum_EnablePiRep)
inline void BlazesRusCode::AltDec::PiNumRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
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

#if defined(AltNum_EnableERep)
inline void BlazesRusCode::AltDec::ENumRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
    #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertENumToNum();
			self.BasicMixedFracRtRSubtraction(Value);
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

#endif

#if defined(AltNum_EnableImaginaryNum)
inline void BlazesRusCode::AltDec::ImaginaryNumberRtRSubtraction(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
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
inline void BlazesRusCode::AltDec::MixedFracRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
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
inline void BlazesRusCode::AltDec::MixedPiOrERtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
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

/*
#if defined(AltNum_EnableFractionals)
inline void BlazesRusCode::AltDec::NumByDivRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnablePiPowers)
inline void BlazesRusCode::AltDec::PiPowerRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
{
}
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)
inline void BlazesRusCode::AltDec::PiNumByDivRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
{
}
#endif
#if defined(AltNum_EnablePiFractional)
inline void BlazesRusCode::AltDec::PiFractionalRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
{
}
#endif
#if defined(AltNum_EnableEFractional)
inline void BlazesRusCode::AltDec::EFractionalRtRSubtraction(RepType RRep, AltDec& self, AltDec& Value)
{
}
#endif
#if defined(AltNum_EnableDecimaledPiOrEFractionals)
inline void BlazesRusCode::AltDec::PiOrEByDivSubtraction(RepType RRep, AltDec& self, AltDec& Value)
{
}
#endif
*/