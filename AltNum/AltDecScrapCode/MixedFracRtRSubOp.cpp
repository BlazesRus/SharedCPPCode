		void MixedFracRtRSubOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
		{
#if defined(AltNum_EnablePIRep)|| defined(AltNum_EnableENum)
            switch(LRep)//Force convert some representations to normal type
            {
#if defined(AltNum_EnablePIRep)
                case RepType::PINum:
                    if(RRep==RepType::MixedPi)
                    {
                        if(self.DecimalHalf!=0)//Convert to normal if not Integer number
                        {
                            self.ConvertToNormType(LRep);LRep = RepType::NormalType;
                        }
                    }
                    else
                        self.ConvertToNormType(LRep);LRep = RepType::NormalType;
                    }
                    break;
#if defined(AltNum_EnablePIPowers)
                case RepType::PIPower:
                    if(RRep==RepType::MixedPi)//Convert to normal PiNum
                    {
                        ConvertPiPowerToPiRep();
                        if(self.DecimalHalf!=0)//Convert to normal if not Integer number
                        {
                            self.ConvertToNormType(LRep);LRep = RepType::NormalType;
                        }
                        else
                            LRep = RepType::RepType::PINum;
                    }
                    else
                        self.ConvertToNormType(LRep);LRep = RepType::NormalType;
                    }
                    break;
#endif
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
                    if(RRep==RepType::MixedE)
                    {
                        if(self.DecimalHalf!=0)//Convert to normal if not Integer number
                        {
                            self.ConvertToNormType(LRep);LRep = RepType::NormalType;
                        }
                    }
                    else
                    {
                        self.ConvertToNormType(LRep);LRep = RepType::NormalType;
                    }
                    break;
                default:
                    break;
            }
#endif
		    switch(LRep)
            {
				case RepType::NormalType:
					switch(RRep)
					{
						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                            
                            break;
                    #if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
		            #elif defined(AltNum_EnableMixedEFractional)
						case RepType::MixedE:
                    #endif
                    #if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
                            switch(LRep)
                            {
                                #if defined(AltNum_EnableMixedPiFractional)
                                case RepType::PINum:
                                    
                                    break;
                                #endif
                                #if defined(AltNum_EnableMixedEFractional)
                                case RepType::ENum:
                                    
                                    break;
                                #endif
                                default://RepType::NormalType
                                    
                                    break;
                            }
                            break;


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
                        //(AltDec(self.IntValue, self.DecimalHalf)/self.ExtraRep) + Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep
                            break;
#if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
#elif defined(AltNum_EnableMixedEFractional)
						case RepType::MixedE:
#endif
#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
                         
#endif
#endif
		#endif
						default:
							self.CatchAllSubtraction(Value, LRep, RRep);
							break;
					}
					break;
#if defined(AltNum_EnableAlternativeRepFractionals)
	#if defined(AltNum_EnablePIRep)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //(self.IntValue*Pi)/self.DecimalHalf - (Y.IntValue+(-Y.DecimalHalf)/Y.ExtraRep)

#if defined(AltNum_EnableMixedPiFractional)
						//case RepType::MixedPi:
                        //(self.IntValue*Pi)/self.DecimalHalf - ((Y.IntValue+(-Y.DecimalHalf)/-Y.ExtraRep)*Pi)
#endif
#if defined(AltNum_EnableMixedEFractional)
						//case RepType::MixedE:
                        //(self.IntValue*Pi)/self.DecimalHalf - ((Y.IntValue+(-Y.DecimalHalf)/-Y.ExtraRep)*e)
#endif
						default:
							self.CatchAllSubtraction(Value, LRep, RRep);
							break;
					}
					break;
					
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //(self.IntValue*e)/self.DecimalHalf - (Y.IntValue+(-Y.DecimalHalf)/Y.ExtraRep)
		#if defined(AltNum_EnablePINum)
						//case RepType::MixedPi:
                        //(self.IntValue*e)/self.DecimalHalf - (Y.IntValue+(-Y.DecimalHalf)/-Y.ExtraRep)
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif
						default:
							self.CatchAllSubtraction(Value, LRep, RRep);
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
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep - (Y.IntValue+(-Y.DecimalHalf)/Y.ExtraRep)
#if defined(AltNum_EnableMixedPiFractional)
						//case RepType::MixedPi:
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep - ((Y.IntValue+(-Y.DecimalHalf)/-Y.ExtraRep)*Pi)
#endif
#if defined(AltNum_EnableMixedEFractional)
						//case RepType::MixedE:
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep - ((Y.IntValue+(-Y.DecimalHalf)/-Y.ExtraRep)*e)
#endif
						default:
							self.CatchAllSubtraction(Value, LRep, RRep);
							break;
					}
					break;
#endif
                case RepType::MixedFrac:
					switch(RRep)
					{
		#if defined(AltNum_EnablePINum)
						//case RepType::MixedPi:
                        //(self.IntValue +- (self.DecimalHalf*-1)/self.ExtraRep)/-self.ExtraRep - ((Y.IntValue+(-Y.DecimalHalf)/-Y.ExtraRep)*Pi)
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif
						default:
							self.CatchAllSubtraction(Value, LRep, RRep);
							break;
					}
                    break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi:
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep - (Y.IntValue+(-Y.DecimalHalf)/Y.ExtraRep)
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif          //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep - (Y.IntValue+(-Y.DecimalHalf)/Y.ExtraRep)
						default:
							self.CatchAllSubtraction(Value, LRep, RRep);
							break;
					}
                    break;
#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep - (Y.IntValue+(-Y.DecimalHalf)/Y.ExtraRep)
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif          //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep - (Y.IntValue+(-Y.DecimalHalf)/Y.ExtraRep)
						default:
							self.CatchAllSubtraction(Value, LRep, RRep);
							break;
					}
                    break;
#endif
#if defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::IFractional:
                case RepType::INumByDiv:
                case RepType::MixedI:

#endif
					throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet from MixedFracRtR";
				default:
					self.CatchAllSubtraction(Value, LRep, RRep);
					break;
		}
#endif