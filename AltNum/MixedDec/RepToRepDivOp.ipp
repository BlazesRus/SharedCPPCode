#if !defined(AltNum_LoadingRepToRep)
	#include "MixedDec.hpp"
	using MixedDec = BlazesRusCode::MixedDec;
	using RepType = BlazesRusCode::RepType;
#endif

inline void MixedDec::NormalDivOp(const RepType& RRep, const MixedDec& Value)
{
	switch (RRep)
	{
	case RepType::NormalType://Fail safe for when converted before switch
		self.BasicUnsignedDivOp(Value);
		break;
	#if defined(MixedDec_EnableFractionals)
	case RepType::NumByDiv://X / (Y / Z) = (XZ)/Y
	{
		self.BasicUnsignedDivOp(Value);
		self.BasicIntMultOp(Value.ExtraRep.Value);
		break;
	}
	#endif
		default:
			Value.ConvertToNormType(RRep);
			self.BasicUnsignedDivOp(Value);
			break;
	}
}

#if defined(MixedDec_EnableFractionals)
inline void MixedDec::NumByDivOp(const RepType& RRep, const MixedDec& rValue)
{
}
#endif

#if defined(MixedDec_EnablePowerOfRepresentation)
inline void MixedDec::PowerDivOp(const RepType& RRep, const MixedDec& rValue)
{
}
#endif


#if defined(MixedDec_EnableApproaching)
inline void MixedDec::ApproachingBottomDivOp(const RepType& RRep, const MixedDec& rValue)
{
}

inline void MixedDec::ApproachingTopDivOp(const RepType& RRep, const MixedDec& rValue)
{
}
#endif

#if defined(MixedDec_EnableApproachingDivided)
void MixedDec::ApproachingMidLeftDivOp(const RepType& RRep, const MixedDec& rValue)
{
}

void MixedDec::ApproachingMidRightDivOp(const RepType& RRep, const MixedDec& rValue)
{
}
#endif

#if defined(AltNum_EnableMixedFractional)
void MixedDec::MixedFracDivOp(RepType& RRep, MixedDec& self, MixedDec& Value)
{
}
#endif