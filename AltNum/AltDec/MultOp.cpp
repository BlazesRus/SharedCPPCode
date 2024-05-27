#include "AltDecBase.hpp"
#include <stdexcept>
using MediumDecVariant = BlazesRusCode::AltDecBase;
using RepType = BlazesRusCode::RepType;

void CatchAllOp(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
    ConvertToNormType(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedMultOp(RValue);
}

void RightSideOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedMultOp(RValue);
}

#pragma region AltDecVariantExclusive

#if defined(AltNum_EnablePiRep)

//Convert right side down to Pi representation and then perform operation
void RightSidePiOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertToPiRep(RRep);
	BasicUnsignedMultOp(RValue);
    BasicUnsignedMultOp(PiNum);
}

//Convert left side down to Pi representation and right side down to normal
//and then perform operation
void LeftSidePiOp(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	ConvertToPiRep(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedMultOp(RValue);
}
#endif

#if defined(AltNum_EnableERep)

//Convert right side down to e representation and then perform operation
void RightSideEOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertToERep(RRep);
	BasicUnsignedMultOp(RValue);
    BasicUnsignedMultOp(ENum);
}

//Convert left side down to e representation and right side down to normal
//and then perform operation
void LeftSideEOp(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	ConvertToERep(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedMultOp(RValue);
}
#endif

#if defined(AltNum_EnableIRep)
//Convert right side down to i representation and then perform operation
void RightSideIOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertToIRep(RRep);
	BasicUnsignedMultOp(RValue);
    DecimalHalf.Flags = 3;
}

