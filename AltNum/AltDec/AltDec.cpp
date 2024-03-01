#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

#if defined(AltDec_UseMirroredInt)
	MirroredInt AltDec::NegativeRep = MirroredInt::NegativeZero;
#endif
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproachingValues)
    AltDec AltDec::AlmostOne = AltDec::ApproachingRightRealValue();
#endif
    AltDec AltDec::Pi = AltDec::PiValue();
    AltDec AltDec::One = AltDec::OneValue();
    AltDec AltDec::Two = AltDec::TwoValue();
    AltDec AltDec::NegativeOne = AltDec::NegativeOneValue();
    AltDec AltDec::Zero = AltDec::ZeroValue();
    AltDec AltDec::PointFive = AltDec::Point5Value();
    AltDec AltDec::JustAboveZero = AltDec::JustAboveZeroValue();
    AltDec AltDec::OneMillionth = AltDec::OneMillionthValue();
    AltDec AltDec::FiveThousandth = AltDec::FiveThousandthValue();
    AltDec AltDec::Minimum = AltDec::MinimumValue();
    AltDec AltDec::Maximum = AltDec::MaximumValue();
    AltDec AltDec::E = AltDec::EValue();
    AltDec AltDec::LN10 = AltDec::LN10Value();
    AltDec AltDec::LN10Mult = AltDec::LN10MultValue();
    AltDec AltDec::HalfLN10Mult = AltDec::HalfLN10MultValue();
    AltDec AltDec::TenMillionth = AltDec::TenMillionthValue();
    AltDec AltDec::FiveMillionth = AltDec::FiveMillionthValue();
    AltDec AltDec::FiveBillionth = AltDec::FiveBillionthValue();
    AltDec AltDec::OneGMillionth = AltDec::OneHundredMillionthValue();
    #if defined(AltNum_EnableNilRep)
    AltDec AltDec::Nil = AltDec::NilValue();
    #endif

    AltDec AltDec::PiNum = AltDec::PiNumValue();
    AltDec AltDec::ENum = AltDec::ENumValue();
    
#if defined(AltNum_EnableInfinityRep)
    AltDec AltDec::Infinity = AltDec::InfinityValue();
#endif
#if defined(AltNum_EnableApproachingValues)
    AltDec AltDec::NegativeInfinity = AltDec::NegativeInfinityValue();
    AltDec AltDec::ApproachingZero = AltDec::ApproachingZeroValue();
#endif
#if defined(AltNum_EnableNaN)
    AltDec AltDec::NaN = AltDec::NaNValue();
	AltDec AltDec::Undefined = AltDec::UndefinedValue();
#endif
    #pragma endregion ValueDefine Source

    #pragma endregion String Function Source