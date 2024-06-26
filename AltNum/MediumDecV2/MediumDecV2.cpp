#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;
using RepType = BlazesRusCode::RepType;

#pragma region ValueSetters
#pragma endregion ValueSetters

#pragma region ValueDefines

MediumDecV2 BlazesRusCode::MediumDecV2::AlmostOneValue()
{
	return MediumDecV2(0, 999999999);
}

MediumDecV2 BlazesRusCode::MediumDecV2::PiNumValue()
{
	return MediumDecV2(3, 141592654);
}

MediumDecV2 BlazesRusCode::MediumDecV2::HundredMilPiNumValue()
{
	return MediumDecV2(314159265, 358979324);
}

MediumDecV2 BlazesRusCode::MediumDecV2::TenMilPiNumValue()
{
	return MediumDecV2(31415926, 535897932);
}

MediumDecV2 BlazesRusCode::MediumDecV2::OneMilPiNumValue()
{
	return MediumDecV2(3141592, 653589793);
}

MediumDecV2 BlazesRusCode::MediumDecV2::TenPiNumValue()
{
	return MediumDecV2(31, 415926536);
}

MediumDecV2 BlazesRusCode::MediumDecV2::ENumValue()
{
	return MediumDecV2(2, 718281828);
}

MediumDecV2 BlazesRusCode::MediumDecV2::ZeroValue()
{
	return MediumDecV2();
}

MediumDecV2 BlazesRusCode::MediumDecV2::OneValue()
{
	return MediumDecV2(1);
}

MediumDecV2 BlazesRusCode::MediumDecV2::TwoValue()
{
	return MediumDecV2(MirroredInt::Two);
}

MediumDecV2 BlazesRusCode::MediumDecV2::NegativeOneValue()
{
	return MediumDecV2(MirroredInt::NegativeOne);
}

MediumDecV2 BlazesRusCode::MediumDecV2::Point5Value()
{
	return MediumDecV2(0, 500000000);
}

MediumDecV2 BlazesRusCode::MediumDecV2::JustAboveZeroValue()
{
	return MediumDecV2(0, 1);
}

MediumDecV2 BlazesRusCode::MediumDecV2::OneMillionthValue()
{
	return MediumDecV2(0, 1000);
}

MediumDecV2 BlazesRusCode::MediumDecV2::FiveThousandthValue()
{
	return MediumDecV2(0, 5000000);
}

MediumDecV2 BlazesRusCode::MediumDecV2::FiveMillionthValue()
{
	return MediumDecV2(0, 5000);
}

MediumDecV2 BlazesRusCode::MediumDecV2::TenMillionthValue()
{
	return MediumDecV2(0, 100);
}

MediumDecV2 BlazesRusCode::MediumDecV2::OneHundredMillionthValue()
{
	return MediumDecV2(0, 10);
}

MediumDecV2 BlazesRusCode::MediumDecV2::FiveBillionthValue()
{
	return MediumDecV2(0, 5);
}

MediumDecV2 BlazesRusCode::MediumDecV2::LN10Value()
{
	return MediumDecV2(2, 302585093);
}

MediumDecV2 BlazesRusCode::MediumDecV2::LN10DivValue()
{
	return MediumDecV2(0, 434294482);
}

MediumDecV2 BlazesRusCode::MediumDecV2::TwiceLN10DivValue()
{
	return MediumDecV2(0, 868588964);
}

MediumDecV2 BlazesRusCode::MediumDecV2::MinimumValue()
{
	return MediumDecV2(MirroredInt::Maximum, 999999999);
}

MediumDecV2 BlazesRusCode::MediumDecV2::MaximumValue()
{
	return MediumDecV2(MirroredInt::Minimum, 999999999);
}

MediumDecV2 BlazesRusCode::MediumDecV2::NegativePointFiveValue()
{
	return MediumDecV2(MirroredInt::NegativeZero, 500000000);
}

MediumDecV2 BlazesRusCode::MediumDecV2::PointOneValue()
{
	return MediumDecV2(0, 100000000);
}

MediumDecV2 BlazesRusCode::MediumDecV2::ImaginaryMinimumValue()
{
	return MediumDecV2(MirroredInt::Maximum, PartialInt(999999999,3));
}

MediumDecV2 BlazesRusCode::MediumDecV2::ImaginaryMaximumValue()
{
	return MediumDecV2(MirroredInt::Minimum, PartialInt(999999999,3));
}

