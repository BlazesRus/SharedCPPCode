#include "MediumDec.hpp"
using MediumDec = BlazesRusCode::MediumDec;

#pragma region ValueSetters

inline void BlazesRusCode::MediumDec::SetValueToPiNum()
{
	IntHalf = 3; DecimalHalf = 141592654;
}

inline void BlazesRusCode::MediumDec::SetValueToHundredMilPiNum()
{
	IntHalf = 314159265; DecimalHalf = 358979324;
}

inline void BlazesRusCode::MediumDec::SetValueToTenMilPiNum()
{
	IntHalf = 31415926; DecimalHalf = 535897932;
}

inline void BlazesRusCode::MediumDec::SetValueToOneMilPiNum()
{
	IntHalf = 3141592; DecimalHalf = 653589793;
}

inline void BlazesRusCode::MediumDec::SetValueToTenPiNum()
{
	IntHalf = 31; DecimalHalf = 415926536;
}

inline void BlazesRusCode::MediumDec::SetValueToENum()
{
	IntHalf = 2; DecimalHalf = 718281828;
}

inline void BlazesRusCode::MediumDec::SetValueToPoint5()
{
	IntHalf = 0; DecimalHalf = 500000000;
}

inline void BlazesRusCode::MediumDec::SetValueToJustAboveZero()
{
	IntHalf = 0; DecimalHalf = 1;
}

inline void BlazesRusCode::MediumDec::SetValueToOneMillionth()
{
	IntHalf = 0; DecimalHalf = 1000;
}

inline void BlazesRusCode::MediumDec::SetValueToFiveThousandth()
{
	IntHalf = 0; DecimalHalf = 5000000;
}

inline void BlazesRusCode::MediumDec::SetValueToFiveMillionth()
{
	IntHalf = 0; DecimalHalf = 5000;
}

inline void BlazesRusCode::MediumDec::SetValueToTenMillionth()
{
	IntHalf = 0; DecimalHalf = 100;
}

inline void BlazesRusCode::MediumDec::SetValueToOneHundredMillionth()
{
	IntHalf = 0; DecimalHalf = 10;
}

inline void BlazesRusCode::MediumDec::SetValueToLN10()
{
	IntHalf = 2; DecimalHalf = 302585093;
}

inline void BlazesRusCode::MediumDec::SetValueToLN10Div()
{
	IntHalf = 0; DecimalHalf = 434294482;
}

inline void BlazesRusCode::MediumDec::SetValueToTwiceLN10Div()
{
	IntHalf = 0; DecimalHalf = 868588964;
}

inline void BlazesRusCode::MediumDec::SetValueToPointOne()
{
	IntHalf = 0; DecimalHalf = 100000000;
}

#pragma endregion ValueSetters

#pragma region ValueDefines

inline MediumDec BlazesRusCode::MediumDec::AlmostOneValue()
{
	return MediumDec(0, 999999999);
}

inline MediumDec BlazesRusCode::MediumDec::PiNumValue()
{
	return MediumDec(3, 141592654);
}

inline MediumDec BlazesRusCode::MediumDec::HundredMilPiNumValue()
{
	return MediumDec(314159265, 358979324);
}

inline MediumDec BlazesRusCode::MediumDec::TenMilPiNumValue()
{
	return MediumDec(31415926, 535897932);
}

inline MediumDec BlazesRusCode::MediumDec::OneMilPiNumValue()
{
	return MediumDec(3141592, 653589793);
}

inline MediumDec BlazesRusCode::MediumDec::TenPiNumValue()
{
	return MediumDec(31, 415926536);
}

inline MediumDec BlazesRusCode::MediumDec::ENumValue()
{
	return MediumDec(2, 718281828);
}

inline MediumDec BlazesRusCode::MediumDec::ZeroValue()
{
	return MediumDec();
}

inline MediumDec BlazesRusCode::MediumDec::OneValue()
{
	return MediumDec(1);
}

inline MediumDec BlazesRusCode::MediumDec::TwoValue()
{
	return MediumDec(MirroredInt::Two);
}

inline MediumDec BlazesRusCode::MediumDec::NegativeOneValue()
{
	return MediumDec(MirroredInt::NegativeOne);
}

inline MediumDec BlazesRusCode::MediumDec::Point5Value()
{
	return MediumDec(0, 500000000);
}

inline MediumDec BlazesRusCode::MediumDec::JustAboveZeroValue()
{
	return MediumDec(0, 1);
}

inline MediumDec BlazesRusCode::MediumDec::OneMillionthValue()
{
	return MediumDec(0, 1000);
}

