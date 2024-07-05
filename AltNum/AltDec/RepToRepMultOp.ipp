#if !defined(AltNum_LoadingRepToRep)
	#include "AltDec.hpp"
	using AltDec = BlazesRusCode::AltDec;
	using RepType = BlazesRusCode::RepType;
#endif

void AltDec::NormalMultOp(const RepType& RRep, const AltDec& Value)
{

}

#if defined(AltNum_EnableFractionals)
void AltDec::NumByMultOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif

#if defined(AltNum_EnablePowerOfRepresentation)
void AltDec::PowerMultOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif


#if defined(AltNum_EnableApproaching)
void AltDec::ApproachingBottomMultOp(const RepType& RRep, const AltDec& rValue)
{
}

void AltDec::ApproachingTopMultOp(const RepType& RRep, const AltDec& rValue)
{
}
	#if defined(AltNum_EnableApproachingDivided)
void AltDec::ApproachingMidLeftMultOp(const RepType& RRep, const AltDec& rValue)
{
}

void AltDec::ApproachingMidRightMultOp(const RepType& RRep, const AltDec& rValue)
{
}
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
void AltDec::MixedFracMultOp(const RepType& RRep, const AltDec& Value)
{
}
#endif