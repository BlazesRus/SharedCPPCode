#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;
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
	if(IsZero())
        return;
    else if(rValue.IsZero()){
        SetAsZero();
        return;
    }
	#if defined(AltNum_EnableInfinityRep)
    else if(DecimalHalf.Value==InfinityRep){
        if(rValue.GetFlags()==3){
            if(GetFlags()==3){
                SwapNegativeStatus();
                SetFlags(0);
            } else
                SetFlags(3);
        }
        return;
    } else if(rValue.DecimalHalf.Value==InfinityRep)
        SetAsInfinityVal();
        if(rValue.GetFlags()==3){
            if(GetFlags()==3){
                SwapNegativeStatus();
                SetFlags(0);
            } else
                SetFlags(3);
        }
    }
    #endif
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
        if(rValue.DecimalHalf.Flags==1){
            if(GetFlags()==0)
                SetFlag(1);
            else
                UnsignedMultOp(PiNum);
        } else if(rValue.DecimalHalf.Flags==2){
            if(GetFlags()==0)
                SetFlag(2);
            else
                UnsignedMultOp(ENum);
        } else if(rValue.DecimalHalf.Flags==3){
            SetFlag(3);
            if(DecimalHalf.Flags==1)
                UnsignedMultOp(PiNum);
            else if(DecimalHalf.Flags==2)
                UnsignedMultOp(ENum);
        }
	}
}