inline MediumDec BlazesRusCode::MediumDec::FiveThousandthValue()
{
	return MediumDec(0, 5000000);
}

inline MediumDec BlazesRusCode::MediumDec::FiveMillionthValue()
{
	return MediumDec(0, 5000);
}

inline MediumDec BlazesRusCode::MediumDec::TenMillionthValue()
{
	return MediumDec(0, 100);
}

inline MediumDec BlazesRusCode::MediumDec::OneHundredMillionthValue()
{
	return MediumDec(0, 10);
}

inline MediumDec BlazesRusCode::MediumDec::FiveBillionthValue()
{
	return MediumDec(0, 5);
}

inline MediumDec BlazesRusCode::MediumDec::LN10Value()
{
	return MediumDec(2, 302585093);
}

inline MediumDec BlazesRusCode::MediumDec::LN10DivValue()
{
	return MediumDec(0, 434294482);
}

inline MediumDec BlazesRusCode::MediumDec::TwiceLN10DivValue()
{
	return MediumDec(0, 868588964);
}

inline MediumDec BlazesRusCode::MediumDec::MinimumValue()
{
	return MediumDec(MirroredInt::Maximum, 999999999);
}

inline MediumDec BlazesRusCode::MediumDec::MaximumValue()
{
	return MediumDec(MirroredInt::Minimum, 999999999);
}

inline MediumDec BlazesRusCode::MediumDec::NegativePointFiveValue()
{
	return MediumDec(MirroredInt::NegativeZero, 500000000);
}

inline MediumDec BlazesRusCode::MediumDec::PointOneValue()
{
	return MediumDec(0, 100000000);
}

#pragma endregion ValueDefines

#pragma region ValueDefine Source
MediumDec MediumDec::AlmostOne = MediumDec::AlmostOneValue();
MediumDec MediumDec::Pi = MediumDec::PiNumValue();
MediumDec MediumDec::One = MediumDec::OneValue();
MediumDec MediumDec::Two = MediumDec::TwoValue();
MediumDec MediumDec::NegativeOne = MediumDec::NegativeOneValue();
MediumDec MediumDec::Zero = MediumDec::ZeroValue();
MediumDec MediumDec::PointFive = MediumDec::Point5Value();
MediumDec MediumDec::JustAboveZero = MediumDec::JustAboveZeroValue();
MediumDec MediumDec::OneMillionth = MediumDec::OneMillionthValue();
MediumDec MediumDec::FiveThousandth = MediumDec::FiveThousandthValue();
MediumDec MediumDec::Minimum = MediumDec::MinimumValue();
MediumDec MediumDec::Maximum = MediumDec::MaximumValue();
MediumDec MediumDec::E = MediumDec::ENumValue();
MediumDec MediumDec::LN10 = MediumDec::LN10Value();
MediumDec MediumDec::LN10Div = MediumDec::LN10DivValue();
MediumDec MediumDec::TwiceLN10Div = MediumDec::TwiceLN10DivValue();
MediumDec MediumDec::TenMillionth = MediumDec::TenMillionthValue();
MediumDec MediumDec::FiveMillionth = MediumDec::FiveMillionthValue();
MediumDec MediumDec::FiveBillionth = MediumDec::FiveBillionthValue();
MediumDec MediumDec::OneGMillionth = MediumDec::OneHundredMillionthValue();

MediumDec MediumDec::PiNum = MediumDec::PiNumValue();
MediumDec MediumDec::ENum = MediumDec::ENumValue();
MediumDec MediumDec::NegativePointFive = MediumDec::NegativePointFiveValue();
MediumDec MediumDec::PointOne = MediumDec::PointOneValue();
#pragma endregion ValueDefine Source

#pragma region Negative_Status

inline bool BlazesRusCode::MediumDec::IsPositive() const
{
	return IntHalf.IsPositive();
}

inline bool BlazesRusCode::MediumDec::IsNegative() const
{
	return IntHalf.IsNegative();
}

inline void BlazesRusCode::MediumDec::SwapNegativeStatus()
{
	IntHalf.Sign ^= 1;
}

#pragma endregion Negative_Status

#pragma region Check_if_value

