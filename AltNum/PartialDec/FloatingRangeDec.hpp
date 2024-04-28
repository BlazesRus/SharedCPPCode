// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "AltNumBase.h"

#include <string>
#include <cmath>
#include "..\OtherFunctions\VariableConversionFunctions.h"

#include <boost/rational.hpp>//Requires boost to reduce fractional(for Pow operations etc)

#include <type_traits>
#include <cstddef>
#include <concepts>//C++20 feature
#include <compare>//used for C++20 feature of spaceship operator
#include "..\IntegerConcept.hpp"
#include "..\MediumDecVariantConcept.hpp"

/*
AltNum_PreventModulusOverride
AltNum_EnableAlternativeModulusResult
*/
#include "MediumDecPreprocessors.h"
#include "RepType.h"

namespace BlazesRusCode
{
	using UInt128 = boost::multiprecision::uint128_t;
	using UInt64 = unsigned __int64;
    class FloatingRangeDec;

    /// <summary>
    /// Represents range of numbers from only 0 to 3.999999999
	/// </summary>
    class DLL_API FloatingRangeDec : PartialInt
    {
	private:
        static unsigned _int64 const UInt64Max = 18446744073709551615;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static unsigned int const DecimalOverflow = 1000000000;
		
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static UInt128 const DecimalOverflowX = 1000000000;
		
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

		//Return IntHalf as unsigned int
        unsigned unsigned int GetIntSection()
        {
			return Flags;
        }
		
		//Return DecHalf as unsigned int
        unsigned unsigned int GetDecSection()
        {
			return Value;
        }

        bool IsNegative()
        {
			return false;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FloatingRangeDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        FloatingRangeDec(const PartialInt& rValue)
        {
			Value = rValue.Value;
			Flags = rValue.Flags;
        }

        FloatingRangeDec(const FloatingRangeDec&) = default;

        FloatingRangeDec& operator=(const unsigned _int64& rhs)
        {
			IntValue = rhs;
			DecimalHalf = 0;
            ExtraRep = 0;
            return *this;
        } const

        FloatingRangeDec& operator=(const FloatingRangeDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
			Value = rValue.Value;
			Flags = rValue.Flags;
            return *this;
        } const

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt()
        {
            return IntValue==0;
        }

        bool IsNotAtZeroInt()
        {
            return IntValue!=0;
        }

        //Detect if at exactly zero
		bool IsZero()
		{
            return DecimalHalf==0&&IntValue==0;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(const PartialInt& Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
            ExtraRep = Value.ExtraRep;
        }

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0; ExtraRep = 0;
        }
        
        /// <summary>
        /// Swaps the negative status.(Does nothing)
        /// </summary>
        void SwapNegativeStatus(){}

    #pragma region Const Representation values
    protected:

    #pragma endregion Const Representation values

    #pragma region RepType

    #pragma endregion RepType

public:
    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        void SetAsMaximum()
        {
            IntValue = 18446744073709551615;
			DecimalHalf = 999999999;
			ExtraRep = 0;
        }
	
        /// <summary>
        /// Sets value to the lowest value that can be stored(zero)
        /// </summary>
        constexpr auto SetAsMinimum = SetAsZero;

    #pragma endregion RangeLimits

    #pragma region ValueDefines
        
        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec PiValue()
        {
            return FloatingRangeDec(3, 141592654);
        }

        //100,000,000xPi(Rounded to 9th decimal digit)
        static FloatingRangeDec HundredMilPiNumVal()
        {
            return FloatingRangeDec(314159265, 358979324);
        }

        //10,000,000xPi(Rounded to 9th decimal digit)
        static FloatingRangeDec TenMilPiNumVal()
        {
            return FloatingRangeDec(31415926, 535897932);
        }

        //1,000,000xPi(Rounded to 9th decimal digit)
        static FloatingRangeDec OneMilPiNumVal()
        {
            return FloatingRangeDec(3141592, 653589793);
        }

        //10xPi(Rounded to 9th decimal digit)
        static FloatingRangeDec TenPiNumVal()
        {
            return FloatingRangeDec(31, 415926536);
        }
        
        static FloatingRangeDec ENumValue()
        {
            return FloatingRangeDec(2, 718281828);
        }

        static FloatingRangeDec EValue()
        {
            return FloatingRangeDec(2, 718281828);
        }
        
        static FloatingRangeDec ZeroValue()
        {
            return FloatingRangeDec();
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec OneValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(1);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec TwoValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(2);
            return NewSelf;
        }

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec Point5Value()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(0, 500000000);
            return NewSelf;
        }

