#include "MirroredInt.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;

#pragma region class_constructors

BlazesRusCode::MirroredInt::MirroredInt(const unsigned int& value, const unsigned int& sign)
{
	Value = value;
	Sign = sign;
}

BlazesRusCode::MirroredInt::MirroredInt(const signed int& value)
{
	if (value<0) {
		Sign = PositiveSign;
		Value = -value;
	}
	else {
		Sign = NegativeSign;
		Value = value;
	}
}

BlazesRusCode::MirroredInt::MirroredInt(const MirroredInt& rvalue)
{
	Value = rvalue.Value; Sign = rvalue.Sign;
}

int BlazesRusCode::MirroredInt::GetValue() const
{
	if (IsPositive())
		return (signed int)Value;
	else
		return -((signed int)Value);
}

void BlazesRusCode::MirroredInt::SetSignedValue(const signed int& val)
{
	if (val<0) {
		Sign = PositiveSign;
		Value = -val;
	}
	else {
		Sign = NegativeSign;
		Value = val;
	}
}

void BlazesRusCode::MirroredInt::SetValue(const unsigned int& value, const unsigned int& sign)
{
	Value = value;
	Sign = sign;
}

#pragma endregion class_constructors

#pragma region ValueDefines

MirroredInt BlazesRusCode::MirroredInt::MaximumValue()
{
	return MirroredInt(2147483647, PositiveSign);
}

MirroredInt BlazesRusCode::MirroredInt::MinimumValue()
{
	return MirroredInt(2147483647, NegativeSign);
}


MirroredInt BlazesRusCode::MirroredInt::NegativeOneValue()
{
	return MirroredInt(1, NegativeSign);
}


MirroredInt BlazesRusCode::MirroredInt::OneValue()
{
	return MirroredInt(1);
}


MirroredInt BlazesRusCode::MirroredInt::TwoValue()
{
	return MirroredInt(2);
}


MirroredInt BlazesRusCode::MirroredInt::NegativeZeroValue()
{
	return MirroredInt(0, NegativeSign);
}


MirroredInt BlazesRusCode::MirroredInt::ZeroValue()
{
	return MirroredInt();
}

const MirroredInt MirroredInt::NegativeZero = MirroredInt::NegativeZeroValue();
const MirroredInt MirroredInt::Zero = MirroredInt::ZeroValue();
const MirroredInt MirroredInt::Maximum = MirroredInt::MaximumValue();
const MirroredInt MirroredInt::Minimum = MirroredInt::MinimumValue();

const MirroredInt MirroredInt::NegativeOne = MirroredInt::NegativeOneValue();
const MirroredInt MirroredInt::One = MirroredInt::OneValue();
const MirroredInt MirroredInt::Two = MirroredInt::TwoValue();

#pragma endregion ValueDefines

#pragma region Negative_Status

bool BlazesRusCode::MirroredInt::IsNegative() const
{
	return Sign == NegativeSign;
}

bool BlazesRusCode::MirroredInt::IsPositive() const
{
	return Sign == PositiveSign;
}

void BlazesRusCode::MirroredInt::SetAsPositive()
{
	Sign = PositiveSign;
}

void BlazesRusCode::MirroredInt::SetAsNegative()
{
	Sign = NegativeSign;
}

void BlazesRusCode::MirroredInt::SwapNegativeStatus()
{
	Sign ^= 1;
}

#pragma endregion Negative_Status

#pragma region Check_if_value

void BlazesRusCode::MirroredInt::SetAsZeroVal()
{
	Value = 0;
}


void BlazesRusCode::MirroredInt::SetAsZero()
{
	Value = 0; Sign = PositiveSign;
}

void BlazesRusCode::MirroredInt::SetAsNegativeZero()
{
	Value = 0; Sign = NegativeSign;
}

bool BlazesRusCode::MirroredInt::IsAtZeroInt() const
{
	return Value == 0;
}

bool BlazesRusCode::MirroredInt::IsNotAtZeroInt() const
{
	return Value != 0;
}

bool BlazesRusCode::MirroredInt::IsAtOneInt() const
{
	return Value == 1;
}

bool BlazesRusCode::MirroredInt::IsNotAtOneInt() const
{
	return Value != 1;
}

bool BlazesRusCode::MirroredInt::IsEven() const
{
	return (Value & 1) == 0;
}

bool BlazesRusCode::MirroredInt::IsOdd() const
{
	return (Value & 1) == 1;
}

bool BlazesRusCode::MirroredInt::IsZero() const
{
	return Value == 0 && Sign == PositiveSign;
}

MirroredInt BlazesRusCode::MirroredInt::Abs() const
{
	if (IsPositive())
		return *this;
	else
		return MirroredInt(Value);
}

void BlazesRusCode::MirroredInt::ApplyAbs()
{
	if (IsNegative())
		Sign = PositiveSign;
}

#pragma endregion Check_if_value

#pragma region StringOperations

