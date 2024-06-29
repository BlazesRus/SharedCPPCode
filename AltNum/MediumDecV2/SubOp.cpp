#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;

inline void MediumDecV2::SubOp_CatchAll(const MediumDecV2& rValue, const RepType& LRep)
{
    ConvertToNormTypeFromOther(LRep);
	auto RValue = rValue.ConvertToNormTypeFromOther(LRep);
	lValue.BasicUnsignedSubOp(RValue);
}

inline void MediumDecV2::SubOp_CatchAllV2(const MediumDecV2& rValue, const RepType& LRep)
{
    ConvertFromAbtract(LRep);
	auto RValue = rValue.ConvertToNormTypeFromOther(LRep);
	BasicUnsignedSubOp(RValue);
}

void MediumDecV2::SubOpSameRep_ApproachingBottom(const MediumDecV2& rValue, const RepType& LRep)
{

}

void MediumDecV2::SubOpSameRep_ApproachingTop(const MediumDecV2& rValue, const RepType& LRep)
{

}

void MediumDecV2::UnsignedSubOp(const MediumDecV2& rValue)
{
    if(rValue.IsZero())
        return;
    else if(IsZero()){
        SetValue(-rValue); return;
    }
	#if defined(AltNum_EnableInfinityRep)
    #endif
	RepType LRep = GetNormRepType();
	RepType RRep = rValue.GetNormRepType();
	if(DecimalHalf.Flags==rValue.DecimalHalf.Flags)//Same flag category
	{
		if(LRep==RRep)
		{
			switch(LRep)
			{
			case RepTypeEnum::NormalType:
				BasicUnsignedSubOp(rValue);
				break;
			break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
#if defined(AltNum_EnableApproaching)
			case RepTypeEnum::ApproachingBottom:
				SubOpSameRep_ApproachingBottom(rValue, LRep);
				break;
			case RepTypeEnum::ApproachingTop:
				SubOpSameRep_ApproachingTop(rValue, LRep);
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
		}
	}
	else//Separate Flag Categories
	{
		if(rValue.DecimalHalf.Flags==3||DecimalHalf.Flags==3){
				throw "Complex number operations not enabled yet.";
        else
            SubOp_CatchAll(rValue, LRep, RRep);
	}
}
