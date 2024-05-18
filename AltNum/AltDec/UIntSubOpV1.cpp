        /// <summary>
        /// Subtraction operation between MediumDecBase and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= unsigned int>
        auto& UIntSubOpV1(const IntType& rValue)
		{
            if (rValue == 0)
                return *this;
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = rValue.GetPiRepType();
                    CatchAllUIntSubtraction(rValue, LRep);
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = rValue.GetERepType();
                    CatchAllUIntSubtraction(rValue, LRep);
                } break;
        #endif
        #if defined(AltNum_EnableIRep)//IRep_to_integer
        		case 3:
                    throw "Can't convert into complex number at moment";
                break;
        #endif
        		default:{
                    RepType LRep = rValue.GetNormRepType();
                    switch(LRep)
                    {
                        case RepType::NormalType:
                            BasicUIntSubOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::NumByDiv:
                        #if defined(AltNum_EnableMixedFractional)
                            if(DecimalHalf.Value==0)//Become Mixed Fraction
                            {

                            }
                            else
                        #endif
                                CatchAllUIntSubtraction(rValue, LRep);
                        break;
            #endif
            #if defined(AltNum_EnablePowerOfRepresentation)
                        case RepType::ToPowerOf:
                            if(DecimalHalf.Value==0&&rValue==IntValue)
                                --ExtraRep;
                            else
                                CatchAllUIntSubtraction(rValue, LRep);
                            break;
            #elif defined(AltNum_EnableMixedFractional)
                        case RepType::MixedFrac://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                            IntValue -= rValue;
                        break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottom:
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTop:
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRight:
                        case RepType::ApproachingMidLeft:
                #endif
    #pragma endregion AltDecVariantExclusive
        					if(IsPositive())
        					{
                                if(rValue>IntValue.Value)
								{
                                    IntValue.IsPositive = 0;
                                    IntValue.Value = rValue - IntValue.Value;
								}
								else
									IntValue.Value -= rValue;
                            }
                            else
                                IntValue.Value += rValue;
                            break;
            #endif
            #ifdef AltNum_EnableInfinity
                        case RepType::Infinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer subtraction on current representation.";
                    }
                } break;
        	}
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= signed int>
        auto& IntSubOpV1(const IntType& rValue)
		{
            if(Value<0)
                UIntAddOpV1(-rValue);
            else
                UIntSubOpV1(rValue);
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and unsigned Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= unsigned int>
        auto SubtractByUIntV1(const IntType& rValue){
            auto self = *this;
            return self.UIntSubOpV1(rValue);
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= signed int>
        auto SubtractByIntV1(const IntType& rValue){
            auto self = *this;
            return self.IntSubOpV1(rValue);
		}