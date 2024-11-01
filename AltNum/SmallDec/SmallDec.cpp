#include "SmallDec.hpp"
using SmallDec = BlazesRusCode::SmallDec;

#pragma region ValueSetters

void BlazesRusCode::SmallDec::SetValueToPoint5()
{
	IntHalf = 0; DecimalHalf = 50;
}

void BlazesRusCode::SmallDec::SetValueToJustAboveZero()
{
	IntHalf = 0; DecimalHalf = 1;
}


void BlazesRusCode::SmallDec::SetValueToPointOne()
{
	IntHalf = 0; DecimalHalf = 10;
}

#pragma endregion ValueSetters

#pragma region ValueDefines

SmallDec BlazesRusCode::SmallDec::AlmostOneValue()
{
	return SmallDec(0, 99);
}

SmallDec BlazesRusCode::SmallDec::ZeroValue()
{
	return SmallDec();
}

SmallDec BlazesRusCode::SmallDec::OneValue()
{
	return SmallDec(1);
}

SmallDec BlazesRusCode::SmallDec::TwoValue()
{
	return SmallDec(2);
}

SmallDec BlazesRusCode::SmallDec::NegativeOneValue()
{
	return SmallDec(1,0,false);
}

SmallDec BlazesRusCode::SmallDec::Point5Value()
{
	return SmallDec(0, 50);
}

SmallDec BlazesRusCode::SmallDec::JustAboveZeroValue()
{
	return SmallDec(0, 1);
}

SmallDec BlazesRusCode::SmallDec::MinimumValue()
{
	return SmallDec(16777215, 99, false);
}

SmallDec BlazesRusCode::SmallDec::MaximumValue()
{
	return SmallDec(16777215, 99);
}

SmallDec BlazesRusCode::SmallDec::NegativePointFiveValue()
{
	return SmallDec(0, 50, false);
}

SmallDec BlazesRusCode::SmallDec::PointOneValue()
{
	return SmallDec(0, 10);
}

#pragma endregion ValueDefines

#pragma region ValueDefine Source

const SmallDec SmallDec::AlmostOne = SmallDec::AlmostOneValue();
const SmallDec SmallDec::One = SmallDec::OneValue();
const SmallDec SmallDec::Two = SmallDec::TwoValue();
const SmallDec SmallDec::NegativeOne = SmallDec::NegativeOneValue();
const SmallDec SmallDec::Zero = SmallDec::ZeroValue();
const SmallDec SmallDec::PointFive = SmallDec::Point5Value();
const SmallDec SmallDec::JustAboveZero = SmallDec::JustAboveZeroValue();

const SmallDec SmallDec::Minimum = SmallDec::MinimumValue();
const SmallDec SmallDec::Maximum = SmallDec::MaximumValue();

const SmallDec SmallDec::NegativePointFive = SmallDec::NegativePointFiveValue();
const SmallDec SmallDec::PointOne = SmallDec::PointOneValue();
#pragma endregion ValueDefine Source

#pragma region Negative_Status

bool BlazesRusCode::SmallDec::IsPositive() const
{
	return IntHalf.IsPositive();
}

bool BlazesRusCode::SmallDec::IsNegative() const
{
	return IntHalf.IsNegative();
}

void BlazesRusCode::SmallDec::SwapNegativeStatus()
{
	Sign ^= 1;
}

#pragma endregion Negative_Status

#pragma region Check_if_value

void BlazesRusCode::SmallDec::SetAsZero()
{
	IntHalf = 0; DecimalHalf = 0;
}

void BlazesRusCode::SmallDec::SetAsOne()
{
	IntHalf = 1; DecimalHalf = 0;
}

void BlazesRusCode::SmallDec::SetAsOneVal()
{
	IntHalf.Value = 1; DecimalHalf = 0;
}

void BlazesRusCode::SmallDec::SetAsValues(const signed int& intVal, const unsigned short& decVal)
{
    if(intVal<0)
    {
        IntHalf = -intVal; DecimalHalf = decVal;
        Sign = NegativeSign;
    }
    else
    {
        IntHalf = intVal; DecimalHalf = decVal;
        Sign = PositiveSign;
    }
}

bool BlazesRusCode::SmallDec::IsAtZeroInt() const
{
	return IntHalf.Value == 0;
}

bool BlazesRusCode::SmallDec::IsNotAtZeroInt() const
{
	return IntHalf.Value != 0;
}

bool BlazesRusCode::SmallDec::IsAtOneInt() const
{
	return IntHalf.Value == 1;
}

bool BlazesRusCode::SmallDec::IsNotAtOneInt() const
{
	return IntHalf.Value != 1;
}

bool BlazesRusCode::SmallDec::IsZero() const
{
	return DecimalHalf == 0 && IntHalf.Value == 0;
}

