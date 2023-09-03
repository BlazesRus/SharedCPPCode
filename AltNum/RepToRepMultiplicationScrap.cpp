		case RepType::NormalType:

			break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			break;
    #if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			break;
    #endif
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
			break;
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:

			break;
	#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			switch (RRep)
			{
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
		#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingMidRight:
			switch (RRep)
			{
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
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
				case RepType::NormalType://Later normalize fractional when integer when viable
					self.BasicMultOp(Value);
					break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
				case RepType::PiNum:
		#if defined(AltNum_EnableDecimaledPiFractionals)
					self.BasicMultOp(Value);
					self.ExtraRep *= -1;
		#else
					self.CatchAllMultiplication(Value, LRep, RRep);
		#endif
					break;
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::ENum:
		#if defined(AltNum_EnableDecimaledEFractionals)
					self.BasicMultOp(Value);
					self.ExtraRep *= -1;
		#else
					self.CatchAllMultiplication(Value, LRep, RRep);
		#endif
					break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
		#if defined(AltNum_EnableDecimaledIFractionals)
					self.BasicMultOp(Value);
					self.ExtraRep *= -1;
		#else
					//ToDo::Add code here
		#endif
					break;
	#endif
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
		#if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			switch (RRep)
			{
				case RepType::NormalType:
					if(Value.DecimalHalf==0)
						self.IntValue *= Value.IntValue;
					else
						self.CatchAllMultiplication(Value, LRep, RRep);
					break;
				default:
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
			}
			break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)//Only one of the 2 switches can be active at once
			switch (RRep)
			{
				case RepType::NormalType://Normalize denom later
					self.BasicMultOp(Value);
					break;
			#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					self.BasicMultOp(Value);
				#if defined(AltNum_EnableDecimaledPiFractionals)
					self.ConvertToNormType(RepType::PiNumByDiv);
				#else
					self.ConvertToNormType(RepType::ENumByDiv);
				#endif
					self.ExtraRep = IRep;
					break;
			#endif							
				default:
					Value.ConvertToNormType(&RRep);
					self.BasicMultOp(Value);
					break;
			}
			break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			switch (RRep)
			{
				case RepType::NormalType:
					self.BasicMultOp(Value);
					break;
		        case RepType::INum:
                    self.BasicMultOp(Value);
                    self.ExtraRep = 0;
                    break;
				default:
					Value.ConvertToNormType(&RRep);
					self.BasicMultOp(Value);
					break;
			}
			break;
    #if defined(AltNum_EnableFractionals)
        #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			break;
        #else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation

			break;
        #endif
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)

			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional)

			break;
    #endif
	#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:

            break;
    #endif
#endif