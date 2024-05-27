#include "AltDecBase.hpp"
using MediumDecVariant = BlazesRusCode::AltDecBase;
using RepType = BlazesRusCode::RepType;

//Convert both left and right side down to normal representation and then perform operation
void CatchAllOp(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
    ConvertToNormType(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedAddOp(RValue);
}

//Convert right side down to normal representation and then perform operation
void RightSideOp(const auto& rValue, const RepType& RRep)
{
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedAddOp(RValue);
}

#pragma region AltDecVariantExclusive

#if defined(AltNum_EnablePiRep)

//Convert left and right side down to Pi representation
//and then perform operation
void PiOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	ConvertToPiRep(LRep);
	auto RValue = rValue.ConvertAsPiRep(RRep);
	BasicUnsignedAddOp(RValue);
}

#endif

#if defined(AltNum_EnableERep)

//Convert left and right side down to e representation
//and then perform operation
void EOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	ConvertToERep(LRep);
	auto RValue = rValue.ConvertAsERep(RRep);
	BasicUnsignedAddOp(RValue);
}

#endif

#if defined(AltNum_EnableIRep)

//Convert left and right side down to e representation
//and then perform operation
void IOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
	ConvertToIRep(LRep);
	auto RValue = rValue.ConvertAsIRep(RRep);
	BasicUnsignedAddOp(RValue);
}

#endif

void SameRep_NumByDiv(const auto& rValue, const RepType& LRep)
{
	if (ExtraRep == rValue.ExtraRep)
		BasicAddOp(rValue);
	else
		CatchAllAdditionV2(LRep);
}

void SameRep_PowerOf(const auto& rValue, const RepType& LRep)
{
	if(IntHalf==rValue.IntHalf&&DecimalHalf.Value==rValue.DecimalHalf.Value)
	{
	#if defined(AltNum_EnableNegativePowerRep)
		if(ExtraRep==rValue.ExtraRep)
	#else
		if(ExtraRep.Value==rValue.ExtraRep.Value)
	#endif
			SetAsOne();
		else
		{
	#if defined(AltNum_EnableNegativePowerRep)
			ExtraRep += rValue.ExtraRep;
			if(ExtraRep.IsZero())
				SetAsOne();
			else if(ExtraRep.IsOne())
				ExtraRep = InitialExtraRep;
	#else
			ExtraRep.Value += rValue.ExtraRep.Value;
			if(ExtraRep.IsZero())
				SetAsOne();
			else if(ExtraRep.IsAtOneInt())
				ExtraRep = InitialExtraRep;
	#endif
		}
	}
	else
		CatchAllAdditionV2(LRep);
}

