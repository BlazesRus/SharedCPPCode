		void MixedFracRtRAddOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
		{
#if defined(AltNum_EnablePIRep)|| defined(AltNum_EnableENum)
            switch(LRep)//Force convert some representations to normal type
            {
#if defined(AltNum_EnablePIRep)
                case RepType::PINum:
#if defined(AltNum_EnablePIPowers)
                case RepType::PIPower:
#endif
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
                    self.ConvertToNormType(LRep);LRep = RepType::NormalType;
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

                            switch(LRep)
                            {
                            #if defined(AltNum_EnablePIRep)
                                case RepType::PINum:
                                    
                                    break;
                                #if defined(AltNum_EnablePIPowers)
                                case RepType::PIPower:
                                   
                                   break;
                                #endif
                            #endif
                                //RepType::ENum:
                                default://RepType::NormalType
                                    
                                    break;
                            }
                            break;
                    #endif
		            #if defined(AltNum_EnableMixedEFractional)
						case RepType::MixedE:

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
                        //(AltDec(self.IntValue, self.DecimalHalf)/self.ExtraRep) / Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep
                            break;
#if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
#elif defined(AltNum_EnableMixedEFractional)
						case RepType::MixedE:
#endif
#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)

#if (defined(AltNum_EnableMixedPiFractional)&&defined(AltNum_EnableDecimaledPiFractionals))||(defined(AltNum_EnableMixedEFractional)&&defined(AltNum_EnableDecimaledEFractionals))
                            
#else
                            
                            
#if defined(AltNum_EnableMixedPiFractional)
                                
#else
                                
#endif
#endif
		#endif
						default:
							self.CatchAllAddition(Value, LRep, RRep);
							break;
					}
					break;
#if defined(AltNum_EnableAlternativeRepFractionals)
	#if defined(AltNum_EnablePIRep)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //(self.IntValue*Pi)/self.DecimalHalf + (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)

#if defined(AltNum_EnableMixedPiFractional)
						//case RepType::MixedPi:
                        //(self.IntValue*Pi)/self.DecimalHalf + ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*Pi)
#endif
#if defined(AltNum_EnableMixedEFractional)
						//case RepType::MixedE:
                        //(self.IntValue*Pi)/self.DecimalHalf + ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*e)
#endif
						default:
							self.CatchAllAddition(Value, LRep, RRep);
							break;
					}
					break;
					
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //(self.IntValue*e)/self.DecimalHalf + (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
		#if defined(AltNum_EnablePINum)
						//case RepType::MixedPi:
                        //(self.IntValue*e)/self.DecimalHalf + (Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif
						default:
							self.CatchAllAddition(Value, LRep, RRep);
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
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep + (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
#if defined(AltNum_EnableMixedPiFractional)
						//case RepType::MixedPi:
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep + ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*Pi)
#endif
#if defined(AltNum_EnableMixedEFractional)
						//case RepType::MixedE:
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep + ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*e)
#endif
						default:
							self.CatchAllAddition(Value, LRep, RRep);
							break;
					}
					break;
#endif
                case RepType::MixedFrac:
					switch(RRep)
					{
		#if defined(AltNum_EnablePINum)
						//case RepType::MixedPi:
                        //(self.IntValue +- (self.DecimalHalf*-1)/self.ExtraRep)/-self.ExtraRep + ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*Pi)
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif
						default:
							self.CatchAllAddition(Value, LRep, RRep);
							break;
					}
                    break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi:
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep + (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif          //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep + (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
						default:
							self.CatchAllAddition(Value, LRep, RRep);
							break;
					}
                    break;
#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep + (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif          //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep + (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
						default:
							self.CatchAllAddition(Value, LRep, RRep);
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
					self.CatchAllAddition(Value, LRep, RRep);
					break;
		}
#endif