        static FloatingRangeDec JustAboveZeroValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(0, 1);
            return NewSelf;
        }

        static FloatingRangeDec OneMillionthValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(0, 1000);
            return NewSelf;
        }

        static FloatingRangeDec FiveThousandthValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(0, 5000000);
            return NewSelf;
        }

        static FloatingRangeDec FiveMillionthValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(0, 5000);
            return NewSelf;
        }

        static FloatingRangeDec TenMillionthValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(0, 100);
            return NewSelf;
        }

        static FloatingRangeDec OneHundredMillionthValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(0, 10);
            return NewSelf;
        }

        static FloatingRangeDec FiveBillionthValue()
        {
            FloatingRangeDec NewSelf = FloatingRangeDec(0, 5);
            return NewSelf;
        }

        static FloatingRangeDec LN10Value()
        {
            return FloatingRangeDec(2, 302585093);
        }

        static FloatingRangeDec LN10MultValue()
        {
            return FloatingRangeDec(0, 434294482);
        }

        static FloatingRangeDec HalfLN10MultValue()
        {
            return FloatingRangeDec(0, 868588964);
        }

        static FloatingRangeDec MaximumValue()
        {
            return FloatingRangeDec(2147483647, 999999999);
        }
public:
        static FloatingRangeDec AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec PiNum;
        
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec ENum;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec Pi;
      
        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec E;
        
        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec Zero;
        
        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        static FloatingRangeDec OneGMillionth;

        //0e-7
        static FloatingRangeDec TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static FloatingRangeDec FiveMillionth;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static FloatingRangeDec Minimum;
        
        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static FloatingRangeDec Maximum;
        
        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static FloatingRangeDec LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static FloatingRangeDec LN10Mult;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static FloatingRangeDec HalfLN10Mult;
    #pragma endregion ValueDefines

    #pragma region String Commands
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void ReadString(const std::string& Value);

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        template<MediumDecVariant VariantType=FloatingRangeDec>
        VariantType GetValueFromString(std::string Value)
        {

        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FloatingRangeDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        FloatingRangeDec(const char* strVal)
        {
            std::string Value = strVal;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FloatingRangeDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FloatingRangeDec(const std::string& Value)
        {
            this->ReadString(Value);
        }

//private:
        //std::string BasicToStringOp();
		
		//std::string BasicToFullStringOp();
public:

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>std.string</returns>
        virtual std::string ToString();

        /// <summary>
        /// Converts to string with digits filled in even when empty
        /// </summary>
        /// <returns>std.string</returns>
        virtual std::string ToFullString();

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToString(); }
    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes
		//Using just Legaacy floating conversion for now since not important for this variant
	
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetFloatVal(const float& Value)
        {
            if (Value < 0.0f) { SetAsZero(); }
			else if (Value >= 18446744073709551616f){//Cap value if too big on initialize (preventing overflow on conversion)
                IntValue = UInt64Max;
                DecimalHalf = 999999999;
            }
            else
            {
				float lValue = Value;
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (float)WholeValue;
                DecimalHalf = (unsigned int)Value * 10000000000;
                IntValue = (unsigned int)WholeValue;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDoubleVal(const double& Value)
        {
            if (Value < 0.0) { SetAsZero(); }
			else if (Value >= 18446744073709551616){//Cap value if too big on initialize (preventing overflow on conversion)
                IntValue = UInt64Max;
                DecimalHalf = 999999999;
            }
            else
            {
				auto lValue = Value;
                UInt64 WholeValue = (UInt64)std::floor(Value);
                lValue -= (double)WholeValue;
                DecimalHalf = (unsigned int)Value * 10000000000;
                IntValue = (unsigned int)WholeValue;
            }
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetDecimalVal(const ldouble& Value)
        {
            if (Value < 0.0L) { SetAsZero(); }
			else if (Value >= 18446744073709551616L){//Cap value if too big on initialize (preventing overflow on conversion)
                IntValue = UInt64Max;
                DecimalHalf = 999999999;
            }
            else
            {
				auto lValue = Value;
                signed __int64 WholeValue = (UInt64)std::floor(lValue);
                lValue -= (ldouble)WholeValue;
                DecimalHalf = (unsigned int)lValue * 10000000000;
                IntValue = (unsigned int)WholeValue;
            }
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetBoolVal(const bool& Value)
        {
            IntValue = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetIntVal(UInt64& Value)
        {
			IntValue = Value;
			DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FloatingRangeDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FloatingRangeDec(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FloatingRangeDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FloatingRangeDec(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FloatingRangeDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FloatingRangeDec(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FloatingRangeDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        FloatingRangeDec(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

        FloatingRangeDec(const FloatingRangeDec& Value)
        {
            this->SetVal(Value);
        }
    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes
public:
        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        void float toFloat()
        {
            auto lValue = *this;
            if(ExtraRep!=0)
                lValue /= ExtraRep;
		    float Value = lValue.IntValue;
            if (DecimalHalf != 0) 
               Value += ((float)lValue.DecimalHalf * 0.000000001f);
            return Value;
        }

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        void double toDouble()
        {
            auto lValue = *this;
            if(ExtraRep!=0)
                lValue /= ExtraRep;
		    double Value = lValue.IntValue;
            if (DecimalHalf != 0) 
               Value += ((double)lValue.DecimalHalf * 0.000000001);
            return Value;
        }

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        void ldouble toDecimal()
        {
            auto lValue = *this;
            if(ExtraRep!=0)
                lValue /= ExtraRep;
		    ldouble Value = lValue.IntValue;
            if (DecimalHalf != 0) 
               Value += ((ldouble)lValue.DecimalHalf * 0.000000001L);
            return Value;
        }
		
        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        unsigned _int64 toInt() {
            auto lValue = *this;
            if(ExtraRep!=0)
                lValue /= ExtraRep; 
            return lValue.IntValue; }

        bool toBool() { return IntValue.IsZero() ? false : true; }

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloatV1(); }
		
        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDoubleV1(); }
		
        /// <summary>
        /// MediumDec Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble() { return toDecimalV1(); }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes
	
    #pragma region Comparison Operators
protected:
		
		//Compare only as if in NormalType representation mode
		std::strong_ordering BasicIntComparison(const int& that) const
		{
			if (auto IntHalfCmp = IntValue <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			if (auto DecimalHalfCmp = DecimalHalf <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}

		//Compare only as if in NormalType representation mode
		std::strong_ordering BasicComparison(const FloatingRangeDec& that) const
		{
			if (auto IntHalfCmp = IntValue <=> that.IntValue; IntHalfCmp != 0)
				return IntHalfCmp;
			if (auto DecimalHalfCmp = DecimalHalf <=> that.DecimalHalf; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
	
public:

		std::strong_ordering operator<=>(const FloatingRangeDec& that) const
		{
            if(ExtraRep!=0)
            {
                FloatingRangeDec lValue = *this;
                lValue.BasicIntDivOp(ExtraRep);
                if(that.ExtraRep!=0){
                    FloatingRangeDec rValue = that;
                    rValue.BasicIntDivOp(that.ExtraRep);
			        return lValue.BasicComparison(rValue);
                }
                else
			        return lValue.BasicComparison(that);
            }
            else if(that.ExtraRep!=0)
            {
                FloatingRangeDec rValue = that;
                rValue.BasicIntDivOp(that.ExtraRep);
                return BasicComparison(rValue);
            }
            else
                return BasicComparison(that);
		}

		std::strong_ordering operator<=>(const int& that) const
		{
            if(ExtraRep!=0)
            {
                FloatingRangeDec lValue = *this;
                lValue.BasicIntDivOp(ExtraRep);
			    return lValue.BasicIntComparison(that);
            }
            else
			    return BasicIntComparison(that);
		}

		bool operator==(const int& that) const
		{
			if (IntValue!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			return true;
		}

		bool operator==(const FloatingRangeDec& that) const
		{
			if (IntValue!=that.IntValue)
				return false;
			if (DecimalHalf!=that.IntValue)
				return false;
		}
    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:
        template<IntegerType IntType=unsigned _int64>
        void PartialUIntDivOp(const IntType& rValue)
        {//Avoid using with special status representations such as approaching zero or result will be incorrect
            UInt128 SelfRes;
            UInt128 Res;
            UInt128 IntHalfRes;
            UInt128 DecimalRes;
            
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntValue: DecimalOverflowX * IntValue + DecimalHalf;
            Res = SelfRes / rValue;
            
            IntHalfRes = Res/DecimalOverflowX;
            DecimalRes = (unsigned _int64)(Res - DecimalOverflowX * IntHalfRes);
		    IntValue = (unsigned _int64)IntHalfRes;
			DecimalHalf = DecimalRes;
        }

protected:
        FloatingRangeDec& BasicUIntDivOp(IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialUIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
            return *this;
        }
		
public:
        void DivideByTwo()
        {
            if(ExtraRep==0)
                ExtraRep = 2;
            else if(ExtraRep<=2147483648)
                ExtraRep *= 2;
            else
            {
                rValue.BasicIntDivOp(65536);//Divided by 2^16
                ExtraRep /= 32768;//Divided by 2^16, and then multiplied by 2
            }

        }

		void BasicIntDivOp(unsigned _int64& Value) { BasicUnsignedIntDivOp(Value); }
        void BasicInt64DivOp(unsigned long long& Value) { BasicUnsignedIntDivOp(Value); }

    #pragma endregion NormalRep Integer Division Operations
		
    #pragma region NormalRep Integer Multiplication Operations
protected:
        template<IntegerType IntType=unsigned int>
        void PartialUIntMultOpV1(const IntType& Value)
            if (DecimalHalf == 0)
                IntValue *= rValue;
            else
			{
                __int64 SRep = IntValue == 0 ? DecimalHalf : DecimalOverflowX * IntValue.Value + DecimalHalf;
                SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntValue = (unsigned int)OverflowVal;
                    DecimalHalf = (unsigned int)SRep;
                }
                else
                {
					IntValue = 0;
                    DecimalHalf = (unsigned int)SRep;
                }
            }
        }

public:
		
        constexpr auto PartialUIntMultOp = PartialUIntMultOpV1<const unsigned int>;
        constexpr auto PartialIntMultOp = PartialUIntMultOpV1<const signed int>;
        constexpr auto PartialUInt64MultOp = PartialUIntMultOpV1<const unsigned long long>;
        constexpr auto PartialInt64MultOp = PartialUIntMultOpV1<const signed long long>;
		
protected:
        template<MediumDecVariant VariantType=FloatingRangeDec, IntegerType IntType=signed int>
        VariantType& BasicUIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return *this;
            }
            else if (IsZero())
                return *this;
            PartialUIntMultOp(Value);
            return *this;
        }
		
public:

        constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<const unsigned int>;
        constexpr auto BasicIntMultOp = BasicUIntMultOpV1<const signed int>;
        constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<const unsigned long long>;
        constexpr auto BasicInt64MultOp = BasicUIntMultOpV1<const signed long long>;

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep Integer Addition Operations
protected:

public:

        /// <summary>
        /// Basic Addition Operation between FloatingRangeDec and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        auto& BasicUIntAddOp(const IntType& rValue)
        {
            IntValue += rValue;
            return *this;
        }

        constexpr auto BasicUIntAddOp = BasicUIntAddOpV1<const unsigned int>;
        constexpr auto BasicIntAddOp = BasicUIntAddOpV1<const signed int>;
        constexpr auto BasicUInt64AddOp = BasicUIntAddOpV1<const unsigned long long>;
        constexpr auto BasicInt64AddOp = BasicUIntAddOpV1<const signed long long>;

	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:
	
public:

		/// <summary>
        /// Basic Subtraction Operation between FloatingRangeDec and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType=unsigned int>
        auto BasicUIntSubOp(const IntType& rValue)
        {
            IntValue -= rValue;
            return *this;
        }

        constexpr auto BasicUIntAddOp = BasicUIntSubOpV1<const unsigned int>;
        constexpr auto BasicIntAddOp = BasicUIntSubOpV1<const signed int>;
        constexpr auto BasicUInt64AddOp = BasicUIntSubOpV1<const unsigned long long>;
        constexpr auto BasicInt64AddOp = BasicUIntSubOpV1<const signed long long>;

	#pragma endregion NormalRep Integer Subtraction Operations

	#pragma region NormalRep AltNum Division Operations
protected:
	
public:
/*
        constexpr auto BasicDivOp = BasicDivOpV1<FloatingRangeDec>;
*/
	#pragma endregion NormalRep AltNum Division Operations

	#pragma region NormalRep AltNum Multiplication Operations
protected:

public:

		/// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned FloatingRangeDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        FloatingRangeDec& BasicMultOp(const FloatingRangeDec& rValue)
		{
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                {
                    IntValue *= rValue.IntValue;
                }
                else
                {
                    UInt128 rRep = rValue.IntValue == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
                    if (rRep >= DecimalOverflowX)
                    {
                        UInt128 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (UInt64)OverflowVal;
                        DecimalHalf = (unsigned int)rRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (unsigned int)rRep;
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
#endif
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
            else if (IntValue == 0)
            {
                UInt128 SRep = DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= FloatingRangeDec::DecimalOverflowX;
                if (rValue.IntValue == 0)
                {
                    DecimalHalf = (unsigned int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf = 1;
                #endif
                    return *this;
                }
                else
                {
                    SRep += DecimalHalf * rValue.IntValue;
                    if (SRep >= FloatingRangeDec::DecimalOverflowX)
                    {
                        UInt128 OverflowVal = SRep / FloatingRangeDec::DecimalOverflowX;
                        SRep -= OverflowVal * FloatingRangeDec::DecimalOverflowX;
                        IntValue = (unsigned _int64) OverflowVal;
                        DecimalHalf = (unsigned int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (unsigned int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
                #endif
                        return *this;
                    }
                }
            }
            else
            {
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    UInt128 SRep = FloatingRangeDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;
                    if (SRep >= FloatingRangeDec::DecimalOverflowX)
                    {
                        UInt128 OverflowVal = SRep / FloatingRangeDec::DecimalOverflowX;
                        SRep -= OverflowVal * FloatingRangeDec::DecimalOverflowX;
                        IntValue = (unsigned __int64)OverflowVal;
                        DecimalHalf = (unsigned int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (unsigned int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf = 1;
                #endif
                        }
                        IntValue = SelfIsNegative ? FloatingRangeDec::NegativeRep : 0;
                    }
				    return *this;
                }
                else if (rValue.IntValue == 0)
                {
                    UInt128 SRep = FloatingRangeDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= FloatingRangeDec::DecimalOverflowX;
                    if (SRep >= FloatingRangeDec::DecimalOverflowX)
                    {
                        UInt128 OverflowVal = SRep / FloatingRangeDec::DecimalOverflowX;
                        SRep -= OverflowVal * FloatingRangeDec::DecimalOverflowX;
                        IntValue = (UInt64)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (unsigned int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (unsigned int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf = 1;
                #endif
                        }
                        IntValue = SelfIsNegative ? FloatingRangeDec::NegativeRep : 0;
                    }
                    return *this;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    UInt128 SRep = IntValue == 0 ? DecimalHalf : FloatingRangeDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    UInt128 Temp03 = (__int64)(rValue.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    UInt128 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= FloatingRangeDec::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    UInt128 IntegerRep = SRep + Temp03 + Temp04;
                    UInt128 IntHalf = IntegerRep / FloatingRangeDec::DecimalOverflow;
                    IntegerRep -= FloatingRangeDec::DecimalOverflowX * IntHalf;
                    IntValue = (UInt64)IntHalf;
                    DecimalHalf = (unsigned int)IntegerRep;
                }
            }
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
#endif
            return *this;
		}

	#pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep AltNum Addition Operations
protected:
	
public:
        /// <summary>
        /// Basic Addition Operation
        /// </summary>
        /// <param name="Value">The value.</param>
        void BasicAddOp(const FloatingRangeDec& Value)
        {
            //Deal with Int section first
            IntValue += Value.IntValue;
            if (Value.DecimalHalf != 0)
            {
				DecimalHalf += Value.DecimalHalf;
				if (DecimalHalf >= MediumDecBase::DecimalOverflow) {
					DecimalHalf -= MediumDecBase::DecimalOverflow; ++IntValue; }
            }
        }
		
	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations
protected:
	
public:
/*
        constexpr auto BasicSubOp = BasicSubOpV1<FloatingRangeDec>;
*/
	#pragma endregion NormalRep AltNum Subtraction Operations

	#pragma region Mixed Fraction Operations
	//Used only in AltDec/MixedDec
	#pragma endregion Mixed Fraction Operations

	#pragma region Other Division Operations

	#pragma endregion Other Division Operations	

	#pragma region Other Multiplication Operations

	#pragma endregion Other Multiplication Operations

	#pragma region Other Addition Operations

	#pragma endregion Other Addition Operations

	#pragma region Other Subtraction Operations

	#pragma endregion Other Subtraction Operations

	#pragma region Modulus Operations
	//Not supported at moment
	#pragma endregion Modulus Operations

	#pragma region Bitwise Operations
	//Not supported at moment
	#pragma endregion Bitwise Operations

    #pragma region Main Operator Overrides
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator/(FloatingRangeDec self, FloatingRangeDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec& operator/=(FloatingRangeDec& self, FloatingRangeDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator*(FloatingRangeDec self, FloatingRangeDec Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec& operator*=(FloatingRangeDec& self, FloatingRangeDec Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator+(FloatingRangeDec self, FloatingRangeDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec& operator+=(FloatingRangeDec& self, FloatingRangeDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator-(FloatingRangeDec self, FloatingRangeDec Value) { return SubOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec& operator-=(FloatingRangeDec& self, FloatingRangeDec Value) { return SubOp(self, Value); }

        /// <summary>
        /// Addition Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator+(FloatingRangeDec self, int Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between FloatingRangeDec and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>FloatingRangeDec</returns>
        template<IntegerType IntType=signed int>
        friend FloatingRangeDec& operator+=(FloatingRangeDec& self, int Value) { return IntAddOp(self, Value); }

        //friend FloatingRangeDec operator+=(FloatingRangeDec* self, int Value) { return IntAddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator-(FloatingRangeDec self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec& operator-=(FloatingRangeDec& self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator*(FloatingRangeDec self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        template<IntegerType IntType=signed int>
        friend FloatingRangeDec& operator*=(FloatingRangeDec& self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator/(FloatingRangeDec self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec& operator/=(FloatingRangeDec& self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// Addition Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator+(FloatingRangeDec self, signed long long Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between FloatingRangeDec and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>FloatingRangeDec</returns>
        template<IntegerType IntType=signed int>
        friend FloatingRangeDec& operator+=(FloatingRangeDec& self, signed long long Value) { return IntAddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator-(FloatingRangeDec self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec& operator-=(FloatingRangeDec& self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator*(FloatingRangeDec self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        template<IntegerType IntType=signed int>
        friend FloatingRangeDec operator*=(FloatingRangeDec& self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator/(FloatingRangeDec self, signed long long Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between FloatingRangeDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec operator/=(FloatingRangeDec& self, signed long long Value) { return IntDivOp(self, Value); }
    
        friend FloatingRangeDec operator+(FloatingRangeDec self, float Value) { return self + (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator-(FloatingRangeDec self, float Value) { return self - (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator*(FloatingRangeDec self, float Value) { return self * (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator/(FloatingRangeDec self, float Value) { return self / (FloatingRangeDec)Value; }

        friend FloatingRangeDec operator+(float Value, FloatingRangeDec self) { return (FloatingRangeDec)Value + self; }
        friend FloatingRangeDec operator-(float Value, FloatingRangeDec self) { return (FloatingRangeDec)Value - self; }
        friend FloatingRangeDec operator*(float Value, FloatingRangeDec self) { return (FloatingRangeDec)Value * self; }
        friend FloatingRangeDec operator/(float Value, FloatingRangeDec self) { return (FloatingRangeDec)Value / self; }

        friend FloatingRangeDec operator+(FloatingRangeDec self, double Value) { return self + (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator-(FloatingRangeDec self, double Value) { return self - (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator*(FloatingRangeDec self, double Value) { return self * (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator/(FloatingRangeDec self, double Value) { return self / (FloatingRangeDec)Value; }

        friend FloatingRangeDec operator+(FloatingRangeDec self, ldouble Value) { return self + (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator-(FloatingRangeDec self, ldouble Value) { return self - (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator*(FloatingRangeDec self, ldouble Value) { return self * (FloatingRangeDec)Value; }
        friend FloatingRangeDec operator/(FloatingRangeDec self, ldouble Value) { return self / (FloatingRangeDec)Value; }

        friend FloatingRangeDec operator+(ldouble Value, FloatingRangeDec self) { return (FloatingRangeDec)Value + self; }
        friend FloatingRangeDec operator-(ldouble Value, FloatingRangeDec self) { return (FloatingRangeDec)Value - self; }
        friend FloatingRangeDec operator*(ldouble Value, FloatingRangeDec self) { return (FloatingRangeDec)Value * self; }
        friend FloatingRangeDec operator/(ldouble Value, FloatingRangeDec self) { return (FloatingRangeDec)Value / self; }


        friend FloatingRangeDec operator+(FloatingRangeDec self, unsigned char Value) { return IntAddOp(self, Value); }
        friend FloatingRangeDec operator-(FloatingRangeDec self, unsigned char Value) { return IntSubOp(self, Value); }
        friend FloatingRangeDec operator*(FloatingRangeDec self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend FloatingRangeDec operator/(FloatingRangeDec self, unsigned char Value) { return self.UnsignedIntDivOp(Value); }

        friend FloatingRangeDec operator+=(FloatingRangeDec& self, unsigned char Value) { return IntAddOp(self, Value); }
        friend FloatingRangeDec operator-=(FloatingRangeDec& self, unsigned char Value) { return IntSubOp(self, Value); }
        friend FloatingRangeDec operator*=(FloatingRangeDec& self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend FloatingRangeDec operator/=(FloatingRangeDec& self, unsigned char Value) { return UnsignedDivOp(self, Value); }
        

        friend FloatingRangeDec operator+(FloatingRangeDec self, unsigned short Value) { return IntAddOp(self, Value); }
        friend FloatingRangeDec operator-(FloatingRangeDec self, unsigned short Value) { return IntSubOp(self, Value); }
        friend FloatingRangeDec operator*(FloatingRangeDec self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend FloatingRangeDec operator/(FloatingRangeDec self, unsigned short Value) { return UnsignedDivOp(self, Value); }

        friend FloatingRangeDec operator+=(FloatingRangeDec& self, unsigned short Value) { return IntAddOp(self, Value); }
        friend FloatingRangeDec operator-=(FloatingRangeDec& self, unsigned short Value) { return IntSubOp(self, Value); }
        friend FloatingRangeDec operator*=(FloatingRangeDec& self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend FloatingRangeDec operator/=(FloatingRangeDec& self, unsigned short Value) { return UnsignedDivOp(self, Value); } 

        friend FloatingRangeDec operator+(FloatingRangeDec self, unsigned _int64 Value) { return IntAddOp(self, Value); }
        friend FloatingRangeDec operator-(FloatingRangeDec self, unsigned _int64 Value) { return IntSubOp(self, Value); }
        friend FloatingRangeDec operator*(FloatingRangeDec self, unsigned _int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend FloatingRangeDec operator/(FloatingRangeDec self, unsigned _int64 Value) { return UnsignedDivOp(self, Value); }
        

        friend FloatingRangeDec operator+=(FloatingRangeDec& self, unsigned _int64 Value) { return IntAddOp(self, Value); }
        friend FloatingRangeDec operator-=(FloatingRangeDec& self, unsigned _int64 Value) { return IntSubOp(self, Value); }
        friend FloatingRangeDec operator*=(FloatingRangeDec& self, unsigned _int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend FloatingRangeDec operator/=(FloatingRangeDec& self, unsigned _int64 Value) { return UnsignedDivOp(self, Value); }
        
        friend FloatingRangeDec operator+(FloatingRangeDec self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend FloatingRangeDec operator-(FloatingRangeDec self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend FloatingRangeDec operator*(FloatingRangeDec self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend FloatingRangeDec operator/(FloatingRangeDec self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

        friend FloatingRangeDec operator+=(FloatingRangeDec& self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend FloatingRangeDec operator-=(FloatingRangeDec& self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend FloatingRangeDec operator*=(FloatingRangeDec& self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend FloatingRangeDec operator/=(FloatingRangeDec& self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma endregion Other Operators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>FloatingRangeDec</returns>
        friend FloatingRangeDec& operator-(FloatingRangeDec& self)
        {
            return self;//Negative numbers not supported
        }

        /// <summary>
        /// ++FloatingRangeDec Operator
        /// </summary>
        /// <returns>FloatingRangeDec &</returns>
        FloatingRangeDec& operator ++()
        {
            ++IntValue;
            return *this;
        }

        /// <summary>
        /// ++FloatingRangeDec Operator
        /// </summary>
        /// <returns>FloatingRangeDec &</returns>
        FloatingRangeDec& operator --()
        {
			if(IntValue==0)
				throw "Can't reduce number into negative values";
            else
                --IntValue;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        FloatingRangeDec operator ++(int)
        {
            FloatingRangeDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>FloatingRangeDec</returns>
        FloatingRangeDec operator --(int)
        {
            FloatingRangeDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>FloatingRangeDec &</returns>
        FloatingRangeDec& operator *()
        {
            return *this;
        }
    #pragma endregion Other Operators

	#pragma region Math Etc Functions
	//Not Supported at moment
	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions
	//Not Supported at moment
	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:

public:
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modifier owner object
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>FloatingRangeDec</returns>
        auto ExpOf()
        {
            //x.ConvertToNormType();//Prevent losing imaginary number status
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
             *      - approximation of e^x in FloatingRangeDec precision
             */
             // Check that x is a valid input.
            assert(IntValue < 709);
            // When x = 0 we already know e^x = 1.
            if (IsZero()) {
                return One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            VariantType x0 = *this;
            x0.Abs();
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            VariantType tn = One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = VariantType::CeilInt(x0 * E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = One / tn;
            }
            return tn;
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        auto NthRootOfV2(int n, const auto& Precision = FiveBillionth)
        {
            int nMinus1 = n - 1;
            auto x[2] = { (One / n) * ((*this*nMinus1) + (*this / PowOf(nMinus1))), targetValue };
            while (Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (One / n) * ((x[1]*nMinus1) + (targetValue / x[1].PowOf(nMinus1)));
            }
            return x[0];
        }
	#pragma endregion Log Functions
    };

    #pragma region ValueDefine Source

    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void MediumDec::ReadString(std::string Value)
    {
        IntValue = 0; DecimalHalf = 0;
	#if !defined(AltNum_EnableMirroredSection)
        bool IsNegative = false;
	#endif
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
            {
	#if !defined(AltNum_EnableMirroredSection)
                IsNegative = true;
	#else
				IntValue.IsPositive = 0;
	#endif
            }
            else if (StringChar == '.')
            {
                ReadingDecimal = true;
            }
            else if(StringChar!=' ')
                break;//Stop Extracting after encounter non-number character such as i
        }
        PlaceNumber = WholeNumberBuffer.length() - 1;
        for (char const& StringChar : WholeNumberBuffer)
        {
            TempInt = VariableConversionFunctions::CharAsInt(StringChar);
            TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
            if (StringChar != '0')
            {
                IntValue += TempInt02;
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
	#if !defined(AltNum_EnableMirroredSection)
        if (IsNegative)
        {
            if (IntValue == 0) { IntValue = NegativeRep; }
            else { IntValue *= -1; }
        }
	#endif
    }

    /// <summary>
    /// Gets the value from string.
    /// </summary>
    /// <param name="Value">The value.</param>
    /// <returns>MediumDec</returns>
    inline MediumDec MediumDec::GetValueFromString(const std::string& Value)
    {
        MediumDec NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

    std::string MediumDec::ToString()
    {
        std::string Value = "";
        int CurrentSection = IntValue;
        unsigned __int8 CurrentDigit;
        std::string DecBuffer = "";
        if (IntValue < 0)
        {
            Value += "-";
            if (IntValue == NegativeRep) { CurrentSection = 0; }
            else { CurrentSection *= -1; }
        }
        for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
        {
            CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
            CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        if (DecimalHalf != 0)
        {
            Value += ".";
            CurrentSection = DecimalHalf;
            for (__int8 Index = 8; Index >= 0; --Index)
            {
                CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
                CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
                if (CurrentDigit != 0)
                {
                    if(!DecBuffer.empty())
                    {
                        Value += DecBuffer;
                        DecBuffer.clear();
                    }
                    Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
                }
                else
                {
                    DecBuffer += VariableConversionFunctions::DigitAsChar(CurrentDigit);
                }
            }
        }
        return Value;
    }

    std::string MediumDec::ToFullString()
    {
        std::string Value = "";
        int CurrentSection = IntValue;
        unsigned __int8 CurrentDigit;
        if (IntValue < 0)
        {
            Value += "-";
            if (IntValue == NegativeRep) { CurrentSection = 0; }
            else { CurrentSection *= -1; }
        }
        for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
        {
            CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
            CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        if (DecimalHalf != 0)
        {
            Value += ".";
            bool HasDigitsUsed = false;
            CurrentSection = DecimalHalf;
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
    #pragma endregion String Function Source
}
}