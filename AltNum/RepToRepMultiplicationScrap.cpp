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
			break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
		#endif
		#if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)//Only one of the 2 switches can be active at once

			break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
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