//Convert left side down to i representation and right side down to normal
//and then perform operation
void LeftSideIOp(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	ConvertToIRep(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedMultOp(RValue);
}
#endif

void SameRep_NumByDiv(const auto& rValue, const RepType& LRep)
{
    //Add Code Here
}

void SameRep_PowerOf(const auto& rValue, const RepType& LRep)
{
    //Add Code Here
}

void SameRep_MixedFrac(const auto& rValue, const RepType& LRep)
{
    //Add Code Here
}

#if defined(AltNum_EnableWithinMinMaxRange)
void SameRep_WithinMinMaxRange
{
	throw "WithinMinMaxRange code not adjusted yet to changes in code.";
}
#endif
#pragma endregion AltDecVariantExclusive

void SameRep_ApproachingBottom(const auto& rValue)
{
    //Add Code Here
}

void SameRep_ApproachingTop(const auto& rValue)
{
    //Add Code Here
}

void NormalToNormalOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	if(LRep==RRep)
	{
		switch(LRep)
		{
			case RepType::NormalType:
				BasicMultOp(rValue); break;
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnableFractionals)
			case RepType::NumByDiv:
				SameRep_NumByDiv(rValue, LRep); break;
	#endif
	#if defined(AltNum_EnablePowerOfRepresentation)
			case RepType::ToPowerOf:
				SameRep_PowerOf(rValue, LRep); break;
	#endif
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac:
				SameRep_MixedFrac(rValue, LRep); break;
	#endif
#pragma endregion AltDecVariantExclusive
	#if defined(AltNum_EnableApproaching)
			case RepType::ApproachingBottom:
				SameRep_ApproachingBottom(rValue); break;
			case RepType::ApproachingTop:
				SameRep_ApproachingTop(rValue); break;
#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
            //To-Do:test if more effective to check via if statement vs try-catch block
			case RepType::ApproachingMidLeft:
                if(rValue.IntHalf.Value==0)
                {
                    if(IntHalf.Value==0)
    				{
    					//0.249..9(ExtraRep:4) * 0.49..9(ExtraRep:2) = ~0.1249..9(ExtraRep:8)
                        try{
                            unsigned int result = ExtraRep.Value * rValue.ExtraRep;
                            ExtraRep.Value = result;
                        }
                        catch (std::overflow_error& e){
                            CatchAllOp(rValue);
                        }
    				} else {
                        //2.249..9(ExtraRep:4) * 0.49..9(ExtraRep:2) = ~1.1249..9(ExtraRep:8)
                        try{
                            unsigned int result = ExtraRep.Value * rValue.ExtraRep;
                            unsigned int intHalfRes = IntHalf.Value / rValue.ExtraRep;
                            if (IntHalf.Value == intHalfRes * rValue)//checking for truncation
						    {
                                ExtraRep.Value = result;
                                IntHalf.Value *= intHalfRes;
                            } else
						        CatchAllOp(rValue);
                        }
                        catch (std::overflow_error& e){
                            CatchAllOp(rValue);
                        }
                    }
                }
                else
					CatchAllOp(rValue);
				break;
			case RepType::ApproachingMidRight:
                if(rValue.IntHalf.Value==0)
                {
                    if(IntHalf.Value==0)
    				{
    					//0.250..01(ExtraRep:4) * 0.50..01(ExtraRep:2) = ~0.1250..01(ExtraRep:8)
                        try{//Can also test via if (ExtraRep.Value == result / rValue) to test if overflowed
                            unsigned int result = ExtraRep.Value * rValue.ExtraRep;
                            ExtraRep.Value = result;
                        }
                        catch (std::overflow_error& e){
                            CatchAllOp(rValue);
                        }
                    } else {
                        try{
                            unsigned int result = ExtraRep.Value * rValue.ExtraRep;
                            unsigned int intHalfRes = IntHalf.Value / rValue.ExtraRep;
                            if (IntHalf.Value == intHalfRes * rValue)//checking for truncation
						    {
                                ExtraRep.Value = result;
                                IntHalf.Value *= intHalfRes;
                            } else
						        CatchAllOp(rValue);
                        }
                        catch (std::overflow_error& e){
                            CatchAllOp(rValue);
                        }
                    }
				}
                else
					CatchAllOp(rValue);
				break;
		#endif
#pragma endregion AltDecVariantExclusive
	#endif
#if defined(AltNum_EnableUndefinedButInRange)
			case RepType::WithinMinMaxRange:
				SameRep_WithinMinMaxRange(rValue, LRep); break;
#endif
			default:
				throw "Unsupported operation";
		}
	}
	else
	{
		switch(LRep)
		{
			case RepType::NormalType:{
				switch(RRep){
					//case RepType::NormalType:{
					//}; break;
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					//case RepType::NumByDiv:{
					//}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					//case RepType::ToPowerOf:{
					//}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					//case RepType::MixedFrac:{
					//}; break;
			#endif
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					#if !defined(AltNum_DisableApproachingTop)
					//case RepType::ApproachingTop:{
					//} break;
					#endif
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
					//case RepType::ApproachingMidLeft:{
					//} break;
					//case RepType::ApproachingMidRight:{
					//} break;
			#endif
		#pragma endregion AltDecVariantExclusive
					default:
						CatchAllOp(rValue, LRep, RRep);
				}
			} break;
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnableFractionals)
			case RepType::NumByDiv:{
				switch(RRep){
					//case RepType::NormalType:{
					//}; break;
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					//case RepType::NumByDiv:{
					//}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					//case RepType::ToPowerOf:{
					//}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					//case RepType::MixedFrac:{
					//}; break;
			#endif
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					#if !defined(AltNum_DisableApproachingTop)
					//case RepType::ApproachingTop:{
					//} break;
					#endif
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
					//case RepType::ApproachingMidLeft:{
					//} break;
					//case RepType::ApproachingMidRight:{
					//} break;
			#endif
		#pragma endregion AltDecVariantExclusive
					default:
						CatchAllOp(rValue, LRep, RRep);
				}
			}; break;
	#endif
	#if defined(AltNum_EnablePowerOfRepresentation)
			case RepType::ToPowerOf:{
				switch(RRep){
					//case RepType::NormalType:{
					//}; break;
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					//case RepType::NumByDiv:{
					//}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					//case RepType::ToPowerOf:{
					//}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					//case RepType::MixedFrac:{
					//}; break;
			#endif
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					#if !defined(AltNum_DisableApproachingTop)
					//case RepType::ApproachingTop:{
					//} break;
					#endif
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
					//case RepType::ApproachingMidLeft:{
					//} break;
					//case RepType::ApproachingMidRight:{
					//} break;
			#endif
		#pragma endregion AltDecVariantExclusive
					default:
						CatchAllOp(rValue, LRep, RRep);
				}
			}; break;
	#endif
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac:{
				switch(RRep){
					//case RepType::NormalType:{
					//}; break;
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					//case RepType::NumByDiv:{
					//}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					//case RepType::ToPowerOf:{
					//}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					//case RepType::MixedFrac:{
					//}; break;
			#endif
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					#if !defined(AltNum_DisableApproachingTop)
					//case RepType::ApproachingTop:{
					//} break;
					#endif
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
					//case RepType::ApproachingMidLeft:{
					//} break;
					//case RepType::ApproachingMidRight:{
					//} break;
			#endif
		#pragma endregion AltDecVariantExclusive
					default:
						CatchAllOp(rValue, LRep, RRep);
				}
			}; break;
	#endif
#pragma endregion AltDecVariantExclusive
			case RepType::ApproachingBottom:{
				switch(RRep){
					//case RepType::NormalType:{
					//}; break;
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					//case RepType::NumByDiv:{
					//}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					//case RepType::ToPowerOf:{
					//}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					//case RepType::MixedFrac:{
					//}; break;
			#endif
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					#if !defined(AltNum_DisableApproachingTop)
					//case RepType::ApproachingTop:{
					//} break;
					#endif
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
					//case RepType::ApproachingMidLeft:{
					//} break;
					//case RepType::ApproachingMidRight:{
					//} break;
			#endif
		#pragma endregion AltDecVariantExclusive
					default:
						CatchAllOp(rValue, LRep, RRep);
				}
			} break;
			case RepType::ApproachingTop:{
				switch(RRep){
				}
			} break;
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingMidLeft:{
				switch(RRep){
					//case RepType::NormalType:{
					//}; break;
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					//case RepType::NumByDiv:{
					//}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					//case RepType::ToPowerOf:{
					//}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					//case RepType::MixedFrac:{
					//}; break;
			#endif
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					//case RepType::ApproachingTop:{
					//} break;
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
					//case RepType::ApproachingMidRight:{
					//} break;
			#endif
		#pragma endregion AltDecVariantExclusive
					default:
						CatchAllOp(rValue, LRep, RRep);
				}
			} break;
			case RepType::ApproachingMidRight:{
				switch(RRep){
					//case RepType::NormalType:{
					//}; break;
		#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					//case RepType::NumByDiv:{
					//}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					//case RepType::ToPowerOf:{
					//}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					//case RepType::MixedFrac:{
					//}; break;
			#endif
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					//case RepType::ApproachingTop:{
					//} break;
		#pragma region AltDecVariantExclusive
					//case RepType::ApproachingMidLeft:{
					//} break;
		#pragma endregion AltDecVariantExclusive
					default:
						CatchAllOp(rValue, LRep, RRep);
				}
			} break;
	#endif
#pragma endregion AltDecVariantExclusive
			case RepType::InfinityRep:
                break;
			default:
				throw "Unsupported operation";
		}
	}
}

void CatchAllOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	RepType convertedLRep = ConvertToNormalEquivalant(LRep, ConvertedLRep);
	auto convertedRVal = rValue.ConvertAsNormalEquivalant(RRep, ConvertedRRep);
	NormalToNormalOperation(convertedRVal.first, convertedLRep, convertedRVal.second);
}

#if defined(AltNum_EnablePiRep)
void PiToNormalOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
#if defined(AltNum_EnableInfinityRep)
    if(RRep==RepType::InfinityRep){
    	if(IsPositive())
    		SetAsInfinity();
    	else
    		SetAsNegativeInfinity();
        return;
    }
#endif
	switch(LRep)
	{
		case RepType::PiNum:{
			switch(RRep){
				//case RepType::NormalType:{
				//}; break;
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				//case RepType::NumByDiv:{
				//}; break;
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
				//case RepType::ToPowerOf:{
				//}; break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
				//case RepType::MixedFrac:{
				//}; break;
		#endif
	#pragma endregion AltDecVariantExclusive
				//case RepType::ApproachingBottom:{
				//} break;
				#if !defined(AltNum_DisableApproachingTop)
				//case RepType::ApproachingTop:{
				//} break;
				#endif
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				//case RepType::ApproachingMidLeft:{
				//} break;
				//case RepType::ApproachingMidRight:{
				//} break;
		#endif
	#pragma endregion AltDecVariantExclusive
				default:
					CatchAllOp(rValue, LRep, RRep);
			}
		} break;
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnablePowerOfRepresentation)
		case RepType::PiPower:{
			switch(RRep){
				//case RepType::NormalType:{
				//}; break;
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				//case RepType::NumByDiv:{
				//}; break;
		#endif
				//case RepType::ToPowerOf:{
				//}; break;
	#pragma endregion AltDecVariantExclusive
				//case RepType::ApproachingBottom:{
				//} break;
				#if !defined(AltNum_DisableApproachingTop)
				//case RepType::ApproachingTop:{
				//} break;
				#endif
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				//case RepType::ApproachingMidLeft:{
				//} break;
				//case RepType::ApproachingMidRight:{
				//} break;
		#endif
	#pragma endregion AltDecVariantExclusive
				default:
					CatchAllOp(rValue, LRep, RRep);
			}
		} break;
	#endif
	#if defined(AltNum_EnableFractionals)
		case RepType::PiNumByDiv:{
			switch(RRep){
				//case RepType::NormalType:{
				//}; break;
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				//case RepType::NumByDiv:{
				//}; break;
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
				//case RepType::ToPowerOf:{
				//}; break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
				//case RepType::MixedFrac:{
				//}; break;
		#endif
	#pragma endregion AltDecVariantExclusive
				//case RepType::ApproachingBottom:{
				//} break;
				#if !defined(AltNum_DisableApproachingTop)
				//case RepType::ApproachingTop:{
				//} break;
				#endif
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				//case RepType::ApproachingMidLeft:{
				//} break;
				//case RepType::ApproachingMidRight:{
				//} break;
		#endif
	#pragma endregion AltDecVariantExclusive
				default:
					CatchAllOp(rValue, LRep, RRep);
			}
		} break;
	#endif
	#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedPi:{
			switch(RRep){
				//case RepType::NormalType:{
				//}; break;
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				//case RepType::NumByDiv:{
				//}; break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
				//case RepType::MixedFrac:{
				//}; break;
		#endif
	#pragma endregion AltDecVariantExclusive
				//case RepType::ApproachingBottom:{
				//} break;
				#if !defined(AltNum_DisableApproachingTop)
				//case RepType::ApproachingTop:{
				//} break;
				#endif
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				//case RepType::ApproachingMidLeft:{
				//} break;
				//case RepType::ApproachingMidRight:{
				//} break;
		#endif
	#pragma endregion AltDecVariantExclusive
				default:
					CatchAllOp(rValue, LRep, RRep);
			}
		} break;
	#endif