#pragma endregion ValueDefines

#pragma region ValueDefine Source
const MediumDecV2 MediumDecV2::AlmostOne = MediumDecV2::AlmostOneValue();
const MediumDecV2 MediumDecV2::Pi = MediumDecV2::PiNumValue();
const MediumDecV2 MediumDecV2::One = MediumDecV2::OneValue();
const MediumDecV2 MediumDecV2::Two = MediumDecV2::TwoValue();
const MediumDecV2 MediumDecV2::NegativeOne = MediumDecV2::NegativeOneValue();
const MediumDecV2 MediumDecV2::Zero = MediumDecV2::ZeroValue();
const MediumDecV2 MediumDecV2::PointFive = MediumDecV2::Point5Value();
const MediumDecV2 MediumDecV2::JustAboveZero = MediumDecV2::JustAboveZeroValue();
const MediumDecV2 MediumDecV2::OneMillionth = MediumDecV2::OneMillionthValue();
const MediumDecV2 MediumDecV2::FiveThousandth = MediumDecV2::FiveThousandthValue();
const MediumDecV2 MediumDecV2::Minimum = MediumDecV2::MinimumValue();
const MediumDecV2 MediumDecV2::Maximum = MediumDecV2::MaximumValue();
const MediumDecV2 MediumDecV2::E = MediumDecV2::ENumValue();
const MediumDecV2 MediumDecV2::LN10 = MediumDecV2::LN10Value();
const MediumDecV2 MediumDecV2::LN10Div = MediumDecV2::LN10DivValue();
const MediumDecV2 MediumDecV2::TwiceLN10Div = MediumDecV2::TwiceLN10DivValue();
const MediumDecV2 MediumDecV2::TenMillionth = MediumDecV2::TenMillionthValue();
const MediumDecV2 MediumDecV2::FiveMillionth = MediumDecV2::FiveMillionthValue();
const MediumDecV2 MediumDecV2::FiveBillionth = MediumDecV2::FiveBillionthValue();
const MediumDecV2 MediumDecV2::OneGMillionth = MediumDecV2::OneHundredMillionthValue();

const MediumDecV2 MediumDecV2::PiNum = MediumDecV2::PiNumValue();
const MediumDecV2 MediumDecV2::ENum = MediumDecV2::ENumValue();
const MediumDecV2 MediumDecV2::NegativePointFive = MediumDecV2::NegativePointFiveValue();
const MediumDecV2 MediumDecV2::PointOne = MediumDecV2::PointOneValue();
const MediumDecV2 MediumDecV2::ImaginaryMinimum = MediumDecV2::ImaginaryMinimumValue();
const MediumDecV2 MediumDecV2::ImaginaryMaximum = MediumDecV2::ImaginaryMaximumValue();
#pragma endregion ValueDefine Source

#pragma region Negative_Status

bool BlazesRusCode::MediumDecV2::IsPositive() const
{
	return IntHalf.IsPositive();
}

bool BlazesRusCode::MediumDecV2::IsNegative() const
{
	return IntHalf.IsNegative();
}

void BlazesRusCode::MediumDecV2::SwapNegativeStatus()
{
	IntHalf.Sign ^= 1;
}

#pragma endregion Negative_Status

#pragma region Check_if_value

#pragma endregion Check_if_value

#pragma region RangeLimits
#pragma endregion RangeLimits

#pragma region ConvertToOtherTypes
#pragma endregion ConvertToOtherTypes

#pragma region Comparison Operators
#pragma endregion Comparison Operators

#pragma region Other Division Operations

void BlazesRusCode::MediumDecV2::DivideByTwo()
{
	MediumDec::DivideByTwo();
}

void BlazesRusCode::MediumDecV2::DivideByFour()
{
	MediumDec::DivideByFour();
}

MediumDecV2 BlazesRusCode::MediumDecV2::DividedByTwo() const
{
	MediumDecV2 result = *this; result.DivideByTwo();
	return result;
}

MediumDecV2 BlazesRusCode::MediumDecV2::DividedByFour() const
{
	MediumDecV2 result = *this; result.DivideByFour();
	return result;
}

void BlazesRusCode::MediumDecV2::UnsignedDivOp_RValueIntSwitch(const MediumDecV2& rValue)
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

void BlazesRusCode::MediumDecV2::MultiplyByTwo()
{
	MediumDec::MultiplyByTwo();
}

