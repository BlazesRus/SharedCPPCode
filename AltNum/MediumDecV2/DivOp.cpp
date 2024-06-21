#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;
using RepTypeEnum = BlazesRusCode::RepTypeEnum;

static void CatchAllOp(MediumDecV2& lValue, const MediumDecV2& rValue, const RepType& LRep, const RepType& RRep)
{
    lValue.ConvertToNormType(LRep.ConvertFromNormalRep(lValue.GetFlags()));
	auto RValue = rValue.ConvertAsNormType(RRep.ConvertFromNormalRep(rValue.GetFlags()));
	lValue.BasicUnsignedDivOp(RValue);
}

static void CatchAllOpV2(MediumDecV2& lValue, const MediumDecV2& rValue, const RepType& LRep, const RepType& RRep)
{
    lValue.ConvertToNormType(LRep.ConvertFromNormalRep(lValue.GetFlags()));
	auto RValue = rValue.ConvertAsNormType(RRep);
	lValue.BasicUnsignedDivOp(RValue);
}

static void SameRepCatchAll(MediumDecV2& lValue, const MediumDecV2& rValue, const RepType& LRep)
{
    lValue.ConvertToNormType(LRep.ConvertFromNormalRep(lValue.GetFlags()));
	auto RValue = rValue.ConvertAsNormType(LRep);
	lValue.BasicUnsignedDivOp(RValue);
}

static void SameRepCatchAllV2(MediumDecV2& lValue, const MediumDecV2& rValue, const RepType& LRep)
{
    lValue.ConvertToNormType(LRep.ConvertFromNormalRep(lValue.GetFlags()));
	auto RValue = rValue.ConvertAsNormType(LRep.ConvertFromNormalRep(rValue.GetFlags()));
	lValue.BasicUnsignedDivOp(RValue);
}

#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive

static void SameRep_ApproachingBottom(MediumDecV2& lValue, const MediumDecV2& rValue)
{
    if(lValue.IntHalf.Value==0)//0.0..01/2.0..01 = ~0.0..01
		return;
	else if(rValue.IntHalf.Value==0)//2.0..01/0.0..01 = infinity
#if defined(AltNum_EnableInfinity)
		SetAsInfinityVal();
#else
		SameRepCatchAll(lValue, rValue, RepType::ApproachingBottom);
#endif
	else
		SameRepCatchAll(lValue, rValue, RepType::ApproachingBottom);
}

static void SameRep_ApproachingTop(MediumDecV2& lValue, const MediumDecV2& rValue)
{
	SameRepCatchAll(lValue, rValue, RepType::ApproachingTop);
}

void MediumDecV2::UnsignedDivOp(const MediumDecV2& rValue)
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
			return;
	} else if(rValue.IsZero()){
		#if defined(AltNum_EnableIndeterminateForms)
			SetAsIndeterminate(DividedByZeroRep);
		#else
			throw "Divide by zero is not allowed with current toggles";
		#endif
	} else if(DecimalHalf<=NaNVariantThreshold&&IntHalf==rValue.IntHalf&&DecimalHalf.Value==rValue.DecimalHalf.Value)
		SetAsOneVal();
	else {
		RepType LRep = GetNormRepType();
		RepType RRep = rValue.GetNormRepType();
		if(DecimalHalf.Flags==rValue.DecimalHalf.Flags)//Same flag category
		{
			if(DecimalHalf.Flags!=0)
				DecimalHalf.Flags = 0;
			if(LRep==RRep)
			{
				switch(LRep)
				{
                case RepTypeEnum::NormalType:
					BasicUnsignedDivOp(rValue);
					break;
                break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
                case RepTypeEnum::ApproachingBottom:
					SameRep_ApproachingBottom(*this, rValue);
                    break;
                case RepTypeEnum::ApproachingTop:
					SameRep_ApproachingTop(*this, rValue);
	                break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #endif
                default:
                    throw "Operation not supported at moment.";
				}
			}
			else
			{
				switch(RRep)
				{
					case RepTypeEnum::NormalType:
					{
						switch(LRep)
						{
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
							case RepTypeEnum::ApproachingBottom:
								if(IntHalf.Value!=0)
									BasicUnsignedDivOp(rValue.ConvertAsNormType(RRep));
								break;
							//case RepTypeEnum::ApproachingTop:
	#endif
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
							default:
								BasicUnsignedDivOp(rValue); break;
						}
					}
					break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
					case RepTypeEnum::ApproachingBottom:
					{
						switch(LRep)
						{
							case RepTypeEnum::NormalType:
								//Add Code
								break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
							case RepTypeEnum::ApproachingTop:
								//Add Code
								break;
	#endif
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
							default:
								BasicUnsignedDivOp(rValue); break;
						}
					}
					break;
					case RepTypeEnum::ApproachingTop:
					{
						switch(LRep)
						{
							case RepTypeEnum::NormalType:
								//Add Code
								break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
							case RepTypeEnum::ApproachingBottom:
								//Add Code
								break;
	#endif
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
							default:
								//Add Code
								break;
						}
					}
					break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #endif
                default:
                    throw "Operation not supported";
				}
			}
		}
		else
		{
			if(LRep==RRep)
			{
				switch(LRep)
				{
                case RepTypeEnum::NormalType:
					BasicUnsignedDivOp(rValue);
					break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
                case RepTypeEnum::ApproachingBottom:
					SameRep_ApproachingBottom(*this, rValue);
                    break;
                case RepTypeEnum::ApproachingTop:
					SameRep_ApproachingTop(*this, rValue);
	                break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #endif
                default:
                    throw "Operation not supported";
				}
				switch(rValue.DecimalHalf.Flags)
				{
					case 1:
						BasicUnsignedDivOp(PiNum); break;
					case 2:
						BasicUnsignedDivOp(ENum); break;
					case 3:
						SwapNegativeStatus();
						DecimalHalf.Flags = 3;
						break;
					default: break;
				}
			}
			else
			{
				switch(RRep)
				{
					case RepTypeEnum::NormalType:
					{
						switch(LRep)
						{
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
							case RepTypeEnum::ApproachingBottom:
								if(IntHalf.Value!=0)
									BasicUnsignedDivOp(rValue.ConvertAsNormType(RRep));
								break;
							//case RepTypeEnum::ApproachingTop:
	#endif
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
							default:
								BasicUnsignedDivOp(rValue); break;
						}
					}
					break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
					case RepTypeEnum::ApproachingBottom:
					{
						switch(LRep)
						{
							case RepTypeEnum::NormalType:
								//Add Code
								break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
							case RepTypeEnum::ApproachingTop:
								//Add Code
								break;
	#endif
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
							default:
								BasicUnsignedDivOp(rValue); break;
						}
					}
					break;
					case RepTypeEnum::ApproachingTop:
					{
						switch(LRep)
						{
							case RepTypeEnum::NormalType:
								//Add Code
								break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
							case RepTypeEnum::ApproachingBottom:
								//Add Code
								break;
	#endif
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
							default:
								//Add Code
								break;
						}
					}
					break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #endif
                default:
                    throw "Operation not supported";
				}
			}
		}
	}
}

