#include "PartialInt.hpp"
using PartialInt = BlazesRusCode::PartialInt;

#pragma region class_constructors

BlazesRusCode::PartialInt::PartialInt(unsigned int value, unsigned int flags)
{
	Value = value;
	Flags = flags;
}

BlazesRusCode::PartialInt::PartialInt(signed int value, unsigned int flags)
{
	Value = value;
	Flags = flags;
}

BlazesRusCode::PartialInt::PartialInt(const PartialInt& rhs)
{
	Value = rhs.Value; Flags = rhs.Flags;
}

void BlazesRusCode::PartialInt::SetValueV2(const PartialInt& rValue)
{
	Value = rValue.Value; Flags = rValue.Flags;
}

void BlazesRusCode::PartialInt::SetValue(unsigned int value, unsigned int flags)
{
	Value = value;
	Flags = flags;
}

#pragma endregion class_constructors

#pragma region ValueDefines

const PartialInt PartialInt::Zero = PartialInt::ZeroValue();

#pragma endregion ValueDefines

#pragma region Check_if_value

bool BlazesRusCode::PartialInt::IsNormalVariant() const
{
	return Flags == 0;
}

bool BlazesRusCode::PartialInt::IsPiVariant() const
{
	return Flags == 1;
}

bool BlazesRusCode::PartialInt::IsEVariant() const
{
	return Flags == 2;
}

bool BlazesRusCode::PartialInt::IsIVariant() const
{
	return Flags == 3;
}

void BlazesRusCode::PartialInt::SwitchToNormal()
{
	Flags = 0;
}

void BlazesRusCode::PartialInt::SwitchToPiVariant()
{
	Flags = 1;
}

void BlazesRusCode::PartialInt::SwitchToEVariant()
{
	Flags = 2;
}

void BlazesRusCode::PartialInt::SwitchToIVariant()
{
	Flags = 3;
}

bool BlazesRusCode::PartialInt::IsAtZero() const
{
	return Value == 0;
}

bool BlazesRusCode::PartialInt::IsNotAtZero() const
{
	return Value != 0;
}

bool BlazesRusCode::PartialInt::IsAtOne() const
{
	return Value == 1;
}

bool BlazesRusCode::PartialInt::IsEven() const
{
	return (Value & 1) == 0;
}

bool BlazesRusCode::PartialInt::IsOdd() const
{
	return (Value & 1) == 1;
}

PartialInt BlazesRusCode::PartialInt::ZeroValue()
{
	return PartialInt();
}

#pragma endregion Check_if_value

#pragma region StringOperations

void BlazesRusCode::PartialInt::ReadString(const std::string& value)
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
	#if defined(AltNum_EnablePiRep)
	if (value.find("Pi") != std::string::npos)
		DecimalHalf.Flags = 1;
	#endif
	#if defined(AltNum_EnableERep)
	if (value.last() == 'e')
		DecimalHalf.Flags = 2;
	#endif
	#if defined(AltNum_EnableIRep)
	if (value.last() == 'i')
		DecimalHalf.Flags = 3;
	#endif
}

BlazesRusCode::PartialInt::PartialInt(const char* strVal)
{
	std::string Value = strVal;
	this->ReadString(Value);
}

BlazesRusCode::PartialInt::PartialInt(const std::string& Value)
{
	this->ReadString(Value);
}

std::string BlazesRusCode::PartialInt::ToString() const
{
    return VariableConversionFunctions::UIntToStringConversion(Value);
}

std::string BlazesRusCode::PartialInt::ToDetailedString() const
{
	std::string result;
	switch(Value)
	{
	//#if defined(AltNum_EnableInfinityRep)
		case InfinityRep:
			result = "∞"; break;
	//#endif
	//#if defined(AltNum_EnableApproaching)
		case ApproachingBottomRep:
			result = "0..01"; break;
		case ApproachingTopRep:
		//#if defined(AltNum_EnableApproachingDivided)
		case ApproachingMidLeftRep://result+"/"+ExtraRep.Value
		case ApproachingMidRightRep://result+"/"+ExtraRep.Value + "0.0..01"
		//#endif
			result = "9..9"; break;
	//#endif
		default:
			result = ToString(); break;
	}
    switch(Flags)
	{
		case PiRep:
			result += "π"; break;
		case ERep:
			result += "e"; break;
		case IRep:
			result += "i"; break;
		default:
			break;
	}
	return result;
}

BlazesRusCode::PartialInt::operator std::string()
{
	return ToString();
}

#pragma endregion StringOperations

void BlazesRusCode::PartialInt::UInt64DivOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result /= rValue;
	Value = (unsigned int)result;
}

void BlazesRusCode::PartialInt::Int64DivOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	result /= rValue;//Assuming right side value is non-negative because does not support negative numbers(not worth small cost to check given primary function of class)
	Value = (unsigned int)result;
}

void BlazesRusCode::PartialInt::UInt64MultOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}

void BlazesRusCode::PartialInt::Int64MultOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}
