		void MixedFracRtRMultOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
		{
		    switch(LRep)
            {
				case RepType::NormalType:
#if defined(AltNum_EnablePIRep)
                case RepType::PINum:
#if defined(AltNum_EnablePIPowers)
                case RepType::PIPower:
#endif
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
					MediumDecVariant LeftSide = self*Value.IntValue;
                    MediumDecVariant RightSide = self/Value.ExtraRep;
                    RightSide *= -Value.DecimalHalf;
                    self = LeftSide+RightSide;
					switch(RRep)
					{
						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
                            break;
		#if defined(AltNum_EnablePINum)
						case RepType::MixedPiByDiv:
                            switch(LRep)
                            {
                #if defined(AltNum_EnablePIRep)
                                case RepType::PINum:
                #if defined(AltNum_EnablePIPowers)//Become Value*Pi^2
                                    self.ExtraRep = -1;
                #else
                                    self.BasicDivOp(PiNumValue());
                #endif
                                    break;
                #if defined(AltNum_EnablePIPowers)
                                case RepType::PIPower:
                                   ++self.ExtraRep;
                                   break;
                #endif
                #endif
                                //RepType::ENum:
                                default://RepType::NormalType
                                    self.BasicMultOp(PiNumValue());
                                    break;
                            }
                            break;
		#endif
		#if defined(AltNum_EnableENum)
						case RepType::MixedEByDiv:
                            self.BasicMultOp(ENumValue());
                            break;
		#endif
						default:
							throw "Incorrect RepToRep function used(Or not enabled).";
							break;
					}
					break;
				case RepType::NumByDiv:
                    //X.Value/X.ExtraRep * Y.IntValue+(-Y.DecimalHalf)/Y.ExtraRep
                    //=
					switch(RRep)
					{
						//case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnablePINum)
						//case RepType::MixedPiByDiv:
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedEByDiv:
		#endif
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
#if defined(AltNum_EnableAlternativeRepFractionals)
	#if defined(AltNum_EnablePIRep)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					switch(RRep)
					{
						//case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnablePINum)
						//case RepType::MixedPiByDiv:
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedEByDiv:
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
						//case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnablePINum)
						//case RepType::MixedPiByDiv:
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedEByDiv:
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
						//case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
		#if defined(AltNum_EnablePINum)
						//case RepType::MixedPiByDiv:
		#endif
		#if defined(AltNum_EnableENum)
						//case RepType::MixedEByDiv:
		#endif
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
					self.CatchAllMultiplication(Value, LRep, RRep);
					break;
		}
#endif