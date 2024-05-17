        /// <summary>
        /// Unsigned multiplication operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= unsigned int>
        auto& UIntMultOpV1(const IntType& rValue)
		{
            if (rValue == 1)
                return *this;
            if (rValue == 0)
            {
                SetAsZero();
                return *this;
            }
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = rValue.GetPiRepType();
                    switch(LRep)
                    {
                        case RepType::PiNum:{

                        } break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::PiNumByDiv:
                        {

                        } break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedPi://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {

                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomPi://(Approaching Towards Zero);(IntValue of 0 results in 0.0...01)
                        {

                        }
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopPi://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRightPi://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeftPi://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
                        {

                        } break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = rValue.GetERepType();
                    switch(LRep)
                    {
                        case RepType::ENum:{

                        } break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::ENumByDiv:
                        {

                        } break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedE://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {

                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomE://(Approaching Towards Zero);(IntValue of 0 results in 0.0...01)
                        {

                        }
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopE://(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRightE://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep-ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep+ApproachingLeftRealrValue if negative)
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeftE://(Approaching Away from Zero is equal to IntValue + 1/ExtraRep+ApproachingLeftRealrValue if positive: IntValue - 1/ExtraRep-ApproachingLeftRealrValue if negative)
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
                        {

                        } break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        #if defined(AltNum_EnableIRep)//IRep_to_integer
        		case 3:{
                    RepType LRep = rValue.GetIRepType();
                    switch(LRep){
                        case RepType::INum:{

                        } break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::INumByDiv:
                        {

                        } break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedI://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {

                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
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

                        }
                        break;
            #endif
            #if defined(AltNum_EnableImaginaryInfinity)
                        case RepType::ImaginaryInfinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        		default:{
                    RepType LRep = rValue.GetNormRepType();
                    switch(LRep)
                    {
                        case RepType::NormalType:
                        {

                        }
                        break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::NumByDiv:
                        {

                        }
                        break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedFrac://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {
                            if (IsAtZeroInt())
                            {
                                
                            }
                            else
                            {
                                boost::rational<unsigned int> lSideFrac = boost::rational<unsigned int>(IntValue.Value, rValue);
                            }
                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottom://(Approaching Towards Zero);(IntValue of 0 results in 0.0...01)
                        {

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

                        } break;
            #endif
            #ifdef AltNum_EnableInfinity
                        case RepType::Infinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        	}
            return *this;
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= signed int>
        auto& IntMultOpV1(const IntType& rValue)
		{
            if(Value<0)
            {
                SwapNegativeStatus();
                UIntMultOpV1(-rValue);
            }
            else
                UIntMultOpV1(rValue);
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= unsigned int>
        auto MultByUIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.UIntDivOpV1(rValue);
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= signed int>
        auto MultByIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.IntDivOpV1(rValue);
		}