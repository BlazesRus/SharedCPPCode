#include "FlaggedInt.hpp"
using FlaggedInt = BlazesRusCode::FlaggedInt;

FlaggedInt FlaggedInt::Maximum = FlaggedInt::MaximumValue();
FlaggedInt FlaggedInt::AltMaximum = FlaggedInt::AltMaximumValue();

FlaggedInt FlaggedInt::One = FlaggedInt::OneValue();
FlaggedInt FlaggedInt::Two = FlaggedInt::TwoValue();

inline BlazesRusCode::FlaggedInt::FlaggedInt(const unsigned int& value, const unsigned int& isAltRep)
{
	Value = value;
	IsAltRep = isAltRep;
}

inline void BlazesRusCode::FlaggedInt::UInt64DivOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result /= rValue;
	Value = (unsigned int)result;
}

inline void BlazesRusCode::FlaggedInt::Int64DivOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	result /= rValue;//Assuming right side value is non-negative because does not support negative numbers(not worth small cost to check given primary function of class)
	Value = (unsigned int)result;
}

inline void BlazesRusCode::FlaggedInt::UInt64MultOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}

inline void BlazesRusCode::FlaggedInt::Int64MultOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}

inline bool BlazesRusCode::FlaggedInt::IsAlternative() const
{
	return IsAltRep == 1;
}

inline bool BlazesRusCode::FlaggedInt::IsNormal()
{
	return IsAltRep == 0;
}

inline void BlazesRusCode::FlaggedInt::SwitchToAlternative()
{
	IsAltRep = 1;
}

inline void BlazesRusCode::FlaggedInt::SwitchToNormal()
{
	IsAltRep = 0;
}

inline bool BlazesRusCode::FlaggedInt::IsAtZero() const
{
	return Value == 0;
}

inline bool BlazesRusCode::FlaggedInt::IsNotAtZero() const
{
	return Value != 0;
}

inline bool BlazesRusCode::FlaggedInt::IsAtOne() const
{
	return Value == 1;
}

inline bool BlazesRusCode::FlaggedInt::IsNotOne() const
{
	return Value != 1;
}

inline bool BlazesRusCode::FlaggedInt::IsEven() const
{
	return (Value & 1) == 0;
}

inline bool BlazesRusCode::FlaggedInt::IsOdd() const
{
	return (Value & 1) == 1;
}

inline FlaggedInt BlazesRusCode::FlaggedInt::MaximumValue()
{
	return FlaggedInt(2147483647, 1);
}

inline FlaggedInt BlazesRusCode::FlaggedInt::AltMaximumValue()
{
	return FlaggedInt(2147483647, 0);
}

inline FlaggedInt BlazesRusCode::FlaggedInt::OneValue()
{
	return FlaggedInt(1, 1);
}

inline FlaggedInt BlazesRusCode::FlaggedInt::TwoValue()
{
	return FlaggedInt(2, 1);
}

inline FlaggedInt BlazesRusCode::FlaggedInt::ZeroValue()
{
	return FlaggedInt(0, 1);
}

inline std::string BlazesRusCode::PartialInt::ToString() const
{
    return VariableConversionFunctions::UIntToStringConversion(Value);
}
