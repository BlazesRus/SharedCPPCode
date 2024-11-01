#include "SmallUDec.hpp"
using SmallUDec = BlazesRusCode::SmallUDec;

#pragma region ValueSetters

void BlazesRusCode::SmallUDec::SetValueToPoint5()
{
	IntHalf = 0; DecimalHalf = 50;
}

void BlazesRusCode::SmallUDec::SetValueToJustAboveZero()
{
	IntHalf = 0; DecimalHalf = 1;
}

void BlazesRusCode::SmallUDec::SetValueToPointOne()
{
	IntHalf = 0; DecimalHalf = 10;
}

#pragma endregion ValueSetters

#pragma region ValueDefines

SmallUDec BlazesRusCode::SmallUDec::AlmostOneValue()
{
	return SmallDec(0, 99);
}

SmallUDec BlazesRusCode::SmallUDec::ZeroValue()
{
	return SmallUDec();
}

SmallUDec BlazesRusCode::SmallUDec::OneValue()
{
	return SmallUDec(1);
}

SmallUDec BlazesRusCode::SmallUDec::TwoValue()
{
	return SmallUDec(2);
}

SmallUDec BlazesRusCode::SmallUDec::Point5Value()
{
	return SmallUDec(0, 50);
}

SmallUDec BlazesRusCode::SmallUDec::JustAboveZeroValue()
{
	return SmallUDec(0, 1);
}

SmallUDec BlazesRusCode::SmallUDec::MaximumValue()
{
	return SmallUDec(33554431, 99);
}

SmallUDec BlazesRusCode::SmallUDec::PointOneValue()
{
	return SmallUDec(0, 10);
}

#pragma endregion ValueDefines

#pragma region ValueDefine Source

const SmallUDec SmallUDec::AlmostOne = SmallUDec::AlmostOneValue();
const SmallUDec SmallUDec::One = SmallUDec::OneValue();
const SmallUDec SmallUDec::Two = SmallUDec::TwoValue();
const SmallUDec SmallUDec::Zero = SmallUDec::ZeroValue();
const SmallUDec SmallUDec::PointFive = SmallUDec::Point5Value();
const SmallUDec SmallUDec::JustAboveZero = SmallUDec::JustAboveZeroValue();

const SmallUDec SmallUDec::Minimum = SmallUDec::ZeroValue();
const SmallUDec SmallUDec::Maximum = SmallUDec::MaximumValue();

const SmallUDec SmallUDec::PointOne = SmallUDec::PointOneValue();
#pragma endregion ValueDefine Source

#pragma region Check_if_value

void BlazesRusCode::SmallUDec::SetAsZero()
{
	IntHalf = 0; DecimalHalf = 0;
}

void BlazesRusCode::SmallUDec::SetAsOne()
{
	IntHalf = 1; DecimalHalf = 0;
}

void BlazesRusCode::SmallUDec::SetAsOneVal()
{
	IntHalf.Value = 1; DecimalHalf = 0;
}

void BlazesRusCode::SmallUDec::SetAsValues(const unsigned int& intVal, const unsigned short& decVal)
{
	IntHalf = intVal; DecimalHalf = decVal;
}

bool BlazesRusCode::SmallUDec::IsAtZeroInt() const
{
	return IntHalf.Value == 0;
}

bool BlazesRusCode::SmallUDec::IsNotAtZeroInt() const
{
	return IntHalf.Value != 0;
}

bool BlazesRusCode::SmallUDec::IsAtOneInt() const
{
	return IntHalf.Value == 1;
}

bool BlazesRusCode::SmallUDec::IsNotAtOneInt() const
{
	return IntHalf.Value != 1;
}

bool BlazesRusCode::SmallUDec::IsZero() const
{
	return DecimalHalf == 0 && IntHalf.Value == 0;
}

bool BlazesRusCode::SmallUDec::IsOne() const
{
	return DecimalHalf == 0 && IntHalf == MirroredInt::One;
}

