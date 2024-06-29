#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;

inline void MediumDecV2::AddOp_CatchAll(const MediumDecV2& rValue, const RepType& LRep, const RepType& RRep)
{
    ConvertToNormTypeFromOther(LRep);
	auto RValue = rValue.ConvertToNormTypeFromOther(RRep);
	lValue.BasicUnsignedAddOp(RValue);
}

inline void MediumDecV2::AddOp_CatchAllV2(const MediumDecV2& rValue, const RepType& LRep)
{
    ConvertToNormTypeFromOther(LRep);
	auto RValue = rValue.ConvertToNormTypeFromOther(LRep);
	lValue.BasicUnsignedAddOp(RValue);
}

void MediumDecV2::AddOpSameRep_ApproachingBottom(const MediumDecV2& rValue, const RepType& LRep)
{

}

void MediumDecV2::AddOpSameRep_ApproachingTop(const MediumDecV2& rValue, const RepType& LRep)
{

}

void MediumDecV2::UnsignedAddOp(const MediumDecV2& rValue)
{
    if(rValue.IsZero())
        return;
    else if(IsZero()){
        SetValue(rValue); return;
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
				BasicUnsignedAddOp(rValue);
				break;
			break;
#if defined(AltNum_EnableApproaching)
			case RepTypeEnum::ApproachingBottom:
				AddOpSameRep_ApproachingBottom(rValue, LRep);
				break;
			case RepTypeEnum::ApproachingTop:
				AddOpSameRep_ApproachingTop(rValue, LRep);
				break;
#endif
			default:
				throw "Operation not supported at moment.";
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
    			case RepTypeEnum::ApproachingTop:{
                    switch(RRep)
                    {
                        default:
                            MultOp_CatchAll(rValue, LRep, RRep); break;
                    }
    			} break;
        #endif
    		}
		}
	}
	else//Separate Flag Categories
	{
		if(rValue.DecimalHalf.Flags==3||DecimalHalf.Flags==3){
				throw "Complex number operations not enabled yet.";
        else
            AddOp_CatchAll(rValue, LRep, RRep);
	}
}
