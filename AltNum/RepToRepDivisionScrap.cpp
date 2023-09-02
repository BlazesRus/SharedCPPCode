void PiNumDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
				case RepType::NormalType:// X*Pi * Y
					self.BasicDivOp(Value);
					break;
		#if defined(AltNum_EnablePiPowers)
				//case RepType::PiPower:
				//	//Add Pi powers code here later
				//	break;
		#endif
				case RepType::NumByDiv://(X*Pi) / (Y / Z) = (XZ)/Y
					self.BasicDivOp(Value);
					self.BasicInt32MultDivOpV2(Value.ExtraRep);
					break;
							
		#if defined(AltNum_EnablePiFractional)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					//(XPi) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf)/(Y)
					self.ExtraRep = 0;
					self.BasicInt32MultDivOpV2(Value.DecimalHalf);
					self.BasicInt32DivOpV2(Value.IntValue);
					break;
		#endif
		#if defined(AltNum_EnableEFractional)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					self.BasicInt32MultDivOpV2(Value.DecimalHalf);
					self.BasicDivOp(ENumValue()*Value.IntValue);
					break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
					//(XPi) / (Y.Value*Pi / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value)
					self.ExtraRep = 0;
					self.BasicInt32DivOpV2(-Value.ExtraRep);
					self.BasicMultDivOp(Value);
					break;
		#elif defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
					//(XPi) / (Y.Value*e / -Y.ExtraRep) = (X*-Y.ExtraRep)/(Y.Value*e)
					self.BasicMultDivOp(-Value.ExtraRep);
					Value.ConvertToNormType(RepType::ENum);
					self.BasicDivOp(Value);
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
				#if defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
				#else
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
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
					self.CatchAllDivision(Value, LRep, RRep);
					break;
    }
}

void ENumDivOp(const RepType& RRep, AltDec& self, AltDec& Value)
{
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
					self.BasicInt32MultDivOpV2(Value.ExtraRep);
					break;
					
				#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					//(Xe) / (Y.IntValue*Pi / Y.DecimalHalf) = (X*Y.DecimalHalf*e)/(Y)
					self.BasicInt32MultDivOpV2(Value.DecimalHalf);
					self.BasicUnsignedDiv(PiNumValue()*Value.IntValue);
					break;
				#endif
				#if defined(AltNum_EnableERep)&&!defined(AltNum_EnableDecimaledEFractionals)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					self.ExtraRep = 0;
					self.BasicInt32MultDivOpV2(Value.DecimalHalf);
					self.BasicInt32DivOpV2(Value.IntValue);
					break;
				#endif

				#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
					self.BasicUnsignedIntMult(-Value.ExtraRep);
					Value.ConvertToNormType(RepType::PiNum);
					self.BasicDivOp(Value);
					break;
				#elif defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
                	self.BasicUnsignedIntMult(-Value.ExtraRep);
					self.ExtraRep = 0;
					self.BasicDivOp(Value);
					break;
				#endif
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
				    #if defined(AltNum_EnableDecimaledIFractionals)
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
				    #else
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
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
		#endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
	}

		case RepType::NormalType:
			NormalDivOp(RRep, self, Value); 
			break;
#if defined(AltNum_EnablePiRep)

	#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:

			break;
	#endif
