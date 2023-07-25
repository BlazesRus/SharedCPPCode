#define AltNum_EnableInfinityRep
#define AltNum_EnableImaginaryNum
 
    #pragma region Comparison Operators
        /// <summary>
        /// Equal to Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator==(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
    #if defined(AltNum_EnableImaginaryNum)
                if(self.DecimalHalf!=InfinityRep&&(self.ExtraRep>=0||self.ExtraRep==PiRep))
    #else
                if(self.DecimalHalf!=InfinityRep)
    #endif
                {
                    self.ConvertToNumRep();
                }

    #if defined(AltNum_EnableImaginaryNum)
                if(Value.DecimalHalf!=InfinityRep&&(Value.ExtraRep>=0||Value.ExtraRep==PiRep))
    #else
                if(Value.DecimalHalf!=InfinityRep)
    #endif
                {
                    Value.ConvertToNumRep();
                }
#else
    #if defined(AltNum_EnableImaginaryNum)
                if(self.ExtraRep>=0||self.ExtraRep==PiRep)
                {
                    self.ConvertToNumRep();
                }
    #else
                self.ConvertToNumRep();
    #endif

    #if defined(AltNum_EnableImaginaryNum)
                if(Value.ExtraRep>=0||Value.ExtraRep==PiRep)
                {
                    Value.ConvertToNumRep();
                }
    #else
                Value.ConvertToNumRep();
    #endif
#endif
#if defined(AltNum_EnableImaginaryNum)
            return (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf && self.ExtraRep == Value.ExtraRep);
#else
            return (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf);
#endif
        }

        /// <summary>
        /// Not equal to Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator!=(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
    #if defined(AltNum_EnableImaginaryNum)
                if(self.DecimalHalf!=InfinityRep&&(self.ExtraRep>=0||self.ExtraRep==PiRep))
    #else
                if(self.DecimalHalf!=InfinityRep)
    #endif
                {
                    self.ConvertToNumRep();
                }

    #if defined(AltNum_EnableImaginaryNum)
                if(Value.DecimalHalf!=InfinityRep&&(Value.ExtraRep>=0||Value.ExtraRep==PiRep))
    #else
                if(Value.DecimalHalf!=InfinityRep)
    #endif
                {
                    Value.ConvertToNumRep();
                }
#else
    #if defined(AltNum_EnableImaginaryNum)
                if(self.ExtraRep>=0||self.ExtraRep==PiRep)
                {
                    self.ConvertToNumRep();
                }
    #else
                self.ConvertToNumRep();
    #endif

    #if defined(AltNum_EnableImaginaryNum)
                if(Value.ExtraRep>=0||Value.ExtraRep==PiRep)
                {
                    Value.ConvertToNumRep();
                }
    #else
                Value.ConvertToNumRep();
    #endif
#endif
#if defined(AltNum_EnableImaginaryNum)
            return (self.IntValue != Value.IntValue || self.DecimalHalf != Value.DecimalHalf || self.ExtraRep != Value.ExtraRep);
#else
            return (self.IntValue != Value.IntValue || self.DecimalHalf != Value.DecimalHalf);
#endif
        }

        /// <summary>
        /// Lesser than Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(Value.ExtraRep==InfinityRep)
                {
                    return self.IntValue<Value.IntValue;
                }
            }
            else if(Value.ExtraRep==InfinityRep)
            {
            
            }
