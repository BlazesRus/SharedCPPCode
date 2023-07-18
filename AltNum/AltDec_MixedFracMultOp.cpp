#include "AltDec.hpp"

//MixedFracRtRMultOp
static void MediumDecVariant::MixedFracMultOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
{
	switch(LRep)
	{
		case RepType::NormalType:
	#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
		#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
		#endif
	#endif
	#if defined(AltNum_EnableENum)
		case RepType::ENum:
	#endif
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
					MediumDecVariant LeftSide = self*Value.IntValue;
					MediumDecVariant RightSide = self/Value.ExtraRep;
					RightSide *= -Value.DecimalHalf;
					self = LeftSide+RightSide;
					break;
				#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
					MediumDecVariant LeftSide = self*Value.IntValue;
					MediumDecVariant RightSide = self/-Value.ExtraRep;
					RightSide *= -Value.DecimalHalf;
					self = LeftSide+RightSide;
					switch(LRep)
					{
					#if defined(AltNum_EnablePiRep)
						case RepType::PiNum:
						#if defined(AltNum_EnablePiPowers)//Become Value*Pi^2
							self.ExtraRep = -1;
						#else
							self.BasicDivOp(PiNumValue());
						#endif
							break;
						#if defined(AltNum_EnablePiPowers)
						case RepType::PiPower:
						   ++self.ExtraRep;
						   break;
						#endif
					#endif
						//RepType::ENum:
						default://RepType::NormalType
							self.BasicMultOp(PiNumValue());
							break;
					}
					break;
			#endif
			#if defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
					MediumDecVariant LeftSide = self*Value.IntValue;
					MediumDecVariant RightSide = self/-Value.ExtraRep;
					RightSide *= -Value.DecimalHalf;
					self = LeftSide+RightSide;
					self.BasicMultOp(ENumValue());
					break;
			#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet from MixedFracRtR";
					break;
			#endif
				default:
					throw "Incorrect RepToRep function used(Or not enabled).";
					break;
			}
			break;
		case RepType::NumByDiv:
			switch(RRep)
			{
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
				//AltDec(self.IntValue, self.DecimalHalf)/self.ExtraRep * Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep
				//= Value.IntValue*(AltDec(self.IntValue, self.DecimalHalf)/self.ExtraRep) + (AltDec(self.IntValue, self.DecimalHalf)/self.ExtraRep)((-Value.DecimalHalf)/Value.ExtraRep)
				//= (Value.IntValue*AltDec(self.IntValue, self.DecimalHalf)/self.ExtraRep) + (AltDec(self.IntValue, self.DecimalHalf)*-Value.DecimalHalf)/(self.ExtraRep*Value.ExtraRep)
				//= ((Value.IntValue*AltDec(self.IntValue, self.DecimalHalf))+((AltDec(self.IntValue, self.DecimalHalf)*-Value.DecimalHalf)/Value.ExtraRep))/self.ExtraRep
					MediumDecVariant NumSide = AltDec(self.IntValue, self.DecimalHalf)*-Value.DecimalHalf;
					NumSide.BasicDivOp(Value.ExtraRep);//Avoid converting to NumByDiv because need to combine with (Value.IntValue*AltDec(self.IntValue, self.DecimalHalf))/self.ExtraRep
					NumSide.BasicAddOp(AltDec(self.IntValue, self.DecimalHalf)*Value.IntValue);
					self.SetFractionalVal(NumSide, self.ExtraRep);
					break;
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
				//AltDec(self.IntValue, self.DecimalHalf)/self.ExtraRep * Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
					MediumDecVariant NumSide = AltDec(self.IntValue, self.DecimalHalf)*-Value.DecimalHalf;
					NumSide.BasicDivOp(-Value.ExtraRep);//Avoid converting to NumByDiv because need to combine with (Value.IntValue*AltDec(self.IntValue, self.DecimalHalf))/self.ExtraRep
					NumSide.BasicAddOp(AltDec(self.IntValue, self.DecimalHalf)*Value.IntValue);
		#if (defined(AltNum_EnableMixedPiFractional)&&defined(AltNum_EnableDecimaledPiFractionals))||(defined(AltNum_EnableMixedEFractional)&&defined(AltNum_EnableDecimaledEFractionals))
					self.SetFractionalVal(NumSide, -self.ExtraRep);
		#else
					NumSide.BasicDivOp(self.ExtraRep);
					self.IntValue = NumSide.IntValue; self.DecimalHalf = NumSide.DecimalHalf;
			#if defined(AltNum_EnableMixedPiFractional)
						self.ExtraRep = PiRep;
			#else
						self.ExtraRep = ERep;
			#endif
		#endif
	#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet from MixedFracRtR";
					break;
	#endif
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
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
					self.CatchAllMultiplication(Value, LRep, RRep);
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
			self.CatchAllMultiplication(Value, LRep, RRep);
			break;
	#endif
}