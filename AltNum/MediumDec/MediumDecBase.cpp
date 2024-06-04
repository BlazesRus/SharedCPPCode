#include "MediumDecBase.hpp"
using MediumDecBase = BlazesRusCode::MediumDecBase;


#pragma region ValueDefine Source
MediumDecBase MediumDecBase::AlmostOne = MediumDecBase::AlmostOneValue();
MediumDecBase MediumDecBase::Pi = MediumDecBase::PiNumValue();
MediumDecBase MediumDecBase::One = MediumDecBase::OneValue();
MediumDecBase MediumDecBase::Two = MediumDecBase::TwoValue();
MediumDecBase MediumDecBase::NegativeOne = MediumDecBase::NegativeOneValue();
MediumDecBase MediumDecBase::Zero = MediumDecBase::ZeroValue();
MediumDecBase MediumDecBase::PointFive = MediumDecBase::Point5Value();
MediumDecBase MediumDecBase::JustAboveZero = MediumDecBase::JustAboveZeroValue();
MediumDecBase MediumDecBase::OneMillionth = MediumDecBase::OneMillionthValue();
MediumDecBase MediumDecBase::FiveThousandth = MediumDecBase::FiveThousandthValue();
MediumDecBase MediumDecBase::Minimum = MediumDecBase::MinimumValue();
MediumDecBase MediumDecBase::Maximum = MediumDecBase::MaximumValue();
MediumDecBase MediumDecBase::E = MediumDecBase::ENumValue();
MediumDecBase MediumDecBase::LN10 = MediumDecBase::LN10Value();
MediumDecBase MediumDecBase::LN10Div = MediumDecBase::LN10DivValue();
MediumDecBase MediumDecBase::TwiceLN10Div = MediumDecBase::TwiceLN10DivValue();
MediumDecBase MediumDecBase::TenMillionth = MediumDecBase::TenMillionthValue();
MediumDecBase MediumDecBase::FiveMillionth = MediumDecBase::FiveMillionthValue();
MediumDecBase MediumDecBase::FiveBillionth = MediumDecBase::FiveBillionthValue();
MediumDecBase MediumDecBase::OneGMillionth = MediumDecBase::OneHundredMillionthValue();

MediumDecBase MediumDecBase::PiNum = MediumDecBase::PiNumValue();
MediumDecBase MediumDecBase::ENum = MediumDecBase::ENumValue();
MediumDecBase MediumDecBase::NegativePointFive = MediumDecBase::NegativePointFiveValue();
#pragma endregion ValueDefine Source

#pragma region NormalRep Integer Addition Operations

inline void MediumDecBase::UnsignedIntegerAddition(const MirroredInt& rValue)
{
	if (DecimalHalf.Value == 0)
		IntHalf.NRepSkippingUnsignedAddOp(rValue);
	else {
		int signBeforeOp = IntHalf.Sign;
		IntHalf.UIntAddOp(rValue.Value);
		if (signBeforeOp != IntHalf.Sign)//Invert the decimal section
			DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
	}
}

inline void MediumDecBase::IntegerAddition(const MirroredInt& rValue)
{
	if (DecimalHalf.Value == 0) {
		IntHalf.NRepSkippingAddOp(rValue);
	}
	else {
		int signBeforeOp = IntHalf.Sign;
		IntHalf += rValue;
		if (signBeforeOp != IntHalf.Sign)//Invert the decimal section
			DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
	}
}

inline void MediumDecBase::UIntAddOp(const unsigned int& rValue)
{
	{
		if (DecimalHalf.Value == 0)
			IntHalf.NRepSkippingUnsignedAddOp(rValue);
		else {
			int signBeforeOp = IntHalf.Sign;
			IntHalf.UIntAddOp(rValue);
			if (signBeforeOp != IntHalf.Sign)//Invert the decimal section
				DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
		}
	}
}

inline MediumDecBase& BlazesRusCode::MediumDecBase::UIntAddOperation(const unsigned int& rValue)
{
	UIntAddOp(rValue); return *this;
}

#pragma endregion NormalRep Integer Addition Operations