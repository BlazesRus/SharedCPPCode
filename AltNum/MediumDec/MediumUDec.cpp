#include "MediumUDec.hpp"
#include "..\..\OtherFunctions\VariableConversionFunctions.h"
using MediumUDec = BlazesRusCode::MediumUDec;

#pragma region ValueSetters

void BlazesRusCode::MediumUDec::SetValueToPiNum()
{
    IntHalf = 3; DecimalHalf = 141592654;
}

void BlazesRusCode::MediumUDec::SetValueToHundredMilPiNum()
{
    IntHalf = 314159265; DecimalHalf = 358979324;
}

void BlazesRusCode::MediumUDec::SetValueToTenMilPiNum()
{
    IntHalf = 31415926; DecimalHalf = 535897932;
}

void BlazesRusCode::MediumUDec::SetValueToOneMilPiNum()
{
    IntHalf = 3141592; DecimalHalf = 653589793;
}

void BlazesRusCode::MediumUDec::SetValueToTenPiNum()
{
    IntHalf = 31; DecimalHalf = 415926536;
}

void BlazesRusCode::MediumUDec::SetValueToENum()
{
    IntHalf = 2; DecimalHalf = 718281828;
}

void BlazesRusCode::MediumUDec::SetValueToPoint5()
{
    IntHalf = 0; DecimalHalf = 500000000;
}

void BlazesRusCode::MediumUDec::SetValueToJustAboveZero()
{
    IntHalf = 0; DecimalHalf = 1;
}

void BlazesRusCode::MediumUDec::SetValueToOneMillionth()
{
    IntHalf = 0; DecimalHalf = 1000;
}

void BlazesRusCode::MediumUDec::SetValueToFiveThousandth()
{
    IntHalf = 0; DecimalHalf = 5000000;
}

void BlazesRusCode::MediumUDec::SetValueToFiveMillionth()
{
    IntHalf = 0; DecimalHalf = 5000;
}

void BlazesRusCode::MediumUDec::SetValueToTenMillionth()
{
    IntHalf = 0; DecimalHalf = 100;
}

void BlazesRusCode::MediumUDec::SetValueToOneHundredMillionth()
{
    IntHalf = 0; DecimalHalf = 10;
}

void BlazesRusCode::MediumUDec::SetValueToLN10()
{
    IntHalf = 2; DecimalHalf = 302585093;
}

void BlazesRusCode::MediumUDec::SetValueToLN10Div()
{
    IntHalf = 0; DecimalHalf = 434294482;
}

void BlazesRusCode::MediumUDec::SetValueToTwiceLN10Div()
{
    IntHalf = 0; DecimalHalf = 868588964;
}

void BlazesRusCode::MediumUDec::SetValueToPointOne()
{
    IntHalf = 0; DecimalHalf = 100000000;
}

#pragma endregion ValueSetters

#pragma region ValueDefines

MediumUDec BlazesRusCode::MediumUDec::AlmostOneValue()
{
    return MediumUDec(0, 999999999);
}

MediumUDec BlazesRusCode::MediumUDec::PiNumValue()
{
    return MediumUDec(3, 141592654);
}

MediumUDec BlazesRusCode::MediumUDec::HundredMilPiNumValue()
{
    return MediumUDec(314159265, 358979324);
}

MediumUDec BlazesRusCode::MediumUDec::TenMilPiNumValue()
{
    return MediumUDec(31415926, 535897932);
}

MediumUDec BlazesRusCode::MediumUDec::OneMilPiNumValue()
{
    return MediumUDec(3141592, 653589793);
}

MediumUDec BlazesRusCode::MediumUDec::TenPiNumValue()
{
    return MediumUDec(31, 415926536);
}

MediumUDec BlazesRusCode::MediumUDec::ENumValue()
{
    return MediumUDec(2, 718281828);
}

MediumUDec BlazesRusCode::MediumUDec::ZeroValue()
{
    return MediumUDec();
}

