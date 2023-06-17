                switch (LRep)
                {
                    case RepType::NormalType:
#if defined(AltNum_EnablePIRep)
                    case RepType::PINum:
#endif
#if defined(AltNum_EnableENum)
                    case RepType::ENum:
#endif
						self.BasicSubOp(Value);
						break;

#if defined(AltNum_EnableImaginaryNum)
                    case RepType::INum:
#ifndef AltNum_EnableComplexNumbers
						throw "Complex number operation not enabled";
#else
						//Complex number operation code here later
#endif
						break;
#endif
						
#if defined(AltNum_EnableMixedFractional)
                    case RepType::MixedFrac://IntValue -- (DecimalHalf*-1)
#if defined(AltNum_EnablePINum)
                    case RepType::MixedPi:
#endif
#if defined(AltNum_EnableENum)
                    case RepType::MixedE:
#endif
#if defined(AltNum_EnableImaginaryNum)
                    case MixedI:
#endif
						throw "BasicMixedSubOp code not implimented yet";//self.BasicMixedSubOp(Value);
						break;
#endif

#if defined(AltNum_EnableApproachingDivided)
                    case RepType::ApproachingBottom:
                        if (self.IntValue == NegativeRep)
                        {
							if (Value < 0)//-0.0..1 + 1

                            else//-0.0..1 - 5

                        }
                        if (self.IntValue == 0)
                        {
                            if (Value < 0)//0.0..1 + 1

                            else//0.0..1 - 5

                        }
                        else if (self.IntValue < 0)
                        {
							if (Value < 0)//-1.0..1 + 2
                            {
								DecimalHalf = 0;
								self.IntValue -= Value;
                            }
                            else//-1.0..1 - 2.0..1 = 1
                            {
								self.IntValue -= Value;
                            }
                        }
                        else
                        {
                            if (Value < 0)// 1.0..1  - 2

                            else//1.0..1 - 1
							
                        }
                        break;
                    case RepType::ApproachingTop:
                        if (self.IntValue == NegativeRep)
                        {
                            if (Value < 0)//-0.9..9 + 1

                            else//-0.9..9 - 5

                        }
                        if (self.IntValue == 0)
                        {
                            if (Value < 0)//0.9..9 + 1

                            else//0.9..9 - 5.9..9 = -5

                        }
                        else if (self.IntValue < 0)
                        {
                            if (Value == 0)//-1.9..9 - 0

                            else if (Value < 0)//-1.9..9 + 2

                            else//-1.9..9 - 2

                        }
                        else
                        {
                            if (Value == 0)//1.9..9 - 0

                            else if (Value < 0)// 1.9..9  - 2

                            else//1.9..9 - 2

                        }
                        break;

//#if defined(AltNum_EnableApproachingDivided)
//                    case RepType::ApproachingBottom:
//                        break;
//                    case RepType::ApproachingTop:
//                        break;
//#endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
                    case RepType::NumByDiv:
#if defined(AltNum_EnablePIRep)
                    case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
#endif
#if defined(AltNum_EnableENum)
                    case RepType::EFractional://  IntValue/DecimalHalf*e Representation
#endif
                        if (self.DecimalHalf == Value.DecimalHalf)
						{
                            self -= Value;
						}
						else
						{
							self.ConvertToNormalRep(LRep); value.ConvertToNormalRep(LRep);
							self.BasicSubOp(Value);
						}
						break;

#if defined(AltNum_EnableDecimaledPiFractionals)
                    case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
#endif
#if defined(AltNum_EnableDecimaledEFractionals)
                    case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
#endif
                        if (self.ExtraRep == Value.ExtraRep)
                            self.BasicSubOp(Value);
						else
						{
							self.ConvertToNormalRep(LRep); value.ConvertToNormalRep(LRep);
							self.BasicSubOp(Value);
						}
						break;
						
#if defined(AltNum_EnableMixedFractional)
                    case RepType::MixedFracByDiv://IntValue -- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnablePINum)
                    case RepType::MixedPiByDiv:
#endif
#if defined(AltNum_EnableENum)
                    case RepType::MixedEByDiv:
#endif
#endif
						throw "BasicMixedSubOp code not implimented yet";
                        break;
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
                    case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                        if (self.DecimalHalf == Value.DecimalHalf)
						{
                            self -= Value;
						}
						else
						{
							self.ConvertToNormalIRep(LRep); value.ConvertToNormalIRep(LRep);
							self.BasicSubOp(Value);
						}
						break;
						
                    case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
                        if (self.ExtraRep == Value.ExtraRep)
                            self.BasicSubOp(Value);
						else
						{
							self.ConvertToNormalIRep(LRep); value.ConvertToNormalIRep(LRep);
							self.BasicSubOp(Value);
						}
						break;
                    case MixedI:
						throw "BasicMixedSubOp code not implimented yet";
                        break;
#endif

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
						throw static_cast<RepType>(LRep)-" RepType subtraction not supported yet";
						//throw static_cast<RepType>(LRep)-" RepType subtraction with"-static_cast<RepType>(RRep)-"not supported yet";
                        break;
                }