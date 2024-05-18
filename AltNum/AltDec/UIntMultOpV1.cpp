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
                        case RepType::PiNum:
                            BasicUIntMultOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::PiNumByDiv:{
                            if(DecimalHalf.Value==0)
                            {
                                boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(IntValue.Value*rValue, ExtraRep.Value);
                                IntValue.Value = rSideFrac.numerator();
                                ExtraRep.Value = rSideFrac.denominator();
                            }
                            else
                            {
                                //Reduce divisor if possible
            					unsigned int divRes = ExtraRep.Value / rValue;
            					if((ExtraRep.Value - rValue * divRes)==0)
            						ExtraRep.Value = divRes;
            					else
            						BasicUIntMultOp(rValue);
                            }
                        } break;
            #endif
            #if defined(AltNum_EnablePowerOfRepresentation)
                        case RepType::PiPower:
                            CatchAllUIntMultiplication(rValue, LRep);
                            break;
            #elif defined(AltNum_EnableMixedFractional)
                        case RepType::MixedPi://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {
                            if (IsAtZeroInt())
                            {
                                unsigned int result = DecimalHalf.Value * rValue;
                                if (ExtraRep.Value == result / rValue)
                                {
                                    boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value, result);
                                    IntValue.Value = rSideFrac.numerator();
						            DecimalHalf.Value = 0;
                                    ExtraRep = rSideFrac.denominator();
                                }
                                else
                                {
                                    boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value, ExtraRep.Value);
                                    IntValue.Value = rSideFrac.numerator();
						            DecimalHalf.Value = 0;
                                    ExtraRep = rSideFrac.denominator();
						            BasicUIntMultOp(rValue);
                                }
                            }
                            else
                            {
                                boost::rational<unsigned int> rSideFrac = boost::rational<unsigned long long>(DecimalHalf.Value*rValue, ExtraRep.Value);
                                IntValue.Value *= rValue;
                                unsigned long long divRes = rSideFrac.numerator() / rSideFrac.denominator();
                                unsigned long long C = rSideFrac.numerator() - rSideFrac.denominator() * divRes;
                                if(divRes!=0)
                                    IntValue.Value += (unsigned int)divRes;
                                DecimalHalf.Value = (unsigned int) C;
                                ExtraRep.Value = (unsigned int) rSideFrac.denominator();
                            }
                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomPi:{

                        } break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopPi:
        					if(IntValue.Value==0)//0.99.9 * 5 = ~4.9..9 
        						IntValue.Value = (int)rValue - 1;
        					else//5.9..9 * 100 = 599.9..9
        						IntValue.Value = (IntValue.Value+1)*(unsigned int)rValue - 1;
    					break;
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRightPi:
        					if(IntValue.Value==0)
        					{
        						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
        						int divRes = ExtraRep/rValue;
        						if((ExtraRep.Value - rValue * divRes)==0)
        						{
        							if(divRes == 0)//Become 0.9..9
        							{
        								IntValue.Value = 1; ExtraRep = 0;
        								DecimalHalf.Value = ApproachingBottomRep;
        							}
        							else
        								ExtraRep = divRes;
        						}
                                else
        						    CatchAllUIntMultiplication(rValue, LRep);
        					}
        					else
        						CatchAllUIntMultiplication(rValue, LRep);
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeftPi:
        					if(IntValue.Value==0)
        					{
        						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
        						int divRes = ExtraRep.Value/rValue;
        						if((ExtraRep.Value - rValue * divRes)==0)
        						{
        							if(divRes == 0)//Become 0.9..9
        							{
        								ExtraRep = 0;
        								DecimalHalf.Value = ApproachingTopRep;
        							}
        							else
        								ExtraRep = divRes;
        						}
                                else
        						    CatchAllUIntMultiplication(rValue, LRep);
        					}
        					else
        						CatchAllUIntMultiplication(rValue, LRep);
                        break;
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
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
                        case RepType::ENum:
                            BasicUIntMultOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::ENumByDiv:{
                            if(DecimalHalf.Value==0)
                            {
                                boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(IntValue.Value*rValue, ExtraRep.Value);
                                IntValue.Value = rSideFrac.numerator();
                                ExtraRep.Value = rSideFrac.denominator();
                            }
                            else
                            {
                                //Reduce divisor if possible
            					unsigned int divRes = ExtraRep.Value / rValue;
            					if((ExtraRep.Value - rValue * divRes)==0)
            						ExtraRep.Value = divRes;
            					else
            						BasicUIntMultOp(rValue);
                            }
                        } break;
            #endif
            #if defined(AltNum_EnablePowerOfRepresentation)
                        case RepType::EPower:
                            CatchAllUIntMultiplication(rValue, LRep);
                            break;
            #elif defined(AltNum_EnableMixedFractional)
                        case RepType::MixedE://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {
                            if (IsAtZeroInt())
                            {
                                unsigned int result = DecimalHalf.Value * rValue;
                                if (ExtraRep.Value == result / rValue)
                                {
                                    boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value, result);
                                    IntValue.Value = rSideFrac.numerator();
						            DecimalHalf.Value = 0;
                                    ExtraRep = rSideFrac.denominator();
                                }
                                else
                                {
                                    boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value, ExtraRep.Value);
                                    IntValue.Value = rSideFrac.numerator();
						            DecimalHalf.Value = 0;
                                    ExtraRep = rSideFrac.denominator();
						            BasicUIntMultOp(rValue);
                                }
                            }
                            else
                            {
                                boost::rational<unsigned int> rSideFrac = boost::rational<unsigned long long>(DecimalHalf.Value*rValue, ExtraRep.Value);
                                IntValue.Value *= rValue;
                                unsigned long long divRes = rSideFrac.numerator() / rSideFrac.denominator();
                                unsigned long long C = rSideFrac.numerator() - rSideFrac.denominator() * divRes;
                                if(divRes!=0)
                                    IntValue.Value += (unsigned int)divRes;
                                DecimalHalf.Value = (unsigned int) C;
                                ExtraRep.Value = (unsigned int) rSideFrac.denominator();
                            }
                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomE:{

                        } break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopE:
        					if(IntValue.Value==0)//0.99.9 * 5 = ~4.9..9 
        						IntValue.Value = (int)rValue - 1;
        					else//5.9..9 * 100 = 599.9..9
        						IntValue.Value = (IntValue.Value+1)*(unsigned int)rValue - 1;
    					break;
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRightE:
        					if(IntValue.Value==0)
        					{
        						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
        						int divRes = ExtraRep/rValue;
        						if((ExtraRep.Value - rValue * divRes)==0)
        						{
        							if(divRes == 0)//Become 0.9..9
        							{
        								IntValue.Value = 1; ExtraRep = 0;
        								DecimalHalf.Value = ApproachingBottomRep;
        							}
        							else
        								ExtraRep = divRes;
        						}
                                else
        						    CatchAllUIntMultiplication(rValue, LRep);
        					}
        					else
        						CatchAllUIntMultiplication(rValue, LRep);
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeftE:
        					if(IntValue.Value==0)
        					{
        						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
        						int divRes = ExtraRep.Value/rValue;
        						if((ExtraRep.Value - rValue * divRes)==0)
        						{
        							if(divRes == 0)//Become 0.9..9
        							{
        								ExtraRep = 0;
        								DecimalHalf = ApproachingTopRep;
        							}
        							else
        								ExtraRep = divRes;
        						}
                                else
        						    CatchAllUIntMultiplication(rValue, LRep);
        					}
        					else
        						CatchAllUIntMultiplication(rValue, LRep);
                        break;
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
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
                        case RepType::INum:
                            BasicUIntMultOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::INumByDiv:{
                            if(DecimalHalf.Value==0)
                            {
                                boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(IntValue.Value*rValue, ExtraRep.Value);
                                IntValue.Value = rSideFrac.numerator();
                                ExtraRep.Value = rSideFrac.denominator();
                            }
                            else
                            {
                                //Reduce divisor if possible
            					unsigned int divRes = ExtraRep.Value / rValue;
            					if((ExtraRep.Value - rValue * divRes)==0)
            						ExtraRep.Value = divRes;
            					else
            						BasicUIntMultOp(rValue);
                            }
                        } break;
            #endif
            #if defined(AltNum_EnableMixedFractional)
                        case RepType::MixedI://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                            if (IsAtZeroInt())
                            {
                                unsigned int result = DecimalHalf.Value * rValue;
                                if (ExtraRep.Value == result / rValue)
                                {
                                    boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value, result);
                                    IntValue.Value = rSideFrac.numerator();
						            DecimalHalf.Value = 0;
                                    ExtraRep = rSideFrac.denominator();
                                }
                                else
                                {
                                    boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value, ExtraRep.Value);
                                    IntValue.Value = rSideFrac.numerator();
						            DecimalHalf.Value = 0;
                                    ExtraRep = rSideFrac.denominator();
						            BasicUIntMultOp(rValue);
                                }
                            }
                            else
                            {
                                boost::rational<unsigned int> rSideFrac = boost::rational<unsigned long long>(DecimalHalf.Value*rValue, ExtraRep.Value);
                                IntValue.Value *= rValue;
                                unsigned long long divRes = rSideFrac.numerator() / rSideFrac.denominator();
                                unsigned long long C = rSideFrac.numerator() - rSideFrac.denominator() * divRes;
                                if(divRes!=0)
                                    IntValue.Value += (unsigned int)divRes;
                                DecimalHalf.Value = (unsigned int) C;
                                ExtraRep.Value = (unsigned int) rSideFrac.denominator();
                            }
                        break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingImaginaryBottom:{

                        } break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingImaginaryTop:
        					if(IntValue.Value==0)//0.99.9 * 5 = ~4.9..9 
        						IntValue.Value = (int)rValue - 1;
        					else//5.9..9 * 100 = 599.9..9
        						IntValue.Value = (IntValue.Value+1)*(unsigned int)rValue - 1;
    					break;
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingImaginaryMidRight:
        					if(IntValue.Value==0)
        					{
        						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
        						unsigned int divRes = ExtraRep.Value/rValue;
        						if((ExtraRep.Value - rValue * divRes)==0)
        						{
        							if(divRes == 0)//Become 0.9..9
        							{
        								IntValue.Value = 1; ExtraRep = 0;
        								DecimalHalf.Value = ApproachingBottomRep;
        							}
        							else
        								ExtraRep = divRes;
        						}
                                else
        						    CatchAllUIntMultiplication(rValue, LRep);
        					}
        					else
        						CatchAllUIntMultiplication(rValue, LRep);
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingImaginaryMidLeft:
        					if(IntValue.Value==0)
        					{
        						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
        						unsigned int divRes = ExtraRep.Value/rValue;
        						if((ExtraRep.Value - rValue * divRes)==0)
        						{
        							if(divRes == 0)//Become 0.9..9
        							{
        								ExtraRep = 0;
        								DecimalHalf = ApproachingTopRep;
        							}
        							else
        								ExtraRep = divRes;
        						}
                                else
        						    CatchAllUIntMultiplication(rValue, LRep);
        					}
        					else
        						CatchAllUIntMultiplication(rValue, LRep);
                        break;
                        #endif
                #endif
    #pragma endregion AltDecVariantExclusive
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
                            BasicUIntMultOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
            #if defined(AltNum_EnableFractionals)
                        case RepType::NumByDiv:{
                            if(DecimalHalf.Value==0)
                            {
                                boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(IntValue.Value*rValue, ExtraRep.Value);
                                IntValue.Value = rSideFrac.numerator();
                                ExtraRep.Value = rSideFrac.denominator();
                            }
                            else
                            {
                                //Reduce divisor if possible
            					unsigned int divRes = ExtraRep.Value / rValue;
            					if((ExtraRep.Value - rValue * divRes)==0)
            						ExtraRep.Value = divRes;
            					else
            						BasicUIntMultOp(rValue);
                            }
                        }
                        break;
            #endif
            #if defined(AltNum_EnablePowerOfRepresentation)
                        case RepType::ToPowerOf:
                            if(DecimalHalf.Value==0)
                            {
                                if(IntValue==rValue)
                                {
                                #if defined(AltNum_EnableNegativePowerRep)
                                    if(ExtraRep<0)
                                        --ExtraRep;
                                    else
                                #endif
                                        ++ExtraRep.Value;
                                }
                                else
                                    CatchAllUIntMultiplication(rValue, LRep);
                            }
                            else
                                CatchAllUIntMultiplication(rValue, LRep);
                            break;
            #elif defined(AltNum_EnableMixedFractional)
                        case RepType::MixedFrac://IntValue + (DecimalHalf.Value)/ExtraRep.Value
                        {
                            if (IsAtZeroInt())
                            {
                                unsigned int result = DecimalHalf.Value * rValue;
                                if (ExtraRep.Value == result / rValue)
                                {
                                    boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value, result);
                                    IntValue.Value = rSideFrac.numerator();
						            DecimalHalf.Value = 0;
                                    ExtraRep = rSideFrac.denominator();
                                }
                                else
                                {
                                    boost::rational<unsigned int> rSideFrac = boost::rational<unsigned int>(DecimalHalf.Value, ExtraRep.Value);
                                    IntValue.Value = rSideFrac.numerator();
						            DecimalHalf.Value = 0;
                                    ExtraRep = rSideFrac.denominator();
						            BasicUIntMultOp(rValue);
                                }
                            }
                            else
                            {
                                boost::rational<unsigned int> rSideFrac = boost::rational<unsigned long long>(DecimalHalf.Value*rValue, ExtraRep.Value);
                                IntValue.Value *= rValue;
                                unsigned long long divRes = rSideFrac.numerator() / rSideFrac.denominator();
                                unsigned long long C = rSideFrac.numerator() - rSideFrac.denominator() * divRes;
                                if(divRes!=0)
                                    IntValue.Value += (unsigned int)divRes;
                                DecimalHalf.Value = (unsigned int) C;
                                ExtraRep.Value = (unsigned int) rSideFrac.denominator();
                            }
                        } break;
            #endif
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottom:{

                        } break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTop:
        					if(IntValue.Value==0)//0.99.9 * 5 = ~4.9..9 
        						IntValue.Value = (int)rValue - 1;
        					else//5.9..9 * 100 = 599.9..9
        						IntValue.Value = (IntValue.Value+1)*(unsigned int)rValue - 1;
    					break;
                        #endif
    #pragma region AltDecVariantExclusive
                #if defined(AltNum_EnableApproachingDivided)
                        case RepType::ApproachingMidRight:
        					if(IntValue.Value==0)
        					{
        						//0.50..1(ExtraRep:2) * 2 = 1.0..1 (ExtraRep:0)
        						unsigned int divRes = ExtraRep.Value/rValue;
        						if((ExtraRep.Value - rValue * divRes)==0)
        						{
        							if(divRes == 0)//Become 0.9..9
        							{
        								IntValue.Value = 1; ExtraRep = 0;
        								DecimalHalf.Value = ApproachingBottomRep;
        							}
        							else
        								ExtraRep = divRes;
        						}
                                else
        						    CatchAllUIntMultiplication(rValue, LRep);
        					}
        					else
        						CatchAllUIntMultiplication(rValue, LRep);
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingMidLeft:
        					if(IntValue.Value==0)
        					{
        						//0.49..9(ExtraRep:2) * 2 = 0.9..9 (ExtraRep:0)
        						unsigned int divRes = ExtraRep.Value/rValue;
        						if((ExtraRep.Value - rValue * divRes)==0)
        						{
        							if(divRes == 0)//Become 0.9..9
        							{
        								ExtraRep = 0;
        								DecimalHalf = ApproachingTopRep;
        							}
        							else
        								ExtraRep = divRes;
        						}
                                else
        						    CatchAllUIntMultiplication(rValue, LRep);
        					}
        					else
        						CatchAllUIntMultiplication(rValue, LRep);
                        break;
                        #endif
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