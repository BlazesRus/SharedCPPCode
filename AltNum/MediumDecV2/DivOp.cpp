#include "MediumDecV2Base.hpp"
using MediumDecVariant = BlazesRusCode::MediumDecV2Base;
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
#pragma endregion AltDecVariantExclusive

void SameRep_ApproachingBottom(const auto& rValue, const RepType& LRep)
{
    if(IntHalf.Value==0)//0.0..01/2.0..01 = ~0.0..01
		return;
	else if(rValue.IntHalf.Value==0)//2.0..01/0.0..01 = infinity
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
	CatchAllDivisionV2(rValue, LRep);
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
#pragma endregion AltDecVariantExclusive
	#if defined(AltNum_EnableApproaching)
			case RepType::ApproachingBottom:
				SameRep_ApproachingBottom(rValue); break;
			case RepType::ApproachingTop:
				SameRep_ApproachingTop(rValue); break;
#pragma region AltDecVariantExclusive
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
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					#if !defined(AltNum_DisableApproachingTop)
					//case RepType::ApproachingTop:{
					//} break;
					#endif
		#pragma region AltDecVariantExclusive
		#pragma endregion AltDecVariantExclusive
					default:
						CatchAllOp(rValue, LRep, RRep);
				}
			} break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
			case RepType::ApproachingBottom:{
				switch(RRep){
					//case RepType::NormalType:{
					//}; break;
		#pragma region AltDecVariantExclusive
		#pragma endregion AltDecVariantExclusive
					//case RepType::ApproachingBottom:{
					//} break;
					#if !defined(AltNum_DisableApproachingTop)
					//case RepType::ApproachingTop:{
					//} break;
					#endif
		#pragma region AltDecVariantExclusive
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
        IntHalf.Value = 0; DecimalHalf = ApproachingBottomRep;
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
	#pragma endregion AltDecVariantExclusive
				//case RepType::ApproachingBottom:{
				//} break;
				//case RepType::ApproachingTop:{
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
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
	#if defined(AltNum_EnableApproaching)
		case RepType::ApproachingBottomPi:{
			switch(RRep){
				//case RepType::NormalType:{
				//}; break;
	#pragma region AltDecVariantExclusive
	#pragma endregion AltDecVariantExclusive
				//case RepType::ApproachingBottom:{
				//} break;
				//case RepType::ApproachingTop:{
				//} break;
	#pragma region AltDecVariantExclusive
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
	#pragma endregion AltDecVariantExclusive
				//case RepType::ApproachingBottom:{
				//} break;
				//case RepType::ApproachingTop:{
				//} break;
	#pragma region AltDecVariantExclusive
	#pragma endregion AltDecVariantExclusive
				default:
					CatchAllOp(rValue, LRep, RRep);
			}
		} break;
		#endif
	#endif
#pragma region AltDecVariantExclusive
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
        IntHalf.Value = 0; DecimalHalf = ApproachingBottomRep;
        ExtraRep = InitialExtraRep;
        return;
    }
#endif
	switch(LRep)
	{
		case RepType::ENum:{
		} break;
#pragma region AltDecVariantExclusive
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
        IntHalf.Value = 0; DecimalHalf.Value = ApproachingBottomRep;
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
		if(RRep^ToPowerOfFlag&&IntHalf==rValue.IntHalf&&DecimalHalf==rValue.DecimalHalf){//(1.5Pi^4)/(1.5Pi^2)=(1.5Pi^2)
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
			if(IntHalf.Value==1)
				SetAsZero();//Techically should maybe be indeterminate but normally can not have even larger infinity anyway
			else
				--IntHalf.Value;
		#elif defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(InfDividedByInfRep);
		#else
			throw "Can't divide infinity by infinity";
		#endif
		} else if(rValue.IsZero())
		#if defined(AltNum_EnableInfinityPowers)
			++IntHalf.Value;//Make into even larger infinity
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
		IntHalf.Value = 0;
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
	} else if(DecimalHalf<=NaNVariantThreshold&&IntHalf==rValue.IntHalf&&DecimalHalf.Value==rValue.DecimalHalf.Value&&ExtraRep==rValue.ExtraRep)
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

