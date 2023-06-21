		void RepToRepDivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
        {
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

		case RepType::UnknownType:
			throw static_cast<RepType>(LRep)-" RepType multiplication with"-static_cast<RepType>(RRep)-"not supported yet";
            break;

        default://No nothing for most of them
        break;
    }

    //RRep Overrides before Main RepToRep Code
    switch(RRep)
    {
#if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
#if defined(AltNum_EnableImaginaryNum)
           if((LRep==RepType::INum
#if defined(AltNum_EnableAlternativeRepFractionals)
            ||LRep==RepType::IFractional ||LRep==RepType::INumByDiv ||LRep==RepType::MixedI)
#else
           ){
#endif
                if(Value.IntValue==0)
                {

                    self.IntValue = self.IntValue<0?NegativeRep:0;
					self.DecimalHalf = ApproachingValRep;
					self.ExtraRep = 0;
					return;
                }
                else
                {
                    Value.DecimalHalf = 1;
                    RRep = RepType::NormalType;
                }
#if defined(AltNum_EnableImaginaryNum)
           if((LRep==RepType::INum
#if defined(AltNum_EnableAlternativeRepFractionals)
            ||LRep==RepType::IFractional ||LRep==RepType::INumByDiv ||LRep==RepType::MixedI)
#else
           }
           else
           {
               Value.DecimalHalf = 1;
               RRep = RepType::NormalType;
           }
#endif
            break;

		case RepType::ApproachingTop:
            Value.DecimalHalf = 999999999;
            Value.ExtraRep = 0;
            RRep = RepType::NormalType;
			break;

#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingBottomDiv:
            Value.ConvertToNormType(RepType::ApproachingBottomDiv);
            RRep = RepType::NormalType;
			break;
		case RepType::ApproachingTopDiv:
            Value.ConvertToNormType(RepType::ApproachingTopDiv);
            RRep = RepType::NormalType;
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

        default://No nothing for most of them
        break;
    }

#if defined(AltNum_EnableNegativeZero)//Treat Negative Zero as zero
    if(LRep==RepType::NegativeZero||RRep==RepType::NegativeZero)
    {
        self.SetAsZero(); return;
    }
#endif

			switch (LRep)//Main switch block starts here
			{
				case RepType::NormalType:
					switch (RRep)
					{
                        case RepType::NormalType://Fail safe for when converted before switch
                            self.BasicMultOp(Value);
                            break;
#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
                        case RepType::PINum://X * (Y*Pi)
                            self.BasicMultOp(Value);
                            self.ExtraRep = PIRep;
                            break;
#endif
#if defined(AltNum_EnableENum)
                            self.BasicMultOp(Value);
                            self.ExtraRep = ERep;
                            break;
#endif
	#if defined(AltNum_EnableImaginaryNum)
						case RepType::INum:
                            self.BasicMultOp(Value);
                            self.ExtraRep = IRep;
	#endif
//							
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif
//

	#if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::NumByDiv:
                            self.BasicMultOp(Value);
                            self.ExtraRep = Value.ExtraRep;
							break;
							
	#if defined(AltNum_EnablePIRep)
						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                            self.BasicMultOp(Value.IntValue);
                            self.DecimalHal = Value.DecimalHalf;
                            self.ExtraRep = Value.ExtraRep;
							break;
	#endif
	#if defined(AltNum_EnableENum)
						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                            self.BasicMultOp(Value.IntValue);
                            self.DecimalHal = Value.DecimalHalf;
                            self.ExtraRep = Value.ExtraRep;
                            break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
#if defined(AltNum_EnableDecimaledPiFractionals) || defined(AltNum_EnableDecimaledEFractionals)
                            self.BasicMultOp(Value);
                            self.ExtraRep = Value.ExtraRep;
                            break;
#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
				case RepType::PINum:
					switch (RRep)
					{
						case RepType::NormalType:// X*Pi * Y
                            self.BasicMultOp(Value);
							break;
	#if defined(AltNum_EnableENum)
						case RepType::ENum:
                            self.CatchAllMultiplication(Value, LRep, RRep);
							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
	#endif
//							
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif
//

	#if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::NumByDiv:
                            self /= Value.ExtraRep;
                            self.BasicMultOp(Value);
							break;
							
	#if defined(AltNum_EnablePIRep)
						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                            self /= Value.DecimalHalf;
                            self.BasicMultOp(PiValue);
                            self.BasicMultOp(Value.IntValue);
							break;
	#endif
	#if defined(AltNum_EnableENum)
						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                            self /= Value.DecimalHalf;
                            self.BasicMultOp(EValue);
                            self.BasicMultOp(Value.IntValue);
							break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                            self /= Value.ExtraRep*-1;
                            self.BasicMultOp(PiValue);
                            self.BasicMultOp(Value);
                            break;
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                            self /= Value.ExtraRep*-1;
                            self.BasicMultOp(EValue);
                            self.BasicMultOp(Value);
                            break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif

						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
	#endif
	
	#if defined(AltNum_EnableENum)
				case RepType::ENum:
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicMultOp(Value);
							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
						case RepType::PINum:
                            Value.ConvertPIToNum();
                            self.BasicMultOp(Value);
							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
						case RepType::INum:
							self.BasicMultOp(Value);
							self.ConvertEToNum();
							self.ExtraRep = IRep;
							break;
	#endif
//							
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::NumByDiv:
                            self /= Value.ExtraRep;
                            self.BasicMultOp(Value);
							break;
							
	#if defined(AltNum_EnableDecimaledPiFractionals)
						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                            self /= Value.ExtraRep*-1;
                            self.BasicMultOp(PiValue);
                            self.BasicMultOp(Value);
                            break;
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                            self /= Value.ExtraRep*-1;
                            self.BasicMultOp(EValue);
                            self.BasicMultOp(Value);
                            break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicMultOp(Value);
                            break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
						case RepType::PINum:
                            Value.ConvertPIToNum()
                            self.BasicMultOp(Value);
							break;
	#endif
	#if defined(AltNum_EnableENum)
						case RepType::ENum:
                            Value.ConvertToNormType(RepType::PINum);
                            self.BasicMultOp(Value);
							break;
	#endif
//							
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							throw static_cast<RepType>(LRep)-" RepType multiplication with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;
	#endif
					
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
				case RepType::PINum:
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicMultOp(Value);
							break;
	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
	#endif
//							
						case RepType::PIPower:
							self.ExtraRep = Value.ExtraRep-1;
                            self.BasicMultOp(Value);
							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif
//
//
	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
				case RepType::PIPower:
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicMultOp(Value);
							break;
						case RepType::PINum://(IntValue.DecimalHalf)*Pi^-ExtraRep representation
                            --self.ExtraRep;
                            self.BasicMultOp(Value);
							break;
	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
	#endif					
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
	#endif
					
	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
				case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
				case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::MixedI:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
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
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
				case RepType::ApproachingTop:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;

	#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingBottomDiv:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
				case RepType::ApproachingTopDiv:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
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
                            self.BasicMultOp(Value);
							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
						case RepType::PINum:
	#if defined(AltNum_EnableDecimaledPiFractionals)
                            self.BasicMultOp(Value);
                            self.ExtraRep *= -1;
    #else
                            self.CatchAllMultiplication(Value, LRep, RRep);
    #endif
							break;
	#endif
	#if defined(AltNum_EnableENum)
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
	#if defined(AltNum_EnableDecimaledIFractionals)
						case RepType::INum:
                            self.BasicMultOp(Value);
                            self.ExtraRep *= -1;
                            break;
    #else
//						case RepType::INum:
//                            break;
    #endif
    #endif
							
//							
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
					
	#if defined(AltNum_EnablePIRep)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					switch (RRep)
					{
						case RepType::NormalType:
                            if(Value.DecimalHalf==0)
                                self.IntValue *= Value.IntValue;
                            else
                                self.CatchAllMultiplication(Value, LRep, RRep);
							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
	#endif
	#if defined(AltNum_EnableENum)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					switch (RRep)
					{
						case RepType::NormalType:
                            if(Value.DecimalHalf==0)
                                self.IntValue *= Value.IntValue;
                            else
                                self.CatchAllMultiplication(Value, LRep, RRep);
							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
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
						case RepType::NormalType://Normalize denom later
                            self.BasicMultOp(Value);
							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
						case RepType::PINum:
                            self.BasicMultOp(Value);
                            self.BasicMultOp(PiNumValue);
							break;
	#endif
	#if defined(AltNum_EnableENum)
						case RepType::ENum:
                            self.BasicMultOp(Value);
                            self.BasicMultOp(ENumValue);
							break;
	#endif
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
//							
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
#endif
	
					
	#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
				case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
				case RepType::MixedEByDiv:
	#endif
	#endif
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							self.CatchAllMultiplication(Value, LRep, RRep);
							break;
					}
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicMultOp(Value);
							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							throw static_cast<RepType>(LRep)-" RepType multiplication with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;
					
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicMultOp(Value);
							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							//self.CatchAllMultiplication(Value, LRep, RRep);
							throw static_cast<RepType>(LRep)-" RepType multiplication with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;
					
				case RepType::MixedI:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
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
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
	#endif
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
	#endif
	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
	#endif
//
	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
	#endif
//							break;
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
	#endif
	#endif
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedOp code not implimented yet";
//							break;//
	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
						default:
							throw static_cast<RepType>(LRep)-" RepType multiplication with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;
	#endif

#if defined(AltNum_EnableComplexNumbers)
//            case RepType::ComplexIRep:
//			    break;
#endif
            default:
            	throw static_cast<RepType>(LRep)-" RepType multiplication with"-static_cast<RepType>(RRep)-"not supported yet";
            	break;
			}
        }
		
