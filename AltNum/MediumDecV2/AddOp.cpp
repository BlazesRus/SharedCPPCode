#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;
using RepTypeEnum = BlazesRusCode::RepTypeEnum;

void SameRep_ApproachingBottom(MediumDecV2& lValue, const MediumDecV2& rValue, const RepType& LRep)
{

}

void SameRep_ApproachingTop(MediumDecV2& lValue, const MediumDecV2& rValue, const RepType& LRep)
{

}

void MediumDecV2::UnsignedAddOp(const MediumDecV2& rValue)
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
				BasicUnsignedAddOp(rValue);
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
		if(rValue.DecimalHalf.Flags==3){
			if(IsZero())
				SetValue(rValue);
			else
				throw "Complex number operations not enabled yet.";
		} else if(DecimalHalf.Flags==3){
			if(rValue.IsZero())
				SetValue(rValue);
			else
				throw "Complex number operations not enabled yet.";
		} else
		{
		}
	}
}
