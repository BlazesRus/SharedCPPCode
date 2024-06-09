#include "MirroredInt.hpp"
using MirroredInt = BlazesRusCode::MirroredInt;

MirroredInt MirroredInt::NegativeZero = MirroredInt::NegativeZeroValue();
MirroredInt MirroredInt::Zero = MirroredInt::ZeroValue();
MirroredInt MirroredInt::Maximum = MirroredInt::MaximumValue();
MirroredInt MirroredInt::Minimum = MirroredInt::MinimumValue();

MirroredInt MirroredInt::NegativeOne = MirroredInt::NegativeOneValue();
MirroredInt MirroredInt::One = MirroredInt::OneValue();
MirroredInt MirroredInt::Two = MirroredInt::TwoValue();

inline BlazesRusCode::MirroredInt::MirroredInt(const unsigned int& value, const unsigned int& sign)
{
	Value = value;
	Sign = sign;
}

inline BlazesRusCode::MirroredInt::MirroredInt(const signed int& value)
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

inline bool BlazesRusCode::MirroredInt::IsNegative() const
{
	return Sign == NegativeSign;
}

inline bool BlazesRusCode::MirroredInt::IsPositive() const
{
	return Sign == PositiveSign;
}

inline void BlazesRusCode::MirroredInt::SetAsPositive()
{
	Sign = PositiveSign;
}

inline void BlazesRusCode::MirroredInt::SetAsNegative()
{
	Sign = NegativeSign;
}

inline void BlazesRusCode::MirroredInt::SetSignedValue(const signed int& val)
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

inline void BlazesRusCode::MirroredInt::SetValue(const unsigned int& value, const unsigned int& sign)
{
	Value = value;
	Sign = sign;
}

//Set as zero without changing sign(including negative zero)

inline void BlazesRusCode::MirroredInt::SetAsZeroVal()
{
	Value = 0;
}

//Return value as real number(negative zero counts as zero)

inline int BlazesRusCode::MirroredInt::GetValue() const
{
	if (IsPositive())
		return (signed int)Value;
	else
		return -((signed int)Value);
}

inline void BlazesRusCode::MirroredInt::SetAsZero()
{
	Value = 0; Sign = PositiveSign;
}

inline void BlazesRusCode::MirroredInt::SetAsNegativeZero()
{
	Value = 0; Sign = NegativeSign;
}

//Is at either zero or negative zero

inline bool BlazesRusCode::MirroredInt::IsAtZeroInt() const
{
	return Value == 0;
}

//Is at neither zero or negative zero

inline bool BlazesRusCode::MirroredInt::IsNotAtZeroInt() const
{
	return Value != 0;
}

//Is at either zero or negative one

inline bool BlazesRusCode::MirroredInt::IsAtOneInt() const
{
	return Value == 1;
}

//Is at neither zero or negative one

inline bool BlazesRusCode::MirroredInt::IsNotAtOneInt() const
{
	return Value != 1;
}

inline bool BlazesRusCode::MirroredInt::IsEven() const
{
	return (Value & 1) == 0;
}

inline bool BlazesRusCode::MirroredInt::IsOdd() const
{
	return (Value & 1) == 1;
}

inline bool BlazesRusCode::MirroredInt::IsZero() const
{
	return Value == 0 && Sign == PositiveSign;
}

//Returns copy of value as Absolute value

inline MirroredInt BlazesRusCode::MirroredInt::Abs() const
{
	if (IsPositive())
		return *this;
	else
		return MirroredInt(Value);
}

inline void BlazesRusCode::MirroredInt::ApplyAbs()
{
	if (IsNegative())
		Sign = PositiveSign;
}

/// <summary>
/// Swaps the negative status.
/// </summary>

inline void BlazesRusCode::MirroredInt::SwapNegativeStatus()
{
	Sign ^= 1;
}

/// <summary>
/// Returns maximum stored value(2147483647)
/// </summary>
/// <returns>MirroredInt</returns>

inline MirroredInt BlazesRusCode::MirroredInt::MaximumValue()
{
	return MirroredInt(2147483647, PositiveSign);
}

/// <summary>
/// Returns minimum stored value(-2147483647)
/// </summary>
/// <returns>MirroredInt</returns>

inline MirroredInt BlazesRusCode::MirroredInt::MinimumValue()
{
	return MirroredInt(2147483647, NegativeSign);
}

/// <summary>
/// Returns the value at negative one
/// </summary>
/// <returns>MirroredInt</returns>

inline MirroredInt BlazesRusCode::MirroredInt::NegativeOneValue()
{
	return MirroredInt(1, NegativeSign);
}

