		/// <summary>
        /// Basic Multiplication Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        bool BasicMultOp(MediumDecVariant& Value)
		{
            if (Value.IsNegative())
            {
                Value.SwapNegativeStatus();
                SwapNegativeStatus();
            }
            if (DecimalHalf == 0)
            {
                if (IntValue == 1) { self = Value; return self; }
                else if (Value.DecimalHalf == 0)
                {
                    IntValue.Value *= Value.IntValue; return self;
                }
                else
                {
                    Value *= IntValue;
                    self = Value;
                }
            }
            else if (Value.DecimalHalf == 0)//Y is integer
            {
                __int64 SRep;
                if(IntValue.IsZero())
                {
                    //0.Y * 0.V == .Y * .V
                    SRep = DecimalHalf * Value.DecimalHalf;
                    SRep /= MediumDecVariant::DecimalOverflowX;
                }
                else
                {
                    SRep = MediumDecVariant::DecimalOverflowX * IntValue.Value + DecimalHalf;
                    SRep *= Value.IntValue.Value;
                }
                if (SRep >= MediumDecVariant::DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                    SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                    IntValue = (signed int)OverflowVal;
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    IntValue = 0;
                    DecimalHalf = (signed int)SRep;
                }
            }
            else if (IntValue == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= MediumDecVariant::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * Value.IntValue;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntValue = OverflowVal;
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                    }
                }
            }
            else if (IntValue == MediumDecVariant::NegativeRep)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= Value.DecimalHalf;
                SRep /= MediumDecVariant::DecimalOverflowX;
                if (Value.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * Value.IntValue;
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntValue = -OverflowVal;
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                    }
                }
            }
            else
            {
                bool SelfIsNegative = IsNegative();
                if (SelfIsNegative)
                {
                    IntValue.Value *= -1;
                }
                if (Value.IntValue.Value == 0)
                {
                    __int64 SRep;
                    if(IntValue.Value==0)
                    {
                        //0.Y * 0.V == .Y * .V
                        SRep = DecimalHalf * Value.DecimalHalf;
                        SRep /= MediumDecVariant::DecimalOverflowX;
                    }
                    else
                    {
                        SRep = MediumDecVariant::DecimalOverflowX * IntValue.Value + DecimalHalf;
                        SRep *= Value.DecimalHalf;
                        SRep /= MediumDecVariant::DecimalOverflowX;
                    }
                    if (SRep >= MediumDecVariant::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / MediumDecVariant::DecimalOverflowX;
                        SRep -= OverflowVal * MediumDecVariant::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        IntValue = SelfIsNegative ? MediumDecVariant::NegativeRep : 0;
                        DecimalHalf = (signed int)SRep;
                    }
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : MediumDecVariant::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= Value.IntValue;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)IntValue * Value.DecimalHalf;//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)Value.DecimalHalf;
                    Temp04 /= MediumDecVariant::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / MediumDecVariant::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)MediumDecVariant::DecimalOverflow;
                    if (IntHalf == 0) { IntValue = (signed int)SelfIsNegative ? MediumDecVariant::NegativeRep : 0; }
                    else { IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf; }
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
		}