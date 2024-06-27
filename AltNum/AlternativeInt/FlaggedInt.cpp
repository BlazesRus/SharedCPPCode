#include "FlaggedInt.hpp"
using FlaggedInt = BlazesRusCode::FlaggedInt;

#pragma region class_constructors

BlazesRusCode::FlaggedInt::FlaggedInt(const unsigned int& value, const unsigned int& isAltRep)
{
	Value = value;
	IsAltRep = isAltRep;
}

BlazesRusCode::FlaggedInt::FlaggedInt(const signed int& value, const unsigned int& isAltRep)
{
	Value = value;
	IsAltRep = isAltRep;
}

BlazesRusCode::FlaggedInt::FlaggedInt(const FlaggedInt& rhs)
{
	Value = rhs.Value; IsAltRep = rhs.IsAltRep;
}

void BlazesRusCode::FlaggedInt::SetValueV2(const FlaggedInt& rValue)
{
	Value = rValue.Value; IsAltRep = rValue.IsAltRep;
}

void BlazesRusCode::FlaggedInt::SetValue(const unsigned int& value, const unsigned int& isAltRep)
{
	Value = value;
	IsAltRep = isAltRep;
}

#pragma endregion class_constructors

#pragma region ValueDefines

FlaggedInt BlazesRusCode::FlaggedInt::MaximumValue()
{
	return FlaggedInt(2147483647, 1);
}

FlaggedInt BlazesRusCode::FlaggedInt::AltMaximumValue()
{
	return FlaggedInt(2147483647, 0);
}

FlaggedInt BlazesRusCode::FlaggedInt::OneValue()
{
	return FlaggedInt(1, 1);
}

FlaggedInt BlazesRusCode::FlaggedInt::TwoValue()
{
	return FlaggedInt(2, 1);
}

FlaggedInt BlazesRusCode::FlaggedInt::ZeroValue()
{
	return FlaggedInt(0, 1);
}

const FlaggedInt FlaggedInt::Maximum = FlaggedInt::MaximumValue();
const FlaggedInt FlaggedInt::AltMaximum = FlaggedInt::AltMaximumValue();

const FlaggedInt FlaggedInt::One = FlaggedInt::OneValue();
const FlaggedInt FlaggedInt::Two = FlaggedInt::TwoValue();

const FlaggedInt FlaggedInt::Zero = FlaggedInt::ZeroValue();

#pragma endregion ValueDefines

#pragma region Check_if_value

bool BlazesRusCode::FlaggedInt::IsAlternative() const
{
	return IsAltRep == 1;
}

bool BlazesRusCode::FlaggedInt::IsNormal()
{
	return IsAltRep == 0;
}

void BlazesRusCode::FlaggedInt::SwitchToAlternative()
{
	IsAltRep = 1;
}

void BlazesRusCode::FlaggedInt::SwitchToNormal()
{
	IsAltRep = 0;
}

bool BlazesRusCode::FlaggedInt::IsAtZero() const
{
	return Value == 0;
}

bool BlazesRusCode::FlaggedInt::IsNotAtZero() const
{
	return Value != 0;
}

bool BlazesRusCode::FlaggedInt::IsAtOne() const
{
	return Value == 1;
}

bool BlazesRusCode::FlaggedInt::IsNotOne() const
{
	return Value != 1;
}

bool BlazesRusCode::FlaggedInt::IsEven() const
{
	return (Value & 1) == 0;
}

bool BlazesRusCode::FlaggedInt::IsOdd() const
{
	return (Value & 1) == 1;
}

#pragma endregion Check_if_value

#pragma region StringOperations

void BlazesRusCode::FlaggedInt::ReadString(const std::string& value)
{
	Value = 0;
	std::string WholeNumberBuffer = "";

	int charAsNumber;
	int charAsNumberInPlace;
	for (char const& StringChar : value)
	{
		if (VariableConversionFunctions::IsDigit(StringChar))
			WholeNumberBuffer += StringChar;
		else if (StringChar != ' ')
			break;//Stop Extracting after encounter non-number character such as i or .
	}
	unsigned int PlaceNumber = WholeNumberBuffer.length() - 1;//Last character is digit one
	for (char const& StringChar : WholeNumberBuffer)
	{
		charAsNumber = VariableConversionFunctions::CharAsInt(StringChar);
		charAsNumberInPlace = (charAsNumber * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
		if (StringChar != '0')
		{
			Value += charAsNumberInPlace;
		}
		PlaceNumber--;
	}
}

BlazesRusCode::FlaggedInt::FlaggedInt(const char* strVal)
{
	std::string Value = strVal;
	this->ReadString(Value);
}


BlazesRusCode::FlaggedInt::FlaggedInt(const std::string& Value)
{
	this->ReadString(Value);
}

std::string BlazesRusCode::FlaggedInt::ToString() const
{
	return VariableConversionFunctions::UIntToStringConversion(Value);
}

std::string BlazesRusCode::FlaggedInt::ToDetailedString(const PartialInt& DecHalf) const
{
	std::string result;
	switch (DecHalf.Value)
	{
		#if defined(AltNum_EnableApproachingDivided)
	case PartialInt::ApproachingMidRightRep://DecHalf.ToDetailedString()+"/"+ExtraRep.Value + "0.0..01"
		result = "/" + ToString() + "+ 0.0..01";
		#endif
	default:
		result = "/" + ToString();
	}
	return result;
}

BlazesRusCode::FlaggedInt::operator std::string()
{
	return ToString();
}

#pragma endregion StringOperations

void BlazesRusCode::FlaggedInt::UInt64DivOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result /= rValue;
	Value = (unsigned int)result;
}

void BlazesRusCode::FlaggedInt::Int64DivOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	result /= rValue;//Assuming right side value is non-negative because does not support negative numbers(not worth small cost to check given primary function of class)
	Value = (unsigned int)result;
}

void BlazesRusCode::FlaggedInt::UInt64MultOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}

void BlazesRusCode::FlaggedInt::Int64MultOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}
