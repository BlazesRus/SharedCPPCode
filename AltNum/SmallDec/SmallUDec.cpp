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
    return SmallUDec(0, 99);
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

void BlazesRusCode::SmallUDec::SetAsValues(const unsigned int& intVal, const unsigned short& decVal)
{
    IntHalf = intVal; DecimalHalf = decVal;
}

bool BlazesRusCode::SmallUDec::IsAtZeroInt() const
{
    return IntHalf == 0;
}

bool BlazesRusCode::SmallUDec::IsNotAtZeroInt() const
{
    return IntHalf != 0;
}

bool BlazesRusCode::SmallUDec::IsAtOneInt() const
{
    return IntHalf == 1;
}

bool BlazesRusCode::SmallUDec::IsNotAtOneInt() const
{
    return IntHalf != 1;
}

bool BlazesRusCode::SmallUDec::IsZero() const
{
    return DecimalHalf == 0 && IntHalf == 0;
}

bool BlazesRusCode::SmallUDec::IsOne() const
{
    return DecimalHalf == 0 && IntHalf == 1;
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
    float Value = (float)IntHalf;
    if (DecimalHalf != 0)
        Value += ((float)DecimalHalf * 0.01f);
    return Value;
}

double BlazesRusCode::SmallUDec::toDouble() const
{
    double Value = (double)IntHalf;
    if (DecimalHalf != 0)
        Value += ((double)DecimalHalf * 0.01);

    return Value;
}

long double BlazesRusCode::SmallUDec::toDecimal() const
{
    long double Value = (long double)IntHalf;
    if (DecimalHalf != 0)
        Value += ((long double)DecimalHalf * 0.0L);

    return Value;
}

#endif

#pragma endregion ConvertToOtherTypes

#pragma region Comparison Operators


#pragma endregion Comparison Operators

#pragma region Other Division Operations

void BlazesRusCode::SmallUDec::DivideByTwo()
{
    if (DecimalHalf == 0 && (IntHalf & 1) == 1)//Check if number is odd
        UIntDivOp(2);
    else
        IntHalf /= 2;
}

void BlazesRusCode::SmallUDec::DivideByFour()
{
    //Checking if divisible by 4 based on
    //https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
    //checking if divible by 8 equals (((n >> 3) << 3) == n)
    if (DecimalHalf == 0 && (((IntHalf >> 2) << 2) == IntHalf))//Check if number can be perfectly divided by 4
        IntHalf /= 4;
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

#pragma endregion NormalRep Integer Addition Operations

#pragma region NormalRep Integer Subtraction Operations

#pragma endregion NormalRep Integer Subtraction Operations

#pragma region Truncation Functions

void BlazesRusCode::SmallUDec::ApplyFloorOf(const int& precision)
{
    switch (precision)
    {
    case 8: DecimalHalf /= 10; DecimalHalf *= 10; break;
    default:
        DecimalHalf = 0;
        break;
    }
}

unsigned int BlazesRusCode::SmallUDec::FloorIntOf() const
{
    if (DecimalHalf == 0)
        return IntHalf;
    else
        return IntHalf - 1;
}

unsigned int BlazesRusCode::SmallUDec::CeilIntOf() const
{
    if (DecimalHalf == 0)
        return IntHalf;
    else
        return IntHalf + 1;
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
            IntHalf += TempInt02;
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
    std::string Value = VariableConversionFunctions::UIntToStringConversion(IntHalf);
    if (DecimalHalf != 0)
    {
        Value += ".";
        Value += std::string(DecimalHalf);
    }
    return Value;
}

std::string BlazesRusCode::SmallUDec::ToFullString()
{
    std::string Value = VariableConversionFunctions::UIntToStringConversion(IntHalf);
    if (DecimalHalf != 0)
    {
        unsigned __int8 CurrentDigit;
        Value += ".";
        if (DecimalHalf > 0)
        {
            unsigned __int8 CurrentSection = DecimalHalf;
            CurrentDigit = CurrentSection / 10;
            CurrentSection -= CurrentDigit * 10;
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
            Value += VariableConversionFunctions::DigitAsChar(CurrentSection);
        }
        else
            Value += "00";

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
        Value += ".00";
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
    if (IsNegative)
        SetAsZero();
    //Cap value if too big on initialize (preventing overflow on conversion)
    else if (Value >= 4294967296.0f)
    {
        IntHalf = 4294967295;
        DecimalHalf = 999999999;
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
    if (IsNegative)
        SetAsZero();
    //Cap value if too big on initialize (preventing overflow on conversion)
    else if (Value >= 4294967296.0)
    {
        IntHalf = 4294967295;
        DecimalHalf = 999999999;
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
    if (IsNegative)
        SetAsZero();
    //Cap value if too big on initialize (preventing overflow on conversion)
    else if (Value >= 4294967296.0L)
    {
        IntHalf = 4294967295;
        DecimalHalf = 999999999;
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

void BlazesRusCode::SmallUDec::SetIntVal(const unsigned int& Value)
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

SmallUDec BlazesRusCode::SmallUDec::NthRootOf(const unsigned int& n, const SmallUDec& precision) const
{
    return NthRoot(*this, n, precision);
}

SmallUDec BlazesRusCode::SmallUDec::AlternativeNthRoot(const SmallUDec& tValue, const unsigned int& n, const SmallUDec& precision)
{
    return NthRootV2<SmallUDec>(tValue, n, precision);
}

#pragma endregion Pow and Sqrt Functions