void SameRep_MixedFrac(const auto& rValue, const RepType& LRep)
{
	if(ExtraRep.Value==rValue.ExtraRep.Value)
	{
		IntHalf += rValue.IntHalf;
		unsigned int result = DecimalHalf.Value+rValue.DecimalHalf.Value;
		if(result>ExtraRep.Value)
		{
			++IntHalf;
			DecimalHalf.Value = result - ExtraRep.Value;
		}
		else
			DecimalHalf.Value = result;
	}
	else
	{
		//Add code here later that normalizes the ExtraRep fields and then performs operation
		CatchAllAdditionV2(LRep);
	}
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
	if (IntHalf.Value == 0)
	{
		if(IsNegative())
		{
			if (rValue.IntHalf == 0)//-0.0..1 + 0.0..1 = 0
				SetAsZero();
			else//-0.0..1 + 5.0..1 = 5
			{
				DecimalHalf = 0;
				IntHalf = rValue.IntHalf;
			}
		}
		else if(rValue.IntHalf != 0)//0.0..1 + 5.0..1 = 5.0..1
		{
			IntHalf = rValue.IntHalf;
		}
	}
	else if (IsNegative())
	{
		if (rValue.IntHalf == 0)//-1.0..1 + 0.0..1  = -1
			DecimalHalf = 0;
		else if (IntHalf.Value == rValue.IntHalf.Value)//-1.01 + 1.01
			SetAsZero();
		else//-1.0..1 + 2.0..1 = 1
		{
			DecimalHalf = 0;
			IntHalf += rValue.IntHalf;
		}
	}
	else if((rValue.IntHalf != 0)//1.0..1 + 1.0..1
		IntHalf += rValue.IntHalf;
}

void SameRep_ApproachingTop(const auto& rValue)
{
	if (IntHalf.Value == 0)
	{
		if(IsNegative())
		{
			if (rValue.IntHalf == 0)//-0.9..9 + 0.9..9 = 0
				SetAsZero();
			else if (rValue.IntHalf == NegativeRep)//-0.9..9 - 0.9..9 = -1.9..8
				IntHalf = -1;
			else if (rValue.IntHalf < 0)//-0.9..9 - 1.9..9 = -2.9..8
			{
				IntHalf = rValue.IntHalf - 1;
			}
			else//-0.9..9 + 5.9..9 = 5
			{
				DecimalHalf = 0; ExtraRep = InitialExtraRep;
				IntHalf = rValue.IntHalf;
			}
		}
		else if (rValue.IntHalf == 0)//0.9..9 + 0.9..9 = 1.9..8
			IntHalf = 1;
		else//0.9..9 + 5.9..9 = 6.9..8
		{
			IntHalf = rValue.IntHalf + 1;
		}
	}
	else if (IsNegative())
	{
		if (rValue.IntHalf == 0)//-1.9..9 + 0.9..9  = -1
		{
			DecimalHalf = 0; ExtraRep = InitialExtraRep;
		}
		else if (IntHalf.Value == rValue.IntHalf.Value)//-1.9..9 + 1.9..9
			SetAsZero();
		else//-1.9..9 + 2.9..9
		{
			DecimalHalf = 0; ExtraRep = InitialExtraRep;
			IntHalf += rValue.IntHalf;
		}
	}
	else if (rValue.IntHalf == 0)//1.9..9 + 0.9..9 = 2.9..8
	{
		if (IntHalf == -1)
			IntHalf = NegativeRep;
		else
			++IntHalf;
	}
	else//1.9..9 + 1.9..9 = 3.9..8
		IntHalf += rValue.IntHalf + 1;
}

void NormalToNormalOperation(const auto& rValue, const RepType& LRep, const RepType& RRep)
{
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
//						case RepType::NormalType:{
//						}; break;
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
//						case RepType::NumByDiv:{
//						}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
//						case RepType::ToPowerOf:{
//						}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac:{
//						}; break;
			#endif
#pragma endregion AltDecVariantExclusive
//						case RepType::ApproachingBottomRep:{
//						} break;
				#if !defined(AltNum_DisableApproachingTop)
//						case RepType::ApproachingTopRep:{
//						} break;
				#endif
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingMidLeft:{
//						} break;
//						case RepType::ApproachingMidRight:{
//						} break;
			#endif
#pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:
						SetValue(rValue);
			#if defined(AltNum_EnableNaN)
					case RepType::Undefined:
					case RepType::NaN:
			#endif
			#if defined(AltNum_EnableNil)
					case RepType::Nil:
			#endif
			#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
						throw "Unsupported operation";
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
					case RepType::UndefinedButInRange:
						throw "Unsupported operation";
				#if defined(AltNum_EnableWithinMinMaxRange)
					case RepType::WithinMinMaxRange:
						throw "WithinMinMaxRange needs to be updated";
				#endif
			#endif
					default:
						CatchAllAddition(rValue, LRep, RRep); break;
				}
			} break;
#pragma region AltDecVariantExclusive
	#if defined(AltNum_EnableFractionals)
			case RepType::NumByDiv:{
				switch(RRep){
//						case RepType::NormalType:{
//						}; break;
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
//						case RepType::NumByDiv:{
//						}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
//						case RepType::ToPowerOf:{
//						}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac:{
//						}; break;
			#endif
#pragma endregion AltDecVariantExclusive
//						case RepType::ApproachingBottomRep:{
//						} break;
				#if !defined(AltNum_DisableApproachingTop)
//						case RepType::ApproachingTopRep:{
//						} break;
				#endif
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingMidLeft:{
//						} break;
//						case RepType::ApproachingMidRight:{
//						} break;
			#endif
#pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:
						SetValue(rValue);
			#if defined(AltNum_EnableNaN)
					case RepType::Undefined:
					case RepType::NaN:
			#endif
			#if defined(AltNum_EnableNil)
					case RepType::Nil:
			#endif
			#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
						throw "Unsupported operation";
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
					case RepType::UndefinedButInRange:
						throw "Unsupported operation";
				#if defined(AltNum_EnableWithinMinMaxRange)
					case RepType::WithinMinMaxRange:
						throw "WithinMinMaxRange needs to be updated";
				#endif
			#endif
					default:
						CatchAllAddition(rValue, LRep, RRep); break;
				}
			}; break;
	#endif
	#if defined(AltNum_EnablePowerOfRepresentation)
			case RepType::ToPowerOf:{
				switch(RRep){
//						case RepType::NormalType:{
//						}; break;
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
//						case RepType::NumByDiv:{
//						}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
//						case RepType::ToPowerOf:{
//						}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac:{
//						}; break;
			#endif
#pragma endregion AltDecVariantExclusive
//						case RepType::ApproachingBottomRep:{
//						} break;
				#if !defined(AltNum_DisableApproachingTop)
//						case RepType::ApproachingTopRep:{
//						} break;
				#endif
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingMidLeft:{
//						} break;
//						case RepType::ApproachingMidRight:{
//						} break;
			#endif
#pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:
						SetValue(rValue);
			#if defined(AltNum_EnableNaN)
					case RepType::Undefined:
					case RepType::NaN:
			#endif
			#if defined(AltNum_EnableNil)
					case RepType::Nil:
			#endif
			#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
						throw "Unsupported operation";
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
					case RepType::UndefinedButInRange:
						throw "Unsupported operation";
				#if defined(AltNum_EnableWithinMinMaxRange)
					case RepType::WithinMinMaxRange:
						throw "WithinMinMaxRange needs to be updated";
				#endif
			#endif
					default:
						CatchAllAddition(rValue, LRep, RRep); break;
				}
			}; break;
	#endif
	#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac:{
				switch(RRep){
//						case RepType::NormalType:{
//						}; break;
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
//						case RepType::NumByDiv:{
//						}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
//						case RepType::ToPowerOf:{
//						}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac:{
//						}; break;
			#endif
#pragma endregion AltDecVariantExclusive
//						case RepType::ApproachingBottomRep:{
//						} break;
				#if !defined(AltNum_DisableApproachingTop)
//						case RepType::ApproachingTopRep:{
//						} break;
				#endif
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingMidLeft:{
//						} break;
//						case RepType::ApproachingMidRight:{
//						} break;
			#endif
#pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:
						SetValue(rValue);
			#if defined(AltNum_EnableNaN)
					case RepType::Undefined:
					case RepType::NaN:
			#endif
			#if defined(AltNum_EnableNil)
					case RepType::Nil:
			#endif
			#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
						throw "Unsupported operation";
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
					case RepType::UndefinedButInRange:
						throw "Unsupported operation";
				#if defined(AltNum_EnableWithinMinMaxRange)
					case RepType::WithinMinMaxRange:
						throw "WithinMinMaxRange needs to be updated";
				#endif
			#endif
					default:
						CatchAllAddition(rValue, LRep, RRep); break;
				}
			}; break;
	#endif
#pragma endregion AltDecVariantExclusive
	#if defined(AltNum_EnableApproaching)
			case RepType::ApproachingBottomRep:{
				switch(RRep){
//						case RepType::NormalType:{
//						}; break;
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
//						case RepType::NumByDiv:{
//						}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
//						case RepType::ToPowerOf:{
//						}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac:{
//						}; break;
			#endif
#pragma endregion AltDecVariantExclusive
//						case RepType::ApproachingBottomRep:{
//						} break;
				#if !defined(AltNum_DisableApproachingTop)
//						case RepType::ApproachingTopRep:{
//						} break;
				#endif
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingMidLeft:{
//						} break;
//						case RepType::ApproachingMidRight:{
//						} break;
			#endif
#pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:
						SetValue(rValue);
			#if defined(AltNum_EnableNaN)
					case RepType::Undefined:
					case RepType::NaN:
			#endif
			#if defined(AltNum_EnableNil)
					case RepType::Nil:
			#endif
			#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
						throw "Unsupported operation";
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
					case RepType::UndefinedButInRange:
						throw "Unsupported operation";
				#if defined(AltNum_EnableWithinMinMaxRange)
					case RepType::WithinMinMaxRange:
						throw "WithinMinMaxRange needs to be updated";
				#endif
			#endif
					default:
						CatchAllAddition(rValue, LRep, RRep); break;
				}
			} break;
			case RepType::ApproachingTopRep:{
				switch(RRep){
//						case RepType::NormalType:{
//						}; break;
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
//						case RepType::NumByDiv:{
//						}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
//						case RepType::ToPowerOf:{
//						}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac:{
//						}; break;
			#endif
#pragma endregion AltDecVariantExclusive
//						case RepType::ApproachingBottomRep:{
//						} break;
				#if !defined(AltNum_DisableApproachingTop)
//						case RepType::ApproachingTopRep:{
//						} break;
				#endif
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingMidLeft:{
//						} break;
//						case RepType::ApproachingMidRight:{
//						} break;
			#endif
#pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:
						SetValue(rValue);
			#if defined(AltNum_EnableNaN)
					case RepType::Undefined:
					case RepType::NaN:
			#endif
			#if defined(AltNum_EnableNil)
					case RepType::Nil:
			#endif
			#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
						throw "Unsupported operation";
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
					case RepType::UndefinedButInRange:
						throw "Unsupported operation";
				#if defined(AltNum_EnableWithinMinMaxRange)
					case RepType::WithinMinMaxRange:
						throw "WithinMinMaxRange needs to be updated";
				#endif
			#endif
					default:
						CatchAllAddition(rValue, LRep, RRep); break;
				}
			} break;
	#endif
#pragma region AltDecVariantExclusive
		#if defined(AltNum_EnableApproachingDivided)
			case RepType::ApproachingMidLeft:{
				switch(RRep){
//						case RepType::NormalType:{
//						}; break;
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
//						case RepType::NumByDiv:{
//						}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
//						case RepType::ToPowerOf:{
//						}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac:{
//						}; break;
			#endif
#pragma endregion AltDecVariantExclusive
//						case RepType::ApproachingBottomRep:{
//						} break;
				#if !defined(AltNum_DisableApproachingTop)
//						case RepType::ApproachingTopRep:{
//						} break;
				#endif
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingMidLeft:{
//						} break;
//						case RepType::ApproachingMidRight:{
//						} break;
			#endif
#pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:
						SetValue(rValue);
			#if defined(AltNum_EnableNaN)
					case RepType::Undefined:
					case RepType::NaN:
			#endif
			#if defined(AltNum_EnableNil)
					case RepType::Nil:
			#endif
			#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
						throw "Unsupported operation";
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
					case RepType::UndefinedButInRange:
						throw "Unsupported operation";
				#if defined(AltNum_EnableWithinMinMaxRange)
					case RepType::WithinMinMaxRange:
						throw "WithinMinMaxRange needs to be updated";
				#endif
			#endif
					default:
						CatchAllAddition(rValue, LRep, RRep); break;
				}
			} break;
			case RepType::ApproachingMidRight:{
				switch(RRep){
//						case RepType::NormalType:{
//						}; break;
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableFractionals)
//						case RepType::NumByDiv:{
//						}; break;
			#endif
			#if defined(AltNum_EnablePowerOfRepresentation)
//						case RepType::ToPowerOf:{
//						}; break;
			#endif
			#if defined(AltNum_EnableMixedFractional)
//						case RepType::MixedFrac:{
//						}; break;
			#endif
#pragma endregion AltDecVariantExclusive
//						case RepType::ApproachingBottomRep:{
//						} break;
				#if !defined(AltNum_DisableApproachingTop)
//						case RepType::ApproachingTopRep:{
//						} break;
				#endif
#pragma region AltDecVariantExclusive
			#if defined(AltNum_EnableApproachingDivided)
//						case RepType::ApproachingMidLeft:{
//						} break;
//						case RepType::ApproachingMidRight:{
//						} break;
			#endif
#pragma endregion AltDecVariantExclusive
					case RepType::InfinityRep:
						SetValue(rValue);
			#if defined(AltNum_EnableNaN)
					case RepType::Undefined:
					case RepType::NaN:
			#endif
			#if defined(AltNum_EnableNil)
					case RepType::Nil:
			#endif
			#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
						throw "Unsupported operation";
			#endif
			#if defined(AltNum_EnableUndefinedButInRange)
					case RepType::UndefinedButInRange:
						throw "Unsupported operation";
				#if defined(AltNum_EnableWithinMinMaxRange)
					case RepType::WithinMinMaxRange:
						throw "WithinMinMaxRange needs to be updated";
				#endif
			#endif
					default:
						CatchAllAddition(rValue, LRep, RRep); break;
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
		case 2:{
			RepType RRep = rValue.GetERepType();
			CatchAllOp(rValue, LRep, RRep); 
		} break;
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
			CatchAllOp(rValue, LRep, RRep);
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
		case 1:{
			RepType RRep = rValue.GetPiRepType();
			CatchAllOp(rValue, LRep, RRep); 
		} break;
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
			CatchAllOp(rValue, LRep, RRep);
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
			if(rValue.IsNotZero())
				throw "Complex number operations not enabled yet.";
			break;
#endif

#if defined(AltNum_EnableERep)
		case 2:
			if(rValue.IsNotZero())
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
            #endif
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
			if(rValue.IsNotZero())
				throw "Complex number operations not enabled yet.";
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
			RepType RRep = rValue.GetERepType();
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
			NormalToNormalOperation(rValue, LRep, RRep);
		} break;
#pragma endregion NormRep_to_NormRep
	}
}

//UnsignedAddOp
auto& MediumDecVariant::UnsignedAddOp(const auto& rValue)
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
