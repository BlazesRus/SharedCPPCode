#if !defined(AltNum_LoadingRepToRep)
	#include "AltDec.hpp"
	using AltDec = BlazesRusCode::AltDec;
	using RepType = BlazesRusCode::RepType;
#endif

void AltDec::NormalDivOp(RepType& RRep, AltDec& Value)
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
void AltDec::ApproachingBottomOp(const RepType& RRep, const AltDec& rValue)
{
}

void AltDec::ApproachingTopOp(const RepType& RRep, const AltDec& rValue)
{
}
	#if defined(AltNum_EnableApproachingDivided)
void AltDec::ApproachingMidLeftOp(const RepType& RRep, const AltDec& rValue)
{
}

void AltDec::ApproachingMidRightOp(const RepType& RRep, const AltDec& rValue)
{
}
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
void AltDec::MixedFracDivOp(RepType& RRep, AltDec& self, AltDec& Value)
{
}
#endif