#endif
#if defined(AltNum_EnableERep)
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:

			break;
		case RepType::ApproachingTDivOp:

			break;

	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:

			break;
		case RepType::ApproachingMidRight:

			break;
	#endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:

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
						self.DecimalHalf *= Value.IntValue;
					else
                    {
            #if defined(AltNum_EnableDecimaledEFractionals)
                        self.ExtraRep = -self.DecimalHalf;
                        self.DecimalHalf = 0;
                        //Skip to performing DivOperation as DecimaledFractional
                        RepToRepDivOp(RepType::ENumByDiv, RepType::NormalType, self, Value);
            #else
                        //Convert to NumByDiv instead
                        self.ExtraRep = self.DecimalHalf;
                        self.DecimalHalf = 0;
                        self.BasicMultDivOp(ENum);
						self.BasicDivOp(Value);
            #endif
                    }
					break;
            #if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific DivOperation code later
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
                    //Skip to performing DivOperation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
            #endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
            }
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
					self.BasicDivOp(Value);
					break;
            #if defined(AltNum_EnablePiRep)
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
            #if defined(AltNum_EnableERep)
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
                case RepType::NumByDiv:
                    // /((Value.Value/(-Value.ExtraRep))*Pi) Representation
                    throw "ToDo:Need to add code here";
                    break;
            #if !defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiFractional:
                    // /((Value.Value/(-Value.ExtraRep))*Pi) Representation
                    throw "ToDo:Need to add code here";
                    break;
            #endif
            #if !defined(AltNum_EnableDecimaledEFractionals)
                case RepType::EFractional:
                    // /((Value.Value/(-Value.ExtraRep))*Pi) Representation
                    throw "ToDo:Need to add code here";
                    break;
            #endif
            #if defined(AltNum_EnableImaginaryNum)
				//ToDo::Add more specific DivOperation code later
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
                    //Skip to performing DivOperation as normal type
					self.ConvertToNormType(&LRep);
                    RepToRepDivOp(RepType::NormalType, RRep, self, Value);
                    break;
            #endif
				default:
					self.CatchAllDivision(Value, LRep, RRep);
					break;
			}
			break;
        #endif
    #endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:

			break;
    #if defined(AltNum_EnableFractionals)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			switch (RRep)
			{
				case RepType::NormalType:
					if(Value.DecimalHalf==0)
						self.DecimalHalf *= Value.IntValue;
					else//Convert to INum
					{
						self.ExtraRep = IRep;
						int LSideDivisor = self.DecimalHalf;
						self.DecimalHalf = 0;
						self.BasicDivOp(LSideDivisor.BasicMultDivOp(Value));
					}
					break;
				default:
					Value.ConvertToNormType(&RRep);
					if(Value.DecimalHalf==0)
						self.DecimalHalf *= Value.IntValue;
					else//Convert to INum
					{
						self.ExtraRep = IRep;
						int LSideDivisor = self.DecimalHalf;
						self.DecimalHalf = 0;
						self.BasicDivOp(LSideDivisor.BasicMultDivOp(Value));
					}
			}
			break;
    #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			switch (RRep)
			{
				case RepType::NormalType:
					self.BasicDivOp(Value);
					break;
				case RepType::INum:
					//SelfNum*i / ValueNum*i
					//SelfNum/ValueNum
					if(Value.DecimalHalf==0)
						self.ExtraRep = Value.ExtraRep;
					else
					{
						self.ExtraRep = 0;
						self.BasicDivOp(Value);
					}
					break;
        #if defined(AltNum_EnableAlternativeRepFractionals)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                    //(SelfNum/(-self.ExtraRep))*i / (IntValue/DecimalHalf*i)
                    //(SelfNum/(-self.ExtraRep)) / (Value.IntValue/Value.DecimalHalf)
                    throw "ToDo:Need to add code here";
                    break;						
			#if defined(AltNum_EnableDecimaledIFractionals)						
				case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
                    //(SelfNum/(-self.ExtraRep))*i / ((Value/(-ExtraRep))*i)
                    //(SelfNum/(-self.ExtraRep)) / (Value/(-ExtraRep))
                    //((SelfNum*-Value.ExtraRep)/(-self.ExtraRep)) / (Value)
                    throw "ToDo:Need to add code here";
                    break;//Convert result to NumByDiv
			#endif
		    #if defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI:
                    //(SelfNum/(-self.ExtraRep))*i / ((IntValue +- (DecimalHalf*-1)/-ExtraRep)i)
                    //(SelfNum/(-self.ExtraRep)) / ((IntValue +- (DecimalHalf*-1)/-ExtraRep))
                    //(SelfNum/(-self.ExtraRep)) / (IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
                    throw "ToDo:Need to add code here";
                    break;
		    #endif
        #endif
				default:
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
			}
			break;
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/ExtraRep)
			switch (RRep)
			{
                //case RepType::NormalType:
				//	break;
				default:
                    self.ConvertToNormType(&LRep);
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
			}
			break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
    #endif
    #if defined(AltNum_EnableMixedPiFractional) || defined(AltNum_EnableMixedEFractional)
			switch (RRep)
			{
                case RepType::NormalType:
                    if(Value.DecimalHalf==0)
                    {
                        int LeftSideAbs = self.IntValue.Abs();
                        int divRes = LeftSideAbs / Value.IntValue;
                        int C = LeftSideAbs - Value.IntValue * divRes;
                        if(C==0)
                        {
                            if(divRes==0)//Convert into non-mixed fractional
                            {
                                throw "ToDo:Finish this code later";
                            }
                            else
                            {
                                throw "ToDo:Finish this code later";
                            }
                        }
                    }
                    else//Convert to NumByDiv
                    {
                        throw "ToDo:Finish this code later";
                    }
					break;
				default:
                    self.ConvertToNormType(&LRep);
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
			}
			break;
    #endif
    if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			switch (RRep)
			{
                case RepType::INum:
        #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
        #elif defined(AltNum_EnableFractionals)
                case RepType::IFractional:
        #endif
                    self.CatchAllImaginaryDivision(Value, LRep, RRep);
				default:
                    self.ConvertToNormalIRep(&LRep);
					Value.ConvertToNormType(&RRep);
					self.BasicDivOp(Value);
            }
    #endif
#endif