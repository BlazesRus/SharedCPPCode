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

#if defined(AltNum_EnableApproaching)

void MediumDecV2::SubOpSameRep_ApproachingBottom(const MediumDecV2& rValue, const RepType& LRep)
{
	if (IntHalf == NegativeRep)
	{
		if (rValue.IntHalf == 0)//-0.0..1 - 0.0..1
		{/*Do Nothing*/
		}
		else if (rValue.IntHalf == NegativeRep)//-0.0..1 + 0.0..1 
			SetAsZero();
		else if (rValue.IsNegative())//-0.0..1 + 1.0..1
		{
			DecimalHalf.Value = 0;
			IntHalf = -rValue.IntHalf;
		}
		else//-0.0..1 - 5.0..1
		{
			IntHalf = -rValue.IntHalf;
		}
	}
	else if (IntHalf == MirroredInt::Zero)
	{
		//if (rValue.IntHalf == MirroredInt::Zero)//0.0..1 - 0.0..1 = 0.0..01
		//else if (rValue.IntHalf == NegativeRep)//0.0..1 + 0.0..1 = 0.0..01
		if (rValue.IntHalf.Value != 0)
		{
			if (rValue.IsNegative())//0.0..1 + 1.0..1
			{
				IntHalf = -rValue.IntHalf;
			} else {//0.0..1 - 5.0..1
				DecimalHalf.Value = 0;
				IntHalf = -rValue.IntHalf;
			}
		}
	}
	else if (IsNegative())
	{
		//if (rValue.IntHalf == MirroredInt::Zero)//-1.0..1 - 0.0..1  = -1
		if (rValue.IntHalf == NegativeRep)//-1.0..1 + 0.0..1
			DecimalHalf.Value = 0;
		else if (IntHalf == rValue.IntHalf)//-1.01 - 1.01
			SetAsZero();
		else if (rValue.IsNegative())//-1.0..1 + 2.0..1
		{
			DecimalHalf.Value = 0;
			IntHalf -= rValue.IntHalf;
		}
		else if (rValue.IntHalf != MirroredInt::Zero)//-1.0..1 - 2.0..1 = 1
		{
			IntHalf -= rValue.IntHalf;
		}
	}
	else
	{
		if (rValue.IntHalf == MirroredInt::Zero)//1.0..1 - 0.0..1
			DecimalHalf.Value = 0;
		//else if (rValue.IntHalf == NegativeRep)//1.0..1 + 0.0..1
		else if (IntHalf == -rValue.IntHalf)//1.0..1 - 1.0..1
			SetAsZero();
		else if (rValue.IsNegative())// 1.0..1  - 2.0..1
		{
			DecimalHalf.Value = 0;
			IntHalf -= rValue.IntHalf;
		}
		else if (rValue.IntHalf != NegativeRep)//1.0..1 - 1.0..1
		{
			IntHalf -= rValue.IntHalf;
		}
	}
}

void MediumDecV2::SubOpSameRep_ApproachingTop(const MediumDecV2& rValue, const RepType& LRep)
{
	if (IntHalf == NegativeRep)
	{
		if (rValue.IntHalf == 0)//-0.9..9 - 0.9..9 = 0
			IntHalf = MirroredInt::NegativeOne;
		else if (rValue.IntHalf == NegativeRep)//-0.9..9 + 0.9..9
			SetAsZero();
		//else if (rValue.IsNegative())//-0.9..9 + 1.9..9
		else if (rValue.IsPositive())//-0.9..9 - 5.9..9 = -6.9..8
			IntHalf = -rValue.IntHalf - 1;
	}
	else if (IntHalf == MirroredInt::Zero)
	{
		if (rValue.IntHalf == MirroredInt::Zero)//0.9..9 - 0.9..9
			SetAsZero();
		else if (rValue.IntHalf == NegativeRep)//0.9..9 + 0.9..9 = 1.9..8
			IntHalf = MirroredInt::One;
		else if (rValue.IsNegative())//0.9..9 + 1.9..9 = 1.9..8
			IntHalf = -rValue.IntHalf;
		else//0.9..9 - 5.9..9 = -5
		{
			DecimalHalf.Value = 0;
			IntHalf = -rValue.IntHalf;
		}
	}
	else if (IsNegative())
	{
		if (rValue.IntHalf == MirroredInt::Zero)//-1.9..9 - 0.9..9  = -2.9..98
			--IntHalf;
		else if (rValue.IntHalf == NegativeRep)//-1.9..9  + 0.9..9 = -1
			DecimalHalf.Value = 0;
		else if (IntHalf == rValue.IntHalf)//-1.01 + 1.01
			SetAsZero();
		else if (rValue.IsNegative()){//-1.9..9 + 2.9..9
			DecimalHalf.Value = 0;
			IntHalf -= rValue.IntHalf;
		}
		else//-1.9..9 - 2.9..9
			IntHalf -= rValue.IntHalf + 1;
	}
	else
	{
		if (rValue.IntHalf == MirroredInt::Zero)//1.9..9 - 0.9..9 = 2
			DecimalHalf.Value = 0;
		else if (rValue.IntHalf == NegativeRep)//1.9..9 + 0.9..9
			++IntHalf;
		else if (IntHalf == -rValue.IntHalf)//1.9..9 - 1.9..9
			SetAsZero();
		else if (rValue.IsNegative())// 1.9..9  - 2.9..9
		{
			DecimalHalf.Value = 0;
			IntHalf -= rValue.IntHalf;
		}
		else//1.9..9 - 2.9..9
		{
			DecimalHalf.Value = 0;
			IntHalf -= rValue.IntHalf;
		}
	}
}

#endif

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
    		switch(LRep)
    		{
    			case RepTypeEnum::NormalType:{
                    switch(RRep)
                    {
                        default:
                            SubOp_CatchAll(rValue, LRep, RRep); break;
                    }
    			} break;
        #if defined(AltNum_EnableApproaching)
    			case RepTypeEnum::ApproachingBottom:{
                    switch(RRep)
                    {
						case RepTypeEnum::ApproachingTop:
							if(IsNegative()){//-1.0..1 - 5.9..9 = -7
								IntHalf.Value = rValue.IntHalf.Value+1;
								DecimalHalf = 0;
							}
							else
								SubOp_CatchAll(rValue, LRep, RRep);
							break;
                        default:
                            SubOp_CatchAll(rValue, LRep, RRep); break;
                    }
    			}	break;
    			case RepTypeEnum::ApproachingTop:{
                    switch(RRep)
                    {
						case RepTypeEnum::ApproachingBottom:
							if(IsNegative()){
								IntHalf.Value = rValue.IntHalf.Value+1;
								DecimalHalf = 0;
							}
							else
								SubOp_CatchAll(rValue, LRep, RRep);
							break;
                        default:
                            SubOp_CatchAll(rValue, LRep, RRep); break;
                    }
    			} break;
        #endif
				default:
					throw "Operation not supported at moment.";
    		}
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
