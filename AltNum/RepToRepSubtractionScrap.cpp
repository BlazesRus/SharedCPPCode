
#if defined(AltNum_EnableFractionals)
void NumByDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
	#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(RepType::);
			self.BasicMixedFracSubOp(Value);
			break;
		#if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
		#elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
		#endif
		#if defined(AltNum_MixedPiOrEEnabled)
			self.ConvertToNormType(RepType::NumByDiv);
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


	#if defined(AltNum_EnablePiPowers)
void PiPowerSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
		case RepType::PiNum:
    #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(RepType::);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
        self.ConvertToNormType(RepType::);
	    #if defined(AltNum_EnableMixedEFractional)
			self.BasicMixedEFracSubOp(Value);
        #else
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, RepType::PiPower, &RRep);
			break;
	}
}
    #endif
	#if defined(AltNum_EnableDecimaledPiFractionals)

void PiNumByDivSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(RepType::PiNumByDiv);
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
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(RepType::PiNumByDiv);
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, RepType::PiNumByDiv, &RRep);
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
			self.ConvertToNormType(RepType::);
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
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(RepType::);
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, RepType::PiFractional, &RRep);
			break;
	}
}
	#endif

#endif


#if defined(AltNum_EnableERep)

	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivSubOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(RepType::ENumByDiv);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedPiFractional)
			self.ConvertToNormType(RepType::ENumByDiv);
			self.BasicMixedEFracSubOp(Value);
        #else
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(RepType::ENumByDiv);
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value,RepType::ENumByDiv, &RRep);
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
			self.ConvertToNormType(RepType::EFractional);
			self.BasicMixedFracSubOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedPiFractional)
			self.ConvertToNormType(RepType::EFractional);
			self.BasicMixedEFracSubOp(Value);
        #else
            //ToDo:Add more precise operation code here later
			self.ConvertToNormType(RepType::EFractional);
			self.BasicMixedPiFracSubOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllSubtraction(&Value, RepType::EFractional, &RRep);
			break;
	}
}
	#endif
#endif
