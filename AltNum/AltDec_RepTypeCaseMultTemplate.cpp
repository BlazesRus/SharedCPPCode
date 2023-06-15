                switch (LRep)
                {
                    case RepType::NormalType:
						self.PartialMultOp(Value);
						break;
#if defined(AltNum_EnablePIRep)&&!defined(AltNum_EnablePIPowers)
                    case RepType::PINum:
						self.PartialMultOp(Value);
						self.PartialMultOp(PINum);
						break;		
#endif
#if defined(AltNum_EnableENum)
                    case RepType::ENum:
						self.PartialMultOp(Value);
						self.PartialMultOp(ENum);
						break;
#endif
#if defined(AltNum_EnableImaginaryNum)
                    case RepType::INum://Xi * Xi = XX
						ExtraRep = 0;
						self.PartialMultOp(Value);
						break;
#endif
						
#if defined(AltNum_EnablePIRep)&&defined(AltNum_EnablePIPowers)
                    case RepType::PINum:
						//Add code that converts into PiPower type representation here later
						break;
                    case RepType::PIPower:
						//Add Pi powers code here later
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
						throw "BasicMixedMultOp code not implimented yet";//self.BasicMixedMultOp(Value);
						break;
#endif

#if defined(AltNum_EnableApproachingDivided)
                    case RepType::ApproachingBottom:
                        if (self.IntValue == NegativeRep)
                        {
       //                     if (Value.IntValue == 0)//-0.0..1 * 0.0..1
       //                     {/*Do Nothing*/}
       //                     else//-0.0..1 * 5.0..1
							//{/*Do Nothing*/}
							return self;
                        }
                        if (self.IntValue == 0)
                        {
       //                     if (Value.IntValue == 0)//0.0..1 * 0.0..1
       //                     {/*Do Nothing*/}
       //                     else//0.0..1 * 5.0..1
							//{/*Do Nothing*/}
							return self;
                        }
                        else if (self.IntValue < 0)
                        {
                            if (Value.IntValue == 0)//-1.0..1 * 0.0..1
							{	IntValue = NegativeRep; ExtraRep = 0; }
                            else//-1.0..1 * 2.0..1

                        }
                        else
                        {
                            if (Value.IntValue == 0)//1.0..1 * 0.0..1
							{	IntValue = 0; ExtraRep = 0; }
                            else//1.0..1 * 1.0..1

                        }
                        break;
                    case RepType::ApproachingTop:
                        if (self.IntValue == NegativeRep)
                        {
       //                     if (Value.IntValue == 0)//-0.9..9 * 0.9..9
							//{/*Do Nothing*/}
       //                     else//-0.9..9 * 5.9..9
							//{/*Do Nothing*/}
                        }
                        if (self.IntValue == 0)
                        {
                            if (Value.IntValue == 0)//0.9..9 * 0.9..9
							{/*Do Nothing*/}
                            else//0.9..9 * 5.9..9

                        }
                        else if (self.IntValue < 0)
                        {
                            if (Value.IntValue == 0)//-1.9..9 * 0.9..9  = -2.9..8
							{/*Do Nothing*/}
                            else//-1.9..9 * 2.9..9

                        }
                        else
                        {
                            if (Value.IntValue == 0)//1.9..9 * 0.9..9
							{/*Do Nothing*/}
                            else//1.9..9 * 2.9..9

                        }
                        break;

//#if defined(AltNum_EnableApproachingDivided)
//                    case RepType::ApproachingBottom:
//                        break;
//                    case RepType::ApproachingTop:
//                        break;
//#endif
#endif

/*#if defined(AltNum_EnableAlternativeRepFractionals)
                    case RepType::NumByDiv:
#if defined(AltNum_EnablePIRep)
                    case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
#endif
#if defined(AltNum_EnableENum)
                    case RepType::EFractional://  IntValue/DecimalHalf*e Representation
#endif

						break;*/

/*#if defined(AltNum_EnableDecimaledPiFractionals)
                    case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
#endif
#if defined(AltNum_EnableDecimaledEFractionals)
                    case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
#endif

						break;*/
						
#if defined(AltNum_EnableMixedFractional)
/*                    case RepType::MixedFracByDiv://IntValue -- (DecimalHalf*-1)/ExtraRep
#if defined(AltNum_EnablePINum)
                    case RepType::MixedPiByDiv:
#endif
#if defined(AltNum_EnableENum)
                    case RepType::MixedEByDiv:
#endif
#endif
						throw "BasicMixedMultOp code not implimented yet";
                        break;*/
#endif

#if defined(AltNum_EnableAlternativeRepFractionals) && defined(AltNum_EnableImaginaryNum)
/*                    case RepType::IFractional://  IntValue/DecimalHalf*i Representation

						break;
						
                    case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation

						break;
                    case MixedI:
						throw "BasicMixedMultOp code not implimented yet";
                        break;*/
#endif

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