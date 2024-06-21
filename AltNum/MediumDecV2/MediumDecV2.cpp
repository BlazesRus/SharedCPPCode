#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;

#pragma region ValueSetters
#pragma endregion ValueSetters

#pragma region ValueDefines

inline MediumDecV2 BlazesRusCode::MediumDecV2::AlmostOneValue()
{
	return MediumDecV2(0, 999999999);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::PiNumValue()
{
	return MediumDecV2(3, 141592654);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::HundredMilPiNumValue()
{
	return MediumDecV2(314159265, 358979324);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::TenMilPiNumValue()
{
	return MediumDecV2(31415926, 535897932);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::OneMilPiNumValue()
{
	return MediumDecV2(3141592, 653589793);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::TenPiNumValue()
{
	return MediumDecV2(31, 415926536);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::ENumValue()
{
	return MediumDecV2(2, 718281828);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::ZeroValue()
{
	return MediumDecV2();
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::OneValue()
{
	return MediumDecV2(1);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::TwoValue()
{
	return MediumDecV2(MirroredInt::Two);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::NegativeOneValue()
{
	return MediumDecV2(MirroredInt::NegativeOne);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::Point5Value()
{
	return MediumDecV2(0, 500000000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::JustAboveZeroValue()
{
	return MediumDecV2(0, 1);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::OneMillionthValue()
{
	return MediumDecV2(0, 1000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::FiveThousandthValue()
{
	return MediumDecV2(0, 5000000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::FiveMillionthValue()
{
	return MediumDecV2(0, 5000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::TenMillionthValue()
{
	return MediumDecV2(0, 100);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::OneHundredMillionthValue()
{
	return MediumDecV2(0, 10);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::FiveBillionthValue()
{
	return MediumDecV2(0, 5);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::LN10Value()
{
	return MediumDecV2(2, 302585093);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::LN10DivValue()
{
	return MediumDecV2(0, 434294482);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::TwiceLN10DivValue()
{
	return MediumDecV2(0, 868588964);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::MinimumValue()
{
	return MediumDecV2(MirroredInt::Maximum, 999999999);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::MaximumValue()
{
	return MediumDecV2(MirroredInt::Minimum, 999999999);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::NegativePointFiveValue()
{
	return MediumDecV2(MirroredInt::NegativeZero, 500000000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::PointOneValue()
{
	return MediumDecV2(0, 100000000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::ImaginaryMinimumValue()
{
	return MediumDecV2(MirroredInt::Maximum, PartialInt(999999999,3));
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::ImaginaryMaximumValue()
{
	return MediumDecV2(MirroredInt::Minimum, PartialInt(999999999,3));
}

#pragma endregion ValueDefines

#pragma region ValueDefine Source
MediumDecV2 MediumDecV2::AlmostOne = MediumDecV2::AlmostOneValue();
MediumDecV2 MediumDecV2::Pi = MediumDecV2::PiNumValue();
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
MediumDecV2 MediumDecV2::E = MediumDecV2::ENumValue();
MediumDecV2 MediumDecV2::LN10 = MediumDecV2::LN10Value();
MediumDecV2 MediumDecV2::LN10Div = MediumDecV2::LN10DivValue();
MediumDecV2 MediumDecV2::TwiceLN10Div = MediumDecV2::TwiceLN10DivValue();
MediumDecV2 MediumDecV2::TenMillionth = MediumDecV2::TenMillionthValue();
MediumDecV2 MediumDecV2::FiveMillionth = MediumDecV2::FiveMillionthValue();
MediumDecV2 MediumDecV2::FiveBillionth = MediumDecV2::FiveBillionthValue();
MediumDecV2 MediumDecV2::OneGMillionth = MediumDecV2::OneHundredMillionthValue();

MediumDecV2 MediumDecV2::PiNum = MediumDecV2::PiNumValue();
MediumDecV2 MediumDecV2::ENum = MediumDecV2::ENumValue();
MediumDecV2 MediumDecV2::NegativePointFive = MediumDecV2::NegativePointFiveValue();
MediumDecV2 MediumDecV2::PointOne = MediumDecV2::PointOneValue();
MediumDecV2 MediumDecV2::ImaginaryMinimum = MediumDecV2::ImaginaryMinimumValue();
MediumDecV2 MediumDecV2::ImaginaryMaximum = MediumDecV2::ImaginaryMaximumValue();
#pragma endregion ValueDefine Source

#pragma region Check_if_value
#pragma endregion Check_if_value

#pragma region RangeLimits
#pragma endregion RangeLimits

#pragma region ConvertToOtherTypes
#pragma endregion ConvertToOtherTypes

#pragma region Comparison Operators
#pragma endregion Comparison Operators

#pragma region Other Division Operations

inline void BlazesRusCode::MediumDecV2::DivideByTwo()
{
	MediumDec::DivideByTwo();
}

inline void BlazesRusCode::MediumDecV2::DivideByFour()
{
	MediumDec::DivideByFour();
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::DividedByTwo() const
{
	MediumDecV2 result = *this; result.DivideByTwo();
	return result;
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::DividedByFour() const
{
	MediumDecV2 result = *this; result.DivideByFour();
	return result;
}

inline void BlazesRusCode::MediumDecV2::UnsignedDivOp_RValueIntSwitch(const MediumDecV2& rValue)
{
	switch(rValue.IntHalf.Value)
	{
		case 2:
			if((IntHalf.Value&1)==1)//Check if number is odd
				BasicUIntDivOp(2);
			else
				IntHalf.Value /= 2;
			break;
		case 4:
			if(((IntHalf.Value >> 2) << 2) == IntHalf.Value)
				IntHalf.Value /= 4;
			else
				BasicUIntDivOp(4);
			break;
		case 8:
			if(((IntHalf.Value >> 3) << 3) == IntHalf)
				IntHalf.Value /= 8;
			else
				BasicUIntDivOp(4);
			break;
		case 16:
			if(((IntHalf.Value >> 4) << 4) == IntHalf.Value)
				IntHalf.Value /= 16;
			else
				BasicUIntDivOp(4);
			break;
		case 32:
			if(((IntHalf.Value >> 5) << 5) == IntHalf.Value)
				IntHalf.Value /= 32;
			else
				BasicUIntDivOp(4);
			break;
		case 0:
			throw "Target value can not be divided by zero";
			break;
		default:
			BasicUIntDivOp(rValue.IntHalf.Value);
			break;
	}
}

#pragma endregion Other Division Operations

#pragma region Other multiplication operations

inline void BlazesRusCode::MediumDecV2::MultiplyByTwo()
{
	MediumDec::MultiplyByTwo();
}

inline void BlazesRusCode::MediumDecV2::MultiplyByFour()
{
	MediumDec::MultiplyByFour();
}


inline MediumDecV2 BlazesRusCode::MediumDecV2::MultipliedByTwo() const
{
	MediumDecV2 result = *this; result.UIntMultOp(2);
	return result;
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::MultipliedByFour() const
{
	MediumDecV2 result = *this; result.UIntMultOp(4);
	return result;
}


#pragma endregion Other multiplication operations

#pragma region Truncation Functions

inline MediumDecV2 BlazesRusCode::MediumDecV2::Abs(const MediumDecV2& tValue) { return AbsV1<MediumDecV2>(tValue); }

inline MediumDecV2 BlazesRusCode::MediumDecV2::AbsOf() const { return Abs(*this); }

inline MediumDecV2 BlazesRusCode::MediumDecV2::Ceil(const MediumDecV2& tValue) { return CeilV1<MediumDecV2>(tValue); }

inline MediumDecV2 BlazesRusCode::MediumDecV2::CeilOf() const { return Ceil(*this); }

inline signed int BlazesRusCode::MediumDecV2::FloorInt(const MediumDecV2& tValue) { return tValue.FloorIntOf(); }

inline signed int BlazesRusCode::MediumDecV2::CeilInt(const MediumDecV2& tValue) { return tValue.CeilIntOf(); }

inline MediumDecV2 BlazesRusCode::MediumDecV2::Trunc(const MediumDecV2& tValue) { return tValue.TruncOfV1<MediumDecV2>(); }

#pragma endregion Truncation Functions

#pragma region String Function Source

std::string MediumDecV2::ToString()
{
	RepType LRep = GetRepType();
	switch(LRep)
	{
		case RepTypeEnum::NormalType:
			return ToBasicString(); break;
#if defined(AltNum_EnablePiRep)
		case RepTypeEnum::PiNum:
			return ToBasicString()+"π";
			break;
#endif
#if defined(AltNum_EnableERep)
		case RepTypeEnum::ENum:
			return ToBasicString()+"e";
			break;
#endif
#if defined(AltNum_EnableIRep)
		case RepTypeEnum::INum:
			return ToBasicString()+"i";
			break;
#endif
#if defined(AltNum_EnableApproaching)
		case RepTypeEnum::ApproachingBottom:
			return IntHalf.ToString()+"0..01";
			break;
		case RepTypeEnum::ApproachingTop:
			return IntHalf.ToString()+"9..9";
			break;
	#if defined(AltNum_EnablePiRep)
		case RepTypeEnum::ApproachingBottomPi:
			return IntHalf.ToString()+"0..01π";
			break;
		case RepTypeEnum::ApproachingTopPi:
			return IntHalf.ToString()+"9..9π";
			break;
	#endif
	#if defined(AltNum_EnableERep)
		case RepTypeEnum::ApproachingBottomE:
			return IntHalf.ToString()+"0..01e";
			break;
		case RepTypeEnum::ApproachingTopE:
			return IntHalf.ToString()+"9..9e";
			break;
	#endif
	#if defined(AltNum_EnableIRep)
		case RepTypeEnum::ApproachingImaginaryBottom:
			return IntHalf.ToString()+"0..01i";
			break;
		case RepTypeEnum::ApproachingImaginaryTop:
			return IntHalf.ToString()+"9..9i";
			break;
	#endif
#endif
#if defined(AltNum_EnableInfinityRep)
		case RepTypeEnum::Infinity:
			return IsNegative()"-∞"?"∞";
			break;
	#if defined(AltNum_EnableIRep)
		case RepTypeEnum::ImaginaryInfinity:
			return IsNegative()"-∞i"?"∞i";
			break;
	#endif
#endif
#if defined(AltNum_EnableNaN)
		case RepTypeEnum::Nil:
			return "Nil"; break;
		case RepTypeEnum::Undefined:
			return "Undefined"; break;
		case RepTypeEnum::NaN:
			return "NaN"; break;
#endif
		default:
			return "";
	}
}

std::string MediumDecV2::ToFullString()
{
	RepType LRep = GetRepType();
	switch(LRep)
	{
		case RepTypeEnum::NormalType:
			return ToFullBasicString(); break;
#if defined(AltNum_EnablePiRep)
		case RepTypeEnum::PiNum:
			return ToFullBasicString()+"π";
			break;
#endif
#if defined(AltNum_EnableERep)
		case RepTypeEnum::ENum:
			return ToFullBasicString()+"e";
			break;
#endif
#if defined(AltNum_EnableIRep)
		case RepTypeEnum::INum:
			return ToFullBasicString()+"i";
			break;
#endif
#if defined(AltNum_EnableApproaching)
		case RepTypeEnum::ApproachingBottom:
			return IntHalf.ToString()+"0..01";
			break;
		case RepTypeEnum::ApproachingTop:
			return IntHalf.ToString()+"9..9";
			break;
	#if defined(AltNum_EnablePiRep)
		case RepTypeEnum::ApproachingBottomPi:
			return IntHalf.ToString()+"0..01π";
			break;
		case RepTypeEnum::ApproachingTopPi:
			return IntHalf.ToString()+"9..9π";
			break;
	#endif
	#if defined(AltNum_EnableERep)
		case RepTypeEnum::ApproachingBottomE:
			return IntHalf.ToString()+"0..01e";
			break;
		case RepTypeEnum::ApproachingTopE:
			return IntHalf.ToString()+"9..9e";
			break;
	#endif
	#if defined(AltNum_EnableIRep)
		case RepTypeEnum::ApproachingImaginaryBottom:
			return IntHalf.ToString()+"0..01i";
			break;
		case RepTypeEnum::ApproachingImaginaryTop:
			return IntHalf.ToString()+"9..9i";
			break;
	#endif
#endif
#if defined(AltNum_EnableInfinityRep)
		case RepTypeEnum::Infinity:
			return IsNegative()"-∞"?"∞";
			break;
	#if defined(AltNum_EnableIRep)
		case RepTypeEnum::ImaginaryInfinity:
			return IsNegative()"-∞i"?"∞i";
			break;
	#endif
#endif
#if defined(AltNum_EnableNaN)
		case RepTypeEnum::Nil:
			return "Nil"; break;
		case RepTypeEnum::Undefined:
			return "Undefined"; break;
		case RepTypeEnum::NaN:
			return "NaN"; break;
#endif
		default:
			return "";
	}
}

#pragma endregion String Function Source

#pragma region Pow and Sqrt Functions

inline MediumDecV2 BlazesRusCode::MediumDecV2::Sqrt(const auto& value, const int& precision)
{
	return SqrtV1<MediumDecV2>(value, precision);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::SqrtOf(const int& precision) const {
	return Sqrt(*this, precision);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::UnsignedNthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision)
{
	return UnsignedNthRootV1<MediumDecV2>(tValue, n, precision);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::NthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision) { return NthRootV1<MediumDecV2>(tValue, n, precision); }

inline MediumDecV2 BlazesRusCode::MediumDecV2::NthRootOf(const unsigned int& n, const MediumDecV2& precision) const
{
	return NthRoot(*this, n, precision);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::AlternativeNthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision)
{
	return NthRootV2<MediumDecV2>(tValue, n, precision);
}

#pragma endregion Pow and Sqrt Functions