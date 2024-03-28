#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::MediumDecBase::RepType;

#if defined(MediumDecV2_UseMirroredInt)
	MirroredInt MediumDecV2::NegativeRep = MirroredInt::NegativeZero;
#endif
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproachingValues)
    MediumDecV2 MediumDecV2::AlmostOne = MediumDecV2::ApproachingRightRealValue();
#endif
    MediumDecV2 MediumDecV2::Pi = MediumDecV2::PiValue();
    MediumDecV2 MediumDecV2::One = MediumDecV2::OneValue();
    MediumDecV2 MediumDecV2::Two = MediumDecV2::TwoValue();
    MediumDecV2 MediumDecV2::NegativeOne = MediumDecV2::NegativeOneValue();
    MediumDecV2 MediumDecV2::Zero = MediumDecV2::ZeroValue();
    MediumDecV2 MediumDecV2::PointFive = MediumDecV2::Point5Value();
    MediumDecV2 MediumDecV2::JustAboveZero = MediumDecV2::JustAboveZeroValue();
    MediumDecV2 MediumDecV2::OneMillionth = MediumDecV2::OneMillionthValue();
    MediumDecV2 MediumDecV2::FiveThousandth = MediumDecV2::FiveThousandthValue();
    MediumDecV2 MediumDecV2::Minimum = MediumDecV2::MinimumValue();
    MediumDecV2 MediumDecV2::Maximum = MediumDecV2::MaximumValue();
    MediumDecV2 MediumDecV2::E = MediumDecV2::EValue();
    MediumDecV2 MediumDecV2::LN10 = MediumDecV2::LN10Value();
    MediumDecV2 MediumDecV2::LN10Mult = MediumDecV2::LN10MultValue();
    MediumDecV2 MediumDecV2::HalfLN10Mult = MediumDecV2::HalfLN10MultValue();
    MediumDecV2 MediumDecV2::TenMillionth = MediumDecV2::TenMillionthValue();
    MediumDecV2 MediumDecV2::FiveMillionth = MediumDecV2::FiveMillionthValue();
    MediumDecV2 MediumDecV2::FiveBillionth = MediumDecV2::FiveBillionthValue();
    MediumDecV2 MediumDecV2::OneGMillionth = MediumDecV2::OneHundredMillionthValue();
    #if defined(AltNum_EnableNilRep)
    MediumDecV2 MediumDecV2::Nil = MediumDecV2::NilValue();
    #endif

    MediumDecV2 MediumDecV2::PiNum = MediumDecV2::PiNumValue();
    MediumDecV2 MediumDecV2::ENum = MediumDecV2::ENumValue();
    
#if defined(AltNum_EnableInfinityRep)
    MediumDecV2 MediumDecV2::Infinity = MediumDecV2::InfinityValue();
#endif
#if defined(AltNum_EnableApproachingValues)
    MediumDecV2 MediumDecV2::NegativeInfinity = MediumDecV2::NegativeInfinityValue();
    MediumDecV2 MediumDecV2::ApproachingZero = MediumDecV2::ApproachingZeroValue();
#endif
#if defined(AltNum_EnableNaN)
    MediumDecV2 MediumDecV2::NaN = MediumDecV2::NaNValue();
	MediumDecV2 MediumDecV2::Undefined = MediumDecV2::UndefinedValue();
#endif
    #pragma endregion ValueDefine Source