inline void BlazesRusCode::MediumDec::SetAsZero()
{
	IntHalf = 0; DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDec::SetAsOne()
{
	IntHalf = 1; DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDec::SetAsOneVal()
{
	IntHalf.Value = 1; DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDec::SetAsValues(const MirroredInt& intVal, const PartialInt& decVal)
{
	IntHalf = 0; DecimalHalf = 0;
}

inline bool BlazesRusCode::MediumDec::IsAtZeroInt() const
{
	return IntHalf.Value == 0;
}

inline bool BlazesRusCode::MediumDec::IsNotAtZeroInt() const
{
	return IntHalf.Value != 0;
}

inline bool BlazesRusCode::MediumDec::IsAtOneInt() const
{
	return IntHalf.Value == 1;
}

inline bool BlazesRusCode::MediumDec::IsNotAtOneInt() const
{
	return IntHalf.Value != 1;
}

inline bool BlazesRusCode::MediumDec::IsZero() const
{
	return DecimalHalf == 0 && IntHalf.Value == 0;
}

inline bool BlazesRusCode::MediumDec::IsOne() const
{
	return DecimalHalf == 0 && IntHalf == MirroredInt::One;
}

inline bool BlazesRusCode::MediumDec::IsNegOne() const
{
	return DecimalHalf == 0 && IntHalf == MirroredInt::NegativeOne;
}

inline bool BlazesRusCode::MediumDec::IsOneVal() const
{
	return DecimalHalf == 0 && IntHalf.Value == 1;
}

inline bool BlazesRusCode::MediumDec::IsOneVariantVal() const
{
	return DecimalHalf.Value == 0 && IntHalf.Value == 1;
}

#pragma endregion Check_if_value

#pragma region RangeLimits

inline void BlazesRusCode::MediumDec::SetAsMaximum()
{
	IntHalf = MirroredInt::Maximum; DecimalHalf = 999999999;
}

inline void BlazesRusCode::MediumDec::SetAsMinimum()
{
	IntHalf = MirroredInt::Minimum; DecimalHalf = 999999999;
}

#pragma endregion RangeLimits

#pragma region ConvertToOtherTypes

    #if defined(AltNum_EnableFloatingConversion)
inline float BlazesRusCode::MediumDec::toFloat() const
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

inline double BlazesRusCode::MediumDec::toDouble() const
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

inline long double BlazesRusCode::MediumDec::toDecimal() const
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
	#endif

#pragma endregion ConvertToOtherTypes

#pragma region Comparison Operators


#pragma endregion Comparison Operators

#pragma region Other Division Operations

inline void BlazesRusCode::MediumDec::DivideByTwo()
{
	if (DecimalHalf == 0 && (IntHalf.Value & 1) == 1)//Check if number is odd
		UIntDivOp(2);
	else
		IntHalf.Value /= 2;
}

inline void BlazesRusCode::MediumDec::DivideByFour()
{
	//Checking if divisible by 4 based on
	//https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
	//checking if divible by 8 equals (((n >> 3) << 3) == n)
	if (DecimalHalf == 0 && (((IntHalf.Value >> 2) << 2) == IntHalf.Value))//Check if number can be perfectly divided by 4
		IntHalf.Value /= 4;
	else
		UIntDivOp(4);
}

inline MediumDec BlazesRusCode::MediumDec::DividedByTwo() const
{
	MediumDec result = *this; result.DivideByTwo();
	return result;
}

inline MediumDec BlazesRusCode::MediumDec::DividedByFour() const
{
	MediumDec result = *this; result.DivideByFour();
	return result;
}

#pragma endregion Other Division Operations

#pragma region Other multiplication operations

inline void BlazesRusCode::MediumDec::MultiplyByTwo()
{
	UIntMultOp(2);
}

inline void BlazesRusCode::MediumDec::MultiplyByFour()
{
	UIntMultOp(4);
}


inline MediumDec BlazesRusCode::MediumDec::MultipliedByTwo() const
{
	MediumDec result = *this; result.UIntMultOp(2);
	return result;
}

inline MediumDec BlazesRusCode::MediumDec::MultipliedByFour() const
{
	MediumDec result = *this; result.UIntMultOp(4);
	return result;
}


#pragma endregion Other multiplication operations

#pragma region NormalRep Integer Addition Operations

inline void MediumDec::UnsignedMirroredAddOp(const MirroredInt& rValue)
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

inline void MediumDec::MirroredAddOp(const MirroredInt& rValue)
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

inline void MediumDec::UIntAddOp(const unsigned int& rValue)
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

inline MediumDec& BlazesRusCode::MediumDec::UIntAddOperation(const unsigned int& rValue)
{
	UIntAddOp(rValue); return *this;
}

#pragma endregion NormalRep Integer Addition Operations

#pragma region NormalRep Integer Subtraction Operations

inline void BlazesRusCode::MediumDec::UnsignedMirroredSubOp(const MirroredInt& rValue)
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

inline void BlazesRusCode::MediumDec::MirroredSubOp(const MirroredInt& rValue)
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

inline void BlazesRusCode::MediumDec::UIntSubOp(const unsigned int& rValue)
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

inline MediumDec& BlazesRusCode::MediumDec::UIntSubOperation(const unsigned int& rValue)
{
	UIntSubOp(rValue); return *this;
}

#pragma endregion NormalRep Integer Subtraction Operations

#pragma region Truncation Functions

inline MediumDec BlazesRusCode::MediumDec::Abs(const MediumDec& tValue) {
	return AbsV1<MediumDec>(tValue);
}

inline void BlazesRusCode::MediumDec::ApplyFloorOf(const int& precision)
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

inline signed int BlazesRusCode::MediumDec::FloorIntOf() const
{
	if (DecimalHalf == 0)
		return GetIntHalf();
	else if (IntHalf == MirroredInt::NegativeZero)
		return -1;
	else
		return GetIntHalf() - 1;
}

inline int BlazesRusCode::MediumDec::CeilIntOf() const
{
	if (DecimalHalf == 0)
		return GetIntHalf();
	else if (IntHalf == MirroredInt::NegativeZero)
		return 0;
	else
		return GetIntHalf() + 1;
}

inline MediumDec BlazesRusCode::MediumDec::Trunc(const MediumDec& tValue) { return tValue.TruncOfV1<MediumDec>(); }

#pragma endregion Truncation Functions

#pragma region String Commands

inline void BlazesRusCode::MediumDec::ReadString(const std::string& Value)
{
	IntHalf = 0; DecimalHalf = 0;

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
	unsigned int PlaceNumber = WholeNumberBuffer.length() - 1;
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
		TempInt = VariableConversionFunctions::CharAsInt(StringChar);
		TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
		if (StringChar != '0')
		{
			DecimalHalf += TempInt02;
		}
		if(Placenumber==0)//Limit stored decimal numbers to the amount it can store
			break;
	}
}

inline BlazesRusCode::MediumDec::MediumDec(const char* strVal)
{
	std::string Value = strVal;
	this->ReadString(Value);
}

inline BlazesRusCode::MediumDec::MediumDec(const std::string& Value)
{
	this->ReadString(Value);
}

inline std::string BlazesRusCode::MediumDec::ToString()
{
	std::string Value = std::string(IntHalf);
	if (DecimalHalf != 0)
	{
		Value += ".";
		Value += std::string(DecimalHalf);
	}
	return Value;
}

inline std::string BlazesRusCode::MediumDec::ToFullString()
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

    #if defined(AltNum_EnableFloatingConversion)
inline void BlazesRusCode::MediumDec::SetFloatVal(const float& Value)
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

inline void BlazesRusCode::MediumDec::SetDoubleVal(const double& Value)
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

inline void BlazesRusCode::MediumDec::SetDecimalVal(const long double& Value)
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
	#endif

inline void BlazesRusCode::MediumDec::SetBoolVal(const bool& Value)
{
	IntHalf = Value == false ? 0 : 1;
	DecimalHalf = 0;
}

inline void BlazesRusCode::MediumDec::SetIntVal(const int& Value)
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

inline void BlazesRusCode::MediumDec::SetUIntVal(const unsigned int& Value)
{
	IntHalf = Value;
	DecimalHalf = 0;
}

#pragma endregion ConvertFromOtherTypes

#pragma region Pow and Sqrt Functions

inline MediumDec BlazesRusCode::MediumDec::Sqrt(const auto& value, const int& precision)
{
	return SqrtV1<MediumDec>(value, precision);
}

inline MediumDec BlazesRusCode::MediumDec::SqrtOf(const int& precision) const {
	return Sqrt(*this, precision);
}

inline MediumDec BlazesRusCode::MediumDec::UnsignedNthRoot(const MediumDec& tValue, const unsigned int& n, const MediumDec& precision)
{
	return UnsignedNthRootV1<MediumDec>(tValue, n, precision);
}

inline MediumDec BlazesRusCode::MediumDec::NthRootOf(const unsigned int& n, const MediumDec& precision) const
{
	return NthRoot(*this, n, precision);
}

inline MediumDec BlazesRusCode::MediumDec::AlternativeNthRoot(const MediumDec& tValue, const unsigned int& n, const MediumDec& precision)
{
	return NthRootV2<MediumDec>(tValue, n, precision);
}

#pragma endregion Pow and Sqrt Functions