MediumUDec BlazesRusCode::MediumUDec::OneValue()
{
    return MediumUDec(1);
}

MediumUDec BlazesRusCode::MediumUDec::TwoValue()
{
    return MediumUDec(Two);
}

MediumUDec BlazesRusCode::MediumUDec::Point5Value()
{
    return MediumUDec(0, 500000000);
}

MediumUDec BlazesRusCode::MediumUDec::JustAboveZeroValue()
{
    return MediumUDec(0, 1);
}

MediumUDec BlazesRusCode::MediumUDec::OneMillionthValue()
{
    return MediumUDec(0, 1000);
}

MediumUDec BlazesRusCode::MediumUDec::FiveThousandthValue()
{
    return MediumUDec(0, 5000000);
}

MediumUDec BlazesRusCode::MediumUDec::FiveMillionthValue()
{
    return MediumUDec(0, 5000);
}

MediumUDec BlazesRusCode::MediumUDec::TenMillionthValue()
{
    return MediumUDec(0, 100);
}

MediumUDec BlazesRusCode::MediumUDec::OneHundredMillionthValue()
{
    return MediumUDec(0, 10);
}

MediumUDec BlazesRusCode::MediumUDec::FiveBillionthValue()
{
    return MediumUDec(0, 5);
}

MediumUDec BlazesRusCode::MediumUDec::LN10Value()
{
    return MediumUDec(2, 302585093);
}

MediumUDec BlazesRusCode::MediumUDec::LN10DivValue()
{
    return MediumUDec(0, 434294482);
}

MediumUDec BlazesRusCode::MediumUDec::TwiceLN10DivValue()
{
    return MediumUDec(0, 868588964);
}

MediumUDec BlazesRusCode::MediumUDec::MinimumValue()
{
    return MediumUDec(0, 0);
}

MediumUDec BlazesRusCode::MediumUDec::MaximumValue()
{
    return MediumUDec(4294967295, 999999999);
}

MediumUDec BlazesRusCode::MediumUDec::PointOneValue()
{
    return MediumUDec(0, 100000000);
}

#pragma endregion ValueDefines

#pragma region ValueDefine Source

const MediumUDec MediumUDec::AlmostOne = MediumUDec::AlmostOneValue();
const MediumUDec MediumUDec::Pi = MediumUDec::PiNumValue();
const MediumUDec MediumUDec::One = MediumUDec::OneValue();
const MediumUDec MediumUDec::Two = MediumUDec::TwoValue();
const MediumUDec MediumUDec::Zero = MediumUDec::ZeroValue();
const MediumUDec MediumUDec::PointFive = MediumUDec::Point5Value();
const MediumUDec MediumUDec::JustAboveZero = MediumUDec::JustAboveZeroValue();
const MediumUDec MediumUDec::OneMillionth = MediumUDec::OneMillionthValue();
const MediumUDec MediumUDec::FiveThousandth = MediumUDec::FiveThousandthValue();
const MediumUDec MediumUDec::Minimum = MediumUDec::MinimumValue();
const MediumUDec MediumUDec::Maximum = MediumUDec::MaximumValue();
const MediumUDec MediumUDec::E = MediumUDec::ENumValue();
const MediumUDec MediumUDec::LN10 = MediumUDec::LN10Value();
const MediumUDec MediumUDec::LN10Div = MediumUDec::LN10DivValue();
const MediumUDec MediumUDec::TwiceLN10Div = MediumUDec::TwiceLN10DivValue();
const MediumUDec MediumUDec::TenMillionth = MediumUDec::TenMillionthValue();
const MediumUDec MediumUDec::FiveMillionth = MediumUDec::FiveMillionthValue();
const MediumUDec MediumUDec::FiveBillionth = MediumUDec::FiveBillionthValue();
const MediumUDec MediumUDec::OneGMillionth = MediumUDec::OneHundredMillionthValue();

