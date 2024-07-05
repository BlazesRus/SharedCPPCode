#if !defined(AltNum_LoadingRepToRep)
	#include "AltDec.hpp"
	using AltDec = BlazesRusCode::AltDec;
	using RepType = BlazesRusCode::RepType;
#endif

inline void AltDec::NormalSubOp(const RepType& RRep, const AltDec& Value)
{

}

#if defined(AltNum_EnableFractionals)
inline void AltDec::NumBySubOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif

#if defined(AltNum_EnablePowerOfRepresentation)
inline void AltDec::PowerSubOp(const RepType& RRep, const AltDec& rValue)
{
}
#endif


#if defined(AltNum_EnableApproaching)
inline void AltDec::ApproachingBottomSubOp(const RepType& RRep, const AltDec& rValue)
{
}

inline void AltDec::ApproachingTopSubOp(const RepType& RRep, const AltDec& rValue)
{
}
	#if defined(AltNum_EnableApproachingDivided)
inline void AltDec::ApproachingMidLeftSubOp(const RepType& RRep, const AltDec& rValue)
{
}

inline void AltDec::ApproachingMidRightSubOp(const RepType& RRep, const AltDec& rValue)
{
}
	#endif
#endif

#if defined(AltNum_EnableMixedFractional)
inline void AltDec::MixedFracSubOp(const RepType& RRep, const AltDec& Value)
{
}
#endif