void BlazesRusCode::MediumDecV2::MultiplyByFour()
{
	MediumDec::MultiplyByFour();
}


MediumDecV2 BlazesRusCode::MediumDecV2::MultipliedByTwo() const
{
	MediumDecV2 result = *this; result.UIntMultOp(2);
	return result;
}

MediumDecV2 BlazesRusCode::MediumDecV2::MultipliedByFour() const
{
	MediumDecV2 result = *this; result.UIntMultOp(4);
	return result;
}


#pragma endregion Other multiplication operations

#pragma region Basic_addition

void MediumDecV2::BasicUnsignedMirroredAddOp(const MirroredInt& rValue)
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

void MediumDecV2::BasicMirroredAddOp(const MirroredInt& rValue)
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

void BlazesRusCode::MediumDecV2::BasicUnsignedAddOp(const MediumDecV2& rValue) {
	if (rValue.DecimalHalf == 0)
		BasicUnsignedMirroredAddOp(rValue.IntHalf);
	else
	{
		int signBeforeOp = IntHalf.Sign;
		IntHalf.UnsignedAddOp(rValue.IntHalf);
		if (signBeforeOp == MirroredInt::NegativeSign)
		{
			if (DecimalHalf.Value == rValue.DecimalHalf.Value) {//5.5 + -4.5
				if (IntHalf.Value == 0)
					SetAsZero();
				else
					DecimalHalf.Value = 0;
			}
			else if (rValue.DecimalHalf.Value>DecimalHalf.Value)
			{
				++IntHalf;
				if (signBeforeOp != IntHalf.Sign)//-1.6 + 2.7 = 1.1
					DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
				else//-1.6 + .7 = -0.9
					DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
			}
			else if (signBeforeOp != IntHalf.Sign)//-1.6 + 2.5 = 0.9
				DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10
			else
				DecimalHalf.Value -= rValue.DecimalHalf.Value;
		}
		else {
			unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
			if (decResult == DecimalOverflow) {//5.4 + 4.6
				++IntHalf;
				if (IntHalf.Value == 0)
					SetAsZero();
				else
					DecimalHalf.Value = 0;
			}
			else if (decResult>DecimalOverflow) {//5.4 + 4.7
				++IntHalf;
				DecimalHalf.Value = decResult - DecimalOverflow;
			}
			else if (signBeforeOp != IntHalf.Sign)
				DecimalHalf.Value = DecimalOverflow - decResult;
			else
				DecimalHalf.Value = decResult;
		}
	}
}

void BlazesRusCode::MediumDecV2::BasicAddOp(const MediumDecV2& rValue) {
	if (rValue.DecimalHalf == 0)
		BasicMirroredAddOp(rValue.IntHalf);
	else {
		int signBeforeOp = IntHalf.Sign;
		IntHalf += rValue.IntHalf;

		if (signBeforeOp == MirroredInt::NegativeSign) {
			if (rValue.IsPositive()) {
				if (DecimalHalf.Value == rValue.DecimalHalf.Value) {
					if (IntHalf.Value == 0)
						SetAsZero();
					else
						DecimalHalf.Value = 0;
				}
				else if (rValue.DecimalHalf.Value>DecimalHalf.Value) {
					++IntHalf;
					if (signBeforeOp != IntHalf.Sign)//-1.6 + 2.7 = 1.1
						DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
					else//-1.6 + .7 = -0.9
						DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
				}
				else if (signBeforeOp != IntHalf.Sign)//-1.6 + 2.5 = 0.9
					DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10
				else
					DecimalHalf.Value -= rValue.DecimalHalf.Value;
			}
			else {
				unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
				if (decResult == DecimalOverflow) {//-5.4 + - 5.6
					--IntHalf;
					if (IntHalf.Value == 0)
						SetAsZero();
					else
						DecimalHalf.Value = 0;
				}
				else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1
					--IntHalf;
					DecimalHalf.Value = decResult - DecimalOverflow;
				}
				else//-5.2 - 5.2 = -10.4
					DecimalHalf.Value = decResult;
			}
		}
		else {
			if (rValue.IsPositive()) {
				unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
				if (decResult == DecimalOverflow) {//5.5 + 4.5 = 10
					++IntHalf;
					if (IntHalf.Value == 0)
						SetAsZero();
					else
						DecimalHalf.Value = 0;
				}
				else if (decResult>DecimalOverflow) {//5.5 + 4.6 = 10.1
					++IntHalf;
					DecimalHalf.Value = decResult - DecimalOverflow;
				}
				else//5.4 + 5.3 = 10.7
					DecimalHalf.Value = decResult;
			}
			else {
				if (DecimalHalf.Value == rValue.DecimalHalf.Value) {//5.5 + -5.5
					if (IntHalf.Value == 0)
						SetAsZero();
					else
						DecimalHalf.Value = 0;
				}
				else if (rValue.DecimalHalf.Value>DecimalHalf.Value) {
					--IntHalf;
					if (signBeforeOp != IntHalf.Sign)//4.3 - 5.4 = -1.1
						DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
					else//4.3 - 2.4 = 1.9
						DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
				}
				else if (signBeforeOp != IntHalf.Sign)//5.4 + - 6.3 = -0.9
					DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 + 3 - 4
				else//4.4 + -2.3 = 2.1
					DecimalHalf.Value -= rValue.DecimalHalf.Value;
			}
		}
	}
}

