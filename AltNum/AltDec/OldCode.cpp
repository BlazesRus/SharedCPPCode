
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
                    if(rValue.IntValue<0)
                    {
                        if (IntValue<0)
                            IntValue = 0;
                        else
                            IntValue = NegativeRep;
                    }
                    else
                    {
                        if (IntValue<0)
                            IntValue = NegativeRep;
                        else
                            IntValue = 0;
                    }
                    ExtraRep = 0;
                }
    #else            
                    SetAsZero();
    #endif
                return *this;
            }
            if (rValue.IntValue==0&&rValue.DecimalHalf==0)
            {
                if (IntValue < 0)
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
                    ExtraRep = 0;
                    BasicUnsignedDivOp(rValue);
                    break;
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    BasicUnsignedDivOp(rValue);
                    if (rValue.ExtraRep<ExtraRep)
                    {
                        AltDecBase PiPowerDivisor = PiPowerNum(ExtraRep - rValue.ExtraRep);
                        ExtraRep = 0;
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
                    int rInverted = rValue.IntValue==NegativeRep?0:-rValue.IntValue;
                    if (IntValue == rValue.IntValue)
                    {
                        IntValue = 1;
                        DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == 0 && rValue.IntValue == NegativeRep)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else
                    {
                        CatchAllDivisionV2(rValue, LRep);//Just convert into normal numbers for now
                    }
                    break;
                }
                case RepType::ApproachingTop:
                {
                    int rInverted = rValue.IntValue==NegativeRep?0:-rValue.IntValue;
                    if (IntValue == rValue.IntValue)
                    {
                        IntValue = 1; DecimalHalf = 0;
                    }
                    else if (IntValue == rInverted)
                    {
                        IntValue = -1; DecimalHalf = 0;
                    }
                    else if (IntValue == NegativeRep && rValue.IntValue == 0)
                    {
                        IntValue = -1; DecimalHalf = 0;

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
                        int InvertedrValue = rValue.IntValue==NegativeRep?0:-(int)rValue.IntValue;
                        if (IntValue == rValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == NegativeRep && rValue.IntValue == 0)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
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
                        int InvertedrValue = rValue.IntValue==NegativeRep?0:-(int)rValue.IntValue;
                        if (IntValue == rValue.IntValue)
                        {
                            IntValue = 1; DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == InvertedrValue)
                        {
                            IntValue = -1; DecimalHalf = 0; ExtraRep = 0;
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
                case RepType::NumByDiv://(AltDecBase(IntValue,DecimalHalf))/ExtraRep
                {
                    //((AltDecBase(IntValue,DecimalHalf))/ExtraRep) / (AltDecBase(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((AltDecBase(IntValue,DecimalHalf))* rValue.ExtraRep/ExtraRep) /(AltDecBase(rValue.IntValue,rValue.DecimalHalf)))
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    if (rValue.DecimalHalf == 0)
                    {
                        BasicUIntMultOp(rValue.ExtraRep);
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
                    }
                    else
                    {//CatchAllDivisionV2(rValue, LRep);
                        BasicUIntMultOp(rValue.ExtraRep);
                        BasicUnsignedDivOp(rValue);
                    }
                }
                break;
                //(Self.IntValue/DecimalHalf)/(rValue.IntValue/rValue.DecimalHalf) =
                //(IntValue*rValue.DecimalHalf)/(DecimalHalf*rValue.IntValue)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
    #endif
    #if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
    #endif
    #if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
    #endif
    #if defined(AltNum_UsingAltFractional)
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int NumRes = rValue.DecimalHalf * IntValue;
                    int DenomRes = DecimalHalf * rValue.IntValue;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
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
                        int result = ExtraRep * rValue.IntValue;
                        if(ExtraRep == result / rValue.IntValue)//checking for overflow
                        {
                            ExtraRep = result;
                        }
                        else
                            BasicUnsignedDivOp(rValue.IntValue);
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
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    if (rValue < 0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int rvDivisor = -rValue.ExtraRep;
                    //=LeftSideNum*rValue.ExtraRep / RightSideNum;
                    AltDecBase LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = AltDecBase(DecimalHalf);
                    else if (IntValue < 0)
                        LeftSideNum = AltDecBase(IntValue * ExtraRep + DecimalHalf);
                    else if (IntValue == 0)
                        LeftSideNum = AltDecBase(-DecimalHalf);
                    else
                        LeftSideNum = AltDecBase(IntValue * ExtraRep - DecimalHalf);
                    LeftSideNum.UIntDivOp(rValue.ExtraRep);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else
                    {
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==NegativeRep ? -rValue.DecimalHalf : -rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue==0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                        }
                    }
                }
                break;//Result as NumByDiv
    #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
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
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    LeftSideNum.UIntDivOp(rvDivisor);
                    if (LeftSideNum.IsZero())
                        SetAsZero();
                    else//Result as NumByDiv
                    {   //Need to have positive ExtraRep value
                        //int RightSideNum = rValue.IntValue==0?-DecimalHalf:(rValue.IntValue*-rValue.ExtraRep)-rValue.DecimalHalf;
                        DecimalHalf = LeftSideNum.DecimalHalf;
                        if(rValue<0)
                        {
                            IntValue = -LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == NegativeRep ? DecimalHalf : (-rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
                        }
                        else
                        {
                            IntValue = LeftSideNum.IntValue;
                            ExtraRep *= rValue.IntValue == 0 ? DecimalHalf : (rValue.IntValue * rValue.ExtraRep) + rValue.DecimalHalf;
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
            if (rValue.IntValue==0&&rValue.DecimalHalf==0) { SetAsZero(); return *this; }
            if (IsZero() || rValue == AltDecBase::One)
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
            {
                if(rValue<0)
                    IntValue *= -1;
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
                    ExtraRep = 0;
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
                    if (IntValue == NegativeRep)
                    {
                        if(rValue<0)
                            IntValue = 0;
                    }
                    else if (IntValue == 0)
                    {
                        if(rValue<0)
                            IntValue = NegativeRep;
                    }
                    else if (IntValue < 0)
                    {
                        if(rValue<0)
                        {
                            if (rValue.IntValue == NegativeRep)//-1.0..1 * -0.0..1
                                IntValue = 0;
                            else//-1.0..1 * -2.0..1
                                IntValue *= rValue.IntValue;
                        }
                        else
                        {
                            if (rValue.IntValue == 0)//-1.0..1 * 0.0..1
                                IntValue = NegativeRep;
                            else//-1.0..1 * 2.0..1
                                IntValue *= rValue.IntValue;
                        }
                    }
                    else
                    {
                        if(rValue<0)
                        {
                            if (rValue.IntValue == NegativeRep)//1.0..1 * -0.0..1
                                IntValue = NegativeRep;
                            else//1.0..1 * 2.0..1
                                IntValue *= rValue.IntValue;
                        }
                        else
                        {
                            if (rValue.IntValue == 0)//1.0..1 * 0.0..1
                                IntValue = 0;
                            else//1.0..1 * 2.0..1
                                IntValue *= rValue.IntValue;
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
                    if(rValue.IntValue==NegativeRep)
                    {
                        if (IntValue == 0
                        {
                            IntValue = NegativeRep;
                            ExtraRep *= rValue.ExtraRep;
                        )
                        else if(IntValue == NegativeRep)
                        {
                            IntValue = 0;
                            ExtraRep *= rValue.ExtraRep;
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    if (rValue.IntValue == 0)
                    {
                        //-0.49..9 * 0.49..9 =  ~-0.249..9 (IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                        //0.49..9 * 0.49..9(IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:2)
                        // =  ~0.249..9 (IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:4)
                        // 0.249..9 * 0.249..9 = ~0.06249..9(IntValue:0 DecimalHalf:ApproachingrValue.Rep ExtraRep:16)
                        if (IntValue == 0 || IntValue == NegativeRep)
                            ExtraRep *= rValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if(rValue<0)
                    {
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0? -IntValue;
                        IsNegative = !IsNegative;
                        int InvertedrValue = -(int)rValue.IntValue;
                        int XZ = IntValue * InvertedrValue;
                        AltDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        AltDecBase YZ = SetAsApproachingMid(0, ExtraRep) * InvertedrValue;
                        AltDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        int XZ = IntValue * rValue.IntValue;
                        AltDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        AltDecBase YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntValue;
                        AltDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                    break;
                }
                case RepType::ApproachingMidRight:
                {
                    if(rValue.IntValue==NegativeRep)
                    {
                        if (IntValue == 0)
                        {
                            IntValue = NegativeRep;
                            ExtraRep *= rValue.ExtraRep;
                        )
                        else if(IntValue == NegativeRep)
                        {
                            IntValue = 0;
                            ExtraRep *= rValue.ExtraRep;
                        }
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if (rValue.IntValue == 0)
                    {
                        if (IntValue == 0 || IntValue == NegativeRep)
                            ExtraRep *= rValue.ExtraRep;
                        else
                        {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                            CatchAllMultiplicationV2(rValue, LRep);
                        }
                    }
                    else if(rValue.IntValue<0)
                    {
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        IsNegative = !IsNegative;
                        int InvertedrValue = -(int)rValue.IntValue;
                        int XZ = IntValue * InvertedrValue;
                        AltDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        AltDecBase YZ = SetAsApproachingMid(0, ExtraRep) * InvertedrValue;
                        AltDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    else
                    {//X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                        bool IsNegative = IntValue<0;
                        if (IsNegative)
                            IntValue = IntValue == NegativeRep:0 ? -IntValue;
                        int XZ = IntValue * rValue.IntValue;
                        AltDecBase XV = SetAsApproachingMid(0, rValue.ExtraRep) * IntValue;
                        AltDecBase YZ = SetAsApproachingMid(0, ExtraRep) * rValue.IntValue;
                        AltDecBase YV = SetAsApproachingMid(0, ExtraRep) * SetAsApproachingMid(0, rValue.ExtraRep);
                        XV += XZ;
                        XV += YZ + YV;
                        if (IsNegative)
                            IntValue = XV.IntValue == 0 ? NegativeRep : -XV.IntValue;
                        DecimalHalf = XV.DecimalHalf;
                        ExtraRep = XV.ExtraRep;
                    }
                    return *this;
                }
                break;
    #endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)//Unfinished code
                case RepType::NumByDiv://(AltDecBase(IntValue,DecimalHalf))/ExtraRep
                {
                    //((AltDecBase(IntValue,DecimalHalf))/ExtraRep) * (AltDecBase(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
                    //((AltDecBase(IntValue,DecimalHalf))*AltDecBase(rValue.IntValue,rValue.DecimalHalf)))/(ExtraRep*rValue.ExtraRep)
                    BasicMultOp(rValue);
                    ExtraRep *= rValue.ExtraRep;
                }
                break;

                //(IntValue*rValue.IntValue)*Pi^2/(DecimalHalf*rValue.DecimalHalf)
    #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                {//Become either PiNum, NumByDiv, or PiPower
        #if defined(AltNum_EnablePiPowers)//Becoming PiPower
                    IntValue *= rValue.IntValue;
                    int divisor = DecimalHalf*rValue.DecimalHalf;
                    DecimalHalf = 0;
                    BasicUIntDiv(divisor);
                    ExtraRep = -2;//Pi^2
        #else//Becoming PiNum
                    IntValue *= rValue.IntValue;
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
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                {
                    IntValue *= rValue.IntValue;
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
                case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                {
                    //(IntValue/DecimalHalf)i*(rValue.IntValue/rValue.DecimalHalf)i
                    //==-1*(IntValue/DecimalHalf)*(rValue.IntValue/rValue.DecimalHalf)
                    int NumRes = -IntValue * rValue.IntValue;
                    int DenomRes = DecimalHalf * rValue.DecimalHalf;
                    signed int divRes = NumRes / DenomRes;
                    signed int RemRes = NumRes - DenomRes * NumRes;
                    DecimalHalf = 0;
                    if (RemRes == 0)
                    {
                        IntValue = divRes;
                        ExtraRep = 0;
                    }
                    else
                    {
                        IntValue = NumRes;
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
                case RepType::MixedFrac://IntValue +- (-DecimalHalf/ExtraRep)
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    //IntValue +- (-DecimalHalf/ExtraRep) = 
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                    int RightSideNum = rValue.IntValue == 0 ? -rValue.DecimalHalf : rValue.IntValue * rValue.ExtraRep - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * -rValue.ExtraRep;
                }
                break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://(IntValue +- (-DecimalHalf/-ExtraRep))*Pi
                {
                    if(rValue<0)
                    {
                        rValue *= -1;
                        SwapNegativeStatus();
                    }
                    int LeftSideNum;
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;

                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes PiNumByDiv
                    //And then multiply by Pi
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #elif defined(AltNum_EnablePiPowers)
                    //Or convert to PiPower (of 2)
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = -2;
                    BasicUnsignedDivOp(ExtraRep * -rValue.ExtraRep);
                    #else
                    //Or convert PiNum and multiply by Pi
                    IntValue = LeftSideNum * RightSideNum;
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
                    if (IntValue == NegativeRep)
                        LeftSideNum = DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * -ExtraRep + DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = -DecimalHalf;
                    else
                        LeftSideNum = IntValue * -ExtraRep + -DecimalHalf;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                    #if defined(AltNum_EnableDecimaledEFractionals)
                    //Becomes ENumByDiv
                    //And then multiply by e
                    IntValue = LeftSideNum * RightSideNum;
                    DecimalHalf = 0;
                    ExtraRep = ExtraRep * rValue.ExtraRep;
                    #else
                    //Or convert ENum and multiply by e
                    IntValue = LeftSideNum * RightSideNum;
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
                    if (IntValue == NegativeRep)
                        LeftSideNum = -DecimalHalf;
                    else if (IntValue < 0)
                        LeftSideNum = IntValue * ExtraRep - DecimalHalf;
                    else if (IntValue == 0)
                        LeftSideNum = DecimalHalf;
                    else
                        LeftSideNum = IntValue * ExtraRep + DecimalHalf;
                    break;
                    int RightSideNum = rValue.IntValue == 0 ? -DecimalHalf : (rValue.IntValue * -rValue.ExtraRep) - rValue.DecimalHalf;
                    //Becomes NumByDiv now
                    IntValue = LeftSideNum * RightSideNum;
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
		
        /// <summary>
        /// Addition AltDecBase modifying operation with right side Integer rValue.
        /// (copy by parameter and pointer version)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=signed int>
        AltDecBase& IntAddOp(const IntType& rValue)
        {
            if (rValue == 0)
                return *this;
	#if defined(AltNum_EnableIRep)
            if(ExtraRep==IRep)
            {
                throw "Can't convert AltDecBase into complex number at moment";
				return *this;
            }
	#endif
	#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
	#endif
	#if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
			else
	#endif
			if(ExtraRep!=0)//Don't convert if mixed fraction
				ConvertToNormTypeV2();
			bool WasNegative = IntValue < 0;
			IntHalfAdditionOp(rValue);
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ (IntValue >= 0))//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
			{
				if(DecimalHalf<0)//Flip the fractional half of mixed fraction if flips to other side
				{
		#if defined(AltNum_EnableAlternativeMixedFrac)
					if(ExtraRep<0)// DecimalHalf:-2,ExtraRep:-3 becomes DecimalHalf:-1, ExtraRep:-3
						DecimalHalf = ExtraRep - DecimalHalf;
					else
		#endif			
						DecimalHalf = -(ExtraRep+DecimalHalf);// DecimalHalf:-2,ExtraRep:3 becomes DecimalHalf:-1, ExtraRep:3
				}
				else
					DecimalHalf = AltDecBase::DecimalOverflow - DecimalHalf;
			}
	#else
            if(WasNegative ^ (IntValue >= 0))
				DecimalHalf = AltDecBase::DecimalOverflow - DecimalHalf;
	#endif
            return *this;
        }
		
        /// <summary>
        /// Addition Operation  with right side AltDecBase
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>AltDecBase&</returns>
        AltDecBase& AddOp(AltDecBase rValue)
        {
            if (IsZero())
            {
                IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                ExtraRep = rValue.ExtraRep; return *this;
            }
            else if (rValue.IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            if (rValue.DecimalHalf == InfinityRep)
            {
                rValue.IntValue == 1 ? SetAsInfinity() : SetAsNegativeInfinity();
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
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableIRep)
                case RepType::INum:
#endif
                    BasicAddOp(rValue);
                    break;

#if defined(AltNum_EnableApproaching)
                case RepType::ApproachingBottom:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.0..1 + 0.0..1 = 0
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//-0.0..1 - 0.0..1 = -0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue < 0)//-0.0..1 - 1.0..1 = -1.0..1
                        {
                            IntValue = rValue.IntValue;
                        }
                        else//-0.0..1 + 5.0..1 = 5
                        {
                            DecimalHalf = 0;
                            IntValue = rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.0..1 + 0.0..1 = 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//0.0..1 - 0.0..1 = 0
                            SetAsZero();
                        else if (rValue.IntValue < 0)//0.0..1 - 1.0..1 = -1
                        {
                            DecimalHalf = 0;
                            IntValue = rValue.IntValue;
                        }
                        else//0.0..1 + 5.0..1 = 5.0..1
                        {
                            IntValue = rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.0..1 + 0.0..1  = -1
                            DecimalHalf = 0;
                        else if (rValue.IntValue == NegativeRep)//-1.0..1 - 0.0..1 = -1.0..1
                        {/*Do Nothing*/
                        }
                        else if (IntValue == -rValue.IntValue)//-1.01 + 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.0..1 - 2.0..1
                        {
                            IntValue += rValue.IntValue;
                        }
                        else//-1.0..1 + 2.0..1 = 1
                        {
                            DecimalHalf = 0;
                            IntValue += rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.0..1 + 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//1.0..1 - 0.0..1
                            DecimalHalf = 0;
                        else if (IntValue == -rValue.IntValue)//1.0..1 - 1.0..1
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.0..1  - 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue += rValue.IntValue;
                        }
                        else//1.0..1 + 1.0..1
                        {
                            IntValue += rValue.IntValue;
                        }
                    }
                    break;
                case RepType::ApproachingTop:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.9..9 + 0.9..9 = 0
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//-0.9..9 - 0.9..9 = -1.9..8
                            IntValue = -1;
                        else if (rValue.IntValue < 0)//-0.9..9 - 1.9..9 = -2.9..8
                        {
                            IntValue = rValue.IntValue - 1;
                        }
                        else//-0.9..9 + 5.9..9 = 5
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.9..9 + 0.9..9 = 1.9..8
                            IntValue = 1;
                        else if (rValue.IntValue == NegativeRep)//0.9..9 - 0.9..9 = 0
                            SetAsZero();
                        else if (rValue.IntValue < 0)//0.9..9 - 1.9..9 = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = rValue.IntValue;
                        }
                        else//0.9..9 + 5.9..9 = 6.9..8
                        {
                            IntValue = rValue.IntValue + 1;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.9..9 + 0.9..9  = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (rValue.IntValue == NegativeRep)//-1.9..9 - 0.9..9 = -2.9..9
                            IntValue += rValue.IntValue;
                        else if (IntValue == -rValue.IntValue)//-1.9..9 + 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.9..9 - 2.9..9
                        {
                            IntValue += rValue.IntValue;
                        }
                        else//-1.9..9 + 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue += rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.9..9 + 0.9..9 = 2.9..8
                        {
                            if (IntValue == -1)
                                IntValue = NegativeRep;
                            else
                                ++IntValue;
                        }
                        else if (rValue.IntValue == NegativeRep)//1.9..9 - 0.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == -rValue.IntValue)//1.9..9 - 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.9..9  - 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue += rValue.IntValue;
                        }
                        else//1.9..9 + 1.9..9 = 3.9..8
                        {
                            IntValue += rValue.IntValue + 1;
                        }
                    }
                    break;

#if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    CatchAllAdditionV2(rValue, RepType::ApproachingMidLeft);
                    break;
                case RepType::ApproachingMidRight:
                    CatchAllAdditionV2(rValue, RepType::ApproachingMidRight);
                    break;
#endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
#if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
#endif
#if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
#endif
#if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*e Representation
#endif
#if defined(AltNum_EnablePiFractional)||defined(AltNum_EnableEFractional)||defined(AltNum_EnableIFractional)
                    if (DecimalHalf == rValue.DecimalHalf)
                    {

                        *this += rValue.IntValue;
                    }
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicAddOp(rValue);
                    }
                    break;
#endif

#if defined(AltNum_EnableDecimaledAlternativeFractionals)
#if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/(ExtraRep*-1))*Pi Representation
#elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/(ExtraRep*-1))*e Representation
#elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/(ExtraRep*-1))*e Representation
#endif
                    if (ExtraRep == rValue.ExtraRep)
                        BasicAddOp(rValue);
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicAddOp(rValue);
                    }
                    break;
#endif
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    if (ExtraRep != rValue.ExtraRep)
                    {
                        ConvertPiPowerToPiRep(); rValue.ConvertPiPowerToPiRep();
                    }
                    BasicAddOp(rValue);
                    break;
#endif    

#if defined(AltNum_EnableMixedFractional)
                    //Ignoring the fact that mixed fraction could be improperly formatted for spend
                    //because should be corrected after every operation
                case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue += rValue.IntValue;
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= rValue.ExtraRep;
                        IntValue += rValue.IntValue * ExtraRep;
                        DecimalHalf += rValue.DecimalHalf * ExtraRep;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= rValue.ExtraRep;
                    }
                    break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
#elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
#endif
#if defined(AltNum_EnableAlternativeMixedFrac)
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue += rValue.IntValue;
                        DecimalHalf += rValue.DecimalHalf;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= -rValue.ExtraRep;
                        IntValue += rValue.IntValue * -ExtraRep;
                        DecimalHalf += rValue.DecimalHalf * -ExtraRep;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= -rValue.ExtraRep;
                    }
                    break;
#endif
#endif

#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                    BasicAddOp(rValue);
                    break;
#if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    if (IntValue == NegativeRep)
                    {
                        //Do nothing?
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {
                        IntValue = NegativeRep;
                    }
                    else
                        IntValue += rValue.IntValue;
                    if (DecimalHalf == InfinityRep)
                    {
                        //Do Nothing?
                    }
                    else if (rValue.DecimalHalf == InfinityRep)
                    {// + positive infinity to right side
                        DecimalHalf = InfinityRep;
                    }
                    else
                        DecimalHalf += rValue.DecimalHalf;
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
                    throw AltDecBase::RepTypeAsString(LRep) + " RepType addition not supported yet";
                    break;
                }
            }
            else
                RepToRepAddOp(LRep, RRep, *this, rValue);
            return *this;
        }
		
        /// <summary>
        /// Subtraction AltDecBase modifying operation with right side Integer rValue.
        /// (copy by parameter and pointer version)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="rValue.">The rValue</param>
        /// <returns>AltDecBase</returns>
        template<IntegerType IntType=signed int>
        AltDecBase& IntSubOp(const IntType& rValue)
        {
            if (rValue == 0)
                return *this;
	#if defined(AltNum_EnableIRep)
            if(ExtraRep==IRep)
            {
                throw "Can't convert AltDecBase into complex number at moment";
				return *this;
            }
	#endif
	#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
	#endif
	#if defined(AltNum_EnableMixedFractional)
            if(DecimalHalf<0)//Mixed Fraction detected
            {}
			else
	#endif
			if(ExtraRep!=0)//Don't convert if mixed fraction
				ConvertToNormTypeV2();
			bool WasNegative = IntValue < 0;
			IntHalfAdditionOp(rValue);
			//If flips to other side of negative, invert the decimals
	#if defined(AltNum_EnableMixedFractional)
			if(WasNegative ^ (IntValue >= 0))//(WasNegative && IntValue >= 0) || (WasNegative == 0 && IntValue < 0)
			{
				if(DecimalHalf<0)//Flip the fractional half of mixed fraction if flips to other side
				{
		#if defined(AltNum_EnableAlternativeMixedFrac)
					if(ExtraRep<0)// DecimalHalf:-2,ExtraRep:-3 becomes DecimalHalf:-1, ExtraRep:-3
						DecimalHalf = ExtraRep - DecimalHalf;
					else
		#endif			
						DecimalHalf = -(ExtraRep+DecimalHalf);// DecimalHalf:-2,ExtraRep:3 becomes DecimalHalf:-1, ExtraRep:3
				}
				else
					DecimalHalf = AltDecBase::DecimalOverflow - DecimalHalf;
			}
	#else
            if(WasNegative ^ (IntValue >= 0))
				DecimalHalf = AltDecBase::DecimalOverflow - DecimalHalf;
	#endif
            return *this;
        }
		
        /// <summary>
        /// Subtraction Operation  with right side AltDecBase
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rightside rValue</param>
        /// <returns>AltDecBase&</returns>
        AltDecBase& SubOp(AltDecBase rValue)
        {
            if (IsZero())
            {
                IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                ExtraRep = rValue.ExtraRep; return *this;
            }
            else if (rValue.IsZero())
                return *this;
#if defined(AltNum_EnableInfinityRep)
            if (DecimalHalf == InfinityRep)
                return *this;
            if (rValue.DecimalHalf == InfinityRep)
            {
                rValue.IntValue == 1 ? SetAsInfinity() : SetAsNegativeInfinity();
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
#if defined(AltNum_EnablePiRep)
                case RepType::PiNum:
#endif
#if defined(AltNum_EnableENum)
                case RepType::ENum:
#endif
#if defined(AltNum_EnableIRep)
                case RepType::INum:
#endif
                    BasicSubOp(rValue);
                    break;

#if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingBottom:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.0..1 - 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//-0.0..1 + 0.0..1 
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-0.0..1 + 1.0..1
                        {
                            DecimalHalf = 0;
                            IntValue = -rValue.IntValue;
                        }
                        else//-0.0..1 - 5.0..1
                        {
                            IntValue = -rValue.IntValue;
                        }
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.0..1 - 0.0..1 = 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//0.0..1 + 0.0..1 = 0

                        else if (rValue.IntValue < 0)//0.0..1 + 1.0..1
                        {
                            IntValue = -rValue.IntValue;
                        }
                        else//0.0..1 - 5.0..1
                        {
                            DecimalHalf = 0;
                            IntValue = -rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.0..1 - 0.0..1  = -1
                        {/*Do Nothing*/
                        }
                        else if (rValue.IntValue == NegativeRep)//-1.0..1 + 0.0..1
                            DecimalHalf = 0;
                        else if (IntValue == rValue.IntValue)//-1.01 - 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.0..1 + 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//-1.0..1 - 2.0..1 = 1
                        {
                            IntValue -= rValue.IntValue;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.0..1 - 0.0..1
                            DecimalHalf = 0;
                        else if (rValue.IntValue == NegativeRep)//1.0..1 + 0.0..1
                        {/*Do Nothing*/
                        }
                        else if (IntValue == -rValue.IntValue)//1.0..1 - 1.0..1
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.0..1  - 2.0..1
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//1.0..1 - 1.0..1
                        {
                            IntValue -= rValue.IntValue;
                        }
                    }
                    break;
                case RepType::ApproachingTop:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == 0)//-0.9..9 - 0.9..9 = 0
                            IntValue = -1;
                        else if (rValue.IntValue == NegativeRep)//-0.9..9 + 0.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-0.9..9 + 1.9..9

                        else//-0.9..9 - 5.9..9 = -6.9..8
                            IntValue = -rValue.IntValue - 1;
                    }
                    if (IntValue == 0)
                    {
                        if (rValue.IntValue == 0)//0.9..9 - 0.9..9
                            SetAsZero();
                        else if (rValue.IntValue == NegativeRep)//0.9..9 + 0.9..9 = 1.9..8
                            IntValue = 1;
                        else if (rValue.IntValue < 0)//0.9..9 + 1.9..9 = 1.9..8
                            IntValue = -rValue.IntValue;
                        else//0.9..9 - 5.9..9 = -5
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue = -rValue.IntValue;
                        }
                    }
                    else if (IntValue < 0)
                    {
                        if (rValue.IntValue == 0)//-1.9..9 - 0.9..9  = -2.9..8
                            --IntValue;
                        else if (rValue.IntValue == NegativeRep)//-1.9..9  + 0.9..9 = -1
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (IntValue == rValue.IntValue)//-1.01 + 1.01
                            SetAsZero();
                        else if (rValue.IntValue < 0)//-1.9..9 + 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//-1.9..9 - 2.9..9
                        {
                            IntValue -= rValue.IntValue + 1;
                        }
                    }
                    else
                    {
                        if (rValue.IntValue == 0)//1.9..9 - 0.9..9 = 2.9..8
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                        }
                        else if (rValue.IntValue == NegativeRep)//1.9..9 + 0.9..9
                            ++IntValue;
                        else if (IntValue == -rValue.IntValue)//1.9..9 - 1.9..9
                            SetAsZero();
                        else if (rValue.IntValue < 0)// 1.9..9  - 2.9..9
                        {
                            DecimalHalf = 0;
                            IntValue -= rValue.IntValue;
                        }
                        else//1.9..9 - 2.9..9
                        {
                            DecimalHalf = 0; ExtraRep = 0;
                            IntValue -= rValue.IntValue;
                        }
                    }
                    break;

#if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingMidLeft:
                    CatchAllSubtractionV2(rValue, RepType::ApproachingMidLeft);
                    break;
                case RepType::ApproachingMidRight:
                    CatchAllSubtractionV2(rValue, RepType::ApproachingMidRight);
                    break;
#endif
#endif

#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
                    if (ExtraRep == rValue.ExtraRep)
                        BasicSubOp(rValue);
                    else
                        CatchAllSubtractionV2(rValue, RepType::NumByDiv);
                    break;
#if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
#endif
#if defined(AltNum_EnableEFractional)
                case RepType::EFractional://  IntValue/DecimalHalf*e Representation
#endif
#if defined(AltNum_EnableIFractional)
                case RepType::IFractional://  IntValue/DecimalHalf*e Representation
#endif
#if defined(AltNum_UsingAltFractional)
                    if (DecimalHalf == rValue.DecimalHalf)
                    {
                        *this -= rValue.IntValue;
                    }
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicSubOp(rValue);
                    }
                    break;
#endif

#if defined(AltNum_EnableDecimaledAlternativeFractionals)
#if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv://  (rValue/(ExtraRep*-1))*Pi Representation
#elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv://(rValue/(ExtraRep*-1))*e Representation
#elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(rValue/(ExtraRep*-1))*e Representation
#endif
                    if (ExtraRep == rValue.ExtraRep)
                        BasicSubOp(rValue);
                    else
                    {
                        ConvertToNormType(LRep); rValue.ConvertToNormType(LRep);
                        BasicSubOp(rValue);
                    }
                    break;
#endif
#endif

#if defined(AltNum_EnablePiPowers)
                case RepType::PiPower:
                    if (ExtraRep != rValue.ExtraRep)
                    {
                        ConvertPiPowerToPiRep(); rValue.ConvertPiPowerToPiRep();
                    }
                    BasicSubOp(rValue);
                    break;
#endif

#if defined(AltNum_EnableMixedFractional)
                    //Ignoring the fact that mixed fraction could be improperly formatted for spend
                    //because should be corrected after every operation
                case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue -= rValue.IntValue;
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= rValue.ExtraRep;
                        IntValue -= rValue.IntValue * ExtraRep;
                        DecimalHalf -= rValue.DecimalHalf * ExtraRep;
                        if (DecimalHalf<0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= rValue.ExtraRep;
                    }
                    break;
#if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (DecimalHalf*-1)/-ExtraRep
#elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE:
#elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI:
#endif
#if defined(AltNum_EnableAlternativeMixedFrac)
                    if (ExtraRep == rValue.ExtraRep)
                    {
                        IntValue -= rValue.IntValue;
                        DecimalHalf -= rValue.DecimalHalf;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                    }
                    else
                    {
                        IntValue *= -rValue.ExtraRep;
                        IntValue -= rValue.IntValue * -ExtraRep;
                        DecimalHalf -= rValue.DecimalHalf * -ExtraRep;
                        if (DecimalHalf>0)//Subtract the overflow
                        {
                            DecimalHalf += ExtraRep;
                            --IntValue;
                        }
                        else//Add the overflow
                        {
                            DecimalHalf += ExtraRep;
                            ++IntValue;
                        }
                        ExtraRep *= -rValue.ExtraRep;
                    }
                    break;
#endif
#endif

#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
                case RepType::UndefinedButInRange:
                    BasicSubOp(rValue);
                    break;
#if defined(AltNum_EnableWithinMinMaxRange)
                case RepType::WithinMinMaxRange:
                    if (IntValue == NegativeRep)
                    {
                        if (rValue.IntValue == NegativeRep)
                            IntValue = 0;
                    }
                    else if (rValue.IntValue == NegativeRep)
                    {//+positive infinity to left side
#if defined(AltNum_EnableNaN)
                        DecimalHalf = UndefinedRep;
#else
                        throw "Uncertain result";
#endif
                    }
                    else
                        IntValue -= rValue.IntValue;
                    if (DecimalHalf == InfinityRep)
                    {
                        if (rValue.DecimalHalf == InfinityRep)
                            DecimalHalf = 0;
                    }
                    else if (rValue.DecimalHalf == InfinityRep)
                    {// subtractive infinity from real number
#if defined(AltNum_EnableNaN)
                        DecimalHalf = UndefinedRep;
#else
                        throw "Uncertain result";
#endif
                    }
                    else
                        DecimalHalf -= rValue.DecimalHalf;
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
                    throw AltDecBase::RepTypeAsString(LRep) + " RepType subtraction not supported yet";
                    break;
                }
            }
            else
                RepToRepSubOp(LRep, RRep, *this, rValue);
            return *this;
        }