bool BlazesRusCode::SmallDec::IsOne() const
{
	return DecimalHalf == 0 && IntHalf == MirroredInt::One;
}

bool BlazesRusCode::SmallDec::IsNegOne() const
{
	return DecimalHalf == 0 && IntHalf == MirroredInt::NegativeOne;
}

bool BlazesRusCode::SmallDec::IsOneVal() const
{
	return DecimalHalf == 0 && IntHalf.Value == 1;
}

bool BlazesRusCode::SmallDec::IsOneVariantVal() const
{
	return DecimalHalf.Value == 0 && IntHalf.Value == 1;
}

#pragma endregion Check_if_value

#pragma region RangeLimits

void BlazesRusCode::SmallDec::SetAsMaximum()
{
	IntHalf = MirroredInt::Maximum; DecimalHalf = 99;
}

void BlazesRusCode::SmallDec::SetAsMinimum()
{
	IntHalf = MirroredInt::Minimum; DecimalHalf = 99;
}

#pragma endregion RangeLimits

#pragma region ConvertToOtherTypes

    #if defined(AltNum_EnableFloatingConversion)
float BlazesRusCode::SmallDec::toFloat() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	float Value;
	if (IntHalf.IsNegative())
	{
		Value = (float)-IntHalf.Value;
		if (DecimalHalf != 0)
            Value -= ((float)DecimalHalf * 0.01f);
	}
	else
	{
		Value = (float)IntHalf.Value;
		if (DecimalHalf != 0)
            Value += ((float)DecimalHalf * 0.01f);
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

double BlazesRusCode::SmallDec::toDouble() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	double Value;
	if (IntHalf < 0)
	{
		Value = (double)-IntHalf.Value;
		if (DecimalHalf != 0)
            Value -= ((double)DecimalHalf * 0.01);
	}
	else
	{
		Value = (double)IntHalf.Value;
		if (DecimalHalf != 0)
            Value += ((double)DecimalHalf * 0.01);
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

long double BlazesRusCode::SmallDec::toDecimal() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	long double Value;
	if (IntHalf < 0)
	{
		Value = (long double)-IntHalf.Value;
		if (DecimalHalf != 0)
            Value -= ((long double)DecimalHalf * 0.01L);
	}
	else
	{
		Value = (long double)IntHalf.Value;
		if (DecimalHalf != 0)
            Value += ((long double)DecimalHalf * 0.01L);
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

void BlazesRusCode::SmallDec::DivideByTwo()
{
	if (DecimalHalf == 0 && (IntHalf.Value & 1) == 1)//Check if number is odd
		UIntDivOp(2);
	else
		IntHalf.Value /= 2;
}

void BlazesRusCode::SmallDec::DivideByFour()
{
	//Checking if divisible by 4 based on
	//https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
	//checking if divible by 8 equals (((n >> 3) << 3) == n)
	if (DecimalHalf == 0 && (((IntHalf.Value >> 2) << 2) == IntHalf.Value))//Check if number can be perfectly divided by 4
		IntHalf.Value /= 4;
	else
		UIntDivOp(4);
}

SmallDec BlazesRusCode::SmallDec::DividedByTwo() const
{
	SmallDec result = *this; result.DivideByTwo();
	return result;
}

SmallDec BlazesRusCode::SmallDec::DividedByFour() const
{
	SmallDec result = *this; result.DivideByFour();
	return result;
}

#pragma endregion Other Division Operations

#pragma region Other multiplication operations

void BlazesRusCode::SmallDec::MultiplyByTwo()
{
	UIntMultOp(2);
}

void BlazesRusCode::SmallDec::MultiplyByFour()
{
	UIntMultOp(4);
}


SmallDec BlazesRusCode::SmallDec::MultipliedByTwo() const
{
	SmallDec result = *this; result.UIntMultOp(2);
	return result;
}

SmallDec BlazesRusCode::SmallDec::MultipliedByFour() const
{
	SmallDec result = *this; result.UIntMultOp(4);
	return result;
}


#pragma endregion Other multiplication operations

#pragma region NormalRep Integer Addition Operations

void SmallDec::UIntAddOp(const unsigned int& rValue)
{
	{
		if (DecimalHalf.Value == 0)
			NRepSkippingUnsignedAddOp(rValue);
		else {
			int signBeforeOp = Sign;
			IntHalf += rValue;
			if (signBeforeOp != Sign)//Invert the decimal section
				DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
		}
	}
}

SmallDec& BlazesRusCode::SmallDec::UIntAddOperation(const unsigned int& rValue)
{
	UIntAddOp(rValue); return *this;
}

#pragma endregion NormalRep Integer Addition Operations

#pragma region NormalRep Integer Subtraction Operations

void BlazesRusCode::SmallDec::UnsignedMirroredSubOp(const MirroredInt& rValue)
{
	if (DecimalHalf.Value == 0)
		NRepSkippingUnsignedSubOp(rValue);
	else {
		int signBeforeOp = Sign;
		IntHalf -= rValue.Value;
		if (signBeforeOp != Sign)//Invert the decimal section
			DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
	}
}

void BlazesRusCode::SmallDec::MirroredSubOp(const MirroredInt& rValue)
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

void BlazesRusCode::SmallDec::UIntSubOp(const unsigned int& rValue)
{
	if (DecimalHalf.Value == 0)
		IntHalf.NRepSkippingUnsignedSubOp(rValue);
	else {
		unsigned int signBeforeOp = IntHalf.Sign;
		IntHalf -= rValue;
		if (signBeforeOp != IntHalf.Sign)//Invert the decimal section
			DecimalHalf = DecimalOverflow - DecimalHalf;
	}
}

SmallDec& BlazesRusCode::SmallDec::UIntSubOperation(const unsigned int& rValue)
{
	UIntSubOp(rValue); return *this;
}

#pragma endregion NormalRep Integer Subtraction Operations

#pragma region Truncation Functions

SmallDec BlazesRusCode::SmallDec::Abs(const SmallDec& tValue) {
	return AbsV1<SmallDec>(tValue);
}

void BlazesRusCode::SmallDec::ApplyFloorOf(const int& precision)
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

signed int BlazesRusCode::SmallDec::FloorIntOf() const
{
	if (DecimalHalf == 0)
		return GetIntHalf();
	else if (IntHalf == MirroredInt::NegativeZero)
		return -1;
	else
		return GetIntHalf() - 1;
}

int BlazesRusCode::SmallDec::CeilIntOf() const
{
	if (DecimalHalf == 0)
		return GetIntHalf();
	else if (IntHalf == MirroredInt::NegativeZero)
		return 0;
	else
		return GetIntHalf() + 1;
}

SmallDec BlazesRusCode::SmallDec::Trunc(const SmallDec& tValue) { return tValue.TruncOfV1<SmallDec>(); }

#pragma endregion Truncation Functions

#pragma region String Commands

void BlazesRusCode::SmallDec::ReadString(const std::string& Value)
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
		if(PlaceNumber==0)//Limit stored decimal numbers to the amount it can store
			break;
	}
}

BlazesRusCode::SmallDec::SmallDec(const char* strVal)
{
	std::string Value = strVal;
	this->ReadString(Value);
}

BlazesRusCode::SmallDec::SmallDec(const std::string& Value)
{
	this->ReadString(Value);
}

std::string BlazesRusCode::SmallDec::ToString()
{
	std::string Value = std::string(IntHalf);
	if (DecimalHalf != 0)
	{
		Value += ".";
		Value += std::string(DecimalHalf);
	}
	return Value;
}

std::string BlazesRusCode::SmallDec::ToFullString()
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
void BlazesRusCode::SmallDec::SetFloatVal(const float& Value)
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

void BlazesRusCode::SmallDec::SetDoubleVal(const double& Value)
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

void BlazesRusCode::SmallDec::SetDecimalVal(const long double& Value)
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

void BlazesRusCode::SmallDec::SetBoolVal(const bool& Value)
{
	IntHalf = Value == false ? 0 : 1;
	DecimalHalf = 0;
}

void BlazesRusCode::SmallDec::SetIntVal(const int& Value)
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

void BlazesRusCode::SmallDec::SetUIntVal(const unsigned int& Value)
{
	IntHalf = Value;
	DecimalHalf = 0;
}

#pragma endregion ConvertFromOtherTypes

#pragma region Pow and Sqrt Functions

SmallDec BlazesRusCode::SmallDec::Sqrt(const auto& value, const int& precision)
{
	return SqrtV1<SmallDec>(value, precision);
}

SmallDec BlazesRusCode::SmallDec::SqrtOf(const int& precision) const {
	return Sqrt(*this, precision);
}

SmallDec BlazesRusCode::SmallDec::UnsignedNthRoot(const SmallDec& tValue, const unsigned int& n, const SmallDec& precision)
{
	return UnsignedNthRootV1<SmallDec>(tValue, n, precision);
}

SmallDec BlazesRusCode::SmallDec::NthRootOf(const unsigned int& n, const SmallDec& precision) const
{
	return NthRoot(*this, n, precision);
}

SmallDec BlazesRusCode::SmallDec::AlternativeNthRoot(const SmallDec& tValue, const unsigned int& n, const SmallDec& precision)
{
	return NthRootV2<SmallDec>(tValue, n, precision);
}

#pragma endregion Pow and Sqrt Functions