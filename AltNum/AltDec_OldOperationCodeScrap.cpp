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
		
		