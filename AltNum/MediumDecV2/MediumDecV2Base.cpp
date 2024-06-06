#include "MediumDecV2Base.hpp"
using MediumDecV2Base = BlazesRusCode::MediumDecV2Base;

#pragma region ValueSetters

inline void BlazesRusCode::MediumDecV2Base::SetValueToPiNum()
{
	IntHalf = 3; DecimalHalf = 141592654;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToHundredMilPiNum()
{
	IntHalf = 314159265; DecimalHalf = 358979324;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToTenMilPiNum()
{
	IntHalf = 31415926; DecimalHalf = 535897932;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToOneMilPiNum()
{
	IntHalf = 3141592; DecimalHalf = 653589793;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToTenPiNum()
{
	IntHalf = 31; DecimalHalf = 415926536;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToENum()
{
	IntHalf = 2; DecimalHalf = 718281828;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToPoint5()
{
	IntHalf = 0; DecimalHalf = 500000000;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToJustAboveZero()
{
	IntHalf = 0; DecimalHalf = 1;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToOneMillionth()
{
	IntHalf = 0; DecimalHalf = 1000;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToFiveThousandth()
{
	IntHalf = 0; DecimalHalf = 5000000;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToFiveMillionth()
{
	IntHalf = 0; DecimalHalf = 5000;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToTenMillionth()
{
	IntHalf = 0; DecimalHalf = 100;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToOneHundredMillionth()
{
	IntHalf = 0; DecimalHalf = 10;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToLN10()
{
	IntHalf = 2; DecimalHalf = 302585093;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToLN10Div()
{
	IntHalf = 0; DecimalHalf = 434294482;
}

inline void BlazesRusCode::MediumDecV2Base::SetValueToTwiceLN10Div()
{
	IntHalf = 0; DecimalHalf = 868588964;
}

#pragma endregion ValueSetters

#pragma region ValueDefines

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::AlmostOneValue()
{
	return MediumDecV2Base(0, 999999999);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::PiNumValue()
{
	return MediumDecV2Base(3, 141592654);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::HundredMilPiNumValue()
{
	return MediumDecV2Base(314159265, 358979324);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::TenMilPiNumValue()
{
	return MediumDecV2Base(31415926, 535897932);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::OneMilPiNumValue()
{
	return MediumDecV2Base(3141592, 653589793);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::TenPiNumValue()
{
	return MediumDecV2Base(31, 415926536);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::ENumValue()
{
	return MediumDecV2Base(2, 718281828);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::ZeroValue()
{
	return MediumDecV2Base();
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::OneValue()
{
	return MediumDecV2Base(1);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::TwoValue()
{
	return MediumDecV2Base(MirroredInt::Two);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::NegativeOneValue()
{
	return MediumDecV2Base(MirroredInt::NegativeOne);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::Point5Value()
{
	return MediumDecV2Base(0, 500000000);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::JustAboveZeroValue()
{
	return MediumDecV2Base(0, 1);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::OneMillionthValue()
{
	return MediumDecV2Base(0, 1000);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::FiveThousandthValue()
{
	return MediumDecV2Base(0, 5000000);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::FiveMillionthValue()
{
	return MediumDecV2Base(0, 5000);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::TenMillionthValue()
{
	return MediumDecV2Base(0, 100);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::OneHundredMillionthValue()
{
	return MediumDecV2Base(0, 10);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::FiveBillionthValue()
{
	return MediumDecV2Base(0, 5);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::LN10Value()
{
	return MediumDecV2Base(2, 302585093);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::LN10DivValue()
{
	return MediumDecV2Base(0, 434294482);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::TwiceLN10DivValue()
{
	return MediumDecV2Base(0, 868588964);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::MinimumValue()
{
	return MediumDecV2Base(MirroredInt::Maximum, 999999999);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::MaximumValue()
{
	return MediumDecV2Base(MirroredInt::Minimum, 999999999);
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::NegativePointFiveValue()
{
	return MediumDecV2Base(MirroredInt::NegativeZero, 500000000);
}

#pragma endregion ValueDefines

#pragma region ValueDefine Source
MediumDecV2Base MediumDecV2Base::AlmostOne = MediumDecV2Base::AlmostOneValue();
MediumDecV2Base MediumDecV2Base::Pi = MediumDecV2Base::PiNumValue();
MediumDecV2Base MediumDecV2Base::One = MediumDecV2Base::OneValue();
MediumDecV2Base MediumDecV2Base::Two = MediumDecV2Base::TwoValue();
MediumDecV2Base MediumDecV2Base::NegativeOne = MediumDecV2Base::NegativeOneValue();
MediumDecV2Base MediumDecV2Base::Zero = MediumDecV2Base::ZeroValue();
MediumDecV2Base MediumDecV2Base::PointFive = MediumDecV2Base::Point5Value();
MediumDecV2Base MediumDecV2Base::JustAboveZero = MediumDecV2Base::JustAboveZeroValue();
MediumDecV2Base MediumDecV2Base::OneMillionth = MediumDecV2Base::OneMillionthValue();
MediumDecV2Base MediumDecV2Base::FiveThousandth = MediumDecV2Base::FiveThousandthValue();
MediumDecV2Base MediumDecV2Base::Minimum = MediumDecV2Base::MinimumValue();
MediumDecV2Base MediumDecV2Base::Maximum = MediumDecV2Base::MaximumValue();
MediumDecV2Base MediumDecV2Base::E = MediumDecV2Base::ENumValue();
MediumDecV2Base MediumDecV2Base::LN10 = MediumDecV2Base::LN10Value();
MediumDecV2Base MediumDecV2Base::LN10Div = MediumDecV2Base::LN10DivValue();
MediumDecV2Base MediumDecV2Base::TwiceLN10Div = MediumDecV2Base::TwiceLN10DivValue();
MediumDecV2Base MediumDecV2Base::TenMillionth = MediumDecV2Base::TenMillionthValue();
MediumDecV2Base MediumDecV2Base::FiveMillionth = MediumDecV2Base::FiveMillionthValue();
MediumDecV2Base MediumDecV2Base::FiveBillionth = MediumDecV2Base::FiveBillionthValue();
MediumDecV2Base MediumDecV2Base::OneGMillionth = MediumDecV2Base::OneHundredMillionthValue();

MediumDecV2Base MediumDecV2Base::PiNum = MediumDecV2Base::PiNumValue();
MediumDecV2Base MediumDecV2Base::ENum = MediumDecV2Base::ENumValue();
MediumDecV2Base MediumDecV2Base::NegativePointFive = MediumDecV2Base::NegativePointFiveValue();
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

/*
inline void BlazesRusCode::MediumDecV2Base::DivideByTwo()
{
	if (DecimalHalf == 0 && (IntHalf.Value & 1) == 1)//Check if number is odd
		UnsignedIntDivOp(2);
	else
		IntHalf /= 2;
}

inline void BlazesRusCode::MediumDecV2Base::DivideByFour()
{
	//Checking if divisible by 4 based on
	//https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
	//checking if divible by 8 equals (((n >> 3) << 3) == n)
	if (DecimalHalf == 0 && (((IntHalf.Value >> 2) << 2) == IntHalf.Value))//Check if number can be perfectly divided by 4
		IntHalf /= 4;
	else
		UnsignedIntDivOp(4);
}
*/

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::DividedByTwo() const
{
	return DividedByTwoV1();
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::DividedByFour() const
{
	return DividedByFourV1();
}

#pragma endregion Other Division Operations

#pragma region Other multiplication operations

/*
inline void BlazesRusCode::MediumDecV2Base::MultiplyByTwo()
{
	UnsignedIntMultOp(2);
}

inline void BlazesRusCode::MediumDecV2Base::MultiplyByFour()
{
	UnsignedIntMultOp(4);
}
*/


inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::MultipliedByTwo() const
{
	return MultipliedByTwoV1();
}

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::MultipliedByFour() const
{
	return MultipliedByFourV1();
}


#pragma endregion Other multiplication operations

/*
#pragma region NormalRep Integer Addition Operations

inline void MediumDecV2Base::UnsignedIntegerAddition(const MirroredInt& rValue)
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

inline void MediumDecV2Base::IntegerAddition(const MirroredInt& rValue)
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

inline void MediumDecV2Base::UIntAddOp(const unsigned int& rValue)
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

inline MediumDecV2Base& BlazesRusCode::MediumDecV2Base::UIntAddOperation(const unsigned int& rValue)
{
	UIntAddOp(rValue); return *this;
}

#pragma endregion NormalRep Integer Addition Operations

#pragma region NormalRep Integer Subtraction Operations

inline void BlazesRusCode::MediumDecV2Base::UnsignedIntegerSubtraction(const MirroredInt& rValue)
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

inline void BlazesRusCode::MediumDecV2Base::IntegerSubtraction(const MirroredInt& rValue)
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

inline void BlazesRusCode::MediumDecV2Base::UIntSubOp(const unsigned int& rValue)
{
	if (DecimalHalf.Value == 0)
		IntHalf.NRepSkippingUnsignedSubOp(rValue);
	else {
		unsigned int signBeforeOp = IntHalf.Sign;
		IntHalf.UIntSubOp(rValue);
		if (signBeforeOp != IntHalf.Sign)//Invert the decimal section
			DecimalHalf = DecimalOverflow - DecimalHalf;
	}
}

inline MediumDecV2Base& BlazesRusCode::MediumDecV2Base::UIntSubOperation(const unsigned int& rValue)
{
	UIntSubOp(rValue); return *this;
}

#pragma endregion NormalRep Integer Subtraction Operations

#pragma region Truncation Functions

inline MediumDecV2Base BlazesRusCode::MediumDecV2Base::Abs(const MediumDecV2Base& tValue) {
	MediumDecV2Base result = tValue; result.ApplyAbs();
	return result;
}

#pragma endregion Truncation Functions

#pragma region String Commands

inline void BlazesRusCode::MediumDecV2Base::ReadString(const std::string& Value)
{
	IntHalf = 0; DecimalHalf = 0;
	int PlaceNumber;
	std::string WholeNumberBuffer = "";
	std::string DecimalBuffer = "";

	bool ReadingDecimal = false;
	int TempInt;
	int TempInt02;
	for (char const& StringChar : Value)
	{
		if (VariableConversionFunctions::IsDigit(StringChar))
		{
			if (ReadingDecimal) { DecimalBuffer += StringChar; }
			else { WholeNumberBuffer += StringChar; }
		}
		else if (StringChar == '-')
			IntHalf.Sign = 0;
		else if (StringChar == '.')
			ReadingDecimal = true;
		else if (StringChar != ' ')
			break;//Stop Extracting after encounter non-number character such as i
	}
	PlaceNumber = WholeNumberBuffer.length() - 1;
	for (char const& StringChar : WholeNumberBuffer)
	{
		TempInt = VariableConversionFunctions::CharAsInt(StringChar);
		TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
		if (StringChar != '0')
		{
			IntHalf.Value += TempInt02;
		}
		PlaceNumber--;
	}
	PlaceNumber = 8;
	for (char const& StringChar : DecimalBuffer)
	{
		//Limit stored decimal numbers to the amount it can store
		if (PlaceNumber > -1)
		{
			TempInt = VariableConversionFunctions::CharAsInt(StringChar);
			TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
			if (StringChar != '0')
			{
				DecimalHalf += TempInt02;
			}
			PlaceNumber--;
		}
	}
}

inline std::string BlazesRusCode::MediumDecV2Base::ToString()
{
	std::string Value = std::string(IntHalf);
	if (DecimalHalf != 0)
	{
		Value += ".";
		Value += std::string(DecimalHalf);
	}
	return Value;
}

inline std::string BlazesRusCode::MediumDecV2Base::ToFullString()
{
	std::string Value = std::string(IntHalf);
	if (DecimalHalf != 0)
	{
		unsigned __int8 CurrentDigit;
		Value += ".";
		bool HasDigitsUsed = false;
		unsigned int CurrentSection = DecimalHalf.Value;
		for (__int8 Index = 8; Index >= 0; --Index)
		{
			if (CurrentSection > 0)
			{
				CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
				CurrentSection -= (CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
				Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
			}
			else
				Value += "0";
		}
	}
	else
	{
		Value += ".000000000";
	}
	return Value;
}

#pragma endregion String Commands

#pragma region ConvertFromOtherTypes

#pragma endregion ConvertFromOtherTypes
*/