		void MixedFracRtRDivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
		{
		    switch(LRep)
            {
				case RepType::NormalType:
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
#endif
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
					switch(RRep)
					{
						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                            // (AltDec(self.IntValue, self.DecimalHalf)*(1 or Pi or Pi^-self.ExtraRep or e)) / (Value.IntValue +- (Value.DecimalHalf*-1)/Value.ExtraRep)
                            break;
                        #if defined(AltNum_EnableMixedPiFractional)
						case RepType::MixedPi:
                            // (AltDec(self.IntValue, self.DecimalHalf)*(1 or Pi or Pi^-self.ExtraRep or e)) / ((Value.IntValue +- (Value.DecimalHalf*-1)/-Value.ExtraRep)*Pi)
                            switch(LRep)
                            {
                            #if defined(AltNum_EnablePiRep)
                                case RepType::PiNum:
                                #if defined(AltNum_EnablePiPowers)//Become Value*Pi^2
                                    
                                #else
                                    
                                #endif
                                    break;
                                #if defined(AltNum_EnablePiPowers)
                                case RepType::PiPower:
                                   
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
                            //(AltDec(self.IntValue, self.DecimalHalf)*(1 or Pi or Pi^-self.ExtraRep or e)) / ((Value.IntValue +- (Value.DecimalHalf*-1)/-Value.ExtraRep)*e)
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
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
#if defined(AltNum_EnableAlternativeRepFractionals)
	#if defined(AltNum_EnablePiRep)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //(self.IntValue*Pi)/self.DecimalHalf / (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)

#if defined(AltNum_EnableMixedPiFractional)
						//case RepType::MixedPi:
                        //(self.IntValue*Pi)/self.DecimalHalf / ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*Pi)
#endif
#if defined(AltNum_EnableMixedEFractional)
						//case RepType::MixedE:
                        //(self.IntValue*Pi)/self.DecimalHalf / ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*e)
#endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
					
	#endif
	#if defined(AltNum_EnableERep)
				case RepType::EFractional://  IntValue/DecimalHalf*e Representation
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //(self.IntValue*e)/self.DecimalHalf / (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
		#if defined(AltNum_EnablePiNum)
						//case RepType::MixedPi:
                        //(self.IntValue*e)/self.DecimalHalf / (Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep / (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
#if defined(AltNum_EnableMixedPiFractional)
						//case RepType::MixedPi:
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep / ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*Pi)
#endif
#if defined(AltNum_EnableMixedEFractional)
						//case RepType::MixedE:
                        //(AltDec(self.IntValue, self.DecimalHalf)* (Pi Or e))/-self.ExtraRep / ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*e)
#endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
#endif
                case RepType::MixedFrac:
					switch(RRep)
					{
		#if defined(AltNum_EnablePiNum)
						//case RepType::MixedPi:
                        //(self.IntValue +- (self.DecimalHalf*-1)/self.ExtraRep)/-self.ExtraRep / ((Value.IntValue+(-Value.DecimalHalf)/-Value.ExtraRep)*Pi)
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
                    break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi:
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep / (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif          //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep / (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
                    break;
#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
					switch(RRep)
					{
						//case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                        //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep / (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
		#if defined(AltNum_EnableENum)
						//case RepType::MixedE:
		#endif          //((self.IntValue +- (self.DecimalHalf*-1)Pi)/-self.ExtraRep)/-self.ExtraRep / (Value.IntValue+(-Value.DecimalHalf)/Value.ExtraRep)
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
					self.CatchAllDivision(Value, LRep, RRep);
					break;
		}
#endif