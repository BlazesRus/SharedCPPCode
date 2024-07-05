#if !defined(AltNum_LoadingRepToRep)
	#include "MediumDecV2.hpp"
	using MediumDecV2 = BlazesRusCode::MediumDecV2;
	using RepType = BlazesRusCode::RepType;
#endif

inline void MediumDecV2::NormalAddOp(const RepType& RRep, const MediumDecV2& Value)
{
	switch(RRep)
	{
		default:
			AddOp_CatchAll(rValue, LRep, RRep); break;
	}
}


#if defined(AltNum_EnableApproaching)
inline void MediumDecV2::ApproachingBottomAddOp(const RepType& RRep, const MediumDecV2& rValue)
{
	switch(RRep)
	{
		default:
			AddOp_CatchAll(rValue, LRep, RRep); break;
	}
}

inline void MediumDecV2::ApproachingTopAddOp(const RepType& RRep, const MediumDecV2& rValue)
{
	switch(RRep)
	{
		default:
			AddOp_CatchAll(rValue, LRep, RRep); break;
	}
}
#endif