const MediumUDec MediumUDec::PiNum = MediumUDec::PiNumValue();
const MediumUDec MediumUDec::ENum = MediumUDec::ENumValue();
const MediumUDec MediumUDec::PointOne = MediumUDec::PointOneValue();
#pragma endregion ValueDefine Source

#pragma region Check_if_value

void BlazesRusCode::MediumUDec::SetAsZero()
{
    IntHalf = 0; DecimalHalf = 0;
}

void BlazesRusCode::MediumUDec::SetAsOne()
{
    IntHalf = 1; DecimalHalf = 0;
}

void BlazesRusCode::MediumUDec::SetAsValues(const unsigned int& intVal, const PartialInt& decVal)
{
    IntHalf = intVal; DecimalHalf = decVal;
}

bool BlazesRusCode::MediumUDec::IsAtZeroInt() const
{
    return IntHalf == 0;
}

bool BlazesRusCode::MediumUDec::IsNotAtZeroInt() const
{
    return IntHalf != 0;
}

bool BlazesRusCode::MediumUDec::IsAtOneInt() const
{
    return IntHalf == 1;
}

bool BlazesRusCode::MediumUDec::IsNotAtOneInt() const
{
    return IntHalf != 1;
}

bool BlazesRusCode::MediumUDec::IsZero() const
{
    return DecimalHalf == 0 && IntHalf == 0;
}

bool BlazesRusCode::MediumUDec::IsOne() const
{
    return DecimalHalf == 0 && IntHalf == 1;
}

#pragma endregion Check_if_value

#pragma region RangeLimits

void BlazesRusCode::MediumUDec::SetAsMaximum()
{
    IntHalf = 4294967295; DecimalHalf = 999999999;
}

void BlazesRusCode::MediumUDec::SetAsMinimum()
{
    IntHalf = 0; DecimalHalf = 0;
}

#pragma endregion RangeLimits

#pragma region ConvertToOtherTypes

#if defined(AltNum_EnableFloatingConversion)

