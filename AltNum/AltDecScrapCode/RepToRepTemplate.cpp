		void RepToRepDivOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
        {
#if defined(AltNum_EnableNegativeZero)//Treat Negative Zero as zero
			if(LRep==RepType::NegativeZero)
			{
				self.SetAsZero(); LRep = RepType::NormalType; 
			}
			if(RRep==RepType::NegativeZero)
			{
				Value.SetAsZero(); RRep = RepType::NormalType; 
			}
#endif
			switch (LRep)
			{
				case RepType::NormalType:
					switch (RRep)
					{
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//	#endif
//	#if (defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)) || defined(AltNum_EnableENum) || defined(AltNum_EnableImaginaryNum)
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
				case RepType::PINum:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#endif
	#if defined(AltNum_EnableENum)
				case RepType::ENum:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
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
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						//case RepType::UnknownType:
						//	throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
							break;
					}
					break;
	#endif
					
	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
				case RepType::PINum:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
				case RepType::PIPower:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//						case RepType::PINum:
//							break;
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif					
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
	#endif
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#endif

	#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingBottom:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
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
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
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
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
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
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
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
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
					
	#if defined(AltNum_EnablePIRep)
				case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
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
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#endif

	#if defined(AltNum_EnableDecimaledPiFractionals)
				case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
				
	#endif
	#if defined(AltNum_EnableDecimaledEFractionals)
				case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
	#endif
	#if defined(AltNum_EnableDecimaledPiFractionals)||defined(AltNum_EnableDecimaledEFractionals)//Only one of the 2 switches can be active at once
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
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
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#endif

	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
				case RepType::IFractional://  IntValue/DecimalHalf*i Representation
					break;
					
				case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
				
					break;
				case RepType::MixedI:
					switch (RRep)
					{
//						case RepType::NormalType:
//							break;
//	#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::ENum:
//							break;
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::INum:
//							break;
//	#endif
//							
//	#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
//						case RepType::PINum:
//							//Add code that converts into PiPower type representation here later
//							break;
//						case RepType::PIPower:
//							//Add Pi powers code here later
//							break;
//	#endif
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPi:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedE:
//	#endif
//	#if defined(AltNum_EnableImaginaryNum)
//						case RepType::MixedI:
//	#endif
//							throw "BasicMixedOp code not implimented yet";//self.BasicMixedDivOp(Value);
//							break;
//	#endif
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottom:
//							break;
//						case RepType::ApproachingTop:
//							break;
//
//	#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingBottomDiv:
//							break;
//						case RepType::ApproachingTopDiv:
//							break;
//	#endif
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals)
//						case RepType::NumByDiv:
//							break;
//							
//	#if defined(AltNum_EnablePIRep)
//						case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//							break;
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//							break;
//	#endif
//
//	#if defined(AltNum_EnableDecimaledPiFractionals)
//						case RepType::PiNumByDiv://  (Value/(ExtraRep/-1))*Pi Representation
//	#endif
//	#if defined(AltNum_EnableDecimaledEFractionals)
//						case RepType::ENumByDiv://(Value/(ExtraRep/-1))*e Representation
//	#endif
//							break;
//							
//	#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//	#if defined(AltNum_EnablePINum)
//						case RepType::MixedPiByDiv:
//	#endif
//	#if defined(AltNum_EnableENum)
//						case RepType::MixedEByDiv:
//	#endif
//	#endif
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
//						case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//							break;
//							
//						case RepType::INumByDiv://(Value/(ExtraRep/-1))*i Representation
//							break;
//						case RepType::MixedI:
//							throw "BasicMixedDivOp code not implimented yet";
//							break;//
//	#endif
//
//	//#if defined(AltNum_EnableComplexNumbers)
//	//                    case RepType::ComplexIRep:
//	//						break;
//	//#endif
//	//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//	//                    case RepType::UndefinedButInRange:
//	//						break;
//	//#endif
//						case RepType::Undefined:
//						case RepType::NaN:
//							throw "Can't perform operations with NaN or Undefined number";
//							break;
//	#if defined(AltNum_EnableNegativeZero)
//						case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
//							break;
//	#endif
						case RepType::UnknownType:
							throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
						default:
							self.CatchAllDivision(Value, LRep, RRep);
							break;
					}
					break;
	#endif

	//#if defined(AltNum_EnableComplexNumbers)
	//                    //based on https://www.varsitytutors.com/sat_mathematics-help/working-with-imaginary-numbers
	//					  //(a+b)(a-b)=(a^2)-(b^2)
	//                    //(x+vi)/(y+zi) =
	//                    //(x-vi)/(y-zi) =
	//                    //(x+vi)/(y-zi) =
	//                    //(x-vi)/(y+zi) =
	//                    case RepType::ComplexIRep:
	//						break;
	//#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
				case RepType::UndefinedButInRange:
					throw static_cast<RepType>(LRep)-" RepType division with"-static_cast<RepType>(RRep)-"not supported yet";
					break;
	#endif
				case RepType::Undefined:
				case RepType::NaN:
					throw "Can't perform operations with NaN or Undefined number";
					break;
	//#if defined(AltNum_EnableNegativeZero)
	//			case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
	//				break;
	//#endif
				default:
					throw static_cast<RepType>(LRep)-" RepType subtraction with"-static_cast<RepType>(RRep)-"not supported yet";
					break;
			}