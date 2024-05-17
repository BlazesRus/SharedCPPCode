        /// <summary>
        /// Multiplication Operation Between AltDecBase and unsigned Integer rValue.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>AltDecBase&</returns>
        template<IntegerType IntType=signed int>
        AltDecBase& UIntMultOp(const IntType& rValue)
        {
            if (IsZero()||rValue==1)
                return *this;
            if (rValue == 0)
            {
                SetAsZero(); return *this;
            }
            RepType LRep = GetRepType();
            switch (LRep)
            {
                case RepType::NormalType:
					BasicUIntMultOp(rValue);
					break;
#if defined(AltNum_EnableAlternativeRepFractionals)
                case RepType::NumByDiv:
                {//Reduce divisor if possible
					int divRes = ExtraRep / rValue;
					if((ExtraRep - rValue * divRes)==0)
						ExtraRep = divRes;
					else
						BasicUIntMultOp(rValue);
                }
                break;
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
                {
					IntValue *= rValue;
					if(IntValue==0)
					{
						DecimalHalf = 0; ExtraRep = 0;
					}
                }
                break;
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
					int divRes = ExtraRep / rValue;
					if((ExtraRep - rValue * divRes)==0)
						ExtraRep = divRes;
					else
						BasicUIntMultOp(rValue);
                }
                break; 
    #endif
#endif
    #ifdef AltNum_EnableInfinity
                case RepType::PositiveInfinity:
                case RepType::NegativeInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableIRep)
                case RepType::INum:
					BasicUIntMultOp(rValue);
					break;
    #endif
    #if defined(AltNum_EnableApproaching)
                case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
					if(IntValue!=0&&IntValue!=NegativeRep)
						CatchAllUIntMultiplication(rValue, LRep);
					break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
					if(IntValue==NegativeRep)
						IntValue = -(int)rValue;
					else if(IntValue==0)
						IntValue = (int)rValue - 1;
					else if(IntValue<0)//-5.9..9 * 100
						IntValue = (IntValue-1)*(int)rValue + 1;
					else//5.9..9 * 100 = 599.9..9
						IntValue = (IntValue+1)*(int)rValue - 1;
					break;
                #endif
        #if defined(AltNum_EnableApproachingDivided)
		        case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                {
					if(IntValue==0)
					{
						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
							}
							else
							{
							}
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
			#if !defined(AltNum_DisableApproachingTop)
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
			#else
								DecimalHalf = 999999999;
			#endif
							}
							else
								ExtraRep = divRes;
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                }
                break;
                #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
					if(IntValue==0)
					{
						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = 1;
								DecimalHalf = ApproachingBottomRep;
							}
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = -1;
								DecimalHalf = ApproachingBottomRep;
							}
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                #endif
        #endif
    #endif
    #if defined(AltNum_EnableImaginaryInfinity)
                case RepType::PositiveImaginaryInfinity:
                case RepType::NegativeImaginaryInfinity:
                    return *this;
                    break;
    #endif
    #if defined(AltNum_EnableApproachingI)
                case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
					if(IntValue==0||IntValue==NegativeRep)
					{
						if(rValue<0)
							IntValue = NegativeRep;
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
					break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
					if(IntValue==NegativeRep)
						IntValue = -(int)rValue;
					else if(IntValue==0)
						IntValue = (int)rValue - 1;
					else if(IntValue<0)//-5.9..9 * 100
						IntValue = (IntValue-1)*(int)rValue + 1;
					else//5.9..9 * 100 = 599.9..9
						IntValue = (IntValue+1)*(int)rValue - 1;
					break;
            #endif
        #if defined(AltNum_EnableApproachingDivided)
                case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                {
					if(IntValue==0)
					{
						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
							}
							else
								ExtraRep = divRes;
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.49..9(ExtraRep:2) * 2 = -0.9..9 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							if(divRes == 0)//Become 0.9..9
							{
			#if !defined(AltNum_DisableApproachingTop)
								ExtraRep = 0;
								DecimalHalf = ApproachingTopRep;
			#else
								DecimalHalf = 999999999;
			#endif
							}
							else
								ExtraRep = divRes;
						}
					}
					else
						CatchAllUIntMultiplication(rValue, LRep);
                }
                break;
            #if !defined(AltNum_DisableApproachingTop)
                case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
				{
					if(IntValue==0)
					{
						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = 1;
								DecimalHalf = ApproachingBottomRep;
							}
						}
					}
					else if(IntValue==NegativeRep)
					{
						//-0.50..1(ExtraRep:2) * 2 = -1.0..1 (ExtraRep:0)
						int divRes = ExtraRep/rValue;
						if((ExtraRep - rValue * divRes)==0)
						{
							ExtraRep = divRes;
							if(divRes == 0)
							{
								IntValue = -1;
								DecimalHalf = ApproachingBottomRep;
							}
						}
					}
					else
					{
						ConvertToNormalIRep(LRep);
						BasicUIntMultOp(rValue);
					}
				}
				break;
            #endif
        #endif
    #endif
    #if defined(AltNum_EnableMixedFractional)
                case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                {
					DecimalHalf *= rValue;
					UIntHalfMultiplicationOp(rValue);
					int divRes = DecimalHalf / -ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtractionOp(increment);
                        else
                            IntHalfAdditionOp(increment);
                        DecimalHalf = DecimalHalf + increment;
                    }
				}
                break;
		#if defined(AltNum_EnableMixedPiFractional)
				case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
		#elif defined(AltNum_EnableMixedEFractional)
				case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
		#elif defined(AltNum_EnableMixedIFractional)
				case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
		#endif
		#if defined(AltNum_EnableAlternativeMixedFrac)
                {
                    DecimalHalf *= rValue;
					UIntHalfMultiplicationOp(rValue);
                    int divRes = DecimalHalf / ExtraRep;
                    if (divRes > 0)
                    {
                        int increment = ExtraRep * divRes;
                        if (IntValue < 0)
                            IntHalfSubtractionOp(increment);
                        else
                            IntHalfAdditionOp(increment);
                        DecimalHalf = DecimalHalf - increment;
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
                    CatchAllUIntMultiplication(rValue, LRep);
                    break;
            }
            return *this;
        }