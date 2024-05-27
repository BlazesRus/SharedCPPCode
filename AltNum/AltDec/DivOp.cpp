#include "AltDecBase.hpp"
using AltDecBase = BlazesRusCode::AltDecBase;
using RepType = BlazesRusCode::RepType;

void CatchAllOp(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
    ConvertToNormType(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedDivOp(RValue);
}

void RightSideOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedDivOp(RValue);
}

#if defined(AltNum_EnablePiRep)

//Convert right side down to Pi representation and then perform operation
void RightSidePiOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertToPiRep(RRep);
	BasicUnsignedDivOp(RValue);
    BasicUnsignedDivOp(PiNum);
}

//Convert left side down to Pi representation and right side down to normal
//and then perform operation
void LeftSidePiOp(const auto& rValue, , const RepType& LRep, const RepType& RRep)
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
	BasicUnsignedDivOp(RValue);
    BasicUnsignedDivOp(ENum);
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
	BasicUnsignedDivOp(RValue);
    SwapNegativeStatus();
    DecimalHalf.Flags = 3;
}

//Convert left side down to i representation and right side down to normal
//and then perform operation
void LeftSideIOp(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	ConvertToIRep(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedDivOp(RValue);
}
#endif

#pragma region AltDecVariantExclusive

void SameRep_NumByDiv(const auto& rValue, const RepType& LRep)
{
/*	//((AltDecBase(IntValue,DecimalHalf))/ExtraRep) / (AltDecBase(rValue.IntValue,rValue.DecimalHalf))/rValue.ExtraRep) = 
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
*/    
	//Add Code Here
}

void SameRep_PowerOf(const auto& rValue, const RepType& LRep)
{
	if(IntValue==rValue.IntValue&&DecimalHalf==rValue.DecimalHalf){//(1.5Pi^4)/(1.5Pi^2)=(1.5Pi^2)
#if defined(AltNum_EnableNegativePowerRep)
		ExtraRep -= rValue.ExtraRep;
#else
		if(ExtraRep.Value>rValue.ExtraRep)
			ExtraRep.Value -= rValue.ExtraRep;
		else {//Result is negative Exponent
			signed int Exp = (signed int)ExtraRep.Value - (signed int)rValue.ExtraRep.Value;
			ResetDivisor();
			BasicIntPowOpV1(Exp);
		}
#endif
	}
	else
		CatchAllOp(rValue, LRep, LRep);
}

void SameRep_MixedFrac(const auto& rValue, const RepType& LRep)
{
/*	int rvDivisor = -rValue.ExtraRep;
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
	}*/
    //Add Code Here
}

#if defined(AltNum_EnableWithinMinMaxRange)
void SameRep_WithinMinMaxRange
{
	throw "WithinMinMaxRange code not adjusted yet to changes in code.";
}
#endif
#pragma endregion AltDecVariantExclusive

void SameRep_ApproachingBottom(const auto& rValue, const RepType& LRep)
{
    if(IntValue.Value==0)//0.0..01/2.0..01 = ~0.0..01
		return;
	else if(rValue.IntValue.Value==0)//2.0..01/0.0..01 = infinity
#if defined(AltNum_EnableInfinity)
		SetAsInfinityVal();
#else
		CatchAllDivisionV2(rValue, LRep);
#endif
	else
		CatchAllDivisionV2(rValue, LRep);
}

void SameRep_ApproachingTop(const auto& rValue, const RepType& LRep)
{
#if defined(AltNum_EnableApproachingDivided)
	//0.9..9/2.9..9 = ~0.9..9/3 (not quite that but close enough)
    if(IntValue.Value==0){
		DecimalHalf.Value = ApproachingMidLeftRep;
		ExtraRep = rValue.IntValue+1;
	} else if(rValue.IntValue.Value==0){
		DecimalHalf.Value = ApproachingBottomRep;
		++IntValue.Value;
	}
	//999999999999999.999999999999999/0.999 999 999 999 999 = 1000000000000001
	//9.999 999 999 999 999/0.999 999 999 999 999 = 10.000000000000009000000000000009000000000000009000000000000009000000000000009000000000000009
	//5.99999999999999999999999999999999999999999999/0.99999999999999999999999999999999999999999999 = 6.0000000000000000000000000000000000000000000500000000000000000000000000000000000000000005
	else
		CatchAllDivisionV2(rValue, LRep);
#else
	CatchAllDivisionV2(rValue, LRep);
#endif
}

void NormalToNormalOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	if(LRep==RRep)
	{
		switch(LRep)
		{
			case RepType::NormalType:
				BasicDivOp(rValue); break;
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
			case RepType::ApproachingMidLeft:
                if(rValue.IntValue.Value==0)
                {
                    if(IntValue.Value==0)
				    {
					    //0.249..9(ExtraRep:4) / 0.49..9(ExtraRep:2) = ~0.49..9(ExtraRep:2)
                        unsigned int result = ExtraRep.Value / rValue.ExtraRep;
                        if (ExtraRep.Value == result * rValue)//checking for truncation
						    ExtraRep.Value = result;
					    else
						    CatchAllOp(rValue);
                    } else {
                        //2.249..9(ExtraRep:4) / 0.49..9(ExtraRep:2) = ~4.49..9(ExtraRep:2)
                        unsigned int result = ExtraRep.Value / rValue.ExtraRep;
                        if (ExtraRep.Value == result * rValue)//checking for truncation
						{
                            ExtraRep.Value = result;
                            IntValue.Value *= rValue.ExtraRep;
                        } else
						    CatchAllOp(rValue);
                    }
				}
                else
					CatchAllOp(rValue);
				break;
			case RepType::ApproachingMidRight:
                if(rValue.IntValue.Value==0)
                {
                    if(IntValue.Value==0)
				    {
                        unsigned int result = ExtraRep.Value / rValue.ExtraRep;
                        if (ExtraRep.Value == result * rValue)//checking for truncation
						    ExtraRep.Value = result;
					    else
						    CatchAllOp(rValue);
                    } else {
                        unsigned int result = ExtraRep.Value / rValue.ExtraRep;
                        if (ExtraRep.Value == result * rValue)//checking for truncation
						{
                            ExtraRep.Value = result;
                            IntValue.Value *= rValue.ExtraRep;
                        } else
						    CatchAllOp(rValue);
                    }
				}
                else
					CatchAllOp(rValue);
				break;
		#endif
#pragma endregion AltDecVariantExclusive
	#endif
			case RepType::InfinityRep:
				break;//Techically should return indeterminate form
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
        #if defined(AltNum_EnableInfinityRep)&&defined(AltNum_EnableApproaching)
            if(RRep==RepType::InfinityRep){
                SetAsApproachingBottomVal();
                return;
            }
        #endif
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

//Both sides have separate Flags
void CatchAllOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	RepType convertedLRep = ConvertToNormalEquivalant(LRep, ConvertedLRep);
	auto convertedRVal = rValue.ConvertAsNormalEquivalant(RRep, ConvertedRRep);
	NormalToNormalOperation(convertedRVal.first, convertedLRep, convertedRVal.second);
}

#if defined(AltNum_EnablePiRep)
void PiToNormalOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
#if defined(AltNum_EnableInfinityRep)&&defined(AltNum_EnableApproaching)
    if(RRep==RepType::InfinityRep){
        IntValue.Value = 0; DecimalHalf = ApproachingBottomRep;
        ExtraRep = InitialExtraRep;
        return;
    }
#endif
	switch(LRep)
	{
		case RepType::PiNum:{
			switch(RRep){
				case RepType::NormalType:
					BasicUnsignedDivOp(rValue); break;
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
				case RepType::NormalType:
					if(rValue.DecimalHalf.Value==0){
					
					}
					else
						BasicUnsignedDivOp(rValue);
					break;
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
					if(rValue.DecimalHalf==0)
                    {
                        try{ ExtraRep *= rValue.IntValue; }
                        catch (std::overflow_error& e){ LeftSidePiOp(rValue); }
                    }
					else
						LeftSidePiOp(rValue);
				}; break;
				case RepType::NumByDiv:{
					LeftSidePiOp(rValue);
				}; break;
				case RepType::ApproachingMidLeft:
                    if(rValue.IntValue.Value==0)
                    {
                        if(IntValue.Value==0)
    				    {
                            unsigned int result = ExtraRep.Value / rValue.ExtraRep;
                            if (ExtraRep.Value == result * rValue)//checking for truncation
    						    ExtraRep.Value = result;
    					    else
    						    LeftSidePiOp(rValue);
                        } else {
                            unsigned int result = ExtraRep.Value / rValue.ExtraRep;
                            if (ExtraRep.Value == result * rValue)//checking for truncation
    						{
                                ExtraRep.Value = result;
                                IntValue.Value *= rValue.ExtraRep;
                            } else
    						    LeftSidePiOp(rValue);
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
					if(rValue.DecimalHalf==0)
						ExtraRep *= rValue.IntValue
					else
						LeftSidePiOp(rValue);
				}; break;
				case RepType::NumByDiv:{
					LeftSidePiOp(rValue);
				}; break;
				case RepType::ApproachingMidRight:
                    if(rValue.IntValue.Value==0)
                    {
                        if(IntValue.Value==0)
    				    {
                            unsigned int result = ExtraRep.Value / rValue.ExtraRep;
                            if (ExtraRep.Value == result * rValue)//checking for truncation
    						    ExtraRep.Value = result;
    					    else
    						    LeftSidePiOp(rValue);
                        } else {
                            unsigned int result = ExtraRep.Value / rValue.ExtraRep;
                            if (ExtraRep.Value == result * rValue)//checking for truncation
    						{
                                ExtraRep.Value = result;
                                IntValue.Value *= rValue.ExtraRep;
                            } else
    						    LeftSidePiOp(rValue);
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
#if defined(AltNum_EnableInfinityRep)&&defined(AltNum_EnableApproaching)
    if(RRep==RepType::InfinityRep){
        IntValue.Value = 0; DecimalHalf = ApproachingBottomRep;
        ExtraRep = InitialExtraRep;
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
#if defined(AltNum_EnableInfinityRep)&&defined(AltNum_EnableApproaching)
    if(RRep==RepType::InfinityRep){
        IntValue.Value = 0; DecimalHalf.Value = ApproachingBottomRep;
        ExtraRep = InitialExtraRep;
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

//Same Representation division
void CatchAllAltOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
#if defined(AltNum_EnablePowerOfRepresentation)
	if(LRep^ToPowerOfFlag){
		if(RRep^ToPowerOfFlag&&IntValue==rValue.IntValue&&DecimalHalf==rValue.DecimalHalf){//(1.5Pi^4)/(1.5Pi^2)=(1.5Pi^2)
	#if defined(AltNum_EnableNegativePowerRep)
			ExtraRep -= rValue.ExtraRep;
	#else
			if(ExtraRep.Value>rValue.ExtraRep)
				ExtraRep.Value -= rValue.ExtraRep;
			else {//Result is negative Exponent
				signed int Exp = (signed int)ExtraRep.Value - (signed int)rValue.ExtraRep.Value;
				ResetDivisor();
				BasicIntPowOpV1(Exp);
			}
	#endif
		} else {
			auto convertedLVal = ConvertToNormalEquivalant(LRep);
			auto convertedRVal = rValue.ConvertAsNormalEquivalant(RRep);
			NormalToNormalOperation(convertedRVal.first, convertedLRep, convertedRVal.second);
		}
		return;
	} else(RRep^ToPowerOfFlag){
		auto convertedLVal = ConvertToNormalEquivalant(LRep);
		auto convertedRVal = rValue.ConvertAsNormalEquivalant(RRep);
		NormalToNormalOperation(convertedRVal.first, convertedLRep, convertedRVal.second);
		return;
	}
#endif
	RepType convertedLRep = GetRepAsNormalEquivalent(LRep);
	RepType convertedRVal = GetRepAsNormalEquivalent(RRep);
	DecimalHalf.Flags = 0;//Dividing by self removes flag
	NormalToNormalOperation(rValue, convertedLRep, convertedRVal);//Ignoring flags during division
}

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
				CatchAllAltOperation(rValue, LRep, RRep);
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
					case RepType::ApproachingMidRightPi:{
					} break;
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
				case RepType::ApproachingMidRightPi:{
				} break;
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
			PiToNormalOperation(rValue, LRep, RRep);
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
				CatchAllAltOperation(rValue, LRep, RRep);
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
				case RepType::ApproachingMidRightE:{
					switch(RRep){
					}
				} break;
			#endif
    #pragma endregion AltDecVariantExclusive
			default:
				throw "Unsupported operation"; break;
			}
		} break;
#endif
		default:{//ERep_to_NormalRep
			RepType RRep = rValue.GetNormRepType();
			EToNormalOperation(rValue, LRep, RRep);
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
				CatchAllAltOperation(rValue, LRep, RRep);
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
		} break;
    #pragma endregion IRep_to_IRep

    #pragma region IRep_to_NormRep
		default:
		{
			RepType RRep = rValue.GetNormRepType();
			IToNormalOperation(rValue, LRep, RRep);
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
			auto convertedVal = rValue.ConvertAsNormalEquivalant(RRep, ConvertedRRep);
			NormalToNormalOperation(convertedVal.first, LRep, convertedVal.second);
		} break;
    #pragma endregion NormRep_to_PiRep
#endif

#if defined(AltNum_EnableERep)
    #pragma region NormRep_to_ERep
		case 2:
		{
			RepType RRep = rValue.GetERepType();
			auto convertedVal = rValue.ConvertAsNormalEquivalant(RRep, ConvertedRRep);
			NormalToNormalOperation(convertedVal.first, LRep, convertedVal.second);
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
				case RepType::ApproachingMidRight:{
					switch(RRep){
					}
				} break;
		#endif
    #pragma endregion AltDecVariantExclusive
				case RepType::InfinityRep:
					SwapNegativeStatus();
					DecimalHalf.Flags = 3;
					break;
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
			NormalToNormalOperation(rValue, LRep, RRep);
		} break;
#pragma endregion NormRep_to_NormRep
	}
}

//UnsignedDivOp
auto& MediumDecVariant::UnsignedDivOp(const auto& rValue)
{
	#if defined(AltNum_EnableInfinityRep)
	if(DecimalHalf.Value==InfinityRep){
		if(rValue.DecimalHalf.Value==InfinityRep)
		{
		#if defined(AltNum_EnableIRep)
			if(rValue.DecimalHalf.Flags==3) {//Divided by Imaginary Infinity
				if(DecimalHalf.Flags==3)
					DecimalHalf.Flags = 0
				else {
					SwapNegativeStatus();
					DecimalHalf.Flags = 3;
				}
			} 
		#endif
		#if defined(AltNum_EnableInfinityPowers)
			if(IntValue.Value==1)
				SetAsZero();//Techically should maybe be indeterminate but normally can not have even larger infinity anyway
			else
				--IntValue.Value;
		#elif defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(InfDividedByInfRep);
		#else
			throw "Can't divide infinity by infinity";
		#endif
		} else if(rValue.IsZero())
		#if defined(AltNum_EnableInfinityPowers)
			++IntValue.Value;//Make into even larger infinity
		#elif defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(InfDividedByZeroRep);
		#elif defined(AltNum_DefineDivideByZeroAsInfinity)
			return *this;
		#elif defined(AltNum_EnableNaN)
            DecimalHalf = NaNRep;
		#else
			throw "Divide by zero is not allowed with current toggles";
		#endif
		return *this;
	} else if(rValue.DecimalHalf.Value==InfinityRep){//Divided by Infinity
        DecimalHalf = ApproachingBottomRep;
		IntValue.Value = 0;
        ExtraRep = InitialExtraRep;
		if(rValue.DecimalHalf.Flags==3){//Divided by Imaginary infinity
			SwapNegativeStatus();
			DecimalHalf.Flags = 3;
		}
		return *this;
	}
	#endif
	if(IsZero()){
		if(rValue.IsZero())
		#if defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(DividedByZeroRep);
		#else
			throw "Divide by zero is not allowed with current toggles";
		#endif
		else
			return *this;
	} else if(rValue.IsZero()){
		#if defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(DividedByZeroRep);
		#else
			throw "Divide by zero is not allowed with current toggles";
		#endif
	} else if(DecimalHalf<=NaNVariantThreshold&&IntValue==rValue.IntValue&&DecimalHalf.Value==rValue.DecimalHalf.Value&&ExtraRep==rValue.ExtraRep)
		SetAsOneVal();
	else {
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
	}
	return *this;
}
