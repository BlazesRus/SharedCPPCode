#if !defined(AltNum_LoadingRepToRep)
	#include "MediumDecV2.hpp"
	using MediumDecV2 = BlazesRusCode::MediumDecV2;
	using RepType = BlazesRusCode::RepType;
#endif

inline void MediumDecV2::NormalDivOp(const RepType& RRep, const MediumDecV2& Value)
{
	switch (RRep)
	{
		case RepType::NormalType://Fail safe for when converted before switch
			self.BasicUnsignedDivOp(Value);
			break;
		default:
			Value.ConvertToNormType(RRep);
			self.BasicUnsignedDivOp(Value);
			break;
	}
}


#if defined(AltNum_EnableApproaching)
inline void MediumDecV2::ApproachingBottomDivOp(const RepType& RRep, const MediumDecV2& rValue)
{
}

inline void MediumDecV2::ApproachingTopDivOp(const RepType& RRep, const MediumDecV2& rValue)
{
}
#endif