#pragma endregion AltDecVariantExclusive
	#if defined(AltNum_EnableApproaching)
		case RepType::ApproachingBottomPi:{
			switch(RRep){
				//case RepType::NormalType:{
				//}; break;
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				//case RepType::NumByDiv:{
				//}; break;
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
				//case RepType::ToPowerOf:{
				//}; break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
				//case RepType::MixedFrac:{
				//}; break;
		#endif
	#pragma endregion AltDecVariantExclusive
				#if !defined(AltNum_DisableApproachingTop)
				//case RepType::ApproachingTop:{
				//} break;
				#endif
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				//case RepType::ApproachingMidLeft:{
				//} break;
				//case RepType::ApproachingMidRight:{
				//} break;
		#endif
	#pragma endregion AltDecVariantExclusive
				default:
					CatchAllOp(rValue, LRep, RRep);
			}
		} break;
		#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTopPi:{
			switch(RRep){
				//case RepType::NormalType:{
				//}; break;
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				//case RepType::NumByDiv:{
				//}; break;
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
				//case RepType::ToPowerOf:{
				//}; break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
				//case RepType::MixedFrac:{
				//}; break;
		#endif
	#pragma endregion AltDecVariantExclusive
				//case RepType::ApproachingBottom:{
				//} break;
	#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				//case RepType::ApproachingMidLeft:{
				//} break;
				//case RepType::ApproachingMidRight:{
				//} break;
		#endif
	#pragma endregion AltDecVariantExclusive
				default:
					CatchAllOp(rValue, LRep, RRep);
			}
		} break;
		#endif
	#endif
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeftPi:{
			switch(RRep){
				case RepType::NormalType:{
                    if(rValue.DecimalHalf==0){
                        try{
                            unsigned int intHalfRes = IntHalf.Value * rValue.IntHalf.Value;
                            unsigned int result = ExtraRep.Value / rValue.IntHalf.Value;
                            if (ExtraRep.Value == result * rValue.IntHalf.Value){
                                ExtraRep.Value = result;
                                IntHalf.Value = intHalfRes;
                            } else
                                LeftSidePiOp(rValue);
                        }
                        catch (std::overflow_error& e){ LeftSidePiOp(rValue); }
                    }
				}; break;
				case RepType::NumByDiv:{
                    if(rValue.DecimalHalf==0){
                        if(rValue.IntHalf==1){
                            try{ ExtraRep *= rValue.ExtraRep; }
                            catch (std::overflow_error& e){ LeftSidePiOp(rValue); }
                        }
                        else
                        {
                            try{
                                unsigned int intHalfRes = IntHalf.Value * rValue.IntHalf.Value;
                                unsigned int result01 = ExtraRep * rValue.ExtraRep;
                                unsigned int result02 = result01 / rValue.IntHalf.Value;
                                if (result01 == result02 * rValue.IntHalf.Value){
                                    ExtraRep.Value = result02;
                                    IntHalf.Value = intHalfRes;
                                } else
                                    LeftSidePiOp(rValue);
                            }
                            catch (std::overflow_error& e){ LeftSidePiOp(rValue); }
                        }
                    }
					LeftSidePiOp(rValue);
				}; break;
				case RepType::ApproachingMidLeft:
					if(rValue.IntHalf.Value==0)
					{
						if(IntHalf.Value==0){
                            try{//Can also test via if (ExtraRep.Value == result / rValue) to test if overflowed
                                unsigned int result = ExtraRep.Value * rValue.ExtraRep;
                                ExtraRep.Value = result;
                            }
                            catch (std::overflow_error& e){
                                LeftSidePiOp(rValue);
                            }
						} else {
                            try{
                                unsigned int result = ExtraRep.Value * rValue.ExtraRep;
                                unsigned int intHalfRes = IntHalf.Value / rValue.ExtraRep;
                                if (IntHalf.Value == intHalfRes * rValue)//checking for truncation
    						    {
                                    ExtraRep.Value = result;
                                    IntHalf.Value *= intHalfRes;
                                } else
    						        LeftSidePiOp(rValue);
                            }
                            catch (std::overflow_error& e){
                                LeftSidePiOp(rValue);
                            }
						}
					}
					else
						LeftSidePiOp(rValue);
					break;
				default:
					LeftSidePiOp(rValue);
			}
		} break;
		case RepType::ApproachingMidRightPi:{
			switch(RRep){
				case RepType::NormalType:{
					LeftSidePiOp(rValue);
				}; break;
				case RepType::ApproachingMidRight:
					if(rValue.IntHalf.Value==0)
					{
						if(IntHalf.Value==0){
                            try{//Can also test via if (ExtraRep.Value == result / rValue) to test if overflowed
                                unsigned int result = ExtraRep.Value * rValue.ExtraRep;
                                ExtraRep.Value = result;
                            }
                            catch (std::overflow_error& e){
                                LeftSidePiOp(rValue);
                            }
						} else {
                            try{
                                unsigned int result = ExtraRep.Value * rValue.ExtraRep;
                                unsigned int intHalfRes = IntHalf.Value / rValue.ExtraRep;
                                if (IntHalf.Value == intHalfRes * rValue)//checking for truncation
    						    {
                                    ExtraRep.Value = result;
                                    IntHalf.Value *= intHalfRes;
                                } else
    						        LeftSidePiOp(rValue);
                            }
                            catch (std::overflow_error& e){
                                LeftSidePiOp(rValue);
                            }
						}
					}
					else
						LeftSidePiOp(rValue);
					break;
				default:
					LeftSidePiOp(rValue);
			}
		} break;
	#endif
#pragma endregion AltDecVariantExclusive
	default:
		throw "Unsupported operation"; break;
	}
}

