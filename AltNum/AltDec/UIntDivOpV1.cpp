        /// <summary>
        /// Unsigned division operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= unsigned int>
        auto UIntDivOpV1(const IntType& rValue)
		{
            if (rValue == 1)
                return *this;
            if (rValue == 0)
            {
                #if defined(AltNum_EnableInfinityRep)&&defined(AltNum_DefineDivideByZeroAsInfinity)
                if (IntValue < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
                #else
                throw "Target rValue can not be divided by zero";
                #endif
            }
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = rValue.GetPiRepType();
                    switch(LRep){
                    }
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = rValue.GetERepType();
                    switch(LRep){
                    }
                } break;
        #endif
        #if defined(AltNum_EnableIRep)//IRep_to_integer
        		case 3:{
                    RepType LRep = rValue.GetIRepType();
                    switch(LRep){
                        case RepType::INum:
                        {
                            #if defined(AltNum_EnableFractionals)
                            ExtraRep = rValue;
                            #else
                            BasicUIntDivOp(rValue);
                            #endif
                        }
                        break;
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
                    #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
                    #endif
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingImaginaryMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                    #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingImaginaryMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                    #endif
                #endif
                        {
                            ConvertToNormalIRep(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
            #endif

                    }
                } break;
        #endif
        		default:{
                    RepType LRep = rValue.GetNormRepType();
                    switch(LRep){
                        case RepType::NormalType:{
                            #if defined(AltNum_EnableAlternativeRepFractionals)
                            ExtraRep = rValue;
                            #else
                            BasicUIntDivOp(rValue);
                            #endif
                        }
                        break;
    #pragma region AltDecVariantExclusive	
            #if defined(AltNum_EnableFractionals)
                        case RepType::NumByDiv:{
                            int result = ExtraRep * rValue;
                            if (ExtraRep.Value == result / rValue)//checking for overflow
                                ExtraRep.Value = result;
                            else
                                BasicUIntDivOp(rValue);
                        }
                        break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedFrac://IntValue + DecimalHalf.Value/ExtraRep.Value
                        {
                            unsigned int divRes;
                            unsigned int C;
                            if (IsAtZeroInt())//Become NumByDiv
                            {
                                divRes = DecimalHalf.Value / ExtraRep.Value;
                                C = DecimalHalf.Value - ExtraRep.Value * divRes;
                                if (C == 0)
                                {
                                    IntValue = divRes;
                                    DecimalHalf = 0;
                                }
                                else
                                {
                                    IntValue.Value = DecimalHalf.Value;
                                    DecimalHalf = 0;
                                    ExtraRep *= rValue;
                                }
                            }
							else
							{
								divRes = IntValue.Value / ExtraRep.Value;
								C = IntValue.Value - ExtraRep.Value * divRes;
								if (C == 0)
								{
									throw "ToDo: Impliment code here";
								}
								else
								{
									throw "ToDo: Impliment code here";
								}
							}
                        }
                        break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.0...01)
                        {
                            if (IsAtZeroInt())
                                return *this;
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTop://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRight://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeft://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
                        {
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
            #endif
            #ifdef AltNum_EnableInfinity
                        case RepType::Infinity:
                            return *this;
                            break;
            #endif
                    }
                } break;
        	}
		}

        /// <summary>
        /// Division operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= signed int>
        auto IntDivOpV1(const IntType& rValue)
		{
            if(Value<0)
            {
                SwapNegativeStatus();
                UIntDivOpV1(-Value);
            }
            else
                UIntDivOpV1(Value);
		}

        /// <summary>
        /// Unsigned division operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base</returns>
        template<IntegerType IntType= unsigned int>
        auto DivByUIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.UIntDivOpV1(rValue);
		}

        /// <summary>
        /// Division operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base</returns>
        template<IntegerType IntType= signed int>
        constexpr auto DivByIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.IntDivOpV1(rValue);
		}