#include "AltDec.hpp"

#if defined(AltNum_EnableMixedFractional)
//MixedFracRtRDivOp
static void MediumDecVariant::MixedFracDivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
	switch(LRep)
	{
		case RepType::NormalType:
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
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
	#endif
				default://Shouldn't be used
					break;
			}
			break;
	#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
		#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
		#endif
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
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
	#endif
				default://Shouldn't be used
					break;
			}
			break;
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
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
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
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
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
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
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
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
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
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					#if defined(AltNum_EnableDecimaledPiFractionals)
					#else
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
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
					break;
		#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					int RightSideNum = Value.IntValue==0?-Value.DecimalHalf:(Value.IntValue*-Value.ExtraRep)-Value.DecimalHalf;
		#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
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
			throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet from MixedFracRtR";
		default:
			self.CatchAllDivision(Value, LRep, RRep);
			break;
	#endif
}
#endif