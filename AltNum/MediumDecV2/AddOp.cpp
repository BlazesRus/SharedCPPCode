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
	if (IntValue == NegativeRep)
	{
		if (rValue.IntValue == 0)//-0.0..1 + 0.0..1 = 0
			SetAsZero();
		else if (rValue.IntValue == NegativeRep)//-0.0..1 - 0.0..1 = -0.0..1
		{/*Do Nothing*/
		}
		else if (rValue.IntValue < 0)//-0.0..1 - 1.0..1 = -1.0..1
		{
			IntValue = rValue.IntValue;
		}
		else//-0.0..1 + 5.0..1 = 5
		{
			DecimalHalf.Value = 0;
			IntValue = rValue.IntValue;
		}
	}
	if (IntValue == 0)
	{
		if (rValue.IntValue == 0)//0.0..1 + 0.0..1 = 0.0..1
		{/*Do Nothing*/
		}
		else if (rValue.IntValue == NegativeRep)//0.0..1 - 0.0..1 = 0
			SetAsZero();
		else if (rValue.IntValue < 0)//0.0..1 - 1.0..1 = -1
		{
			DecimalHalf.Value = 0;
			IntValue = rValue.IntValue;
		}
		else//0.0..1 + 5.0..1 = 5.0..1
		{
			IntValue = rValue.IntValue;
		}
	}
	else if (IntValue < 0)
	{
		if (rValue.IntValue == 0)//-1.0..1 + 0.0..1  = -1
			DecimalHalf.Value = 0;
		else if (rValue.IntValue == NegativeRep)//-1.0..1 - 0.0..1 = -1.0..1
		{/*Do Nothing*/
		}
		else if (IntValue == -rValue.IntValue)//-1.01 + 1.01
			SetAsZero();
		else if (rValue.IntValue < 0)//-1.0..1 - 2.0..1
		{
			IntValue += rValue.IntValue;
		}
		else//-1.0..1 + 2.0..1 = 1
		{
			DecimalHalf.Value = 0;
			IntValue += rValue.IntValue;
		}
	}
	else
	{
		if (rValue.IntValue == 0)//1.0..1 + 0.0..1
		{/*Do Nothing*/
		}
		else if (rValue.IntValue == NegativeRep)//1.0..1 - 0.0..1
			DecimalHalf.Value = 0;
		else if (IntValue == -rValue.IntValue)//1.0..1 - 1.0..1
			SetAsZero();
		else if (rValue.IntValue < 0)// 1.0..1  - 2.0..1
		{
			DecimalHalf.Value = 0;
			IntValue += rValue.IntValue;
		}
		else//1.0..1 + 1.0..1
		{
			IntValue += rValue.IntValue;
		}
	}
}

void MediumDecV2::AddOpSameRep_ApproachingTop(const MediumDecV2& rValue, const RepType& LRep)
{
	if (IntValue == NegativeRep)
	{
		if (rValue.IntValue == 0)//-0.9..9 + 0.9..9 = 0
			SetAsZero();
		else if (rValue.IntValue == NegativeRep)//-0.9..9 - 0.9..9 = -1.9..8
			IntValue = -1;
		else if (rValue.IntValue < 0)//-0.9..9 - 1.9..9 = -2.9..8
		{
			IntValue = rValue.IntValue - 1;
		}
		else//-0.9..9 + 5.9..9 = 5
		{
			DecimalHalf.Value = 0; ExtraRep = 0;
			IntValue = rValue.IntValue;
		}
	}
	if (IntValue == 0)
	{
		if (rValue.IntValue == 0)//0.9..9 + 0.9..9 = 1.9..8
			IntValue = 1;
		else if (rValue.IntValue == NegativeRep)//0.9..9 - 0.9..9 = 0
			SetAsZero();
		else if (rValue.IntValue < 0)//0.9..9 - 1.9..9 = -1
		{
			DecimalHalf.Value = 0; ExtraRep = 0;
			IntValue = rValue.IntValue;
		}
		else//0.9..9 + 5.9..9 = 6.9..8
		{
			IntValue = rValue.IntValue + 1;
		}
	}
	else if (IntValue < 0)
	{
		if (rValue.IntValue == 0)//-1.9..9 + 0.9..9  = -1
		{
			DecimalHalf.Value = 0; ExtraRep = 0;
		}
		else if (rValue.IntValue == NegativeRep)//-1.9..9 - 0.9..9 = -2.9..9
			IntValue += rValue.IntValue;
		else if (IntValue == -rValue.IntValue)//-1.9..9 + 1.9..9
			SetAsZero();
		else if (rValue.IntValue < 0)//-1.9..9 - 2.9..9
		{
			IntValue += rValue.IntValue;
		}
		else//-1.9..9 + 2.9..9
		{
			DecimalHalf.Value = 0; ExtraRep = 0;
			IntValue += rValue.IntValue;
		}
	}
	else
	{
		if (rValue.IntValue == 0)//1.9..9 + 0.9..9 = 2.9..8
		{
			if (IntValue == -1)
				IntValue = NegativeRep;
			else
				++IntValue;
		}
		else if (rValue.IntValue == NegativeRep)//1.9..9 - 0.9..9
		{
			DecimalHalf.Value = 0; ExtraRep = 0;
		}
		else if (IntValue == -rValue.IntValue)//1.9..9 - 1.9..9
			SetAsZero();
		else if (rValue.IntValue < 0)// 1.9..9  - 2.9..9
		{
			DecimalHalf.Value = 0; ExtraRep = 0;
			IntValue += rValue.IntValue;
		}
		else//1.9..9 + 1.9..9 = 3.9..8
		{
			IntValue += rValue.IntValue + 1;
		}
	}
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
                            AddOp_CatchAll(rValue, LRep, RRep); break;
                    }
    			} break;
        #if defined(AltNum_EnableApproaching)
    			case RepTypeEnum::ApproachingBottom:{
                    switch(RRep)
                    {
                        default:
                            AddOp_CatchAll(rValue, LRep, RRep); break;
                    }
    			}	break;
    			case RepTypeEnum::ApproachingTop:{
                    switch(RRep)
                    {
                        default:
                            AddOp_CatchAll(rValue, LRep, RRep); break;
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
