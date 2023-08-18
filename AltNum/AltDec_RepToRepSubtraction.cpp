#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalSubOp(RepType& RRep, AltDec& self, AltDec& Value)
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
			Value.ConvertToNormType(&RRep);
			self.BasicSubOp(Value);
			break;
	}
}

#if defined(AltNum_EnableFractionals)
void NumByDivSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
			self.ConvertToNormType(&LRep);
        #if defined(AltNum_EnableMixedPiFractional)
			self.BasicMixedPiFracSubOp(Value);
        #else
			self.BasicMixedEFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, RepType::NumByDiv, &RRep);
			break;
	}
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumSubOp(RepType& RRep, AltDec& self, AltDec& Value)
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
			self.ConvertToNormType(&LRep);
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
    	            self.ConvertToNormType(&LRep);
    	            self.BasicMixedPiFracSubOp(Value);
    			}
            #elif defined(AltNum_EnableDecimaledPiFractionals)//Result is DecimaledPiFractional
                if(Value.IntValue!=0)
                    self.IntHalfSubtractionOp(Value.IntValue);
                if(self.IntValue==0)
                {
                    self.IntValue = Value.DecimalHalf;
                }
                else
                {
                    self.IntValue *= -Value.ExtraRep;
                    self.IntHalfAdditionOp(&self.DecimalHalf);//adding as negative of the representation
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
			self.CatchAllSubtraction(&Value, &LRep, &RRep);
			break;
	}
}

//	#if defined(AltNum_EnablePiPowers)
//void PiPowerSubOp(RepType& RRep, AltDec& self, AltDec& Value)
//{
//	switch (RRep)
//	{
//		case RepType::PiNum:
//    #if defined(AltNum_EnableMixedFractional)
//		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
//			self.ConvertToNormType(&LRep);
//			self.BasicMixedFracSubOp(Value);
//			break;
//    #if defined(AltNum_EnableMixedPiFractional)
//		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
//    #elif defined(AltNum_EnableMixedEFractional)
//		case RepType::MixedE:
//    #endif
//    #if defined(AltNum_MixedPiOrEEnabled)
//        self.ConvertToNormType(&LRep);
//	    #if defined(AltNum_EnableMixedEFractional)
//			self.BasicMixedEFracSubOp(Value);
//        #else
//			self.BasicMixedPiFracSubOp(Value);
//	    #endif
//			break;
//    #endif
//#endif
//		default:
//			self.CatchAllSubtraction(&Value, &LRep, &RRep);
//			break;
//	}
//}
	
	#if defined(AltNum_EnableDecimaledPiFractionals)
/*
void PiNumByDivSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedEFractional)
			self.ConvertToNormType(&LRep);
			self.BasicMixedEFracSubOp(Value);
        #else
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(&LRep);
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, &LRep, &RRep);
			break;
	}
}
	#elif defined(AltNum_EnablePiFractional)
void PiFractionalSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedEFractional)
			self.ConvertToNormType(&LRep);
			self.BasicMixedEFracSubOp(Value);
        #else
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(&LRep);
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, &LRep, &RRep);
			break;
	}
}
	#endif
*/
#endif

#if defined(AltNum_EnableERep)
void ENumSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
    #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
        #if defined(AltNum_EnableMixedPiFractional)
			self.ConvertToNormType(&LRep);
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
                self.ExtraRep = EByDivisorRep;
			}
			else
			{
	            self.ConvertToNormType(&LRep);
	            self.BasicMixedPiFracSubOp(Value);
			}
            #elif defined(AltNum_EnableDecimaledEFractionals)//Result is DecimaledEFractional
            if(Value.IntValue!=0)
                self.IntHalfSubtractionOp(Value.IntValue);
            if(self.IntValue==0)
            {
                self.IntValue = Value.DecimalHalf;
            }
            else
            {
                self.IntValue *= -Value.ExtraRep;
                self.IntHalfAdditionOp(&self.DecimalHalf);//adding as negative of the representation
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
			self.CatchAllSubtraction(&Value, &LRep, &RRep);
			break;
	}
}