float BlazesRusCode::MediumUDec::toFloat() const
{
    #if defined(AltNum_UseLegacyFloatingConversion)
    float Value = (float)IntHalf;
    if (DecimalHalf != 0)
        Value += ((float)DecimalHalf * 0.000000001f);
    return Value;
    #else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
    if (IntHalf == 0)//Exponent is negative
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

double BlazesRusCode::MediumUDec::toDouble() const
{
    #if defined(AltNum_UseLegacyFloatingConversion)
    double Value = (double)IntHalf;
    if (DecimalHalf != 0)
        Value += ((double)DecimalHalf * 0.000000001);
    return Value;
    #else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
    if (IntHalf == 0)//Exponent is negative
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

long double BlazesRusCode::MediumUDec::toDecimal() const
{
    #if defined(AltNum_UseLegacyFloatingConversion)
    long double Value = (long double)IntHalf;
    if (DecimalHalf != 0)
        Value += ((long double)DecimalHalf * 0.000000001L);
    return Value;
    #else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
    if (IntHalf == 0)//Exponent is negative
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

void BlazesRusCode::MediumUDec::DivideByTwo()
{
    if (DecimalHalf == 0 && (IntHalf & 1) == 1)//Check if number is odd
        UIntDivOp(2);
    else
        IntHalf /= 2;
}

void BlazesRusCode::MediumUDec::DivideByFour()
{
    //Checking if divisible by 4 based on
    //https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
    //checking if divible by 8 equals (((n >> 3) << 3) == n)
    if (DecimalHalf == 0 && (((IntHalf >> 2) << 2) == IntHalf))//Check if number can be perfectly divided by 4
        IntHalf /= 4;
    else
        UIntDivOp(4);
}

MediumUDec BlazesRusCode::MediumUDec::DividedByTwo() const
{
    MediumUDec result = *this; result.DivideByTwo();
    return result;
}

MediumUDec BlazesRusCode::MediumUDec::DividedByFour() const
{
    MediumUDec result = *this; result.DivideByFour();
    return result;
}

#pragma endregion Other Division Operations

#pragma region Other multiplication operations

void BlazesRusCode::MediumUDec::MultiplyByTwo()
{
    UIntMultOp(2);
}

void BlazesRusCode::MediumUDec::MultiplyByFour()
{
    UIntMultOp(4);
}


MediumUDec BlazesRusCode::MediumUDec::MultipliedByTwo() const
{
    MediumUDec result = *this; result.UIntMultOp(2);
    return result;
}

MediumUDec BlazesRusCode::MediumUDec::MultipliedByFour() const
{
    MediumUDec result = *this; result.UIntMultOp(4);
    return result;
}


#pragma endregion Other multiplication operations

#pragma region NormalRep Integer Addition Operations

#pragma endregion NormalRep Integer Addition Operations

#pragma region NormalRep Integer Subtraction Operations

#pragma endregion NormalRep Integer Subtraction Operations

#pragma region Truncation Functions

void BlazesRusCode::MediumUDec::ApplyFloorOf(const int& precision)
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
}

unsigned int BlazesRusCode::MediumUDec::FloorIntOf() const
{
    if (DecimalHalf == 0)
        return IntHalf;
    else
        return IntHalf - 1;
}

unsigned int BlazesRusCode::MediumUDec::CeilIntOf() const
{
    if (DecimalHalf == 0)
        return IntHalf;
    else
        return IntHalf + 1;
}

MediumUDec BlazesRusCode::MediumUDec::Trunc(const MediumUDec& tValue) { return tValue.TruncOfV1<MediumUDec>(); }

#pragma endregion Truncation Functions

#pragma region String Commands

void BlazesRusCode::MediumUDec::ReadString(const std::string& Value)
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

BlazesRusCode::MediumUDec::MediumUDec(const char* strVal)
{
    std::string Value = strVal;
    this->ReadString(Value);
}

BlazesRusCode::MediumUDec::MediumUDec(const std::string& Value)
{
    this->ReadString(Value);
}

std::string BlazesRusCode::MediumUDec::ToString()
{
    std::string Value = VariableConversionFunctions::UIntToStringConversion(IntHalf);
    if (DecimalHalf != 0)
    {
        Value += ".";
        Value += std::string(DecimalHalf);
    }
    return Value;
}

std::string BlazesRusCode::MediumUDec::ToFullString()
{
    std::string Value = VariableConversionFunctions::UIntToStringConversion(IntHalf);
    if (DecimalHalf != 0)
    {
        unsigned __int8 CurrentDigit;
        Value += ".";
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
void BlazesRusCode::MediumUDec::SetFloatVal(const float& Value)
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

void BlazesRusCode::MediumUDec::SetDoubleVal(const double& Value)
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

void BlazesRusCode::MediumUDec::SetDecimalVal(const long double& Value)
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

void BlazesRusCode::MediumUDec::SetBoolVal(const bool& Value)
{
    IntHalf = Value == false ? 0 : 1;
    DecimalHalf = 0;
}

void BlazesRusCode::MediumUDec::SetIntVal(const unsigned int& Value)
{
    IntHalf = Value;
    DecimalHalf = 0;
}

#pragma endregion ConvertFromOtherTypes

#pragma region Pow and Sqrt Functions

MediumUDec BlazesRusCode::MediumUDec::Sqrt(const auto& value, const int& precision)
{
    return SqrtV1<MediumUDec>(value, precision);
}

MediumUDec BlazesRusCode::MediumUDec::SqrtOf(const int& precision) const {
    return Sqrt(*this, precision);
}

MediumUDec BlazesRusCode::MediumUDec::NthRootOf(const unsigned int& n, const MediumUDec& precision) const
{
    return NthRoot(*this, n, precision);
}

MediumUDec BlazesRusCode::MediumUDec::AlternativeNthRoot(const MediumUDec& tValue, const unsigned int& n, const MediumUDec& precision)
{
    return NthRootV2<MediumUDec>(tValue, n, precision);
}

#pragma endregion Pow and Sqrt Functions