                switch (LRep)
                {
                    case RepType::NormalType:
						self.BasicAddOp(Value);
						break;
//#if defined(AltNum_EnablePIRep)
//                    case RepType::PINum:
//					    break;
//#endif
//#if defined(AltNum_EnableENum)
//                    case RepType::ENum:
//#endif
//#if defined(AltNum_EnableImaginaryNum)
//                    case RepType::INum:
//                        break;
//#endif
//
//#if AltNum_EnableMixedFractional
//                    case RepType::ComplexIRep:
//                        break;
//                    case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
//						break;
//#if defined(AltNum_EnableENum)
//                    case RepType::MixedE:
//						break;
//#endif
//#if defined(AltNum_EnableImaginaryNum)
//                    case MixedI:
//						break;
//#endif
//#endif
//
//#if defined(AltNum_EnableApproachingDivided)
//                    case RepType::ApproachingBottom:
//                        break;
//                    case RepType::ApproachingTop:
//                        break;
//#if defined(AltNum_EnableApproachingDivided)
//                    case RepType::ApproachingBottom:
//                        break;
//                    case RepType::ApproachingTop:
//                        break;
//#endif
//#endif
//
//#if defined(AltNum_EnableAlternativeRepFractionals)
//                    case RepType::NumByDiv:
//                        break;
//#if defined(AltNum_EnablePIRep)
//                    case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//                        break;
//#endif
//#if defined(AltNum_EnableENum)
//                    case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//                        break;
//#endif
//#if defined(AltNum_EnableImaginaryNum)
//                    case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//                        break;
//#endif
//#if defined(AltNum_EnableDecimaledPiFractionals)
//                    case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
//                        break;
//#endif
//#if defined(AltNum_EnableDecimaledEFractionals)
//                    case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
//                        break;
//#endif
//#if defined(AltNum_EnableImaginaryNum)
//                    case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
//                        break;
//#endif
//#endif
//#if defined(AltNum_EnablePIPowers)
//                    case RepType::PIPower:
//						break;
//#endif
//#if defined(AltNum_EnableComplexNumbers)
//                    case RepType::ComplexIRep:
//						break;
//#endif
//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
//                    case RepType::UndefinedButInRange:
//						break;
//#endif
                    case RepType::Undefined:
                    case RepType::NaN:
						throw "Can't perform operations with NaN or Undefined number";
						break;
#if defined(AltNum_EnableNegativeZero)
                    case RepType::NegativeZero://Treat operation as with Zero in most cases(only used in very rare cases)
						break;
#endif
                    default:
						throw static_cast<RepType>(LRep)+" RepType addition not supported yet";
						//throw static_cast<RepType>(LRep)+" RepType addition with"+static_cast<RepType>(RRep)+"not supported yet";
                        break;
                }