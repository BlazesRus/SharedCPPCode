        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>AltDec</returns>
        static MediumDecVariant& AddOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
            if (self == Zero)
            {
                self.IntValue = Value.IntValue; self.DecimalHalf = Value.DecimalHalf;
                self.ExtraRep = Value.ExtraRep; return self;
            }
            else if (Value == Zero)
                return self;
#if defined(AltNum_EnableInfinityRep)
            if (self.DecimalHalf==InfinityRep)
                return self;
            if (Value.DecimalHalf == InfinityRep)
            {
                Value.IntValue == 1 ? self.SetAsInfinity() : self.SetAsNegativeInfinity();
                return self;
            }
#endif
            RepType LRep = self.GetRepType();
            RepType RRep = Value.GetRepType();
            if(LRep==RRep)
            {
                switch (LRep)
                {
                    case RepType::NormalType:
                    case RepType::PINum:
#if defined(AltNum_EnableENum)
                    case RepType::ENum:
#elif defined(AltNum_EnableImaginaryNum)
                    case RepType::INum:
#endif
                        self.BasicAddOp(Value);
                        break;

#if AltNum_EnableMixedFractional
#endif
                    case RepType::ApproachingBottom:
                        if (self.IntValue == NegativeRep)
                        {
                            if (Value.IntValue == 0)//0.01-0.01
                                self.SetAsZero();
                            else if (Value.IntValue == NegativeRep)//-0.01 - 0.01
                            {/*Do Nothing*/}
                            else if (Value.IntValue < 0)//-0.01 - 1.01
                            {
                                self.IntValue = Value.IntValue;
                            }
                            else
                            {
                            }
                        }
                        else if (Value.IntValue == NegativeRep)
                        {
                            if (self.IntValue == 0)//0.01-0.01
                                self.SetAsZero();
                            else if (self.IntValue < 0)//-1.01 - 0.01
                            {/*Do Nothing*/}
                            else
                            {
                            }
                        }
                        else if (self.IntValue < 0)
                        {
                            if (Value.IntValue < 0)//-1.01 - 1.01
                            {
                                self.IntValue += Value.IntValue;
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            if (Value.IntValue < 0)
                            {
                            }
                            else//1.01+1.01
                            {
                                self.IntValue += Value.IntValue;
                            }
                        }
                        break;
                    case RepType::ApproachingTop:
                        if (self.IntValue == NegativeRep)
                        {
                            if (Value.IntValue == 0)//0.9-0.9
                                self.SetAsZero();
                            else if (Value.IntValue == NegativeRep)//-0.9 - 0.9
                                self.IntValue = -1;
                            else if (Value.IntValue < 0)//-0.9 - 1.9
                            {
                                self.IntValue = Value.IntValue - 1;
                            }
                            else
                            {
                            }
                        }
                        else if (Value.IntValue == NegativeRep)
                        {
                            if (self.IntValue == 0)//0.9-0.9
                                self.SetAsZero();
                            else if (self.IntValue < 0)//-1.9 - 0.9
                            {
                                --self.IntValue;
                            }
                            else
                            {
                            }
                        }
                        else if (self.IntValue < 0)
                        {
                            if (Value.IntValue < 0)//-1.9 - 1.9
                            {
                                self.IntValue += Value.IntValue - 1;
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            if (Value.IntValue < 0)
                            {
                            }
                            else
                            {
                                self.IntValue += Value.IntValue;
                            }
                        }
                        break;
                    case RepType::NumByDiv:
                        if(self.ExtraRep==Value.ExtraRep)
                            self.BasicAddOp(Value);
                        else
                            self.CatchAllAddition(Value, LRep, RRep);
                        break;
#if defined(AltNum_EnableENum)
                    case RepType::ENumByDiv:
                        if (self.ExtraRep == Value.ExtraRep)
                            self.BasicAddOp(Value);
                        else
                            self.CatchAllAddition(Value, LRep, RRep);
                        break;
#elif defined(AltNum_EnableImaginaryNum)
                    case RepType::INumByDiv:
                        if (self.ExtraRep == Value.ExtraRep)
                            self.BasicAddOp(Value);
                        else
                            self.CatchAllAddition(Value, LRep, RRep);
                        break;
#endif
                    default:
                        self.CatchAllAddition(Value, LRep, RRep);
                        break;
                }
            }
            else
            {
                RepToRepAddOp(LRep, RRep, self, Value);
            }
            return self;
        }
		
        void RepToRepAddOp(RepType& LRep, RepType& RRep, MediumDecVariant& self, MediumDecVariant& Value)
        {
            switch (LRep)
            {
                //X.?? + ???
                case RepType::NormalType:
                    switch (RRep)
                    {
                        case RepType::NumByDiv:
                            Value.ConvertToNormType(RRep);
                            self.BasicAddOp(Value);
                            break;
#if defined(AltNum_EnablePIRep)
                        case RepType::PINum:
                            Value.ConvertPIToNum();
                            self.BasicAddOp(Value);
                            break;
#endif
#if defined(AltNum_EnableENum)
                        case RepType::ENum:
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledEFractionals)
                        case RepType::ENumByDiv:
#endif
                        case RepType::EFractional:
                            Value.ConvertEToNum();
                            self.BasicAddOp(Value);
                            break;
#endif
#if defined(AltNum_EnableImaginaryNum)
                        case RepType::INum:
#ifndef AltNum_EnableComplexNumbers
                            throw "Complex number operation not enabled";
#else
                            throw "Complex number operation not supported yet";
#endif
                            break;
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
                        case RepType::INumByDiv:
#endif
                        case RepType::IFractional:
							throw "Normal RepType addition with"+static_cast<RepType>(RRep)+"detected not supported yet";
                            break;
#endif
                        case RepType::UnknownType:
							throw "Normal representation addition with"+static_cast<RepType>(Value.RepType)+"detected not supported yet.";
                        default:
							self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                //(X.??*PI) + ???
                case RepType::PINum:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertPIToNum();
                            self.BasicAddOp(Value);
                            break;
#if defined(AltNum_EnableImaginaryNum)
                        case RepType::INum:
#ifndef AltNum_EnableComplexNumbers
                            throw "Complex number operation not enabled";
#else
                            throw "Complex number operation not supported yet";
#endif
                            break;
#if defined(AltNum_EnableAlternativeRepFractionals)
#if defined(AltNum_EnableDecimaledIFractionals)
                        case RepType::INumByDiv:
#endif
                        case RepType::IFractional:
							throw "Normal RepType addition with"+static_cast<RepType>(RRep)+"detected not supported yet";
                            break;
#endif
                        case RepType::UnknownType:
                            throw "Pi RepType addition with"+static_cast<RepType>(Value.RepType)+"detected not supported yet.";
                        default:
                        //case RepType::ENum: //(X.??*PI) + (Y.??*e)
                        //case RepType::ENumByDiv: //(X.??*PI) + ((Y.??*e)/(Y_ExtraRep*-1))
                        //case RepType::ApproachingTop: //(X.??*PI) + Y.9...9
                        //case RepType::ApproachingBottom: //(X.??*PI) + Y.0...1
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                //(X.??/X_ExtraRep) + ???
                case RepType::NumByDiv:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertToNormType(LRep);
                            self.BasicAddOp(Value);
                            break;
                        case RepType::ApproachingTop:
                            self.ConvertToNormType(LRep);
                            self.BasicAddOp(AlmostOne);
                            break;
                        case RepType::ApproachingBottom:
                            self.ConvertToNormType(LRep);
                            self.BasicAddOp(JustAboveZero);
                            break;
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingBottom://IntValue.000...1 + ???
                    switch (RRep)
                    {
                        case RepType::ApproachingTop:
                            if(self.IntValue<0)
                            {
                                if (self.IntValue == NegativeRep)
                                {
                                    if(Value.IntValue == NegativeRep)//-0.0.......1 + -0.9.......9
                                    {
                                        self.IntValue = -1; self.DecimalHalf = 0;
                                        self.ExtraRep = 0; return self;
                                    }
                                    else if(Value.IntValue<0)//-0.0.......1 + -?.9.......9
                                    {
                                        self.IntValue--;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                        self.IntValue += Value.IntValue;
                                    }
                                    else
                                    {
                                        self.CatchAllAddition(Value, LRep, RRep);
                                    }
                                }
                                else
                                {
                                    if(Value.IntValue == NegativeRep)//-?.9.......9 + -0.0.......1
                                    {
                                        self.IntValue--;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                    }
                                    else if(Value.IntValue<0)//-?.9.......9 + -?.0.......1
                                    {
                                        self.IntValue--;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                        self.IntValue += Value.IntValue;
                                    }
                                    else
                                    {
                                        self.CatchAllAddition(Value, LRep, RRep);
                                    }
                                }
                            }
                            else
                            {
                                if(Value.IntValue<0)
                                {
                                    self.CatchAllAddition(Value, LRep, RRep);
                                }
                                else if(self.IntValue==0)//0.99...9 + Y.00...1
                                {
                                    self.IntValue = Value.IntValue+1;
                                    self.ExtraRep = 0; self.DecimalHalf = 0;
                                }
                                else//X.99...9 + Y.00...1
                                {
                                    self.IntValue++;
                                    self.ExtraRep = 0; self.DecimalHalf = 0;
                                    self.IntValue += Value.IntValue;
                                }
                            }
       //                     if (self.IntValue == NegativeRep)
       //                     {
       //                         if (Value.IntValue == NegativeRep)//-0.0.......1 + -0.9.......9
       //                         {
       //                             self.IntValue = -1; self.DecimalHalf = 0;
       //                             self.ExtraRep = 0; return self;
       //                         }
       //                     }
       //                     else if (Value.IntValue == NegativeRep)
       //                     {
       //					  	self.CatchAllAddition(Value, LRep, RRep);
       //                     }
       //                     else if (self.IntValue < 0)
       //                     {
       //                         if (Value.IntValue < 0)
       //                         {
       //                             self.IntValue++;
       //                             self.ExtraRep = 0;
       //                         }
       //                         self.IntValue += Value.IntValue;
       //                     }
       //                     else
       //                     {
       //                         if (Value.IntValue>= 0)
       //                         {
       //                             self.IntValue++;
       //                             self.ExtraRep = 0;
       //                         }
       //                         self.IntValue += Value.IntValue;
       //                     }
                            break;
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ApproachingTop://IntValue.9999... + ???
                    switch (RRep)
                    {
                        case RepType::ApproachingBottom:
                            if(self.IntValue<0)
                            {
                                if (self.IntValue == NegativeRep)
                                {
                                    if(Value.IntValue == NegativeRep)//-0.9.......9 + -0.0.......1
                                    {
                                        self.IntValue = -1; self.DecimalHalf = 0;
                                        self.ExtraRep = 0; return self;
                                    }
                                    else if(Value.IntValue<0)//-0.9.......9 + -?.0.......1
                                    {
                                        self.IntValue = Value.IntValue - 1;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                    }
                                    else
                                    {
                                        self.CatchAllAddition(Value, LRep, RRep);
                                    }
                                }
                                else
                                {
                                    if(Value.IntValue == NegativeRep)//-?.9.......9 + -0.0.......1
                                    {
                                        self.IntValue--;
                                        self.ExtraRep = 0; self.DecimalHalf = 0;
                                    }
                                    else if(Value.IntValue<0)//-?.9.......9 + -?.0.......1
                                    {
                                        self.IntValue--; self.DecimalHalf = 0;
                                        self.ExtraRep = 0;
                                        self.IntValue += Value.IntValue;
                                    }
                                    else
                                    {
                                        self.CatchAllAddition(Value, LRep, RRep);
                                    }
                                }
                            }
                            else
                            {
                                if(Value.IntValue<0)
                                {
                                    self.CatchAllAddition(Value, LRep, RRep);
                                }
                                else if(self.IntValue==0)//0.99...9 + Y.00...1
                                {
                                    self.IntValue = Value.IntValue+1;
                                    self.ExtraRep = 0; self.DecimalHalf = 0;
                                }
                                else//X.99...9 + Y.00...1
                                {
                                    self.IntValue++;
                                    self.ExtraRep = 0; self.DecimalHalf = 0;
                                    self.IntValue += Value.IntValue;
                                }
                            }
                            break;
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#if defined(AltNum_EnableENum)
                case RepType::ENum:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertEToNum();
                            self.BasicAddOp(Value);
                            break;
      //                  case RepType::ENumByDiv:
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
                case RepType::ENumByDiv:
                    switch (RRep)
                    {
                        case RepType::NormalType:
                            self.ConvertEToNum();
                            self.BasicAddOp(Value);
                            break;
      //                  case RepType::ENum:
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#elif defined(AltNum_EnableImaginaryNum)
                case RepType::INum:
                    //if(RRep==RepType::INumByDiv)
                    //{
                    //}
                    //else
                    //{
                        throw "Can't add imaginary numbers to real numbers unless complex representation enabled.";
                    //}
                    break;
#elif defined(AltNum_EnablePIPowers)
                case RepType::PIPower:
                    switch (RRep)
                    {
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#endif
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac:
                    switch (RRep)
                    {
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#if defined(AltNum_EnableENum)
                case RepType::MixedE:
                    switch (RRep)
                    {
                        default:
                            self.CatchAllAddition(Value, LRep, RRep);
                            break;
                    }
                    break;
#elif defined(AltNum_EnableImaginaryNum)
                case RepType::MixedE:
#if defined(AltNum_EnableComplexNum)
     //               switch (RRep)
     //               {
     //                   default:
     //                       self.CatchAllAddition(Value, LRep, RRep);
     //                       break;
                    //}
                    //break;
//                case RepType::ComplexIRep:
//                    switch (RRep)
//                    {
////                        case RepType::NormalType:
////                            break;
//                        default:
//                            self.CatchAllAddition(Value, LRep, RRep);
//                            break;
//					}
//					break;
#else
                    switch (RRep)
                    {
                        //case RepType::INum:
                        //	break;
                        //case RepType::INumByDiv:
                        //	break;
                        default:
                            throw "Can't add imaginary numbers to real numbers unless complex representation enabled.";
                            break;
                    }
                    break;
#endif
#endif
                default:
                    self.CatchAllAddition(Value, LRep, RRep);
                    break;
                
            }
        }
		
        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecVariant&</returns>
        static MediumDecVariant& SubOp(MediumDecVariant& self, MediumDecVariant& Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if (self.DecimalHalf == InfinityRep)
                return self;
            if (Value.DecimalHalf == InfinityRep)
            {
                Value.IntValue == 1 ? self.SetAsInfinity() : self.SetAsNegativeInfinity();
                return self;
            }
#endif
            RepType LRep = self.GetRepType();
            RepType RRep = Value.GetRepType();
            if(LRep==RRep)
            {
                if(self.ExtraRep==0||Value.ExtraRep==PIRep)
                {
//#if AltNum_EnableMixedFractional
//                    if(self.DecimalHalf<0)//MixedFractional
//                    {
//                    
//                    }
//                    else
//                    {
//#endif
//                        self.BasicSubOp(Value);
//#if AltNum_EnableMixedFractional
//                    }
//#endif
//                }
//    #if defined(AltNum_EnableImaginaryNum)
//                else if(self.ExtraRep==IERep)
//                {
//                }
//                else if(self.ExtraRep>0)
//    #elif defined(AltNum_EnableENum)
//                else if(self.ExtraRep==IERep)
//                {
//                }
//                else if(self.ExtraRep>0)
//    #else
//                else//(Value/ExtraRep) Representation
//    #endif
//                {
//                
//                }
//    #if defined(AltNum_EnableImaginaryNum) || defined(AltNum_EnableENum)
//                else
//                {
//                
//                }
//    #endif
//                if(self.ExtraRep!=0&&self.IntValue==0&&self.DecimalHalf==0)
//                    self.ExtraRep = 0;
            }
            else
            {
                RepToRepSubOp(LRep, RRep, self, Value);
            }
            return self;
        }