//Right side value converted to Normal Equivalant before performing operation
void CatchAllPiOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	auto convertedRVal = rValue.ConvertAsNormalEquivalant(RRep, ConvertedRRep);
	PiToNormalOperation(convertedRVal.first, convertedLRep, convertedRVal.second);
}
#endif

#if defined(AltNum_EnablePiRep)
void EToNormalOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
#if defined(AltNum_EnableInfinityRep)
    if(RRep==RepType::InfinityRep){
    	if(IsPositive())
    		SetAsInfinity();
    	else
    		SetAsNegativeInfinity();
        return;
    }
#endif
	switch(LRep)
	{
		case RepType::ENum:{
		} break;
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnablePowerOfRepresentation)
		case RepType::EPower:{
		} break;
	#endif
	#if defined(AltNum_EnableFractionals)
		case RepType::ENumByDiv:{
		} break;
	#endif
	#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedE:{
		} break;
	#endif
#pragma endregion AltDecVariantExclusive
	#if defined(AltNum_EnableApproaching)
		case RepType::ApproachingBottomE:{
		} break;
		#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTopE:{
		} break;
		#endif
	#endif
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeftE:{
		} break;
		case RepType::ApproachingMidRightE:{
		} break;
	#endif
#pragma endregion AltDecVariantExclusive
	default:
		throw "Unsupported operation"; break;
	}
}

//Right side value converted to Normal Equivalant before performing operation
void CatchAllEOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	auto convertedRVal = rValue.ConvertAsNormalEquivalant(RRep, ConvertedRRep);
	EToNormalOperation(convertedRVal.first, convertedLRep, convertedRVal.second);
}
#endif

#if defined(AltNum_EnablePiRep)
void IToNormalOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
#if defined(AltNum_EnableInfinityRep)
    if(RRep==RepType::InfinityRep){
    	if(IsPositive())
    		SetAsImaginaryInfinity();
    	else
    		SetAsNegativeImaginaryInfinity();
        return;
    }
#endif
	switch(LRep)
	{
		case RepType::INum:{
			switch(RRep){
			}
		} break;
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnableFractionals)
		case RepType::INumByDiv:{
			switch(RRep){
			}
		} break;
	#endif
	#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedI:{
			switch(RRep){
			}
		} break;
	#endif
#pragma endregion AltDecVariantExclusive
	#if defined(AltNum_EnableApproaching)
		case RepType::ApproachingImaginaryBottom:{
			switch(RRep){
			}
		} break;
		#if !defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingImaginaryTop:{
			switch(RRep){
			}
		} break;
		#endif
	#endif
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingImaginaryMidLeft:{
			switch(RRep){
			}
		} break;
		case RepType::ApproachingImaginaryMidRight:{
			switch(RRep){
			}
		} break;
	#endif
#pragma endregion AltDecVariantExclusive
#if defined(AltNum_EnableImaginaryInfinity)
		case RepType::ImaginaryInfinity:
			break;
#endif
	default:
		throw "Unsupported operation"; break;
	}
}

//Right side value converted to Normal Equivalant before performing operation
void CatchAllIOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	auto convertedRVal = rValue.ConvertAsNormalEquivalant(RRep, ConvertedRRep);
	IToNormalOperation(convertedRVal.first, convertedLRep, convertedRVal.second);
}
#endif

