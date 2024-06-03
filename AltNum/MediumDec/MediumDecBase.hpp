// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecPreprocessors.h"
//#include "..\VirtualTableBase.hpp"//Virtual Structure for the class to make sure can override virtually

#include <string>
#include <cmath>

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "..\Concepts\MediumDecVariantConcept.hpp"


#include "..\AlternativeInt\MirroredInt.hpp"
#include "..\AlternativeInt\PartialInt.hpp"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumDecBase;

    //Reduced version of MediumDec result for modulus result and other stuff
    class DLL_API MediumDecBase : public AltNumBase//AltNumBase is used as base class to identify as MediumDec variant
    {
    public:
    #pragma region DigitStorage

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        MirroredInt IntHalf;

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        PartialInt DecimalHalf;

		//Return IntHalf as signed int
        signed int GetIntHalf()
        {
			return (signed int)IntHalf;
        }

    #pragma endregion DigitStorage

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned _int64 const DecimalOverflowX = 1000000000;

        /// <summary>
        /// Value when IntHalf is at -0.XXXXXXXXXX (when has decimal part)(with Negative Zero the Decimal Half is Zero)
        /// </summary>
        static MirroredInt const NegativeRep;

    #pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecBase(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecBase"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecBase(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
        }

        MediumDecBase& operator=(const MediumDecBase& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumDecBase& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<MediumDecVariant VariantType>
        MediumDecBase operator[](VariantType variantValue) const
        {
            MediumDecBase newSelf = MediumDecBase(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

    #pragma endregion class_constructors

    #pragma region Negative_Status

        bool IsPositive() const
        { return IntHalf.IsPositive(); }

        bool IsNegative() const
        { return IntHalf.IsNegative(); }

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        { IntHalf.Sign ^= 1; }

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator-() const
        { MediumDecBase self = *this; self.SwapNegativeStatus(); return self; }

    #pragma endregion Negative_Status


    #pragma region Check_if_value

		//Set value as exactly zero
        void SetAsZero()
        { IntHalf = 0; DecimalHalf = 0; }

		//Set value as exactly one
        void SetAsOne()
        { IntHalf = 1; DecimalHalf = 0; }
		
		//Set as +-1 while keeping current sign
        void SetAsOneVal()
        { IntHalf.Value = 1; DecimalHalf = 0; }

        void SetAsValues(const MirroredInt& intVal = MirroredInt::Zero, const PartialInt& decVal = PartialInt::Zero)
        { IntHalf = 0; DecimalHalf = 0; }

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt() const
        { return IntHalf.Value==0; }

        bool IsNotAtZeroInt() const
        { return IntHalf.Value!=0; }

        bool IsAtOneInt() const
        { return IntHalf.Value==1; }

        bool IsNotAtOneInt() const
        { return IntHalf.Value!=1; }

        //Detect if at exactly zero(only overridden with MixedDec)
		bool IsZero() const
		{ return DecimalHalf==0&&IntHalf.Value==0; }
		
		bool IsOne() const
        { return DecimalHalf==0&&IntHalf==MirroredInt::One; }
		
		bool IsNegOne() const
        { return DecimalHalf==0&&IntHalf==MirroredInt::NegativeOne; }
		
		bool IsOneVal() const
		{ return DecimalHalf==0&&IntHalf.Value==1; }

		bool IsOneVariantVal() const
		{ return DecimalHalf.Value==0&&IntHalf.Value==1; }

    #pragma endregion Check_if_value

    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        { IntHalf = MirroredInt::Maximum; DecimalHalf = 999999999; }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMinimum()
        { IntHalf = MirroredInt::Minimum; DecimalHalf = 999999999; }
	
    #pragma endregion RangeLimits

    #pragma region ValueSetters

        /// <summary>
        /// Sets value to Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        void  SetValueToPiNum()
        {
            IntHalf = 3; DecimalHalf = 141592654;
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToHundredMilPiNum()
        {
            IntHalf = 314159265; DecimalHalf = 358979324;
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToTenMilPiNum()
        {
            IntHalf = 31415926; DecimalHalf = 535897932;
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        void  SetValueToOneMilPiNum()
        {
            IntHalf = 3141592; DecimalHalf = 653589793;
        }

        //10xPi(Rounded to 9th decimal digit)
        void  SetValueToTenPiNum()
        {
            IntHalf = 31; DecimalHalf = 415926536;
        }

        /// <summary>
        /// Euler's number rounded to 9th digit(2.718281828)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        void  SetValueToENum()
        {
            IntHalf = 2; DecimalHalf = 718281828;
        }
        
        //Sets value to value at 0.5
        void  SetValueToPoint5()
        {
            IntHalf = 0; DecimalHalf = 500000000;
        }

        void  SetValueToJustAboveZero()
        {
            IntHalf = 0; DecimalHalf = 1;
        }

        /// <summary>
        /// Sets the value at .000001000
        /// </summary>
        void  SetValueToOneMillionth()
        {
            IntHalf = 0; DecimalHalf = 1000;
        }

        /// <summary>
        /// Sets the value at "0.005"
        /// </summary>
        /// <returns>MediumDec</returns>
        void  SetValueToFiveThousandth()
        {
            IntHalf = 0; DecimalHalf = 5000000;
        }

        /// <summary>
        /// Sets the value at "0.000005"
        /// </summary>
        void  SetValueToFiveMillionth()
        {
            IntHalf = 0; DecimalHalf = 5000;
        }

        //0e-7
        void  SetValueToTenMillionth()
        {
            IntHalf = 0; DecimalHalf = 100;
        }

        /// <summary>
        /// Sets the value to .000000010
        /// </summary>
        void  SetValueToOneHundredMillionth()
        {
            IntHalf = 0; DecimalHalf = 10;
        }

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        void  SetValueToLN10()
        {
            IntHalf = 2; DecimalHalf = 302585093;
        }

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        void  SetValueToTenthLN10()
        {
            IntHalf = 0; DecimalHalf = 434294482;
        }

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        void  SetValueToFifthLN10()
        {
            IntHalf = 0; DecimalHalf = 868588964;
        }

    #pragma endregion ValueSetters

    #pragma region ValueDefines
    private://Each class needs to define it's own
        
        static MediumDecBase AlmostOneValue()
        { return MediumDecBase(0, 999999999); }

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase PiNumValue()
        { return MediumDecBase(3, 141592654); }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecBase HundredMilPiNumVal()
        { return MediumDecBase(314159265, 358979324); }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecBase TenMilPiNumVal()
        { return MediumDecBase(31415926, 535897932); }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecBase OneMilPiNumVal()
        { return MediumDecBase(3141592, 653589793);}

        //10xPi(Rounded to 9th decimal digit)
        static MediumDecBase TenPiNumVal()
        { return MediumDecBase(31, 415926536); }
        
        static MediumDecBase ENumValue()
        { return MediumDecBase(2, 718281828); }
        
        static MediumDecBase ZeroValue()
        { return MediumDecBase(); }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase OneValue()
        { return MediumDecBase(1); }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase TwoValue()
        { return MediumDecBase(MirroredInt::Two);}

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase NegativeOneValue()
        { return MediumDecBase(MirroredInt::NegativeOne);}

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Point5Value()
        { return MediumDecBase(0, 500000000); }

        static MediumDecBase JustAboveZeroValue()
        { return MediumDecBase(0, 1); }

        static MediumDecBase OneMillionthValue()
        { return MediumDecBase(0, 1000); }

        static MediumDecBase FiveThousandthValue()
        { return MediumDecBase(0, 5000000); }

        static MediumDecBase FiveMillionthValue()
        { return MediumDecBase(0, 5000);}

        static MediumDecBase TenMillionthValue()
        { return MediumDecBase(0, 100); }

        static MediumDecBase OneHundredMillionthValue()
        { return MediumDecBase(0, 10); }

        static MediumDecBase FiveBillionthValue()
        { return MediumDecBase(0, 5); }

        static MediumDecBase LN10Value()
        { return MediumDecBase(2, 302585093); }

        static MediumDecBase TenthLN10Value()
        { return MediumDecBase(0, 434294482); }

        static MediumDecBase FifthLN10Value()
        { return MediumDecBase(0, 868588964); }

        static MediumDecBase MinimumValue()
        { return MediumDecBase(MirroredInt::Maximum, 999999999); }

        static MediumDecBase MaximumValue()
        { return MediumDecBase(MirroredInt::Minimum, 999999999); }

        static MediumDecBase AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase ENum;
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase OneGMillionth;

        //0e-7
        static MediumDecBase TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDecBase FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecBase</returns>
        static MediumDecBase NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDecBase Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDecBase Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDecBase LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecBase TenthLN10;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecBase FifthLN10;

public:
    #pragma endregion ValueDefines

    #pragma region String Commands

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(const std::string& Value)
        {
            IntHalf = 0; DecimalHalf = 0;
            int PlaceNumber;
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
            PlaceNumber = WholeNumberBuffer.length() - 1;
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
                //Limit stored decimal numbers to the amount it can store
                if (PlaceNumber > -1)
                {
                    TempInt = VariableConversionFunctions::CharAsInt(StringChar);
                    TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
                    if (StringChar != '0')
                    {
                        DecimalHalf += TempInt02;
                    }
                    PlaceNumber--;
                }
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecBase(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const std::string& Value)
        {
            this->ReadString(Value);
        }

public:

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToString()
        {
            std::string Value = std::string(IntHalf);
            if (DecimalHalf != 0)
            {
                Value += ".";
                Value += std::string(DecimalHalf);
            }
            return Value;
        }

        /// <summary>
        /// Converts to string with digits filled in even when empty
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToFullString()
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

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToString(); }

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetFloatVal(const float& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			float lValue = Value;
            bool IsNegative = Value < 0.0f;
            if (IsNegative) { lValue *= -1.0f; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0f)
            {
                if (IsNegative)
					IntHalf = MirroredInt(2147483647,0);
                else
                    IntHalf = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (float)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                IntHalf = MirroredInt((unsigned int)WholeValue,IsNegative?0:1);
            }
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDoubleVal(const double& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			double lValue = Value;
            bool IsNegative = Value < 0.0;
            if (IsNegative) { lValue *= -1.0; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0)
            {
                if (IsNegative)
					IntHalf = MirroredInt(2147483647,0);
                else
                    IntHalf = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (double)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                IntHalf = MirroredInt((unsigned int)WholeValue,IsNegative?0:1);
            }
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDecimalVal(const long double& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			long double lValue = Value;
            bool IsNegative = Value < 0.0L;
            if (IsNegative) { lValue *= -1.0L; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (lValue >= 2147483648.0L)
            {
                if (IsNegative)
					IntHalf = MirroredInt(2147483647,0);
                else
                    IntHalf = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(lValue);
                lValue -= (long double)WholeValue;
                DecimalHalf = (signed int)lValue * 10000000000;
                IntHalf = MirroredInt((unsigned int)WholeValue,IsNegative?0:1);
            }
	#else//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetBoolVal(const bool& Value)
        {
            IntHalf = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(const int& Value)
        {
			if(Value<0)
			{
				IntHalf.Sign = MirroredInt::NegativeSign;
				IntHalf.Value = -Value;
			}
			else
				IntHalf = Value;
			DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetUIntVal(const unsigned int& Value)
        {
			IntHalf = Value;
			DecimalHalf = 0;
        }

/*
        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const float& Value){ this->SetFloatVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const double& Value){ this->SetDoubleVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const long double& Value){ this->SetDecimalVal(Value); }
*/

        MediumDecBase(const unsigned __int64& Value){ this->SetUIntVal(Value); }
        MediumDecBase(const signed __int64& Value){ this->SetIntVal(Value); }
        MediumDecBase(const unsigned char& Value){ this->SetUIntVal(Value); }
        MediumDecBase(const signed char& Value){ this->SetIntVal(Value); }
        MediumDecBase(const unsigned short& Value){ this->SetUIntVal(Value); }
        MediumDecBase(const signed short& Value){ this->SetIntVal(Value); }
        MediumDecBase(const unsigned int& Value){ this->SetUIntVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const bool& Value){ this->SetBoolVal(Value); }

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

        //To-Do: Add more exact conversion from floating point format to MediumDec variant

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        float toFloat()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            float Value;
            if (IntHalf.IsNegative())
            {
                Value = (float)-IntHalf.Value;
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
                Value = (float)IntHalf.Value;
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntHalf.Value==0)//Exponent is negative
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

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        double toDouble()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
		    double Value;
            if (IntHalf < 0)
            {
                Value = (double)-IntHalf.Value;
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
                Value = (double)IntHalf.Value;
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntHalf.Value==0)//Exponent is negative
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

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        long double toDecimal()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            long double Value;
            if (IntHalf < 0)
            {
                Value = (long double)-IntHalf.Value;
                if (DecimalHalf != 0) { Value -= ((long double)DecimalHalf * 0.000000001L); }
            }
            else
            {
                Value = (long double)IntHalf.Value;
                if (DecimalHalf != 0) { Value += ((long double)DecimalHalf * 0.000000001L); }
            }
            return Value;
	#else//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntHalf.Value==0)//Exponent is negative
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

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toInt() { return (signed int) IntHalf; }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toUInt() { return (unsigned int) IntHalf; }

        bool toBool() { return IntHalf.IsZero() ? false : true; }

/*
        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }
		
        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDouble(); }
		
        /// <summary>
        /// MediumDec Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator long double() { return toDecimal(); }
*/

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to uint explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed __int64() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to ubyte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned char() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to byte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed char() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to ushort explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned short() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to short explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed short() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes

    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
		template<MediumDecVariant VariantType=MediumDecBase>
		std::strong_ordering BasicComparisonV1(const VariantType& that) const
		{
			if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			unsigned int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (MediumDecBase DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
		//Compare only as if in NormalType representation mode
		std::strong_ordering BasicIntComparison(const int& that) const
		{
			if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = DecimalHalf.Value>0?1:0;
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}

public:

		std::strong_ordering operator<=>(const MediumDecBase& that) const
		{//return BasicComparison(that);
			if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntHalf but with negative DecimalHalf
			unsigned int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			unsigned int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}

		std::strong_ordering operator<=>(const int& that) const
		{
			return BasicIntComparison(that);
		}

		bool operator==(const MediumDecBase& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.DecimalHalf)
				return false;
            return true;
		}

		bool operator!=(const MediumDecBase& that) const
		{
			if (IntHalf!=that.IntHalf)
				return true;
			if (DecimalHalf!=that.DecimalHalf)
				return true;
            return false;
		}

		bool operator==(const int& that) const
		{
			if (IntHalf!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			return true;
		}

		bool operator!=(const int& that) const
		{
			if (IntHalf!=that)
				return true;
			if (DecimalHalf!=0)
				return true;
			return false;
		}

    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:

        template<IntegerType IntType=unsigned int>
        void PartialUIntDivOpV1(const IntType& rValue)
        {//Avoid using with special status representations such as approaching zero or result will be incorrect
            unsigned _int64 SelfRes;
            unsigned _int64 Res;
            unsigned _int64 IntHalfRes;
            unsigned _int64 DecimalRes;
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntHalf.Value: DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
            Res = SelfRes / rValue;
			
            IntHalfRes = Res/DecimalOverflowX;
            DecimalRes = Res - DecimalOverflowX * IntHalfRes;
		    IntHalf.Value = (unsigned int)IntHalfRes;
			DecimalHalf.Value = (unsigned int)DecimalRes;
        }

        template<IntegerType IntType=signed int>
        void PartialIntDivOpV1(const IntType& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                PartialUIntDivOp(-Value);
            }
            else
                PartialUIntDivOp(Value);
        }

public:
        void PartialUIntDivOp(const unsigned int& rValue) { PartialUIntDivOpV1(rValue); }
        void PartialIntDivOp(const signed int& rValue) { PartialIntDivOpV1(rValue); }
        void PartialUInt64DivOp(const unsigned int& rValue) { PartialUIntDivOpV1(rValue); }
        void PartialInt64DivOp(const signed __int64& rValue) { PartialIntDivOpV1(rValue); }

        void UnsignedPartialIntDivOp(const signed int& rValue) { PartialUIntDivOpV1(rValue); }
        void UnsignedPartialInt64DivOp(const signed __int64& rValue) { PartialUIntDivOpV1(rValue); }

protected:

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        void BasicUIntDivOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialUIntDivOpV1(Value);
            if (IntHalf == 0 && DecimalHalf == 0)
				DecimalHalf = 1;//Prevent Dividing into nothing
        }
		
        template<IntegerType IntType=signed int>
        void BasicIntDivOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialIntDivOpV1(Value);
            if (IntHalf == 0 && DecimalHalf == 0)
				DecimalHalf = 1;//Prevent Dividing into nothing
        }

        template<IntegerType IntType=unsigned int>
        MediumDecBase& BasicUIntDivOperationV1(const IntType& rValue)
        { BasicUIntDivOpV1(rValue); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDecBase& BasicIntDivOperationV1(const IntType& rValue)
        { BasicIntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        const auto BasicDivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntDivOperationV1(rValue); }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        const auto BasicDivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntDivOperationV1(rValue); }

public:

        void BasicUIntDivOp(const unsigned int& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicIntDivOp(const signed int& rValue) { BasicIntDivOpV1(rValue); }
        void BasicUInt64DivOp(const unsigned __int64& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt64DivOp(const signed __int64& rValue) { BasicIntDivOpV1(rValue); }

        void UnsignedBasicIntDivOp(const signed int& rValue) { BasicUIntDivOpV1(rValue); }
        void UnsignedBasicInt64DivOp(const signed __int64& rValue) { BasicUIntDivOpV1(rValue); }

        void BasicUInt8DivOp(const unsigned char& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt8DivOp(const signed char& rValue) { BasicIntDivOpV1(rValue); }
        void BasicUInt16DivOp(const unsigned short& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt16DivOp(const signed short& rValue) { BasicIntDivOpV1(rValue); }

        MediumDecBase& BasicUIntDivOperation(const unsigned int& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecBase& BasicIntDivOperation(const signed int& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecBase& BasicUInt64DivOperation(const unsigned __int64& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecBase& BasicInt64DivOperation(const signed __int64& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecBase& BasicUInt8DivOperation(const unsigned char& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecBase& BasicInt8DivOperation(const signed char& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecBase& BasicUInt16DivOperation(const unsigned short& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecBase& BasicInt16DivOperation(const signed short& rValue) { return BasicIntDivOperationV1(rValue); }

        const MediumDecBase BasicDivideByUInt(const unsigned int& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecBase BasicDivideByInt(const signed int& rValue) { return BasicDivideByIntV1(rValue); }
        const MediumDecBase BasicDivideByUInt64(const unsigned __int64& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecBase BasicDivideByInt64(const signed __int64& rValue) { return BasicDivideByIntV1(rValue); }

        const MediumDecBase UnsignedBasicDivideByInt(const signed int& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecBase UnsignedBasicDivideByInt64(const signed __int64& rValue) { return BasicDivideByUIntV1(rValue); }

        const MediumDecBase BasicDivideByUInt8(const unsigned char& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecBase BasicDivideByInt8(const signed char& rValue) { return BasicDivideByIntV1(rValue); }
        const MediumDecBase BasicDivideByUInt16(const unsigned short& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecBase BasicDivideByInt16(const signed short& rValue) { return BasicDivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations
protected:

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        bool UnsignedPartialDivOpV1(const VariantType& rValue)
        {
            unsigned _int64 SelfRes = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
            unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf.Value + rValue.DecimalHalf.Value;

            unsigned _int64 IntHalfRes = SelfRes / ValueRes;
            unsigned _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
			IntHalf.Value = (unsigned int) IntHalfRes;
            DecimalHalf.Value = DecimalRes;
            if (IntHalfRes == 0 && DecimalRes == 0)
                return true;
            else
                return false;
        }

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicUnsignedDivOpV1(const VariantType& rValue)
		{
			if(DecimalHalf==0)
			{
				if(rValue.DecimalHalf==0)
				{
					switch(rValue.IntHalf.Value)
					{
						case 2:
							if((IntHalf.Value&1)==1)//Check if number is odd
								UnsignedBasicIntDivOp(2);
							else
								IntHalf.Value /= 2;
							break;
						case 4:
							if(((IntHalf.Value >> 2) << 2) == IntHalf.Value)
								IntHalf.Value /= 4;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 8:
							if(((IntHalf.Value >> 3) << 3) == IntHalf.Value)
								IntHalf.Value /= 8;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 16:
							if(((IntHalf.Value >> 4) << 4) == IntHalf.Value)
								IntHalf.Value /= 16;
							else
								UnsignedBasicIntDivOp(4);
							break;
						case 32:
							if(((IntHalf.Value >> 5) << 5) == IntHalf.Value)
								IntHalf.Value /= 32;
							else
								UnsignedBasicIntDivOp(4);
							break;
                        case 0:
                            throw "Target rValue can not be divided by zero";
                            break;
						default:
							UnsignedBasicIntDivOp(rValue.IntHalf.Value);
							break;
					}
				}
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
                else if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
				        DecimalHalf.Value = 1;
#else
                else
                    UnsignedPartialDivOp(rValue);
#endif
			}
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
            else if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf.Value = 1;
#else
            else
                UnsignedPartialDivOp(rValue);
#endif
		}

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicDivOpV1(const VariantType& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                BasicUnsignedDivOp(-Value);
            }
            else
                BasicUnsignedDivOp(Value);
        }

public:

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool UnsignedPartialDivOp(const MediumDecBase& rValue){ return UnsignedPartialDivOpV1(rValue); }
		
		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void BasicUnsignedDivOp(const MediumDecBase& rValue){ BasicUnsignedDivOpV1(rValue); }
		
		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void BasicDivOp(const MediumDecBase& rValue){ BasicDivOpV1(rValue); }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecBase& BasicUnsignedDivOperation(const MediumDecBase& rValue)
		{ BasicUnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecBase& BasicDivOperation(const MediumDecBase& rValue)
		{ BasicDivOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        const MediumDecBase BasicDivideByUnsigned(const MediumDecBase& rValue)
        { MediumDecBase lValue = *this; return lValue.BasicUnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        const MediumDecBase BasicDivideBy(const MediumDecBase& rValue)
        { MediumDecBase lValue = *this; return lValue.BasicDivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecBase&</returns>
        void DivideByTwo()
        {
            if(DecimalHalf==0&&(IntHalf.Value&1)==1)//Check if number is odd
                UnsignedBasicIntDivOp(2);
            else
                IntHalf /= 2;
        }

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecBase&</returns>
        void DivideByFour()
        {
            //Checking if divisible by 4 based on
            //https://www.geeksforgeeks.org/check-number-divisible-8-using-bitwise-operators/
            //checking if divible by 8 equals (((n >> 3) << 3) == n)
            if(DecimalHalf==0&&(((IntHalf.Value >> 2) << 2) == IntHalf.Value))//Check if number can be perfectly divided by 4
                IntHalf /= 4;
            else
                UnsignedBasicIntDivOp(4);
        }

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicDivOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicIntDivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const signed __int64& rValue) { return lValue.BasicInt64DivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicUIntDivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64DivOperation(rValue); }

        friend MediumDecBase& operator/=(MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicInt8DivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicInt16DivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicUInt8DivOperation(rValue); }
        friend MediumDecBase& operator/=(MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicUInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicDivideBy(rValue); }
		
        /// <summary>
        /// Division operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator/(MediumDecBase lValue, const signed int& rValue) { return lValue.BasicIntDivOperation(rValue); }
        friend MediumDecBase operator/(MediumDecBase lValue, const signed __int64& rValue) { return lValue.BasicInt64DivOperation(rValue); }
        friend MediumDecBase operator/(MediumDecBase lValue, const unsigned int& rValue) { return lValue.BasicUIntDivOperation(rValue); }
        friend MediumDecBase operator/(MediumDecBase lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64DivOperation(rValue); }

        friend MediumDecBase operator/(MediumDecBase lValue, const signed char& rValue) { return lValue.BasicInt8DivOperation(rValue); }
        friend MediumDecBase operator/(MediumDecBase lValue, const signed short& rValue) { return lValue.BasicInt16DivOperation(rValue); }
        friend MediumDecBase operator/(MediumDecBase lValue, const unsigned char& rValue) { return lValue.BasicUInt8DivOperation(rValue); }
        friend MediumDecBase operator/(MediumDecBase lValue, const unsigned short& rValue) { return lValue.BasicUInt16DivOperation(rValue); }
		
        friend MediumDecBase operator/(const signed int& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const signed __int64& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const unsigned int& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const unsigned __int64& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicDivideBy(rValue); }

        friend MediumDecBase operator/(const signed char& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const signed short& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const unsigned char& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicDivideBy(rValue); }
        friend MediumDecBase operator/(const unsigned short& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicDivideBy(rValue); }

	#pragma endregion Other Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
		/// <summary>
        /// Partial version of BasicUIntMultOpV1 without zero checks
        /// (Modifies owner object) 
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=int>
        void PartialUIntMultOpV1(const IntType& rValue)
        {
            if (DecimalHalf == 0)
                IntHalf.Value *= rValue;
            else
			{
                __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntHalf.Value = (unsigned int)OverflowVal;
                    DecimalHalf.Value = (unsigned int)SRep;
                }
                else
                {
					IntHalf.Value = 0;
                    DecimalHalf.Value = (unsigned int)SRep;
                }
            }
        }
		
		//Partial version of BasicIntMultOpV1 without zero checks
		//Modifies owner object
        template<IntegerType IntType=signed int>
        void PartialIntMultOpV1(const IntType& Value)
        {
            if(Value<0)
            {
                SwapNegativeStatus();
                PartialUIntMultOpV1(-Value);
            }
            else
                PartialUIntMultOp(Value);
        }
public:
		
        void PartialUIntMultOp(const unsigned int& rValue) { PartialUIntMultOpV1(rValue); }
        void PartialIntMultOp(const signed int& rValue) { PartialIntMultOpV1(rValue); }
        void PartialUInt64MultOp(const unsigned __int64& rValue) { PartialUIntMultOpV1(rValue); }
        void PartialInt64MultOp(const signed __int64& rValue) { PartialIntMultOpV1(rValue); }

        void UnsignedPartialIntMultOp(const signed int& rValue) { PartialUIntMultOpV1(rValue); }
        void UnsignedPartialInt64MultOp(const signed __int64& rValue) { PartialUIntMultOpV1(rValue); }
		
protected:
        template<IntegerType IntType=signed int>
        void BasicUIntMultOpV1(const IntType& rValue)
        {
            if (rValue == 0)
            {
                SetAsZero();
                return;
            }
            else if (IsZero())
                return;
            PartialUIntMultOpV1(rValue);
        }

        template<IntegerType IntType=signed int>
        void BasicIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return;
            }
            else if (IsZero())
                return;
			PartialIntMultOpV1(Value);
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntMultOperationV1(const IntType& Value)
        { BasicUIntMultOpV1(Value); return *this; }

        template<IntegerType IntType=unsigned int>
        auto& BasicIntMultOperationV1(const IntType& Value)
        { BasicIntMultOpV1(Value); return *this; }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        const MediumDecBase BasicMultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntMultOperationV1(rValue); }

		/// <summary>
        /// Basic multiplication operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        const MediumDecBase BasicMultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntMultOperationV1(rValue); }

public:

        void BasicUIntMultOp(const unsigned int& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicIntMultOp(const signed int& rValue) { BasicIntMultOpV1(rValue); }
        void BasicUInt64MultOp(const unsigned __int64& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt64MultOp(const signed __int64& rValue) { BasicIntMultOpV1(rValue); }

        void UnsignedBasicIntMultOp(const signed int& rValue) { BasicUIntMultOpV1(rValue); }
        void UnsignedBasicInt64MultOp(const signed __int64& rValue) { BasicUIntMultOpV1(rValue); }

        void BasicUInt8MultOp(const unsigned char& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt8MultOp(const signed char& rValue) { BasicIntMultOpV1(rValue); }
        void BasicUInt16MultOp(const unsigned short& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt16MultOp(const signed short& rValue) { BasicIntMultOpV1(rValue); }

        MediumDecBase& BasicUIntMultOperation(const unsigned int& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecBase& BasicIntMultOperation(const signed int& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecBase& BasicUInt64MultOperation(const unsigned __int64& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecBase& BasicInt64MultOperation(const signed __int64& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecBase& BasicUInt8MultOperation(const unsigned char& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecBase& BasicInt8MultOperation(const signed char& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecBase& BasicUInt16MultOperation(const unsigned short& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecBase& BasicInt16MultOperation(const signed short& rValue) { return BasicIntMultOperationV1(rValue); }

        const MediumDecBase BasicMultiplyByUInt(const unsigned int& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecBase BasicMultiplyByInt(const signed int& rValue) { return BasicMultiplyByIntV1(rValue); }
        const MediumDecBase BasicMultiplyByUInt64(const unsigned __int64& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecBase BasicMultiplyByInt64(const signed __int64& rValue) { return BasicMultiplyByIntV1(rValue); }

        const MediumDecBase UnsignedBasicMultiplyByInt(const signed int& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecBase UnsignedBasicMultiplyByInt64(const signed __int64& rValue) { return BasicMultiplyByUIntV1(rValue); }

        const MediumDecBase BasicMultiplyByUInt8(const unsigned char& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecBase BasicMultiplyByInt8(const signed char& rValue) { return BasicMultiplyByIntV1(rValue); }
        const MediumDecBase BasicMultiplyByUInt16(const unsigned short& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecBase BasicMultiplyByInt16(const signed short& rValue) { return BasicMultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations
protected:

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicUnsignedMultOpV1(const VariantType& rValue)
		{
            if (DecimalHalf == 0)
            {
                if (IntHalf.Value == 1)
                {
					if(IntHalf.IsNegative())
						IntHalf = -rValue.IntHalf;
					else
						IntHalf = rValue.IntHalf;
					DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                    IntHalf *= rValue.IntHalf;
                else {
                    __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf.Value : DecimalOverflowX * rValue.IntHalf.Value + rValue.DecimalHalf.Value;
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)rRep;
                        return;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                    #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf.Value = 1;
                    #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
							SetAsZero(); return; }
                    #endif
                        IntHalf.Value = 0;
                        return;
                    }
                }
            }
            else if (IntHalf.Value == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf.Value;
                SRep /= DecimalOverflowX;
                if (rValue.IntHalf == 0)
                {
                    DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf.Value = 1;
				#elif !defined(AltNum_AllowNegativeZero)
                    if(DecimalHalf==0){
						SetAsZero(); return; }
                #endif
                    return;
                }
                else
                {
                    SRep += (__int64)DecimalHalf.Value * rValue.IntHalf.Value;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = OverflowVal;
                        DecimalHalf.Value = (signed int)SRep;
						return;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf.Value = 1;
				#elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
							SetAsZero(); return; }
                #endif
                        return;
                    }
                }
            }
            else
            {
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.IntHalf.Value;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)SRep;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf.Value = 1;
				#elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
							SetAsZero(); return; }
                #endif
                        }
                        IntHalf.Value = 0;
                    }
				    return;
                }
                else if (rValue.IntHalf == 0)
                {
                    __int64 SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.DecimalHalf.Value;
                    SRep /= DecimalOverflowX;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)SRep;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf.Value = 1;
				#elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
							SetAsZero(); return; }
                #endif
                        }
                        IntHalf.Value = 0;
                    }
                    return;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.IntHalf.Value;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    unsigned __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf.Value);//Temp03 holds __int64 version of X *.V
                    unsigned __int64 Temp04 = (__int64)DecimalHalf.Value * (__int64)rValue.DecimalHalf.Value;
                    Temp04 /= DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    unsigned __int64 IntegerRep = SRep + Temp03 + Temp04;
                    unsigned __int64 intHalf = IntegerRep / DecimalOverflow;
                    IntegerRep -= intHalf * DecimalOverflowX;
                    IntHalf.Value = (unsigned int) intHalf;
                    DecimalHalf.Value = (unsigned int)IntegerRep;
                }
            }
			#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntHalf==0)
                DecimalHalf.Value = 1;
			#elif !defined(AltNum_AllowNegativeZero)
            if(DecimalHalf==0)
				SetAsZero();
			#endif
		}

        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicMultOpV1(const VariantType& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                BasicUnsignedMultOp(-Value);
            }
            else
                BasicUnsignedMultOp(Value);
        }

public:

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        void BasicUnsignedMultOp(const MediumDecBase& rValue){ BasicUnsignedMultOpV1(rValue); }
		
        void BasicMultOp(const MediumDecBase& rValue){ BasicMultOpV1(rValue); }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecBase& BasicUnsignedMultOperation(const MediumDecBase& rValue)
        { BasicUnsignedMultOp(rValue); return *this; }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecBase& BasicMultOperation(const MediumDecBase& rValue)
		{ BasicMultOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecBase BasicMultiplyByUnsigned(const MediumDecBase& rValue)
        { MediumDecBase lValue = *this; return lValue.BasicUnsignedMultOperation(rValue); }

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecBase BasicMultiplyBy(const MediumDecBase& rValue)
        { MediumDecBase lValue = *this; return lValue.BasicMultOperation(rValue); }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region Other multiplication operations

        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>void</returns>
        void MultipleByTwo()
        {
	        UnsignedBasicIntMultOp(2);
        }

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>void</returns>
        void MultipleByFour()
        {
	        UnsignedBasicIntMultOp(4);
        }

        /// <summary>
        /// *= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicMultOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicIntMultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const signed __int64& rValue) { return lValue.BasicInt64MultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicUIntMultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64MultOperation(rValue); }

        friend MediumDecBase& operator*=(MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicInt8MultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicInt16MultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicUInt8MultOperation(rValue); }
        friend MediumDecBase& operator*=(MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicUInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator*(MediumDecBase lValue, const MediumDecBase& rValue) { return lValue.BasicMultOperation(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecBase operator*(MediumDecBase lValue, const signed int& rValue) { return lValue.BasicIntMultOperation(rValue); }
        friend MediumDecBase operator*(MediumDecBase lValue, const signed __int64& rValue) { return lValue.BasicInt64MultOperation(rValue); }
        friend MediumDecBase operator*(MediumDecBase lValue, const unsigned int& rValue) { return lValue.BasicUIntMultOperation(rValue); }
        friend MediumDecBase operator*(MediumDecBase lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64MultOperation(rValue); }
		
        friend MediumDecBase operator*(MediumDecBase lValue, const signed char& rValue) { return lValue.BasicInt8MultOperation(rValue); }
        friend MediumDecBase operator*(MediumDecBase lValue, const signed short& rValue) { return lValue.BasicInt16MultOperation(rValue); }
        friend MediumDecBase operator*(MediumDecBase lValue, const unsigned char& rValue) { return lValue.BasicUInt8MultOperation(rValue); }
        friend MediumDecBase operator*(MediumDecBase lValue, const unsigned short& rValue) { return lValue.BasicUInt16MultOperation(rValue); }
		
        friend MediumDecBase operator*(signed int lValue, MediumDecBase rValue)  { return rValue.BasicIntMultOperation(lValue); }
        friend MediumDecBase operator*(signed __int64 lValue, MediumDecBase& rValue)  { return rValue.BasicInt64MultOperation(lValue); }
        friend MediumDecBase operator*(unsigned int lValue, MediumDecBase& rValue)  { return rValue.BasicUIntMultOperation(lValue); }
        friend MediumDecBase operator*(unsigned __int64 lValue, MediumDecBase& rValue)  { return rValue.BasicUInt64MultOperation(lValue); }
		
        friend MediumDecBase operator*(signed char lValue, MediumDecBase& rValue)  { return rValue.BasicInt8MultOperation(lValue); }
        friend MediumDecBase operator*(signed short lValue, MediumDecBase& rValue)  { return rValue.BasicInt16MultOperation(lValue); }
        friend MediumDecBase operator*(unsigned char lValue, MediumDecBase& rValue)  { return rValue.BasicUInt8MultOperation(lValue); }
        friend MediumDecBase operator*(unsigned short lValue, MediumDecBase& rValue)  { return rValue.BasicUInt16MultOperation(lValue); }

	#pragma endregion Other multiplication operations

	#pragma region NormalRep Integer Addition Operations
protected:
		
        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void BasicUIntAddOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingUnsignedIntegerAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntAddOp((unsigned int)rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void BasicIntAddOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingIntegerAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue;
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntAddOperationV1(const IntType& rValue)
        { BasicUIntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& BasicIntAddOperationV1(const IntType& rValue)
        { BasicIntAddOpV1(rValue); return *this; }

		/// <summary>
        /// Basic addition operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto BasicAddByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntAddOperationV1(rValue); }

		/// <summary>
        /// Basic addition operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto BasicAddByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntAddOperationV1(rValue); }

public:

        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UnsignedIntegerAddition(const MirroredInt& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingUnsignedAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntAddOp(rValue.Value);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void IntegerAddition(const MirroredInt& rValue)
        {
			if(DecimalHalf.Value==0){
				IntHalf.NRepSkippingAddOp(rValue);
			} else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue;
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        void BasicUIntAddOp(const unsigned int& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingUnsignedAddOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntAddOp(rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        void BasicIntAddOp(const signed int& rValue) { BasicIntAddOpV1(rValue); }
        void BasicUInt64AddOp(const unsigned __int64& rValue) { BasicUIntAddOpV1(rValue); }
        void BasicInt64AddOp(const signed __int64& rValue) { BasicIntAddOpV1(rValue); }

        void UnsignedBasicIntAddOp(const signed int& rValue) { BasicUIntAddOpV1(rValue); }
        void UnsignedBasicInt64AddOp(const signed __int64& rValue) { BasicUIntAddOpV1(rValue); }

        void BasicUInt8AddOp(const unsigned char& rValue) { BasicUIntAddOpV1(rValue); }
        void BasicInt8AddOp(const signed char& rValue) { BasicIntAddOpV1(rValue); }
        void BasicUInt16AddOp(const unsigned short& rValue) { BasicUIntAddOpV1(rValue); }
        void BasicInt16AddOp(const signed short& rValue) { BasicIntAddOpV1(rValue); }

        MediumDecBase& BasicUIntAddOperation(const unsigned int& rValue)
        { BasicUIntAddOp(rValue); return *this; }

        MediumDecBase& BasicIntAddOperation(const signed int& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecBase& BasicUInt64AddOperation(const unsigned __int64& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecBase& BasicInt64AddOperation(const signed __int64& rValue) { return BasicIntAddOperationV1(rValue); }
		
        MediumDecBase& BasicUInt8AddOperation(const unsigned char& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecBase& BasicInt8AddOperation(const signed char& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecBase& BasicUInt16AddOperation(const unsigned short& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecBase& BasicInt16AddOperation(const signed short& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecBase BasicAddByUInt(const unsigned int& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecBase BasicAddByInt(const signed int& rValue) { return BasicAddByIntV1(rValue); }
        MediumDecBase BasicAddByUInt64(const unsigned __int64& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecBase BasicAddByInt64(const signed __int64& rValue) { return BasicAddByIntV1(rValue); }

        MediumDecBase UnsignedBasicAddByInt(const signed int& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecBase UnsignedBasicAddByInt64(const signed __int64& rValue) { return BasicAddByUIntV1(rValue); }

        MediumDecBase BasicAddByUInt8(const unsigned char& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecBase BasicAddByInt8(const signed char rValue) { return BasicAddByIntV1(rValue); }
        MediumDecBase BasicAddByUInt16(const unsigned short& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecBase BasicAddByInt16(const signed short& rValue) { return BasicAddByIntV1(rValue); }
    	
	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:
		
        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void BasicUIntSubOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingUnsignedIntegerSubOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntSubOp((unsigned int)rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void BasicIntSubOpV1(const IntType& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingIntegerSubOp(rValue);
			else {
				unsigned int signBeforeOp = IntHalf.Sign;
				IntHalf -= rValue;
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntSubOperationV1(const IntType& rValue)
        { BasicUIntSubOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& BasicIntSubOperationV1(const IntType& rValue)
        { BasicIntSubOpV1(rValue); return *this; }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto BasicSubtractByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntSubOperationV1(rValue); }

		/// <summary>
        /// Basic Subtraction operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto BasicSubtractByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntSubOperationV1(rValue); }

public:

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UnsignedIntegerSubtraction(const MirroredInt& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingUnsignedSubOp(rValue);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntSubOp(rValue.Value);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void IntegerSubtraction(const MirroredInt& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingSubOp(rValue);
			else {
				unsigned int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue;
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
			}
        }

        /// <summary>
        /// Basic Subtraction operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void BasicUIntSubOp(const unsigned int& rValue)
        {
			if(DecimalHalf.Value==0)
				IntHalf.NRepSkippingUnsignedSubOp(rValue);
			else {
				unsigned int signBeforeOp = IntHalf.Sign;
				IntHalf.UIntSubOp(rValue);
				if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
					DecimalHalf = DecimalOverflow - DecimalHalf;
			}
        }

        void BasicIntSubOp(const signed int& rValue) { BasicIntSubOpV1(rValue); }
        void BasicUInt64SubOp(const unsigned __int64& rValue) { BasicUIntSubOpV1(rValue); }
        void BasicInt64SubOp(const signed __int64& rValue) { BasicIntSubOpV1(rValue); }

        void UnsignedBasicIntSubOp(const signed int& rValue) { BasicUIntSubOpV1(rValue); }
        void UnsignedBasicInt64SubOp(const signed __int64& rValue) { BasicUIntSubOpV1(rValue); }

        void BasicUInt8SubOp(const unsigned char& rValue) { BasicUIntSubOpV1(rValue); }
        void BasicInt8SubOp(const signed char& rValue) { BasicIntSubOpV1(rValue); }
        void BasicUInt16SubOp(const unsigned short& rValue) { BasicUIntSubOpV1(rValue); }
        void BasicInt16SubOp(const signed short& rValue) { BasicIntSubOpV1(rValue); }

        MediumDecBase& BasicUIntSubOperation(const unsigned int& rValue)
        { BasicUIntSubOp(rValue); return *this; }

        MediumDecBase& BasicIntSubOperation(const signed int& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecBase& BasicUInt64SubOperation(const unsigned __int64& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecBase& BasicInt64SubOperation(const signed __int64& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecBase& BasicUInt8SubOperation(const unsigned char& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecBase& BasicInt8SubOperation(const signed char& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecBase& BasicUInt16SubOperation(const unsigned short& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecBase& BasicInt16SubOperation(const signed short& rValue) { return BasicIntSubOperationV1(rValue); }

        MediumDecBase BasicSubtractByUInt(const unsigned int& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecBase BasicSubtractByInt(const signed int& rValue) { return BasicSubtractByIntV1(rValue); }
        MediumDecBase BasicSubtractByUInt64(const unsigned __int64& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecBase BasicSubtractByInt64(const signed __int64& rValue) { return BasicSubtractByIntV1(rValue); }

        MediumDecBase UnsignedBasicSubtractByInt(const signed int& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecBase UnsignedBasicSubtractByInt64(const signed __int64& rValue) { return BasicSubtractByUIntV1(rValue); }

        MediumDecBase BasicSubtractByUInt8(const unsigned char& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecBase BasicSubtractByInt8(const signed char rValue) { return BasicSubtractByIntV1(rValue); }
        MediumDecBase BasicSubtractByUInt16(const unsigned short& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecBase BasicSubtractByInt16(const signed short& rValue) { return BasicSubtractByIntV1(rValue); }
    	
	#pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep AltNum Addition/Subtraction Operations
protected:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicUnsignedAddOpV1(const VariantType& rValue)
        {
			if(rValue.DecimalHalf==0)
                UnsignedIntegerAddition(rValue.IntHalf);
			else 
            {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UnsignedAddOp(rValue.IntHalf);
                if (signBeforeOp==MirroredInt::NegativeSign)
                {
					if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -4.5
						if(IntHalf.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					}
                    else if(rValue.DecimalHalf.Value>DecimalHalf.Value)
                    {
						++IntHalf;
						if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
							DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                        else//-1.6 + .7 = -0.9
							DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
					} else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
						DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10
					else
						DecimalHalf.Value -= rValue.DecimalHalf.Value;
                } else {
					unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
					if(decResult==DecimalOverflow){//5.4 + 4.6
						++IntHalf;
						if(IntHalf.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					} else if(decResult>DecimalOverflow){//5.4 + 4.7
						++IntHalf;
						DecimalHalf.Value = decResult - DecimalOverflow;
					}
					else if(signBeforeOp!=IntHalf.Sign)
						DecimalHalf.Value = DecimalOverflow - decResult;
					else
						DecimalHalf.Value = decResult;
                }
			}
		}
	
	    /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicAddOpV1(const VariantType& rValue)
        {
            if (rValue.DecimalHalf == 0)
                IntegerAddition(rValue.IntHalf);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf += rValue.IntHalf;
				
                if (signBeforeOp==MirroredInt::NegativeSign){
					if(rValue.IsPositive()){
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							++IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
								DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
							else//-1.6 + .7 = -0.9
								DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
						} else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
							DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10 
						else
							DecimalHalf.Value -= rValue.DecimalHalf.Value;
					} else {
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                        if (decResult == DecimalOverflow) {//-5.4 + - 5.6
                            --IntHalf;
                            if (IntHalf.Value == 0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1 
							--IntHalf;
							DecimalHalf.Value = decResult - DecimalOverflow;
						} else//-5.2 - 5.2 = -10.4
							DecimalHalf.Value = decResult;
					}
                } else {
					if(rValue.IsPositive()){
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
						if(decResult==DecimalOverflow){//5.5 + 4.5 = 10
							++IntHalf;
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(decResult>DecimalOverflow){//5.5 + 4.6 = 10.1
							++IntHalf;
							DecimalHalf.Value = decResult - DecimalOverflow;
						} else//5.4 + 5.3 = 10.7
							DecimalHalf.Value = decResult;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -5.5
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							--IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//4.3 - 5.4 = -1.1
								DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
							else//4.3 - 2.4 = 1.9
								DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
						} else if(signBeforeOp!=IntHalf.Sign)//5.4 + - 6.3 = -0.9
							DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 + 3 - 4
						else//4.4 + -2.3 = 2.1
							DecimalHalf.Value -= rValue.DecimalHalf.Value;
					}
                }
			}
		}
	
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicUnsignedSubOpV1(const VariantType& rValue)
        {
            if (rValue.DecimalHalf == 0)
                UnsignedIntegerSubtraction(rValue.IntHalf);
			else {
				int signBeforeOp = IntHalf.Sign;
				IntHalf.UnsignedSubOp(rValue.IntHalf);
                if (signBeforeOp==MirroredInt::NegativeSign){//-5 - B 
					unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                    if (decResult == DecimalOverflow){//-5.4 - 5.6
                        --IntHalf;
                        if (IntHalf.Value == 0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1 
						--IntHalf;
						DecimalHalf.Value = decResult - DecimalOverflow;
					} else//-5.2 - 5.2 = -10.4
						DecimalHalf.Value = decResult;
                } else {//5.XX - B
					if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - 5.5 = 10
						if(IntHalf.Value==0)
							SetAsZero();
						else
							DecimalHalf.Value = 0;
					} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
						--IntHalf;
						if(signBeforeOp!=IntHalf.Sign)//5.4 - 5.7 = -0.3
							DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
						else//5.4 - 3.6 = 1.8
							DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
					} else if(signBeforeOp!=IntHalf.Sign)//5.3 - 7.2 = -1.9
						DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
					else//5.4 - 5.3 = 0.1 
						DecimalHalf.Value -= rValue.DecimalHalf.Value;
                }
			}
		}
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void BasicSubOpV1(const VariantType& rValue)
        {
			if(rValue.DecimalHalf==0)
                IntegerSubtraction(rValue.IntHalf);
			else 
			{
				int signBeforeOp = IntHalf.Sign;
				IntHalf -= rValue.IntHalf;
                if (signBeforeOp==MirroredInt::NegativeSign)
				{
					if(rValue.IsPositive())
					{
						unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
						if(decResult==DecimalOverflow){//-5.4 - 5.6
							--IntHalf;
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(decResult>DecimalOverflow){//-5.4 - 5.7 = -11.1 
							--IntHalf;
							DecimalHalf.Value = decResult - DecimalOverflow;
						} else//-5.2 - 5.2 = -10.4
							DecimalHalf.Value = decResult;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//-5.4 - -4.4
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
							++IntHalf;
							if(signBeforeOp!=IntHalf.Sign)//-5.4 - -6.5 = 1.1
								DecimalHalf = DecimalOverflow - DecimalHalf.Value - rValue.DecimalHalf;
							else//-5.4 - -4.5 = -0.9 == -5.4 + 4.5
								DecimalHalf.Value += rValue.DecimalHalf.Value;
						} else if(signBeforeOp!=IntHalf.Sign)//-5.4 - -7.3 = 1.9
							DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;
						else//-5.4 - -3.3 = -2.1
							DecimalHalf.Value -= rValue.DecimalHalf.Value;
					}
                } else {
					if(rValue.IsPositive()){
                        if (DecimalHalf.Value == rValue.DecimalHalf.Value) {//5.5 - 5.5 = 10
                            if (IntHalf.Value == 0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        }
                        else if (rValue.DecimalHalf.Value > DecimalHalf.Value) {
                            --IntHalf;
                            if (signBeforeOp != IntHalf.Sign)//5.4 - 5.7 = -0.3
                                DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                            else//5.4 - 3.6 = 1.8
                                DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                        }
                        else if (signBeforeOp != IntHalf.Sign)//5.3 - 7.2 = -1.9
                            DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
                        else//5.4 - 5.3 = 0.1 
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
					} else {
						if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - -5.5 = 11
							++IntHalf;
							if(IntHalf.Value==0)
								SetAsZero();
							else
								DecimalHalf.Value = 0;
						} else if(rValue.DecimalHalf.Value>DecimalHalf.Value){//5.4 - -5.7 = 11.1
							++IntHalf;
							DecimalHalf.Value = DecimalHalf.Value + rValue.DecimalHalf.Value - DecimalOverflow;
						} else//5.4 - -5.3 = 10.7 
							DecimalHalf.Value += rValue.DecimalHalf.Value;
					}
                }
			}
		}

public:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedAddOp(const MediumDecBase& rValue){ BasicUnsignedAddOpV1(rValue); }

	    /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicAddOp(const MediumDecBase& rValue){ BasicAddOpV1(rValue); }

	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedSubOp(const MediumDecBase& rValue){ BasicUnsignedSubOpV1(rValue); }
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicSubOp(const MediumDecBase& rValue){ BasicSubOpV1(rValue); }

        //Basic addition operation
        MediumDecBase& BasicUnsignedAddOperation(const MediumDecBase& rValue)
        { BasicUnsignedAddOp(rValue); return *this; }
		
        //Basic addition operation
        MediumDecBase& BasicAddOperation(const MediumDecBase& rValue)
        { BasicAddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecBase BasicAddByUnsigned(const MediumDecBase& rValue)
        { MediumDecBase lValue = *this; return lValue.BasicUnsignedAddOperation(rValue); } const

		/// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecBase BasicAddBy(const MediumDecBase& rValue)
        { MediumDecBase lValue = *this; return lValue.BasicAddOperation(rValue); } const

        //Basic subtraction operation
        MediumDecBase& BasicUnsignedSubOperation(const MediumDecBase& rValue)
        { BasicUnsignedSubOp(rValue); return *this; }

        //Basic subtraction operation
        MediumDecBase& BasicSubOperation(const MediumDecBase& rValue)
        { BasicSubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecBase BasicSubtractByUnsigned(const MediumDecBase& rValue)
        { MediumDecBase lValue = *this; return lValue.BasicUnsignedSubOperation(rValue); } const

		/// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecBase BasicSubtractBy(const MediumDecBase& rValue)
        { MediumDecBase lValue = *this; return lValue.BasicSubOperation(rValue); } const

    #pragma endregion NormalRep AltNum Addition/Subtraction Operations

	#pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicAddOperation(rValue); }

        /// <summary>
        /// += operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicIntAddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const signed __int64& rValue) { return lValue.BasicInt64AddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicUIntAddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64AddOperation(rValue); }

        friend MediumDecBase& operator+=(MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicInt8AddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicInt16AddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicUInt8AddOperation(rValue); }
        friend MediumDecBase& operator+=(MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicUInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator+(MediumDecBase lValue, const MediumDecBase& rValue) { return lValue.BasicAddOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecBase operator+(MediumDecBase lValue, const signed int& rValue) { return lValue.BasicIntAddOperation(rValue); }
        friend MediumDecBase operator+(MediumDecBase lValue, const signed __int64& rValue) { return lValue.BasicInt64AddOperation(rValue); }
        friend MediumDecBase operator+(MediumDecBase lValue, const unsigned int& rValue) { return lValue.BasicUIntAddOperation(rValue); }
        friend MediumDecBase operator+(MediumDecBase lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64AddOperation(rValue); }
		
        friend MediumDecBase operator+(MediumDecBase lValue, const signed char& rValue) { return lValue.BasicInt8AddOperation(rValue); }
        friend MediumDecBase operator+(MediumDecBase lValue, const signed short& rValue) { return lValue.BasicInt16AddOperation(rValue); }
        friend MediumDecBase operator+(MediumDecBase lValue, const unsigned char& rValue) { return lValue.BasicUInt8AddOperation(rValue); }
        friend MediumDecBase operator+(MediumDecBase lValue, const unsigned short& rValue) { return lValue.BasicUInt16AddOperation(rValue); }
		
        friend MediumDecBase operator+(const signed int& lValue, MediumDecBase rValue)  { return rValue.BasicIntAddOperation(lValue); }
        friend MediumDecBase operator+(const signed __int64& lValue, MediumDecBase rValue)  { return rValue.BasicInt64AddOperation(lValue); }
        friend MediumDecBase operator+(const unsigned int& lValue, MediumDecBase rValue)  { return rValue.BasicUIntAddOperation(lValue); }
        friend MediumDecBase operator+(const unsigned __int64& lValue, MediumDecBase rValue)  { return rValue.BasicUInt64AddOperation(lValue); }
		
        friend MediumDecBase operator+(const signed char& lValue, MediumDecBase rValue)  { return rValue.BasicInt8AddOperation(lValue); }
        friend MediumDecBase operator+(const signed short& lValue, MediumDecBase rValue)  { return rValue.BasicInt16AddOperation(lValue); }
        friend MediumDecBase operator+(const unsigned char& lValue, MediumDecBase rValue)  { return rValue.BasicUInt8AddOperation(lValue); }
        friend MediumDecBase operator+(const unsigned short& lValue, MediumDecBase rValue)  { return rValue.BasicUInt16AddOperation(lValue); }

	#pragma endregion Other addition operations

	#pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const MediumDecBase& rValue) { return lValue.BasicSubOperation(rValue); }

        /// <summary>
        /// -= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const signed int& rValue) { return lValue.BasicIntSubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const signed __int64& rValue) { return lValue.BasicInt64SubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const unsigned int& rValue) { return lValue.BasicUIntSubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64SubOperation(rValue); }

        friend MediumDecBase& operator-=(MediumDecBase& lValue, const signed char& rValue) { return lValue.BasicInt8SubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const signed short& rValue) { return lValue.BasicInt16SubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const unsigned char& rValue) { return lValue.BasicUInt8SubOperation(rValue); }
        friend MediumDecBase& operator-=(MediumDecBase& lValue, const unsigned short& rValue) { return lValue.BasicUInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator-(MediumDecBase lValue, const MediumDecBase& rValue) { return lValue.BasicSubOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecBase operator-(MediumDecBase lValue, const signed int& rValue) { return lValue.BasicIntSubOperation(rValue); }
        friend MediumDecBase operator-(MediumDecBase lValue, const signed __int64& rValue) { return lValue.BasicInt64SubOperation(rValue); }
        friend MediumDecBase operator-(MediumDecBase lValue, const unsigned int& rValue) { return lValue.BasicUIntSubOperation(rValue); }
        friend MediumDecBase operator-(MediumDecBase lValue, const unsigned __int64& rValue) { return lValue.BasicUInt64SubOperation(rValue); }
		
        friend MediumDecBase operator-(MediumDecBase lValue, const signed char& rValue) { return lValue.BasicInt8SubOperation(rValue); }
        friend MediumDecBase operator-(MediumDecBase lValue, const signed short& rValue) { return lValue.BasicInt16SubOperation(rValue); }
        friend MediumDecBase operator-(MediumDecBase lValue, const unsigned char& rValue) { return lValue.BasicUInt8SubOperation(rValue); }
        friend MediumDecBase operator-(MediumDecBase lValue, const unsigned short& rValue) { return lValue.BasicUInt16SubOperation(rValue); }
		
        friend MediumDecBase operator-(const signed int& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const signed __int64& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const unsigned int& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const unsigned __int64& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicSubtractBy(rValue); }
		
        friend MediumDecBase operator-(const signed char& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const signed short& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const unsigned char& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicSubtractBy(rValue); }
        friend MediumDecBase operator-(const unsigned short& lValue, const MediumDecBase& rValue) { return ((MediumDecBase)lValue).BasicSubtractBy(rValue); }

	#pragma endregion Other subtraction operations

	#pragma region Modulus Operations
	#pragma endregion Modulus Operations

	#pragma region Bitwise Operations
    //Update code later
    /*
    #if defined(AltNum_EnableBitwiseOverride)
        /// <summary>
        /// Bitwise XOR Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=signed int>
        friend MediumDec operator^(MediumDec self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntHalf ^= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntHalf < 0;
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntHalf == NegativeRep)
                {
                    self.IntHalf = (0 & Value) * -1;
                    self.DecimalHalf ^= Value;
                }
                else
                {
                    self.IntHalf ^= Value; self.DecimalHalf ^= Value;
                }
            }
            return self;
        }

        /// <summary>
        /// Bitwise Or Operation Between MediumDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDec</returns>
        template<IntegerType IntType=signed int>
        friend MediumDec operator|(MediumDec self, IntType Value)
        {
            if (self.DecimalHalf == 0) { self.IntHalf |= Value; return self; }
            else
            {
                bool SelfIsNegative = self.IntHalf < 0;
                bool ValIsNegative = Value < 0;
                if (SelfIsNegative && self.IntHalf == NegativeRep)
                {
                    self.IntHalf = (0 & Value) * -1;
                    self.DecimalHalf |= Value;
                }
                else
                {
                    self.IntHalf |= Value; self.DecimalHalf |= Value;
                }
            }
            return self;
        }
    #endif
    */
	#pragma endregion Bitwise Operations

    /*
    #pragma region Floating Operator Overrides
    
        friend MediumDecBase operator+(const MediumDecBase& self, const float& Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(const MediumDecBase& self, const float& Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(const MediumDecBase& self, const float& Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(const MediumDecBase& self, const float& Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(const float& Value, const MediumDecBase& self) { return (MediumDecBase)Value + self; }
        friend MediumDecBase operator-(const float& Value, const MediumDecBase& self) { return (MediumDecBase)Value - self; }
        friend MediumDecBase operator*(const float& Value, const MediumDecBase& self) { return (MediumDecBase)Value * self; }
        friend MediumDecBase operator/(const float& Value, const MediumDecBase& self) { return (MediumDecBase)Value / self; }

        friend MediumDecBase operator+(const MediumDecBase& self, const double& Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(const MediumDecBase& self, const double& Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(const MediumDecBase& self, const double& Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(const MediumDecBase& self, const double& Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(const MediumDecBase& self, const long double& Value) { return self + (MediumDecBase)Value; }
        friend MediumDecBase operator-(const MediumDecBase& self, const long double& Value) { return self - (MediumDecBase)Value; }
        friend MediumDecBase operator*(const MediumDecBase& self, const long double& Value) { return self * (MediumDecBase)Value; }
        friend MediumDecBase operator/(const MediumDecBase& self, const long double& Value) { return self / (MediumDecBase)Value; }

        friend MediumDecBase operator+(const long double& Value, const MediumDecBase& self) { return (MediumDecBase)Value + self; }
        friend MediumDecBase operator-(const long double& Value, const MediumDecBase& self) { return (MediumDecBase)Value - self; }
        friend MediumDecBase operator*(const long double& Value, const MediumDecBase& self) { return (MediumDecBase)Value * self; }
        friend MediumDecBase operator/(const long double& Value, const MediumDecBase& self) { return (MediumDecBase)Value / self; }

    #pragma endregion Floating Operator Overrides
    */


    #pragma region Other Operators

        /// <summary>
        /// ++MediumDecBase Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator ++()
        {
            if (DecimalHalf == 0)
                ++IntHalf;
            else if (IntHalf == NegativeRep)
                IntHalf = MirroredInt::Zero;
            else
                ++IntHalf;
            return *this;
        }

        /// <summary>
        /// ++MediumDecBase Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator --()
        {
            if (DecimalHalf == 0)
                --IntHalf;
            else if (IntHalf == 0)
                IntHalf = NegativeRep;
            else
                --IntHalf;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator ++(int)
        {
            MediumDecBase tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDecBase</returns>
        MediumDecBase operator --(int)
        {
            MediumDecBase tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        MediumDecBase& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	#pragma region Truncation Functions

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        void ApplyAbs(){ IntHalf.ApplyAbs(); }

protected:

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        template<MediumDecVariant VariantType = MediumDecBase>
        VariantType AbsOfV1() {
            VariantType result = *this; result.ApplyAbs();
            return result;
        }

public:

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        MediumDecBase AbsOf() { return AbsOfV1(); }

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        static MediumDecBase Abs(const MediumDecBase& tValue) {
            MediumDecBase result = tValue; result.ApplyAbs();
            return result;
        }



        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        void FloorOf(const int& precision = 0)
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
            default:
				break;
            }
            if(IntHalf==MirroredInt::NegativeZero&&DecimalHalf==0)
                IntHalf = 0;
        }

protected:

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
		template<MediumDecVariant VariantType=MediumDecBase>
        VariantType& CeilOfV1()
        {
            if (DecimalHalf != 0)
				return VariantType(IntValue+1);
            else
				return *this;
        }
		
        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>VariantType&</returns>
		template<MediumDecVariant VariantType=MediumDecBase>
        static VariantType CeilV1(const VariantType& tValue)
        {
			if(tValue.IntHalf==MirroredInt::NegativeZero)
				return VariantType::One;
            else if (tValue.DecimalHalf != 0)
				return VariantType(tValue.IntValue+1);
            else
				return *this;
        }
		
        /// <summary>
        /// Returns floored value with all fractional digits after specified precision cut off.
        /// </summary>
        /// <param name="Value">The target value to apply on.</param>
        /// <param name="precision">The precision.</param>
        static VariantType FloorV1(VariantType tValue, const int& precision = 0)
        {
			unsigned int decimalRes = DecimalHalf.Value;
            switch (precision)
            {
            case 8: decimalRes /= 10; decimalRes *= 10; break;
            case 7: decimalRes /= 100; decimalRes *= 100; break;
            case 6: decimalRes /= 1000; decimalRes *= 1000; break;
            case 5: decimalRes /= 10000; decimalRes *= 10000; break;
            case 4: decimalRes /= 100000; decimalRes *= 100000; break;
            case 3: decimalRes /= 1000000; decimalRes *= 1000000; break;
            case 2: decimalRes /= 10000000; decimalRes *= 10000000; break;
            case 1: decimalRes /= 100000000; decimalRes *= 100000000; break;
            default: decimalRes = 0; break;
            }
			if(decimalRes==0&&IntValue==MirroredInt::NegativeZero)
				return VariantType();
			else
				return VariantType(tValue.IntHalf, PartialInt(decimalRes,tValue.DecimalHalf.Flags));
            return Value;
        }
		
		
public:

		MediumDecBase CeilOf() { return CeilOfV1(); }
		
		static MediumDecBase Ceil(const MediumDecBase& tValue) { return CeilV1(tValue); }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        signed int FloorIntOf()
        {
            if (DecimalHalf == 0)
                return (int)IntHalf;
            else if (IntHalf == MirroredInt::NegativeZero)
				return -1;
            else
                return (int)IntHalf - 1;
        }

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to Value (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
		static signed int FloorInt(const MediumDecBase& tValue) { return tValue.FloorIntOf(); }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
        int CeilIntOf()
        {
            if (DecimalHalf == 0)
                return (int)IntHalf;
            else if (IntHalf == MirroredInt::NegativeZero)
				return 0;
            else
                return (int)IntHalf + 1;
        }

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to Value (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecBase&</returns>
		static signed int CeilInt(const MediumDecBase& tValue) { return tValue.CeilIntOf(); }

        /// <summary>
        /// Cuts off the decimal point from number
        /// </summary>
        /// <returns>MediumDecBase &</returns>
        auto& TruncOf()
        {
            DecimalHalf = 0;
            if (IntHalf == NegativeRep)
                IntHalf = 0;
            return *this;
        }

		static MediumDecBase Trunc(MediumDecBase tValue) { return tValue.TruncOf(); }

	#pragma endregion Truncation Functions

	#pragma region Pow and Sqrt Functions
protected:

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        template<MediumDecVariant VariantType = MediumDecBase>
        VariantType SqrtOfV1(const int& precision=7)
        { 
            if(IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
            else if (DecimalHalf == 0)
            {
                VariantType value = *this;
                bool AutoSetValue = true;
                switch (IntHalf.Value)
                {
                case 1: value.IntHalf = 1; break;
                case 4: value.IntHalf = 2; break;
                case 9: value.IntHalf = 3; break;
                case 16: value.IntHalf = 4; break;
                case 25: value.IntHalf = 5; break;
                case 36: value.IntHalf = 6; break;
                case 49: value.IntHalf = 7; break;
                case 64: value.IntHalf = 8; break;
                case 81: value.IntHalf = 9; break;
                case 100: value.IntHalf = 10; break;
                case 121: value.IntHalf = 11; break;
                case 144: value.IntHalf = 12; break;
                case 169: value.IntHalf = 13; break;
                case 196: value.IntHalf = 14; break;
                case 225: value.IntHalf = 15; break;
                case 256: value.IntHalf = 16; break;
                case 289: value.IntHalf = 17; break;
                case 324: value.IntHalf = 18; break;
                case 361: value.IntHalf = 19; break;
                case 400: value.IntHalf = 20; break;
                case 1600: value.IntHalf = 40; break;
                default:
                    AutoSetValue = false;
                    break;
                }
                if(AutoSetValue)
                    return value;//Technically both positive and negative numbers of same equal the result
            }

            VariantType number = *this;
            VariantType start = VariantType(), end = number;
            VariantType mid;

            // variable to store the answer 
            VariantType ans;

            // for computing integral part 
            // of square root of number 
            while (start <= end) {
                mid = (start + end);
                mid.DivideByTwo();
                if (mid * mid == number) {
                    ans = mid;
                    break;
                }

                // incrementing start if integral 
                // part lies on right side of the mid 
                if (mid * mid < number) {
                    start = mid + 1;
                    ans = mid;
                }

                // decrementing end if integral part 
                // lies on the left side of the mid 
                else {
                    end = mid - 1;
                }
            }

            // For computing the fractional part 
            // of square root up to given precision 
            VariantType increment = VariantType(0,100000000);//0.1
            for (int i = 0; i < precision; ++i) {
                while (ans * ans <= number) {
                    ans += increment;
                }

                // loop terminates when ans * ans > number 
                ans = ans - increment;
                increment = increment / 10;
            }
            return ans;
        }

public:

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    MediumDecBase SqrtOf(const int& precision = 7) {
        return SqrtOfV1(precision);
    }

    /// <summary>
    /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
    /// </summary>
    static auto Sqrt(const auto& value, const int& precision = 7)
    {
        return value.SqrtOf(precision);
    }

protected:
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<MediumDecVariant VariantType=MediumDecBase, IntegerType IntType=unsigned int>
        void UIntPowOfOperationV1(const IntType& expValue)
        {
            if (expValue == 1) { return; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&(expValue&1)==1)
                    IntHalf.Sign = MirroredInt::PositiveSign;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
				IntType exp = expValue;
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                VariantType self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (exp > 0)
                {
                    // If expValue is odd, multiply self with result
                    if ((exp&1) == 1)
                        BasicUnsignedMultOp(self);
                    // n must be even now
                    exp = exp >> 1; // y = y/2
                    self.BasicUnsignedMultOp(self); // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.Sign = MirroredInt::NegativeSign;
            }
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<MediumDecVariant VariantType=MediumDecBase, IntegerType IntType=signed int>
        void IntPowOfOperationV1(const IntType& expValue)
        {
            if (expValue == 1) { return; }//Return self
            else if (expValue == 0)
            {
                IntHalf = 1; DecimalHalf = 0;
            }
            else if (expValue < 0)//Negative Pow
            {
                IntType exp = expValue * -1;
                if (DecimalHalf == 0 && IntHalf == 10 && expValue >= -9)
                {
                    IntHalf = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[exp];
                    if(IsNegative()&&(exp&1)==1)
                        IntHalf.Sign = MirroredInt::PositiveSign;
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    //Switches from negative to positive if exp is odd number
                    bool IsNegative = IsPositive()?false:(exp&1)==1?false:true;
                    VariantType self = AbsOf();//Prevent needing to flip the sign
                    IntHalf = 1; DecimalHalf = 0;// Initialize result
                    while (exp > 0)
                    {
                        // If expValue is odd, multiply self with result
                        if (exp & 1 == 1)
                            BasicUnsignedDivOp(self);
                        // n must be even now
                        exp = exp >> 1; // y = y/2
                        self.BasicUnsignedMultOp(self); //  Change x to x^2
                    }
                    if(IsNegative)
                        IntHalf.Sign = MirroredInt::NegativeSign;
                }
            }
            else if (DecimalHalf == 0 && IntHalf.Value == 10)
            {
                if(IsNegative()&&(expValue&1)==1)
                    IntHalf.Sign = MirroredInt::PositiveSign;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                //Switches from negative to positive if exp is odd number
				IntType exp = expValue;
                bool IsNegative = IsPositive()?false:(exp&1)==1?false:true;
                VariantType self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (exp > 0)
                {
                    // If expValue is odd, multiply self with result
                    if ((exp & 1) == 1)
                        BasicUnsignedMultOp(self);
                    // n must be even now
                    exp = exp >> 1; // y = y/2
                    self.BasicUnsignedMultOp(self); // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.Sign = MirroredInt::NegativeSign;
            }
        }

		template<MediumDecVariant VariantType=MediumDecBase>
		VariantType UnsignedNegIntPowerV1(const unsigned int& expValue)
		{
			unsigned int exp = expValue;
			//Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
			//Switches from negative to positive if exp is odd number
			bool IsNegative = IsPositive()?false:(exp&1)==1?false:true;
            VariantType self = AbsOf();
			IntHalf = 1; DecimalHalf = 0;// Initialize result
			while (exp > 0)
			{
				// If expValue is odd, divide self with result
				if ((exp & 1) == 1)
                    BasicUnsignedDivOp(self);
				// n must be even now
				exp = exp >> 1; // y = y/2
                self.BasicUnsignedMultOp(self); // Change x to x^2
			}
			if(IsNegative)
				IntHalf.Sign = 0;
		}

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// (Doesn't modify owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<MediumDecVariant VariantType = MediumDecBase, IntegerType IntType = signed int>
        MediumDecBase UIntPowOfV1(const signed int& expValue)
        {
            auto result = *this; UIntPowOfOperationV1(expValue);
            return result;
        }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// (Doesn't modify owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<MediumDecVariant VariantType = MediumDecBase, IntegerType IntType = signed int>
        MediumDecBase IntPowOfV1(const signed int& expValue)
        {
            auto result = *this; IntPowOfOperationV1(expValue);
            return result;
        }

public:

        MediumDecBase UnsignedNegIntPower(const unsigned int& expValue)
		{ return UnsignedNegIntPowerV1(expValue); }

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
		/// (Modifies owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecBase UIntPowOfOp(const unsigned int& expValue)
		{ UIntPowOfOperationV1(expValue); return *this; }
        MediumDecBase UInt64PowOfOp(const unsigned __int64& expValue)
		{ IntPowOfOperationV1(expValue); return *this; }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
		/// (Modifies owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecBase IntPowOfOp(const signed int& expValue)
		{ IntPowOfOperationV1(expValue); return *this; }
        MediumDecBase Int64PowOfOp(const signed __int64& expValue)
		{ IntPowOfOperationV1(expValue); return *this; }
		
        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
		/// (Doesn't modify owner object) 
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecBase UIntPowOf(const unsigned int& expValue)
        { return UIntPowOfV1(expValue); }
        MediumDecBase UInt64PowOf(const unsigned __int64& expValue)
        { return UIntPowOfV1(expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
		/// (Doesn't modify owner object)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecBase IntPowOf(const signed int& expValue)
		{ return IntPowOfV1(expValue); }
        MediumDecBase Int64PowOf(const signed __int64& expValue)
		{ return IntPowOfV1(expValue); }

protected:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
		template<MediumDecVariant VariantType=MediumDecBase>
        VariantType NthRootOfV1(const unsigned int& n, const VariantType& precision = VariantType::JustAboveZero)
        {
            if (IsNegative())
                throw "Nth root of a negative number requires imaginary number support";
            VariantType xPre = ((*this - 1) / n) + 1;//Estimating initial guess based on https://math.stackexchange.com/questions/787019/what-initial-guess-is-used-for-finding-n-th-root-using-newton-raphson-method
            int nMinus1 = n - 1;

            // initializing difference between two
            // roots by INT_MAX
            VariantType delX = VariantType(2147483647);

            //  xK denotes current value of x
            VariantType xK;

            //  loop until we reach desired accuracy
            do
            {
                //  calculating current value from previous
                // value by newton's method
                
                xK = (xPre * nMinus1 + BasicDivideByUnsigned(xPre.UIntPowOfV1(nMinus1))) / n;
                delX = VariantType::Abs(xK - xPre);
                xPre = xK;
            } while (delX > precision);
            return xK;
        }
		
public:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
        MediumDecBase NthRootOf(const unsigned int& n, const MediumDecBase& precision = MediumDecBase::JustAboveZero){ return NthRootOfV1(n, precision); }

protected:

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
		template<MediumDecVariant VariantType=MediumDecBase>
        static VariantType FractionalPowV1(const auto& value, const boost::rational<unsigned int>& Frac)
		{
			VariantType targetVal = UIntPowOf(Frac.numerator());
			VariantType CalcVal = MediumDecVariant::NthRoot(targetVal, Frac.denominator());
			return CalcVal;
		}

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
		template<MediumDecVariant VariantType=MediumDecBase>
        VariantType FractionalPowV2(const VariantType& value, const signed int& expNum, const unsigned int& expDenom)
		{
			auto targetVal = IntPowOf(expNum);
			auto CalcVal = MediumDecVariant::NthRoot(targetVal, expDenom);
			return CalcVal;
		}

        /// <summary>
        /// Calculate to power of unsigned expValue
		/// (Doesn't modify owner object)
        /// </summary>
		template<MediumDecVariant VariantType=MediumDecBase>
        VariantType BasicUnsignedPowOfV1(const auto& expValue)
		{
			boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
			if(expValue.IntHalf.Value==0)
				return FractionalPowV1(Frac);
			else {
				VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
				CalcVal *= FractionalPowV1(Frac);
				return CalcVal;
			}
		}

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
		template<MediumDecVariant VariantType=MediumDecBase>
        VariantType BasicPowOfV1(const auto& expValue)
		{
			boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
			if (expValue.IntHalf.IsNegative()){//Negative Exponent
				if(expValue.IntHalf.Value==0)
					return VariantType::One/FractionalPowV1(Frac);
				else {
					VariantType CalcVal = One / UIntPowOf(expValue.IntHalf.Value);
					CalcVal /= FractionalPowV1(Frac);
					return CalcVal;
				}
			} else {
				if(expValue.IntHalf.Value==0)
					return FractionalPowV1(Frac);
				else {
					VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
					CalcVal *= FractionalPowV1(Frac);
					return CalcVal;
				}
			}
		}

public:

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
        MediumDecBase BasicUnsignedPowOf(const auto& expValue)
		{ return BasicUnsignedPowOfV1(expValue); }

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
        MediumDecBase BasicPowOf(const auto& expValue)
		{ return BasicPowOfV1(expValue); }

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:	
	
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>VariantType</returns>
		template<MediumDecVariant VariantType=MediumDecBase>
        static VariantType Exp(const VariantType& x)
        {
            /*
             * Evaluates f(x) = e^x for any x in the interval [-709, 709].
             * If x < -709 or x > 709, raises an assertion error. Implemented
             * using the truncated Taylor series of e^x with ceil(|x| * e) * 12
             * terms. Achieves at least 14 and at most 16 digits of precision
             * over the entire interval.
             * Performance - There are exactly 36 * ceil(|x| * e) + 5
             * operations; 69,413 in the worst case (x = 709 or -709):
             * - (12 * ceil(|x| * e)) + 2 multiplications
             * - (12 * ceil(|x| * e)) + 1 divisions
             * - (12 * ceil(|x| * e)) additions
             * - 1 rounding
             * - 1 absolute value
             * Accuracy - Over a sample of 10,000 linearly spaced points in
             * [-709, 709] we have the following error statistics:
             * - Max relative error = 8.39803e-15
             * - Min relative error = 0.0
             * - Avg relative error = 0.0
             * - Med relative error = 1.90746e-15
             * - Var relative error = 0.0
             * - 0.88 percent of the values have less than 15 digits of precision
             * Args:
             *      - x: power of e to evaluate
             * Returns:
             *      - approximation of e^x in VariantType precision
             */
             // Check that x is a valid input.
            assert(x.IntHalf.Value < 709);
            // When x = 0 we already know e^x = 1.
            if (x.IsZero()) {
                return VariantType::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            VariantType x0 = VariantType::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            VariantType tn = VariantType::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = VariantType::CeilInt(x0 * VariantType::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + VariantType::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x.IsNegative()) {
                tn = VariantType::One / tn;
            }
            return tn;
        }
		
public:

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>VariantType</returns>
        static VariantType Exp(const VariantType& x) { return ExpV1(x); }

	#pragma endregion Log Functions

    #pragma region Trigonomic Functions

/*
        /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        auto SinOf()
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            for (int i = 1; i < 7; ++i)
            {
                SinValue += Pow(*this, 2 * i + 1)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
            }
            return SinValue;
        }

        /// <summary>
        /// Get Cos from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        auto CosOf()
        {
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            for (int i = 1; i < 7; ++i)
            {
                CosValue += Pow(*this, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue;
        }

        /// <summary>
        /// Get Tan from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        auto TanOf()
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            for (int i = 1; i < 7; ++i)
            {
                SinValue += Pow(*this, 2 * i)*(i % 2 == 0 ? 1 : -1)  / VariableConversionFunctions::Fact(2 * i + 1);
                CosValue += Pow(*this, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return SinValue / CosValue;
        }

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        auto ATanOf()
        {
            auto SinValue = One  / VariableConversionFunctions::Fact(1);
            auto CosValue = One / VariableConversionFunctions::Fact(0);
            //Angle as Radian
            for (int i = 1; i < 7; ++i)
            { // That's Taylor series!!
                SinValue += Pow(*this, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i + 1);
                CosValue += Pow(*this, 2 * i)*(i % 2 == 0 ? 1 : -1) / VariableConversionFunctions::Fact(2 * i);
            }
            return CosValue / SinValue;
        }

        /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecBase</returns>
        static auto Sin(const auto& Value){ return SinOf(Value); }
            

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        /// <returns>MediumDecBase</returns>
        static auto ArcTan2(const auto& y, const auto& x)
        {
            auto coeff_1 = PiNum / 4;
            auto coeff_2 = coeff_1 * 3;
            auto abs_y = Abs(y) + JustAboveZero;// kludge to prevent 0/0 condition
            auto r;
            auto angle;
            if (x.IsPositive())
            {
                r = (x - abs_y) / (x + abs_y);
                angle = coeff_1 - coeff_1 * r;
            }
            else
            {
                r = (x + abs_y) / (abs_y - x);
                angle = coeff_2 - coeff_1 * r;
            }
            if (y.IsNegative())
                return -angle;// negate if in quad III or IV
            else
                return angle;
        }

        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        static auto SinFromAngle(auto Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntHalf.Value == 0)
                {
                    Value.IntHalf = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntHalf.Value %= 360;
                    Value.IntHalf.Value = 360 - Value.IntHalf;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntHalf.Value %= 360;
            }
            if(Value.DecimalHalf==0)
            {
                switch(Value.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return Zero;
                        break;
                    case 90://0.5 Pi Radians
                        return One;
                        break;
                    case 270://1.5 Pi Radians
                        return NegativeOne;
                        break;
                    case 30://0.1666666666 Pi Radians
                    case 150://0.833333333 Pi Radians
                        return PointFive;
                    case 210:
                    case 330:
                        return NegPointFive;
                    default:
                        //Angle as Radian
                        auto Radius = PiNum * Value / 180;
                        return Sin(Radius);
                        break;
                }
            }
            else
            {
                //Angle as Radian
                auto Radius = PiNum * Value / 180;
                return Sin(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns></returns>
        static auto CosFromAngle(auto Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntHalf.Value == 0)
                {
                    Value.IntHalf = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntHalf.Value %= 360;
                    Value.IntHalf.Value = 360 - Value.IntHalf;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntHalf.Value %= 360;
            }
            if(Value.DecimalHalf==0)
            {
                switch(Value.IntHalf.Value)
                {
                    case 0:
                        return One;
                        break;
                    case 60:
                        return PointFive;
                        break;
                    case 90://0.5 Pi Radians
                    case 270://1.5 Pi Radians
                        return Zero;
                        break;
                    case 180://Pi Radians
                        return NegativeOne;
                        break;
                    case 120:
                    case 240:
                        return NegPointFive;
                    default:
                        //Angle as Radian
                        auto Radius = Pi * Value / 180;
                        return Cos(Radius)
                        break;
                }
            }
            else
            {
                //Angle as Radian
                auto Radius = PiNum * Value / 180;
                return Cos(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
        static auto TanFromAngle(auto Value)
        {
            if (Value.IsNegative())
            {
                if (Value.IntHalf.Value == 0)
                {
                    Value.IntHalf = 359; Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf;
                }
                else
                {
                    Value.SwapNegativeStatus();
                    Value.IntHalf.Value %= 360;
                    Value.IntHalf.Value = 360 - Value.IntHalf;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
            }
            else
            {
                Value.IntHalf.Value %= 360;
            }
            if(Value.DecimalHalf==0)
            {
                switch(Value.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return Zero;
                        break;
                    case 90://0.5 Pi Radians
                        return Maximum;//Positive Infinity
                        break;
                    case 270://1.5 Pi Radians
                        return Minimum;//Negative Infinity
                        break;
                    default:
                        return Tan(PiNum * Value / 180);
                        break;
                }
            }
            else
                return Tan(PiNum * Value / 180);
        }
*/

    #pragma endregion Trigonomic Functions
    };
	
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproaching)
    MediumDecBase MediumDecBase::AlmostOne = ApproachingRightRealValue();
#endif
    MediumDecBase MediumDecBase::Pi = PiNumValue();
    MediumDecBase MediumDecBase::One = OneValue();
    MediumDecBase MediumDecBase::Two = TwoValue();
    MediumDecBase MediumDecBase::NegativeOne = NegativeOneValue();
    MediumDecBase MediumDecBase::Zero = ZeroValue();
    MediumDecBase MediumDecBase::PointFive = Point5Value();
    MediumDecBase MediumDecBase::JustAboveZero = JustAboveZeroValue();
    MediumDecBase MediumDecBase::OneMillionth = OneMillionthValue();
    MediumDecBase MediumDecBase::FiveThousandth = FiveThousandthValue();
    MediumDecBase MediumDecBase::Minimum = MinimumValue();
    MediumDecBase MediumDecBase::Maximum = MaximumValue();
    MediumDecBase MediumDecBase::E = ENumValue();
    MediumDecBase MediumDecBase::LN10 = LN10Value();
    MediumDecBase MediumDecBase::LN10Mult = LN10MultValue();
    MediumDecBase MediumDecBase::HalfLN10Mult = HalfLN10MultValue();
    MediumDecBase MediumDecBase::TenMillionth = TenMillionthValue();
    MediumDecBase MediumDecBase::FiveMillionth = FiveMillionthValue();
    MediumDecBase MediumDecBase::FiveBillionth = FiveBillionthValue();
    MediumDecBase MediumDecBase::OneGMillionth = OneHundredMillionthValue();
	
    MediumDecBase MediumDecBase::PiNum = PiNumValue();
    MediumDecBase MediumDecBase::ENum = ENumValue();
    #pragma endregion ValueDefine Source
}