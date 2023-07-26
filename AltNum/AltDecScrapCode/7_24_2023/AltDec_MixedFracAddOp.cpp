#include "AltDec.hpp"

#if defined(AltNum_EnableMixedFractional)
//MixedFracRtRAddOp
static void MediumDecVariant::MixedFracAddOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
	switch(LRep)
	{
		/*case RepType::NormalType:
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					if(self.DecimalHalf==0)//Avoid needing to convert if Leftside is not decimal format representation
                    {
                        if(self.IntValue<0)
                        {
                            if(Value.IntValue==NegativeRep)
                            {
                                self.DecimalHalf = Value.DecimalHalf;
                                self.ExtraRep = Value.ExtraRep;
                            }
                            else if(Value.IntValue<0)
                            {
                                self.IntValue += Value.IntValue;
                                self.DecimalHalf = Value.DecimalHalf;
                                self.ExtraRep = Value.ExtraRep;
                            }
                            else//(Value.IntValue>0)
                            {
                                if(Value.IntValue>-self.IntValue)//check for flipping of sign
                                {
                                    self.IntValue += Value.IntValue - 1;
                                    self.DecimalHalf = Value.ExtraRep - Value.DecimalHalf;
                                }
                                else
                                {
                                    self.IntValue += Value.IntValue;
                                    self.DecimalHalf = Value.ExtraRep - Value.DecimalHalf;
                                }
                                self.ExtraRep = Value.ExtraRep;
                            }
                        }
                        else//(self.IntValue>0)
                        {
                            if(Value.IntValue==NegativeRep)
                            {
                                self.DecimalHalf = Value.ExtraRep - Value.DecimalHalf;
                                self.ExtraRep = Value.ExtraRep;
                            }
                            else if(Value.IntValue<0)
                            {
                                self.IntValue += Value.IntValue;
                                if(-Value.IntValue>self.IntValue)//check for flipping of sign
                                {
                                    self.IntValue += Value.IntValue;
                                    if(self.IntValue==-1)
                                        self.IntValue = NegativeRep;
                                    else
                                        ++self.IntValue;
                                }
                                self.DecimalHalf = Value.ExtraRep - Value.DecimalHalf;
                                self.ExtraRep = Value.ExtraRep;
                            }
                            else//(Value.IntValue>0)
                            {
                                self.IntValue += Value.IntValue;
                                self.DecimalHalf = Value.DecimalHalf;
                                self.ExtraRep = Value.ExtraRep;
                            }
                        }     
                    }
                    else
                    {
                        MediumDecVariant RightSideNum = MediumDecVariant(Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf);
                        self.BasicMultOp(Value.ExtraRep);
                        self += RightSideNum;
                        if(self.DecimalHalf==0)
                        {
                            if(self.IntValue!=0)//Set as Zero if both are zero
                            {
                                self.DecimalHalf = -self.DecimalHalf;
                                self.ExtraRep = Value.ExtraRep;
                            }
                        }
                        else
                        {
                            if(self.IntValue!=0&&self.IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
                                self.DecimalHalf = -self.DecimalHalf;
                            self.ExtraRep = Value.ExtraRep;
                        }
                    }
                    break;
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
    #endif
    #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
					//Converting into PiNum
                    MediumDecVariant RightSideNum = MediumDecVariant(Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep) - Value.DecimalHalf);
		#if defined(AltNum_EnableMixedPiFractional)
                    RightSideNum *= PiNum;
        #else
                    RightSideNum *= ENum;
        #endif
                    self.BasicMultOp(-Value.ExtraRep);
                    self += RightSideNum;
                    if(self.DecimalHalf==0)
                    {
                        if(self.IntValue!=0)//Set as Zero if both are zero
                        {
                            self.DecimalHalf = -self.DecimalHalf;
                            self.ExtraRep = -Value.ExtraRep;
                        }
                    }
                    else
                    {
                        if(self.IntValue!=0&&self.IntValue!=NegativeRep)//Turn into NumByDiv instead of mixed fraction if
                            self.DecimalHalf = -self.DecimalHalf;
                        self.ExtraRep = -Value.ExtraRep;
                    }
                    break;
    #endif
    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
        #if defined(AltNum_EnableComplexNum)
	                throw "Complex number code not implimented yet.";
        #else
	                throw "Complex number operation not enabled currently.";
        #endif
                    break;
    #endif
				default://Shouldn't be used
					//Value.ConvertToNormType(RRep);
	                //self.BasicAddOp(Value);
					break;
			}
			break;*/
	#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;//Return MixedFrac or NormalType
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					break;//Return MixedPi
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
                    break;//Return MixedFrac or NormalType
   #endif
    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
        #if defined(AltNum_EnableComplexNum)
	                throw "Complex number code not implimented yet.";
        #else
	                throw "Complex number operation not enabled currently.";
        #endif
    #endif
				default://Shouldn't be used
					break;
			}
			break;
		#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;//Return MixedFrac or NormalType
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					break;//Return MixedPi
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
                    break;//Return MixedFrac or NormalType
   #endif
    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
        #if defined(AltNum_EnableComplexNum)
	                throw "Complex number code not implimented yet.";
        #else
	                throw "Complex number operation not enabled currently.";
        #endif
    #endif
				default://Shouldn't be used
					break;
			}
			break;
		#endif
	#endif
	#if defined(AltNum_EnableENum)
		case RepType::ENum:
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					break;
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
    #endif
    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
        #if defined(AltNum_EnableComplexNum)
	                throw "Complex number code not implimented yet.";
        #else
	                throw "Complex number operation not enabled currently.";
        #endif
    #endif
				default://Shouldn't be used
					break;
			}
			break;
	#endif
		case RepType::NumByDiv:
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					break;
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
    #endif
    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
        #if defined(AltNum_EnableComplexNum)
	                throw "Complex number code not implimented yet.";
        #else
	                throw "Complex number operation not enabled currently.";
        #endif
    #endif
				default://Shouldn't be used
					break;
			}
			break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiRep)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					break;
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
    #endif
    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
        #if defined(AltNum_EnableComplexNum)
	                throw "Complex number code not implimented yet.";
        #else
	                throw "Complex number operation not enabled currently.";
        #endif
    #endif
				default://Shouldn't be used
					break;
			}
			break;
			
		#endif
		#if defined(AltNum_EnableERep)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					break;
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
    #endif
    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
        #if defined(AltNum_EnableComplexNum)
	                throw "Complex number code not implimented yet.";
        #else
	                throw "Complex number operation not enabled currently.";
        #endif
    #endif
				default://Shouldn't be used
					break;
			}
			break;
			
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
		#endif
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					#if defined(AltNum_EnableDecimaledPiFractionals)
					#else
					#endif
					break;
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					#if defined(AltNum_EnableDecimaledPiFractionals)
					#else
					#endif
    #endif
    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
        #if defined(AltNum_EnableComplexNum)
	                throw "Complex number code not implimented yet.";
        #else
	                throw "Complex number operation not enabled currently.";
        #endif
    #endif
				default://Shouldn't be used
					break;
			}
			break;
	#endif
		case RepType::MixedFrac:
			//Convert both sides to Fractionals first
			//self.IntValue +- (-self.DecimalHalf/self.ExtraRep) = 
			int LeftSideNum;
			if(self.IntValue==NegativeRep)
				LeftSideNum = self.DecimalHalf;
			else if(self.IntValue<0)
				LeftSideNum = self.IntValue*self.ExtraRep + self.DecimalHalf;
			else if(self.IntValue==0)
				LeftSideNum = -self.DecimalHalf;
			else
				LeftSideNum = self.IntValue*self.ExtraRep - self.DecimalHalf;
			switch(RRep)
			{
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					break;
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
	#endif
				default://Shouldn't be used
					break;
			}
			break;
	#if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi:
			int LeftSideNum;
			if(self.IntValue==NegativeRep)
				LeftSideNum = self.DecimalHalf;
			else if(self.IntValue<0)
				LeftSideNum = self.IntValue*-self.ExtraRep + self.DecimalHalf;
			else if(self.IntValue==0)
				LeftSideNum = -self.DecimalHalf;
			else
				LeftSideNum = self.IntValue*-self.ExtraRep + -self.DecimalHalf;
			break;
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;
				default://Shouldn't be used
					break;
			}
			break;
	#elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			int LeftSideNum;
			if(self.IntValue==NegativeRep)
				LeftSideNum = self.DecimalHalf;
			else if(self.IntValue<0)
				LeftSideNum = self.IntValue*-self.ExtraRep + self.DecimalHalf;
			else if(self.IntValue==0)
				LeftSideNum = -self.DecimalHalf;
			else
				LeftSideNum = self.IntValue*-self.ExtraRep + -self.DecimalHalf;
			break;
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:Value.IntValue*Value.ExtraRep - Value.DecimalHalf;
					break;
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
		case RepType::IFractional:
		case RepType::INumByDiv:
		#if defined(AltNum_EnableMixedEFractional)
		case RepType::MixedI:
		#endif
			throw static_cast<RepType>(LRep)-" RepType addition with"-static_cast<RepType>(RRep)-"not supported yet from MixedFracRtR";
		default:
			self.CatchAllAddition(Value, LRep, RRep);
			break;
	#endif
}
#endif