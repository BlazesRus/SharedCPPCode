#include "MediumDecBase.hpp"
using MediumDecBase = BlazesRusCode::MediumDecBase;

#pragma region ValueSetters

inline void BlazesRusCode::MediumDecBase::SetValueToPiNum()
{
	IntHalf = 3; DecimalHalf = 141592654;
}

inline void BlazesRusCode::MediumDecBase::SetValueToHundredMilPiNum()
{
	IntHalf = 314159265; DecimalHalf = 358979324;
}

inline void BlazesRusCode::MediumDecBase::SetValueToTenMilPiNum()
{
	IntHalf = 31415926; DecimalHalf = 535897932;
}

inline void BlazesRusCode::MediumDecBase::SetValueToOneMilPiNum()
{
	IntHalf = 3141592; DecimalHalf = 653589793;
}

inline void BlazesRusCode::MediumDecBase::SetValueToTenPiNum()
{
	IntHalf = 31; DecimalHalf = 415926536;
}

inline void BlazesRusCode::MediumDecBase::SetValueToENum()
{
	IntHalf = 2; DecimalHalf = 718281828;
}

inline void BlazesRusCode::MediumDecBase::SetValueToPoint5()
{
	IntHalf = 0; DecimalHalf = 500000000;
}

inline void BlazesRusCode::MediumDecBase::SetValueToJustAboveZero()
{
	IntHalf = 0; DecimalHalf = 1;
}

inline void BlazesRusCode::MediumDecBase::SetValueToOneMillionth()
{
	IntHalf = 0; DecimalHalf = 1000;
}

inline void BlazesRusCode::MediumDecBase::SetValueToFiveThousandth()
{
	IntHalf = 0; DecimalHalf = 5000000;
}

inline void BlazesRusCode::MediumDecBase::SetValueToFiveMillionth()
{
	IntHalf = 0; DecimalHalf = 5000;
}

inline void BlazesRusCode::MediumDecBase::SetValueToTenMillionth()
{
	IntHalf = 0; DecimalHalf = 100;
}

inline void BlazesRusCode::MediumDecBase::SetValueToOneHundredMillionth()
{
	IntHalf = 0; DecimalHalf = 10;
}

inline void BlazesRusCode::MediumDecBase::SetValueToLN10()
{
	IntHalf = 2; DecimalHalf = 302585093;
}

inline void BlazesRusCode::MediumDecBase::SetValueToLN10Div()
{
	IntHalf = 0; DecimalHalf = 434294482;
}

inline void BlazesRusCode::MediumDecBase::SetValueToTwiceLN10Div()
{
	IntHalf = 0; DecimalHalf = 868588964;
}

#pragma endregion ValueSetters

#pragma region ValueDefines