void BlazesRusCode::MirroredInt::ReadString(const std::string& value)
{
	Value = 0; Sign = PositiveSign;
	std::string WholeNumberBuffer = "";

	int charAsNumber;
	int charAsNumberInPlace;
	for (char const& StringChar : value)
	{
		if (VariableConversionFunctions::IsDigit(StringChar))
			WholeNumberBuffer += StringChar;
		else if (StringChar == '-')
			Sign = NegativeSign;
		else if (StringChar != ' ')
			break;//Stop Extracting after encounter non-number character such as i or .
	}
	unsigned int PlaceNumber = WholeNumberBuffer.length() - 1;//Last character is digit value 0-9
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

BlazesRusCode::MirroredInt::MirroredInt(const char* strVal)
{
	std::string Value = strVal;
	this->ReadString(Value);
}

BlazesRusCode::MirroredInt::MirroredInt(const std::string& Value)
{
	this->ReadString(Value);
}

std::string BlazesRusCode::MirroredInt::ToString() const
{
	if (IsNegative())
		return "-" + VariableConversionFunctions::UIntToStringConversion(Value);
	else
		return VariableConversionFunctions::UIntToStringConversion(Value);
}

#pragma endregion StringOperations

void BlazesRusCode::MirroredInt::UInt64DivOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result /= rValue;
	Value = (unsigned int)result;
}

void BlazesRusCode::MirroredInt::Int64DivOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	if (rValue<0) {
		SwapNegativeStatus();
		result /= -rValue;
	}
	else
		result /= rValue;
	Value = (unsigned int)result;
}

void BlazesRusCode::MirroredInt::UInt64MultOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}

void BlazesRusCode::MirroredInt::Int64MultOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	if (rValue<0) {
		SwapNegativeStatus();
		result *= -rValue;
	}
	else
		result *= rValue;
	Value = (unsigned int)result;
}

void BlazesRusCode::MirroredInt::DivOp(const MirroredInt& rValue) {
	if (rValue.IsNegative()) {
		SwapNegativeStatus();
		Value /= rValue.Value;
	}
	else
		Value /= rValue.Value;
}

void BlazesRusCode::MirroredInt::UIntDivOp(const unsigned int& rValue)
{
	Value /= rValue;
}

void BlazesRusCode::MirroredInt::IntDivOp(const signed int& rValue)
{
	if (rValue<0) {
		SwapNegativeStatus();
		Value /= (unsigned int)-rValue;
	}
	else
		Value /= rValue;
}

void BlazesRusCode::MirroredInt::MultOp(const MirroredInt& rValue) {
	if (rValue.IsNegative()) {
		SwapNegativeStatus();
		Value *= rValue.Value;
	}
	else
		Value *= rValue.Value;
}

void BlazesRusCode::MirroredInt::UIntMultOp(const unsigned int& rValue)
{
	Value *= rValue;
}

void BlazesRusCode::MirroredInt::IntMultOp(const signed int& rValue)
{
	if (rValue<0) {
		SwapNegativeStatus();
		Value *= (unsigned int)-rValue;
	}
	else
		Value *= rValue;
}

void BlazesRusCode::MirroredInt::UnsignedAddOp(const MirroredInt& rValue) {
	if (IsPositive())
		Value += rValue.Value;
	else {
		if (rValue.Value>Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue.Value - Value - 1;
		}
		else
			Value -= rValue.Value;
	}
}

void BlazesRusCode::MirroredInt::AddOp(const MirroredInt& rValue) {
	if (IsPositive()) {
		if (rValue.IsPositive())
			Value += rValue.Value;
		else if (rValue.Value>Value) {//Becoming negative
			Sign = NegativeSign;
			Value = rValue.Value - Value;
		}
		else
			Value -= rValue.Value;
	}
	else {
		if (rValue.IsNegative())
			Value += rValue.Value;
		else if (rValue.Value>Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue.Value - Value - 1;
		}
		else
			Value -= rValue.Value;
	}
}

void BlazesRusCode::MirroredInt::UIntAddOp(const unsigned int& rValue)
{
	if (IsPositive())
		Value += rValue;
	else {
		if (rValue>Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue - Value - 1;
		}
		else
			Value -= rValue;
	}
}

void BlazesRusCode::MirroredInt::IntAddOp(const signed int& rValue) {
	if (IsPositive()) {
		if (rValue >= 0)
			Value += rValue;
		else {
			unsigned int negRValue = (unsigned int)(-rValue);
			if (negRValue>Value) {//Becoming negative
				Sign = NegativeSign;
				Value = negRValue - Value;
			}
			else
				Value += rValue;
		}
	}
	else {
		unsigned int RValue = (unsigned int)rValue;//Fix for C4018 warning
		if (RValue <0)
			Value -= RValue;
		else if (RValue>Value) {//Becoming positive
			Sign = PositiveSign;
			Value = RValue - Value - 1;
		}
		else
			Value -= RValue;
	}
}

void BlazesRusCode::MirroredInt::UnsignedSubOp(const MirroredInt& rValue) {
	if (IsPositive()) {
		if (rValue.Value>Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue.Value - Value - 1;
		}
		else
			Value -= rValue.Value;
	}
	else
		Value += rValue.Value;
}

