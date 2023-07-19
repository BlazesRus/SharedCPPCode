#include "AltDec.hpp"

bool MediumDecVariant::RepToRepAddOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
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
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform operations with NaN or Undefined number";
			break;
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
		case RepType::UnknownType:
			throw "Can't perform operations with unknown/non-enabled format number";
			break;

		default://No nothing for most of them
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

		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform operations with NaN or Undefined number";
			break;
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
#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
#if defined(AltNum_EnableComplexNumbers)
					//place complex number operation code here later
#else
					throw "related imaginary format operation not supported yet";
#endif
					break;
#endif
					
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

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
//						case RepType::PiNum:
//						case RepType::PiPower:
//                      case RepType::ENum:
//						case RepType::NumByDiv:
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					Value.ConvertToNormType(RRep);
					self.BasicAddOp(Value);
					break;
			}
			break;
#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
		case RepType::PiNum:
			switch (RRep)
			{
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif
					
#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif

				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif

#if defined(AltNum_EnableENum)
		case RepType::ENum:
			switch (RRep)
			{
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif
	
#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			switch (RRep)
			{
#if defined(AltNum_EnableComplexNumbers)
				case RepType::INum://Only used when converted
					  //Add Complex number operation code here later
					  break;
				case RepType::ComplexIRep:
					break;
#endif										
					
#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
				default:
					throw static_cast<RepType>(LRep)-" RepType addition with"-static_cast<RepType>(RRep)-"not supported yet";
					break;
			}
			break;
#endif
			
#if defined(AltNum_EnablePiRep)&&defined(AltNum_EnablePiPowers)
		case RepType::PiNum:
			switch (RRep)
			{
#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
#endif
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
#endif
#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
#endif
#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
#endif
#endif

#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
		case RepType::PiPower:
			switch (RRep)
			{
				//case RepType::NormalType:
				//	break;
				//case RepType::PiNum://(IntValue.DecimalHalf)*Pi^-ExtraRep representation
				//	break;
#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
#endif
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif					

#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
#endif
#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
#endif
#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
#endif
#endif

#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif
			
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
			switch (RRep)
			{
//						case RepType::NormalType:
//							break;
#if defined(AltNum_EnablePiRep)
//						case RepType::PiNum:
//							break;
#if defined(AltNum_EnablePiPowers)
//						case RepType::PiPower:
//							//Add Pi powers code here later
//							break;
#endif
#endif
#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
#endif
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
	#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
	#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi:
#elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
#endif
#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
			switch (RRep)
			{
//						case RepType::NormalType:
//							break;
#if defined(AltNum_EnablePiRep)
//						case RepType::PiNum:
//							break;
#if defined(AltNum_EnablePiPowers)
//						case RepType::PiPower:
//							//Add Pi powers code here later
//							break;
#endif
#endif
	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
		#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
		#endif
		#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
	#endif
	#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
	#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif
#endif

#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			switch (RRep)
			{
#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
#endif
					self.IntValue = self.IntValue<0?NegativeRep:0;
					self.DecimalHalf = 1;
					self.ExtraRep = IRep;
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
#endif
//
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
		case RepType::ApproachingTop:
			switch (RRep)
			{
//						case RepType::NormalType:
//							break;
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
		#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
		#endif
		#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
	#endif
	#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
	#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;

#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingBottomDiv:
			switch (RRep)
			{
//						case RepType::NormalType:
//							break;
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
		#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
		#endif
		#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
	#endif
	#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
	#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
		case RepType::ApproachingTopDiv:
			switch (RRep)
			{
//						case RepType::NormalType:
//							break;
#if defined(AltNum_EnablePiRep)
//						case RepType::PiNum:
//							break;
#if defined(AltNum_EnablePiPowers)
//						case RepType::PiPower:
//							//Add Pi powers code here later
//							break;
#endif
#endif
#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
#endif
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
		#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
		#endif
		#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
	#endif
	#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
	#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
		case RepType::NumByDiv:
			switch (RRep)
			{
				case RepType::NormalType://Later normalize fractional when integer when viable
						self.BasicAddOp(Value.BasicMultOp(self.DecimalHalf));
						if(self.IntValue==0&&self.DecimalHalf==0)
							self.SetAsZero();
					break;
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//                            break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
//							
#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
#endif
#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
#endif
//
#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
#endif
#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
#endif
#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
					int ReversedVExtraRep = -ExtraRep;
					if(self.DecimalHalf==ReversedVExtraRep)
					{
						if(RRep==PiNumByDiv)
							Value.ConvertToNormType(RepType::PiNum);
						else
							Value.ConvertToNormType(RepType::ENum);
						self.BasicAddOp(Value.BasicMultOp(self.DecimalHalf));
						if(self.IntValue==0&&self.DecimalHalf==0)
							self.SetAsZero();
					}
					else
					{
						Value.ConvertToNormType(RRep);
						self.BasicAddOp(Value.BasicMultOp(self.DecimalHalf));
						if(self.IntValue==0&&self.DecimalHalf==0)
							self.SetAsZero();
					}
					break;
#endif
#endif

#if defined(AltNum_EnableMixedFractional)
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
					throw static_cast<RepType>(LRep)-" RepType addition with"-static_cast<RepType>(RRep)-"not supported yet";
					break;
					
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
					throw static_cast<RepType>(LRep)-" RepType addition with"-static_cast<RepType>(RRep)-"not supported yet";
					break;
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.BasicAddOp(Value.BasicMultOp(self.DecimalHalf));
					if(self.IntValue==0&&self.DecimalHalf==0)
						self.SetAsZero();
					break;
			}
			break;
			
#if defined(AltNum_EnablePiRep)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			switch (RRep)
			{
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif
					
#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
	#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
	#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif
#if defined(AltNum_EnableENum)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			switch (RRep)
			{
#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
#endif
				
#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
	#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
	#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		
#endif
#if defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
#endif
#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)//Only one of the 2 switches can be active at once
			switch (RRep)
			{
#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					//place complex number based code here later
					break;
#endif
					
#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
	#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
	#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif

			
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			switch (RRep)
			{
#if defined(AltNum_EnableComplexNumbers)
				case RepType::INum:
					  //Add Complex number operation code here later
					  break;
				case RepType::ComplexIRep:
					break;
#endif
					
#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
	#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
	#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
	#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
	#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
	#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
#endif
#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
#endif
				default:
					throw static_cast<RepType>(LRep)-" RepType addition with"-static_cast<RepType>(RRep)-"not supported yet";
					break;
			}
			break;
#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			switch (RRep)
			{
	#if defined(AltNum_EnableComplexNumbers)
				case RepType::INum:
					  //Add Complex number operation code here later
					  break;
				case RepType::ComplexIRep:
					break;
	#endif
				
	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
		#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
		#endif
		#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
		#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
		#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
					MixedIFracRtRAddOp(LRep, RRep, self, Value);
					break;
			#endif
	#endif
	#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
	#endif
				default:
					throw static_cast<RepType>(LRep)-" RepType subtraction with"-static_cast<RepType>(RRep)-"not supported yet";
					break;
			}
			break;
#endif
			
if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
			switch (RRep)
			{
//						case RepType::NormalType:
//							break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
//						case RepType::PiNum:
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
	#endif
//							
	#if defined(AltNum_EnablePiRep)&&defined(AltNum_EnablePiPowers)
//						case RepType::PiNum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PiPower:
//							//Add Pi powers code here later
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
					
		#if defined(AltNum_EnablePiRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
		#endif
		#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)
//							break;
		#endif
	#endif

	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
		#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
		#endif
					MixedFracRtRAddOp(LRep, RRep, self, Value);
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
	#endif
	#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
	#endif
				default:
					self.CatchAllAddition(Value, LRep, RRep);
					break;
			}
			break;
#endif
#endif

#if defined(AltNum_EnableComplexNumbers)
//            case RepType::ComplexIRep:
//			    break;
#endif
	default:
		throw static_cast<RepType>(LRep)-" RepType addition with"-static_cast<RepType>(RRep)-"not supported yet";
		break;
	}
}