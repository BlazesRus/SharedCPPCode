#if !defined(AltNum_LoadingRepToRep)
	#include "AltDec.hpp"
	using AltDec = BlazesRusCode::AltDec;
	using RepType = BlazesRusCode::RepType;
#endif

void AltDec::NormalAddOp(const RepType& RRep, const AltDec& Value)
{

}

#if defined(AltNum_EnableFractionals)
void AltDec::NumByAddOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif

#if defined(AltNum_EnablePowerOfRepresentation)
void AltDec::PowerAddOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif


#if defined(AltNum_EnableApproaching)
void AltDec::ApproachingBottomAddOp(const RepType& RRep, const AltDec& rValue)
{
}

void AltDec::ApproachingTopAddOp(const RepType& RRep, const AltDec& rValue)
{
}
	#if defined(AltNum_EnableApproachingDivided)
void AltDec::ApproachingMidLeftAddOp(const RepType& RRep, const AltDec& rValue)
{
}

void AltDec::ApproachingMidRightAddOp(const RepType& RRep, const AltDec& rValue)
{
}
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
void AltDec::MixedFracAddOp(const RepType& RRep, const AltDec& Value)
{
}
#endif