#endif
            self.ConvertToNumRep(); Value.ConvertToNumRep();
            if (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf) { return false; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValueIsNegative = Value.IntValue < 0;
                if (ValueIsNegative && SelfIsNegative == false) { return false; }// 5 > -5
                else if (ValueIsNegative == false && SelfIsNegative) { return true; }// -5 <5
                else
                {//Both are either positive or negative
                    if (Value.DecimalHalf == 0)
                    {
                        if (self.DecimalHalf == 0)
                            return self.IntValue < Value.IntValue;
                        else
                        {
                            if (self.IntValue == NegativeRep)
                            {//-0.5<0
                                if (Value.IntValue >= 0)
                                    return true;
                            }
                            else if (self.IntValue < Value.IntValue) { return true; }//5.5 < 6
                            else if (self.IntValue == Value.IntValue) { return self.IntValue < 0 ? true : false; }//-5.5<-5 vs 5.5 > 5
                        }
                    }
                    else if (self.DecimalHalf == 0)
                    {
                        if (Value.IntValue == NegativeRep)
                        {
                            if (self.IntValue <= -1)
                                return true;
                        }
                        else if (self.IntValue < Value.IntValue)
                            return true;// 5 < 6.5
                        else if (Value.IntValue == self.IntValue)
                            return Value.IntValue < 0 ? false : true;//5 < 5.5 vs -5 > -5.5
                    }
                    //Assuming both are non-whole numbers if reach here
                    if (self.IntValue == NegativeRep)
                        self.IntValue = 0;
                    if (Value.IntValue == NegativeRep)
                        Value.IntValue = 0;
                    if (SelfIsNegative)
                    {//Larger number = farther down into negative
                        if (self.IntValue > Value.IntValue)
                            return true;
                        else if (self.IntValue == Value.IntValue)
                            return self.DecimalHalf > Value.DecimalHalf;
                    }
                    else
                    {
                        if (self.IntValue < Value.IntValue)
                            return true;
                        else if (self.IntValue == Value.IntValue)
                            return self.DecimalHalf < Value.DecimalHalf;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Lesser than or Equal to Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator<=(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(self.IntValue==Value.IntValue||Value.IntValue==1)
                    return true;
                else
                    return false;
            }
            else if(Value.ExtraRep==InfinityRep)
            {
            
            }
#endif
            self.ConvertToNumRep(); Value.ConvertToNumRep();
            if (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf) { return true; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValueIsNegative = Value.IntValue < 0;
                if (ValueIsNegative && SelfIsNegative == false) { return false; }//5>=-5
                else if (ValueIsNegative == false && SelfIsNegative) { return true; }//-5<=5
                else
                {
                    if (Value.DecimalHalf == 0)
                    {
                        if (self.DecimalHalf == 0)
                            return self.IntValue <= Value.IntValue;
                        else
                        {
                            if (self.IntValue == NegativeRep)
                            {//-0.5<0
                                if (Value >= 0)
                                    return true;
                            }
                            else if (self.IntValue < Value) { return true; }//5.5<=6
                            else if (self.IntValue == Value) { return self.IntValue < 0 ? true : false; }
                        }
                    }
                    else if (self.DecimalHalf == 0)
                    {
                        if (Value.IntValue == NegativeRep && self.IntValue <= 1)
                        {//-1<-0.5
                            if (self.IntValue <= -1)
                                return true;
                        }
                        else if (self.IntValue < Value.IntValue)
                            return true;
                        else if (Value.IntValue == self.IntValue)
                            return Value.IntValue < 0 ? false : true;//5 <= 5.5 vs -5 >= -5.5
                    }
                    //Assuming both are non-whole numbers if reach here
                    if (self.IntValue == NegativeRep)
                        self.IntValue = 0;
                    if (Value.IntValue == NegativeRep)
                        Value.IntValue = 0;
                    if (SelfIsNegative)//Both are either positive or negative
                    {//Larger number = farther down into negative
                        if (self.IntValue > Value.IntValue)
                            return true;
                        else if (self.IntValue == Value.IntValue)
                            return self.DecimalHalf > Value.DecimalHalf;
                    }
                    else
                    {
                        if (self.IntValue < Value.IntValue)
                            return true;
                        else if (self.IntValue == Value.IntValue)
                            return self.DecimalHalf < Value.DecimalHalf;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Greater than Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>bool</returns>
        friend bool operator>(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(Value.ExtraRep==InfinityRep)
                {
                    if(self.IntValue==Value.IntValue)
                        return false;
                    else if(Value.IntValue==1)
                        return false;
                    else
                        return true;
                }
            }
            else if(Value.ExtraRep==InfinityRep)
            {
            
            }
#endif
            self.ConvertToNumRep(); Value.ConvertToNumRep();
            if (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf) { return false; }
            else
            {

                bool SelfIsNegative = self.IntValue < 0;
                bool ValueIsNegative = Value.IntValue < 0;
                if (ValueIsNegative && SelfIsNegative == false) { return true; }//5 > -5
                else if (ValueIsNegative == false && SelfIsNegative) { return false; }//-5<5
                else if (Value.DecimalHalf == 0)
                {
                    if (self.DecimalHalf == 0)
                        return self.IntValue > Value.IntValue;
                    else
                    {
                        if (self.IntValue == NegativeRep)
                        {//-0.5>-1
                            if (Value <= -1)
                                return true;
                        }
                        else if (self.IntValue > Value) { return true; }
                        else if (self.IntValue == Value) { return self.IntValue < 0 ? false : true; }
                    }
                }
                else if (self.DecimalHalf == 0)
                {
                    if (Value.IntValue == NegativeRep)
                    {
                        if (self.IntValue >= 0)
                            return true;
                    }
                    else if (self.IntValue > Value.IntValue)
                        return true;
                    else if (Value.IntValue == self.IntValue)
                        return Value.IntValue < 0 ? true : false;//5 < 5.5 vs -5 > -5.5
                }
                //Assuming both are non-whole numbers if reach here
                if (self.IntValue == NegativeRep)
                    self.IntValue = 0;
                if (Value.IntValue == NegativeRep)
                    Value.IntValue = 0;
                if (SelfIsNegative)//Both are either positive or negative
                {//Larger number = farther down into negative
                    if (self.IntValue < Value.IntValue)
                        return true;
                    else if (self.IntValue == Value.IntValue)
                        return self.DecimalHalf < Value.DecimalHalf;
                }
                else
                {
                    if (self.IntValue > Value.IntValue)
                        return true;
                    else if (self.IntValue == Value.IntValue)
                        return self.DecimalHalf > Value.DecimalHalf;
                }
            }
            return false;
        }

        /// <summary>
        /// Greater than or Equal to Operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value</param>
        /// <returns>bool</returns>
        friend bool operator>=(MediumDecVariant self, MediumDecVariant Value)
        {
#if defined(AltNum_EnableInfinityRep)
            if(self.ExtraRep==InfinityRep)
            {
                if(Value.ExtraRep==InfinityRep)
                {
                    if(self.IntValue==Value.IntValue||Value.IntValue==-1)
                        return true;
                    else
                        return false;
                }
                else
                {
                
                }
            }
            else if(Value.ExtraRep==InfinityRep)
            {
            
            }
#endif
            self.ConvertToNumRep(); Value.ConvertToNumRep();
            if (self.IntValue == Value.IntValue && self.DecimalHalf == Value.DecimalHalf) { return true; }
            else
            {
                bool SelfIsNegative = self.IntValue < 0;
                bool ValueIsNegative = Value.IntValue < 0;
                if (ValueIsNegative && SelfIsNegative == false) { return true; }
                else if (ValueIsNegative == false && SelfIsNegative) { return false; }
                else if (Value.DecimalHalf == 0)
                {
                    if (self.DecimalHalf == 0)
                        return self.IntValue >= Value.IntValue;
                    else
                    {
                        if (self.IntValue == NegativeRep)
                        {
                            if (Value <= -1)
                                return true;
                        }
                        else if (self.IntValue > Value)
                            return true;
                        else if (self.IntValue == Value)
                            return self.IntValue < 0 ? false : true;//-5.5<-5 vs 5.5>5
                    }
                }
                else if (self.DecimalHalf == 0)//return self.IntValue > Value;
                {
                    if (Value.IntValue == NegativeRep)
                    {//0>-0.5
                        if (self.IntValue >= 0)
                            return true;
                    }
                    else if (self.IntValue > Value.IntValue)
                        return true;
                    else if (Value.IntValue == self.IntValue)
                        return Value.IntValue < 0 ? true : false;//5 <= 5.5 vs -5 >= -5.5
                }
                //Assuming both are non-whole numbers if reach here
                if (self.IntValue == NegativeRep)
                    self.IntValue = 0;
                if (Value.IntValue == NegativeRep)
                    Value.IntValue = 0;
                if (SelfIsNegative)//Both are either positive or negative
                {//Larger number = farther down into negative
                    if (self.IntValue < Value.IntValue)//-5.5 >= -6.5
                        return true;
                    else if (self.IntValue == Value.IntValue)//-5.5 >= -5.6
                        return self.DecimalHalf < Value.DecimalHalf;
                }
                else
                {
                    if (self.IntValue > Value.IntValue)
                        return true;
                    else if (self.IntValue == Value.IntValue)
                        return self.DecimalHalf > Value.DecimalHalf;
                }
            }
            return false;
        }

    #pragma endregion Comparison Operators