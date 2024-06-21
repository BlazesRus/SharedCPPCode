#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;
using RepTypeEnum = BlazesRusCode::RepTypeEnum;
/*
void MediumDecV2::MultOp_CatchAllOp(const MediumDecV2& rValue, const RepType& LRep, const RepType& RRep)
{
    lValue.ConvertToNormType(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	lValue.BasicUnsignedMultOp(RValue);
}

#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive

void MediumDecV2::MultOp_SameRep_ApproachingBottom(const MediumDecV2& rValue, const RepType& LRep)
{

}

void MediumDecV2::MultOp_SameRep_ApproachingTop(const MediumDecV2& rValue, const RepType& LRep)
{

}
*/

void MediumDecV2::UnsignedMultOp(const MediumDecV2& rValue)
{
	RepType LRep = GetNormRepType();
	RepType RRep = rValue.GetNormRepType();
	if(DecimalHalf.Flags==rValue.DecimalHalf.Flags)//Same flag category
	{
		if(LRep==RRep)
		{
			switch(DecimalHalf.Flags)
			{
				case 1:
				BasicUnsignedMultOp(PiNum); break;
				case 2:
				BasicUnsignedMultOp(ENum); break;
				case 3:
				SwapNegativeStatus(); DecimalHalf.Flags = 0;
				break;
			}
			switch(LRep)
			{
				case RepTypeEnum::NormalType:{
					BasicUnsignedMultOp(rValue);
				}
				break;
			}

		}
		else
		{
		}
	}
	else//Separate flag category
	{
		switch(rValue.DecimalHalf.Flags)
		{
			case 1:
			{
			}
			break;
			case 2:
			{
			}
			break;
			case 3:
			{
			}
			break;
		}
	}
}
