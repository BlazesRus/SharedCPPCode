#include "AltDecBase.hpp"
using MediumDecVariant = BlazesRusCode::AltDecBase;
using RepType = BlazesRusCode::RepType;

void CatchAllOp(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
    ConvertToNormType(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsigned___Op(RValue);
}

void RightSideOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsigned___Op(RValue);
}

#pragma region AltDecVariantExclusive

void RightSidePiOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertToPiRep(RRep);
	BasicUnsigned___Op(RValue);
    BasicUnsigned___Op(PiNum);
}

void RightSideEOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertToERep(RRep);
	BasicUnsigned___Op(RValue);
    BasicUnsigned___Op(ENum);
}

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
					case RepType::PiNum:
						BasicAddOp(rValue); break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnablePowerOfRepresentation)
					case RepType::PiPower:
						SameRep_PowerOf(rValue, LRep); break;
				#endif
				#if defined(AltNum_EnableFractionals)
					case RepType::PiNumByDiv:
						SameRep_NumByDiv(rValue, LRep); break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedPi:
						SameRep_MixedFrac(rValue, LRep); break;
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
						CatchAllAdditionV2(rValue, LRep);
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
		case 3:
			if(rValue.IsNotZero())
				throw "Complex number operations not enabled yet.";
			break;
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
				case RepType::ApproachingMidRightPi:{
				} break;
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
					case RepType::ENum:
						BasicAddOp(rValue); break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnablePowerOfRepresentation)
					case RepType::EPower:
						SameRep_PowerOf(rValue, LRep); break;
				#endif
				#if defined(AltNum_EnableFractionals)
					case RepType::ENumByDiv:
						SameRep_NumByDiv(rValue, LRep); break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedE:
						SameRep_MixedFrac(rValue, LRep); break;
				#endif
    #pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
					case RepType::ApproachingBottomE:
						SameRep_ApproachingBottom(rValue); break;
					case RepType::ApproachingTopE:
						SameRep_ApproachingTop(rValue); break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingMidLeftE:
					case RepType::ApproachingMidRightE:
						CatchAllAdditionV2(rValue, LRep);
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
			}
		} break;
    #pragma endregion ERep_to_ERep

#if defined(AltNum_EnableIRep)
		case 3://ERep_to_IRep
			if(rValue.IsNotZero())
				throw "Complex number operations not enabled yet.";
			break;
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
			throw "Complex number operations not enabled yet.";
			break;
#endif

#if defined(AltNum_EnableERep)
		case 2:
			throw "Complex number operations not enabled yet.";
			break;
#endif

    #pragma region IRep_to_IRep
		case 3:
		{
			RepType RRep = rValue.GetIRepType();
			if(LRep==RRep)
			{
				switch(LRep)
				{
					case RepType::INum:
						BasicAddOp(rValue); break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableFractionals)
					case RepType::INumByDiv:
						SameRep_NumByDiv(rValue, LRep); break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
					case RepType::MixedI:
						SameRep_MixedFrac(rValue, LRep); break;
				#endif
    #pragma endregion AltDecVariantExclusive
			#if defined(AltNum_EnableApproaching)
					case RepType::ApproachingImaginaryBottom:
						SameRep_ApproachingBottom(rValue); break;
					case RepType::ApproachingImaginaryTop:
						SameRep_ApproachingTop(rValue); break;
    #pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
					case RepType::ApproachingImaginaryMidLeft:
					case RepType::ApproachingImaginaryMidRight:
						CatchAllAdditionV2(rValue, LRep);
						break;
				#endif
    #pragma endregion AltDecVariantExclusive
			#endif
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
					case RepType::ApproachingImaginaryMidRight:{
						switch(RRep){
						}
					} break;
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
				case RepType::ApproachingImaginaryMidRight:{
					switch(RRep){
					}
				} break;
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
				case RepType::ApproachingBottomRep:{
					switch(RRep){
					}
				} break;
				case RepType::ApproachingTopRep:{
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
				case RepType::ApproachingBottomRep:{
					switch(RRep){
					}
				} break;
				case RepType::ApproachingTopRep:{
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
			if(IsZero())
				SetAsValue(rValue);
			else
				throw "Complex number operations not enabled yet.";
			break;
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
					case RepType::NormalType:
						BasicAddOp(rValue); break;
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
					case RepType::ApproachingMidRight:
						CatchAllAdditionV2(rValue, LRep);
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
					case RepType::ApproachingBottomRep:{
						switch(RRep){
						}
					} break;
					case RepType::ApproachingTopRep:{
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
					    break;
					default:
						throw "Unsupported operation";
				}
			}
		} break;
#pragma endregion NormRep_to_NormRep
	}
}

//UnsignedAddOp or UnsignedSubOp
auto& MediumDecVariant::Unsigned___Op(const auto& rValue)
{
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
