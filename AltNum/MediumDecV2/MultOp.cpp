#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;

inline void MediumDecV2::MultOp_CatchAll(const MediumDecV2& rValue, const RepType& LRep, const RepType& RRep)
{
    ConvertFromAbtract(LRep);
	auto RValue = rValue.ConvertAsNormType(RRep);
	lValue.BasicUnsignedMultOp(RValue);
}

inline void MediumDecV2::MultOp_CatchAllV2(const MediumDecV2& rValue, const RepType& LRep)
{
    ConvertFromAbtract(LRep);
	auto RValue = rValue.ConvertAsNormType(LRep);
	lValue.BasicUnsignedMultOp(RValue);
}

void MediumDecV2::MultOpSameRep_ApproachingBottom(const MediumDecV2& rValue, const RepType& LRep)
{

}

void MediumDecV2::MultOpSameRep_ApproachingTop(const MediumDecV2& rValue, const RepType& LRep)
{

}


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
		return;
    }
    #endif
	RepType LRep = GetNormRepType();
	RepType RRep = rValue.GetNormRepType();
	if(LRep==RRep)
	{
		switch(LRep)
		{
			case RepTypeEnum::NormalType:{
				BasicUnsignedMultOp(rValue);
			} break;
    #if defined(AltNum_EnableApproaching)
            case RepTypeEnum::ApproachingBottom:
				MultOpSameRep_ApproachingBottom(rValue); break;
            case RepTypeEnum::ApproachingTop:
				MultOpSameRep_ApproachingTop(rValue); break;
    #endif
    #if defined(AltDec_EnableUndefinedButInRange)
            case RepType::UndefinedButInRange:
                throw "Operation not supported at moment."; break;
    #endif
            default:
                throw "Operation not supported at moment."; break;
		}

	}
	else
	{
		switch(LRep)
		{
			case RepTypeEnum::NormalType:{
                switch(RRep)
                {
                    default:
                        MultOp_CatchAll(rValue, LRep, RRep); break;
                }
			} break;
    #if defined(AltNum_EnableApproaching)
			case RepTypeEnum::ApproachingBottom:{
                switch(RRep)
                {
                    default:
                        MultOp_CatchAll(rValue, LRep, RRep); break;
                }
			}	break;
			case RepTypeEnum::ApproachingTop:
                switch(RRep)
                {
                    default:
                        MultOp_CatchAll(rValue, LRep, RRep); break;
                }
			} break;
    #endif
		}
	}
	if(DecimalHalf.Flags==rValue.DecimalHalf.Flags)//Same flag category
	{
        switch(DecimalHalf.Flags)
        {
            case 1:
                UnsignedMultOp(PiNum); break;
            case 2:
                UnsignedMultOp(ENum); break;
            case 3:
                SwapNegativeStatus(); DecimalHalf.Flags = 0;
                break;
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