/// <summary>
/// Returns the value at one
/// </summary>
/// <returns>MirroredInt</returns>

inline MirroredInt BlazesRusCode::MirroredInt::OneValue()
{
	return MirroredInt(1);
}

/// <summary>
/// Returns the value at two
/// </summary>
/// <returns>MirroredInt</returns>

inline MirroredInt BlazesRusCode::MirroredInt::TwoValue()
{
	return MirroredInt(2);
}

/// <summary>
/// Returns the value at negative zero(for negative fractions)
/// </summary>
/// <returns>MirroredInt</returns>

inline MirroredInt BlazesRusCode::MirroredInt::NegativeZeroValue()
{
	return MirroredInt(0, NegativeSign);
}

/// <summary>
/// Returns the value at zero
/// </summary>
/// <returns>MirroredInt</returns>

inline MirroredInt BlazesRusCode::MirroredInt::ZeroValue()
{
	return MirroredInt();
}

//Division operation

inline void BlazesRusCode::MirroredInt::UInt64DivOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result /= rValue;
	Value = (unsigned int)result;
}

inline void BlazesRusCode::MirroredInt::Int64DivOp(const signed __int64& rValue)
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

inline void BlazesRusCode::MirroredInt::UInt64MultOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}

inline void BlazesRusCode::MirroredInt::Int64MultOp(const signed __int64& rValue)
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

inline void BlazesRusCode::MirroredInt::DivOp(const MirroredInt& rValue) {
	if (rValue.IsNegative()) {
		SwapNegativeStatus();
		Value /= rValue.Value;
	}
	else
		Value /= rValue.Value;
}

inline void BlazesRusCode::MirroredInt::UIntDivOp(const unsigned int& rValue)
{
	Value /= rValue;
}

inline void BlazesRusCode::MirroredInt::IntDivOp(const signed int& rValue)
{
	if (rValue<0) {
		SwapNegativeStatus();
		Value /= (unsigned int)-rValue;
	}
	else
		Value /= rValue;
}

inline void BlazesRusCode::MirroredInt::MultOp(const MirroredInt& rValue) {
	if (rValue.IsNegative()) {
		SwapNegativeStatus();
		Value *= rValue.Value;
	}
	else
		Value *= rValue.Value;
}

inline void BlazesRusCode::MirroredInt::UIntMultOp(const unsigned int& rValue)
{
	Value *= rValue;
}

inline void BlazesRusCode::MirroredInt::IntMultOp(const signed int& rValue)
{
	if (rValue<0) {
		SwapNegativeStatus();
		Value *= (unsigned int)-rValue;
	}
	else
		Value *= rValue;
}

inline void BlazesRusCode::MirroredInt::UnsignedAddOp(const MirroredInt& rValue) {
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

inline void BlazesRusCode::MirroredInt::AddOp(const MirroredInt& rValue) {
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

inline void BlazesRusCode::MirroredInt::UIntAddOp(const unsigned int& rValue) {
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

inline void BlazesRusCode::MirroredInt::IntAddOp(const signed int& rValue) {
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

inline void BlazesRusCode::MirroredInt::UnsignedSubOp(const MirroredInt& rValue) {
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

inline void BlazesRusCode::MirroredInt::SubOp(const MirroredInt& rValue) {
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

inline void BlazesRusCode::MirroredInt::UIntSubOp(const unsigned int& rValue) {
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

inline void BlazesRusCode::MirroredInt::IntSubOp(const signed int& rValue) {
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

inline void BlazesRusCode::MirroredInt::NRepSkippingUnsignedAddOp(const MirroredInt& rValue) {
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

inline void BlazesRusCode::MirroredInt::NRepSkippingUnsignedSubOp(const MirroredInt& rValue) {
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

inline void BlazesRusCode::MirroredInt::NRepSkippingAddOp(const MirroredInt& rValue) {
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

inline void BlazesRusCode::MirroredInt::NRepSkippingSubOp(const MirroredInt& rValue) {
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

inline void BlazesRusCode::MirroredInt::NRepSkippingUIntAddOp(const unsigned int& rValue) {
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

inline void BlazesRusCode::MirroredInt::NRepSkippingUIntSubOp(const unsigned int& rValue) {
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

inline void BlazesRusCode::MirroredInt::NRepSkippingIntegerAddOp(const signed int& rValue) {
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

inline void BlazesRusCode::MirroredInt::NRepSkippingIntegerSubOp(const signed int& rValue) {
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

inline BlazesRusCode::MirroredInt::operator std::string()
{
	if (IsNegative())
		return "-" + VariableConversionFunctions::UIntToStringConversion(Value);
	else
		return VariableConversionFunctions::UIntToStringConversion(Value);
}