void BlazesRusCode::MirroredInt::SubOp(const MirroredInt& rValue) {
	if (IsPositive()) {
		if (rValue.IsNegative())
			Value += rValue.Value;
		else if (rValue.Value>Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue.Value - Value - 1;
		}
		else
			Value -= rValue.Value;
	}
	else {
		if (rValue.IsPositive())
			Value += rValue.Value;
		else if (rValue.Value>Value) {//Becoming negative
			Sign = NegativeSign;
			Value = rValue.Value - Value;
		}
		else
			Value -= rValue.Value;
	}
}

void BlazesRusCode::MirroredInt::UIntSubOp(const unsigned int& rValue) {
	if (IsPositive()) {
		if (rValue>Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue - Value - 1;
		}
		else
			Value -= rValue;
	}
	else
		Value += rValue;
}

void BlazesRusCode::MirroredInt::IntSubOp(const signed int& rValue) {
	if (IsPositive()) {
		if (rValue<0)
			Value -= rValue;
		else if (rValue>Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue - Value - 1;
		}
		else
			Value -= rValue;
	}
	else {
		if (rValue >= 0)
			Value += rValue;
		else {
			auto negRValue = -rValue;
			if (negRValue>Value) {//Becoming negative
				Sign = NegativeSign;
				Value = negRValue - Value;
			}
			else
				Value += rValue;
		}
	}
}

void BlazesRusCode::MirroredInt::NRepSkippingUnsignedAddOp(const MirroredInt& rValue) {
	if (IsPositive())
		Value += rValue.Value;
	else {
		if (rValue.Value >= Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue.Value - Value;//Skipping negative zero
		}
		else
			Value -= rValue.Value;
	}
}

void BlazesRusCode::MirroredInt::NRepSkippingUnsignedSubOp(const MirroredInt& rValue) {
	if (IsPositive()) {
		if (rValue.Value >= Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue.Value - Value;//Skipping negative zero
		}
		else
			Value -= rValue.Value;
	}
	else
		Value += rValue.Value;
}

void BlazesRusCode::MirroredInt::NRepSkippingAddOp(const MirroredInt& rValue) {
	if (IsPositive()) {
		if (rValue.IsPositive())
			Value += rValue.Value;
		else if (rValue.Value > Value) {//Becoming negative
			Sign = NegativeSign;
			Value = rValue.Value - Value;
		}
		else
			Value -= rValue.Value;
	}
	else {
		if (rValue.IsNegative())
			Value += rValue.Value;
		else if (rValue.Value >= Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue.Value - Value;//Skipping negative zero
		}
		else
			Value -= rValue.Value;
	}
}

void BlazesRusCode::MirroredInt::NRepSkippingSubOp(const MirroredInt& rValue) {
	if (IsPositive()) {
		if (rValue.IsNegative())
			Value += rValue.Value;
		else if (rValue.Value >= Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue.Value - Value;//Skipping negative zero
		}
		else
			Value -= rValue.Value;
	}
	else {
		if (rValue.IsPositive())
			Value += rValue.Value;
		else if (rValue.Value > Value) {//Becoming negative
			Sign = NegativeSign;
			Value = rValue.Value - Value;
		}
		else
			Value -= rValue.Value;
	}
}

void BlazesRusCode::MirroredInt::NRepSkippingUIntAddOp(const unsigned int& rValue) {
	if (IsPositive())
		Value += rValue;
	else {
		if (rValue >= Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue - Value;//Skipping negative zero
		}
		else
			Value -= rValue;
	}
}

void BlazesRusCode::MirroredInt::NRepSkippingUIntSubOp(const unsigned int& rValue) {
	if (IsPositive()) {
		if (rValue >= Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rValue - Value;//Skipping negative zero
		}
		else
			Value -= rValue;
	}
	else
		Value += rValue;
}

void BlazesRusCode::MirroredInt::NRepSkippingIntegerAddOp(const signed int& rValue) {
	bool rIsNeg = rValue < 0;
	unsigned int rMag = (unsigned int)(rIsNeg ? -rValue : rValue);
	if (IsPositive()) {
		if (rIsNeg == false)
			Value += rMag;
		else if (rMag > Value) {//Becoming negative
			Sign = NegativeSign;
			Value = rMag - Value;
		}
		else
			Value -= rMag;
	}
	else {
		if (rIsNeg)
			Value += rMag;
		else if (rMag >= Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rMag - Value;//Skipping negative zero
		}
		else
			Value -= rMag;
	}
}

void BlazesRusCode::MirroredInt::NRepSkippingIntegerSubOp(const signed int& rValue) {
	bool rIsNeg = rValue < 0;
	unsigned int rMag = (unsigned int)(rIsNeg ? -rValue : rValue);
	if (IsPositive()) {
		if (rIsNeg)
			Value += rMag;
		else if (rMag >= Value) {//Becoming positive
			Sign = PositiveSign;
			Value = rMag - Value;//Skipping negative zero
		}
		else
			Value -= rMag;
	}
	else {
		if (rIsNeg == false)
			Value += rMag;
		else if (rMag > Value) {//Becoming negative
			Sign = NegativeSign;
			Value = rMag - Value;
		}
		else
			Value -= rMag;
	}
}