/*
	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedPiFractional)
			self.ConvertToNormType(&LRep);
			self.BasicMixedEFracSubOp(Value);
        #else
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(&LRep);
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, &LRep, &RRep);
			break;
	}
}
	#elif defined(AltNum_EnableEFractional)
void EFractionalSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedPiFractional)
			self.ConvertToNormType(&LRep);
			self.BasicMixedEFracSubOp(Value);
        #else
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(&LRep);
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, &LRep, &RRep);
			break;
	}
}
	#endif
*/
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
void MixedFracSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
			switch (RRep)
			{
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	#elif defined(AltNum_EnableMixedEFractional)
				//case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
        //#if defined(AltNum_EnableMixedPiFractional)&&AltNum_EnableDecimaledPiFractionals
        //            break;//Give result as PiByDiv
        //#elif defined(AltNum_EnableMixedEFractional)&&AltNum_EnableDecimaledEFractionals
        //            break;//Give result as EByDiv
        //#else
			        AltDec LeftSideNum;
			        if(self.IntValue==NegativeRep)
				        LeftSideNum = AltDec(self.DecimalHalf);
			        else if(self.IntValue<0)
				        LeftSideNum = AltDec(self.IntValue*self.ExtraRep + self.DecimalHalf);
			        else if(self.IntValue==0)
				        LeftSideNum = AltDec(-self.DecimalHalf);
			        else
				        LeftSideNum = AltDec(self.IntValue*self.ExtraRep - self.DecimalHalf);
                    AltDec RightSideNum = AltDec(Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf);
	    #if defined(AltNum_EnableMixedPiFractional)
                    RightSideNum *= PiNum;
        #else
                    RightSideNum *= ENum;
        #endif
                    int InvertedVDivisor = -Value.ExtraValue;
                    if(self.ExtraRep==InvertedVDivisor)
                    {
                        LeftSideNum.BasicSubOp(RightSideNum);
                        self.IntValue = LeftSide.IntValue;
                        self.DecimalHalf = LeftSide.DecimalHalf;
                    }
                    else
                    {
                        self.ExtraRep *= InvertedVDivisor;
                        LeftSideNum *= InvertedVDivisor;
                        RightSideNum *= self.ExtraRep;
                        LeftSideNum.BasicSubOp(RightSideNum);
                        self.IntValue = LeftSide.IntValue;
                        self.DecimalHalf = LeftSide.DecimalHalf;
                    }
                    break;//Give result as NumByDiv
        //#endif
    #endif
				default:
					self.CatchAllSubtraction(&Value, &LRep, &RRep);
					break;
            }
}
	#if defined(AltNum_EnableMixedPiFractional)||AltNum_EnableMixedEFractional
void MixedPiESubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
			switch (RRep)
			{
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
                        LeftSideNum.BasicSubOp(RightSideNum);
                        self.IntValue = LeftSide.IntValue;
                        self.DecimalHalf = LeftSide.DecimalHalf;
                    }
                    else
                    {
                        self.ExtraRep *= Value.ExtraRep;
                        LeftSideNum *= Value.ExtraRep;
                        RightSideNum *= self.ExtraRep;
                        LeftSideNum.BasicSubOp(RightSideNum);
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
				default:
					self.CatchAllSubtraction(&Value, &LRep, &RRep);
					break;
            }
#endif

inline void BlazesRusCode::AltDec::RepToRepSubOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
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
    //RRep Overrides before Main RepToRep Code
    switch(RRep)
    {
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
		case RepType::ApproachingBottomDiv:
        #if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTopDiv:
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
    			self.ConvertToNormType(&LRep);
    			self.BasicMixedFracSubOp(Value);
#if defined(AltNum_EnableMixedPiFractional)
            else if(RRep==RepType::MixedPi)
#elif defined(AltNum_EnableMixedEFractional)
            else if(RRep==RepType::MixedE)
#endif
#if defined(AltNum_MixedPiOrEEnabled)
    			self.ConvertToNormType(&LRep);
    		#if defined(AltNum_EnableMixedPiFractional)
    			self.BasicMixedPiFracSubOp(Value);
    		#else
    			self.BasicMixedEFracSubOp(Value);
    		#endif
#endif
            else
#endif
			    self.CatchAllSubtraction(&Value, &LRep, &RRep);
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
			throw "Can't perform operations with NaN or Undefined number";
			break;
	#endif
		default:
			throw AltDec::RepTypeAsString(LRep)+" RepType subtraction with "+AltDec::RepTypeAsString(RRep)+" not supported yet";
			break;
	}
}