bool BlazesRusCode::SmallUDec::IsNegOne() const
{
	return DecimalHalf == 0 && IntHalf == MirroredInt::NegativeOne;
}

bool BlazesRusCode::SmallUDec::IsOneVal() const
{
	return DecimalHalf == 0 && IntHalf.Value == 1;
}

bool BlazesRusCode::SmallUDec::IsOneVariantVal() const
{
	return DecimalHalf.Value == 0 && IntHalf.Value == 1;
}

#pragma endregion Check_if_value

#pragma region RangeLimits

void BlazesRusCode::SmallUDec::SetAsMaximum()
{
	IntHalf = 4294967295; DecimalHalf = 99;
}

void BlazesRusCode::SmallUDec::SetAsMinimum()
{
	IntHalf = 0; DecimalHalf = 0;
}

#pragma endregion RangeLimits

#pragma region ConvertToOtherTypes

    #if defined(AltNum_EnableFloatingConversion)
float BlazesRusCode::SmallUDec::toFloat() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	float Value;
	Value = (float)IntHalf.Value;
	if (DecimalHalf != 0)
        Value += ((float)DecimalHalf * 0.01f);
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

double BlazesRusCode::SmallUDec::toDouble() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	double Value;
	Value = (double)IntHalf.Value;
	if (DecimalHalf != 0)
        Value += ((double)DecimalHalf * 0.01);
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

