#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;

void SameRep_ApproachingBottom(MediumDecV2& lValue, const MediumDecV2& rValue, const RepType& LRep)
{

}

void SameRep_ApproachingTop(MediumDecV2& lValue, const MediumDecV2& rValue, const RepType& LRep)
{

}

void MediumDecV2::UnsignedSubOp(const MediumDecV2& rValue)
{
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
				SameRep_ApproachingBottom(*this, rValue, LRep);
				break;
			case RepTypeEnum::ApproachingTop:
				SameRep_ApproachingTop(*this, rValue, LRep);
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
	else//Separate Categories
	{
	}
}
