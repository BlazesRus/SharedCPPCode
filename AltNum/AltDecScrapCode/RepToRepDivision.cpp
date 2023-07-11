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
        			throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
                    break;
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::IFractional:
                case RepType::INumByDiv:
                case RepType::MixedI:

#endif
#if defined(AltNum_EnableApproachingValues)&&defined(AltNum_EnableImaginaryInfinity)
                    if(RRep==RepType::ApproachingBottom)
                    {
                        if(Value.IntValue==0)
                        {
                            if(self.IntValue<0)//NegativeValue / 0.0..1 = Negative Infinity
                                self.IntValue = -1;
                            else//PositiveValue / 0.0..1 = Infinity
                                self.IntValue = 1;
                            self.DecimalHal = InfinityRep;
                            self.ExtraRep = IRep;
                            return;
                        }
                        else
                        {
#endif
                            Value.DecimalHalf = 1;
                            RRep = RepType::NormalType;
#if defined(AltNum_EnableApproachingValues)&&defined(AltNum_EnableImaginaryInfinity)
                        }
                    }
#endif
                    break;
#endif
                default://No nothing for most of them
                break;
            }
        
            //RRep Overrides before Main RepToRep Code
            switch(RRep)
            {
        #if defined(AltNum_EnableApproachingValues)
                case RepType::ApproachingBottom:
                    if(Value.IntValue==0)
                    {
                        if(self.IntValue<0)//NegativeValue / 0.0..1 = Negative Infinity
                            self.IntValue = -1;
                        else//PositiveValue / 0.0..1 = Infinity
                            self.IntValue = 1;
                        self.DecimalHal = InfinityRep;
                        self.ExtraRep = 0;//Not really needed because not checking this value unless imaginary infinity detected
                        return;
                    }
                    else
                    {
                        Value.DecimalHalf = 1;
                        RRep = RepType::NormalType;
                    }
                    break;
        
        		case RepType::ApproachingTop:
                    if(LRep==RepType::NormalType&&Value.IntValue)//1/0.9..9 = 1.0..1
                    {//(For positive left side values)Techically returns self.IntValue + 0.0..self.IntValue
                        self.ExtraRep = 0;
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
                            self.BasicDivOp(Value);
                            break;
	#if defined(AltNum_EnableImaginaryNum)
						case RepType::INum:
                            throw "related imaginary format operation not supported yet";
                            break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::NumByDiv://X / (Y / Z) = (XZ)/Y
                            self.BasicDivOp(Value);
                            self.BasicMultOp(Value.ExtraRep);
							break;
							
	    #if defined(AltNum_EnablePiRep)
						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
							//X / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(YPi)
                            self.BasicMultOp(Value.DecimalHalf);
                            self.BasicDivOp(PiNumValue()*Value.IntValue);
                            break;
	    #endif
	    #if defined(AltNum_EnableENum)
						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                            self.BasicMultOp(Value.DecimalHalf);
                            self.BasicDivOp(ENumValue()*Value.IntValue);
                            break;
	    #endif

	    #if defined(AltNum_EnableDecimaledPiFractionals)
						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #elif defined(AltNum_EnableDecimaledEFractionals)
						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
        #endif
        #if defined(AltNum_EnableDecimaledPiFractionals) || defined(AltNum_EnableDecimaledEFractionals)
                            self.BasicMultOp(-Value.ExtraRep);
	        #if defined(AltNum_EnableDecimaledPiFractionals)
                            Value.ConvertToNormType(RepType::PiNum);
            #elif defined(AltNum_EnableDecimaledEFractionals)
                            Value.ConvertToNormType(RepType::ENum);
            #endif
                            self.BasicDivOp(Value);
                            break;
        #endif
    #endif
							
	#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
	    #if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
						case RepType::MixedE:
	    #endif
							MixedFracRtRDivOp(LRep, RRep, self, Value);
							break;
	#endif

    #if defined(AltNum_EnableImaginaryNum)
    //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
						case RepType::INum:
                            self.BasicDivOp(Value);
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
        #if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
	        #if defined(AltNum_EnableDecimaledIFractionals)
						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
            #endif
        #endif
        #if defined(AltNum_EnableMixedIFractional)
                        case RepType::MixedI:
        #endif
        //Placeholder code(Converting to i rep for now)
        #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
                            Value.ConvertToNormalIRep(RRep);
                            self.BasicDivOp(Value);
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
                            break;
        #endif
        #if defined(AltNum_EnableComplexNumbers)
                        case RepType::ComplexIRep:
                            break;
        #endif
	#endif
						default:
//						case RepType::PiNum:
//						case RepType::PiPower:
//                      case RepType::ENum:
                            Value.ConvertToNormType(RRep);
                            self.BasicDivOp(Value);
							break;
					}
					break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
				case RepType::PiNum:
					switch (RRep)
					{
						case RepType::NormalType:// X*Pi * Y
                            self.BasicDivOp(Value);
							break;				
	#if defined(AltNum_EnablePiPowers)
//						case RepType::PiPower:
//							//Add Pi powers code here later
//							break;
	#endif

						case RepType::NumByDiv://(X*Pi) / (Y / Z) = (XZ)/Y
                            self.BasicDivOp(Value);
                            self.BasicMultOp(Value.ExtraRep);
							break;
							
	#if defined(AltNum_EnablePiRep)
						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
							//(XPi) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(Y)
                            self.ExtraRep = 0;
                            self.BasicMultOp(Value.DecimalHalf);
                            self.BasicDivOp(Value.IntValue);
                            break;
	#endif
	#if defined(AltNum_EnableENum)
						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                            self.BasicMultOp(Value.DecimalHalf);
                            self.BasicDivOp(ENumValue()*Value.IntValue);
                            break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                            //(XPi) / (Y.Value*Pi / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value)
                            self.ExtraRep = 0;
                            self.BasicDivOp(-Value.ExtraRep);
                            self.BasicMultOp(Value);
                            break;
	#elif defined(AltNum_EnableDecimaledEFractionals)
						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                            //(XPi) / (Y.Value*e / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value*e)
                            self.BasicMultOp(-Value.ExtraRep);
                            Value.ConvertToNormType(RepType::ENum);
                            self.BasicDivOp(Value);
                            break;
	#endif
							
	#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
	    #if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
						case RepType::MixedE:
	    #endif
							MixedFracRtRDivOp(LRep, RRep, self, Value);
							break;
	#endif

    #if defined(AltNum_EnableImaginaryNum)
    //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
						case RepType::INum:
                            self.BasicDivOp(Value);
                            self.BasicDivOp(PiNum);
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
        #if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
	        #if defined(AltNum_EnableDecimaledIFractionals)
						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
            #endif
        #endif
        #if defined(AltNum_EnableMixedIFractional)
                        case RepType::MixedI:
        #endif
        //Placeholder code(Converting to i rep for now)
        #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
                            Value.ConvertToNormalIRep(RRep);
                            self.BasicDivOp(Value);
                            self.BasicDivOp(PiNum);
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
                            break;
        #endif
        #if defined(AltNum_EnableComplexNumbers)
                        case RepType::ComplexIRep:
                            break;
        #endif
    #endif
						default:
//						case RepType::ENum:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#endif
	
	#if defined(AltNum_EnableENum)
				case RepType::ENum:
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicDivOp(Value);
							break;
	#if defined(AltNum_EnablePiRep)
						case RepType::PiNum:
                            Value.ConvertPiToNum();
                            self.BasicDivOp(Value);
							break;
	#if defined(AltNum_EnablePiPowers)
						case RepType::PiPower:
                            Value.ConvertPiPowerToNum();
                            self.BasicDivOp(Value);
							break;
	#endif			

	#if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::NumByDiv://(X*E) / (Y / Z) = (XZ*E)/Y
                            self.BasicDivOp(Value);
                            self.BasicMultOp(Value.ExtraRep);
							break;
							
	    #if defined(AltNum_EnablePiRep)
						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
							//(Xe) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf*e)/(Y)
                            self.BasicMultOp(Value.DecimalHalf);
                            self.BasicDivOp(PiNumValue()*Value.IntValue);
                            break;
	    #endif
	    #if defined(AltNum_EnableENum)
						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                            self.ExtraRep = 0;
                            self.BasicMultOp(Value.DecimalHalf);
                            self.BasicDivOp(Value.IntValue);
                            break;
	    #endif

	    #if defined(AltNum_EnableDecimaledPiFractionals)
						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
                            self.BasicMultOp(-Value.ExtraRep);
                            Value.ConvertToNormType(RepType::PiNum);
                            self.BasicDivOp(Value);
                            break;
	    #elif defined(AltNum_EnableDecimaledEFractionals)
						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                            self.ExtraRep = 0;
                            self.BasicDivOp(-Value.ExtraRep);
                            self.BasicMultOp(Value);
                            break;
	    #endif
    #endif

	#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
	    #if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
						case RepType::MixedE:
	    #endif
							MixedFracRtRDivOp(LRep, RRep, self, Value);
							break;
	#endif

    #if defined(AltNum_EnableImaginaryNum)
    //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
						case RepType::INum:
                            self.BasicDivOp(Value);
                            self.BasicDivOp(ENum);
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
        #if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
	        #if defined(AltNum_EnableDecimaledIFractionals)
						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
            #endif
        #endif
        #if defined(AltNum_EnableMixedIFractional)
                        case RepType::MixedI:
        #endif
        //Placeholder code(Converting to i rep for now)
        #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
                            Value.ConvertToNormalIRep(RRep);
                            self.BasicDivOp(Value);
                            self.BasicDivOp(ENum);
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
                            break;
        #endif
        #if defined(AltNum_EnableComplexNumbers)
                        case RepType::ComplexIRep:
                            break;
        #endif
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case RepType::INum:
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicDivOp(Value);
                            break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
						case RepType::PiNum:
                            Value.ConvertPiToNum()
                            self.BasicDivOp(Value);
							break;
	#endif
	#if defined(AltNum_EnableENum)
						case RepType::ENum:
                            Value.ConvertToNormType(RepType::PiNum);
                            self.BasicDivOp(Value);
							break;
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
							break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//
	    #if defined(AltNum_EnableDecimaledIFractionals)
						case RepType::INumByDiv://Value/(-ExtraRep))*i Representation
							//(AltDec(self.IntValue, self.DecimalHalf)i)/(Value/(-Value.ExtraRep))*i)
                            //=(AltDec(self.IntValue, self.DecimalHalf))/(Value/(-Value.ExtraRep)))
                            //=(AltDec(self.IntValue, self.DecimalHalf)*-Value.ExtraRep)/Value
	                    self.BasicMultOp(-Value.ExtraRep);
                        #if defined(AltNum_EnableAlternativeRepFractionals)
                            if(Value.DecimalHalf==0)
                            {
                                self.BasicMultOp(-Value.ExtraRep);
                                self.ExtraRep = Value.ExtraRep;
                            }
                            else
                            {
                        #endif
                                self.ExtraRep = 0;
                                self.BasicDivOp(Value);
	                    #if defined(AltNum_EnableAlternativeRepFractionals)
                            }
	                    #endif
                            break;
        #endif
                        #if defined(AltNum_EnableMixedIFractional)
						case RepType::MixedI:
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endi
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;
	#endif
					
	#if defined(AltNum_EnablePiRep)&&defined(AltNum_EnablePiPowers)
				case RepType::PiNum:
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicDivOp(Value);
							break;
	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
	#endif
						case RepType::PiPower:
							self.ExtraRep = Value.ExtraRep-1;
                            self.BasicDivOp(Value);
							break;
//
	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
							break;
	#endif


    #if defined(AltNum_EnableImaginaryNum)
    //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
						case RepType::INum:
                            self.BasicDivOp(Value);
                            self.BasicDivOp(PiNum);
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
        #if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
	        #if defined(AltNum_EnableDecimaledIFractionals)
						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
            #endif
        #endif
        #if defined(AltNum_EnableMixedIFractional)
                        case RepType::MixedI:
        #endif
        //Placeholder code(Converting to i rep for now)
        #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
                            Value.ConvertToNormalIRep(RRep);
                            self.BasicDivOp(Value);
                            self.BasicDivOp(PiNum);
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
                            break;
        #endif
        #if defined(AltNum_EnableComplexNumbers)
                        case RepType::ComplexIRep:
                            break;
        #endif
    #endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
				case RepType::PiPower:
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicDivOp(Value);
							break;
						case RepType::PiNum://(IntValue.DecimalHalf)*Pi^-ExtraRep representation
                            --self.ExtraRep;
                            self.BasicDivOp(Value);
							break;
	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
	#endif				

	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
//								break;
        #endif
    #endif

	#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
	    #if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
        #elif defined(AltNum_EnableMixedEFractional)
						case RepType::MixedE:
	    #endif
							MixedFracRtRDivOp(LRep, RRep, self, Value);
							break;
	#endif

    #if defined(AltNum_EnableImaginaryNum)
    //Num/Yi = Num/Yi * i/i = Numi/-Y = -Numi/Y
						case RepType::INum:
                            self.BasicDivOp(Value);
                            self.BasicDivOp(PiPowerNum(-self.ExtraRep));
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
        #if defined(AltNum_EnableAlternativeRepFractionals)
						case RepType::IFractional://  IntValue/DecimalHalf*i Representation							
	        #if defined(AltNum_EnableDecimaledIFractionals)
						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
            #endif
        #endif
        #if defined(AltNum_EnableMixedIFractional)
                        case RepType::MixedI:
        #endif
        //Placeholder code(Converting to i rep for now)
        #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
                            Value.ConvertToNormalIRep(RRep);
                            self.BasicDivOp(Value);
                            self.BasicDivOp(PiPowerNum(-self.ExtraRep));
                            if(self.IntValue==NegativeRep)
                                self.IntValue = 0;
                            else
                                self.IntValue = self.IntValue==0?NegativeRep:-self.IntValue;
                            break;
                            self.ExtraRep = IRep;
                            break;
        #endif
        #if defined(AltNum_EnableComplexNumbers)
                        case RepType::ComplexIRep:
                            break;
        #endif
    #endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
                #if defined(AltNum_EnableDecimaledIFractionals)						
						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                #endif
#endif
#if defined(AltNum_EnableMixedIFractional)
                        case RepType::MixedI:
#endif
                            throw "Code not implimented yet for this imaginary number division operation.";
							break;
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
				case RepType::ApproachingTop:
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;

	#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingBottomDiv:
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
				case RepType::ApproachingTopDiv:
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
						case RepType::INum:
                            throw "related imaginary format operation not supported yet";
							break;
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
//							
	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
	#if defined(AltNum_EnablePiNum)
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
                            if(DecimalHalf==0)
                                ExtraRep *= Value.IntValue;
                            else
                                self.BasicDivOp(Value);
							break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
						case RepType::PiNum:
						    //(self.Value/self.ExtraRep) / (Value.Value*Pi)
							if(Value.DecimalHalf==0)
							{
								self.ExtraRep *= Value.IntValue;
								self.BasicDivOp(PiNum);
							}
							else
							{
								self.BasicDivOp(Value);
								self.BasicDivOp(PiNum);
							}
							break;
	#endif
	#if defined(AltNum_EnableENum)
						case RepType::ENum:
							if(Value.DecimalHalf==0)
							{
								self.ExtraRep *= Value.IntValue;
								self.BasicDivOp(ENum);
							}
							else
							{
								self.BasicDivOp(Value);
								self.BasicDivOp(ENum);
							}
							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
						case RepType::INum:
							//(self.Value/self.ExtraRep) / (Value.Value*i)
							//=(self.Value/self.ExtraRep) / (Value.Value*i) * i/i
							//=-(self.Value/self.ExtraRep)i / Value.Value
							self.SwapNegativeStatus();
 							if(Value.DecimalHalf==0)
							{
							#if defined(AltNum_EnableDecimaledPiFractionals)//Turn into INumByDiv
								self.ExtraRep *= -Value.IntValue;
							#else
								self.ExtraRep *= Value.IntValue;
								if(self.DecimalHalf==0)//Turn into IFractional
								{
									self.DecimalHalf = self.ExtraRep;
									self.ExtraRep = IByDivisorRep;
								}
								else//Turn into INum
								{
									self.ConvertToNormType(RepType::NumByDiv);
									self.ExtraRep = IRep;
								}
							#endif
							}
							else
							{
								self.BasicDivOp(Value);
								self.ConvertToNormType(RepType::NumByDiv);
								self.ExtraRep = IRep;
							}
                            break;
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
					
	#if defined(AltNum_EnablePiRep)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					switch (RRep)
					{
						case RepType::NormalType:
                            if(Value.DecimalHalf==0)
                                self.DecimalHalf *= Value.IntValue;
                            else
                                self.CatchAllDivision(Value, LRep, RRep);
							break;
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
                                self.DecimalHalf *= Value.IntValue;
                            else
                                self.CatchAllDivision(Value, LRep, RRep);
							break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
//						case RepType::PiNum:
//							break;
	#endif
	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
						case RepType::INum:
                            throw "related imaginary format operation not supported yet";
							break;
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
                            self.BasicDivOp(Value);
							break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
						case RepType::PiNum:
                            if(Value.DecimalHalf==0)
                                self.ExtraRep *= Value.IntValue;
                            else
                                self.BasicDivOp(Value);
                            self.BasicDivOp(Value);
                            if(RRep==RepType::PiNumByDiv)
                                self.ExtraRep *= -1;//Becomes NumByDiv instead
                            else    
                                self.BasicDivOp(PiNum);
							break;
	#endif
	#if defined(AltNum_EnableENum)
						case RepType::ENum:
                            if(Value.DecimalHalf==0)
                                self.ExtraRep *= Value.IntValue;
                            else
                                self.BasicDivOp(Value);
                            self.BasicDivOp(Value);
                            if(RRep==RepType::ENumByDiv)
                                self.ExtraRep *= -1;//Becomes NumByDiv instead
                            else    
                                self.BasicDivOp(ENumValue);
							break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
						case RepType::INum:
                            throw "related imaginary format operation not supported yet";
							break;
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
//								break;
        #endif
    #endif

	#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
	    #if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
        #else
						case RepType::MixedE:
	    #endif
							MixedFracRtRMultOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi:
		#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE:
		#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
		#endif
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #elif defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
//								break;
        #endif
    #endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
//							break;
                        #if defined(AltNum_EnableMixedIFractional)
						case RepType::MixedI:
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	#endif
    #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
    #endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicDivOp(Value);
							break;
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	        #endif
            #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
            #endif
						default:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;

	    #if defined(AltNum_EnableDecimaledIFractionals)	
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
					switch (RRep)
					{
						case RepType::NormalType:
                            self.BasicDivOp(Value);
							break;
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
							
	        #if defined(AltNum_EnablePiRep)&&defined(AltNum_EnablePiPowers)
//						case RepType::PiNum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PiPower:
//							//Add Pi powers code here later
//							break;
	#endif

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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	    #endif
	    #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
	        #endif
            #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
            #endif
						default:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;
        #endif
  #endif
	    #if defined(AltNum_EnableMixedIFractional)
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
//						case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
	            #endif
	            #if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
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
							MixedFracRtRDivOp(LRep, RRep, self, Value);
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
							MixedIFracRtRDivOp(LRep, RRep, self, Value);
							break;
                        #endif
            #endif
            #if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
            #endif
						default:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;
	    #endif

#if defined(AltNum_EnableComplexNumbers)
//            case RepType::ComplexIRep:
//			    break;
#endif
            default:
            	throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
            	break;
			}
        }