long double BlazesRusCode::SmallUDec::toDecimal() const
{
	#if defined(AltNum_UseLegacyFloatingConversion)
	long double Value;
	Value = (long double)IntHalf.Value;
	if (DecimalHalf != 0)
        Value += ((long double)DecimalHalf * 0.01L);
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

void BlazesRusCode::SmallUDec::DivideByTwo()
{
	if (DecimalHalf == 0 && (IntHalf.Value & 1) == 1)//Check if number is odd
		UIntDivOp(2);
	else
		IntHalf.Value /= 2;
}

void BlazesRusCode::SmallUDec::DivideByFour()
{
	//Checking if divisible by 4 based on
	//https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
	//checking if divible by 8 equals (((n >> 3) << 3) == n)
	if (DecimalHalf == 0 && (((IntHalf.Value >> 2) << 2) == IntHalf.Value))//Check if number can be perfectly divided by 4
		IntHalf.Value /= 4;
	else
		UIntDivOp(4);
}

SmallUDec BlazesRusCode::SmallUDec::DividedByTwo() const
{
	SmallUDec result = *this; result.DivideByTwo();
	return result;
}

SmallUDec BlazesRusCode::SmallUDec::DividedByFour() const
{
	SmallUDec result = *this; result.DivideByFour();
	return result;
}

#pragma endregion Other Division Operations

#pragma region Other multiplication operations

void BlazesRusCode::SmallUDec::MultiplyByTwo()
{
	UIntMultOp(2);
}

void BlazesRusCode::SmallUDec::MultiplyByFour()
{
	UIntMultOp(4);
}


SmallUDec BlazesRusCode::SmallUDec::MultipliedByTwo() const
{
	SmallUDec result = *this; result.UIntMultOp(2);
	return result;
}

SmallUDec BlazesRusCode::SmallUDec::MultipliedByFour() const
{
	SmallUDec result = *this; result.UIntMultOp(4);
	return result;
}


#pragma endregion Other multiplication operations

#pragma region NormalRep Integer Addition Operations

void SmallUDec::UIntAddOp(const unsigned int& rValue)
{
	IntHalf += rValue;
}

SmallUDec& BlazesRusCode::SmallUDec::UIntAddOperation(const unsigned int& rValue)
{
	IntHalf += rValue; return *this;
}

#pragma endregion NormalRep Integer Addition Operations

#pragma region NormalRep Integer Subtraction Operations

void BlazesRusCode::SmallUDec::UIntSubOp(const unsigned int& rValue)
{
	IntHalf -= rValue;
}

SmallUDec& BlazesRusCode::SmallUDec::UIntSubOperation(const unsigned int& rValue)
{
	IntHalf -= rValue; return *this;
}

#pragma endregion NormalRep Integer Subtraction Operations

#pragma region Truncation Functions

SmallUDec BlazesRusCode::SmallUDec::Abs(const SmallUDec& tValue) {
	return AbsV1<SmallUDec>(tValue);
}

void BlazesRusCode::SmallUDec::ApplyFloorOf(const int& precision)
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

signed int BlazesRusCode::SmallUDec::FloorIntOf() const
{
	if (DecimalHalf == 0)
		return GetIntHalf();
	else if (IntHalf == MirroredInt::NegativeZero)
		return -1;
	else
		return GetIntHalf() - 1;
}

int BlazesRusCode::SmallUDec::CeilIntOf() const
{
	if (DecimalHalf == 0)
		return GetIntHalf();
	else if (IntHalf == MirroredInt::NegativeZero)
		return 0;
	else
		return GetIntHalf() + 1;
}

SmallUDec BlazesRusCode::SmallUDec::Trunc(const SmallUDec& tValue) { return tValue.TruncOfV1<SmallUDec>(); }

#pragma endregion Truncation Functions

#pragma region String Commands

void BlazesRusCode::SmallUDec::ReadString(const std::string& Value)
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

BlazesRusCode::SmallUDec::SmallUDec(const char* strVal)
{
	std::string Value = strVal;
	this->ReadString(Value);
}

BlazesRusCode::SmallUDec::SmallUDec(const std::string& Value)
{
	this->ReadString(Value);
}

std::string BlazesRusCode::SmallUDec::ToString()
{
	std::string Value = std::string(IntHalf);
	if (DecimalHalf != 0)
	{
		Value += ".";
		Value += std::string(DecimalHalf);
	}
	return Value;
}

std::string BlazesRusCode::SmallUDec::ToFullString()
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
void BlazesRusCode::SmallUDec::SetFloatVal(const float& Value)
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

void BlazesRusCode::SmallUDec::SetDoubleVal(const double& Value)
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

void BlazesRusCode::SmallUDec::SetDecimalVal(const long double& Value)
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

void BlazesRusCode::SmallUDec::SetBoolVal(const bool& Value)
{
	IntHalf = Value == false ? 0 : 1;
	DecimalHalf = 0;
}

void BlazesRusCode::SmallUDec::SetIntVal(const int& Value)
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

void BlazesRusCode::SmallUDec::SetUIntVal(const unsigned int& Value)
{
	IntHalf = Value;
	DecimalHalf = 0;
}

#pragma endregion ConvertFromOtherTypes

#pragma region Pow and Sqrt Functions

SmallUDec BlazesRusCode::SmallUDec::Sqrt(const auto& value, const int& precision)
{
	return SqrtV1<SmallUDec>(value, precision);
}

SmallUDec BlazesRusCode::SmallUDec::SqrtOf(const int& precision) const {
	return Sqrt(*this, precision);
}

SmallUDec BlazesRusCode::SmallUDec::UnsignedNthRoot(const SmallUDec& tValue, const unsigned int& n, const SmallUDec& precision)
{
	return UnsignedNthRootV1<SmallUDec>(tValue, n, precision);
}

SmallUDec BlazesRusCode::SmallUDec::NthRootOf(const unsigned int& n, const SmallUDec& precision) const
{
	return NthRoot(*this, n, precision);
}

SmallUDec BlazesRusCode::SmallUDec::AlternativeNthRoot(const SmallUDec& tValue, const unsigned int& n, const SmallUDec& precision)
{
	return NthRootV2<SmallUDec>(tValue, n, precision);
}

#pragma endregion Pow and Sqrt Functions