#if !defined(AltNum_LoadingRepToRep)
	#include "AltDec.hpp"
	using AltDec = BlazesRusCode::AltDec;
	using RepType = BlazesRusCode::RepType;
#endif

void AltDec::NormalDivOp(const RepType& RRep, const AltDec& Value)
{
	switch (RRep)
	{
	case RepType::NormalType://Fail safe for when converted before switch
		self.BasicUnsignedDivOp(Value);
		break;
	#if defined(AltNum_EnableFractionals)
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

#if defined(AltNum_EnableFractionals)
void AltDec::NumByDivOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif

#if defined(AltNum_EnablePowerOfRepresentation)
void AltDec::PowerDivOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif


#if defined(AltNum_EnableApproaching)
void AltDec::ApproachingBottomDivOp(const RepType& RRep, const AltDec& rValue)
{
}

void AltDec::ApproachingTopDivOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif

#if defined(AltNum_EnableApproachingDivided)
void AltDec::ApproachingMidLeftDivOp(const RepType& RRep, const AltDec& rValue)
{
}

void AltDec::ApproachingMidRightDivOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif

#if defined(AltNum_EnableMixedFractional)
void AltDec::MixedFracDivOp(const RepType& RRep, const AltDec& Value)
{
}
#endif