#if defined(AltNum_EnablePiRep)
//PiRep_to_others
void PiRepSwitch(const auto& rValue)
{
	RepType LRep = GetPiRepType();
	switch(rValue.DecimalHalf.Flags)
	{
    #pragma region PiRep_to_PiRep
		case 1:{
			RepType RRep = rValue.GetPiRepType();
			if(LRep==RRep)
			{
				switch(LRep)
				{
					case RepType::PiNum:{
					} break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnablePowerOfRepresentation)
					case RepType::PiPower:{
					} break;
				#endif
				#if defined(AltNum_EnableFractionals)
					case RepType::PiNumByDiv:{
					} break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedPi:{
					} break;
				#endif
    #pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
					case RepType::ApproachingBottomPi:
						SameRep_ApproachingBottom(rValue); break;
					case RepType::ApproachingTopPi:
						SameRep_ApproachingTop(rValue); break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingMidLeftPi:
					case RepType::ApproachingMidRightPi:
						LeftSidePiOp(rValue, LRep, RRep);
						break;
				#endif
    #pragma endregion AltDecVariantExclusive
			#endif
            #if defined(AltNum_EnableUndefinedButInRange)
            #endif
				default:
					throw "Unsupported operation"; break;
				}
			}
			else
			{
				switch(LRep)
				{
					case RepType::PiNum:{
					} break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnablePowerOfRepresentation)
					case RepType::PiPower:{
					} break;
				#endif
				#if defined(AltNum_EnableFractionals)
					case RepType::PiNumByDiv:{
					} break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedPi:{
					} break;
				#endif
    #pragma endregion AltDecVariantExclusive
				#if defined(AltNum_EnableApproaching)
					case RepType::ApproachingBottomPi:{
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingTopPi:{
					} break;
					#endif
				#endif
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingMidLeftPi:{
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingMidRightPi:{
					} break;
					#endif
				#endif
    #pragma endregion AltDecVariantExclusive
				default:
					throw "Unsupported operation"; break;
				}
			}
		} break;
    #pragma endregion PiRep_to_PiRep
#if defined(AltNum_EnableERep)
		case 2:
			RightSideEOp(rValue, RRep); break;
#endif
#if defined(AltNum_EnableIRep)
		case 3:{
			RepType RRep = rValue.GetIRepType();
			switch(LRep)
			{
				case RepType::PiNum:{
				} break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::PiPower:{
				} break;
			#endif
			#if defined(AltNum_EnableFractionals)
				case RepType::PiNumByDiv:{
				} break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedPi:{
				} break;
			#endif
    #pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingBottomPi:{
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingTopPi:{
				} break;
				#endif
			#endif
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeftPi:{
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingMidRightPi:{
				} break;
				#endif
			#endif
    #pragma endregion AltDecVariantExclusive
			default:
				throw "Unsupported operation"; break;
			}
		} break;
#endif
    #pragma region PiRep_to_NormRep
		default:{
			RepType RRep = rValue.GetNormRepType();
			switch(LRep)
			{
				case RepType::PiNum:{
				} break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::PiPower:{
				} break;
			#endif
			#if defined(AltNum_EnableFractionals)
				case RepType::PiNumByDiv:{
				} break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedPi:{
				} break;
			#endif
    #pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingBottomPi:{
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingTopPi:{
				} break;
				#endif
			#endif
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeftPi:{
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingMidRightPi:{
				} break;
				#endif
			#endif
    #pragma endregion AltDecVariantExclusive
			default:
				throw "Unsupported operation"; break;
			}
		} break;
    #pragma endregion PiRep_to_NormRep
	}
}
#endif

#if defined(AltNum_EnableERep)
//ERep_to_others
void ERepSwitch(const auto& rValue)
{
	RepType LRep = GetERepType();
	switch(rValue.DecimalHalf.Flags)
	{
#if defined(AltNum_EnablePiRep)
		case 1:
			RightSidePiOp(rValue, RRep); break;
#endif
    #pragma region ERep_to_ERep
		case 2:
		{
			RepType RRep = rValue.GetERepType();
			if(LRep==RRep)
			{
				switch(LRep)
				{
					case RepType::ENum:{
					} break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnablePowerOfRepresentation)
					case RepType::EPower:{
					} break;
				#endif
				#if defined(AltNum_EnableFractionals)
					case RepType::ENumByDiv:{
					} break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedE:{
					} break;
				#endif
    #pragma endregion AltDecVariantExclusive
				#if defined(AltNum_EnableApproaching)
					case RepType::ApproachingBottomE:{
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingTopE:{
					} break;
					#endif
				#endif
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingMidLeftE:{
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingMidRightE:{
					} break;
					#endif
				#endif
    #pragma endregion AltDecVariantExclusive
				default:
					throw "Unsupported operation"; break;
				}
			}
			else
			{
				switch(LRep)
				{
					case RepType::ENum:{
						switch(RRep){
						}
					} break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnablePowerOfRepresentation)
					case RepType::EPower:{
						switch(RRep){
						}
					} break;
				#endif
				#if defined(AltNum_EnableFractionals)
					case RepType::ENumByDiv:{
						switch(RRep){
						}
					} break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedE:{
						switch(RRep){
						}
					} break;
				#endif
    #pragma endregion AltDecVariantExclusive
				#if defined(AltNum_EnableApproaching)
					case RepType::ApproachingBottomE:{
						switch(RRep){
						}
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingTopE:{
						switch(RRep){
						}
					} break;
					#endif
				#endif
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingMidLeftE:{
						switch(RRep){
						}
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingMidRightE:{
						switch(RRep){
						}
					} break;
					#endif
				#endif
    #pragma endregion AltDecVariantExclusive
				default:
					throw "Unsupported operation"; break;
				}
			}
		} break;
    #pragma endregion ERep_to_ERep
#if defined(AltNum_EnableIRep)
		case 3:{//ERep_to_IRep
			RepType RRep = rValue.GetIRepType();
			switch(LRep)
			{
				case RepType::ENum:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::EPower:{
					switch(RRep){
					}
				} break;
			#endif
			#if defined(AltNum_EnableFractionals)
				case RepType::ENumByDiv:{
					switch(RRep){
					}
				} break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedE:{
					switch(RRep){
					}
				} break;
			#endif
    #pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingBottomE:{
					switch(RRep){
					}
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingTopE:{
					switch(RRep){
					}
				} break;
				#endif
			#endif
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeftE:{
					switch(RRep){
					}
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingMidRightE:{
					switch(RRep){
					}
				} break;
				#endif
			#endif
    #pragma endregion AltDecVariantExclusive
			default:
				throw "Unsupported operation"; break;
			}
		} break;
#endif
		default:{//ERep_to_NormalRep
			RepType RRep = rValue.GetNormRepType();
			switch(LRep)
			{
				case RepType::ENum:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::EPower:{
					switch(RRep){
					}
				} break;
			#endif
			#if defined(AltNum_EnableFractionals)
				case RepType::ENumByDiv:{
					switch(RRep){
					}
				} break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedE:{
					switch(RRep){
					}
				} break;
			#endif
    #pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingBottomE:{
					switch(RRep){
					}
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingTopE:{
					switch(RRep){
					}
				} break;
				#endif
			#endif
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeftE:{
					switch(RRep){
					}
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingMidRightE:{
					switch(RRep){
					}
				} break;
				#endif
			#endif
    #pragma endregion AltDecVariantExclusive
			default:
				throw "Unsupported operation"; break;
			}
		} break;
	}
}
#endif

#if defined(AltNum_EnableIRep)
//IRep_to_others
void IRepSwitch(const auto& rValue)
{
	RepType LRep = GetIRepType();
	switch(rValue.DecimalHalf.Flags)
	{
#if defined(AltNum_EnablePiRep)
		case 1:
			RightSidePiOp(rValue, RRep); break;
#endif

#if defined(AltNum_EnableERep)
		case 2:
			RightSideEOp(rValue, RRep); break;
#endif

    #pragma region IRep_to_IRep
		case 3:
		{
			RepType RRep = rValue.GetIRepType();
			if(LRep==RRep)
			{
				switch(LRep)
				{
					case RepType::INum:{
					} break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableFractionals)
					case RepType::INumByDiv:{
					} break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedI:{
					} break;
				#endif
    #pragma endregion AltDecVariantExclusive
				#if defined(AltNum_EnableApproaching)
					case RepType::ApproachingImaginaryBottom:{
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingImaginaryTop:{
					} break;
					#endif
				#endif
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingImaginaryMidLeft:{
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingImaginaryMidRight:{
					} break;
					#endif
				#endif
    #pragma endregion AltDecVariantExclusive
#if defined(AltNum_EnableImaginaryInfinity)
					case RepType::ImaginaryInfinity:{
					} break;
#endif
				default:
					throw "Unsupported operation"; break;
				}
			}
			else
			{
				switch(LRep)
				{
					case RepType::INum:{
						switch(RRep){
						}
					} break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableFractionals)
					case RepType::INumByDiv:{
						switch(RRep){
						}
					} break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedI:{
						switch(RRep){
						}
					} break;
				#endif
    #pragma endregion AltDecVariantExclusive
				#if defined(AltNum_EnableApproaching)
					case RepType::ApproachingImaginaryBottom:{
						switch(RRep){
						}
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingImaginaryTop:{
						switch(RRep){
						}
					} break;
					#endif
				#endif
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingImaginaryMidLeft:{
						switch(RRep){
						}
					} break;
					#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingImaginaryMidRight:{
						switch(RRep){
						}
					} break;
					#endif
				#endif
    #pragma endregion AltDecVariantExclusive
#if defined(AltNum_EnableImaginaryInfinity)
					case RepType::ImaginaryInfinity:{
						switch(RRep){
						}
					} break;
#endif
				default:
					throw "Unsupported operation"; break;
				}
			}
		} break;
    #pragma endregion IRep_to_IRep

    #pragma region IRep_to_NormRep
		default:
		{
			RepType RRep = rValue.GetNormRepType();
			switch(LRep)
			{
				case RepType::INum:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
				case RepType::INumByDiv:{
					switch(RRep){
					}
				} break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedI:{
					switch(RRep){
					}
				} break;
			#endif
    #pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
				case RepType::ApproachingImaginaryBottom:{
					switch(RRep){
					}
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryTop:{
					switch(RRep){
					}
				} break;
				#endif
			#endif
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingImaginaryMidLeft:{
					switch(RRep){
					}
				} break;
				#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingImaginaryMidRight:{
					switch(RRep){
					}
				} break;
				#endif
			#endif
    #pragma endregion AltDecVariantExclusive
#if defined(AltNum_EnableImaginaryInfinity)
				case RepType::ImaginaryInfinity:{
					switch(RRep){
					}
				} break;
#endif
			default:
				throw "Unsupported operation"; break;
			}
		} break;
    #pragma region IRep_to_NormRep
	}
}
#endif

//NormalRep_to_others
void NormRepSwitch(const auto& rValue)
{
	RepType LRep = GetNormRepType();
	switch(rValue.DecimalHalf.Flags)
	{
#if defined(AltNum_EnablePiRep)
    #pragma region NormRep_to_PiRep
		case 1:
		{
			RepType RRep = rValue.GetPiRepType();
			switch(LRep)
			{
				case RepType::NormalType:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				case RepType::NumByDiv:{
					switch(RRep){
					}
				}; break;
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::ToPowerOf:{
					switch(RRep){
					}
				}; break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac:{
					switch(RRep){
					}
				}; break;
		#endif
    #pragma endregion AltDecVariantExclusive
				case RepType::ApproachingBottom:{
					switch(RRep){
					}
				} break;
				case RepType::ApproachingTop:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeft:{
					switch(RRep){
					}
				} break;
			#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingMidRight:{
					switch(RRep){
					}
				} break;
			#endif
		#endif
    #pragma endregion AltDecVariantExclusive
#if defined(AltNum_EnableInfinityRep)
				case RepType::InfinityRep:{
					switch(RRep){
					}
				} break;
#endif
				default:
					throw "Unsupported operation";
			}
		} break;
    #pragma endregion NormRep_to_PiRep
#endif

#if defined(AltNum_EnableERep)
    #pragma region NormRep_to_ERep
		case 2:
		{
			RepType RRep = rValue.GetERepType();
			switch(LRep)
			{
				case RepType::NormalType:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				case RepType::NumByDiv:{
					switch(RRep){
					}
				}; break;
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::ToPowerOf:{
					switch(RRep){
					}
				}; break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac:{
					switch(RRep){
					}
				}; break;
		#endif
    #pragma endregion AltDecVariantExclusive
				case RepType::ApproachingBottom:{
					switch(RRep){
					}
				} break;
				case RepType::ApproachingTop:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeft:{
					switch(RRep){
					}
				} break;
			#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingMidRight:{
					switch(RRep){
					}
				} break;
			#endif
		#endif
    #pragma endregion AltDecVariantExclusive
				case RepType::InfinityRep:{
					switch(RRep){
					}
				} break;
				default:
					throw "Unsupported operation";
			}
		} break;
    #pragma endregion NormRep_to_ERep
#endif

#if defined(AltNum_EnableIRep)
    #pragma region NormRep_to_IRep
		case 3:
		{
			RepType RRep = rValue.GetIRepType();
			switch(LRep)
			{
				case RepType::NormalType:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableFractionals)
				case RepType::NumByDiv:{
					switch(RRep){
					}
				}; break;
		#endif
		#if defined(AltNum_EnablePowerOfRepresentation)
				case RepType::ToPowerOf:{
					switch(RRep){
					}
				}; break;
		#endif
		#if defined(AltNum_EnableMixedFractional)
				case RepType::MixedFrac:{
					switch(RRep){
					}
				}; break;
		#endif
    #pragma endregion AltDecVariantExclusive
				case RepType::ApproachingBottom:{
					switch(RRep){
					}
				} break;
				case RepType::ApproachingTop:{
					switch(RRep){
					}
				} break;
    #pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
				case RepType::ApproachingMidLeft:{
					switch(RRep){
					}
				} break;
			#if !defined(AltNum_DisableApproachingTop)
				case RepType::ApproachingMidRight:{
					switch(RRep){
					}
				} break;
			#endif
		#endif
    #pragma endregion AltDecVariantExclusive
				case RepType::InfinityRep:{
					switch(RRep){
					}
				} break;
				default:
					throw "Unsupported operation";
			}
		} break;
    #pragma endregion NormRep_to_IRep
#endif

    #pragma region NormRep_to_NormRep
		default:
		{
			RepType RRep = rValue.GetNormRepType();
			if(LRep==RRep)
			{
				switch(LRep)
				{
					case RepType::NormalType:{
					}; break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					case RepType::NumByDiv:{
					}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					case RepType::ToPowerOf:{
					}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedFrac:{
					}; break;
			#endif
    #pragma endregion AltDecVariantExclusive
					case RepType::ApproachingBottom:{
					} break;
					case RepType::ApproachingTop:{
					} break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingMidLeft:{
					} break;
				#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingMidRight:{
					} break;
				#endif
			#endif
    #pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:{
					} break;
					default:
						throw "Unsupported operation";
				}
			}
			else
			{
				switch(LRep)
				{
					case RepType::NormalType:{
						switch(RRep){
						}
					} break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
					case RepType::NumByDiv:{
						switch(RRep){
						}
					}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
					case RepType::ToPowerOf:{
						switch(RRep){
						}
					}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedFrac:{
						switch(RRep){
						}
					}; break;
			#endif
    #pragma endregion AltDecVariantExclusive
					case RepType::ApproachingBottom:{
						switch(RRep){
						}
					} break;
					case RepType::ApproachingTop:{
						switch(RRep){
						}
					} break;
    #pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingMidLeft:{
						switch(RRep){
						}
					} break;
				#if !defined(AltNum_DisableApproachingTop)
					case RepType::ApproachingMidRight:{
						switch(RRep){
						}
					} break;
				#endif
			#endif
    #pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:{
						switch(RRep){
						}
					} break;
					default:
						throw "Unsupported operation";
				}
			}
		} break;
#pragma endregion NormRep_to_NormRep
	}
}

