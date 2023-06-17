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
                    case RepType::INum://Xi * Yi = XY
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
                    case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
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
							{	IntValue = NegativeRep; }
                            else//-1.0..1 * 2.0..1
								IntValue *= Value.IntValue;
                        }
                        else
                        {
                            if (Value.IntValue == 0)//1.0..1 * 0.0..1
							{	IntValue = 0; }
                            else//1.0..1 * 2.0..1
								IntValue *= Value.IntValue;
                        }
                        break;
                    case RepType::ApproachingTop://Just going to convert into normal numbers for now
						self.ConvertToNumRep();
						Value.ConvertToNumRep();
						self.PartialMultOp(Value);
                        break;

#if defined(AltNum_EnableApproachingDivided)
                    case RepType::ApproachingBottomDiv:
						if(Value.IntValue==0)
						{
							//-0.49..9 * 0.49..9 =  ~-0.249..9 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:4)
							//0.49..9 * 0.49..9(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:2)
                            // =  ~0.249..9 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:4)
							// 0.249..9 * 0.249..9 = ~0.06249..9(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:16)
							if(self.IntValue==0||self.IntValue==NegativeRep)
								self.ExtraRep *= Value.ExtraRep;
							else
							{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
								bool IsNegative = self.IntValue<0;
								if(IsNegative)
									self.IntValue *= -1;
								MediumDecVariant XV = SetAsApproachingMid(0, Value.ExtraRep)*self.IntValue;
								MediumDecVariant YV = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
								XV *= YV;
								if(IsNegative)
									self.IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
								self.DecimalHalf = XV.DecimalHalf;
								self.ExtraRep = XV.ExtraRep;
							}
							return self;
						}
						else
						{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
							bool IsNegative = self.IntValue<0;
							if(IsNegative)
								self.IntValue = self.IntValue==NegativeRep:0?-self.IntValue;
							int XZ = self.IntValue * Value.IntValue;
							MediumDecVariant XV = SetAsApproachingMid(0, Value.ExtraRep)*self.IntValue;
							MediumDecVariant YZ = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
							MediumDecVariant YV = SetAsApproachingMid(0, Self.ExtraRep)*SetAsApproachingMid(0, Value.ExtraRep);
							XV += XZ;
							XV += YZ+YV;
							if(IsNegative)
								self.IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
							self.DecimalHalf = XV.DecimalHalf;
							self.ExtraRep = XV.ExtraRep;
						}
                        break;
                    case RepType::ApproachingTop://Just going to convert into normal numbers for now
						if(Value.IntValue==0)
						{
							//0.50..1 * 0.50..1(IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:-2)
                            // =  ~0.250..1 (IntValue:0 DecimalHalf:ApproachingValueRep ExtraRep:-4)
							if(self.IntValue==0||self.IntValue==NegativeRep)
								self.ExtraRep *= -Value.ExtraRep;
							else
							{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
								bool IsNegative = self.IntValue<0;
								if(IsNegative)
									self.IntValue *= -1;
								MediumDecVariant XV = SetAsApproachingMid(0, Value.ExtraRep)*self.IntValue;
								MediumDecVariant YV = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
								XV *= YV;
								if(IsNegative)
									self.IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
								self.DecimalHalf = XV.DecimalHalf;
								self.ExtraRep = XV.ExtraRep;
							}
							return self;
						}
						else
						{//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
							bool IsNegative = self.IntValue<0;
							if(IsNegative)
								self.IntValue = self.IntValue==NegativeRep:0?-self.IntValue;
							int XZ = self.IntValue * Value.IntValue;
							MediumDecVariant XV = SetAsApproachingMid(0, Value.ExtraRep)*self.IntValue;
							MediumDecVariant YZ = SetAsApproachingMid(0, Self.ExtraRep)*Value.IntValue;
							MediumDecVariant YV = SetAsApproachingMid(0, Self.ExtraRep)*SetAsApproachingMid(0, Value.ExtraRep);
							XV += XZ;
							XV += YZ+YV;
							if(IsNegative)
								self.IntValue = XV.IntValue==0?NegativeRep:-XV.IntValue;
							self.DecimalHalf = XV.DecimalHalf;
							self.ExtraRep = XV.ExtraRep;
						}
                        break;
#endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
                    case RepType::NumByDiv://(IntValue.DecimalHalf)/ExtraRep
					//(self.(IntValue.DecimalHalf)/self.ExtraRep) * (Value.(IntValue.DecimalHalf)/Value.ExtraRep) = 
					//(self.(IntValue.DecimalHalf)*Value.(IntValue.DecimalHalf))/(self.ExtraRep*Value.ExtraRep)
						self.PartialMultOp(Value);
						self.ExtraRep *= Value.ExtraRep;
						break;
						
					//(Self.IntValue*Value.IntValue)/(self.DecimalHalf/Value.DecimalHalf)
#if defined(AltNum_EnablePIRep)
                    case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
						int NumRes = Self.IntValue*Value.IntValue;
						int DenomRes = self.DecimalHalf/Value.DecimalHalf;
						//Reduce size of fractional if viable
						signed _int64 DivRes = NumRes / DenomRes;
						signed _int64 RemRes = NumRes - DenomRes * NumRes;
						if(RemRes==0)
						{
#ifdef AltNum_EnablePIPowers
							//Set as DivRes*Pi^2
#else
							Self.SetEVal(DivRes);
						}
						else
						{
#ifdef AltNum_EnableBoostFractionalReduction//Add code here to reduce size of fractional using boost library code
#else
							Self.IntValue = NumRes;
							Self.DecimalHalf = DenomRes;
#endif
						}
#ifndef AltNum_EnablePIPowers
						Self *= PINumValue();
#endif
						break;
#endif
#if defined(AltNum_EnableENum)
                    case RepType::EFractional://  IntValue/DecimalHalf*e Representation
						
						int NumRes = Self.IntValue*Value.IntValue;
						int DenomRes = self.DecimalHalf/Value.DecimalHalf;
						//Reduce size of fractional if viable
						signed _int64 DivRes = NumRes / DenomRes;
						signed _int64 RemRes = NumRes - DenomRes * NumRes;
						if(RemRes==0)
						{
							Self.SetEVal(DivRes);
						}
						else
						{
#ifdef AltNum_EnableBoostFractionalReduction//Add code here to reduce size of fractional using boost library code
#else
							Self.IntValue = NumRes;
							Self.DecimalHalf = DenomRes;
#endif
						}
						Self *= ENumValue();
						break;
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)
                    case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
#endif
#if defined(AltNum_EnableDecimaledEFractionals)
                    case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
#endif
						break;
						
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