
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="Value">The rightside rValue</param>
        /// <returns>AltDecBase&</returns>
        AltDecBase& DivOp(AltDecBase rValue)
        {
            //Warning:Modifies Negative value into positive number(Don't use with target Value that is important not to modify)
            if (IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (rValue.DecimalHalf == InfinityRep)
            {
                if (DecimalHalf == InfinityRep)//https://www.philforhumanity.com/Infinity_Divided_by_Infinity.html
    #if defined(AltNum_EnableNaN)
                    DecimalHalf = NaNRep;//Infinity divided by infinity equals undefined
    #else
                    throw "Result is undefined(Infinity by Infinity)";
    #endif
                else
    #if defined(AltNum_EnableApproaching)
                {
                    DecimalHalf = ApproachingBottomRep;
                    if(rValue.IntHalf<0)
                    {
                        if (IntHalf<0)
                            IntHalf = 0;
                        else
                            IntHalf = NegativeRep;
                    }
                    else
                    {
                        if (IntHalf<0)
                            IntHalf = NegativeRep;
                        else
                            IntHalf = 0;
                    }
                    ExtraRep = InitialExtraRep;
                }
    #else            
                    SetAsZero();
    #endif
                return *this;
            }
            if (rValue.IntHalf==0&&rValue.DecimalHalf==0)
            {
                if (IntHalf < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
            }
#else
            if (rValue.IsZero())
                throw "Target value can not be divided by zero unless infinity enabled";
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicUnsignedDivOp(rValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableIRep)
                case RepType::INum://Xi / Yi = (X(Sqrt(-1))/(Y(Sqrt(-1)) = X/Y
#endif
#if (defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)) || defined(AltNum_EnableENum) || defined(AltNum_EnableIRep)
                    ExtraRep = InitialExtraRep;
                    BasicUnsignedDivOp(rValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    BasicUnsignedDivOp(rValue);
                    if (rValue.ExtraRep<ExtraRep)
                    {
                        AltDecBase PiPowerDivisor = PiPowerNum(ExtraRep - rValue.ExtraRep);
                        ExtraRep = InitialExtraRep;
                        BasicUnsignedDivOp(PiPowerDivisor);
                    }
                    else
                    {
                        ExtraRep -= rValue.ExtraRep;
                    }
                    break;
#endif

#if defined(AltNum_EnableApproaching)
                case RepType::ApproachingBottom:
                {
                    int rInverted = rValue.IntHalf==NegativeRep?0:-rValue.IntHalf;
                    if (IntHalf == rValue.IntHalf)
                    {
                        IntHalf = 1;
                        DecimalHalf = 0;
                    }
                    else if (IntHalf == rInverted)
                    {
                        IntHalf = -1; DecimalHalf = 0;
                    }
                    else if (IntHalf == 0 && rValue.IntHalf == NegativeRep)
                    {
                        IntHalf = -1; DecimalHalf = 0;
                    }
                    else if (IntHalf == NegativeRep && rValue.IntHalf == 0)
                    {
                        IntHalf = -1; DecimalHalf = 0;
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
                case RepType::ApproachingTop:
                {
                    int rInverted = rValue.IntHalf==NegativeRep?0:-rValue.IntHalf;
                    if (IntHalf == rValue.IntHalf)
                    {
                        IntHalf = 1; DecimalHalf = 0;
                    }
                    else if (IntHalf == rInverted)
                    {
                        IntHalf = -1; DecimalHalf = 0;
                    }
                    else if (IntHalf == NegativeRep && rValue.IntHalf == 0)
                    {
                        IntHalf = -1; DecimalHalf = 0;

                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    if (ExtraRep == rValue.ExtraRep)
                    {// 0.249..9 / 0.249..9 = 1
                        int InvertedrValue = rValue.IntHalf==NegativeRep?0:-(int)rValue.IntHalf;
                        if (IntHalf == rValue.IntHalf)
                        {
                            IntHalf = 1; DecimalHalf = 0; ExtraRep = InitialExtraRep;
                        }
                        else if (IntHalf == InvertedrValue)
                        {
                            IntHalf = -1; DecimalHalf = 0; ExtraRep = InitialExtraRep;
                        }
                        else if (IntHalf == NegativeRep && rValue.IntHalf == 0)
                        {
                            IntHalf = -1; DecimalHalf = 0; ExtraRep = InitialExtraRep;
                        }
                        else
                        {
                            CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                case RepType::ApproachingMidRight:
                    if (ExtraRep == rValue.ExtraRep)
                    {//0.50..1 / 0.50..1 = 1
                        int InvertedrValue = rValue.IntHalf==NegativeRep?0:-(int)rValue.IntHalf;
                        if (IntHalf == rValue.IntHalf)
                        {
                            IntHalf = 1; DecimalHalf = 0; ExtraRep = InitialExtraRep;
                        }
                        else if (IntHalf == InvertedrValue)
                        {
                            IntHalf = -1; DecimalHalf = 0; ExtraRep = InitialExtraRep;
                        }
                        else
                        {
                            CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                        }
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
    #endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
                case RepType::NumByDiv://(AltDecBase(IntHalf,DecimalHalf))/ExtraRep
                {
                    //((AltDecBase(IntHalf,DecimalHalf))/ExtraRep) / (AltDecBase(rValue.IntHalf,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((AltDecBase(IntHalf,DecimalHalf))* rValue.ExtraRep/ExtraRep) /(AltDecBase(rValue.IntHalf,rValue.DecimalHalf)))
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntHalf;
                        if(ExtraRep == result / rValue.IntHalf)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntHalf);
                    }
                    else
                    {//CatchAllDivisionV2(rValue, LRep);
                        BasicUIntMultOp(rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
                //(Self.IntHalf/DecimalHalf)/(rValue.IntHalf/rValue.DecimalHalf) =
                //(IntHalf*rValue.DecimalHalf)/(DecimalHalf*rValue.IntHalf)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntHalf/DecimalHalf*Pi Representation
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntHalf/DecimalHalf*e Representation
    #endif
    #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntHalf/DecimalHalf*i Representation
    #endif
    #if defined(AltNum_UsingAltFractional)
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int NumRes = rValue.DecimalHalf * IntHalf;
                    int DenomRes = DecimalHalf * rValue.IntHalf;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntHalf = divRes;
                        ExtraRep = InitialExtraRep;
                    }
                    else
                    {
                        IntHalf = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
    #endif
#endif
#if defined(AltNum_EnableDecimaledAlternativeFractionals)
    #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(Value/(ExtraRep*-1))*e Representation
    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
    #endif
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntHalf;
                        if(ExtraRep == result / rValue.IntHalf)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntHalf);
                    }
                    else
                    {
                        BasicUIntMultOp(-rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
#endif

                //Turn MixedFrac into fractional and then apply			
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntHalf +- (-DecimalHalf/ExtraRep)
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int rvDivisor = -rValue.ExtraRep;
                    //=LeftSideNum*rValue.ExtraRep / RightSideNum;
                    AltDecBase LeftSideNum;
                    if (IntHalf == NegativeRep)
                        LeftSideNum = AltDecBase(DecimalHalf);
                    else if (IntHalf < 0)
                        LeftSideNum = AltDecBase(IntHalf * ExtraRep + DecimalHalf);
                    else if (IntHalf == 0)
                        LeftSideNum = AltDecBase(-DecimalHalf);
                    else
                        LeftSideNum = AltDecBase(IntHalf * ExtraRep - DecimalHalf);
                    LeftSideNum.UIntDivOp(rValue.ExtraRep);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else
                    {
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntHalf = -LeftSideNum.IntHalf;
                            ExtraRep *= rValue.IntHalf==NegativeRep ? -rValue.DecimalHalf : -rValue.IntHalf * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                        else
                        {
                            IntHalf = LeftSideNum.IntHalf;
                            ExtraRep *= rValue.IntHalf==0 ? -rValue.DecimalHalf : rValue.IntHalf * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                    }
                }
                break;//Result as NumByDiv
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntHalf +- (-DecimalHalf/-ExtraRep))*Pi
    #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
    #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
    #endif
    #if defined(AltNum_MixedAltEnabled)
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    //=LeftSideNum*-rValue.ExtraRep / RightSideNum;
                    int rvDivisor = -rValue.ExtraRep;
                    AltDecBase LeftSideNum;
                    if (IntHalf == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntHalf < 0)
                        LeftSideNum = IntHalf * -ExtraRep + DecimalHalf;
                    else if (IntHalf == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntHalf * -ExtraRep + -DecimalHalf;
                    LeftSideNum.UIntDivOp(rvDivisor);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else//Result as NumByDiv
                    {   //Need to have positive ExtraRep value
                        //int RightSideNum = rValue.IntHalf==0?-DecimalHalf:(rValue.IntHalf*-rValue.ExtraRep)-rValue.DecimalHalf;
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntHalf = -LeftSideNum.IntHalf;
                            ExtraRep *= rValue.IntHalf == NegativeRep ? DecimalHalf : (-rValue.IntHalf * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                        else
                        {
                            IntHalf = LeftSideNum.IntHalf;
                            ExtraRep *= rValue.IntHalf == 0 ? DecimalHalf : (rValue.IntHalf * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                    }
                }
                break;
    #endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rValue.DecimalHalf == InfinityRep)
                    {
                        if (DecimalHalf == InfinityRep)
                            SetVal(One);
                        else
                            #if defined(AltNum_EnableNaN)
                            DecimalHalf = UndefinedRep;
                        #else
                            throw "Undefined result(results in undefined expression)";
                        #endif
                    }
                    else
                        BasicUnsignedDivOp(rValue);
                }
                break;
    #if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
    #endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw AltDecBase::RepTypeAsString(LRep) + " RepType division not supported yet";
                    break;
                }
            }
            else
                RepToRepDivOp(LRep, RRep, *this, rValue);
            return *this;
        }
		
        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>AltDecBase&</returns>
        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>AltDecBase&</returns>
        AltDecBase& MultOp(AltDecBase rValue)
        {
            if (rValue.IntHalf==0&&rValue.DecimalHalf==0) { SetAsZero(); return *this; }
            if (IsZero() || rValue == AltDecBase::One)
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
            {
                if(rValue<0)
                    IntHalf *= -1;
                return *this;
            }
#endif
            RepType LRep = GetRepType();
            RepType RRep = rValue.GetRepType();
            if (LRep == RRep)
            {
                switch (LRep)
                {
                case RepType::NormalType:
                    BasicMultOp(rValue);
                    break;
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
                    BasicMultOp(rValue);
#if defined(AltNum_EnablePiPowers)
                    ExtraRep = -2;
#else
                    BasicMultOp(PiNum);
#endif
                    break;
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
                    BasicMultOp(rValue);
                    BasicMultOp(ENum);
                    break;
#endif
#if defined(AltNum_EnableIRep)
                case RepType::INum://Xi * Yi = -XY
                    ExtraRep = InitialExtraRep;
                    BasicMultOp(-rValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    ExtraRep += rValue.ExtraRep;
                    BasicMultOp(rValue);
                    break;
#endif

#if defined(AltNum_EnableApproaching)
                case RepType::ApproachingBottom:
                {
                    if (IntHalf == NegativeRep)
                    {
                        if(rValue<0)
                            IntHalf = 0;
                    }
                    else if (IntHalf == 0)
                    {
                        if(rValue<0)
                            IntHalf = NegativeRep;
                    }
                    else if (IntHalf < 0)
                    {
                        if(rValue<0)
                        {
                            if (rValue.IntHalf == NegativeRep)//-1.0..1 * -0.0..1
                                IntHalf = 0;
                            else//-1.0..1 * -2.0..1
                                IntHalf *= rValue.IntHalf;
                        }
                        else
                        {
                            if (rValue.IntHalf == 0)//-1.0..1 * 0.0..1
                                IntHalf = NegativeRep;
                            else//-1.0..1 * 2.0..1
                                IntHalf *= rValue.IntHalf;
                        }
                    }
                    else
                    {
                        if(rValue<0)
                        {
                            if (rValue.IntHalf == NegativeRep)//1.0..1 * -0.0..1
                                IntHalf = NegativeRep;
                            else//1.0..1 * 2.0..1
                                IntHalf *= rValue.IntHalf;
                        }
                        else
                        {
                            if (rValue.IntHalf == 0)//1.0..1 * 0.0..1
                                IntHalf = 0;
                            else//1.0..1 * 2.0..1
                                IntHalf *= rValue.IntHalf;
                        }
                    }
                    return *this;
                }
                break;
                case RepType::ApproachingTop://Just going to convert into normal numbers for now
                {
                    CatchAllMultiplicationV2(rValue, LRep);
                }    
                break;

    #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                {
                    if(rValue.IntHalf==NegativeRep)
                    {
                        if (IntHalf == 0
                        {
                            IntHalf = NegativeRep;
                            ExtraRep *= rValue.ExtraRep;
                        )
                        else if(IntHalf == NegativeRep)
                        {
                            IntHalf = 0;
                            ExtraRep *= rValue.ExtraRep;
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    if (rValue.IntHalf == 0)
                    {
                        //-0.49..9 * 0.49..9 =  ~-0.249..9 (IntHalf:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                        //0.49..9 * 0.49..9(IntHalf:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:2)
                        // =  ~0.249..9 (IntHalf:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                        // 0.249..9 * 0.249..9 = ~0.06249..9(IntHalf:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:16)
                        if (IntHalf == 0 || IntHalf == NegativeRep)
                            ExtraRep *= rValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if(rValue<0)
                    {
                        bool IsNegative = IntHalf<0;
                        if (IsNegative)
                            IntHalf = IntHalf == NegativeRep:0? -IntHalf;
                        IsNegative = !IsNegative;
                        int InvertedrValue = -(int)rValue.IntHalf;
                        int XZ = IntHalf * InvertedrValue;
                        AltDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntHalf;
                        AltDecBase YZ = SetAsApproachingMid(0, ExtraRep) * InvertedrValue;
                        AltDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntHalf = XV.IntHalf == 0 ? NegativeRep : -XV.IntHalf;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntHalf<0;
                        if (IsNegative)
                            IntHalf = IntHalf == NegativeRep:0 ? -IntHalf;
                        int XZ = IntHalf * rValue.IntHalf;
                        AltDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntHalf;
                        AltDecBase YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntHalf;
                        AltDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntHalf = XV.IntHalf == 0 ? NegativeRep : -XV.IntHalf;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                    break;
                }
                case RepType::ApproachingMidRight:
                {
                    if(rValue.IntHalf==NegativeRep)
                    {
                        if (IntHalf == 0)
                        {
                            IntHalf = NegativeRep;
                            ExtraRep *= rValue.ExtraRep;
                        )
                        else if(IntHalf == NegativeRep)
                        {
                            IntHalf = 0;
                            ExtraRep *= rValue.ExtraRep;
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if (rValue.IntHalf == 0)
                    {
                        if (IntHalf == 0 || IntHalf == NegativeRep)
                            ExtraRep *= rValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if(rValue.IntHalf<0)
                    {
                        bool IsNegative = IntHalf<0;
                        if (IsNegative)
                            IntHalf = IntHalf == NegativeRep:0 ? -IntHalf;
                        IsNegative = !IsNegative;
                        int InvertedrValue = -(int)rValue.IntHalf;
                        int XZ = IntHalf * InvertedrValue;
                        AltDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntHalf;
                        AltDecBase YZ = SetAsApproachingMid(0, ExtraRep) * InvertedrValue;
                        AltDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntHalf = XV.IntHalf == 0 ? NegativeRep : -XV.IntHalf;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntHalf<0;
                        if (IsNegative)
                            IntHalf = IntHalf == NegativeRep:0 ? -IntHalf;
                        int XZ = IntHalf * rValue.IntHalf;
                        AltDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntHalf;
                        AltDecBase YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntHalf;
                        AltDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntHalf = XV.IntHalf == 0 ? NegativeRep : -XV.IntHalf;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                }
                break;
    #endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
                case RepType::NumByDiv://(AltDecBase(IntHalf,DecimalHalf))/ExtraRep
                {
                    //((AltDecBase(IntHalf,DecimalHalf))/ExtraRep) * (AltDecBase(rValue.IntHalf,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((AltDecBase(IntHalf,DecimalHalf))*AltDecBase(rValue.IntHalf,rValue.DecimalHalf)))/(ExtraRep*rValue.ExtraRep)
                    BasicMultOp(rValue);
                    ExtraRep *= rValue.ExtraRep;
                }
                break;

                //(IntHalf*rValue.IntHalf)*Pi^2/(DecimalHalf*rValue.DecimalHalf)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntHalf/DecimalHalf*Pi Representation
                {//Become either PiNum, NumByDiv, or PiPower
        #if defined(AltNum_EnablePiPowers)//Becoming PiPower
                    IntHalf *= rValue.IntHalf;
                    int divisor = DecimalHalf*rValue.DecimalHalf;
                    DecimalHalf = 0;
                    BasicUIntDiv(divisor);
                    ExtraRep = -2;//Pi^2
        #else//Becoming PiNum
                    IntHalf *= rValue.IntHalf;
                    DecimalHalf *= rValue.DecimalHalf;
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicMultOp(PiNum);
                    BasicUIntDivOp(divisor);
                    ExtraRep = PiRep;
        #endif         
                }
                break;
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntHalf/DecimalHalf*e Representation
                {
                    IntHalf *= rValue.IntHalf;
                    DecimalHalf *= rValue.DecimalHalf;
                    int divisor = DecimalHalf;
                    DecimalHalf = 0;
                    BasicMultOp(ENum);
                    BasicUIntDivOp(divisor);
                    ExtraRep = ERep;
                }
                break;
    #endif

    #if defined(AltNum_EnableIRep)
                case RepType::IFractional://  IntHalf/DecimalHalf*i Representation
                {
                    //(IntHalf/DecimalHalf)i*(rValue.IntHalf/rValue.DecimalHalf)i
                    //==-1*(IntHalf/DecimalHalf)*(rValue.IntHalf/rValue.DecimalHalf)
                    int NumRes = -IntHalf * rValue.IntHalf;
                    int DenomRes = DecimalHalf * rValue.DecimalHalf;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntHalf = divRes;
                        ExtraRep = InitialExtraRep;
                    }
                    else
                    {
                        IntHalf = NumRes;
                        ExtraRep = DenomRes;
                    }
                }
                break;
    #endif

    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
        #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/-ExtraRep)*Pi Representation
                    //(rValue/(-ExtraRep))*Pi * (rValue.rValue/(-rValue.ExtraRep))*Pi
        #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/-ExtraRep)*e Representation
        #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/-ExtraRep)*i Representation
                    //(rValue/(-ExtraRep))*i * (rValue.rValue/(-rValue.ExtraRep))*i
        #endif
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    ExtraRep *= -rValue.ExtraRep;
                    BasicUnsignedMultOp(rValue);
            #if defined(AltNum_EnableDecimaledPiFractionals)
                #ifdef AltNum_EnablePiPowers//Convert to PiPower representation
                    BasicUnsignedDivOp(-ExtraRep);
                    ExtraRep = -2;
                #else
                    BasicUnsignedMultOp(PiNum);
                #endif
            #elif defined(AltNum_EnableDecimaledEFractionals)
                    BasicUnsignedMultOp(ENum);
            #else
                    SwapNegativeStatus();
            #endif
                }
                break;
    #endif
#endif

                    //Turn MixedFrac into fractional and then apply
#if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntHalf +- (-DecimalHalf/ExtraRep)
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    //IntHalf +- (-DecimalHalf/ExtraRep) = 
                    int LeftSideNum;
                    if (IntHalf == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntHalf < 0)
                        LeftSideNum = IntHalf * ExtraRep + DecimalHalf;
                    else if (IntHalf == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntHalf * ExtraRep - DecimalHalf;
                    int RightSideNum = rValue.IntHalf == 0 ? -rValue.DecimalHalf : rValue.IntHalf * rValue.ExtraRep - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntHalf = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntHalf +- (-DecimalHalf/-ExtraRep))*Pi
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntHalf == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntHalf < 0)
                        LeftSideNum = IntHalf * -ExtraRep + DecimalHalf;
                    else if (IntHalf == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntHalf * -ExtraRep + -DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntHalf == 0 ? -DecimalHalf : (rValue.IntHalf * -rValue.ExtraRep) - rValue.DecimalHalf;

                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes PiNumByDiv
                    //And then multiply by Pi
                    IntHalf = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #elif defined(AltNum_EnablePiPowers)
                    //Or convert to PiPower (of 2)
                    IntHalf = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #else
                    //Or convert PiNum and multiply by Pi
                    IntHalf = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicMultOp(PiNum);
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #endif
                }
                break;

#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntHalf == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntHalf < 0)
                        LeftSideNum = IntHalf * -ExtraRep + DecimalHalf;
                    else if (IntHalf == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntHalf * -ExtraRep + -DecimalHalf;
                    int RightSideNum = rValue.IntHalf == 0 ? -DecimalHalf : (rValue.IntHalf * -rValue.ExtraRep) - rValue.DecimalHalf;
                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes ENumByDiv
                    //And then multiply by e
                    IntHalf = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #else
                    //Or convert ENum and multiply by e
                    IntHalf = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicMultOp(PiNum);
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #endif
                }
                break;
#elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntHalf == NegativeRep)
                        LeftSideNum = -DecimalHalf;
                    else if (IntHalf < 0)
                        LeftSideNum = IntHalf * ExtraRep - DecimalHalf;
                    else if (IntHalf == 0)
                        LeftSideNum = DecimalHalf;
                    else
                        LeftSideNum = IntHalf * ExtraRep + DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntHalf == 0 ? -DecimalHalf : (rValue.IntHalf * -rValue.ExtraRep) - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntHalf = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
#endif
#endif
#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                {
                    if (rValue.DecimalHalf == InfinityRep)
                        DecimalHalf = InfinityRep;
                    else
                        BasicMultOp(rValue);
                }
                break;
#if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    throw "Uncertain how to perform operation with unbalanced ranged";
                    break;
#endif
#endif
#if defined(AltNum_EnableNaN)
                case RepType::Undefined:
                case RepType::NaN:
                    throw "Can't perform operations with NaN or Undefined number";
                    break;
#endif
                default:
                    throw AltDecBase::RepTypeAsString(LRep) + " RepType multiplication not supported yet";
                    break;
                }
            }
            else
                RepToRepMultOp(LRep, RRep, *this, rValue);
            return *this;
        }
		
