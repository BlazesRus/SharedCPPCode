

#if defined(AltNum_EnableFractionals)
/*
void NumByDivAddOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
            //ToDo:Add more precise AddOperation code here later
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracAddOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
			self.ConvertToNormType(&LRep);
	    #if defined(AltNum_EnableMixedPiFractional)
			self.BasicMixedPiFracAddOp(Value);
        #else
			self.BasicMixedEFracAddOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllAddition(Value, RepType::NumByDiv, RRep);
			break;
	}
}
*/
#endif

#if defined(AltNum_EnablePiRep)


//	#if defined(AltNum_EnablePiPowers)
//void PiPowerAddOp(RepType& RRep, AltDec& self, AltDec& Value)
//{
//	switch (RRep)
//	{
//		case RepType::PiNum:
//#if defined(AltNum_EnableMixedFractional)
//		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
//			self.ConvertToNormType(RepType::PiPower);
//			self.BasicMixedFracAddOp(Value);
//			break;
//    #if defined(AltNum_EnableMixedPiFractional)
//		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
//    #elif defined(AltNum_EnableMixedEFractional)
//		case RepType::MixedE:
//    #endif
//    #if defined(AltNum_MixedPiOrEEnabled)
//        self.ConvertToNormType(&LRep);
//	    #if defined(AltNum_EnableMixedEFractional)
//			self.BasicMixedEFracAddOp(Value);
//        #else
//			self.BasicMixedPiFracAddOp(Value);
//	    #endif
//			break;
//    #endif
//#endif
//		default:
//			self.CatchAllAddition(Value, LRep, RRep);
//			break;
//	}
//}
	
	#if defined(AltNum_EnableDecimaledPiFractionals)
/*
void PiNumByDivAddOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
    #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracAddOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedEFractional)
            self.ConvertToNormType(&LRep);
			self.BasicMixedEFracAddOp(Value);
        #else
            //ToDo:Add more precise AddOperation code here later
            self.ConvertToNormType(&LRep);
			self.BasicMixedPiFracAddOp(Value);
	    #endif
			break;
    #endif
		default:
			self.CatchAllAddition(Value, LRep, RRep);
			break;
	}
}
	#elif defined(AltNum_EnablePiFractional)
void PiFractionalAddOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracAddOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedEFractional)
            self.ConvertToNormType(&LRep);
			self.BasicMixedEFracAddOp(Value);
        #else
            //ToDo:Add more precise AddOperation code here later
            self.ConvertToNormType(&LRep);
			self.BasicMixedPiFracAddOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllAddition(&Value, &LRep, &RRep);
			break;
	}
}
	#endif
*/
#endif

#if defined(AltNum_EnableERep)

/*
	#if defined(AltNum_EnableDecimaledEFractionals)
void ENumByDivAddOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracAddOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedPiFractional)
			self.ConvertToNormType(&LRep);
			self.BasicMixedEFracAddOp(Value);
        #else
            //ToDo:Add more precise AddOperation code here later
			self.ConvertToNormType(&LRep);
			self.BasicMixedPiFracAddOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllAddition(&Value, &LRep, &RRep);
			break;
	}
}
	#elif defined(AltNum_EnableEFractional)
void EFractionalAddOp(RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
			self.ConvertToNormType(&LRep);
			self.BasicMixedFracAddOp(Value);
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_MixedPiOrEEnabled)
	    #if defined(AltNum_EnableMixedPiFractional)
			self.ConvertToNormType(&LRep);
			self.BasicMixedEFracAddOp(Value);
        #else
            //ToDo:Add more precise AddOperation code here later
			self.ConvertToNormType(&LRep);
			self.BasicMixedPiFracAddOp(Value);
	    #endif
			break;
    #endif
#endif
		default:
			self.CatchAllAddition(&Value, &LRep, &RRep);
			break;
	}
}
	#endif
*/
#endif
