        /// <summary>
        /// Division Operation Between AltDecBase and unsigned Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=signed int>
        AltDecBase& UIntDivOp(const IntType& rValue)
        {
            if (rValue == 1)
                return *this;
            if (rValue == 0)
            {
                #if defined(AltNum_EnableInfinityRep)
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
                #else
                throw "Target rValue can not be divided by zero";
                #endif
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {

#if defined(AltNum_EnableAlternativeRepFractionals)

    #if defined(AltNum_UsingAltFractional)
                #if defined(AltNum_EnablePiFractional)
                case RepType::PiFractional:
                #endif
                #if defined(AltNum_EnableEFractional)
                case RepType::EFractional:
                #endif
                #if defined(AltNum_EnableIFractional)
                case RepType::IFractional:
                #endif
                {//Checking for overflow before applying based on https://www.geeksforgeeks.org/check-integer-overflow-multiplication/#
                    int result = ExtraRep * rValue;
                    if (DecimalHalf == result / rValue)//checking for overflow
                        DecimalHalf = result;
                    else
                        CatchAllIntDivision(rValue, LRep);
                }
                break;
    #endif
#endif
    #if defined(AltNum_EnableDecimaledAlternativeFractionals)
                #if defined(AltNum_EnableDecimaledPiFractionals)
                case RepType::PiNumByDiv:
                    #elif defined(AltNum_EnableDecimaledEFractionals)
                case RepType::ENumByDiv:
                    #elif defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv:
                    #endif
                {
                    int result = ExtraRep * rValue;
                    if (ExtraRep == result / rValue)//checking for overflow
                        ExtraRep = result;
                    else
                        BasicUIntDivOp(rValue);
                }
                break; 
    #endif


    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::ImaginaryInfinity:
                    return *this;
                    break;
    #endif

    #if defined(AltNum_EnableMixedFractional)

        #if defined(AltNum_EnableMixedPiFractional)
                case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
        #elif defined(AltNum_EnableMixedEFractional)
                case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
        #elif defined(AltNum_EnableMixedIFractional)
                case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
        #endif
        #if defined(AltNum_EnableAlternativeMixedFrac)
                {
                    int divRes;
                    int C;
                    if (IsAtZeroInt())//Become Fractional
                    {
                        divRes = DecimalHalf / ExtraRep;//-4/-3
                        C = DecimalHalf - ExtraRep * divRes;//-4 - -3
                        if (C == 0)
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
    
                            #endif
                        }
                        else
                        {
                            #if defined(AltNum_MixedAltFracHasFractionalAccess)//Become Non-Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_MixedAltFracHasDecimaledFractionalAccess)//Become Decimaled Fractional
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedPiFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #elif defined(AltNum_EnableMixedEFractional)//Become NumByDiv
                            throw "ToDo: Impliment code here";
                            #else//if(AltNum_EnableMixedEFractional)--Become NumByDiv
    
                            #endif
                        }
                        //return *this;
                    }
                    divRes = IntValue / ExtraRep;
                    if (divRes != 0)
                    {
                        C = IntValue - ExtraRep * divRes;
                        if (C == 0)
                        {
                            throw "ToDo: Impliment code here";
                        }
                        else
                        {
                            throw "ToDo: Impliment code here";
                        }
                    }
                    else
                    {
                        throw "ToDo: Impliment code here";
                    }
                }
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
                    BasicIntDivOp(rValue);
                    break;
            }
            return *this;
        }