#pragma endregion Basic_addition

#pragma region Basic_subtraction

void BlazesRusCode::MediumDecV2::BasicUnsignedMirroredSubOp(const MirroredInt& rValue)
{
	if (DecimalHalf.Value == 0)
		IntHalf.NRepSkippingUnsignedSubOp(rValue);
	else {
		int signBeforeOp = IntHalf.Sign;
		IntHalf.UIntSubOp(rValue.Value);
		if (signBeforeOp != IntHalf.Sign)//Invert the decimal section
			DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
	}
}

void BlazesRusCode::MediumDecV2::BasicMirroredSubOp(const MirroredInt& rValue)
{
	if (DecimalHalf.Value == 0)
		IntHalf.NRepSkippingSubOp(rValue);
	else {
		unsigned int signBeforeOp = IntHalf.Sign;
		IntHalf += rValue;
		if (signBeforeOp != IntHalf.Sign)//Invert the decimal section
			DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
	}
}

void BlazesRusCode::MediumDecV2::BasicUnsignedSubOp(const MediumDecV2& rValue) {
    if (rValue.DecimalHalf == 0)
        BasicUnsignedMirroredSubOp(rValue.IntHalf);
	else {
		int signBeforeOp = IntHalf.Sign;
		IntHalf.UnsignedSubOp(rValue.IntHalf);
        if (signBeforeOp==MirroredInt::NegativeSign){//-5 - B
			unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
            if (decResult == DecimalOverflow){//-5.4 - 5.6
                --IntHalf;
                if (IntHalf.Value == 0)
                    SetAsZero();
                else
                    DecimalHalf.Value = 0;
            } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1
				--IntHalf;
				DecimalHalf.Value = decResult - DecimalOverflow;
			} else//-5.2 - 5.2 = -10.4
				DecimalHalf.Value = decResult;
        } else {//5.XX - B
			if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - 5.5 = 10
				if(IntHalf.Value==0)
					SetAsZero();
				else
					DecimalHalf.Value = 0;
			} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
				--IntHalf;
				if(signBeforeOp!=IntHalf.Sign)//5.4 - 5.7 = -0.3
					DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
				else//5.4 - 3.6 = 1.8
					DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
			} else if(signBeforeOp!=IntHalf.Sign)//5.3 - 7.2 = -1.9
				DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
			else//5.4 - 5.3 = 0.1
				DecimalHalf.Value -= rValue.DecimalHalf.Value;
        }
	}
}

