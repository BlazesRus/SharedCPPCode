#include "AltDec.hpp"

using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

#if defined(AltNum_EnableFractionals)
#endif

#if defined(AltNum_EnablePiRep)
#endif
#if defined(AltNum_EnablePiPowers)
#endif
#if defined(AltNum_EnablePiFractional)
#endif

#if defined(AltNum_EnableERep)
#endif
#if defined(AltNum_EnableEFractional)
#endif

#if defined(AltNum_EnableDecimaledPiOrEFractionals)
#endif

#if defined(AltNum_EnableApproachingValues)
    #if !defined(AltNum_DisableApproachingTop)
    #endif
#endif

#if defined(AltNum_EnableApproachingDivided)
#if !defined(AltNum_DisableApproachingTop)
#endif
#endif


#if defined(AltNum_EnableImaginaryNum)
#endif

#if defined(AltNum_EnableDecimaledIFractionals)
#elif defined(AltNum_EnableIFractional)
#endif

#if defined(AltNum_EnableApproachingI)
    #if !defined(AltNum_DisableApproachingTop)

    #endif
	#if defined(AltNum_EnableApproachingDivided)


	    #if !defined(AltNum_DisableApproachingTop)

	    #endif
    #endif
#endif

#if defined(AltNum_EnableMixedFractional)

#endif

#if defined(AltNum_MixedPiOrEEnabled)

#elif defined(AltNum_EnableMixedIFractional)

#endif