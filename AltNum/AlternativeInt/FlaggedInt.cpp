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

/// <summary>
/// Returns the value at one
/// </summary>
/// <returns>FlaggedInt</returns>

inline FlaggedInt BlazesRusCode::FlaggedInt::OneValue()
{
	return FlaggedInt(1, 1);
}

/// <summary>
/// Returns the value at two
/// </summary>
/// <returns>FlaggedInt</returns>

inline FlaggedInt BlazesRusCode::FlaggedInt::TwoValue()
{
	return FlaggedInt(2, 1);
}

/// <summary>
/// Returns the value at zero
/// </summary>
/// <returns>FlaggedInt</returns>

inline FlaggedInt BlazesRusCode::FlaggedInt::ZeroValue()
{
	return FlaggedInt(0, 1);
}

/// <summary>
/// MirroredInt to int explicit conversion
/// </summary>
/// <returns>The result of the operator.</returns>

inline BlazesRusCode::FlaggedInt::operator std::string()
{
	return VariableConversionFunctions::UIntToStringConversion(Value);
}