inline MediumDecBase BlazesRusCode::MediumDecBase::AlmostOneValue()
{
	return MediumDecBase(0, 999999999);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::PiNumValue()
{
	return MediumDecBase(3, 141592654);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::HundredMilPiNumValue()
{
	return MediumDecBase(314159265, 358979324);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::TenMilPiNumValue()
{
	return MediumDecBase(31415926, 535897932);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::OneMilPiNumValue()
{
	return MediumDecBase(3141592, 653589793);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::TenPiNumValue()
{
	return MediumDecBase(31, 415926536);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::ENumValue()
{
	return MediumDecBase(2, 718281828);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::ZeroValue()
{
	return MediumDecBase();
}

inline MediumDecBase BlazesRusCode::MediumDecBase::OneValue()
{
	return MediumDecBase(1);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::TwoValue()
{
	return MediumDecBase(MirroredInt::Two);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::NegativeOneValue()
{
	return MediumDecBase(MirroredInt::NegativeOne);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::Point5Value()
{
	return MediumDecBase(0, 500000000);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::JustAboveZeroValue()
{
	return MediumDecBase(0, 1);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::OneMillionthValue()
{
	return MediumDecBase(0, 1000);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::FiveThousandthValue()
{
	return MediumDecBase(0, 5000000);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::FiveMillionthValue()
{
	return MediumDecBase(0, 5000);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::TenMillionthValue()
{
	return MediumDecBase(0, 100);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::OneHundredMillionthValue()
{
	return MediumDecBase(0, 10);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::FiveBillionthValue()
{
	return MediumDecBase(0, 5);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::LN10Value()
{
	return MediumDecBase(2, 302585093);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::LN10DivValue()
{
	return MediumDecBase(0, 434294482);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::TwiceLN10DivValue()
{
	return MediumDecBase(0, 868588964);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::MinimumValue()
{
	return MediumDecBase(MirroredInt::Maximum, 999999999);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::MaximumValue()
{
	return MediumDecBase(MirroredInt::Minimum, 999999999);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::NegativePointFiveValue()
{
	return MediumDecBase(MirroredInt::NegativeZero, 500000000);
}



#pragma endregion ValueDefines

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

#pragma region Check_if_value

inline void BlazesRusCode::MediumDecBase::SetAsZero()
{
	IntHalf = 0; DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDecBase::SetAsOne()
{
	IntHalf = 1; DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDecBase::SetAsOneVal()
{
	IntHalf.Value = 1; DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDecBase::SetAsValues(const MirroredInt& intVal, const PartialInt& decVal)
{
	IntHalf = 0; DecimalHalf = 0;
}

inline bool BlazesRusCode::MediumDecBase::IsAtZeroInt() const
{
	return IntHalf.Value == 0;
}

inline bool BlazesRusCode::MediumDecBase::IsNotAtZeroInt() const
{
	return IntHalf.Value != 0;
}

inline bool BlazesRusCode::MediumDecBase::IsAtOneInt() const
{
	return IntHalf.Value == 1;
}

inline bool BlazesRusCode::MediumDecBase::IsNotAtOneInt() const
{
	return IntHalf.Value != 1;
}

inline bool BlazesRusCode::MediumDecBase::IsZero() const
{
	return DecimalHalf == 0 && IntHalf.Value == 0;
}

inline bool BlazesRusCode::MediumDecBase::IsOne() const
{
	return DecimalHalf == 0 && IntHalf == MirroredInt::One;
}

inline bool BlazesRusCode::MediumDecBase::IsNegOne() const
{
	return DecimalHalf == 0 && IntHalf == MirroredInt::NegativeOne;
}

inline bool BlazesRusCode::MediumDecBase::IsOneVal() const
{
	return DecimalHalf == 0 && IntHalf.Value == 1;
}

inline bool BlazesRusCode::MediumDecBase::IsOneVariantVal() const
{
	return DecimalHalf.Value == 0 && IntHalf.Value == 1;
}

#pragma endregion Check_if_value

#pragma region RangeLimits

inline void BlazesRusCode::MediumDecBase::SetAsMaximum()
{
	IntHalf = MirroredInt::Maximum; DecimalHalf = 999999999;
}

inline void BlazesRusCode::MediumDecBase::SetAsMinimum()
{
	IntHalf = MirroredInt::Minimum; DecimalHalf = 999999999;
}

#pragma endregion RangeLimits

#pragma region ConvertToOtherTypes

inline float BlazesRusCode::MediumDecBase::toFloat() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	float Value;
	if (IntHalf.IsNegative())
	{
		Value = (float)-IntHalf.Value;
		if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
	}
	else
	{
		Value = (float)IntHalf.Value;
		if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
	}
	return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
	if (IntHalf.Value == 0)//Exponent is negative
	{
		//To-Do:Add code here
	}
	else
	{
		//To-Do:Add code here
	}
	return 0.0f;//Placeholder
	#endif
}

inline double BlazesRusCode::MediumDecBase::toDouble() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	double Value;
	if (IntHalf < 0)
	{
		Value = (double)-IntHalf.Value;
		if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
	}
	else
	{
		Value = (double)IntHalf.Value;
		if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
	}
	return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
	if (IntHalf.Value == 0)//Exponent is negative
	{
		//To-Do:Add code here
	}
	else
	{
		//To-Do:Add code here
	}
	return 0.0;//Placeholder
	#endif
}

inline long double BlazesRusCode::MediumDecBase::toDecimal() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	long double Value;
	if (IntHalf < 0)
	{
		Value = (long double)-IntHalf.Value;
		if (DecimalHalf != 0) { Value -= ((long double)DecimalHalf * 0.000000001L); }
	}
	else
	{
		Value = (long double)IntHalf.Value;
		if (DecimalHalf != 0) { Value += ((long double)DecimalHalf * 0.000000001L); }
	}
	return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
	if (IntHalf.Value == 0)//Exponent is negative
	{
		//To-Do:Add code here
	}
	else
	{
		//To-Do:Add code here
	}
	return 0.0L;//Placeholder
	#endif
}

#pragma endregion ConvertToOtherTypes

#pragma region Comparison Operators

inline std::strong_ordering BlazesRusCode::MediumDecBase::BasicIntComparison(const int& that) const
{
	if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
		return IntHalfCmp;
	//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
	unsigned int lVal = DecimalHalf.Value > 0 ? 1 : 0;
	if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
		return DecimalHalfCmp;
}

#pragma endregion Comparison Operators

#pragma region Other Division Operations

inline void BlazesRusCode::MediumDecBase::DivideByTwo()
{
	if (DecimalHalf == 0 && (IntHalf.Value & 1) == 1)//Check if number is odd
		UnsignedIntDivOp(2);
	else
		IntHalf /= 2;
}

inline void BlazesRusCode::MediumDecBase::DivideByFour()
{
	//Checking if divisible by 4 based on
	//https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
	//checking if divible by 8 equals (((n >> 3) << 3) == n)
	if (DecimalHalf == 0 && (((IntHalf.Value >> 2) << 2) == IntHalf.Value))//Check if number can be perfectly divided by 4
		IntHalf /= 4;
	else
		UnsignedIntDivOp(4);
}

inline MediumDecBase BlazesRusCode::MediumDecBase::DividedByTwo() const
{
	return DividedByTwoV1();
}

inline MediumDecBase BlazesRusCode::MediumDecBase::DividedByFour() const
{
	return DividedByFourV1();
}

#pragma endregion Other Division Operations

#pragma region Other multiplication operations

inline void BlazesRusCode::MediumDecBase::MultiplyByTwo()
{
	UnsignedIntMultOp(2);
}

inline void BlazesRusCode::MediumDecBase::MultiplyByFour()
{
	UnsignedIntMultOp(4);
}


inline MediumDecBase BlazesRusCode::MediumDecBase::MultipliedByTwo() const
{
	return MultipliedByTwoV1();
}

inline MediumDecBase BlazesRusCode::MediumDecBase::MultipliedByFour() const
{
	return MultipliedByFourV1();
}


#pragma endregion Other multiplication operations

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

#pragma region NormalRep Integer Subtraction Operations

inline void BlazesRusCode::MediumDecBase::UnsignedIntegerSubtraction(const MirroredInt& rValue)
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

/// <summary>
/// Basic Subtraction operation between MediumDec Variant and MirroredInt
/// that ignores special representation status
/// (Modifies owner object)
/// </summary>
/// <param name="rValue">The right side value</param>

inline void BlazesRusCode::MediumDecBase::IntegerSubtraction(const MirroredInt& rValue)
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

inline void BlazesRusCode::MediumDecBase::UIntSubOp(const unsigned int& rValue)
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

inline MediumDecBase& BlazesRusCode::MediumDecBase::UIntSubOperation(const unsigned int& rValue)
{
	UIntSubOp(rValue); return *this;
}

#pragma endregion NormalRep Integer Subtraction Operations

#pragma region Truncation Functions

inline MediumDecBase BlazesRusCode::MediumDecBase::Abs(const MediumDecBase& tValue) {
	MediumDecBase result = tValue; result.ApplyAbs();
	return result;
}

inline void BlazesRusCode::MediumDecBase::FloorOf(const int& precision)
{
	switch (precision)
	{
	case 8: DecimalHalf.Value /= 10; DecimalHalf.Value *= 10; break;
	case 7: DecimalHalf.Value /= 100; DecimalHalf.Value *= 100; break;
	case 6: DecimalHalf.Value /= 1000; DecimalHalf.Value *= 1000; break;
	case 5: DecimalHalf.Value /= 10000; DecimalHalf.Value *= 10000; break;
	case 4: DecimalHalf.Value /= 100000; DecimalHalf.Value *= 100000; break;
	case 3: DecimalHalf.Value /= 1000000; DecimalHalf.Value *= 1000000; break;
	case 2: DecimalHalf.Value /= 10000000; DecimalHalf.Value *= 10000000; break;
	case 1: DecimalHalf.Value /= 100000000; DecimalHalf.Value *= 100000000; break;
	case 0:
		DecimalHalf = 0;
		break;
	default:
		break;
	}
	if (IntHalf == MirroredInt::NegativeZero && DecimalHalf == 0)
		IntHalf = 0;
}

inline signed int BlazesRusCode::MediumDecBase::FloorIntOf() const
{
	if (DecimalHalf == 0)
		return GetIntHalf();
	else if (IntHalf == MirroredInt::NegativeZero)
		return -1;
	else
		return GetIntHalf() - 1;
}

inline int BlazesRusCode::MediumDecBase::CeilIntOf() const
{
	if (DecimalHalf == 0)
		return GetIntHalf();
	else if (IntHalf == MirroredInt::NegativeZero)
		return 0;
	else
		return GetIntHalf() + 1;
}

inline MediumDecBase BlazesRusCode::MediumDecBase::TruncOf()
{
	DecimalHalf = 0;
	if (IntHalf == NegativeRep)
		IntHalf = 0;
	return *this;
}

#pragma endregion Truncation Functions

#pragma region String Commands

inline void BlazesRusCode::MediumDecBase::ReadString(const std::string& Value)
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

inline std::string BlazesRusCode::MediumDecBase::ToString()
{
	std::string Value = std::string(IntHalf);
	if (DecimalHalf != 0)
	{
		Value += ".";
		Value += std::string(DecimalHalf);
	}
	return Value;
}

inline std::string BlazesRusCode::MediumDecBase::ToFullString()
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


inline void BlazesRusCode::MediumDecBase::SetFloatVal(const float& Value)
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	float lValue = Value;
	bool IsNegative = Value < 0.0f;
	if (IsNegative) { lValue *= -1.0f; }
	//Cap value if too big on initialize (preventing overflow on conversion)
	if (Value >= 2147483648.0f)
	{
		if (IsNegative)
			IntHalf = MirroredInt(2147483647, 0);
		else
			IntHalf = 2147483647;
		DecimalHalf = 999999999;
	}
	else
	{
		signed __int64 WholeValue = (signed __int64)std::floor(Value);
		lValue -= (float)WholeValue;
		DecimalHalf = (signed int)Value * 10000000000;
		IntHalf = MirroredInt((unsigned int)WholeValue, IsNegative ? 0 : 1);
	}
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
	//To-Do:Add code here
	#endif
}

inline void BlazesRusCode::MediumDecBase::SetDoubleVal(const double& Value)
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	double lValue = Value;
	bool IsNegative = Value < 0.0;
	if (IsNegative) { lValue *= -1.0; }
	//Cap value if too big on initialize (preventing overflow on conversion)
	if (Value >= 2147483648.0)
	{
		if (IsNegative)
			IntHalf = MirroredInt(2147483647, 0);
		else
			IntHalf = 2147483647;
		DecimalHalf = 999999999;
	}
	else
	{
		signed __int64 WholeValue = (signed __int64)std::floor(Value);
		lValue -= (double)WholeValue;
		DecimalHalf = (signed int)Value * 10000000000;
		IntHalf = MirroredInt((unsigned int)WholeValue, IsNegative ? 0 : 1);
	}
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
	//To-Do:Add code here
	#endif
}

inline void BlazesRusCode::MediumDecBase::SetDecimalVal(const long double& Value)
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	long double lValue = Value;
	bool IsNegative = Value < 0.0L;
	if (IsNegative) { lValue *= -1.0L; }
	//Cap value if too big on initialize (preventing overflow on conversion)
	if (lValue >= 2147483648.0L)
	{
		if (IsNegative)
			IntHalf = MirroredInt(2147483647, 0);
		else
			IntHalf = 2147483647;
		DecimalHalf = 999999999;
	}
	else
	{
		signed __int64 WholeValue = (signed __int64)std::floor(lValue);
		lValue -= (long double)WholeValue;
		DecimalHalf = (signed int)lValue * 10000000000;
		IntHalf = MirroredInt((unsigned int)WholeValue, IsNegative ? 0 : 1);
	}
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
	//To-Do:Add code here
	#endif
}

inline void BlazesRusCode::MediumDecBase::SetBoolVal(const bool& Value)
{
	IntHalf = Value == false ? 0 : 1;
	DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDecBase::SetIntVal(const int& Value)
{
	if (Value<0)
	{
		IntHalf.Sign = MirroredInt::NegativeSign;
		IntHalf.Value = -Value;
	}
	else
		IntHalf = Value;
	DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDecBase::SetUIntVal(const unsigned int& Value)
{
	IntHalf = Value;
	DecimalHalf = 0;
}

#pragma endregion ConvertFromOtherTypes