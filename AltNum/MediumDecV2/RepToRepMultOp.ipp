#if !defined(AltNum_LoadingRepToRep)
	#include "MediumDecV2.hpp"
	using MediumDecV2 = BlazesRusCode::MediumDecV2;
	using RepType = BlazesRusCode::RepType;
#endif

inline void MediumDecV2::NormalMultOp(const RepType& RRep, const MediumDecV2& Value)
{
	switch(RRep)
	{
		default:
			MultOp_CatchAll(rValue, LRep, RRep); break;
	}
}


#if defined(AltNum_EnableApproaching)
inline void MediumDecV2::ApproachingBottomMultOp(const RepType& RRep, const MediumDecV2& rValue)
{
	switch(RRep)
	{
		default:
			MultOp_CatchAll(rValue, LRep, RRep); break;
	}
}

inline void MediumDecV2::ApproachingTopMultOp(const RepType& RRep, const MediumDecV2& rValue)
{
	switch(RRep)
	{
		default:
			MultOp_CatchAll(rValue, LRep, RRep); break;
	}
}
#endif