void BlazesRusCode::MediumDecV2::BasicSubOp(const MediumDecV2& rValue) {
	if(rValue.DecimalHalf==0)
        BasicMirroredSubOp(rValue.IntHalf);
	else
	{
		int signBeforeOp = IntHalf.Sign;
		IntHalf -= rValue.IntHalf;
        if (signBeforeOp==MirroredInt::NegativeSign)
		{
			if(rValue.IsPositive())
			{
				unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
				if(decResult==DecimalOverflow){//-5.4 - 5.6
					--IntHalf;
					if(IntHalf.Value==0)
						SetAsZero();
					else
						DecimalHalf.Value = 0;
				} else if(decResult>DecimalOverflow){//-5.4 - 5.7 = -11.1
					--IntHalf;
					DecimalHalf.Value = decResult - DecimalOverflow;
				} else//-5.2 - 5.2 = -10.4
					DecimalHalf.Value = decResult;
			} else {
				if(DecimalHalf.Value==rValue.DecimalHalf.Value){//-5.4 - -4.4
					if(IntHalf.Value==0)
						SetAsZero();
					else
						DecimalHalf.Value = 0;
				} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
					++IntHalf;
					if(signBeforeOp!=IntHalf.Sign)//-5.4 - -6.5 = 1.1
						DecimalHalf = DecimalOverflow - DecimalHalf.Value - rValue.DecimalHalf;
					else//-5.4 - -4.5 = -0.9 == -5.4 + 4.5
						DecimalHalf.Value += rValue.DecimalHalf.Value;
				} else if(signBeforeOp!=IntHalf.Sign)//-5.4 - -7.3 = 1.9
					DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;
				else//-5.4 - -3.3 = -2.1
					DecimalHalf.Value -= rValue.DecimalHalf.Value;
			}
        } else {
			if(rValue.IsPositive()){
                if (DecimalHalf.Value == rValue.DecimalHalf.Value) {//5.5 - 5.5 = 10
                    if (IntHalf.Value == 0)
                        SetAsZero();
                    else
                        DecimalHalf.Value = 0;
                }
                else if (rValue.DecimalHalf.Value > DecimalHalf.Value) {
                    --IntHalf;
                    if (signBeforeOp != IntHalf.Sign)//5.4 - 5.7 = -0.3
                        DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                    else//5.4 - 3.6 = 1.8
                        DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                }
                else if (signBeforeOp != IntHalf.Sign)//5.3 - 7.2 = -1.9
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
                else//5.4 - 5.3 = 0.1
                    DecimalHalf.Value -= rValue.DecimalHalf.Value;
			} else {
				if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - -5.5 = 11
					++IntHalf;
					if(IntHalf.Value==0)
						SetAsZero();
					else
						DecimalHalf.Value = 0;
				} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){//5.4 - -5.7 = 11.1
					++IntHalf;
					DecimalHalf.Value = DecimalHalf.Value + rValue.DecimalHalf.Value - DecimalOverflow;
				} else//5.4 - -5.3 = 10.7
					DecimalHalf.Value += rValue.DecimalHalf.Value;
			}
        }
	}
}

#pragma endregion Basic_subtraction

#pragma region Truncation Functions

MediumDecV2 BlazesRusCode::MediumDecV2::Abs(const MediumDecV2& tValue) { return AbsV1<MediumDecV2>(tValue); }

MediumDecV2 BlazesRusCode::MediumDecV2::AbsOf() const { return Abs(*this); }

MediumDecV2 BlazesRusCode::MediumDecV2::Ceil(const MediumDecV2& tValue) { return CeilV1<MediumDecV2>(tValue); }

MediumDecV2 BlazesRusCode::MediumDecV2::CeilOf() const { return Ceil(*this); }

signed int BlazesRusCode::MediumDecV2::FloorInt(const MediumDecV2& tValue) { return tValue.FloorIntOf(); }

signed int BlazesRusCode::MediumDecV2::CeilInt(const MediumDecV2& tValue) { return tValue.CeilIntOf(); }

MediumDecV2 BlazesRusCode::MediumDecV2::Trunc(const MediumDecV2& tValue) { return tValue.TruncOfV1<MediumDecV2>(); }

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

MediumDecV2 BlazesRusCode::MediumDecV2::Sqrt(const auto& value, const int& precision)
{
	return SqrtV1<MediumDecV2>(value, precision);
}

MediumDecV2 BlazesRusCode::MediumDecV2::SqrtOf(const int& precision) const {
	return Sqrt(*this, precision);
}

MediumDecV2 BlazesRusCode::MediumDecV2::UnsignedNthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision)
{
	return UnsignedNthRootV1<MediumDecV2>(tValue, n, precision);
}

MediumDecV2 BlazesRusCode::MediumDecV2::NthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision) { return NthRootV1<MediumDecV2>(tValue, n, precision); }

MediumDecV2 BlazesRusCode::MediumDecV2::NthRootOf(const unsigned int& n, const MediumDecV2& precision) const
{
	return NthRoot(*this, n, precision);
}

MediumDecV2 BlazesRusCode::MediumDecV2::AlternativeNthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision)
{
	return NthRootV2<MediumDecV2>(tValue, n, precision);
}

#pragma endregion Pow and Sqrt Functions