#include "PartialInt.hpp"
using PartialInt = BlazesRusCode::PartialInt;

PartialInt PartialInt::Zero = PartialInt::ZeroValue();

inline BlazesRusCode::PartialInt::PartialInt(unsigned int value, unsigned int flags)
{
	Value = value;
	Flags = flags;
}

inline void BlazesRusCode::PartialInt::UInt64DivOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result /= rValue;
	Value = (unsigned int)result;
}

inline void BlazesRusCode::PartialInt::Int64DivOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	result /= rValue;//Assuming right side value is non-negative because does not support negative numbers(not worth small cost to check given primary function of class)
	Value = (unsigned int)result;
}

inline void BlazesRusCode::PartialInt::UInt64MultOp(const unsigned __int64& rValue)
{
	unsigned __int64 result = (unsigned __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}

inline void BlazesRusCode::PartialInt::Int64MultOp(const signed __int64& rValue)
{
	signed __int64 result = (signed __int64)Value;
	result *= rValue;
	Value = (unsigned int)result;
}

inline bool BlazesRusCode::PartialInt::IsNormalVariant() const
{
	return Flags == 0;
}

inline bool BlazesRusCode::PartialInt::IsPiVariant() const
{
	return Flags == 1;
}

inline bool BlazesRusCode::PartialInt::IsEVariant() const
{
	return Flags == 2;
}

inline bool BlazesRusCode::PartialInt::IsIVariant() const
{
	return Flags == 3;
}

inline void BlazesRusCode::PartialInt::SwitchToNormal()
{
	Flags = 0;
}

inline void BlazesRusCode::PartialInt::SwitchToPiVariant()
{
	Flags = 1;
}

inline void BlazesRusCode::PartialInt::SwitchToEVariant()
{
	Flags = 2;
}

inline void BlazesRusCode::PartialInt::SwitchToIVariant()
{
	Flags = 3;
}

//Is at zero value

inline bool BlazesRusCode::PartialInt::IsAtZero() const
{
	return Value == 0;
}

//Is not at zero value

inline bool BlazesRusCode::PartialInt::IsNotAtZero() const
{
	return Value != 0;
}

//Is at one value

inline bool BlazesRusCode::PartialInt::IsAtOne() const
{
	return Value == 1;
}

inline bool BlazesRusCode::PartialInt::IsEven() const
{
	return (Value & 1) == 0;
}

inline bool BlazesRusCode::PartialInt::IsOdd() const
{
	return (Value & 1) == 1;
}

/// <summary>
/// Returns the value at zero
/// </summary>
/// <returns>MirroredInt</returns>

inline PartialInt BlazesRusCode::PartialInt::ZeroValue()
{
	return PartialInt();
}

inline BlazesRusCode::PartialInt::operator std::string()
{
    return VariableConversionFunctions::UIntToStringConversion(Value);
}