//UnsignedMultOp
auto& MediumDecVariant::UnsignedMultOp(const auto& rValue)
{
	#if defined(AltNum_EnableInfinityRep)
	if(DecimalHalf.Value==InfinityRep){
		if(rValue.IsZero())
		#if defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(ZeroTimesInfinityRep);
		#else
			throw "Can't multiply 0 by infinity";
		#endif
		return *this;
	} else if(rValue.DecimalHalf.Value==InfinityRep){
		if(IsZero())//Can not multiply 0 by infinity according to https://brilliant.org/wiki/is-infinity-times-zero-zero/
		#if defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(ZeroTimesInfinityRep);
		#else
			throw "Can't multiply 0 by infinity";
		#endif
		else
			SetAsInfinityVal();
		return *this;
	}
	else
	#endif
	switch(DecimalHalf.Flags)
	{
#if defined(AltNum_EnablePiRep)
		case 1:
			PiRepSwitch(rValue); break;
#endif
#if defined(AltNum_EnableERep)
		case 2:
			ERepSwitch(rValue); break;
#endif
#if defined(AltNum_EnableIRep)//IRep_to_others
		case 3:
            IRepSwitch(rValue); break;
#endif
		default:
			NormalRepSwitch(rValue); break;
	}
	return *this;
}
