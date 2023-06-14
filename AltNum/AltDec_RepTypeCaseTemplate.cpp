                switch (LRep)
                {
                    case RepType::NormalType:
#if defined(AltNum_EnablePIRep)
                    case RepType::PINum:
#endif
#if defined(AltNum_EnableENum)
                    case RepType::ENum:
#endif
#if defined(AltNum_EnableImaginaryNum)
                    case RepType::INum:
#endif
#if defined(AltNum_EnableMixedFractional)
                    case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnablePINum)
                    case RepType::MixedPi:
#endif
#if defined(AltNum_EnableENum)
                    case RepType::MixedE:
#endif
#if defined(AltNum_EnableImaginaryNum)
                    case MixedI:
#endif
#endif
						self.BasicAddOp(Value);
						break;


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

//#if defined(AltNum_EnableAlternativeRepFractionals)
//                    case RepType::NumByDiv:
//#if defined(AltNum_EnablePIRep)
//                    case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
//#endif
//#if defined(AltNum_EnableENum)
//                    case RepType::EFractional://  IntValue/DecimalHalf*e Representation
//#endif
//#if defined(AltNum_EnableImaginaryNum)
//                    case RepType::IFractional://  IntValue/DecimalHalf*i Representation
//#endif
//#if defined(AltNum_EnableDecimaledPiFractionals)
//                    case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
//#endif
//#if defined(AltNum_EnableDecimaledEFractionals)
//                    case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
//#endif
//#if defined(AltNum_EnableImaginaryNum)
//                    case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
//#endif
//#if defined(AltNum_EnableMixedFractional)
//                    case RepType::MixedFracByDiv://IntValue +- (DecimalHalf*-1)/ExtraRep
//#if defined(AltNum_EnablePINum)
//                    case RepType::MixedPiByDiv:
//#endif
//#if defined(AltNum_EnableENum)
//                    case RepType::MixedEByDiv:
//#endif
//#if defined(AltNum_EnableImaginaryNum)
//                    case MixedI:
//#endif
//#endif
//                        break;
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