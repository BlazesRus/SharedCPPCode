        /// <summary>
        /// Addition operation between MediumDecBase and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= unsigned int>
        auto& UIntAddOpV1(const IntType& rValue)
		{
            if (rValue == 0)
                return *this;
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = rValue.GetPiRepType();
                    CatchAllUIntAddition(rValue, LRep);
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = rValue.GetERepType();
                    CatchAllUIntAddition(rValue, LRep);
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
                            BasicUIntAddOp(rValue);
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
                                CatchAllUIntAddition(rValue, LRep);
                        break;
            #endif
            #if defined(AltNum_EnablePowerOfRepresentation)
                        case RepType::ToPowerOf:
                            if(DecimalHalf.Value==0&&rValue==IntValue)
                                ++ExtraRep;
                            else
                                CatchAllUIntAddition(rValue, LRep);
                            break;
            #elif defined(AltNum_EnableMixedFractional)
                        case RepType::MixedFrac://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                            IntValue += rValue;
                        break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottom:
        					if(IsNegative())
        					{
                                //Add code
                            }
                            else
                                IntValue += rValue;
                            break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTop:
        					if(IsNegative())
        					{
                                //Add code
                            }
                            else
                                IntValue += rValue;
                            break;
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRight:
        					if(IsNegative())
        					{
                                //Add code
                            }
                            else
                                IntValue += rValue;
                        break;
                        case RepType::ApproachingMidLeft:
        					if(IsNegative())
        					{
                                //Add code
                            }
                            else
                                IntValue += rValue;
                        break;
                #endif
    #pragma endregion AltDecVariantExclusive
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
		}

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= signed int>
        auto& IntAddOpV1(const IntType& rValue)
		{
            if(Value<0)
                UIntSubOpV1(-rValue);
            else
                UIntAddOpV1(rValue);
		}

        /// <summary>
        /// Addition operation between MediumDec variant and unsigned Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= unsigned int>
        auto AddByUIntV1(const IntType& rValue){
            auto self = *this;
            return self.UIntAddOpV1(rValue);
		}

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= signed int>
        auto AddByIntV1(const IntType& rValue){
            auto self = *this;
            return self.IntAddOpV1(rValue);
		}