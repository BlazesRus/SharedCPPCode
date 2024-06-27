#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;

#if defined(AltNum_EnablePiRep)
	#if defined(AltNum_UseBuiltinVirtualTable)
RepType BlazesRusCode::VirtualTable_GetPiRepType() const
	#else
RepType BlazesRusCode::MediumDecV2::GetPiRepType() const
	#endif
{
	switch (DecimalHalf.Value)
	{
		#if defined(AltNum_EnableApproaching)
	case ApproachingBottomRep:
		return RepType::ApproachingTopPi; break;
	case ApproachingTopRep:
		return RepType::ApproachingTopPi; break;
		#endif
	default:
		return RepType::PiNum;
	}
}
#endif

#if defined(AltNum_EnableERep)
	#if defined(AltNum_UseBuiltinVirtualTable)
RepType BlazesRusCode::VirtualTable_GetERepType() const
	#else
RepType BlazesRusCode::MediumDecV2::GetERepType() const
	#endif
{
	switch (DecimalHalf.Value)
    {
#if defined(AltNum_EnableApproaching)
		case ApproachingBottomRep:
			return RepType::ApproachingTopE; break;
		case ApproachingTopRep:
			return RepType::ApproachingTopE; break;
#endif
		default:
			return RepType::ENum;
	}
}


#endif

#if defined(AltNum_EnableIRep)
	#if defined(AltNum_UseBuiltinVirtualTable)
RepType BlazesRusCode::VirtualTable_GetIRepType() const
	#else
RepType BlazesRusCode::MediumDecV2::GetIRepType() const
	#endif
{
	switch (DecimalHalf.Value)
	{
		#if defined(AltNum_EnableImaginaryInfinity)
	case InfinityRep:
		return RepType::ImaginaryInfinity; break;
		#endif
		#if defined(AltNum_EnableApproaching)
	case ApproachingBottomRep:
		return RepType::ApproachingImaginaryBottom; break;
	case ApproachingTopRep:
		return RepType::ApproachingImaginaryTop; break;
		#endif
	default:
		return RepType::INum;
	}
}


#endif

#if defined(AltNum_UseBuiltinVirtualTable)
RepType BlazesRusCode::VirtualTable_GetNormRepType() const
#else
RepType BlazesRusCode::MediumDecV2::GetNormRepType() const
#endif
{
    switch(DecimalHalf.Value)
    {
#if defined(AltNum_EnableApproaching)
        case ApproachingBottomRep:
            return RepType::ApproachingBottom; break;
        case ApproachingTopRep:
            return RepType::ApproachingTop; break;
#endif
#if defined(AltNum_EnableInfinityRep)
        case InfinityRep:
            return RepType::Infinity; break;
#endif
#if defined(AltNum_EnableNaN)
        case NaNRep:
            return RepType::NaN; break;
        case UndefinedRep:
            return RepType::Undefined; break;
#endif
#if defined(AltNum_EnableNil)
        case NilRep:
            return RepType::Nil; break;
#endif
        default:
#if defined(AltNum_EnableIndeterminateForms)
			if(DecimalHalf>IndeterminateRep)
				return RepType::IndeterminateForm;
#endif
            return RepType::NormalType; break;
    }
}

#if defined(AltNum_UseBuiltinVirtualTable)
RepType BlazesRusCode::MediumDecV2::VirtualTable_GetRepType() const
#else
RepType BlazesRusCode::MediumDecV2::GetRepType() const
#endif
{
    switch(DecimalHalf.Flags)
    {
#if defined(AltNum_EnablePiRep)
        case 1:
            return GetPiRepType(); break;
#endif
#if defined(AltNum_EnableERep)
        case 2:
            return GetERepType(); break;
#endif
#if defined(AltNum_EnableIRep)
        case 3:
            return GetIRepType(); break;
#elif defined(MediumDecV2_EnableWithinMinMaxRange)
        case 3:
			//If IntHalf==???, then left side range value equals negative infinity
			//If DecimalHalf.tValue==???, then right side range value equals positive infinity
			//IntHalf represents left side minimum
			//For DecimalHalf.tValue represents right side maximum value with negative numbers represents at numbers above ???
			return RepType::WithinMinMaxRange;
#endif
        default:
            return GetNormRepType(); break;
    }
}